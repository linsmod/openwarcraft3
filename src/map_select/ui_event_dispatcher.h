#ifndef __UI_EVENT_DISPATCHER_H__
#define __UI_EVENT_DISPATCHER_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"

// 事件分发器配置
typedef struct {
    int double_click_time;         // 双击检测时间间隔（毫秒）
    int double_click_distance;      // 双击检测最大距离（像素）
    int drag_threshold;            // 拖拽检测阈值（像素）
} ui_event_dispatcher_config_t;

// 事件分发器
typedef struct {
    ui_event_dispatcher_config_t config;
    ui_component_t *root;          // 根组件
    ui_component_t *focused;       // 当前获得焦点的组件
    ui_component_t *hovered;        // 当前鼠标悬停的组件
    ui_component_t *dragging;       // 当前正在拖拽的组件
    ui_component_t *captured;       // 当前捕获鼠标的组件（优先接收所有鼠标事件）
    ui_component_t *last_clicked;   // 上次点击的组件（用于双击检测）
    int last_click_time;           // 上次点击时间
    float last_click_x;             // 上次点击X坐标
    float last_click_y;             // 上次点击Y坐标
    bool mouse_buttons[5];          // 鼠标按钮状态
    canvas2d_context_t *ctx;
} ui_event_dispatcher_t;

// 创建默认事件分发器配置
ui_event_dispatcher_config_t UIEventDispatcher_GetDefaultConfig(void);

// 初始化事件分发器
int UIEventDispatcher_Init(ui_event_dispatcher_t *dispatcher, ui_component_t *root, canvas2d_context_t *ctx);

// 清理事件分发器
void UIEventDispatcher_Shutdown(ui_event_dispatcher_t *dispatcher);

// 设置根组件
void UIEventDispatcher_SetRoot(ui_event_dispatcher_t *dispatcher, ui_component_t *root);

// 分发鼠标按下事件
bool UIEventDispatcher_DispatchMouseDown(ui_event_dispatcher_t *dispatcher, float x, float y, int button, int timestamp);

// 分发鼠标释放事件
bool UIEventDispatcher_DispatchMouseUp(ui_event_dispatcher_t *dispatcher, float x, float y, int button, int timestamp);

// 分发鼠标移动事件
bool UIEventDispatcher_DispatchMouseMove(ui_event_dispatcher_t *dispatcher, float x, float y, int timestamp);

// 分发鼠标滚轮事件
bool UIEventDispatcher_DispatchMouseWheel(ui_event_dispatcher_t *dispatcher, float x, float y, int delta, int timestamp);

// 分发右键菜单事件
bool UIEventDispatcher_DispatchContextMenu(ui_event_dispatcher_t *dispatcher, float x, float y, int timestamp);

// 分发键盘按下事件
bool UIEventDispatcher_DispatchKeyDown(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, bool repeat, int timestamp);

// 分发键盘释放事件
bool UIEventDispatcher_DispatchKeyUp(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, int timestamp);

// 分发键盘按键事件
bool UIEventDispatcher_DispatchKeyPress(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, int timestamp);

// 更新事件分发器（处理动画、双击检测等）
void UIEventDispatcher_Update(ui_event_dispatcher_t *dispatcher, int msec);

// 帧捕获（命中测试）
ui_component_t* UIEventDispatcher_Capture(ui_event_dispatcher_t *dispatcher, float x, float y);

// 事件冒泡处理（从target到root）
bool UIEventDispatcher_BubbleEvent(ui_event_dispatcher_t *dispatcher, ui_component_t *target, ui_event_t *event);

// 焦点管理
void UIEventDispatcher_SetFocus(ui_event_dispatcher_t *dispatcher, ui_component_t *component);
ui_component_t* UIEventDispatcher_GetFocus(ui_event_dispatcher_t *dispatcher);
void UIEventDispatcher_ClearFocus(ui_event_dispatcher_t *dispatcher);

// 鼠标捕获管理
void UIEventDispatcher_CaptureMouse(ui_event_dispatcher_t *dispatcher, ui_component_t *component);
ui_component_t* UIEventDispatcher_GetCaptured(ui_event_dispatcher_t *dispatcher);
void UIEventDispatcher_ReleaseMouse(ui_event_dispatcher_t *dispatcher);

#endif // __UI_EVENT_DISPATCHER_H__
