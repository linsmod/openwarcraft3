#ifndef __UI_COMPONENT_H__
#define __UI_COMPONENT_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "../common/scene.h"
#include "../html/layout.h"

// 前向声明
typedef struct ui_component_t ui_component_t;
typedef struct lay_context lay_context;

// ==================== 组件类型枚举 ====================

typedef enum {
    UI_COMPONENT_TYPE_UNKNOWN = 0,
    UI_COMPONENT_TYPE_BUTTON,
    UI_COMPONENT_TYPE_LIST,
    UI_COMPONENT_TYPE_SCROLLBAR,
    UI_COMPONENT_TYPE_CONTAINER,
    UI_COMPONENT_TYPE_LIST_ITEM,
    UI_COMPONENT_TYPE_TEXT,
    UI_COMPONENT_TYPE_EDITBOX,
    UI_COMPONENT_TYPE_CHECKBOX,
    UI_COMPONENT_TYPE_SLIDER,
    UI_COMPONENT_TYPE_IMAGE,
    UI_COMPONENT_TYPE_PANEL,
    UI_COMPONENT_TYPE_COUNT
} ui_component_type_t;

// ==================== 组件标志 ====================

typedef enum {
    UI_FLAG_VISIBLE     = 0x01,  // 组件可见
    UI_FLAG_ENABLED     = 0x02,  // 组件可用
    UI_FLAG_FOCUSED     = 0x04,  // 组件获得焦点
    UI_FLAG_HOVERED     = 0x08,  // 组件被悬停
    UI_FLAG_DRAGGABLE   = 0x10,  // 组件可以被拖拽
    UI_FLAG_DRAGGING    = 0x20,  // 组件正在拖拽中
    UI_FLAG_CAPTURED    = 0x40,  // 组件捕获了鼠标
    UI_FLAG_ACCEPT_FOCUS= 0x80,  // 组件接受焦点
    UI_FLAG_TAB_STOP    = 0x100, // Tab键停止
    UI_FLAG_CLIPPING    = 0x200  // 启用裁剪（子组件超出边界时不显示）
} ui_component_flags_t;

// ==================== 事件类型 ====================

typedef enum {
    UI_EVENT_MOUSE_DOWN = 0,
    UI_EVENT_MOUSE_UP,
    UI_EVENT_MOUSE_MOVE,
    UI_EVENT_MOUSE_ENTER,
    UI_EVENT_MOUSE_LEAVE,
    UI_EVENT_MOUSE_WHEEL,
    UI_EVENT_CLICK,
    UI_EVENT_DOUBLE_CLICK,
    UI_EVENT_KEY_DOWN,
    UI_EVENT_KEY_UP,
    UI_EVENT_CHAR,
    UI_EVENT_FOCUS_GAINED,
    UI_EVENT_FOCUS_LOST,
    UI_EVENT_VALUE_CHANGED,
    UI_EVENT_SCROLL,
    UI_EVENT_FOCUS,
    UI_EVENT_TEXT_INPUT,
    UI_EVENT_KEY_PRESS,
    UI_EVENT_CONTEXT_MENU,
    UI_EVENT_BLUR,
    UI_EVENT_DRAG_END,
    UI_EVENT_DRAG_START,
    UI_EVENT_DRAG,
    UI_EVENT_RESIZE,
    UI_EVENT_MAX
} ui_event_type_t;

// 鼠标按钮
typedef enum {
    UI_MOUSE_BUTTON_LEFT,
    UI_MOUSE_BUTTON_MIDDLE,
    UI_MOUSE_BUTTON_RIGHT,
    UI_MOUSE_BUTTON_X1,
    UI_MOUSE_BUTTON_X2,
} ui_mouse_button_t;

// ==================== 事件数据结构 ====================

typedef struct ui_event_t {
    ui_event_type_t type;
    ui_component_t *target;           // 事件目标组件
    ui_component_t *current_target;   // 当前处理事件的组件
    float x;                          // 鼠标X坐标
    float y;                          // 鼠标Y坐标
    int button;                       // 鼠标按钮（1=左键, 2=中键, 3=右键）
    int key;                          // 键盘按键代码
    unsigned int character;           // 字符（用于CHAR事件）
    bool propagation_stopped;         // 是否停止事件传播
    bool default_prevented;           // 是否阻止默认行为
    float scroll_delta;               // 滚轮增量
    int timestamp;
    void* user_data;
} ui_event_t;

