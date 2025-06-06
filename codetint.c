#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tree_sitter/api.h>

// Declare external Tree-sitter language functions
const TSLanguage *tree_sitter_python(void);
const TSLanguage *tree_sitter_c(void);
const TSLanguage *tree_sitter_cpp(void);
const TSLanguage *tree_sitter_javascript(void);
const TSLanguage *tree_sitter_html(void);
const TSLanguage *tree_sitter_css(void);

// Structure to hold language information and its associated parser and default query
typedef struct {
    const char *name;
    const char *extension;
    const TSLanguage *(*language_function)(void);
    const char *default_query_path; // Path to default .scm query file for this language
} LanguageInfo;

// Define supported languages and their properties
LanguageInfo supported_languages[] = {
    {"python", ".py", tree_sitter_python, "queries/python.scm"},
    {"c",      ".c",  tree_sitter_c,      "queries/c.scm"},
    {"cpp",    ".cpp", tree_sitter_cpp,   "queries/cpp.scm"},
    {"javascript", ".js", tree_sitter_javascript, "queries/javascript.scm"},
    {"html",   ".html", tree_sitter_html,     "queries/html.scm"},
    {"css",    ".css", tree_sitter_css,      "queries/css.scm"},
    {NULL, NULL, NULL, NULL} // Sentinel to mark the end of the array
};


// Predefined color themes
typedef struct {
    const char *name;
    const char *function_builtin;
    const char *function;
    const char *string;
    const char *comment;
    const char *keyword;
    const char *keyword_control;
    const char *type;
    const char *variable;
    const char *constant;
    const char *literal;
    const char *reset;
} ColorTheme;

