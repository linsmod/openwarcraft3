#include "canvas2d.h"
#include <stdio.h>
#include <stdlib.h>

// Example program demonstrating Canvas 2D API usage
void draw_basic_shapes(canvas2d_t *canvas) {
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    if (!ctx) return;

    printf("Drawing basic shapes...\n");

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

int canvas2d_runtest() {

    printf("Canvas 2D API Example\n");
    printf("====================\n");

    // Create a canvas
    canvas2d_t *canvas = canvas2d_create(800, 800);
    if (!canvas) {
        printf("Failed to create canvas\n");
        return 1;
    }

    printf("Canvas created successfully\n");

    // Draw various examples
    draw_basic_shapes(canvas);
    draw_transformed_shapes(canvas);
    draw_text_example(canvas);
    draw_path_example(canvas);
    draw_image_example(canvas);
    draw_composite_operations(canvas);

    printf("All examples drawn successfully\n");

    // Clean up
    canvas2d_destroy(canvas);
    printf("Canvas destroyed\n");
    return 0;
}