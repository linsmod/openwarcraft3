#include "ui_component.h"
#include <stdlib.h>
#include <string.h>

// 双击时间间隔（毫秒）
#define DOUBLE_CLICK_TIME 500

// ==================== 事件操作 ====================

void UIEvent_StopPropagation(ui_event_t *event) {
    if (event) {
        event->propagation_stopped = true;
    }
}

void UIEvent_PreventDefault(ui_event_t *event) {
    if (event) {
        event->default_prevented = true;
    }
}

// ==================== 组件标志操作 ====================

bool UIComponent_IsVisible(const ui_component_t *component) {
    return component ? (component->flags & UI_FLAG_VISIBLE) != 0 : false;
}

bool UIComponent_IsEnabled(const ui_component_t *component) {
    return component ? (component->flags & UI_FLAG_ENABLED) != 0 : false;
}

bool UIComponent_IsFocused(const ui_component_t *component) {
    return component ? (component->flags & UI_FLAG_FOCUSED) != 0 : false;
}

bool UIComponent_IsHovered(const ui_component_t *component) {
    return component ? (component->flags & UI_FLAG_HOVERED) != 0 : false;
}

void UIComponent_SetVisible(ui_component_t *component, bool visible) {
    if (component) {
        if (visible) {
            component->flags |= UI_FLAG_VISIBLE;
        } else {
            component->flags &= ~UI_FLAG_VISIBLE;
        }
    }
}

void UIComponent_SetEnabled(ui_component_t *component, bool enabled) {
    if (component) {
        if (enabled) {
            component->flags |= UI_FLAG_ENABLED;
        } else {
            component->flags &= ~UI_FLAG_ENABLED;
        }
    }
}

void UIComponent_SetFocused(ui_component_t *component, bool focused) {
    if (component) {
        if (focused) {
            component->flags |= UI_FLAG_FOCUSED;
        } else {
            component->flags &= ~UI_FLAG_FOCUSED;
        }
    }
}

// ==================== 事件处理 ====================

bool UIComponent_AddEventHandler(ui_component_t *component, ui_event_type_t event_type, ui_event_handler_t handler, void *user_data) {
    if (!component || event_type < 0 || event_type >= 32) {
        return false;
    }

    component->event_handlers[event_type] = handler;
    component->event_handler_user_data[event_type] = user_data;
    return true;
}

bool UIComponent_RemoveEventHandler(ui_component_t *component, ui_event_type_t event_type) {
    if (!component || event_type < 0 || event_type >= 32) {
        return false;
    }

    component->event_handlers[event_type] = NULL;
    component->event_handler_user_data[event_type] = NULL;
    return true;
}

bool UIComponent_TriggerEvent(ui_component_t *component, ui_event_t *event) {
    if (!component || !event) {
        return false;
    }

    // 初始化事件
    event->target = component;
    event->current_target = component;
    event->propagation_stopped = false;
    event->default_prevented = false;

    // 检查事件类型是否在有效范围内
    int event_type_index = (int)event->type;
    if (event_type_index < 0 || event_type_index >= 32) {
        return false;
    }

    // 调用注册的事件处理器
    ui_event_handler_t handler = component->event_handlers[event_type_index];
    void *user_data = component->event_handler_user_data[event_type_index];

    if (handler) {
        return handler(component, event, user_data);
    }

    return false;
}

// ==================== 坐标转换 ====================

void UIComponent_ScreenToLocal(ui_component_t *component, float screen_x, float screen_y, float *local_x, float *local_y) {
    if (!component || !local_x || !local_y) {
        return;
    }

    // 计算局部坐标（相对于组件左上角）
    *local_x = screen_x - component->x;
    *local_y = screen_y - component->y;
}

void UIComponent_LocalToScreen(ui_component_t *component, float local_x, float local_y, float *screen_x, float *screen_y) {
    if (!component || !screen_x || !screen_y) {
        return;
    }

    // 计算屏幕坐标
    *screen_x = local_x + component->x;
    *screen_y = local_y + component->y;
}

// ==================== 双击检测 ====================

bool UIComponent_CheckDoubleClick(ui_component_t *component, float x, float y, int current_time) {
    if (!component) {
        return false;
    }

    // 检查时间和距离
    int time_diff = current_time - component->last_click_time;
    float dist_x = x - component->last_click_x;
    float dist_y = y - component->last_click_y;
    float distance = dist_x * dist_x + dist_y * dist_y; // 使用平方距离避免开方

    // 更新点击信息
    component->last_click_time = current_time;
    component->last_click_x = x;
    component->last_click_y = y;

    // 判断是否为双击（时间间隔小于阈值且位置接近）
    return time_diff < DOUBLE_CLICK_TIME && distance < 25.0f; // 5像素的平方
}

// ==================== 基础组件初始化 ====================

