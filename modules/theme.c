#include <string.h>
#include "theme.h"

static ColorTheme themes[] = {
    {
        "default", "\x1b[35m", "\x1b[34m", "\x1b[32m", "\x1b[90m", "\x1b[31m", "\x1b[31;1m]",
        "\x1b[36m", "\x1b[33m", "\x1b[35;1m]", "\x1b[32;1m]", "\x1b[90m", "\x1b[0m"
    },
    {
        "gruvbox", "\x1b[38;5;142m", "\x1b[38;5;109m", "\x1b[38;5;108m", "\x1b[38;5;245m",
        "\x1b[38;5;167m", "\x1b[38;5;167;1m", "\x1b[38;5;142m", "\x1b[38;5;223m", "\x1b[38;5;175m]",
        "\x1b[38;5;208m", "\x1b[38;5;245m", "\x1b[0m"
    },
    {
        "tokyonight-night", "\x1b[38;5;73m", "\x1b[38;5;110m", "\x1b[38;5;158m", "\x1b[38;5;102m]",
        "\x1b[38;5;175m", "\x1b[38;5;175;1m", "\x1b[38;5;117m", "\x1b[38;5;188m", "\x1b[38;5;215m]",
        "\x1b[38;5;215m", "\x1b[38;5;102m", "\x1b[0m"
    },
    {
        "tokyonight-storm", "\x1b[38;5;73m", "\x1b[38;5;75m", "\x1b[38;5;114m", "\x1b[38;5;102m]",
        "\x1b[38;5;176m", "\x1b[38;5;176;1m", "\x1b[38;5;117m", "\x1b[38;5;188m", "\x1b[38;5;215m]",
        "\x1b[38;5;215m", "\x1b[38;5;102m", "\x1b[0m"
    },
    {
        "catppuccin-mocha", "\x1b[38;5;117m", "\x1b[38;5;75m", "\x1b[38;5;114m", "\x1b[38;5;102m]",
        "\x1b[38;5;176m", "\x1b[38;5;176;1m", "\x1b[38;5;117m", "\x1b[38;5;188m", "\x1b[38;5;215m]",
        "\x1b[38;5;215m", "\x1b[38;5;102m", "\x1b[0m"
    },
    {
        "dracula", "\x1b[38;5;141m", "\x1b[38;5;117m", "\x1b[38;5;228m", "\x1b[38;5;102m]",
        "\x1b[38;5;212m", "\x1b[38;5;212;1m", "\x1b[38;5;117m", "\x1b[38;5;255m", "\x1b[38;5;141m]",
        "\x1b[38;5;215m", "\x1b[38;5;102m", "\x1b[0m"
    },
    {
        "nord", "\x1b[38;5;116m", "\x1b[38;5;81m", "\x1b[38;5;108m", "\x1b[38;5;59m",
        "\x1b[38;5;81m", "\x1b[38;5;81;1m", "\x1b[38;5;116m", "\x1b[38;5;188m", "\x1b[38;5;180m]",
        "\x1b[38;5;209m", "\x1b[38;5;59m", "\x1b[0m"
    },
    {
        "solarized-dark", "\x1b[38;5;37m", "\x1b[38;5;33m", "\x1b[38;5;37m", "\x1b[38;5;240m]",
        "\x1b[38;5;64m", "\x1b[38;5;64;1m", "\x1b[38;5;136m", "\x1b[38;5;254m", "\x1b[38;5;125m]",
        "\x1b[38;5;166m", "\x1b[38;5;240m", "\x1b[0m"
    },
    {
        "solarized-light", "\x1b[38;5;37m", "\x1b[38;5;33m", "\x1b[38;5;37m", "\x1b[38;5;244m]",
        "\x1b[38;5;64m", "\x1b[38;5;64;1m", "\x1b[38;5;136m", "\x1b[38;5;235m", "\x1b[38;5;125m]",
        "\x1b[38;5;166m", "\x1b[38;5;244m", "\x1b[0m"
    },
    {
        "one-dark", "\x1b[38;5;39m", "\x1b[38;5;75m", "\x1b[38;5;114m", "\x1b[38;5;59m",
        "\x1b[38;5;176m", "\x1b[38;5;176;1m", "\x1b[38;5;39m", "\x1b[38;5;145m", "\x1b[38;5;215m]",
        "\x1b[38;5;215m", "\x1b[38;5;59m", "\x1b[0m"
    },
    {
        "monokai", "\x1b[38;5;81m", "\x1b[38;5;148m", "\x1b[38;5;186m", "\x1b[38;5;102m]",
        "\x1b[38;5;197m", "\x1b[38;5;197;1m", "\x1b[38;5;81m", "\x1b[38;5;255m", "\x1b[38;5;141m]",
        "\x1b[38;5;208m", "\x1b[38;5;102m", "\x1b[0m"
    },
    {
        "github-dark", "\x1b[38;5;117m", "\x1b[38;5;183m", "\x1b[38;5;150m", "\x1b[38;5;102m]",
        "\x1b[38;5;204m", "\x1b[38;5;204;1m", "\x1b[38;5;117m", "\x1b[38;5;188m", "\x1b[38;5;117m]",
        "\x1b[38;5;215m", "\x1b[38;5;102m", "\x1b[0m"
    }
};

