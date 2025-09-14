#pragma once
#include "../common/common.h"
typedef struct Units_NightElfUpgradeFunc {
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Resm;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Resw;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rema;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rerh;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Reuv;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Renb;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Reib;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Remk;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Resc;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Remg;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Redt;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Redc;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Resi;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Reht;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Recb;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Repb;
} Units_NightElfUpgradeFunc;

void Init_Units_NightElfUpgradeFunc(Units_NightElfUpgradeFunc *config);
void Shutdown_Units_NightElfUpgradeFunc(Units_NightElfUpgradeFunc *config);

void Init_Units_NightElfUpgradeFunc(Units_NightElfUpgradeFunc *config) {
    config->Resm.Requires2 = "etoe"; 
    config->Resm.Requires1 = "etoa"; 
    config->Resm.Requires = ""; 
    config->Resm.Requirescount = 3; 
    config->Resm.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Resm.Art = "ReplaceableTextures\\CommandButtons\\BTNStrengthOfTheMoon.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedStrengthOfTheMoon.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedStrengthOfTheMoon.blp"; 
    config->Resw.Requires2 = "etoe"; 
    config->Resw.Requires1 = "etoa"; 
    config->Resw.Requires = ""; 
    config->Resw.Requirescount = 3; 
    config->Resw.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->Resw.Art = "ReplaceableTextures\\CommandButtons\\BTNStrengthOfTheWild.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedStrengthOfTheWild.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedStrengthOfTheWild.blp"; 
    config->Rema.Requires2 = "etoe"; 
    config->Rema.Requires1 = "etoa"; 
    config->Rema.Requires = ""; 
    config->Rema.Requirescount = 3; 
    config->Rema.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Rema.Art = "ReplaceableTextures\\CommandButtons\\BTNMoonArmor.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedMoonArmor.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedMoonArmor.blp"; 
    config->Rerh.Requires2 = "etoe"; 
    config->Rerh.Requires1 = "etoa"; 
    config->Rerh.Requires = ""; 
    config->Rerh.Requirescount = 3; 
    config->Rerh.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Rerh.Art = "ReplaceableTextures\\CommandButtons\\BTNReinforcedHides.blp,ReplaceableTextures\\CommandButtons\\BTNImprovedReinforcedHides.blp,ReplaceableTextures\\CommandButtons\\BTNAdvancedReinforcedHides.blp"; 
    config->Reuv.Buttonpos = &MAKE(VECTOR3,2,0); 
    config->Reuv.Art = "ReplaceableTextures\\CommandButtons\\BTNUltravision.blp"; 
    config->Renb.Requires = "etoe"; 
    config->Renb.Buttonpos = &MAKE(VECTOR3,3,0); 
    config->Renb.Art = "ReplaceableTextures\\CommandButtons\\BTNNaturesBlessing.blp"; 
    config->Reib.Requires = "etoa"; 
    config->Reib.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Reib.Art = "ReplaceableTextures\\CommandButtons\\BTNImprovedBows.blp"; 
    config->Remk.Requires = "edob,etoe"; 
    config->Remk.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Remk.Art = "ReplaceableTextures\\CommandButtons\\BTNMarksmanship.blp"; 
    config->Resc.Requires = "edob"; 
    config->Resc.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Resc.Art = "ReplaceableTextures\\CommandButtons\\BTNSentinel.blp"; 
    config->Remg.Requires = "edob,etoe"; 
    config->Remg.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Remg.Art = "ReplaceableTextures\\CommandButtons\\BTNUpgradeMoonGlaive.blp"; 
    config->Redt.Requires1 = "etoe"; 
    config->Redt.Requires = ""; 
    config->Redt.Requirescount = 2; 
    config->Redt.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Redt.Art = "ReplaceableTextures\\CommandButtons\\BTNDOTAdeptTraining.blp,ReplaceableTextures\\CommandButtons\\BTNDOTMasterTraining.blp"; 
    config->Redc.Requires1 = "etoe"; 
    config->Redc.Requires = ""; 
    config->Redc.Requirescount = 2; 
    config->Redc.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Redc.Art = "ReplaceableTextures\\CommandButtons\\BTNDOCAdeptTraining.blp,ReplaceableTextures\\CommandButtons\\BTNDOCMasterTraining.blp"; 
    config->Resi.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Resi.Art = "ReplaceableTextures\\CommandButtons\\BTNDryadDispelMagic.blp"; 
    config->Reht.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Reht.Art = "ReplaceableTextures\\CommandButtons\\BTNTameHippogriff.blp"; 
    config->Recb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Recb.Art = "ReplaceableTextures\\CommandButtons\\BTNCorrosiveBreath.blp"; 
    config->Repb.Requires = "etoa,edob"; 
    config->Repb.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Repb.Art = "ReplaceableTextures\\CommandButtons\\BTNImpalingBolt.blp"; 
}

