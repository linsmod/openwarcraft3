#include "ui_event_dispatcher.h"
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// 默认配置值
#define DEFAULT_DOUBLE_CLICK_TIME 500
#define DEFAULT_DOUBLE_CLICK_DISTANCE 5
#define DEFAULT_DRAG_THRESHOLD 3

ui_event_dispatcher_config_t UIEventDispatcher_GetDefaultConfig(void) {
    ui_event_dispatcher_config_t config = {
        .double_click_time = DEFAULT_DOUBLE_CLICK_TIME,
        .double_click_distance = DEFAULT_DOUBLE_CLICK_DISTANCE,
        .drag_threshold = DEFAULT_DRAG_THRESHOLD
    };
    return config;
}

int UIEventDispatcher_Init(ui_event_dispatcher_t *dispatcher, ui_component_t *root, canvas2d_context_t *ctx) {
    if (!dispatcher || !ctx) {
        return -1;
    }

    memset(dispatcher, 0, sizeof(ui_event_dispatcher_t));
    dispatcher->config = UIEventDispatcher_GetDefaultConfig();
    dispatcher->root = root;
    dispatcher->focused = NULL;
    dispatcher->hovered = NULL;
    dispatcher->dragging = NULL;
    dispatcher->captured = NULL;
    dispatcher->last_clicked = NULL;
    dispatcher->last_click_time = 0;
    dispatcher->last_click_x = 0.0f;
    dispatcher->last_click_y = 0.0f;
    dispatcher->ctx = ctx;

    return 0;
}

void UIEventDispatcher_Shutdown(ui_event_dispatcher_t *dispatcher) {
    if (!dispatcher) return;
    // 确保释放鼠标捕获
    if (dispatcher->captured) {
        SDL_CaptureMouse(SDL_FALSE);
    }
    dispatcher->root = NULL;
    dispatcher->focused = NULL;
    dispatcher->hovered = NULL;
    dispatcher->dragging = NULL;
    dispatcher->captured = NULL;
    dispatcher->last_clicked = NULL;
}

void UIEventDispatcher_SetRoot(ui_event_dispatcher_t *dispatcher, ui_component_t *root) {
    if (!dispatcher) return;
    dispatcher->root = root;
}

// 命中测试：从叶子节点开始向上查找
static ui_component_t* hit_test_recursive(ui_component_t *component, float x, float y) {
    if (!component || !UIComponent_IsVisible(component)) {
        return NULL;
    }

    // 首先检查子组件（从后往前，因为后绘制的在上面）
    if (component->vtable && component->vtable->get_child_count) {
        int child_count = component->vtable->get_child_count(component);
        for (int i = child_count - 1; i >= 0; i--) {
            ui_component_t *child = component->vtable->get_child(component, i);
            ui_component_t *hit = hit_test_recursive(child, x, y);
            if (hit) {
                return hit;
            }
        }
    }

    // 检查当前组件
    if (component->vtable && component->vtable->hit_test) {
        if (component->vtable->hit_test(component, x, y)) {
            return component;
        }
    }

    return NULL;
}

ui_component_t* UIEventDispatcher_Capture(ui_event_dispatcher_t *dispatcher, float x, float y) {
    if (!dispatcher) return NULL;
    return hit_test_recursive(dispatcher->root, x, y);
}

