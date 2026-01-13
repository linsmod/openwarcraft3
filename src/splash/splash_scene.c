#include "splash_scene.h"
#include "../client/client.h"
#include "../ui/ui_html_viewer.h"
#include "../ui/ui_container.h"
#include "common/scene.h"
#include "common/shared.h"
#include "ui/ui_component.h"
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

// ========================================
// 前置声明
// ========================================
static void SplashScene_OnClick(scene_t *scene, event_t *event);

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
    .pause = NULL,
    .resume = NULL,
    // 细化的事件处理函数
    .on_click = SplashScene_OnClick
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

    float width,height;
    UIComponent_GetSize(scene->root_component, &width, &height);
    
    canvas2d_context_t *canvas_ctx = scene->canvas_ctx;
    ui_html_viewer_t *viewer = UIHTMLViewer_Create(0.0f, 0.0f, width, height, canvas_ctx);
    if (!viewer) {
        printf("SplashScene: Failed to create HTML viewer\n");
        free(data);
        return -1;
    }
    
    // 设置HTML viewer不拦截点击事件，让事件能传播到场景
    UIHTMLViewer_SetElementClicked(viewer, NULL, NULL);
    
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

// 处理键盘按下事件
static void SplashScene_OnClick(scene_t *scene, event_t *event) {
    splash_scene_data_t *data = (splash_scene_data_t*)scene->user_data;
    if (!data || data->is_finished) return;
    
    // 如果不允许跳过，不处理输入
    if (!data->can_skip) {
        return;
    }
    
    printf("SplashScene: Key down, skipping splash\n");
    
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
    
    if (scene->manager) {
        SceneManager_SetTransition(scene->manager, transition);
    }
}

// 获取场景实例
scene_t* SplashScene_GetInstance(void) {
    return &g_splash_scene;
}

