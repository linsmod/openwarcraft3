#ifndef __UI_LIST_ITEM_H__
#define __UI_LIST_ITEM_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"

// UI 列表项内容绘制回调（只负责绘制内容区域，边框和背景由item自己绘制）
typedef void (*ui_list_item_draw_callback_t)(
    ui_component_t *item,
    const char *text,
    void *user_data,
    float x,      // 内容区域x坐标（已去除边框和内边距）
    float y,      // 内容区域y坐标
    float width,  // 内容区域宽度
    float height, // 内容区域高度
    bool selected,
    bool hovered
);

// 内边距常量
#define UI_LIST_ITEM_PADDING_LEFT 3
#define UI_LIST_ITEM_PADDING_RIGHT 3
#define UI_LIST_ITEM_PADDING_TOP 0
#define UI_LIST_ITEM_PADDING_BOTTOM 0
#define UI_LIST_ITEM_BORDER_WIDTH 1

// UI 列表项组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件
    char text[256];                   // 显示文本
    void *user_data;                  // 项的用户数据
    bool selected;                    // 是否被选中
    bool hovered;                     // 是否被悬停

    // 颜色配置（可选，如果不设置则使用父列表的默认颜色）
    bool use_custom_colors;           // 是否使用自定义颜色
    UICOLOR32 bg_colors;             // 背景色 {normal, hover, active, disabled}
    UICOLOR32 text_colors;            // 文本色 {normal, hover, active, disabled}

    // 自定义绘制回调（可选）
    ui_list_item_draw_callback_t draw_callback;
    void *draw_callback_user_data;

    // 字体配置
    float font_size;                  // 字体大小
    int font_flags;                   // 字体标志（加粗、斜体等）
} ui_list_item_t;

// 创建列表项（简单版）
ui_list_item_t* UIListItem_Create(const char *text, void *user_data, canvas2d_context_t *ctx);

// 创建列表项（带配置）
ui_list_item_t* UIListItem_CreateEx(const char *text, void *user_data, float font_size,
                                   const UICOLOR32 *bg_colors, const UICOLOR32 *text_colors,
                                   canvas2d_context_t *ctx);

// 初始化列表项
int UIListItem_Init(ui_list_item_t *item, const char *text, void *user_data,
                   float font_size, canvas2d_context_t *ctx);

// 清理列表项
void UIListItem_Shutdown(ui_list_item_t *item);

// 销毁列表项
void UIListItem_Destroy(ui_list_item_t *item);

// 设置文本
void UIListItem_SetText(ui_list_item_t *item, const char *text);

// 获取文本
const char* UIListItem_GetText(const ui_list_item_t *item);

// 设置用户数据
void UIListItem_SetUserData(ui_list_item_t *item, void *user_data);

// 获取用户数据
void* UIListItem_GetUserData(const ui_list_item_t *item);

// 设置选中状态
void UIListItem_SetSelected(ui_list_item_t *item, bool selected);

// 获取选中状态
bool UIListItem_IsSelected(const ui_list_item_t *item);

// 设置悬停状态
void UIListItem_SetHovered(ui_list_item_t *item, bool hovered);

// 获取悬停状态
bool UIListItem_IsHovered(const ui_list_item_t *item);

// 设置自定义颜色
void UIListItem_SetColors(ui_list_item_t *item, const UICOLOR32 *bg_colors, const UICOLOR32 *text_colors);

// 设置背景颜色
void UIListItem_SetBgColors(ui_list_item_t *item, const UICOLOR32 *colors);

// 设置文本颜色
void UIListItem_SetTextColors(ui_list_item_t *item, const UICOLOR32 *colors);

// 设置字体大小
void UIListItem_SetFontSize(ui_list_item_t *item, float font_size);

// 获取字体大小
float UIListItem_GetFontSize(const ui_list_item_t *item);

// 设置自定义绘制回调
void UIListItem_SetDrawCallback(ui_list_item_t *item, ui_list_item_draw_callback_t callback, void *user_data);

// 更新列表项
void UIListItem_Update(ui_list_item_t *item, int msec);

// 渲染列表项
void UIListItem_Render(ui_list_item_t *item);

#endif // __UI_LIST_ITEM_H__
