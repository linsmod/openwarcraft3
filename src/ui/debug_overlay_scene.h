#ifndef __DEBUG_OVERLAY_SCENE_H__
#define __DEBUG_OVERLAY_SCENE_H__

#include "../common/scene.h"

// 创建调试覆盖层场景
scene_t* DebugOverlayScene_Create(void);

// 初始化调试覆盖层场景
int DebugOverlayScene_Init(scene_t *scene, const scene_params_t *params);

// 更新调试覆盖层场景
scene_transition_t* DebugOverlayScene_Update(scene_t *scene, int msec);

// 渲染调试覆盖层场景
void DebugOverlayScene_Render(scene_t *scene);

#endif // __DEBUG_OVERLAY_SCENE_H__
