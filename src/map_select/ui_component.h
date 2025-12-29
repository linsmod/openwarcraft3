#ifndef __UI_COMPONENT_H__
#define __UI_COMPONENT_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "../common/scene.h"

// 前向声明
typedef struct ui_component_t ui_component_t;

// UI 事件类型
typedef enum {
    UI_EVENT_MOUSE_ENTER,     // 鼠标进入
    UI_EVENT_MOUSE_LEAVE,     // 鼠标离开
    UI_EVENT_MOUSE_DOWN,      // 鼠标按下
    UI_EVENT_MOUSE_UP,        // 鼠标释放
    UI_EVENT_CLICK,           // 单击
    UI_EVENT_DOUBLE_CLICK,    // 双击
    UI_EVENT_MOUSE_MOVE,      // 鼠标移动
    UI_EVENT_MOUSE_WHEEL,     // 鼠标滚轮
    UI_EVENT_DRAG_START,      // 拖拽开始
    UI_EVENT_DRAG,            // 拖拽中
    UI_EVENT_DRAG_END,        // 拖拽结束
    UI_EVENT_CONTEXT_MENU,   // 右键菜单
    UI_EVENT_KEY_DOWN,        // 键盘按下
    UI_EVENT_KEY_UP,          // 键盘释放
    UI_EVENT_KEY_PRESS,       // 键盘按键
    UI_EVENT_TEXT_INPUT,      // 文本输入（支持中文输入法）
    UI_EVENT_FOCUS,           // 获得焦点
    UI_EVENT_BLUR,            // 失去焦点
    UI_EVENT_RESIZE,          // 尺寸改变
    UI_EVENT_SCROLL,          // 滚动
} ui_event_type_t;

// 鼠标按钮
typedef enum {
    UI_MOUSE_BUTTON_LEFT,
    UI_MOUSE_BUTTON_MIDDLE,
    UI_MOUSE_BUTTON_RIGHT,
    UI_MOUSE_BUTTON_X1,
    UI_MOUSE_BUTTON_X2,
} ui_mouse_button_t;

// UI 事件数据
typedef struct {
    ui_event_type_t type;      // 事件类型
    ui_component_t *target;    // 目标组件
    ui_component_t *current_target; // 当前目标（用于事件冒泡）
    int timestamp;             // 时间戳（毫秒）
    bool propagation_stopped;  // 是否停止传播
    bool default_prevented;    // 是否阻止默认行为
} ui_event_t;

typedef struct {
    COLOR32 normal;
    COLOR32 hover;
    COLOR32 active;
    COLOR32 disabled;
} ui_color_t, UICOLOR32;

// 鼠标事件数据
typedef struct {
    ui_event_t base;
    float x;                   // 鼠标X坐标
    float y;                   // 鼠标Y坐标
    float screen_x;            // 屏幕X坐标
    float screen_y;            // 屏幕Y坐标
    int button;                // 按钮号 (ui_mouse_button_t)
    int click_count;           // 点击次数（用于区分单击/双击）
    float delta_x;             // X轴移动增量（用于拖拽）
    float delta_y;             // Y轴移动增量（用于拖拽）
    int delta;                 // 滚轮增量（用于滚轮事件）
    int buttons;               // 按钮状态掩码
} ui_mouse_event_t;

// 键盘事件数据
typedef struct {
    ui_event_t base;
    int key;                   // 按键码
    int scancode;              // 扫描码
    int modifiers;            // 修饰键状态（Shift/Ctrl/Alt等）
    bool repeat;               // 是否为重复按键
} ui_keyboard_event_t;

// 焦点事件数据
typedef struct {
    ui_event_t base;
    ui_component_t *related_target; // 相关目标（失去焦点时的新焦点，或获得焦点时的旧焦点）
} ui_focus_event_t;

// 尺寸事件数据
typedef struct {
    ui_event_t base;
    float old_width;
    float old_height;
    float new_width;
    float new_height;
} ui_resize_event_t;

// 滚动事件数据
typedef struct {
    ui_event_t base;
    float scroll_x;
    float scroll_y;
    float delta_x;
    float delta_y;
} ui_scroll_event_t;

