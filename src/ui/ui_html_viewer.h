#ifndef __UI_HTML_VIEWER_H__
#define __UI_HTML_VIEWER_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"
#include "html/html.h"
#include "html/layout.h"

// ==================== 前向声明 ====================

typedef struct ui_html_viewer_t ui_html_viewer_t;

// ==================== 回调类型 ====================

typedef void (*html_viewer_element_clicked_fn)(ui_html_viewer_t *viewer,
                                           xmlNode *element,
                                           void *user_data);

typedef void (*html_viewer_link_clicked_fn)(ui_html_viewer_t *viewer,
                                         const char *url,
                                         void *user_data);

// ==================== 组件结构 ====================

struct ui_html_viewer_t {
    ui_component_t base;

    context *html_ctx;
    bool loaded;
    char html_filename[256];

    float zoom;
    float scroll_x;
    float scroll_y;
    float scroll_max_x;
    float scroll_max_y;

    char font_path[256];
    DWORD font_size;

    COLOR32 background_color;

    html_viewer_element_clicked_fn on_element_clicked;
    html_viewer_link_clicked_fn on_link_clicked;
    void *callback_user_data;

};

// ==================== 创建和销毁 ====================

ui_html_viewer_t* UIHTMLViewer_Create(float x, float y, float width, float height,
                                   canvas2d_context_t *ctx);

int UIHTMLViewer_Init(ui_html_viewer_t *viewer, canvas2d_context_t *ctx);

void UIHTMLViewer_Destroy(ui_html_viewer_t *viewer);

// ==================== HTML文档加载 ====================

int UIHTMLViewer_LoadFromFile(ui_html_viewer_t *viewer, const char *filename);

int UIHTMLViewer_LoadFromMemory(ui_html_viewer_t *viewer,
                               const char *html_data,
                               size_t length);

int UIHTMLViewer_Reload(ui_html_viewer_t *viewer);

void UIHTMLViewer_Unload(ui_html_viewer_t *viewer);

// ==================== 渲染控制 ====================

void UIHTMLViewer_SetZoom(ui_html_viewer_t *viewer, float zoom);

void UIHTMLViewer_SetScroll(ui_html_viewer_t *viewer, float x, float y);

void UIHTMLViewer_GetScroll(const ui_html_viewer_t *viewer, float *x, float *y);

int UIHTMLViewer_SetFont(ui_html_viewer_t *viewer, const char *font_path, DWORD size);

// ==================== DOM查询 ====================

xmlNode* UIHTMLViewer_FindById(ui_html_viewer_t *viewer, const char *id);

// ==================== 动画支持 ====================

void UIHTMLViewer_ScanKeyframes(ui_html_viewer_t *viewer);

void UIHTMLViewer_ReapplyAnimations(ui_html_viewer_t *viewer);

// ==================== 事件回调 ====================

void UIHTMLViewer_SetElementClicked(ui_html_viewer_t *viewer,
                                  html_viewer_element_clicked_fn callback,
                                  void *user_data);

void UIHTMLViewer_SetLinkClicked(ui_html_viewer_t *viewer,
                                html_viewer_link_clicked_fn callback,
                                void *user_data);

// ==================== 更新和渲染 ====================

void UIHTMLViewer_Update(ui_html_viewer_t *viewer, int msec);

void UIHTMLViewer_Render(ui_html_viewer_t *viewer);

#endif // __UI_HTML_VIEWER_H__
