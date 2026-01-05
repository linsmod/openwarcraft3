#include "ui_html_viewer.h"
#include "../html/html.h"
#include "../html/html_context.h"
#include "common/event.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 虚函数
static void html_viewer_init(ui_component_t *component, canvas2d_context_t *ctx);
static void html_viewer_shutdown(ui_component_t *component);
static void html_viewer_update(ui_component_t *component, int msec);
static void html_viewer_render(ui_component_t *component);
static ui_component_t* html_viewer_hit_test(ui_component_t *component, float x, float y);
static void html_viewer_on_click(ui_component_t *component, event_t *event);
static void html_viewer_print_tree(const ui_component_t *component, int indent, const char *common);

static const ui_component_vtable_t html_viewer_vtable = {
    .init = html_viewer_init,
    .shutdown = html_viewer_shutdown,
    .update = html_viewer_update,
    .render = html_viewer_render,
    .hit_test = html_viewer_hit_test,
    .on_click = html_viewer_on_click,
    .print_tree = html_viewer_print_tree
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

static ui_component_t* html_viewer_hit_test(ui_component_t *component, float x, float y) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    // 首先检查是否在viewer的边界内
    if (x < component->x || x >= component->x + component->width ||
        y < component->y || y >= component->y + component->height) {
        return NULL;
    }

    // 如果没有加载HTML或没有context，返回viewer自身（可能需要交互）
    if (!viewer->loaded || !viewer->html_ctx) {
        return component;
    }

    // 将全局坐标转换为viewer的本地坐标
    float local_x = x - component->x - viewer->scroll_x;
    float local_y = y - component->y - viewer->scroll_y;

    // 应用zoom缩放
    if (viewer->zoom != 1.0f) {
        local_x /= viewer->zoom;
        local_y /= viewer->zoom;
    }

    // 使用hittest查找命中的HTML节点
    xmlNode *hit_node = html_context_find_by_point(viewer->html_ctx, local_x, local_y);

    if (hit_node) {
        // 获取节点的私有数据（ui_xmlnode_t）
        ui_component_t *node_component = (ui_component_t *)hit_node->_private;
        
        // 如果节点有对应的组件，返回该组件
        if (node_component) {
            return node_component;
        }
    }

    // 如果没有命中任何节点，返回viewer自身（viewer也可以响应点击）
    return component;
}

static void html_viewer_on_click(ui_component_t *component, event_t *event) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)component;

    // 如果没有加载HTML，不处理
    if (!viewer->html_ctx) {
        return;
    }

    // 将全局坐标转换为viewer的本地坐标
    float local_x = event->mouse.x - component->x - viewer->scroll_x;
    float local_y = event->mouse.y - component->y - viewer->scroll_y;

    // 应用zoom缩放
    if (viewer->zoom != 1.0f) {
        local_x /= viewer->zoom;
        local_y /= viewer->zoom;
    }

    // 使用hittest查找被点击的元素
    xmlNode *elem = html_context_find_by_point(viewer->html_ctx, local_x, local_y);

    // 处理元素点击
    if (elem && viewer->on_element_clicked) {
        viewer->on_element_clicked(viewer, elem, viewer->callback_user_data);
        return; // 处理了元素点击，不再处理链接点击
    }

    // 处理链接点击（如果元素是<a>标签且有点击链接回调）
    if (elem && elem->name && xmlStrcmp(elem->name, BAD_CAST "a") == 0) {
        if (viewer->on_link_clicked) {
            xmlChar *href = xmlGetProp(elem, BAD_CAST "href");
            if (href) {
                viewer->on_link_clicked(viewer, (char*)href, viewer->callback_user_data);
                xmlFree(href);
            }
        }
    }
}

static void html_viewer_print_tree(const ui_component_t *component, int indent, const char *common) {
    const ui_html_viewer_t *viewer = (const ui_html_viewer_t *)component;
    
    // 打印自身的 common 信息（由 UIComponent_PrintTree 提供）
    printf("%s\n", common);
    
    // 如果已加载 HTML，打印内部布局树
    if (viewer && viewer->loaded && viewer->html_ctx) {
        html_context_print_layout_info(viewer->html_ctx, indent + 1);
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
        ui_component_t* viewer_comp = (ui_component_t*)viewer;
        viewer->html_ctx = html_context_create(viewer_comp->width,viewer_comp->height);
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
                               size_t length,int width,int height) {
    if (!viewer || !html_data || length == 0) return -1;

    if (!viewer->html_ctx) {
        viewer->html_ctx = html_context_create(width,height);
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

void UIHTMLViewer_PrintLayoutTree(ui_html_viewer_t *viewer) {
    if (!viewer) {
        printf("HTML Viewer is NULL\n");
        return;
    }
    
    if (!viewer->loaded || !viewer->html_ctx) {
        printf("HTML Viewer: No HTML loaded\n");
        return;
    }
    
    printf("=== HTML Viewer Layout Tree ===\n");
    printf("File: %s\n", viewer->html_filename);
    printf("Zoom: %.2f, Scroll: (%.2f, %.2f)\n", 
           viewer->zoom, viewer->scroll_x, viewer->scroll_y);
    printf("-----------------------------\n");
    
    html_context_print_layout_info(viewer->html_ctx,0);
}