ColorTheme themes[] = {
    {
        "default",
        "\x1b[35m", // function.builtin - magenta
        "\x1b[34m", // function - blue
        "\x1b[32m", // string - green
        "\x1b[90m", // comment - bright black
        "\x1b[31m", // keyword - red
        "\x1b[31;1m", // keyword.control - bright red/bold
        "\x1b[36m", // type - cyan
        "\x1b[33m", // variable - yellow
        "\x1b[35;1m", // constant - bright magenta
        "\x1b[32;1m", // literal - bright green
        "\x1b[0m"    // reset
    },
    {
        "gruvbox",
        "\x1b[38;5;142m", // function.builtin - gruvbox aqua
        "\x1b[38;5;109m", // function - gruvbox blue
        "\x1b[38;5;108m", // string - gruvbox green
        "\x1b[38;5;245m", // comment - gruvbox gray
        "\x1b[38;5;167m", // keyword - gruvbox red
        "\x1b[38;5;167;1m", // keyword.control - gruvbox red bold
        "\x1b[38;5;142m", // type - gruvbox yellow
        "\x1b[38;5;223m", // variable - gruvbox fg
        "\x1b[38;5;175m", // constant - gruvbox purple
        "\x1b[38;5;208m", // literal - gruvbox orange
        "\x1b[0m"    // reset
    },
    {
        "tokyonight-night",
        "\x1b[38;5;73m",  // function.builtin - teal
        "\x1b[38;5;110m", // function - blue
        "\x1b[38;5;158m", // string - green
        "\x1b[38;5;102m", // comment - comment gray
        "\x1b[38;5;175m", // keyword - magenta
        "\x1b[38;5;175;1m", // keyword.control - magenta bold
        "\x1b[38;5;117m", // type - cyan
        "\x1b[38;5;188m", // variable - fg
        "\x1b[38;5;215m", // constant - orange
        "\x1b[38;5;215m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "tokyonight-storm",
        "\x1b[38;5;73m",  // function.builtin - teal
        "\x1b[38;5;75m",  // function - blue
        "\x1b[38;5;114m", // string - green
        "\x1b[38;5;102m", // comment - comment gray
        "\x1b[38;5;176m", // keyword - purple
        "\x1b[38;5;176;1m", // keyword.control - purple bold
        "\x1b[38;5;117m", // type - cyan
        "\x1b[38;5;188m", // variable - fg
        "\x1b[38;5;215m", // constant - orange
        "\x1b[38;5;215m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "catppuccin-mocha",
        "\x1b[38;5;117m", // function.builtin - sky
        "\x1b[38;5;75m",  // function - blue
        "\x1b[38;5;114m", // string - green
        "\x1b[38;5;102m", // comment - overlay1
        "\x1b[38;5;176m", // keyword - mauve
        "\x1b[38;5;176;1m", // keyword.control - mauve bold
        "\x1b[38;5;117m", // type - sky
        "\x1b[38;5;188m", // variable - text
        "\x1b[38;5;215m", // constant - peach
        "\x1b[38;5;215m", // literal - peach
        "\x1b[0m"    // reset
    },
    {
        "dracula",
        "\x1b[38;5;141m", // function.builtin - purple
        "\x1b[38;5;117m", // function - cyan
        "\x1b[38;5;228m", // string - yellow
        "\x1b[38;5;102m", // comment - comment
        "\x1b[38;5;212m", // keyword - pink
        "\x1b[38;5;212;1m", // keyword.control - pink bold
        "\x1b[38;5;117m", // type - cyan
        "\x1b[38;5;255m", // variable - foreground
        "\x1b[38;5;141m", // constant - purple
        "\x1b[38;5;215m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "nord",
        "\x1b[38;5;116m", // function.builtin - frost3
        "\x1b[38;5;81m",  // function - frost2
        "\x1b[38;5;108m", // string - aurora green
        "\x1b[38;5;59m",  // comment - polar night3
        "\x1b[38;5;81m",  // keyword - frost2
        "\x1b[38;5;81;1m", // keyword.control - frost2 bold
        "\x1b[38;5;116m", // type - frost3
        "\x1b[38;5;188m", // variable - snow storm1
        "\x1b[38;5;180m", // constant - aurora yellow
        "\x1b[38;5;209m", // literal - aurora orange
        "\x1b[0m"    // reset
    },
    {
        "solarized-dark",
        "\x1b[38;5;37m",  // function.builtin - cyan
        "\x1b[38;5;33m",  // function - blue
        "\x1b[38;5;37m",  // string - cyan
        "\x1b[38;5;240m", // comment - base01
        "\x1b[38;5;64m",  // keyword - green
        "\x1b[38;5;64;1m", // keyword.control - green bold
        "\x1b[38;5;136m", // type - yellow
        "\x1b[38;5;254m", // variable - base0
        "\x1b[38;5;125m", // constant - magenta
        "\x1b[38;5;166m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "solarized-light",
        "\x1b[38;5;37m",  // function.builtin - cyan
        "\x1b[38;5;33m",  // function - blue
        "\x1b[38;5;37m",  // string - cyan
        "\x1b[38;5;244m", // comment - base1
        "\x1b[38;5;64m",  // keyword - green
        "\x1b[38;5;64;1m", // keyword.control - green bold
        "\x1b[38;5;136m", // type - yellow
        "\x1b[38;5;235m", // variable - base00
        "\x1b[38;5;125m", // constant - magenta
        "\x1b[38;5;166m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "one-dark",
        "\x1b[38;5;39m",  // function.builtin - cyan
        "\x1b[38;5;75m",  // function - blue
        "\x1b[38;5;114m", // string - green
        "\x1b[38;5;59m",  // comment - comment gray
        "\x1b[38;5;176m", // keyword - purple
        "\x1b[38;5;176;1m", // keyword.control - purple bold
        "\x1b[38;5;39m",  // type - cyan
        "\x1b[38;5;145m", // variable - mono 1
        "\x1b[38;5;215m", // constant - orange
        "\x1b[38;5;215m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "monokai",
        "\x1b[38;5;81m",  // function.builtin - cyan
        "\x1b[38;5;148m", // function - green
        "\x1b[38;5;186m", // string - yellow
        "\x1b[38;5;102m", // comment - comment
        "\x1b[38;5;197m", // keyword - red
        "\x1b[38;5;197;1m", // keyword.control - red bold
        "\x1b[38;5;81m",  // type - cyan
        "\x1b[38;5;255m", // variable - white
        "\x1b[38;5;141m", // constant - purple
        "\x1b[38;5;208m", // literal - orange
        "\x1b[0m"    // reset
    },
    {
        "github-dark",
        "\x1b[38;5;117m", // function.builtin - blue
        "\x1b[38;5;183m", // function - purple
        "\x1b[38;5;150m", // string - teal
        "\x1b[38;5;102m", // comment - gray
        "\x1b[38;5;204m", // keyword - red
        "\x1b[38;5;204;1m", // keyword.control - red bold
        "\x1b[38;5;117m", // constant - blue
        "\x1b[38;5;188m", // variable - fg default
        "\x1b[38;5;117m", // constant - blue
        "\x1b[38;5;215m", // literal - orange
        "\x1b[0m"    // reset
    }
};

