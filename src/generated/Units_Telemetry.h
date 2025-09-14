#pragma once
#include "../common/common.h"
typedef struct Units_Telemetry {
    union {
        LPCSTR u;
        float t;
        LPCSTR s;
        LPCSTR r;
        float q;
        LPCSTR p;
        float o;
        LPCSTR n;
        float m;
        LPCSTR l;
        LPCSTR k;
        float j;
        LPCSTR i;
        LPCSTR h;
        float g;
        float f;
        LPCSTR e;
        LPCSTR d;
        LPCSTR c;
        LPCSTR b;
        LPCSTR a;
    } Telemetry;
} Units_Telemetry;

void Init_Units_Telemetry(Units_Telemetry *config);
void Shutdown_Units_Telemetry(Units_Telemetry *config);

void Init_Units_Telemetry(Units_Telemetry *config) {
    config->Telemetry.u = "-1019274656927319569"; 
    config->Telemetry.t = 6169467044321463317; 
    config->Telemetry.s = "-6170793653628190315"; 
    config->Telemetry.r = "-2382913011035714490"; 
    config->Telemetry.q = 2831672247888008556; 
    config->Telemetry.p = "-6123244557153587235"; 
    config->Telemetry.o = 4192987606572922140; 
    config->Telemetry.n = "-446920761596970885"; 
    config->Telemetry.m = 8079072272279897596; 
    config->Telemetry.l = "-5801382533913064927"; 
    config->Telemetry.k = "-3879491486085606535"; 
    config->Telemetry.j = 8514950617083462119; 
    config->Telemetry.i = "-1232246075420074842"; 
    config->Telemetry.h = "-7984411119628152567"; 
    config->Telemetry.g = 793747866433114248; 
    config->Telemetry.f = 2354477290916240923; 
    config->Telemetry.e = "-872748036714025544"; 
    config->Telemetry.d = "-8008807713700409405"; 
    config->Telemetry.c = "-930154931662156553"; 
    config->Telemetry.b = "-3304013799902092087"; 
    config->Telemetry.a = "-6968993115968202010"; 
}

