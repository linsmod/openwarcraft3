#ifndef __SCENE_H__
#define __SCENE_H__

#include "../common/shared.h"
#include "canvas2d/canvas2d.h"
#include "../common/event.h"
#include "ui/ui_component.h"

// 前向声明
typedef struct scene_t scene_t;
typedef struct scene_manager_t scene_manager_t;
typedef struct scene_params_t scene_params_t;
typedef struct scene_transition_t scene_transition_t;
typedef struct ui_component_t ui_component_t;

// ========================================
// 场景参数系统 - 支持场景间参数传递
// ========================================

// 参数类型
typedef enum {
    PARAM_TYPE_STRING,
    PARAM_TYPE_INT,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_BOOL,
    PARAM_TYPE_POINTER,
    PARAM_TYPE_CUSTOM
} param_type_t;

// 参数项
typedef struct {
    char key[64];
    param_type_t type;
    union {
        char string_val[256];
        int int_val;
        float float_val;
        bool bool_val;
        void* ptr_val;
    };
    size_t custom_size;  // 用于自定义类型
    void* custom_data;   // 自定义数据（需要用户管理释放）
} scene_param_t;

// 参数容器
struct scene_params_t {
    scene_param_t params[32];
    int count;
    bool shared;  // 是否为共享实例（不应被销毁）
};

// 创建和销毁参数容器
scene_params_t* SceneParams_Create(void);
scene_params_t* SceneParams_CreateShared(void);
void SceneParams_Destroy(scene_params_t *params);
void SceneParams_Clear(scene_params_t *params);

// 设置参数
void SceneParams_SetString(scene_params_t *params, const char *key, const char *value);
void SceneParams_SetInt(scene_params_t *params, const char *key, int value);
void SceneParams_SetFloat(scene_params_t *params, const char *key, float value);
void SceneParams_SetBool(scene_params_t *params, const char *key, bool value);
void SceneParams_SetPointer(scene_params_t *params, const char *key, void* value);
void SceneParams_SetCustom(scene_params_t *params, const char *key, void* data, size_t size);

// 获取参数
bool SceneParams_GetString(const scene_params_t *params, const char *key, char* output, size_t max_len);
bool SceneParams_GetInt(const scene_params_t *params, const char *key, int* output);
bool SceneParams_GetFloat(const scene_params_t *params, const char *key, float* output);
bool SceneParams_GetBool(const scene_params_t *params, const char *key, bool* output);
bool SceneParams_GetPointer(const scene_params_t *params, const char *key, void** output);
bool SceneParams_GetCustom(const scene_params_t *params, const char *key, void** output, size_t* size);

// 检查参数是否存在
bool SceneParams_Has(const scene_params_t *params, const char *key);

// ========================================
// 场景跳转类型
// ========================================
typedef enum {
    TRANSITION_SWITCH,   // 切换场景（关闭当前，打开新场景）
    TRANSITION_PUSH,     // 压入场景（叠加，不关闭当前）
    TRANSITION_POP,      // 弹出场景（返回上一个场景）
    TRANSITION_POP_TO,   // 弹出到指定场景
    TRANSITION_QUIT      // 退出程序
} transition_type_t;

// 场景跳转请求
struct scene_transition_t {
    transition_type_t type;
    scene_t* target_scene;          // 目标场景（用于SWITCH/PUSH）
    const char* target_scene_name;   // 目标场景名称（可选）
    scene_params_t* params;          // 传递给目标场景的参数
    scene_params_t* result;          // 返回给调用者的结果
    bool executed;                   // 是否已执行
};

// ========================================
// 场景状态
// ========================================
typedef enum {
    SCENE_STATE_UNINITIALIZED,
    SCENE_STATE_INITIALIZED,
    SCENE_STATE_ACTIVE,
    SCENE_STATE_PAUSED,
    SCENE_STATE_TRANSITION_OUT,
    SCENE_STATE_SHUTDOWN
} scene_state_t;

