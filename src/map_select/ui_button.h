#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"
#include "ui_text.h"

// UI 按钮状态
typedef enum {
    UI_BUTTON_STATE_NORMAL,    // 正常状态
    UI_BUTTON_STATE_HOVER,     // 鼠标悬停
    UI_BUTTON_STATE_PRESSED,   // 按下状态
    UI_BUTTON_STATE_DISABLED   // 禁用状态
} ui_button_state_t;

// UI 按钮配置
typedef struct {
    char text[128];                   // 按钮文本
    UICOLOR32 bg_colors;              // 背景色 {normal, hover, active, disabled}
    UICOLOR32 border_colors;          // 边框色 {normal, hover, active, disabled}
    UICOLOR32 text_colors;            // 文本颜色 {normal, hover, active, disabled}
    float font_size;                  // 字体大小
    float border_width;               // 边框宽度
    bool double_click_enabled;        // 是否启用双击
} ui_button_config_t;

// UI 按钮组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件
    ui_button_state_t state;         // 按钮状态
    ui_button_config_t config;       // 按钮配置
    ui_text_t text_component;        // 文本组件
} ui_button_t;

// 创建默认按钮配置
ui_button_config_t UIButton_GetDefaultConfig(void);

// 创建按钮（简单版）
ui_button_t* UIButton_Create(float x, float y, float width, float height, canvas2d_context_t *ctx);

// 创建按钮（带配置）
ui_button_t* UIButton_CreateWithConfig(float x, float y, float width, float height,
                                      const ui_button_config_t *config, canvas2d_context_t *ctx);

// 初始化按钮
int UIButton_Init(ui_button_t *button, const ui_button_config_t *config, canvas2d_context_t *ctx);

// 清理按钮
void UIButton_Shutdown(ui_button_t *button);

// 销毁按钮
void UIButton_Destroy(ui_button_t *button);

// 设置按钮文本
void UIButton_SetText(ui_button_t *button, const char *text);

// 获取按钮文本
const char* UIButton_GetText(ui_button_t *button);

// 设置按钮位置
void UIButton_SetPosition(ui_button_t *button, float x, float y);

// 设置按钮大小
void UIButton_SetSize(ui_button_t *button, float width, float height);

// 设置按钮启用状态
void UIButton_SetEnabled(ui_button_t *button, bool enabled);

// 获取按钮启用状态
bool UIButton_IsEnabled(const ui_button_t *button);

// 获取按钮状态
ui_button_state_t UIButton_GetState(ui_button_t *button);

// 添加单击事件处理器
bool UIButton_AddOnClick(ui_button_t *button, ui_event_handler_t handler, void *user_data);

// 添加双击事件处理器
bool UIButton_AddOnDoubleClick(ui_button_t *button, ui_event_handler_t handler, void *user_data);

// 添加鼠标悬停事件处理器
bool UIButton_AddOnMouseEnter(ui_button_t *button, ui_event_handler_t handler, void *user_data);

// 添加鼠标离开事件处理器
bool UIButton_AddOnMouseLeave(ui_button_t *button, ui_event_handler_t handler, void *user_data);

// 更新按钮
void UIButton_Update(ui_button_t *button, int msec);

// 渲染按钮
void UIButton_Render(ui_button_t *button);

#endif // __UI_BUTTON_H__
