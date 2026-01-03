#include "ui_button.h"
#include "common/shared.h"
#include "ui/ui_component.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void button_init(ui_component_t *component, canvas2d_context_t *ctx) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button) return;

    // 初始化文本组件
    UIText_Init(&button->text_component, ctx);
    
    // 设置文本属性
    strncpy(button->text_component.text, button->config.text, 511);
    button->text_component.text[511] = '\0';
    button->text_component.color = button->config.text_colors.normal;
    button->text_component.font_size = button->config.font_size;
    button->text_component.align = UI_TEXT_ALIGN_CENTER;
    button->text_component.valign = UI_TEXT_VALIGN_MIDDLE;
    button->text_component.wrap = false;
    button->text_component.wrap_width = 0;
}

static void button_shutdown(ui_component_t *component) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button) return;

    UIText_Shutdown(&button->text_component);
}

static void button_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
    // 可以添加动画逻辑
}

static void button_render(ui_component_t *component) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsVisible(component)) return;

    // 根据状态选择颜色
    COLOR32 bg_color, border_color, text_color;
    switch (button->state) {
        case UI_BUTTON_STATE_NORMAL:
            bg_color = button->config.bg_colors.normal;
            border_color = button->config.border_colors.normal;
            text_color = button->config.text_colors.normal;
            break;
        case UI_BUTTON_STATE_HOVER:
            bg_color = button->config.bg_colors.hover;
            border_color = button->config.border_colors.hover;
            text_color = button->config.text_colors.hover;
            break;
        case UI_BUTTON_STATE_PRESSED:
            bg_color = button->config.bg_colors.active;
            border_color = button->config.border_colors.active;
            text_color = button->config.text_colors.active;
            break;
        case UI_BUTTON_STATE_DISABLED:
            bg_color = button->config.bg_colors.disabled;
            border_color = button->config.border_colors.disabled;
            text_color = button->config.text_colors.disabled;
            break;
        default:
            bg_color = button->config.bg_colors.normal;
            border_color = button->config.border_colors.normal;
            text_color = button->config.text_colors.normal;
            break;
    }

    // 绘制背景
    canvas2d_set_fill_style(button->base.ctx, bg_color);
    canvas2d_fill_rect(button->base.ctx, component->x, component->y, component->width, component->height);

    // 绘制边框
    if (button->config.border_width > 0) {
        canvas2d_set_stroke_style(button->base.ctx, border_color);
        canvas2d_set_line_width(button->base.ctx, button->config.border_width);
        canvas2d_stroke_rect(button->base.ctx, component->x, component->y, component->width, component->height);
    }

    // 更新文本组件
    UIText_SetColor(&button->text_component, text_color);

    // 按下状态时文本向右下偏移
    float text_offset_x = 0;
    float text_offset_y = 0;
    if (button->state == UI_BUTTON_STATE_PRESSED) {
        text_offset_x = 1.0f;
        text_offset_y = 2.0f;
    }

    UIText_SetPosition(&button->text_component,
                       component->x + component->width / 2.0f + text_offset_x,
                       component->y + component->height / 2.0f + text_offset_y);
    UIText_SetFontSize(&button->text_component, button->config.font_size);

    UIText_Render(&button->text_component);
}

static void button_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void button_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void button_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool button_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

static bool button_on_mouse_enter(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    if (button->state != UI_BUTTON_STATE_PRESSED) {
        button->state = UI_BUTTON_STATE_HOVER;
    }
    return true;
}

static bool button_on_mouse_leave(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button) return false;


    // Set state to normal when mouse leave whatever pressed or not.
    if (UIComponent_IsEnabled(component)) {
        button->state = UI_BUTTON_STATE_NORMAL;
    }
    return true;
}

static bool button_on_mouse_down(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    button->state = UI_BUTTON_STATE_PRESSED;
    return true;
}

static bool button_on_mouse_up(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    if (button->state == UI_BUTTON_STATE_PRESSED) {
        button->state = UI_BUTTON_STATE_HOVER;
    }
    return true;
}

static bool button_on_click(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    if (button->state != UI_BUTTON_STATE_DISABLED) {
        return true;
    }
    return false;
}

static bool button_on_double_click(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    if (button->config.double_click_enabled) {
        return true;
    }
    return false;
}

