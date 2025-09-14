#pragma once
#include "../common/common.h"
typedef struct UI_SoundInfo_MiscData {
    union {
        float ListenerMaxCinematicDistance;
        LPVECTOR3 ListenerAOA;
        LPVECTOR3 ListenerDistance;
    } Listener;
} UI_SoundInfo_MiscData;

void Init_UI_SoundInfo_MiscData(UI_SoundInfo_MiscData *config);
void Shutdown_UI_SoundInfo_MiscData(UI_SoundInfo_MiscData *config);

void Init_UI_SoundInfo_MiscData(UI_SoundInfo_MiscData *config) {
    config->Listener.ListenerMaxCinematicDistance = 1500; 
    config->Listener.ListenerAOA = &MAKE(VECTOR3,85,70,55,45,35,25); 
    config->Listener.ListenerDistance = &MAKE(VECTOR3,1700,1550,1400,1200,1000,800); 
}

