#include "ui_text.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void text_init(ui_component_t *component, canvas2d_context_t *ctx) {
    (void)component;
    (void)ctx;
}

static void text_shutdown(ui_component_t *component) {
    (void)component;
}

static void text_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
    // 可以添加文本动画逻辑，如淡入淡出、打字效果等
}

static void text_render(ui_component_t *component) {
    ui_text_t *text = (ui_text_t *)component;
    if (!text || !UIComponent_IsVisible(component)) return;
    if (text->text[0] == '\0') return;

    canvas2d_set_fill_style(component->ctx, text->color);
    canvas2d_set_font_size(component->ctx, text->font_size);

    if (text->wrap && text->wrap_width > 0) {
        // 简单的自动换行实现
        float line_height = text->font_size > 0 ? text->font_size * 1.2f : 20.0f;
        float max_chars_per_line = text->wrap_width / 10.0f;
        int line = 0;
        int start = 0;
        int len = strlen(text->text);

        while (start < len) {
            int end = start + (int)max_chars_per_line;
            if (end > len) end = len;

            char line_text[256];
            int line_len = end - start;
            if (line_len >= 256) line_len = 255;
            strncpy(line_text, text->text + start, line_len);
            line_text[line_len] = '\0';

            float render_x = component->x;
            float render_y = component->y + line * line_height;

            float text_width = line_len * 10.0f;
            switch (text->align) {
                case UI_TEXT_ALIGN_CENTER:
                    render_x = component->x - text_width / 2;
                    break;
                case UI_TEXT_ALIGN_RIGHT:
                    render_x = component->x - text_width;
                    break;
                case UI_TEXT_ALIGN_LEFT:
                default:
                    render_x = component->x;
                    break;
            }

            canvas2d_fill_text(component->ctx, line_text, render_x, render_y);
            start = end;
            line++;
        }
    } else {
        float render_x, render_y;
        UIText_CalcPosition(text, &render_x, &render_y);
        canvas2d_fill_text(component->ctx, text->text, render_x, render_y);
    }
}

static void text_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void text_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void text_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool text_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_text_vtable = {
    .init = text_init,
    .shutdown = text_shutdown,
    .update = text_update,
    .render = text_render,
    .set_position = text_set_position,
    .set_size = text_set_size,
    .set_bounds = text_set_bounds,
    .hit_test = text_hit_test,
    .on_mouse_enter = NULL,
    .on_mouse_leave = NULL,
    .on_mouse_down = NULL,
    .on_mouse_up = NULL,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = NULL,
    .on_mouse_wheel = NULL,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = NULL,
    .on_key_up = NULL,
    .on_text_input = NULL,
    .on_text_input = NULL,
    .on_focus = NULL,
    .on_blur = NULL,
    .on_resize = NULL,
    .on_scroll = NULL,
    .add_child = NULL,
    .remove_child = NULL,
    .get_child_count = NULL,
    .get_child = NULL,
    .get_custom_data = NULL,
    .set_custom_data = NULL,
};

// ==================== 公共API实现 ====================

ui_text_t* UIText_Create(float x, float y, const char *text, COLOR32 color, float font_size,
                        ui_text_align_t align, ui_text_valign_t valign, canvas2d_context_t *ctx) {
    ui_text_t *ui_text = malloc(sizeof(ui_text_t));
    if (!ui_text) return NULL;

    if (UIText_Init(ui_text, ctx) != 0) {
        free(ui_text);
        return NULL;
    }

    ui_text->base.x = x;
    ui_text->base.y = y;
    ui_text->base.width = 0;
    ui_text->base.height = font_size;
    strncpy(ui_text->text, text, 511);
    ui_text->text[511] = '\0';
    ui_text->color = color;
    ui_text->font_size = font_size;
    ui_text->align = align;
    ui_text->valign = valign;
    ui_text->wrap = false;
    ui_text->wrap_width = 0;

    return ui_text;
}

int UIText_Init(ui_text_t *text, canvas2d_context_t *ctx) {
    if (!text || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&text->base, UI_COMPONENT_TYPE_TEXT, &g_text_vtable, ctx);

    text->text[0] = '\0';
    text->color = MAKE(COLOR32, 255, 255, 255, 255);
    text->font_size = 16.0f;
    text->align = UI_TEXT_ALIGN_LEFT;
    text->valign = UI_TEXT_VALIGN_TOP;
    text->wrap = false;
    text->wrap_width = 0;

    return 0;
}

void UIText_SetText(ui_text_t *text, const char *content) {
    if (!text || !content) return;
    strncpy(text->text, content, 511);
    text->text[511] = '\0';
}

void UIText_SetPosition(ui_text_t *text, float x, float y) {
    if (!text) return;
    if (text->base.vtable && text->base.vtable->set_position) {
        text->base.vtable->set_position(&text->base, x, y);
    }
}

void UIText_SetColor(ui_text_t *text, COLOR32 color) {
    if (!text) return;
    text->color = color;
}

void UIText_SetFontSize(ui_text_t *text, float font_size) {
    if (!text) return;
    text->font_size = font_size;
    text->base.height = font_size;
}

void UIText_SetAlign(ui_text_t *text, ui_text_align_t align, ui_text_valign_t valign) {
    if (!text) return;
    text->align = align;
    text->valign = valign;
}

void UIText_SetWrap(ui_text_t *text, bool wrap, float wrap_width) {
    if (!text) return;
    text->wrap = wrap;
    text->wrap_width = wrap_width;
}

void UIText_CalcPosition(const ui_text_t *text, float *out_x, float *out_y) {
    if (!text || !out_x || !out_y) return;

    *out_x = text->base.x;
    *out_y = text->base.y;

    float text_width = canvas2d_measure_text(text->base.ctx, text->text);
    float text_height = text->font_size > 0 ? text->font_size : 16.0f;

    // 水平对齐
    switch (text->align) {
        case UI_TEXT_ALIGN_CENTER:
            if (text->wrap) {
                *out_x = text->base.x;
            } else {
                *out_x = text->base.x - text_width / 2;
            }
            break;
        case UI_TEXT_ALIGN_RIGHT:
            *out_x = text->base.x - text_width;
            break;
        case UI_TEXT_ALIGN_LEFT:
        default:
            *out_x = text->base.x;
            break;
    }

    // 垂直对齐
    switch (text->valign) {
        case UI_TEXT_VALIGN_MIDDLE:
            *out_y = text->base.y - text_height / 2;
            break;
        case UI_TEXT_VALIGN_BOTTOM:
            *out_y = text->base.y - text_height;
            break;
        case UI_TEXT_VALIGN_TOP:
        default:
            *out_y = text->base.y;
            break;
    }
}

void UIText_Update(ui_text_t *text, int msec) {
    if (!text) return;
    if (text->base.vtable && text->base.vtable->update) {
        text->base.vtable->update(&text->base, msec);
    }
}

void UIText_Render(ui_text_t *text) {
    if (!text) return;
    if (text->base.vtable && text->base.vtable->render) {
        text->base.vtable->render(&text->base);
    }
}

void UIText_Shutdown(ui_text_t *text) {
    if (!text) return;

    if (text->base.vtable && text->base.vtable->shutdown) {
        text->base.vtable->shutdown(&text->base);
    }

    UIComponent_ShutdownBase(&text->base);
}

void UIText_Destroy(ui_text_t *text) {
    if (!text) return;
    UIText_Shutdown(text);
    free(text);
}
