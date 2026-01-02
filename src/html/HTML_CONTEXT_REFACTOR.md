# HTML Context API 重构说明

## 概述

本次重构将HTML模块从全局API升级为基于Context的多实例API，支持同时加载和处理多个HTML文档。

## 新增文件

### 1. `html_context.h`
HTML Context API的完整接口定义，提供了所有Context相关的函数声明。

### 2. `html.h` 更新
在原有全局API基础上，新增了Context相关的函数声明，分为两个部分：
- **旧版全局API**：保持向后兼容，内部使用全局context（index 0）
- **新版Context API**：推荐的API，支持多实例

### 3. `html.c` 更新
在文件末尾添加了Context API的实现函数，包括：

#### Context生命周期
- `html_context_create()` - 创建HTML上下文
- `html_context_destroy()` - 销毁HTML上下文

#### 文档加载
- `html_context_load_file()` - 从文件加载HTML
- `html_context_load_memory()` - 从内存加载HTML
- `html_context_get_document()` - 获取XML文档
- `html_context_get_root()` - 获取根元素

#### 布局系统
- `html_context_get_layout()` - 获取布局上下文
- `html_context_update()` - 更新和布局

#### 渲染
- `html_context_render()` - 渲染HTML文档
- `html_context_set_font()` - 设置渲染字体

#### 动画系统
- `html_context_scan_keyframes()` - 扫描关键帧动画
- `html_context_reapply_animations()` - 重新应用动画

#### DOM操作
- `html_context_find_by_id()` - 根据ID查找元素

## 使用方法

### 旧版API（向后兼容）

```c
// 初始化HTML（使用全局context）
int result = html_init("test.html");
if (result == 0) {
    printf("HTML initialized successfully\n");
}

// 更新和布局
html_update_and_layout(delta_time, 0);  // page_index = 0

// 渲染
html_render();

// 清理
html_destroy();
```

### 新版Context API（推荐）

```c
// 1. 创建HTML上下文
context *ctx = html_context_create();
if (!ctx) {
    printf("Failed to create HTML context\n");
    return -1;
}

// 2. 加载HTML文件
int result = html_context_load_file(ctx, "test.html");
if (result != 0) {
    html_context_destroy(ctx);
    return -1;
}

// 3. 设置字体（可选）
html_context_set_font(ctx, "/path/to/font.ttf", 16);

// 4. 扫描关键帧动画（如果有）
html_context_scan_keyframes(ctx);

// 5. 在主循环中更新和渲染
void update(float delta_time) {
    // 更新HTML内容和布局
    html_context_update(ctx, delta_time);
}

void render() {
    // 渲染HTML文档
    html_context_render(ctx);
}

// 6. 清理
html_context_destroy(ctx);
```

### 多实例示例

```c
// 创建多个HTML上下文
context *ctx1 = html_context_create();
context *ctx2 = html_context_create();

// 分别加载不同的HTML文件
html_context_load_file(ctx1, "page1.html");
html_context_load_file(ctx2, "page2.html");

// 分别扫描动画
html_context_scan_keyframes(ctx1);
html_context_scan_keyframes(ctx2);

// 在主循环中分别更新和渲染
void update(float delta_time) {
    html_context_update(ctx1, delta_time);
    html_context_update(ctx2, delta_time);
}

void render() {
    // 分别渲染（可以设置不同的位置和缩放）
    html_context_render(ctx1);
    html_context_render(ctx2);
}

// 清理
html_context_destroy(ctx1);
html_context_destroy(ctx2);
```

## API对比

| 功能 | 旧版API | 新版Context API |
|------|---------|---------------|
| 创建上下文 | 隐式（在html_init中） | 显式：`html_context_create()` |
| 加载文件 | `html_init(filename)` | `html_context_load_file(ctx, filename)` |
| 加载内存 | 不支持 | `html_context_load_memory(ctx, data, len)` |
| 更新布局 | `html_update_and_layout(dt, idx)` | `html_context_update(ctx, dt)` |
| 渲染 | `html_render()` | `html_context_render(ctx)` |
| 销毁 | `html_destroy()` | `html_context_destroy(ctx)` |
| 多实例 | 通过page_index索引 | 每个独立的context |
| DOM查询 | 不支持 | `html_context_find_by_id(ctx, id)` |

## 迁移指南

### 从旧版迁移到新版

1. **创建Context替代全局初始化**
   ```c
   // 旧代码
   html_init("test.html");

   // 新代码
   context *ctx = html_context_create();
   html_context_load_file(ctx, "test.html");
   ```

2. **更新和渲染使用context参数**
   ```c
   // 旧代码
   html_update_and_layout(delta_time, 0);
   html_render();

   // 新代码
   html_context_update(ctx, delta_time);
   html_context_render(ctx);
   ```

3. **管理context生命周期**
   ```c
   // 旧代码
   html_destroy();

   // 新代码
   html_context_destroy(ctx);
   ```

## 后续工作

1. **Canvas和Layout上下文管理**
   - 将全局的canvas和layout改为context独立
   - 每个HTML context使用自己的canvas
   - 通过纹理合成与主canvas集成

2. **创建ui_html_viewer组件**
   - 基于ui_component框架
   - 封装HTML context
   - 实现完整的生命周期和事件处理

3. **增强DOM操作**
   - 实现XPath查询
   - 支持CSS选择器查询
   - 提供DOM操作API

4. **完善动画系统**
   - 支持CSS动画属性
   - 提供动画控制API（播放、暂停、停止）
   - 支持动画事件

## 注意事项

1. **向后兼容性**
   - 旧的全局API仍然可用
   - 旧API内部使用全局context数组
   - 建议新代码使用Context API

2. **线程安全**
   - 当前Context API不是线程安全的
   - 多线程使用需要外部加锁

3. **内存管理**
   - 每个context独立管理内存
   - 必须调用`html_context_destroy()`释放资源

4. **错误处理**
   - 创建context失败返回NULL
   - 加载文件失败返回非0值
   - 建议检查所有返回值

## 编译

确保你的CMakeLists.txt包含所有必要的源文件：

```cmake
# HTML模块
set(HTML_SOURCES
    src/html/html.c
    src/html/css.c
    src/html/animation/anim_state.c
    src/html/css/css_animation.c
    src/html/css/css_gradient.c
    src/html/renderer/render_gradient.c
    src/html/renderer/render_utils.c
)

add_library(html STATIC ${HTML_SOURCES})
```

## 测试

创建简单的测试程序验证Context API：

```c
#include "html/html.h"

int main() {
    // 创建context
    context *ctx = html_context_create();
    if (!ctx) return 1;

    // 加载HTML
    if (html_context_load_file(ctx, "test.html") != 0) {
        html_context_destroy(ctx);
        return 1;
    }

    // 获取根元素
    xmlNode *root = html_context_get_root(ctx);
    printf("Root element: %s\n", root ? (char*)root->name : "NULL");

    // 查找元素
    xmlNode *elem = html_context_find_by_id(ctx, "my-element");
    printf("Found element: %s\n", elem ? (char*)elem->name : "Not found");

    // 清理
    html_context_destroy(ctx);

    return 0;
}
```

## 总结

本次重构为HTML模块提供了更好的封装和多实例支持，为后续封装为ui_component组件奠定了基础。同时保持了向后兼容性，现有代码可以继续使用旧API。
