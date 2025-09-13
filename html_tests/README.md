# HTML Renderer Test

这个目录包含HTML渲染功能的测试文件和使用说明。

## 文件说明

- `test_render.html` - 测试HTML页面，包含各种HTML元素
- `test_html_render.c` - 测试程序（需要编译环境支持）

## 功能特性

HTML渲染器支持以下功能：

### 已实现的功能
- ✅ HTML文档解析
- ✅ 布局计算（使用lay库）
- ✅ 基本元素渲染（div, p, h1-h6, ul, ol, li, table等）
- ✅ 文本渲染
- ✅ 表单元素渲染（input, textarea, select）
- ✅ 图片元素占位符
- ✅ 调试信息面板
- ✅ 多种渲染模式（静态、动画）

### 渲染模式
1. **静态模式** (HTML_RENDER_MODE_STATIC) - 默认模式，只计算一次布局
2. **动画模式** (HTML_RENDER_MODE_ANIMATED) - 每帧重新计算布局
3. **交互模式** (HTML_RENDER_MODE_INTERACTIVE) - 为将来交互功能预留

### 元素支持
- **块级元素**: div, p, h1-h6, table, form, ul, ol
- **内联元素**: span, a, img, input, textarea, select
- **文本节点**: 自动处理和渲染

### 调试功能
- 彩色边框标识不同元素类型
- 右上角信息面板显示渲染状态
- 控制台输出调试信息

## 使用方法

### 1. 初始化渲染器
```c
context *html_ctx = your_html_context;
html_render_init(html_ctx);
```

### 2. 设置渲染模式
```c
// 静态模式（默认）
html_render_set_mode(HTML_RENDER_MODE_STATIC);

// 动画模式
html_render_set_mode(HTML_RENDER_MODE_ANIMATED);
```

### 3. 渲染帧
```c
// 在主循环中调用
html_render_frame();
```

### 4. 清理资源
```c
html_render_cleanup();
```

## 集成到现有项目

要将HTML渲染器集成到现有项目中：

1. 包含必要的头文件
2. 初始化HTML解析上下文
3. 调用html_render_init()初始化渲染器
4. 在渲染循环中调用html_render_frame()
5. 适当的时候调用html_render_cleanup()

## 示例用法

参考 `test_html_render.c` 文件中的示例代码。

## 注意事项

1. 确保项目已正确配置libxml2和lay库的依赖
2. 渲染器使用项目的图形API进行绘制
3. 当前版本主要用于调试和演示，生产环境使用前需要进一步优化
4. 图片和CSS支持有限，主要用于布局和基本样式

## 扩展功能

未来可以添加的功能：
- 完整的CSS支持
- 图片加载和渲染
- 交互功能（鼠标、键盘事件）
- 滚动和视口支持
- 更好的文本换行和格式化
- 动画和过渡效果