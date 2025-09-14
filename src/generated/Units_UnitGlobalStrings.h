#pragma once
#include "../common/common.h"
typedef struct Units_UnitGlobalStrings {
    union {
        LPCSTR MechanicalClass;
        LPCSTR UndeadClass;
        LPCSTR GiantClass;
    } Categories;
} Units_UnitGlobalStrings;

void Init_Units_UnitGlobalStrings(Units_UnitGlobalStrings *config);
void Shutdown_Units_UnitGlobalStrings(Units_UnitGlobalStrings *config);

void Init_Units_UnitGlobalStrings(Units_UnitGlobalStrings *config) {
    config->Categories.MechanicalClass = "机械型"; 
    config->Categories.UndeadClass = "不死型"; 
    config->Categories.GiantClass = "巨型"; 
}

