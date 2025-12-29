#ifndef __UI_INPUT_H__
#define __UI_INPUT_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"
#include "ui_text.h"

// UI 输入框组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;           // 基础组件
    char text[256];                 // 文本内容
    char placeholder[256];          // 占位符文本
    COLOR32 text_color;             // 文本颜色
    COLOR32 placeholder_color;      // 占位符颜色
    COLOR32 bg_color;               // 背景颜色
    COLOR32 border_color;           // 边框颜色
    COLOR32 focus_border_color;     // 焦点时边框颜色
    COLOR32 selection_bg_color;     // 选中背景颜色
    float font_size;                // 字体大小
int cursor_pos;                 // 光标位置
    int selection_start;            // 选中开始位置
    int selection_end;              // 选中结束位置
    int scroll_offset;             // 水平滚动偏移
    bool focused;                   // 是否获得焦点
    bool readonly;                   // 是否只读
    bool password;                  // 是否密码模式
    char password_char;             // 密码显示字符（如 '*'）
    int max_length;                 // 最大长度
    ui_text_align_t align;          // 文本对齐方式
    
    // 光标闪烁相关
    bool cursor_blink_visible;      // 光标是否可见
    int cursor_blink_timer;         // 闪烁计时器
    int cursor_blink_interval;      // 闪烁间隔（毫秒）
} ui_input_t;

// 创建输入框
ui_input_t* UIInput_Create(float x, float y, float width, float height, float font_size,
                          const char *placeholder, canvas2d_context_t *ctx);

// 初始化输入框
int UIInput_Init(ui_input_t *input, canvas2d_context_t *ctx);

// 设置文本内容
void UIInput_SetText(ui_input_t *input, const char *text);

// 获取文本内容
const char* UIInput_GetText(ui_input_t *input);

// 设置占位符
void UIInput_SetPlaceholder(ui_input_t *input, const char *placeholder);

// 设置文本颜色
void UIInput_SetTextColor(ui_input_t *input, COLOR32 color);

// 设置背景颜色
void UIInput_SetBgColor(ui_input_t *input, COLOR32 color);

// 设置边框颜色
void UIInput_SetBorderColor(ui_input_t *input, COLOR32 color);

// 设置焦点边框颜色
void UIInput_SetFocusBorderColor(ui_input_t *input, COLOR32 color);

// 设置字体大小
void UIInput_SetFontSize(ui_input_t *input, float font_size);

// 设置是否只读
void UIInput_SetReadOnly(ui_input_t *input, bool readonly);

// 设置是否密码模式
void UIInput_SetPassword(ui_input_t *input, bool password, char password_char);

// 设置最大长度
void UIInput_SetMaxLength(ui_input_t *input, int max_length);

// 设置对齐方式
void UIInput_SetAlign(ui_input_t *input, ui_text_align_t align);

// 更新输入框
void UIInput_Update(ui_input_t *input, int msec);

// 渲染输入框
void UIInput_Render(ui_input_t *input);

// 清理输入框
void UIInput_Shutdown(ui_input_t *input);

// 销毁输入框
void UIInput_Destroy(ui_input_t *input);

// 处理焦点状态
void UIInput_SetFocused(ui_input_t *input, bool focused);
bool UIInput_IsFocused(ui_input_t *input);

// 清除文本
void UIInput_Clear(ui_input_t *input);

#endif // __UI_INPUT_H__
