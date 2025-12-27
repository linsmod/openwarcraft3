#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <stdint.h>

/**
 * Rendering utilities for HTML/CSS elements
 */

/**
 * Draw a rounded rectangle
 * @param x, y Top-left position
 * @param width, height Dimensions
 * @param radius Corner radius (same for all corners)
 * @param color Fill color
 */
void render_rounded_rect(float x, float y, float width, float height,
                       float radius, uint32_t color);

/**
 * Draw a rounded rectangle with border
 * @param x, y Top-left position
 * @param width, height Dimensions
 * @param radius Corner radius
 * @param fill_color Fill color
 * @param border_color Border color
 * @param border_width Border thickness
 */
void render_rounded_rect_border(float x, float y, float width, float height,
                               float radius, uint32_t fill_color,
                               uint32_t border_color, float border_width);

/**
 * Draw a filled rectangle with alpha blending
 * @param x, y Top-left position
 * @param width, height Dimensions
 * @param color Color with alpha (0xAABBGGRR format)
 */
void render_rect_fill_alpha(float x, float y, float width, float height,
                         uint32_t color);

/**
 * Apply opacity to a color
 * @param color Original color (0xAABBGGRR format)
 * @param opacity Opacity value (0.0 to 1.0)
 * @return Color with applied alpha
 */
uint32_t apply_opacity(uint32_t color, float opacity);

/**
 * Blend two colors
 * @param color1, color2 Colors to blend (0xAABBGGRR format)
 * @param t Blend factor (0.0 = color1, 1.0 = color2)
 * @return Blended color
 */
uint32_t blend_colors(uint32_t color1, uint32_t color2, float t);

#endif // RENDER_UTILS_H
