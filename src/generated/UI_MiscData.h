#pragma once
#include "../common/common.h"
typedef struct UI_MiscData {
    union {
        LPCOLOR32 ActiveButtonColor;
        float BashTextFadeStart;
        float BashTextLifetime;
        LPVECTOR3 BashTextVelocity;
        LPCOLOR32 BashTextColor;
        float ManaBurnTextFadeStart;
        float ManaBurnTextLifetime;
        LPVECTOR3 ManaBurnTextVelocity;
        LPCOLOR32 ManaBurnTextColor;
        float CriticalStrikeTextFadeStart;
        float CriticalStrikeTextLifetime;
        LPVECTOR3 CriticalStrikeTextVelocity;
        LPCOLOR32 CriticalStrikeTextColor;
        float MissTextFadeStart;
        float MissTextLifetime;
        LPVECTOR3 MissTextVelocity;
        LPCOLOR32 MissTextColor;
        float BountyTextFadeStart;
        float BountyTextLifetime;
        LPVECTOR3 BountyTextVelocity;
        LPCOLOR32 BountyTextColor;
        float LumberTextFadeStart;
        float LumberTextLifetime;
        LPVECTOR3 LumberTextVelocity;
        LPCOLOR32 LumberTextColor;
        float GoldTextFadeStart;
        float GoldTextLifetime;
        LPVECTOR3 GoldTextVelocity;
        LPCOLOR32 GoldTextColor;
    } Misc;
    union {
        float QuestIndicatorTimeout;
    } QuestIndicatorTimeout;
    union {
        float bottom;
        float top;
        float right;
        float left;
    } CameraMargins;
    union {
        float bottom;
        float top;
        float right;
        float left;
    } UnpathableMargins;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } DefaultZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } TerrainZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } MenuZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } TutorialZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } HumanZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } OrcZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } UndeadZFog;
    union {
        LPCOLOR32 Color;
        float Density;
        float End;
        float Start;
        float Style;
    } NightElfZFog;
    union {
        LPCSTR MinHeight;
        float MaxHeight;
        float MaxSlope;
    } Terrain;
    union {
        float SmoothLevels;
        float MaximizeRadius;
    } FlyerMap;
    union {
        float MusicFadeInterval;
        float MusicFadeRate;
        float MaxDist;
        float MinDist;
        float Attenuation;
    } Sound;
    union {
        LPVECTOR3 Direction;
    } Light;
    union {
        float AllyOffset;
        LPCOLOR32 ColorEnemy;
        LPCOLOR32 ColorNeutral;
        LPCOLOR32 ColorFriend;
        float ImageWalkableZOffset;
        float ScaleFactor;
        LPCSTR TextureDotted02;
        LPCSTR Texture02;
        LPCSTR TextureDotted01;
        LPCSTR Texture01;
        float Size01;
        LPCSTR TextureDotted00;
        LPCSTR Texture00;
        float Size00;
        float NumSizes;
    } SelectionCircle;
    union {
        float DCT_d2;
        float DCT_d1;
        float DCT_sd;
        float DCT_s2;
        float DCT_s1;
        // Chance for a vertex to create a puff (per interval), in percent
        float PuffChance;
        // Interval between checks for new puffs, in milliseconds
        float PuffInterval;
        // Duration of a puff, in milliseconds
        float PuffDuration;
        LPCSTR PuffModel;
        LPCSTR Texture;
    } Blight;
    union {
        float WavesDepth;
        float DeepLevelPathing;
        float MaxDepth;
        float DeepLevel;
        float MinDepth;
    } Water;
    union {
        LPCOLOR32 FogColorBlightMask;
        float FogColorVisibleAlpha;
        float FogColorFogMaskAlpha;
        float FogColorDarkMaskAlpha;
        float FogColorBlackMaskAlpha;
        LPCOLOR32 FogColorCreepAllied;
        LPCOLOR32 FogColorDestructable;
        LPCOLOR32 FogColorHero;
        LPCOLOR32 FogColorItem;
        LPCOLOR32 FogColorResource;
        LPCOLOR32 FogColorEnemy;
        LPCOLOR32 FogColorAlly;
        LPCOLOR32 FogColorPlayer;
        LPCOLOR32 ShadowImageColor;
        LPVECTOR3 FoggedBoundaryObject;
        LPVECTOR3 BoundaryObject;
        LPVECTOR3 DarkMaskedObject;
        LPVECTOR3 BlackMaskedObject;
        LPVECTOR3 FoggedObject;
        LPVECTOR3 FoggedBoundaryTerrain;
        LPVECTOR3 BoundaryTerrain;
        LPVECTOR3 DarkMaskedTerrain;
        LPVECTOR3 BlackMaskedTerrain;
        LPVECTOR3 FoggedTerrain;
    } FogOfWar;
    union {
        LPCOLOR32 MinimapColorBorder;
    } Minimap;
    union {
        LPCOLOR32 SelTargetColor;
        LPCOLOR32 TargetLightColor;
    } TargetingColors;
    union {
        float CustomLoadTimerDuration;
        float ControlFadeDuration;
    } Glue;
    union {
        // lower bound, everything below this is "very fast"
        float AttackFast;
        float AttackAverage;
        float AttackSlow;
        // upper bound, everything above this is "very slow"
        float AttackVerySlow;
        // upper bound, everything above this is "very fast"
        float SpeedFast;
        float SpeedAverage;
        float SpeedSlow;
        // lower bound, everything below this is "very slow"
        float SpeedVerySlow;
        float MeleeRangeMax;
    } InfoPanel;
    union {
        // Uther
        float BattleNetTier5;
        // ArchMage
        float BattleNetTier4;
        // Knight
        float BattleNetTier3;
        // Footman
        float BattleNetTier2;
    } BattleNetTiers;
    union {
        float BuildingsOcclude;
        // Time to fade to OccluderColor, in milliseconds
        float OccluderFadeTime;
        LPCOLOR32 OccluderColor;
        LPCOLOR32 MarkColor;
        LPCSTR MarkImage;
    } Occlusion;
    union {
        float PlacementAlpha;
    } PlacementAlpha;
    union {
        LPCOLOR32 PingAdvisorColor;
        LPCOLOR32 PingAttackColor;
        LPCOLOR32 PingColor;
    } PingColor;
} UI_MiscData;

