
#include "scene.h"
#include "../ui/ui_component.h"
#include "../ui/ui_container.h"

#include "../html/layout.h"
#include "../canvas2d/canvas2d.h"
#include "common/shared.h"
#include "r_local.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>

// ========================================
// 场景参数系统实现
// ========================================

scene_params_t* SceneParams_Create() {
    scene_params_t *params = (scene_params_t*)malloc(sizeof(scene_params_t));
    if (!params) return NULL;
    
    memset(params, 0, sizeof(scene_params_t));
    return params;
}

scene_params_t* SceneParams_CreateShared() {
    scene_params_t *params = (scene_params_t*)malloc(sizeof(scene_params_t));
    if (!params) return NULL;
    
    memset(params, 0, sizeof(scene_params_t));
    params->shared = true;
    return params;
}

void SceneParams_Destroy(scene_params_t *params) {
    if (!params) return;
    
    if(params->shared) {
        // 共享实例不应被销毁
        return;
    }
    // 释放自定义数据
    for (int i = 0; i < params->count; i++) {
        if (params->params[i].type == PARAM_TYPE_CUSTOM) {
            free(params->params[i].custom_data);
        }
    }
    
    free(params);
}

void SceneParams_Clear(scene_params_t *params) {
    if (!params) return;
    
    // 释放自定义数据
    for (int i = 0; i < params->count; i++) {
        if (params->params[i].type == PARAM_TYPE_CUSTOM) {
            free(params->params[i].custom_data);
        }
    }
    
    memset(params, 0, sizeof(scene_params_t));
}

// 内部辅助函数：查找参数
static scene_param_t* find_param(scene_params_t *params, const char *key) {
    if (!params || !key) return NULL;
    
    for (int i = 0; i < params->count; i++) {
        if (strcmp(params->params[i].key, key) == 0) {
            return &params->params[i];
        }
    }
    return NULL;
}

// 内部辅助函数：添加或更新参数
static scene_param_t* add_or_update_param(scene_params_t *params, const char *key, param_type_t type) {
    if (!params || !key) return NULL;
    if (params->count >= 32) {
        printf("SceneParams: Maximum number of parameters (32) reached\n");
        return NULL;
    }
    
    // 检查是否已存在
    scene_param_t *param = find_param(params, key);
    if (param) {
        // 更新现有参数
        param->type = type;
        return param;
    }
    
    // 添加新参数
    param = &params->params[params->count++];
    strncpy(param->key, key, sizeof(param->key) - 1);
    param->key[sizeof(param->key) - 1] = '\0';
    param->type = type;
    return param;
}

// 设置参数
void SceneParams_SetString(scene_params_t *params, const char *key, const char *value) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_STRING);
    if (param && value) {
        strncpy(param->string_val, value, sizeof(param->string_val) - 1);
        param->string_val[sizeof(param->string_val) - 1] = '\0';
    }
}

void SceneParams_SetInt(scene_params_t *params, const char *key, int value) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_INT);
    if (param) {
        param->int_val = value;
    }
}

void SceneParams_SetFloat(scene_params_t *params, const char *key, float value) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_FLOAT);
    if (param) {
        param->float_val = value;
    }
}

void SceneParams_SetBool(scene_params_t *params, const char *key, bool value) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_BOOL);
    if (param) {
        param->bool_val = value;
    }
}

void SceneParams_SetPointer(scene_params_t *params, const char *key, void* value) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_POINTER);
    if (param) {
        param->ptr_val = value;
    }
}

void SceneParams_SetCustom(scene_params_t *params, const char *key, void* data, size_t size) {
    scene_param_t *param = add_or_update_param(params, key, PARAM_TYPE_CUSTOM);
    if (param && data) {
        // 分配并复制数据
        param->custom_data = malloc(size);
        if (param->custom_data) {
            memcpy(param->custom_data, data, size);
            param->custom_size = size;
        } else {
            printf("SceneParams: Failed to allocate memory for custom data\n");
        }
    }
}

// 获取参数
bool SceneParams_GetString(const scene_params_t *params, const char *key, char* output, size_t max_len) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_STRING) {
        strncpy(output, param->string_val, max_len - 1);
        output[max_len - 1] = '\0';
        return true;
    }
    return false;
}

bool SceneParams_GetInt(const scene_params_t *params, const char *key, int* output) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_INT) {
        *output = param->int_val;
        return true;
    }
    return false;
}

bool SceneParams_GetFloat(const scene_params_t *params, const char *key, float* output) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_FLOAT) {
        *output = param->float_val;
        return true;
    }
    return false;
}

bool SceneParams_GetBool(const scene_params_t *params, const char *key, bool* output) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_BOOL) {
        *output = param->bool_val;
        return true;
    }
    return false;
}

bool SceneParams_GetPointer(const scene_params_t *params, const char *key, void** output) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_POINTER) {
        *output = param->ptr_val;
        return true;
    }
    return false;
}

bool SceneParams_GetCustom(const scene_params_t *params, const char *key, void** output, size_t* size) {
    scene_param_t *param = find_param((scene_params_t*)params, key);
    if (param && param->type == PARAM_TYPE_CUSTOM) {
        *output = param->custom_data;
        if (size) *size = param->custom_size;
        return true;
    }
    return false;
}

