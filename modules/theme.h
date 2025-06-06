#ifndef THEME_H
#define THEME_H

#include <stdio.h>

// The structure that defines a color theme's properties.
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
    const char *line_number;
    const char *reset;
} ColorTheme;

// --- Public Function Declarations ---

// Retrieves a theme by its name. Returns NULL if not found.
ColorTheme* theme_get_by_name(const char *name);

// Gets the ANSI color code for a given capture name from a specific theme.
const char* theme_get_ansi_color(const ColorTheme *theme, const char *capture_name);

// Gets the corresponding HTML class for a capture name.
const char* theme_get_html_class(const char *capture_name);

// Prints a comma-separated list of available theme names.
void theme_print_available(FILE *stream);

// Gets the total number of available themes.
size_t theme_get_count(void);

#endif // THEME_H