void Init_UI_MiscData(UI_MiscData *config);
void Shutdown_UI_MiscData(UI_MiscData *config);

void Init_UI_MiscData(UI_MiscData *config) {
    config->Misc.ActiveButtonColor = &MAKE(COLOR32,255,75,150,255); 
    config->Misc.BashTextFadeStart = 2; 
    config->Misc.BashTextLifetime = 5; 
    config->Misc.BashTextVelocity = &MAKE(VECTOR3,0,0.04,100); 
    config->Misc.BashTextColor = &MAKE(COLOR32,255,0,0,255); 
    config->Misc.ManaBurnTextFadeStart = 2; 
    config->Misc.ManaBurnTextLifetime = 5; 
    config->Misc.ManaBurnTextVelocity = &MAKE(VECTOR3,0,0.04,100); 
    config->Misc.ManaBurnTextColor = &MAKE(COLOR32,255,82,82,255); 
    config->Misc.CriticalStrikeTextFadeStart = 2; 
    config->Misc.CriticalStrikeTextLifetime = 5; 
    config->Misc.CriticalStrikeTextVelocity = &MAKE(VECTOR3,0,0.04,100); 
    config->Misc.CriticalStrikeTextColor = &MAKE(COLOR32,255,255,0,0); 
    config->Misc.MissTextFadeStart = 1; 
    config->Misc.MissTextLifetime = 3; 
    config->Misc.MissTextVelocity = &MAKE(VECTOR3,0,0.03,100); 
    config->Misc.MissTextColor = &MAKE(COLOR32,255,255,0,0); 
    config->Misc.BountyTextFadeStart = 2; 
    config->Misc.BountyTextLifetime = 3; 
    config->Misc.BountyTextVelocity = &MAKE(VECTOR3,0,0.03,100); 
    config->Misc.BountyTextColor = &MAKE(COLOR32,255,255,220,0); 
    config->Misc.LumberTextFadeStart = 1; 
    config->Misc.LumberTextLifetime = 2; 
    config->Misc.LumberTextVelocity = &MAKE(VECTOR3,0,0.03,100); 
    config->Misc.LumberTextColor = &MAKE(COLOR32,255,0,200,80); 
    config->Misc.GoldTextFadeStart = 1; 
    config->Misc.GoldTextLifetime = 2; 
    config->Misc.GoldTextVelocity = &MAKE(VECTOR3,0,0.03,100); 
    config->Misc.GoldTextColor = &MAKE(COLOR32,255,255,220,0); 
    config->QuestIndicatorTimeout.QuestIndicatorTimeout = 10; 
    config->CameraMargins.bottom = 2; 
    config->CameraMargins.top = 2; 
    config->CameraMargins.right = 4; 
    config->CameraMargins.left = 4; 
    config->UnpathableMargins.bottom = 4; 
    config->UnpathableMargins.top = 8; 
    config->UnpathableMargins.right = 6; 
    config->UnpathableMargins.left = 6; 
    config->DefaultZFog.Color = &MAKE(COLOR32,0,0,0,0); 
    config->DefaultZFog.Density = 0; 
    config->DefaultZFog.End = 50000; 
    config->DefaultZFog.Start = 20000; 
    config->DefaultZFog.Style = 0; 
    config->TerrainZFog.Color = &MAKE(COLOR32,255,0,0,0); 
    config->TerrainZFog.Density = 0.5; 
    config->TerrainZFog.End = 5000; 
    config->TerrainZFog.Start = 3000; 
    config->TerrainZFog.Style = 0; 
    config->MenuZFog.Color = &MAKE(COLOR32,255,11,20,11); 
    config->MenuZFog.Density = 1; 
    config->MenuZFog.End = 5500; 
    config->MenuZFog.Start = 100; 
    config->MenuZFog.Style = 0; 
    config->TutorialZFog.Color = &MAKE(COLOR32,255,0,51,77); 
    config->TutorialZFog.Density = 0.5; 
    config->TutorialZFog.End = 2000; 
    config->TutorialZFog.Start = 0; 
    config->TutorialZFog.Style = 0; 
    config->HumanZFog.Color = &MAKE(COLOR32,255,77,77,51); 
    config->HumanZFog.Density = 0.5; 
    config->HumanZFog.End = 9000; 
    config->HumanZFog.Start = 0; 
    config->HumanZFog.Style = 0; 
    config->OrcZFog.Color = &MAKE(COLOR32,255,77,51,0); 
    config->OrcZFog.Density = 0.5; 
    config->OrcZFog.End = 2950; 
    config->OrcZFog.Start = 0; 
    config->OrcZFog.Style = 0; 
    config->UndeadZFog.Color = &MAKE(COLOR32,255,0,51,51); 
    config->UndeadZFog.Density = 0.5; 
    config->UndeadZFog.End = 8000; 
    config->UndeadZFog.Start = 0; 
    config->UndeadZFog.Style = 0; 
    config->NightElfZFog.Color = &MAKE(COLOR32,255,51,77,128); 
    config->NightElfZFog.Density = 0.5; 
    config->NightElfZFog.End = 1600; 
    config->NightElfZFog.Start = 0; 
    config->NightElfZFog.Style = 0; 
    config->Terrain.MinHeight = "-128"; 
    config->Terrain.MaxHeight = 1536; 
    config->Terrain.MaxSlope = 50; 
    config->FlyerMap.SmoothLevels = 3; 
    config->FlyerMap.MaximizeRadius = 6; 
    config->Sound.MusicFadeInterval = 500; 
    config->Sound.MusicFadeRate = 16; 
    config->Sound.MaxDist = 20000; 
    config->Sound.MinDist = 1200; 
    config->Sound.Attenuation = 3.0; 
    config->Light.Direction = &MAKE(VECTOR3,0.3,0.3,-0.25); 
    config->SelectionCircle.AllyOffset = 0.2; 
    config->SelectionCircle.ColorEnemy = &MAKE(COLOR32,255,255,0,0); 
    config->SelectionCircle.ColorNeutral = &MAKE(COLOR32,255,255,255,0); 
    config->SelectionCircle.ColorFriend = &MAKE(COLOR32,255,0,255,0); 
    config->SelectionCircle.ImageWalkableZOffset = 15; 
    config->SelectionCircle.ScaleFactor = 72; 
    config->SelectionCircle.TextureDotted02 = "ReplaceableTextures\\Selection\\SelectionCircleLargeDotted"; 
    config->SelectionCircle.Texture02 = "ReplaceableTextures\\Selection\\SelectionCircleLarge"; 
    config->SelectionCircle.TextureDotted01 = "ReplaceableTextures\\Selection\\SelectionCircleMedDotted"; 
    config->SelectionCircle.Texture01 = "ReplaceableTextures\\Selection\\SelectionCircleMed"; 
    config->SelectionCircle.Size01 = 300; 
    config->SelectionCircle.TextureDotted00 = "ReplaceableTextures\\Selection\\SelectionCircleSmallDotted"; 
    config->SelectionCircle.Texture00 = "ReplaceableTextures\\Selection\\SelectionCircleSmall"; 
    config->SelectionCircle.Size00 = 100; 
    config->SelectionCircle.NumSizes = 3; 
    config->Blight.DCT_d2 = 20; 
    config->Blight.DCT_d1 = 5; 
    config->Blight.DCT_sd = 15; 
    config->Blight.DCT_s2 = 10; 
    config->Blight.DCT_s1 = 5; 
    config->Blight.PuffChance = 0.7; // Chance for a vertex to create a puff (per interval), in percent
    config->Blight.PuffInterval = 2500; // Interval between checks for new puffs, in milliseconds
    config->Blight.PuffDuration = 3000; // Duration of a puff, in milliseconds
    config->Blight.PuffModel = "Environment\\BlightDoodad\\BlightDoodad.mdl"; 
    config->Blight.Texture = "TerrainArt\\Blight\\Blight"; 
    config->Water.WavesDepth = 25; 
    config->Water.DeepLevelPathing = 52; 
    config->Water.MaxDepth = 72; 
    config->Water.DeepLevel = 64; 
    config->Water.MinDepth = 10; 
    config->FogOfWar.FogColorBlightMask = &MAKE(COLOR32,0,63,63,64); 
    config->FogOfWar.FogColorVisibleAlpha = 255; 
    config->FogOfWar.FogColorFogMaskAlpha = 128; 
    config->FogOfWar.FogColorDarkMaskAlpha = 40; 
    config->FogOfWar.FogColorBlackMaskAlpha = 0; 
    config->FogOfWar.FogColorCreepAllied = &MAKE(COLOR32,255,255,120,0); 
    config->FogOfWar.FogColorDestructable = &MAKE(COLOR32,255,0,160,128); 
    config->FogOfWar.FogColorHero = &MAKE(COLOR32,255,255,255,128); 
    config->FogOfWar.FogColorItem = &MAKE(COLOR32,255,240,240,0); 
    config->FogOfWar.FogColorResource = &MAKE(COLOR32,255,32,128,128); 
    config->FogOfWar.FogColorEnemy = &MAKE(COLOR32,255,255,0,0); 
    config->FogOfWar.FogColorAlly = &MAKE(COLOR32,255,255,255,0); 
    config->FogOfWar.FogColorPlayer = &MAKE(COLOR32,255,255,255,255); 
    config->FogOfWar.ShadowImageColor = &MAKE(COLOR32,170,0,0,0); 
    config->FogOfWar.FoggedBoundaryObject = &MAKE(VECTOR3,255,64,64,96); 
    config->FogOfWar.BoundaryObject = &MAKE(VECTOR3,255,0,0,0); 
    config->FogOfWar.DarkMaskedObject = &MAKE(VECTOR3,255,32,32,48); 
    config->FogOfWar.BlackMaskedObject = &MAKE(VECTOR3,255,0,0,0); 
    config->FogOfWar.FoggedObject = &MAKE(VECTOR3,255,64,64,96); 
    config->FogOfWar.FoggedBoundaryTerrain = &MAKE(VECTOR3,170,16,16,32); 
    config->FogOfWar.BoundaryTerrain = &MAKE(VECTOR3,230,0,0,0); 
    config->FogOfWar.DarkMaskedTerrain = &MAKE(VECTOR3,230,0,0,0); 
    config->FogOfWar.BlackMaskedTerrain = &MAKE(VECTOR3,255,0,0,0); 
    config->FogOfWar.FoggedTerrain = &MAKE(VECTOR3,170,16,16,32); 
    config->Minimap.MinimapColorBorder = &MAKE(COLOR32,255,60,60,70); 
    config->TargetingColors.SelTargetColor = &MAKE(COLOR32,255,240,200,0); 
    config->TargetingColors.TargetLightColor = &MAKE(COLOR32,180,0,255,0); 
    config->Glue.CustomLoadTimerDuration = 10.0; 
    config->Glue.ControlFadeDuration = 0.25; 
    config->InfoPanel.AttackFast = 1; // lower bound, everything below this is "very fast"
    config->InfoPanel.AttackAverage = 1.5; 
    config->InfoPanel.AttackSlow = 2; 
    config->InfoPanel.AttackVerySlow = 3; // upper bound, everything above this is "very slow"
    config->InfoPanel.SpeedFast = 350; // upper bound, everything above this is "very fast"
    config->InfoPanel.SpeedAverage = 280; 
    config->InfoPanel.SpeedSlow = 220; 
    config->InfoPanel.SpeedVerySlow = 175; // lower bound, everything below this is "very slow"
    config->InfoPanel.MeleeRangeMax = 128; 
    config->BattleNetTiers.BattleNetTier5 = 1500; // Uther
    config->BattleNetTiers.BattleNetTier4 = 500; // ArchMage
    config->BattleNetTiers.BattleNetTier3 = 250; // Knight
    config->BattleNetTiers.BattleNetTier2 = 25; // Footman
    config->Occlusion.BuildingsOcclude = 0; 
    config->Occlusion.OccluderFadeTime = 300; // Time to fade to OccluderColor, in milliseconds
    config->Occlusion.OccluderColor = &MAKE(COLOR32,128,255,255,255); 
    config->Occlusion.MarkColor = &MAKE(COLOR32,255,255,0,255); 
    config->Occlusion.MarkImage = "ReplaceableTextures\\Occlusion\\OcclusionMark"; 
    config->PlacementAlpha.PlacementAlpha = 200; 
    config->PingColor.PingAdvisorColor = &MAKE(COLOR32,255,255,255,0); 
    config->PingColor.PingAttackColor = &MAKE(COLOR32,255,255,0,0); 
    config->PingColor.PingColor = &MAKE(COLOR32,255,0,255,0); 
}

