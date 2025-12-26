#include "ui_text.h"
#include <string.h>
#include <stdio.h>

// 初始化 UI 文本
int UIText_Init(ui_text_t *text, const ui_text_config_t *config, canvas2d_context_t *ctx) {
    if (!text || !config || !ctx) {
        return -1;
    }

    memset(text, 0, sizeof(ui_text_t));
    text->config = *config;
    text->ctx = ctx;

    printf("UIText initialized: text='%s', pos=(%.1f,%.1f)\n",
           config->text, config->x, config->y);

    return 0;
}

// 设置文本内容
void UIText_SetText(ui_text_t *text, const char *content) {
    if (!text || !content) return;
    strncpy(text->config.text, content, 511);
    text->config.text[511] = '\0';
}

// 设置文本位置
void UIText_SetPosition(ui_text_t *text, float x, float y) {
    if (!text) return;
    text->config.x = x;
    text->config.y = y;
}

// 设置文本颜色
void UIText_SetColor(ui_text_t *text, COLOR32 color) {
    if (!text) return;
    text->config.color = color;
}

// 设置字体大小
void UIText_SetFontSize(ui_text_t *text, float font_size) {
    if (!text) return;
    text->config.font_size = font_size;
}

// 设置对齐方式
void UIText_SetAlign(ui_text_t *text, ui_text_align_t align, ui_text_valign_t valign) {
    if (!text) return;
    text->config.align = align;
    text->config.valign = valign;
}

// 设置换行选项
void UIText_SetWrap(ui_text_t *text, bool wrap, float wrap_width) {
    if (!text) return;
    text->config.wrap = wrap;
    text->config.wrap_width = wrap_width;
}

// 设置可见性
void UIText_SetVisible(ui_text_t *text, bool visible) {
    if (!text) return;
    text->config.visible = visible;
}

// 获取可见性
bool UIText_IsVisible(const ui_text_t *text) {
    if (!text) return false;
    return text->config.visible;
}

// 计算文本渲染位置
void UIText_CalcPosition(const ui_text_t *text, float *out_x, float *out_y) {
    if (!text || !out_x || !out_y) return;

    *out_x = text->config.x;
    *out_y = text->config.y;

    // 简单的文本宽度估算（每个字符约10像素）
    float text_width = strlen(text->config.text) * 10.0f;
    float text_height = text->config.font_size > 0 ? text->config.font_size : 16.0f;

    // 水平对齐
    switch (text->config.align) {
        case UI_TEXT_ALIGN_CENTER:
            if (text->config.wrap) {
                *out_x = text->config.x; // 居中对齐基于参考点
            } else {
                *out_x = text->config.x - text_width / 2;
            }
            break;
        case UI_TEXT_ALIGN_RIGHT:
            *out_x = text->config.x - text_width;
            break;
        case UI_TEXT_ALIGN_LEFT:
        default:
            *out_x = text->config.x;
            break;
    }

    // 垂直对齐
    switch (text->config.valign) {
        case UI_TEXT_VALIGN_MIDDLE:
            *out_y = text->config.y - text_height / 2;
            break;
        case UI_TEXT_VALIGN_BOTTOM:
            *out_y = text->config.y - text_height;
            break;
        case UI_TEXT_VALIGN_TOP:
        default:
            *out_y = text->config.y;
            break;
    }
}

// 更新文本
void UIText_Update(ui_text_t *text, int msec) {
    if (!text) return;
    // 这里可以添加文本动画逻辑，如淡入淡出、打字效果等
}

// 渲染文本
void UIText_Render(ui_text_t *text) {
    if (!text || !text->config.visible) return;
    if (text->config.text[0] == '\0') return;

    const ui_text_config_t *cfg = &text->config;

    canvas2d_set_fill_style(text->ctx, cfg->color);

    if (cfg->wrap && cfg->wrap_width > 0) {
        // 简单的自动换行实现
        float line_height = cfg->font_size > 0 ? cfg->font_size * 1.2f : 20.0f;
        float max_chars_per_line = cfg->wrap_width / 10.0f; // 估算每行字符数
        int line = 0;
        int start = 0;
        int len = strlen(cfg->text);

        while (start < len) {
            int end = start + (int)max_chars_per_line;
            if (end > len) end = len;

            // 提取一行文本
            char line_text[256];
            int line_len = end - start;
            if (line_len >= 256) line_len = 255;
            strncpy(line_text, cfg->text + start, line_len);
            line_text[line_len] = '\0';

            // 计算渲染位置
            float render_x = cfg->x;
            float render_y = cfg->y + line * line_height;

            // 根据对齐方式调整X位置
            float text_width = line_len * 10.0f;
            switch (cfg->align) {
                case UI_TEXT_ALIGN_CENTER:
                    render_x = cfg->x - text_width / 2;
                    break;
                case UI_TEXT_ALIGN_RIGHT:
                    render_x = cfg->x - text_width;
                    break;
                case UI_TEXT_ALIGN_LEFT:
                default:
                    render_x = cfg->x;
                    break;
            }

            canvas2d_fill_text(text->ctx, line_text, render_x, render_y);
            start = end;
            line++;
        }
    } else {
        // 计算渲染位置
        float render_x, render_y;
        UIText_CalcPosition(text, &render_x, &render_y);
        canvas2d_fill_text(text->ctx, cfg->text, render_x, render_y);
    }
}

// 清理文本
void UIText_Shutdown(ui_text_t *text) {
    if (!text) return;
    memset(text, 0, sizeof(ui_text_t));
}