static bool button_on_mouse_move(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    if (!button || !UIComponent_IsEnabled(component)) return false;

    bool is_hovered = button_hit_test(component, event->mouse.x, event->mouse.y);

    if (is_hovered && button->state != UI_BUTTON_STATE_PRESSED) {
        button->state = UI_BUTTON_STATE_HOVER;
    } else if (!is_hovered && button->state != UI_BUTTON_STATE_PRESSED) {
        button->state = UI_BUTTON_STATE_NORMAL;
    }

    return false;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_button_vtable = {
    .init = button_init,
    .shutdown = button_shutdown,
    .update = button_update,
    .render = button_render,
    .set_position = button_set_position,
    .set_size = button_set_size,
    .set_bounds = button_set_bounds,
    .hit_test = button_hit_test,
    .on_mouse_enter = button_on_mouse_enter,
    .on_mouse_leave = button_on_mouse_leave,
    .on_mouse_down = button_on_mouse_down,
    .on_mouse_up = button_on_mouse_up,
    .on_click = button_on_click,
    .on_double_click = button_on_double_click,
    .on_mouse_move = button_on_mouse_move,
    .on_mouse_wheel = NULL,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = NULL,
    .on_key_up = NULL,
    .on_key_press = NULL,
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

ui_button_t* UIButton_Create(float x, float y, float width, float height, canvas2d_context_t *ctx) {
    ui_button_config_t config = UIButton_GetDefaultConfig();
    return UIButton_CreateWithConfig(x, y, width, height, &config, ctx);
}

ui_button_t* UIButton_CreateWithConfig(float x, float y, float width, float height,
                                      const ui_button_config_t *config, canvas2d_context_t *ctx) {
    if (!config || !ctx) {
        return NULL;
    }

    ui_button_t *button = malloc(sizeof(ui_button_t));
    if (!button) {
        return NULL;
    }

    if (UIButton_Init(button, config, ctx) != 0) {
        free(button);
        return NULL;
    }

    button->base.x = x;
    button->base.y = y;
    button->base.width = width;
    button->base.height = height;

    printf("UIButton_Create: x=%.1f, y=%.1f, w=%.1f, h=%.1f, text='%s'\n", x, y, width, height, config->text);
    return button;
}

void UIButton_Destroy(ui_button_t *button) {
    if (!button) return;
    UIButton_Shutdown(button);
    free(button);
}

ui_button_config_t UIButton_GetDefaultConfig(void) {
    ui_button_config_t config = {
        .text = "",
        .bg_colors = {
            .normal = {60, 60, 70, 255},
            .hover = {80, 80, 90, 255},
            .active = {50, 50, 60, 255},
            .disabled = {40, 40, 50, 200}
        },
        .border_colors = {
            .normal = {200, 200, 200, 255},
            .hover = {220, 220, 220, 255},
            .active = {180, 180, 180, 255},
            .disabled = {100, 100, 100, 255}
        },
        .text_colors = {
            .normal = {255, 255, 255, 255},
            .hover = {255, 255, 255, 255},
            .active = {255, 255, 255, 255},
            .disabled = {180, 180, 180, 255}
        },
        .font_size = 16.0f,
        .border_width = 1.0f,
        .double_click_enabled = false
    };
    return config;
}

int UIButton_Init(ui_button_t *button, const ui_button_config_t *config, canvas2d_context_t *ctx) {
    if (!button || !config || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&button->base, UI_COMPONENT_TYPE_BUTTON, &g_button_vtable, ctx);

    // 复制配置
    button->config = *config;

    // 初始化状态
    button->state = UI_BUTTON_STATE_NORMAL;

    // 调用虚函数init
    if (button->base.vtable && button->base.vtable->init) {
        button->base.vtable->init(&button->base, ctx);
    }

    printf("UIButton initialized: text='%s'\n", config->text);
    return 0;
}

void UIButton_Shutdown(ui_button_t *button) {
    if (!button) return;

    // 调用虚函数shutdown
    if (button->base.vtable && button->base.vtable->shutdown) {
        button->base.vtable->shutdown(&button->base);
    }

    // 关闭基础组件
    UIComponent_ShutdownBase(&button->base);
}

void UIButton_SetText(ui_button_t *button, const char *text) {
    if (!button || !text) return;
    strncpy(button->config.text, text, 127);
    button->config.text[127] = '\0';
    UIText_SetText(&button->text_component, text);
}

const char* UIButton_GetText(ui_button_t *button) {
    if (!button) return "";
    return button->config.text;
}

void UIButton_SetPosition(ui_button_t *button, float x, float y) {
    if (!button) return;
    if (button->base.vtable && button->base.vtable->set_position) {
        button->base.vtable->set_position(&button->base, x, y);
    }
}

void UIButton_SetSize(ui_button_t *button, float width, float height) {
    if (!button) return;
    if (button->base.vtable && button->base.vtable->set_size) {
        button->base.vtable->set_size(&button->base, width, height);
    }
}

void UIButton_SetEnabled(ui_button_t *button, bool enabled) {
    if (!button) return;
    UIComponent_SetEnabled(&button->base, enabled);
    if (!enabled) {
        button->state = UI_BUTTON_STATE_DISABLED;
    } else if (button->state == UI_BUTTON_STATE_DISABLED) {
        button->state = UI_BUTTON_STATE_NORMAL;
    }
}

bool UIButton_IsEnabled(const ui_button_t *button) {
    return button ? UIComponent_IsEnabled((ui_component_t *)button) : false;
}

ui_button_state_t UIButton_GetState(ui_button_t *button) {
    return button ? button->state : UI_BUTTON_STATE_DISABLED;
}

bool UIButton_AddOnClick(ui_button_t *button, ui_event_handler_t handler, void *user_data) {
    return button ? UIComponent_AddEventHandler(&button->base, EVENT_CLICK, handler, user_data) : false;
}

bool UIButton_AddOnDoubleClick(ui_button_t *button, ui_event_handler_t handler, void *user_data) {
    return button ? UIComponent_AddEventHandler(&button->base, EVENT_DOUBLE_CLICK, handler, user_data) : false;
}

bool UIButton_AddOnMouseEnter(ui_button_t *button, ui_event_handler_t handler, void *user_data) {
    return button ? UIComponent_AddEventHandler(&button->base, EVENT_MOUSE_ENTER, handler, user_data) : false;
}

bool UIButton_AddOnMouseLeave(ui_button_t *button, ui_event_handler_t handler, void *user_data) {
    return button ? UIComponent_AddEventHandler(&button->base, EVENT_MOUSE_LEAVE, handler, user_data) : false;
}

void UIButton_Update(ui_button_t *button, int msec) {
    if (!button) return;
    if (button->base.vtable && button->base.vtable->update) {
        button->base.vtable->update(&button->base, msec);
    }
}

void UIButton_Render(ui_button_t *button) {
    if (!button) return;
    if (button->base.vtable && button->base.vtable->render) {
        button->base.vtable->render(&button->base);
    }
}
