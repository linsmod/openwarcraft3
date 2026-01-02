# HTML Context API 文档

## 简介

HTML Context API提供了基于Context的多实例HTML处理能力，允许同时加载和渲染多个HTML文档。相比旧的全局API，Context API提供了更好的封装性、可扩展性和多实例支持。

## 快速开始

### 最简单的示例

```c
#include "html/html.h"

// 创建context
context *ctx = html_context_create();

// 加载HTML文件
html_context_load_file(ctx, "test.html");

// 更新和渲染（在主循环中）
html_context_update(ctx, delta_time);
html_context_render(ctx);

// 清理
html_context_destroy(ctx);
```

## API概览

### 生命周期管理

#### 创建Context
```c
context* html_context_create(void);
```
创建一个新的HTML上下文。
- **返回**: 新的context指针，失败返回NULL
- **注意**: 必须在不再使用时调用`html_context_destroy()`释放

#### 销毁Context
```c
void html_context_destroy(context *ctx);
```
销毁HTML上下文并释放所有相关资源。
- **参数**:
  - `ctx`: HTML上下文
- **注意**: 销毁后不能再使用此context

### 文档加载

#### 从文件加载
```c
int html_context_load_file(context *ctx, const char *filename);
```
从文件加载HTML文档。
- **参数**:
  - `ctx`: HTML上下文
  - `filename`: HTML文件路径
- **返回**: 0成功，非0失败
- **示例**:
  ```c
  if (html_context_load_file(ctx, "index.html") != 0) {
      printf("Failed to load HTML file\n");
  }
  ```

#### 从内存加载
```c
int html_context_load_memory(context *ctx, const char *html_data, size_t length);
```
从内存缓冲区加载HTML文档。
- **参数**:
  - `ctx`: HTML上下文
  - `html_data`: HTML数据缓冲区
  - `length`: 数据长度（字节）
- **返回**: 0成功，非0失败
- **示例**:
  ```c
  const char *html = "<html><body>Hello</body></html>";
  html_context_load_memory(ctx, html, strlen(html));
  ```

### 文档查询

#### 获取XML文档
```c
xmlDoc* html_context_get_document(context *ctx);
```
获取底层的XML文档对象。
- **参数**:
  - `ctx`: HTML上下文
- **返回**: XML文档指针，失败返回NULL

#### 获取根元素
```c
xmlNode* html_context_get_root(context *ctx);
```
获取HTML文档的根元素（通常是`<html>`）。
- **参数**:
  - `ctx`: HTML上下文
- **返回**: 根节点指针，失败返回NULL
- **示例**:
  ```c
  xmlNode *root = html_context_get_root(ctx);
  printf("Root element: %s\n", (char*)root->name);
  ```

#### 根据ID查找元素
```c
xmlNode* html_context_find_by_id(context *ctx, const char *id);
```
根据ID属性查找元素。
- **参数**:
  - `ctx`: HTML上下文
  - `id`: 元素ID字符串
- **返回**: 元素节点指针，未找到返回NULL
- **示例**:
  ```c
  xmlNode *header = html_context_find_by_id(ctx, "main-header");
  if (header) {
      printf("Found header: %s\n", (char*)header->name);
  }
  ```

### 更新和渲染

#### 更新和布局
```c
void html_context_update(context *ctx, float delta_time);
```
更新HTML内容（动画）并重新计算布局。
- **参数**:
  - `ctx`: HTML上下文
  - `delta_time`: 距离上次更新时间（秒）
- **说明**: 应该在每一帧调用一次
- **示例**:
  ```c
  void update_loop(float delta_time) {
      html_context_update(ctx, delta_time);
  }
  ```

#### 渲染HTML
```c
void html_context_render(context *ctx);
```
渲染HTML文档到当前canvas。
- **参数**:
  - `ctx`: HTML上下文
- **说明**: 渲染背景和所有HTML元素
- **示例**:
  ```c
  void render_loop() {
      html_context_render(ctx);
  }
  ```

### 布局系统

#### 获取布局上下文
```c
lay_context* html_context_get_layout(context *ctx);
```
获取HTML的布局上下文对象。
- **参数**:
  - `ctx`: HTML上下文
- **返回**: 布局上下文指针
- **说明**: 可以用于高级布局查询和操作

### 字体管理

#### 设置字体
```c
bool html_context_set_font(context *ctx, const char *font_path, DWORD size);
```
设置HTML文本渲染的字体。
- **参数**:
  - `ctx`: HTML上下文
  - `font_path`: 字体文件路径（支持TTF/OTF）
  - `size`: 字体大小（像素）
