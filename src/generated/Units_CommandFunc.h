#pragma once
#include "../common/common.h"
typedef struct Units_CommandFunc {
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdMove;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdAttack;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdAttackGround;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdBuild;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdBuildHuman;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdBuildOrc;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdBuildNightElf;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdBuildUndead;
    union {
        LPVECTOR3 ButtonPos;
        LPCSTR Art;
    } CmdCancel;
    union {
        LPVECTOR3 ButtonPos;
        LPCSTR Art;
    } CmdCancelBuild;
    union {
        LPVECTOR3 ButtonPos;
        LPCSTR Art;
    } CmdCancelTrain;
    union {
        LPVECTOR3 ButtonPos;
        LPCSTR Art;
    } CmdCancelRevive;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdHoldPos;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdPatrol;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdPurchase;
    union {
        LPCSTR PlacementModel;
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdRally;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdSelectSkill;
    union {
        LPVECTOR3 Buttonpos;
        LPCSTR Art;
    } CmdStop;
} Units_CommandFunc;

void Init_Units_CommandFunc(Units_CommandFunc *config);
void Shutdown_Units_CommandFunc(Units_CommandFunc *config);

void Init_Units_CommandFunc(Units_CommandFunc *config) {
    config->CmdMove.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->CmdMove.Art = "CommandMove"; 
    config->CmdAttack.Buttonpos = &MAKE(VECTOR3,3,0); 
    config->CmdAttack.Art = "CommandAttack"; 
    config->CmdAttackGround.Buttonpos = &MAKE(VECTOR3,3,1); 
    config->CmdAttackGround.Art = "CommandAttackGround"; 
    config->CmdBuild.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->CmdBuild.Art = "CommandBasicStruct"; 
    config->CmdBuildHuman.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->CmdBuildHuman.Art = "CommandBasicStructHuman"; 
    config->CmdBuildOrc.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->CmdBuildOrc.Art = "CommandBasicStructOrc"; 
    config->CmdBuildNightElf.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->CmdBuildNightElf.Art = "CommandBasicStructNightElf"; 
    config->CmdBuildUndead.Buttonpos = &MAKE(VECTOR3,0,2); 
    config->CmdBuildUndead.Art = "CommandBasicStructUndead"; 
    config->CmdCancel.ButtonPos = &MAKE(VECTOR3,3,2); 
    config->CmdCancel.Art = "CommandCancel"; 
    config->CmdCancelBuild.ButtonPos = &MAKE(VECTOR3,3,2); 
    config->CmdCancelBuild.Art = "CommandCancel"; 
    config->CmdCancelTrain.ButtonPos = &MAKE(VECTOR3,3,2); 
    config->CmdCancelTrain.Art = "CommandCancel"; 
    config->CmdCancelRevive.ButtonPos = &MAKE(VECTOR3,3,2); 
    config->CmdCancelRevive.Art = "CommandCancel"; 
    config->CmdHoldPos.Buttonpos = &MAKE(VECTOR3,2,0); 
    config->CmdHoldPos.Art = "CommandHoldPosition"; 
    config->CmdPatrol.Buttonpos = &MAKE(VECTOR3,0,1); 
    config->CmdPatrol.Art = "CommandPatrol"; 
    config->CmdPurchase.Buttonpos = &MAKE(VECTOR3,0,0); 
    config->CmdPurchase.Art = "CommandPurchase"; 
    config->CmdRally.PlacementModel = "UI\\Feedback\\RallyPoint\\RallyPoint.mdl"; 
    config->CmdRally.Buttonpos = &MAKE(VECTOR3,3,1); 
    config->CmdRally.Art = "CommandRally"; 
    config->CmdSelectSkill.Buttonpos = &MAKE(VECTOR3,3,1); 
    config->CmdSelectSkill.Art = "CommandNewSkill"; 
    config->CmdStop.Buttonpos = &MAKE(VECTOR3,1,0); 
    config->CmdStop.Art = "CommandStop"; 
}