// ========================================
// 输入事件 - 现在使用统一的 event_t
// 定义在 src/common/event.h 中
// ========================================

// 向后兼容的类型别名
typedef event_t input_event_t;
typedef event_type_t input_event_type_t;

// 向后兼容的宏定义
#define INPUT_EVENT_KEY_DOWN       EVENT_KEY_DOWN
#define INPUT_EVENT_KEY_UP         EVENT_KEY_UP
#define INPUT_EVENT_MOUSE_DOWN     EVENT_MOUSE_DOWN
#define INPUT_EVENT_MOUSE_UP       EVENT_MOUSE_UP
#define INPUT_EVENT_MOUSE_MOTION   EVENT_MOUSE_MOTION
#define INPUT_EVENT_MOUSE_WHEEL    EVENT_MOUSE_WHEEL
#define INPUT_EVENT_TEXT_INPUT     EVENT_TEXT_INPUT
#define INPUT_EVENT_TEXT_EDITING   EVENT_TEXT_EDITING
#define INPUT_EVENT_QUIT           EVENT_QUIT
#define INPUT_EVENT_CLICK          EVENT_CLICK
#define INPUT_EVENT_DOUBLE_CLICK   EVENT_DOUBLE_CLICK
#define INPUT_EVENT_DRAG_START     EVENT_DRAG_START
#define INPUT_EVENT_DRAG           EVENT_DRAG
#define INPUT_EVENT_DRAG_END       EVENT_DRAG_END
#define INPUT_EVENT_MOUSE_ENTER    EVENT_MOUSE_ENTER
#define INPUT_EVENT_MOUSE_LEAVE    EVENT_MOUSE_LEAVE

// ========================================
// 场景接口函数指针类型
// ========================================
typedef int (*scene_init_fn)(scene_t *scene, const scene_params_t *params);
typedef void (*scene_shutdown_fn)(scene_t *scene);
typedef scene_transition_t* (*scene_update_fn)(scene_t *scene, int msec);
typedef void (*scene_layout_fn)(scene_t *scene);
typedef void (*scene_render_background_fn)(scene_t *scene, size2_t vpsize);
typedef void (*scene_render_fn)(scene_t *scene);
typedef void (*scene_pause_fn)(scene_t *scene, const scene_params_t *result);
typedef void (*scene_resume_fn)(scene_t *scene);
typedef void (*scene_on_input_fn)(scene_t *scene, input_event_t *event);

// ========================================
// 场景结构
// ========================================
#include "html/layout.h"
struct scene_t {
    ui_component_t *root_component;
    const char *name;
    scene_state_t state;
    void *user_data;
    
    // 启动参数（由SceneManager管理）
    const scene_params_t *launch_params;
    canvas2d_t *canvas;
    canvas2d_context_t *canvas_ctx;
    lay_context *lay_ctx;
    
    // 场景接口函数
    scene_init_fn init;
    scene_shutdown_fn shutdown;
    scene_update_fn update;      // 可返回跳转请求
    scene_layout_fn layout;
    scene_render_fn render;
    scene_pause_fn pause;        // 暂停时接收返回值
    scene_resume_fn resume;
    scene_on_input_fn on_input;  // 可返回跳转请求
    
    // 场景管理器指针
    scene_manager_t *manager;
};

// ========================================
// 场景管理器
// ========================================
struct scene_manager_t {
    scene_t *stack[8];  // 场景栈，最多支持8层
    int stack_size;
    scene_t *current_scene;
    
    // ============= 默认资源系统 =============
    // 所有场景共享的默认资源（可选使用）
    void *default_canvas;              // 默认 canvas (void* 避免循环依赖)
    void *default_canvas_ctx;          // 默认 canvas 上下文
    void *default_lay_ctx;             // 默认 lay 布局上下文
    void *default_event_dispatcher;     // 默认事件分发器
    ui_component_t *default_root;     // 默认根容器
    
    // 场景注册表（用于通过名称查找场景）
    scene_t *registered_scenes[32];  // 最多支持32个场景
    int registered_count;
    
