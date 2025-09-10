#ifndef CANVAS2D_TEST_H
#define CANVAS2D_TEST_H

#include "canvas2d.h"

// 全局变量声明，用于在游戏运行时持续显示canvas2d测试
extern canvas2d_t *g_canvas2d_test;
extern int g_test_frame_count;

// 函数声明
int canvas2d_init_test(void);
void canvas2d_update_test(void);
void canvas2d_render_test(void);
void canvas2d_cleanup_test(void);

#endif // CANVAS2D_TEST_H