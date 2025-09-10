// example.c
#include "canvas2d.h"
#include "canvas2d_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局变量定义
canvas2d_t *g_canvas2d_test = NULL;
int g_test_frame_count = 0;

// Example program demonstrating Canvas 2D API usage
void draw_basic_shapes(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    // printf("Drawing basic shapes...\n");

    // Clear the canvas
    canvas2d_clear_rect(ctx, 0, 0, canvas->width, canvas->height);

    // Draw a filled rectangle
    canvas2d_set_fill_style(ctx, 1.0f, 0.0f, 0.0f, 1.0f); // Red
    canvas2d_fill_rect(ctx, 50, 50, 100, 80);

    // Draw a stroked rectangle
    canvas2d_set_stroke_style(ctx, 0.0f, 1.0f, 0.0f, 1.0f); // Green
    canvas2d_set_line_width(ctx, 3.0f);
    canvas2d_stroke_rect(ctx, 200, 50, 100, 80);

    // Draw a filled circle
    canvas2d_set_fill_style(ctx, 0.0f, 0.0f, 1.0f, 1.0f); // Blue
    canvas2d_fill_circle(ctx, 400, 90, 40);

    // Draw a stroked circle
    canvas2d_set_stroke_style(ctx, 1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    canvas2d_stroke_circle(ctx, 550, 90, 40);

    // Draw a filled triangle
    canvas2d_triangle_t triangle = {
        .points = {
            {700, 50},
            {750, 130},
            {650, 130}
        }
    };
    canvas2d_set_fill_style(ctx, 1.0f, 0.0f, 1.0f, 1.0f); // Magenta
    canvas2d_fill_triangle(ctx, &triangle);

    // Draw a stroked triangle
    canvas2d_set_stroke_style(ctx, 0.0f, 1.0f, 1.0f, 1.0f); // Cyan
    canvas2d_stroke_triangle(ctx, &triangle);
}

void draw_transformed_shapes(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing transformed shapes...\n");

    // Save the current state
    canvas2d_save(ctx);

    // Translate and rotate
    canvas2d_translate(ctx, 200, 200);
    canvas2d_rotate(ctx, M_PI / 4); // 45 degrees
    canvas2d_set_fill_style(ctx, 0.5f, 0.5f, 0.5f, 1.0f); // Gray
    canvas2d_fill_rect(ctx, 0, 0, 80, 60);

    // Restore the state
    canvas2d_restore(ctx);

    // Draw a scaled rectangle
    canvas2d_save(ctx);
    canvas2d_translate(ctx, 400, 200);
    canvas2d_scale(ctx, 1.5f, 0.5f);
    canvas2d_set_fill_style(ctx, 0.8f, 0.2f, 0.2f, 1.0f); // Dark red
    canvas2d_fill_rect(ctx, -40, -30, 80, 60);
    canvas2d_restore(ctx);
}

void draw_text_example(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing text...\n");

    // Draw filled text
    canvas2d_set_fill_style(ctx, 0.0f, 0.0f, 0.0f, 1.0f); // Black
    canvas2d_fill_text(ctx, "Hello Canvas 2D!", 50, 300);

    // Draw stroked text
    canvas2d_set_stroke_style(ctx, 1.0f, 0.0f, 0.0f, 1.0f); // Red
    canvas2d_set_line_width(ctx, 2.0f);
    canvas2d_stroke_text(ctx, "Stroked Text", 50, 350);

    // Set font and draw more text
    canvas2d_set_font(ctx, "Fonts\\dfst-m3u.ttf", 16.0f);
    canvas2d_set_fill_style(ctx, 0.0f, 0.0f, 1.0f, 1.0f); // Blue
    canvas2d_fill_text(ctx, "With custom font", 50, 400);
}

void draw_path_example(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing paths...\n");

    // Create a path (star shape)
    canvas2d_path_t star = {0};
    star.count = 10;
    star.points = (canvas2d_point_t*)malloc(star.count * sizeof(canvas2d_point_t));

    float centerX = 200;
    float centerY = 500;
    float outerRadius = 40;
    float innerRadius = 20;

    for (int i = 0; i < star.count; i++) {
        float angle = (i / (float)star.count) * 2.0f * M_PI;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        star.points[i].x = centerX + cos(angle) * radius;
        star.points[i].y = centerY + sin(angle) * radius;
    }

    // Draw filled star
    canvas2d_set_fill_style(ctx, 1.0f, 0.8f, 0.0f, 1.0f); // Gold
    canvas2d_fill_path(ctx, &star);

    // Draw stroked star
    canvas2d_set_stroke_style(ctx, 0.0f, 0.0f, 0.0f, 1.0f); // Black
    canvas2d_stroke_path(ctx, &star);

    free(star.points);
}

void draw_image_example(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing images...\n");

    // Create a simple test image (4x4 checkerboard pattern)
    unsigned char *testData = malloc(4 * 4 * 4); // 4x4 RGBA
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int idx = (y * 4 + x) * 4;
            if ((x + y) % 2 == 0) {
                // White square
                testData[idx] = 255;     // R
                testData[idx + 1] = 255; // G
                testData[idx + 2] = 255; // B
                testData[idx + 3] = 255; // A
            } else {
                // Black square
                testData[idx] = 0;       // R
                testData[idx + 1] = 0;   // G
                testData[idx + 2] = 0;   // B
                testData[idx + 3] = 255; // A
            }
        }
    }

    // Create image from data
    canvas2d_image_t *testImage = canvas2d_create_image_from_data(testData, 4, 4, 4);
    if (testImage) {
        // Draw the image at different scales and positions
        canvas2d_draw_image(ctx, testImage, 400, 450);
        canvas2d_draw_image_scaled(ctx, testImage, 450, 450, 80, 80);
        canvas2d_draw_image_sliced(ctx, testImage, 0, 0, 2, 2, 550, 450, 40, 40);
        
        // Clean up
        canvas2d_destroy_image(testImage);
    } else {
        printf("Failed to create test image\n");
    }
}

