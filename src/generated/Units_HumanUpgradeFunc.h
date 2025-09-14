#pragma once
#include "../common/common.h"
typedef struct Units_HumanUpgradeFunc {
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhme;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhra;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhar;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhla;
    union {
        LPCSTR Requires2;
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhac;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhgb;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhlh;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhde;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhan;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhpt;
    union {
        LPCSTR Requires1;
        LPCSTR Requires;
        float Requirescount;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhst;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhri;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhse;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhfl;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Rhhb;
} Units_HumanUpgradeFunc;

void Init_Units_HumanUpgradeFunc(Units_HumanUpgradeFunc *config);
void Shutdown_Units_HumanUpgradeFunc(Units_HumanUpgradeFunc *config);

void Init_Units_HumanUpgradeFunc(Units_HumanUpgradeFunc *config) {
    config->Rhme.Requires2 = "hcas"; 
    config->Rhme.Requires1 = "hkee"; 
    config->Rhme.Requires = ""; 
    config->Rhme.Requirescount = 3; 
    config->Rhme.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Rhme.Art = "ReplaceableTextures\\CommandButtons\\BTNSteelMelee.blp,ReplaceableTextures\\CommandButtons\\BTNThoriumMelee.blp,ReplaceableTextures\\CommandButtons\\BTNArcaniteMelee.blp"; 
    config->Rhra.Requires2 = "hcas"; 
    config->Rhra.Requires1 = "hkee"; 
    config->Rhra.Requires = ""; 
    config->Rhra.Requirescount = 3; 
    config->Rhra.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->Rhra.Art = "ReplaceableTextures\\CommandButtons\\BTNHumanMissileUpOne.blp,ReplaceableTextures\\CommandButtons\\BTNHumanMissileUpTwo.blp,ReplaceableTextures\\CommandButtons\\BTNHumanMissileUpThree.blp"; 
    config->Rhar.Requires2 = "hcas"; 
    config->Rhar.Requires1 = "hkee"; 
    config->Rhar.Requires = ""; 
    config->Rhar.Requirescount = 3; 
    config->Rhar.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->Rhar.Art = "ReplaceableTextures\\CommandButtons\\BTNHumanArmorUpOne.blp,ReplaceableTextures\\CommandButtons\\BTNHumanArmorUpTwo.blp,ReplaceableTextures\\CommandButtons\\BTNHumanArmorUpThree.blp"; 
    config->Rhla.Requires2 = "hcas"; 
    config->Rhla.Requires1 = "hkee"; 
    config->Rhla.Requires = ""; 
    config->Rhla.Requirescount = 3; 
    config->Rhla.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Rhla.Art = "ReplaceableTextures\\CommandButtons\\BTNLeatherUpgradeOne.blp,ReplaceableTextures\\CommandButtons\\BTNLeatherUpgradeTwo.blp,ReplaceableTextures\\CommandButtons\\BTNLeatherUpgradeThree.blp"; 
    config->Rhac.Requires2 = "hcas"; 
    config->Rhac.Requires1 = "hkee"; 
    config->Rhac.Requires = ""; 
    config->Rhac.Requirescount = 3; 
    config->Rhac.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->Rhac.Art = "ReplaceableTextures\\CommandButtons\\BTNStoneArchitecture.blp,ReplaceableTextures\\CommandButtons\\BTNArcaniteArchitecture.blp,ReplaceableTextures\\CommandButtons\\BTNImbuedMasonry.blp"; 
    config->Rhgb.Requires = "hcas"; 
    config->Rhgb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rhgb.Art = "ReplaceableTextures\\CommandButtons\\BTNHumanArtilleryUpOne.blp"; 
    config->Rhlh.Requires1 = "hcas"; 
    config->Rhlh.Requires = "hkee"; 
    config->Rhlh.Requirescount = 2; 
    config->Rhlh.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Rhlh.Art = "ReplaceableTextures\\CommandButtons\\BTNHumanLumberUpgrade1.blp,ReplaceableTextures\\CommandButtons\\BTNHumanLumberUpgrade2.blp"; 
    config->Rhde.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rhde.Art = "ReplaceableTextures\\CommandButtons\\BTNDefend.blp"; 
    config->Rhan.Requires = "hlum,hcas,hbla"; 
    config->Rhan.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Rhan.Art = "ReplaceableTextures\\CommandButtons\\BTNAnimalWarTraining.blp"; 
    config->Rhpt.Requires1 = "hcas"; 
    config->Rhpt.Requires = ""; 
    config->Rhpt.Requirescount = 2; 
    config->Rhpt.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rhpt.Art = "ReplaceableTextures\\CommandButtons\\BTNPriestAdept.blp,ReplaceableTextures\\CommandButtons\\BTNPriestMaster.blp"; 
    config->Rhst.Requires1 = "hcas"; 
    config->Rhst.Requires = ""; 
    config->Rhst.Requirescount = 2; 
    config->Rhst.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rhst.Art = "ReplaceableTextures\\CommandButtons\\BTNSorceressAdept.blp,ReplaceableTextures\\CommandButtons\\BTNSorceressMaster.blp"; 
    config->Rhri.Requires = "harm"; 
    config->Rhri.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rhri.Art = "ReplaceableTextures\\CommandButtons\\BTNDwarvenLongRifle.blp"; 
    config->Rhse.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Rhse.Art = "ReplaceableTextures\\CommandButtons\\BTNMagicalSentry.blp"; 
    config->Rhfl.Requires = "hkee"; 
    config->Rhfl.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Rhfl.Art = "ReplaceableTextures\\CommandButtons\\BTNFlare.blp"; 
    config->Rhhb.Requires = ""; 
    config->Rhhb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Rhhb.Art = "ReplaceableTextures\\CommandButtons\\BTNStormHammer.blp"; 
}

