#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"

// UI 标签对齐方式
typedef enum {
    UI_LABEL_ALIGN_LEFT,       // 左对齐
    UI_LABEL_ALIGN_CENTER,     // 居中
    UI_LABEL_ALIGN_RIGHT        // 右对齐
} ui_label_align_t;

// UI 标签垂直对齐方式
typedef enum {
    UI_LABEL_VALIGN_TOP,       // 顶部对齐
    UI_LABEL_VALIGN_MIDDLE,    // 垂直居中
    UI_LABEL_VALIGN_BOTTOM     // 底部对齐
} ui_label_valign_t;

// UI 标签组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件
    char text[512];                  // 文本内容
    COLOR32 text_color;               // 文本颜色
    COLOR32 bg_color;                 // 背景颜色（可选，alpha=0表示透明）
    float font_size;                 // 字体大小
    ui_label_align_t align;          // 水平对齐方式
    ui_label_valign_t valign;        // 垂直对齐方式
    bool has_background;              // 是否显示背景
} ui_label_t;

// 创建标签（指定位置和大小）
ui_label_t* UILabel_Create(float width, float height, const char *text,
                          COLOR32 text_color, float font_size,
                          ui_label_align_t align, ui_label_valign_t valign,
                          canvas2d_context_t *ctx);

// 创建带背景的标签
ui_label_t* UILabel_CreateWithBackground(float width, float height, const char *text,
                                         COLOR32 text_color, COLOR32 bg_color, float font_size,
                                         ui_label_align_t align, ui_label_valign_t valign,
                                         canvas2d_context_t *ctx);

// 初始化标签
int UILabel_Init(ui_label_t *label, canvas2d_context_t *ctx);

// 设置文本内容
void UILabel_SetText(ui_label_t *label, const char *content);

// 设置文本颜色
void UILabel_SetTextColor(ui_label_t *label, COLOR32 color);

// 设置背景颜色
void UILabel_SetBgColor(ui_label_t *label, COLOR32 color);

// 设置是否显示背景
void UILabel_SetBackgroundVisible(ui_label_t *label, bool visible);

// 设置字体大小
void UILabel_SetFontSize(ui_label_t *label, float font_size);

// 设置对齐方式
void UILabel_SetAlign(ui_label_t *label, ui_label_align_t align, ui_label_valign_t valign);

// 设置位置和大小
void UILabel_SetBounds(ui_label_t *label, float x, float y, float width, float height);

// 更新标签
void UILabel_Update(ui_label_t *label, int msec);

// 渲染标签
void UILabel_Render(ui_label_t *label);

// 清理标签
void UILabel_Shutdown(ui_label_t *label);

// 销毁标签
void UILabel_Destroy(ui_label_t *label);

#endif // __UI_LABEL_H__