void draw_composite_operations(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing with composite operations...\n");

    // Draw base shapes
    canvas2d_set_fill_style(ctx, 1.0f, 0.0f, 0.0f, 1.0f); // Red
    canvas2d_fill_rect(ctx, 50, 600, 100, 100);

    // Draw overlapping shape with different composite operations
    canvas2d_set_global_composite_operation(ctx, CANVAS2D_SOURCE_OVER);
    canvas2d_set_fill_style(ctx, 0.0f, 0.0f, 1.0f, 1.0f); // Blue
    canvas2d_fill_circle(ctx, 100, 650, 50);

    canvas2d_set_global_composite_operation(ctx, CANVAS2D_MULTIPLY);
    canvas2d_set_fill_style(ctx, 0.0f, 1.0f, 0.0f, 1.0f); // Green
    canvas2d_fill_circle(ctx, 150, 700, 50);

    canvas2d_set_global_composite_operation(ctx, CANVAS2D_SCREEN);
    canvas2d_set_fill_style(ctx, 1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    canvas2d_fill_circle(ctx, 200, 650, 50);

    // Reset composite operation
    canvas2d_set_global_composite_operation(ctx, CANVAS2D_SOURCE_OVER);
}

#include "../common/common.h"
#include "../client/client.h"
#include "client/client.h"
#include "server/server.h"

// 初始化canvas2d测试
int canvas2d_init_test() {
    printf("Initializing Canvas 2D Test...\n");
    
    // 创建画布
    g_canvas2d_test = canvas2d_create(800, 800);
    if (!g_canvas2d_test) {
        printf("Failed to create canvas\n");
        return 1;
    }

    printf("Canvas created successfully\n");
    g_test_frame_count = 0;
    return 0;
}

void canvas2d_update_bg(){
    // 绘制canvas2d测试
    if (g_canvas2d_test && g_canvas2d_test->texture) {
        // 在屏幕显示canvas2d测试
        // 在屏幕显示canvas2d测试，调整为更合适的比例
        RECT screen_rect = {
            0.0f, // x offset to window
            0.1f,  // y 
            0.5f,  // width 
            0.5f   // height
        };
        
        // 使用深色半透明背景，让canvas2d内容更突出
        COLOR32 bg_color = {64, 64, 64, 200}; // Dark gray with higher alpha
        re.DrawImage(cl.pics[0], &screen_rect, &MAKE(RECT,0,0,1,1), bg_color);
        
        // 在canvas2d测试区域显示标题
        static char title[64];
        sprintf(title, "Canvas 2D Test - Frame %d", g_test_frame_count);
        re.PrintSysText(title, (int)(screen_rect.x * WINDOW_WIDTH), (int)(screen_rect.y * WINDOW_HEIGHT), COLOR32_WHITE);
        
        // 将canvas2d纹理渲染到屏幕上
        // 由于canvas2d纹理是800x800，我们需要调整大小以适应显示区域
        float canvas_width = 800.0f;
        float canvas_height = 800.0f;
        
        // 计算缩放比例以适应显示区域，保持宽高比
        float scale_x = screen_rect.w / (canvas_width / WINDOW_WIDTH);
        float scale_y = screen_rect.h / (canvas_height / WINDOW_HEIGHT);
        
        // 计算居中偏移
        float scaled_width = (canvas_width / WINDOW_WIDTH) * scale_x;
        float scaled_height = (canvas_height / WINDOW_HEIGHT) * scale_y;
        float offset_x = screen_rect.x + (screen_rect.w - scaled_width) / 2;
        float offset_y = screen_rect.y + 25.0f /WINDOW_HEIGHT; // 留出标题空间
        
        // 创建纹理显示区域
        RECT texture_rect = {
            offset_x,
            offset_y,
            scaled_width,
            scaled_height
        };
        
        // 渲染canvas2d纹理
        if (g_canvas2d_test->texture) {
            // 使用白色渲染纹理，这样canvas2d的内容会以原始颜色显示
            COLOR32 texture_color = {128, 255, 255, 255}; // 完全不透明的白色
            re.DrawImage(g_canvas2d_test->texture, &texture_rect, &MAKE(RECT,0,0,1,1), texture_color);
        } else {
            // 如果纹理不可用，显示提示信息
            re.PrintSysText("Canvas 2D initializing...", (int)(screen_rect.x * WINDOW_WIDTH), (int)((screen_rect.y + 40) * WINDOW_HEIGHT), COLOR32_WHITE);
        }
    }
}

// 更新canvas2d测试内容
void canvas2d_update_test() {
    if (!g_canvas2d_test) return;

    
    g_test_frame_count++;
    
    // 每帧都重绘测试内容，确保动画流畅
    if (g_test_frame_count % 1 == 0) { // 每帧都重绘
        canvas2d_update_bg();

        canvas2d_context_t *ctx = canvas2d_get_context(g_canvas2d_test);
        if (!ctx) return;

        // printf("Updating Canvas 2D Test (frame %d)\n", g_test_frame_count);

        // 清除画布
        // canvas2d_clear_rect(ctx, 0, 0, g_canvas2d_test->width, g_canvas2d_test->height);

        // 绘制基本图形
        // draw_basic_shapes(g_canvas2d_test);
        
        // 添加动画效果
        float time_factor = g_test_frame_count * 0.01f;
        
        // 绘制旋转的圆形
        canvas2d_save(ctx);
        canvas2d_translate(ctx, 400, 200);
        canvas2d_rotate(ctx, time_factor);
        canvas2d_set_fill_style(ctx, 0.5f + 0.5f * sin(time_factor), 0.2f, 0.8f, 1.0f);
        // canvas2d_fill_circle(ctx, 0, 0, 30);
        canvas2d_restore(ctx);

        // 绘制移动的矩形
        canvas2d_set_fill_style(ctx, 0.8f, 0.4f, 0.2f, 1.0f);
        float rect_x = 100 + 50 * sin(time_factor * 0.5f);
        // canvas2d_fill_rect(ctx, rect_x, 400, 80, 60);

        // 绘制动态文本 - 与矩形左对齐
        canvas2d_set_fill_style(ctx, 0.0f, 0.0f, 0.0f, 1.0f);
        char text_buffer[64];
        sprintf(text_buffer, "Canvas 2D Test - Frame %d", g_test_frame_count);
        canvas2d_fill_text(ctx, text_buffer, rect_x, 50);
        
        // 绘制缩放效果
        float scale = 1.0f + 0.3f * sin(time_factor * 0.8f);
        canvas2d_save(ctx);
        canvas2d_translate(ctx, 600, 500);
        canvas2d_scale(ctx, scale, scale);
        canvas2d_set_stroke_style(ctx, 1.0f, 0.0f, 1.0f, 1.0f);
        canvas2d_set_line_width(ctx, 3.0f);
        canvas2d_stroke_rect(ctx, -25, -25, 50, 50);
        canvas2d_restore(ctx);
    }
}

// 渲染canvas2d测试到屏幕
void canvas2d_render_test() {
    if (!g_canvas2d_test) return;
    
    // 将canvas2d的纹理渲染到游戏屏幕上
    // 在屏幕右上角显示canvas2d内容
    float screen_x = WINDOW_WIDTH - g_canvas2d_test->width - 10; // 右边距10像素
    float screen_y = 10; // 上边距10像素
    
    // 使用R_DrawImageEx来渲染canvas2d的纹理到屏幕
    DRAWIMAGE drawImage = {
        .texture = g_canvas2d_test->texture,
        .screen = {screen_x, screen_y, (float)g_canvas2d_test->width / 1000.0f, (float)g_canvas2d_test->height / 1000.0f},
        .uv = {0, 0, 1, 1},
        .color = COLOR32_WHITE,
        .rotate = false,
        .shader = SHADER_UI,
        .alphamode = BLEND_MODE_BLEND
    };

    R_DrawImageEx(&drawImage);
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

// 原始的测试函数（保持兼容性）
int canvas2d_runtest() {
    printf("Canvas 2D API Example (Single Run)\n");
    printf("===================================\n");

    // 创建一个临时画布
    canvas2d_t *canvas = canvas2d_create(800, 800);
    if (!canvas) {
        printf("Failed to create canvas\n");
        return 1;
    }

    printf("Canvas created successfully\n");

    // 绘制各种示例
    draw_basic_shapes(canvas);
    draw_transformed_shapes(canvas);
    draw_text_example(canvas);
    draw_path_example(canvas);
    draw_image_example(canvas);
    draw_composite_operations(canvas);

    printf("All examples drawn successfully\n");

    // 清理
    canvas2d_destroy(canvas);
    printf("Canvas destroyed\n");
    return 0;
}