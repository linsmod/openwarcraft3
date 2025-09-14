#pragma once
#include "../common/common.h"
typedef struct Units_HumanUpgradeStrings {
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhme;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhra;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhar;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhla;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhac;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhgb;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhlh;
    union {
        LPCSTR Hotkey;
        LPCSTR UberTip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhde;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhan;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhpt;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rhst;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhri;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhse;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhfl;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rhhb;
} Units_HumanUpgradeStrings;

void Init_Units_HumanUpgradeStrings(Units_HumanUpgradeStrings *config);
void Shutdown_Units_HumanUpgradeStrings(Units_HumanUpgradeStrings *config);

void Init_Units_HumanUpgradeStrings(Units_HumanUpgradeStrings *config) {
    config->Rhme.Hotkey = "S,S,S"; 
    config->Rhme.Ubertip = {"增加民兵，步兵，狮鹫骑士和骑士的攻击力。"��的敌方单位也造成一定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加民兵，步兵，狮鹫骑士和骑士的攻击力。"��复速度，生命值和攻击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"" 进一步增加民兵，步兵，狮鹫骑士和骑士的攻击力。"�的猛禽德鲁伊，树妖，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhme.Tip = "(|cffffcc00S|r)升级到铁剑术,(|cffffcc00S|r)升级到钢剑术,(|cffffcc00S|r)升级到毒剑术"; 
    config->Rhme.Name = "铁剑术,钢剑术,毒剑术"; 
    config->Rhra.Hotkey = "G,G,G"; 
    config->Rhra.Ubertip = {"增加矮人火枪手，迫击炮小队，蒸汽坦克和矮人直升机的攻击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"" 进一步增加矮人火枪手，迫击炮小队，蒸汽坦克和矮人直升机的攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"" 进一步增加矮人火枪手，迫击炮小队，蒸汽坦克和矮人直升机的攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhra.Tip = "(|cffffcc00G|r)升级到黑火药,(|cffffcc00G|r)升级到精练火药,(|cffffcc00G|r)升级到终极火药"; 
    config->Rhra.Name = "黑火药,精练火药,终极火药"; 
    config->Rhar.Hotkey = "P,P,P"; 
    config->Rhar.Ubertip = {"增加民兵，步兵，骑士，矮人直升机和蒸汽坦克的护甲。"��击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""进一步增加民兵，步兵，骑士，矮人直升机和蒸汽坦克的护甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。""进一步增加民兵，步兵，骑士，矮人直升机和蒸汽坦克的护甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhar.Tip = "(|cffffcc00P|r)铁甲,(|cffffcc00P|r)"; 
    config->Rhar.Name = "铁甲,钢甲,重金甲"; 
    config->Rhla.Hotkey = "A,A,A"; 
    config->Rhla.Ubertip = {"增加矮人火枪手，迫击炮小队和狮鹫骑士的护甲。"��甲。"��击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"" 进一步增加矮人火枪手，迫击炮小队和狮鹫骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"" 进一步增加矮人火枪手，迫击炮小队和狮鹫骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhla.Tip = "(|cffffcc00A|r)升级到镶皮甲,(|cffffcc00A|r)升级到加强型镶皮甲,(|cffffcc00A|r)升级到龙甲"; 
    config->Rhla.Name = "镶皮甲,加强型镶皮甲,龙甲"; 
    config->Rhac.Hotkey = "M,M,M"; 
    config->Rhac.Ubertip = {"增加人类建筑物的生命值和护甲。"��骑士的护甲。"��甲。"��击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"" 进一步增加人类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"" 进一步增加人类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhac.Tip = "(|cffffcc00M|r)升级到加强型石工技术,(|cffffcc00M|r)升级到高级石工技术,(|cffffcc00M|r)升级到终极石工技术"; 
    config->Rhac.Name = "加强型石工技术,高级石工技术,终极石工技术"; 
    config->Rhgb.Hotkey = "B"; 
    config->Rhgb.Ubertip = "使得矮人直升机能攻击地面单位."�"��骑士的护甲。"��甲。"��击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"" 进一步增加人类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"" 进一步增加人类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhgb.Tip = "(|cffffcc00B|r)矮人直升机炸弹"; 
    config->Rhgb.Name = "矮人直升机炸弹"; 
    config->Rhlh.Hotkey = "L,L"; 
    config->Rhlh.Ubertip = {"增加<Rhlh,mod1>点农民每次能携带的木材数量。"甲。"��甲。"��击力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"进一步增加<Rhlh类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhlh.Tip = "(|cffffcc00L|r)改进型伐木效率,(|cffffcc00L|r)高级伐木效率"; 
    config->Rhlh.Name = "改进型伐木效率,高级伐木效率"; 
    config->Rhde.Hotkey = "D"; 
    config->Rhde.UberTip = "使得步兵能拥有防御技能，有效地抵抗敌人的穿刺型攻击。"��力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"进一步增加<Rhlh类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhde.Tip = "(|cffffcc00D|r)防御"; 
    config->Rhde.Name = "防御"; 
    config->Rhan.Hotkey = "A"; 
    config->Rhan.Ubertip = "增加<Rhan,base1>点骑士和狮鹫骑士的生命值。"��穿刺型攻击。"��力。"��定伤害。"��面单位也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。"进一步增加<Rhlh类建筑物的生命值和护甲。"��骑士的护甲。"�甲。"攻击力。"�击力。并使其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhan.Tip = "(|cffffcc00A|r)研究训兽术"; 
    config->Rhan.Name = "训兽术"; 
    config->Rhpt.Hotkey = "T,T"; 
    config->Rhpt.Ubertip = {"增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有驱逐魔法技能。"��也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有心灵之火技能。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhpt.Tip = "(|cffffcc00T|r)"; 
    config->Rhpt.Name = "牧师专家训练,牧师大师训练"; 
    config->Rhst.Hotkey = "O,O"; 
    config->Rhst.Ubertip = {"增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有隐形技能。"��。"��也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有变形技能。"��。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������}; (null)
    config->Rhst.Tip = "(|cffffcc00O|r)"; 
    config->Rhst.Name = "女巫专家训练,女巫大师训练"; 
    config->Rhri.Hotkey = "L"; 
    config->Rhri.Ubertip = "增加矮人火枪手的攻击范围。"��度，生命值，攻击力，还能使其拥有隐形技能。"��。"��也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有变形技能。"��。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhri.Tip = "(|cffffcc00L|r)升级到长管火枪"; 
    config->Rhri.Name = "长管火枪"; 
    config->Rhse.Hotkey = "M"; 
    config->Rhse.Ubertip = "让人类的各类塔能探测到隐形单位。"��命值，攻击力，还能使其拥有隐形技能。"��。"��也能攻击空中单位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有变形技能。"��。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhse.Tip = "(|cffffcc00M|r)研究魔法岗哨"; 
    config->Rhse.Name = "魔法岗哨"; 
    config->Rhfl.Hotkey = "F"; 
    config->Rhfl.Ubertip = "让每个迫击炮小队拥有一个照明弹，照明弹能驱散地图中任何区域的战争迷雾。|n能探测到隐形单位。"位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有变形技能。"��。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhfl.Tip = "(|cffffcc00F|r)研究照明弹"; 
    config->Rhfl.Name = "照明弹"; 
    config->Rhhb.Hotkey = "H"; 
    config->Rhhb.Ubertip = "让狮鹫骑士的战锤能进行弹射,从而对两个敌人造成伤害。"�何区域的战争迷雾。|n能探测到隐形单位。"位。"��"�|r"击地面单位和空中单位。|r"攻击地面单位和空中单位。|r"00等级3|r－转化<AUdr,Data31,%>%的生命值。"�增加<AUfa,Data32>点护甲。"��星伤害。"��的生命值。"�造成<AUcs,Data31>点的伤害，|n总共达到<AUcs,Data32>点的伤害。"�。""增加牧师的魔法值，魔法恢复速度，生命值，攻击力，还能使其拥有变形技能。"��。"��其拥有变熊技能。"��。"��速。|n持续<AUfa,Data21>秒。"�小闪电的攻击力。"mod1>点农民每次能携带的木材数量。"��甲。"��骑士的护甲。"�甲。"攻击力。"�，角鹰兽和奇美拉怪兽的防御力。"��速。|n持续<AUfa,Data31>秒。"�小闪电的攻击力。"������; (null)
    config->Rhhb.Tip = "(|cffffcc00H|r)研究风暴战锤"; 
    config->Rhhb.Name = "风暴战锤"; 
}

