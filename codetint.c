#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tree_sitter/api.h>

#include "modules/theme.h"

// External Tree-sitter language functions
const TSLanguage *tree_sitter_python(void);
const TSLanguage *tree_sitter_c(void);
const TSLanguage *tree_sitter_cpp(void);
const TSLanguage *tree_sitter_javascript(void);
const TSLanguage *tree_sitter_html(void);
const TSLanguage *tree_sitter_css(void);

// Structure to hold language information and associated parser and default query
typedef struct {
    const char *name;
    const char *extension;
    const TSLanguage *(*language_function)(void);
    const char *default_query_path;
} LanguageInfo;

LanguageInfo supported_languages[] = {
    {"python", ".py", tree_sitter_python, "queries/python.scm"},
    {"c", ".c",  tree_sitter_c, "queries/c.scm"},
    {"cpp", ".cpp", tree_sitter_cpp, "queries/cpp.scm"},
    {"javascript", ".js", tree_sitter_javascript, "queries/javascript.scm"},
    {"html", ".html", tree_sitter_html, "queries/html.scm"},
    {"css", ".css", tree_sitter_css, "queries/css.scm"},
    {NULL, NULL, NULL, NULL} // Sentinel to mark the end of the array
};


// Print usage help
void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s [options] <file_path>\n\n", progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -q FILE    Use external query file for highlights\n");
    fprintf(stderr, "  -c THEME   Select color theme (default: default)\n");
    fprintf(stderr, "  -l LANG    Explicitly set language (e.g., 'python', 'c', 'javascript'). Overrides file extension detection.\n");
    fprintf(stderr, "  -o FILE    Output to file instead of stdout\n");
    fprintf(stderr, "  --html     Output HTML instead of ANSI colors\n");
    fprintf(stderr, "  -n, --line-numbers Show line numbers\n\n");
    fprintf(stderr, "Available themes: ");
    for (size_t i = 0; i < THEMES_COUNT; i++) {
        fprintf(stderr, "%s%s", themes[i].name, (i < THEMES_COUNT - 1) ? ", " : "\n");
    }
    fprintf(stderr, "Supported languages (auto-detected by extension or set with -l): ");
    for (int i = 0; supported_languages[i].name != NULL; i++) {
        fprintf(stderr, "%s%s", supported_languages[i].name, (supported_languages[i+1].name != NULL) ? ", " : "\n");
    }
}

// Load the content of a file into a malloc'ed buffer
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

// Helper function to print a section of text, handling line numbers and HTML escaping
void print_code_section(FILE *out,
                        const char *code_buffer,
                        size_t start_byte,
                        size_t end_byte, 
                        bool show_line_numbers,
                        int line_num_padding, 
                        uint32_t *current_line_num_ptr,
                        bool *at_line_start_ptr, 
                        bool output_html
                        ) {
    for (size_t i = start_byte; i < end_byte; ++i) {
        if (show_line_numbers && *at_line_start_ptr) {
            if (output_html) {
                if (*current_line_num_ptr > 1) {
                    fprintf(out, "</span></div>\n");
                }
                fprintf(out, "<div class=\"line\" id=\"L%u\">", *current_line_num_ptr);
                fprintf(out, "<span class=\"line-number\">%*u</span>", line_num_padding, *current_line_num_ptr);
                fprintf(out, "<span class=\"code-line-content\">");
            } else { // ANSI BLOCK
                fprintf(out, "%s%*u │%s ",
                    selected_theme->ansi_line_number,
                    line_num_padding,
                    *current_line_num_ptr,
                    selected_theme->ansi_reset
                );
            }
            *at_line_start_ptr = false;
        }

        char c = code_buffer[i];
        if (output_html) {
            if (c == '&') fputs("&amp;", out);
            else if (c == '<') fputs("&lt;", out);
            else if (c == '>') fputs("&gt;", out);
            else fputc(c, out);
        } else {
            fputc(c, out);
        }

        if (c == '\n') {
            (*current_line_num_ptr)++;
            *at_line_start_ptr = true;
        }
    }
}


