#include "client.h"

#include <SDL.h>
#include <stdio.h>

//struct {
//    LPCSTR command;
//    DWORD key;
//} hotkey_t;
//
//hotkey_t hotkeys = {
//    { "show_quests",  }
//};

mouseEvent_t mouse;

static void pan_camera(float x, float y, float sensivity) {
    cl.viewDef.camerastate->origin.x += x * sensivity;
    cl.viewDef.camerastate->origin.y += y * sensivity;
    fprintf(stdout, "motion delta %2f,%2f\n", x,y);
    // fprintf(stdout, "cam_distance %2f\n", cl.viewDef.camerastate->distance);
    // fprintf(stdout, "fov %2f\n", cl.viewDef.camerastate->fov);
    fprintf(stdout, " cam_origin %2f %2f %2f\n", cl.viewDef.camerastate->origin.x, cl.viewDef.camerastate->origin.y, cl.viewDef.camerastate->origin.z);
    // MSG_WriteByte(&cls.netchan.message, clc_move);
    // MSG_WriteShort(&cls.netchan.message, x * sensivity);
    // MSG_WriteShort(&cls.netchan.message, y * sensivity);
}

void CL_Input(void) {
    static int moved = false;
    SDL_Event event;
    mouse.event = UI_EVENT_NONE;
    while(SDL_PollEvent(&event)) {
        DWORD mousevt = K_MOUSE1 + event.button.button - 1;
        switch(event.type) {
            case SDL_MOUSEBUTTONDOWN:
                mouse.origin.x = event.button.x;
                mouse.origin.y = event.button.y;
                Key_Event(mousevt, true, event.button.timestamp);
                break;
            case SDL_MOUSEBUTTONUP:
                mouse.origin.x = event.button.x;
                mouse.origin.y = event.button.y;
                Key_Event(mousevt, false, event.button.timestamp);
                break;
            case SDL_MOUSEMOTION:
                mouse.origin.x = event.motion.x;
                mouse.origin.y = event.motion.y;
                break;
        }
        
        switch(event.type) {
            case SDL_KEYDOWN:
                Key_Event(event.key.keysym.sym, true, event.key.timestamp);
                break;
            case SDL_KEYUP:
                Key_Event(event.key.keysym.sym, false, event.key.timestamp);
//                if(event.key.keysym.sym == SDLK_ESCAPE) {
//                    return Com_Quit();
//                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                moved = false;
                mouse.origin.x = event.button.x;
                mouse.origin.y = event.button.y;
                mouse.button = event.button.button;
                switch (event.button.button) {
                    case 1:
                        mouse.event = UI_LEFT_MOUSE_DOWN;
                        break;
                    case 2:
                        mouse.event = UI_RIGHT_MOUSE_DOWN;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouse.origin.x = event.button.x;
                mouse.origin.y = event.button.y;
                mouse.button = 0;
                switch (event.button.button) {
                    case 1:
                        mouse.event = UI_LEFT_MOUSE_UP;
                        break;
                    case 2:
                        mouse.event = UI_RIGHT_MOUSE_UP;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                mouse.origin.x = event.motion.x;
                mouse.origin.y = event.motion.y;
                switch (mouse.button) {
                    case 1:
                        cl.selection.rect.w = event.motion.x - cl.selection.rect.x;
                        cl.selection.rect.h = event.motion.y - cl.selection.rect.y;
                        moved = true;
                        break;
                    case 3:
                        moved = true;
                        pan_camera(-event.motion.xrel, event.motion.yrel, 1);
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        return Com_Quit();
                    default:
                        break;
                }
                break;
        }
    }
//    cl.viewDef.camerastate->origin.z = CM_GetHeightAtPoint(cl.viewDef.camerastate->origin.x, cl.viewDef.camerastate->origin.y);
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
