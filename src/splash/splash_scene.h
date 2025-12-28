#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "../common/scene.h"
#include "../common/shared.h"

// Splash 场景 API

// Scene 初始化
int SplashScene_Init(scene_t *scene, const scene_params_t *params);

// Scene 关闭
void SplashScene_Shutdown(scene_t *scene);

// Scene 更新
// 返回值：scene_transition_t* - 如果需要跳转场景，返回跳转请求
scene_transition_t* SplashScene_Update(scene_t *scene, int msec);

// Scene 渲染
void SplashScene_Render(scene_t *scene);

// Scene 输入处理
// 返回值：scene_transition_t* - 如果需要跳转场景，返回跳转请求
scene_transition_t* SplashScene_OnInput(scene_t *scene, input_event_t *event);

// 获取场景实例
scene_t* SplashScene_GetInstance(void);

#endif // __SPLASH_SCENE_H__
