#include "cl_game_scene.h"
#include "client.h"
#include "../server/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 外部声明的全局变量
extern mouseEvent_t mouse;

// ========================================
// Game 场景数据结构
// ========================================
typedef struct {
    char current_map_path[MAX_PATHLEN];
    char start_folder[MAX_PATHLEN];
    bool paused;
} game_scene_data_t;

// ========================================
// Scene 接口实现
// ========================================

// Scene实例
static scene_t g_game_scene = {
    .name = "Game",
    .state = SCENE_STATE_UNINITIALIZED,
    .user_data = NULL,
    .launch_params = NULL,
    .manager = NULL,
    .init = GameScene_Init,
    .shutdown = GameScene_Shutdown,
    .update = GameScene_Update,
    .render = GameScene_Render,
    .on_input = GameScene_OnInput,
    .pause = GameScene_Pause,
    .resume = GameScene_Resume
};

// Scene 初始化
int GameScene_Init(scene_t *scene, const scene_params_t *params) {
    printf("GameScene: Initializing...\n");
    
    // 分配场景数据
    game_scene_data_t *data = (game_scene_data_t*)malloc(sizeof(game_scene_data_t));
    if (!data) {
        printf("GameScene: Failed to allocate scene data\n");
        return -1;
    }
    
    memset(data, 0, sizeof(game_scene_data_t));
    data->paused = false;
    scene->user_data = data;
    
    // 检查启动参数
    if (params) {
        char map_path[MAX_PATHLEN];
        char start_folder[MAX_PATHLEN];
        
        if (SceneParams_GetString(params, "map_path", map_path, sizeof(map_path))) {
            if (strlen(map_path) > 0) {
                strncpy(data->current_map_path, map_path, MAX_PATHLEN - 1);
                data->current_map_path[MAX_PATHLEN - 1] = '\0';
                
                printf("GameScene: Loading map: %s\n", data->current_map_path);
                
                // 加载地图
                SV_Map(data->current_map_path);
                
                // 发送 new 命令连接到服务器
                extern void CL_ConnectionlessPacket(void);
                CL_ConnectionlessPacket();
            } else {
                printf("GameScene: No map_path parameter provided\n");
                free(data);
                return -1;
            }
        } else {
            printf("GameScene: No map_path in params\n");
            free(data);
            return -1;
        }
        
        // 保存start_folder（用于返回mapselect时恢复位置）
        SceneParams_GetString(params, "start_folder", start_folder, sizeof(start_folder));
        strncpy(data->start_folder, start_folder, MAX_PATHLEN - 1);
        data->start_folder[MAX_PATHLEN - 1] = '\0';
    } else {
        printf("GameScene: No parameters provided\n");
        free(data);
        return -1;
    }
    
    printf("GameScene: Initialized successfully\n");
    return 0;
}

// Scene 关闭
void GameScene_Shutdown(scene_t *scene) {
    printf("GameScene: Shutting down...\n");
    
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (data) {
        free(data);
        scene->user_data = NULL;
    }
    
    printf("GameScene: Shutdown complete\n");
}

// Scene 更新
scene_transition_t* GameScene_Update(scene_t *scene, int msec) {
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (!data) return NULL;
    
    // 如果暂停了，不更新游戏逻辑
    if (data->paused) {
        return NULL;
    }
    
    // 更新服务器逻辑
    SV_Frame(msec);
    
    // 更新客户端逻辑（不包括输入，输入在on_input中处理）
    cl.time += msec;
    CL_ReadPackets();
    CL_SendCommand();
    CL_PrepRefresh();
    
    return NULL;
}

// Scene 渲染
void GameScene_Render(scene_t *scene) {
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (!data) return;
    
    // 渲染3D场景
    re.ResizeIfNeeded();
    V_RenderView();
    
    SCR_DrawOverlays();
}

