// canvas2d.h
#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <stdbool.h>
#include "r_local.h"

typedef struct canvas2d_t canvas2d_t;
typedef struct canvas2d_context_t canvas2d_context_t;

typedef enum {
    CANVAS2D_BUTT,
    CANVAS2D_ROUND,
    CANVAS2D_SQUARE
} canvas2d_line_cap_t;

typedef enum {
    CANVAS2D_MITER_JOIN,
    CANVAS2D_ROUND_JOIN,
    CANVAS2D_BEVEL_JOIN
} canvas2d_line_join_t;

typedef enum {
    CANVAS2D_SOURCE_OVER,
    CANVAS2D_SOURCE_IN,
    CANVAS2D_SOURCE_OUT,
    CANVAS2D_SOURCE_ATOP,
    CANVAS2D_DESTINATION_OVER,
    CANVAS2D_DESTINATION_IN,
    CANVAS2D_DESTINATION_OUT,
    CANVAS2D_DESTINATION_ATOP,
    CANVAS2D_LIGHTER,
    CANVAS2D_COPY,
    CANVAS2D_XOR
} canvas2d_global_composite_operation_t;

typedef struct {
    COLOR32 strokeStyle;
    COLOR32 fillStyle;
    float lineWidth;
    canvas2d_line_cap_t lineCap;
    canvas2d_line_join_t lineJoin;
    float miterLimit;
    canvas2d_global_composite_operation_t globalCompositeOperation;
    MATRIX4 transformMatrix; // 添加变换矩阵到状态
    VECTOR2 *pathPoints;
    int pathPointsCount;
    int pathPointsCapacity;
    bool pathOpen;
} canvas2d_state_t;

struct canvas2d_t {
    int width;
    int height;
    bool initialized;
    canvas2d_context_t *context;
};

struct canvas2d_context_t {
    canvas2d_t *canvas;
    canvas2d_state_t state;
    canvas2d_state_t *stateStack;
    int stateStackSize;
    int stateStackCapacity;
};

// Canvas management
canvas2d_t* canvas2d_create(int width, int height);
void canvas2d_destroy(canvas2d_t *canvas);
canvas2d_context_t* canvas2d_get_context(canvas2d_t *canvas);
void canvas2d_begin_draw(canvas2d_t *canvas);
void canvas2d_end_draw(canvas2d_t *canvas);

// Drawing operations
void canvas2d_clear_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);
void canvas2d_fill_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);
void canvas2d_stroke_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);
void canvas2d_fill_circle(canvas2d_context_t *ctx, float x, float y, float radius);
void canvas2d_stroke_circle(canvas2d_context_t *ctx, float x, float y, float radius);
void canvas2d_fill_text(canvas2d_context_t *ctx, const char *text, float x, float y);
void canvas2d_stroke_text(canvas2d_context_t *ctx, const char *text, float x, float y);

// Image operations
void canvas2d_draw_image(canvas2d_context_t *ctx, LPCTEXTURE texture, float x, float y);
void canvas2d_draw_image_scaled(canvas2d_context_t *ctx, LPCTEXTURE texture, float x, float y, float width, float height);

// Transform operations
void canvas2d_translate(canvas2d_context_t *ctx, float x, float y);
void canvas2d_rotate(canvas2d_context_t *ctx, float angle);
void canvas2d_scale(canvas2d_context_t *ctx, float x, float y);
void canvas2d_reset_transform(canvas2d_context_t *ctx);

// State management
void canvas2d_save(canvas2d_context_t *ctx);
void canvas2d_restore(canvas2d_context_t *ctx);

// Style operations
void canvas2d_set_stroke_style(canvas2d_context_t *ctx, COLOR32 color);
void canvas2d_set_fill_style(canvas2d_context_t *ctx, COLOR32 color);
void canvas2d_set_line_width(canvas2d_context_t *ctx, float width);

// Utility
const char* canvas2d_get_error_string(void);

#endif // CANVAS2D_H