#define THEMES_COUNT (sizeof(themes)/sizeof(ColorTheme))

// Global selected theme pointer
ColorTheme *selected_theme = NULL;

// Get color by capture name for ANSI output
const char *get_ansi_color(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return selected_theme->function_builtin;
    if (strcmp(capture_name, "function") == 0) return selected_theme->function;
    if (strcmp(capture_name, "function.call") == 0) return selected_theme->function;
    if (strcmp(capture_name, "string") == 0) return selected_theme->string;
    if (strcmp(capture_name, "comment") == 0) return selected_theme->comment;
    if (strcmp(capture_name, "keyword.control") == 0) return selected_theme->keyword_control;
    if (strcmp(capture_name, "keyword") == 0) return selected_theme->keyword;
    if (strcmp(capture_name, "keyword.function") == 0) return selected_theme->keyword;
    if (strcmp(capture_name, "keyword.type") == 0) return selected_theme->keyword;
    if (strcmp(capture_name, "keyword.import") == 0) return selected_theme->keyword;
    if (strcmp(capture_name, "keyword.return") == 0) return selected_theme->keyword;
    if (strcmp(capture_name, "type") == 0) return selected_theme->type;
    if (strcmp(capture_name, "variable") == 0) return selected_theme->variable;
    if (strcmp(capture_name, "constant") == 0) return selected_theme->constant;
    if (strcmp(capture_name, "constant.builtin") == 0) return selected_theme->constant;
    if (strcmp(capture_name, "literal") == 0) return selected_theme->literal;
    if (strcmp(capture_name, "number") == 0) return selected_theme->literal;
    return selected_theme->reset; // default reset
}

// Get color by capture name for HTML output (CSS classes)
const char *get_html_class(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return "function-builtin";
    if (strcmp(capture_name, "function") == 0) return "function";
    if (strcmp(capture_name, "function.call") == 0) return "function";
    if (strcmp(capture_name, "string") == 0) return "string";
    if (strcmp(capture_name, "comment") == 0) return "comment";
    if (strcmp(capture_name, "keyword.control") == 0) return "keyword-control";
    if (strcmp(capture_name, "keyword") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.function") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.type") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.import") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.return") == 0) return "keyword";
    if (strcmp(capture_name, "type") == 0) return "type";
    if (strcmp(capture_name, "variable") == 0) return "variable";
    if (strcmp(capture_name, "constant") == 0) return "constant";
    if (strcmp(capture_name, "constant.builtin") == 0) return "constant";
    if (strcmp(capture_name, "literal") == 0) return "literal";
    if (strcmp(capture_name, "number") == 0) return "literal";
    return NULL;
}

// Print usage help
void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s [options] <file_path>\n", progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -q FILE     Use external query file for highlights\n");
    fprintf(stderr, "  -c THEME    Select color theme (default: default)\n");
    fprintf(stderr, "  -l LANG     Explicitly set language (e.g., 'python', 'c', 'javascript'). Overrides file extension detection.\n");
    fprintf(stderr, "  -o FILE     Output to file instead of stdout\n");
    fprintf(stderr, "  --html      Output HTML instead of ANSI colors\n");
    fprintf(stderr, "Available themes: ");
    for (size_t i = 0; i < THEMES_COUNT; i++) {
        fprintf(stderr, "%s%s", themes[i].name, (i < THEMES_COUNT - 1) ? ", " : "\n");
    }
    fprintf(stderr, "Supported languages (auto-detected by extension or set with -l): ");
    for (int i = 0; supported_languages[i].name != NULL; i++) {
        fprintf(stderr, "%s%s", supported_languages[i].name, (supported_languages[i+1].name != NULL) ? ", " : "\n");
    }
}

