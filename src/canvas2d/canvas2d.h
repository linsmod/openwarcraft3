#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../common/shared.h"
#include "../cmath3/cmath3.h"
#include "../renderer/r_local.h"
// Canvas 2D API based on Web Canvas API

// Forward declarations
typedef struct canvas2d_t canvas2d_t;
typedef struct canvas2d_context_t canvas2d_context_t;
typedef struct canvas2d_image_t canvas2d_image_t;

// Canvas 2D API Types
typedef enum {
    CANVAS2D_FILL = 0,
    CANVAS2D_STROKE = 1,
    CANVAS2D_FILL_AND_STROKE = 2
} canvas2d_draw_mode_t;

typedef enum {
    CANVAS2D_SOURCE_OVER = 0,
    CANVAS2D_SOURCE_IN = 1,
    CANVAS2D_SOURCE_OUT = 2,
    CANVAS2D_SOURCE_ATOP = 3,
    CANVAS2D_DESTINATION_OVER = 4,
    CANVAS2D_DESTINATION_IN = 5,
    CANVAS2D_DESTINATION_OUT = 6,
    CANVAS2D_DESTINATION_ATOP = 7,
    CANVAS2D_LIGHTER = 8,
    CANVAS2D_COPY = 9,
    CANVAS2D_XOR = 10,
    CANVAS2D_MULTIPLY = 11,
    CANVAS2D_SCREEN = 12,
    CANVAS2D_OVERLAY = 13,
    CANVAS2D_DARKEN = 14,
    CANVAS2D_LIGHTEN = 15,
    CANVAS2D_COLOR_DODGE = 16,
    CANVAS2D_COLOR_BURN = 17,
    CANVAS2D_HARD_LIGHT = 18,
    CANVAS2D_SOFT_LIGHT = 19,
    CANVAS2D_DIFFERENCE = 20,
    CANVAS2D_EXCLUSION = 21,
    CANVAS2D_HUE = 22,
    CANVAS2D_SATURATION = 23,
    CANVAS2D_COLOR = 24,
    CANVAS2D_LUMINOSITY = 25
} canvas2d_global_composite_operation_t;

typedef enum {
    CANVAS2D_BUTT = 0,
    CANVAS2D_ROUND_CAP = 1,
    CANVAS2D_SQUARE_CAP = 2
} canvas2d_line_cap_t;

typedef enum {
    CANVAS2D_MITER_JOIN = 0,
    CANVAS2D_ROUND_JOIN = 1,
    CANVAS2D_BEVEL_JOIN = 2
} canvas2d_line_join_t;

typedef enum {
    CANVAS2D_REPEAT = 0,
    CANVAS2D_REPEAT_X = 1,
    CANVAS2D_REPEAT_Y = 2,
    CANVAS2D_NO_REPEAT = 3
} canvas2d_repeat_mode_t;

// Canvas 2D API Structures
typedef struct {
    float r, g, b, a;
} canvas2d_color_t;

typedef struct {
    float x, y;
} canvas2d_point_t;

typedef struct {
    float x, y, width, height;
} canvas2d_rect_t;

typedef struct {
    float x, y, radius;
} canvas2d_circle_t;

typedef struct {
    canvas2d_point_t points[3];
} canvas2d_triangle_t;

typedef struct {
    canvas2d_point_t points[4];
} canvas2d_quad_t;

typedef struct {
    canvas2d_point_t *points;
    int count;
} canvas2d_path_t;

typedef struct {
    canvas2d_color_t strokeStyle;
    canvas2d_color_t fillStyle;
    float lineWidth;
    canvas2d_line_cap_t lineCap;
    canvas2d_line_join_t lineJoin;
    float miterLimit;
    canvas2d_global_composite_operation_t globalCompositeOperation;
    canvas2d_repeat_mode_t repeatMode;
    canvas2d_image_t *patternImage;
} canvas2d_state_t;

// Canvas 2D API Functions
canvas2d_t* canvas2d_create(int width, int height);
void canvas2d_destroy(canvas2d_t *canvas);

canvas2d_context_t* canvas2d_get_context(canvas2d_t *canvas);
void canvas2d_resize(canvas2d_t *canvas, int width, int height);

// Context operations
void canvas2d_clear_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);
void canvas2d_fill_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);
void canvas2d_stroke_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);

void canvas2d_fill_circle(canvas2d_context_t *ctx, float x, float y, float radius);
void canvas2d_stroke_circle(canvas2d_context_t *ctx, float x, float y, float radius);

void canvas2d_fill_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle);
void canvas2d_stroke_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle);

void canvas2d_fill_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad);
void canvas2d_stroke_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad);

void canvas2d_fill_path(canvas2d_context_t *ctx, const canvas2d_path_t *path);
void canvas2d_stroke_path(canvas2d_context_t *ctx, const canvas2d_path_t *path);

// Text operations
void canvas2d_fill_text(canvas2d_context_t *ctx, const char *text, float x, float y);
void canvas2d_stroke_text(canvas2d_context_t *ctx, const char *text, float x, float y);
void canvas2d_set_font(canvas2d_context_t *ctx, const char *fontFamily, float fontSize);

// Image operations
canvas2d_image_t* canvas2d_create_image(const char *filename);
canvas2d_image_t* canvas2d_create_image_from_data(unsigned char *data, int width, int height, int channels);
void canvas2d_destroy_image(canvas2d_image_t *image);
void canvas2d_draw_image(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y);
void canvas2d_draw_image_scaled(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y, float width, float height);
void canvas2d_draw_image_sliced(canvas2d_context_t *ctx, canvas2d_image_t *image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);

// Transform operations
void canvas2d_translate(canvas2d_context_t *ctx, float x, float y);
void canvas2d_rotate(canvas2d_context_t *ctx, float angle);
void canvas2d_scale(canvas2d_context_t *ctx, float x, float y);
void canvas2d_reset_transform(canvas2d_context_t *ctx);
void canvas2d_save(canvas2d_context_t *ctx);
void canvas2d_restore(canvas2d_context_t *ctx);

// Style operations
void canvas2d_set_stroke_style(canvas2d_context_t *ctx, float r, float g, float b, float a);
void canvas2d_set_fill_style(canvas2d_context_t *ctx, float r, float g, float b, float a);
void canvas2d_set_line_width(canvas2d_context_t *ctx, float width);
void canvas2d_set_line_cap(canvas2d_context_t *ctx, canvas2d_line_cap_t cap);
void canvas2d_set_line_join(canvas2d_context_t *ctx, canvas2d_line_join_t join);
void canvas2d_set_global_composite_operation(canvas2d_context_t *ctx, canvas2d_global_composite_operation_t operation);

// Utility functions
const char* canvas2d_get_error_string(void);
void canvas2d_log_error(const char *message);
int canvas2d_runtest();

// Internal structures (implementation details)
struct canvas2d_t {
    int width;
    int height;
    canvas2d_context_t *context;
    LPTEXTURE texture;
    LPBUFFER buffer;
    LPFONT font;
    bool initialized;
};

struct canvas2d_context_t {
    canvas2d_t *canvas;
    canvas2d_state_t state;
    canvas2d_state_t *stateStack;
    int stateStackSize;
    int stateStackCapacity;
    bool pathStarted;
    canvas2d_path_t *currentPath;
    float transformMatrix[6];
};

struct canvas2d_image_t {
    LPTEXTURE texture;
    int width;
    int height;
    int channels;
    unsigned char *data;
    char filename[256];
    bool loaded;
};

#endif // CANVAS2D_H