#ifndef LIBCODEIMAGE_H
#define LIBCODEIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

// Function to generate a code image from a file.
// All string arguments should be null-terminated.
// font_name: The friendly name of the font (e.g., "JetBrainsMono-Regular").
// font_size: The desired font height in pixels.
// img_width, img_height: Desired image dimensions. Pass 0 for auto-calculation.
// Returns 0 on success, 1 on failure.
int code_to_image_generate(
    const char *input_file_path,
    const char *output_image_path,
    const char *font_name,
    float font_size,
    int img_width,
    int img_height
);

#ifdef __cplusplus
}
#endif

#endif // LIBCODEIMAGE_H
