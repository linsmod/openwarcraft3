#pragma once
#include "../common/common.h"
typedef struct Units_CommandStrings {
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdMove;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdAttack;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdAttackGround;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdBuild;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdBuildHuman;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdBuildOrc;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdBuildNightElf;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdBuildUndead;
    union {
        float Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdCancel;
    union {
        float Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdCancelBuild;
    union {
        float Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdCancelTrain;
    union {
        float Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdCancelRevive;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdHoldPos;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdPatrol;
    union {
        LPCSTR Hotkey;
        LPCSTR Tip;
    } CmdPurchase;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdRally;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdSelectSkill;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
    } CmdStop;
    union {
        LPCSTR Controlrevoked;
        LPCSTR Controlgranted;
        LPCSTR Goldminelow;
        LPCSTR Herodeath;
        LPCSTR Herokilledhero;
        LPCSTR Upkeeplevel;
        LPCSTR Goldminedestroyed;
        LPCSTR Lumberfromally;
        LPCSTR Goldfromally;
        LPCSTR Goldandlumberfromally;
        LPCSTR Allyminimapping;
        LPCSTR Allytownattack;
        LPCSTR Allyunderattack;
        LPCSTR Herodies;
        LPCSTR Townattack;
        LPCSTR Unitattack;
        LPCSTR BattleStations;
        LPCSTR Backtowork;
        LPCSTR Calltoarms;
        LPCSTR Cantdrop;
        LPCSTR Cooldownstock;
        LPCSTR Outofstock;
        LPCSTR Pitalreadysacrificing;
        LPCSTR Alreadybeinghealed;
        LPCSTR RepairHPmaxed;
        LPCSTR UnitHPmaxed;
        LPCSTR HPmanamaxed;
        LPCSTR Manamaxed;
        LPCSTR HPmaxed;
        LPCSTR Heromaxed;
        LPCSTR Notundead;
        LPCSTR Cantpolymorphunit;
        LPCSTR Illusionscantpickup;
        LPCSTR Illusionscantharvest;
        LPCSTR Notdot;
        LPCSTR Notmorphing;
        LPCSTR Notillusion;
        LPCSTR Notunsummoned;
        LPCSTR Nottransport;
        LPCSTR Notsummoned;
        LPCSTR Notancient;
        LPCSTR Notsapper;
        LPCSTR NeedInventory;
        LPCSTR Inventoryfull;
        LPCSTR Canttargetloc;
        LPCSTR Cantattackloc;
        LPCSTR Notdisabled;
        LPCSTR Notorganic;
        LPCSTR Notmechanical;
        LPCSTR Notbonecorpse;
        LPCSTR Notfleshycorpse;
        LPCSTR Notcorpse;
        LPCSTR Nohero;
        LPCSTR Notinvulnerable;
        LPCSTR Nothidden;
        LPCSTR Notenemy;
        LPCSTR Notneutral;
        LPCSTR Notfriendly;
        LPCSTR Notowned;
        LPCSTR Notself;
        LPCSTR Nottownhall;
        LPCSTR Notfrozenbldg;
        LPCSTR Nodebris;
        LPCSTR Nowall;
        LPCSTR Notree;
        LPCSTR Noitem;
        LPCSTR Noward;
        LPCSTR Nostructure;
        LPCSTR Noair;
        LPCSTR Noground;
        LPCSTR Nounits;
        LPCSTR Cantfindfriendlycorpse;
        LPCSTR Cantfindcorpse;
        LPCSTR Needemptytree;
        LPCSTR Targetpit;
        LPCSTR Targetacolyte;
        LPCSTR Targetwisp;
        LPCSTR Targetbunkerunit;
        LPCSTR Targetmanauser;
        LPCSTR Targetarmoredtransport;
        LPCSTR Targetancient;
        LPCSTR Targetorganicground;
        LPCSTR Targetmoveable;
        LPCSTR Targetmechanical;
        LPCSTR Targetundead;
        LPCSTR Targetbonecorpse;
        LPCSTR Targetfleshycorpse;
        LPCSTR Targetcorpse;
        LPCSTR Targetenemyhero;
        LPCSTR Targethero;
        LPCSTR Targetenemy;
        LPCSTR Targetneutral;
        LPCSTR Targetally;
        LPCSTR Targetowned;
        LPCSTR Targetrepair;
        LPCSTR Targettree;
        LPCSTR Targetitem;
        LPCSTR Targetward;
        LPCSTR Targetstructure;
        LPCSTR Targetair;
        LPCSTR Targetground;
        LPCSTR Targetstructuretree;
        LPCSTR Targetunit;
        LPCSTR Notthisunit;
        LPCSTR UnderRange;
        LPCSTR Notinrange;
        LPCSTR Minenotentangleable;
        LPCSTR Mustbeclosertomine;
        LPCSTR Mustroottoentangle;
        LPCSTR Cantrootunit;
        LPCSTR Cantroot;
        LPCSTR Cantland;
        LPCSTR Stumpblocked;
        LPCSTR Teleportfail;
        LPCSTR Buildingblocked;
        LPCSTR Tooclosetohall;
        LPCSTR Tooclosetomine;
        LPCSTR Offblight;
        LPCSTR Outofbounds;
        LPCSTR Cantplace;
        LPCSTR Cantsee;
        LPCSTR Archerridertarget;
        LPCSTR Mounthippogryphtarget;
        LPCSTR Coupletarget;
        LPCSTR Treeoccupied;
        LPCSTR Dispelmagictarget;
        LPCSTR Deathcoiltarget;
        LPCSTR Holybolttarget;
        LPCSTR Immunetomagic;
        LPCSTR Hibernating;
        LPCSTR Creeptoopowerful;
        LPCSTR Alreadyrebuilding;
        LPCSTR Underconstruction;
        LPCSTR Undeadbuilding;
        LPCSTR Humanbuilding;
        LPCSTR Targgetresources;
        LPCSTR Targgetmine;
        LPCSTR Nototherplayersmine;
        LPCSTR Acolytealreadymining;
        LPCSTR Blightringfull;
        LPCSTR Entangledminefull;
        LPCSTR Blightminefirst;
        LPCSTR Entangleminefirst;
        LPCSTR Targetblightedmine;
        LPCSTR Targetwispresources;
        LPCSTR Alreadyblightedmine;
        LPCSTR Alreadyentangled;
        LPCSTR Notblightedmine;
        LPCSTR Notentangledmine;
        LPCSTR Onlyattackers;
        LPCSTR Cantpossess;
        LPCSTR Cantspiritwolf;
        LPCSTR Cantcyclone;
        LPCSTR Cantdevour;
        LPCSTR Canttransport;
        LPCSTR Noroom;
        LPCSTR Cooldown;
        LPCSTR Nomana;
        LPCSTR Nolumber;
        LPCSTR Nogold;
        LPCSTR Maxsupply;
        LPCSTR Nofood;
        LPCSTR Itemincontrolgroup;
        LPCSTR Invalidcontrolgroup;
        LPCSTR Emptycontrolgroup;
    } Errors;
} Units_CommandStrings;

void Init_Units_CommandStrings(Units_CommandStrings *config);
void Shutdown_Units_CommandStrings(Units_CommandStrings *config);

void Init_Units_CommandStrings(Units_CommandStrings *config) {
    config->CmdMove.Hotkey = "M"; 
    config->CmdMove.Ubertip = "命令你的部队去目标区域，在移动的过程中对于敌人的攻击他们将不予理睬。如果将该命令指向某个单位，那么就等于施放了一个跟随命令。"点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdMove.Tip = "(|cffffcc00M|r)移动"; 
    config->CmdAttack.Hotkey = "A"; 
    config->CmdAttack.Ubertip = "命令你的部队去目标区域，在移动过程中他们会攻击遇到的所有敌人。如果将该命令指向某个单位，那么你的部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdAttack.Tip = "(|cffffcc00A|r)攻击"; 
    config->CmdAttackGround.Hotkey = "G"; 
    config->CmdAttackGround.Ubertip = "命令你的部队对于某个区域进行强攻，直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdAttackGround.Tip = "(|cffffcc00G|r)攻击地面"; 
    config->CmdBuild.Hotkey = "B"; 
    config->CmdBuild.Ubertip = "能显示出当前你所能建造的建筑物。"��直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdBuild.Tip = "(|cffffcc00B|r)建造建筑物"; 
    config->CmdBuildHuman.Hotkey = "B"; 
    config->CmdBuildHuman.Ubertip = "能显示出当前你所能建造的建筑物。"��直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdBuildHuman.Tip = "(|cffffcc00B|r)建造建筑物"; 
    config->CmdBuildOrc.Hotkey = "B"; 
    config->CmdBuildOrc.Ubertip = "能显示出当前你所能建造的建筑物。"��直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdBuildOrc.Tip = "(|cffffcc00B|r)建造建筑物"; 
    config->CmdBuildNightElf.Hotkey = "B"; 
    config->CmdBuildNightElf.Ubertip = "能显示出当前你所能建造的建筑物。"��直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdBuildNightElf.Tip = "(|cffffcc00B|r)建造建筑物"; 
    config->CmdBuildUndead.Hotkey = "B"; 
    config->CmdBuildUndead.Ubertip = "能显示出当前你所能建造的建筑物。"��直到他们被施放了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdBuildUndead.Tip = "(|cffffcc00B|r)召唤建筑物"; 
    config->CmdCancel.Hotkey = 512; 
    config->CmdCancel.Ubertip = "取消当前还未被执行的命令而改为施放其他的命令。"��了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdCancel.Tip = "(|cffffcc00ESC|r)取消"; 
    config->CmdCancelBuild.Hotkey = 512; 
    config->CmdCancelBuild.Ubertip = "取消当前的建造命令而改为施放其他的命令。"��令。"��了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdCancelBuild.Tip = "(|cffffcc00ESC|r)取消"; 
    config->CmdCancelTrain.Hotkey = 512; 
    config->CmdCancelTrain.Ubertip = "停止训练当前的单位。"��为施放其他的命令。"��令。"��了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdCancelTrain.Tip = "(|cffffcc00ESC|r)取消"; 
    config->CmdCancelRevive.Hotkey = 512; 
    config->CmdCancelRevive.Ubertip = "停止复活当前的单位。"��为施放其他的命令。"��令。"��了停止命令或者其他一个命令，他们才会停止进攻。"��部队会忽视其他单位的攻击而一直猛攻那个单位，直到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdCancelRevive.Tip = "(|cffffcc00ESC|r)取消"; 
    config->CmdHoldPos.Hotkey = "H"; 
    config->CmdHoldPos.Ubertip = "让你的部队位于原地不动，他们能对进入射程范围内的敌人进行攻击。被施放了该命令后，你的单位不会去追捕敌人的单位也不会去进攻敌人的远程攻击单位。"��到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdHoldPos.Tip = "(|cffffcc00H|r)保持原位"; 
    config->CmdPatrol.Hotkey = "P"; 
    config->CmdPatrol.Ubertip = "让你的部队在指定的两地之间进行巡逻。一旦敌人进入其射程，他们就会进行攻击。"��后，你的单位不会去追捕敌人的单位也不会去进攻敌人的远程攻击单位。"��到它被完全摧毁。"cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdPatrol.Tip = "(|cffffcc00P|r)巡逻"; 
    config->CmdPurchase.Hotkey = "s"; 
    config->CmdPurchase.Tip = "(|cffffcc00S|r)出售物品"; 
    config->CmdRally.Hotkey = "Y"; 
    config->CmdRally.Ubertip = "让生产出来的单位自动走到指定的地点。你可以将集结点设在金矿或者树木上以便让农民出来以后自动进行采集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdRally.Tip = "(|cffffcc00Y|r)设集结点"; 
    config->CmdSelectSkill.Hotkey = "O"; 
    config->CmdSelectSkill.Ubertip = "打开技能菜单，以便让你将点数来分配在英雄的各个技能上。"��矿或者树木上以便让农民出来以后自动进行采集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdSelectSkill.Tip = "(|cffffcc00O|r)英雄技能"; 
    config->CmdStop.Hotkey = "S"; 
    config->CmdStop.Ubertip = "无论单位先前被施放了什么命令都让其停止下来，不过他们随后也会对进入射程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"������; (null)
    config->CmdStop.Tip = "(|cffffcc00S|r)停止"; 
    config->Errors.Controlrevoked = "你已经丧失了对%s单位的控制权"; 
    config->Errors.Controlgranted = "你现在可以控制%s的单位了。"; 
    config->Errors.Goldminelow = "金矿快要采空了,金矿快要采空了,金矿快要采空了,金矿快要采空了"; 
    config->Errors.Herodeath = "%s，%s(等级%d)已经阵亡,%s，%s(等级"; 
    config->Errors.Herokilledhero = "%s被%s击败。"; 
    config->Errors.Upkeeplevel = "维修费等级%d。"; 
    config->Errors.Goldminedestroyed = "一个金矿已经倒塌,一个金矿已经倒塌,我们的金矿资源已经被采空,金矿已经被采空"; 
    config->Errors.Lumberfromally = "从%s那里收到%d木材。"; 
    config->Errors.Goldfromally = "从%s那里收到%d金矿。"; 
    config->Errors.Goldandlumberfromally = "从%s那里收到%d金矿和%d木材。"; 
    config->Errors.Allyminimapping = "%s已经标好了路。"; 
    config->Errors.Allytownattack = "%s的基地正遭受围攻!"; 
    config->Errors.Allyunderattack = "%s正遭受攻击。"; 
    config->Errors.Herodies = "%s已经阵亡。"; 
    config->Errors.Townattack = "基地正在被围攻!"; 
    config->Errors.Unitattack = "已经加入了战斗。"; 
    config->Errors.BattleStations = "找不到任何农民。"; 
    config->Errors.Backtowork = "找不到任何民兵。"; 
    config->Errors.Calltoarms = "找不到任何农民。"; 
    config->Errors.Cantdrop = "物品必须放于物品栏中。"; 
    config->Errors.Cooldownstock = "还未准备好。"; 
    config->Errors.Outofstock = "还未准备好。"; 
    config->Errors.Pitalreadysacrificing = "牺牲深渊正在牺牲一个侍僧。"; 
    config->Errors.Alreadybeinghealed = "目标正被医疗。"; 
    config->Errors.RepairHPmaxed = "目标并未受到伤害。"; 
    config->Errors.UnitHPmaxed = "生命值已满。"; 
    config->Errors.HPmanamaxed = "生命值和魔法值都已满。"; 
    config->Errors.Manamaxed = "魔法值已满。"; 
    config->Errors.HPmaxed = "英雄生命值已满。"; 
    config->Errors.Heromaxed = "英雄已达到最高等级。"; 
    config->Errors.Notundead = "不能以不死单位作为目标。"; 
    config->Errors.Cantpolymorphunit = "该单位对变形魔法免疫。"; 
    config->Errors.Illusionscantpickup = "幻象不能捡起物品。"; 
    config->Errors.Illusionscantharvest = "幻象不能采集资源。"; 
    config->Errors.Notdot = "不能以猛禽德鲁伊作为目标。"; 
    config->Errors.Notmorphing = "不能以变形单位作为目标。"; 
    config->Errors.Notillusion = "不能以幻象作为目标。"; 
    config->Errors.Notunsummoned = "该单位正在被反召唤。"; 
    config->Errors.Nottransport = "不能以运输机或者碉堡作为目标。"; 
    config->Errors.Notsummoned = "不能以召唤出来的单位作为目标。"; 
    config->Errors.Notancient = "不能以古树作为目标。"; 
    config->Errors.Notsapper = "不能以地精工兵作为目标。"; 
    config->Errors.NeedInventory = "只有英雄能捡起物品。"; 
    config->Errors.Inventoryfull = "物品已经满了。"; 
    config->Errors.Canttargetloc = "不能以那里作为目标。"; 
    config->Errors.Cantattackloc = "不能进攻那里。"; 
    config->Errors.Notdisabled = "魔法施放者不能进行移动。"; 
    config->Errors.Notorganic = "不能以非机械单位作为目标。"; 
    config->Errors.Notmechanical = "必须以非机械单位作为目标。"; 
    config->Errors.Notbonecorpse = "不能以骨骼尸体作为目标。"; 
    config->Errors.Notfleshycorpse = "不能以血肉尸体作为目标。"; 
    config->Errors.Notcorpse = "目标必须是有生单位。"; 
    config->Errors.Nohero = "不能以英雄作为目标。"; 
    config->Errors.Notinvulnerable = "目标是无敌的。"; 
    config->Errors.Nothidden = "不能以在建筑物或者运输机内的单位作为目标。"; 
    config->Errors.Notenemy = "不能以敌人的单位作为目标。"; 
    config->Errors.Notneutral = "不能以中立单位作为目标。"; 
    config->Errors.Notfriendly = "不能以友军的单位作为目标。"; 
    config->Errors.Notowned = "不能以自己的单位作为目标。"; 
    config->Errors.Notself = "不能以自身作为目标。"; 
    config->Errors.Nottownhall = "必须以友军的城镇大厅作为目标,必须以友军的大厅作为目标,必须以友军的城镇大厅作为目标,必须以友军的城镇大厅作为目标"; 
    config->Errors.Notfrozenbldg = "目标建筑物已经被冰冻住。"; 
    config->Errors.Nodebris = "不能以残骸作为目标。"; 
    config->Errors.Nowall = "不能以墙壁作为目标。"; 
    config->Errors.Notree = "不能以树木作为目标。"; 
    config->Errors.Noitem = "不能以物品作为目标。"; 
    config->Errors.Noward = "不能以守卫作为目标。"; 
    config->Errors.Nostructure = "不能以建筑物作为目标。"; 
    config->Errors.Noair = "不能以空中单位作为目标。"; 
    config->Errors.Noground = "不能以地面单位作为目标。"; 
    config->Errors.Nounits = "不能以单位作为目标。"; 
    config->Errors.Cantfindfriendlycorpse = "附近没有友军单位的尸体。"; 
    config->Errors.Cantfindcorpse = "附近没有可使用的尸体。"; 
    config->Errors.Needemptytree = "该树已经被一只猫头鹰占有。"; 
    config->Errors.Targetpit = "必须以一个牺牲深渊作为目标。"; 
    config->Errors.Targetacolyte = "必须以一个侍僧作为目标。"; 
    config->Errors.Targetwisp = "必须以一个小精灵作为目标。"; 
    config->Errors.Targetbunkerunit = "必须以一个农民作为目标。"; 
    config->Errors.Targetmanauser = "必须以一个带有魔法值的单位作为目标。"; 
    config->Errors.Targetarmoredtransport = "必须以装甲运输车作为目标"; 
    config->Errors.Targetancient = "必须以古树作为目标。"; 
    config->Errors.Targetorganicground = "必须以一个非机械地面单位作为目标。"; 
    config->Errors.Targetmoveable = "必须以一个可移动单位作为目标。"; 
    config->Errors.Targetmechanical = "必须以机械单位作为目标。"; 
    config->Errors.Targetundead = "必须以不死单位作为目标。"; 
    config->Errors.Targetbonecorpse = "必须以一件骨骼尸体作为目标。"; 
    config->Errors.Targetfleshycorpse = "必须以一件血肉尸体作为目标。"; 
    config->Errors.Targetcorpse = "必须以尸体作为目标。"; 
    config->Errors.Targetenemyhero = "必须以敌人的英雄作为目标。"; 
    config->Errors.Targethero = "必须以一个英雄作为目标。"; 
    config->Errors.Targetenemy = "必须以敌军的一个单位作为目标。"; 
    config->Errors.Targetneutral = "必须以一个中立单位作为目标。"; 
    config->Errors.Targetally = "必须以友军的一个单位作为目标。"; 
    config->Errors.Targetowned = "必须以你自己的单位作为目标。"; 
    config->Errors.Targetrepair = "必须以建筑物或者机械单位作为目标。"; 
    config->Errors.Targettree = "必须以树木作为目标。"; 
    config->Errors.Targetitem = "必须以一件物品来作为目标。"; 
    config->Errors.Targetward = "必须以一个守卫来作为目标。"; 
    config->Errors.Targetstructure = "必须以建筑物作为目标。"; 
    config->Errors.Targetair = "必须以空中单位作为目标。"; 
    config->Errors.Targetground = "必须以地面单位作为目标。"; 
    config->Errors.Targetstructuretree = "必须以树或者建筑物作为目标。"; 
    config->Errors.Targetunit = "必须以某个单位来作为目标。"; 
    config->Errors.Notthisunit = "不能以该单位作为目标。"; 
    config->Errors.UnderRange = "目标不在有效范围之内。"; 
    config->Errors.Notinrange = "目标不在有效范围之内。"; 
    config->Errors.Minenotentangleable = "该金矿不能被缠绕。"; 
    config->Errors.Mustbeclosertomine = "必须在金矿附近进行扎根。"; 
    config->Errors.Mustroottoentangle = "必须在金矿附近进行扎根。"; 
    config->Errors.Cantrootunit = "该单位不能在进行扎根了。"; 
    config->Errors.Cantroot = "不能在那里扎跟。"; 
    config->Errors.Cantland = "不能在那里着陆。"; 
    config->Errors.Stumpblocked = "什么东西封住了那个树桩。"; 
    config->Errors.Teleportfail = "该单位不能被传送。"; 
    config->Errors.Buildingblocked = "该新生产出的单位没有地方被放置。"; 
    config->Errors.Tooclosetohall = "不能在离城镇大厅过近的地方创建一个金矿。"; 
    config->Errors.Tooclosetomine = "不能建造在离金矿过近的地方。"; 
    config->Errors.Offblight = "必须在荒芜之地上召唤建筑物。"; 
    config->Errors.Outofbounds = "目标区域已超出地图边界。"; 
    config->Errors.Cantplace = "不能在那里进行建造。"; 
    config->Errors.Cantsee = "必须先探测那里。"; 
    config->Errors.Archerridertarget = "必须以一个弓箭手作为目标。"; 
    config->Errors.Mounthippogryphtarget = "必须以一头角鹰兽作为目标。"; 
    config->Errors.Coupletarget = "不能和该单位合为一体。"; 
    config->Errors.Treeoccupied = "该树已经被占用，请选择其他的树木。"; 
    config->Errors.Dispelmagictarget = "必须以一个单位或者一片荒芜之地作为目标。"; 
    config->Errors.Deathcoiltarget = "必须以友军或者敌人的有生单位作为目标。"; 
    config->Errors.Holybolttarget = "必须以友军或者敌人的有生单位作为目标。"; 
    config->Errors.Immunetomagic = "那单位对魔法免疫。"; 
    config->Errors.Hibernating = "那单位正在休眠。"; 
    config->Errors.Creeptoopowerful = "那野生单位太强大了。"; 
    config->Errors.Alreadyrebuilding = "该建筑物正在被建造。"; 
    config->Errors.Underconstruction = "该建筑物正在被建造。"; 
    config->Errors.Undeadbuilding = "必须以不死族的一个建筑物作为目标。"; 
    config->Errors.Humanbuilding = "必须以一个人类的建筑物作为目标。"; 
    config->Errors.Targgetresources = "必须以资源来作为目标。"; 
    config->Errors.Targgetmine = "必须以一个金矿作为目标。"; 
    config->Errors.Nototherplayersmine = "不能使用已经被别人占据的金矿。"; 
    config->Errors.Acolytealreadymining = "选中的侍僧已经在采矿了。"; 
    config->Errors.Blightringfull = "那个金矿不需要更多的侍僧。"; 
    config->Errors.Entangledminefull = "那个金矿不需要更多的小精灵。"; 
    config->Errors.Blightminefirst = "必须先让其变成一个闹鬼金矿。"; 
    config->Errors.Entangleminefirst = "必须先将金矿缠绕。"; 
    config->Errors.Targetblightedmine = "必须以一个闹鬼金矿为目标"; 
    config->Errors.Targetwispresources = "必须以一棵树木或者一个缠绕金矿为目标。"; 
    config->Errors.Alreadyblightedmine = "这金矿已经被闹鬼了。"; 
    config->Errors.Alreadyentangled = "这金矿已经被缠绕了。"; 
    config->Errors.Notblightedmine = "不能使用一个被闹鬼的金矿。"; 
    config->Errors.Notentangledmine = "不能使用一个被缠绕的金矿。"; 
    config->Errors.Onlyattackers = "只对进攻型单位有效。"; 
    config->Errors.Cantpossess = "不能在该单位上施放占据魔法。"; 
    config->Errors.Cantspiritwolf = "不能在该单位上施放野兽幽魂魔法。"; 
    config->Errors.Cantcyclone = "不能在该单位上施放飓风魔法。"; 
    config->Errors.Cantdevour = "不能吞噬该单位。"; 
    config->Errors.Canttransport = "不能装载该单位。"; 
    config->Errors.Noroom = "没有剩余的空间了。"; 
    config->Errors.Cooldown = "该技能还并未准备好。"; 
    config->Errors.Nomana = "魔法值不够。"; 
    config->Errors.Nolumber = "木材不够。"; 
    config->Errors.Nogold = "金矿不够。"; 
    config->Errors.Maxsupply = "达到人口上限，无法继续生产单位。"; 
    config->Errors.Nofood = "建造更多的农场来增加你的人口,建造更多的地洞来增加你的人口,召唤更多的通灵塔来增加你的人口,建造更多的月亮井来增加你的人口"; 
    config->Errors.Itemincontrolgroup = "不能将物品编入编队。"; 
    config->Errors.Invalidcontrolgroup = "不能编入你所不能控制的单位。"; 
    config->Errors.Emptycontrolgroup = "不能进行空编队。"; 
}