bool SceneParams_Has(const scene_params_t *params, const char *key) {
    return find_param((scene_params_t*)params, key) != NULL;
}


void SceneParams_Copy(const scene_params_t* src, scene_params_t** dest) {
    if (!src || !dest) return;
    
    *dest = SceneParams_Create();
    if (!*dest) return;
    
    for (int i = 0; i < src->count; i++) {
        const scene_param_t *src_param = &src->params[i];
        switch (src_param->type) {
            case PARAM_TYPE_STRING:
                SceneParams_SetString(*dest, src_param->key, src_param->string_val);
                break;
            case PARAM_TYPE_INT:
                SceneParams_SetInt(*dest, src_param->key, src_param->int_val);
                break;
            case PARAM_TYPE_FLOAT:
                SceneParams_SetFloat(*dest, src_param->key, src_param->float_val);
                break;
            case PARAM_TYPE_BOOL:
                SceneParams_SetBool(*dest, src_param->key, src_param->bool_val);
                break;
            case PARAM_TYPE_POINTER:
                SceneParams_SetPointer(*dest, src_param->key, src_param->ptr_val);
                break;
            case PARAM_TYPE_CUSTOM:
                SceneParams_SetCustom(*dest, src_param->key, src_param->custom_data, src_param->custom_size);
                break;
            default:
                break;
        }
    }
}

// ========================================
// 场景跳转请求实现
// ========================================

scene_transition_t* SceneTransition_Create(
    transition_type_t type,
    scene_t* target_scene,
    const scene_params_t* params,
    const scene_params_t* result
) {
    scene_transition_t *transition = (scene_transition_t*)malloc(sizeof(scene_transition_t));
    if (!transition) return NULL;
    
    memset(transition, 0, sizeof(scene_transition_t));
    transition->type = type;
    transition->target_scene = target_scene;
    transition->target_scene_name = NULL;
    transition->params = (scene_params_t*)params;
    transition->result = (scene_params_t*)result;
    transition->executed = false;
    
    return transition;
}

scene_transition_t* SceneTransition_CreateByName(
    transition_type_t type,
    const char* target_scene_name,
    const scene_params_t* params,
    const scene_params_t* result
) {
    scene_transition_t *transition = SceneTransition_Create(type, NULL, params, result);
    if (transition) {
        transition->target_scene_name = target_scene_name;
    }
    return transition;
}

void SceneTransition_Destroy(scene_transition_t *transition) {
    if (!transition) return;
    // 注意：params和result由调用者管理，这里不释放
    free(transition);
}

// ========================================
// 场景管理器实现
// ========================================

scene_manager_t* SceneManager_Create(int width, int height) {
    scene_manager_t *mgr = (scene_manager_t*)malloc(sizeof(scene_manager_t));
    if (!mgr) return NULL;
    
    memset(mgr, 0, sizeof(scene_manager_t));
    mgr->registered_count = 0;
    
    // 初始化鼠标状态
    SceneManager_InitMouseState(mgr);
    
    // ============= 创建默认资源 =============
    // 注意：这里使用 void* 类型以避免循环依赖
    // 实际类型在实现中转换
    printf("SceneManager: Creating default resources (%dx%d)\n", width, height);
    
    // 创建默认 canvas
    mgr->default_canvas = canvas2d_create(width, height);
    if (mgr->default_canvas) {
        mgr->default_canvas_ctx = canvas2d_get_context((canvas2d_t *)mgr->default_canvas);
        mgr->default_lay_ctx = canvas2d_getlayctx((canvas2d_t *)mgr->default_canvas);
        printf("  Created default canvas and context\n");
    }
    
    // 创建默认根容器
    if (mgr->default_canvas_ctx) {
        mgr->default_root = (ui_component_t *)UIContainer_Create(
            0.0f, 0.0f, (float)width, (float)height,
            MAKE(COLOR32, 0, 0, 0, 0),  // 透明背景
            MAKE(COLOR32, 0, 0, 0, 0),
            (canvas2d_context_t *)mgr->default_canvas_ctx
        );
        if (mgr->default_root) {
            lay_insert(mgr->default_lay_ctx, 
                canvas2d_getlayid((canvas2d_t *)mgr->default_canvas), 
                mgr->default_root->lay_item_id);
            printf("  Created default root container\n");
        }
    }
    
    return mgr;
}

int SceneManager_RegisterScene(scene_manager_t *mgr, scene_t *scene) {
    if (!mgr || !scene) return -1;
    
    // 检查是否已注册
    for (int i = 0; i < mgr->registered_count; i++) {
        if (mgr->registered_scenes[i] == scene || 
            strcmp(mgr->registered_scenes[i]->name, scene->name) == 0) {
            printf("SceneManager: Scene '%s' already registered\n", scene->name);
            return -1;
        }
    }
    
    // 检查是否超过最大数量
    if (mgr->registered_count >= 32) {
        printf("SceneManager: Maximum number of scenes (32) reached\n");
        return -1;
    }
    
    // 注册场景
    mgr->registered_scenes[mgr->registered_count++] = scene;
    scene->manager = mgr;
    printf("SceneManager: Registered scene '%s'\n", scene->name);
    
    return 0;
}