// Load the content of a file into a malloc'ed buffer, returns size via out_size
char *load_file(const char *filename, size_t *out_size) {
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

// Function to get language info based on file extension
LanguageInfo* get_language_info_from_path(const char* filepath) {
    const char *dot = strrchr(filepath, '.');
    if (!dot || dot == filepath) return NULL; // No extension or starts with dot

    for (int i = 0; supported_languages[i].name != NULL; i++) {
        if (strcmp(dot, supported_languages[i].extension) == 0) {
            return &supported_languages[i];
        }
    }
    return NULL; // Language not found
}

// Function to get language info based on explicit language name
LanguageInfo* get_language_info_from_name(const char* lang_name) {
    for (int i = 0; supported_languages[i].name != NULL; i++) {
        if (strcmp(lang_name, supported_languages[i].name) == 0) {
            return &supported_languages[i];
        }
    }
    return NULL; // Language not found
}


int main(int argc, char **argv) {
    const char *input_file = NULL;
    const char *query_file = NULL;
    const char *output_file = NULL;
    const char *explicit_lang_name = NULL;
    bool output_html = false;
    selected_theme = &themes[0]; // default theme
    
    LanguageInfo *current_lang_info = NULL;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            query_file = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            const char *theme_name = argv[++i];
            bool found = false;
            for (size_t t = 0; t < THEMES_COUNT; t++) {
                if (strcmp(themes[t].name, theme_name) == 0) {
                    selected_theme = &themes[t];
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Unknown theme '%s'\n", theme_name);
                print_usage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            explicit_lang_name = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--html") == 0) {
            output_html = true;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        } else {
            input_file = argv[i];
        }
    }

    if (!input_file) {
        print_usage(argv[0]);
        return 1;
    }

    // Determine language: explicit -l flag takes precedence, then file extension
    if (explicit_lang_name) {
        current_lang_info = get_language_info_from_name(explicit_lang_name);
        if (!current_lang_info) {
            fprintf(stderr, "Error: Unknown language '%s' specified with -l flag.\n", explicit_lang_name);
            print_usage(argv[0]);
            return 1;
        }
    } else {
        current_lang_info = get_language_info_from_path(input_file);
        if (!current_lang_info) {
            fprintf(stderr, "Error: Could not determine language for '%s' from file extension.\n", input_file);
            fprintf(stderr, "Please specify it using the -l flag.\n");
            print_usage(argv[0]);
            return 1;
        }
    }


    // Load source code
    size_t code_size;
    char *code = load_file(input_file, &code_size);
    if (!code) {
        perror("Failed to open input file");
        return 1;
    }

    // Load query string
    char *query_str = NULL;
    if (query_file) {
        size_t query_size;
        query_str = load_file(query_file, &query_size);
        if (!query_str) {
            perror("Failed to open query file");
            free(code);
            return 1;
        }
    } else {
        // Load default query based on detected language
        if (current_lang_info->default_query_path) {
            size_t query_size;
            query_str = load_file(current_lang_info->default_query_path, &query_size);
            if (!query_str) {
                fprintf(stderr, "Failed to load default query for %s from %s\n", current_lang_info->name, current_lang_info->default_query_path);
                free(code);
                return 1;
            }
        } else {
            fprintf(stderr, "No default query path defined for language '%s'\n", current_lang_info->name);
            free(code);
            return 1;
        }
    }

    // Initialize parser
    TSParser *parser = ts_parser_new();
    if (!parser) {
        fprintf(stderr, "Failed to create parser\n");
        free(code);
        free(query_str);
        return 1;
    }
    
    // Set language based on detected/explicit choice
    if (!ts_parser_set_language(parser, current_lang_info->language_function())) {
        fprintf(stderr, "Failed to set language for %s. Version mismatch?\n", current_lang_info->name);
        ts_parser_delete(parser);
        free(code);
        free(query_str);
        return 1;
    }

    TSTree *tree = ts_parser_parse_string(parser, NULL, code, code_size);
    if (!tree) {
        fprintf(stderr, "Failed to parse code\n");
        ts_parser_delete(parser);
        free(code);
        free(query_str);
        return 1;
    }
    
    TSNode root = ts_tree_root_node(tree);

    // Compile query using the selected language
    TSQueryError error_type;
    uint32_t error_offset;
    TSQuery *query = ts_query_new(current_lang_info->language_function(), query_str, strlen(query_str), &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "Query parse error at offset %u, error type: %d\n", error_offset, error_type);
        ts_tree_delete(tree);
        ts_parser_delete(parser);
        free(code);
        free(query_str);
        return 1;
    }

    // Open output file or stdout
    FILE *out = stdout;
    if (output_file) {
        out = fopen(output_file, "w");
        if (!out) {
            perror("Failed to open output file");
            ts_query_delete(query);
            ts_tree_delete(tree);
            ts_parser_delete(parser);
            free(code);
            free(query_str);
            return 1;
        }
    }

    TSQueryCursor *cursor = ts_query_cursor_new();
    if (!cursor) {
        fprintf(stderr, "Failed to create query cursor\n");
        if (output_file) fclose(out);
        ts_query_delete(query);
        ts_tree_delete(tree);
        ts_parser_delete(parser);
        free(code);
        free(query_str);
        return 1;
    }
    
    ts_query_cursor_exec(cursor, query, root);

    size_t current_byte = 0;
    TSQueryMatch match; // <-- Declaration added here

    if (output_html) {
        // Print HTML header with dynamic CSS based on selected theme
        fprintf(out, "<!DOCTYPE html>\n<html><head><style>\n");

        // Base styles
        fprintf(out, "body { font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace; margin: 20px; }\n");
        fprintf(out, "pre { margin: 0; line-height: 1.4; }\n");

        // Theme-specific styles (consider moving these to external CSS files for better modularity)
        // For brevity, using the existing if-else if structure.
        if (strcmp(selected_theme->name, "gruvbox") == 0) {
            fprintf(out, "body { background: #282828; color: #ebdbb2; }\n");
            fprintf(out, ".function-builtin { color: #8ec07c; }\n");
            fprintf(out, ".function { color: #83a598; }\n");
            fprintf(out, ".string { color: #b8bb26; }\n");
            fprintf(out, ".comment { color: #928374; font-style: italic; }\n");
            fprintf(out, ".keyword { color: #fb4934; }\n");
            fprintf(out, ".keyword-control { color: #fb4934; font-weight: bold; }\n");
            fprintf(out, ".type { color: #fabd2f; }\n");
            fprintf(out, ".variable { color: #ebdbb2; }\n");
            fprintf(out, ".constant { color: #d3869b; }\n");
            fprintf(out, ".literal { color: #fe8019; }\n");
        } else if (strcmp(selected_theme->name, "tokyonight-night") == 0) {
            fprintf(out, "body { background: #1a1b26; color: #c0caf5; }\n");
            fprintf(out, ".function-builtin { color: #1abc9c; }\n");
            fprintf(out, ".function { color: #7aa2f7; }\n");
            fprintf(out, ".string { color: #9ece6a; }\n");
            fprintf(out, ".comment { color: #565f89; font-style: italic; }\n");
            fprintf(out, ".keyword { color: #bb9af7; }\n");
            fprintf(out, ".keyword-control { color: #bb9af7; font-weight: bold; }\n");
            fprintf(out, ".type { color: #2ac3de; }\n");
            fprintf(out, ".variable { color: #c0caf5; }\n");
            fprintf(out, ".constant { color: #ff9e64; }\n");
            fprintf(out, ".literal { color: #ff9e64; }\n");
        } else if (strcmp(selected_theme->name, "dracula") == 0) {
            fprintf(out, "body { background: #282a36; color: #f8f8f2; }\n");
            fprintf(out, ".function-builtin { color: #bd93f9; }\n");
            fprintf(out, ".function { color: #8be9fd; }\n");
            fprintf(out, ".string { color: #f1fa8c; }\n");
            fprintf(out, ".comment { color: #6272a4; font-style: italic; }\n");
            fprintf(out, ".keyword { color: #ff79c6; }\n");
            fprintf(out, ".keyword-control { color: #ff79c6; font-weight: bold; }\n");
            fprintf(out, ".type { color: #8be9fd; }\n");
            fprintf(out, ".variable { color: #f8f8f2; }\n");
            fprintf(out, ".constant { color: #bd93f9; }\n");
            fprintf(out, ".literal { color: #ffb86c; }\n");
        } else {
            // Default theme (dark)
            fprintf(out, "body { background: #1e1e1e; color: #d4d4d4; }\n");
            fprintf(out, ".function-builtin { color: #dcdcaa; }\n");
            fprintf(out, ".function { color: #569cd6; }\n");
            fprintf(out, ".string { color: #ce9178; }\n");
            fprintf(out, ".comment { color: #6a9955; font-style: italic; }\n");
            fprintf(out, ".keyword { color: #c586c0; }\n");
            fprintf(out, ".keyword-control { color: #c586c0; font-weight: bold; }\n");
            fprintf(out, ".type { color: #4ec9b0; }\n");
            fprintf(out, ".variable { color: #9cdcfe; }\n");
            fprintf(out, ".constant { color: #569cd6; font-weight: bold; }\n");
            fprintf(out, ".literal { color: #b5cea8; }\n");
        }
        
        fprintf(out, "</style></head><body><pre>\n");
    }

    while (ts_query_cursor_next_match(cursor, &match)) {
        for (uint32_t i = 0; i < match.capture_count; i++) {
            TSQueryCapture capture = match.captures[i];
            uint32_t name_len;
            const char *name = ts_query_capture_name_for_id(query, capture.index, &name_len);
            if (!name) continue;

            TSNode node = capture.node;
            uint32_t start = ts_node_start_byte(node);
            uint32_t end = ts_node_end_byte(node);
            if (start >= end || end > code_size) continue;

            // Print non-highlighted text before this capture
            if (start > current_byte) {
                if (output_html) {
                    for (size_t j = current_byte; j < start; j++) {
                        char c = code[j];
                        if (c == '&') fputs("&amp;", out);
                        else if (c == '<') fputs("&lt;", out);
                        else if (c == '>') fputs("&gt;", out);
                        else fputc(c, out);
                    }
                } else {
                    fwrite(code + current_byte, 1, start - current_byte, out);
                }
            }

            // Determine the actual range to print for this highlight
            // It should start from max(current_byte, start) to avoid reprinting
            size_t print_start = (current_byte > start) ? current_byte : start;
            size_t print_end = end;

            // Only print if there's actual content to print for this highlight
            if (print_start < print_end) {
                if (output_html) {
                    const char *cls = get_html_class(name);
                    if (cls) fprintf(out, "<span class=\"%s\">", cls);

                    // Print escaped code for the actual highlight range
                    for (size_t j = print_start; j < print_end; j++) {
                        char c = code[j];
                        if (c == '&') fputs("&amp;", out);
                        else if (c == '<') fputs("&lt;", out);
                        else if (c == '>') fputs("&gt;", out);
                        else fputc(c, out);
                    }

                    if (cls) fprintf(out, "</span>");
                } else {
                    // ANSI output
                    fprintf(out, "%s", get_ansi_color(name));
                    fwrite(code + print_start, 1, print_end - print_start, out);
                    fprintf(out, "%s", selected_theme->reset);
                }
            }

            // Update current_byte to the end of the *current* capture's range,
            // only if it advances the overall progress.
            if (end > current_byte) {
                current_byte = end;
            }
        }
    }

    // Print remaining text after the last highlight
    if (current_byte < code_size) {
        if (output_html) {
            for (size_t j = current_byte; j < code_size; j++) {
                char c = code[j];
                if (c == '&') fputs("&amp;", out);
                else if (c == '<') fputs("&lt;", out);
                else if (c == '>') fputs("&gt;", out);
                else fputc(c, out);
            }
        } else {
            fwrite(code + current_byte, 1, code_size - current_byte, out);
        }
    }

    if (output_html) {
        fprintf(out, "</pre></body></html>\n");
    }

    // Proper cleanup
    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
    ts_tree_delete(tree);
    ts_parser_delete(parser);
    free(code);
    free(query_str);

    if (output_file) fclose(out);

    return 0;
}
