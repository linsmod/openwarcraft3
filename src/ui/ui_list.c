#include "ui_list.h"
#include "common/shared.h"
#include "ui_event_dispatcher.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// ==================== 虚函数实现 ====================

static void list_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return;
    // visible_count 会在 UIList_Create 中设置完尺寸后计算
}

static void list_shutdown(ui_component_t *component) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return;

    // 销毁所有列表项
    for (int i = 0; i < list->item_count; i++) {
        if (list->items[i]) {
            UIListItem_Destroy(list->items[i]);
            list->items[i] = NULL;
        }
    }
    list->item_count = 0;
}

static void list_update(ui_component_t *component, int msec) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return;
    (void)msec;

    // 处理待更新的选中项
    if (list->pending_selected_index >= 0) {
        int old_index = list->selected_index;
        int new_index = list->pending_selected_index;
        list->selected_index = new_index;
        list->pending_selected_index = -1;  // 清除待处理标志

        // 确保选中项可见
        if (new_index < list->scroll_offset) {
            list->scroll_offset = new_index;
        } else if (new_index >= list->scroll_offset + list->visible_count) {
            list->scroll_offset = new_index - list->visible_count + 1;
        }

        int max_offset = list->item_count - list->visible_count;
        if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;

        // 触发回调
        if (list->on_selected_changed && old_index != new_index) {
            list->on_selected_changed(list, new_index, list->callback_user_data);
        }
    }
}

static void list_render(ui_component_t *component) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsVisible(component)) return;

    // 绘制边框
    canvas2d_set_stroke_style(component->ctx, list->border_color);
    canvas2d_set_line_width(component->ctx, 1.0f);
    canvas2d_stroke_rect(component->ctx, component->x, component->y, component->width, component->height);

    // 裁剪区域
    // canvas2d_save(component->ctx);
    // canvas2d_begin_path(component->ctx);
    // canvas2d_rect(component->ctx, component->x, component->y, component->width, component->height);
    // canvas2d_clip(component->ctx);

    // 绘制列表项（只渲染可见区域）
    float item_y = component->y;
    int start_index = list->scroll_offset;
    int end_index = start_index + list->visible_count;
    if (end_index > list->item_count) end_index = list->item_count;

    for (int i = start_index; i < end_index; i++) {
        ui_list_item_t *item = list->items[i];
        if (!item) continue;

        bool selected = (i == list->selected_index);
        UIListItem_SetSelected(item, selected);

        // 设置item的位置和尺寸
        item->base.x = component->x + 1;
        item->base.y = item_y;
        item->base.width = component->width - 2;
        item->base.height = list->item_height;
        // 渲染item
        UIListItem_Render(item);

        item_y += list->item_height + list->item_spacing;
    }

    // 恢复裁剪
    // canvas2d_restore(component->ctx);

    // 绘制滚动条
    if (list->show_scrollbar && list->item_count > list->visible_count) {
        float scrollbar_x = component->x + component->width - 12;
        float scrollbar_width = 10;
        float track_y = component->y;
        float track_height = component->height;
        int max_scroll = list->item_count - list->visible_count;
        float thumb_height = track_height * list->visible_count / list->item_count;
        float thumb_y = track_y + (track_height - thumb_height) * list->scroll_offset / max_scroll;

        // 滚动槽背景
        canvas2d_set_fill_style(component->ctx, MAKE(COLOR32, 80, 80, 90, 255));
        canvas2d_fill_rect(component->ctx, scrollbar_x, track_y, scrollbar_width, track_height);

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
static bool list_on_mouse_move(ui_component_t *component, event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    // 处理滚动条拖动
    if (list->is_dragging_scrollbar) {
        int max_scroll = list->item_count - list->visible_count;
        float track_height = component->height;
        float delta_y = event->mouse.y - list->scrollbar_drag_start_y;
        float thumb_height = track_height * list->visible_count / list->item_count;
        
        // 计算新的滚动偏移
        float delta_ratio = delta_y / (track_height - thumb_height);
        int new_offset = list->scrollbar_drag_start_offset + (int)(delta_ratio * max_scroll);
        
        // 限制范围
        if (new_offset < 0) new_offset = 0;
        if (new_offset > max_scroll) new_offset = max_scroll;
        
        list->scroll_offset = new_offset;
        return true;
    }

    // 更新所有可见items的hover状态
    float item_y = component->y;
    for (int i = 0; i < list->visible_count; i++) {
        int item_index = list->scroll_offset + i;
        if (item_index >= list->item_count) break;

        ui_list_item_t *item = list->items[item_index];
        if (!item) continue;

        bool was_hovered = UIListItem_IsHovered(item);
        bool is_hovered = (event->mouse.y >= item_y && event->mouse.y < item_y + list->item_height);

        if (is_hovered && !was_hovered) {
            // 鼠标进入item
            UIListItem_SetHovered(item, true);
        } else if (!is_hovered && was_hovered) {
            // 鼠标离开item
            UIListItem_SetHovered(item, false);
        }

        item_y += list->item_height + list->item_spacing;
    }

    return true;
}

static bool list_on_mouse_up(ui_component_t *component, event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list) return false;
    
    // 结束滚动条拖动
    if (list->is_dragging_scrollbar) {
        list->is_dragging_scrollbar = false;
        // 释放鼠标捕获
        if (list->dispatcher) {
            UIEventDispatcher_ReleaseMouse(list->dispatcher);
        }
    }
    return true;
}

