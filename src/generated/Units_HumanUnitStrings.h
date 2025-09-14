#pragma once
#include "../common/common.h"
typedef struct Units_HumanUnitStrings {
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } halt;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Hamg;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } harm;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hars;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hbar;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hbla;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hcas;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hhou;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hfoo;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hgra;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hctw;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hgtw;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hwtw;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hgyr;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hkee;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hkni;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hlum;
    union {
        LPCSTR Name;
    } hmil;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR UberTip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Hmkg;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hmpr;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hmtm;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Propernames;
        LPCSTR Name;
    } Hpal;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hpea;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hrif;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hsor;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hmtt;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } htow;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } hwat;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } hwt2;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Name;
    } hwt3;
    union {
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Hotkey;
        LPCSTR Name;
    } hgry;
} Units_HumanUnitStrings;

void Init_Units_HumanUnitStrings(Units_HumanUnitStrings *config);
void Shutdown_Units_HumanUnitStrings(Units_HumanUnitStrings *config);

void Init_Units_HumanUnitStrings(Units_HumanUnitStrings *config) {
    config->halt.Ubertip = "召唤新的英雄或者复活死去的英雄。"��。 |n|cffffcc00能进攻地面单位。|r"��恒之树之后，能使玩家建造许多新的建筑物和单位。"��位和树木。|r"行升级来获得更佳的效果。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->halt.Tip = "(|cffffcc00A|r)建造国王祭坛"; 
    config->halt.Hotkey = "A"; 
    config->halt.Name = "国王祭坛"; 
    config->Hamg.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-archmage.blp"; 
    config->Hamg.Revivetip = "(|cffffcc00A|r)复活大魔法师"; 
    config->Hamg.Ubertip = "一位神秘的英雄，特别擅长于远程攻击。他能学到暴风雪，召唤水元素，辉煌光环和群体传送魔法。|n|n|cffffcc00能攻击地面和空中单位。|r"�|r"行升级来获得更佳的效果。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Hamg.Tip = "(|cffffcc00A|r)召唤大魔法师"; 
    config->Hamg.Hotkey = "A"; 
    config->Hamg.Propernames = "特恩,尼拉斯,安德若麦斯,沙尔,爱仑,麦那斯,兰德泽尔,多里尔,佩里尔,雷克斯,费德里德爱仑,达拉,凯恩"; 
    config->Hamg.Name = "大魔法师"; 
    config->harm.Ubertip = "能生产出蒸汽坦克，迫击炮小队，和矮人直升机。 还包括照明弹和矮人直升机炸弹的升级。"�魔法。|n|n|cffffcc00能攻击地面和空中单位。|r"�|r"行升级来获得更佳的效果。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->harm.Tip = "(|cffffcc00W|r)建造车间"; 
    config->harm.Hotkey = "W"; 
    config->harm.Name = "车间"; 
    config->hars.Ubertip = "在这里可以训练出牧师和女巫。还可以对他们的技能进行研究，使得人类的防御塔具有探测隐形单位能力的魔法岗哨也是在这里进行研究的。"��升级来获得更佳的效果。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hars.Tip = "(|cffffcc00R|r)神秘圣地"; 
    config->hars.Hotkey = "R"; 
    config->hars.Name = "神秘圣地"; 
    config->hbar.Ubertip = "最基本的产兵建筑物。能训练出人类的步兵，矮人火枪手和骑士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hbar.Tip = "(|cffffcc00B|r)建造兵营"; 
    config->hbar.Hotkey = "B"; 
    config->hbar.Name = "兵营"; 
    config->hbla.Ubertip = "能对护甲，武器和火药进行升级。"��的步兵，矮人火枪手和骑士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hbla.Tip = "(|cffffcc00S|r)铁匠铺"; 
    config->hbla.Hotkey = "S"; 
    config->hbla.Name = "铁匠铺"; 
    config->hcas.Ubertip = "升级到城堡之后能使玩家建造许多新的建筑物和单位。"��和骑士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hcas.Tip = "(|cffffcc00U|r)升级到城堡"; 
    config->hcas.Hotkey = "U"; 
    config->hcas.Name = "城堡"; 
    config->hhou.Ubertip = "提供人口，增加可造单位数量的最大值。"��物和单位。"��和骑士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hhou.Tip = "(|cffffcc00F|r)建造农场"; 
    config->hhou.Hotkey = "F"; 
    config->hhou.Name = "农场"; 
    config->hfoo.Ubertip = "步兵能学习到防御模式技能。|n|n|cffffcc00能攻击地面单位。|r"�士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hfoo.Tip = "(|cffffcc00F|r)生产步兵"; 
    config->hfoo.Hotkey = "F"; 
    config->hfoo.Name = "步兵"; 
    config->hgra.Ubertip = "能训练出狮鹫骑士。还包括对风暴战锤的研究。"�面单位。|r"�士。步兵的防御模式，矮人火枪手的长管火枪以及骑士和狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hgra.Tip = "(|cffffcc00G|r)建造狮鹫笼"; 
    config->hgra.Hotkey = "G"; 
    config->hgra.Name = "狮鹫笼"; 
    config->hctw.Ubertip = "重型的防御性建筑物，对付成群结队的敌人尤为有效。还能学到魔法岗哨技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hctw.Tip = "(|cffffcc00C|r)升级到炮塔"; 
    config->hctw.Hotkey = "C"; 
    config->hctw.Name = "炮塔"; 
    config->hgtw.Ubertip = "基本的防守型建筑物，能学习到魔法岗哨技能。|n|n|cffffcc00能攻击地面和空中单位。|r"ffcc00能攻击地面单位和树木。|r"�狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hgtw.Tip = "(|cffffcc00G|r)升级到防御塔"; 
    config->hgtw.Hotkey = "G"; 
    config->hgtw.Name = "防御塔"; 
    config->hwtw.Ubertip = "基本的侦察型建筑物，能升级到炮塔或者防御塔，还能学习到魔法岗哨技能。"�位。|r"ffcc00能攻击地面单位和树木。|r"�狮鹫骑士的训兽术也都是在这里进行研究的。"��。|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hwtw.Tip = "(|cffffcc00T|r)"; 
    config->hwtw.Hotkey = "T"; 
    config->hwtw.Name = "哨塔"; 
    config->hgyr.Ubertip = "一种快速移动的飞行机器，能出色地完成侦察任务也能有效地抵抗敌人的空中单位，能获得矮人直升机炸弹的升级。|n能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hgyr.Tip = "(|cffffcc00G|r)生产矮人直升机"; 
    config->hgyr.Hotkey = "G"; 
    config->hgyr.Name = "矮人直升机"; 
    config->hkee.Ubertip = "升级到主城之后能使玩家建造许多新的建筑物和单位。"��效地抵抗敌人的空中单位，能获得矮人直升机炸弹的升级。|n能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hkee.Tip = "(|cffffcc00U|r)升级到主城"; 
    config->hkee.Hotkey = "U"; 
    config->hkee.Name = "主城"; 
    config->hkni.Ubertip = "强大的地面单位，能学到训兽术。 |n|n|cffffcc00能攻击地面单位。|r"敌人的空中单位，能获得矮人直升机炸弹的升级。|n能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hkni.Tip = "(|cffffcc00K|r)"; 
    config->hkni.Hotkey = "K"; 
    config->hkni.Name = "骑士"; 
    config->hlum.Ubertip = "能储存采集到的木材。还包括对伐木效率和石工技术的研究。"��|r"敌人的空中单位，能获得矮人直升机炸弹的升级。|n能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hlum.Tip = "(|cffffcc00L|r)建造伐木场"; 
    config->hlum.Hotkey = "L"; 
    config->hlum.Name = "伐木场"; 
    config->hmil.Name = "民兵"; 
    config->Hmkg.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-mountainking.blp"; 
    config->Hmkg.Revivetip = "(|cffffcc00M|r)复活山丘之王"; 
    config->Hmkg.UberTip = "战士型英雄，特别擅长于冲锋陷阵。能学习到风暴之锤，雷霆一击，重击和天神下凡。|n|n|cffffcc00能攻击地面单位。|r"能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Hmkg.Tip = "(|cffffcc00M|r)召唤山丘之王"; 
    config->Hmkg.Hotkey = "M"; 
    config->Hmkg.Propernames = "波尔,穆林,萨哥尔斯,凯尔维,哥里穆,布里,黑哥因,萨尔丁,斑迪斯,伽尔,比泽尔,穆迪,爱哥诺尔"; 
    config->Hmkg.Name = "山丘之王"; 
    config->hmpr.Casterupgradetip = {"- 医疗"英雄，特别擅长于冲锋陷阵。能学习到风暴之锤，雷霆一击，重击和天神下凡。|n|n|cffffcc00能攻击地面单位。|r"能看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->hmpr.Casterupgradename = "初级,专家级,大师级"; 
    config->hmpr.Ubertip = "一开始就拥有强大的医疗能力，随后还能学习到驱逐魔法和心灵之火这两项技能。|n|n|cffffcc00能攻击地面和空中单位。|r"看见隐形单位。|n|n|cffffcc00能攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hmpr.Tip = "(|cffffcc00P|r)训练牧师"; 
    config->hmpr.Hotkey = "P"; 
    config->hmpr.Name = "牧师"; 
    config->hmtm.Ubertip = "远距离攻城单位，对付建筑物特别地有效，但是速度很慢很容易遭受敌人的近身攻击。还能获得照明弹技能。 |n|n|cffffcc00能攻击地面单位和树木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hmtm.Tip = "(|cffffcc00M|r)"; 
    config->hmtm.Hotkey = "M"; 
    config->hmtm.Name = "迫击炮小队"; 
    config->Hpal.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-paladin.blp"; 
    config->Hpal.Revivetip = "(|cffffcc00L|r)复活圣骑士"; 
    config->Hpal.Ubertip = "战士型英雄，特别擅长于保护自己周围的部队，能学习到神圣之光，神圣之盾，专注光环和复活这四项技能。|n|n|cffffcc00能攻击地面单位。|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Hpal.Tip = "(|cffffcc00L|r)圣骑士"; 
    config->Hpal.Hotkey = "L"; 
    config->Hpal.Propernames = "哥兰尼斯,卓尔,塞杰尔,马拉克,哥维拉德,摩鲁尼,阿杰曼德,伯拉多尔,曼那达尔,赞恩,阿留尔斯,阿尤里尔斯,卡那维尔德,布赞恩"; 
    config->Hpal.Name = "圣骑士"; 
    config->hpea.Ubertip = "人类的基本工作单位，能采集金矿和木材还能建造和修理建筑物。紧急情况之下还可以变成民兵。 |n|n|cffffcc00能攻击地面单位和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hpea.Tip = "(|cffffcc00P|r)生产农民"; 
    config->hpea.Hotkey = "P"; 
    config->hpea.Name = "农民"; 
    config->hrif.Ubertip = "非常适合于对付敌人的空中单位，还能获得长管火枪的升级。|n|n|cffffcc00能攻击地面和空中单位。|r"n|n|cffffcc00能攻击地面单位和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 医疗|n- 驱逐魔法|n- 心灵之火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hrif.Tip = "(|cffffcc00R|r)生产矮人火枪手"; 
    config->hrif.Hotkey = "R"; 
    config->hrif.Name = "矮人火枪手"; 
    config->hsor.Casterupgradetip = {"- 减速"合于对付敌人的空中单位，还能获得长管火枪的升级。|n|n|cffffcc00能攻击地面和空中单位。|r"n|n|cffffcc00能攻击地面单位和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->hsor.Casterupgradename = "初级,专家级,大师级"; 
    config->hsor.Ubertip = "一开始能施放减慢敌人移动和进攻速度的减速魔法，随后还能学习到隐形术和变形术。|n|n|cffffcc00能攻击地面和空中单位。|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hsor.Tip = "(|cffffcc00S|r)"; 
    config->hsor.Hotkey = "S"; 
    config->hsor.Name = "女巫"; 
    config->hmtt.Ubertip = "重型装甲车辆，特别擅长于对付敌人的建筑物。 |n|n|cffffcc00能攻击建筑物。|r"变形术。|n|n|cffffcc00能攻击地面和空中单位。|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hmtt.Tip = "(|cffffcc00T|r)训练蒸汽坦克"; 
    config->hmtt.Hotkey = "T"; 
    config->hmtt.Name = "蒸汽坦克"; 
    config->htow.Ubertip = "基本建筑物，用来训练农民和存贮资源，在升级到了主城和城堡之后能让玩家建造许多新的建筑物和单位。"�和空中单位。|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->htow.Tip = "(|cffffcc00H|r)建造城镇大厅"; 
    config->htow.Hotkey = "H"; 
    config->htow.Name = "城镇大厅"; 
    config->hwat.EditorSuffix = "(等级"; 
    config->hwat.Name = "水元素"; 
    config->hwt2.EditorSuffix = "(等级"; 
    config->hwt2.Name = "水元素"; 
    config->hwt3.EditorSuffix = "(等级"; 
    config->hwt3.Name = "水元素"; 
    config->hgry.Ubertip = "一种威力巨大的飞行单位，狮鹫上面骑乘着一个矮人族的锤手。能学到风暴战锤技能。|n|n|cffffcc00能攻击地面和空中单位。|r"�和树木。|r"��|r"�木。|r"�攻击空中单位。|r"��|n|cffffcc00能进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术"��"��值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 减速|n- 隐形术|n- 变形术"��火"�冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->hgry.Tip = "(|cffffcc00G|r)训练狮鹫骑士"; 
    config->hgry.Hotkey = "G"; 
    config->hgry.Name = "狮鹫骑士"; 
}

