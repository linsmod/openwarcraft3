#pragma once
#include "../common/common.h"
typedef struct Units_NeutralAbilityFunc {
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } AChw;
    union {
        LPCSTR order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACr2;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACm2;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACm3;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Adtg;
    union {
        LPCSTR Targetart;
        LPCSTR Specialart;
        LPCSTR Casterart;
    } Arng;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANdc;
    union {
        LPCSTR Effectart;
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BNdc;
    union {
    } SNdc;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANsl;
    union {
        LPCSTR Effectsound;
        LPCSTR Buffart;
    } BNsl;
    union {
        LPCSTR Effectart;
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
        LPCSTR Researchart;
    } SNin;
    union {
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Effectart;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANdp;
    union {
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANrc;
    union {
        LPCSTR Targetart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANfd;
    union {
    } Achd;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bchd;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ANpi;
    union {
        LPCSTR Specialartattach;
        LPCSTR Specialart;
        LPCSTR Targetart;
    } BNpi;
    union {
        LPCSTR Effectsound;
    } Abun;
    union {
        LPCSTR Effectsound;
    } Acar;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Aloa;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Adro;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Adri;
    union {
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Andt;
    union {
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ANgl;
    union {
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ANlg;
    union {
    } ANre;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetart;
    } ANrl;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetart;
    } ANrm;
    union {
        LPCSTR Targetattach;
        LPCSTR Casterattach;
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Aneu;
    union {
        LPCSTR Order;
        LPCSTR Effectart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
        LPCSTR Researchart;
    } ANin;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ANsp;
    union {
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Asds;
    union {
        LPCSTR Unorder;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Amrf;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Anhe;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPCSTR Casterart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACsw;
    union {
        LPCSTR Order;
        LPCSTR Effectart;
        LPCSTR Casterattach;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACtc;
    union {
        LPCSTR Order;
        LPCSTR Effectart;
        LPCSTR Casterattach;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACt2;
    union {
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACtb;
    union {
        LPCSTR OrderOff;
        LPCSTR OrderOn;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACbl;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACdv;
    union {
        LPCSTR Order;
        LPCSTR Casterart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANrf;
    union {
        LPCSTR Effectsound;
        LPCSTR Effectsoundlooped;
        LPCSTR Effectart;
    } XErf;
    union {
        LPCSTR Effectart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } ANrn;
    union {
        float Missilespeed;
        LPCSTR Missileart;
        LPCSTR Researchart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ANfb;
    union {
    } Assp;
    union {
    } Aspd;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } AOac;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BOac;
    union {
        LPCSTR Buffart;
    } Btlf;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACad;
    union {
        LPCSTR Effectart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
        LPCSTR Tip;
    } ACrn;
    union {
        LPCSTR Order;
        LPCSTR Targetart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } Ambd;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACsl;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Casterattach;
        LPCSTR Casterart;
    } Asla;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Order;
        LPCSTR Casterattach;
        LPCSTR Casterart;
        LPCSTR Art;
    } ACsp;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACba;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACat;
    union {
        LPCSTR Order;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACam;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACbh;
    union {
        LPCSTR Order;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACbz;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACca;
    union {
        LPCSTR Order;
        LPCSTR Targetart;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACcl;
    union {
        float Missilespeed;
        float Missilearc;
        LPCSTR Missileart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACcw;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACct;
    union {
        LPCSTR order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } SCc1;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACdc;
    union {
        LPCSTR Unorder;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACds;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Aenr;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACev;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACes;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACsf;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACs9;
    union {
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACfb;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACfr;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACfa;
    union {
        LPCSTR Order;
        LPCSTR Effectart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACfn;
    union {
        LPCSTR Order;
    } Ahr3;
    union {
        LPCSTR Unorder;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACim;
    union {
        LPCSTR Order;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACrf;
    union {
        LPCSTR Unorder;
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } ACsa;
    union {
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACsh;
    union {
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACst;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACah;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACua;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACvp;
    union {
        LPCSTR Order;
        LPCSTR Casterart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Awrs;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACav;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACvs;
    union {
        LPCSTR Art;
    } Aarm;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Barm;
    union {
        LPCSTR Art;
    } ACnr;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } SNdd;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Adsm;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } SNeq;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } SCae;
    union {
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } ACen;
} Units_NeutralAbilityFunc;

void Init_Units_NeutralAbilityFunc(Units_NeutralAbilityFunc *config);
void Shutdown_Units_NeutralAbilityFunc(Units_NeutralAbilityFunc *config);

void Init_Units_NeutralAbilityFunc(Units_NeutralAbilityFunc *config) {
    config->AChw.Order = "healingward"; 
    config->AChw.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->AChw.Art = "ReplaceableTextures\\CommandButtons\\BTNHealingWard.blp"; 
    config->ACr2.order = "rejuvination"; 
    config->ACr2.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACr2.Art = "ReplaceableTextures\\CommandButtons\\BTNRejuvenation.blp"; 
    config->ACm2.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->ACm2.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNMagicImmunity.blp"; 
    config->ACm3.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACm3.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNMagicImmunity.blp"; 
    config->Adtg.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Adtg.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNScout.blp"; 
    config->Arng.Targetart = "Abilities\\Weapons\\Mortar\\MortarMissile.mdl"; 
    config->Arng.Specialart = "Objects\\Spawnmodels\\Human\\HumanLargeDeathExplode\\HumanLargeDeathExplode.mdl"; 
    config->Arng.Casterart = "Abilities\\Spells\\Orc\\WarStomp\\WarStompCaster.mdl"; 
    config->ANdc.Order = "darkconversion"; 
    config->ANdc.Researchbuttonpos = &MAKE(VECTOR3,2,0); 
    config->ANdc.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ANdc.Researchart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->ANdc.Art = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->BNdc.Effectart = "Abilities\\Spells\\Demon\\DarkConversion\\ZombifyTarget.mdl"; 
    config->BNdc.Targetattach = "overhead"; 
    config->BNdc.Targetart = "Abilities\\Spells\\Undead\\Sleep\\SleepTarget.mdl"; 
    config->BNdc.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->ANsl.Order = "soulpreservation"; 
    config->ANsl.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->ANsl.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ANsl.Researchart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->ANsl.Art = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->BNsl.Effectsound = "SoulPreservation"; 
    config->BNsl.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->SNin.Effectart = "Units\\Demon\\Infernal\\InfernalBirth.mdl"; 
    config->SNin.Order = "inferno"; 
    config->SNin.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->SNin.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->SNin.Art = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->SNin.Researchart = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->ANdp.Researchbuttonpos = &MAKE(VECTOR3,0,0); 
    config->ANdp.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ANdp.Effectart = "Abilities\\Spells\\Demon\\DarkPortal\\DarkPortalTarget.mdl"; 
    config->ANdp.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDizzy.blp"; 
    config->ANdp.Art = "ReplaceableTextures\\CommandButtons\\BTNDizzy.blp"; 
    config->ANrc.Researchbuttonpos = &MAKE(VECTOR3,1,0); 
    config->ANrc.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ANrc.Researchart = "ReplaceableTextures\\CommandButtons\\BTNInfernalStone.blp"; 
    config->ANrc.Art = "ReplaceableTextures\\CommandButtons\\BTNInfernalStone.blp"; 
    config->ANfd.Targetart = "Abilities\\Spells\\Demon\\DemonBoltImpact\\DemonBoltImpact.mdl"; 
    config->ANfd.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->ANfd.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ANfd.Researchart = "ReplaceableTextures\\CommandButtons\\BTNCorpseExplode.blp"; 
    config->ANfd.Art = "ReplaceableTextures\\CommandButtons\\BTNCorpseExplode.blp"; 
    config->Bchd.Targetattach = "overhead"; 
    config->Bchd.Targetart = "Abilities\\Spells\\Orc\\StasisTrap\\StasisTotemTarget.mdl"; 
    config->Bchd.Buffart = "ReplaceableTextures\\CommandButtons\\BTNDizzy.blp"; 
    config->ANpi.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ANpi.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNImmolation.blp"; 
    config->BNpi.Specialartattach = "head"; 
    config->BNpi.Specialart = "Abilities\\Spells\\NightElf\\Immolation\\ImmolationDamage.mdl"; 
    config->BNpi.Targetart = ""; 
    config->Abun.Effectsound = "LoadUnload"; 
    config->Acar.Effectsound = "LoadUnload"; 
    config->Aloa.Order = "load"; 
    config->Aloa.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Aloa.Art = "ReplaceableTextures\\CommandButtons\\BTNLoad.blp"; 
    config->Adro.Order = "unload"; 
    config->Adro.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Adro.Art = "ReplaceableTextures\\CommandButtons\\BTNUnLoad.blp"; 
    config->Adri.Order = "unload"; 
    config->Adri.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Adri.Art = "ReplaceableTextures\\CommandButtons\\BTNUnLoad.blp"; 
    config->Andt.Casterart = "Abilities\\Spells\\Other\\TempSpellArt\\TempSpellArt.mdl"; 
    config->Andt.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Andt.Art = "ReplaceableTextures\\CommandButtons\\BTNReveal.blp"; 
    config->ANgl.Targetart = "Abilities\\Spells\\Other\\TempSpellArt\\TempSpellArt.mdl"; 
    config->ANgl.Casterart = "Abilities\\Spells\\Other\\TempSpellArt\\TempSpellArt.mdl"; 
    config->ANgl.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ANgl.Art = "ReplaceableTextures\\CommandButtons\\BTNGLExchange.blp"; 
    config->ANlg.Targetart = "Abilities\\Spells\\Other\\TempSpellArt\\TempSpellArt.mdl"; 
    config->ANlg.Casterart = "Abilities\\Spells\\Other\\TempSpellArt\\TempSpellArt.mdl"; 
    config->ANlg.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->ANlg.Art = "ReplaceableTextures\\CommandButtons\\BTNLGExchange.blp"; 
    config->ANrl.Effectsoundlooped = "FountainOfLifeLoop"; 
    config->ANrl.Targetart = "Abilities\\Spells\\Other\\ANrm\\ANrmTarget.mdl"; 
    config->ANrm.Effectsoundlooped = "FountainOfLifeLoop"; 
    config->ANrm.Targetart = "Abilities\\Spells\\Other\\ANrl\\ANrlTarget.mdl"; 
    config->Aneu.Targetattach = "overhead"; 
    config->Aneu.Casterattach = "overhead"; 
    config->Aneu.Targetart = "Abilities\\Spells\\Other\\Aneu\\AneuTarget.mdl"; 
    config->Aneu.Casterart = "Abilities\\Spells\\Other\\Aneu\\AneuCaster.mdl"; 
    config->Aneu.Unbuttonpos = &MAKE(VECTOR3,3,2); 
    config->Aneu.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Aneu.Unart = "ReplaceableTextures\\CommandButtons\\BTNSelectHeroOff.blp"; 
    config->Aneu.Art = "ReplaceableTextures\\CommandButtons\\BTNSelectHeroOn.blp"; 
    config->ANin.Order = "inferno"; 
    config->ANin.Effectart = "Units\\Demon\\Infernal\\InfernalBirth.mdl"; 
    config->ANin.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->ANin.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ANin.Art = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->ANin.Researchart = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->ANsp.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->ANsp.Art = "ReplaceableTextures\\CommandButtons\\BTNSpy.blp"; 
    config->Asds.Order = "selfdestruct"; 
    config->Asds.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Asds.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Asds.Unart = "ReplaceableTextures\\CommandButtons\\BTNSelfDestructOff.blp"; 
    config->Asds.Art = "ReplaceableTextures\\CommandButtons\\BTNSelfDestructOn.blp"; 
    config->Amrf.Unorder = "unravenform"; 
    config->Amrf.Order = "ravenform"; 
    config->Amrf.Unbuttonpos = &MAKE(VECTOR3,3,2); 
    config->Amrf.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->Amrf.Unart = "ReplaceableTextures\\CommandButtons\\BTNDruidOfTheTalon.blp"; 
    config->Amrf.Art = "ReplaceableTextures\\CommandButtons\\BTNRavenForm.blp"; 
    config->Anhe.Orderoff = "healoff"; 
    config->Anhe.Orderon = "healon"; 
    config->Anhe.Order = "heal"; 
    config->Anhe.Targetart = "Abilities\\Spells\\Human\\Heal\\HealTarget.mdl"; 
    config->Anhe.Casterart = ""; 
    config->Anhe.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Anhe.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Anhe.Unart = "ReplaceableTextures\\CommandButtons\\BTNHealOff.blp"; 
    config->Anhe.Art = "ReplaceableTextures\\CommandButtons\\BTNHealOn.blp"; 
    config->ACsw.Orderoff = "slowoff"; 
    config->ACsw.Orderon = "slowon"; 
    config->ACsw.Order = "slow"; 
    config->ACsw.Casterart = "Abilities\\Spells\\Human\\Slow\\SlowCaster.mdl"; 
    config->ACsw.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->ACsw.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACsw.Unart = "ReplaceableTextures\\CommandButtons\\BTNSlowOff.blp"; 
    config->ACsw.Art = "ReplaceableTextures\\CommandButtons\\BTNSlowOn.blp"; 
    config->ACtc.Order = "creepthunderclap"; 
    config->ACtc.Effectart = ""; 
    config->ACtc.Casterattach = "origin"; 
    config->ACtc.Casterart = "Abilities\\Spells\\Human\\Thunderclap\\ThunderClapCaster.mdl"; 
    config->ACtc.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACtc.Art = "ReplaceableTextures\\CommandButtons\\BTNGolemThunderclap.blp"; 
    config->ACt2.Order = "creepthunderclap"; 
    config->ACt2.Effectart = ""; 
    config->ACt2.Casterattach = "origin"; 
    config->ACt2.Casterart = "Abilities\\Spells\\Human\\Thunderclap\\ThunderClapCaster.mdl"; 
    config->ACt2.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACt2.Art = "ReplaceableTextures\\CommandButtons\\BTNGolemThunderclap.blp"; 
    config->ACtb.Order = "creepthunderbolt"; 
    config->ACtb.Missilespeed = 1000; 
    config->ACtb.Missileart = "Abilities\\Weapons\\RockBoltMissile\\RockBoltMissile.mdl"; 
    config->ACtb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACtb.Art = "ReplaceableTextures\\CommandButtons\\BTNGolemStormBolt.blp"; 
    config->ACbl.OrderOff = "bloodlustoff"; 
    config->ACbl.OrderOn = "bloodluston"; 
    config->ACbl.Order = "bloodlust"; 
    config->ACbl.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->ACbl.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACbl.Unart = "ReplaceableTextures\\CommandButtons\\BTNBloodLustOff.blp"; 
    config->ACbl.Art = "ReplaceableTextures\\CommandButtons\\BTNBloodLustOn.blp"; 
    config->ACdv.Order = "creepdevour"; 
    config->ACdv.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACdv.Art = "ReplaceableTextures\\CommandButtons\\BTNRedDragonDevour.blp"; 
    config->ANrf.Order = "rainoffire"; 
    config->ANrf.Casterart = ""; 
    config->ANrf.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->ANrf.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ANrf.Researchart = "ReplaceableTextures\\CommandButtons\\BTNFire.blp"; 
    config->ANrf.Art = "ReplaceableTextures\\CommandButtons\\BTNFire.blp"; 
    config->XErf.Effectsound = "RainOfFireWave"; 
    config->XErf.Effectsoundlooped = "RainOfFireLoop"; 
    config->XErf.Effectart = "Abilities\\Spells\\Demon\\RainOfFire\\RainOfFireTarget.mdl"; 
    config->ANrn.Effectart = "Abilities\\Spells\\Orc\\Reincarnation\\ReincarnationTarget.mdl"; 
    config->ANrn.Researchbuttonpos = &MAKE(VECTOR3,2,0); 
    config->ANrn.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ANrn.Researchart = "ReplaceableTextures\\CommandButtons\\BTNReincarnation.blp"; 
    config->ANrn.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNReincarnation.blp"; 
    config->ANfb.Missilespeed = 1000; 
    config->ANfb.Missileart = "Abilities\\Weapons\\FireBallMissile\\FireBallMissile.mdl"; 
    config->ANfb.Researchart = "ReplaceableTextures\\CommandButtons\\BTNFireBolt.blp"; 
    config->ANfb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ANfb.Art = "ReplaceableTextures\\CommandButtons\\BTNFireBolt.blp"; 
    config->AOac.Targetattach = "origin"; 
    config->AOac.Targetart = "Abilities\\Spells\\Orc\\WarDrums\\DrumsCasterHeal.mdl"; 
    config->AOac.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->AOac.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNGnollCommandAura.blp"; 
    config->BOac.Targetattach = "origin"; 
    config->BOac.Targetart = "Abilities\\Spells\\Other\\GeneralAuraTarget\\GeneralAuraTarget.mdl"; 
    config->BOac.Buffart = "ReplaceableTextures\\CommandButtons\\BTNGnollCommandAura.blp"; 
    config->Btlf.Buffart = "ReplaceableTextures\\CommandButtons\\BTNAcorn.blp"; 
    config->ACad.Order = "animatedead"; 
    config->ACad.Specialart = "Abilities\\Spells\\Undead\\AnimateDead\\AnimateDeadTarget.mdl"; 
    config->ACad.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ACad.Art = "ReplaceableTextures\\CommandButtons\\BTNAnimateDead.blp"; 
    config->ACrn.Effectart = "Abilities\\Spells\\Orc\\Reincarnation\\ReincarnationTarget.mdl"; 
    config->ACrn.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ACrn.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNReincarnation.blp"; 
    config->ACrn.Tip = "Reincarnation"; 
    config->Ambd.Order = "manaburn"; 
    config->Ambd.Targetart = "Abilities\\Spells\\NightElf\\ManaBurn\\ManaBurnTarget.mdl"; 
    config->Ambd.Researchbuttonpos = &MAKE(VECTOR3,0,0); 
    config->Ambd.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Ambd.Researchart = "ReplaceableTextures\\CommandButtons\\BTNManaBurn.blp"; 
    config->Ambd.Art = "ReplaceableTextures\\CommandButtons\\BTNManaBurn.blp"; 
    config->ACsl.Order = "sleep"; 
    config->ACsl.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACsl.Art = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->Asla.Effectsoundlooped = "CreepSleepSnoreLoop"; 
    config->Asla.Casterattach = "overhead"; 
    config->Asla.Casterart = "Abilities\\Spells\\Other\\CreepSleep\\CreepSleepTarget.mdl"; 
    config->ACsp.Effectsoundlooped = "CreepSleepSnoreLoop"; 
    config->ACsp.Order = "creepsleep"; 
    config->ACsp.Casterattach = "overhead"; 
    config->ACsp.Casterart = "Abilities\\Spells\\Other\\CreepSleep\\CreepSleepTarget.mdl"; 
    config->ACsp.Art = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->ACba.Targetattach = "origin"; 
    config->ACba.Targetart = "Abilities\\Spells\\Human\\Brilliance\\Brilliance.mdl"; 
    config->ACba.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACba.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNBrilliance.blp"; 
    config->ACat.Targetattach = "origin"; 
    config->ACat.Targetart = "Abilities\\Spells\\NightElf\\TrueshotAura\\TrueshotAura.mdl"; 
    config->ACat.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACat.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNTrueShot.blp"; 
    config->ACam.Order = "antimagicshell"; 
    config->ACam.Requires = "Ruba"; 
    config->ACam.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACam.Art = "ReplaceableTextures\\CommandButtons\\BTNAntiMagicShell.blp"; 
    config->ACbh.Order = "bash"; 
    config->ACbh.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACbh.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNBash.blp"; 
    config->ACbz.Order = "blizzard"; 
    config->ACbz.Casterart = ""; 
    config->ACbz.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACbz.Art = "ReplaceableTextures\\CommandButtons\\BTNBlizzard.blp"; 
    config->ACca.Order = "carrionswarm"; 
    config->ACca.Specialart = "Abilities\\Spells\\Undead\\CarrionSwarm\\CarrionSwarmDamage.mdl"; 
    config->ACca.Missilespeed = 1100; 
    config->ACca.Missileart = "Abilities\\Spells\\Undead\\CarrionSwarm\\CarrionSwarmMissile.mdl"; 
    config->ACca.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACca.Art = "ReplaceableTextures\\CommandButtons\\BTNCarrionSwarm.blp"; 
    config->ACcl.Order = "chainlightning"; 
    config->ACcl.Targetart = "Abilities\\Weapons\\Bolt\\BoltImpact.mdl"; 
    config->ACcl.Missilespeed = 1500; 
    config->ACcl.Missileart = "Abilities\\Spells\\Orc\\LightningBolt\\LightningBoltMissile.mdl"; 
    config->ACcl.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACcl.Art = "ReplaceableTextures\\CommandButtons\\BTNChainLightning.blp"; 
    config->ACcw.Missilespeed = 1500; 
    config->ACcw.Missilearc = 0.15; 
    config->ACcw.Missileart = "Abilities\\Weapons\\ColdArrow\\ColdArrowMissile.mdl"; 
    config->ACcw.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->ACcw.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACcw.Unart = "ReplaceableTextures\\CommandButtons\\BTNColdArrowsOff.blp"; 
    config->ACcw.Art = "ReplaceableTextures\\CommandButtons\\BTNColdArrowsOn.blp"; 
    config->ACct.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACct.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNCriticalStrike.blp"; 
    config->SCc1.order = "cyclone"; 
    config->SCc1.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->SCc1.Art = "ReplaceableTextures\\CommandButtons\\BTNCyclone.blp"; 
    config->ACdc.Order = "deathcoil"; 
    config->ACdc.Specialart = "Abilities\\Spells\\Undead\\DeathCoil\\DeathCoilSpecialArt.mdl"; 
    config->ACdc.Missilespeed = 1100; 
    config->ACdc.Missileart = "Abilities\\Spells\\Undead\\DeathCoil\\DeathCoilMissile.mdl"; 
    config->ACdc.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACdc.Art = "ReplaceableTextures\\CommandButtons\\BTNDeathCoil.blp"; 
    config->ACds.Unorder = "undivineshield"; 
    config->ACds.Order = "divineshield"; 
    config->ACds.Unbuttonpos = &MAKE(VECTOR3,1,2); 
    config->ACds.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACds.Unart = "ReplaceableTextures\\CommandButtons\\BTNDivineShieldOff.blp"; 
    config->ACds.Art = "ReplaceableTextures\\CommandButtons\\BTNDivineIntervention.blp"; 
    config->Aenr.Order = "entanglingroots"; 
    config->Aenr.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Aenr.Art = "ReplaceableTextures\\CommandButtons\\BTNEntanglingRoots.blp"; 
    config->ACev.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACev.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNEvasion.blp"; 
    config->ACes.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACes.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNEvasion.blp"; 
    config->ACsf.Order = "spiritwolf"; 
    config->ACsf.Specialart = "Abilities\\Spells\\Orc\\FeralSpirit\\feralspirittarget.mdl"; 
    config->ACsf.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACsf.Art = "ReplaceableTextures\\CommandButtons\\BTNSpiritWolf.blp"; 
    config->ACs9.Order = "spiritwolf"; 
    config->ACs9.Specialart = "Abilities\\Spells\\Orc\\FeralSpirit\\feralspirittarget.mdl"; 
    config->ACs9.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACs9.Art = "ReplaceableTextures\\CommandButtons\\BTNRazorback.blp"; 
    config->ACfb.Missilespeed = 1000; 
    config->ACfb.Missileart = "Abilities\\Weapons\\FireBallMissile\\FireBallMissile.mdl"; 
    config->ACfb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACfb.Art = "ReplaceableTextures\\CommandButtons\\BTNFireBolt.blp"; 
    config->ACfr.Order = "forceofnature"; 
    config->ACfr.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACfr.Art = "ReplaceableTextures\\CommandButtons\\BTNEnt.blp"; 
    config->ACfa.Order = "frostarmor"; 
    config->ACfa.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACfa.Art = "ReplaceableTextures\\CommandButtons\\BTNFrostArmor.blp"; 
    config->ACfn.Order = "frostnova"; 
    config->ACfn.Effectart = "Abilities\\Spells\\Undead\\FrostNova\\FrostNovaTarget.mdl"; 
    config->ACfn.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACfn.Art = "ReplaceableTextures\\CommandButtons\\BTNGlacier.blp"; 
    config->Ahr3.Order = "harvest"; 
    config->ACim.Unorder = "unimmolation"; 
    config->ACim.Order = "immolation"; 
    config->ACim.Unbuttonpos = &MAKE(VECTOR3,1,2); 
    config->ACim.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACim.Unart = "ReplaceableTextures\\CommandButtons\\BTNImmolationOff.blp"; 
    config->ACim.Art = "ReplaceableTextures\\CommandButtons\\BTNImmolationOn.blp"; 
    config->ACrf.Order = "rainoffire"; 
    config->ACrf.Casterart = ""; 
    config->ACrf.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->ACrf.Art = "ReplaceableTextures\\CommandButtons\\BTNFire.blp"; 
    config->ACsa.Unorder = "unflamingarrows"; 
    config->ACsa.Order = "flamingarrows"; 
    config->ACsa.Missilespeed = 1500; 
    config->ACsa.Missileart = "Abilities\\Weapons\\SearingArrow\\SearingArrowMissile.mdl"; 
    config->ACsa.Unbuttonpos = &MAKE(VECTOR3,1,2); 
    config->ACsa.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->ACsa.Unart = "ReplaceableTextures\\CommandButtons\\BTNSearingArrowsOff.blp"; 
    config->ACsa.Art = "ReplaceableTextures\\CommandButtons\\BTNSearingArrowsOn.blp"; 
    config->ACsh.Order = "shockwave"; 
    config->ACsh.Missilespeed = 1050; 
    config->ACsh.Missileart = "Abilities\\Spells\\Orc\\Shockwave\\ShockwaveMissile.mdl"; 
    config->ACsh.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACsh.Art = "ReplaceableTextures\\CommandButtons\\BTNShockWave.blp"; 
    config->ACst.Order = "shockwave"; 
    config->ACst.Missilespeed = 1050; 
    config->ACst.Missileart = "Abilities\\Spells\\Orc\\Shockwave\\ShockwaveMissile.mdl"; 
    config->ACst.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACst.Art = "ReplaceableTextures\\CommandButtons\\BTNShockWave.blp"; 
    config->ACah.Targetattach = "origin"; 
    config->ACah.Targetart = "Abilities\\Spells\\NightElf\\ThornsAura\\ThornsAura.mdl"; 
    config->ACah.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACah.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNThorns.blp"; 
    config->ACua.Targetattach = "origin"; 
    config->ACua.Targetart = "Abilities\\Spells\\Undead\\UnholyAura\\UnholyAura.mdl"; 
    config->ACua.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACua.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNUnholyAura.blp"; 
    config->ACvp.Targetattach = "origin"; 
    config->ACvp.Targetart = "Abilities\\Spells\\Undead\\VampiricAura\\VampiricAura.mdl"; 
    config->ACvp.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACvp.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNVampiricAura.blp"; 
    config->Awrs.Order = "stomp"; 
    config->Awrs.Casterart = "Abilities\\Spells\\Orc\\WarStomp\\WarStompCaster.mdl"; 
    config->Awrs.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Awrs.Art = "ReplaceableTextures\\CommandButtons\\BTNWarStomp.blp"; 
    config->ACav.Targetattach = "origin"; 
    config->ACav.Targetart = "Abilities\\Spells\\Human\\DevotionAura\\DevotionAura.mdl"; 
    config->ACav.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->ACav.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNDevotion.blp"; 
    config->ACvs.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACvs.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNEnvenomedSpear.blp"; 
    config->Aarm.Art = ""; 
    config->Barm.Effectsoundlooped = "FountainOfLifeLoop"; 
    config->Barm.Targetart = "Abilities\\Spells\\Other\\ANrl\\ANrlTarget.mdl"; 
    config->Barm.Buffart = ""; 
    config->ACnr.Art = "ReplaceableTextures\\CommandButtons\\BTNHeal.blp"; 
    config->SNdd.Order = "deathanddecay"; 
    config->SNdd.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->SNdd.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->SNdd.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDeathAndDecay.blp"; 
    config->SNdd.Art = "ReplaceableTextures\\CommandButtons\\BTNDeathAndDecay.blp"; 
    config->Adsm.Order = "dispel"; 
    config->Adsm.Specialart = "Abilities\\Spells\\Human\\DispelMagic\\DispelMagicTarget.mdl"; 
    config->Adsm.Targetart = "Abilities\\Spells\\Human\\DispelMagic\\DispelMagicTarget.mdl"; 
    config->Adsm.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Adsm.Art = "ReplaceableTextures\\CommandButtons\\BTNDispelMagic.blp"; 
    config->SNeq.Order = "earthquake"; 
    config->SNeq.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->SNeq.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->SNeq.Researchart = "ReplaceableTextures\\CommandButtons\\BTNEarthquake.blp"; 
    config->SNeq.Art = "ReplaceableTextures\\CommandButtons\\BTNEarthquake.blp"; 
    config->SCae.Targetattach = "origin"; 
    config->SCae.Targetart = "Abilities\\Spells\\Orc\\CommandAura\\CommandAura.mdl"; 
    config->SCae.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->SCae.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNCommand.blp"; 
    config->ACen.Order = "ensnare"; 
    config->ACen.Missilespeed = 1500; 
    config->ACen.Missileart = "Abilities\\Spells\\Orc\\Ensnare\\EnsnareMissile.mdl"; 
    config->ACen.Requires = "Roen"; 
    config->ACen.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->ACen.Art = "ReplaceableTextures\\CommandButtons\\BTNEnsnare.blp"; 
}

