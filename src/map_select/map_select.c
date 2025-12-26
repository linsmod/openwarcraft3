#include "map_select.h"
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
static browser_item_t g_browser_items[MAX_MAPS];
static int g_browser_count = 0;
static browser_item_t g_all_items[MAX_MAPS];  // 保存所有文件的原始列表
static int g_all_count = 0;
static int g_selected_index = 0;
static int g_scroll_offset = 0;
static map_select_state_t g_state = MAP_SELECT_STATE_INIT;

// 当前浏览的路径
static char g_current_path[MAX_PATH_LEN] = "";
static bool g_at_root = true;

// Canvas2D 画布
static canvas2d_t *g_canvas = NULL;
static canvas2d_context_t *g_ctx = NULL;

// 全局标志：是否在地图选择模式
bool g_in_map_select = false;

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
    g_browser_count = 0;
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
            
            // 同时保存到浏览器列表（初始时两者相同）
            if (g_browser_count < MAX_MAPS) {
                g_browser_items[g_browser_count] = *all_item;
                g_browser_count++;
            }
        }
        
        line_start = line_end + 1;
    }
    
    MemFree(listfile_content);
    
    printf("Loaded %d items from (listfile)\n", g_browser_count);
    return g_browser_count;
}

// 过滤显示当前路径下的文件
static void FilterCurrentPath(void) {
    static browser_item_t temp_items[MAX_MAPS];
    int filtered_count = 0;
    
    // 添加 ".." 目录（如果不是在根目录）
    if (!g_at_root && filtered_count < MAX_MAPS) {
        strcpy(temp_items[filtered_count].name, "..");
        strcpy(temp_items[filtered_count].full_path, g_current_path);
        temp_items[filtered_count].type = ITEM_TYPE_FOLDER;
        filtered_count++;
    }
    
    for (int i = 0; i < g_all_count && filtered_count < MAX_MAPS; i++) {
        const char *item_path = g_all_items[i].full_path;
        
        if (g_at_root) {
            // 根目录：显示顶级文件夹和根目录下的地图文件
            const char *first_sep = strchr(item_path, '\\');
            
            if (!first_sep) {
                // 根目录下的文件
                if (IsMapFile(item_path)) {
                    temp_items[filtered_count] = g_all_items[i];
                    filtered_count++;
                }
            } else {
                // 顶级文件夹
                char folder_name[256];
                strncpy(folder_name, item_path, first_sep - item_path);
                folder_name[first_sep - item_path] = '\0';
                
                bool exists = false;
                for (int j = 0; j < filtered_count; j++) {
                    if (temp_items[j].type == ITEM_TYPE_FOLDER && 
                        strcmp(temp_items[j].name, folder_name) == 0) {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists) {
                    strcpy(temp_items[filtered_count].name, folder_name);
                    strncpy(temp_items[filtered_count].full_path, item_path, 
                            first_sep - item_path);
                    temp_items[filtered_count].full_path[first_sep - item_path] = '\0';
                    temp_items[filtered_count].type = ITEM_TYPE_FOLDER;
                    filtered_count++;
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
                    for (int j = 0; j < filtered_count; j++) {
                        if (temp_items[j].type == ITEM_TYPE_FOLDER && 
                            strcmp(temp_items[j].name, folder_name) == 0) {
                            exists = true;
                            break;
                        }
                    }
                    
                    if (!exists) {
                        strcpy(temp_items[filtered_count].name, folder_name);
                        char new_path[MAX_PATH_LEN];
                        strncpy(new_path, item_path, 
                                path_len + (next_sep - remaining) + 1);
                        new_path[path_len + (next_sep - remaining) + 1] = '\0';
                        strcpy(temp_items[filtered_count].full_path, new_path);
                        temp_items[filtered_count].type = ITEM_TYPE_FOLDER;
                        filtered_count++;
                    }
                } else if (IsMapFile(item_path)) {
                    // 地图文件
                    temp_items[filtered_count] = g_all_items[i];
                    filtered_count++;
                }
            }
        }
    }
    
    // 复制结果回原数组
    for (int i = 0; i < filtered_count; i++) {
        g_browser_items[i] = temp_items[i];
    }
    g_browser_count = filtered_count;
    
    // 排序：文件夹在前，文件在后
    for (int i = 0; i < g_browser_count - 1; i++) {
        for (int j = i + 1; j < g_browser_count; j++) {
            if (temp_items[i].type > temp_items[j].type ||
                (temp_items[i].type == temp_items[j].type &&
                 strcmp(temp_items[i].name, temp_items[j].name) > 0)) {
                browser_item_t temp = temp_items[i];
                temp_items[i] = temp_items[j];
                temp_items[j] = temp;
            }
        }
    }
    
    // 复制排序后的结果
    for (int i = 0; i < filtered_count; i++) {
        g_browser_items[i] = temp_items[i];
    }
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
    
    // 从(listfile)加载地图列表
    g_map_count = 0;
    g_browser_count = 0;
    
    int loaded = MapSelect_LoadMapsFromListfile();
    if (loaded == 0) {
        printf("No maps found in (listfile), using test data\n");
        // 添加测试数据
        strcpy(g_browser_items[0].name, "Human01.w3m");
        strcpy(g_browser_items[0].full_path, "Maps\\Campaign\\Human01.w3m");
        g_browser_items[0].type = ITEM_TYPE_MAP_W3M;
        g_browser_count++;
        
        strcpy(g_browser_items[1].name, "Human02.w3m");
        strcpy(g_browser_items[1].full_path, "Maps\\Campaign\\Human02.w3m");
        g_browser_items[1].type = ITEM_TYPE_MAP_W3M;
        g_browser_count++;
    } else {
        // 过滤到根目录
        g_at_root = true;
        g_current_path[0] = '\0';
        FilterCurrentPath();
    }
    
    g_state = MAP_SELECT_STATE_LIST;
    g_selected_index = 0;
    g_scroll_offset = 0;
    g_in_map_select = true;
    
    printf("Map Selection Screen initialized with %d items\n", g_browser_count);
    return 0;
}

// 更新地图选择界面
void MapSelect_Update(int msec) {
    if (g_state == MAP_SELECT_STATE_DONE) {
        return;
    }
}

// 绘制浏览器项
static void DrawBrowserItem(int index, float x, float y, float width, float height, bool selected) {
    if (index >= g_browser_count) return;
    
    browser_item_t *item = &g_browser_items[index];
    
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
    if (item->type == ITEM_TYPE_MAP_W3M) {
        strcpy(icon, "[3]");
    } else if (item->type == ITEM_TYPE_MAP_W3X) {
        strcpy(icon, "[X]");
    }
    
    canvas2d_set_fill_style(g_ctx, (COLOR32){255, 255, 255, 255});
    canvas2d_fill_text(g_ctx, icon, x + 10, y + 10);
    
    // 文件名
    canvas2d_fill_text(g_ctx, item->name, x + 50, y + 10);
}

// 绘制地图预览信息
static void DrawMapPreview(float x, float y, float width, float height) {
    if (g_selected_index >= g_browser_count) return;
    
    browser_item_t *item = &g_browser_items[g_selected_index];
    
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
    if (g_selected_index >= g_browser_count) return;
    
    browser_item_t *item = &g_browser_items[g_selected_index];
    
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
    
    // 文件列表区域
    float list_x = 50;
    float list_y = 100;
    float item_width = 400;
    float item_height = 50;
    float item_spacing = 5;
    
    // 计算可见的项数量
    int visible_items = (768 - list_y) / (item_height + item_spacing);
    if (visible_items > g_browser_count - g_scroll_offset) {
        visible_items = g_browser_count - g_scroll_offset;
    }
    
    // 绘制文件列表
    for (int i = 0; i < visible_items; i++) {
        int item_index = g_scroll_offset + i;
        float y = list_y + i * (item_height + item_spacing);
        DrawBrowserItem(item_index, list_x, y, item_width, item_height, 
                     item_index == g_selected_index);
    }
    
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
    canvas2d_fill_text(g_ctx, "Use UP/DOWN arrows to navigate, ENTER to select/open", 50, 720);
    canvas2d_fill_text(g_ctx, "ESC to quit", 900, 720);
    
    // 显示滚动信息
    if (g_browser_count > visible_items) {
        char scroll_info[64];
        sprintf(scroll_info, "%d-%d / %d", 
                g_scroll_offset + 1, 
                g_scroll_offset + visible_items,
                g_browser_count);
        canvas2d_fill_text(g_ctx, scroll_info, 50 + item_width - 100, 720);
    }
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
    
    // 重置选择和滚动
    g_selected_index = 0;
    g_scroll_offset = 0;
}

// 处理输入事件
bool MapSelect_HandleInput(int key, bool down) {
    if (g_state == MAP_SELECT_STATE_DONE) return false;
    
    if (down) {
        switch (key) {
            case SDLK_UP:
                if (g_selected_index > 0) {
                    g_selected_index--;
                    if (g_selected_index < g_scroll_offset) {
                        g_scroll_offset = g_selected_index;
                    }
                }
                return true;
                
            case SDLK_DOWN:
                if (g_selected_index < g_browser_count - 1) {
                    g_selected_index++;
                    // 检查是否需要滚动
                    float item_height = 50;
                    float item_spacing = 5;
                    float list_y = 100;
                    int visible_items = (768 - list_y) / (item_height + item_spacing);
                    if (g_selected_index >= g_scroll_offset + visible_items) {
                        g_scroll_offset = g_selected_index - visible_items + 1;
                    }
                }
                return true;
                
            case SDLK_RETURN:
                if (g_browser_count > 0) {
                    browser_item_t *item = &g_browser_items[g_selected_index];
                    
                    if (item->type == ITEM_TYPE_FOLDER) {
                        // 进入文件夹
                        EnterFolder(item->name);
                    } else {
                        // 选择地图
                        printf("Selected map: %s\n", item->full_path);
                        g_state = MAP_SELECT_STATE_DONE;
                    }
                }
                return true;
                
            case SDLK_ESCAPE:
                // 退出游戏
                return true;
        }
    }
    
    return false;
}

// 获取选中的地图
const char* MapSelect_GetSelectedMap(void) {
    if (g_selected_index < g_browser_count) {
        browser_item_t *item = &g_browser_items[g_selected_index];
        if (item->type == ITEM_TYPE_MAP_W3M || item->type == ITEM_TYPE_MAP_W3X) {
            return item->full_path;
        }
    }
    return NULL;
}

// 清理地图选择界面
void MapSelect_Shutdown(void) {
    printf("Shutting down Map Selection Screen...\n");
    
    if (g_canvas) {
        canvas2d_destroy(g_canvas);
        g_canvas = NULL;
        g_ctx = NULL;
    }
    
    g_map_count = 0;
    g_browser_count = 0;
    g_state = MAP_SELECT_STATE_INIT;
    g_in_map_select = false;
}
