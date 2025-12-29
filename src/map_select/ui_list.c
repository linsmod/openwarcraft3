#include "ui_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void list_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return;

    // 计算可见项数
    list->visible_count = (int)((component->height - list->item_spacing) / (list->item_height + list->item_spacing));
    if (list->visible_count < 1) list->visible_count = 1;
}

static void list_shutdown(ui_component_t *component) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return;

    UIList_ClearItems(list);
}

static void list_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
    // 可以添加滚动动画逻辑
}

static void list_render(ui_component_t *component) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsVisible(component)) return;

    // 使用基础组件背景渲染
    UIComponent_RenderBackground(component);

    // 绘制边框
    canvas2d_set_stroke_style(component->ctx, list->border_color);
    canvas2d_set_line_width(component->ctx, 1.0f);
    canvas2d_stroke_rect(component->ctx, component->x, component->y, component->width, component->height);

    // 裁剪区域
    // canvas2d_save(component->ctx);
    // canvas2d_begin_path(component->ctx);
    // canvas2d_rect(component->ctx, component->x, component->y, component->width, component->height);
    // canvas2d_clip(component->ctx);

    // 绘制列表项
    float item_y = component->y;
    for (int i = 0; i < list->item_count; i++) {
        int display_index = i + list->scroll_offset;

        if (display_index >= list->item_count) break;
        if (item_y + list->item_height > component->y + component->height) break;

        bool selected = (display_index == list->selected_index);

        if (list->draw_callback) {
            list->draw_callback(display_index, list->items[display_index].text, list->user_data,
                              component->x, item_y, component->width, list->item_height, selected);
        } else {
            // 默认绘制
            if (selected) {
                canvas2d_set_fill_style(component->ctx, list->selected_bg_color);
                canvas2d_fill_rect(component->ctx, component->x + 2, item_y, component->width - 4, list->item_height);
                canvas2d_set_fill_style(component->ctx, list->selected_text_color);
            } else {
                canvas2d_set_fill_style(component->ctx, list->text_color);
            }

            canvas2d_set_font_size(component->ctx, list->font_size);
            canvas2d_fill_text(component->ctx, list->items[display_index].text,
                             component->x + 5, item_y + list->font_size);
        }

        item_y += list->item_height + list->item_spacing;
    }

    // 恢复裁剪
    // canvas2d_restore(component->ctx);

    // 绘制滚动条
    if (list->show_scrollbar && list->item_count > list->visible_count) {
        float scrollbar_x = component->x + component->width - 12;
        float scrollbar_width = 10;
        float scrollbar_height = component->height;
        float track_height = scrollbar_height - 20;
        float thumb_height = track_height * list->visible_count / list->item_count;
        float thumb_y = component->y + 10 + (track_height - thumb_height) * list->scroll_offset / (list->item_count - list->visible_count);

        // 滚动槽背景
        canvas2d_set_fill_style(component->ctx, MAKE(COLOR32, 80, 80, 90, 255));
        canvas2d_fill_rect(component->ctx, scrollbar_x, component->y + 5, scrollbar_width, scrollbar_height - 10);

        // 滚动条拇指
        canvas2d_set_fill_style(component->ctx, MAKE(COLOR32, 120, 120, 130, 255));
        canvas2d_fill_rect(component->ctx, scrollbar_x + 1, thumb_y, scrollbar_width - 2, thumb_height);
    }
}

static void list_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void list_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void list_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool list_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

// 列表特定的鼠标事件处理
static bool list_on_mouse_down(ui_component_t *component, ui_mouse_event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    // 检查是否点击列表项
    float item_y = component->y;
    for (int i = 0; i < list->visible_count; i++) {
        int item_index = list->scroll_offset + i;
        if (item_index >= list->item_count) break;

        if (event->y >= item_y && event->y < item_y + list->item_height) {
            UIList_SetSelected(list, item_index);
            return true;
        }

        item_y += list->item_height + list->item_spacing;
    }

    return false;
}

static bool list_on_mouse_wheel(ui_component_t *component, ui_mouse_event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    int max_offset = list->item_count - list->visible_count;
    int scroll_delta = -event->delta / 120; // 标准化滚轮值

    list->scroll_offset += scroll_delta;
    if (list->scroll_offset < 0) list->scroll_offset = 0;
    if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;

    return true;
}

