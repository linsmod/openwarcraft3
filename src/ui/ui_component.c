#include "ui_component.h"
#include "../common/event.h"
#include "../common/scene.h"
#include "layx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>

// 双击时间间隔（毫秒）
#define DOUBLE_CLICK_TIME 500

// 默认滚动配置（接近浏览器默认值）
static const ui_scroll_config_t default_scroll_config = {
    .deceleration_rate = 0.985f,    // 减速系数
    .overscroll_stiffness = 0.2f,  // 弹性系数
    .overscroll_damping = 0.9f,    // 弹性阻尼
    .scroll_threshold = 5.0f,      // 停止滚动阈值 (px/s)
    .max_overscroll = 100.0f        // 最大过度滚动距离 (px)
};

// ==================== 事件操作 ====================

void UIEvent_StopPropagation(event_t *event) {
    if (event) {
        event->propagation_stopped = true;
    }
}

void UIEvent_PreventDefault(event_t *event) {
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
void UIComponent_GetContentBoxRect(ui_component_t *component, float *x, float *y, float *width, float *height){
    layx_scalar x_,y_,width_,height_;
    layx_get_rect_xywh(component->lay_ctx, component->lay_item_id, &x_, &y_, &width_, &height_);

    // layx_scalar l, t,r,b;
    // layx_get_margin_ltrb(component->lay_ctx, component->lay_item_id, &l,&t,&r,&b);
    *x = (float)x_;
    *y = (float)y_;
    *width = (float)width_;
    *height = (float)height_;
}
const char* UIComponent_GetTypeName(int typeid){
    switch(typeid){
        case UI_COMPONENT_TYPE_BUTTON:
            return "BUTTON";
        case UI_COMPONENT_TYPE_LIST:
            return "LIST";
        case UI_COMPONENT_TYPE_SCROLLBAR:
            return "SCROLLBAR";
        case UI_COMPONENT_TYPE_CONTAINER:
            return "CONTAINER";
        case UI_COMPONENT_TYPE_LIST_ITEM:
            return "LIST_ITEM";
        case UI_COMPONENT_TYPE_TEXT:
            return "TEXT";
        case UI_COMPONENT_TYPE_EDITBOX:
            return "EDITBOX";
        case UI_COMPONENT_TYPE_CHECKBOX:
            return "CHECKBOX";
        case UI_COMPONENT_TYPE_SLIDER:
            return "SLIDER";
        case UI_COMPONENT_TYPE_IMAGE:
            return "IMAGE";
        case UI_COMPONENT_TYPE_PANEL:
            return "PANEL";
        case UI_COMPONENT_TYPE_HTML_VIEWER:
            return "HTML_VIEWER";
        case UI_COMPONENT_TYPE_HTML_DOC:
            return "HTML_DOC";
        case UI_COMPONENT_TYPE_HTML_NODE:
            return "HTML_NODE";
        default:
            return "UI_COMPONENT_UNKNOWN";
    }
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

bool UIComponent_AddEventHandler(ui_component_t *component, ui_event_type_t event_type, event_handler_t handler, void *user_data) {
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

void UIComponent_TriggerEvent(ui_component_t *component, event_t *event) {
    if (!component || !event) {
        return;
    }

    // 初始化事件
    event->target = component;
    event->current_target = component;
    event->propagation_stopped = false;
    event->default_prevented = false;

    // 检查事件类型是否在有效范围内
    int event_type_index = (int)event->type;
    if (event_type_index < 0 || event_type_index >= 32) {
        return;
    }

    // 调用注册的事件处理器
    ui_event_handler_t handler = component->event_handlers[event_type_index];
    void *user_data = component->event_handler_user_data[event_type_index];

    if (handler) {
        handler(component, event, user_data);
    }

    return;
}

// 内部辅助函数：递归执行hitTest
ui_component_t* UIComponent_HitTest(ui_component_t *component, float x, float y) {
    if (!component || !(component->flags & UI_FLAG_VISIBLE)) {
        return NULL;
    }
    
    // 检查鼠标坐标是否在组件边界内
    if (x >= component->x && x <= component->x + component->width &&
        y >= component->y && y <= component->y + component->height) {
        
        // 如果有hit_test函数，执行hit_test函数
        if(component->vtable->hit_test){
            return component->vtable->hit_test(component,x,y);
        }
        // 如果有子组件，从后往前检查（因为后渲染的在上层）
        else if (component->children && component->child_count > 0) {
            for (int i = component->child_count - 1; i >= 0; i--) {
                ui_component_t *child = component->children[i];
                ui_component_t *hit_child = UIComponent_HitTest(child, x, y);
                if (hit_child) {
                    return hit_child;  // 返回命中的子组件
                }
            }
        }
        
        // 如果没有子组件命中，返回当前组件
        return component;
    }
    
    return NULL;
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
    component->bg_color.normal = MAKE(COLOR32, 0, 0, 0, 0);
    component->bg_color.hover = MAKE(COLOR32, 0, 0, 0, 0);
    component->bg_color.active = MAKE(COLOR32, 0, 0, 0, 0);
    component->bg_color.disabled = MAKE(COLOR32, 0, 0, 0, 0);
    component->text_color = MAKE(COLOR32, 255, 255, 255, 255);
    component->font_size = 0;
    component->text_align = 0;
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
    if(ctx && ctx->lay_ctx){
        component->lay_ctx = ctx->lay_ctx;
        component->lay_item_id = layx_item(ctx->lay_ctx);
    }
    else{
        component->lay_item_id = LAYX_INVALID_ID;
    }

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
static void UIComponent_RenderBackground(ui_component_t *component) {
    if (!component || !UIComponent_IsVisible(component)) return;
    
    // 从布局系统获取组件的实际位置和尺寸
    layx_scalar x, y, w, h;
    layx_get_rect_xywh(component->lay_ctx, component->lay_item_id, &x, &y, &w, &h);

    // 根据组件状态选择背景色
    COLOR32 bg_color;
    if (!UIComponent_IsEnabled(component)) {
        bg_color = component->bg_color.disabled;
    } else if (component->flags & UI_FLAG_HOVERED) {
        bg_color = component->bg_color.hover;
    } else if (component->flags & UI_FLAG_FOCUSED) {
        bg_color = component->bg_color.active;
    } else {
        bg_color = component->bg_color.normal;
    }

    // 如果背景色透明则不绘制
    if (bg_color.a == 0) return;

    canvas2d_set_fill_style(component->ctx, bg_color);
    canvas2d_fill_rect(component->ctx, (float)x, (float)y, (float)w, (float)h);
}

// 绘制组件边框
static void UIComponent_RenderBorder(ui_component_t *component) {
    if (!component || !UIComponent_IsVisible(component)) return;
    
    // 从布局系统获取组件的实际位置和尺寸
    layx_scalar x, y, w, h;
    layx_get_rect_xywh(component->lay_ctx, component->lay_item_id, &x, &y, &w, &h);

    // 默认实现：检查是否有边框颜色（对于非容器组件，这里使用一个简单的默认边框）
    // 对于容器组件（ui_container_t），边框在 container_render 中绘制
    // 这里提供一个基础实现，子类可以覆盖
    
    // 默认不绘制边框，除非组件有特定的标志或状态
    // 例如：当组件获得焦点时，可以绘制一个焦点边框
    if (component->flags & UI_FLAG_FOCUSED) {
        COLOR32 focus_color = MAKE(COLOR32, 0, 120, 215, 255); // 蓝色焦点边框
        canvas2d_set_stroke_style(component->ctx, focus_color);
        canvas2d_set_line_width(component->ctx, 2.0f);
        canvas2d_stroke_rect(component->ctx, (float)x, (float)y, (float)w, (float)h);
    }
}

float UIComponent_GetScrollFactor(const ui_component_t *component){
    if(component && component->vtable->get_scroll_factor) {
        return component->vtable->get_scroll_factor(component);
    }
    else{
        return 1.0f;
    }
}
float UIComponent_GetScrollX(const ui_component_t *component) {
    if(component && component->vtable->get_scroll_x) {
        return component->vtable->get_scroll_x(component);
    }
    else{
        return 0.0f;
    }
}
float UIComponent_GetScrollY(const ui_component_t *component) {
    if(component && component->vtable->get_scroll_y) {
        return component->vtable->get_scroll_y(component);
    }
    else{
        return 0.0f;
    }
}
float UIComponent_GetMaxScrollX(const ui_component_t *component) {
    if(component && component->vtable->get_max_scroll_x) {
        return component->vtable->get_max_scroll_x(component);
    }
    else{
        return 0.0f;
    }
}
float UIComponent_GetMaxScrollY(const ui_component_t *component) {
    if(component && component->vtable->get_max_scroll_y) {
        return component->vtable->get_max_scroll_y(component);
    }
    else{
        return 0.0f;
    }
}
float UIComponent_CanScrollVertically(const ui_component_t *component) {
    if(component && component->vtable->can_scroll_vertically) {
        return component->vtable->can_scroll_vertically(component);
    }
    else{
        return 0.0f;
    }
}
float UIComponent_CanScrollHorizontally(const ui_component_t *component) {
    if(component && component->vtable->can_scroll_horizontally) {
        return component->vtable->can_scroll_horizontally(component);
    }
    else{
        return 0.0f;
    }
}
void UIComponent_SetScrollY(ui_component_t *component, float scroll_y) {
    if(component && component->vtable->set_scroll_y) {
        component->vtable->set_scroll_y(component, scroll_y);
    }
}
void UIComponent_SetScrollX(ui_component_t *component, float scroll_x) {
    if(component && component->vtable->set_scroll_x) {
        component->vtable->set_scroll_x(component, scroll_x);
    }
}
void UIComponent_OnScrolled(ui_component_t *component,int dx,int dy) {
    if(component && component->vtable->on_scrolled){
        component->vtable->on_scrolled(component,dx,dy);
    }
}

// ==================== 惯性滚动和弹性边界 ====================

// 前向声明
static void UIComponent_CheckInertiaScroll(ui_component_t *component);

// 初始化惯性滚动（设置初始速度）
void UIComponent_ScrollWithInertia(ui_component_t *component, 
                                   float delta_x, 
                                   float delta_y,uint64_t timestamp_ms) {
    if (!component) return;
    
    uint64_t current_time = SDL_GetTicks();
    ui_scroll_state_t* state = &component->scroll_state;
    
    // 根据累积的位移计算一个初始速度
    // delta_x 和 delta_y 应该是累积的滚动量，不是速度
    // 假设传入的是短时间内累积的滚动位移（像素）
    
    // 根据位移计算一个初始速度
    // 如果delta是在100ms内累积的，那么速度 = delta / 0.1
    float time_window = 0.1f; // 假设是100ms内的累积
    float initial_velocity_scale = 1.0f / time_window;
    
    state->velocity_x = delta_x * initial_velocity_scale;
    state->velocity_y = delta_y * initial_velocity_scale;
    
    state->is_scrolling = true;
    state->is_decelerating = true;
    state->last_update_time = current_time;
    
    // 请求动画帧，触发持续的动画更新
    UIComponent_RequestAnimationFrame(component);
}

// 更新惯性滚动动画（每帧调用）
void UIComponent_UpdateScrollAnimation(ui_component_t *component) {
    if (!component) return;
    
    // 先检查是否需要触发惯性滚动
    UIComponent_CheckInertiaScroll(component);
    
    ui_scroll_state_t* state = &component->scroll_state;
    
    if (!state->is_scrolling && !state->is_decelerating) {
        return;
    }
    
    // 计算时间差
    uint64_t current_time = SDL_GetTicks();
    uint64_t dt_ms = current_time - state->last_update_time;
    if (dt_ms == 0 || dt_ms > 100) {  // 防止时间差过大（如窗口失焦后恢复）
        state->last_update_time = current_time;
        return;
    }
    
    state->last_update_time = current_time;
    float dt_sec = dt_ms / 1000.0f;
    
    float old_scroll_x = UIComponent_GetScrollX(component);
    float old_scroll_y = UIComponent_GetScrollY(component);
    float new_scroll_x = old_scroll_x;
    float new_scroll_y = old_scroll_y;
    
    bool can_scroll_x = UIComponent_CanScrollHorizontally(component);
    bool can_scroll_y = UIComponent_CanScrollVertically(component);
    float max_scroll_x = UIComponent_GetMaxScrollX(component);
    float max_scroll_y = UIComponent_GetMaxScrollY(component);
    
    // 处理惯性滚动
    if (state->is_decelerating) {
        // 应用减速
        float decay = powf(default_scroll_config.deceleration_rate, dt_sec * 60.0f);
        state->velocity_x *= decay;
        state->velocity_y *= decay;
        
        // 计算位移
        float delta_x = state->velocity_x * dt_sec;
        float delta_y = state->velocity_y * dt_sec;
        
        // 应用滚动
        if (can_scroll_x) {
            new_scroll_x = old_scroll_x - delta_x;
        }
        if (can_scroll_y) {
            new_scroll_y = old_scroll_y - delta_y;
        }
        
        // 检查是否停止
        float speed_x = fabsf(state->velocity_x);
        float speed_y = fabsf(state->velocity_y);
        if (speed_x < default_scroll_config.scroll_threshold &&
            speed_y < default_scroll_config.scroll_threshold &&
            state->overscroll_x == 0 && state->overscroll_y == 0) {
            state->velocity_x = 0;
            state->velocity_y = 0;
            state->is_decelerating = false;
            state->is_scrolling = false;
        }
    }
    
    // 应用弹性边界
    bool has_overscroll = false;
    
    if (new_scroll_x < 0) {
        state->overscroll_x = new_scroll_x;
        new_scroll_x = 0;
        has_overscroll = true;
    } else if (can_scroll_x && new_scroll_x > max_scroll_x) {
        state->overscroll_x = new_scroll_x - max_scroll_x;
        new_scroll_x = max_scroll_x;
        has_overscroll = true;
    } else {
        state->overscroll_x *= default_scroll_config.overscroll_damping;
        if (fabsf(state->overscroll_x) < 0.5f) state->overscroll_x = 0;
    }
    
    if (new_scroll_y < 0) {
        state->overscroll_y = new_scroll_y;
        new_scroll_y = 0;
        has_overscroll = true;
    } else if (can_scroll_y && new_scroll_y > max_scroll_y) {
        state->overscroll_y = new_scroll_y - max_scroll_y;
        new_scroll_y = max_scroll_y;
        has_overscroll = true;
    } else {
        state->overscroll_y *= default_scroll_config.overscroll_damping;
        if (fabsf(state->overscroll_y) < 0.5f) state->overscroll_y = 0;
    }
    
    // 应用过度滚动的回弹力
    if (state->overscroll_x != 0 || state->overscroll_y != 0) {
        state->velocity_x -= state->overscroll_x * default_scroll_config.overscroll_stiffness * 60.0f * dt_sec;
        state->velocity_y -= state->overscroll_y * default_scroll_config.overscroll_stiffness * 60.0f * dt_sec;
        
        if (!has_overscroll) {
            state->is_decelerating = true;
        }
    }
    
    // 限制过度滚动距离
    state->overscroll_x = CLAMP(state->overscroll_x, 
                               -default_scroll_config.max_overscroll,
                               default_scroll_config.max_overscroll);
    state->overscroll_y = CLAMP(state->overscroll_y,
                               -default_scroll_config.max_overscroll,
                               default_scroll_config.max_overscroll);
    
    // 更新滚动位置（包含过度滚动的偏移）
    float final_scroll_x = new_scroll_x + state->overscroll_x;
    float final_scroll_y = new_scroll_y + state->overscroll_y;
    
    if (can_scroll_x && final_scroll_x != old_scroll_x) {
        UIComponent_SetScrollX(component, final_scroll_x);
    }
    if (can_scroll_y && final_scroll_y != old_scroll_y) {
        UIComponent_SetScrollY(component, final_scroll_y);
    }
    
    // 检查是否需要继续动画
    if (state->is_decelerating || state->overscroll_x != 0 || state->overscroll_y != 0) {
        // 需要继续动画，将在主循环中再次调用
    } else {
        state->is_scrolling = false;
    }
    
    if (final_scroll_x != old_scroll_x || final_scroll_y != old_scroll_y) {
        UIComponent_OnScrolled(component, 
                              final_scroll_x - old_scroll_x,
                              final_scroll_y - old_scroll_y);
    }
}

// 检查组件是否正在滚动（用于确定是否需要更新动画）
bool UIComponent_IsScrolling(const ui_component_t *component) {
    if (!component) return false;
    const ui_scroll_state_t* state = &component->scroll_state;
    return state->is_scrolling || state->is_decelerating || 
           state->overscroll_x != 0 || state->overscroll_y != 0;
}

// 停止惯性滚动（用于触摸/鼠标按下时）
void UIComponent_StopInertiaScroll(ui_component_t *component) {
    if (!component) return;
    ui_scroll_state_t* state = &component->scroll_state;
    state->velocity_x = 0;
    state->velocity_y = 0;
    state->is_scrolling = false;
    state->is_decelerating = false;
    state->overscroll_x = 0;
    state->overscroll_y = 0;
    
    // 取消动画帧请求
    UIComponent_CancelAnimationFrame(component);
}

// ==================== RequestAnimationFrame 机制 ====================

// 设置场景管理器（用于 RequestAnimationFrame 机制）
void UIComponent_SetSceneManager(ui_component_t *component, struct scene_manager_t *mgr) {
    if (component) {
        component->scene_manager = mgr;
        // 递归设置所有子组件的 scene_manager
        if (component->children) {
            for (int i = 0; i < component->child_count; i++) {
                UIComponent_SetSceneManager(component->children[i], mgr);
            }
        }
    }
}

// 请求动画帧（将组件注册到 SceneManager 的动画列表中）
// 这个函数会自动调用组件的 vtable->update_animation 方法
void UIComponent_RequestAnimationFrame(ui_component_t *component) {
    if (!component || !component->scene_manager) {
        return;
    }
    
    // 注册到 SceneManager 的动画列表
    SceneManager_RegisterAnimationComponent(component->scene_manager, component);
}

// 取消动画帧请求（从 SceneManager 的动画列表中移除）
void UIComponent_CancelAnimationFrame(ui_component_t *component) {
    if (!component || !component->scene_manager) {
        return;
    }
    
    // 从 SceneManager 的动画列表中移除
    SceneManager_UnregisterAnimationComponent(component->scene_manager, component);
}

// 组件的默认 update_animation 实现（处理惯性滚动）
void UIComponent_UpdateAnimationDefault(ui_component_t *component) {
    UIComponent_UpdateScrollAnimation(component);
}
void UIComponent_ScrollBy(ui_component_t *component, float delta_x, float delta_y, uint64_t current_time) {
    if (!component) return;
    
    // 如果有自定义实现，使用它
    if (component->vtable && component->vtable->scroll_by) {
        component->vtable->scroll_by(component, delta_x, delta_y);
        return;
    }

    ui_scroll_state_t* state = &component->scroll_state;
    
    // 如果距离上次滚动时间超过一定阈值，重置累积
    if (current_time - state->last_scroll_time > 100) { // 100ms无滚动则重置
        state->accumulated_delta_x = 0;
        state->accumulated_delta_y = 0;
        state->accumulation_start_time = current_time;
    }
    
    // 累积滚动量
    state->accumulated_delta_x += delta_x;
    state->accumulated_delta_y += delta_y;
    state->last_scroll_time = current_time;
    
    // 限制最大累积量，避免过大
    const float MAX_ACCUMULATION = 200.0f;
    state->accumulated_delta_x = CLAMP(state->accumulated_delta_x, -MAX_ACCUMULATION, MAX_ACCUMULATION);
    state->accumulated_delta_y = CLAMP(state->accumulated_delta_y, -MAX_ACCUMULATION, MAX_ACCUMULATION);
    printf("Accumulated delta: (%f, %f)\n", state->accumulated_delta_x, state->accumulated_delta_y);
    
    // 立即应用当前滚动（为了响应性）
    bool scrolled = false;
    bool can_scroll_x = UIComponent_CanScrollHorizontally(component);
    bool can_scroll_y = UIComponent_CanScrollVertically(component);
    
    if ((delta_x == 0 && delta_y == 0) || (!can_scroll_x && !can_scroll_y)) {
        return;
    }
    
    float scroll_factor = UIComponent_GetScrollFactor(component);
    float old_scroll_x = UIComponent_GetScrollX(component);
    float old_scroll_y = UIComponent_GetScrollY(component);
    float max_scroll_x = UIComponent_GetMaxScrollX(component);
    float max_scroll_y = UIComponent_GetMaxScrollY(component);
    
    float new_scroll_x = old_scroll_x;
    float new_scroll_y = old_scroll_y;
    
    // 应用弹性滚动
    if (delta_y != 0 && can_scroll_y) {
        new_scroll_y = old_scroll_y - delta_y * scroll_factor;
        
        // 弹性边界处理
        if (new_scroll_y < 0) {
            new_scroll_y = old_scroll_y - delta_y * scroll_factor * 0.3f;
        } else if (new_scroll_y > max_scroll_y) {
            new_scroll_y = old_scroll_y - delta_y * scroll_factor * 0.3f;
        }
        
        // 最终边界限制
        if (new_scroll_y < 0) new_scroll_y = 0;
        if (new_scroll_y > max_scroll_y) new_scroll_y = max_scroll_y;
        
        if (new_scroll_y != old_scroll_y) {
            UIComponent_SetScrollY(component, new_scroll_y);
            scrolled = true;
        }
    }
    
    if (delta_x != 0 && can_scroll_x) {
        new_scroll_x = old_scroll_x - delta_x * scroll_factor;
        
        // 弹性边界处理
        if (new_scroll_x < 0) {
            new_scroll_x = old_scroll_x - delta_x * scroll_factor * 0.3f;
        } else if (new_scroll_x > max_scroll_x) {
            new_scroll_x = old_scroll_x - delta_x * scroll_factor * 0.3f;
        }
        
        // 最终边界限制
        if (new_scroll_x < 0) new_scroll_x = 0;
        if (new_scroll_x > max_scroll_x) new_scroll_x = max_scroll_x;
        
        if (new_scroll_x != old_scroll_x) {
            UIComponent_SetScrollX(component, new_scroll_x);
            scrolled = true;
        }
    }
    
    if (scrolled) {
        UIComponent_OnScrolled(component, 
                              new_scroll_x - old_scroll_x,
                              new_scroll_y - old_scroll_y);
        
        // 标记需要检查惯性
        state->needs_inertia_check = true;
        state->inertia_check_time = current_time + 50; // 50ms后检查
        
        // 停止任何现有的惯性滚动
        state->is_decelerating = false;
        state->is_scrolling = false;
    }
}
// 检查并触发惯性滚动（在主循环中定期调用）
void UIComponent_CheckInertiaScroll(ui_component_t *component) {
    if (!component) return;
    
    ui_scroll_state_t* state = &component->scroll_state;
    uint64_t current_time = SDL_GetTicks();
    
    // 如果不需要检查或还未到检查时间，返回
    if (!state->needs_inertia_check || current_time < state->inertia_check_time) {
        return;
    }
    
    // 如果最近还有滚动事件，重新安排检查
    if (current_time - state->last_scroll_time < 30) { // 30ms内还有滚动
        state->inertia_check_time = current_time + 50;
        return;
    }
    
    // 检查累积的滚动量是否足够触发惯性
    float accumulated_time = (float)(state->last_scroll_time - state->accumulation_start_time);
    if (accumulated_time == 0) accumulated_time = 1;
    
    // 计算平均速度
    float avg_speed_x = (state->accumulated_delta_x * 1000.0f) / accumulated_time;
    float avg_speed_y = (state->accumulated_delta_y * 1000.0f) / accumulated_time;
    
    float speed = sqrtf(avg_speed_x * avg_speed_x + avg_speed_y * avg_speed_y);
    
    // 如果速度足够快，触发惯性滚动
    const float MIN_INERTIA_SPEED = 30.0f;
    if (speed > MIN_INERTIA_SPEED && 
        (fabsf(state->accumulated_delta_x) > 10 || fabsf(state->accumulated_delta_y) > 10)) {
        
        // 停止当前所有动画
        state->velocity_x = 0;
        state->velocity_y = 0;
        state->is_scrolling = false;
        state->is_decelerating = false;
        
        // 根据累积量设置惯性
        UIComponent_ScrollWithInertia(component, 
                                     state->accumulated_delta_x * 2.0f, // 乘以系数增强效果
                                     state->accumulated_delta_y * 2.0f,
                                     current_time);
    }
    
    // 重置累积状态
    state->accumulated_delta_x = 0;
    state->accumulated_delta_y = 0;
    state->needs_inertia_check = false;
}

void UIComponent_SetBgColor(ui_component_t *component, COLOR32 color) {
    if (component) {
        // 设置所有状态为相同颜色
        component->bg_color.normal = color;
        component->bg_color.hover = color;
        component->bg_color.active = color;
        component->bg_color.disabled = color;
    }
}

COLOR32 UIComponent_GetBgColor(const ui_component_t *component) {
    if (!component) return MAKE(COLOR32, 0, 0, 0, 0);

    // 返回当前状态的背景色
    if (!UIComponent_IsEnabled(component)) {
        return component->bg_color.disabled;
    } else if (component->flags & UI_FLAG_HOVERED) {
        return component->bg_color.hover;
    } else if (component->flags & UI_FLAG_FOCUSED) {
        return component->bg_color.active;
    } else {
        return component->bg_color.normal;
    }
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

// ==================== 调试函数 ====================

void UIComponent_PrintTree(const ui_component_t *component, int indent) {
    if (!component) {
        return;
    }

    // 打印缩进
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    // 打印组件信息
    const char *type_name = UIComponent_GetTypeName(component->type);
    const char *visible = UIComponent_IsVisible(component) ? "V" : "-";
    const char *enabled = UIComponent_IsEnabled(component) ? "E" : "-";
    const char *focused = UIComponent_IsFocused(component) ? "F" : "-";

    layx_scalar x,y,w,h;
    layx_get_rect_xywh(component->lay_ctx, component->lay_item_id,&x,&y,&w,&h);

    char common[128];
    sprintf(common,"[%s%s%s] %s @ (%.f, %.f) [%.f x %.f]",
           visible, enabled, focused, type_name,
           x,y,w,h);

    // 递归打印子组件
    if(component->vtable->print_tree){
        // print self and its children
        component->vtable->print_tree(component, indent + 1,common);
    }
    else{
        printf(common,"");
        for (int i = 0; i < component->child_count; i++) {
            UIComponent_PrintTree(component->children[i], indent + 1);
        }
    }
    
}

// ==================== 布局API实现 - 对使用者隐藏lay细节 ====================

void UIComponent_SetLayoutContext(ui_component_t *component, layx_context *ctx) {
    if (!component) {
        return;
    }
    component->lay_ctx = ctx;
}

layx_id UIComponent_CreateLayoutItem(ui_component_t *component) {
    if (!component || !component->lay_ctx) {
        return LAYX_INVALID_ID;
    }
    if (component->lay_item_id == LAYX_INVALID_ID) {
        component->lay_item_id = layx_item(component->lay_ctx);
    }
    return component->lay_item_id;
}

layx_id UIComponent_GetLayoutItem(const ui_component_t *component) {
    return component ? component->lay_item_id : LAYX_INVALID_ID;
}

void UIComponent_SetLayoutMargins(ui_component_t *component, float left, float top, float right, float bottom) {
    UIComponent_SetMargin(component, top, right, bottom, left);
}

void UIComponent_SetSize(ui_component_t *component, float width, float height) {
    if (!component) return;
    
    // 更新组件尺寸
    component->width = width;
    component->height = height;
    
    // 如果有布局上下文，同步到lay
    if (component->lay_ctx && component->lay_item_id != LAYX_INVALID_ID) {
        layx_set_size(component->lay_ctx, component->lay_item_id, width, height);
    }
}

void UIComponent_GetSize(const ui_component_t *component, float *width, float *height) {
    if (!component) {
        if (width) *width = 0.0f;
        if (height) *height = 0.0f;
        return;
    }
    if (width) *width = component->width;
    if (height) *height = component->height;
}

void UIComponent_SetMargin(ui_component_t *component, float top, float right, float bottom, float left) {
    if (!component) return;
    
    // 更新组件边距
    component->margin[0] = top;
    component->margin[1] = right;
    component->margin[2] = bottom;
    component->margin[3] = left;
    
    // 如果有布局上下文，同步到lay
    if (component->lay_ctx && component->lay_item_id != LAYX_INVALID_ID) {
        layx_set_margin_ltrb(component->lay_ctx, component->lay_item_id, left, top, right, bottom);
    }
}

void UIComponent_SetAlignSelf(ui_component_t *component, uint32_t flags) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_align_self(component->lay_ctx, component->lay_item_id, flags);
}

void UIComponent_SetFlexDirection(ui_component_t *component, uint32_t direction) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_flex_direction(component->lay_ctx, component->lay_item_id, direction);
}

// Flex属性设置
void UIComponent_SetFlexGrow(ui_component_t *component, float grow) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_flex_grow(component->lay_ctx, component->lay_item_id, (layx_scalar)grow);
}

void UIComponent_SetFlexShrink(ui_component_t *component, float shrink) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_flex_shrink(component->lay_ctx, component->lay_item_id, (layx_scalar)shrink);
}

void UIComponent_SetFlexBasis(ui_component_t *component, float basis) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_flex_basis(component->lay_ctx, component->lay_item_id, (layx_scalar)basis);
}

