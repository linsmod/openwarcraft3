#pragma once
#include "../common/common.h"
typedef struct Units_UndeadUpgradeFunc {
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rume;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rura;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Ruar;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rucr;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Ruac;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rugf;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Ruwb;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rusf;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rune;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Ruba;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rufb;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rusl;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rupc;
} Units_UndeadUpgradeFunc;

void Init_Units_UndeadUpgradeFunc(Units_UndeadUpgradeFunc *config);
void Shutdown_Units_UndeadUpgradeFunc(Units_UndeadUpgradeFunc *config);

void Init_Units_UndeadUpgradeFunc(Units_UndeadUpgradeFunc *config) {
    config->Rume.Requires2 = "unp2"; 
    config->Rume.Requires1 = "unp1"; 
    config->Rume.Requires = ""; 
    config->Rume.Requirescount = 3; 
    config->Rume.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Rume.Art = "ReplaceableTextures\\CommandButtons\\BTNUnholyStrength.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedUnholyStrength.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedUnholyStrength.blp"; 
    config->Rura.Requires2 = "unp2"; 
    config->Rura.Requires1 = "unp1"; 
    config->Rura.Requires = ""; 
    config->Rura.Requirescount = 3; 
    config->Rura.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->Rura.Art = "ReplaceableTextures\\CommandButtons\\BTNCreatureAttack.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedCreatureAttack.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedCreatureAttack.blp"; 
    config->Ruar.Requires2 = "unp2"; 
    config->Ruar.Requires1 = "unp1"; 
    config->Ruar.Requires = ""; 
    config->Ruar.Requirescount = 3; 
    config->Ruar.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Ruar.Art = "ReplaceableTextures\\CommandButtons\\BTNUnholyArmor.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedUnholyArmor.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedUnholyArmor.blp"; 
    config->Rucr.Requires2 = "unp2"; 
    config->Rucr.Requires1 = "unp1"; 
    config->Rucr.Requires = ""; 
    config->Rucr.Requirescount = 3; 
    config->Rucr.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Rucr.Art = "ReplaceableTextures\\CommandButtons\\BTNCreatureCarapace.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedCreatureCarapace.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedCreatureCarapace.blp"; 
    config->Ruac.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Ruac.Art = "ReplaceableTextures\\CommandButtons\\BTNCannibalize.blp"; 
    config->Rugf.Requires = "ugrv,unp2"; 
    config->Rugf.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rugf.Art = "ReplaceableTextures\\CommandButtons\\BTNGhoulFrenzy.blp"; 
    config->Ruwb.Requires = "ugrv,unp1"; 
    config->Ruwb.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Ruwb.Art = "ReplaceableTextures\\CommandButtons\\BTNWeb.blp"; 
    config->Rusf.Requires = "ugrv,unp2"; 
    config->Rusf.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Rusf.Art = "ReplaceableTextures\\CommandButtons\\BTNStoneForm.blp"; 
    config->Rune.Requires1 = "unp2"; 
    config->Rune.Requires = ""; 
    config->Rune.Requirescount = 2; 
    config->Rune.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rune.Art = "ReplaceableTextures\\CommandButtons\\BTNNecromancerAdept.blp,ReplaceableTextures\\CommandButtons\\BTNNecromancerMaster.blp"; 
    config->Ruba.Requires1 = "unp2"; 
    config->Ruba.Requires = ""; 
    config->Ruba.Requirescount = 2; 
    config->Ruba.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Ruba.Art = "ReplaceableTextures\\CommandButtons\\BTNBansheeAdept.blp,ReplaceableTextures\\CommandButtons\\BTNBansheeMaster.blp"; 
    config->Rufb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rufb.Art = "ReplaceableTextures\\CommandButtons\\BTNFreezingBreath.blp"; 
    config->Rusl.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Rusl.Art = "ReplaceableTextures\\CommandButtons\\BTNSkeletalLongevity.blp"; 
    config->Rupc.Requires = "unp2"; 
    config->Rupc.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rupc.Art = "ReplaceableTextures\\CommandButtons\\BTNPlagueCloud.blp"; 
}

