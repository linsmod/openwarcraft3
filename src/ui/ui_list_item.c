#include "ui_list_item.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void list_item_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_list_item_t *item = (ui_list_item_t *)component;
    if (!item) return;
    // 已在 UIListItem_Init 中初始化
}

static void list_item_shutdown(ui_component_t *component) {
    ui_list_item_t *item = (ui_list_item_t *)component;
    if (!item) return;
    // 清理资源
}

static void list_item_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
    // 可以添加动画逻辑
}

static void list_item_render(ui_component_t *component) {
    ui_list_item_t *item = (ui_list_item_t *)component;
    if (!item || !UIComponent_IsVisible(component)) return;

    float x = component->x;
    float y = component->y;
    float width = component->width;
    float height = component->height;

    // 确定当前状态
    int state = 0; // normal
    if (item->selected) state = 2; // active
    else if (item->hovered) state = 1; // hover

    // 选择颜色
    COLOR32 bg_color, text_color;
    if (item->use_custom_colors) {
        switch (state) {
            case 0: bg_color = item->bg_colors.normal; text_color = item->text_colors.normal; break;
            case 1: bg_color = item->bg_colors.hover; text_color = item->text_colors.hover; break;
            case 2: bg_color = item->bg_colors.active; text_color = item->text_colors.active; break;
            default: bg_color = item->bg_colors.disabled; text_color = item->text_colors.disabled; break;
        }
    } else {
        // 使用默认颜色
        bg_color = MAKE(COLOR32, 60, 60, 70, 255);
        text_color = MAKE(COLOR32, 220, 220, 220, 255);
        if (state == 1) {
            bg_color = MAKE(COLOR32, 80, 80, 90, 255);
            text_color = MAKE(COLOR32, 255, 255, 255, 255);
        } else if (state == 2) {
            bg_color = MAKE(COLOR32, 100, 100, 150, 255);
            text_color = MAKE(COLOR32, 255, 255, 255, 255);
        }
    }

    // 绘制边框（item 总是绘制边框）
    canvas2d_set_stroke_style(component->ctx, MAKE(COLOR32, 150, 150, 150, 255));
    canvas2d_set_line_width(component->ctx, UI_LIST_ITEM_BORDER_WIDTH);
    canvas2d_stroke_rect(component->ctx, x, y, width, height);

    // 绘制背景（item 总是绘制背景）
    canvas2d_set_fill_style(component->ctx, bg_color);
    canvas2d_fill_rect(component->ctx, x + UI_LIST_ITEM_BORDER_WIDTH, y + UI_LIST_ITEM_BORDER_WIDTH,
                      width - UI_LIST_ITEM_BORDER_WIDTH * 2, height - UI_LIST_ITEM_BORDER_WIDTH * 2);

    // 计算内容区域（去除边框和内边距）
    float content_x = x + UI_LIST_ITEM_BORDER_WIDTH + UI_LIST_ITEM_PADDING_LEFT;
    float content_y = y + UI_LIST_ITEM_BORDER_WIDTH + UI_LIST_ITEM_PADDING_TOP;
    float content_width = width - UI_LIST_ITEM_BORDER_WIDTH * 2 - UI_LIST_ITEM_PADDING_LEFT - UI_LIST_ITEM_PADDING_RIGHT;
    float content_height = height - UI_LIST_ITEM_BORDER_WIDTH * 2 - UI_LIST_ITEM_PADDING_TOP - UI_LIST_ITEM_PADDING_BOTTOM;

    // 使用自定义绘制回调绘制内容
    if (item->draw_callback) {
        item->draw_callback(component, item->text, item->draw_callback_user_data,
                          content_x, content_y, content_width, content_height,
                          item->selected, item->hovered);
        return;
    }

    // 默认绘制内容（文本）
    canvas2d_set_fill_style(component->ctx, text_color);
    canvas2d_set_font_size(component->ctx, item->font_size);
    canvas2d_fill_text(component->ctx, item->text,
                     content_x, content_y + item->font_size/2 + (content_height - item->font_size)/2);
}

static void list_item_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void list_item_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void list_item_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool list_item_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

static void list_item_on_mouse_enter(ui_component_t *component, event_t *event) {
    ui_list_item_t *item = (ui_list_item_t *)component;
    if (!item || !UIComponent_IsEnabled(component)) return;
    item->hovered = true;
}

