// modules/theme.c
#include "theme.h"
#include <string.h> // For strcmp

// Color themes definitions
ColorTheme themes[] = {
    {
        "default",
        "\x1b[35m",       // function.builtin
        "\x1b[34m",       // function
        "\x1b[32m",       // string
        "\x1b[90m",       // comment
        "\x1b[31m",       // keyword
        "\x1b[31;1m",     // keyword.control
        "\x1b[36m",       // type
        "\x1b[33m",       // variable
        "\x1b[35;1m",     // constant
        "\x1b[32;1m",     // literal
        "\x1b[90m",       // line_number (Bright Black/Gray)
        "\x1b[0m"         // reset
    },
    {
        "gruvbox",
        "\x1b[38;5;142m", // function.builtin
        "\x1b[38;5;109m", // function
        "\x1b[38;5;108m", // string
        "\x1b[38;5;245m", // comment
        "\x1b[38;5;167m", // keyword
        "\x1b[38;5;167;1m",// keyword.control
        "\x1b[38;5;142m", // type
        "\x1b[38;5;223m", // variable
        "\x1b[38;5;175m", // constant
        "\x1b[38;5;208m", // literal
        "\x1b[38;5;245m", // line_number (Gruvbox Gray)
        "\x1b[0m"         // reset
    },
    {
        "tokyonight-night",
        "\x1b[38;5;73m",  // function.builtin
        "\x1b[38;5;110m", // function
        "\x1b[38;5;158m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;175m", // keyword
        "\x1b[38;5;175;1m",// keyword.control
        "\x1b[38;5;117m", // type
        "\x1b[38;5;188m", // variable
        "\x1b[38;5;215m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;102m", // line_number (Comment Gray)
        "\x1b[0m"         // reset
    },
    {
        "tokyonight-storm",
        "\x1b[38;5;73m",  // function.builtin
        "\x1b[38;5;75m",  // function
        "\x1b[38;5;114m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;176m", // keyword
        "\x1b[38;5;176;1m",// keyword.control
        "\x1b[38;5;117m", // type
        "\x1b[38;5;188m", // variable
        "\x1b[38;5;215m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;102m", // line_number (Comment Gray)
        "\x1b[0m"         // reset
    },
    {
        "catppuccin-mocha",
        "\x1b[38;5;117m", // function.builtin
        "\x1b[38;5;75m",  // function
        "\x1b[38;5;114m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;176m", // keyword
        "\x1b[38;5;176;1m",// keyword.control
        "\x1b[38;5;117m", // type
        "\x1b[38;5;188m", // variable
        "\x1b[38;5;215m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;102m", // line_number (Overlay1)
        "\x1b[0m"         // reset
    },
    {
        "dracula",
        "\x1b[38;5;141m", // function.builtin
        "\x1b[38;5;117m", // function
        "\x1b[38;5;228m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;212m", // keyword
        "\x1b[38;5;212;1m",// keyword.control
        "\x1b[38;5;117m", // type
        "\x1b[38;5;255m", // variable
        "\x1b[38;5;141m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;102m", // line_number (Comment Gray)
        "\x1b[0m"         // reset
    },
    {
        "nord",
        "\x1b[38;5;116m", // function.builtin
        "\x1b[38;5;81m",  // function
        "\x1b[38;5;108m", // string
        "\x1b[38;5;59m",  // comment
        "\x1b[38;5;81m",  // keyword
        "\x1b[38;5;81;1m",// keyword.control
        "\x1b[38;5;116m", // type
        "\x1b[38;5;188m", // variable
        "\x1b[38;5;180m", // constant
        "\x1b[38;5;209m", // literal
        "\x1b[38;5;59m",  // line_number (Polar Night 3)
        "\x1b[0m"         // reset
    },
    {
        "solarized-dark",
        "\x1b[38;5;37m",  // function.builtin
        "\x1b[38;5;33m",  // function
        "\x1b[38;5;37m",  // string
        "\x1b[38;5;240m", // comment
        "\x1b[38;5;64m",  // keyword
        "\x1b[38;5;64;1m",// keyword.control
        "\x1b[38;5;136m", // type
        "\x1b[38;5;254m", // variable
        "\x1b[38;5;125m", // constant
        "\x1b[38;5;166m", // literal
        "\x1b[38;5;240m", // line_number (Base01)
        "\x1b[0m"         // reset
    },
    {
        "solarized-light",
        "\x1b[38;5;37m",  // function.builtin
        "\x1b[38;5;33m",  // function
        "\x1b[38;5;37m",  // string
        "\x1b[38;5;244m", // comment
        "\x1b[38;5;64m",  // keyword
        "\x1b[38;5;64;1m",// keyword.control
        "\x1b[38;5;136m", // type
        "\x1b[38;5;235m", // variable
        "\x1b[38;5;125m", // constant
        "\x1b[38;5;166m", // literal
        "\x1b[38;5;244m", // line_number (Base1)
        "\x1b[0m"         // reset
    },
    {
        "one-dark",
        "\x1b[38;5;39m",  // function.builtin
        "\x1b[38;5;75m",  // function
        "\x1b[38;5;114m", // string
        "\x1b[38;5;59m",  // comment
        "\x1b[38;5;176m", // keyword
        "\x1b[38;5;176;1m",// keyword.control
        "\x1b[38;5;39m",  // type
        "\x1b[38;5;145m", // variable
        "\x1b[38;5;215m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;59m",  // line_number (Comment Gray)
        "\x1b[0m"         // reset
    },
    {
        "monokai",
        "\x1b[38;5;81m",  // function.builtin
        "\x1b[38;5;148m", // function
        "\x1b[38;5;186m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;197m", // keyword
        "\x1b[38;5;197;1m",// keyword.control
        "\x1b[38;5;81m",  // type
        "\x1b[38;5;255m", // variable
        "\x1b[38;5;141m", // constant
        "\x1b[38;5;208m", // literal
        "\x1b[38;5;102m", // line_number (Comment)
        "\x1b[0m"         // reset
    },
    {
        "github-dark",
        "\x1b[38;5;117m", // function.builtin
        "\x1b[38;5;183m", // function
        "\x1b[38;5;150m", // string
        "\x1b[38;5;102m", // comment
        "\x1b[38;5;204m", // keyword
        "\x1b[38;5;204;1m",// keyword.control
        "\x1b[38;5;117m", // type
        "\x1b[38;5;188m", // variable
        "\x1b[38;5;117m", // constant
        "\x1b[38;5;215m", // literal
        "\x1b[38;5;102m", // line_number (Gray)
        "\x1b[0m"         // reset
    }
};

const size_t THEMES_COUNT = sizeof(themes) / sizeof(ColorTheme);

// Initialize selected_theme to the default theme
ColorTheme *selected_theme = &themes[0];

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

// Function to set the globally selected theme
bool set_selected_theme(const char *theme_name) {
    for (size_t t = 0; t < THEMES_COUNT; t++) {
        if (strcmp(themes[t].name, theme_name) == 0) {
            selected_theme = &themes[t];
            return true;
        }
    }
    return false; // Theme not found
}
