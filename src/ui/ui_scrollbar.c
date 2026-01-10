#include "ui_scrollbar.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// 滚动条最小滑块尺寸（像素）
#define MIN_THUMB_SIZE 20

// 虚函数声明
static void scrollbar_init(ui_component_t *component, canvas2d_context_t *ctx);
static void scrollbar_shutdown(ui_component_t *component);
static void scrollbar_render(ui_component_t *component);
static void scrollbar_on_mouse_down(ui_component_t *component, event_t *event);
static void scrollbar_on_mouse_move(ui_component_t *component, event_t *event);
static void scrollbar_on_mouse_up(ui_component_t *component, event_t *event);

static const ui_component_vtable scrollbar_vtable = {
    .init = scrollbar_init,
    .shutdown = scrollbar_shutdown,
    .render = scrollbar_render,
    .on_mouse_down = scrollbar_on_mouse_down,
    .on_mouse_move = scrollbar_on_mouse_move,
    .on_mouse_up = scrollbar_on_mouse_up
};

// ==================== 初始化和销毁 ====================

static void scrollbar_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    
    scrollbar->orientation = SCROLLBAR_ORIENTATION_VERTICAL;
    scrollbar->scroll_value = 0.0f;
    scrollbar->page_size = 0.1f;  // 默认页面大小
    
    scrollbar->dragging = false;
    scrollbar->drag_start_value = 0.0f;
    scrollbar->drag_start_pos = 0.0f;
    
    // 默认样式
    scrollbar->track_width = 12;
    scrollbar->track_color = (COLOR32){224, 224, 224, 255};  // 浅灰色
    scrollbar->thumb_color = (COLOR32){176, 176, 176, 255};  // 深灰色
    scrollbar->border_color = (COLOR32){192, 192, 192, 255};  // 边框颜色
    
    scrollbar->on_scroll_changed = NULL;
    scrollbar->user_data = NULL;
}

static void scrollbar_shutdown(ui_component_t *component) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    // 清理资源（如果有）
}

// ==================== 创建和销毁 ====================

ui_scrollbar_t* UIScrollbar_Create(float x, float y, float width, float height,
                                 scrollbar_orientation_t orientation,
                                 canvas2d_context_t *ctx) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)malloc(sizeof(ui_scrollbar_t));
    if (!scrollbar) {
        return NULL;
    }
    
    // 初始化基类
    UIComponent_InitBase((ui_component_t *)scrollbar, UI_COMPONENT_TYPE_SCROLLBAR,
                        &scrollbar_vtable, ctx);
    
    // 设置位置和大小
    UIComponent_SetPosition((ui_component_t *)scrollbar, x, y);
    UIComponent_SetSize((ui_component_t *)scrollbar, width, height);
    
    // 调用初始化
    scrollbar_init((ui_component_t *)scrollbar, ctx);
    
    // 设置方向
    scrollbar->orientation = orientation;
    
    // 设置组件标志
    ((ui_component_t *)scrollbar)->flags |= UI_FLAG_VISIBLE;
    
    return scrollbar;
}

void UIScrollbar_Destroy(ui_scrollbar_t *scrollbar) {
    if (!scrollbar) return;
    
    UIComponent_ShutdownBase((ui_component_t *)scrollbar);
    free(scrollbar);
}

// ==================== 属性设置 ====================

void UIScrollbar_SetValue(ui_scrollbar_t *scrollbar, float value) {
    if (!scrollbar) return;
    
    // 限制滚动值在 [0, 1] 范围内
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
    
    scrollbar->scroll_value = value;
}

float UIScrollbar_GetValue(const ui_scrollbar_t *scrollbar) {
    if (!scrollbar) return 0.0f;
    return scrollbar->scroll_value;
}

void UIScrollbar_SetPageSize(ui_scrollbar_t *scrollbar, float page_size) {
    if (!scrollbar) return;
    
    // 限制页面大小在 [0, 1] 范围内
    if (page_size < 0.0f) page_size = 0.0f;
    if (page_size > 1.0f) page_size = 1.0f;
    
    scrollbar->page_size = page_size;
}

void UIScrollbar_SetColors(ui_scrollbar_t *scrollbar,
                        COLOR32 track_color,
                        COLOR32 thumb_color,
                        COLOR32 border_color) {
    if (!scrollbar) return;
    
    scrollbar->track_color = track_color;
    scrollbar->thumb_color = thumb_color;
    scrollbar->border_color = border_color;
}

void UIScrollbar_SetScrollCallback(ui_scrollbar_t *scrollbar,
                                 void (*callback)(ui_component_t *component, float value, void *user_data),
                                 void *user_data) {
    if (!scrollbar) return;
    
    scrollbar->on_scroll_changed = callback;
    scrollbar->user_data = user_data;
}

// ==================== 渲染 ====================

