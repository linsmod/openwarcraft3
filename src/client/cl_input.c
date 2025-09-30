#include "client.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

//struct {
//    LPCSTR command;
//    DWORD key;
//} hotkey_t;
//
//hotkey_t hotkeys = {
//    { "show_quests",  }
//};

mouseEvent_t mouse;

// static void pan_camera(float x, float y, float sensivity) {
//     cl.viewDef.camerastate->origin.x += x * sensivity;
//     cl.viewDef.camerastate->origin.y += y * sensivity;
//     fprintf(stdout, "motion delta %2f,%2f\n", x,y);
//     // fprintf(stdout, "cam_distance %2f\n", cl.viewDef.camerastate->distance);
//     // fprintf(stdout, "fov %2f\n", cl.viewDef.camerastate->fov);
//     fprintf(stdout, " cam_origin %2f %2f %2f\n", cl.viewDef.camerastate->origin.x, cl.viewDef.camerastate->origin.y, cl.viewDef.camerastate->origin.z);
//     // MSG_WriteByte(&cls.netchan.message, clc_move);
//     // MSG_WriteShort(&cls.netchan.message, x * sensivity);
//     // MSG_WriteShort(&cls.netchan.message, y * sensivity);
// }

// 输入处理函数声明
static void CL_HandleMouseButtonDown(SDL_Event* event, int* moved);
static void CL_HandleMouseButtonUp(SDL_Event* event, int* moved);
static void CL_HandleMouseMotion(SDL_Event* event, int* moved);
static void CL_HandleKeyDown(SDL_Event* event);
static void CL_HandleKeyUp(SDL_Event* event);
static void CL_HandleMouseWheel(SDL_Event* event);
static void CL_HandleWindowEvent(SDL_Event* event);

// 鼠标按下事件处理函数声明
static void CL_HandleLeftMouseDown(SDL_Event* event);
static void CL_HandleMiddleMouseDown(SDL_Event* event);
static void CL_HandleRightMouseDown(SDL_Event* event);

// 鼠标释放事件处理函数声明
static void CL_HandleLeftMouseUp(SDL_Event* event, int* moved);
static void CL_HandleMiddleMouseUp(SDL_Event* event, int* moved);
static void CL_HandleRightMouseUp(SDL_Event* event, int* moved);

// 选择相关函数声明
static void CL_HandleSingleSelection(int x, int y);
static void CL_HandleAreaSelection(const RECT* rect);

// 缩放功能函数声明
static void CL_HandleZoom(bool zoomIn);
static float CL_ClampZoomDistance(float distance, float min, float max);

// 网络通信函数声明
static void CL_SendCommand(const char* format, ...);

static void pan_camera(float x, float y, float sensivity) {
    cl.viewDef.camerastate->origin.x += x * sensivity;
    cl.viewDef.camerastate->origin.y += y * sensivity;
    MSG_WriteByte(&cls.netchan.message, clc_move);
    MSG_WriteShort(&cls.netchan.message, x * sensivity);
    MSG_WriteShort(&cls.netchan.message, y * sensivity);
}
void CL_Input(void) {
    static int moved = false;
    SDL_Event event;
    mouse.event = UI_EVENT_NONE;
    
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_MOUSEBUTTONDOWN:
                CL_HandleMouseButtonDown(&event, &moved);
                break;
            case SDL_MOUSEBUTTONUP:
                CL_HandleMouseButtonUp(&event, &moved);
                break;
            case SDL_MOUSEMOTION:
                CL_HandleMouseMotion(&event, &moved);
                break;
            case SDL_KEYDOWN:
                CL_HandleKeyDown(&event);
                break;
            case SDL_KEYUP:
                CL_HandleKeyUp(&event);
                break;
            case SDL_MOUSEWHEEL:
                CL_HandleMouseWheel(&event);
                break;
            case SDL_WINDOWEVENT:
                CL_HandleWindowEvent(&event);
                break;
        }
    }
}

// 鼠标按下事件处理
static void CL_HandleMouseButtonDown(SDL_Event* event, int* moved) {
    DWORD mousevt = K_MOUSE1 + event->button.button - 1;
    *moved = false;
    mouse.origin.x = event->button.x;
    mouse.origin.y = event->button.y;
    Key_Event(mousevt, true, event->button.timestamp);
    mouse.button = event->button.button;
    
    switch (event->button.button) {
        case 1: // Left button
            CL_HandleLeftMouseDown(event);
            break;
        case 2: // Middle button
            CL_HandleMiddleMouseDown(event);
            break;
        case 3: // Right button
            CL_HandleRightMouseDown(event);
            break;
    }
}

// 左键按下处理
static void CL_HandleLeftMouseDown(SDL_Event* event) {
    mouse.event = UI_LEFT_MOUSE_DOWN;
    cl.selection.in_progress = true;
    cl.selection.rect.x = mouse.origin.x;
    cl.selection.rect.y = mouse.origin.y;
    cl.selection.rect.w = 0;
    cl.selection.rect.h = 0;
}

