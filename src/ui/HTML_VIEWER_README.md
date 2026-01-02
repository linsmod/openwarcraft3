# UI HTML Viewer 组件

## 概述

HTML Viewer组件封装HTML context，使其能够在UI系统中使用。

## 基本用法

```c
// 创建组件
ui_html_viewer_t *viewer = UIHTMLViewer_Create(x, y, width, height, ctx);

// 加载HTML
UIHTMLViewer_LoadFromFile(viewer, "test.html");

// 设置回调
UIHTMLViewer_SetElementClicked(viewer, callback, user_data);

// 主循环
UIHTMLViewer_Update(viewer, msec);
UIHTMLViewer_Render(viewer);

// 清理
UIHTMLViewer_Destroy(viewer);
```

## API

### 创建/销毁
- `UIHTMLViewer_Create()` - 创建组件
- `UIHTMLViewer_Destroy()` - 销毁组件

### 加载HTML
- `UIHTMLViewer_LoadFromFile()` - 从文件加载
- `UIHTMLViewer_LoadFromMemory()` - 从内存加载
- `UIHTMLViewer_Reload()` - 重新加载
- `UIHTMLViewer_Unload()` - 卸载

### 渲染控制
- `UIHTMLViewer_SetZoom()` - 设置缩放
- `UIHTMLViewer_SetScroll()` - 设置滚动位置
- `UIHTMLViewer_GetScroll()` - 获取滚动位置
- `UIHTMLViewer_SetFont()` - 设置字体

### DOM操作
- `UIHTMLViewer_FindById()` - 根据ID查找元素

### 动画
- `UIHTMLViewer_ScanKeyframes()` - 扫描关键帧
- `UIHTMLViewer_ReapplyAnimations()` - 重新应用动画

### 事件回调
- `UIHTMLViewer_SetElementClicked()` - 设置元素点击回调
- `UIHTMLViewer_SetLinkClicked()` - 设置链接点击回调

## 在场景中使用

参考 `ui_html_viewer_example.c` 查看完整示例。