static void scrollbar_render(ui_component_t *component) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    
    if (!UIComponent_IsVisible(component)) return;
    
    float x = component->x;
    float y = component->y;
    float width = component->width;
    float height = component->height;
    
    // 绘制轨道
    canvas2d_draw_rect_fill(component->ctx, x, y, width, height, scrollbar->track_color);
    canvas2d_draw_rect(component->ctx, x, y, width, height, scrollbar->border_color);
    
    // 计算滑块位置和大小
    float thumb_size;
    float thumb_pos;
    
    if (scrollbar->orientation == SCROLLBAR_ORIENTATION_VERTICAL) {
        // 垂直滚动条
        thumb_size = height * scrollbar->page_size;
        if (thumb_size < MIN_THUMB_SIZE) thumb_size = MIN_THUMB_SIZE;
        if (thumb_size > height) thumb_size = height;
        
        float available_height = height - thumb_size;
        thumb_pos = y + available_height * scrollbar->scroll_value;
        
        // 绘制滑块
        canvas2d_draw_rect_fill(component->ctx, x + 1, thumb_pos, width - 2, thumb_size, scrollbar->thumb_color);
    } else {
        // 水平滚动条
        thumb_size = width * scrollbar->page_size;
        if (thumb_size < MIN_THUMB_SIZE) thumb_size = MIN_THUMB_SIZE;
        if (thumb_size > width) thumb_size = width;
        
        float available_width = width - thumb_size;
        thumb_pos = x + available_width * scrollbar->scroll_value;
        
        // 绘制滑块
        canvas2d_draw_rect_fill(component->ctx, thumb_pos, y + 1, thumb_size, height - 2, scrollbar->thumb_color);
    }
}

// ==================== 事件处理 ====================

static void scrollbar_on_mouse_down(ui_component_t *component, event_t *event) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    
    float mouse_x = event->mouse.x;
    float mouse_y = event->mouse.y;
    
    // 检查是否点击在滑块上
    float thumb_size, thumb_pos, track_start, track_end;
    
    if (scrollbar->orientation == SCROLLBAR_ORIENTATION_VERTICAL) {
        thumb_size = component->height * scrollbar->page_size;
        if (thumb_size < MIN_THUMB_SIZE) thumb_size = MIN_THUMB_SIZE;
        
        float available_height = component->height - thumb_size;
        thumb_pos = component->y + available_height * scrollbar->scroll_value;
        track_start = component->y;
        track_end = component->y + component->height;
        
        // 检查是否点击在滑块上
        if (mouse_y >= thumb_pos && mouse_y <= thumb_pos + thumb_size) {
            scrollbar->dragging = true;
            scrollbar->drag_start_value = scrollbar->scroll_value;
            scrollbar->drag_start_pos = mouse_y;
            event->propagation_stopped = true;
            return;
        }
        
        // 点击在轨道上，跳转到该位置
        float new_value = (mouse_y - component->y - thumb_size / 2) / (component->height - thumb_size);
        if (new_value < 0.0f) new_value = 0.0f;
        if (new_value > 1.0f) new_value = 1.0f;
        
        scrollbar->scroll_value = new_value;
        if (scrollbar->on_scroll_changed) {
            scrollbar->on_scroll_changed(component, new_value, scrollbar->user_data);
        }
        event->propagation_stopped = true;
    } else {
        // 水平滚动条
        thumb_size = component->width * scrollbar->page_size;
        if (thumb_size < MIN_THUMB_SIZE) thumb_size = MIN_THUMB_SIZE;
        
        float available_width = component->width - thumb_size;
        thumb_pos = component->x + available_width * scrollbar->scroll_value;
        track_start = component->x;
        track_end = component->x + component->width;
        
        // 检查是否点击在滑块上
        if (mouse_x >= thumb_pos && mouse_x <= thumb_pos + thumb_size) {
            scrollbar->dragging = true;
            scrollbar->drag_start_value = scrollbar->scroll_value;
            scrollbar->drag_start_pos = mouse_x;
            event->propagation_stopped = true;
            return;
        }
        
        // 点击在轨道上，跳转到该位置
        float new_value = (mouse_x - component->x - thumb_size / 2) / (component->width - thumb_size);
        if (new_value < 0.0f) new_value = 0.0f;
        if (new_value > 1.0f) new_value = 1.0f;
        
        scrollbar->scroll_value = new_value;
        if (scrollbar->on_scroll_changed) {
            scrollbar->on_scroll_changed(component, new_value, scrollbar->user_data);
        }
        event->propagation_stopped = true;
    }
}

static void scrollbar_on_mouse_move(ui_component_t *component, event_t *event) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    
    if (!scrollbar->dragging) return;
    
    float mouse_pos = (scrollbar->orientation == SCROLLBAR_ORIENTATION_VERTICAL) 
                     ? event->mouse.y 
                     : event->mouse.x;
    
    float thumb_size;
    if (scrollbar->orientation == SCROLLBAR_ORIENTATION_VERTICAL) {
        thumb_size = component->height * scrollbar->page_size;
    } else {
        thumb_size = component->width * scrollbar->page_size;
    }
    
    if (thumb_size < MIN_THUMB_SIZE) thumb_size = MIN_THUMB_SIZE;
    
    // 计算拖拽偏移
    float delta = mouse_pos - scrollbar->drag_start_pos;
    float track_size = (scrollbar->orientation == SCROLLBAR_ORIENTATION_VERTICAL)
                       ? component->height
                       : component->width;
    
    float available_size = track_size - thumb_size;
    float delta_value = delta / available_size;
    
    // 更新滚动值
    float new_value = scrollbar->drag_start_value + delta_value;
    if (new_value < 0.0f) new_value = 0.0f;
    if (new_value > 1.0f) new_value = 1.0f;
    
    scrollbar->scroll_value = new_value;
    
    // 调用回调
    if (scrollbar->on_scroll_changed) {
        scrollbar->on_scroll_changed(component, new_value, scrollbar->user_data);
    }
    
    event->propagation_stopped = true;
}

static void scrollbar_on_mouse_up(ui_component_t *component, event_t *event) {
    ui_scrollbar_t *scrollbar = (ui_scrollbar_t *)component;
    
    if (scrollbar->dragging) {
        scrollbar->dragging = false;
        event->propagation_stopped = true;
    }
}