// 中键按下处理
static void CL_HandleMiddleMouseDown(SDL_Event* event) {
    mouse.event = UI_MIDDLE_MOUSE_DOWN;
    // 可以添加中键功能，如拖拽地图等
}

// 右键按下处理
static void CL_HandleRightMouseDown(SDL_Event* event) {
    mouse.event = UI_RIGHT_MOUSE_DOWN;
    // 右键按下时的预备操作
}

// 鼠标释放事件处理
static void CL_HandleMouseButtonUp(SDL_Event* event, int* moved) {
    mouse.origin.x = event->button.x;
    mouse.origin.y = event->button.y;
    mouse.button = 0;
    
    switch (event->button.button) {
        case 1: // Left button
            CL_HandleLeftMouseUp(event, moved);
            break;
        case 2: // Middle button
            CL_HandleMiddleMouseUp(event, moved);
            break;
        case 3: // Right button
            CL_HandleRightMouseUp(event, moved);
            break;
    }
}

// 左键释放处理
static void CL_HandleLeftMouseUp(SDL_Event* event, int* moved) {
    mouse.event = UI_LEFT_MOUSE_UP;
    RECT const r = cl.selection.rect;
    cl.selection.in_progress = false;
    
    // 判断是点击还是框选
    if (fabs(r.w) + fabs(r.h) < 10) {
        CL_HandleSingleSelection(event->button.x, event->button.y);
    } else {
        CL_HandleAreaSelection(&r);
    }
}

// 单个实体选择
static void CL_HandleSingleSelection(int x, int y) {
    DWORD entnum;
    VECTOR3 point;
    
    // 优先选择实体
    if (re.TraceEntity(&cl.viewDef, x, y, &entnum)) {
        CL_SendCommand("select %d", entnum);
    }
    // 其次选择地点
    else if (re.TraceLocation(&cl.viewDef, x, y, &point)) {
        CL_SendCommand("point %d %d", (int)point.x, (int)point.y);
    }
}

// 区域选择处理
static void CL_HandleAreaSelection(const RECT* rect) {
    DWORD selected[MAX_SELECTED_ENTITIES] = { 0 };
    DWORD num = re.EntitiesInRect(&cl.viewDef, rect, MAX_SELECTED_ENTITIES, selected);
    
    if (num == 0) return;
    
    char buffer[1024] = { 0 };
    strcpy(buffer, "select");
    
    FOR_LOOP(i, num) {
        sprintf(buffer + strlen(buffer), " %d", selected[i]);
    }
    
    CL_SendCommand(buffer);
}

// 右键释放处理（War3风格攻击/移动）
static void CL_HandleRightMouseUp(SDL_Event* event, int* moved) {
    mouse.event = UI_RIGHT_MOUSE_UP;
    
    if (*moved) {
        DWORD entnum;
        VECTOR3 point;
        
        // 检查是否右键点击实体（攻击）
        if (re.TraceEntity(&cl.viewDef, event->button.x, event->button.y, &entnum)) {
            CL_SendCommand("attack %d", entnum);
        }
        // 检查是否右键点击地面（移动）
        else if (re.TraceLocation(&cl.viewDef, event->button.x, event->button.y, &point)) {
            VECTOR2 location = { (float)point.x, (float)point.y };
            CL_SendCommand("move %f %f", location.x, location.y);
        }
    }
}

// 中键释放处理
static void CL_HandleMiddleMouseUp(SDL_Event* event, int* moved) {
    mouse.event = UI_MIDDLE_MOUSE_UP;
    // 可以添加中键释放功能
}

// 鼠标移动处理
static void CL_HandleMouseMotion(SDL_Event* event, int* moved) {
    mouse.origin.x = event->motion.x;
    mouse.origin.y = event->motion.y;
    
    switch (mouse.button) {
        case 1: // 左键拖拽选择
            cl.selection.rect.w = event->motion.x - cl.selection.rect.x;
            cl.selection.rect.h = event->motion.y - cl.selection.rect.y;
            *moved = true;
            break;
        case 3: // 右键拖拽相机
            *moved = true;
            pan_camera(-event->motion.xrel, event->motion.yrel, 1);
            break;
    }
}

// 键盘按下处理
static void CL_HandleKeyDown(SDL_Event* event) {
    // War3风格 Page Up/Down 缩放
    if (event->key.keysym.sym == SDLK_PAGEUP) {
        CL_HandleZoom(true); // 放大
    } else if (event->key.keysym.sym == SDLK_PAGEDOWN) {
        CL_HandleZoom(false); // 缩小
    }
    
    Key_Event(event->key.keysym.sym, true, event->key.timestamp);
}

// 键盘释放处理
static void CL_HandleKeyUp(SDL_Event* event) {
    Key_Event(event->key.keysym.sym, false, event->key.timestamp);
}

