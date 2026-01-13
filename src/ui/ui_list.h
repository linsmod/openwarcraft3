#ifndef __UI_LIST_H__
#define __UI_LIST_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "../common/scene.h"
#include "ui_component.h"
#include "ui_list_item.h"

// 前向声明
typedef struct ui_list_t ui_list_t;

// 列表选中项改变回调函数类型
typedef void (*ui_list_on_selected_changed_t)(ui_list_t *list, int index, void *user_data);

// 最大列表项数量
#define UI_LIST_MAX_ITEMS 500

// UI 列表组件（继承自 ui_component_t）
struct ui_list_t {
    ui_component_t base;              // 基础组件（包含bg_color）
    scene_manager_t *scene_manager;   // 场景管理器（用于鼠标捕获）
    ui_list_item_t *items[UI_LIST_MAX_ITEMS];  // 列表项指针数组
    ui_list_on_selected_changed_t on_selected_changed;  // 选中项改变回调
    void *callback_user_data;         // 回调用户数据
    int item_count;
    int selected_index;
    int pending_selected_index;       // 待触发的选中索引（用于update中回调）
    int scroll_offset;
    int visible_count;
    float scroll_pos;
    float item_height;                // 单项高度
    float item_spacing;               // 项间距
    float font_size;                  // 字体大小
    COLOR32 border_color;             // 边框色
    bool show_scrollbar;              // 是否显示滚动条
    bool is_dragging_scrollbar;       // 是否正在拖动滚动条
    float scrollbar_drag_start_y;      // 滚动条拖动起始Y坐标
    int scrollbar_drag_start_offset;   // 滚动条拖动起始偏移量
};

// 创建默认列表配置
ui_list_t* UIList_Create(float width, float height,
                        float item_height, float font_size, canvas2d_context_t *ctx);

// 初始化 UI 列表
int UIList_Init(ui_list_t *list, canvas2d_context_t *ctx);

// 设置场景管理器（用于鼠标捕获功能）
void UIList_SetSceneManager(ui_list_t *list, scene_manager_t *manager);

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

// 获取列表项
ui_list_item_t* UIList_GetItem(ui_list_t *list, int index);

// 设置默认item样式
void UIList_SetDefaultItemStyle(ui_list_t *list, float font_size);

// 设置选中项改变回调函数
void UIList_SetSelectedChangedCallback(ui_list_t *list, ui_list_on_selected_changed_t callback, void *user_data);

// 更新列表
void UIList_Update(ui_list_t *list, int msec);

// 渲染列表
void UIList_Render(ui_list_t *list);

// 清理列表
void UIList_Shutdown(ui_list_t *list);

// 销毁列表
void UIList_Destroy(ui_list_t *list);

#endif // __UI_LIST_H__
