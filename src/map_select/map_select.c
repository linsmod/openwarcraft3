#include "map_select.h"
#include "ui_list.h"
#include "ui_button.h"
#include "ui_text.h"
#include "ui_container.h"
#include "ui_event_dispatcher.h"
#include "../canvas2d/canvas2d.h"
#include "../client/client.h"
#include "../client/cl_game_scene.h"
#include "../common/common.h"
#include "../common/scene.h"
#include "../common/cmodel.h"
#include "../common/mapinfo.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// 最大地图数量
#define MAX_MAPS 500

// 最大路径长度（使用shared.h中的MAX_PATHLEN）

// 文件浏览器项类型
typedef enum {
    ITEM_TYPE_FOLDER,
    ITEM_TYPE_MAP_W3M,
    ITEM_TYPE_MAP_W3X
} item_type_t;

// 文件浏览器项
typedef struct {
    char name[256];            // 显示名称
    char full_path[MAX_PATHLEN]; // 完整路径
    item_type_t type;          // 类型
} browser_item_t;

// 全局状态
static map_info_t g_maps[MAX_MAPS];
static int g_map_count = 0;
static browser_item_t g_all_items[MAX_MAPS];  // 保存所有文件的原始列表
static int g_filtered_indices[MAX_MAPS];  // 过滤后的项在 g_all_items 中的索引
static int g_all_count = 0;
static int g_filtered_count = 0;
static map_select_state_t g_state = MAP_SELECT_STATE_INIT;
static char *g_start_map_path = NULL;

// 当前浏览的路径
static char g_current_path[MAX_PATHLEN] = "";
static bool g_at_root = true;

// 当前预览的地图路径（用于检测选中变化）
static char g_current_preview_map[MAX_PATHLEN] = "";

// Canvas2D 画布
static canvas2d_t *g_canvas = NULL;
static canvas2d_context_t *g_ctx = NULL;

// 事件分发器
static ui_event_dispatcher_t g_event_dispatcher;

// 根容器组件（场景级别的根容器）
static ui_component_t *g_root_container = NULL;

// UI 组件（使用新的基类系统）
static ui_component_t *g_ui_list = NULL;
static ui_component_t *g_start_button = NULL;
static ui_component_t *g_title_text = NULL;
static ui_component_t *g_path_text = NULL;
static ui_component_t *g_hint_text1 = NULL;
static ui_component_t *g_hint_text2 = NULL;
static ui_component_t *g_preview_container = NULL;
static ui_component_t *g_preview_title_text = NULL;      // "Map Preview"
static ui_component_t *g_preview_filename_text = NULL;     // 地图文件名
static ui_component_t *g_preview_name_text = NULL;        // 地图名称
static ui_component_t *g_preview_author_text = NULL;       // 作者
static ui_component_t *g_preview_players_text = NULL;     // 推荐玩家数
static ui_component_t *g_preview_type_text = NULL;        // 文件类型
static ui_component_t *g_preview_path_text = NULL;       // 完整路径

// ========================================
// 辅助函数
// ========================================

// 开始游戏 - 公共函数
static void StartGame(const char *map_path, const char *source) {
    printf("Starting game (%s): %s\n", source, map_path);
    g_start_map_path = strdup(map_path);
    g_state = MAP_SELECT_STATE_DONE;
}

// START GAME 按钮点击回调
static bool OnStartGameClick(ui_component_t *component, ui_event_t *event, void *user_data) {
    (void)component;
    (void)event;
    (void)user_data;
    
    int selected = UIList_GetSelected((ui_list_t *)g_ui_list);
    if (selected >= 0) {
        void *item_data = UIList_GetSelectedUserData((ui_list_t *)g_ui_list);
        if (item_data) {
            int all_index = (int)(intptr_t)item_data;
            if (all_index >= 0 && all_index < g_all_count) {
                browser_item_t *item = &g_all_items[all_index];
                if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
                    StartGame(item->full_path, "button click");
                }
            }
        }
    }
    return true;
}

// 自定义列表项内容绘制函数（item级别，只绘制内容区域）
static void DrawBrowserItemCustom(
    ui_component_t *item_component,
    const char *text,
    void *user_data,
    float x,      // 内容区域x坐标（已去除边框和内边距）
    float y,      // 内容区域y坐标
    float width,  // 内容区域宽度
    float height, // 内容区域高度
    bool selected,
    bool hovered
) {
    (void)item_component;
    (void)selected;
    (void)hovered;

    // 图标/类型指示
    char icon[4] = "[F]";
    if (user_data) {
        int all_index = (int)(intptr_t)user_data;
        // -1 表示 ".." 返回上级目录
        if (all_index >= 0 && all_index < g_all_count) {
            browser_item_t *item = &g_all_items[all_index];
            if (item->type == ITEM_TYPE_MAP_W3M) {
                strcpy(icon, "[3]");
            } else if (item->type == ITEM_TYPE_MAP_W3X) {
                strcpy(icon, "[X]");
            }
        }
    }

    // 绘制图标（使用较小字体）
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 255, 255, 255});
    canvas2d_set_font_size(g_ctx, 12.0f);
    float icon_text_height = 12.0f * 1.2f;
    float icon_y = y + (height - icon_text_height) / 2;
    canvas2d_fill_text(g_ctx, icon, x + 5, icon_y);

    // 绘制文件名（使用回调传入的text参数）
    canvas2d_set_font_size(g_ctx, 14.0f);
    float text_text_height = 14.0f * 1.2f;
    float text_y = y + (height - text_text_height) / 2;
    canvas2d_fill_text(g_ctx, text, x + 35, text_y);
}

// 检查文件扩展名是否为地图文件
static bool IsMapFile(const char *filename) {
    size_t len = strlen(filename);
    if (len < 5) return false;
    
    const char *ext = filename + len - 4;
    if (strcmp(ext, ".w3m") == 0) return true;
    if (strcmp(ext, ".w3x") == 0) return true;
    return false;
}