void UIComponent_InitBase(ui_component_t *component, ui_component_type_t type, const ui_component_vtable_t *vtable, canvas2d_context_t *ctx) {
    if (!component) {
        return;
    }

    memset(component, 0, sizeof(ui_component_t));
    component->type = type;
    component->vtable = vtable;
    component->ctx = ctx;
    component->flags = UI_FLAG_VISIBLE | UI_FLAG_ENABLED;
    component->x = 0.0f;
    component->y = 0.0f;
    component->width = 0.0f;
    component->height = 0.0f;
    component->bg_color = MAKE(COLOR32, 0, 0, 0, 0);
    component->margin[0] = 0.0f;
    component->margin[1] = 0.0f;
    component->margin[2] = 0.0f;
    component->margin[3] = 0.0f;
    component->padding[0] = 0.0f;
    component->padding[1] = 0.0f;
    component->padding[2] = 0.0f;
    component->padding[3] = 0.0f;
    component->parent = NULL;
    component->children = NULL;
    component->child_count = 0;
    component->child_capacity = 0;
    component->user_data = NULL;
    component->last_click_time = 0;
    component->last_click_x = 0.0f;
    component->last_click_y = 0.0f;
    component->drag_start_x = 0.0f;
    component->drag_start_y = 0.0f;
    component->drag_offset_x = 0.0f;
    component->drag_offset_y = 0.0f;

    // 初始化事件处理器数组
    memset(component->event_handlers, 0, sizeof(component->event_handlers));
    memset(component->event_handler_user_data, 0, sizeof(component->event_handler_user_data));
}

void UIComponent_ShutdownBase(ui_component_t *component) {
    if (!component) {
        return;
    }

    // 清理子组件（如果有）
    if (component->children) {
        // 注意：这里只释放数组，不释放子组件本身
        // 子组件应该由父组件在关闭前单独释放
        free(component->children);
        component->children = NULL;
    }

    component->child_count = 0;
    component->child_capacity = 0;
}

// ==================== 样式相关函数 ====================

// 绘制组件背景
void UIComponent_RenderBackground(ui_component_t *component) {
    if (!component || !UIComponent_IsVisible(component)) return;
    
    COLOR32 bg_color = component->bg_color;
    // 如果背景色透明则不绘制
    if (bg_color.a == 0) return;
    
    canvas2d_set_fill_style(component->ctx, bg_color);
    canvas2d_fill_rect(component->ctx, component->x, component->y, component->width, component->height);
}

void UIComponent_SetBgColor(ui_component_t *component, COLOR32 color) {
    if (component) {
        component->bg_color = color;
    }
}

COLOR32 UIComponent_GetBgColor(const ui_component_t *component) {
    return component ? component->bg_color : MAKE(COLOR32, 0, 0, 0, 0);
}

void UIComponent_SetMargin(ui_component_t *component, float top, float right, float bottom, float left) {
    if (!component) return;
    component->margin[0] = top;
    component->margin[1] = right;
    component->margin[2] = bottom;
    component->margin[3] = left;
}

void UIComponent_GetMargin(const ui_component_t *component, float *top, float *right, float *bottom, float *left) {
    if (!component) {
        if (top) *top = 0.0f;
        if (right) *right = 0.0f;
        if (bottom) *bottom = 0.0f;
        if (left) *left = 0.0f;
        return;
    }
    if (top) *top = component->margin[0];
    if (right) *right = component->margin[1];
    if (bottom) *bottom = component->margin[2];
    if (left) *left = component->margin[3];
}

void UIComponent_SetPadding(ui_component_t *component, float top, float right, float bottom, float left) {
    if (!component) return;
    component->padding[0] = top;
    component->padding[1] = right;
    component->padding[2] = bottom;
    component->padding[3] = left;
}

void UIComponent_GetPadding(const ui_component_t *component, float *top, float *right, float *bottom, float *left) {
    if (!component) {
        if (top) *top = 0.0f;
        if (right) *right = 0.0f;
        if (bottom) *bottom = 0.0f;
        if (left) *left = 0.0f;
        return;
    }
    if (top) *top = component->padding[0];
    if (right) *right = component->padding[1];
    if (bottom) *bottom = component->padding[2];
    if (left) *left = component->padding[3];
}

void UIComponent_GetContentRect(const ui_component_t *component, float *x, float *y, float *width, float *height) {
    if (!component) {
        if (x) *x = 0.0f;
        if (y) *y = 0.0f;
        if (width) *width = 0.0f;
        if (height) *height = 0.0f;
        return;
    }
    
    if (x) *x = component->x + component->padding[3];
    if (y) *y = component->y + component->padding[0];
    if (width) {
        *width = component->width - component->padding[1] - component->padding[3];
        if (*width < 0.0f) *width = 0.0f;
    }
    if (height) {
        *height = component->height - component->padding[0] - component->padding[2];
        if (*height < 0.0f) *height = 0.0f;
    }
}
