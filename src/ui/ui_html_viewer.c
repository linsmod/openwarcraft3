#include "ui_html_viewer.h"
#include "../html/html.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 虚函数
static void html_viewer_init(ui_component_t *component, canvas2d_context_t *ctx);
static void html_viewer_shutdown(ui_component_t *component);
static void html_viewer_update(ui_component_t *component, int msec);
static void html_viewer_render(ui_component_t *component);
static void html_viewer_on_click(ui_component_t *component, event_t *event);

static const ui_component_vtable_t html_viewer_vtable = {
    .init = html_viewer_init,
    .shutdown = html_viewer_shutdown,
    .update = html_viewer_update,
    .render = html_viewer_render,
    .on_click = html_viewer_on_click
};

static void html_viewer_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    viewer->html_ctx = NULL;
    viewer->loaded = false;
    viewer->html_filename[0] = '\0';

    viewer->zoom = 1.0f;
    viewer->scroll_x = 0.0f;
    viewer->scroll_y = 0.0f;
    viewer->scroll_max_x = 0.0f;
    viewer->scroll_max_y = 0.0f;

    viewer->font_path[0] = '\0';
    viewer->font_size = 14;

    viewer->background_color = (COLOR32){255, 255, 255, 255};

    viewer->on_element_clicked = NULL;
    viewer->on_link_clicked = NULL;
    viewer->callback_user_data = NULL;
}

static void html_viewer_shutdown(ui_component_t *component) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    if (viewer->html_ctx) {
        html_context_destroy(viewer->html_ctx);
        viewer->html_ctx = NULL;
    }

    viewer->loaded = false;
}

static void html_viewer_update(ui_component_t *component, int msec) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    if (!viewer->loaded || !viewer->html_ctx) return;

    float delta_time = msec / 1000.0f;
    html_context_update(viewer->html_ctx, delta_time);
}

static void html_viewer_render(ui_component_t *component) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    if (!viewer->loaded || !viewer->html_ctx || !UIComponent_IsVisible(component)) return;

    canvas2d_save(component->ctx);

    canvas2d_translate(component->ctx, viewer->scroll_x, viewer->scroll_y);

    if (viewer->zoom != 1.0f) {
        canvas2d_translate(component->ctx, component->x, component->y);
        canvas2d_scale(component->ctx, viewer->zoom, viewer->zoom);
        canvas2d_translate(component->ctx, -component->x, -component->y);
    }

    html_context_render(viewer->html_ctx);

    canvas2d_restore(component->ctx);
}

static void html_viewer_on_click(ui_component_t *component, event_t *event) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    // 如果没有设置点击回调，让事件继续传播
    if (!viewer->html_ctx || !viewer->on_element_clicked) {
        return;
    }

    float local_x = event->mouse.x - component->x - viewer->scroll_x;
    float local_y = event->mouse.y - component->y - viewer->scroll_y;

    if (viewer->zoom != 1.0f) {
        local_x /= viewer->zoom;
        local_y /= viewer->zoom;
    }

    // 查找被点击的元素
    xmlNode *elem = html_context_find_by_id(viewer->html_ctx, "clicked");
    if (elem && viewer->on_element_clicked) {
        viewer->on_element_clicked(viewer, elem, viewer->callback_user_data);
    }
}

ui_html_viewer_t* UIHTMLViewer_Create(float x, float y, float width, float height,
                                   canvas2d_context_t *ctx) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)malloc(sizeof(ui_html_viewer_t));
    if (!viewer) {
        return NULL;
    }

    UIComponent_InitBase((ui_component_t *)viewer, UI_COMPONENT_TYPE_HTML_VIEWER,
                        &html_viewer_vtable, ctx);

    UIComponent_SetPosition((ui_component_t *)viewer, x, y);
    UIComponent_SetSize((ui_component_t *)viewer, width, height);

    html_viewer_init((ui_component_t *)viewer, ctx);

    return viewer;
}

int UIHTMLViewer_Init(ui_html_viewer_t *viewer, canvas2d_context_t *ctx) {
    if (!viewer || !ctx) return -1;

    UIComponent_InitBase((ui_component_t *)viewer, UI_COMPONENT_TYPE_HTML_VIEWER,
                        &html_viewer_vtable, ctx);

    html_viewer_init((ui_component_t *)viewer, ctx);

    return 0;
}

void UIHTMLViewer_Destroy(ui_html_viewer_t *viewer) {
    if (!viewer) return;

    if (viewer->html_ctx) {
        html_context_destroy(viewer->html_ctx);
    }

    UIComponent_ShutdownBase((ui_component_t *)viewer);
    free(viewer);
}

