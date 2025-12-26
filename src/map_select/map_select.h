#ifndef __MAP_SELECT_H__
#define __MAP_SELECT_H__

#include "../common/shared.h"

// 地图信息结构
typedef struct {
    char filename[256];        // 地图文件名
    char name[128];            // 地图显示名称
    char author[128];           // 作者
    char description[512];      // 地图描述
    char recommended[32];       // 推荐玩家数
    int width;                 // 地图宽度
    int height;                // 地图高度
} map_info_t;

// 地图选择界面状态
typedef enum {
    MAP_SELECT_STATE_INIT,      // 初始化
    MAP_SELECT_STATE_LIST,      // 显示地图列表
    MAP_SELECT_STATE_LOADING,   // 加载地图
    MAP_SELECT_STATE_DONE       // 完成
} map_select_state_t;

// 初始化地图选择界面
int MapSelect_Init(void);

// 更新地图选择界面
void MapSelect_Update(int msec);

// 渲染地图选择界面
void MapSelect_Render(void);

// 处理输入事件
bool MapSelect_HandleInput(int key, bool down);

// 处理鼠标事件
bool MapSelect_HandleMouseEvent(void);

// 获取选中的地图
const char* MapSelect_GetSelectedMap(void);

// 清理地图选择界面
void MapSelect_Shutdown(void);

// 从(listfile)加载地图列表
int MapSelect_LoadMapsFromListfile(void);

// 加载并保存地图信息到txt文件
bool MapSelect_LoadAndSaveMapInfo(const char *mapPath);

// 保存地图信息到txt文件
void MapSelect_SaveMapInfoToFile(const char *mapPath, LPCMAPINFO info);

#endif // __MAP_SELECT_H__
