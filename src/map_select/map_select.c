#include "map_select.h"
#include "ui_list.h"
#include "ui_button.h"
#include "ui_text.h"
#include "ui_container.h"
#include "../canvas2d/canvas2d.h"
#include "../client/client.h"
#include "../common/common.h"
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

// 最大路径长度
#define MAX_PATH_LEN 512

// 文件浏览器项类型
typedef enum {
    ITEM_TYPE_FOLDER,
    ITEM_TYPE_MAP_W3M,
    ITEM_TYPE_MAP_W3X
} item_type_t;

// 文件浏览器项
typedef struct {
    char name[256];            // 显示名称
    char full_path[MAX_PATH_LEN]; // 完整路径
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

// 当前浏览的路径
static char g_current_path[MAX_PATH_LEN] = "";
static bool g_at_root = true;

// 当前预览的地图路径（用于检测选中变化）
static char g_current_preview_map[MAX_PATH_LEN] = "";

// Canvas2D 画布
static canvas2d_t *g_canvas = NULL;
static canvas2d_context_t *g_ctx = NULL;

// UI 列表组件
static ui_list_t g_ui_list;

// UI 按钮组件
static ui_button_t g_start_button;

// UI 文本组件
static ui_text_t g_title_text;
static ui_text_t g_path_text;
static ui_text_t g_hint_text1;
static ui_text_t g_hint_text2;

// UI 容器组件（地图预览）
static ui_container_t g_preview_container;
static ui_text_t g_preview_title_text;      // "Map Preview"
static ui_text_t g_preview_filename_text;     // 地图文件名
static ui_text_t g_preview_name_text;        // 地图名称
static ui_text_t g_preview_author_text;       // 作者
static ui_text_t g_preview_players_text;     // 推荐玩家数
static ui_text_t g_preview_type_text;        // 文件类型
static ui_text_t g_preview_path_text;       // 完整路径

// 全局标志：是否在地图选择模式
bool g_in_map_select = false;

// START GAME 按钮点击回调
static void OnStartGameClick(void *user_data) {
    int selected = UIList_GetSelected(&g_ui_list);
    if (selected >= 0) {
        void *user_data = UIList_GetSelectedUserData(&g_ui_list);
        if (user_data) {
            int all_index = (int)(intptr_t)user_data;
            if (all_index >= 0 && all_index < g_all_count) {
                browser_item_t *item = &g_all_items[all_index];
                if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
                    printf("Starting game (button click): %s\n", item->full_path);
                    g_state = MAP_SELECT_STATE_DONE;
                }
            }
        }
    }
}

