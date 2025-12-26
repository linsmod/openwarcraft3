#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"

// UI 按钮状态
typedef enum {
    UI_BUTTON_STATE_NORMAL,    // 正常状态
    UI_BUTTON_STATE_HOVER,     // 鼠标悬停
    UI_BUTTON_STATE_PRESSED,   // 按下状态
    UI_BUTTON_STATE_DISABLED   // 禁用状态
} ui_button_state_t;

// UI 按钮点击回调
typedef void (*ui_button_click_callback_t)(void *user_data);

// UI 按钮配置
typedef struct {
    float x;                          // 按钮 X 位置
    float y;                          // 按钮 Y 位置
    float width;                      // 按钮宽度
    float height;                     // 按钮高度
    char text[128];                   // 按钮文本
    COLOR32 bg_color[4];              // 背景色 [正常, 悬停, 按下, 禁用]
    COLOR32 border_color[4];         // 边框色 [正常, 悬停, 按下, 禁用]
    COLOR32 text_color[4];            // 文本颜色 [正常, 悬停, 按下, 禁用]
    float border_width;               // 边框宽度
    ui_button_click_callback_t on_click;  // 点击回调函数
    void *user_data;                  // 用户数据
    bool enabled;                     // 是否启用
    bool visible;                     // 是否可见 (默认 true)
} ui_button_config_t;

// UI 按钮状态
typedef struct {
    ui_button_state_t state;
    ui_button_config_t config;
    bool is_hovered;
    bool is_pressed;
    canvas2d_context_t *ctx;
} ui_button_t;

// 创建默认按钮配置
ui_button_config_t UIButton_GetDefaultConfig(void);

// 初始化 UI 按钮
int UIButton_Init(ui_button_t *button, const ui_button_config_t *config, canvas2d_context_t *ctx);

// 设置按钮文本
void UIButton_SetText(ui_button_t *button, const char *text);

// 设置按钮位置
void UIButton_SetPosition(ui_button_t *button, float x, float y);

// 设置按钮大小
void UIButton_SetSize(ui_button_t *button, float width, float height);

// 设置按钮启用状态
void UIButton_SetEnabled(ui_button_t *button, bool enabled);

// 获取按钮启用状态
bool UIButton_IsEnabled(const ui_button_t *button);

// 检查点是否在按钮区域内
bool UIButton_IsPointInButton(const ui_button_t *button, float x, float y);

// 处理鼠标移动（悬停检测）
bool UIButton_HandleMouseMove(ui_button_t *button, float x, float y);

// 处理鼠标点击
bool UIButton_HandleMouseClick(ui_button_t *button, float x, float y, bool down);

// 更新按钮（处理动画等）
void UIButton_Update(ui_button_t *button, int msec);

// 渲染按钮
void UIButton_Render(ui_button_t *button);

// 清理按钮
void UIButton_Shutdown(ui_button_t *button);

#endif // __UI_BUTTON_H__
