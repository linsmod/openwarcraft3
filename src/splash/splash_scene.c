#include "splash_scene.h"
#include "../client/client.h"
#include "../ui/ui_html_viewer.h"
#include "../ui/ui_container.h"
#include "common/scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    .render = NULL,
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
    data->display_duration = 5000.0f;  // 显示5秒
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
    
    // 创建HTML Viewer组件，填满整个场景
    float scene_width = scene->root_component->width;
    float scene_height = scene->root_component->height;
    
    canvas2d_context_t *canvas_ctx = scene->canvas_ctx;
    ui_html_viewer_t *viewer = UIHTMLViewer_Create(0.0f, 0.0f, scene_width, scene_height, canvas_ctx);
    if (!viewer) {
        printf("SplashScene: Failed to create HTML viewer\n");
        free(data);
        return -1;
    }
    
    // 加载HTML文件
    if (UIHTMLViewer_LoadFromFile(viewer, "../html_tests/splash.html") != 0) {
        printf("SplashScene: Failed to load splash.html\n");
        UIHTMLViewer_Destroy(viewer);
        free(data);
        return -1;
    }
    
    // 将HTML Viewer添加到根容器
    UIContainer_AddChild((ui_container_t *)scene->root_component, (ui_component_t *)viewer);
    
    // 保存viewer和数据
    scene->user_data = data;
    
    printf("SplashScene: Initialized successfully (HTML viewer: %p)\n", viewer);
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
    
    // 注意：viewer组件会通过Scene_RenderUI的清理自动销毁
    // 因为它是通过UIContainer_AddChild添加到根容器的
    
    printf("SplashScene: Shutdown complete\n");
}

// Scene 更新
scene_transition_t* SplashScene_Update(scene_t *scene, int msec) {
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (!data || data->is_finished) return NULL;
    
    // 注意：HTML更新由场景管线的Scene_UpdateUI自动处理
    // 不需要在这里手动调用 html_update_and_layout
    
    // 累加时间
    data->elapsed_time += (float)msec;
    
    // 检查是否需要自动跳转（如果设置了超时）
    // 这里保持原有的调试模式：不自动切换，只等待用户点击
    
    return NULL;
}
void splash_on_mouse_down(scene_t* scene, void *user_data) {
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
            
        }
        
        default:
            break;
    }
}

// 获取场景实例
scene_t* SplashScene_GetInstance(void) {
    return &g_splash_scene;
}