static bool list_on_mouse_down(ui_component_t *component, event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    // 检查是否点击滚动条
    if (list->show_scrollbar && list->item_count > list->visible_count) {
        float scrollbar_x = component->x + component->width - 12;
        float scrollbar_width = 10;
        
        if (event->mouse.x >= scrollbar_x && event->mouse.x < scrollbar_x + scrollbar_width) {
            // 点击了滚动条，计算滚动条拇指位置
            int max_scroll = list->item_count - list->visible_count;
            float track_height = component->height;
            float thumb_height = track_height * list->visible_count / list->item_count;
            float thumb_y = component->y + (track_height - thumb_height) * list->scroll_offset / max_scroll;
            
            if (event->mouse.y >= thumb_y && event->mouse.y < thumb_y + thumb_height) {
                // 点击了拇指，开始拖动
                list->is_dragging_scrollbar = true;
                list->scrollbar_drag_start_y = event->mouse.y;
                list->scrollbar_drag_start_offset = list->scroll_offset;
                // 捕获鼠标，防止鼠标移出范围后丢失事件
                if (list->dispatcher) {
                    UIEventDispatcher_CaptureMouse(list->dispatcher, component);
                }
                return true;
            } else {
                // 点击了滚动槽，跳转到点击位置
                float click_ratio = (event->mouse.y - component->y) / track_height;
                int new_offset = (int)(click_ratio * max_scroll + 0.5f);
                if (new_offset < 0) new_offset = 0;
                if (new_offset > max_scroll) new_offset = max_scroll;
                list->scroll_offset = new_offset;
                list->is_dragging_scrollbar = true;
                list->scrollbar_drag_start_y = event->mouse.y;
                list->scrollbar_drag_start_offset = list->scroll_offset;
                // 捕获鼠标，防止鼠标移出范围后丢失事件
                if (list->dispatcher) {
                    UIEventDispatcher_CaptureMouse(list->dispatcher, component);
                }
                return true;
            }
        }
    }

    // 检查是否点击列表项
    float item_y = component->y;
    for (int i = 0; i < list->visible_count; i++) {
        int item_index = list->scroll_offset + i;
        if (item_index >= list->item_count) break;

        ui_list_item_t *item = list->items[item_index];
        if (event->mouse.y >= item_y && event->mouse.y < item_y + list->item_height) {
            list->pending_selected_index = item_index;
            return true;
        }

        item_y += list->item_height + list->item_spacing;
    }

    return false;
}

static bool list_on_mouse_wheel(ui_component_t *component, event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    int max_offset = list->item_count - list->visible_count;
    int scroll_delta = -event->wheel.delta; // 标准化滚轮值

    list->scroll_offset += scroll_delta;
    if (list->scroll_offset < 0) list->scroll_offset = 0;
    if (list->scroll_offset > max_offset) list->scroll_offset = max_offset;

    return true;
}

