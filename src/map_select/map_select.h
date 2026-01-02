#ifndef __MAP_SELECT_H__
#define __MAP_SELECT_H__

#include "../common/shared.h"

// ========================================
// 前向声明
// ========================================
typedef struct scene_t scene_t;
typedef struct scene_params_t scene_params_t;
typedef struct scene_transition_t scene_transition_t;

#include "../common/scene.h"  // 包含完整定义

// ========================================
// 地图信息结构
// ========================================
typedef struct {
    char filename[256];        // 地图文件名
    char name[128];            // 地图显示名称
    char author[128];           // 作者
    char description[512];      // 地图描述
    char recommended[32];       // 推荐玩家数
    int width;                 // 地图宽度
    int height;                // 地图高度
} map_info_t;

// ========================================
// 地图选择界面状态
// ========================================
typedef enum {
    MAP_SELECT_STATE_INIT,      // 初始化
    MAP_SELECT_STATE_LIST,      // 显示地图列表
    MAP_SELECT_STATE_LOADING,   // 加载地图
    MAP_SELECT_STATE_DONE       // 完成
} map_select_state_t;

// ========================================
// 原有API（保持向后兼容）
// ========================================
// 初始化地图选择界面（接收 scene 参数以使用默认资源）
int MapSelect_Init(scene_t *scene);

// 渲染地图选择界面
void MapSelect_Render(void);

// 处理输入事件
bool MapSelect_HandleInput(int key, bool down);

// 处理鼠标事件
bool MapSelect_HandleMouseEvent(void);

// 获取选中的地图（兼容旧代码）
const char* MapSelect_GetSelectedMap(void);
char* MapSelect_GetStartMap(void);

// 清理地图选择界面
void MapSelect_Shutdown(void);

// 从(listfile)加载地图列表
int MapSelect_LoadMapsFromListfile(void);

// 加载并保存地图信息到txt文件
bool MapSelect_LoadAndSaveMapInfo(const char *mapPath);

// 保存地图信息到txt文件
void MapSelect_SaveMapInfoToFile(const char *mapPath, LPCMAPINFO info);

// ========================================
// Scene 接口 - 新的API
// ========================================

// Scene 初始化
// 参数说明：
//   - "start_folder" (string, 可选): 启动时显示的文件夹路径
int MapSelectScene_Init(scene_t *scene, const scene_params_t *params);

// Scene 关闭
void MapSelectScene_Shutdown(scene_t *scene);

// Scene 更新
// 返回值：scene_transition_t* - 如果需要跳转场景，返回跳转请求
scene_transition_t* MapSelectScene_Update(scene_t *scene, int msec);

// Scene 渲染
void MapSelectScene_Render(scene_t *scene);

// Scene 输入处理
void MapSelectScene_OnInput(scene_t *scene, input_event_t *event);

// ========================================
// 获取场景实例
// ========================================
scene_t* MapSelectScene_GetInstance(void);

#endif // __MAP_SELECT_H__
