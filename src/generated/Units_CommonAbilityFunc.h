#pragma once
#include "../common/common.h"
typedef struct Units_CommonAbilityFunc {
    union {
        LPCSTR Casterart;
    } AHer;
    union {
        LPCSTR Specialattach;
        LPCSTR Specialart;
    } Amov;
    union {
        LPCSTR Order;
        LPVECTOR3 UnButtonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Ahar;
    union {
        LPCSTR Order;
    } Ahrl;
    union {
        LPCSTR Order;
        LPCSTR Targetart;
    } Arev;
    union {
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
    } Adet;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Adta;
    union {
        LPCSTR Effectart;
    } Xbdt;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bdet;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bvul;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bspe;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bfro;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Arep;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BSTN;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BPSE;
} Units_CommonAbilityFunc;

void Init_Units_CommonAbilityFunc(Units_CommonAbilityFunc *config);
void Shutdown_Units_CommonAbilityFunc(Units_CommonAbilityFunc *config);

void Init_Units_CommonAbilityFunc(Units_CommonAbilityFunc *config) {
    config->AHer.Casterart = "Abilities\\Spells\\Other\\Levelup\\LevelupCaster.mdl"; 
    config->Amov.Specialattach = "origin"; 
    config->Amov.Specialart = "Abilities\\Spells\\Human\\MassTeleport\\MassTeleportTarget.mdl"; 
    config->Ahar.Order = "harvest"; 
    config->Ahar.UnButtonpos = &MAKE(VECTOR3,3,1); 
    config->Ahar.Buttonpos = &MAKE(VECTOR3,3,1); 
    config->Ahar.Unart = "CommandReturnGold"; 
    config->Ahar.Art = "CommandHarvest"; 
    config->Ahrl.Order = "harvest"; 
    config->Arev.Order = "revive"; 
    config->Arev.Targetart = "Abilities\\Spells\\Human\\ReviveHuman\\ReviveHuman.mdl,Abilities\\Spells\\Orc\\ReviveOrc\\ReviveOrc.mdl,Abilities\\Spells\\Undead\\ReviveUndead\\ReviveUndead.mdl,Abilities\\Spells\\NightElf\\ReviveNightElf\\ReviveNightElf.mdl,Abilities\\Spells\\Demon\\ReviveDemon\\ReviveDemon.mdl"; 
    config->Adet.Casterart = ""; 
    config->Adet.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Adta.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Adta.Art = "ReplaceableTextures\\CommandButtons\\BTNReveal.blp"; 
    config->Xbdt.Effectart = "Abilities\\Spells\\Other\\Andt\\Andt.mdl"; 
    config->Bdet.Targetart = ""; 
    config->Bdet.Buffart = ""; 
    config->Bvul.Targetattach = "origin"; 
    config->Bvul.Targetart = "Abilities\\Spells\\Human\\DivineShield\\DivineShieldTarget.mdl"; 
    config->Bvul.Buffart = "ReplaceableTextures\\CommandButtons\\BTNInvulnerable.blp"; 
    config->Bspe.Targetart = "Abilities\\Spells\\Items\\AIsp\\SpeedTarget.mdl"; 
    config->Bspe.Buffart = "ReplaceableTextures\\CommandButtons\\BTNBoots.blp"; 
    config->Bfro.Targetart = "Abilities\\Spells\\Other\\FrostDamage\\FrostDamage.mdl"; 
    config->Bfro.Buffart = "ReplaceableTextures\\CommandButtons\\BTNFrost.blp"; 
    config->Arep.Orderoff = "repairoff"; 
    config->Arep.Orderon = "repairon"; 
    config->Arep.Order = "repair"; 
    config->Arep.Unbuttonpos = &MAKE(VECTOR3,1,1); 
    config->Arep.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Arep.Unart = "ReplaceableTextures\\CommandButtons\\BTNRepairOff.blp"; 
    config->Arep.Art = "ReplaceableTextures\\CommandButtons\\BTNRepairOn.blp"; 
    config->BSTN.Targetattach = "overhead"; 
    config->BSTN.Targetart = "Abilities\\Spells\\Human\\Thunderclap\\ThunderclapTarget.mdl"; 
    config->BSTN.Buffart = "ReplaceableTextures\\CommandButtons\\BTNStun.blp"; 
    config->BPSE.Targetattach = "overhead"; 
    config->BPSE.Targetart = "Abilities\\Spells\\Human\\Thunderclap\\ThunderclapTarget.mdl"; 
    config->BPSE.Buffart = "ReplaceableTextures\\CommandButtons\\BTNStun.blp"; 
}