// UI 事件回调函数类型
typedef bool (*ui_event_handler_t)(ui_component_t *component, ui_event_t *event, void *user_data);

// UI 组件类型
typedef enum {
    UI_COMPONENT_TYPE_BASE,   // 基础组件
    UI_COMPONENT_TYPE_BUTTON,  // 按钮
    UI_COMPONENT_TYPE_TEXT,    // 文本
    UI_COMPONENT_TYPE_LIST,    // 列表
    UI_COMPONENT_TYPE_LIST_ITEM, // 列表项
    UI_COMPONENT_TYPE_CONTAINER, // 容器
    UI_COMPONENT_TYPE_SCROLLBAR, // 滚动条
    UI_COMPONENT_TYPE_EDITBOX,   // 编辑框
    UI_COMPONENT_TYPE_CHECKBOX,  // 复选框
    UI_COMPONENT_TYPE_SLIDER,    // 滑块
    UI_COMPONENT_TYPE_IMAGE,     // 图片
    UI_COMPONENT_TYPE_PANEL,     // 面板
} ui_component_type_t;

// UI 组件标志
typedef enum {
    UI_FLAG_VISIBLE         = (1 << 0),  // 可见
    UI_FLAG_ENABLED         = (1 << 1),  // 启用
    UI_FLAG_FOCUSED         = (1 << 2),  // 获得焦点
    UI_FLAG_HOVERED         = (1 << 3),  // 鼠标悬停
    UI_FLAG_DRAGGING        = (1 << 4),  // 正在拖拽
    UI_FLAG_DRAGGABLE       = (1 << 6),  // 可拖拽
    UI_FLAG_RESIZABLE       = (1 << 7),  // 可调整大小
    UI_FLAG_SCROLLABLE      = (1 << 8),  // 可滚动
    UI_FLAG_CLIPPING        = (1 << 9),  // 裁剪子组件
    UI_FLAG_ACCEPT_FOCUS    = (1 << 10), // 接受焦点
    UI_FLAG_TAB_STOP        = (1 << 11), // Tab键可访问
} ui_component_flags_t;

// UI 组件虚拟函数表
typedef struct ui_component_vtable {
    // 基础生命周期方法
    void (*init)(ui_component_t *component, canvas2d_context_t *ctx);
    void (*shutdown)(ui_component_t *component);
    void (*update)(ui_component_t *component, int msec);
    void (*render)(ui_component_t *component);

    // 布局方法
    void (*set_position)(ui_component_t *component, float x, float y);
    void (*set_size)(ui_component_t *component, float width, float height);
    void (*set_bounds)(ui_component_t *component, float x, float y, float width, float height);

    // 命中测试
    bool (*hit_test)(ui_component_t *component, float x, float y);

    // 鼠标事件处理
    bool (*on_mouse_enter)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_mouse_leave)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_mouse_down)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_mouse_up)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_click)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_double_click)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_mouse_move)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_mouse_wheel)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_context_menu)(ui_component_t *component, ui_mouse_event_t *event);

    // 拖拽事件处理
    bool (*on_drag_start)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_drag)(ui_component_t *component, ui_mouse_event_t *event);
    bool (*on_drag_end)(ui_component_t *component, ui_mouse_event_t *event);

// 键盘事件处理
    bool (*on_key_down)(ui_component_t *component, ui_keyboard_event_t *event);
    bool (*on_key_up)(ui_component_t *component, ui_keyboard_event_t *event);
    bool (*on_key_press)(ui_component_t *component, ui_keyboard_event_t *event);
    bool (*on_text_input)(ui_component_t *component, const char *text);  // 文本输入（支持中文输入法）

    // 焦点事件处理
    bool (*on_focus)(ui_component_t *component, ui_focus_event_t *event);
    bool (*on_blur)(ui_component_t *component, ui_focus_event_t *event);

    // 其他事件处理
    bool (*on_resize)(ui_component_t *component, ui_resize_event_t *event);
    bool (*on_scroll)(ui_component_t *component, ui_scroll_event_t *event);

    // 容器相关方法（仅容器组件实现）
    int (*add_child)(ui_component_t *parent, ui_component_t *child);
    bool (*remove_child)(ui_component_t *parent, ui_component_t *child);
    int (*get_child_count)(ui_component_t *parent);
    ui_component_t* (*get_child)(ui_component_t *parent, int index);

    // 自定义方法
    void* (*get_custom_data)(ui_component_t *component, const char *key);
    bool (*set_custom_data)(ui_component_t *component, const char *key, void *data);
} ui_component_vtable_t;