int main(int argc, char **argv) {
    const char *input_file = NULL;
    const char *query_file = NULL;
    const char *output_file = NULL;
    const char *explicit_lang_name = NULL;
    bool output_html = false;
    bool show_line_numbers = false;
    
    LanguageInfo *current_lang_info = NULL;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            query_file = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            const char *theme_name = argv[++i];
            if (!set_selected_theme(theme_name)) {
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
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--line-numbers") == 0) {
            show_line_numbers = true;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
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

    // Compile query
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
    TSQueryMatch match;

    // Line number related variables
    uint32_t current_line_num = 1;
    bool at_line_start = true;

    int line_num_padding = 0;
    if (show_line_numbers) {
        uint32_t total_lines = 1;
        for (size_t i = 0; i < code_size; ++i) {
            if (code[i] == '\n') {
                total_lines++;
            }
        }
        
        char temp_buffer[16];
        line_num_padding = snprintf(temp_buffer, sizeof(temp_buffer), "%u", total_lines);
        if (line_num_padding < 4) line_num_padding = 4;
    }

    if (output_html) {
        fprintf(out, "<!DOCTYPE html>\n<html><head><title>Highlighted Code</title>\n");
        fprintf(out, "<meta charset=\"utf-8\">\n");
        fprintf(out, "<style>\n");
        fprintf(out, "body { background-color: #1a1a1a; color: #e0e0e0; font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace; margin: 20px; }\n");
        fprintf(out, "pre { margin: 0; line-height: 1.4; white-space: pre-wrap; word-wrap: break-word; }\n");
        fprintf(out, ".code-container { background-color: #0d0d0d; border: 1px solid #333; padding: 10px; border-radius: 5px; overflow-x: auto; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); }\n");
        fprintf(out, ".line { display: flex; align-items: baseline; }\n");
        fprintf(out, ".line:hover { background-color: rgba(255, 255, 255, 0.05); }\n");

        if (show_line_numbers) {
            fprintf(out,
                ".line-number { "
                "color: %s; "
                "text-align: right; "
                "user-select: none; -webkit-user-select: none; "
                "display: inline-block; "
                "min-width: %dch; "
                "padding-right: 1em; "
                "margin-right: 1em; "
                "border-right: 1px solid #333; "
                "}\n",
                selected_theme->html_line_number,
                line_num_padding
            );
            // Style for the span that holds actual code content
            fprintf(out, ".code-line-content { display: block; flex-grow: 1; }\n");
        }
        
        // Base highlighting styles
        fprintf(out, "/* Base highlighting styles (will be overridden by theme-specific rules) */\n");
        fprintf(out, ".function-builtin { color: %s; }\n", themes[0].html_function_builtin);
        fprintf(out, ".function { color: %s; }\n", themes[0].html_function);
        fprintf(out, ".string { color: %s; }\n", themes[0].html_string);
        fprintf(out, ".comment { color: %s; font-style: italic; }\n", themes[0].html_comment);
        fprintf(out, ".keyword { color: %s; }\n", themes[0].html_keyword);
        fprintf(out, ".keyword-control { color: %s; font-weight: bold; }\n", themes[0].html_keyword_control);
        fprintf(out, ".type { color: %s; }\n", themes[0].html_type);
        fprintf(out, ".variable { color: %s; }\n", themes[0].html_variable);
        fprintf(out, ".constant { color: %s; }\n", themes[0].html_constant);
        fprintf(out, ".literal { color: %s; }\n", themes[0].html_literal);

        // Generate all theme CSS classes
        for (size_t t = 0; t < THEMES_COUNT; t++) {
            fprintf(out, ".theme-%s body { background: ", themes[t].name);
            if (strcmp(themes[t].name, "gruvbox") == 0) fprintf(out, "#282828; color: #ebdbb2; }\n");
            else if (strcmp(themes[t].name, "dracula") == 0) fprintf(out, "#282a36; color: #f8f8f2; }\n");
            else if (strcmp(themes[t].name, "nord") == 0) fprintf(out, "#2E3440; color: #D8DEE9; }\n");
            else if (strcmp(themes[t].name, "one-dark") == 0) fprintf(out, "#282C34; color: #ABB2BF; }\n");
            else if (strcmp(themes[t].name, "tokyonight-night") == 0) fprintf(out, "#1a1b26; color: #a9b1d6; }\n");
            else if (strcmp(themes[t].name, "tokyonight-storm") == 0) fprintf(out, "#24283b; color: #c0caf5; }\n");
            else if (strcmp(themes[t].name, "catppuccin-mocha") == 0) fprintf(out, "#1E1E2E; color: #CDD6F4; }\n");
            else if (strcmp(themes[t].name, "solarized-dark") == 0) fprintf(out, "#002b36; color: #839496; }\n");
            else if (strcmp(themes[t].name, "solarized-light") == 0) fprintf(out, "#fdf6e3; color: #586e75; }\n");
            else if (strcmp(themes[t].name, "monokai") == 0) fprintf(out, "#272822; color: #F8F8F2; }\n");
            else if (strcmp(themes[t].name, "github-dark") == 0) fprintf(out, "#22272E; color: #ADBAC7; }\n");
            else fprintf(out, "#1e1e1e; color: #d4d4d4; }\n"); // Default fallback
            
            // Generation for specific capture types using HTML colors
            fprintf(out, ".theme-%s .function-builtin { color: %s; }\n", themes[t].name, themes[t].html_function_builtin);
            fprintf(out, ".theme-%s .function { color: %s; }\n", themes[t].name, themes[t].html_function);
            fprintf(out, ".theme-%s .string { color: %s; }\n", themes[t].name, themes[t].html_string);
            fprintf(out, ".theme-%s .comment { color: %s; font-style: italic; }\n", themes[t].name, themes[t].html_comment);
            fprintf(out, ".theme-%s .keyword { color: %s; }\n", themes[t].name, themes[t].html_keyword);
            fprintf(out, ".theme-%s .keyword-control { color: %s; font-weight: bold; }\n", themes[t].name, themes[t].html_keyword_control);
            fprintf(out, ".theme-%s .type { color: %s; }\n", themes[t].name, themes[t].html_type);
            fprintf(out, ".theme-%s .variable { color: %s; }\n", themes[t].name, themes[t].html_variable);
            fprintf(out, ".theme-%s .constant { color: %s; }\n", themes[t].name, themes[t].html_constant);
            fprintf(out, ".theme-%s .literal { color: %s; }\n", themes[t].name, themes[t].html_literal);
            fprintf(out, ".theme-%s .line-number { color: %s; }\n", themes[t].name, themes[t].html_line_number);
        }
        
        fprintf(out, "</style>\n");

        // JavaScript for Copy-to-Clipboard and Theme Switcher
        fprintf(out, "<script>\n");
        // Function to show a temporary message box for feedback
        fprintf(out, "function showMessage(message, isError = false) {\n");
        fprintf(out, "  let msgBox = document.getElementById('copyMessageBox');\n");
        fprintf(out, "  if (!msgBox) {\n");
        fprintf(out, "    msgBox = document.createElement('div');\n");
        fprintf(out, "    msgBox.id = 'copyMessageBox';\n");
        fprintf(out, "    msgBox.style.cssText = 'position: fixed; top: 20px; right: 20px; padding: 10px 20px; background-color: #333; color: white; border-radius: 5px; z-index: 1000; opacity: 0; transition: opacity 0.5s ease-in-out;';\n");
        fprintf(out, "    document.body.appendChild(msgBox);\n");
        fprintf(out, "  }\n");
        fprintf(out, "  msgBox.textContent = message;\n");
        fprintf(out, "  msgBox.style.backgroundColor = isError ? '#dc3545' : '#28a745'; // Red for error, green for success\n");
        fprintf(out, "  msgBox.style.opacity = '1';\n");
        fprintf(out, "  setTimeout(() => {\n");
        fprintf(out, "    msgBox.style.opacity = '0';\n");
        fprintf(out, "  }, 2000);\n");
        fprintf(out, "}\n");
        fprintf(out, "\n");
        // Function to copy code, prioritizing modern Clipboard API
        fprintf(out, "function copyCode() {\n");
        fprintf(out, "  const codeElement = document.getElementById('code-content');\n");
        fprintf(out, "  if (codeElement) {\n");
        fprintf(out, "    const textToCopy = Array.from(codeElement.children)\n");
        fprintf(out, "      .map(lineDiv => {\n");
        fprintf(out, "        const codeContentSpan = lineDiv.querySelector('.code-line-content');\n");
        fprintf(out, "        return codeContentSpan ? codeContentSpan.textContent : '';\n");
        fprintf(out, "      })\n");
        fprintf(out, "      .join('\\n');\n");
        fprintf(out, "\n");
        fprintf(out, "    if (navigator.clipboard && navigator.clipboard.writeText) {\n");
        fprintf(out, "      navigator.clipboard.writeText(textToCopy)\n");
        fprintf(out, "        .then(() => {\n");
        fprintf(out, "          showMessage('Code copied to clipboard!');\n");
        fprintf(out, "        })\n");
        fprintf(out, "        .catch(err => {\n");
        fprintf(out, "          console.error('Failed to copy code (Clipboard API): ', err);\n");
        fprintf(out, "          showMessage('Failed to copy code. Please try manually.', true);\n");
        fprintf(out, "        });\n");
        fprintf(out, "    } else {\n");
        fprintf(out, "      // Fallback for older browsers or restricted environments (less reliable)\n");
        fprintf(out, "      const tempTextArea = document.createElement('textarea');\n");
        fprintf(out, "      tempTextArea.value = textToCopy;\n");
        fprintf(out, "      document.body.appendChild(tempTextArea);\n");
        fprintf(out, "      tempTextArea.select();\n");
        fprintf(out, "      try {\n");
        fprintf(out, "        const successful = document.execCommand('copy');\n");
        fprintf(out, "        if (successful) {\n");
        fprintf(out, "          showMessage('Code copied (fallback)!');\n");
        fprintf(out, "        } else {\n");
        fprintf(out, "          showMessage('Failed to copy code. Manual copy required.', true);\n");
        fprintf(out, "        }\n");
        fprintf(out, "      } catch (err) {\n");
        fprintf(out, "        console.error('Failed to copy code (execCommand): ', err);\n");
        fprintf(out, "        showMessage('Failed to copy code. Manual copy required.', true);\n");
        fprintf(out, "      }\n");
        fprintf(out, "      document.body.removeChild(tempTextArea);\n");
        fprintf(out, "    }\n");
        fprintf(out, "  }\n");
        fprintf(out, "}\n");
        fprintf(out, "\n");
        // Function to apply selected theme to the body class
        fprintf(out, "function applyTheme(themeName) {\n");
        fprintf(out, "  document.body.className = 'theme-' + themeName;\n");
        fprintf(out, "  localStorage.setItem('selectedTheme', themeName);\n");
        fprintf(out, "}\n");
        fprintf(out, "\n");
        // Event listener to apply saved theme on DOM load
        fprintf(out, "document.addEventListener('DOMContentLoaded', () => {\n");
        fprintf(out, "  const savedTheme = localStorage.getItem('selectedTheme');\n");
        fprintf(out, "  if (savedTheme) {\n");
        fprintf(out, "    applyTheme(savedTheme);\n");
        fprintf(out, "  } else {\n");
        fprintf(out, "    applyTheme('%s'); // Apply default theme on first load\n", selected_theme->name);
        fprintf(out, "  }\n");
        fprintf(out, "});\n");
        fprintf(out, "</script>\n");
        fprintf(out, "</head>\n");

        fprintf(out, "<body class=\"theme-%s\">\n", selected_theme->name);
        fprintf(out, "<div>\n"); // Controls container
        fprintf(out, "  <button onclick=\"copyCode()\" style=\"margin-right: 10px; padding: 8px 15px;\">Copy Code</button>\n");
        fprintf(out, "  <label for=\"theme-select\">Theme:</label>\n");
        fprintf(out, "  <select id=\"theme-select\" onchange=\"applyTheme(this.value)\" style=\"padding: 8px; border-radius: 4px;\">\n");
        for (size_t t = 0; t < THEMES_COUNT; t++) {
            fprintf(out, "    <option value=\"%s\"%s>%s</option>\n", 
                            themes[t].name, 
                            (strcmp(themes[t].name, selected_theme->name) == 0 ? " selected" : ""),
                            themes[t].name);
        }
        fprintf(out, "  </select>\n");
        fprintf(out, "</div>\n");
        fprintf(out, "<br>\n");

        fprintf(out, "<div class=\"code-container\">\n");
        fprintf(out, "<pre><code id=\"code-content\">");

        // Initial line div for the very first line if line numbers are enabled
        if (show_line_numbers) {
            fprintf(out, "<div class=\"line\" id=\"L%u\">", current_line_num);
            fprintf(out, "<span class=\"line-number\">%*u</span>", line_num_padding, current_line_num);
            fprintf(out, "<span class=\"code-line-content\">");
            at_line_start = false; // Reset to false after printing first line number
        }
    }

    while (ts_query_cursor_next_match(cursor, &match)) {
        TSQueryCapture sorted_captures[match.capture_count];
        memcpy(sorted_captures, match.captures, match.capture_count * sizeof(TSQueryCapture));

        for (uint32_t i = 0; i < match.capture_count; i++) {
            for (uint32_t j = i + 1; j < match.capture_count; j++) {
                if (ts_node_start_byte(sorted_captures[j].node) < ts_node_start_byte(sorted_captures[i].node)) {
                    TSQueryCapture temp = sorted_captures[i];
                    sorted_captures[i] = sorted_captures[j];
                    sorted_captures[j] = temp;
                }
            }
        }

        for (uint32_t i = 0; i < match.capture_count; i++) {
            TSQueryCapture capture = sorted_captures[i];

            uint32_t name_len;
            const char *name = ts_query_capture_name_for_id(query, capture.index, &name_len);
            if (!name) continue;

            TSNode node = capture.node;
            uint32_t start = ts_node_start_byte(node);
            uint32_t end = ts_node_end_byte(node);

            if (start < current_byte) continue;
            if (start >= end || end > code_size) continue;

            print_code_section(out, code, current_byte, start, show_line_numbers, line_num_padding, &current_line_num, &at_line_start, output_html);

            if (output_html) {
                const char *cls = get_html_class(name);
                if (cls) fprintf(out, "<span class=\"%s\">", cls);
                print_code_section(out, code, start, end, show_line_numbers, line_num_padding, &current_line_num, &at_line_start, output_html);
                if (cls) fprintf(out, "</span>");
            } else {
                fprintf(out, "%s", get_ansi_color(name));
                print_code_section(out, code, start, end, show_line_numbers, line_num_padding, &current_line_num, &at_line_start, output_html);
                fprintf(out, "%s", selected_theme->ansi_reset);
            }

            current_byte = end;
        }
    }

    if (current_byte < code_size) {
        print_code_section(out, code, current_byte, code_size, show_line_numbers, line_num_padding, &current_line_num, &at_line_start, output_html);
    }
    
    if (output_html && show_line_numbers) {
        if (!at_line_start || current_line_num == 1) { // Current_line_num == 1 implies it was possibly a single-line file
            fprintf(out, "</span></div>\n");
        }
    }

    if (output_html) {
        fprintf(out, "</code></pre>\n");
        fprintf(out, "</div>\n");
        fprintf(out, "</body></html>\n");
    }

    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
    ts_tree_delete(tree);
    ts_parser_delete(parser);
    free(code);
    free(query_str);

    if (output_file) fclose(out);

    return 0;
}
