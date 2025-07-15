#include "../client/client.h"
#include "../server/server.h"

#include "SDL.h"

extern LPTEXTURE Texture;

void Sys_Quit(void) {
    exit(0);
}

int main(int argc, LPSTR argv[]) {
    Com_Init("/work/war3files");
    
    SV_Map("Maps\\Campaign\\Human01.w3m");
    // SV_Map("(2)BootyBay.w3m");
    
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
