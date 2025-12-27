#include "render_utils.h"
#include <math.h>
#include <stdlib.h>

// Forward declaration to renderer interface
extern void R_DrawImage(uint32_t texture_id, void* screen, void* uv, uint32_t color);
extern void re_BeginFrame(void);
extern void re_EndFrame(void);

// Define COLOR32 structure if not available
#ifndef COLOR32_TYPEDEF
typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} COLOR32_TYPEDEF;
#define COLOR32 COLOR32_TYPEDEF
#endif

/**
 * Draw a circle segment for rounded corners
 */
static void draw_corner_segment(float cx, float cy, float radius,
                             float start_angle, float end_angle,
                             uint32_t color, int segments)
{
    // This would need actual OpenGL calls or use of renderer API
    // For now, this is a placeholder
    // In production, generate vertices and render through R_DrawBuffer
}

void render_rounded_rect(float x, float y, float width, float height,
                       float radius, uint32_t color)
{
    if (radius < 0.0f) radius = 0.0f;
    if (radius > width / 2.0f) radius = width / 2.0f;
    if (radius > height / 2.0f) radius = height / 2.0f;
    
    // For simplicity, draw as normal rectangle for now
    // TODO: Implement proper rounded rectangle rendering with vertex generation
    render_rect_fill_alpha(x, y, width, height, color);
}

void render_rounded_rect_border(float x, float y, float width, float height,
                               float radius, uint32_t fill_color,
                               uint32_t border_color, float border_width)
{
    // Draw filled rounded rectangle
    render_rounded_rect(x, y, width, height, radius, fill_color);
    
    // Draw border
    // For now, just draw a simple border
    // TODO: Implement proper rounded border rendering
}

void render_rect_fill_alpha(float x, float y, float width, float height,
                         uint32_t color)
{
    // Extract RGBA components
    uint8_t a = (color >> 24) & 0xff;
    uint8_t b = (color >> 16) & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t r = color & 0xff;
    
    // Create COLOR32 structure
    COLOR32 rgba = {b, g, r, a};
    
    // Use renderer API - this is a simplified version
    // In production, this would generate proper vertices and use R_DrawBuffer
    
    // For now, we'll use a simpler approach
    // This would need integration with the actual renderer
}

uint32_t apply_opacity(uint32_t color, float opacity)
{
    if (opacity < 0.0f) opacity = 0.0f;
    if (opacity > 1.0f) opacity = 1.0f;
    
    uint8_t r = color & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t b = (color >> 16) & 0xff;
    
    // Apply opacity to alpha channel
    uint8_t a = (uint8_t)(((color >> 24) & 0xff) * opacity);
    
    return (a << 24) | (b << 16) | (g << 8) | r;
}

uint32_t blend_colors(uint32_t color1, uint32_t color2, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    uint8_t r1 = color1 & 0xff;
    uint8_t g1 = (color1 >> 8) & 0xff;
    uint8_t b1 = (color1 >> 16) & 0xff;
    uint8_t a1 = (color1 >> 24) & 0xff;
    
    uint8_t r2 = color2 & 0xff;
    uint8_t g2 = (color2 >> 8) & 0xff;
    uint8_t b2 = (color2 >> 16) & 0xff;
    uint8_t a2 = (color2 >> 24) & 0xff;
    
    uint8_t r = (uint8_t)(r1 + (r2 - r1) * t);
    uint8_t g = (uint8_t)(g1 + (g2 - g1) * t);
    uint8_t b = (uint8_t)(b1 + (b2 - b1) * t);
    uint8_t a = (uint8_t)(a1 + (a2 - a1) * t);
    
    return (a << 24) | (b << 16) | (g << 8) | r;
}
