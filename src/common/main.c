#include "../client/client.h"
#include "../server/server.h"
#include "../map_select/map_select.h"

#include <SDL2/SDL.h>

#define USAGE \
"Usage:\n" \
"  openwarcraft3 -mpq=<full path to MPQ file> [-map=<path to map inside MPQ>]\n"\
"\n" \
"Examples:\n" \
"  openwarcraft3 -mpq=/Users/John/War3.mpq -map=Maps\\Campaign\\Human02.w3m\n" \
"  openwarcraft3 -mpq=/Users/John/War3.mpq  # Will show map selection screen\n" \
"\n" \
"Notes:\n" \
"  - The MPQ path must be an absolute path on your filesystem.\n" \
"  - The map path must use the internal path format from MPQ.\n" \
"  - If -map is not specified, a map selection screen will be shown.\n"

extern LPTEXTURE Texture;

HANDLE FS_AddArchive(LPCSTR);

void Sys_Quit(void) {
    exit(0);
}
int html_init(LPCSTR filename);
#include "canvas2d/canvas2d.h"
int main(int argc, LPSTR argv[]) {
    char *map = NULL;
    BOOL mpq = 0;
    
    for (int i = 0; i < argc; i++) {
        if (!strncmp(argv[i], "-mpq=", 5)) {
            FS_AddArchive(argv[i]+5);
            mpq = 1;
        }
        if (!strncmp(argv[i], "-map=", 5)) {
            map = argv[i]+5;
        }
    }
    
    // 检查 MPQ 参数
    if (!mpq) {
        printf(USAGE);
        return 1;
    }
    
    // 初始化游戏系统
    Com_Init();
    html_init("../html_tests/test_enhanced_css.html");
    
    // 检查是否提供了地图参数
    if (!map) {
        // 没有提供地图，显示地图选择界面
        printf("No map specified, showing map selection screen...\n");
        
        if (MapSelect_Init() != 0) {
            printf("Failed to initialize map selection screen\n");
            return 1;
        }
        
        // 地图选择界面主循环
        while (!map) {
            DWORD startTime = SDL_GetTicks();
            
            // 处理 SDL 事件
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    MapSelect_Shutdown();
                    return 0;
                }
                else if (event.type == SDL_KEYDOWN) {
                    MapSelect_HandleInput(event.key.keysym.sym, true);
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    // 处理鼠标事件
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // 更新鼠标状态
                        mouse.origin.x = event.button.x;
                        mouse.origin.y = event.button.y;
                        mouse.button = event.button.button;
                        mouse.event = UI_LEFT_MOUSE_DOWN;
                        MapSelect_HandleMouseEvent();
                    }
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    // 更新鼠标位置
                    mouse.origin.x = event.motion.x;
                    mouse.origin.y = event.motion.y;
                }
            }
            
            // 检查是否选择了地图
            if (!map) {
                 const char* startMap = MapSelect_GetStartMap();
                if(startMap)
                    map = strdup(startMap);
            }
            
            // 更新和渲染
            DWORD currentTime = SDL_GetTicks();
            DWORD msec = currentTime - startTime;
            
            // 渲染屏幕
            SCR_UpdateScreen();
            
            // 帧率控制
            if (msec < 16) {
                SDL_Delay(16 - msec);
            }
        }
        
        // 清理地图选择界面
        MapSelect_Shutdown();
    }
    
    // 加载选中的地图
    SV_Map(map);
    // canvas2d_runtest();
    DWORD startTime = SDL_GetTicks();
    while (true) {
        DWORD currentTime = SDL_GetTicks();
        DWORD msec = currentTime - startTime;
        // Cap the frame rate to 60 FPS
        if (msec < 16) {
            SDL_Delay(16 - msec);
            currentTime = SDL_GetTicks();
            msec = currentTime - startTime;
        }

        SV_Frame(msec);
        CL_Frame(msec);
        startTime = currentTime;
    }
    
    return 0;
}
