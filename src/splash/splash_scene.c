#include "splash_scene.h"
#include "../client/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 外部HTML渲染函数
extern void html_update_and_layout(float delta_time);
extern int html_destroy();

// ========================================
// Splash 场景数据结构
// ========================================
typedef struct {
    float elapsed_time;      // 已经过的时间（毫秒）
    float display_duration;  // 显示持续时间（毫秒），默认3秒
    float fade_in_duration;  // 淡入时间（毫秒）
    float fade_out_duration; // 淡出时间（毫秒）
    bool can_skip;           // 是否允许跳过
    bool fading_out;         // 是否正在淡出
    bool is_finished;        // 是否已完成
} splash_scene_data_t;

// ========================================
// Scene 接口实现
// ========================================

// Scene实例
static scene_t g_splash_scene = {
    .name = "Splash",
    .state = SCENE_STATE_UNINITIALIZED,
    .user_data = NULL,
    .launch_params = NULL,
    .manager = NULL,
    .init = SplashScene_Init,
    .shutdown = SplashScene_Shutdown,
    .update = SplashScene_Update,
    .render = SplashScene_Render,
    .on_input = SplashScene_OnInput,
    .pause = NULL,
    .resume = NULL
};

// Scene 初始化
int SplashScene_Init(scene_t *scene, const scene_params_t *params) {
    printf("SplashScene: Initializing...\n");
    
    // 分配场景数据
    splash_scene_data_t *data = (splash_scene_data_t*)malloc(sizeof(splash_scene_data_t));
    if (!data) {
        printf("SplashScene: Failed to allocate scene data\n");
        return -1;
    }
    
    memset(data, 0, sizeof(splash_scene_data_t));
    
    // 设置默认参数
    data->elapsed_time = 0.0f;
    data->display_duration = 5000.0f;  // 
    data->fade_in_duration = 500.0f;   // 淡入0.5秒
    data->fade_out_duration = 500.0f;  // 淡出0.5秒
    data->can_skip = true;             // 允许跳过
    data->fading_out = false;
    data->is_finished = false;
    
    // 从参数读取配置
    if (params) {
        int duration = PARAMS_GET_INT(params, "duration", 5000); 
        int fade_in = PARAMS_GET_INT(params, "fade_in", 500);
        int fade_out = PARAMS_GET_INT(params, "fade_out", 500);
        bool skip = PARAMS_GET_BOOL(params, "can_skip", true);
        
        data->display_duration = (float)duration;
        data->fade_in_duration = (float)fade_in;
        data->fade_out_duration = (float)fade_out;
        data->can_skip = skip;
        
        printf("SplashScene: Configured - duration: %dms, fade_in: %dms, fade_out: %dms, can_skip: %s\n",
               duration, fade_in, fade_out, skip ? "true" : "false");
    }
    
    scene->user_data = data;
    
    printf("SplashScene: Initialized successfully\n");
    return 0;
}

// Scene 关闭
void SplashScene_Shutdown(scene_t *scene) {
    printf("SplashScene: Shutting down...\n");
    
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (data) {
        free(data);
        scene->user_data = NULL;
    }
    
    printf("SplashScene: Shutdown complete\n");
}

// Scene 更新
scene_transition_t* SplashScene_Update(scene_t *scene, int msec) {
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (!data || data->is_finished) return NULL;
    
    // 更新HTML渲染
    float delta_time = (float)msec / 1000.0f;  // 毫秒转秒
    html_update_and_layout(delta_time);
    
    // 累加时间（仅用于调试信息）
    data->elapsed_time += (float)msec;
    
    // 调试模式：不自动切换，只等待用户点击
    // 移除了自动切换的逻辑，splash屏幕会一直显示直到用户点击
    
    return NULL;
}

// Scene 渲染
void SplashScene_Render(scene_t *scene) {
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (!data) return;
    
    // 计算透明度（用于淡入淡出效果）
    float alpha = 1.0f;
    
    // 淡入阶段
    if (data->elapsed_time < data->fade_in_duration) {
        alpha = data->elapsed_time / data->fade_in_duration;
    }
    // 淡出阶段
    else if (data->elapsed_time >= (data->fade_in_duration + data->display_duration)) {
        float fade_out_elapsed = data->elapsed_time - (data->fade_in_duration + data->display_duration);
        alpha = 1.0f - (fade_out_elapsed / data->fade_out_duration);
        if (alpha < 0.0f) alpha = 0.0f;
    }
    html_render();
}

// Scene 输入处理
void SplashScene_OnInput(scene_t *scene, input_event_t *event) {
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (!data || data->is_finished) return;
    
    // 如果不允许跳过，不处理输入
    if (!data->can_skip) {
        return;
    }
    
    switch (event->type) {
        case INPUT_EVENT_MOUSE_DOWN: {
            // 任意键或鼠标点击跳过
            printf("SplashScene: Skipped by user input\n");
            data->is_finished = true;
            
            // 创建跳转到下一个场景的请求
            scene_params_t *params = NULL;
            const char* target_scene_name = "MapSelect";
            
            if (scene->launch_params) {
                char map_path[MAX_PATHLEN];
                if (SceneParams_GetString(scene->launch_params, "map_path", map_path, sizeof(map_path)) && strlen(map_path) > 0) {
                    params = SceneParams_Create();
                    SceneParams_SetString(params, "map_path", map_path);
                    SceneParams_SetString(params, "start_folder", "");
                    target_scene_name = "Game";
                }
            }
            
            scene_transition_t *transition = SceneTransition_CreateByName(
                TRANSITION_SWITCH,
                target_scene_name,
                params,
                NULL
            );
            
            scene->manager->pending_transition = transition;
        }
        
        default:
            break;
    }
}

// 获取场景实例
scene_t* SplashScene_GetInstance(void) {
    return &g_splash_scene;
}
