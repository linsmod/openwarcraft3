#pragma once
#include "../common/common.h"
typedef struct Units_OrcUpgradeStrings {
    union {
        LPCSTR Name;
    } Roch;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rome;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rora;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Roar;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rwdm;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Ropg;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Robs;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rows;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Roen;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip;
        LPCSTR Tip;
        LPCSTR Name;
    } Rovs;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rowd;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rost;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rosp;
    union {
        LPCSTR Hotkey;
        LPCSTR Ubertip[];
        LPCSTR Tip;
        LPCSTR Name;
    } Rotr;
} Units_OrcUpgradeStrings;

void Init_Units_OrcUpgradeStrings(Units_OrcUpgradeStrings *config);
void Shutdown_Units_OrcUpgradeStrings(Units_OrcUpgradeStrings *config);

void Init_Units_OrcUpgradeStrings(Units_OrcUpgradeStrings *config) {
    config->Roch.Name = "混乱"; 
    config->Rome.Hotkey = "M,M,M"; 
    config->Rome.Ubertip = {"增加兽人步兵，掠夺者和牛头人的攻击力。"��来，不过他们随后也会对进入射程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者和牛头人的攻击力。"害并在<AOws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者和牛头人的攻击力。"害并在<AOws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rome.Tip = "升级到(|cffffcc00M|r)钢铁近战武器,升级到(|cffffcc00M|r)金属近战武器,升级到(|cffffcc00M|r)终极近战武器"; 
    config->Rome.Name = "钢铁近战武器,金属近战武器,终极近战武器"; 
    config->Rora.Hotkey = "R,R,R"; 
    config->Rora.Ubertip = {"增加巨魔猎头者，双足飞龙和投石车的远程攻击力。"��他们随后也会对进入射程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加巨魔猎头者，双足飞龙和投石车的远程攻击力。"ws,Dur2>秒内使其处于昏晕状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加巨魔猎头者，双足飞龙和投石车的远程攻击力。"ws,Dur3>秒内使其处于昏晕状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rora.Tip = "升级到(|cffffcc00R|r)钢铁远程武器,升级到(|cffffcc00R|r)金属远程武器,升级到(|cffffcc00R|r)终极远程武器"; 
    config->Rora.Name = "钢铁远程武器,金属远程武器,终极远程武器"; 
    config->Roar.Hotkey = "A,A,A"; 
    config->Roar.Ubertip = {"增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Roar.Tip = "升级到(|cffffcc00A|r)钢铁护甲,升级到(|cffffcc00A|r)钢铁护甲,升级到(|cffffcc00A|r)终极护甲"; 
    config->Roar.Name = "钢铁护甲,金属护甲,终极护甲"; 
    config->Rwdm.Hotkey = "D"; 
    config->Rwdm.Ubertip = "经过升级以后，科多兽的战鼓光环能给予周围的单位更多的攻击力附加值。"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Rwdm.Tip = "升级(|cffffcc00D|r)战鼓"; 
    config->Rwdm.Name = "战鼓攻击力升级"; 
    config->Ropg.Hotkey = "G"; 
    config->Ropg.Ubertip = "使得苦工，兽人步兵和掠夺者对敌人建筑物的攻击能掠夺到一定的资源。"��"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Ropg.Tip = "(|cffffcc00G|r)掠夺"; 
    config->Ropg.Name = "掠夺"; 
    config->Robs.Hotkey = "B"; 
    config->Robs.Ubertip = "能增加兽人步兵<Robs,base1>点的生命值和<Robs,base2>点的攻击力。"��的资源。"��"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Robs.Tip = "研究(|cffffcc00B|r)狂暴力量"; 
    config->Robs.Name = "狂暴力量"; 
    config->Rows.Hotkey = "P"; 
    config->Rows.Ubertip = "粉碎技能使得牛头人有一定的概率能释放出范围攻击。"��力。"��的资源。"��"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Rows.Tip = "研究(|cffffcc00P|r)粉碎"; 
    config->Rows.Name = "粉碎"; 
    config->Roen.Hotkey = "N"; 
    config->Roen.Ubertip = "使得掠夺者能将目标单位(包括空中单位)困在地面上而不能移动。"资源。"��"��程的敌人进行攻击或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Roen.Tip = "研究(|cffffcc00N|r)诱捕"; 
    config->Roen.Name = "诱捕"; 
    config->Rovs.Hotkey = "E"; 
    config->Rovs.Ubertip = "使得双足飞龙的攻击带有毒性。一个被浸毒武器攻击到的单位会在一段时间内持续受到伤害。"��或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"�击地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����; (null)
    config->Rovs.Tip = "研究(|cffffcc00E|r)浸毒武器"; 
    config->Rovs.Name = "浸毒武器"; 
    config->Rowd.Hotkey = "D,D"; 
    config->Rowd.Ubertip = {"增加巫医的魔法最大值，魔法恢复速度，生命值和攻击力。还能使其获得静止陷阱技能。"��。"��或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""增加巫医的魔法最大值，魔法恢复速度，生命值和攻击力。还能使其获得静止治疗守卫技能。"�地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rowd.Tip = "(|cffffcc00D|r)巫医专家训练,(|cffffcc00D|r)巫医大师训练"; 
    config->Rowd.Name = "巫医专家训练,巫医大师训练"; 
    config->Rost.Hotkey = "M,M"; 
    config->Rost.Ubertip = {"增加萨满祭司的魔法最大值，魔法恢复速度，生命值和攻击力。还能使其获得闪电护盾技能。"��或者追捕。"��集资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""增加萨满祭司的魔法最大值，魔法恢复速度，生命值和攻击力。还能使其获得嗜血术。"��能。"�地面单位。|r"的其他单位。每次跳跃都会减小闪电的攻击力。""进一步增加兽人步兵，掠夺者，牛头人，巨魔猎头者，双足飞龙和投石车的护甲。"��状态。"cc00能攻击地面单位。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rost.Tip = "(|cffffcc00M|r)"; 
    config->Rost.Name = "萨满祭司专家训练,萨满祭司大师训练"; 
    config->Rosp.Hotkey = "S,S,S"; 
    config->Rosp.Ubertip = {"使得兽族的建筑物周围都布满了具有攻击力的尖刺，如果敌人每进行一次近身攻击就会受到<Rosp,base1>点伤害。"��资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""使得兽族的建筑物周围都布满了具有攻击力的尖刺，如果敌人每进行一次近身攻击就会受到<Rosp,mod1>点伤害。"��其他单位。每次跳跃都会减小闪电的攻击力。""使得兽族的建筑物周围都布满了具有攻击力的尖刺，如果敌人每进行一次近身攻击就会受到<Rosp,mod1>点伤害。"��。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rosp.Tip = "升级到(|cffffcc00S|r)尖刺障碍,升级到(|cffffcc00S|r)中级尖刺障碍,升级到(|cffffcc00S|r)高级尖刺障碍"; 
    config->Rosp.Name = "尖刺障碍,中级尖刺障碍,高级尖刺障碍"; 
    config->Rotr.Hotkey = "T"; 
    config->Rotr.Ubertip = {"加快巫医和巨魔猎头者的生命值恢复。"��力的尖刺，如果敌人每进行一次近身攻击就会受到<Rosp,base1>点伤害。"��资源。你也可以将集结点设在某个单位身上，这样新单位出来以后就会自动跟着那个单位。"a31>点攻击力，<AOws,Dur3>秒的击晕效果。"�动速度以及<AOae,Data32,%>%的攻击速度。"和隐形技能。"��速度，持续<AOwk,Dur3>秒。""进一步加快巫医和巨魔猎头者的生命值恢复。"��刺，如果敌人每进行一次近身攻击就会受到<Rosp,mod1>点伤害。"��其他单位。每次跳跃都会减小闪电的攻击力。""进一步加快巫医和巨魔猎头者的生命值恢复。"��刺，如果敌人每进行一次近身攻击就会受到<Rosp,mod1>点伤害。"��。|r"单位。每次跳跃都会减小闪电的攻击力。"�����}; (null)
    config->Rotr.Tip = "研究(|cffffcc00T|r)巨魔再生,研究(|cffffcc00T|r)中级巨魔再生,研究(|cffffcc00T|r)高级巨魔再生"; 
    config->Rotr.Name = "巨魔再生,中级巨魔再生,高级巨魔再生"; 
}