// ==================== 事件处理器类型 ====================

typedef bool (*ui_event_handler_t)(ui_component_t *component, ui_event_t *event, void *user_data);

// 鼠标事件数据
typedef struct {
    ui_event_t base;
    float x;
    float y;
    float screen_x;
    float screen_y;
    int button;
    int click_count;
    float delta_x;
    float delta_y;
    int delta;
    int buttons;
} ui_mouse_event_t;

// 键盘事件数据
typedef struct {
    ui_event_t base;
    int key;
    int scancode;
    int modifiers;
    bool repeat;
} ui_keyboard_event_t;

// 焦点事件数据
typedef struct {
    ui_event_t base;
    ui_component_t *related_target;
} ui_focus_event_t;

// 大小改变事件数据
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

// ==================== 背景颜色状态 ====================

typedef struct {
    COLOR32 normal;
    COLOR32 hover;
    COLOR32 active;
    COLOR32 disabled;
} ui_bg_color_t, UICOLOR32;

// ==================== 组件虚函数表 ====================

typedef struct ui_component_vtable {
    // 基础生命周期方法
    void (*init)(ui_component_t *component, canvas2d_context_t *ctx);
    void (*shutdown)(ui_component_t *component);
    void (*update)(ui_component_t *component, int msec);
    void (*layout)(ui_component_t* component, ui_component_t* root,size2_t vpsize);
    void (*render)(ui_component_t *component);
    
    // 渲染辅助方法（可选，用于自定义渲染）
    void (*render_background)(ui_component_t *component);
    void (*render_border)(ui_component_t *component);
    
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
    bool (*on_text_input)(ui_component_t *component, const char *text);
    
    // 焦点事件处理
    bool (*on_focus)(ui_component_t *component, ui_focus_event_t *event);
    bool (*on_blur)(ui_component_t *component, ui_focus_event_t *event);
    
    // 尺寸改变事件
    bool (*on_resize)(ui_component_t *component, void *event);
    
    // 滚动事件
    bool (*on_scroll)(ui_component_t *component, void *event);
    
    // 容器特定方法
    int (*add_child)(ui_component_t *component, ui_component_t *child);
    bool (*remove_child)(ui_component_t *component, ui_component_t *child);
    int (*get_child_count)(ui_component_t *component);
    ui_component_t* (*get_child)(ui_component_t *component, int index);
    
    // 自定义数据
    void* (*get_custom_data)(ui_component_t *component, const char *key);
    void (*set_custom_data)(ui_component_t *component, const char *key, void *data);
} ui_component_vtable_t;
// ==================== 组件结构 ====================

struct ui_component_t {
    // 基础属性
    ui_component_type_t type;        // 组件类型
    const ui_component_vtable_t *vtable;  // 虚函数表
    canvas2d_context_t *ctx;         // 渲染上下文
    
    // 布局属性
    float x, y;                     // 组件位置（由布局系统自动更新）
    float width, height;             // 组件尺寸（由布局系统自动更新）
    float margin[4];                 // 边距 [top, right, bottom, left]
    float padding[4];                // 内边距 [top, right, bottom, left]
    
    // 布局相关（内部使用）
    lay_context *lay_ctx;            // 布局上下文指针
    lay_id lay_item_id;              // 组件的布局项ID
    
    // 样式
    ui_bg_color_t bg_color;          // 背景颜色
    
    // 状态标志
    int flags;
    
    // 组件层次
    ui_component_t *parent;
    ui_component_t **children;
    int child_count;
    int child_capacity;
    
    // 事件处理
    ui_event_handler_t event_handlers[UI_EVENT_MAX];
    void *event_handler_user_data[UI_EVENT_MAX];
    
    // 用户数据
    void *user_data;
    
    // 双击检测
    int last_click_time;
    float last_click_x;
    float last_click_y;
    
    // 拖拽相关
    float drag_start_x;
    float drag_start_y;
    float drag_offset_x;
    float drag_offset_y;
};

// ==================== 事件操作 ====================

void UIEvent_StopPropagation(ui_event_t *event);
void UIEvent_PreventDefault(ui_event_t *event);

// ==================== 组件标志操作 ====================

bool UIComponent_IsVisible(const ui_component_t *component);
bool UIComponent_IsEnabled(const ui_component_t *component);
bool UIComponent_IsFocused(const ui_component_t *component);
bool UIComponent_IsHovered(const ui_component_t *component);