// 自定义列表项绘制函数
static void DrawBrowserItemCustom(
    int index,
    const char *text,
    void *user_data,
    float x,
    float y,
    float width,
    float height,
    bool selected
) {
    // 背景
    if (selected) {
        canvas2d_set_fill_style(g_ctx, (COLOR32){100, 150, 255, 200});
    } else {
        canvas2d_set_fill_style(g_ctx, (COLOR32){50, 50, 50, 200});
    }
    canvas2d_fill_rect(g_ctx, x, y, width, height);

    // 边框
    canvas2d_set_stroke_style(g_ctx, (COLOR32){200, 200, 200, 255});
    canvas2d_set_line_width(g_ctx, 2.0f);
    canvas2d_stroke_rect(g_ctx, x, y, width, height);

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

    // 绘制图标
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 255, 255, 255});
    canvas2d_fill_text(g_ctx, icon, x + 10, y + 10);

    // 绘制文件名（使用回调传入的text参数）
    canvas2d_fill_text(g_ctx, text, x + 50, y + 10);
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
        char line[MAX_PATH_LEN];
        strncpy(line, line_start, line_len < MAX_PATH_LEN - 1 ? line_len : MAX_PATH_LEN - 1);
        line[line_len < MAX_PATH_LEN - 1 ? line_len : MAX_PATH_LEN - 1] = '\0';
        
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
            strncpy(all_item->full_path, line, MAX_PATH_LEN - 1);
            all_item->full_path[MAX_PATH_LEN - 1] = '\0';
            
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
    UIList_ClearItems(&g_ui_list);
    
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
        UIList_AddItem(&g_ui_list, name, (void*)(intptr_t)idx);
    }
    
    printf("Filtered %d items, UI list has %d items\n", g_filtered_count, UIList_GetItemCount(&g_ui_list));
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
    
    // 初始化 UI 列表
    ui_list_config_t list_config = {
        .x = 50.0f,
        .y = 100.0f,
        .width = 400.0f,
        .height = 500.0f,
        .item_height = 50.0f,
        .item_spacing = 5.0f,
        .bg_color = {30, 30, 40, 255},
        .selected_bg_color = {100, 150, 255, 200},
        .border_color = {200, 200, 200, 255},
        .text_color = {255, 255, 255, 255},
        .selected_text_color = {255, 255, 255, 255},
        .show_scrollbar = true,
        .draw_callback = DrawBrowserItemCustom,
        .user_data = NULL
    };
    
    if (UIList_Init(&g_ui_list, &list_config, g_ctx) != 0) {
        printf("Failed to initialize UI list\n");
        return -1;
    }
    
    // 初始化 START GAME 按钮
    ui_button_config_t button_config = {
        .x = 480.0f,
        .y = 420.0f,
        .width = 200.0f,
        .height = 50.0f,
        .text = "START GAME [ENTER]",
        .bg_color = {
            {0, 150, 0, 255},      // 正常
            {0, 180, 0, 255},      // 悬停
            {0, 120, 0, 255},      // 按下
            {100, 100, 100, 255}   // 禁用
        },
        .border_color = {
            {0, 255, 0, 255},      // 正常
            {50, 255, 50, 255},    // 悬停
            {0, 200, 0, 255},      // 按下
            {150, 150, 150, 255}   // 禁用
        },
        .text_color = {
            {255, 255, 255, 255},  // 正常
            {255, 255, 255, 255},  // 悬停
            {255, 255, 255, 255},  // 按下
            {180, 180, 180, 255}   // 禁用
        },
        .border_width = 2.0f,
        .on_click = OnStartGameClick,
        .user_data = NULL,
        .enabled = true,
        .visible = true
    };
    
    if (UIButton_Init(&g_start_button, &button_config, g_ctx) != 0) {
        printf("Failed to initialize UI button\n");
        return -1;
    }
    
    // 初始化标题文本
    ui_text_config_t title_config = {
        .x = 400.0f,
        .y = 30.0f,
        .text = "SELECT MAP",
        .color = {255, 215, 0, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_CENTER,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    
    if (UIText_Init(&g_title_text, &title_config, g_ctx) != 0) {
        printf("Failed to initialize title text\n");
        return -1;
    }
    
    // 初始化路径文本
    ui_text_config_t path_config = {
        .x = 50.0f,
        .y = 60.0f,
        .text = "Root",
        .color = {200, 200, 200, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    
    if (UIText_Init(&g_path_text, &path_config, g_ctx) != 0) {
        printf("Failed to initialize path text\n");
        return -1;
    }
    
    // 初始化提示文本1
    ui_text_config_t hint1_config = {
        .x = 50.0f,
        .y = 710.0f,
        .text = "Use UP/DOWN arrows to navigate, ENTER to select/open",
        .color = {200, 200, 200, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    
    if (UIText_Init(&g_hint_text1, &hint1_config, g_ctx) != 0) {
        printf("Failed to initialize hint text 1\n");
        return -1;
    }
    
    // 初始化提示文本2
    ui_text_config_t hint2_config = {
        .x = 900.0f,
        .y = 710.0f,
        .text = "ESC to quit",
        .color = {200, 200, 200, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    
    if (UIText_Init(&g_hint_text2, &hint2_config, g_ctx) != 0) {
        printf("Failed to initialize hint text 2\n");
        return -1;
    }
    
    // 初始化地图预览容器
    ui_container_config_t preview_config = {
        .x = 480.0f,
        .y = 100.0f,
        .width = 480.0f,
        .height = 300.0f,
        .bg_color = {40, 40, 50, 230},
        .border_color = {255, 215, 0, 255},
        .border_width = 2.0f,
        .visible = true,
        .max_items = 7  // 6个文本组件
    };
    
    if (UIContainer_Init(&g_preview_container, &preview_config, g_ctx) != 0) {
        printf("Failed to initialize preview container\n");
        return -1;
    }
    
    // 初始化预览文本组件（标题 "Map Preview"）
    ui_text_config_t preview_title_config = {
        .x = 500.0f,
        .y = 130.0f,
        .text = "Map Preview",
        .color = {255, 215, 0, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_title_text, &preview_title_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_title_text);
    
    // 初始化预览文本组件（文件名）
    ui_text_config_t preview_filename_config = {
        .x = 500.0f,
        .y = 172.0f,
        .text = "",
        .color = {200, 200, 200, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_filename_text, &preview_filename_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_filename_text);
    
    // 初始化预览文本组件（地图名称）
    ui_text_config_t preview_name_config = {
        .x = 500.0f,
        .y = 194.0f,
        .text = "",
        .color = {180, 180, 180, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_name_text, &preview_name_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_name_text);
    
    // 初始化预览文本组件（作者）
    ui_text_config_t preview_author_config = {
        .x = 500.0f,
        .y = 216.0f,
        .text = "",
        .color = {160, 160, 160, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_author_text, &preview_author_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_author_text);
    
    // 初始化预览文本组件（推荐玩家数）
    ui_text_config_t preview_players_config = {
        .x = 500.0f,
        .y = 238.0f,
        .text = "",
        .color = {140, 140, 140, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_players_text, &preview_players_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_players_text);
    
    // 初始化预览文本组件（文件类型）
    ui_text_config_t preview_type_config = {
        .x = 500.0f,
        .y = 260.0f,
        .text = "",
        .color = {150, 150, 150, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_type_text, &preview_type_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_type_text);
    
    // 初始化预览文本组件（完整路径）
    ui_text_config_t preview_path_config = {
        .x = 500.0f,
        .y = 282.0f,
        .text = "",
        .color = {100, 100, 100, 255},
        .font_size = 16.0f,
        .align = UI_TEXT_ALIGN_LEFT,
        .valign = UI_TEXT_VALIGN_TOP,
        .wrap = false,
        .wrap_width = 0
    };
    UIText_Init(&g_preview_path_text, &preview_path_config, g_ctx);
    UIContainer_AddText(&g_preview_container, &g_preview_path_text);
    
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
    g_in_map_select = true;
    
    printf("Map Selection Screen initialized\n");
    return 0;
}

// 更新地图选择界面
void MapSelect_Update(int msec) {
    if (g_state == MAP_SELECT_STATE_DONE) {
        return;
    }
}



// 更新地图预览信息
static void UpdateMapPreview(void) {
    int selected = UIList_GetSelected(&g_ui_list);
    
    // 如果没有选中项，隐藏预览容器
    if (selected < 0) {
        UIContainer_SetVisible(&g_preview_container, false);
        return;
    }
    
    void *user_data = UIList_GetSelectedUserData(&g_ui_list);
    if (!user_data) {
        UIContainer_SetVisible(&g_preview_container, false);
        return;
    }
    
    int all_index = (int)(intptr_t)user_data;
    // -1 表示 ".." 返回上级目录，不显示预览
    if (all_index < 0 || all_index >= g_all_count) {
        UIContainer_SetVisible(&g_preview_container, false);
        return;
    }
    
    browser_item_t *item = &g_all_items[all_index];
    
    // 只为地图文件显示预览
    if (item->type != ITEM_TYPE_MAP_W3M && item->type != ITEM_TYPE_MAP_W3X) {
        UIContainer_SetVisible(&g_preview_container, false);
        return;
    }
    
    // 显示预览容器
    UIContainer_SetVisible(&g_preview_container, true);
    
    // 获取地图信息
    LPCMAPINFO info = CM_GetMapInfo();
    
    // 更新文件名
    UIText_SetText(&g_preview_filename_text, item->name);
    UIText_SetVisible(&g_preview_filename_text, true);
    
    // 更新地图名称（如果可用）
    if (info && info->mapName) {
        char name_text[128];
        snprintf(name_text, sizeof(name_text), "Name: %s", info->mapName);
        UIText_SetText(&g_preview_name_text, name_text);
        UIText_SetVisible(&g_preview_name_text, true);
    } else {
        UIText_SetVisible(&g_preview_name_text, false);
    }
    
    // 更新作者（如果可用）
    if (info && info->mapAuthor) {
        char author_text[128];
        snprintf(author_text, sizeof(author_text), "Author: %s", info->mapAuthor);
        UIText_SetText(&g_preview_author_text, author_text);
        UIText_SetVisible(&g_preview_author_text, true);
    } else {
        UIText_SetVisible(&g_preview_author_text, false);
    }
    
    // 更新推荐玩家数（如果可用）
    if (info && info->playersRecommended) {
        char players_text[128];
        snprintf(players_text, sizeof(players_text), "Players: %s", info->playersRecommended);
        UIText_SetText(&g_preview_players_text, players_text);
        UIText_SetVisible(&g_preview_players_text, true);
    } else {
        UIText_SetVisible(&g_preview_players_text, false);
    }
    
    // 更新文件类型
    char type_text[128];
    sprintf(type_text, "Type: %s", 
            item->type == ITEM_TYPE_MAP_W3M ? "Warcraft III Map (.w3m)" : "Warcraft III Expansion Map (.w3x)");
    UIText_SetText(&g_preview_type_text, type_text);
    UIText_SetVisible(&g_preview_type_text, true);
    
    // 更新完整路径
    char path_text[128];
    snprintf(path_text, sizeof(path_text), "Path: %s", item->full_path);
    UIText_SetText(&g_preview_path_text, path_text);
    UIText_SetVisible(&g_preview_path_text, true);
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
    
    // 标题和当前路径
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 215, 0, 255});
    canvas2d_fill_text(g_ctx, "SELECT MAP", 400, 30);
    
    // 显示当前路径
    if (g_at_root) {
        canvas2d_set_fill_style(g_ctx, (COLOR32){200, 200, 200, 255});
        canvas2d_fill_text(g_ctx, "Root", 50, 60);
    } else {
        canvas2d_set_fill_style(g_ctx, (COLOR32){200, 200, 200, 255});
        canvas2d_fill_text(g_ctx, g_current_path, 50, 60);
    }
    
    // 渲染 UI 列表
    UIList_Render(&g_ui_list);
    
    // 检测选中变化并自动加载地图信息
    int selected = UIList_GetSelected(&g_ui_list);
    if (selected >= 0) {
        void *user_data = UIList_GetSelectedUserData(&g_ui_list);
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
    UIContainer_Render(&g_preview_container);
    
    // 渲染 START GAME 按钮
    UIButton_Render(&g_start_button);
    
    // 渲染文本组件
    // 更新路径文本
    if (g_at_root) {
        UIText_SetText(&g_path_text, "Root");
    } else {
        UIText_SetText(&g_path_text, g_current_path);
    }
    UIText_Render(&g_title_text);
    UIText_Render(&g_path_text);
    UIText_Render(&g_hint_text1);
    UIText_Render(&g_hint_text2);
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
            strncpy(g_current_path, folder_path, MAX_PATH_LEN - 1);
            g_current_path[MAX_PATH_LEN - 1] = '\0';
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
    
    // 先让 UI 列表处理导航键
    if (UIList_HandleInput(&g_ui_list, key, down)) {
        return true;
    }
    
    if (down) {
        switch (key) {
            case SDLK_RETURN: {
                int selected = UIList_GetSelected(&g_ui_list);
                if (selected >= 0) {
                    void *user_data = UIList_GetSelectedUserData(&g_ui_list);
                    if (user_data) {
                        int all_index = (int)(intptr_t)user_data;
                        if (all_index < 0 || all_index >= g_all_count) break;
                        
                        browser_item_t *item = &g_all_items[all_index];
                        
                        if (item->type == ITEM_TYPE_FOLDER) {
                            // 进入文件夹
                            EnterFolder(item->name);
                        } else {
                            // 选择地图 - 地图信息已在预览时加载，直接开始游戏
                            printf("Starting game: %s\n", item->full_path);
                            g_state = MAP_SELECT_STATE_DONE;
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
    
    // 处理按钮鼠标事件
    bool down = (mouse.event == UI_LEFT_MOUSE_DOWN);
    if (UIButton_HandleMouseClick(&g_start_button, mouse.origin.x, mouse.origin.y, down)) {
        handled = true;
    }
    
    // 处理 UI 列表鼠标事件（只处理左键按下）
    if (mouse.event == UI_LEFT_MOUSE_DOWN && UIList_HandleMouseClick(&g_ui_list, mouse.origin.x, mouse.origin.y)) {
        // 双击检测：如果是地图文件且当前已选中该项，则进入/选择
        int selected = UIList_GetSelected(&g_ui_list);
        if (selected >= 0) {
            void *user_data = UIList_GetSelectedUserData(&g_ui_list);
            if (user_data) {
                int all_index = (int)(intptr_t)user_data;
                if (all_index >= 0 && all_index < g_all_count) {
                    browser_item_t *item = &g_all_items[all_index];
                    if (item->type == ITEM_TYPE_FOLDER) {
                        // 点击进入文件夹
                        EnterFolder(item->name);
                        handled = true;
                    } else if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
                        // 地图文件，单击只是选中，不直接开始游戏
                        // 双击或者点击 START GAME 按钮才开始游戏
                        printf("Map selected: %s\n", item->full_path);
                        handled = true;
                    }
                }
            }
        }
    }
    
    return handled;
}

// 获取选中的地图
const char* MapSelect_GetSelectedMap(void) {
    int selected = UIList_GetSelected(&g_ui_list);
    if (selected >= 0) {
        void *user_data = UIList_GetSelectedUserData(&g_ui_list);
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
    
    UIList_Shutdown(&g_ui_list);
    UIButton_Shutdown(&g_start_button);
    UIText_Shutdown(&g_title_text);
    UIText_Shutdown(&g_path_text);
    UIText_Shutdown(&g_hint_text1);
    UIText_Shutdown(&g_hint_text2);
    
    // 清理预览容器和组件
    UIContainer_Shutdown(&g_preview_container);
    UIText_Shutdown(&g_preview_title_text);
    UIText_Shutdown(&g_preview_filename_text);
    UIText_Shutdown(&g_preview_name_text);
    UIText_Shutdown(&g_preview_author_text);
    UIText_Shutdown(&g_preview_players_text);
    UIText_Shutdown(&g_preview_type_text);
    UIText_Shutdown(&g_preview_path_text);
    
    if (g_canvas) {
        canvas2d_destroy(g_canvas);
        g_canvas = NULL;
        g_ctx = NULL;
    }
    
    g_map_count = 0;
    g_state = MAP_SELECT_STATE_INIT;
    g_in_map_select = false;
}
