#include "theme.h"
#include <string.h> // For strcmp

// Color themes definitions
ColorTheme themes[] = {
    {
        "default",
        // ANSI escape codes
        "\x1b[35m",       // ansi_function.builtin
        "\x1b[34m",       // ansi_function
        "\x1b[32m",       // ansi_string
        "\x1b[90m",       // ansi_comment
        "\x1b[31m",       // ansi_keyword
        "\x1b[31;1m",     // ansi_keyword.control
        "\x1b[36m",       // ansi_type
        "\x1b[33m",       // ansi_variable
        "\x1b[35;1m",     // ansi_constant
        "\x1b[32;1m",     // ansi_literal
        "\x1b[90m",       // ansi_line_number
        "\x1b[0m",        // ansi_reset (note: now the reset is also part of struct, so removed from main logic)

        // HTML (hex) color codes
        "#B28CFF",       // html_function.builtin
        "#66B2FF",       // html_function
        "#9CCC65",       // html_string
        "#7F7F7F",       // html_comment
        "#CC7832",       // html_keyword
        "#CC7832",       // html_keyword_control (same as keyword, or a bolder variant)
        "#DA70D6",       // html_type
        "#FFFFFF",       // html_variable
        "#9C9CFF",       // html_constant
        "#6A8759",       // html_literal
        "#7F7F7F"        // html_line_number
    },
    {
        "gruvbox",
        // ANSI escape codes
        "\x1b[38;5;142m", // ansi_function.builtin
        "\x1b[38;5;109m", // ansi_function
        "\x1b[38;5;108m", // ansi_string
        "\x1b[38;5;245m", // ansi_comment
        "\x1b[38;5;167m", // ansi_keyword
        "\x1b[38;5;167;1m",// ansi_keyword.control
        "\x1b[38;5;142m", // ansi_type
        "\x1b[38;5;223m", // ansi_variable
        "\x1b[38;5;175m", // ansi_constant
        "\x1b[38;5;208m", // ansi_literal
        "\x1b[38;5;245m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#A9B665",    // html_function.builtin
        "#89B482",    // html_function
        "#A9B665",    // html_string
        "#928374",    // html_comment
        "#FE8019",    // html_keyword
        "#FE8019",    // html_keyword.control
        "#D3869B",    // html_type
        "#EBDBB2",    // html_variable
        "#D8A657",    // html_constant
        "#FABD2F",    // html_literal
        "#928374"     // html_line_number
    },
    {
        "tokyonight-night",
        // ANSI escape codes
        "\x1b[38;5;73m",  // ansi_function.builtin
        "\x1b[38;5;110m", // ansi_function
        "\x1b[38;5;158m", // ansi_string
        "\x1b[38;5;102m", // ansi_comment
        "\x1b[38;5;175m", // ansi_keyword
        "\x1b[38;5;175;1m",// ansi_keyword.control
        "\x1b[38;5;117m", // ansi_type
        "\x1b[38;5;188m", // ansi_variable
        "\x1b[38;5;215m", // ansi_constant
        "\x1b[38;5;215m", // ansi_literal
        "\x1b[38;5;102m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#7DCFFF",    // html_function.builtin
        "#7AA87B",    // html_function
        "#9ECCBB",    // html_string
        "#565F89",    // html_comment
        "#BB9AFD",    // html_keyword
        "#BB9AFD",    // html_keyword.control
        "#73DACA",    // html_type
        "#C0CAF5",    // html_variable
        "#BB9AFD",    // html_constant
        "#FF9E64",    // html_literal
        "#565F89"     // html_line_number
    },
    {
        "tokyonight-storm",
        // ANSI escape codes
        "\x1b[38;5;73m",  // ansi_function.builtin
        "\x1b[38;5;75m",  // ansi_function
        "\x1b[38;5;114m", // ansi_string
        "\x1b[38;5;102m", // ansi_comment
        "\x1b[38;5;176m", // ansi_keyword
        "\x1b[38;5;176;1m",// ansi_keyword.control
        "\x1b[38;5;117m", // ansi_type
        "\x1b[38;5;188m", // ansi_variable
        "\x1b[38;5;215m", // ansi_constant
        "\x1b[38;5;215m", // ansi_literal
        "\x1b[38;5;102m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#7DCFFF",    // html_function.builtin
        "#7AA87B",    // html_function
        "#9ECCBB",    // html_string
        "#565F89",    // html_comment
        "#BB9AFD",    // html_keyword
        "#BB9AFD",    // html_keyword.control
        "#73DACA",    // html_type
        "#C0CAF5",    // html_variable
        "#BB9AFD",    // html_constant
        "#FF9E64",    // html_literal
        "#565F89"     // html_line_number
    },
    {
        "catppuccin-mocha",
        // ANSI escape codes
        "\x1b[38;5;117m", // ansi_function.builtin
        "\x1b[38;5;75m",  // ansi_function
        "\x1b[38;5;114m", // ansi_string
        "\x1b[38;5;102m", // ansi_comment
        "\x1b[38;5;176m", // ansi_keyword
        "\x1b[38;5;176;1m",// ansi_keyword.control
        "\x1b[38;5;117m", // ansi_type
        "\x1b[38;5;188m", // ansi_variable
        "\x1b[38;5;215m", // ansi_constant
        "\x1b[38;5;215m", // ansi_literal
        "\x1b[38;5;102m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#89B4FA",    // html_function.builtin (sky)
        "#89B4FA",    // html_function (sky)
        "#A6E3A1",    // html_string (green)
        "#6C7086",    // html_comment (subtext0)
        "#CBA6F7",    // html_keyword (mauve)
        "#CBA6F7",    // html_keyword.control
        "#F5C2E7",    // html_type (pink)
        "#CDD6F4",    // html_variable (text)
        "#F38BA8",    // html_constant (red)
        "#FAB387",    // html_literal (peach)
        "#6C7086"     // html_line_number (subtext0)
    },
    {
        "dracula",
        // ANSI escape codes
        "\x1b[38;5;141m", // ansi_function.builtin (purple)
        "\x1b[38;5;117m", // ansi_function (green)
        "\x1b[38;5;228m", // ansi_string (yellow)
        "\x1b[38;5;102m", // ansi_comment (gray)
        "\x1b[38;5;212m", // ansi_keyword (pink)
        "\x1b[38;5;212;1m",// ansi_keyword.control (pink bold)
        "\x1b[38;5;117m", // ansi_type (cyan)
        "\x1b[38;5;255m", // ansi_variable (white)
        "\x1b[38;5;141m", // ansi_constant (purple)
        "\x1b[38;5;215m", // ansi_literal (orange)
        "\x1b[38;5;102m", // ansi_line_number (gray)
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#BD93F9",    // html_function.builtin
        "#50FA7B",    // html_function
        "#F1FA8C",    // html_string
        "#6272A4",    // html_comment
        "#FF79C6",    // html_keyword
        "#FF79C6",    // html_keyword.control
        "#8BE9FD",    // html_type
        "#F8F8F2",    // html_variable
        "#BD93F9",    // html_constant
        "#FFB86C",    // html_literal
        "#6272A4"     // html_line_number
    },
    {
        "nord",
        // ANSI escape codes
        "\x1b[38;5;116m", // ansi_function.builtin
        "\x1b[38;5;81m",  // ansi_function
        "\x1b[38;5;108m", // ansi_string
        "\x1b[38;5;59m",  // ansi_comment
        "\x1b[38;5;81m",  // ansi_keyword
        "\x1b[38;5;81;1m",// ansi_keyword.control
        "\x1b[38;5;116m", // ansi_type
        "\x1b[38;5;188m", // ansi_variable
        "\x1b[38;5;180m", // ansi_constant
        "\x1b[38;5;209m", // ansi_literal
        "\x1b[38;5;59m",  // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#88C0D0",    // html_function.builtin (polar night 4)
        "#88C0D0",    // html_function (polar night 4)
        "#A3BE8C",    // html_string (frost green)
        "#4C566A",    // html_comment (polar night 3)
        "#81A1C1",    // html_keyword (frost blue)
        "#81A1C1",    // html_keyword.control
        "#8FBCBB",    // html_type (frost cyan)
        "#D8DEE9",    // html_variable (snow storm 2)
        "#B48EAD",    // html_constant (frost purple)
        "#D08770",    // html_literal (aurora red)
        "#4C566A"     // html_line_number (polar night 3)
    },
    {
        "solarized-dark",
        // ANSI escape codes
        "\x1b[38;5;37m",  // ansi_function.builtin
        "\x1b[38;5;33m",  // ansi_function
        "\x1b[38;5;37m",  // ansi_string
        "\x1b[38;5;240m", // ansi_comment
        "\x1b[38;5;64m",  // ansi_keyword
        "\x1b[38;5;64;1m",// ansi_keyword.control
        "\x1b[38;5;136m", // ansi_type
        "\x1b[38;5;254m", // ansi_variable
        "\x1b[38;5;125m", // ansi_constant
        "\x1b[38;5;166m", // ansi_literal
        "\x1b[38;5;240m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#268BD2",    // html_function.builtin
        "#268BD2",    // html_function
        "#859900",    // html_string
        "#586E75",    // html_comment
        "#CB4B16",    // html_keyword
        "#CB4B16",    // html_keyword.control
        "#B58900",    // html_type
        "#93A1A1",    // html_variable
        "#6C71C4",    // html_constant
        "#DC322F",    // html_literal
        "#586E75"     // html_line_number
    },
    {
        "solarized-light",
        // ANSI escape codes
        "\x1b[38;5;37m",  // ansi_function.builtin
        "\x1b[38;5;33m",  // ansi_function
        "\x1b[38;5;37m",  // ansi_string
        "\x1b[38;5;244m", // ansi_comment
        "\x1b[38;5;64m",  // ansi_keyword
        "\x1b[38;5;64;1m",// ansi_keyword.control
        "\x1b[38;5;136m", // ansi_type
        "\x1b[38;5;235m", // ansi_variable
        "\x1b[38;5;125m", // ansi_constant
        "\x1b[38;5;166m", // ansi_literal
        "\x1b[38;5;244m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#268BD2",    // html_function.builtin
        "#268BD2",    // html_function
        "#859900",    // html_string
        "#93A1A1",    // html_comment
        "#CB4B16",    // html_keyword
        "#CB4B16",    // html_keyword.control
        "#B58900",    // html_type
        "#586E75",    // html_variable
        "#6C71C4",    // html_constant
        "#DC322F",    // html_literal
        "#93A1A1"     // html_line_number
    },
    {
        "one-dark",
        // ANSI escape codes
        "\x1b[38;5;39m",  // ansi_function.builtin
        "\x1b[38;5;75m",  // ansi_function
        "\x1b[38;5;114m", // ansi_string
        "\x1b[38;5;59m",  // ansi_comment
        "\x1b[38;5;176m", // ansi_keyword
        "\x1b[38;5;176;1m",// ansi_keyword.control
        "\x1b[38;5;39m",  // ansi_type
        "\x1b[38;5;145m", // ansi_variable
        "\x1b[38;5;215m", // ansi_constant
        "\x1b[38;5;215m", // ansi_literal
        "\x1b[38;5;59m",  // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#61AFEF",    // html_function.builtin
        "#61AFEF",    // html_function
        "#98C379",    // html_string
        "#5C6370",    // html_comment
        "#C678DD",    // html_keyword
        "#C678DD",    // html_keyword.control
        "#E5C07B",    // html_type
        "#ABB2BF",    // html_variable
        "#D19A66",    // html_constant
        "#E06C75",    // html_literal
        "#5C6370"     // html_line_number
    },
    {
        "monokai",
        // ANSI escape codes
        "\x1b[38;5;81m",  // ansi_function.builtin
        "\x1b[38;5;148m", // ansi_function
        "\x1b[38;5;186m", // ansi_string
        "\x1b[38;5;102m", // ansi_comment
        "\x1b[38;5;197m", // ansi_keyword
        "\x1b[38;5;197;1m",// ansi_keyword.control
        "\x1b[38;5;81m",  // ansi_type
        "\x1b[38;5;255m", // ansi_variable
        "\x1b[38;5;141m", // ansi_constant
        "\x1b[38;5;208m", // ansi_literal
        "\x1b[38;5;102m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#A6E22E",    // html_function.builtin
        "#A6E22E",    // html_function
        "#E6DB74",    // html_string
        "#75715E",    // html_comment
        "#F92672",    // html_keyword
        "#F92672",    // html_keyword.control
        "#66D9EF",    // html_type
        "#F8F8F2",    // html_variable
        "#AE81FF",    // html_constant
        "#FD971F",    // html_literal
        "#75715E"     // html_line_number
    },
    {
        "github-dark",
        // ANSI escape codes
        "\x1b[38;5;117m", // ansi_function.builtin
        "\x1b[38;5;183m", // ansi_function
        "\x1b[38;5;150m", // ansi_string
        "\x1b[38;5;102m", // ansi_comment
        "\x1b[38;5;204m", // ansi_keyword
        "\x1b[38;5;204;1m",// ansi_keyword.control
        "\x1b[38;5;117m", // ansi_type
        "\x1b[38;5;188m", // ansi_variable
        "\x1b[38;5;117m", // ansi_constant
        "\x1b[38;5;215m", // ansi_literal
        "\x1b[38;5;102m", // ansi_line_number
        "\x1b[0m",        // ansi_reset

        // HTML (hex) color codes
        "#88B0EF",    // html_function.builtin (blue)
        "#88B0EF",    // html_function (blue)
        "#7BB97F",    // html_string (green)
        "#6A737D",    // html_comment (gray)
        "#D19A66",    // html_keyword (orange)
        "#D19A66",    // html_keyword.control
        "#E6C07B",    // html_type (yellow)
        "#C0CAF5",    // html_variable (text)
        "#D19A66",    // html_constant (orange)
        "#F8F8F2",    // html_literal (white/light)
        "#6A737D"     // html_line_number (gray)
    }
};