void SceneManager_Destroy(scene_manager_t *mgr) {
    if (!mgr) return;
    
    // 清理待处理的跳转请求
    if (mgr->pending_transition) {
        SceneTransition_Destroy(mgr->pending_transition);
        mgr->pending_transition = NULL;
    }
    
    // 清理所有场景
    while (mgr->stack_size > 0) {
        SceneManager_PopScene(mgr, NULL);
    }
    
    // ============= 销毁默认资源 =============
    printf("SceneManager: Destroying default resources\n");
    
    // 释放鼠标捕获
    if (mgr->captured) {
        SDL_CaptureMouse(SDL_FALSE);
        mgr->captured = NULL;
    }
    
    // 销毁默认根容器（如果有）
    if (mgr->default_root) {
        UIContainer_Destroy((ui_container_t *)mgr->default_root);
        mgr->default_root = NULL;
        printf("  Destroyed default root container\n");
    }
    
    // 销毁默认布局上下文（如果有）
    if (mgr->default_lay_ctx) {
        lay_destroy_context((lay_context *)mgr->default_lay_ctx);
        free(mgr->default_lay_ctx);
        mgr->default_lay_ctx = NULL;
        printf("  Destroyed default layout context\n");
    }
    
    // 销毁默认 canvas（如果有）
    if (mgr->default_canvas) {
        canvas2d_destroy((canvas2d_t *)mgr->default_canvas);
        mgr->default_canvas = NULL;
        mgr->default_canvas_ctx = NULL;
        printf("  Destroyed default canvas\n");
    }
    
    free(mgr);
}

void SceneManager_PushScene(scene_manager_t *mgr, scene_t *scene, const scene_params_t *params) {
    if (!mgr || !scene || mgr->stack_size >= 8) {
        return;
    }
    
    // 如果有当前场景，先暂停它
    if (mgr->current_scene) {
        mgr->current_scene->state = SCENE_STATE_PAUSED;
        SCENE_PAUSE(mgr->current_scene, NULL);
    }
    
    // 设置启动参数
    scene->launch_params = params;
    scene->manager = mgr;
    scene->canvas = mgr->default_canvas;
    scene->canvas_ctx = mgr->default_canvas_ctx;
    scene->lay_ctx = mgr->default_lay_ctx;
    scene->root_component = mgr->default_root;
    
    // 初始化场景
    if (scene->state == SCENE_STATE_UNINITIALIZED) {
        if (SCENE_INIT(scene, params) == 0) {
            scene->state = SCENE_STATE_INITIALIZED;
        } else {
            printf("Scene %s initialization failed\n", scene->name);
            scene->launch_params = NULL;
            return;
        }
    } else {
        // 如果已经初始化过，调用resume
        SCENE_RESUME(scene);
    }
    
    scene->state = SCENE_STATE_ACTIVE;
    
    // 压入栈
    mgr->stack[mgr->stack_size++] = scene;
    mgr->current_scene = scene;
    
    printf("Pushed scene: %s (params count: %d)\n", scene->name, 
           params ? params->count : 0);
}

void SceneManager_PopScene(scene_manager_t *mgr, const scene_params_t *result) {
    if (!mgr || mgr->stack_size == 0) {
        return;
    }
    
    scene_t *scene = mgr->current_scene;
    if (!scene) return;
    
    printf("Popping scene: %s (result count: %d)\n", scene->name,
           result ? result->count : 0);
    
    // 关闭场景
    scene->state = SCENE_STATE_SHUTDOWN;
    SCENE_SHUTDOWN(scene);
    scene->launch_params = NULL;
    
    // 从栈中移除
    mgr->stack[--mgr->stack_size] = NULL;
    
    // 恢复上一个场景
    if (mgr->stack_size > 0) {
        mgr->current_scene = mgr->stack[mgr->stack_size - 1];
        mgr->current_scene->state = SCENE_STATE_ACTIVE;
        SCENE_RESUME(mgr->current_scene);
        printf("Resumed scene: %s\n", mgr->current_scene->name);
        
        // 传递返回值给暂停的场景（如果支持）
        if (mgr->current_scene->pause && result) {
            // 调用pause函数传递返回值
            mgr->current_scene->pause(mgr->current_scene, result);
        }
    } else {
        mgr->current_scene = NULL;
    }
}

void SceneManager_SwitchScene(scene_manager_t *mgr, scene_t *scene, const scene_params_t *params) {
    if (!mgr) return;
    
    // 如果当前有场景，先弹出
    if (mgr->current_scene) {
        SceneManager_PopScene(mgr, NULL);
    }
    
    // 压入新场景
    SceneManager_PushScene(mgr, scene, params);
}

void SceneManager_Transition(scene_manager_t *mgr, scene_transition_t *transition) {
    if (!mgr || !transition || transition->executed) {
        return;
    }
    
    transition->executed = true;
    
    switch (transition->type) {
        case TRANSITION_SWITCH: {
            scene_t *target = transition->target_scene;
            if (!target && transition->target_scene_name) {
                target = SceneManager_GetSceneByName(mgr, transition->target_scene_name);
            }
            if (target) {
                printf("Transition: SWITCH to %s\n", target->name);
                SceneManager_SwitchScene(mgr, target, transition->params);
            } else {
                printf("Transition failed: Target scene not found\n");
            }
            break;
        }
        
        case TRANSITION_PUSH: {
            scene_t *target = transition->target_scene;
            if (!target && transition->target_scene_name) {
                target = SceneManager_GetSceneByName(mgr, transition->target_scene_name);
            }
            if (target) {
                printf("Transition: PUSH to %s\n", target->name);
                SceneManager_PushScene(mgr, target, transition->params);
            } else {
                printf("Transition failed: Target scene not found\n");
            }
            break;
        }
        
        case TRANSITION_POP: {
            printf("Transition: POP (result count: %d)\n",
                   transition->result ? transition->result->count : 0);
            SceneManager_PopScene(mgr, transition->result);
            break;
        }
        
        case TRANSITION_QUIT:
            printf("Transition: QUIT\n");
            // 设置全局退出标志（需要在主循环中检查）
            break;
            
        default:
            printf("Unknown transition type: %d\n", transition->type);
            break;
    }
}

