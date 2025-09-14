#pragma once
#include "../common/common.h"
typedef struct Units_UndeadUnitStrings {
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uaco;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uabo;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Hotkey;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Propernames;
        LPCSTR Name;
    } Udea;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uban;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ucry;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Hotkey;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Propernames;
        LPCSTR Name;
    } Udre;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ufro;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ugar;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ugrm;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ugho;
    union {
        LPCSTR ScoreScreenIcon;
        LPCSTR Hotkey;
        LPCSTR Revivetip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Propernames;
        LPCSTR Name;
    } Ulic;
    union {
        LPCSTR Casterupgradetip;
        LPCSTR Casterupgradename;
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } unec;
    union {
        LPCSTR Tip;
        LPCSTR Name;
    } uske;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uaod;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } unpl;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } unp1;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } unp2;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } usep;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } utod;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ugol;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uzig;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uzg1;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } umtw;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ubon;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } usap;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } uslh;
    union {
        LPCSTR Name;
    } ushd;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } ugrv;
} Units_UndeadUnitStrings;

void Init_Units_UndeadUnitStrings(Units_UndeadUnitStrings *config);
void Shutdown_Units_UndeadUnitStrings(Units_UndeadUnitStrings *config);

void Init_Units_UndeadUnitStrings(Units_UndeadUnitStrings *config) {
    config->uaco.Hotkey = "C"; 
    config->uaco.Ubertip = "不死族的基本工人单位。能召唤建筑物，采集金矿和进行修复工作。在牺牲深渊里牺牲以后侍僧还可以变为阴影。|n|n|cffffcc00能攻击地面单位。|r"地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uaco.Tip = "训练(|cffffcc00C|r)侍僧"; 
    config->uaco.Name = "侍僧"; 
    config->uabo.Hotkey = "A"; 
    config->uabo.Ubertip = "一种重型地面单位，能施放疾病云雾技能。|n|n|cffffcc00能攻击地面单位。|r"�深渊里牺牲以后侍僧还可以变为阴影。|n|n|cffffcc00能攻击地面单位。|r"地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uabo.Tip = "训练(|cffffcc00A|r)憎恶"; 
    config->uabo.Name = "憎恶"; 
    config->Udea.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-deathknight.blp"; 
    config->Udea.Hotkey = "D"; 
    config->Udea.Revivetip = "复活(|cffffcc00D|r)死亡骑士"; 
    config->Udea.Ubertip = "是人类圣骑士的邪恶对手。能学习到死亡缠绕，死亡契约，邪恶光环和操纵死尸这四种技能。|n|n|cffffcc00能攻击地面单位。|r"能攻击地面单位。|r"地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾"��阱"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 净化|n- 闪电护盾|n- 嗜血术"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Udea.Tip = "召唤(|cffffcc00D|r)死亡骑士"; 
    config->Udea.Propernames = "夜之哀伤,灵之领主,死亡风暴,玛达泽尔,黑神领主,光之领主,血浴男爵,血刃男爵,恐惧公爵,愤之公爵,霜之男爵,黑镰领主,哀冬公爵,皮尔喏得尔男爵,摩特男爵"; 
    config->Udea.Name = "死亡骑士"; 
    config->uban.Casterupgradetip = {"- 诅咒"圣骑士的邪恶对手。能学习到死亡缠绕，死亡契约，邪恶光环和操纵死尸这四种技能。|n|n|cffffcc00能攻击地面单位。|r"能攻击地面单位。|r"地面单位和空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->uban.Casterupgradename = "初级,专家级,大师级"; 
    config->uban.Hotkey = "B"; 
    config->uban.Ubertip = "一种魔法单位，一开始能施放诅咒技能，从而让敌人有一定的概率击空。随后还能学习到反魔法外壳和占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uban.Tip = "训练(|cffffcc00B|r)女妖"; 
    config->uban.Name = "女妖"; 
    config->ucry.Hotkey = "F"; 
    config->ucry.Ubertip = "一种远程攻击单位。能学习到蛛网技能。|n|n|cffffcc00能攻击地面单位。|r"�随后还能学习到反魔法外壳和占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ucry.Tip = "训练(|cffffcc00F|r)"; 
    config->ucry.Name = "穴居恶魔"; 
    config->Udre.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-dreadlord.blp"; 
    config->Udre.Hotkey = "E"; 
    config->Udre.Revivetip = "复活(|cffffcc00E|r)恐惧魔王"; 
    config->Udre.Ubertip = "一种狡猾的英雄，能学习到腐臭蜂群，睡眠，吸血光环和地狱火技能。|n|n|cffffcc00能攻击地面单位。|r"�占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Udre.Tip = "召唤(|cffffcc00E|r)恐惧魔王"; 
    config->Udre.Propernames = "特罗达尔,内罗司,布里克尔,内卡罗司,费罗司,德塞克斯,玛迪比恩,尼克塞塔斯,巴那泽尔,拉塞盖罗斯,阿玛克斯,滋费恩,罗塞拉斯,泽内塔,穆里奇,阿尔盖蒙,白尼或尔,维恩盖"; 
    config->Udre.Name = "恐惧魔王"; 
    config->ufro.Hotkey = "F"; 
    config->ufro.Ubertip = "一种重型的飞行单位，能学习到冰冻喷吐技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"面单位。|r"�占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ufro.Tip = "训练(|cffffcc00F|r)冰霜巨龙"; 
    config->ufro.Name = "冰霜巨龙"; 
    config->ugar.Hotkey = "A"; 
    config->ugar.Ubertip = "一种飞行单位。能学习到石像形态技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"位。|r"面单位。|r"�占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ugar.Tip = "训练(|cffffcc00A|r)石像鬼"; 
    config->ugar.Name = "石像鬼"; 
    config->ugrm.Hotkey = "A"; 
    config->ugrm.Ubertip = "一种飞行单位。 |n|n|cffffcc00能攻击地面单位和空中单位。|r"��面单位和空中单位。|r"位。|r"面单位。|r"�占据魔法。|n|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ugrm.Tip = "训练(|cffffcc00A|r)"; 
    config->ugrm.Name = "石像形态下的石像鬼"; 
    config->ugho.Hotkey = "G"; 
    config->ugho.Ubertip = "一种基本的地面单位，也能采集木材。能学习到吞食尸体和食尸鬼狂热技能。|n|n|cffffcc00能攻击地面单位和树木。|r"|n|cffffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ugho.Tip = "训练(|cffffcc00G|r)食尸鬼"; 
    config->ugho.Name = "食尸鬼"; 
    config->Ulic.ScoreScreenIcon = "UI\\Glues\\ScoreScreen\\scorescreen-hero-lich.blp"; 
    config->Ulic.Hotkey = "L"; 
    config->Ulic.Revivetip = "复活(|cffffcc00L|r)巫妖"; 
    config->Ulic.Ubertip = "一种神秘的英雄，能学习到霜冻护甲，霜冻新星，黑暗仪式和死亡凋零技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"ffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳"��"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 诅咒|n- 反魔法外壳|n- 占据"��守卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->Ulic.Tip = "召唤(|cffffcc00L|r)巫妖"; 
    config->Ulic.Propernames = "奥迪恩,拉斯,摩尔本特,拉奇,阿杰尔,卡里那伽,拉克尔,迪恩尔,卡里斯,维尼穆,纳泽,拉斯尔,霜冻舞者,克纳摩塞,阿拉恩迪尔"; 
    config->Ulic.Name = "巫妖"; 
    config->unec.Casterupgradetip = {"- 复活死尸"英雄，能学习到霜冻护甲，霜冻新星，黑暗仪式和死亡凋零技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"ffcc00能攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������}; (null)
    config->unec.Casterupgradename = "初级,专家级,大师级"; 
    config->unec.Hotkey = "N"; 
    config->unec.Ubertip = "一种魔法单位。一开始能施放复活死尸技能。随后还能学习到邪恶狂热和残废技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->unec.Tip = "训练(|cffffcc00N|r)亡灵巫师"; 
    config->unec.Name = "亡灵巫师"; 
    config->uske.Tip = "召唤骷髅战士"; 
    config->uske.Name = "骷髅战士"; 
    config->uaod.Hotkey = "A"; 
    config->uaod.Ubertip = "能召唤出新的英雄和复活阵亡的英雄。"��能。随后还能学习到邪恶狂热和残废技能。|n|n|cffffcc00能攻击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uaod.Tip = "召唤(|cffffcc00A|r)黑暗祭坛"; 
    config->uaod.Name = "黑暗祭坛"; 
    config->unpl.Hotkey = "N"; 
    config->unpl.Ubertip = "不死族的基本建筑物。能训练出侍僧和存贮采集到木材资源。在升级到了亡者大厅和黑色城堡之后能让玩家建造许多新的建筑物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->unpl.Tip = "召唤(|cffffcc00N|r)大墓地"; 
    config->unpl.Name = "大墓地"; 
    config->unp1.Hotkey = "U"; 
    config->unp1.Ubertip = "升级到了亡者大厅之后能让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�家建造许多新的建筑物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->unp1.Tip = "(|cffffcc00U|r)升级到亡者大厅"; 
    config->unp1.Name = "亡者大厅"; 
    config->unp2.Hotkey = "U"; 
    config->unp2.Ubertip = "升级到了亡者大厅之后能让玩家建造许多新的建筑物和单位。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�家建造许多新的建筑物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->unp2.Tip = "(|cffffcc00U|r)黑色城堡"; 
    config->unp2.Name = "黑色城堡"; 
    config->usep.Hotkey = "C"; 
    config->usep.Ubertip = "一种主要的产兵建筑物，能训练出食尸鬼，地穴恶魔和石像鬼。还包括对食尸鬼狂热，吞食尸体，石像形态和蛛网的研究。"��物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->usep.Tip = "召唤(|cffffcc00C|r)地穴"; 
    config->usep.Name = "地穴"; 
    config->utod.Hotkey = "T"; 
    config->utod.Ubertip = "能训练出亡灵巫师和女妖。还包括对亡灵巫师和女妖的升级，骨质增强术也是在这里研究的。"��，石像形态和蛛网的研究。"��物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->utod.Tip = "召唤(|cffffcc00T|r)诅咒神庙"; 
    config->utod.Name = "诅咒神庙"; 
    config->ugol.Hotkey = "G"; 
    config->ugol.Ubertip = "在金矿被闹鬼了之后侍僧才可以从中采集金矿资源。"��升级，骨质增强术也是在这里研究的。"��，石像形态和蛛网的研究。"��物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ugol.Tip = "(|cffffcc00G|r)闹鬼金矿"; 
    config->ugol.Name = "闹鬼金矿"; 
    config->uzig.Hotkey = "Z"; 
    config->uzig.Ubertip = "能提供人口，从而增加可造单位数量的最大值。在经过升级以后能变成一个可以攻击地面单位和空中单位的建筑物。"��究。"��物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uzig.Tip = "召唤(|cffffcc00Z|r)通灵塔"; 
    config->uzig.Name = "通灵塔"; 
    config->uzg1.Hotkey = "T"; 
    config->uzg1.Ubertip = "一种防御性建筑物。|n|n|cffffcc00能攻击地面单位和空中单位。|r"�能变成一个可以攻击地面单位和空中单位的建筑物。"��究。"��物和单位。"�位和空中单位。|r"空中单位。|r"�法值。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uzg1.Tip = "升级到(|cffffcc00T|r)"; 
    config->uzg1.Name = "幽魂之塔"; 
    config->umtw.Hotkey = "M"; 
    config->umtw.Ubertip = "能存放尸体，也是一种远程的攻城武器。对付建筑物特别地有效，但是自己本身也移动缓慢而容易遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->umtw.Tip = "训练(|cffffcc00M|r)绞肉车"; 
    config->umtw.Name = "绞肉车"; 
    config->ubon.Hotkey = "B"; 
    config->ubon.Ubertip = "能生产出霜冻巨龙。还包括对冰冻喷吐的研究。"��物特别地有效，但是自己本身也移动缓慢而容易遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ubon.Tip = "召唤(|cffffcc00B|r)埋骨地"; 
    config->ubon.Name = "埋骨地"; 
    config->usap.Hotkey = "S"; 
    config->usap.Ubertip = "能将侍僧转化成阴影。阴影是一种能看见敌方隐形单位的隐形单位。自己本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->usap.Tip = "召唤(|cffffcc00S|r)牺牲深渊"; 
    config->usap.Name = "牺牲深渊"; 
    config->uslh.Hotkey = "H"; 
    config->uslh.Ubertip = "能生产出憎恶和绞肉车。还包括对疾病云雾的研究。"��的隐形单位。自己本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->uslh.Tip = "召唤(|cffffcc00H|r)屠宰场"; 
    config->uslh.Name = "屠宰场"; 
    config->ushd.Name = "阴影"; 
    config->ugrv.Hotkey = "V"; 
    config->ugrv.Ubertip = "能对不死族单位的攻防进行升级。也能存放收集到的尸体和木材资源。"��本身也不能攻击敌人。"��遭受攻击。还能学习到疾病云雾技能。|n|n|cffffcc00能攻击地面单位和树木。|r"�。"��进攻地面和空中单位。|r"�的单位对疾病云雾免疫。"h,Data31>点的伤害。"�"��标<AEmb,Data31>点魔法值。"��度。 |n|cffffcc00等级3|r－造成<AHtc,Data31>点伤害，减慢<AHtc,Data33,%>%的移动速度，以及<AHtc,Data34,%>%的攻击速度。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热"��魔法恢复速度，生命值和攻击力。并使其具有占据技能。"��。"��攻击就会受到<AEah,Data21>点的伤害。"��值。">秒。"�小闪电的攻击力。""- 复活死尸|n- 邪恶狂热|n- 残废"��卫"�巨龙的攻击力。"��。""��法值。"��续<AEst,Dur3>秒"��近身攻击就会受到<AEah,Data31>点的伤害。"��值。">秒。"�小闪电的攻击力。"������; (null)
    config->ugrv.Tip = "召唤(|cffffcc00V|r)坟场"; 
    config->ugrv.Name = "坟场"; 
}

