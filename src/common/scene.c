#include "scene.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

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

scene_manager_t* SceneManager_Create(void) {
    scene_manager_t *mgr = (scene_manager_t*)malloc(sizeof(scene_manager_t));
    if (!mgr) return NULL;
    
    memset(mgr, 0, sizeof(scene_manager_t));
    mgr->registered_count = 0;
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

void SceneManager_Update(scene_manager_t *mgr, int msec) {
    if (!mgr || !mgr->current_scene) return;
    
    
    if(mgr->pending_transition) {
        SceneManager_Transition(mgr, mgr->pending_transition);
        SceneTransition_Destroy(mgr->pending_transition);
        mgr->pending_transition = NULL;
        return;
    }
    // 调用场景的update函数，检查是否有跳转请求
    scene_transition_t *transition = SCENE_UPDATE(mgr->current_scene, msec);
    if (transition) {
        mgr->pending_transition = transition;
        SceneManager_Transition(mgr, mgr->pending_transition);
        SceneTransition_Destroy(mgr->pending_transition);
        mgr->pending_transition = NULL;
    }
}

void SceneManager_Render(scene_manager_t *mgr) {
    if (!mgr) return;
    
    // 从栈底到栈顶依次渲染（实现叠加效果）
    for (int i = 0; i < mgr->stack_size; i++) {
        if (mgr->stack[i]->state == SCENE_STATE_ACTIVE || 
            mgr->stack[i]->state == SCENE_STATE_PAUSED) {
            SCENE_RENDER(mgr->stack[i]);
        }
    }
}

void SceneManager_OnInput(scene_manager_t *mgr, input_event_t *event) {
    if (!mgr || !event) return;
    
    // 事件从栈顶向栈底分发，支持事件冒泡
    event->handled = false;
    
    for (int i = mgr->stack_size - 1; i >= 0; i--) {
        scene_t *scene = mgr->stack[i];
        if (scene->state == SCENE_STATE_ACTIVE && scene->on_input) {
            SCENE_ON_INPUT(scene, event);
            if (event->handled) {
                break;  // 事件已被处理，停止分发
            }
        }
    }
}