// Scene 输入处理
scene_transition_t* GameScene_OnInput(scene_t *scene, input_event_t *event) {
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (!data) return NULL;
    
    switch (event->type) {
        case INPUT_EVENT_KEY_DOWN: {
            // 暂停状态下只处理暂停菜单的输入
            if (data->paused) {
                // 暂停菜单的输入由暂停场景处理，这里忽略
                break;
            }
            
            // 正常游戏状态下的输入
            switch (event->key.key) {
                case K_ESCAPE: {
                    // ESC键：打开暂停菜单（叠加场景）
                    printf("GameScene: Pausing game\n");
                    data->paused = true;
                    
                    // 暂时返回NULL，等待外部处理暂停菜单
                    // 或者可以在这里创建暂停场景的请求
                    break;
                }
                
                case K_F1: {
                    // F1: 返回地图选择界面
                    printf("GameScene: Returning to map select\n");
                    
                    // 创建返回参数（传递当前文件夹位置）
                    scene_params_t *params = SceneParams_Create();
                    if (params) {
                        if (strlen(data->start_folder) > 0) {
                            SceneParams_SetString(params, "start_folder", data->start_folder);
                        }
                        
                        // 切换回MapSelect场景
                        scene_transition_t *transition = SceneTransition_CreateByName(
                            TRANSITION_SWITCH,
                            "MapSelect",
                            params,
                            NULL
                        );
                        
                        return transition;
                    }
                    break;
                }
                
                default: {
                    // 其他键盘按键 - 转换为Key_Event
                    Key_Event(event->key.key, true, event->key.key);
                    break;
                }
            }
            break;
        }
        
        case INPUT_EVENT_KEY_UP: {
            // 暂停状态下忽略键盘释放
            if (data->paused) {
                break;
            }
            
            Key_Event(event->key.key, false, event->key.key);
            break;
        }
        
        case INPUT_EVENT_MOUSE_DOWN: {
            // 暂停状态下忽略鼠标输入
            if (data->paused) {
                break;
            }
            
            // 更新全局mouse状态
            mouse.origin.x = event->mouse.x;
            mouse.origin.y = event->mouse.y;
            mouse.button = event->mouse.button;
            
            switch (event->mouse.button) {
                case 1:
                    mouse.event = UI_LEFT_MOUSE_DOWN;
                    cl.selection.in_progress = true;
                    cl.selection.rect.x = mouse.origin.x;
                    cl.selection.rect.y = mouse.origin.y;
                    cl.selection.rect.w = 0;
                    cl.selection.rect.h = 0;
                    break;
                case 2:
                    mouse.event = UI_MIDDLE_MOUSE_DOWN;
                    break;
                case 3:
                    mouse.event = UI_RIGHT_MOUSE_DOWN;
                    break;
            }
            break;
        }
        
        case INPUT_EVENT_MOUSE_UP: {
            if (data->paused) {
                break;
            }
            
            mouse.origin.x = event->mouse.x;
            mouse.origin.y = event->mouse.y;
            mouse.button = 0;
            
            switch (event->mouse.button) {
                case 1: {
                    mouse.event = UI_LEFT_MOUSE_UP;
                    RECT const r = cl.selection.rect;
                    cl.selection.in_progress = false;
                    
                    // 判断是点击还是框选
                    if (fabs(r.w) + fabs(r.h) < 10) {
                        // 单个实体选择
                        DWORD entnum;
                        VECTOR3 point;
                        if (re.TraceEntity(&cl.viewDef, event->mouse.x, event->mouse.y, &entnum)) {
                            CL_SendNetworkCommand("select %d", entnum);
                        } else if (re.TraceLocation(&cl.viewDef, event->mouse.x, event->mouse.y, &point)) {
                            CL_SendNetworkCommand("point %d %d", (int)point.x, (int)point.y);
                        }
                    } else {
                        // 区域选择
                        DWORD selected[64] = { 0 };
                        DWORD num = re.EntitiesInRect(&cl.viewDef, &r, 64, selected);
                        if (num > 0) {
                            char buffer[1024] = { 0 };
                            strcpy(buffer, "select");
                            for (int i = 0; i < num; i++) {
                                sprintf(buffer + strlen(buffer), " %d", selected[i]);
                            }
                            CL_SendNetworkCommand("%s", buffer);
                        }
                    }
                    break;
                }
                case 2:
                    mouse.event = UI_MIDDLE_MOUSE_UP;
                    break;
                case 3: {
                    mouse.event = UI_RIGHT_MOUSE_UP;
                    // War3风格攻击/移动
                    DWORD entnum;
                    VECTOR3 point;
                    if (re.TraceEntity(&cl.viewDef, event->mouse.x, event->mouse.y, &entnum)) {
                        CL_SendNetworkCommand("attack %d", entnum);
                    } else if (re.TraceLocation(&cl.viewDef, event->mouse.x, event->mouse.y, &point)) {
                        VECTOR2 location = { (float)point.x, (float)point.y };
                        CL_SendNetworkCommand("move %f %f", location.x, location.y);
                    }
                    break;
                }
            }
            break;
        }
        
        case INPUT_EVENT_MOUSE_MOTION: {
            if (data->paused) {
                break;
            }
            
            mouse.origin.x = event->motion.x;
            mouse.origin.y = event->motion.y;
            break;
        }
        
        case INPUT_EVENT_MOUSE_WHEEL: {
            if (data->paused) {
                break;
            }
            
            // 处理鼠标滚轮（缩放等）
            if (event->wheel.delta > 0) {
                Key_Event(K_MWHEELUP, true, 0);
                Key_Event(K_MWHEELUP, false, 0);
            } else {
                Key_Event(K_MWHEELDOWN, true, 0);
                Key_Event(K_MWHEELDOWN, false, 0);
            }
            break;
        }
        
        default:
            break;
    }
    
    return NULL;
}

// Scene 暂停（从暂停菜单返回时调用）
void GameScene_Pause(scene_t *scene, const scene_params_t *result) {
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (!data) return;
    
    printf("GameScene: Paused (called from overlay scene)\n");
    data->paused = true;
    
    // 可以在这里处理暂停菜单返回的结果
    // 例如：是否要返回地图选择
    if (result) {
        bool return_to_mapselect = PARAMS_GET_BOOL(result, "return_to_mapselect", false);
        if (return_to_mapselect) {
            printf("GameScene: Pause menu requested return to map select\n");
            // 这个逻辑需要在Update中处理返回
        }
    }
}

// Scene 恢复（从暂停菜单关闭时调用）
void GameScene_Resume(scene_t *scene) {
    game_scene_data_t *data = (game_scene_data_t*)scene->user_data;
    if (!data) return;
    
    printf("GameScene: Resumed\n");
    data->paused = false;
}

// 获取场景实例
scene_t* GameScene_GetInstance(void) {
    return &g_game_scene;
}
