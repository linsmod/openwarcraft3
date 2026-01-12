#include "input_converter.h"
#include "common/shared.h"
#include "r_local.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_keyboard.h>
#include <string.h>

bool ConvertSDLEvent(SDL_Event *sdl_event, event_t *output) {
    if (!sdl_event || !output) {
        return false;
    }
    
    memset(output, 0, sizeof(event_t));
    VECTOR2 scale = R_GetDisplayScale();
    switch (sdl_event->type) {
        case SDL_QUIT:
            output->type = INPUT_EVENT_QUIT;
            return true;
            
        case SDL_KEYDOWN:
            output->type = INPUT_EVENT_KEY_DOWN;
            output->key.key = sdl_event->key.keysym.sym;
            output->key.down = true;
            return true;
            
        case SDL_KEYUP:
            output->type = INPUT_EVENT_KEY_UP;
            output->key.key = sdl_event->key.keysym.sym;
            output->key.down = false;
            return true;
            
        case SDL_MOUSEBUTTONDOWN:
            output->type = INPUT_EVENT_MOUSE_DOWN;
            output->mouse.button = sdl_event->button.button;
            output->mouse.x = sdl_event->button.x * 1.0 / scale.x;
            output->mouse.y =  sdl_event->button.y * 1.0 / scale.y;
            output->mouse.down = true;
            return true;
            
        case SDL_MOUSEBUTTONUP:
            output->type = INPUT_EVENT_MOUSE_UP;
            output->mouse.button = sdl_event->button.button;
            output->mouse.x = sdl_event->button.x * 1.0 / scale.x;
            output->mouse.y = sdl_event->button.y * 1.0 / scale.y;
            output->mouse.down = false;
            return true;
            
        case SDL_MOUSEMOTION:
            output->type = INPUT_EVENT_MOUSE_MOTION;
            output->motion.x = sdl_event->motion.x* 1.0 / scale.x;
            output->motion.y =  sdl_event->motion.y * 1.0 / scale.y;
            output->motion.dx = sdl_event->motion.xrel * 1.0 / scale.x;
            output->motion.dy = sdl_event->motion.yrel * 1.0 / scale.y;
            return true;
            
        case SDL_MOUSEWHEEL:
            output->type = INPUT_EVENT_MOUSE_WHEEL;
            output->wheel.delta = (float)sdl_event->wheel.y;
            output->timestamp = SDL_GetTicks();  // 设置时间戳
            // 滚轮事件本身不包含鼠标位置，需要单独获取
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            output->wheel.x = mouse_x * 1.0 / scale.x;
            output->wheel.y =  mouse_y * 1.0 / scale.y;
            // 获取键盘修饰键状态
            SDL_Keymod keymod = SDL_GetModState();
            output->wheel.modifiers = 0;
            if (keymod & KMOD_SHIFT)   output->wheel.modifiers |= KEY_MODIFIER_SHIFT;
            if (keymod & KMOD_CTRL)    output->wheel.modifiers |= KEY_MODIFIER_CTRL;
            if (keymod & KMOD_ALT)     output->wheel.modifiers |= KEY_MODIFIER_ALT;
            if (keymod & KMOD_GUI)     output->wheel.modifiers |= KEY_MODIFIER_SUPER;
            return true;
            
        case SDL_TEXTINPUT:
            output->type = INPUT_EVENT_TEXT_INPUT;
            strncpy(output->text.text, sdl_event->text.text, 31);
            output->text.text[31] = '\0';
            printf("ConvertSDLEvent: TEXTINPUT text='%s'\n", output->text.text);
            return true;

        case SDL_TEXTEDITING:
            output->type = INPUT_EVENT_TEXT_EDITING;
            strncpy(output->editing.text, sdl_event->edit.text, 31);
            output->editing.text[31] = '\0';
            output->editing.start = sdl_event->edit.start;
            output->editing.length = sdl_event->edit.length;
            printf("ConvertSDLEvent: TEXTEDITING text='%s', start=%d, length=%d\n",
                   output->editing.text, output->editing.start, output->editing.length);
            return true;
        case SDL_WINDOWEVENT:
        case SDL_KEYMAPCHANGED:
            break;
        // 可以添加更多SDL事件类型的转换
        default:
            // 忽略其他事件
            return false;
    }
}