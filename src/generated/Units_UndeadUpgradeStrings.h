#pragma once
#include "../common/common.h"
typedef struct Units_UndeadUpgradeStrings {
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rume;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rura;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Ruar;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rucr;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Ruac;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rugf;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Ruwb;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rusf;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rune;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Ruba;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rufb;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rusl;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rupc;
} Units_UndeadUpgradeStrings;

void Init_Units_UndeadUpgradeStrings(Units_UndeadUpgradeStrings *config);
void Shutdown_Units_UndeadUpgradeStrings(Units_UndeadUpgradeStrings *config);

void Init_Units_UndeadUpgradeStrings(Units_UndeadUpgradeStrings *config) {
    config->Rume.Hotkey = "S,S,S"; 
    config->Rume.Ubertip = {"增加食尸鬼，绞肉车，憎恶和骷髅战士的攻击力。"��尸体和木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加食尸鬼，绞肉车，憎恶和骷髅战士的攻击力。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加食尸鬼，绞肉车，憎恶和骷髅战士的攻击力。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加食尸鬼，绞肉车，憎恶和骷髅战士的攻击力。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Rume.Tip = "升级到"; 
    config->Rume.Name = "邪恶力量,中级邪恶力量,高级邪恶力量"; 
    config->Rura.Hotkey = "A,A,A"; 
    config->Rura.Ubertip = {"增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。"��尸体和木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Rura.Tip = "升级到(|cffffcc00A|r)生物攻击,升级到(|cffffcc00A|r)中级生物攻击,升级到"; 
    config->Rura.Name = "生物攻击,中级生物攻击,高级生物攻击"; 
    config->Ruar.Hotkey = "U,U,U"; 
    config->Ruar.Ubertip = {"增加食尸鬼，憎恶和骷髅战士的防御力。"��。"��。"��尸体和木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加食尸鬼，憎恶和骷髅战士的防御力。"��攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加食尸鬼，憎恶和骷髅战士的防御力。"��攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加食尸鬼，憎恶和骷髅战士的防御力。"��攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Ruar.Tip = "升级到(|cffffcc00U|r)邪恶装甲,升级到(|cffffcc00U|r)中级邪恶装甲,升级到"; 
    config->Ruar.Name = "邪恶装甲,中级邪恶装甲,高级邪恶装甲"; 
    config->Rucr.Hotkey = "C,C,C"; 
    config->Rucr.Ubertip = {"增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。"��尸体和木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Rucr.Tip = "升级到(|cffffcc00C|r)生物甲壳,升级到(|cffffcc00C|r)中级生物甲壳,升级到(|cffffcc00C|r)生物甲壳"; 
    config->Rucr.Name = "生物甲壳,中级生物甲壳,高级生物甲壳"; 
    config->Ruac.Hotkey = "C"; 
    config->Ruac.Ubertip = "使得食尸鬼能吞食附近的一个尸体来恢复自己的生命值。"��木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Ruac.Tip = "研究(|cffffcc00C|r)吞食尸体"; 
    config->Ruac.Name = "吞食尸体"; 
    config->Rugf.Hotkey = "Z"; 
    config->Rugf.Ubertip = "增加食尸鬼<Rugf,base1,%>%的攻击速度并加快其移动速度。"��"��木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Rugf.Tip = "研究(|cffffcc00Z|r)食尸鬼狂热"; 
    config->Rugf.Name = "食尸鬼狂热"; 
    config->Ruwb.Hotkey = "W"; 
    config->Ruwb.Ubertip = "使得地穴恶魔能将敌人的某个空中单位捕获到地面，从而让己方的地面单位可以对其进行攻击。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Ruwb.Tip = "研究(|cffffcc00W|r)蛛网"; 
    config->Ruwb.Name = "蛛网"; 
    config->Rusf.Hotkey = "S"; 
    config->Rusf.Ubertip = "石像形态技能能使石像鬼变成一座具有极高护甲和生命值恢复速度的雕像。在这期间石像鬼也不能攻击任何敌人。"��习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur2>秒。"��身攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Rusf.Tip = "(|cffffcc00S|r)石像形态"; 
    config->Rusf.Name = "石像形态"; 
    config->Rune.Hotkey = "E,E"; 
    config->Rune.Ubertip = {"增加亡灵巫师的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有邪恶狂热技能。"��不能攻击任何敌人。"��习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加亡灵巫师的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有残废技能。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加亡灵巫师的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有残废技能。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Rune.Tip = "(|cffffcc00E|r)亡灵巫师专家训练,(|cffffcc00E|r)亡灵巫师大师训练"; 
    config->Rune.Name = "亡灵巫师专家训练,亡灵巫师大师训练"; 
    config->Ruba.Hotkey = "A,A"; 
    config->Ruba.Ubertip = {"增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有反魔法外壳技能。"��"��不能攻击任何敌人。"��习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->Ruba.Tip = "(|cffffcc00A|r)女妖专家训练,(|cffffcc00A|r)女妖大师训练"; 
    config->Ruba.Name = "女妖专家训练,女妖大师训练"; 
    config->Rufb.Hotkey = "B"; 
    config->Rufb.Ubertip = "使得霜冻巨龙的攻击能在一段时间内使受攻击的建筑物暂时失去功效。"��魔法外壳技能。"��"��不能攻击任何敌人。"��习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Rufb.Tip = "研究(|cffffcc00B|r)冰冻喷吐"; 
    config->Rufb.Name = "冰冻喷吐"; 
    config->Rusl.Hotkey = "S"; 
    config->Rusl.Ubertip = "能增加召唤出来的骷髅战士的持续时间(增加<Rusl,base1>秒)。 "��去功效。"��魔法外壳技能。"��"��不能攻击任何敌人。"��习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"r,Data31,%>%的远程攻击力。",Cost3>点魔法召唤一头猫头鹰。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Rusl.Tip = "研究(|cffffcc00S|r)骨质增强术"; 
    config->Rusl.Name = "骨质增强术"; 
    config->Rupc.Hotkey = "D"; 
    config->Rupc.Ubertip = "使得憎恶能带有疾病云雾光环，从而在<Aap1,Data11>秒内对周围的单位造成每秒<Aap1,Data12>点的伤害。而绞肉车的攻击也可以带有疾病效果从而在<Aap2,Data11>秒内对周围的单位造成每秒<Aap2,Data12>点的伤害。不死族的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""增加女妖的魔法最大值，魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""进一步增加地穴恶魔，石像鬼，冰霜巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Rupc.Tip = "研究(|cffffcc00D|r)疾病云雾"; 
    config->Rupc.Name = "疾病云雾"; 
}

