#ifndef __SCENE_EVENTS_H__
#define __SCENE_EVENTS_H__

#include "../common/shared.h"

// ========================================
// 细粒度事件系统
// ========================================

// 前向声明
typedef struct scene_t scene_t;
typedef struct scene_transition_t scene_transition_t;

// ========================================
// 事件类型枚举
// ========================================
typedef enum {
    // 键盘事件
    SCENE_EVENT_KEY_DOWN,
    SCENE_EVENT_KEY_UP,
    
    // 鼠标事件
    SCENE_EVENT_MOUSE_DOWN,
    SCENE_EVENT_MOUSE_UP,
    SCENE_EVENT_MOUSE_MOVE,
    SCENE_EVENT_MOUSE_WHEEL,
    
    // 生命周期事件
    SCENE_EVENT_ACTIVATE,     // 场景激活（切换为当前场景）
    SCENE_EVENT_DEACTIVATE,   // 场景失活（被暂停或切换出去）
    SCENE_EVENT_RESUME,       // 场景恢复（从暂停状态返回）
    
    // 窗口事件
    SCENE_EVENT_RESIZE,
    SCENE_EVENT_FOCUS_GAINED,
    SCENE_EVENT_FOCUS_LOST,
    
    // 游戏特有事件
    SCENE_EVENT_GAME_START,     // 游戏开始
    SCENE_EVENT_GAME_END,       // 游戏结束
    SCENE_EVENT_PLAYER_DIED,     // 玩家死亡
    SCENE_EVENT_LEVEL_COMPLETE,  // 关卡完成
    
    // 退出事件
    SCENE_EVENT_QUIT,
    
    // 未知事件
    SCENE_EVENT_UNKNOWN
} scene_event_type_t;

// ========================================
// 事件数据结构
// ========================================

// 键盘事件数据
typedef struct {
    int key;          // SDL key code
    bool repeat;       // 是否是重复按键（按住不放）
    bool ctrl;         // Ctrl键状态
    bool shift;        // Shift键状态
    bool alt;          // Alt键状态
} scene_key_event_t;

// 鼠标按下/释放事件数据
typedef struct {
    int button;        // 按钮编号（1=左，2=中，3=右）
    float x;           // 屏幕坐标X
    float y;           // 屏幕坐标Y
    int click_count;   // 点击次数（用于双击检测）
} scene_mouse_button_event_t;

// 鼠标移动事件数据
typedef struct {
    float x;           // 当前位置X
    float y;           // 当前位置Y
    float dx;          // 相对位置X（移动距离）
    float dy;          // 相对位置Y（移动距离）
} scene_mouse_move_event_t;

// 鼠标滚轮事件数据
typedef struct {
    float delta;       // 滚动增量（正值向上，负值向下）
} scene_mouse_wheel_event_t;

// 窗口调整大小事件数据
typedef struct {
    int width;
    int height;
} scene_resize_event_t;

// 游戏事件数据
typedef struct {
    int reason;        // 游戏结束原因
    void* data;        // 额外数据
} scene_game_event_t;

// 统一事件结构
typedef struct {
    scene_event_type_t type;
    float timestamp;    // 事件时间戳（秒）
    bool handled;       // 事件是否已处理
    
    union {
        scene_key_event_t key;
        scene_mouse_button_event_t mouse_button;
        scene_mouse_move_event_t mouse_move;
        scene_mouse_wheel_event_t mouse_wheel;
        scene_resize_event_t resize;
        scene_game_event_t game;
    };
} scene_event_t;

// ========================================
// 场景事件处理器 - 细粒度回调
// ========================================

// 键盘事件处理器
typedef scene_transition_t* (*scene_on_key_down_fn)(scene_t *scene, const scene_key_event_t *event);
typedef scene_transition_t* (*scene_on_key_up_fn)(scene_t *scene, const scene_key_event_t *event);

// 鼠标事件处理器
typedef scene_transition_t* (*scene_on_mouse_down_fn)(scene_t *scene, const scene_mouse_button_event_t *event);
typedef scene_transition_t* (*scene_on_mouse_up_fn)(scene_t *scene, const scene_mouse_button_event_t *event);
typedef scene_transition_t* (*scene_on_mouse_move_fn)(scene_t *scene, const scene_mouse_move_event_t *event);
typedef scene_transition_t* (*scene_on_mouse_wheel_fn)(scene_t *scene, const scene_mouse_wheel_event_t *event);

// 生命周期事件处理器
typedef void (*scene_on_activate_fn)(scene_t *scene);
typedef void (*scene_on_deactivate_fn)(scene_t *scene);
typedef void (*scene_on_resume_fn)(scene_t *scene);

// 窗口事件处理器
typedef void (*scene_on_resize_fn)(scene_t *scene, const scene_resize_event_t *event);
typedef void (*scene_on_focus_gained_fn)(scene_t *scene);
typedef void (*scene_on_focus_lost_fn)(scene_t *scene);

// 游戏事件处理器
typedef scene_transition_t* (*scene_on_game_start_fn)(scene_t *scene);
typedef scene_transition_t* (*scene_on_game_end_fn)(scene_t *scene, const scene_game_event_t *event);
typedef void (*scene_on_player_died_fn)(scene_t *scene, const scene_game_event_t *event);
typedef void (*scene_on_level_complete_fn)(scene_t *scene, const scene_game_event_t *event);

// 退出事件处理器
typedef scene_transition_t* (*scene_on_quit_fn)(scene_t *scene);

// ========================================
// 场景事件处理器集合
// ========================================
typedef struct scene_event_handlers_t {
    // 键盘事件
    scene_on_key_down_fn on_key_down;
    scene_on_key_up_fn on_key_up;
    
    // 鼠标事件
    scene_on_mouse_down_fn on_mouse_down;
    scene_on_mouse_up_fn on_mouse_up;
    scene_on_mouse_move_fn on_mouse_move;
    scene_on_mouse_wheel_fn on_mouse_wheel;
    
    // 生命周期事件
    scene_on_activate_fn on_activate;
    scene_on_deactivate_fn on_deactivate;
    scene_on_resume_fn on_resume;
    
    // 窗口事件
    scene_on_resize_fn on_resize;
    scene_on_focus_gained_fn on_focus_gained;
    scene_on_focus_lost_fn on_focus_lost;
    
    // 游戏事件
    scene_on_game_start_fn on_game_start;
    scene_on_game_end_fn on_game_end;
    scene_on_player_died_fn on_player_died;
    scene_on_level_complete_fn on_level_complete;
    
    // 退出事件
    scene_on_quit_fn on_quit;
} scene_event_handlers_t;

#endif // __SCENE_EVENTS_H__