void UIComponent_SetFlex(ui_component_t *component, float grow, float shrink, float basis) {
    if (!component || !component->lay_ctx || component->lay_item_id == LAYX_INVALID_ID) {
        return;
    }
    layx_set_flex_properties(component->lay_ctx, component->lay_item_id, 
                             (layx_scalar)grow, (layx_scalar)shrink, (layx_scalar)basis);
}

void UIComponent_SetPosition(ui_component_t *component, float x, float y) {
    if (!component) return;
    component->x = x;
    component->y = y;
}

void UIComponent_Layout(ui_component_t *root) {
    if (!root || !root->lay_ctx) {
        return;
    }

    // 运行布局计算
    layx_run_context(root->lay_ctx);
    
    // 调试输出：打印布局后的组件树
    // printf("\n========== UI Layout Tree (After Layout) ==========\n");
    // UIComponent_PrintTree(root, 0);
    // printf("===================================\n\n");
}

// ==================== 组件渲染默认实现 ====================

// 默认的滚动条渲染实现
void UIComponent_RenderScrollbars(ui_component_t *component) {
    if (!component || !UIComponent_IsVisible(component)) return;
    if (!component->vtable) return;
    
    // 检查是否有必要的滚动查询函数
    if (!component->vtable->can_scroll_vertically || 
        !component->vtable->can_scroll_horizontally ||
        !component->vtable->get_scroll_percent_x ||
        !component->vtable->get_scroll_percent_y) {
        return;
    }
    
    // 滚动条配置 - 使用固定宽度
    const int scrollbar_width = 12;
    const int scrollbar_height = 14; // 滚动条高度（水平）
    
    // 滚动条颜色
    COLOR32 scrollbar_track_color = {224, 224, 224, 255}; // 轨道颜色：浅灰色
    COLOR32 scrollbar_thumb_color = {176, 176, 176, 255}; // 滑块颜色：深灰色
    COLOR32 scrollbar_border_color = {192, 192, 192, 255}; // 边框颜色
    
    // 获取滚动状态
    bool can_scroll_v = component->vtable->can_scroll_vertically(component);
    bool can_scroll_h = component->vtable->can_scroll_horizontally(component);
    
    // 获取组件位置和尺寸
    float viewport_x = component->x;
    float viewport_y = component->y;
    float viewport_w = component->width;
    float viewport_h = component->height;
    
    // 绘制垂直滚动条（右侧）
    if (can_scroll_v) {
        // 计算轨道位置（在viewport右侧）
        float track_x = viewport_x + viewport_w - scrollbar_width;
        float track_y = viewport_y;
        float track_w = scrollbar_width;
        float track_h = viewport_h;
        
        // 绘制轨道
        canvas2d_set_fill_style(component->ctx, scrollbar_track_color);
        canvas2d_fill_rect(component->ctx, track_x, track_y, track_w, track_h);
        canvas2d_set_stroke_style(component->ctx, scrollbar_border_color);
        canvas2d_set_line_width(component->ctx, 1.0f);
        canvas2d_stroke_rect(component->ctx, track_x, track_y, track_w, track_h);
        
        // 计算滑块位置和大小
        float scroll_percent = component->vtable->get_scroll_percent_y(component);
        
        // 滑块高度：固定最小高度20px，最大不超过track
        float thumb_height = 20.0f;
        if (thumb_height > track_h - 2) {
            thumb_height = track_h - 2;
        }
        
        // 滑块Y位置：根据滚动百分比
        float thumb_y = track_y + (track_h - thumb_height) * scroll_percent / 100.0f;
        float thumb_x = track_x + 1; // 留出1px边距
        float thumb_w = track_w - 2; // 留出两边边距
        
        // 绘制滑块
        canvas2d_set_fill_style(component->ctx, scrollbar_thumb_color);
        canvas2d_fill_rect(component->ctx, thumb_x, thumb_y, thumb_w, thumb_height);
    }
    
    // 绘制水平滚动条（底部）
    if (can_scroll_h) {
        // 计算轨道位置（在viewport底部）
        // 如果有垂直滚动条，水平滚动条要缩短以避免重叠
        float track_w = viewport_w;
        if (can_scroll_v) {
            track_w -= scrollbar_width;
        }
        
        float track_x = viewport_x;
        float track_y = viewport_y + viewport_h - scrollbar_height;
        float track_h = scrollbar_height;
        
        // 绘制轨道
        canvas2d_set_fill_style(component->ctx, scrollbar_track_color);
        canvas2d_fill_rect(component->ctx, track_x, track_y, track_w, track_h);
        canvas2d_set_stroke_style(component->ctx, scrollbar_border_color);
        canvas2d_set_line_width(component->ctx, 1.0f);
        canvas2d_stroke_rect(component->ctx, track_x, track_y, track_w, track_h);
        
        // 计算滑块位置和大小
        float scroll_percent = component->vtable->get_scroll_percent_x(component);
        
        // 滑块宽度：固定最小宽度20px，最大不超过track
        float thumb_width = 20.0f;
        if (thumb_width > track_w - 2) {
            thumb_width = track_w - 2;
        }
        
        // 滑块X位置：根据滚动百分比
        float thumb_x = track_x + (track_w - thumb_width) * scroll_percent / 100.0f;
        float thumb_y = track_y + 1; // 留出1px边距
        float thumb_h = track_h - 2; // 留出上下边距
        
        // 绘制滑块
        canvas2d_set_fill_style(component->ctx, scrollbar_thumb_color);
        canvas2d_fill_rect(component->ctx, thumb_x, thumb_y, thumb_width, thumb_h);
    }
}

// 默认的组件渲染实现
void UIComponent_Render(ui_component_t *comp) {
    if (!comp || !UIComponent_IsVisible(comp)) return;
    if (comp->vtable && comp->vtable->render_background) {
        comp->vtable->render_background(comp);
    } else {
        UIComponent_RenderBackground(comp);
    }

    if (comp->vtable && comp->vtable->render_border) {
        comp->vtable->render_border(comp);
    } else {
        UIComponent_RenderBorder(comp);
    }
    // 调用组件的 render 方法（如果存在）
    if (comp->vtable && comp->vtable->render) {
        comp->vtable->render(comp);
    }
}