#ifndef __EVENT_H__
#define __EVENT_H__

#include "shared.h"
#include <stddef.h>

// 前向声明
typedef struct ui_component_t ui_component_t;

// ==================== 统一事件类型枚举 ====================

typedef enum {
    // 鼠标基础事件
    EVENT_MOUSE_DOWN = 0,
    EVENT_MOUSE_UP,
    EVENT_MOUSE_MOTION,
    EVENT_MOUSE_WHEEL,
    
    // 细化鼠标事件
    EVENT_CLICK,
    EVENT_DOUBLE_CLICK,
    EVENT_DRAG_START,
    EVENT_DRAG,
    EVENT_DRAG_END,
    EVENT_MOUSE_ENTER,
    EVENT_MOUSE_LEAVE,
    
    // 键盘事件
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_TEXT_INPUT,
    EVENT_TEXT_EDITING,
    
    // 焦点事件
    EVENT_FOCUS,
    EVENT_BLUR,
    EVENT_FOCUS_GAINED,
    EVENT_FOCUS_LOST,
    
    // 应用事件
    EVENT_QUIT,
    EVENT_SCREEN_RESIZE,
    EVENT_CONTEXT_MENU,
    
    // UI事件
    EVENT_VALUE_CHANGED,
    EVENT_SCROLL,
    EVENT_CHAR,
    EVENT_MOUSE_MOVE,
    EVENT_RESIZE,
    
    EVENT_MAX
} event_type_t;

// ==================== 统一事件结构 ====================

typedef struct event_t {
    // 基础信息
    event_type_t type;
    
    // 事件目标
    ui_component_t *target;
    ui_component_t *current_target;
    
    // 时间戳
    int timestamp;
    
    // 传播控制
    bool propagation_stopped;
    bool default_prevented;
    
    // 用户数据
    void *user_data;

    bool dispatch_immediately;
    
        // 鼠标事件数据
        struct {
            int button;
            float x;
            float y;
            float start_x;      // 拖拽起始位置
            float start_y;
            bool down;
            int click_count;
        } mouse;

        // 键盘事件数据
        struct {
            int key;
            int scancode;
            int modifiers;
            bool repeat;
            bool down;
        } keyboard;
        
    // 事件数据
    union {
        // 鼠标移动事件数据（用于鼠标移动事件）
        struct {
            float x;
            float y;
            float dx;          // 相对移动量
            float dy;
        } motion;
        
        // 滚轮事件数据
        struct {
            float delta_y;
            float delta_x;
        } wheel;
        
        
        // 文本输入数据
        struct {
            char text[32];
        } text;
        
        // 文本编辑数据
        struct {
            char text[32];
            int start;
            int length;
        } editing;
        
        // 屏幕尺寸变化
        struct {
            float old_width;
            float old_height;
            float new_width;
            float new_height;
        } resize;
        
        // 滚动事件
        struct {
            float scroll_x;
            float scroll_y;
            float delta_x;
            float delta_y;
        } scroll;
    };
} event_t;

// ==================== 事件操作 ====================

void Event_StopPropagation(event_t *event);
void Event_PreventDefault(event_t *event);

// ==================== 事件处理器类型 ====================

typedef void (*event_handler_t)(ui_component_t *component, event_t *event, void *user_data);

#endif // __EVENT_H__