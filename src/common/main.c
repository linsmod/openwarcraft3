#include "../client/client.h"
#include "../server/server.h"

#include "SDL.h"

extern LPTEXTURE Texture;

void Sys_Quit(void) {
    exit(0);
}

int main(int argc, LPSTR argv[]) {
    if(Com_Init("/work/war3files")!=0){
        return -1;
    }
    
    if(!SV_Map("Maps\\Campaign\\Human02.w3m")){
        return -1;
    }
    
    DWORD startTime = SDL_GetTicks();
    
    while (true) {
        DWORD currentTime = SDL_GetTicks();
        DWORD msec = currentTime - startTime;
        SV_Frame(msec);
        CL_Frame(msec);
        startTime = currentTime;
    }

    return 0;
}
