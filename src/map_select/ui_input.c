#include "ui_input.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// ==================== 虚函数实现 ====================

static void input_init(ui_component_t *component, canvas2d_context_t *ctx) {
    (void)component;
    (void)ctx;
}

static void input_shutdown(ui_component_t *component) {
    (void)component;
}

static void input_update(ui_component_t *component, int msec) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input) return;
    
    // 更新光标闪烁
    if (input->focused && !input->readonly) {
        input->cursor_blink_timer += msec;
        if (input->cursor_blink_timer >= input->cursor_blink_interval) {
            input->cursor_blink_visible = !input->cursor_blink_visible;
            input->cursor_blink_timer = 0;
        }
    } else {
        // 失去焦点时，光标始终不可见
        input->cursor_blink_visible = false;
    }
}

static void input_render(ui_component_t *component) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input || !UIComponent_IsVisible(component)) return;

    // 绘制背景
    canvas2d_set_fill_style(component->ctx, input->bg_color);
    canvas2d_fill_rect(component->ctx, component->x, component->y, component->width, component->height);

    // 绘制边框
    if (input->focused) {
        canvas2d_set_fill_style(component->ctx, input->focus_border_color);
    } else {
        canvas2d_set_fill_style(component->ctx, input->border_color);
    }
    canvas2d_set_line_width(component->ctx, 2.0f);
    canvas2d_stroke_rect(component->ctx, component->x, component->y, component->width, component->height);

    // 计算文本显示区域（考虑padding）
    float padding_top = component->padding[0];
    float padding_bottom = component->padding[2];
    float padding_left = component->padding[3];
    float padding_right = component->padding[1];

    float text_x = component->x + padding_left - input->scroll_offset;
    float text_y = component->y + padding_top;
    float text_height = component->height - padding_top - padding_bottom;

    // 设置字体
    canvas2d_set_font_size(component->ctx, input->font_size);

    // 确定要显示的文本
    const char *display_text = input->text[0] != '\0' ? input->text : input->placeholder;
    COLOR32 text_color = input->text[0] != '\0' ? input->text_color : input->placeholder_color;
    BOOL has_input = (input->text[0] == '\0');
    canvas2d_set_fill_style(component->ctx, text_color);

    // 如果是密码模式，将文本替换为密码字符
    char display_buffer[256];
    if (input->password && input->text[0] != '\0') {
        int len = strlen(input->text);
        if (len > 255) len = 255;
        memset(display_buffer, input->password_char, len);
        display_buffer[len] = '\0';
        display_text = display_buffer;
    }

    // 绘制文本
    canvas2d_fill_text(component->ctx, display_text, text_x,
                       text_y + text_height / 2 + input->font_size / 3);

    // 绘制光标（仅当获得焦点且不是只读模式时）
    if (input->focused && !input->readonly && input->cursor_blink_visible) {
      float cursor_x =
          text_x +
          canvas2d_measure_text(component->ctx,
                                input->text[0] != '\0'
                                    ? strndup(input->text, input->cursor_pos)
                                    : "");
      float cursor_y = component->y + padding_top + 2;
      float cursor_height =
          component->height - padding_top - padding_bottom - 4;

      canvas2d_set_fill_style(component->ctx,
                              MAKE(COLOR32, 255, 255, 255, 255));
      canvas2d_fill_rect(component->ctx, cursor_x, cursor_y, 2, cursor_height);
    }
}

static void input_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void input_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void input_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static bool input_hit_test(ui_component_t *component, float x, float y) {
    return x >= component->x && x < component->x + component->width &&
           y >= component->y && y < component->y + component->height;
}

// 鼠标按下事件
static bool input_on_mouse_down(ui_component_t *component, ui_mouse_event_t *event) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input) return false;
    
    // 获取焦点
    input->focused = true;
    
    // 启动 SDL 文本输入（支持输入法）
    SDL_StartTextInput();
    SDL_Rect rect = (SDL_Rect){(int)component->x, (int)component->y, (int)component->width, (int)component->height};
    SDL_SetTextInputRect(&rect);
    // 重置光标闪烁状态，使光标立即显示
    input->cursor_blink_visible = true;
    input->cursor_blink_timer = 0;
    
    return true;
}
// 鼠标释放事件
static bool input_on_mouse_up(ui_component_t *component, ui_mouse_event_t *event) {
    (void)component;
    (void)event;
    return false;
}

