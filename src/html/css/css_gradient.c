#include "css_gradient.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

/**
 * Parse hex color string to RGBA
 * Supports #RGB, #RRGGBB, and named colors (basic set)
 */
static bool parse_color(const char *color_str, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (!color_str || !r || !g || !b || !a) return false;
    
    *a = 255; // Default alpha
    
    // Skip whitespace
    while (*color_str && isspace(*color_str)) color_str++;
    
    // Handle hex colors
    if (color_str[0] == '#') {
        const char *hex = color_str + 1;
        int len = strlen(hex);
        
        // #RGB format
        if (len == 3) {
            unsigned int vr=0, vg=0, vb=0;
            if (sscanf(hex, "%1x%1x%1x", &vr, &vg, &vb) != 3) return false;
            *r = vr * 17;
            *g = vg * 17;
            *b = vb * 17;
            return true;
        }
        // #RRGGBB format
        else if (len == 6) {
            unsigned int val;
            if (sscanf(hex, "%06x", &val) != 1) return false;
            *r = (val >> 16) & 0xff;
            *g = (val >> 8) & 0xff;
            *b = val & 0xff;
            return true;
        }
        // #RRGGBBAA format (with alpha)
        else if (len == 8) {
            unsigned int val;
            if (sscanf(hex, "%08x", &val) != 1) return false;
            *r = (val >> 24) & 0xff;
            *g = (val >> 16) & 0xff;
            *b = (val >> 8) & 0xff;
            *a = val & 0xff;
            return true;
        }
    }
    
    // Handle rgba() format
    if (strncmp(color_str, "rgba(", 5) == 0) {
        float fr, fg, fb, fa;
        if (sscanf(color_str + 5, "%f,%f,%f,%f)", &fr, &fg, &fb, &fa) == 4) {
            *r = (uint8_t)(fr * 255);
            *g = (uint8_t)(fg * 255);
            *b = (uint8_t)(fb * 255);
            *a = (uint8_t)(fa * 255);
            return true;
        }
    }
    
    // Handle rgb() format
    if (strncmp(color_str, "rgb(", 4) == 0) {
        int ir, ig, ib;
        if (sscanf(color_str + 4, "%d,%d,%d)", &ir, &ig, &ib) == 3) {
            *r = (uint8_t)ir;
            *g = (uint8_t)ig;
            *b = (uint8_t)ib;
            return true;
        }
    }
    
    return false;
}

/**
 * Parse percentage or pixel value to float (0.0 to 1.0 for percentage)
 */
static float parse_value(const char *value_str)
{
    if (!value_str) return 0.0f;
    
    // Remove whitespace
    while (*value_str && isspace(*value_str)) value_str++;
    
    float value = 0.0f;
    sscanf(value_str, "%f", &value);
    
    // Check for percentage
    if (strstr(value_str, "%")) {
        return value / 100.0f;
    }
    
    // Pixels - normalize to 0.0-1.0 range (assuming 100px max)
    return value / 100.0f;
}

/**
 * Parse gradient direction keyword
 */
static gradient_direction_t parse_direction(const char *dir_str)
{
    if (!dir_str) return GRAD_DIR_TO_BOTTOM;
    
    char dir[32] = {0};
    sscanf(dir_str, "%31s", dir);
    
    // Convert to lowercase for comparison
    for (int i = 0; dir[i]; i++) {
        dir[i] = tolower(dir[i]);
    }
    
    if (strstr(dir, "top")) {
        return strstr(dir, "right") ? GRAD_DIR_TO_TOP_RIGHT :
               strstr(dir, "left") ? GRAD_DIR_TO_TOP_LEFT :
               GRAD_DIR_TO_TOP;
    } else if (strstr(dir, "bottom")) {
        return strstr(dir, "right") ? GRAD_DIR_TO_BOTTOM_RIGHT :
               strstr(dir, "left") ? GRAD_DIR_TO_BOTTOM_LEFT :
               GRAD_DIR_TO_BOTTOM;
    } else if (strstr(dir, "right")) {
        return GRAD_DIR_TO_RIGHT;
    } else if (strstr(dir, "left")) {
        return GRAD_DIR_TO_LEFT;
    }
    
    return GRAD_DIR_TO_BOTTOM;
}

