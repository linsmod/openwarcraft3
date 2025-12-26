#include "map_select.h"
#include "ui_list.h"
#include "../canvas2d/canvas2d.h"
#include "../client/client.h"
#include "../common/common.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

// Canvas2D 画布
static canvas2d_t *g_canvas = NULL;
static canvas2d_context_t *g_ctx = NULL;

// UI 列表组件
static ui_list_t g_ui_list;

// 全局标志：是否在地图选择模式
bool g_in_map_select = false;

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
    if (strchr(filename, '\\') != NULL) {
        return ITEM_TYPE_FOLDER;
    }
    
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



// 绘制地图预览信息
static void DrawMapPreview(float x, float y, float width, float height) {
    int selected = UIList_GetSelected(&g_ui_list);
    if (selected < 0) return;
    
    void *user_data = UIList_GetSelectedUserData(&g_ui_list);
    if (!user_data) return;
    
    int all_index = (int)(intptr_t)user_data;
    // -1 表示 ".." 返回上级目录，不显示预览
    if (all_index < 0 || all_index >= g_all_count) return;
    
    browser_item_t *item = &g_all_items[all_index];
    
    // 只为地图文件显示预览
    if (item->type != ITEM_TYPE_MAP_W3M && item->type != ITEM_TYPE_MAP_W3X) {
        return;
    }
    
    // 背景
    canvas2d_set_fill_style(g_ctx, (COLOR32){40, 40, 50, 230});
    canvas2d_fill_rect(g_ctx, x, y, width, height);
    
    // 边框
    canvas2d_set_stroke_style(g_ctx, (COLOR32){255, 215, 0, 255});
    canvas2d_set_line_width(g_ctx, 2.0f);
    canvas2d_stroke_rect(g_ctx, x, y, width, height);
    
    // 标题
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 215, 0, 255});
    canvas2d_fill_text(g_ctx, "Map Preview", x + 20, y + 30);
    
    // 地图文件名
    canvas2d_set_fill_style(g_ctx, (COLOR32){200, 200, 200, 255});
    canvas2d_fill_text(g_ctx, item->name, x + 20, y + 60);
    
    // 完整路径
    canvas2d_set_fill_style(g_ctx, (COLOR32){150, 150, 150, 255});
    canvas2d_fill_text(g_ctx, item->full_path, x + 20, y + 90);
    
    // 文件类型
    char type_text[64];
    sprintf(type_text, "Type: %s", 
            item->type == ITEM_TYPE_MAP_W3M ? "Warcraft III Map (.w3m)" : "Warcraft III Expansion Map (.w3x)");
    canvas2d_fill_text(g_ctx, type_text, x + 20, y + 120);
}

// 绘制开始游戏按钮
static void DrawStartButton(float x, float y, float width, float height) {
    int selected = UIList_GetSelected(&g_ui_list);
    if (selected < 0) return;
    
    void *user_data = UIList_GetSelectedUserData(&g_ui_list);
    if (!user_data) return;
    
    int all_index = (int)(intptr_t)user_data;
    // -1 表示 ".." 返回上级目录，不显示按钮
    if (all_index < 0 || all_index >= g_all_count) return;
    
    browser_item_t *item = &g_all_items[all_index];
    
    // 只为地图文件显示按钮
    if (item->type != ITEM_TYPE_MAP_W3M && item->type != ITEM_TYPE_MAP_W3X) {
        return;
    }
    
    // 背景
    canvas2d_set_fill_style(g_ctx, (COLOR32){0, 150, 0, 255});
    canvas2d_fill_rect(g_ctx, x, y, width, height);
    
    // 边框
    canvas2d_set_stroke_style(g_ctx, (COLOR32){0, 255, 0, 255});
    canvas2d_set_line_width(g_ctx, 2.0f);
    canvas2d_stroke_rect(g_ctx, x, y, width, height);
    
    // 按钮文本
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 255, 255, 255});
    canvas2d_fill_text(g_ctx, "START GAME [ENTER]", x + 30, y + 20);
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
    
    // 地图预览区域
    float preview_x = 480;
    float preview_y = 100;
    float preview_width = 480;
    float preview_height = 300;
    DrawMapPreview(preview_x, preview_y, preview_width, preview_height);
    
    // 开始游戏按钮
    float button_x = 480;
    float button_y = 420;
    float button_width = 200;
    float button_height = 50;
    DrawStartButton(button_x, button_y, button_width, button_height);
    
    // 提示文本
    canvas2d_set_fill_style(g_ctx, (COLOR32){200, 200, 200, 255});
    canvas2d_fill_text(g_ctx, "Use UP/DOWN arrows to navigate, ENTER to select/open", 50, 710);
    canvas2d_fill_text(g_ctx, "ESC to quit", 900, 710);
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
                            // 选择地图
                            printf("Selected map: %s\n", item->full_path);
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
    
    if (g_canvas) {
        canvas2d_destroy(g_canvas);
        g_canvas = NULL;
        g_ctx = NULL;
    }
    
    g_map_count = 0;
    g_state = MAP_SELECT_STATE_INIT;
    g_in_map_select = false;
}
