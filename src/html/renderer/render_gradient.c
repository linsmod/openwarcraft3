#include "render_gradient.h"
#include "../css/css_gradient.h"
#include <stdlib.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "common/shared.h"
#define MAX_CACHED_GRADIENTS 32
// Cache entry
typedef struct {
    linear_gradient_t gradient;
    uint32_t texture_id;
    int width;
    int height;
    bool used;
} gradient_cache_entry_t;

// Global cache
static gradient_cache_entry_t g_gradient_cache[MAX_CACHED_GRADIENTS];
static int g_cache_count = 0;

#define GL_TEXTURE_2D 0x0DE1
#define GL_RGBA 0x1908
#define GL_UNSIGNED_BYTE 0x1401
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F

/**
 * Generate gradient hash for cache lookup
 * Simplified hash - in production use proper hash function
 */
static uint32_t gradient_hash(const linear_gradient_t *gradient)
{
    uint32_t hash = 0;
    const uint8_t *data = (const uint8_t*)gradient;
    size_t len = sizeof(linear_gradient_t);
    
    for (size_t i = 0; i < len; i++) {
        hash = hash * 31 + data[i];
    }
    
    return hash;
}

/**
 * Check if two gradients are equal
 */
static bool gradient_equals(const linear_gradient_t *g1, const linear_gradient_t *g2)
{
    if (g1->direction != g2->direction) return false;
    if (g1->stop_count != g2->stop_count) return false;
    
    // Check angle for directional gradients
    if (g1->direction == GRAD_DIR_ANGLE) {
        if (g1->angle != g2->angle) return false;
    }
    
    // Check all stops
    for (int i = 0; i < g1->stop_count; i++) {
        if (g1->stops[i].offset != g2->stops[i].offset) return false;
        if (g1->stops[i].r != g2->stops[i].r) return false;
        if (g1->stops[i].g != g2->stops[i].g) return false;
        if (g1->stops[i].b != g2->stops[i].b) return false;
        if (g1->stops[i].a != g2->stops[i].a) return false;
    }
    
    return true;
}

uint32_t cache_gradient_texture(const linear_gradient_t *gradient,
                             int width, int height)
{
    if (!gradient || width <= 0 || height <= 0) return 0;
    
    // Check cache first
    for (int i = 0; i < g_cache_count; i++) {
        if (g_gradient_cache[i].used &&
            g_gradient_cache[i].width == width &&
            g_gradient_cache[i].height == height &&
            gradient_equals(&g_gradient_cache[i].gradient, gradient)) {
            return g_gradient_cache[i].texture_id;
        }
    }
    
    // Create new texture data
    uint8_t *texture_data = (uint8_t*)malloc(width * height * 4);
    if (!texture_data) return 0;
    
    // Generate gradient texture
    int data_size = css_gradient_create_texture(gradient, width, height, texture_data);
    if (data_size == 0) {
        free(texture_data);
        return 0;
    }
    
    // Create OpenGL texture
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    free(texture_data);
    
    // Add to cache if space available
    if (g_cache_count < MAX_CACHED_GRADIENTS) {
        memcpy(&g_gradient_cache[g_cache_count].gradient, gradient,
               sizeof(linear_gradient_t));
        g_gradient_cache[g_cache_count].texture_id = texture_id;
        g_gradient_cache[g_cache_count].width = width;
        g_gradient_cache[g_cache_count].height = height;
        g_gradient_cache[g_cache_count].used = true;
        g_cache_count++;
    }
    
    return texture_id;
}

void clear_gradient_cache(void)
{
    // In a full implementation, this would delete OpenGL textures
    // For now, just mark cache as empty
    for (int i = 0; i < MAX_CACHED_GRADIENTS; i++) {
        g_gradient_cache[i].used = false;
    }
    g_cache_count = 0;
}

uint32_t render_gradient_rect(float x, float y, float width, float height,
                          const linear_gradient_t *gradient,
                          uint32_t texture_id)
{
    if (!gradient) return 0;
    
    // Get or create gradient texture
    uint32_t tex_id = cache_gradient_texture(gradient, (int)width, (int)height);
    if (tex_id == 0) return 0;
    
    // Render the gradient as a textured rectangle
    // This would use R_DrawImage with the gradient texture
    
    return tex_id;
}