bool css_parse_linear_gradient(const char *gradient_str, linear_gradient_t *gradient)
{
    if (!gradient_str || !gradient) return false;
    
    memset(gradient, 0, sizeof(linear_gradient_t));
    
    // Check if it's a linear-gradient
    char *gradient_start = strstr(gradient_str, "linear-gradient(");
    if (!gradient_start) return false;
    
    const char *paren_end = strchr(gradient_start + 16, ')');
    if (!paren_end) return false;
    
    // Extract content inside parentheses
    size_t content_len = paren_end - (gradient_start + 16);
    char *content = malloc(content_len + 1);
    if (!content) return false;
    
    strncpy(content, gradient_start + 16, content_len);
    content[content_len] = '\0';
    
    // Parse direction or angle (before first comma)
    char *comma = strchr(content, ',');
    if (!comma) {
        free(content);
        return false;
    }
    
    char dir_str[64] = {0};
    strncpy(dir_str, content, comma - content < 63 ? comma - content : 63);
    
    // Check if it's an angle
    if (strstr(dir_str, "deg")) {
        gradient->direction = GRAD_DIR_ANGLE;
        sscanf(dir_str, "%f", &gradient->angle);
    } else {
        gradient->direction = parse_direction(dir_str);
    }
    
    // Parse color stops
    gradient->stop_count = 0;
    const char *stop_start = comma + 1;
    
    while (*stop_start && gradient->stop_count < 16) {
        // Skip whitespace
        while (*stop_start && isspace(*stop_start)) stop_start++;
        if (!*stop_start) break;
        
        // Find next comma or end
        const char *next_comma = strchr(stop_start, ',');
        if (!next_comma) next_comma = stop_start + strlen(stop_start);
        
        // Extract stop (color + optional offset)
        char stop_str[128] = {0};
        size_t stop_len = next_comma - stop_start < 127 ? next_comma - stop_start : 127;
        strncpy(stop_str, stop_start, stop_len);
        stop_str[stop_len] = '\0';
        
        // Parse color and offset
        char color_str[64] = {0};
        float offset = 0.0f;
        
        // Check if there's an offset (percentage or pixels)
        char *space = strrchr(stop_str, ' ');
        if (space) {
            strncpy(color_str, stop_str, space - stop_str < 63 ? space - stop_str : 63);
            color_str[space - stop_str < 63 ? space - stop_str : 63] = '\0';
            offset = parse_value(space + 1);
        } else {
            strncpy(color_str, stop_str, 63);
        }
        
        // Parse color
        uint8_t r, g, b, a;
        if (parse_color(color_str, &r, &g, &b, &a)) {
            gradient->stops[gradient->stop_count].r = r;
            gradient->stops[gradient->stop_count].g = g;
            gradient->stops[gradient->stop_count].b = b;
            gradient->stops[gradient->stop_count].a = a;
            gradient->stops[gradient->stop_count].offset = offset;
            gradient->stop_count++;
        }
        
        stop_start = next_comma + 1;
    }
    
    free(content);
    
    // Normalize stops if needed (ensure first is 0.0, last is 1.0)
    if (gradient->stop_count >= 2) {
        if (gradient->stops[0].offset != 0.0f) {
            gradient->stops[0].offset = 0.0f;
        }
        if (gradient->stops[gradient->stop_count - 1].offset != 1.0f) {
            gradient->stops[gradient->stop_count - 1].offset = 1.0f;
        }
    }
    
    return gradient->stop_count >= 2;
}

void css_gradient_get_color(const linear_gradient_t *gradient, float position,
                          uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    if (!gradient || gradient->stop_count == 0) {
        *r = *g = *b = *a = 0;
        return;
    }
    
    // Clamp position
    if (position < 0.0f) position = 0.0f;
    if (position > 1.0f) position = 1.0f;
    
    // Find the two stops to interpolate between
    int lower = 0;
    int upper = gradient->stop_count - 1;
    
    for (int i = 0; i < gradient->stop_count - 1; i++) {
        if (position >= gradient->stops[i].offset && 
            position <= gradient->stops[i + 1].offset) {
            lower = i;
            upper = i + 1;
            break;
        }
    }
    
    // If only one stop, return that color
    if (lower == upper) {
        *r = gradient->stops[lower].r;
        *g = gradient->stops[lower].g;
        *b = gradient->stops[lower].b;
        *a = gradient->stops[lower].a;
        return;
    }
    
    // Interpolate between stops
    const gradient_stop_t *lower_stop = &gradient->stops[lower];
    const gradient_stop_t *upper_stop = &gradient->stops[upper];
    
    float range = upper_stop->offset - lower_stop->offset;
    float t = range > 0.0f ? (position - lower_stop->offset) / range : 0.0f;
    
    *r = (uint8_t)(lower_stop->r + (upper_stop->r - lower_stop->r) * t);
    *g = (uint8_t)(lower_stop->g + (upper_stop->g - lower_stop->g) * t);
    *b = (uint8_t)(lower_stop->b + (upper_stop->b - lower_stop->b) * t);
    *a = (uint8_t)(lower_stop->a + (upper_stop->a - lower_stop->a) * t);
}

int css_gradient_create_texture(const linear_gradient_t *gradient,
                             int width, int height,
                             uint8_t *texture_data)
{
    if (!gradient || !texture_data || width <= 0 || height <= 0) return 0;
    
    // Determine gradient direction and sampling
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float position = 0.0f;
            
            // Calculate position based on direction
            switch (gradient->direction) {
                case GRAD_DIR_TO_BOTTOM:
                    position = (float)y / (float)(height - 1);
                    break;
                case GRAD_DIR_TO_TOP:
                    position = 1.0f - (float)y / (float)(height - 1);
                    break;
                case GRAD_DIR_TO_RIGHT:
                    position = (float)x / (float)(width - 1);
                    break;
                case GRAD_DIR_TO_LEFT:
                    position = 1.0f - (float)x / (float)(width - 1);
                    break;
                case GRAD_DIR_ANGLE: {
                    float angle_rad = gradient->angle * M_PI / 180.0f;
                    float cx = (float)width / 2.0f;
                    float cy = (float)height / 2.0f;
                    float dx = (float)x - cx;
                    float dy = (float)y - cy;
                    float max_dist = sqrtf(cx * cx + cy * cy);
                    float dist = sqrtf(dx * dx + dy * dy);
                    
                    // Project onto angle direction
                    float dir_x = cosf(angle_rad);
                    float dir_y = sinf(angle_rad);
                    float proj = dx * dir_x + dy * dir_y;
                    position = (proj / max_dist + 1.0f) / 2.0f;
                    break;
                }
            }
            
            // Clamp position
            if (position < 0.0f) position = 0.0f;
            if (position > 1.0f) position = 1.0f;
            
            // Get interpolated color
            uint8_t r, g, b, a;
            css_gradient_get_color(gradient, position, &r, &g, &b, &a);
            
            // Write to texture
            int idx = (y * width + x) * 4;
            texture_data[idx + 0] = r;
            texture_data[idx + 1] = g;
            texture_data[idx + 2] = b;
            texture_data[idx + 3] = a;
        }
    }
    
    return width * height * 4;
}