void UIComponent_SetVisible(ui_component_t *component, bool visible);
void UIComponent_SetEnabled(ui_component_t *component, bool enabled);
void UIComponent_SetFocused(ui_component_t *component, bool focused);

const char* UIComponent_GetTypeName(int typeid);

// ==================== 事件处理 ====================

bool UIComponent_AddEventHandler(ui_component_t *component, ui_event_type_t event_type,
                                 ui_event_handler_t handler, void *user_data);
bool UIComponent_RemoveEventHandler(ui_component_t *component, ui_event_type_t event_type);
bool UIComponent_TriggerEvent(ui_component_t *component, ui_event_t *event);

// ==================== 坐标转换 ====================

void UIComponent_ScreenToLocal(ui_component_t *component, float screen_x, float screen_y,
                                float *local_x, float *local_y);
void UIComponent_LocalToScreen(ui_component_t *component, float local_x, float local_y,
                                float *screen_x, float *screen_y);

// ==================== 双击检测 ====================

bool UIComponent_CheckDoubleClick(ui_component_t *component, float x, float y, int current_time);

// ==================== 基础组件操作 ====================

void UIComponent_InitBase(ui_component_t *component, ui_component_type_t type,
                          const ui_component_vtable_t *vtable, canvas2d_context_t *ctx);
void UIComponent_ShutdownBase(ui_component_t *component);
void UIComponent_Destroy(ui_component_t *component);

// ==================== 组件渲染 ====================

void UIComponent_Render(ui_component_t *component);
void UIComponent_RenderBackground(ui_component_t *component);
void UIComponent_RenderBorder(ui_component_t *component);

// ==================== 样式相关函数 ====================

void UIComponent_SetBgColor(ui_component_t *component, COLOR32 color);
COLOR32 UIComponent_GetBgColor(const ui_component_t *component);

void UIComponent_SetMargin(ui_component_t *component, float top, float right, float bottom, float left);
void UIComponent_GetMargin(const ui_component_t *component, float *top, float *right, float *bottom, float *left);

void UIComponent_SetPadding(ui_component_t *component, float top, float right, float bottom, float left);
void UIComponent_GetPadding(const ui_component_t *component, float *top, float *right, float *bottom, float *left);

void UIComponent_GetContentRect(const ui_component_t *component, float *x, float *y,
                                float *width, float *height);

// ==================== 布局API - 对使用者隐藏lay细节 ====================

// 设置布局上下文
void UIComponent_SetLayoutContext(ui_component_t *component, lay_context *ctx);

// 创建布局项（用于初始化根组件）
lay_id UIComponent_CreateLayoutItem(ui_component_t *component);

// 获取布局项ID
lay_id UIComponent_GetLayoutItem(const ui_component_t *component);

// 设置组件的尺寸（自动同步到布局系统）
void UIComponent_SetSize(ui_component_t *component, float width, float height);

// 获取组件的尺寸
void UIComponent_GetSize(const ui_component_t *component, float *width, float *height);

// 设置布局尺寸（旧API，用于初始化）
void UIComponent_SetLayoutSize(ui_component_t *component, float width, float height);

// 设置组件的边距（自动同步到布局系统）
void UIComponent_SetMarginLayout(ui_component_t *component, float top, float right, float bottom, float left);

// 设置布局边距（旧API）
void UIComponent_SetLayoutMargins(ui_component_t *component, float left, float top, float right, float bottom);

// 设置布局行为标志（如 LAY_HFILL, LAY_VFILL 等）
void UIComponent_SetBehave(ui_component_t *component, uint32_t flags);

// 设置布局容器标志（如 LAY_ROW, LAY_COLUMN 等）
void UIComponent_SetContain(ui_component_t *component, uint32_t flags);

// 设置布局容器标志（旧API别名）
void UIComponent_SetLayoutContain(ui_component_t *component, uint32_t flags);

// 设置组件位置（直接设置，不经过布局系统）
void UIComponent_SetPosition(ui_component_t *component, float x, float y);

// 运行布局计算并应用到整个组件树
void UIComponent_Layout(ui_component_t *root);

// ==================== 调试函数 ====================

// 打印布局树（用于调试）
void UIComponent_PrintTree(const ui_component_t *component, int indent);

#endif // __UI_COMPONENT_H__
