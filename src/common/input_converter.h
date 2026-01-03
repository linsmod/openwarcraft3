#ifndef __INPUT_CONVERTER_H__
#define __INPUT_CONVERTER_H__

#include "scene.h"
#include <SDL2/SDL_events.h>

// SDL事件转换为统一的event_t
// 返回true表示成功转换，false表示忽略该事件
bool ConvertSDLEvent(SDL_Event *sdl_event, event_t *output);

#endif // __INPUT_CONVERTER_H__