const size_t THEMES_COUNT = sizeof(themes) / sizeof(ColorTheme);

// Initialize selected_theme to the default theme
ColorTheme *selected_theme = &themes[0];

// This function now returns ANSI codes from the selected theme's ANSI fields
const char *get_ansi_color(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return selected_theme->ansi_function_builtin;
    if (strcmp(capture_name, "function") == 0) return selected_theme->ansi_function;
    if (strcmp(capture_name, "function.call") == 0) return selected_theme->ansi_function;
    if (strcmp(capture_name, "string") == 0) return selected_theme->ansi_string;
    if (strcmp(capture_name, "comment") == 0) return selected_theme->ansi_comment;
    if (strcmp(capture_name, "keyword.control") == 0) return selected_theme->ansi_keyword_control;
    if (strcmp(capture_name, "keyword") == 0) return selected_theme->ansi_keyword;
    if (strcmp(capture_name, "keyword.function") == 0) return selected_theme->ansi_keyword;
    if (strcmp(capture_name, "keyword.type") == 0) return selected_theme->ansi_keyword;
    if (strcmp(capture_name, "keyword.import") == 0) return selected_theme->ansi_keyword;
    if (strcmp(capture_name, "keyword.return") == 0) return selected_theme->ansi_keyword;
    if (strcmp(capture_name, "type") == 0) return selected_theme->ansi_type;
    if (strcmp(capture_name, "variable") == 0) return selected_theme->ansi_variable;
    if (strcmp(capture_name, "constant") == 0) return selected_theme->ansi_constant;
    if (strcmp(capture_name, "constant.builtin") == 0) return selected_theme->ansi_constant;
    if (strcmp(capture_name, "literal") == 0) return selected_theme->ansi_literal;
    if (strcmp(capture_name, "number") == 0) return selected_theme->ansi_literal;
    return selected_theme->ansi_reset; // default reset
}