    // 待处理的跳转请求
    scene_transition_t *pending_transition;
    
    // 当前输入事件
    event_t current_input_event;
    bool input_event_valid;
    
    // 鼠标状态追踪（用于生成细化的事件）
    ui_component_t *mouse_target;       // 当前鼠标命中的组件
    ui_component_t *dragging_component;  // 当前拖拽的组件
    float drag_start_x;                  // 拖拽起始X
    float drag_start_y;                  // 拖拽起始Y
    bool is_dragging;                   // 是否正在拖拽
    bool mouse_buttons[5];               // 鼠标按钮状态
    ui_component_t *last_clicked;       // 上次点击的组件
    int last_click_time;                 // 上次点击时间
    float last_click_x;                 // 上次点击X
    float last_click_y;                 // 上次点击Y
    int double_click_time;               // 双击检测时间间隔（毫秒）
    float drag_threshold;               // 拖拽阈值（像素）
};

// ========================================
// 场景参数辅助宏
// ========================================
#define PARAMS_GET_STRING(params, key, default_val) \
    ({ char val[256] = default_val; SceneParams_GetString(params, key, val, sizeof(val)); val; })

#define PARAMS_GET_INT(params, key, default_val) \
    ({ int val = default_val; SceneParams_GetInt(params, key, &val); val; })

#define PARAMS_GET_FLOAT(params, key, default_val) \
    ({ float val = default_val; SceneParams_GetFloat(params, key, &val); val; })

#define PARAMS_GET_BOOL(params, key, default_val) \
    ({ bool val = default_val; SceneParams_GetBool(params, key, &val); val; })

#define PARAMS_GET_POINTER(params, key, default_val) \
    ({ void* val = default_val; SceneParams_GetPointer(params, key, &val); val; })

// ========================================
// 场景跳转辅助宏
// ========================================
// 创建切换场景请求
#define TRANSITION_SWITCH_TO(scene_name, params) \
    SceneTransition_Create(TRANSITION_SWITCH, scene_name, params, NULL)

// 创建压入场景请求
#define TRANSITION_PUSH_TO(scene_name, params) \
    SceneTransition_Create(TRANSITION_PUSH, scene_name, params, NULL)

// 创建弹出场景请求（带返回值）
#define TRANSITION_POP_WITH(result) \
    SceneTransition_Create(TRANSITION_POP, NULL, NULL, result)

// 创建退出请求
#define TRANSITION_QUIT_GAME() \
    SceneTransition_Create(TRANSITION_QUIT, NULL, NULL, NULL)

// ========================================
// 场景管理API
// ========================================
scene_manager_t* SceneManager_Create(int width, int height);  // 创建时指定默认canvas尺寸
void SceneManager_Destroy(scene_manager_t *mgr);

// 场景注册
int SceneManager_RegisterScene(scene_manager_t *mgr, scene_t *scene);

// 场景栈操作（支持参数传递）
void SceneManager_PushScene(scene_manager_t *mgr, scene_t *scene, const scene_params_t *params);
void SceneManager_PopScene(scene_manager_t *mgr, const scene_params_t *result);
void SceneManager_SwitchScene(scene_manager_t *mgr, scene_t *scene, const scene_params_t *params);
void SceneManager_Transition(scene_manager_t *mgr, scene_transition_t *transition);

// 场景查询
scene_t* SceneManager_GetCurrentScene(scene_manager_t *mgr);
scene_t* SceneManager_GetSceneByName(scene_manager_t *mgr, const char *name);
scene_t* SceneManager_GetSceneByIndex(scene_manager_t *mgr, int index);

// 检查场景是否已注册
bool SceneManager_IsSceneRegistered(scene_manager_t *mgr, const char *name);

// 获取调用者的返回值（用于pause时的result）
const scene_params_t* SceneManager_GetPreviousResult(scene_manager_t *mgr);

// ============= 默认资源API =============
// 获取默认 canvas
void* SceneManager_GetDefaultCanvas(scene_manager_t *mgr);
void* SceneManager_GetDefaultCanvasContext(scene_manager_t *mgr);

