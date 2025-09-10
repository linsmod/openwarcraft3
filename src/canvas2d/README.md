# Canvas 2D API

基于游戏引擎2D渲染实现的类似Web Canvas API的库。

## 特性

- 类似Web Canvas API的简单易用的接口
- 支持基本的2D绘图操作（矩形、圆形、三角形、四边形）
- 支持路径绘制
- 支持文本渲染
- 支持图像加载和绘制
- 支持变换操作（平移、旋转、缩放）
- 支持混合模式
- 支持状态保存和恢复

## 文件结构

```
src/canvas2d/
├── canvas2d.h      - Canvas 2D API头文件
├── canvas2d.c      - Canvas 2D API实现
├── example.c       - 示例程序
├── Makefile        - 构建配置
└── README.md       - 说明文档
```

## API 概览

### Canvas 创建和销毁

```c
// 创建Canvas
canvas2d_t* canvas2d_create(int width, int height);

// 获取上下文
canvas2d_context_t* canvas2d_get_context(canvas2d_t *canvas);

// 销毁Canvas
void canvas2d_destroy(canvas2d_t *canvas);
```

### 基本绘图操作

```c
// 清除矩形区域
void canvas2d_clear_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);

// 绘制填充矩形
void canvas2d_fill_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);

// 绘制矩形边框
void canvas2d_stroke_rect(canvas2d_context_t *ctx, float x, float y, float width, float height);

// 绘制填充圆形
void canvas2d_fill_circle(canvas2d_context_t *ctx, float x, float y, float radius);

// 绘制圆形边框
void canvas2d_stroke_circle(canvas2d_context_t *ctx, float x, float y, float radius);

// 绘制填充三角形
void canvas2d_fill_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle);

// 绘制三角形边框
void canvas2d_stroke_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle);

// 绘制填充四边形
void canvas2d_fill_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad);

// 绘制四边形边框
void canvas2d_stroke_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad);
```

### 路径绘制

```c
// 绘制填充路径
void canvas2d_fill_path(canvas2d_context_t *ctx, const canvas2d_path_t *path);

// 绘制路径边框
void canvas2d_stroke_path(canvas2d_context_t *ctx, const canvas2d_path_t *path);
```

### 文本渲染

```c
// 绘制填充文本
void canvas2d_fill_text(canvas2d_context_t *ctx, const char *text, float x, float y);

// 绘制文本边框
void canvas2d_stroke_text(canvas2d_context_t *ctx, const char *text, float x, float y);

// 设置字体
void canvas2d_set_font(canvas2d_context_t *ctx, const char *fontFamily, float fontSize);
```

### 图像操作

```c
// 从文件创建图像
canvas2d_image_t* canvas2d_create_image(const char *filename);

// 从数据创建图像
canvas2d_image_t* canvas2d_create_image_from_data(unsigned char *data, int width, int height, int channels);

// 销毁图像
void canvas2d_destroy_image(canvas2d_image_t *image);

// 绘制图像
void canvas2d_draw_image(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y);

// 绘制缩放图像
void canvas2d_draw_image_scaled(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y, float width, float height);

// 绘制图像切片
void canvas2d_draw_image_sliced(canvas2d_context_t *ctx, canvas2d_image_t *image, 
                               float sx, float sy, float sw, float sh, 
                               float dx, float dy, float dw, float dh);
```

### 变换操作

```c
// 平移
void canvas2d_translate(canvas2d_context_t *ctx, float x, float y);

// 旋转
void canvas2d_rotate(canvas2d_context_t *ctx, float angle);

// 缩放
void canvas2d_scale(canvas2d_context_t *ctx, float x, float y);

// 重置变换
void canvas2d_reset_transform(canvas2d_context_t *ctx);

// 保存状态
void canvas2d_save(canvas2d_context_t *ctx);

// 恢复状态
void canvas2d_restore(canvas2d_context_t *ctx);
```

### 样式设置

```c
// 设置边框样式
void canvas2d_set_stroke_style(canvas2d_context_t *ctx, float r, float g, float b, float a);

// 设置填充样式
void canvas2d_set_fill_style(canvas2d_context_t *ctx, float r, float g, float b, float a);

// 设置线宽
void canvas2d_set_line_width(canvas2d_context_t *ctx, float width);

// 设置线帽样式
void canvas2d_set_line_cap(canvas2d_context_t *ctx, canvas2d_line_cap_t cap);

// 设置线连接样式
void canvas2d_set_line_join(canvas2d_context_t *ctx, canvas2d_line_join_t join);

// 设置全局混合模式
void canvas2d_set_global_composite_operation(canvas2d_context_t *ctx, canvas2d_global_composite_operation_t operation);
```

### 错误处理

```c
// 获取错误信息
const char* canvas2d_get_error_string(void);

// 记录错误
void canvas2d_log_error(const char *message);
```

## 构建和使用

### 构建示例程序

```bash
cd src/canvas2d
make
```

### 运行示例程序

```bash
cd src/canvas2d
make run
```

### 编译自己的程序

```c
#include "canvas2d.h"

int main() {
    // 创建Canvas
    canvas2d_t *canvas = canvas2d_create(800, 600);
    if (!canvas) {
        printf("Failed to create canvas: %s\n", canvas2d_get_error_string());
        return 1;
    }

    // 获取上下文
    canvas2d_context_t *ctx = canvas2d_get_context(canvas);
    
    // 绘制一些图形
    canvas2d_set_fill_style(ctx, 1.0f, 0.0f, 0.0f, 1.0f); // 红色
    canvas2d_fill_rect(ctx, 50, 50, 100, 100);
    
    // 清理
    canvas2d_destroy(canvas);
    
    return 0;
}
```

编译命令：

```bash
gcc -I/path/to/src/canvas2d your_program.c -o your_program -L/path/to/build -lcanvas2d
```

## 示例程序

示例程序展示了Canvas 2D API的主要功能：

1. 基本图形绘制
2. 变换操作
3. 文本渲染
4. 路径绘制
5. 图像操作
6. 混合模式

运行示例程序：

```bash
cd src/canvas2d
make clean && make && ./canvas2d_example
```

## 依赖项

- OpenGL
- SDL2
- StormLib
- 数学库 (cmath3)
- 游戏引擎核心库

## 注意事项

- 该API基于现有的游戏引擎2D渲染实现
- 需要正确初始化OpenGL上下文
- 图像格式支持取决于底层渲染器
- 性能优化建议：批量绘制、减少状态切换

## 扩展功能

可以考虑添加以下功能：

1. 更复杂的路径操作（贝塞尔曲线）
2. 滤镜效果
3. 动画支持
4. 事件处理
5. 更多的混合模式
6. 碰撞检测

## 许可证

请参考项目的许可证文件。