bool UIEventDispatcher_BubbleEvent(ui_event_dispatcher_t *dispatcher, ui_component_t *target, ui_event_t *event) {
    if (!dispatcher || !target || !event) {
        return false;
    }

    ui_component_t *current = target;
    bool handled = false;

    // 从目标组件开始，向上冒泡到根组件
    while (current && !event->propagation_stopped) {
        event->current_target = current;

        // 根据事件类型调用相应的虚函数
        bool vtable_result = false;
        switch (event->type) {
            case UI_EVENT_MOUSE_ENTER:
                if (current->vtable && current->vtable->on_mouse_enter) {
                    vtable_result = current->vtable->on_mouse_enter(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_MOUSE_LEAVE:
                if (current->vtable && current->vtable->on_mouse_leave) {
                    vtable_result = current->vtable->on_mouse_leave(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_MOUSE_DOWN:
                if (current->vtable && current->vtable->on_mouse_down) {
                    vtable_result = current->vtable->on_mouse_down(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_MOUSE_UP:
                if (current->vtable && current->vtable->on_mouse_up) {
                    vtable_result = current->vtable->on_mouse_up(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_CLICK:
                if (current->vtable && current->vtable->on_click) {
                    vtable_result = current->vtable->on_click(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_DOUBLE_CLICK:
                if (current->vtable && current->vtable->on_double_click) {
                    vtable_result = current->vtable->on_double_click(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_MOUSE_MOVE:
                if (current->vtable && current->vtable->on_mouse_move) {
                    vtable_result = current->vtable->on_mouse_move(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_MOUSE_WHEEL:
                if (current->vtable && current->vtable->on_mouse_wheel) {
                    vtable_result = current->vtable->on_mouse_wheel(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_DRAG_START:
                if (current->vtable && current->vtable->on_drag_start) {
                    vtable_result = current->vtable->on_drag_start(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_DRAG:
                if (current->vtable && current->vtable->on_drag) {
                    vtable_result = current->vtable->on_drag(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_DRAG_END:
                if (current->vtable && current->vtable->on_drag_end) {
                    vtable_result = current->vtable->on_drag_end(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_CONTEXT_MENU:
                if (current->vtable && current->vtable->on_context_menu) {
                    vtable_result = current->vtable->on_context_menu(current, (ui_mouse_event_t *)event);
                }
                break;
            case UI_EVENT_KEY_DOWN:
                if (current->vtable && current->vtable->on_key_down) {
                    vtable_result = current->vtable->on_key_down(current, (ui_keyboard_event_t *)event);
                }
                break;
            case UI_EVENT_KEY_UP:
                if (current->vtable && current->vtable->on_key_up) {
                    vtable_result = current->vtable->on_key_up(current, (ui_keyboard_event_t *)event);
                }
                break;
            case UI_EVENT_KEY_PRESS:
                if (current->vtable && current->vtable->on_key_press) {
                    vtable_result = current->vtable->on_key_press(current, (ui_keyboard_event_t *)event);
                }
                break;
            case UI_EVENT_TEXT_INPUT:
                if (current->vtable && current->vtable->on_text_input) {
                    // 从event.target字段中获取文本数据
                    const char *text = (const char *)(uintptr_t)event->target;
                    vtable_result = current->vtable->on_text_input(current, text);
                }
                break;
            case UI_EVENT_FOCUS:
                if (current->vtable && current->vtable->on_focus) {
                    vtable_result = current->vtable->on_focus(current, (ui_focus_event_t *)event);
                }
                break;
            case UI_EVENT_BLUR:
                if (current->vtable && current->vtable->on_blur) {
                    vtable_result = current->vtable->on_blur(current, (ui_focus_event_t *)event);
                }
                break;
            case UI_EVENT_RESIZE:
                if (current->vtable && current->vtable->on_resize) {
                    vtable_result = current->vtable->on_resize(current, (ui_resize_event_t *)event);
                }
                break;
            case UI_EVENT_SCROLL:
                if (current->vtable && current->vtable->on_scroll) {
                    vtable_result = current->vtable->on_scroll(current, (ui_scroll_event_t *)event);
                }
                break;
            default:
                break;
        }

        // 调用注册的事件处理器
        bool handler_result = UIComponent_TriggerEvent(current, event);

        handled = handled || vtable_result || handler_result;

        // 向上移动到父组件
        current = current->parent;
    }

    return handled;
}

bool UIEventDispatcher_DispatchMouseDown(ui_event_dispatcher_t *dispatcher, float x, float y, int button, int timestamp) {
    if (!dispatcher) return false;

    // 如果有组件捕获了鼠标，优先发送给它
    ui_component_t *target = UIEventDispatcher_Capture(dispatcher, x, y);

    if(target){
        printf("Mouse down:  target component: %s, %2f %2f\n", UIComponent_GetTypeName( target->type), target->x, target->y);
    }
    // 更新鼠标按钮状态
    if (button >= 0 && button < 5) {
        dispatcher->mouse_buttons[button] = true;
    }

    ui_mouse_event_t event = {
        .base = {
            .type = UI_EVENT_MOUSE_DOWN,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .x = x,
        .y = y,
        .screen_x = x,
        .screen_y = y,
        .button = button,
        .click_count = 0,
        .delta_x = 0,
        .delta_y = 0,
        .buttons = 0
    };

    // 计算按钮状态掩码
    for (int i = 0; i < 5; i++) {
        if (dispatcher->mouse_buttons[i]) {
            event.buttons |= (1 << i);
        }
    }

    bool handled = UIEventDispatcher_BubbleEvent(dispatcher, target, &event.base);

    if(handled)
        return true;
    
    // 检查是否开始拖拽
    if (target && !event.base.propagation_stopped && !event.base.default_prevented &&
        (target->flags & UI_FLAG_DRAGGABLE) &&
        button == UI_MOUSE_BUTTON_LEFT) {
        if (target->vtable && target->vtable->on_drag_start) {
            target->flags |= UI_FLAG_DRAGGING;
            dispatcher->dragging = target;
            target->drag_start_x = x;
            target->drag_start_y = y;
            target->drag_offset_x = 0;
            target->drag_offset_y = 0;

            ui_mouse_event_t drag_event = event;
            drag_event.base.type = UI_EVENT_DRAG_START;
            UIEventDispatcher_BubbleEvent(dispatcher, target, &drag_event.base);
        }
    }

    // 处理焦点
    if (target && button == UI_MOUSE_BUTTON_LEFT) {
        ui_component_t *old_focus = dispatcher->focused;
        if (target != old_focus) {
            if (old_focus && (old_focus->flags & UI_FLAG_ACCEPT_FOCUS)) {
                ui_focus_event_t blur_event = {
                    .base = {
                        .type = UI_EVENT_BLUR,
                        .timestamp = timestamp
                    },
                    .related_target = target
                };
                UIEventDispatcher_BubbleEvent(dispatcher, old_focus, &blur_event.base);
            }

            if (target->flags & UI_FLAG_ACCEPT_FOCUS) {
                ui_focus_event_t focus_event = {
                    .base = {
                        .type = UI_EVENT_FOCUS,
                        .timestamp = timestamp
                    },
                    .related_target = old_focus
                };
                UIEventDispatcher_BubbleEvent(dispatcher, target, &focus_event.base);
                dispatcher->focused = target;
            }
        }
    }

    return handled;
}

bool UIEventDispatcher_DispatchMouseUp(ui_event_dispatcher_t *dispatcher, float x, float y, int button, int timestamp) {
    if (!dispatcher) return false;

    // 如果有组件捕获了鼠标，优先发送给它
    ui_component_t *target = dispatcher->captured ? dispatcher->captured : UIEventDispatcher_Capture(dispatcher, x, y);

    // 更新鼠标按钮状态
    if (button >= 0 && button < 5) {
        dispatcher->mouse_buttons[button] = false;
    }

    ui_mouse_event_t event = {
        .base = {
            .type = UI_EVENT_MOUSE_UP,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .x = x,
        .y = y,
        .screen_x = x,
        .screen_y = y,
        .button = button,
        .click_count = 0,
        .delta_x = 0,
        .delta_y = 0,
        .buttons = 0
    };

    // 计算按钮状态掩码
    for (int i = 0; i < 5; i++) {
        if (dispatcher->mouse_buttons[i]) {
            event.buttons |= (1 << i);
        }
    }

    bool handled = UIEventDispatcher_BubbleEvent(dispatcher, target, &event.base);
    if(handled) {
        //printf("MouseUp handled by component %p\n", target);
        return true;
    }

    // 处理点击事件（如果鼠标在同一个组件上按下和释放）
    if (target && button == UI_MOUSE_BUTTON_LEFT) {
        bool same_component = (target == dispatcher->last_clicked);

        // 双击检测
        bool is_double_click = false;
        if (same_component) {
            int time_diff = timestamp - dispatcher->last_click_time;
            float dx = x - dispatcher->last_click_x;
            float dy = y - dispatcher->last_click_y;
            float distance = dx * dx + dy * dy;

            if (time_diff < dispatcher->config.double_click_time &&
                distance < (float)(dispatcher->config.double_click_distance * dispatcher->config.double_click_distance)) {
                is_double_click = true;
            }
        }

        if (is_double_click) {
            // 双击事件
            ui_mouse_event_t double_click_event = event;
            double_click_event.base.type = UI_EVENT_DOUBLE_CLICK;
            double_click_event.click_count = 2;
            UIEventDispatcher_BubbleEvent(dispatcher, target, &double_click_event.base);
        } else {
            // 单击事件
            ui_mouse_event_t click_event = event;
            click_event.base.type = UI_EVENT_CLICK;
            click_event.click_count = 1;
            UIEventDispatcher_BubbleEvent(dispatcher, target, &click_event.base);
        }

        // 更新点击信息
        dispatcher->last_clicked = target;
        dispatcher->last_click_time = timestamp;
        dispatcher->last_click_x = x;
        dispatcher->last_click_y = y;
    }

    // 结束拖拽
    if (dispatcher->dragging) {
        ui_mouse_event_t drag_end_event = event;
        drag_end_event.base.type = UI_EVENT_DRAG_END;
        drag_end_event.delta_x = x - dispatcher->dragging->drag_start_x;
        drag_end_event.delta_y = y - dispatcher->dragging->drag_start_y;
        UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->dragging, &drag_end_event.base);

        dispatcher->dragging->flags &= ~UI_FLAG_DRAGGING;
        dispatcher->dragging = NULL;
    }

    return handled;
}

bool UIEventDispatcher_DispatchMouseMove(ui_event_dispatcher_t *dispatcher, float x, float y, int timestamp) {
    if (!dispatcher) return false;

    // 如果有组件捕获了鼠标，优先以它为目标
    ui_component_t *target = dispatcher->captured ? dispatcher->captured : UIEventDispatcher_Capture(dispatcher, x, y);

    // if (target){
    //     printf("Mouse move:  target component: %s (%d, %d)\n", UIComponent_GetTypeName(target->type), (int)x, (int)y);
    // }

    // 处理鼠标进入/离开事件（只有当没有组件捕获鼠标时才更新hover状态）
    if (!dispatcher->captured) {
        ui_component_t *old_hovered = dispatcher->hovered;
        dispatcher->hovered = target;

        if (old_hovered != target) {
            // 触发离开事件
            if (old_hovered) {
                ui_mouse_event_t leave_event = {
                    .base = {
                        .type = UI_EVENT_MOUSE_LEAVE,
                        .timestamp = timestamp
                    },
                    .x = x,
                    .y = y,
                    .screen_x = x,
                    .screen_y = y
                };
                UIEventDispatcher_BubbleEvent(dispatcher, old_hovered, &leave_event.base);
            }

            // 触发进入事件
            if (target) {
                ui_mouse_event_t enter_event = {
                    .base = {
                        .type = UI_EVENT_MOUSE_ENTER,
                        .timestamp = timestamp
                    },
                    .x = x,
                    .y = y,
                    .screen_x = x,
                    .screen_y = y
                };
                UIEventDispatcher_BubbleEvent(dispatcher, target, &enter_event.base);
            }
        }
    }

    // 处理拖拽事件
    if (dispatcher->dragging) {
        float dx = x - dispatcher->dragging->drag_start_x;
        float dy = y - dispatcher->dragging->drag_start_y;
        dispatcher->dragging->drag_offset_x = dx;
        dispatcher->dragging->drag_offset_y = dy;

        ui_mouse_event_t drag_event = {
            .base = {
                .type = UI_EVENT_DRAG,
                .timestamp = timestamp
            },
            .x = x,
            .y = y,
            .screen_x = x,
            .screen_y = y,
            .delta_x = dx,
            .delta_y = dy
        };
        UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->dragging, &drag_event.base);
    }

    // 处理鼠标移动事件
    ui_mouse_event_t move_event = {
        .base = {
            .type = UI_EVENT_MOUSE_MOVE,
            .timestamp = timestamp
        },
        .x = x,
        .y = y,
        .screen_x = x,
        .screen_y = y
    };

    return UIEventDispatcher_BubbleEvent(dispatcher, target, &move_event.base);
}

bool UIEventDispatcher_DispatchMouseWheel(ui_event_dispatcher_t *dispatcher, float x, float y, int delta, int timestamp) {
    if (!dispatcher) return false;

    ui_component_t *target = UIEventDispatcher_Capture(dispatcher, x, y);

    ui_mouse_event_t event = {
        .base = {
            .type = UI_EVENT_MOUSE_WHEEL,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .x = x,
        .y = y,
        .screen_x = x,
        .screen_y = y,
        .button = -1,
        .click_count = 0,
        .delta = delta,
        .buttons = 0
    };

    return UIEventDispatcher_BubbleEvent(dispatcher, target, &event.base);
}

bool UIEventDispatcher_DispatchContextMenu(ui_event_dispatcher_t *dispatcher, float x, float y, int timestamp) {
    if (!dispatcher) return false;

    ui_component_t *target = UIEventDispatcher_Capture(dispatcher, x, y);

    ui_mouse_event_t event = {
        .base = {
            .type = UI_EVENT_CONTEXT_MENU,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .x = x,
        .y = y,
        .screen_x = x,
        .screen_y = y,
        .button = UI_MOUSE_BUTTON_RIGHT,
        .click_count = 0,
        .buttons = 0
    };

    return UIEventDispatcher_BubbleEvent(dispatcher, target, &event.base);
}

bool UIEventDispatcher_DispatchKeyDown(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, bool repeat, int timestamp) {
    if (!dispatcher) return false;

    ui_keyboard_event_t event = {
        .base = {
            .type = UI_EVENT_KEY_DOWN,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .key = key,
        .scancode = scancode,
        .modifiers = modifiers,
        .repeat = repeat
    };

    // 首先发送给有焦点的组件
    if (dispatcher->focused) {
        if (UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->focused, &event.base)) {
            return true;
        }
    }

    // 然后发送给根组件
    return UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->root, &event.base);
}

bool UIEventDispatcher_DispatchKeyUp(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, int timestamp) {
    if (!dispatcher) return false;

    ui_keyboard_event_t event = {
        .base = {
            .type = UI_EVENT_KEY_UP,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .key = key,
        .scancode = scancode,
        .modifiers = modifiers,
        .repeat = false
    };

    if (dispatcher->focused) {
        if (UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->focused, &event.base)) {
            return true;
        }
    }

    return UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->root, &event.base);
}

bool UIEventDispatcher_DispatchKeyPress(ui_event_dispatcher_t *dispatcher, int key, int scancode, int modifiers, int timestamp) {
    if (!dispatcher) return false;

    ui_keyboard_event_t event = {
        .base = {
            .type = UI_EVENT_KEY_PRESS,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        },
        .key = key,
        .scancode = scancode,
        .modifiers = modifiers,
        .repeat = false
    };

    if (dispatcher->focused) {
        if (UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->focused, &event.base)) {
            return true;
        }
    }

    return UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->root, &event.base);
}

bool UIEventDispatcher_DispatchTextInput(ui_event_dispatcher_t *dispatcher, const char *text, int timestamp) {
    if (!dispatcher || !text) return false;

    // 首先发送给有焦点的组件
    if (dispatcher->focused) {
        // 创建一个文本输入事件，将文本作为事件数据传递
        ui_event_t event = {
            .type = UI_EVENT_TEXT_INPUT,
            .timestamp = timestamp,
            .propagation_stopped = false,
            .default_prevented = false
        };
        
        // 将文本数据存储在事件的target字段中（临时使用）
        // 这是一个变通方法，因为ui_event_t没有直接的文本字段
        event.target = (ui_component_t *)(uintptr_t)text;
        
        if (UIEventDispatcher_BubbleEvent(dispatcher, dispatcher->focused, &event)) {
            return true;
        }
    }

    return false;
}

void UIEventDispatcher_Update(ui_event_dispatcher_t *dispatcher, int msec) {
    if (!dispatcher) return;
    // 可以在这里处理动画、定时器等
}

void UIEventDispatcher_SetFocus(ui_event_dispatcher_t *dispatcher, ui_component_t *component) {
    if (!dispatcher) return;

    ui_component_t *old_focus = dispatcher->focused;
    if (old_focus != component) {
        if (old_focus) {
            ui_focus_event_t blur_event = {
                .base = {
                    .type = UI_EVENT_BLUR,
                    .timestamp = 0
                },
                .related_target = component
            };
            UIEventDispatcher_BubbleEvent(dispatcher, old_focus, &blur_event.base);
            old_focus->flags &= ~UI_FLAG_FOCUSED;
        }

        if (component && (component->flags & UI_FLAG_ACCEPT_FOCUS)) {
            ui_focus_event_t focus_event = {
                .base = {
                    .type = UI_EVENT_FOCUS,
                    .timestamp = 0
                },
                .related_target = old_focus
            };
            UIEventDispatcher_BubbleEvent(dispatcher, component, &focus_event.base);
            component->flags |= UI_FLAG_FOCUSED;
        }

        dispatcher->focused = component;
    }
}

ui_component_t* UIEventDispatcher_GetFocus(ui_event_dispatcher_t *dispatcher) {
    return dispatcher ? dispatcher->focused : NULL;
}

void UIEventDispatcher_ClearFocus(ui_event_dispatcher_t *dispatcher) {
    UIEventDispatcher_SetFocus(dispatcher, NULL);
}

// ==================== 鼠标捕获管理 ====================

// 捕获鼠标（组件将优先接收所有鼠标事件，即使鼠标移出窗口范围）
void UIEventDispatcher_CaptureMouse(ui_event_dispatcher_t *dispatcher, ui_component_t *component) {
    if (!dispatcher) return;
    dispatcher->captured = component;
    // 启用 SDL 鼠标捕获，即使鼠标移出窗口也能接收事件
    SDL_CaptureMouse(SDL_TRUE);
}

// 获取当前捕获鼠标的组件
ui_component_t* UIEventDispatcher_GetCaptured(ui_event_dispatcher_t *dispatcher) {
    return dispatcher ? dispatcher->captured : NULL;
}

// 释放鼠标捕获
void UIEventDispatcher_ReleaseMouse(ui_event_dispatcher_t *dispatcher) {
    if (!dispatcher) return;
    dispatcher->captured = NULL;
    // 禁用 SDL 鼠标捕获
    SDL_CaptureMouse(SDL_FALSE);
}
