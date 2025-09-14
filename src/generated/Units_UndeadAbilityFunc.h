#pragma once
#include "../common/common.h"
typedef struct Units_UndeadAbilityFunc {
    union {
        LPCSTR Effectsound;
    } Amtc;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Atru;
    union {
        LPCSTR Order;
        LPCSTR Effectsound;
    } Aaha;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Auns;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Buns;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Effectart;
    } Abgm;
    union {
        LPCSTR SpecialArt;
    } Agyd;
    union {
        LPCSTR Order;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Alam;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Asac;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
        LPCSTR Requires;
    } Acan;
    union {
    } Aspa;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bspa;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        LPCSTR Requires;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Aweb;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bweb;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bwea;
    union {
        LPCSTR Unorder;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Requires;
        LPCSTR Unart;
        LPCSTR Art;
    } Astn;
    union {
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Amel;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Amed;
    union {
        LPCSTR Targetart;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Aapl;
    union {
        LPCSTR Targetart;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Aap2;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bapl;
    union {
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Apts;
    union {
    } Afrb;
    union {
        LPCSTR Order;
        float Missilespeed;
        float Missilearc;
        LPCSTR Missileart;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Afrz;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bfrz;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPCSTR Effectart;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Arai;
    union {
        LPCSTR Buffart;
    } Arai;
    union {
        LPCSTR Order;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Auhf;
    union {
        LPCSTR Order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Suhf;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Buhf;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Acrs;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bcrs;
    union {
        LPCSTR Order;
        LPCSTR Requires;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Aams;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bams;
    union {
        LPCSTR Order;
        float Missilespeed;
        LPCSTR Missileart;
        float Requiresamount;
        LPCSTR Requires;
        LPCSTR SpecialArt;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Apos;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUdc;
    union {
        LPCSTR Order;
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUdp;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUdp;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUan;
    union {
        LPCSTR Specialart;
        LPCSTR Buffart;
    } BUan;
    union {
        LPCSTR Order;
        LPCSTR Specialart;
        float Missilespeed;
        LPCSTR Missileart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUcs;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUsl;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUsl;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUst;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Researchbuttonpos;
        LPCSTR Researchart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } AUav;
    union {
        LPCSTR Specialartattach;
        LPCSTR Specialart;
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUav;
    union {
        LPCSTR Order;
        float Missilespeed;
        float Missilearc;
        LPCSTR Missileart;
        LPCSTR Targetart;
        LPCSTR Effectart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUds;
    union {
        LPCSTR Order;
        LPCSTR Effectart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUfn;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPCSTR Researchart;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } AUfa;
    union {
        LPCSTR Specialartattach;
        LPCSTR Specialart;
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUfa;
    union {
        LPCSTR Order;
        LPCSTR Targetart;
        LPCSTR Casterart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUdr;
    union {
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUdd;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Effectart;
    } XUdd;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUdd;
    union {
        LPCSTR order;
        LPVECTOR3 Buttonpos;
        float Requiresamount;
        LPCSTR Requires;
        LPCSTR Art;
    } Acri;
    union {
        LPCSTR order;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } Scri;
    union {
        LPCSTR Targetart;
        LPCSTR Buffart;
    } Bcri;
    union {
        LPCSTR Orderoff;
        LPCSTR Orderon;
        LPCSTR Order;
        LPCSTR Effectsoundlooped;
        LPVECTOR3 Unbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Unart;
        LPCSTR Art;
    } Arst;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Researchart;
        LPCSTR Art;
    } AUau;
    union {
        LPCSTR Targetattach;
        LPCSTR Targetart;
        LPCSTR Buffart;
    } BUau;
    union {
        LPCSTR Effectart;
        LPCSTR Order;
        LPVECTOR3 Researchbuttonpos;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
        LPCSTR Researchart;
    } AUin;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetattach1;
        LPCSTR Targetattach;
        float Targetattachcount;
        LPCSTR TargetArt;
    } Xfus;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetattach3;
        LPCSTR Targetattach2;
        LPCSTR Targetattach1;
        LPCSTR Targetattach;
        float Targetattachcount;
        LPCSTR TargetArt;
    } Xfum;
    union {
        LPCSTR Effectsoundlooped;
        LPCSTR Targetattach5;
        LPCSTR Targetattach4;
        LPCSTR Targetattach3;
        LPCSTR Targetattach2;
        LPCSTR Targetattach1;
        LPCSTR Targetattach;
        float Targetattachcount;
        LPCSTR TargetArt;
    } Xful;
} Units_UndeadAbilityFunc;

void Init_Units_UndeadAbilityFunc(Units_UndeadAbilityFunc *config);
void Shutdown_Units_UndeadAbilityFunc(Units_UndeadAbilityFunc *config);

void Init_Units_UndeadAbilityFunc(Units_UndeadAbilityFunc *config) {
    config->Amtc.Effectsound = "LoadUnload"; 
    config->Atru.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Atru.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNShadeTrueSight.blp"; 
    config->Aaha.Order = "harvest"; 
    config->Aaha.Effectsound = "AcolyteMining"; 
    config->Auns.Order = "unsummon"; 
    config->Auns.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Auns.Art = "ReplaceableTextures\\CommandButtons\\BTNUnsummonBuilding.blp"; 
    config->Buns.Effectsoundlooped = "AcolyteUnsummonLoop"; 
    config->Buns.Targetart = "Abilities\\Spells\\Undead\\Unsummon\\UnsummonTarget.mdl"; 
    config->Buns.Buffart = "ReplaceableTextures\\CommandButtons\\BTNUnsummonBuilding.blp"; 
    config->Abgm.Effectsoundlooped = "MineDomeLoop"; 
    config->Abgm.Effectart = "Abilities\\Spells\\Undead\\UndeadMine\\UndeadMineCircle.mdl"; 
    config->Agyd.SpecialArt = "Abilities\\Spells\\Undead\\Graveyard\\GraveMarker.mdl"; 
    config->Alam.Order = "sacrifice"; 
    config->Alam.Requires = "usap"; 
    config->Alam.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->Alam.Art = "ReplaceableTextures\\CommandButtons\\BTNSacrifice.blp"; 
    config->Asac.Order = "sacrifice"; 
    config->Asac.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->Asac.Art = "ReplaceableTextures\\CommandButtons\\BTNSacrifice.blp"; 
    config->Acan.Order = "cannibalize"; 
    config->Acan.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Acan.Art = "ReplaceableTextures\\CommandButtons\\BTNCannibalize.blp"; 
    config->Acan.Requires = "Ruac"; 
    config->Bspa.Targetattach = "origin"; 
    config->Bspa.Targetart = "Abilities\\Weapons\\CryptFiendMissile\\CryptFiendMissileTarget.mdl"; 
    config->Bspa.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSpider.blp"; 
    config->Aweb.Orderoff = "weboff"; 
    config->Aweb.Orderon = "webon"; 
    config->Aweb.Order = "web"; 
    config->Aweb.Missilespeed = 1500; 
    config->Aweb.Missileart = "Abilities\\Spells\\Undead\\Web\\Webmissile.mdl"; 
    config->Aweb.Requires = "Ruwb"; 
    config->Aweb.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Aweb.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Aweb.Unart = "ReplaceableTextures\\CommandButtons\\BTNWebOff.blp"; 
    config->Aweb.Art = "ReplaceableTextures\\CommandButtons\\BTNWebOn.blp"; 
    config->Bweb.Targetattach = "origin"; 
    config->Bweb.Targetart = "Abilities\\Spells\\Undead\\Web\\WebTarget.mdl"; 
    config->Bweb.Buffart = "ReplaceableTextures\\CommandButtons\\BTNWeb.blp"; 
    config->Bwea.Targetattach = "chest,mount"; 
    config->Bwea.Targetart = "Abilities\\Spells\\Undead\\Web\\Web_AirTarget.mdl"; 
    config->Bwea.Buffart = "ReplaceableTextures\\CommandButtons\\BTNWeb.blp"; 
    config->Astn.Unorder = "unstoneform"; 
    config->Astn.Order = "stoneform"; 
    config->Astn.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Astn.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Astn.Requires = "Rusf"; 
    config->Astn.Unart = "ReplaceableTextures\\CommandButtons\\BTNGargoyle.blp"; 
    config->Astn.Art = "ReplaceableTextures\\CommandButtons\\BTNStoneForm.blp"; 
    config->Amel.Order = "loadcorpse"; 
    config->Amel.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Amel.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Amel.Unart = "ReplaceableTextures\\CommandButtons\\BTNUndeadLoadOff.blp"; 
    config->Amel.Art = "ReplaceableTextures\\CommandButtons\\BTNUndeadLoadOn.blp"; 
    config->Amed.Order = "unloadcorpse"; 
    config->Amed.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Amed.Art = "ReplaceableTextures\\CommandButtons\\BTNUndeadUnLoad.blp"; 
    config->Aapl.Targetart = "Abilities\\Spells\\Undead\\PlagueCloud\\PlagueCloudCaster.mdl"; 
    config->Aapl.Requires = "Rupc"; 
    config->Aapl.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Aapl.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNPlagueCloud.blp"; 
    config->Aap2.Targetart = ""; 
    config->Aap2.Requires = "Rupc"; 
    config->Aap2.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Aap2.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNPlagueCloud.blp"; 
    config->Bapl.Targetattach = "head"; 
    config->Bapl.Targetart = "Units\\Undead\\PlagueCloud\\PlagueCloudtarget.mdl"; 
    config->Bapl.Buffart = "ReplaceableTextures\\CommandButtons\\BTNPlagueCloud.blp"; 
    config->Apts.Requires = "Rupc"; 
    config->Apts.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Apts.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNPlagueCloud.blp"; 
    config->Afrz.Order = "freezingbreath"; 
    config->Afrz.Missilespeed = 900; 
    config->Afrz.Missilearc = 0.0; 
    config->Afrz.Missileart = "Abilities\\Spells\\Undead\\FreezingBreath\\FreezingBreathMissile.mdl"; 
    config->Afrz.Requires = "Rufb"; 
    config->Afrz.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Afrz.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNFreezingBreath.blp"; 
    config->Bfrz.Targetattach = "origin"; 
    config->Bfrz.Targetart = "Abilities\\Spells\\Undead\\FreezingBreath\\FreezingBreathTargetArt.mdl"; 
    config->Bfrz.Buffart = "ReplaceableTextures\\CommandButtons\\BTNFreezingBreath.blp"; 
    config->Arai.Orderoff = "raisedeadoff"; 
    config->Arai.Orderon = "raisedeadon"; 
    config->Arai.Order = "raisedead"; 
    config->Arai.Effectart = "Abilities\\Spells\\Undead\\RaiseSkeletonWarrior\\RaiseSkeleton.mdl"; 
    config->Arai.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Arai.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Arai.Unart = "ReplaceableTextures\\CommandButtons\\BTNRaiseDeadOff.blp"; 
    config->Arai.Art = "ReplaceableTextures\\CommandButtons\\BTNRaiseDeadOn.blp"; 
    config->Arai.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSkeletonWarrior.blp"; 
    config->Auhf.Order = "unholyfrenzy"; 
    config->Auhf.Requires = "Rune"; 
    config->Auhf.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Auhf.Art = "ReplaceableTextures\\CommandButtons\\BTNUnholyFrenzy.blp"; 
    config->Suhf.Order = "unholyfrenzy"; 
    config->Suhf.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Suhf.Art = "ReplaceableTextures\\CommandButtons\\BTNUnholyFrenzy.blp"; 
    config->Buhf.Targetattach = "overhead"; 
    config->Buhf.Targetart = "Abilities\\Spells\\Undead\\UnholyFrenzy\\UnholyFrenzyTarget.mdl"; 
    config->Buhf.Buffart = "ReplaceableTextures\\CommandButtons\\BTNUnholyFrenzy.blp"; 
    config->Acrs.Orderoff = "curseoff"; 
    config->Acrs.Orderon = "curseon"; 
    config->Acrs.Order = "curse"; 
    config->Acrs.Unbuttonpos = &MAKE(VECTOR3,0,2); 
    config->Acrs.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->Acrs.Unart = "ReplaceableTextures\\CommandButtons\\BTNCurseOff.blp"; 
    config->Acrs.Art = "ReplaceableTextures\\CommandButtons\\BTNCurseOn.blp"; 
    config->Bcrs.Targetattach = "overhead"; 
    config->Bcrs.Targetart = "Abilities\\Spells\\Undead\\Curse\\CurseTarget.mdl"; 
    config->Bcrs.Buffart = "ReplaceableTextures\\CommandButtons\\BTNCurse.blp"; 
    config->Aams.Order = "antimagicshell"; 
    config->Aams.Requires = "Ruba"; 
    config->Aams.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->Aams.Art = "ReplaceableTextures\\CommandButtons\\BTNAntiMagicShell.blp"; 
    config->Bams.Targetattach = "overhead"; 
    config->Bams.Targetart = "Abilities\\Spells\\Undead\\AntiMagicShell\\AntiMagicShell.mdl"; 
    config->Bams.Buffart = "ReplaceableTextures\\CommandButtons\\BTNAntiMagicShell.blp"; 
    config->Apos.Order = "possession"; 
    config->Apos.Missilespeed = 1500; 
    config->Apos.Missileart = "Abilities\\Spells\\Undead\\Possession\\PossessionMissile.mdl"; 
    config->Apos.Requiresamount = 2; 
    config->Apos.Requires = "Ruba"; 
    config->Apos.SpecialArt = ""; 
    config->Apos.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Apos.Art = "ReplaceableTextures\\CommandButtons\\BTNPossession.blp"; 
    config->AUdc.Order = "deathcoil"; 
    config->AUdc.Specialart = "Abilities\\Spells\\Undead\\DeathCoil\\DeathCoilSpecialArt.mdl"; 
    config->AUdc.Missilespeed = 1100; 
    config->AUdc.Missileart = "Abilities\\Spells\\Undead\\DeathCoil\\DeathCoilMissile.mdl"; 
    config->AUdc.Researchbuttonpos = &MAKE(VECTOR3,0,0); 
    config->AUdc.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->AUdc.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDeathCoil.blp"; 
    config->AUdc.Art = "ReplaceableTextures\\CommandButtons\\BTNDeathCoil.blp"; 
    config->AUdp.Order = "deathpact"; 
    config->AUdp.Targetattach = "origin"; 
    config->AUdp.Targetart = "Abilities\\Spells\\Undead\\DeathPact\\DeathPactTarget.mdl"; 
    config->AUdp.Casterart = "Abilities\\Spells\\Undead\\DeathPact\\DeathPactCaster.mdl"; 
    config->AUdp.Researchbuttonpos = &MAKE(VECTOR3,1,0); 
    config->AUdp.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->AUdp.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDeathPact.blp"; 
    config->AUdp.Art = "ReplaceableTextures\\CommandButtons\\BTNDeathPact.blp"; 
    config->BUdp.Targetattach = "origin"; 
    config->BUdp.Targetart = "Abilities\\Spells\\Undead\\DeathPact\\DeathPactCaster.mdl"; 
    config->BUdp.Buffart = "ReplaceableTextures\\CommandButtons\\BTNDeathPact.blp"; 
    config->AUan.Order = "animatedead"; 
    config->AUan.Specialart = "Abilities\\Spells\\Undead\\AnimateDead\\AnimateDeadTarget.mdl"; 
    config->AUan.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->AUan.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->AUan.Researchart = "ReplaceableTextures\\CommandButtons\\BTNAnimateDead.blp"; 
    config->AUan.Art = "ReplaceableTextures\\CommandButtons\\BTNAnimateDead.blp"; 
    config->BUan.Specialart = "Objects\\Spawnmodels\\Undead\\UndeadLargeDeathExplode\\UndeadLargeDeathExplode.mdl"; 
    config->BUan.Buffart = "ReplaceableTextures\\CommandButtons\\BTNAnimateDead.blp"; 
    config->AUcs.Order = "carrionswarm"; 
    config->AUcs.Specialart = "Abilities\\Spells\\Undead\\CarrionSwarm\\CarrionSwarmDamage.mdl"; 
    config->AUcs.Missilespeed = 1100; 
    config->AUcs.Missileart = "Abilities\\Spells\\Undead\\CarrionSwarm\\CarrionSwarmMissile.mdl"; 
    config->AUcs.Researchbuttonpos = &MAKE(VECTOR3,0,0); 
    config->AUcs.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->AUcs.Researchart = "ReplaceableTextures\\CommandButtons\\BTNCarrionSwarm.blp"; 
    config->AUcs.Art = "ReplaceableTextures\\CommandButtons\\BTNCarrionSwarm.blp"; 
    config->AUsl.Order = "sleep"; 
    config->AUsl.Researchbuttonpos = &MAKE(VECTOR3,1,0); 
    config->AUsl.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->AUsl.Researchart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->AUsl.Art = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->BUsl.Effectsoundlooped = "CreepSleepSnoreLoop"; 
    config->BUsl.Targetattach = "overhead"; 
    config->BUsl.Targetart = "Abilities\\Spells\\Undead\\Sleep\\SleepTarget.mdl"; 
    config->BUsl.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->BUst.Targetart = "Abilities\\Spells\\Undead\\Sleep\\SleepSpecialArt.mdl"; 
    config->BUst.Buffart = "ReplaceableTextures\\CommandButtons\\BTNSleep.blp"; 
    config->AUav.Targetattach = "origin"; 
    config->AUav.Targetart = "Abilities\\Spells\\Undead\\VampiricAura\\VampiricAura.mdl"; 
    config->AUav.Researchbuttonpos = &MAKE(VECTOR3,2,0); 
    config->AUav.Researchart = "ReplaceableTextures\\CommandButtons\\BTNVampiricAura.blp"; 
    config->AUav.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->AUav.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNVampiricAura.blp"; 
    config->BUav.Specialartattach = "chest"; 
    config->BUav.Specialart = "Abilities\\Spells\\Undead\\VampiricAura\\VampiricAuraTarget.mdl"; 
    config->BUav.Targetattach = "origin"; 
    config->BUav.Targetart = "Abilities\\Spells\\Other\\GeneralAuraTarget\\GeneralAuraTarget.mdl"; 
    config->BUav.Buffart = "ReplaceableTextures\\CommandButtons\\BTNVampiricAura.blp"; 
    config->AUds.Order = "darksummoning"; 
    config->AUds.Missilespeed = 1500; 
    config->AUds.Missilearc = 0.15; 
    config->AUds.Missileart = "Abilities\\Spells\\Undead\\DarkSummoning\\DarkSummonMissile.mdl"; 
    config->AUds.Targetart = "Abilities\\Spells\\Undead\\Darksummoning\\DarkSummonTarget.mdl"; 
    config->AUds.Effectart = "Abilities\\Spells\\Undead\\Darksummoning\\DarkSummonTarget.mdl"; 
    config->AUds.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->AUds.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->AUds.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDarkSummoning.blp"; 
    config->AUds.Art = "ReplaceableTextures\\CommandButtons\\BTNDarkSummoning.blp"; 
    config->AUfn.Order = "frostnova"; 
    config->AUfn.Effectart = "Abilities\\Spells\\Undead\\FrostNova\\FrostNovaTarget.mdl"; 
    config->AUfn.Researchbuttonpos = &MAKE(VECTOR3,0,0); 
    config->AUfn.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->AUfn.Researchart = "ReplaceableTextures\\CommandButtons\\BTNGlacier.blp"; 
    config->AUfn.Art = "ReplaceableTextures\\CommandButtons\\BTNGlacier.blp"; 
    config->AUfa.Order = "frostarmor"; 
    config->AUfa.Researchbuttonpos = &MAKE(VECTOR3,1,0); 
    config->AUfa.Researchart = "ReplaceableTextures\\CommandButtons\\BTNFrostArmor.blp"; 
    config->AUfa.Buttonpos = &MAKE(VECTOR3,1,2); 
    config->AUfa.Art = "ReplaceableTextures\\CommandButtons\\BTNFrostArmor.blp"; 
    config->BUfa.Specialartattach = "chest"; 
    config->BUfa.Specialart = "Abilities\\Spells\\Undead\\FrostArmor\\FrostArmorDamage.mdl"; 
    config->BUfa.Targetattach = "chest"; 
    config->BUfa.Targetart = "Abilities\\Spells\\Undead\\FrostArmor\\FrostArmorTarget.mdl"; 
    config->BUfa.Buffart = "ReplaceableTextures\\CommandButtons\\BTNFrostArmor.blp"; 
    config->AUdr.Order = "darkritual"; 
    config->AUdr.Targetart = "Abilities\\Spells\\Undead\\DarkRitual\\DarkRitualTarget.mdl"; 
    config->AUdr.Casterart = "Abilities\\Spells\\Undead\\DarkRitual\\DarkRitualCaster.mdl"; 
    config->AUdr.Researchbuttonpos = &MAKE(VECTOR3,2,0); 
    config->AUdr.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->AUdr.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDarkRitual.blp"; 
    config->AUdr.Art = "ReplaceableTextures\\CommandButtons\\BTNDarkRitual.blp"; 
    config->AUdd.Order = "deathanddecay"; 
    config->AUdd.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->AUdd.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->AUdd.Researchart = "ReplaceableTextures\\CommandButtons\\BTNDeathAndDecay.blp"; 
    config->AUdd.Art = "ReplaceableTextures\\CommandButtons\\BTNDeathAndDecay.blp"; 
    config->XUdd.Effectsoundlooped = "DeathAndDecayLoop"; 
    config->XUdd.Effectart = "Abilities\\Spells\\Undead\\DeathandDecay\\DeathandDecayTarget.mdl"; 
    config->BUdd.Targetart = "Abilities\\Spells\\Undead\\DeathandDecay\\DeathandDecayDamage.mdl"; 
    config->BUdd.Buffart = "ReplaceableTextures\\CommandButtons\\BTNDeathAndDecay.blp"; 
    config->Acri.order = "cripple"; 
    config->Acri.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Acri.Requiresamount = 2; 
    config->Acri.Requires = "Rune"; 
    config->Acri.Art = "ReplaceableTextures\\CommandButtons\\BTNCripple.blp"; 
    config->Scri.order = "cripple"; 
    config->Scri.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->Scri.Art = "ReplaceableTextures\\CommandButtons\\BTNCripple.blp"; 
    config->Bcri.Targetart = "Abilities\\Spells\\Undead\\Cripple\\CrippleTarget.mdl"; 
    config->Bcri.Buffart = "ReplaceableTextures\\CommandButtons\\BTNCripple.blp"; 
    config->Arst.Orderoff = "restorationoff"; 
    config->Arst.Orderon = "restorationon"; 
    config->Arst.Order = "restoration"; 
    config->Arst.Effectsoundlooped = "AcolyteRestoreLoop"; 
    config->Arst.Unbuttonpos = &MAKE(VECTOR3,1,1); 
    config->Arst.Buttonpos = &MAKE(VECTOR3,1,1); 
    config->Arst.Unart = "ReplaceableTextures\\CommandButtons\\BTNRepairOff.blp"; 
    config->Arst.Art = "ReplaceableTextures\\CommandButtons\\BTNRepairOn.blp"; 
    config->AUau.Targetattach = "origin"; 
    config->AUau.Targetart = "Abilities\\Spells\\Undead\\UnholyAura\\UnholyAura.mdl"; 
    config->AUau.Researchbuttonpos = &MAKE(VECTOR3,2,0); 
    config->AUau.Buttonpos = &MAKE(VECTOR3,2,2); 
    config->AUau.Researchart = "ReplaceableTextures\\CommandButtons\\BTNUnholyAura.blp"; 
    config->AUau.Art = "ReplaceableTextures\\PassiveButtons\\PASBTNUnholyAura.blp"; 
    config->BUau.Targetattach = "origin"; 
    config->BUau.Targetart = "Abilities\\Spells\\Other\\GeneralAuraTarget\\GeneralAuraTarget.mdl"; 
    config->BUau.Buffart = "ReplaceableTextures\\CommandButtons\\BTNUnholyAura.blp"; 
    config->AUin.Effectart = "Units\\Demon\\Infernal\\InfernalBirth.mdl"; 
    config->AUin.Order = "inferno"; 
    config->AUin.Researchbuttonpos = &MAKE(VECTOR3,3,0); 
    config->AUin.Buttonpos = &MAKE(VECTOR3,3,2); 
    config->AUin.Art = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->AUin.Researchart = "ReplaceableTextures\\CommandButtons\\BTNInfernal.blp"; 
    config->Xfus.Effectsoundlooped = "UndeadFireSmall"; 
    config->Xfus.Targetattach1 = "sprite,fourth"; 
    config->Xfus.Targetattach = "sprite,first"; 
    config->Xfus.Targetattachcount = 2; 
    config->Xfus.TargetArt = "Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire2.mdl,Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire1.mdl"; 
    config->Xfum.Effectsoundlooped = "UndeadFireMedium"; 
    config->Xfum.Targetattach3 = "sprite,fifth"; 
    config->Xfum.Targetattach2 = "sprite,fourth"; 
    config->Xfum.Targetattach1 = "sprite,second"; 
    config->Xfum.Targetattach = "sprite,first"; 
    config->Xfum.Targetattachcount = 4; 
    config->Xfum.TargetArt = "Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire2.mdl,Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire1.mdl,Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire0.mdl,Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire2.mdl"; 
    config->Xful.Effectsoundlooped = "UndeadFireLarge"; 
    config->Xful.Targetattach5 = "sprite,sixth"; 
    config->Xful.Targetattach4 = "sprite,fourth"; 
    config->Xful.Targetattach3 = "sprite,third"; 
    config->Xful.Targetattach2 = "sprite,fifth"; 
    config->Xful.Targetattach1 = "sprite,second"; 
    config->Xful.Targetattach = "sprite,first"; 
    config->Xful.Targetattachcount = 6; 
    config->Xful.TargetArt = "Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire1.mdl,Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire0.mdl,Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire0.mdl,Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire1.mdl,Environment\\UndeadBuildingFire\\UndeadLargeBuildingFire2.mdl,Environment\\UndeadBuildingFire\\UndeadSmallBuildingFire0.mdl"; 
}

