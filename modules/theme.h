#ifndef THEME_H
#define THEME_H

#include <stdbool.h> // For bool type
#include <stddef.h>

// Color themes
typedef struct {
    const char *name;

    // ANSI escape codes for terminal output
    const char *ansi_function_builtin;
    const char *ansi_function;
    const char *ansi_string;
    const char *ansi_comment;
    const char *ansi_keyword;
    const char *ansi_keyword_control;
    const char *ansi_type;
    const char *ansi_variable;
    const char *ansi_constant;
    const char *ansi_literal;
    const char *ansi_line_number;
    const char *ansi_reset;

    // HTML (hex) color codes for web output
    const char *html_function_builtin;
    const char *html_function;
    const char *html_string;
    const char *html_comment;
    const char *html_keyword;
    const char *html_keyword_control;
    const char *html_type;
    const char *html_variable;
    const char *html_constant;
    const char *html_literal;
    const char *html_line_number;

} ColorTheme;

// Extern declaration for the themes array and THEMES_COUNT
extern ColorTheme themes[];
extern const size_t THEMES_COUNT;

// Global selected theme pointer
extern ColorTheme *selected_theme;

// Function declarations
const char *get_ansi_color(const char *capture_name); // Still returns ANSI
const char *get_html_class(const char *capture_name); // Still returns HTML class names
// NEW: Function to get HTML color directly from the selected theme
const char *get_html_color(const char *capture_name);
bool set_selected_theme(const char *theme_name);

#endif // THEME_H