// 键盘按下事件
static bool input_on_key_down(ui_component_t *component, ui_keyboard_event_t *event) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input || !input->focused || input->readonly) return false;

    int text_len = strlen(input->text);

    switch (event->key) {
        case SDLK_BACKSPACE: {
            // 删除光标前的字符
            if (input->cursor_pos > 0) {
                memmove(input->text + input->cursor_pos - 1, input->text + input->cursor_pos, text_len - input->cursor_pos + 1);
                input->cursor_pos--;
                // 调整滚动偏移
                if (input->cursor_pos < input->scroll_offset) {
                    input->scroll_offset = input->cursor_pos;
                }
            }
            return true;
        }
        case SDLK_DELETE: {
            // 删除光标后的字符
            if (input->cursor_pos < text_len) {
                memmove(input->text + input->cursor_pos, input->text + input->cursor_pos + 1, text_len - input->cursor_pos);
            }
            return true;
        }
        case SDLK_LEFT: {
            // 光标左移
            if (input->cursor_pos > 0) {
                input->cursor_pos--;
                if (input->cursor_pos < input->scroll_offset) {
                    input->scroll_offset = input->cursor_pos;
                }
            }
            return true;
        }
        case SDLK_RIGHT: {
            // 光标右移
            if (input->cursor_pos < text_len) {
                input->cursor_pos++;
            }
            return true;
        }
        case SDLK_HOME: {
            // 光标移到开头
            input->cursor_pos = 0;
            input->scroll_offset = 0;
            return true;
        }
        case SDLK_END: {
            // 光标移到结尾
            input->cursor_pos = text_len;
            return true;
        }
        case SDLK_RETURN:
        case SDLK_KP_ENTER: {
            // 回车键，可以触发输入完成事件
            return true;
        }
        case SDLK_ESCAPE: {
            // ESC键，可以取消输入
            input->focused = false;
            return true;
        }
        default:
            break;
    }
    
    return false;
}

// 文本输入事件（支持中文输入法）
static bool input_on_text_input(ui_component_t *component, const char *text) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input || !input->focused || input->readonly || !text) return false;
    
    int text_len = strlen(input->text);
    size_t new_text_len = strlen(text);
    
    // 检查最大长度限制
    if (input->max_length > 0 && text_len + new_text_len >= input->max_length) {
        return false;
    }
    
    // 插入文本
    memmove(input->text + input->cursor_pos + new_text_len, input->text + input->cursor_pos, text_len - input->cursor_pos + 1);
    memcpy(input->text + input->cursor_pos, text, new_text_len);
    input->cursor_pos += new_text_len;
    
    // 调整滚动偏移以保持光标可见
    float char_width = canvas2d_measure_text(component->ctx, "M");
    float visible_width = component->width - component->padding[1] - component->padding[3];
    int max_visible_chars = (int)(visible_width / char_width);
    
    if (input->cursor_pos - input->scroll_offset > max_visible_chars) {
        input->scroll_offset = input->cursor_pos - max_visible_chars;
    }
    
return true;
}

// 焦点失去事件
static bool input_on_blur(ui_component_t *component, ui_focus_event_t *event) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input) return false;
    
    (void)event;  // 未使用
    
    // 停止 SDL 文本输入
    SDL_StopTextInput();
    
    // 失去焦点
    input->focused = false;
    
    return false;
}

