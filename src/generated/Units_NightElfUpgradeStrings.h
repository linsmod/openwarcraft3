#pragma once
#include "../common/common.h"
typedef struct Units_NightElfUpgradeStrings {
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Resm;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Resw;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rema;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rerh;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Reuv;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Renb;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Reib;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Remk;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Resc;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Remg;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Redt;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Redc;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Resi;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Reht;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Recb;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Repb;
} Units_NightElfUpgradeStrings;

void Init_Units_NightElfUpgradeStrings(Units_NightElfUpgradeStrings *config);
void Shutdown_Units_NightElfUpgradeStrings(Units_NightElfUpgradeStrings *config);

void Init_Units_NightElfUpgradeStrings(Units_NightElfUpgradeStrings *config) {
    config->Resm.Hotkey = "M,M,M"; 
    config->Resm.Ubertip = {"增加弓箭手，女猎手，弩车和角鹰兽骑士的攻击力。"��中单位。|r"级。|n|n|cffffcc00能攻击地面单位和空中单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加弓箭手，女猎手，弩车和角鹰兽骑士的攻击力。"魔法值。"��近身攻击该目标的敌人在<AUfa,Dur1>秒内减速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加弓箭手，女猎手，弩车和角鹰兽骑士的攻击力。"魔法值。"��近身攻击该目标的敌人在<AUfa,Dur1>秒内减速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Resm.Tip = "升级到(|cffffcc00M|r)月之力量,升级到(|cffffcc00M|r)中级月之力量,升级到("; 
    config->Resm.Name = "月之力量,中级月之力量,高级月之力量"; 
    config->Resw.Hotkey = "W,W,W"; 
    config->Resw.Ubertip = {"增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Resw.Tip = "升级到(|cffffcc00W|r)野性力量,升级到(|cffffcc00W|r)中级野性力量,升级到(|cffffcc00W|r)高级野性力量"; 
    config->Resw.Name = "野性力量,中级野性力量,高级野性力量"; 
    config->Rema.Hotkey = "A,A,A"; 
    config->Rema.Ubertip = {"增加弓箭手，女猎手和角鹰兽骑士的防御力。"��的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加弓箭手，女猎手和角鹰兽骑士的防御力。"��的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加弓箭手，女猎手和角鹰兽骑士的防御力。"��的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的攻击力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Rema.Tip = "升级到(|cffffcc00A|r)月之护甲,升级到(|cffffcc00A|r)中级月之护甲,升级到"; 
    config->Rema.Name = "月之护甲,中级月之护甲,高级月之护甲"; 
    config->Rerh.Hotkey = "R,R,R"; 
    config->Rerh.Ubertip = {"增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Rerh.Tip = "升级到(|cffffcc00R|r)加强隐藏,升级到(|cffffcc00R|r)中级加强隐藏,升级到(|cffffcc00R|r)高级加强隐藏"; 
    config->Rerh.Name = "加强隐藏,中级加强隐藏,高级加强隐藏"; 
    config->Reuv.Hotkey = "U"; 
    config->Reuv.Ubertip = "使得暗夜精灵族单位能在夜间和白天看得一样远。"��禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Reuv.Tip = "升级到(|cffffcc00U|r)夜视能力"; 
    config->Reuv.Name = "夜视能力"; 
    config->Renb.Hotkey = "N"; 
    config->Renb.Ubertip = "能提高所有古树的移动速度和护甲。"��得一样远。"��禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Renb.Tip = "研究(|cffffcc00N|r)自然的祝福"; 
    config->Renb.Name = "自然的祝福"; 
    config->Reib.Hotkey = "I"; 
    config->Reib.Ubertip = "增加弓箭手和角鹰兽骑士的攻击范围。"��一样远。"��禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Reib.Tip = "研究(|cffffcc00I|r)硬弓"; 
    config->Reib.Name = "硬弓"; 
    config->Remk.Hotkey = "M"; 
    config->Remk.Ubertip = "能增加弓箭手和角鹰兽骑士<Remk,base1>点的攻击力。"��禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Remk.Tip = "研究(|cffffcc00M|r)射击术"; 
    config->Remk.Name = "射击术"; 
    config->Resc.Hotkey = "S"; 
    config->Resc.Ubertip = "使得女猎手能放出一只猫头鹰栖息在一棵树上进行侦察。能看见隐形单位。"��和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Resc.Tip = "研究(|cffffcc00S|r)哨兵"; 
    config->Resc.Name = "哨兵"; 
    config->Remg.Hotkey = "G"; 
    config->Remg.Ubertip = "使得女猎手的攻击能带有弹射效果。"��棵树上进行侦察。能看见隐形单位。"��和奇美拉怪兽的防御力。"�单位。|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Remg.Tip = "升级(|cffffcc00G|r)月刃"; 
    config->Remg.Name = "升级月刃"; 
    config->Redt.Hotkey = "A,A"; 
    config->Redt.Ubertip = {"增加猛禽德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有风暴之鸦技能。"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加猛禽德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有飓风技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Redt.Tip = "("; 
    config->Redt.Name = "猛禽德鲁伊专家训练,猛禽德鲁伊大师训练"; 
    config->Redc.Hotkey = "L,L"; 
    config->Redc.Ubertip = {"增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有生命恢复技能。"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-}; (null)
    config->Redc.Tip = "(|cffffcc00L|r)"; 
    config->Redc.Name = "利爪德鲁伊专家训练,利爪德鲁伊大师训练"; 
    config->Resi.Hotkey = "S"; 
    config->Resi.Ubertip = "使得树妖能去除掉敌人身上的有利魔法效果和友军单位身上的不利魔法效果。能伤害召唤出来的单位。"��能。"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Resi.Tip = "研究(|cffffcc00S|r)驱散魔法"; 
    config->Resi.Name = "驱散魔法"; 
    config->Reht.Hotkey = "I"; 
    config->Reht.Ubertip = "使得弓箭手能骑乘在角鹰兽的背上从而成为角鹰兽骑士。角鹰兽骑士既能攻击地面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Reht.Tip = "研究(|cffffcc00I|r)驯服角鹰兽"; 
    config->Reht.Name = "驯服角鹰兽"; 
    config->Recb.Hotkey = "B"; 
    config->Recb.Ubertip = "使得奇美拉怪兽能对敌方的建筑物喷出酸性的毒汁。"��。角鹰兽骑士既能攻击地面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Recb.Tip = "研究(|cffffcc00B|r)腐蚀喷吐"; 
    config->Recb.Name = "腐蚀喷吐"; 
    config->Repb.Hotkey = "P"; 
    config->Repb.Ubertip = "使得弩车的攻击能穿透敌人从而对其他站在后面的敌方单位也造成一定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加利爪德鲁伊暗夜精灵状态下的魔法最大值，魔法恢复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加变熊之后的利爪德鲁伊，变为风暴之鸦之后的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"�����-; (null)
    config->Repb.Tip = "研究(|cffffcc00P|r)穿刺之箭"; 
    config->Repb.Name = "穿刺之箭"; 
}