scene_t* SceneManager_GetCurrentScene(scene_manager_t *mgr) {
    return mgr ? mgr->current_scene : NULL;
}

scene_t* SceneManager_GetSceneByName(scene_manager_t *mgr, const char *name) {
    if (!mgr || !name) return NULL;
    
    // 优先在场景注册表中查找
    for (int i = 0; i < mgr->registered_count; i++) {
        if (strcmp(mgr->registered_scenes[i]->name, name) == 0) {
            return mgr->registered_scenes[i];
        }
    }
    
    // 回退：在场景栈中查找（向后兼容）
    for (int i = 0; i < mgr->stack_size; i++) {
        if (strcmp(mgr->stack[i]->name, name) == 0) {
            return mgr->stack[i];
        }
    }
    
    return NULL;
}

scene_t* SceneManager_GetSceneByIndex(scene_manager_t *mgr, int index) {
    if (!mgr || index < 0 || index >= mgr->stack_size) return NULL;
    return mgr->stack[index];
}

bool SceneManager_IsSceneRegistered(scene_manager_t *mgr, const char *name) {
    if (!mgr || !name) return false;
    
    for (int i = 0; i < mgr->registered_count; i++) {
        if (strcmp(mgr->registered_scenes[i]->name, name) == 0) {
            return true;
        }
    }
    
    return false;
}

const scene_params_t* SceneManager_GetPreviousResult(scene_manager_t *mgr) {
    // 这个函数暂时不实现，因为结果通过pause函数传递
    // 如果需要其他方式访问，可以存储在manager中
    return NULL;
}

void Scene_LayoutUI(scene_t *scene, int msec);

void SceneManager_Update(scene_manager_t *mgr, int msec) {
    if (!mgr || !mgr->current_scene) return;
    
    
    if(mgr->pending_transition) {
        SceneManager_Transition(mgr, mgr->pending_transition);
        SceneTransition_Destroy(mgr->pending_transition);
        mgr->pending_transition = NULL;
        return;
    }
    // 先更新非UI逻辑
    // 调用场景的update函数，检查是否有跳转请求
    scene_transition_t *transition = SCENE_UPDATE(mgr->current_scene, msec);
    if (transition) {
        mgr->pending_transition = transition;
        SceneManager_Transition(mgr, mgr->pending_transition);
        SceneTransition_Destroy(mgr->pending_transition);
        mgr->pending_transition = NULL;
    }

    // 再更新UI逻辑
    Scene_UpdateUI(mgr->current_scene, msec);

    // 从栈底到栈顶依次渲染（实现叠加效果）
    for (int i = 0; i < mgr->stack_size; i++) {
        if (mgr->stack[i]->state == SCENE_STATE_ACTIVE || 
            mgr->stack[i]->state == SCENE_STATE_PAUSED) {
            Scene_LayoutUI(mgr->stack[i], msec);
        }
    }
}

void SceneManager_Render(scene_manager_t *mgr) {
    if (!mgr) return;
    
    // 从栈底到栈顶依次渲染（实现叠加效果）
    for (int i = 0; i < mgr->stack_size; i++) {
        if (mgr->stack[i]->state == SCENE_STATE_ACTIVE || 
            mgr->stack[i]->state == SCENE_STATE_PAUSED) {
            Scene_RenderUI(mgr->stack[i]);
        }
    }
}

// ========================================
// 场景管理器事件处理辅助函数实现
// ========================================

void SceneManager_InitMouseState(scene_manager_t *mgr) {
    if (!mgr) return;
    
    mgr->mouse_target = NULL;
    mgr->dragging_component = NULL;
    mgr->drag_start_x = 0;
    mgr->drag_start_y = 0;
    mgr->is_dragging = false;
    memset(mgr->mouse_buttons, 0, sizeof(mgr->mouse_buttons));
    mgr->last_clicked = NULL;
    mgr->last_click_time = 0;
    mgr->last_click_x = 0;
    mgr->last_click_y = 0;
    mgr->double_click_time = 500;  // 双击时间间隔500ms
    mgr->drag_threshold = 3.0f;    // 拖拽阈值3像素
    
    // 初始化焦点和鼠标捕获
    mgr->focused = NULL;
    mgr->captured = NULL;
}

