#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <dirent.h>

// Include the API header for this library
#include "libcodeimage.h"

// Define STB_IMAGE_WRITE_IMPLEMENTATION and STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

// --- GLOBAL CONSTANT ---
#define CHANNELS 3 // Define CHANNELS as a global constant for RGB images

// --- Data Structures ---

// Structure to hold information about a discovered font
typedef struct {
    char *name;
    char *path;
} FontInfo;

// Global list of discovered fonts (managed internally by the library)
static FontInfo *discovered_fonts = NULL; // static: local to this compilation unit
static int discovered_fonts_count = 0;
static int discovered_fonts_capacity = 0;

// --- Helper Functions ---

static void hex_to_rgb(const char* hex_color, uint8_t* r, uint8_t* g, uint8_t* b) {
    if (!hex_color || strlen(hex_color) != 7 || hex_color[0] != '#') {
        *r = *g = *b = 0; // Default to black on error
        return;
    }
    sscanf(hex_color + 1, "%2hhx%2hhx%2hhx", r, g, b);
}

static void draw_char_bitmap(uint8_t* img_pixels, int img_width, int img_height,
                      uint8_t* char_pixels, int char_width, int char_height,
                      int draw_x, int draw_y,
                      uint8_t r, uint8_t g, uint8_t b) {
    for (int cy = 0; cy < char_height; ++cy) {
        for (int cx = 0; cx < char_width; ++cx) {
            int img_px = draw_x + cx;
            int img_py = draw_y + cy;

            // Check boundaries
            if (img_px >= 0 && img_px < img_width && img_py >= 0 && img_py < img_height) {
                uint8_t alpha = char_pixels[cy * char_width + cx];
                int img_idx = (img_py * img_width + img_px) * CHANNELS;

                float alpha_norm = alpha / 255.0f;

                img_pixels[img_idx + 0] = (uint8_t)(alpha_norm * r + (1.0f - alpha_norm) * img_pixels[img_idx + 0]);
                img_pixels[img_idx + 1] = (uint8_t)(alpha_norm * g + (1.0f - alpha_norm) * img_pixels[img_idx + 1]);
                img_pixels[img_idx + 2] = (uint8_t)(alpha_norm * b + (1.0f - alpha_norm) * img_pixels[img_idx + 2]);
            }
        }
    }
}

static int draw_text(uint8_t* img_pixels, int img_width, int img_height,
               int start_x, int start_y, const char* text,
               stbtt_fontinfo* font, float scale, uint8_t r, uint8_t g, uint8_t b) {

    int x_cursor = start_x;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    int baseline = (int)(ascent * scale);

    for (int i = 0; text[i]; ++i) {
        int codepoint = (unsigned char)text[i];

        int char_width, char_height, x_offset, y_offset;
        uint8_t* char_bitmap = stbtt_GetCodepointBitmap(font, 0, scale, codepoint, &char_width, &char_height, &x_offset, &y_offset);

        if (char_bitmap) {
            int draw_x = x_cursor + x_offset;
            int draw_y = start_y + baseline + y_offset;

            draw_char_bitmap(img_pixels, img_width, img_height,
                             char_bitmap, char_width, char_height,
                             draw_x, draw_y, r, g, b);

            free(char_bitmap);
        }

        int advance_width;
        stbtt_GetCodepointHMetrics(font, codepoint, &advance_width, NULL);
        x_cursor += (int)(advance_width * scale);
    }
    return x_cursor;
}

static void add_font(const char* name, const char* path) {
    if (discovered_fonts_count >= discovered_fonts_capacity) {
        discovered_fonts_capacity = (discovered_fonts_capacity == 0) ? 4 : discovered_fonts_capacity * 2;
        discovered_fonts = realloc(discovered_fonts, sizeof(FontInfo) * discovered_fonts_capacity);
        if (!discovered_fonts) {
            fprintf(stderr, "Memory allocation failed for fonts list!\n");
            exit(EXIT_FAILURE);
        }
    }
    discovered_fonts[discovered_fonts_count].name = strdup(name);
    discovered_fonts[discovered_fonts_count].path = strdup(path);
    if (!discovered_fonts[discovered_fonts_count].name || !discovered_fonts[discovered_fonts_count].path) {
        fprintf(stderr, "Memory allocation failed for font name/path!\n");
        exit(EXIT_FAILURE);
    }
    discovered_fonts_count++;
}