static const size_t THEMES_COUNT = sizeof(themes) / sizeof(ColorTheme);

size_t theme_get_count() {
    return THEMES_COUNT;
}

ColorTheme* theme_get_by_name(const char *name) {
    for (size_t i = 0; i < THEMES_COUNT; i++) {
        if (strcmp(themes[i].name, name) == 0) {
            return &themes[i];
        }
    }
    return NULL; // Not found
}

void theme_print_available(FILE *stream) {
    for (size_t i = 0; i < THEMES_COUNT; i++) {
        fprintf(stream, "%s%s", themes[i].name, (i < THEMES_COUNT - 1) ? ", " : "");
    }
}

const char* theme_get_ansi_color(const ColorTheme *theme, const char *capture_name) {
    if (!theme) return "\x1b[0m"; // Safety check

    if (strcmp(capture_name, "function.builtin") == 0) return theme->function_builtin;
    if (strcmp(capture_name, "function") == 0) return theme->function;
    if (strcmp(capture_name, "function.call") == 0) return theme->function;
    if (strcmp(capture_name, "string") == 0) return theme->string;
    if (strcmp(capture_name, "comment") == 0) return theme->comment;
    if (strcmp(capture_name, "keyword.control") == 0) return theme->keyword_control;
    if (strcmp(capture_name, "keyword") == 0) return theme->keyword;
    if (strcmp(capture_name, "keyword.function") == 0) return theme->keyword;
    if (strcmp(capture_name, "keyword.type") == 0) return theme->keyword;
    if (strcmp(capture_name, "keyword.import") == 0) return theme->keyword;
    if (strcmp(capture_name, "keyword.return") == 0) return theme->keyword;
    if (strcmp(capture_name, "type") == 0) return theme->type;
    if (strcmp(capture_name, "variable") == 0) return theme->variable;
    if (strcmp(capture_name, "constant") == 0) return theme->constant;
    if (strcmp(capture_name, "constant.builtin") == 0) return theme->constant;
    if (strcmp(capture_name, "literal") == 0) return theme->literal;
    if (strcmp(capture_name, "number") == 0) return theme->literal;
    return theme->reset; // default reset
}

const char* theme_get_html_class(const char *capture_name) {
    if (strcmp(capture_name, "function.builtin") == 0) return "function-builtin";
    if (strcmp(capture_name, "function") == 0) return "function";
    if (strcmp(capture_name, "function.call") == 0) return "function";
    if (strcmp(capture_name, "string") == 0) return "string";
    if (strcmp(capture_name, "comment") == 0) return "comment";
    if (strcmp(capture_name, "keyword.control") == 0) return "keyword-control";
    if (strcmp(capture_name, "keyword") == 0) return "keyword";
    // ... add all other class mappings here ...
    if (strcmp(capture_name, "type") == 0) return "type";
    if (strcmp(capture_name, "variable") == 0) return "variable";
    if (strcmp(capture_name, "constant") == 0) return "constant";
    if (strcmp(capture_name, "literal") == 0) return "literal";
    if (strcmp(capture_name, "number") == 0) return "literal";
    return NULL;
}