// 内部辅助函数：递归执行hitTest
static ui_component_t* component_hit_test(ui_component_t *component, float x, float y) {
    if (!component || !(component->flags & UI_FLAG_VISIBLE)) {
        return NULL;
    }
    
    // 检查鼠标坐标是否在组件边界内
    if (x >= component->x && x <= component->x + component->width &&
        y >= component->y && y <= component->y + component->height) {
        
        // 如果有子组件，从后往前检查（因为后渲染的在上层）
        if (component->children && component->child_count > 0) {
            for (int i = component->child_count - 1; i >= 0; i--) {
                ui_component_t *child = component->children[i];
                ui_component_t *hit_child = component_hit_test(child, x, y);
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

ui_component_t* SceneManager_HitTest(scene_manager_t *mgr, float x, float y) {
    if (!mgr || !mgr->current_scene || !mgr->current_scene->root_component) {
        return NULL;
    }
    
    // 从根组件开始递归遍历
    return component_hit_test(mgr->current_scene->root_component, x, y);
}

void static ProcessUIHandlessEvent(scene_t *scene, event_t *event){
        switch (event->type) {
            // 鼠标事件
            case EVENT_MOUSE_DOWN:
                if (scene->on_mouse_down) {
                    scene->on_mouse_down(scene, event);
                }
                break;
            case EVENT_MOUSE_UP:
                if (scene->on_mouse_up) {
                    scene->on_mouse_up(scene, event);
                }
                break;
            case EVENT_MOUSE_MOTION:
                if (scene->on_mouse_move) {
                    scene->on_mouse_move(scene, event);
                }
                break;
            case EVENT_MOUSE_WHEEL:
                if (scene->on_mouse_wheel) {
                    scene->on_mouse_wheel(scene, event);
                }
                break;
            case EVENT_CLICK:
                if (scene->on_click) {
                    scene->on_click(scene, event);
                }
                break;
            case EVENT_DOUBLE_CLICK:
                if (scene->on_double_click) {
                    scene->on_double_click(scene, event);
                }
                break;
            case EVENT_DRAG_START:
                if (scene->on_drag_start) {
                    scene->on_drag_start(scene, event);
                }
                break;
            case EVENT_DRAG:
                if (scene->on_drag) {
                    scene->on_drag(scene, event);
                }
                break;
            case EVENT_DRAG_END:
                if (scene->on_drag_end) {
                    scene->on_drag_end(scene, event);
                }
                break;
            case EVENT_MOUSE_ENTER:
                if (scene->on_mouse_enter) {
                    scene->on_mouse_enter(scene, event);
                }
                break;
            case EVENT_MOUSE_LEAVE:
                if (scene->on_mouse_leave) {
                    scene->on_mouse_leave(scene, event);
                }
                break;
            case EVENT_CONTEXT_MENU:
                if (scene->on_context_menu) {
                    scene->on_context_menu(scene, event);
                }
                break;
                
            // 键盘事件
            case EVENT_KEY_DOWN:
                if (scene->on_key_down) {
                    scene->on_key_down(scene, event);
                }
                break;
            case EVENT_KEY_UP:
                if (scene->on_key_up) {
                    scene->on_key_up(scene, event);
                }
                break;
            case EVENT_TEXT_INPUT:
                if (scene->on_text_input) {
                    scene->on_text_input(scene, event);
                }
                break;
                
            // 焦点事件
            case EVENT_FOCUS:
                if (scene->on_focus) {
                    scene->on_focus(scene, event);
                }
                break;
            case EVENT_BLUR:
                if (scene->on_blur) {
                    scene->on_blur(scene, event);
                }
                break;
                
            default:
                break;
        }
}
// ========================================
// 事件冒泡处理
// ========================================

// 事件优先级：
// 1. vtable 中的默认事件处理函数（如按钮的状态切换）
// 2. event_handlers 数组中注册的用户自定义事件处理器
// 3. 场景的 on_* 函数指针（用于没有子元素的场景处理事件的需要）

void SceneManager_BubbleEvent(scene_manager_t *mgr, ui_component_t *target, event_t *event) {
    if (!mgr || !event) return;
    if(!target){
        ProcessUIHandlessEvent(mgr->current_scene, event);
        return;
    }
    // 从目标组件开始，向上遍历到根组件（事件冒泡）
    ui_component_t *current = target;
    scene_t *scene = mgr->current_scene;
    bool handled = false;
    while (current) {
        event->current_target = current;
        // ========================================
        // 阶段1: 调用 vtable 中的默认事件处理函数
        // ========================================
        if (current->vtable) {
            switch (event->type) {
                // 鼠标事件
                case EVENT_MOUSE_DOWN:
                    if (current->vtable->on_mouse_down) {
                        handled = current->vtable->on_mouse_down(current, event);
                    }
                    break;
                case EVENT_MOUSE_UP:
                    if (current->vtable->on_mouse_up) {
                        handled = current->vtable->on_mouse_up(current, event);
                    }
                    break;
                case EVENT_MOUSE_MOTION:
                    if (current->vtable->on_mouse_move) {
                        handled = current->vtable->on_mouse_move(current, event);
                    }
                    break;
                case EVENT_MOUSE_WHEEL:
                    if (current->vtable->on_mouse_wheel) {
                        handled = current->vtable->on_mouse_wheel(current, event);
                    }
                    break;
                case EVENT_CLICK:
                    if (current->vtable->on_click) {
                        handled = current->vtable->on_click(current, event);
                    }
                    break;
                case EVENT_DOUBLE_CLICK:
                    if (current->vtable->on_double_click) {
                        handled = current->vtable->on_double_click(current, event);
                    }
                    break;
                case EVENT_DRAG_START:
                    if (current->vtable->on_drag_start) {
                        handled = current->vtable->on_drag_start(current, event);
                    }
                    break;
                case EVENT_DRAG:
                    if (current->vtable->on_drag) {
                        handled = current->vtable->on_drag(current, event);
                    }
                    break;
                case EVENT_DRAG_END:
                    if (current->vtable->on_drag_end) {
                        handled = current->vtable->on_drag_end(current, event);
                    }
                    break;
                case EVENT_MOUSE_ENTER:
                    if (current->vtable->on_mouse_enter) {
                        handled = current->vtable->on_mouse_enter(current, event);
                    }
                    break;
                case EVENT_MOUSE_LEAVE:
                    if (current->vtable->on_mouse_leave) {
                        handled = current->vtable->on_mouse_leave(current, event);
                    }
                    break;
                case EVENT_CONTEXT_MENU:
                    if (current->vtable->on_context_menu) {
                        handled = current->vtable->on_context_menu(current, event);
                    }
                    break;
                    
                // 键盘事件
                case EVENT_KEY_DOWN:
                    if (current->vtable->on_key_down) {
                        handled = current->vtable->on_key_down(current, event);
                    }
                    break;
                case EVENT_KEY_UP:
                    if (current->vtable->on_key_up) {
                        handled = current->vtable->on_key_up(current, event);
                    }
                    break;
                case EVENT_TEXT_INPUT:
                    if (current->vtable->on_text_input) {
                        handled = current->vtable->on_text_input(current, event);
                    }
                    break;
                    
                // 焦点事件
                case EVENT_FOCUS:
                    if (current->vtable->on_focus) {
                        handled = current->vtable->on_focus(current, event);
                    }
                    break;
                case EVENT_BLUR:
                    if (current->vtable->on_blur) {
                        handled = current->vtable->on_blur(current, event);
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        // ========================================
        // 阶段2: 调用用户通过 AddEventHandler 注册的事件处理器
        // ========================================
        if (!handled && event->type < EVENT_MAX) {
            event_handler_t handler = current->event_handlers[event->type];
            void *user_data = current->event_handler_user_data[event->type];
            if (handler) {
                handled = handler(current, event, user_data);
                if (handled) {
                    event->handled = true;
                }
            }
        }

        // 如果事件被标记为停止传播，则停止冒泡
        if (event->propagation_stopped) {
            return;
        }
        
        // 移动到父组件（继续冒泡）
        current = current->parent;
    }


    // ========================================
    // 阶段三， 发送给scene的事件函数处理
    // ========================================
    if(!handled){
        ProcessUIHandlessEvent(scene, event);
    }
}

void SceneManager_ProcessEvent(scene_manager_t *mgr, event_t *event) {
    if (!mgr || !event) return;
    
    event->timestamp = SDL_GetTicks();
    event->propagation_stopped = false;
    event->target = NULL;
    event->current_target = NULL;
    
    ui_component_t *hit_target = NULL;
    

    // 处理鼠标事件
    switch (event->type) {
        case INPUT_EVENT_MOUSE_DOWN: {
            // 更新鼠标按钮状态
            int button = event->mouse.button;
            if (button >= 0 && button < 5) {
                mgr->mouse_buttons[button] = true;
            }
            
            // 执行hitTest
            hit_target = SceneManager_HitTest(mgr, event->mouse.x, event->mouse.y);
            
            // 记录拖拽起始位置
            mgr->drag_start_x = event->mouse.x;
            mgr->drag_start_y = event->mouse.y;
            
            // 检测双击
            if (hit_target == mgr->last_clicked &&
                event->timestamp - mgr->last_click_time < mgr->double_click_time) {
                // 双击
                input_event_t click_event = *event;
                click_event.type = INPUT_EVENT_DOUBLE_CLICK;
                click_event.target = hit_target;
                click_event.mouse.click_count = 2;
                SceneManager_BubbleEvent(mgr, hit_target, &click_event);
                if (click_event.propagation_stopped) {
                    event->handled = true;
                    return;
                }
            } else {
                // 单击（暂时不生成单击事件，等待mouse_up）
                event->mouse.click_count = 1;
            }
            
            break;
        }
        
        case INPUT_EVENT_MOUSE_UP: {
            int button = event->mouse.button;
            if (button >= 0 && button < 5) {
                mgr->mouse_buttons[button] = false;
            }
            
            // 检查是否是拖拽结束
            if (mgr->is_dragging && mgr->dragging_component) {
                input_event_t drag_end_event = *event;
                drag_end_event.type = INPUT_EVENT_DRAG_END;
                drag_end_event.target = mgr->dragging_component;
                drag_end_event.mouse.start_x = mgr->drag_start_x;
                drag_end_event.mouse.start_y = mgr->drag_start_y;
                SceneManager_BubbleEvent(mgr, mgr->dragging_component, &drag_end_event);
                
                mgr->is_dragging = false;
                mgr->dragging_component = NULL;
                event->handled = drag_end_event.handled;
                return;
            }
            
            // 生成单击事件
            hit_target = SceneManager_HitTest(mgr, event->mouse.x, event->mouse.y);
            if (hit_target) {
                input_event_t click_event = *event;
                click_event.type = INPUT_EVENT_CLICK;
                click_event.target = hit_target;
                click_event.mouse.click_count = 1;
                SceneManager_BubbleEvent(mgr, hit_target, &click_event);
                
                // 记录点击信息用于双击检测
                mgr->last_clicked = hit_target;
                mgr->last_click_time = event->timestamp;
                mgr->last_click_x = event->mouse.x;
                mgr->last_click_y = event->mouse.y;
                
                if (click_event.propagation_stopped) {
                    event->handled = true;
                    return;
                }
            }
            break;
        }
        
        case INPUT_EVENT_MOUSE_MOTION: {
            float dx = event->motion.x - event->motion.dx;
            float dy = event->motion.y - event->motion.dy;
            
            // 检查是否是拖拽
            if (mgr->is_dragging && mgr->dragging_component) {
                input_event_t drag_event = *event;
                drag_event.type = INPUT_EVENT_DRAG;
                drag_event.target = mgr->dragging_component;
                drag_event.mouse.start_x = mgr->drag_start_x;
                drag_event.mouse.start_y = mgr->drag_start_y;
                drag_event.mouse.x = event->motion.x;
                drag_event.mouse.y = event->motion.y;
                SceneManager_BubbleEvent(mgr, mgr->dragging_component, &drag_event);
                event->handled = drag_event.handled;
                return;
            }
            
            // 检查是否开始拖拽
            if (mgr->mouse_buttons[0] || mgr->mouse_buttons[1] || mgr->mouse_buttons[2]) {
                float drag_dist_x = event->motion.x - mgr->drag_start_x;
                float drag_dist_y = event->motion.y - mgr->drag_start_y;
                float drag_dist = sqrtf(drag_dist_x * drag_dist_x + drag_dist_y * drag_dist_y);
                
                if (drag_dist > mgr->drag_threshold && !mgr->is_dragging) {
                    // 开始拖拽
                    hit_target = SceneManager_HitTest(mgr, event->motion.x, event->motion.y);
                    if (hit_target && hit_target->flags & UI_FLAG_DRAGGABLE) {
                        mgr->is_dragging = true;
                        mgr->dragging_component = hit_target;
                        
                        input_event_t drag_start_event = *event;
                        drag_start_event.type = INPUT_EVENT_DRAG_START;
                        drag_start_event.target = hit_target;
                        drag_start_event.mouse.start_x = mgr->drag_start_x;
                        drag_start_event.mouse.start_y = mgr->drag_start_y;
                        drag_start_event.mouse.x = event->motion.x;
                        drag_start_event.mouse.y = event->motion.y;
                        SceneManager_BubbleEvent(mgr, hit_target, &drag_start_event);
                        
                        if (drag_start_event.propagation_stopped) {
                            event->handled = true;
                            return;
                        }
                    }
                }
            }
            
            // 检测鼠标进入/离开
            hit_target = SceneManager_HitTest(mgr, event->motion.x, event->motion.y);
            if (hit_target != mgr->mouse_target) {
                // 鼠标离开
                if (mgr->mouse_target) {
                    input_event_t leave_event = *event;
                    leave_event.type = INPUT_EVENT_MOUSE_LEAVE;
                    leave_event.target = mgr->mouse_target;
                    SceneManager_BubbleEvent(mgr, mgr->mouse_target, &leave_event);
                }
                
                // 鼠标进入
                if (hit_target) {
                    input_event_t enter_event = *event;
                    enter_event.type = INPUT_EVENT_MOUSE_ENTER;
                    enter_event.target = hit_target;
                    SceneManager_BubbleEvent(mgr, hit_target, &enter_event);
                }
                
                mgr->mouse_target = hit_target;
            }
            break;
        }
        
        default:
            // 其他事件类型不需要细化
            SceneManager_BubbleEvent(mgr, hit_target, event);
            break;
    }
}

void SceneManager_OnInput(scene_manager_t *mgr, event_t *event) {
    if (!mgr || !event) return;
    
    // 处理事件细化（生成CLICK、DRAG、ENTER/LEAVE等细化事件）
    SceneManager_ProcessEvent(mgr, event);
    
    if (event->handled) {
        return;  // 事件已被细化逻辑处理
    }
}

// ========================================
// 焦点管理实现
// ========================================

void SceneManager_SetFocus(scene_manager_t *mgr, ui_component_t *component) {
    if (!mgr) return;

    ui_component_t *old_focus = mgr->focused;
    if (old_focus != component) {
        if (old_focus) {
            event_t blur_event = {
                .type = EVENT_BLUR,
                .timestamp = SDL_GetTicks()
            };
            SceneManager_BubbleEvent(mgr, old_focus, &blur_event);
            old_focus->flags &= ~UI_FLAG_FOCUSED;
        }

        if (component && (component->flags & UI_FLAG_ACCEPT_FOCUS)) {
            event_t focus_event = {
                .type = EVENT_FOCUS,
                .timestamp = SDL_GetTicks()
            };
            SceneManager_BubbleEvent(mgr, component, &focus_event);
            component->flags |= UI_FLAG_FOCUSED;
        }

        mgr->focused = component;
    }
}

ui_component_t* SceneManager_GetFocus(scene_manager_t *mgr) {
    return mgr ? mgr->focused : NULL;
}

void SceneManager_ClearFocus(scene_manager_t *mgr) {
    SceneManager_SetFocus(mgr, NULL);
}

// ========================================
// 鼠标捕获管理实现
// ========================================

// 捕获鼠标（组件将优先接收所有鼠标事件，即使鼠标移出窗口范围）
void SceneManager_CaptureMouse(scene_manager_t *mgr, ui_component_t *component) {
    if (!mgr) return;
    mgr->captured = component;
    // 启用 SDL 鼠标捕获，即使鼠标移出窗口也能接收事件
    SDL_CaptureMouse(SDL_TRUE);
}

// 获取当前捕获鼠标的组件
ui_component_t* SceneManager_GetCaptured(scene_manager_t *mgr) {
    return mgr ? mgr->captured : NULL;
}

// 释放鼠标捕获
void SceneManager_ReleaseMouse(scene_manager_t *mgr) {
    if (!mgr) return;
    mgr->captured = NULL;
    // 禁用 SDL 鼠标捕获
    SDL_CaptureMouse(SDL_FALSE);
}

// ========================================
// 默认资源API实现
// ========================================

void* SceneManager_GetDefaultCanvas(scene_manager_t *mgr) {
    return mgr ? mgr->default_canvas : NULL;
}

void* SceneManager_GetDefaultCanvasContext(scene_manager_t *mgr) {
    return mgr ? mgr->default_canvas_ctx : NULL;
}

void* SceneManager_GetDefaultLayoutContext(scene_manager_t *mgr) {
    return mgr ? mgr->default_lay_ctx : NULL;
}

ui_component_t* SceneManager_GetDefaultRoot(scene_manager_t *mgr) {
    return mgr ? mgr->default_root : NULL;
}


void Scene_UpdateUI(scene_t *scene, int msec) {
    if (!scene || !scene->root_component) return;
    
    // 递归更新所有组件
    ui_component_t **stack[128];
    int stack_size = 0;
    stack[stack_size++] = &scene->root_component;
    
    while (stack_size > 0) {
        ui_component_t **comp_ptr = stack[--stack_size];
        ui_component_t *comp = *comp_ptr;
        if (!comp) continue;
        
        // 调用组件的update方法（如果存在）
        if (comp->vtable && comp->vtable->update) {
            comp->vtable->update(comp, msec);
        }
        
        // 添加子组件到栈（用于容器组件）
        if (comp->children) {
            for (int i = 0; i < comp->child_count; i++) {
                if (stack_size < 128) {
                    stack[stack_size++] = &comp->children[i];
                }
            }
        }
    }
}

void Scene_LayoutUI(scene_t *scene, int msec) {
    if (!scene || !scene->root_component) return;
    
    // 使用lay库对使用children注册的组件布局
    UIComponent_Layout(scene->root_component);

    // 递归所有组件, 调用组件的layout方法（如果存在）
    ui_component_t **stack[128];
    int stack_size = 0;
    stack[stack_size++] = &scene->root_component;
    
    while (stack_size > 0) {
        ui_component_t **comp_ptr = stack[--stack_size];
        ui_component_t *comp = *comp_ptr;
        if (!comp) continue;
        
        // 调用组件的layout方法（如果存在）
        if (comp->vtable && comp->vtable->layout) {
            // lay_get_rect_xywh(comp->lay_ctx, comp->lay_item_id,&comp->x, &comp->y, &comp->width, &comp->height);
            comp->vtable->layout(comp, scene->root_component);
        }
        
        // 添加子组件到栈（用于容器组件）
        if (comp->children) {
            for (int i = 0; i < comp->child_count; i++) {
                if (stack_size < 128) {
                    stack[stack_size++] = &comp->children[i];
                }
            }
        }
    }
}

void Scene_RenderUI(scene_t *scene) {
    if (!scene || !scene->root_component) return;
    
    // 递归渲染所有组件
    ui_component_t **stack[128];
    int stack_size = 0;
    stack[stack_size++] = &scene->root_component;
    canvas2d_draw_debug_grid(scene->canvas_ctx, 0, 0, scene->root_component->width, 
        scene->root_component->height, 40, 30, 1);
    while (stack_size > 0) {
        ui_component_t **comp_ptr = stack[--stack_size];
        ui_component_t *comp = *comp_ptr;
        if (!comp) continue;
        
        // 只渲染可见的组件
        if (comp->flags & UI_FLAG_VISIBLE) {
            // 调用组件的render方法（如果存在）,否则使用默认的
            // 优先使用 vtable 中的渲染函数，如果存在的话
            lay_scalar x, y, w, h;
            lay_get_rect_xywh(scene->lay_ctx, comp->lay_item_id, &x,&y,&w,&h);

            lay_scalar l, t, r, b;
            lay_get_margins_ltrb(scene->lay_ctx, comp->lay_item_id, &l, &t, &r, &b);
            // comp->x = x+l;
            // comp->y = y+t;
            // comp->width = w;
            // comp->height = h;

            comp->x = x;
            comp->y = y;
            comp->width = w;
            comp->height = h;

            if (comp->vtable && comp->vtable->render) {
                comp->vtable->render(comp);
            } else {
                UIComponent_Render(comp);
                // 添加子组件到栈（用于容器组件）
                if (comp->children) {
                    for (int i = 0; i < comp->child_count; i++) {
                        if (stack_size < 128) {
                            stack[stack_size++] = &comp->children[i];
                        }
                    }
                }
            }
        }
    }
}
