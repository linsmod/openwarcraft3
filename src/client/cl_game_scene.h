#ifndef __CL_GAME_SCENE_H__
#define __CL_GAME_SCENE_H__

#include "../common/scene.h"
#include "../common/shared.h"

// Game 场景 API

// Scene 初始化
// 参数说明：
//   - "map_path" (string, 必需): 要加载的地图路径
//   - "start_folder" (string, 可选): 返回mapselect时的文件夹路径
int GameScene_Init(scene_t *scene, const scene_params_t *params);

// Scene 关闭
void GameScene_Shutdown(scene_t *scene);

// Scene 更新
// 返回值：scene_transition_t* - 如果需要跳转场景，返回跳转请求
scene_transition_t* GameScene_Update(scene_t *scene, int msec);

// Scene 渲染
void GameScene_Render(scene_t *scene);

// Scene 输入处理
void GameScene_OnInput(scene_t *scene, input_event_t *event);

// Scene 暂停（例如按ESC打开暂停菜单时）
void GameScene_Pause(scene_t *scene, const scene_params_t *result);

// Scene 恢复（从暂停菜单返回）
void GameScene_Resume(scene_t *scene);

// 获取场景实例
scene_t* GameScene_GetInstance(void);

#endif // __CL_GAME_SCENE_H__