// UI 组件基类
struct ui_component_t {
    const ui_component_vtable_t *vtable;  // 虚函数表
    ui_component_type_t type;             // 组件类型
    ui_component_t *parent;                // 父组件
    canvas2d_context_t *ctx;              // 画布上下文

    // 位置和尺寸
    float x;
    float y;
    float width;
    float height;

    // 样式属性
    UICOLOR32 bg_color;
    float margin[4];
    float padding[4];

    // 标志和状态
    uint32_t flags;
    void *user_data;                      // 用户数据

    // 事件处理器（可选）
    ui_event_handler_t event_handlers[32]; // 各类型事件处理器
    void *event_handler_user_data[32];     // 各类型事件处理器的用户数据

    // 子组件（容器组件使用）
    ui_component_t **children;
    int child_count;
    int child_capacity;

    // 拖拽状态
    float drag_start_x;
    float drag_start_y;
    float drag_offset_x;
    float drag_offset_y;

    // 双击检测
    int last_click_time;
    float last_click_x;
    float last_click_y;
};

// 辅助函数：事件操作
void UIEvent_StopPropagation(ui_event_t *event);
void UIEvent_PreventDefault(ui_event_t *event);

// 辅助函数：组件标志操作
bool UIComponent_IsVisible(const ui_component_t *component);
bool UIComponent_IsEnabled(const ui_component_t *component);
bool UIComponent_IsFocused(const ui_component_t *component);
bool UIComponent_IsHovered(const ui_component_t *component);
void UIComponent_SetVisible(ui_component_t *component, bool visible);
const char* UIComponent_GetTypeName(int typeid);
void UIComponent_SetEnabled(ui_component_t *component, bool enabled);
void UIComponent_SetFocused(ui_component_t *component, bool focused);

// 辅助函数：事件处理
bool UIComponent_AddEventHandler(ui_component_t *component, ui_event_type_t event_type, ui_event_handler_t handler, void *user_data);
bool UIComponent_RemoveEventHandler(ui_component_t *component, ui_event_type_t event_type);
bool UIComponent_TriggerEvent(ui_component_t *component, ui_event_t *event);

// 辅助函数：坐标转换
void UIComponent_ScreenToLocal(ui_component_t *component, float screen_x, float screen_y, float *local_x, float *local_y);
void UIComponent_LocalToScreen(ui_component_t *component, float local_x, float local_y, float *screen_x, float *screen_y);

// 辅助函数：样式设置
void UIComponent_SetBgColor(ui_component_t *component, COLOR32 color);
COLOR32 UIComponent_GetBgColor(const ui_component_t *component);
void UIComponent_SetMargin(ui_component_t *component, float top, float right, float bottom, float left);
void UIComponent_GetMargin(const ui_component_t *component, float *top, float *right, float *bottom, float *left);
void UIComponent_SetPadding(ui_component_t *component, float top, float right, float bottom, float left);
void UIComponent_GetPadding(const ui_component_t *component, float *top, float *right, float *bottom, float *left);

// 辅助函数：获取内容区域（考虑padding）
void UIComponent_GetContentRect(const ui_component_t *component, float *x, float *y, float *width, float *height);

// 辅助函数：双击检测
bool UIComponent_CheckDoubleClick(ui_component_t *component, float x, float y, int current_time);

// 基础组件初始化（为其他组件提供默认实现）
void UIComponent_InitBase(ui_component_t *component, ui_component_type_t type, const ui_component_vtable_t *vtable, canvas2d_context_t *ctx);
void UIComponent_ShutdownBase(ui_component_t *component);

// 基础渲染函数
void UIComponent_RenderBackground(ui_component_t *component);

#endif // __UI_COMPONENT_H__
