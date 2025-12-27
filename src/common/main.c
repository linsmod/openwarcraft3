#include "../client/client.h"
#include "../server/server.h"
#include "../map_select/map_select.h"
#include "../client/cl_game_scene.h"
#include "../splash/splash_scene.h"
#include "scene.h"
#include "canvas2d/canvas2d.h"

#include <SDL2/SDL.h>

#define USAGE \
"Usage:\n" \
"  openwarcraft3 -mpq=<full path to MPQ file> [-map=<path to map inside MPQ>]\n" \
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
void Sys_Quit(void) { exit(0); }
int html_init(LPCSTR filename);

// 将输入事件转换为scene的输入事件类型
static input_event_type_t convert_sdl_event_type(SDL_Event* event) {
    switch (event->type) {
        case SDL_KEYDOWN:
            return INPUT_EVENT_KEY_DOWN;
        case SDL_KEYUP:
            return INPUT_EVENT_KEY_UP;
        case SDL_MOUSEBUTTONDOWN:
            return INPUT_EVENT_MOUSE_DOWN;
        case SDL_MOUSEBUTTONUP:
            return INPUT_EVENT_MOUSE_UP;
        case SDL_MOUSEMOTION:
            return INPUT_EVENT_MOUSE_MOTION;
        case SDL_MOUSEWHEEL:
            return INPUT_EVENT_MOUSE_WHEEL;
        default:
            return INPUT_EVENT_KEY_DOWN; // 默认值
    }
}

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
        FS_AddArchive("/home/wulin/Warcraft-III-1.27a/War3.mpq"); // 默认MPQ路径，方便测试
        // printf(USAGE);
        // return 1;
    }
    // 初始化游戏系统
    Com_Init();
    html_init("../html_tests/splash.html");
    
    // 初始化SceneManager
    scene_manager_t *scene_mgr = SceneManager_Create();
    if (!scene_mgr) {
        printf("Failed to create SceneManager\n");
        return 1;
    }
    
    // 获取Splash、MapSelect和Game场景实例
    scene_t *splash_scene = SplashScene_GetInstance();
    scene_t *map_select_scene = MapSelectScene_GetInstance();
    scene_t *game_scene = GameScene_GetInstance();
    
    // 注册场景到SceneManager
    if (SceneManager_RegisterScene(scene_mgr, splash_scene) != 0) {
        printf("Failed to register Splash scene\n");
        SceneManager_Destroy(scene_mgr);
        return 1;
    }
    
    if (SceneManager_RegisterScene(scene_mgr, map_select_scene) != 0) {
        printf("Failed to register MapSelect scene\n");
        SceneManager_Destroy(scene_mgr);
        return 1;
    }
    
    if (SceneManager_RegisterScene(scene_mgr, game_scene) != 0) {
        printf("Failed to register Game scene\n");
        SceneManager_Destroy(scene_mgr);
        return 1;
    }
    
    // 先启动splash场景，传递参数给splash以决定后续跳转
    // 使用共享参数，因为场景会在其生命周期内保持引用
    scene_params_t *splash_params = SceneParams_CreateShared();
    if (splash_params) {
        if (map) {
            SceneParams_SetString(splash_params, "map_path", map);
            SceneParams_SetString(splash_params, "next_scene", "Game");
        } else {
            SceneParams_SetString(splash_params, "next_scene", "MapSelect");
        }
        
        printf("Starting splash screen...\n");
        SceneManager_SwitchScene(scene_mgr, splash_scene, splash_params);
        // 不销毁共享参数，让程序退出时自动清理
    } else {
        printf("Failed to create splash parameters\n");
        SceneManager_Destroy(scene_mgr);
        return 1;
    }
    
    // 主游戏循环 - 使用SceneManager
    DWORD startTime = SDL_GetTicks();
    while (true) {
        DWORD currentTime = SDL_GetTicks();
        float dt =currentTime - startTime;
        
        // 处理SDL事件并分发给当前场景
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SceneManager_Destroy(scene_mgr);
                return 0;
            }
            
            // 转换SDL事件为scene输入事件
            input_event_t input_event;
            input_event.type = convert_sdl_event_type(&event);
            input_event.handled = false;
            
            switch (input_event.type) {
                case INPUT_EVENT_KEY_DOWN:
                    input_event.key.key = event.key.keysym.sym;
                    input_event.key.down = true;
                    break;
                case INPUT_EVENT_KEY_UP:
                    input_event.key.key = event.key.keysym.sym;
                    input_event.key.down = false;
                    break;
                case INPUT_EVENT_MOUSE_DOWN:
                    input_event.mouse.button = event.button.button;
                    input_event.mouse.x = event.button.x;
                    input_event.mouse.y = event.button.y;
                    input_event.mouse.down = true;
                    break;
                case INPUT_EVENT_MOUSE_UP:
                    input_event.mouse.button = event.button.button;
                    input_event.mouse.x = event.button.x;
                    input_event.mouse.y = event.button.y;
                    input_event.mouse.down = false;
                    break;
                case INPUT_EVENT_MOUSE_MOTION:
                    input_event.motion.x = event.motion.x;
                    input_event.motion.y = event.motion.y;
                    input_event.motion.dx = event.motion.xrel;
                    input_event.motion.dy = event.motion.yrel;
                    break;
                case INPUT_EVENT_MOUSE_WHEEL:
                    input_event.wheel.delta = event.wheel.y;
                    break;
                default:
                    break;
            }
            
            // 分发输入事件给当前场景
            SceneManager_OnInput(scene_mgr, &input_event);
        }
        // 更新当前场景（如果有transition会自动设置到pending）
        SceneManager_Update(scene_mgr, dt);
        
        // 渲染当前场景
        // 注意：SceneManager_Render只是调用scene->render()，不会刷新屏幕
        // 我们需要在这里统一处理屏幕刷新
        
        re.BeginFrame();
        SceneManager_Render(scene_mgr);
        re.EndFrame();
        
        // 帧率控制
        DWORD frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
        
        startTime = currentTime;
    }
    
    // 清理SceneManager
    SceneManager_Destroy(scene_mgr);
    
    return 0;
}