static void list_item_on_mouse_leave(ui_component_t *component, event_t *event) {
    ui_list_item_t *item = (ui_list_item_t *)component;
    if (!item) return;
    item->hovered = false;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_list_item_vtable = {
    .init = list_item_init,
    .shutdown = list_item_shutdown,
    .update = list_item_update,
    .render = list_item_render,
    .set_position = list_item_set_position,
    .set_size = list_item_set_size,
    .set_bounds = list_item_set_bounds,
    .hit_test = list_item_hit_test,
    .on_mouse_enter = list_item_on_mouse_enter,
    .on_mouse_leave = list_item_on_mouse_leave,
    .on_mouse_down = NULL,
    .on_mouse_up = NULL,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = NULL,
    .on_mouse_wheel = NULL,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = NULL,
    .on_key_up = NULL,
    .on_text_input = NULL,
    .on_focus = NULL,
    .on_blur = NULL,
    .on_resize = NULL,
    .on_scroll = NULL,
    .add_child = NULL,
    .remove_child = NULL,
    .get_child_count = NULL,
    .get_child = NULL,
    .get_custom_data = NULL,
    .set_custom_data = NULL,
};

// ==================== 公共API实现 ====================

ui_list_item_t* UIListItem_Create(const char *text, void *user_data, canvas2d_context_t *ctx) {
    return UIListItem_CreateEx(text, user_data, 14.0f, NULL, NULL, ctx);
}

ui_list_item_t* UIListItem_CreateEx(const char *text, void *user_data, float font_size,
                                   const UICOLOR32 *bg_colors, const UICOLOR32 *text_colors,
                                   canvas2d_context_t *ctx) {
    ui_list_item_t *item = malloc(sizeof(ui_list_item_t));
    if (!item) return NULL;

    if (UIListItem_Init(item, text, user_data, font_size, ctx) != 0) {
        free(item);
        return NULL;
    }

    // 设置颜色
    if (bg_colors && text_colors) {
        item->use_custom_colors = true;
        item->bg_colors = *bg_colors;
        item->text_colors = *text_colors;
    }

    return item;
}

int UIListItem_Init(ui_list_item_t *item, const char *text, void *user_data,
                   float font_size, canvas2d_context_t *ctx) {
    if (!item || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&item->base, UI_COMPONENT_TYPE_LIST_ITEM, &g_list_item_vtable, ctx);

    // 初始化文本
    if (text) {
        strncpy(item->text, text, 255);
        item->text[255] = '\0';
    } else {
        item->text[0] = '\0';
    }

    item->user_data = user_data;
    item->selected = false;
    item->hovered = false;
    item->use_custom_colors = false;
    item->font_size = font_size;
    item->font_flags = 0;
    item->draw_callback = NULL;
    item->draw_callback_user_data = NULL;

    return 0;
}

void UIListItem_Shutdown(ui_list_item_t *item) {
    if (!item) return;

    // 调用虚函数shutdown
    if (item->base.vtable && item->base.vtable->shutdown) {
        item->base.vtable->shutdown(&item->base);
    }

    // 关闭基础组件
    UIComponent_ShutdownBase(&item->base);
}

void UIListItem_Destroy(ui_list_item_t *item) {
    if (!item) return;
    UIListItem_Shutdown(item);
    free(item);
}

void UIListItem_SetText(ui_list_item_t *item, const char *text) {
    if (!item || !text) return;
    strncpy(item->text, text, 255);
    item->text[255] = '\0';
}

const char* UIListItem_GetText(const ui_list_item_t *item) {
    return item ? item->text : "";
}

void UIListItem_SetUserData(ui_list_item_t *item, void *user_data) {
    if (!item) return;
    item->user_data = user_data;
}

void* UIListItem_GetUserData(const ui_list_item_t *item) {
    return item ? item->user_data : NULL;
}

void UIListItem_SetSelected(ui_list_item_t *item, bool selected) {
    if (!item) return;
    item->selected = selected;
}

bool UIListItem_IsSelected(const ui_list_item_t *item) {
    return item ? item->selected : false;
}

void UIListItem_SetHovered(ui_list_item_t *item, bool hovered) {
    if (!item) return;
    item->hovered = hovered;
}

bool UIListItem_IsHovered(const ui_list_item_t *item) {
    return item ? item->hovered : false;
}

void UIListItem_SetColors(ui_list_item_t *item, const UICOLOR32 *bg_colors, const UICOLOR32 *text_colors) {
    if (!item || !bg_colors || !text_colors) return;
    item->use_custom_colors = true;
    item->bg_colors = *bg_colors;
    item->text_colors = *text_colors;
}

void UIListItem_SetBgColors(ui_list_item_t *item, const UICOLOR32 *colors) {
    if (!item || !colors) return;
    item->use_custom_colors = true;
    item->bg_colors = *colors;
}

void UIListItem_SetTextColors(ui_list_item_t *item, const UICOLOR32 *colors) {
    if (!item || !colors) return;
    item->use_custom_colors = true;
    item->text_colors = *colors;
}

void UIListItem_SetFontSize(ui_list_item_t *item, float font_size) {
    if (!item) return;
    item->font_size = font_size;
}

float UIListItem_GetFontSize(const ui_list_item_t *item) {
    return item ? item->font_size : 0.0f;
}

void UIListItem_SetDrawCallback(ui_list_item_t *item, ui_list_item_draw_callback_t callback, void *user_data) {
    if (!item) return;
    item->draw_callback = callback;
    item->draw_callback_user_data = user_data;
}

void UIListItem_Update(ui_list_item_t *item, int msec) {
    if (!item) return;
    if (item->base.vtable && item->base.vtable->update) {
        item->base.vtable->update(&item->base, msec);
    }
}

void UIListItem_Render(ui_list_item_t *item) {
    if (!item) return;
    if (item->base.vtable && item->base.vtable->render) {
        item->base.vtable->render(&item->base);
    }
}
