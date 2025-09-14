#pragma once
#include "../common/common.h"
typedef struct Units_NightElfUnitStrings {
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ebal;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } etrp;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } echm;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        float Casterupgradecount;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } edoc;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } edcm;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } edot;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } edtm;
    union {
        LPCSTR Name;
    } egol;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ewsp;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Propernames;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } Edem;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Propernames;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } Edmm;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Propernames;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } Ekee;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Propernames;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } Emoo;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } esen;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } earc;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } edry;
    union {
        LPCSTR Name;
    } efon;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ehip;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } ehpr;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } etol;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } etoa;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } etoe;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } edob;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } eate;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } eaoe;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } eaom;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } eaow;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } edos;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } emow;
} Units_NightElfUnitStrings;

void Init_Units_NightElfUnitStrings(Units_NightElfUnitStrings *config);
void Shutdown_Units_NightElfUnitStrings(Units_NightElfUnitStrings *config);

void Init_Units_NightElfUnitStrings(Units_NightElfUnitStrings *config) {
    config->ebal.Ubertip = "一种远距离的攻城武器。对付建筑物特别有效。还能得到穿刺之箭的升级。|n|n|cffffcc00能攻击地面单位和树木。|r"|r"|r"。|r"|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->ebal.Tip = "训练(|cffffcc00B|r)弩车"; 
    config->ebal.Hotkey = "B"; 
    config->ebal.Name = "弩车"; 
    config->etrp.Ubertip = "一种防御性古树。在扎根以后，会向空中投掷大量的石块以对来犯的敌人进行反击。|n|n|cffffcc00能攻击地面和空中单位。|r"|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->etrp.Tip = "建造(|cffffcc00P|r)远古保护者"; 
    config->etrp.Hotkey = "P"; 
    config->etrp.Name = "远古保护者"; 
    config->echm.Ubertip = "一种双头飞龙。能学到腐蚀喷吐技能。|n|n|cffffcc00能攻击地面单位。|r"�人进行反击。|n|n|cffffcc00能攻击地面和空中单位。|r"|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->echm.Tip = "训练(|cffffcc00C|r)"; 
    config->echm.Hotkey = "C"; 
    config->echm.Name = "奇美拉"; 
    config->edoc.Casterupgradetip = {"- 咆哮"头飞龙。能学到腐蚀喷吐技能。|n|n|cffffcc00能攻击地面单位。|r"�人进行反击。|n|n|cffffcc00能攻击地面和空中单位。|r"|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复|n- 变熊"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����}; (null)
    config->edoc.Casterupgradename = "初级,专家级,大师级"; 
    config->edoc.Casterupgradecount = 3; 
    config->edoc.Ubertip = "一种近战型魔法单位。一开始能施放咆哮技能，从而可以增加周围单位的攻击力。随后还能学习到生命恢复和变熊这两种技能。|n|n|cffffcc00能攻击地面单位。|r"��位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复|n- 变熊"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edoc.Tip = "训练(|cffffcc00C|r)利爪德鲁伊"; 
    config->edoc.Hotkey = "C"; 
    config->edoc.EditorSuffix = "(暗夜精灵状态)"; 
    config->edoc.Name = "利爪德鲁伊"; 
    config->edcm.Casterupgradetip = {"- 咆哮"战型魔法单位。一开始能施放咆哮技能，从而可以增加周围单位的攻击力。随后还能学习到生命恢复和变熊这两种技能。|n|n|cffffcc00能攻击地面单位。|r"��位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复|n- 变熊"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����}; (null)
    config->edcm.Casterupgradename = "初级,专家级,大师级"; 
    config->edcm.Ubertip = "一种近战型魔法单位。一开始能施放咆哮技能，从而可以增加周围单位的攻击力。随后还能学习到生命恢复和变熊这两种技能。|n|n|cffffcc00能攻击地面单位。|r"��位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 咆哮|n- 生命恢复|n- 变熊"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edcm.Tip = "训练(|cffffcc00C|r)"; 
    config->edcm.Hotkey = "C"; 
    config->edcm.EditorSuffix = "(熊的状态)"; 
    config->edcm.Name = "利爪德鲁伊"; 
    config->edot.Casterupgradetip = {"- 精灵之火"魔法单位。一开始能施放咆哮技能，从而可以增加周围单位的攻击力。随后还能学习到生命恢复和变熊这两种技能。|n|n|cffffcc00能攻击地面单位。|r"��位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����}; (null)
    config->edot.Casterupgradename = "初级,专家级,大师级"; 
    config->edot.Ubertip = "一种灵活的魔法单位。开始就能施放精灵之火，从而能降低某个单位的护甲并让其不能隐形。随后还能学习到风暴之鸦和飓风这两种技能。|n|n|cffffcc00能攻击地面和空中单位。|r"0能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edot.Tip = "训练(|cffffcc00T|r)"; 
    config->edot.Hotkey = "T"; 
    config->edot.EditorSuffix = "(暗夜精灵状态)"; 
    config->edot.Name = "猛禽德鲁伊"; 
    config->edtm.Casterupgradetip = {"- 精灵之火"魔法单位。开始就能施放精灵之火，从而能降低某个单位的护甲并让其不能隐形。随后还能学习到风暴之鸦和飓风这两种技能。|n|n|cffffcc00能攻击地面和空中单位。|r"0能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����}; (null)
    config->edtm.Casterupgradename = "初级,专家级,大师级"; 
    config->edtm.Ubertip = "一种灵活的魔法单位。开始就能施放精灵之火，从而能降低某个单位的护甲并让其不能隐形。随后还能学习到风暴之鸦和飓风这两种技能。|n|n|cffffcc00能攻击地面和空中单位。|r "能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edtm.Tip = "训练"; 
    config->edtm.Hotkey = "T"; 
    config->edtm.EditorSuffix = "(风暴之鸦状态)"; 
    config->edtm.Name = "猛禽德鲁伊"; 
    config->egol.Name = "被缠绕金矿"; 
    config->ewsp.Ubertip = "暗夜精灵族基本的工人单位。能采集金矿和木材。还能建造精灵族的建筑物并进行修理更新。能自我爆炸从而伤害到周围被召唤出来的单位并吸收一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->ewsp.Tip = "训练(|cffffcc00W|r)小精灵"; 
    config->ewsp.Hotkey = "W"; 
    config->ewsp.Name = "小精灵"; 
    config->Edem.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-demonhunter.blp"; 
    config->Edem.Propernames = "影之歌,影之愤怒,黑暗语者,追火者,黑暗舞者,黑暗恐惧,黑暗悲伤,信德维拉,伤痛杀手,地狱守护者,狂怒之人,愤怒狂奔者,火焰舞者,血之愤怒,恐惧剑刃"; 
    config->Edem.Revivetip = "复活(|cffffcc00D|r)恶魔猎手"; 
    config->Edem.Ubertip = "一种灵活的英雄，能学习到献祭，闪避法力燃烧和变身这四项技能。|n|n|cffffcc00能攻击地面单位。|r"�爆炸从而伤害到周围被召唤出来的单位并吸收一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->Edem.Tip = "召唤(|cffffcc00D|r)恶魔猎手"; 
    config->Edem.Hotkey = "D"; 
    config->Edem.Name = "恶魔猎手"; 
    config->Edmm.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-demonhunter.blp"; 
    config->Edmm.Propernames = "影之歌,影之愤怒,黑暗语者,追火者,黑暗舞者,黑暗恐惧,黑暗悲伤,信德维拉,伤痛杀手,地狱守护者,狂怒之人,愤怒狂奔者,火焰舞者,血之愤怒,恐惧剑刃"; 
    config->Edmm.Revivetip = "复活(|cffffcc00D|r)恶魔猎手"; 
    config->Edmm.Ubertip = "一种灵活的英雄，能学习到献祭，闪避法力燃烧和变身这四项技能。|n|n|cffffcc00能攻击地面单位。|r"�爆炸从而伤害到周围被召唤出来的单位并吸收一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->Edmm.Tip = "召唤(|cffffcc00D|r)恶魔猎手"; 
    config->Edmm.Hotkey = "D"; 
    config->Edmm.EditorSuffix = "(恶魔形态)"; 
    config->Edmm.Name = "恶魔猎手"; 
    config->Ekee.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-keeperofthegrove.blp"; 
    config->Ekee.Propernames = "拉罗德,安奴比理司,南迪倍,卡理法可司,斑德拉尔,马罗内,哥尔比恩,达哥达,奴达,奥哥玛,塞恩特理司,塞理德恩"; 
    config->Ekee.Revivetip = "复活(|cffffcc00K|r)丛林守护者"; 
    config->Ekee.Ubertip = "一种神秘的英雄，特别擅长于自然类的魔法。能学习到纠缠根须，自然之力，荆刺光环和宁静这四项技能。|n|n|cffffcc00能攻击地面和空中单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->Ekee.Tip = "召唤(|cffffcc00K|r)丛林守护者"; 
    config->Ekee.Hotkey = "K"; 
    config->Ekee.Name = "丛林守护者"; 
    config->Emoo.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-priestessofthemoon.blp"; 
    config->Emoo.Propernames = "卡塞理司,阿多拉,摩拉,费莱理,安娜拉,可拉,摩维,德拉司,米拉,塞塔,特伽拉,阿理尔,迪安娜"; 
    config->Emoo.Revivetip = "复活(|cffffcc00P|r)月之女祭司"; 
    config->Emoo.Ubertip = "战士型英雄，擅长于远程攻击。能学习到侦察，灼热之箭，强击光环和群星坠落这四项技能。|n|n|cffffcc00能攻击地面和空中单位。|r"空中单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->Emoo.Tip = "召唤(|cffffcc00P|r)月之女祭司"; 
    config->Emoo.Hotkey = "P"; 
    config->Emoo.Name = "月之女祭司"; 
    config->esen.Ubertip = "一种灵活的远程攻击单位，能学习到哨兵和月刃技能。 |n|n|cffffcc00能攻击地面单位。|r"技能。|n|n|cffffcc00能攻击地面和空中单位。|r"空中单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->esen.Tip = "训练(|cffffcc00H|r)女猎手"; 
    config->esen.Hotkey = "H"; 
    config->esen.Name = "女猎手"; 
    config->earc.Ubertip = "一种基本的远程攻击单位。能学习到射击术，硬弓和驯服角鹰兽这三项技能。|n|n|cffffcc00能攻击地面和空中单位。|r"和空中单位。|r"空中单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->earc.Tip = "训练(|cffffcc00A|r)弓箭手"; 
    config->earc.Hotkey = "A"; 
    config->earc.Name = "弓箭手"; 
    config->edry.Ubertip = "她的毒性攻击能减慢敌人的速度并慢慢地消耗敌人的生命值。她还具有驱魔技能和魔法免疫技能。|n|n|cffffcc00能攻击地面和空中单位。|r"单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edry.Tip = "训练(|cffffcc00D|r)树妖"; 
    config->edry.Hotkey = "D"; 
    config->edry.Name = "树妖"; 
    config->efon.Name = "树人"; 
    config->ehip.Ubertip = "一种近战型飞行单位。能学习到驯服角鹰兽技能。|n|n|cffffcc00能攻击空中单位。|r"�魔法免疫技能。|n|n|cffffcc00能攻击地面和空中单位。|r"单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->ehip.Tip = "训练(|cffffcc00H|r)角鹰兽"; 
    config->ehip.Hotkey = "H"; 
    config->ehip.Name = "角鹰兽"; 
    config->ehpr.Ubertip = "弓箭手骑乘在了角鹰兽上面就成为了角鹰兽骑士。 |n|n|cffffcc00能攻击地面和空中单位。|r"疫技能。|n|n|cffffcc00能攻击地面和空中单位。|r"单位。|r"�一定范围内所有单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->ehpr.Tip = "骑乘(|cffffcc00R|r)角鹰兽"; 
    config->ehpr.Hotkey = "R"; 
    config->ehpr.Name = "角鹰兽骑士"; 
    config->etol.Ubertip = "暗夜精灵族的基本建筑物。能训练小精灵和缠绕金矿。在升级到了远古之树和永恒之树之后能让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->etol.Tip = "建造(|cffffcc00T|r)生命之树"; 
    config->etol.Hotkey = "T"; 
    config->etol.Name = "生命之树"; 
    config->etoa.Ubertip = "升级到了远古之树之后能让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->etoa.Tip = "(|cffffcc00U|r)升级到远古之树"; 
    config->etoa.Hotkey = "U"; 
    config->etoa.Name = "远古之树"; 
    config->etoe.Ubertip = "升级到了永恒之树之后能让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->etoe.Tip = "(|cffffcc00U|r)升级到永恒之树"; 
    config->etoe.Hotkey = "U"; 
    config->etoe.Name = "永恒之树"; 
    config->edob.Ubertip = "能对所有单位的攻防进行升级，还包括对夜视能力的升级。"��|n|n|cffffcc00能攻击地面单位。|r"�让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edob.Tip = "建造(|cffffcc00H|r)猎手大厅"; 
    config->edob.Hotkey = "H"; 
    config->edob.Name = "猎手大厅"; 
    config->eate.Ubertip = "能在这里召唤新英雄，复活阵亡的英雄。"��视能力的升级。"��|n|n|cffffcc00能攻击地面单位。|r"�让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->eate.Tip = "建造(|cffffcc00A|r)"; 
    config->eate.Hotkey = "A"; 
    config->eate.Name = "长者祭坛"; 
    config->eaoe.Ubertip = "能生产出暗夜精灵族的地面魔法单位：利爪德鲁伊和树妖。还包括对利爪德鲁伊和驱魔技能的升级。|n|n|cffffcc00能攻击地面单位。|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->eaoe.Tip = "建造(|cffffcc00L|r)知识古树"; 
    config->eaoe.Hotkey = "L"; 
    config->eaoe.Name = "知识古树"; 
    config->eaom.Ubertip = "能生产出：弓箭手，女猎手和弩车。还包括对弓箭手，女猎手和弩车的各类升级。|n|n|cffffcc00能攻击地面单位。|r"攻击地面单位。|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->eaom.Tip = "建造(|cffffcc00R|r)战争古树"; 
    config->eaom.Hotkey = "R"; 
    config->eaom.Name = "战争古树"; 
    config->eaow.Ubertip = "能生产出：角鹰兽和猛禽德鲁伊。还包括对角鹰兽和猛禽德鲁伊的各类升级。|n|n|cffffcc00能攻击地面单位。|r"。|r"攻击地面单位。|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->eaow.Tip = "建造(|cffffcc00W|r)风之古树"; 
    config->eaow.Hotkey = "W"; 
    config->eaow.Name = "风之古树"; 
    config->edos.Ubertip = "能训练出奇美拉怪兽。 还包括对腐蚀喷吐的研究。"�猛禽德鲁伊的各类升级。|n|n|cffffcc00能攻击地面单位。|r"。|r"攻击地面单位。|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->edos.Tip = "建造(|cffffcc00C|r)"; 
    config->edos.Hotkey = "C"; 
    config->edos.Name = "奇美拉栖木"; 
    config->emow.Ubertip = "能提供人口，从而增加可造单位数量的最大值。还能补充暗夜精灵单位的魔法值和生命值。在夜间，它也能自我恢复魔法能量。"�|r"|n|cffffcc00能攻击地面单位。|r"�单位的魔法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 精灵之火|n- 风暴之鸦|n- 飓风"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"�����; (null)
    config->emow.Tip = "建造(|cffffcc00M|r)月亮井"; 
    config->emow.Hotkey = "M"; 
    config->emow.Name = "月亮井"; 
}