- **返回**: true成功，false失败
- **示例**:
  ```c
  html_context_set_font(ctx, "/usr/share/fonts/TTF/DejaVuSans.ttf", 16);
  ```

### 动画系统

#### 扫描关键帧
```c
void html_context_scan_keyframes(context *ctx);
```
扫描HTML文档中的`@keyframes`规则并存储。
- **参数**:
  - `ctx`: HTML上下文
- **说明**: 应该在加载HTML后调用一次
- **示例**:
  ```c
  html_context_scan_keyframes(ctx);
  printf("Found %d keyframes\n", html_context_get_keyframes_count(ctx));
  ```

#### 重新应用动画
```c
void html_context_reapply_animations(context *ctx);
```
重新应用所有元素的动画属性。
- **参数**:
  - `ctx`: HTML上下文
- **说明**: 通常在扫描关键帧后调用

## 使用模式

### 模式1：单页面应用

```c
// 初始化
context *ctx = html_context_create();
html_context_load_file(ctx, "app.html");
html_context_set_font(ctx, "font.ttf", 14);
html_context_scan_keyframes(ctx);

// 主循环
while (running) {
    float dt = get_delta_time();

    // 更新
    html_context_update(ctx, dt);

    // 渲染
    clear_screen();
    html_context_render(ctx);
    present_screen();
}

// 清理
html_context_destroy(ctx);
```

### 模式2：多标签页浏览器

```c
#define MAX_TABS 5

typedef struct {
    context *ctx;
    char title[256];
    bool active;
} tab_t;

tab_t tabs[MAX_TABS];
int current_tab = 0;
int tab_count = 0;

// 添加新标签
int add_tab(const char *html_file, const char *title) {
    if (tab_count >= MAX_TABS) return -1;

    context *ctx = html_context_create();
    if (!ctx) return -1;

    if (html_context_load_file(ctx, html_file) != 0) {
        html_context_destroy(ctx);
        return -1;
    }

    html_context_scan_keyframes(ctx);

    tabs[tab_count].ctx = ctx;
    strncpy(tabs[tab_count].title, title, 255);
    tabs[tab_count].active = false;
    tab_count++;

    return tab_count - 1;
}

// 切换标签
void switch_tab(int index) {
    if (index < 0 || index >= tab_count) return;

    tabs[current_tab].active = false;
    current_tab = index;
    tabs[current_tab].active = true;
}

// 更新和渲染（只渲染当前标签）
void update_render_loop() {
    float dt = get_delta_time();

    // 更新所有标签（保持动画同步）
    for (int i = 0; i < tab_count; i++) {
        html_context_update(tabs[i].ctx, dt);
    }

    // 只渲染当前标签
    clear_screen();
    html_context_render(tabs[current_tab].ctx);

    // 渲染标签栏（自定义UI）
    render_tab_bar();

    present_screen();
}
```

### 模式3：预览多个HTML文件

```c
// 创建多个context
context *views[3];

views[0] = html_context_create();
views[1] = html_context_create();
views[2] = html_context_create();

// 加载不同的HTML
html_context_load_file(views[0], "preview1.html");
html_context_load_file(views[1], "preview2.html");
html_context_load_file(views[2], "preview3.html");

// 在网格中渲染
void render_grid() {
    float dt = get_delta_time();

    // 更新所有
    for (int i = 0; i < 3; i++) {
        html_context_update(views[i], dt);
    }

    // 分别渲染到不同位置（需要canvas变换支持）
    clear_screen();

    // 预览1 (左上)
    save_canvas_transform();
    translate_canvas(0, 0);
    html_context_render(views[0]);
    restore_canvas_transform();

    // 预览2 (右上)
    save_canvas_transform();
    translate_canvas(400, 0);
    html_context_render(views[1]);
    restore_canvas_transform();

    // 预览3 (下方)
    save_canvas_transform();
    translate_canvas(200, 300);
    html_context_render(views[2]);
    restore_canvas_transform();

    present_screen();
}
```

## 与UI组件集成

### 基本集成

