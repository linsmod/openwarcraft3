#include "ui_button.h"
#include <string.h>
#include <stdio.h>

// 创建默认按钮配置
ui_button_config_t UIButton_GetDefaultConfig(void) {
    ui_button_config_t config = {
        .x = 0.0f,
        .y = 0.0f,
        .width = 100.0f,
        .height = 30.0f,
        .text = "",
        .bg_color = {
            {60, 60, 70, 255},      // 正常
            {80, 80, 90, 255},      // 悬停
            {50, 50, 60, 255},      // 按下
            {40, 40, 50, 200}       // 禁用
        },
        .border_color = {
            {200, 200, 200, 255},  // 正常
            {220, 220, 220, 255},  // 悬停
            {180, 180, 180, 255},  // 按下
            {100, 100, 100, 255}   // 禁用
        },
        .text_color = {
            {255, 255, 255, 255},  // 正常
            {255, 255, 255, 255},  // 悬停
            {255, 255, 255, 255},  // 按下
            {180, 180, 180, 255}   // 禁用
        },
        .border_width = 1.0f,
        .on_click = NULL,
        .user_data = NULL,
        .enabled = true,
        .visible = true
    };
    return config;
}

// 初始化 UI 按钮
int UIButton_Init(ui_button_t *button, const ui_button_config_t *config, canvas2d_context_t *ctx) {
    if (!button || !config || !ctx) {
        return -1;
    }

    memset(button, 0, sizeof(ui_button_t));
    button->config = *config;
    button->ctx = ctx;
    button->state = UI_BUTTON_STATE_NORMAL;
    button->is_hovered = false;
    button->is_pressed = false;

    printf("UIButton initialized: text='%s', pos=(%.1f,%.1f), size=(%.1fx%.1f)\n",
           config->text, config->x, config->y, config->width, config->height);

    return 0;
}

// 设置按钮文本
void UIButton_SetText(ui_button_t *button, const char *text) {
    if (!button || !text) return;
    strncpy(button->config.text, text, 127);
    button->config.text[127] = '\0';
}

// 设置按钮位置
void UIButton_SetPosition(ui_button_t *button, float x, float y) {
    if (!button) return;
    button->config.x = x;
    button->config.y = y;
}

// 设置按钮大小
void UIButton_SetSize(ui_button_t *button, float width, float height) {
    if (!button) return;
    button->config.width = width;
    button->config.height = height;
}

// 设置按钮启用状态
void UIButton_SetEnabled(ui_button_t *button, bool enabled) {
    if (!button) return;
    button->config.enabled = enabled;
    if (!enabled) {
        button->state = UI_BUTTON_STATE_DISABLED;
        button->is_hovered = false;
        button->is_pressed = false;
    } else if (button->state == UI_BUTTON_STATE_DISABLED) {
        button->state = UI_BUTTON_STATE_NORMAL;
    }
}

// 获取按钮启用状态
bool UIButton_IsEnabled(const ui_button_t *button) {
    if (!button) return false;
    return button->config.enabled;
}

// 检查点是否在按钮区域内
bool UIButton_IsPointInButton(const ui_button_t *button, float x, float y) {
    if (!button) return false;
    const ui_button_config_t *cfg = &button->config;
    return x >= cfg->x && x < cfg->x + cfg->width &&
           y >= cfg->y && y < cfg->y + cfg->height;
}

// 处理鼠标移动（悬停检测）
bool UIButton_HandleMouseMove(ui_button_t *button, float x, float y) {
    if (!button || !button->config.enabled) return false;

    bool was_hovered = button->is_hovered;
    button->is_hovered = UIButton_IsPointInButton(button, x, y);

    // 更新状态
    if (button->is_hovered) {
        if (!button->is_pressed) {
            button->state = UI_BUTTON_STATE_HOVER;
        } else {
            button->state = UI_BUTTON_STATE_PRESSED;
        }
    } else {
        if (!button->is_pressed) {
            button->state = UI_BUTTON_STATE_NORMAL;
        }
    }

    return button->is_hovered != was_hovered;
}

// 处理鼠标点击
bool UIButton_HandleMouseClick(ui_button_t *button, float x, float y, bool down) {
    if (!button || !button->config.enabled) return false;

    bool was_hovered = button->is_hovered;
    button->is_hovered = UIButton_IsPointInButton(button, x, y);

    if (button->is_hovered) {
        if (down) {
            button->is_pressed = true;
            button->state = UI_BUTTON_STATE_PRESSED;
        } else {
            // 鼠标释放时，如果之前按下且仍在按钮内，触发点击事件
            if (button->is_pressed) {
                button->is_pressed = false;
                button->state = UI_BUTTON_STATE_HOVER;
                
                // 触发点击回调
                if (button->config.on_click) {
                    button->config.on_click(button->config.user_data);
                }
                return true;
            } else {
                button->state = UI_BUTTON_STATE_HOVER;
            }
        }
    } else {
        if (down) {
            button->is_pressed = false;
        } else {
            button->is_pressed = false;
        }
        button->state = UI_BUTTON_STATE_NORMAL;
    }

    return false;
}

// 更新按钮
void UIButton_Update(ui_button_t *button, int msec) {
    if (!button) return;
    // 这里可以添加按钮动画逻辑，如按下时的缩放效果等
}

// 渲染按钮
void UIButton_Render(ui_button_t *button) {
    if (!button || !button->config.visible) return;

    const ui_button_config_t *cfg = &button->config;
    int state_idx = button->state;

    // 获取当前状态的颜色
    COLOR32 bg_color = cfg->bg_color[state_idx];
    COLOR32 border_color = cfg->border_color[state_idx];
    COLOR32 text_color = cfg->text_color[state_idx];

    // 绘制背景
    canvas2d_set_fill_style(button->ctx, bg_color);
    canvas2d_fill_rect(button->ctx, cfg->x, cfg->y, cfg->width, cfg->height);

    // 绘制边框
    if (cfg->border_width > 0) {
        canvas2d_set_stroke_style(button->ctx, border_color);
        canvas2d_set_line_width(button->ctx, cfg->border_width);
        canvas2d_stroke_rect(button->ctx, cfg->x, cfg->y, cfg->width, cfg->height);
    }

    // 绘制文本（居中）
    if (cfg->text[0] != '\0') {
        canvas2d_set_fill_style(button->ctx, text_color);
        // 简单居中计算（假设文本高度约为按钮高度的一半）
        float text_x = cfg->x + (cfg->width - 100) / 2; // 假设文本宽度约100
        float text_y = cfg->y + cfg->height / 2 + 5;
        canvas2d_fill_text(button->ctx, cfg->text, text_x, text_y);
    }
}

// 清理按钮
void UIButton_Shutdown(ui_button_t *button) {
    if (!button) return;
    memset(button, 0, sizeof(ui_button_t));
}
