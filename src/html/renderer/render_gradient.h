#ifndef RENDER_GRADIENT_H
#define RENDER_GRADIENT_H

#include <stdint.h>
#include "../css/css_gradient.h"

/**
 * Render a gradient rectangle
 * @param x, y Top-left position
 * @param width, height Dimensions
 * @param gradient Gradient definition
 * @param texture_id OpenGL texture ID for cached gradient
 * @return New texture ID if created, texture_id if using cached, 0 on error
 */
uint32_t render_gradient_rect(float x, float y, float width, float height,
                          const linear_gradient_t *gradient,
                          uint32_t texture_id);

/**
 * Cache a gradient as a texture
 * @param gradient Gradient definition
 * @param width Gradient texture width
 * @param height Gradient texture height
 * @return OpenGL texture ID
 */
uint32_t cache_gradient_texture(const linear_gradient_t *gradient,
                             int width, int height);

/**
 * Clear gradient cache (free all cached textures)
 */
void clear_gradient_cache(void);

#endif // RENDER_GRADIENT_H