// 获取默认布局上下文
void* SceneManager_GetDefaultLayoutContext(scene_manager_t *mgr);

// 获取默认事件分发器
void* SceneManager_GetDefaultEventDispatcher(scene_manager_t *mgr);

// 获取默认根容器
ui_component_t* SceneManager_GetDefaultRoot(scene_manager_t *mgr);

// ============= 场景资源管理 =============
// 初始化场景时自动使用默认资源
void SceneManager_InitSceneWithDefaults(scene_manager_t *mgr, scene_t *scene);

// 主循环接口
void SceneManager_Update(scene_manager_t *mgr, int msec);
void SceneManager_Render(scene_manager_t *mgr);
void SceneManager_OnInput(scene_manager_t *mgr, event_t *event);

// ========================================
// 场景跳转请求API
// ========================================
// 创建跳转请求
scene_transition_t* SceneTransition_Create(
    transition_type_t type,
    scene_t* target_scene,
    const scene_params_t* params,
    const scene_params_t* result
);

// 创建通过名称查找的跳转请求
scene_transition_t* SceneTransition_CreateByName(
    transition_type_t type,
    const char* target_scene_name,
    const scene_params_t* params,
    const scene_params_t* result
);

// 销毁跳转请求
void SceneTransition_Destroy(scene_transition_t *transition);

// ========================================
// 场景辅助宏（保持向后兼容）
// ========================================
#define SCENE_INIT(scene, params) \
    ((scene)->init ? (scene)->init(scene, params) : 0)

#define SCENE_SHUTDOWN(scene) \
    if ((scene)->shutdown) (scene)->shutdown(scene)

#define SCENE_UPDATE(scene, msec) \
    ((scene)->update ? (scene)->update(scene, msec) : NULL)

#define SCENE_RENDER(scene) \
    if ((scene)->render) (scene)->render(scene)

#define SCENE_PAUSE(scene, result) \
    if ((scene)->pause) (scene)->pause(scene, result)

#define SCENE_RESUME(scene) \
    if ((scene)->resume) (scene)->resume(scene)

#define SCENE_ON_INPUT(scene, event) \
    ((scene)->on_input ? (scene)->on_input(scene, event) : NULL)

// ========================================
// 场景管理器事件处理辅助函数
// ========================================
// 初始化场景管理器的鼠标状态
void SceneManager_InitMouseState(scene_manager_t *mgr);

// 执行hitTest（返回鼠标命中的组件）
ui_component_t* SceneManager_HitTest(scene_manager_t *mgr, float x, float y);

// 处理事件细化
void SceneManager_ProcessEvent(scene_manager_t *mgr, event_t *event);

// 递归处理事件冒泡
bool SceneManager_BubbleEvent(scene_manager_t *mgr, ui_component_t *target, input_event_t *event);

// 场景构造宏（新版）
#define DEFINE_SCENE(name, init_fn, shutdown_fn, update_fn, render_fn, on_input_fn) \
    scene_t name = { \
        .name = #name, \
        .state = SCENE_STATE_UNINITIALIZED, \
        .user_data = NULL, \
        .launch_params = NULL, \
        .root_component = NULL, \
        .manager = NULL, \
        .init = init_fn, \
        .shutdown = shutdown_fn, \
        .update = update_fn, \
        .render = render_fn, \
        .on_input = on_input_fn, \
        .pause = NULL, \
        .resume = NULL \
    }

// ========================================
// 场景UI辅助函数
// ========================================
// 设置场景的根容器组件
void Scene_SetRootComponent(scene_t *scene, ui_component_t *root);

// 获取场景的根容器组件
ui_component_t* Scene_GetRootComponent(scene_t *scene);

// 更新场景的所有UI组件
void Scene_UpdateUI(scene_t *scene, int msec);

// 渲染场景的所有UI组件
void Scene_RenderUI(scene_t *scene);

#endif // __SCENE_H__