// 鼠标滚轮处理（War3风格缩放）
static void CL_HandleMouseWheel(SDL_Event* event) {
    if (event->wheel.y != 0) {
        CL_HandleZoom(event->wheel.y > 0); // 向前滚轮放大，向后缩小
    }
}

// 缩放处理
static void CL_HandleZoom(bool zoomIn) {
    float zoom_factor = 1.1f; // 10% 缩放每刻度
    float current_distance = cl.viewDef.camerastate[0].distance;
    float new_distance;
    
    if (zoomIn) {
        // 放大（靠近地面）
        new_distance = current_distance / zoom_factor;
    } else {
        // 缩小（鸟瞰视图）
        new_distance = current_distance * zoom_factor;
    }
    
    // War3风格距离范围
    const float min_distance = 100.0f;   // 最近缩放
    const float max_distance = 3000.0f;  // 最远缩放
    
    // 限制缩放范围
    new_distance = CL_ClampZoomDistance(new_distance, min_distance, max_distance);
    
    // 立即更新本地相机
    cl.viewDef.camerastate[0].distance = new_distance;
    
    // 发送到服务器同步
    CL_SendCommand("zoom %f", new_distance);
}

// 限制缩放距离
static float CL_ClampZoomDistance(float distance, float min, float max) {
    if (distance < min) return min;
    if (distance > max) return max;
    return distance;
}

// 窗口事件处理
static void CL_HandleWindowEvent(SDL_Event* event) {
    switch (event->window.event) {
        case SDL_WINDOWEVENT_CLOSE: // 退出游戏
            Com_Quit();
            break;
        default:
            break;
    }
}

// 发送命令到服务器的辅助函数
static void CL_SendCommand(const char* format, ...) {
    va_list args;
    char buffer[1024];
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    MSG_WriteByte(&cls.netchan.message, clc_stringcmd);
    SZ_Printf(&cls.netchan.message, buffer);
}

void IN_SelectDown(void) {
    cl.selection.in_progress = true;
    cl.selection.rect.x = mouse.origin.x;
    cl.selection.rect.y = mouse.origin.y;
    cl.selection.rect.w = 0;
    cl.selection.rect.h = 0;

    VECTOR2 m = {
        mouse.origin.x * 0.8 / WINDOW_WIDTH,
        mouse.origin.y * 0.6 / WINDOW_HEIGHT
    };
    
    FOR_LOOP(layer_id, MAX_LAYOUT_LAYERS) {
        if (cl.layout[layer_id] == NULL)
            continue;
        LPCUIFRAME frames = SCR_Clear(cl.layout[layer_id]);
        FOR_LOOP(object_id, MAX_LAYOUT_OBJECTS) {
            LPCUIFRAME frame = frames+object_id;
            if (frame->flags.type != FT_TEXTURE)
                continue;
            RECT const screen = Rect_div(SCR_LayoutRect(frame), 10000);
            if (Rect_contains(&screen, &m)) {
                cl.selection.in_progress = false;
            }
        }
    }
}

void IN_SelectUp(void) {
    if (!cl.selection.in_progress)
        return;
    RECT const r = cl.selection.rect;
    cl.selection.in_progress = false;
    DWORD entnum;
    VECTOR3 point;
    if (fabs(r.w)+fabs(r.h) < 10) {
        if (re.TraceEntity(&cl.viewDef, r.x, r.y, &entnum)) {
            MSG_WriteByte(&cls.netchan.message, clc_stringcmd);
            SZ_Printf(&cls.netchan.message, "select %d", entnum);
        }
        if (re.TraceLocation(&cl.viewDef, r.x, r.y, &point)){
            MSG_WriteByte(&cls.netchan.message, clc_stringcmd);
            SZ_Printf(&cls.netchan.message, "point %d %d", (int)point.x, (int)point.y);
        }
    } else {
        DWORD selected[MAX_SELECTED_ENTITIES] = { 0 };
        DWORD num = re.EntitiesInRect(&cl.viewDef, &cl.selection.rect, MAX_SELECTED_ENTITIES, selected);
        char buffer[1024] = { 0 };
        if (num == 0)
            return;
        strcpy(buffer, "select");
        FOR_LOOP(i, num) {
            sprintf(buffer+strlen(buffer), " %d", selected[i]);
        }
        MSG_WriteByte(&cls.netchan.message, clc_stringcmd);
        SZ_Printf(&cls.netchan.message, buffer);
    }
}

void CL_ForwardToServer_f(void) {
    extern LPCSTR current_command;
    MSG_WriteByte(&cls.netchan.message, clc_stringcmd);
    SZ_Printf(&cls.netchan.message, current_command+4);
}

void CL_InitInput(void) {
    Cmd_AddCommand("+select", IN_SelectDown);
    Cmd_AddCommand("-select", IN_SelectUp);
    Cmd_AddCommand("cmd", CL_ForwardToServer_f);
}