// 获取文件类型
static item_type_t GetFileType(const char *filename) {
    size_t len = strlen(filename);
    if (len < 5) return ITEM_TYPE_FOLDER;
    
    const char *ext = filename + len - 4;
    if (strcmp(ext, ".w3m") == 0) return ITEM_TYPE_MAP_W3M;
    if (strcmp(ext, ".w3x") == 0) return ITEM_TYPE_MAP_W3X;
    return ITEM_TYPE_FOLDER;
}

// 获取文件夹/文件名称（不含完整路径）
static const char* GetDisplayName(const char *path) {
    const char *last_sep = strrchr(path, '\\');
    if (last_sep) {
        return last_sep + 1;
    }
    return path;
}

// 从(listfile)加载地图列表
int MapSelect_LoadMapsFromListfile(void) {
    printf("Loading maps from (listfile)...\n");
    
    // 读取(listfile)
    LPSTR listfile_content = FS_ReadFileIntoString("(listfile)");
    if (!listfile_content) {
        printf("Failed to read (listfile)\n");
        return 0;
    }
    
    // 保存(listfile)内容到文件，方便检查
    FILE *listfile_out = fopen("listfile_dump.txt", "w");
    if (listfile_out) {
        fprintf(listfile_out, "%s", listfile_content);
        fclose(listfile_out);
        printf("Saved (listfile) to listfile_dump.txt\n");
    } else {
        printf("Warning: Failed to save (listfile) to file\n");
    }
    
    // 清空浏览器项
    g_all_count = 0;
    
    // 解析(listfile)内容
    char *line_start = listfile_content;
    char *line_end;
    
    while ((line_end = strchr(line_start, '\n')) != NULL && g_all_count < MAX_MAPS) {
        // 计算行长度
        size_t line_len = line_end - line_start;
        
        // 移除换行符
        char line[MAX_PATHLEN];
        strncpy(line, line_start, line_len < MAX_PATHLEN - 1 ? line_len : MAX_PATHLEN - 1);
        line[line_len < MAX_PATHLEN - 1 ? line_len : MAX_PATHLEN - 1] = '\0';
        
        // 移除回车符（Windows风格）
        char *cr = strrchr(line, '\r');
        if (cr) *cr = '\0';
        
        // 跳过空行和特殊文件
        if (strlen(line) == 0) {
            line_start = line_end + 1;
            continue;
        }
        
        // 保留所有地图文件
        if (IsMapFile(line)) {
            // 保存到原始列表
            browser_item_t *all_item = &g_all_items[g_all_count];
            strncpy(all_item->full_path, line, MAX_PATHLEN - 1);
            all_item->full_path[MAX_PATHLEN - 1] = '\0';
            
            const char *display_name = GetDisplayName(line);
            strncpy(all_item->name, display_name, 255);
            all_item->name[255] = '\0';
            
            all_item->type = GetFileType(line);
            
            g_all_count++;
            
        }
        
        line_start = line_end + 1;
    }
    
    MemFree(listfile_content);
    
    printf("Loaded %d items from (listfile)\n", g_all_count);
    return g_all_count;
}

