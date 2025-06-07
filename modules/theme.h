// modules/theme.h
#ifndef THEME_H
#define THEME_H

#include <stdbool.h> // For bool type
#include <stddef.h>

// Color themes
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

// Extern declaration for the themes array and THEMES_COUNT
extern ColorTheme themes[];
extern const size_t THEMES_COUNT;

// Global selected theme pointer
extern ColorTheme *selected_theme;

// Function declarations
const char *get_ansi_color(const char *capture_name);
const char *get_html_class(const char *capture_name);
bool set_selected_theme(const char *theme_name);

#endif // THEME_H
