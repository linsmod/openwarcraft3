#ifndef CSS_GRADIENT_H
#define CSS_GRADIENT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * CSS Gradient Parser and Renderer
 * Supports linear-gradient() syntax for CSS backgrounds
 */

// Gradient direction
typedef enum {
    GRAD_DIR_TO_TOP,          // to top
    GRAD_DIR_TO_TOP_RIGHT,    // to top right
    GRAD_DIR_TO_RIGHT,        // to right
    GRAD_DIR_TO_BOTTOM_RIGHT, // to bottom right
    GRAD_DIR_TO_BOTTOM,       // to bottom
    GRAD_DIR_TO_BOTTOM_LEFT,  // to bottom left
    GRAD_DIR_TO_LEFT,         // to left
    GRAD_DIR_TO_TOP_LEFT,     // to top left
    GRAD_DIR_ANGLE            // specific angle (degrees)
} gradient_direction_t;

// Gradient color stop
typedef struct {
    float offset;          // 0.0 to 1.0
    uint8_t r, g, b;     // RGB values
    uint8_t a;            // Alpha
} gradient_stop_t;

// Linear gradient definition
typedef struct {
    gradient_direction_t direction;
    float angle;           // for GRAD_DIR_ANGLE
    gradient_stop_t stops[16];  // Max 16 color stops
    int stop_count;
} linear_gradient_t;

/**
 * Parse CSS linear-gradient() syntax
 * @param gradient_str The gradient string (e.g., "linear-gradient(to bottom, #1a1a2e, #0f3460)")
 * @param gradient Output gradient structure
 * @return true if parsing succeeded, false otherwise
 */
bool css_parse_linear_gradient(const char *gradient_str, linear_gradient_t *gradient);

/**
 * Calculate gradient color at a specific position
 * @param gradient The gradient definition
 * @param position Normalized position (0.0 to 1.0)
 * @param r, g, b, a Output color values
 */
void css_gradient_get_color(const linear_gradient_t *gradient, float position, 
                          uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

/**
 * Create gradient texture for rendering
 * @param gradient The gradient definition
 * @param width Gradient texture width
 * @param height Gradient texture height
 * @param texture Output texture data (RGBA format, 4 bytes per pixel)
 * @return Size of texture data in bytes
 */
int css_gradient_create_texture(const linear_gradient_t *gradient, 
                             int width, int height, 
                             uint8_t *texture_data);

#endif // CSS_GRADIENT_H
