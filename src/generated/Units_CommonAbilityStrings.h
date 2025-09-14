#pragma once
#include "../common/common.h"
typedef struct Units_CommonAbilityStrings {
    union {
    } AHer;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Unhotkey;
        LPCSTR Hotkey;
        LPCSTR Unubertip;
        LPCSTR Untip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Ahar;
    union {
        LPCSTR EditorSuffix;
        LPCSTR Unhotkey;
        LPCSTR Hotkey;
        LPCSTR Unubertip;
        LPCSTR Untip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Ahrl;
    union {
        LPCSTR Name;
    } Arev;
    union {
        LPCSTR Name;
    } Adet;
    union {
        LPCSTR Hotkey;
        LPCSTR Tip;
        LPCSTR Name;
    } Adta;
    union {
        LPCSTR Bufftip;
    } Bdet;
    union {
        LPCSTR Bufftip;
    } Bvul;
    union {
        LPCSTR Bufftip;
    } Bspe;
    union {
        LPCSTR Bufftip;
    } Bfro;
    union {
        LPCSTR Unhotkey;
        LPCSTR Hotkey;
        LPCSTR Unubertip;
        LPCSTR Untip;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Arep;
    union {
        LPCSTR Bufftip;
    } BSTN;
    union {
        LPCSTR Bufftip;
    } BPSE;
} Units_CommonAbilityStrings;

void Init_Units_CommonAbilityStrings(Units_CommonAbilityStrings *config);
void Shutdown_Units_CommonAbilityStrings(Units_CommonAbilityStrings *config);

void Init_Units_CommonAbilityStrings(Units_CommonAbilityStrings *config) {
    config->Ahar.EditorSuffix = "(金矿和木材)"; 
    config->Ahar.Unhotkey = "E"; 
    config->Ahar.Hotkey = "G"; 
    config->Ahar.Unubertip = "将资源运送到最近的城镇大厅。"��<AOws,Data11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Ahar.Untip = "(|cffffcc00E|r)返送资源"; 
    config->Ahar.Ubertip = "可以采集金矿和木材资源。"��。"��<AOws,Data11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Ahar.Tip = "(|cffffcc00G|r)采集资源"; 
    config->Ahar.Name = "采集资源"; 
    config->Ahrl.EditorSuffix = "(采集木材)"; 
    config->Ahrl.Unhotkey = "E"; 
    config->Ahrl.Hotkey = "G"; 
    config->Ahrl.Unubertip = "将采集到的资源运送到大墓地或者坟场。"11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Ahrl.Untip = "(|cffffcc00E|r)返送资源"; 
    config->Ahrl.Ubertip = "从树木那里采集木材。"��墓地或者坟场。"11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Ahrl.Tip = "(|cffffcc00G|r)采集资源"; 
    config->Ahrl.Name = "采集资源"; 
    config->Arev.Name = "复活英雄"; 
    config->Adet.Name = "探测"; 
    config->Adta.Hotkey = "R"; 
    config->Adta.Tip = "(|cffffcc00R|r)显示"; 
    config->Adta.Name = "显示"; 
    config->Bdet.Bufftip = "被探测到"; 
    config->Bvul.Bufftip = "无敌"; 
    config->Bspe.Bufftip = "速度加快"; 
    config->Bfro.Bufftip = "被减速"; 
    config->Arep.Unhotkey = "R"; 
    config->Arep.Hotkey = "R"; 
    config->Arep.Unubertip = "|cffc3dbff点击右键以取消自动施放功能。|r"11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Arep.Untip = "|cffc3dbff点击右键以启动自动施放功能。|r"11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Arep.Ubertip = "修理建筑物和机械单位，需要消耗资源。"11>点的伤害并在<AOws,Dur1>秒内使其处于昏晕状态。"s,Dur1>秒的击晕效果。 |n|cffffcc00等级2|r－<AOws,Data21>点攻击力，<AOws,Dur2>秒的击晕效果。 |n|cffffcc00等级3|r－<AOws,Data31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""重击地面，对周围的地面单位造成<AOws,Data21>点的伤害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""重击地面，对周围的地面单位造成<AOws,Data31>点的伤害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����H; (null)
    config->Arep.Tip = "(|cffffcc00R|r)修理"; 
    config->Arep.Name = "修理"; 
    config->BSTN.Bufftip = "被击晕"; 
    config->BPSE.Bufftip = "被击晕"; 
}