static bool list_on_key_down(ui_component_t *component, event_t *event) {
    ui_list_t *list = (ui_list_t *)component;
    if (!list || !UIComponent_IsEnabled(component)) return false;

    switch (event->key.key) {
        case SDLK_UP: // 上箭头
            if (list->selected_index > 0) {
                list->pending_selected_index = list->selected_index - 1;
                return true;
            }
            break;
        case SDLK_DOWN: // 下箭头
            if (list->selected_index < list->item_count - 1) {
                list->pending_selected_index = list->selected_index + 1;
                return true;
            }
            break;
        case SDLK_PAGEUP: // Page Up
            if (list->item_count > 0) {
                int new_offset = list->scroll_offset - (list->visible_count - 1);
                if (new_offset < 0) new_offset = 0;
                list->pending_selected_index = new_offset;
            }
            return true;
        case SDLK_PAGEDOWN: // Page Down
            if (list->item_count > 0) {
                int max_offset = list->item_count - list->visible_count;
                if (max_offset < 0) max_offset = 0;
                int new_offset = list->scroll_offset + (list->visible_count - 1);
                if (new_offset > max_offset) new_offset = max_offset;
                int new_index = new_offset + list->visible_count - 1;
                if (new_index >= list->item_count) new_index = list->item_count - 1;
                list->pending_selected_index = new_index;
            }
            return true;
        case SDLK_HOME: // Home
            if (list->item_count > 0) {
                list->pending_selected_index = 0;
            }
            return true;
        case SDLK_END: // End
            if (list->item_count > 0) {
                list->pending_selected_index = list->item_count - 1;
            }
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
    .on_mouse_up = list_on_mouse_up,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = list_on_mouse_move,
    .on_mouse_wheel = list_on_mouse_wheel,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = list_on_key_down,
    .on_key_up = NULL,
    .on_text_input = NULL,
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
    list->dispatcher = NULL;  // 初始化为NULL，需要在创建后设置
    list->item_height = item_height;
    list->item_spacing = 2.0f;
    list->font_size = font_size;
    UIComponent_SetBgColor(&list->base, MAKE(COLOR32, 60, 60, 70, 255));
    list->border_color = MAKE(COLOR32, 120, 120, 130, 255);
    list->show_scrollbar = true;
    list->is_dragging_scrollbar = false;
    list->scrollbar_drag_start_y = 0.0f;
    list->scrollbar_drag_start_offset = 0;

    // 计算可见项数（必须在设置完尺寸后）
    list->visible_count = (int)(list->base.height / (list->item_height + list->item_spacing));
    if (list->visible_count < 1) list->visible_count = 1;

// 启用焦点、Tab访问和拖拽
    list->base.flags |= UI_FLAG_ACCEPT_FOCUS | UI_FLAG_TAB_STOP | UI_FLAG_DRAGGABLE;

    return list;
}

int UIList_Init(ui_list_t *list, canvas2d_context_t *ctx) {
    if (!list || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&list->base, UI_COMPONENT_TYPE_LIST, &g_list_vtable, ctx);

    list->dispatcher = NULL;
    list->on_selected_changed = NULL;
    list->callback_user_data = NULL;
    list->item_height = 24.0f;
    list->item_spacing = 2.0f;
    list->font_size = 14.0f;
    UIComponent_SetBgColor(&list->base, MAKE(COLOR32, 60, 60, 70, 255));
    list->border_color = MAKE(COLOR32, 120, 120, 130, 255);
    list->show_scrollbar = true;
    list->is_dragging_scrollbar = false;
    list->scrollbar_drag_start_y = 0.0f;
    list->scrollbar_drag_start_offset = 0;

    // 初始化状态
    list->item_count = 0;
    list->selected_index = -1;
    list->pending_selected_index = -1;
    list->scroll_offset = 0;
    list->visible_count = 0;
    list->scroll_pos = 0;

// 启用焦点、Tab访问和拖拽
    list->base.flags |= UI_FLAG_ACCEPT_FOCUS | UI_FLAG_TAB_STOP | UI_FLAG_DRAGGABLE;

    return 0;
}

int UIList_AddItem(ui_list_t *list, const char *text, void *user_data) {
    if (!list || list->item_count >= UI_LIST_MAX_ITEMS) {
        return -1;
    }

    // 创建新的列表项
    ui_list_item_t *item = UIListItem_CreateEx(text, user_data, list->font_size, NULL, NULL, list->base.ctx);
    if (!item) {
        return -1;
    }

    // 设置item的字体大小
    UIListItem_SetFontSize(item, list->font_size);

    list->items[list->item_count] = item;
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

    // 销毁所有列表项
    for (int i = 0; i < list->item_count; i++) {
        if (list->items[i]) {
            UIListItem_Destroy(list->items[i]);
            list->items[i] = NULL;
        }
    }
    list->item_count = 0;
    list->selected_index = -1;
    list->scroll_offset = 0;
}

void UIList_SetSelected(ui_list_t *list, int index) {
    if (!list || index < 0 || index >= list->item_count) {
        return;
    }

    // 只更新状态，实际索引在update中更新并触发回调
    if (list->selected_index != index) {
        list->pending_selected_index = index;
    }
}

int UIList_GetSelected(ui_list_t *list) {
    return list ? list->selected_index : -1;
}

const char* UIList_GetSelectedText(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    ui_list_item_t *item = list->items[list->selected_index];
    return item ? UIListItem_GetText(item) : NULL;
}

void* UIList_GetSelectedUserData(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    ui_list_item_t *item = list->items[list->selected_index];
    return item ? UIListItem_GetUserData(item) : NULL;
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

// 获取列表项
ui_list_item_t* UIList_GetItem(ui_list_t *list, int index) {
    if (!list || index < 0 || index >= list->item_count) {
        return NULL;
    }
    return list->items[index];
}

// 设置默认item样式
void UIList_SetDefaultItemStyle(ui_list_t *list, float font_size) {
    if (!list) return;
    list->font_size = font_size;
    // 更新所有现有items
    for (int i = 0; i < list->item_count; i++) {
        if (list->items[i]) {
            UIListItem_SetFontSize(list->items[i], font_size);
        }
    }
}

// 设置选中项改变回调函数
void UIList_SetSelectedChangedCallback(ui_list_t *list, ui_list_on_selected_changed_t callback, void *user_data) {
    if (!list) return;
    list->on_selected_changed = callback;
    list->callback_user_data = user_data;
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

// 设置事件分发器（用于鼠标捕获功能）
void UIList_SetDispatcher(ui_list_t *list, ui_event_dispatcher_t *dispatcher) {
    if (!list) return;
    list->dispatcher = dispatcher;
}