// This function maps capture names to HTML class names (unchanged)
const char *get_html_class(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return "function-builtin";
    if (strcmp(capture_name, "function") == 0) return "function";
    if (strcmp(capture_name, "function.call") == 0) return "function";
    if (strcmp(capture_name, "string") == 0) return "string";
    if (strcmp(capture_name, "comment") == 0) return "comment";
    if (strcmp(capture_name, "keyword.control") == 0) return "keyword-control";
    if (strcmp(capture_name, "keyword") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.function") == 0) return "keyword";
    if (strcmp(capture_name, "keyword.type") == 0) return "type"; // Changed from keyword
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

// NEW: This function returns HTML color codes from the selected theme's HTML fields
const char *get_html_color(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return selected_theme->html_function_builtin;
    if (strcmp(capture_name, "function") == 0) return selected_theme->html_function;
    if (strcmp(capture_name, "function.call") == 0) return selected_theme->html_function;
    if (strcmp(capture_name, "string") == 0) return selected_theme->html_string;
    if (strcmp(capture_name, "comment") == 0) return selected_theme->html_comment;
    if (strcmp(capture_name, "keyword.control") == 0) return selected_theme->html_keyword_control;
    if (strcmp(capture_name, "keyword") == 0) return selected_theme->html_keyword;
    if (strcmp(capture_name, "keyword.function") == 0) return selected_theme->html_keyword;
    if (strcmp(capture_name, "keyword.type") == 0) return selected_theme->html_type; // Changed from keyword
    if (strcmp(capture_name, "keyword.import") == 0) return selected_theme->html_keyword;
    if (strcmp(capture_name, "keyword.return") == 0) return selected_theme->html_keyword;
    if (strcmp(capture_name, "type") == 0) return selected_theme->html_type;
    if (strcmp(capture_name, "variable") == 0) return selected_theme->html_variable;
    if (strcmp(capture_name, "constant") == 0) return selected_theme->html_constant;
    if (strcmp(capture_name, "constant.builtin") == 0) return selected_theme->html_constant;
    if (strcmp(capture_name, "literal") == 0) return selected_theme->html_literal;
    if (strcmp(capture_name, "number") == 0) return selected_theme->html_literal;
    return NULL; // For HTML, if no specific color, we rely on parent styles or default
}


// Function to set the globally selected theme (unchanged)
bool set_selected_theme(const char *theme_name) {
    for (size_t t = 0; t < THEMES_COUNT; t++) {
        if (strcmp(themes[t].name, theme_name) == 0) {
            selected_theme = &themes[t];
            return true;
        }
    }
    return false; // Theme not found
}
