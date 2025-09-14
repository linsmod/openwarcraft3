#pragma once
#include "../common/common.h"
typedef struct Units_OrcUpgradeFunc {
    union {
        LPCSTR Art;
    } Roch;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rome;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rora;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Roar;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rwdm;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Ropg;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Robs;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rows;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Roen;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rovs;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rowd;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rost;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rosp;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rotr;
} Units_OrcUpgradeFunc;

void Init_Units_OrcUpgradeFunc(Units_OrcUpgradeFunc *config);
void Shutdown_Units_OrcUpgradeFunc(Units_OrcUpgradeFunc *config);

void Init_Units_OrcUpgradeFunc(Units_OrcUpgradeFunc *config) {
    config->Roch.Art = "ReplaceableTextures\\CommandButtons\\BTNOrcMeleeUpOne.blp"; 
    config->Rome.Requires2 = "ofrt"; 
    config->Rome.Requires1 = "ostr"; 
    config->Rome.Requires = ""; 
    config->Rome.Requirescount = 3; 
    config->Rome.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Rome.Art = "ReplaceableTextures\\CommandButtons\\BTNOrcMeleeUpOne.blp,ReplaceableTextures\\CommandButtons\\BTNOrcMeleeUpTwo.blp,ReplaceableTextures\\CommandButtons\\BTNOrcMeleeUpThree.blp"; 
    config->Rora.Requires2 = "ofrt"; 
    config->Rora.Requires1 = "ostr"; 
    config->Rora.Requires = ""; 
    config->Rora.Requirescount = 3; 
    config->Rora.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->Rora.Art = "ReplaceableTextures\\CommandButtons\\BTNSteelRanged.blp,ReplaceableTextures\\CommandButtons\\BTNThoriumRanged.blp,ReplaceableTextures\\CommandButtons\\BTNArcaniteRanged.blp"; 
    config->Roar.Requires2 = "ofrt"; 
    config->Roar.Requires1 = "ostr"; 
    config->Roar.Requires = ""; 
    config->Roar.Requirescount = 3; 
    config->Roar.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Roar.Art = "ReplaceableTextures\\CommandButtons\\BTNSteelArmor.blp,ReplaceableTextures\\CommandButtons\\BTNThoriumArmor.blp,ReplaceableTextures\\CommandButtons\\BTNArcaniteArmor.blp"; 
    config->Rwdm.Requires = "ofrt,ofor"; 
    config->Rwdm.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rwdm.Art = "ReplaceableTextures\\CommandButtons\\BTNDrum.blp"; 
    config->Ropg.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Ropg.Art = "ReplaceableTextures\\CommandButtons\\BTNPillage.blp"; 
    config->Robs.Requires = "ofrt"; 
    config->Robs.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Robs.Art = "ReplaceableTextures\\CommandButtons\\BTNBerserk.blp"; 
    config->Rows.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rows.Art = "ReplaceableTextures\\CommandButtons\\BTNSmash.blp"; 
    config->Roen.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Roen.Art = "ReplaceableTextures\\CommandButtons\\BTNEnsnare.blp"; 
    config->Rovs.Requires = "ofrt"; 
    config->Rovs.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Rovs.Art = "ReplaceableTextures\\CommandButtons\\BTNEnvenomedSpear.blp"; 
    config->Rowd.Requires1 = "ofrt"; 
    config->Rowd.Requires = ""; 
    config->Rowd.Requirescount = 2; 
    config->Rowd.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rowd.Art = "ReplaceableTextures\\CommandButtons\\BTNWitchDoctorAdept.blp,ReplaceableTextures\\CommandButtons\\BTNWitchDoctorMaster.blp"; 
    config->Rost.Requires1 = "ofrt"; 
    config->Rost.Requires = ""; 
    config->Rost.Requirescount = 2; 
    config->Rost.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rost.Art = "ReplaceableTextures\\CommandButtons\\BTNShamanAdept.blp,ReplaceableTextures\\CommandButtons\\BTNShamanMaster.blp"; 
    config->Rosp.Requires2 = "ofrt"; 
    config->Rosp.Requires1 = "ostr"; 
    config->Rosp.Requires = ""; 
    config->Rosp.Requirescount = 3; 
    config->Rosp.Buttonpos = &MAKE(VECTOR3,2,0); 
    config->Rosp.Art = "ReplaceableTextures\\CommandButtons\\BTNSpikedBarricades.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedSpikedBarricades.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedSpikedBarricades.blp"; 
    config->Rotr.Requires = "ostr,ofor"; 
    config->Rotr.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rotr.Art = "ReplaceableTextures\\CommandButtons\\BTNRegenerate.blp"; 
}