// Recursive function to collect .ttf font files
static void collect_fonts_recursive(const char* base_path) {
    DIR *dir = opendir(base_path);
    if (!dir) {
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            collect_fonts_recursive(path);
        } else if (entry->d_type == DT_REG) {
            const char* ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".ttf") == 0) {
                char font_name[256];
                strncpy(font_name, entry->d_name, ext - entry->d_name);
                font_name[ext - entry->d_name] = '\0';
                add_font(font_name, path);
            }
        }
    }
    closedir(dir);
}

// Function to free all discovered font info
static void free_discovered_fonts_internal() {
    for (int i = 0; i < discovered_fonts_count; ++i) {
        free(discovered_fonts[i].name);
        free(discovered_fonts[i].path);
    }
    free(discovered_fonts);
    discovered_fonts = NULL;
    discovered_fonts_count = 0;
    discovered_fonts_capacity = 0;
}

static char *load_file(const char *filename, size_t *out_size) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;
    
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }
    
    long sz = ftell(fp);
    if (sz < 0) {
        fclose(fp);
        return NULL;
    }
    
    rewind(fp);
    char *buf = malloc(sz + 1);
    if (!buf) {
        fclose(fp);
        return NULL;
    }
    
    size_t read_size = fread(buf, 1, sz, fp);
    buf[read_size] = '\0';
    fclose(fp);
    
    if (out_size) *out_size = read_size;
    return buf;
}

static void get_code_dimensions(const char* code_buffer, stbtt_fontinfo* font, float scale, float font_pixel_height, float line_spacing_multiplier, int padding, int* out_max_width, int* out_total_height) {
    *out_max_width = 0;
    *out_total_height = 0;

    if (!code_buffer || !font) {
        fprintf(stderr, "DEBUG: get_code_dimensions received null code_buffer or font.\n");
        return;
    }

    int line_count = 0;
    int current_line_char_count = 0;
    int max_line_char_count = 0;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    float font_line_height_base = (ascent - descent + lineGap) * scale;

    int space_advance_width_units;
    stbtt_GetCodepointHMetrics(font, ' ', &space_advance_width_units, NULL);
    float assumed_char_width = space_advance_width_units * scale;
    
    if (assumed_char_width <= 0) {
        stbtt_GetCodepointHMetrics(font, 'M', &space_advance_width_units, NULL);
        assumed_char_width = space_advance_width_units * scale;
        if (assumed_char_width <= 0) {
            assumed_char_width = font_pixel_height * 0.6f;
        }
    }
    // fprintf(stderr, "DEBUG: Assumed Char Width for calculation: %.2f pixels\n", assumed_char_width);


    for (size_t i = 0; code_buffer[i] != '\0'; ++i) {
        int codepoint = (unsigned char)code_buffer[i];
        if (codepoint == '\n') {
            if (current_line_char_count > max_line_char_count) {
                max_line_char_count = current_line_char_count;
            }
            current_line_char_count = 0;
            line_count++;
        } else if (codepoint == '\t') {
            current_line_char_count += 4;
        } else {
            current_line_char_count++;
        }
    }
    if (current_line_char_count > max_line_char_count) {
        max_line_char_count = current_line_char_count;
    }
    line_count++;

    *out_max_width = (int)(max_line_char_count * assumed_char_width + 0.5f);
    *out_max_width += 2 * padding;

    *out_total_height = (int)(line_count * font_line_height_base * line_spacing_multiplier + 0.5f);
    *out_total_height += 2 * padding;

    if (*out_max_width < (int)(font_pixel_height * 10)) *out_max_width = (int)(font_pixel_height * 10);
    if (*out_total_height < (int)(font_pixel_height * 3)) *out_total_height = (int)(font_pixel_height * 3);

    // fprintf(stderr, "DEBUG: Lines: %d, Max Chars per line: %d\n", line_count, max_line_char_count);
    // fprintf(stderr, "DEBUG: Calculated Image Dimensions (before user override): %dx%d\n", *out_max_width, *out_total_height);
}