// 键盘按键事件
static bool input_on_key_press(ui_component_t *component, ui_keyboard_event_t *event) {
    ui_input_t *input = (ui_input_t *)component;
    if (!input || !input->focused || input->readonly || event->repeat) return false;

    // 处理可打印字符
    if (event->key >= 32 && event->key <= 126) {
        int text_len = strlen(input->text);
        
        // 检查最大长度限制
        if (input->max_length > 0 && text_len >= input->max_length) {
            return false;
        }
        
        // 插入字符
        memmove(input->text + input->cursor_pos + 1, input->text + input->cursor_pos, text_len - input->cursor_pos + 1);
        input->text[input->cursor_pos] = (char)event->key;
        input->cursor_pos++;
        
        // 调整滚动偏移以保持光标可见
        float char_width = canvas2d_measure_text(component->ctx, "M");
        float visible_width = component->width - component->padding[1] - component->padding[3];
        int max_visible_chars = (int)(visible_width / char_width);
        
        if (input->cursor_pos - input->scroll_offset > max_visible_chars) {
            input->scroll_offset = input->cursor_pos - max_visible_chars;
        }
        
        return true;
    }
    
    return false;
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_input_vtable = {
    .init = input_init,
    .shutdown = input_shutdown,
    .update = input_update,
    .render = input_render,
    .set_position = input_set_position,
    .set_size = input_set_size,
    .set_bounds = input_set_bounds,
    .hit_test = input_hit_test,
    .on_mouse_enter = NULL,
    .on_mouse_leave = NULL,
    .on_mouse_down = input_on_mouse_down,
    .on_mouse_up = input_on_mouse_up,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = NULL,
    .on_mouse_wheel = NULL,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = input_on_key_down,
    .on_key_up = NULL,
    .on_key_press = input_on_key_press,
    .on_text_input = input_on_text_input,
    .on_focus = NULL,
    .on_blur = input_on_blur,
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

ui_input_t* UIInput_Create(float x, float y, float width, float height, float font_size,
                          const char *placeholder, canvas2d_context_t *ctx) {
    ui_input_t *input = malloc(sizeof(ui_input_t));
    if (!input) return NULL;

    if (UIInput_Init(input, ctx) != 0) {
        free(input);
        return NULL;
    }

    input->base.x = x;
    input->base.y = y;
    input->base.width = width;
    input->base.height = height;
    
    if (placeholder) {
        strncpy(input->placeholder, placeholder, 255);
        input->placeholder[255] = '\0';
    } else {
        input->placeholder[0] = '\0';
    }
    
    input->font_size = font_size;

    return input;
}

int UIInput_Init(ui_input_t *input, canvas2d_context_t *ctx) {
    if (!input || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&input->base, UI_COMPONENT_TYPE_EDITBOX, &g_input_vtable, ctx);

    input->text[0] = '\0';
    input->placeholder[0] = '\0';
    input->text_color = MAKE(COLOR32, 255, 255, 255, 255);
    input->placeholder_color = MAKE(COLOR32, 128, 128, 128, 255);
    input->bg_color = MAKE(COLOR32, 80, 80, 90, 255);
    input->border_color = MAKE(COLOR32, 120, 120, 130, 255);
    input->focus_border_color = MAKE(COLOR32, 100, 180, 255, 255);
    input->selection_bg_color = MAKE(COLOR32, 100, 150, 255, 128);
    input->font_size = 16.0f;
    input->cursor_pos = 0;
    input->selection_start = 0;
    input->selection_end = 0;
    input->scroll_offset = 0;
    input->focused = false;
    input->readonly = false;
    input->password = false;
    input->password_char = '*';
    input->max_length = 255;
input->align = UI_TEXT_ALIGN_LEFT;
    
    // 初始化光标闪烁
    input->cursor_blink_visible = true;
    input->cursor_blink_timer = 0;
    input->cursor_blink_interval = 1000;  // 1000ms 闪烁间隔（1秒）

    // 设置padding

    // 启用焦点和Tab访问
    input->base.flags |= UI_FLAG_ACCEPT_FOCUS | UI_FLAG_TAB_STOP;

    return 0;
}

void UIInput_SetText(ui_input_t *input, const char *text) {
    if (!input || !text) return;
    strncpy(input->text, text, 255);
    input->text[255] = '\0';
    input->cursor_pos = strlen(input->text);
    input->scroll_offset = 0;
}

const char* UIInput_GetText(ui_input_t *input) {
    return input ? input->text : NULL;
}

void UIInput_SetPlaceholder(ui_input_t *input, const char *placeholder) {
    if (!input || !placeholder) return;
    strncpy(input->placeholder, placeholder, 255);
    input->placeholder[255] = '\0';
}

void UIInput_SetTextColor(ui_input_t *input, COLOR32 color) {
    if (!input) return;
    input->text_color = color;
}

void UIInput_SetBgColor(ui_input_t *input, COLOR32 color) {
    if (!input) return;
    input->bg_color = color;
}

void UIInput_SetBorderColor(ui_input_t *input, COLOR32 color) {
    if (!input) return;
    input->border_color = color;
}

void UIInput_SetFocusBorderColor(ui_input_t *input, COLOR32 color) {
    if (!input) return;
    input->focus_border_color = color;
}

void UIInput_SetFontSize(ui_input_t *input, float font_size) {
    if (!input) return;
    input->font_size = font_size;
}

void UIInput_SetReadOnly(ui_input_t *input, bool readonly) {
    if (!input) return;
    input->readonly = readonly;
}

void UIInput_SetPassword(ui_input_t *input, bool password, char password_char) {
    if (!input) return;
    input->password = password;
    if (password_char != '\0') {
        input->password_char = password_char;
    }
}

void UIInput_SetMaxLength(ui_input_t *input, int max_length) {
    if (!input) return;
    input->max_length = max_length;
}

void UIInput_SetAlign(ui_input_t *input, ui_text_align_t align) {
    if (!input) return;
    input->align = align;
}

void UIInput_Update(ui_input_t *input, int msec) {
    if (!input) return;
    if (input->base.vtable && input->base.vtable->update) {
        input->base.vtable->update(&input->base, msec);
    }
}

void UIInput_Render(ui_input_t *input) {
    if (!input) return;
    if (input->base.vtable && input->base.vtable->render) {
        input->base.vtable->render(&input->base);
    }
}

void UIInput_Shutdown(ui_input_t *input) {
    if (!input) return;

    if (input->base.vtable && input->base.vtable->shutdown) {
        input->base.vtable->shutdown(&input->base);
    }

    UIComponent_ShutdownBase(&input->base);
}

void UIInput_Destroy(ui_input_t *input) {
    if (!input) return;
    UIInput_Shutdown(input);
    free(input);
}

void UIInput_SetFocused(ui_input_t *input, bool focused) {
    if (!input) return;
    input->focused = focused;
}

bool UIInput_IsFocused(ui_input_t *input) {
    return input ? input->focused : false;
}

void UIInput_Clear(ui_input_t *input) {
    if (!input) return;
    input->text[0] = '\0';
    input->cursor_pos = 0;
    input->scroll_offset = 0;
}