int UIHTMLViewer_LoadFromFile(ui_html_viewer_t *viewer, const char *filename) {
    if (!viewer || !filename) return -1;

    if (!viewer->html_ctx) {
        viewer->html_ctx = html_context_create();
        if (!viewer->html_ctx) return -1;
    }

    strncpy(viewer->html_filename, filename, 255);
    viewer->html_filename[255] = '\0';

    int result = html_context_load_file(viewer->html_ctx, filename);
    if (result != 0) return -1;

    html_context_scan_keyframes(viewer->html_ctx);
    html_context_reapply_animations(viewer->html_ctx);

    if (viewer->font_path[0] != '\0') {
        html_context_set_font(viewer->html_ctx, viewer->font_path, viewer->font_size);
    }

    viewer->loaded = true;
    return 0;
}

int UIHTMLViewer_LoadFromMemory(ui_html_viewer_t *viewer,
                               const char *html_data,
                               size_t length) {
    if (!viewer || !html_data || length == 0) return -1;

    if (!viewer->html_ctx) {
        viewer->html_ctx = html_context_create();
        if (!viewer->html_ctx) return -1;
    }

    viewer->html_filename[0] = '\0';

    int result = html_context_load_memory(viewer->html_ctx, html_data, length);
    if (result != 0) return -1;

    viewer->loaded = true;
    return 0;
}

int UIHTMLViewer_Reload(ui_html_viewer_t *viewer) {
    if (!viewer || viewer->html_filename[0] == '\0') return -1;

    char filename[256];
    strncpy(filename, viewer->html_filename, 255);
    filename[255] = '\0';

    UIHTMLViewer_Unload(viewer);
    return UIHTMLViewer_LoadFromFile(viewer, filename);
}

void UIHTMLViewer_Unload(ui_html_viewer_t *viewer) {
    if (!viewer) return;

    if (viewer->html_ctx) {
        html_context_destroy(viewer->html_ctx);
        viewer->html_ctx = NULL;
    }

    viewer->loaded = false;
}

void UIHTMLViewer_SetZoom(ui_html_viewer_t *viewer, float zoom) {
    if (!viewer) return;
    viewer->zoom = zoom > 0.1f ? zoom : 0.1f;
}

void UIHTMLViewer_SetScroll(ui_html_viewer_t *viewer, float x, float y) {
    if (!viewer) return;
    viewer->scroll_x = x;
    viewer->scroll_y = y;
}

void UIHTMLViewer_GetScroll(const ui_html_viewer_t *viewer, float *x, float *y) {
    if (!viewer || !x || !y) return;
    *x = viewer->scroll_x;
    *y = viewer->scroll_y;
}

int UIHTMLViewer_SetFont(ui_html_viewer_t *viewer, const char *font_path, DWORD size) {
    if (!viewer || !font_path) return -1;

    strncpy(viewer->font_path, font_path, 255);
    viewer->font_path[255] = '\0';
    viewer->font_size = size;

    if (viewer->html_ctx) {
        return html_context_set_font(viewer->html_ctx, font_path, size) ? 0 : -1;
    }

    return 0;
}

xmlNode* UIHTMLViewer_FindById(ui_html_viewer_t *viewer, const char *id) {
    if (!viewer || !viewer->html_ctx || !id) return NULL;
    return html_context_find_by_id(viewer->html_ctx, id);
}

void UIHTMLViewer_ScanKeyframes(ui_html_viewer_t *viewer) {
    if (!viewer || !viewer->html_ctx) return;
    html_context_scan_keyframes(viewer->html_ctx);
}

void UIHTMLViewer_ReapplyAnimations(ui_html_viewer_t *viewer) {
    if (!viewer || !viewer->html_ctx) return;
    html_context_reapply_animations(viewer->html_ctx);
}

void UIHTMLViewer_SetElementClicked(ui_html_viewer_t *viewer,
                                  html_viewer_element_clicked_fn callback,
                                  void *user_data) {
    if (!viewer) return;
    viewer->on_element_clicked = callback;
    viewer->callback_user_data = user_data;
}

void UIHTMLViewer_SetLinkClicked(ui_html_viewer_t *viewer,
                                html_viewer_link_clicked_fn callback,
                                void *user_data) {
    if (!viewer) return;
    viewer->on_link_clicked = callback;
    viewer->callback_user_data = user_data;
}

void UIHTMLViewer_Update(ui_html_viewer_t *viewer, int msec) {
    if (!viewer) return;
    html_viewer_update((ui_component_t *)viewer, msec);
}

void UIHTMLViewer_Render(ui_html_viewer_t *viewer) {
    if (!viewer) return;
    html_viewer_render((ui_component_t *)viewer);
}
