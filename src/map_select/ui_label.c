#include "ui_label.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void label_init(ui_component_t *component, canvas2d_context_t *ctx) {
    (void)component;
    (void)ctx;
}

static void label_shutdown(ui_component_t *component) {
    (void)component;
}

static void label_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
}

static void label_render(ui_component_t *component) {
    ui_label_t *label = (ui_label_t *)component;
    if (!label || !UIComponent_IsVisible(component)) return;
    if (label->text[0] == '\0') return;

    // 绘制背景（如果启用且不透明）
    if (label->has_background && label->bg_color.a > 0) {
        canvas2d_set_fill_style(component->ctx, label->bg_color);
        canvas2d_fill_rect(component->ctx, component->x, component->y, component->width, component->height);
    }

    // 设置字体和颜色
    canvas2d_set_fill_style(component->ctx, label->text_color);
    canvas2d_set_font_size(component->ctx, label->font_size);

    // 计算文本位置
    float text_width = canvas2d_measure_text(component->ctx, label->text);
    float text_height = label->font_size > 0 ? label->font_size : 16.0f;
    float render_x = component->x;
    float render_y = component->y;

    // 水平对齐
    switch (label->align) {
        case UI_LABEL_ALIGN_CENTER:
            render_x = component->x + (component->width - text_width) / 2.0f;
            break;
        case UI_LABEL_ALIGN_RIGHT:
            render_x = component->x + component->width - text_width;
            break;
        case UI_LABEL_ALIGN_LEFT:
        default:
            render_x = component->x;
            break;
    }

    // 垂直对齐（文本基线对齐，需要向上偏移以补偿基线）
    switch (label->valign) {
        case UI_LABEL_VALIGN_MIDDLE:
            // 文本垂直居中：基线在中心向上偏移字体高度的35%
            render_y = component->y + component->height / 2.0f - label->font_size * 0.35f;
            break;
        case UI_LABEL_VALIGN_BOTTOM:
            render_y = component->y + component->height - label->font_size * 0.2f;
            break;
        case UI_LABEL_VALIGN_TOP:
        default:
            render_y = component->y + label->font_size * 0.8f;
            break;
    }

    // 绘制文本
    canvas2d_fill_text(component->ctx, label->text, render_x, render_y);
}

static void label_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void label_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void label_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool label_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_label_vtable = {
    .init = label_init,
    .shutdown = label_shutdown,
    .update = label_update,
    .render = label_render,
    .set_position = label_set_position,
    .set_size = label_set_size,
    .set_bounds = label_set_bounds,
    .hit_test = label_hit_test,
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

ui_label_t* UILabel_Create(float x, float y, float width, float height, const char *text,
                          COLOR32 text_color, float font_size,
                          ui_label_align_t align, ui_label_valign_t valign,
                          canvas2d_context_t *ctx) {
    return UILabel_CreateWithBackground(x, y, width, height, text, text_color,
                                       MAKE(COLOR32, 0, 0, 0, 0), font_size,
                                       align, valign, ctx);
}

ui_label_t* UILabel_CreateWithBackground(float x, float y, float width, float height, const char *text,
                                         COLOR32 text_color, COLOR32 bg_color, float font_size,
                                         ui_label_align_t align, ui_label_valign_t valign,
                                         canvas2d_context_t *ctx) {
    ui_label_t *label = malloc(sizeof(ui_label_t));
    if (!label) return NULL;

    if (UILabel_Init(label, ctx) != 0) {
        free(label);
        return NULL;
    }

    label->base.x = x;
    label->base.y = y;
    label->base.width = width;
    label->base.height = height;
    strncpy(label->text, text, 511);
    label->text[511] = '\0';
    label->text_color = text_color;
    label->bg_color = bg_color;
    label->font_size = font_size;
    label->align = align;
    label->valign = valign;
    label->has_background = (bg_color.a > 0);

    return label;
}

int UILabel_Init(ui_label_t *label, canvas2d_context_t *ctx) {
    if (!label || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&label->base, UI_COMPONENT_TYPE_TEXT, &g_label_vtable, ctx);

    label->text[0] = '\0';
    label->text_color = MAKE(COLOR32, 255, 255, 255, 255);
    label->bg_color = MAKE(COLOR32, 0, 0, 0, 0);
    label->font_size = 16.0f;
    label->align = UI_LABEL_ALIGN_LEFT;
    label->valign = UI_LABEL_VALIGN_TOP;
    label->has_background = false;

    return 0;
}

void UILabel_SetText(ui_label_t *label, const char *content) {
    if (!label || !content) return;
    strncpy(label->text, content, 511);
    label->text[511] = '\0';
}

void UILabel_SetTextColor(ui_label_t *label, COLOR32 color) {
    if (!label) return;
    label->text_color = color;
}

void UILabel_SetBgColor(ui_label_t *label, COLOR32 color) {
    if (!label) return;
    label->bg_color = color;
    label->has_background = (color.a > 0);
}

void UILabel_SetBackgroundVisible(ui_label_t *label, bool visible) {
    if (!label) return;
    label->has_background = visible && (label->bg_color.a > 0);
}

void UILabel_SetFontSize(ui_label_t *label, float font_size) {
    if (!label) return;
    label->font_size = font_size;
}

void UILabel_SetAlign(ui_label_t *label, ui_label_align_t align, ui_label_valign_t valign) {
    if (!label) return;
    label->align = align;
    label->valign = valign;
}

void UILabel_SetBounds(ui_label_t *label, float x, float y, float width, float height) {
    if (!label) return;
    if (label->base.vtable && label->base.vtable->set_bounds) {
        label->base.vtable->set_bounds(&label->base, x, y, width, height);
    }
}

void UILabel_Update(ui_label_t *label, int msec) {
    if (!label) return;
    if (label->base.vtable && label->base.vtable->update) {
        label->base.vtable->update(&label->base, msec);
    }
}

void UILabel_Render(ui_label_t *label) {
    if (!label) return;
    if (label->base.vtable && label->base.vtable->render) {
        label->base.vtable->render(&label->base);
    }
}

void UILabel_Shutdown(ui_label_t *label) {
    if (!label) return;

    if (label->base.vtable && label->base.vtable->shutdown) {
        label->base.vtable->shutdown(&label->base);
    }

    UIComponent_ShutdownBase(&label->base);
}

void UILabel_Destroy(ui_label_t *label) {
    if (!label) return;
    UILabel_Shutdown(label);
    free(label);
}
