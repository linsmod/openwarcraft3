// example.c
#include "canvas2d.h"
#include "canvas2d_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 全局变量定义
canvas2d_t *g_canvas2d_test = NULL;
int g_test_frame_count = 0;

// Example program demonstrating Canvas 2D API usage
void draw_basic_shapes(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    // Clear the canvas
    canvas2d_clear_rect(ctx, 0, 0, canvas->width, canvas->height);

    // Draw a filled rectangle
    canvas2d_set_fill_style(ctx, (COLOR32){255, 0, 0, 255}); // Red
    canvas2d_fill_rect(ctx, 50, 50, 100, 80);

    // Draw a stroked rectangle
    canvas2d_set_stroke_style(ctx, (COLOR32){0, 255, 0, 255}); // Green
    canvas2d_set_line_width(ctx, 3.0f);
    canvas2d_stroke_rect(ctx, 200, 50, 100, 80);

    // Draw a filled circle
    canvas2d_set_fill_style(ctx, (COLOR32){0, 0, 255, 255}); // Blue
    canvas2d_fill_circle(ctx, 400, 90, 40);

    // Draw a stroked circle
    canvas2d_set_stroke_style(ctx, (COLOR32){255, 255, 0, 255}); // Yellow
    canvas2d_stroke_circle(ctx, 550, 90, 40);
}

void draw_transformed_shapes(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    // Save the current state
    canvas2d_save(ctx);

    // Translate and rotate
    canvas2d_translate(ctx, 200, 200);
    canvas2d_rotate(ctx, M_PI / 4); // 45 degrees
    canvas2d_set_fill_style(ctx, (COLOR32){128, 128, 128, 255}); // Gray
    canvas2d_fill_rect(ctx, 0, 0, 80, 60);

    // Restore the state
    canvas2d_restore(ctx);

    // Draw a scaled rectangle
    canvas2d_save(ctx);
    canvas2d_translate(ctx, 400, 200);
    canvas2d_scale(ctx, 1.5f, 0.5f);
    canvas2d_set_fill_style(ctx, (COLOR32){204, 51, 51, 255}); // Dark red
    canvas2d_fill_rect(ctx, -40, -30, 80, 60);
    canvas2d_restore(ctx);
}

void draw_text_example(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    // Draw filled text
    canvas2d_set_fill_style(ctx, (COLOR32){0, 0, 0, 255}); // Black
    canvas2d_fill_text(ctx, "Hello Canvas 2D!", 50, 300);

    // Draw stroked text
    canvas2d_set_stroke_style(ctx, (COLOR32){255, 0, 0, 255}); // Red
    canvas2d_set_line_width(ctx, 2.0f);
    canvas2d_stroke_text(ctx, "Stroked Text", 50, 350);

    // Draw more text
    canvas2d_set_fill_style(ctx, (COLOR32){0, 0, 255, 255}); // Blue
    canvas2d_fill_text(ctx, "Canvas 2D API", 50, 400);
}

// 初始化canvas2d测试
int canvas2d_init_test() {
    printf("Initializing Canvas 2D Test...\n");
    
    // 创建画布
    g_canvas2d_test = canvas2d_create(800, 600);
    if (!g_canvas2d_test) {
        printf("Failed to create canvas\n");
        return 1;
    }

    printf("Canvas created successfully\n");
    g_test_frame_count = 0;
    return 0;
}


// 渲染canvas2d测试到屏幕
void draw_test_tips_canvas(canvas2d_context_t *ctx) {
    if (!ctx) return;
    
    // 绘制半透明背景
    canvas2d_set_fill_style(ctx, (COLOR32){64, 64, 64, 200});
    canvas2d_fill_rect(ctx, 400, 20, 400, 300);
    
    // 绘制标题
    canvas2d_set_fill_style(ctx, (COLOR32){255, 255, 255, 255});
    char title[64];
    sprintf(title, "Canvas 2D - Frame %d", g_test_frame_count);
    canvas2d_fill_text(ctx, title, 410, 30);
}

// 更新canvas2d测试内容
void canvas2d_test_render_frame() {
    if (!g_canvas2d_test) return;

    g_test_frame_count++;

    draw_test_tips_canvas(g_canvas2d_test->context);

    canvas2d_context_t *ctx = canvas2d_get_context(g_canvas2d_test);
    if (!ctx) return;

    // 清除画布
    canvas2d_clear_rect(ctx, 0, 0, g_canvas2d_test->width, g_canvas2d_test->height);

    // 添加动画效果
    float time_factor = g_test_frame_count * 0.01f;
    
    // 绘制背景渐变
    for (int y = 0; y < g_canvas2d_test->height; y += 20) {
        float t = (float)y / g_canvas2d_test->height;
        COLOR32 bg_color = {
            (BYTE)(64 + 64 * sin(time_factor + t * 2)),
            (BYTE)(64 + 64 * cos(time_factor + t * 3)),
            (BYTE)(128 + 64 * sin(time_factor + t * 4)),
            255
        };
        canvas2d_set_fill_style(ctx, bg_color);
        canvas2d_fill_rect(ctx, 0, y, g_canvas2d_test->width, 20);
    }

    // 绘制旋转的圆形
    canvas2d_save(ctx);
    canvas2d_translate(ctx, 400, 200);
    canvas2d_rotate(ctx, time_factor);
    COLOR32 circle_color = {
        (BYTE)(128 + 127 * sin(time_factor)),
        51,
        (BYTE)(128 + 127 * cos(time_factor)),
        255
    };
    canvas2d_set_fill_style(ctx, circle_color);
    canvas2d_fill_circle(ctx, 0, 0, 30);
    canvas2d_restore(ctx);

    // 绘制移动的矩形
    COLOR32 rect_color = {204, 102, 51, 255};
    canvas2d_set_fill_style(ctx, rect_color);
    float rect_x = 100 + 50 * sin(time_factor * 0.5f);
    canvas2d_fill_rect(ctx, rect_x, 400, 80, 60);

    // 绘制动态文本
    canvas2d_set_fill_style(ctx, (COLOR32){255, 255, 255, 255});
    char text_buffer[64];
    sprintf(text_buffer, "Frame: %d", g_test_frame_count);
    canvas2d_fill_text(ctx, text_buffer, 20, 30);
    
    canvas2d_fill_text(ctx, "Canvas 2D Demo", 20, 60);

    // 绘制缩放效果
    float scale = 1.0f + 0.3f * sin(time_factor * 0.8f);
    canvas2d_save(ctx);
    canvas2d_translate(ctx, 600, 400);
    canvas2d_scale(ctx, scale, scale);
    canvas2d_set_stroke_style(ctx, (COLOR32){255, 0, 255, 255});
    canvas2d_set_line_width(ctx, 3.0f);
    canvas2d_stroke_rect(ctx, -25, -25, 50, 50);
    canvas2d_restore(ctx);

    // 绘制一些静态示例
    draw_basic_shapes(g_canvas2d_test);
    draw_transformed_shapes(g_canvas2d_test);
    draw_text_example(g_canvas2d_test);
}


// 清理canvas2d测试
void canvas2d_cleanup_test() {
    if (g_canvas2d_test) {
        printf("Cleaning up Canvas 2D Test...\n");
        canvas2d_destroy(g_canvas2d_test);
        g_canvas2d_test = NULL;
        g_test_frame_count = 0;
    }
}