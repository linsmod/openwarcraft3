// canvas2d_test.h
#ifndef CANVAS2D_TEST_H
#define CANVAS2D_TEST_H

#include "canvas2d.h"

// 全局变量声明
extern canvas2d_t *g_canvas2d_test;
extern int g_test_frame_count;

// 测试函数声明
int canvas2d_init_test();
void canvas2d_test_render_frame();
void draw_test_tips();
void canvas2d_cleanup_test();
int canvas2d_runtest();

// 示例绘制函数
void draw_basic_shapes(canvas2d_t *canvas);
void draw_transformed_shapes(canvas2d_t *canvas);
void draw_text_test(canvas2d_t *canvas);

#endif // CANVAS2D_TEST_H