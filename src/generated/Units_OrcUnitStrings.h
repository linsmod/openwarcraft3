#pragma once
#include "../common/common.h"
typedef struct Units_OrcUnitStrings {
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } oalt;
    union {
        LPCSTR Name;
    } oang;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } obar;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } obea;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Obla;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ocat;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } odoc;
    union {
        LPCSTR Name;
    } otot;
    union {
        LPCSTR Name;
    } oeye;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Ofar;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ofor;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ofrt;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ogre;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ogru;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ohun;
    union {
        LPCSTR Name;
    } ohwd;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } okod;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } osld;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } opeo;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } orai;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } oshm;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ostr;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } osw1;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } osw2;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } osw3;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } otau;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Otch;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } otrb;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } otto;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } owyv;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } owtw;
} Units_OrcUnitStrings;

void Init_Units_OrcUnitStrings(Units_OrcUnitStrings *config);
void Shutdown_Units_OrcUnitStrings(Units_OrcUnitStrings *config);

void Init_Units_OrcUnitStrings(Units_OrcUnitStrings *config) {
    config->oalt.Ubertip = "能召唤新的英雄和复活阵亡的英雄。"��最大值。还能补充暗夜精灵单位的魔法值和生命值。在夜间，它也能自我恢复魔法能量。"�|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->oalt.Tip = "建造(|cffffcc00A|r)风暴祭坛"; 
    config->oalt.Hotkey = "A"; 
    config->oalt.Name = "风暴祭坛"; 
    config->oang.Name = "远古守护者"; 
    config->obar.Ubertip = "兽族基本的产兵建筑物。能训练出兽人步兵，巨魔猎头者和投石车。还包括狂暴力量和巨魔再生的升级。"��自我恢复魔法能量。"�|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->obar.Tip = "建造(|cffffcc00B|r)兵营"; 
    config->obar.Hotkey = "B"; 
    config->obar.Name = "兵营"; 
    config->obea.Ubertip = "能训练出掠夺者，科多兽和双足飞龙。这里还包括诱捕，浸毒武器和战鼓的升级。"��和巨魔再生的升级。"��自我恢复魔法能量。"�|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->obea.Tip = "建造(|cffffcc00E|r)兽栏"; 
    config->obea.Hotkey = "E"; 
    config->obea.Name = "兽栏"; 
    config->Obla.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-blademaster.blp"; 
    config->Obla.Revivetip = "复活(|cffffcc00B|r)剑圣"; 
    config->Obla.Ubertip = "一种较为灵活的英雄，特别擅长于一对一。能学习到镜像，疾步风，致命一击和剑刃风暴这四种技能。|n|n|cffffcc00能攻击地面单位。|r"cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Obla.Tip = "召唤(|cffffcc00B|r)剑圣"; 
    config->Obla.Hotkey = "B"; 
    config->Obla.Propernames = "托伽拉,尼可拉,卡金德,米恺撒,萨穆罗,阿可喏斯,玛组鲁,尤舒拉,达舍,可伽米,阿拉斯凯奇,穆哥尔,朱倍尔"; 
    config->Obla.Name = "剑圣"; 
    config->ocat.Ubertip = "一种重型攻城武器，特别擅长于对付敌人的建筑物，但是同自己本身也很容易遭到攻击。|n|n|cffffcc00能攻击地面单位和树木。|r"位。|r"cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ocat.Tip = "训练(|cffffcc00C|r)投石车"; 
    config->ocat.Hotkey = "C"; 
    config->ocat.Name = "投石车"; 
    config->odoc.Casterupgradetip = {"- 岗哨守卫"城武器，特别擅长于对付敌人的建筑物，但是同自己本身也很容易遭到攻击。|n|n|cffffcc00能攻击地面单位和树木。|r"位。|r"cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->odoc.Casterupgradename = "初级,专家级,大师级"; 
    config->odoc.Ubertip = "一种魔法单位，一开始能施放岗哨魔法，从而能侦察到一定的区域。随后这种单位还能学习到静止陷阱和治疗守卫。|n|n|cffffcc00能攻击地面单位和空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->odoc.Tip = "训练(|cffffcc00W|r)巨魔巫医"; 
    config->odoc.Hotkey = "W"; 
    config->odoc.Name = "巨魔巫医"; 
    config->otot.Name = "静止陷阱"; 
    config->oeye.Name = "岗哨守卫"; 
    config->Ofar.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-farseer.blp"; 
    config->Ofar.Revivetip = "复活(|cffffcc00F|r)先知"; 
    config->Ofar.Ubertip = "一种神秘的英雄，特别擅长于远程攻击和侦察。能学习到闪电链，透视，野兽幽魂和地震这四种技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Ofar.Tip = "召唤(|cffffcc00F|r)先知"; 
    config->Ofar.Hotkey = "F"; 
    config->Ofar.Propernames = "伽德尔,尼盖尔,卡滋尔,玛奇斯,巴尔,盖罗,卡伽尔,那滋盖尔,摩盖尔,卡拉盖尔,费恩理斯"; 
    config->Ofar.Name = "先知"; 
    config->ofor.Ubertip = "能存储采集到的木材。这里还包括对兽族各种单位的攻防升级，尖刺障碍也是在这里进行研究的。"��。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ofor.Tip = "建造(|cffffcc00M|r)战争磨坊"; 
    config->ofor.Hotkey = "M"; 
    config->ofor.Name = "战争磨坊"; 
    config->ofrt.Ubertip = "升级到了堡垒之后能让玩家建造许多新的建筑物和单位。"��级，尖刺障碍也是在这里进行研究的。"��。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ofrt.Tip = "(|cffffcc00U|r)升级到堡垒"; 
    config->ofrt.Hotkey = "U"; 
    config->ofrt.Name = "堡垒"; 
    config->ogre.Ubertip = "兽族的基本建筑物。能训练出苦工，在升级到了要塞和堡垒之后能让玩家建造许多新的建筑物和单位。"n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ogre.Tip = "建造(|cffffcc00H|r)大厅"; 
    config->ogre.Hotkey = "H"; 
    config->ogre.Name = "大厅"; 
    config->ogru.Ubertip = "基本的兽族地面单位。能得到狂暴力量的升级。|n|n|cffffcc00能攻击地面单位。|r"�多新的建筑物和单位。"n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ogru.Tip = "训练(|cffffcc00G|r)兽人步兵"; 
    config->ogru.Hotkey = "G"; 
    config->ogru.Name = "兽人步兵"; 
    config->ohun.Ubertip = "一种能有效对空的单位。能学习到巨魔再生技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r "和单位。"n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ohun.Tip = "训练(|cffffcc00H|r)巨魔猎头者"; 
    config->ohun.Hotkey = "H"; 
    config->ohun.Name = "巨魔猎头者"; 
    config->ohwd.Name = "治疗守卫"; 
    config->okod.Ubertip = "一种笨重的战争野兽，上面骑着一个兽人鼓手。能学到战鼓和吞噬技能。战鼓能提高周围单位的攻击力，它本身也能进行升级。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->okod.Tip = "训练(|cffffcc00K|r)科多兽"; 
    config->okod.Hotkey = "K"; 
    config->okod.Name = "科多兽"; 
    config->osld.Ubertip = "能生产出兽族的魔法单位：萨满祭司和巨魔巫医。这里也可以进行对萨满祭司和巨魔巫医的各种魔法升级。"��身也能进行升级。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->osld.Tip = "建造(|cffffcc00S|r)灵魂归宿"; 
    config->osld.Hotkey = "S"; 
    config->osld.Name = "灵魂归宿"; 
    config->opeo.Ubertip = "兽族的基本工人单位。能采集金矿和木材。还能建造建筑物和进行修理。在钻入地洞以后还能对来犯的敌人进行反击。|n|n|cffffcc00能攻击地面单位和树木。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->opeo.Tip = "训练(|cffffcc00P|r)苦工"; 
    config->opeo.Hotkey = "P"; 
    config->opeo.Name = "苦工"; 
    config->orai.Ubertip = "一种机动性很强的狼骑士。对付建筑物特别的有效，能学习到诱捕技能。|n|n|cffffcc00能攻击地面单位。|r"�人进行反击。|n|n|cffffcc00能攻击地面单位和树木。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 岗哨守卫|n- 静止陷阱|n- 治疗守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->orai.Tip = "训练(|cffffcc00R|r)掠夺者"; 
    config->orai.Hotkey = "R"; 
    config->orai.Name = "掠夺者"; 
    config->oshm.Casterupgradetip = {"- 净化"动性很强的狼骑士。对付建筑物特别的有效，能学习到诱捕技能。|n|n|cffffcc00能攻击地面单位。|r"�人进行反击。|n|n|cffffcc00能攻击地面单位和树木。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->oshm.Casterupgradename = "初级,专家级,大师级"; 
    config->oshm.Ubertip = "一种魔法单位。一开始能施放净化技能，从而能减慢敌人的速度和驱逐其身上的魔法效果。随后还能学习到闪电护盾和嗜血术。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->oshm.Tip = "训练(|cffffcc00S|r)萨满祭司"; 
    config->oshm.Hotkey = "S"; 
    config->oshm.Name = "萨满祭司"; 
    config->ostr.Ubertip = "在升级到要塞以后能使玩家建造许多新的建筑物和单位。"��速度和驱逐其身上的魔法效果。随后还能学习到闪电护盾和嗜血术。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ostr.Tip = "(|cffffcc00U|r)升级到要塞"; 
    config->ostr.Hotkey = "U"; 
    config->ostr.Name = "要塞"; 
    config->osw1.EditorSuffix = "(等级"; 
    config->osw1.Name = "幽魂之狼"; 
    config->osw2.EditorSuffix = "(等级"; 
    config->osw2.Name = "恐惧之狼"; 
    config->osw3.EditorSuffix = "(等级"; 
    config->osw3.Name = "阴影之狼"; 
    config->otau.Ubertip = "一种大型的单位，能学习到粉碎技能。|n|n|cffffcc00能攻击地面单位。|r"�其身上的魔法效果。随后还能学习到闪电护盾和嗜血术。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->otau.Tip = "训练(|cffffcc00T|r)牛头人"; 
    config->otau.Hotkey = "T"; 
    config->otau.Name = "牛头人"; 
    config->Otch.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-taurenchieftain.blp"; 
    config->Otch.Revivetip = "复活(|cffffcc00T|r)牛头人酋长"; 
    config->Otch.Ubertip = "一种战士型英雄，特别擅长于近战和挨打。能学习到震荡波，战争践踏，耐久光环和重生这四种技能。|n|n|cffffcc00能攻击地面单位。|r"ffcc00能攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Otch.Tip = "召唤(|cffffcc00T|r)牛头人酋长"; 
    config->Otch.Hotkey = "T"; 
    config->Otch.Propernames = "玛恩,塞盖尔,塔穆,德恩,卡穆,可尔,穆恩尔,盖罗可,玛拉尔,塞阿尔"; 
    config->Otch.Name = "牛头人酋长"; 
    config->otrb.Ubertip = "能提供人口，从而增加可造单位数量的最大值。苦工在进入其中以后还能对来犯的敌人进行反击。|n|n|cffffcc00能攻击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->otrb.Tip = "建造(|cffffcc00O|r)兽人地洞"; 
    config->otrb.Hotkey = "O"; 
    config->otrb.Name = "兽人地洞"; 
    config->otto.Ubertip = "能训练出牛头人来。 还包括对粉碎技能的研究。"�工在进入其中以后还能对来犯的敌人进行反击。|n|n|cffffcc00能攻击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->otto.Tip = "建造(|cffffcc00T|r)牛头人图腾"; 
    config->otto.Hotkey = "T"; 
    config->otto.Name = "牛头人图腾"; 
    config->owyv.Ubertip = "一种高度机动的飞行单位。特别擅长于侦察。能获得浸毒武器的升级。|n|n|cffffcc00能攻击地面单位和空中单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->owyv.Tip = "训练(|cffffcc00W|r)双足飞龙"; 
    config->owyv.Hotkey = "W"; 
    config->owyv.Name = "双足飞龙"; 
    config->owtw.Ubertip = "兽族的防御性建筑物。 |n|n|cffffcc00能攻击地面单位和空中单位。|r"级。|n|n|cffffcc00能攻击地面单位和空中单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->owtw.Tip = "建造(|cffffcc00W|r)了望塔"; 
    config->owtw.Hotkey = "W"; 
    config->owtw.Name = "了望塔"; 
}

