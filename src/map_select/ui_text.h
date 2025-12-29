#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"

// UI 文本对齐方式
typedef enum {
    UI_TEXT_ALIGN_LEFT,       // 左对齐
    UI_TEXT_ALIGN_CENTER,     // 居中
    UI_TEXT_ALIGN_RIGHT        // 右对齐
} ui_text_align_t;

// UI 文本垂直对齐方式
typedef enum {
    UI_TEXT_VALIGN_TOP,       // 顶部对齐
    UI_TEXT_VALIGN_MIDDLE,    // 垂直居中
    UI_TEXT_VALIGN_BOTTOM     // 底部对齐
} ui_text_valign_t;

// UI 文本组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件
    char text[512];                  // 文本内容
    COLOR32 color;                   // 文本颜色
    float font_size;                 // 字体大小
    ui_text_align_t align;           // 水平对齐方式
    ui_text_valign_t valign;         // 垂直对齐方式
    bool wrap;                       // 是否自动换行
    float wrap_width;                // 换行宽度（仅在wrap=true时有效）
} ui_text_t;

// 创建默认文本配置
ui_text_t* UIText_Create(float x, float y, const char *text, COLOR32 color, float font_size,
                        ui_text_align_t align, ui_text_valign_t valign, canvas2d_context_t *ctx);

// 初始化 UI 文本
int UIText_Init(ui_text_t *text, canvas2d_context_t *ctx);

// 设置文本内容
void UIText_SetText(ui_text_t *text, const char *content);

// 设置文本位置
void UIText_SetPosition(ui_text_t *text, float x, float y);

// 设置文本颜色
void UIText_SetColor(ui_text_t *text, COLOR32 color);

// 设置字体大小
void UIText_SetFontSize(ui_text_t *text, float font_size);

// 设置对齐方式
void UIText_SetAlign(ui_text_t *text, ui_text_align_t align, ui_text_valign_t valign);

// 设置换行选项
void UIText_SetWrap(ui_text_t *text, bool wrap, float wrap_width);

// 计算文本渲染位置
void UIText_CalcPosition(const ui_text_t *text, float *out_x, float *out_y);

// 更新文本
void UIText_Update(ui_text_t *text, int msec);

// 渲染文本
void UIText_Render(ui_text_t *text);

// 清理文本
void UIText_Shutdown(ui_text_t *text);

// 销毁文本
void UIText_Destroy(ui_text_t *text);

#endif // __UI_TEXT_H__
