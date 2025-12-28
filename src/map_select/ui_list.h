#ifndef __UI_LIST_H__
#define __UI_LIST_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"

// 最大列表项数量
#define UI_LIST_MAX_ITEMS 500

// UI 列表项绘制回调
typedef void (*ui_list_item_draw_callback_t)(
    int index, 
    const char *text, 
    void *user_data,
    float x, 
    float y, 
    float width, 
    float height, 
    bool selected
);

// UI 列表配置
typedef struct {
    float x;                          // 列表 X 位置
    float y;                          // 列表 Y 位置
    float width;                      // 列表宽度
    float height;                     // 列表高度
    float item_height;                // 单项高度
    float item_spacing;               // 项间距
    float font_size;                  // 字体大小
    COLOR32 bg_color;                 // 背景色
    COLOR32 selected_bg_color;        // 选中背景色
    COLOR32 border_color;             // 边框色
    COLOR32 text_color;               // 文本颜色
    COLOR32 selected_text_color;      // 选中文本颜色
    bool show_scrollbar;              // 是否显示滚动条
    ui_list_item_draw_callback_t draw_callback;  // 自定义绘制回调
    void *user_data;                  // 用户数据
} ui_list_config_t;

// UI 列表项
typedef struct {
    char text[256];                   // 显示文本
    void *user_data;                  // 项的用户数据
} ui_list_item_t;

// UI 列表状态
typedef struct {
    ui_list_item_t items[UI_LIST_MAX_ITEMS];
    int item_count;
    int selected_index;
    int scroll_offset;
    int visible_count;
    float scroll_pos;
    bool is_scrolling;
    ui_list_config_t config;
    canvas2d_context_t *ctx;
} ui_list_t;

// 初始化 UI 列表
int UIList_Init(ui_list_t *list, const ui_list_config_t *config, canvas2d_context_t *ctx);

// 添加列表项
int UIList_AddItem(ui_list_t *list, const char *text, void *user_data);

// 清空列表项
void UIList_ClearItems(ui_list_t *list);

// 设置选中项
void UIList_SetSelected(ui_list_t *list, int index);

// 获取选中项
int UIList_GetSelected(ui_list_t *list);

// 获取选中项文本
const char* UIList_GetSelectedText(ui_list_t *list);

// 获取选中项用户数据
void* UIList_GetSelectedUserData(ui_list_t *list);

// 获取列表项数量
int UIList_GetItemCount(ui_list_t *list);

// 设置滚动位置
void UIList_SetScrollOffset(ui_list_t *list, int offset);

// 获取滚动位置
int UIList_GetScrollOffset(ui_list_t *list);

// 处理键盘输入（上下键导航）
bool UIList_HandleInput(ui_list_t *list, int key, bool down);

// 处理鼠标点击
bool UIList_HandleMouseClick(ui_list_t *list, int x, int y);

// 处理鼠标滚轮
bool UIList_HandleMouseWheel(ui_list_t *list, int delta);

// 更新列表（处理滚动动画等）
void UIList_Update(ui_list_t *list, int msec);

// 渲染列表
void UIList_Render(ui_list_t *list);

// 绘制滚动条
static void UIList_DrawScrollbar(ui_list_t *list);

// 清理列表
void UIList_Shutdown(ui_list_t *list);

#endif // __UI_LIST_H__