// 过滤显示当前路径下的文件
static void FilterCurrentPath(void) {
    g_filtered_count = 0;
    
    // 清空UI列表
    UIList_ClearItems((ui_list_t *)g_ui_list);
    
    // 添加 ".." 目录（如果不是在根目录）- 使用特殊索引 -1
    if (!g_at_root && g_filtered_count < MAX_MAPS) {
        g_filtered_indices[g_filtered_count++] = -1;  // -1 表示 ".." 返回上级目录
    }
    
    for (int i = 0; i < g_all_count && g_filtered_count < MAX_MAPS; i++) {
        const char *item_path = g_all_items[i].full_path;
        
        if (g_at_root) {
            // 根目录：显示顶级文件夹和根目录下的地图文件
            const char *first_sep = strchr(item_path, '\\');
            
            if (!first_sep) {
                // 根目录下的文件
                if (IsMapFile(item_path)) {
                    g_filtered_indices[g_filtered_count++] = i;
                }
            } else {
                // 顶级文件夹
                char folder_name[256];
                strncpy(folder_name, item_path, first_sep - item_path);
                folder_name[first_sep - item_path] = '\0';
                
                bool exists = false;
                for (int j = 0; j < g_filtered_count; j++) {
                    int idx = g_filtered_indices[j];
                    if (g_all_items[idx].type == ITEM_TYPE_FOLDER &&
                        strcmp(g_all_items[idx].name, folder_name) == 0) {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists) {
                    g_filtered_indices[g_filtered_count++] = i;
                }
            }
        } else {
            // 子目录：显示该目录下的子文件夹和地图文件
            size_t path_len = strlen(g_current_path);
            if (strncmp(item_path, g_current_path, path_len) == 0) {
                const char *remaining = item_path + path_len;
                
                // 跳过路径后的反斜杠
                if (remaining[0] == '\\') remaining++;
                
                const char *next_sep = strchr(remaining, '\\');
                
                if (next_sep) {
                    // 子文件夹
                    char folder_name[256];
                    strncpy(folder_name, remaining, next_sep - remaining);
                    folder_name[next_sep - remaining] = '\0';
                    
                    bool exists = false;
                    for (int j = 0; j < g_filtered_count; j++) {
                        int idx = g_filtered_indices[j];
                        if (g_all_items[idx].type == ITEM_TYPE_FOLDER &&
                            strcmp(g_all_items[idx].name, folder_name) == 0) {
                            exists = true;
                            break;
                        }
                    }
                    
                    if (!exists) {
                        g_filtered_indices[g_filtered_count++] = i;
                    }
                } else if (IsMapFile(item_path)) {
                    // 地图文件
                    g_filtered_indices[g_filtered_count++] = i;
                }
            }
        }
    }
    
    // 排序：文件夹在前，文件在后
    for (int i = 0; i < g_filtered_count - 1; i++) {
        for (int j = i + 1; j < g_filtered_count; j++) {
            int idx_i = g_filtered_indices[i];
            int idx_j = g_filtered_indices[j];
            
            // 获取类型，-1 表示 ".." 返回上级目录，视为文件夹
            item_type_t type_i = (idx_i == -1) ? ITEM_TYPE_FOLDER : g_all_items[idx_i].type;
            item_type_t type_j = (idx_j == -1) ? ITEM_TYPE_FOLDER : g_all_items[idx_j].type;
            
            // 获取名称
            const char *name_i = (idx_i == -1) ? ".." : g_all_items[idx_i].name;
            const char *name_j = (idx_j == -1) ? ".." : g_all_items[idx_j].name;
            
            if (type_i > type_j || (type_i == type_j && strcmp(name_i, name_j) > 0)) {
                int temp = g_filtered_indices[i];
                g_filtered_indices[i] = g_filtered_indices[j];
                g_filtered_indices[j] = temp;
            }
        }
    }
    
    // 将过滤后的项添加到 UI 列表（存储索引而不是指针）
    for (int i = 0; i < g_filtered_count; i++) {
        int idx = g_filtered_indices[i];
        const char *name = (idx == -1) ? ".." : g_all_items[idx].name;
        int item_index = UIList_AddItem((ui_list_t *)g_ui_list, name, (void*)(intptr_t)idx);
        
        // 设置每个item的内容绘制回调
        if (item_index >= 0) {
            ui_list_item_t *item = UIList_GetItem((ui_list_t *)g_ui_list, item_index);
            if (item) {
                UIListItem_SetDrawCallback(item, DrawBrowserItemCustom, (void*)(intptr_t)idx);
            }
        }
    }
    
    printf("Filtered %d items, UI list has %d items\n", g_filtered_count, UIList_GetItemCount((ui_list_t *)g_ui_list));
}

// 初始化地图选择界面
int MapSelect_Init(void) {
    printf("Initializing Map Selection Screen...\n");
    
    // 创建 Canvas
    g_canvas = canvas2d_create(1024, 768);
    if (!g_canvas) {
        printf("Failed to create canvas for map selection\n");
        return -1;
    }
    
    g_ctx = canvas2d_get_context(g_canvas);
    
    // 创建根容器（场景级别的根组件）
    g_root_container = (ui_component_t *)UIContainer_Create(
        0.0f, 0.0f, 1024.0f, 768.0f,
        MAKE(COLOR32, 0, 0, 0, 0),  // 透明背景
        MAKE(COLOR32, 0, 0, 0, 0),
        g_ctx
    );
    if (!g_root_container) {
        printf("Failed to create root container\n");
        return -1;
    }
    
    // 初始化事件分发器，设置根组件
    if (UIEventDispatcher_Init(&g_event_dispatcher, g_root_container, g_ctx) != 0) {
        printf("Failed to initialize event dispatcher\n");
        return -1;
    }
    
    // 创建 UI 列表
    g_ui_list = (ui_component_t *)UIList_Create(30.0f, 80.0f, 380.0f, 580.0f, 50.0f, 14.0f, g_ctx);
    if (!g_ui_list) {
        printf("Failed to create UI list\n");
        return -1;
    }
    
    // 将列表设置为焦点组件（这样键盘事件才能被它接收）
    UIEventDispatcher_SetFocus(&g_event_dispatcher, g_ui_list);
    
    // 将列表添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_ui_list);
    
    // 创建 START GAME 按钮
    ui_button_config_t button_config = UIButton_GetDefaultConfig();
    strncpy(button_config.text, "START GAME [ENTER]", 127);
    button_config.bg_colors.normal = (COLOR32){0, 150, 0, 255};
    button_config.bg_colors.hover = (COLOR32){0, 180, 0, 255};
    button_config.bg_colors.active = (COLOR32){0, 120, 0, 255};
    button_config.bg_colors.disabled = (COLOR32){100, 100, 100, 255};
    button_config.border_colors.normal = (COLOR32){0, 255, 0, 255};
    button_config.border_colors.hover = (COLOR32){50, 255, 50, 255};
    button_config.border_colors.active = (COLOR32){0, 200, 0, 255};
    button_config.border_colors.disabled = (COLOR32){150, 150, 150, 255};
    button_config.text_colors.normal = (COLOR32){255, 255, 255, 255};
    button_config.text_colors.hover = (COLOR32){255, 255, 255, 255};
    button_config.text_colors.active = (COLOR32){255, 255, 255, 255};
    button_config.text_colors.disabled = (COLOR32){180, 180, 180, 255};
    button_config.font_size = 20.0f;
    button_config.border_width = 2.0f;
    
    g_start_button = (ui_component_t *)UIButton_CreateWithConfig(440.0f, 500.0f, 280.0f, 50.0f, &button_config, g_ctx);
    if (!g_start_button) {
        printf("Failed to create UI button\n");
        return -1;
    }
    
    // 添加按钮点击事件
    UIButton_AddOnClick((ui_button_t *)g_start_button, OnStartGameClick, NULL);
    
    // 将按钮添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_start_button);
    
    // 创建标题文本
    g_title_text = (ui_component_t *)UIText_Create(450.0f, 20.0f, "SELECT MAP",
                                                   (COLOR32){255, 215, 0, 255}, 30.0f,
                                                   UI_TEXT_ALIGN_CENTER, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_title_text) {
        printf("Failed to create title text\n");
        return -1;
    }
    
    // 将标题文本添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_title_text);
    
    // 创建路径文本
    g_path_text = (ui_component_t *)UIText_Create(30.0f, 50.0f, "Root",
                                                  (COLOR32){200, 200, 200, 255}, 18.0f,
                                                  UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_path_text) {
        printf("Failed to create path text\n");
        return -1;
    }
    
    // 将路径文本添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_path_text);
    
    // 创建提示文本1
    g_hint_text1 = (ui_component_t *)UIText_Create(30.0f, 710.0f, "UP/DOWN to navigate, ENTER to select",
                                                   (COLOR32){200, 200, 200, 255}, 16.0f,
                                                   UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_hint_text1) {
        printf("Failed to create hint text 1\n");
        return -1;
    }
    
    // 将提示文本1添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_hint_text1);
    
    // 创建提示文本2
    g_hint_text2 = (ui_component_t *)UIText_Create(900.0f, 710.0f, "ESC to quit",
                                                   (COLOR32){200, 200, 200, 255}, 16.0f,
                                                   UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_hint_text2) {
        printf("Failed to create hint text 2\n");
        return -1;
    }
    
    // 将提示文本2添加到根容器
    UIContainer_AddChild((ui_container_t *)g_root_container, g_hint_text2);
    
    // 创建地图预览容器
    g_preview_container = (ui_component_t *)UIContainer_Create(440.0f, 80.0f, 524.0f, 380.0f,
                                                               (COLOR32){40, 40, 50, 230},
                                                               (COLOR32){255, 215, 0, 255}, g_ctx);
    if (!g_preview_container) {
        printf("Failed to create preview container\n");
        return -1;
    }
    
    // 创建预览文本组件（标题 "Map Preview"）
    g_preview_title_text = (ui_component_t *)UIText_Create(460.0f, 105.0f, "Map Preview",
                                                          (COLOR32){255, 215, 0, 255}, 18.0f,
                                                          UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_title_text) {
        printf("Failed to create preview title text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_title_text);
    
    // 创建预览文本组件（文件名）
    g_preview_filename_text = (ui_component_t *)UIText_Create(460.0f, 135.0f, "",
                                                              (COLOR32){200, 200, 200, 255}, 18.0f,
                                                              UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_filename_text) {
        printf("Failed to create preview filename text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_filename_text);
    
    // 创建预览文本组件（地图名称）
    g_preview_name_text = (ui_component_t *)UIText_Create(460.0f, 160.0f, "",
                                                         (COLOR32){180, 180, 180, 255}, 18.0f,
                                                         UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_name_text) {
        printf("Failed to create preview name text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_name_text);
    
    // 创建预览文本组件（作者）
    g_preview_author_text = (ui_component_t *)UIText_Create(460.0f, 185.0f, "",
                                                         (COLOR32){160, 160, 160, 255}, 18.0f,
                                                         UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_author_text) {
        printf("Failed to create preview author text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_author_text);
    
    // 创建预览文本组件（推荐玩家数）
    g_preview_players_text = (ui_component_t *)UIText_Create(460.0f, 210.0f, "",
                                                           (COLOR32){140, 140, 140, 255}, 18.0f,
                                                           UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_players_text) {
        printf("Failed to create preview players text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_players_text);
    
    // 创建预览文本组件（文件类型）
    g_preview_type_text = (ui_component_t *)UIText_Create(460.0f, 235.0f, "",
                                                        (COLOR32){150, 150, 150, 255}, 18.0f,
                                                        UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_type_text) {
        printf("Failed to create preview type text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_type_text);
    
    // 创建预览文本组件（完整路径）
    g_preview_path_text = (ui_component_t *)UIText_Create(460.0f, 260.0f, "",
                                                      (COLOR32){100, 100, 100, 255}, 16.0f,
                                                      UI_TEXT_ALIGN_LEFT, UI_TEXT_VALIGN_TOP, g_ctx);
    if (!g_preview_path_text) {
        printf("Failed to create preview path text\n");
        return -1;
    }
    UIContainer_AddChild((ui_container_t *)g_preview_container, g_preview_path_text);
    
    // 从(listfile)加载地图列表
    g_map_count = 0;
    
    int loaded = MapSelect_LoadMapsFromListfile();
    if (loaded == 0) {
        printf("No maps found in (listfile), using test data\n");
        // 添加测试数据
        strcpy(g_all_items[0].name, "Human01.w3m");
        strcpy(g_all_items[0].full_path, "Maps\\Campaign\\Human01.w3m");
        g_all_items[0].type = ITEM_TYPE_MAP_W3M;
        g_all_count++;
        
        strcpy(g_all_items[1].name, "Human02.w3m");
        strcpy(g_all_items[1].full_path, "Maps\\Campaign\\Human02.w3m");
        g_all_items[1].type = ITEM_TYPE_MAP_W3M;
        g_all_count++;
    }
    
    // 过滤到根目录
    g_at_root = true;
    g_current_path[0] = '\0';
    FilterCurrentPath();
    
    g_state = MAP_SELECT_STATE_LIST;
    
    printf("Map Selection Screen initialized\n");
    return 0;
}



// 更新地图预览信息
static void UpdateMapPreview(void) {
    int selected = UIList_GetSelected((ui_list_t *)g_ui_list);
    
    // 如果没有选中项，隐藏预览容器
    if (selected < 0) {
        UIComponent_SetVisible(g_preview_container, false);
        return;
    }
    
    void *user_data = UIList_GetSelectedUserData((ui_list_t *)g_ui_list);
    if (!user_data) {
        UIComponent_SetVisible(g_preview_container, false);
        return;
    }
    
    int all_index = (int)(intptr_t)user_data;
    // -1 表示 ".." 返回上级目录，不显示预览
    if (all_index < 0 || all_index >= g_all_count) {
        UIComponent_SetVisible(g_preview_container, false);
        return;
    }
    
    browser_item_t *item = &g_all_items[all_index];
    
    // 只为地图文件显示预览
    if (item->type != ITEM_TYPE_MAP_W3M && item->type != ITEM_TYPE_MAP_W3X) {
        UIComponent_SetVisible(g_preview_container, false);
        return;
    }
    
    // 显示预览容器
    UIComponent_SetVisible(g_preview_container, true);
    
    // 获取地图信息
    LPCMAPINFO info = CM_GetMapInfo();
    
    // 更新文件名
    UIText_SetText((ui_text_t *)g_preview_filename_text, item->name);
    UIComponent_SetVisible(g_preview_filename_text, true);
    
    // 更新地图名称（如果可用）
    if (info && info->mapName) {
        char name_text[128];
        snprintf(name_text, sizeof(name_text), "Name: %s", info->mapName);
        UIText_SetText((ui_text_t *)g_preview_name_text, name_text);
        UIComponent_SetVisible(g_preview_name_text, true);
    } else {
        UIComponent_SetVisible(g_preview_name_text, false);
    }
    
    // 更新作者（如果可用）
    if (info && info->mapAuthor) {
        char author_text[128];
        snprintf(author_text, sizeof(author_text), "Author: %s", info->mapAuthor);
        UIText_SetText((ui_text_t *)g_preview_author_text, author_text);
        UIComponent_SetVisible(g_preview_author_text, true);
    } else {
        UIComponent_SetVisible(g_preview_author_text, false);
    }
    
    // 更新推荐玩家数（如果可用）
    if (info && info->playersRecommended) {
        char players_text[128];
        snprintf(players_text, sizeof(players_text), "Players: %s", info->playersRecommended);
        UIText_SetText((ui_text_t *)g_preview_players_text, players_text);
        UIComponent_SetVisible(g_preview_players_text, true);
    } else {
        UIComponent_SetVisible(g_preview_players_text, false);
    }
    
    // 更新文件类型
    char type_text[128];
    sprintf(type_text, "Type: %s",
            item->type == ITEM_TYPE_MAP_W3M ? "Warcraft III Map (.w3m)" : "Warcraft III Expansion Map (.w3x)");
    UIText_SetText((ui_text_t *)g_preview_type_text, type_text);
    UIComponent_SetVisible(g_preview_type_text, true);
    
    // 更新完整路径
    char path_text[128];
    snprintf(path_text, sizeof(path_text), "Path: %s", item->full_path);
    UIText_SetText((ui_text_t *)g_preview_path_text, path_text);
    UIComponent_SetVisible(g_preview_path_text, true);
}

// 加载并保存地图信息到txt文件
bool MapSelect_LoadAndSaveMapInfo(const char *mapPath) {
    printf("Loading map info for: %s\n", mapPath);
    
    // 加载地图
    if (!CM_LoadMap(mapPath)) {
        printf("Failed to load map: %s\n", mapPath);
        return false;
    }
    
    // 获取地图信息
    LPCMAPINFO info = CM_GetMapInfo();
    if (!info) {
        printf("Failed to get map info\n");
        return false;
    }
    
    // 保存到txt文件
    MapSelect_SaveMapInfoToFile(mapPath, info);
    
    printf("Map info loaded and saved successfully\n");
    return true;
}

// 保存地图信息到txt文件
void MapSelect_SaveMapInfoToFile(const char *mapPath, LPCMAPINFO info) {
    // 生成txt文件名：将.w3m/w3x替换为.txt
    char txtPath[512];
    const char *ext = strrchr(mapPath, '.');
    if (ext) {
        size_t baseLen = ext - mapPath;
        strncpy(txtPath, mapPath, baseLen);
        txtPath[baseLen] = '\0';
        strcat(txtPath, ".txt");
    } else {
        strcpy(txtPath, mapPath);
        strcat(txtPath, ".txt");
    }
    
    FILE *fp = fopen(txtPath, "w");
    if (!fp) {
        printf("Failed to create file: %s\n", txtPath);
        return;
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "       WAR3 MAP INFO\n");
    fprintf(fp, "========================================\n\n");
    
    fprintf(fp, "File Path: %s\n", mapPath);
    fprintf(fp, "File Format: %d\n", info->fileFormat);
    fprintf(fp, "Editor Version: %d\n", info->editorVersion);
    fprintf(fp, "Number of Saves: %d\n", info->numberOfSaves);
    
    fprintf(fp, "\n--- Map Information ---\n");
    fprintf(fp, "Name: %s\n", info->mapName ? info->mapName : "(null)");
    fprintf(fp, "Author: %s\n", info->mapAuthor ? info->mapAuthor : "(null)");
    fprintf(fp, "Description: %s\n", info->mapDescription ? info->mapDescription : "(null)");
    fprintf(fp, "Players Recommended: %s\n", info->playersRecommended ? info->playersRecommended : "(null)");
    fprintf(fp, "Flags: 0x%08X\n", info->flags);
    fprintf(fp, "Main Ground Type: %c\n", info->mainGroundType);
    fprintf(fp, "Campaign Background: %d\n", info->campaignBackgroundNumber);
    fprintf(fp, "Loading Screen: %d\n", info->loadingScreenNumber);
    
    fprintf(fp, "\n--- Loading Screen Text ---\n");
    fprintf(fp, "Title: %s\n", info->loadingScreenTitle ? info->loadingScreenTitle : "(null)");
    fprintf(fp, "Subtitle: %s\n", info->loadingScreenSubtitle ? info->loadingScreenSubtitle : "(null)");
    fprintf(fp, "Text: %s\n", info->loadingScreenText ? info->loadingScreenText : "(null)");
    
    fprintf(fp, "\n--- Prologue Screen Text ---\n");
    fprintf(fp, "Title: %s\n", info->prologueScreenTitle ? info->prologueScreenTitle : "(null)");
    fprintf(fp, "Subtitle: %s\n", info->prologueScreenSubtitle ? info->prologueScreenSubtitle : "(null)");
    fprintf(fp, "Text: %s\n", info->prologueScreenText ? info->prologueScreenText : "(null)");
    
    fprintf(fp, "\n--- Camera Bounds ---\n");
    fprintf(fp, "Bounds: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f]\n",
            info->cameraBounds.bounds[0], info->cameraBounds.bounds[1],
            info->cameraBounds.bounds[2], info->cameraBounds.bounds[3],
            info->cameraBounds.bounds[4], info->cameraBounds.bounds[5],
            info->cameraBounds.bounds[6], info->cameraBounds.bounds[7]);
    fprintf(fp, "Margin: left=%d, right=%d, top=%d, bottom=%d\n",
            info->cameraBounds.margin.left, info->cameraBounds.margin.right,
            info->cameraBounds.margin.top, info->cameraBounds.margin.bottom);
    
    fprintf(fp, "\n--- Playable Area ---\n");
    fprintf(fp, "Width: %d\n", info->playableArea.width);
    fprintf(fp, "Height: %d\n", info->playableArea.height);
    
    fprintf(fp, "\n--- Players ---\n");
    int playerCount = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (info->players[i].used) {
            playerCount++;
            fprintf(fp, "  Player %d:\n", i);
            fprintf(fp, "    Type: ");
            switch (info->players[i].playerType) {
                case kPlayerTypeNone: fprintf(fp, "None\n"); break;
                case kPlayerTypeHuman: fprintf(fp, "Human\n"); break;
                case kPlayerTypeComputer: fprintf(fp, "Computer\n"); break;
                case kPlayerTypeNeutral: fprintf(fp, "Neutral\n"); break;
                case kPlayerTypeRescuable: fprintf(fp, "Rescuable\n"); break;
                default: fprintf(fp, "Unknown\n"); break;
            }
            fprintf(fp, "    Race: ");
            switch (info->players[i].playerRace) {
                case kPlayerRaceNone: fprintf(fp, "None\n"); break;
                case kPlayerRaceHuman: fprintf(fp, "Human\n"); break;
                case kPlayerRaceOrc: fprintf(fp, "Orc\n"); break;
                case kPlayerRaceUndead: fprintf(fp, "Undead\n"); break;
                case kPlayerRaceNightElf: fprintf(fp, "Night Elf\n"); break;
                default: fprintf(fp, "Unknown\n"); break;
            }
            fprintf(fp, "    Name: %s\n", info->players[i].playerName ? info->players[i].playerName : "(null)");
            fprintf(fp, "    Start Position: (%.2f, %.2f)\n", 
                    info->players[i].startingPosition.x, info->players[i].startingPosition.y);
            fprintf(fp, "    Flags: 0x%08X\n", info->players[i].flags);
            fprintf(fp, "    Ally Low: 0x%08X\n", info->players[i].allyLowPrioritiesFlags);
            fprintf(fp, "    Ally High: 0x%08X\n", info->players[i].allyHighPrioritiesFlags);
        }
    }
    fprintf(fp, "Total Players Used: %d\n", playerCount);
    
    fprintf(fp, "\n--- Teams ---\n");
    fprintf(fp, "Number of Teams: %d\n", info->num_teams);
    for (int i = 0; i < info->num_teams; i++) {
        fprintf(fp, "  Team %d:\n", i);
        fprintf(fp, "    Name: %s\n", info->teams[i].name ? info->teams[i].name : "(null)");
        fprintf(fp, "    Flags: 0x%08X\n", info->teams[i].flags);
        fprintf(fp, "    Player Masks: 0x%08X\n", info->teams[i].playerMasks);
    }
    
    fprintf(fp, "\n--- Map Script ---\n");
    fprintf(fp, "Script File: %s\n", info->mapscriptName ? info->mapscriptName : "(null)");
    
    fprintf(fp, "\n========================================\n");
    
    fclose(fp);
    printf("Map info saved to: %s\n", txtPath);
}

// 渲染地图选择界面
void MapSelect_Render(void) {
    if (g_state == MAP_SELECT_STATE_DONE) return;
    
    // 清除背景
    canvas2d_set_fill_style(g_ctx, (COLOR32){30, 30, 40, 255});
    canvas2d_fill_rect(g_ctx, 0, 0, 1024, 768);
    canvas2d_draw_debug_grid(g_ctx, 0, 0, 1024, 768, 40, 30, true);

    // 更新事件分发器
    UIEventDispatcher_Update(&g_event_dispatcher, 0);

    // 渲染 UI 列表
    UIList_Render((ui_list_t *)g_ui_list);
    
    // 检测选中变化并自动加载地图信息
    int selected = UIList_GetSelected((ui_list_t *)g_ui_list);
    if (selected >= 0) {
        void *user_data = UIList_GetSelectedUserData((ui_list_t *)g_ui_list);
        if (user_data) {
            int all_index = (int)(intptr_t)user_data;
            if (all_index >= 0 && all_index < g_all_count) {
                browser_item_t *item = &g_all_items[all_index];
                if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
                    // 比较当前路径是否变化
                    if (strcmp(item->full_path, g_current_preview_map) != 0) {
                        // 路径变化，加载新地图信息
                        strcpy(g_current_preview_map, item->full_path);
                        printf("Loading map info for preview: %s\n", g_current_preview_map);
                        MapSelect_LoadAndSaveMapInfo(g_current_preview_map);
                    }
                }
            }
        }
    } else {
        // 没有选中项，清空当前预览
        g_current_preview_map[0] = '\0';
    }
    
    // 更新并渲染地图预览容器
    UpdateMapPreview();
    UIContainer_Render((ui_container_t *)g_preview_container);
    
    // 渲染 START GAME 按钮
    UIButton_Render((ui_button_t *)g_start_button);
    
    // 在右下角显示鼠标位置和调试信息
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 255, 0, 255});
    canvas2d_set_font_size(g_ctx, 14.0f);
    
    char mouse_pos_text[128];
    snprintf(mouse_pos_text, sizeof(mouse_pos_text),
             "Mouse: (%d, %d) | Event: %d | Button: %d",
             (int)mouse.origin.x, (int)mouse.origin.y, mouse.event, mouse.button);
    canvas2d_fill_text(g_ctx, mouse_pos_text, 1024 - 400, 768 - 30);
    
    // 检查鼠标是否在按钮内
    bool in_button = (mouse.origin.x >= g_start_button->x &&
                      mouse.origin.x < g_start_button->x + g_start_button->width &&
                      mouse.origin.y >= g_start_button->y &&
                      mouse.origin.y < g_start_button->y + g_start_button->height);
    char button_state_text[128];
    snprintf(button_state_text, sizeof(button_state_text),
             "Button Rect: (%.0f, %.0f) w=%.0f h=%.0f | InButton: %s | State: %d",
             g_start_button->x, g_start_button->y,
             g_start_button->width, g_start_button->height,
             in_button ? "YES" : "NO",
             ((ui_button_t *)g_start_button)->state);
    canvas2d_fill_text(g_ctx, button_state_text, 1024 - 600, 768 - 55);
    
    // 渲染文本组件
    // 更新路径文本
    if (g_at_root) {
        UIText_SetText((ui_text_t *)g_path_text, "Root");
    } else {
        UIText_SetText((ui_text_t *)g_path_text, g_current_path);
    }
    UIText_Render((ui_text_t *)g_title_text);
    UIText_Render((ui_text_t *)g_path_text);
    UIText_Render((ui_text_t *)g_hint_text1);
    UIText_Render((ui_text_t *)g_hint_text2);
    
}

// 进入文件夹
static void EnterFolder(const char *folder_path) {
    if (strcmp(folder_path, "..") == 0) {
        // 返回上级目录
        char *last_sep = strrchr(g_current_path, '\\');
        if (last_sep) {
            *last_sep = '\0';
            // 如果路径为空，返回根目录
            if (strlen(g_current_path) == 0) {
                g_at_root = true;
            }
        } else {
            // 没有上级目录，已经在根目录
            g_at_root = true;
            g_current_path[0] = '\0';
        }
    } else {
        // 进入子目录
        if (g_at_root) {
            // 从根目录进入，直接使用文件夹名
            strncpy(g_current_path, folder_path, MAX_PATHLEN - 1);
            g_current_path[MAX_PATHLEN - 1] = '\0';
        } else {
            // 从子目录进入，追加到当前路径
            size_t len = strlen(g_current_path);
            strncat(g_current_path, "\\", sizeof(g_current_path) - len - 1);
            strncat(g_current_path, folder_path, sizeof(g_current_path) - strlen(g_current_path) - 1);
        }
        g_at_root = false;
    }
    
    // 重新过滤列表
    FilterCurrentPath();
}

// 处理输入事件
bool MapSelect_HandleInput(int key, bool down) {
    if (g_state == MAP_SELECT_STATE_DONE) return false;

    // 使用事件分发器处理键盘事件
    if (down) {
        // 按键按下
        if (UIEventDispatcher_DispatchKeyDown(&g_event_dispatcher, key, 0, 0, false, SDL_GetTicks())) {
            return true;
        }
    } else {
        // 按键释放（暂不处理keyup）
        // UIEventDispatcher_DispatchKeyUp(&g_event_dispatcher, key, 0, 0, SDL_GetTicks());
    }
    
    if (down) {
        switch (key) {
            case SDLK_RETURN: {
                int selected = UIList_GetSelected((ui_list_t *)g_ui_list);
                if (selected >= 0) {
                    void *user_data = UIList_GetSelectedUserData((ui_list_t *)g_ui_list);
                    if (user_data) {
                        int all_index = (int)(intptr_t)user_data;
                        if (all_index < 0 || all_index >= g_all_count) break;
                        
                        browser_item_t *item = &g_all_items[all_index];
                        
                        if (item->type == ITEM_TYPE_FOLDER) {
                            // 进入文件夹
                            EnterFolder(item->name);
                        } else {
                            // 选择地图 - 地图信息已在预览时加载，直接开始游戏
                            StartGame(item->full_path, "keyboard");
                        }
                    }
                } else {
                    // 处理 ".." 返回上级目录的特殊情况
                    EnterFolder("..");
                }
                return true;
            }
            
            case SDLK_ESCAPE:
                // 退出游戏
                return true;
        }
    }
    
    return false;
}

// 处理鼠标事件
bool MapSelect_HandleMouseEvent(void) {
    if (g_state == MAP_SELECT_STATE_DONE) return false;
    
    bool handled = false;
    
    // printf("[Mouse] Event=%d, Pos=(%d,%d), Button=%d\n",
    //        mouse.event, (int)mouse.origin.x, (int)mouse.origin.y, mouse.button);
    
    // 使用事件分发器处理鼠标事件
    switch (mouse.event) {
        case UI_LEFT_MOUSE_DOWN:
            handled = UIEventDispatcher_DispatchMouseDown(&g_event_dispatcher,
                                                          mouse.origin.x, mouse.origin.y,
                                                          UI_MOUSE_BUTTON_LEFT, SDL_GetTicks());
            break;
        case UI_LEFT_MOUSE_UP:
            handled = UIEventDispatcher_DispatchMouseUp(&g_event_dispatcher,
                                                        mouse.origin.x, mouse.origin.y,
                                                        UI_MOUSE_BUTTON_LEFT, SDL_GetTicks());
            break;
        case UI_LEFT_MOUSE_DRAGGED:
            handled = UIEventDispatcher_DispatchMouseMove(&g_event_dispatcher,
                                                          mouse.origin.x, mouse.origin.y,
                                                          SDL_GetTicks());
            break;
    }
    
    return handled;
}
char* MapSelect_GetStartMap(void) {
    return g_start_map_path;
}   

// 获取选中的地图
const char* MapSelect_GetSelectedMap(void) {
    int selected = UIList_GetSelected((ui_list_t *)g_ui_list);
    if (selected >= 0) {
        void *user_data = UIList_GetSelectedUserData((ui_list_t *)g_ui_list);
        if (user_data) {
            int all_index = (int)(intptr_t)user_data;
            if (all_index < 0 || all_index >= g_all_count) return NULL;
            
            browser_item_t *item = &g_all_items[all_index];
            if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
                return item->full_path;
            }
        }
    }
    return NULL;
}

// 清理地图选择界面
void MapSelect_Shutdown(void) {
    printf("Shutting down Map Selection Screen...\n");
    
    // 清理事件分发器
    UIEventDispatcher_Shutdown(&g_event_dispatcher);
    
    // 清理根容器（会递归清理所有子组件）
    if (g_root_container) {
        UIContainer_Destroy((ui_container_t *)g_root_container);
        g_root_container = NULL;
    }
    
    // 所有UI组件已经由根容器清理，只需置空指针
    g_ui_list = NULL;
    g_start_button = NULL;
    g_title_text = NULL;
    g_path_text = NULL;
    g_hint_text1 = NULL;
    g_hint_text2 = NULL;
    g_preview_container = NULL;
    g_preview_title_text = NULL;
    g_preview_filename_text = NULL;
    g_preview_name_text = NULL;
    g_preview_author_text = NULL;
    g_preview_players_text = NULL;
    g_preview_type_text = NULL;
    g_preview_path_text = NULL;
    
    if (g_canvas) {
        canvas2d_destroy(g_canvas);
        g_canvas = NULL;
        g_ctx = NULL;
    }
    
    // 释放地图路径内存
    if (g_start_map_path) {
        free(g_start_map_path);
        g_start_map_path = NULL;
    }
    
    g_map_count = 0;
    g_state = MAP_SELECT_STATE_INIT;
}
// ========================================
// Scene 接口实现
// ========================================

// Scene实例
static scene_t g_map_select_scene = {
    .name = "MapSelect",
    .state = SCENE_STATE_UNINITIALIZED,
    .user_data = NULL,
    .launch_params = NULL,
    .manager = NULL,
    .init = MapSelectScene_Init,
    .shutdown = MapSelectScene_Shutdown,
    .update = MapSelectScene_Update,
    .render = MapSelectScene_Render,
    .on_input = MapSelectScene_OnInput,
    .pause = NULL,
    .resume = NULL
};

// Scene 初始化
int MapSelectScene_Init(scene_t *scene, const scene_params_t *params) {
    printf("MapSelectScene: Initializing...\n");
    
    // 检查启动参数
    if (params) {
        const char *start_folder = PARAMS_GET_STRING(params, "start_folder", "");
        if (strlen(start_folder) > 0) {
            printf("Starting folder: %s\n", start_folder);
            // TODO: 可以导航到指定文件夹
        }
    }
    
    // 调用原有的初始化函数
    int result = MapSelect_Init();
    if (result != 0) {
        printf("MapSelectScene: MapSelect_Init failed with code %d\n", result);
        return -1;
    }
    
    // 设置场景的根组件
    Scene_SetRootComponent(scene, g_root_container);
    
    printf("MapSelectScene: Initialized successfully\n");
    return 0;
}

// Scene 关闭
void MapSelectScene_Shutdown(scene_t *scene) {
    printf("MapSelectScene: Shutting down...\n");
    
    // 调用原有的关闭函数
    MapSelect_Shutdown();
    
    printf("MapSelectScene: Shutdown complete\n");
}

// Scene 更新
scene_transition_t* MapSelectScene_Update(scene_t *scene, int msec) {
    // 更新场景的所有UI组件
    Scene_UpdateUI(scene, msec);
    
    // 检查是否需要切换到游戏场景
    if (g_state == MAP_SELECT_STATE_DONE && g_start_map_path) {
        
        // 创建跳转参数
        scene_params_t *params = SceneParams_Create();
        if (params) {
            SceneParams_SetString(params, "map_path", g_start_map_path);
            SceneParams_SetString(params, "start_folder", g_current_path);
            
            // 创建切换到Game场景的请求
            // 使用场景名称创建跳转请求
            scene_transition_t *transition = SceneTransition_CreateByName(
                TRANSITION_SWITCH,
                "Game",
                params,   // 传递地图路径
                NULL     // 不需要返回值
            );
            
            // 重置状态
            g_state = MAP_SELECT_STATE_INIT;
            if (g_start_map_path) {
                free(g_start_map_path);
                g_start_map_path = NULL;
            }
            
            return transition;
        }
    }
    
    return NULL;
}

// 获取场景实例
scene_t* MapSelectScene_GetInstance(void) {
    return &g_map_select_scene;
}

// Scene 渲染
void MapSelectScene_Render(scene_t *scene) {
    // 绘制背景
    canvas2d_set_fill_style(g_ctx, (COLOR32){30, 30, 40, 255});
    canvas2d_fill_rect(g_ctx, 0, 0, 1024, 768);
    
    // 渲染场景的所有UI组件（递归渲染根容器及其所有子组件）
    Scene_RenderUI(scene);
}

// Scene 输入处理
void MapSelectScene_OnInput(scene_t *scene, input_event_t *event) {
    // 优先将事件分发到场景的UI组件系统
    bool handled = Scene_DispatchInputToUI(scene, event);
    
    // 如果UI组件没有处理事件，则使用旧的处理逻辑
    if (!handled) {
        switch (event->type) {
            case INPUT_EVENT_KEY_DOWN:
            case INPUT_EVENT_KEY_UP:
                // 键盘事件 - 使用事件分发器处理
                MapSelect_HandleInput(event->key.key, event->key.down);
                break;
                
            case INPUT_EVENT_MOUSE_DOWN: {
                // 鼠标按下事件 - 坐标已在main.c中归一化
                mouse.origin.x = event->mouse.x;
                mouse.origin.y = event->mouse.y;
                mouse.button = event->mouse.button;
                mouse.event = UI_LEFT_MOUSE_DOWN;
                
                MapSelect_HandleMouseEvent();
                break;
            }
                
            case INPUT_EVENT_MOUSE_UP: {
                // 鼠标释放事件 - 坐标已在main.c中归一化
                mouse.origin.x = event->mouse.x;
                mouse.origin.y = event->mouse.y;
                mouse.button = 0;
                mouse.event = UI_LEFT_MOUSE_UP;
                
                MapSelect_HandleMouseEvent();
                break;
            }
                
            case INPUT_EVENT_MOUSE_MOTION: {
                // 鼠标移动事件 - 坐标已在main.c中归一化
                mouse.origin.x = event->motion.x;
                mouse.origin.y = event->motion.y;
                mouse.event = UI_LEFT_MOUSE_DRAGGED;
                
                MapSelect_HandleMouseEvent();
                break;
            }
                
            default:
                break;
        }
    }
}