```c
typedef struct {
    ui_component_t base;      // UI组件基础

    context *html_ctx;       // HTML上下文
    char html_file[256];     // HTML文件路径
    bool loaded;             // 是否已加载
} ui_html_component_t;

// 初始化
void html_component_init(ui_html_component_t *comp, const char *html_file) {
    strncpy(comp->html_file, html_file, 255);

    comp->html_ctx = html_context_create();
    if (comp->html_ctx) {
        html_context_load_file(comp->html_ctx, html_file);
        html_context_scan_keyframes(comp->html_ctx);
        comp->loaded = true;
    }
}

// 更新
static void html_component_update(ui_component_t *base, int msec) {
    ui_html_component_t *comp = (ui_html_component_t *)base;
    if (!comp->loaded) return;

    float dt = msec / 1000.0f;
    html_context_update(comp->html_ctx, dt);
}

// 渲染
static void html_component_render(ui_component_t *base) {
    ui_html_component_t *comp = (ui_html_component_t *)base;
    if (!comp->loaded) return;

    // 在组件位置渲染
    save_canvas_transform();
    translate_canvas(base->x, base->y);
    html_context_render(comp->html_ctx);
    restore_canvas_transform();
}

// 销毁
static void html_component_shutdown(ui_component_t *base) {
    ui_html_component_t *comp = (ui_html_component_t *)base;

    if (comp->html_ctx) {
        html_context_destroy(comp->html_ctx);
        comp->html_ctx = NULL;
    }
    comp->loaded = false;
}
```

## 性能考虑

### 内存使用

每个HTML context大约占用：
- DOM树：约100KB-1MB（取决于HTML大小）
- 布局数据：约50KB-500KB
- 动画数据：约10KB-100KB
- **总计**：约200KB-2MB per context

### 最佳实践

1. **限制context数量**
   - 避免创建过多的context（建议<10个）
   - 不活跃的context可以销毁或暂停

2. **延迟加载**
   ```c
   // 只有在需要时才加载
   if (!tab.loaded && tab_should_be_visible(index)) {
       html_context_load_file(tab.ctx, tab.html_file);
       tab.loaded = true;
   }
   ```

3. **批量更新**
   ```c
   // 更新所有context
   for (int i = 0; i < count; i++) {
       html_context_update(contexts[i], delta_time);
   }

   // 然后统一渲染
   for (int i = 0; i < count; i++) {
       html_context_render(contexts[i]);
   }
   ```

4. **缓存和重用**
   ```c
   // 重用context而不是频繁创建销毁
   static context *cached_ctx = NULL;

   if (!cached_ctx) {
       cached_ctx = html_context_create();
   }

   // 重新加载而不是创建新的
   html_context_load_file(cached_ctx, new_file);
   ```

## 错误处理

所有函数都应该检查返回值：

```c
context *ctx = html_context_create();
if (!ctx) {
    fprintf(stderr, "Failed to create context\n");
    return -1;
}

if (html_context_load_file(ctx, "test.html") != 0) {
    fprintf(stderr, "Failed to load HTML\n");
    html_context_destroy(ctx);
    return -1;
}

if (!html_context_set_font(ctx, "font.ttf", 16)) {
    fprintf(stderr, "Warning: Failed to set font\n");
    // 可以继续，使用默认字体
}

xmlNode *elem = html_context_find_by_id(ctx, "missing-id");
if (!elem) {
    fprintf(stderr, "Element not found, using fallback\n");
}
```

## 限制

1. **不支持XPath查询**
   - 当前`html_context_find_by_id()`使用简单遍历
   - 复杂查询需要使用libxml的XPath API

2. **没有CSS选择器支持**
   - 不能通过class name或标签名查询
   - 需要手动遍历DOM树

3. **动画属性有限**
   - 只支持部分CSS动画属性
   - 复杂的动画可能不工作

4. **非线程安全**
   - Context API不是线程安全的
   - 多线程访问需要外部同步

## 未来改进

1. **完整的DOM API**
   - XPath查询支持
   - CSS选择器支持
   - DOM操作（添加/删除节点）

2. **增强的动画系统**
   - 更多的CSS动画属性
   - 动画事件（开始/结束/循环）
   - 动画控制（暂停/恢复/反向）

3. **性能优化**
   - 增量渲染
   - 视口裁剪
   - 文本缓存

4. **Canvas集成**
   - 每个context独立的canvas
   - 纹理合成到主canvas
   - 离屏渲染支持

## 相关文档

- [HTML_CONTEXT_REFACTOR.md](./HTML_CONTEXT_REFACTOR.md) - 重构说明和迁移指南
- [README.md](./README.md) - 原始HTML模块文档
- [layout_readme.md](./layout_readme.md) - 布局系统说明

## 许可证

继承自Hubbub项目（MIT License）