// --- Public API Function ---
int code_to_image_generate(
    const char *input_file_path,
    const char *output_image_path,
    const char *font_name,
    float font_size,
    int img_width_arg,
    int img_height_arg
) {
    if (discovered_fonts) {
        free_discovered_fonts_internal();
    }
    collect_fonts_recursive("modules/Fonts");

    char *code_content = NULL;
    size_t code_content_size = 0;
    
    if (!input_file_path) {
        fprintf(stderr, "Error: Input file path is NULL.\n");
        free_discovered_fonts_internal();
        return 1;
    }

    code_content = load_file(input_file_path, &code_content_size);
    if (!code_content) {
        fprintf(stderr, "Error: Could not read input file '%s'.\n", input_file_path);
        free_discovered_fonts_internal();
        return 1;
    }

    const char* font_to_load_path = NULL;
    if (!font_name && discovered_fonts_count > 0) {
        font_to_load_path = discovered_fonts[0].path;
        fprintf(stderr, "No font specified. Defaulting to '%s'.\n", discovered_fonts[0].name);
    } else if (!font_name && discovered_fonts_count == 0) {
         fprintf(stderr, "Error: No fonts found in 'modules/Fonts/' directory. Cannot proceed without a font.\n");
         free(code_content);
         free_discovered_fonts_internal();
         return 1;
    } else {
        for (int i = 0; i < discovered_fonts_count; ++i) {
            if (strcmp(font_name, discovered_fonts[i].name) == 0) {
                font_to_load_path = discovered_fonts[i].path;
                break;
            }
        }
        if (!font_to_load_path) {
            fprintf(stderr, "Error: Specified font '%s' not found.\n", font_name);
            free(code_content);
            free_discovered_fonts_internal();
            return 1;
        }
    }

    long font_buffer_size;
    unsigned char* font_buffer = NULL;

    FILE* font_file = fopen(font_to_load_path, "rb");
    if (!font_file) {
        fprintf(stderr, "Error: Could not open font file '%s'. This should not happen if discovered correctly.\n", font_to_load_path);
        free(code_content);
        free_discovered_fonts_internal();
        return 1;
    }

    fseek(font_file, 0, SEEK_END);
    font_buffer_size = ftell(font_file);
    fseek(font_file, 0, SEEK_SET);

    font_buffer = (unsigned char*)malloc(font_buffer_size);
    if (!font_buffer) {
        fprintf(stderr, "Failed to allocate font buffer memory!\n");
        fclose(font_file);
        free(code_content);
        free_discovered_fonts_internal();
        return 1;
    }
    fread(font_buffer, 1, font_buffer_size, font_file);
    fclose(font_file);

    stbtt_fontinfo font_info;
    if (!stbtt_InitFont(&font_info, font_buffer, 0)) {
        fprintf(stderr, "Failed to initialize font from '%s'!\n", font_to_load_path);
        free(font_buffer);
        free(code_content);
        free_discovered_fonts_internal();
        return 1;
    }

    float scale = stbtt_ScaleForPixelHeight(&font_info, font_size);

    // --- Determine Image Dimensions ---
    int calculated_img_width, calculated_img_height;
    float line_spacing_multiplier = 1.5f;
    int inner_padding = 20;

    get_code_dimensions(code_content, &font_info, scale, font_size, line_spacing_multiplier, inner_padding, &calculated_img_width, &calculated_img_height);

    // Use user-provided dimensions if available, otherwise use calculated ones
    int img_width = (img_width_arg > 0) ? img_width_arg : calculated_img_width;
    int img_height = (img_height_arg > 0) ? img_height_arg : calculated_img_height;
    
    // Ensure minimums if calculated dimensions are too small or user provides tiny ones
    if (img_width < 200) img_width = 200;
    if (img_height < 100) img_height = 100;


    // Allocate memory for image pixels
    uint8_t *pixels = (uint8_t *)malloc(img_width * img_height * CHANNELS); 
    if (!pixels) {
        fprintf(stderr, "Failed to allocate pixel buffer memory!\n");
        free(font_buffer);
        free(code_content);
        free_discovered_fonts_internal();
        return 1;
    }


    // --- Define Colors ---
    uint8_t bg_r, bg_g, bg_b;
    uint8_t code_bg_r, code_bg_g, code_bg_b;
    uint8_t default_text_r, default_text_g, default_text_b;

    hex_to_rgb("#1a1a1a", &bg_r, &bg_g, &bg_b);
    hex_to_rgb("#0d0d0d", &code_bg_r, &code_bg_g, &code_bg_b);
    hex_to_rgb("#f8f8f2", &default_text_r, &default_text_g, &default_text_b);


    // --- 3. Fill Background ---
    for (int y = 0; y < img_height; ++y) {
        for (int x = 0; x < img_width; ++x) {
            int index = (y * img_width + x) * CHANNELS;
            pixels[index + 0] = bg_r;
            pixels[index + 1] = bg_g;
            pixels[index + 2] = bg_b;
        }
    }

    // --- 4. Draw Code Block Background ---
    int code_block_x = inner_padding;
    int code_block_y = inner_padding;
    int code_block_width = img_width - 2 * inner_padding;
    int code_block_height = img_height - 2 * inner_padding;

    for (int y = code_block_y; y < code_block_y + code_block_height; ++y) {
        for (int x = code_block_x; x < code_block_x + code_block_width; ++x) {
            if (x >=0 && x < img_width && y >= 0 && y < img_height) { // Safety check
                int index = (y * img_width + x) * CHANNELS;
                pixels[index + 0] = code_bg_r;
                pixels[index + 1] = code_bg_g;
                pixels[index + 2] = code_bg_b;
            }
        }
    }

    // --- 5. Draw Loaded Code Content ---
    int current_line_y = code_block_y + (int)(font_size * 0.25);
    float line_spacing_factor = 1.5f;
    
    int ascent_draw, descent_draw, lineGap_draw;
    stbtt_GetFontVMetrics(&font_info, &ascent_draw, &descent_draw, &lineGap_draw);
    float actual_font_line_height = (ascent_draw - descent_draw + lineGap_draw) * scale * line_spacing_factor;

    char *line_start = code_content;
    char *line_end;

    char temp_line_buffer[2048];

    while (true) {
        line_end = strchr(line_start, '\n');
        size_t line_len;

        if (line_end != NULL) {
            line_len = line_end - line_start;
        } else {
            line_len = strlen(line_start);
        }
        
        if (line_len >= sizeof(temp_line_buffer)) {
            line_len = sizeof(temp_line_buffer) - 1;
        }
        strncpy(temp_line_buffer, line_start, line_len);
        temp_line_buffer[line_len] = '\0';

        draw_text(pixels, img_width, img_height, code_block_x + 10, current_line_y, temp_line_buffer, &font_info, scale, default_text_r, default_text_g, default_text_b);
        current_line_y += (int)actual_font_line_height;

        if (line_end == NULL || *line_start == '\0') {
            break;
        }
        line_start = line_end + 1;
    }


    // --- 6. Save the Image ---
    if (stbi_write_png(output_image_path, img_width, img_height, CHANNELS, pixels, img_width * CHANNELS)) {
        // printf("Successfully wrote '%s'\n", output_image_path); // Removed console output for library use
    } else {
        fprintf(stderr, "Failed to write PNG file '%s'!\n", output_image_path);
        free(font_buffer);
        free(pixels);
        free(code_content);
        free_discovered_fonts_internal();
        return 1;
    }

    // --- Cleanup ---
    free(font_buffer);
    free(pixels);
    free(code_content);
    free_discovered_fonts_internal();
    return 0;
}