static bool list_on_key_down(ui_component_t *component, ui_keyboard_event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    int max_offset = list->item_count - list->visible_count;

    switch (event->key) {
        case 0x40000050: // 上箭头
            if (list->selected_index > 0) {
                list->selected_index--;
                if (list->selected_index < list->scroll_offset) {
                    list->scroll_offset = list->selected_index;
                }
                return true;
            }
            break;
        case 0x40000051: // 下箭头
            if (list->selected_index < list->item_count - 1) {
                list->selected_index++;
                if (list->selected_index >= list->scroll_offset + list->visible_count) {
                    list->scroll_offset = list->selected_index - list->visible_count + 1;
                }
                return true;
            }
            break;
        case 0x40000052: // Page Up
            list->scroll_offset -= list->visible_count - 1;
            if (list->scroll_offset < 0) list->scroll_offset = 0;
            return true;
        case 0x40000053: // Page Down
            list->scroll_offset += list->visible_count - 1;
            if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;
            return true;
        case 0x40000049: // Home
            list->scroll_offset = 0;
            return true;
        case 0x4000004D: // End
            list->scroll_offset = max_offset;
            return true;
    }

    return false;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_list_vtable = {
    .init = list_init,
    .shutdown = list_shutdown,
    .update = list_update,
    .render = list_render,
    .set_position = list_set_position,
    .set_size = list_set_size,
    .set_bounds = list_set_bounds,
    .hit_test = list_hit_test,
    .on_mouse_enter = NULL,
    .on_mouse_leave = NULL,
    .on_mouse_down = list_on_mouse_down,
    .on_mouse_up = NULL,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = NULL,
    .on_mouse_wheel = list_on_mouse_wheel,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = list_on_key_down,
    .on_key_up = NULL,
    .on_key_press = NULL,
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

ui_list_t* UIList_Create(float x, float y, float width, float height,
                        float item_height, float font_size, canvas2d_context_t *ctx) {
    ui_list_t *list = malloc(sizeof(ui_list_t));
    if (!list) return NULL;

    if (UIList_Init(list, ctx) != 0) {
        free(list);
        return NULL;
    }

    list->base.x = x;
    list->base.y = y;
    list->base.width = width;
    list->base.height = height;
    list->item_height = item_height;
    list->item_spacing = 2.0f;
    list->font_size = font_size;
    list->base.bg_color = MAKE(COLOR32, 60, 60, 70, 255);
    list->selected_bg_color = MAKE(COLOR32, 100, 100, 150, 255);
    list->border_color = MAKE(COLOR32, 120, 120, 130, 255);
    list->text_color = MAKE(COLOR32, 220, 220, 220, 255);
    list->selected_text_color = MAKE(COLOR32, 255, 255, 255, 255);
    list->show_scrollbar = true;
    list->draw_callback = NULL;
    list->user_data = NULL;

    // 启用焦点和Tab访问
    list->base.flags |= UI_FLAG_ACCEPT_FOCUS | UI_FLAG_TAB_STOP;

    return list;
}

int UIList_Init(ui_list_t *list, canvas2d_context_t *ctx) {
    if (!list || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&list->base, UI_COMPONENT_TYPE_LIST, &g_list_vtable, ctx);

    list->item_height = 24.0f;
    list->item_spacing = 2.0f;
    list->font_size = 14.0f;
    list->base.bg_color = MAKE(COLOR32, 60, 60, 70, 255);
    list->selected_bg_color = MAKE(COLOR32, 100, 100, 150, 255);
    list->border_color = MAKE(COLOR32, 120, 120, 130, 255);
    list->text_color = MAKE(COLOR32, 220, 220, 220, 255);
    list->selected_text_color = MAKE(COLOR32, 255, 255, 255, 255);
    list->show_scrollbar = true;
    list->draw_callback = NULL;
    list->user_data = NULL;

    // 初始化状态
    list->item_count = 0;
    list->selected_index = -1;
    list->scroll_offset = 0;
    list->visible_count = 0;
    list->scroll_pos = 0;

    // 启用焦点和Tab访问
    list->base.flags |= UI_FLAG_ACCEPT_FOCUS | UI_FLAG_TAB_STOP;

    return 0;
}

int UIList_AddItem(ui_list_t *list, const char *text, void *user_data) {
    if (!list || list->item_count >= UI_LIST_MAX_ITEMS) {
        return -1;
    }

    strncpy(list->items[list->item_count].text, text, 255);
    list->items[list->item_count].text[255] = '\0';
    list->items[list->item_count].user_data = user_data;

    int index = list->item_count;
    list->item_count++;

    // 如果这是第一个项，自动选中
    if (list->selected_index == -1) {
        list->selected_index = 0;
    }

    return index;
}

void UIList_ClearItems(ui_list_t *list) {
    if (!list) return;

    list->item_count = 0;
    list->selected_index = -1;
    list->scroll_offset = 0;
}

void UIList_SetSelected(ui_list_t *list, int index) {
    if (!list || index < 0 || index >= list->item_count) {
        return;
    }

    list->selected_index = index;

    // 确保选中项可见
    if (index < list->scroll_offset) {
        list->scroll_offset = index;
    } else if (index >= list->scroll_offset + list->visible_count) {
        list->scroll_offset = index - list->visible_count + 1;
    }

    int max_offset = list->item_count - list->visible_count;
    if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;
}

int UIList_GetSelected(ui_list_t *list) {
    return list ? list->selected_index : -1;
}

const char* UIList_GetSelectedText(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    return list->items[list->selected_index].text;
}

void* UIList_GetSelectedUserData(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    return list->items[list->selected_index].user_data;
}

int UIList_GetItemCount(ui_list_t *list) {
    return list ? list->item_count : 0;
}

void UIList_SetScrollOffset(ui_list_t *list, int offset) {
    if (!list) return;

    int max_offset = list->item_count - list->visible_count;
    if (max_offset < 0) max_offset = 0;

    list->scroll_offset = offset;
    if (list->scroll_offset < 0) list->scroll_offset = 0;
    if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;
}

int UIList_GetScrollOffset(ui_list_t *list) {
    return list ? list->scroll_offset : 0;
}

void UIList_SetDrawCallback(ui_list_t *list, ui_list_item_draw_callback_t callback, void *user_data) {
    if (!list) return;
    list->draw_callback = callback;
    list->user_data = user_data;
}

void UIList_Update(ui_list_t *list, int msec) {
    if (!list) return;
    if (list->base.vtable && list->base.vtable->update) {
        list->base.vtable->update(&list->base, msec);
    }
}

void UIList_Render(ui_list_t *list) {
    if (!list) return;
    if (list->base.vtable && list->base.vtable->render) {
        list->base.vtable->render(&list->base);
    }
}

void UIList_Shutdown(ui_list_t *list) {
    if (!list) return;

    if (list->base.vtable && list->base.vtable->shutdown) {
        list->base.vtable->shutdown(&list->base);
    }

    UIComponent_ShutdownBase(&list->base);
}

void UIList_Destroy(ui_list_t *list) {
    if (!list) return;
    UIList_Shutdown(list);
    free(list);
}
