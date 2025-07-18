#include "../g_local.h"

#define INFO_PANEL_UNIT_DETAIL 0.180, 0.120
#define BUILDQUEUE_OFFSET 0.0281
#define MULTISELECT_COLUMNS 6
#define MULTISELECT_OFFSET 0.031, 0.050
#define COMMAND_BUTTON_POSITION(x,y) 0.2365 + (x) * 0.0434, 0.1131 - (y) * 0.0434
#define COMMAND_BUTTON_SIZE 0.039
#define INVENTORY_BUTTON_POSITION(x,y) 0.1315 + (x) * 0.0394, 0.0971 - (y) * 0.0384
#define INVENTORY_BUTTON_SIZE 0.033
#define PORTRTAIT_SIZE 0.08
#define PORTRTAIT_POSITION 0.215, 0.03
#define BUILD_QUEUE_POSITION 0.0100, -0.0390
#define BUILD_QUEUE_SIZE 0.0280, 0.0310
#define BUILD_QUEUE_IMAGES_POSITION 0.0095, -0.0800
#define BUILD_QUEUE_IMAGES_SIZE 0.0200, 0.0215
#define MULTISELECT_SIZE 0.025
#define MULTISELECT_POSITION 0.3260, 0.0820

static void Init_SimpleProgressIndicator(void) {
    UI_FRAME(SimpleProgressIndicator);
    SimpleProgressIndicator->Width = MAKE(VECTOR2, INFO_PANEL_UNIT_DETAIL).x;
    UI_SetTexture(SimpleProgressIndicator, "SimpleXpBarConsole", true);
    UI_SetTexture2(SimpleProgressIndicator, "SimpleXpBarBorder", true);
    SimpleProgressIndicator->Color = MAKE(COLOR32,160,0,160,255);
    UI_WriteFrameWithChildren(SimpleProgressIndicator, NULL);
}

static void Init_SimpleInfoPanelIconDamage(LPFRAMEDEF parent, LPEDICT unit) {
    DWORD const dmgBase = UNIT_ATTACK1_DAMAGE_BASE(unit->class_id);
    DWORD const dmgDice = UNIT_ATTACK1_DAMAGE_NUMBER_OF_DICE(unit->class_id);
    DWORD const dmgNumSides = UNIT_ATTACK1_DAMAGE_SIDES_PER_DIE(unit->class_id);
    UI_FRAME(SimpleInfoPanelIconDamage);
    UI_CHILD_FRAME(InfoPanelIconBackdrop, SimpleInfoPanelIconDamage);
    UI_CHILD_FRAME(InfoPanelIconLevel, SimpleInfoPanelIconDamage);
    UI_CHILD_FRAME(InfoPanelIconValue, SimpleInfoPanelIconDamage);
    UI_SetParent(SimpleInfoPanelIconDamage, parent);
    UI_SetPoint(SimpleInfoPanelIconDamage, FRAMEPOINT_TOPLEFT, parent, FRAMEPOINT_TOPLEFT, 0, -0.040);
    UI_SetText(InfoPanelIconLevel, "1");
    UI_SetText(InfoPanelIconValue, "%d - %d", dmgBase + dmgDice, dmgBase + dmgDice * dmgNumSides);
    UI_SetTexture(InfoPanelIconBackdrop, "InfoPanelIconDamagePierce", true);
}

static void Init_SimpleInfoPanelIconArmor(LPFRAMEDEF parent, LPEDICT unit) {
    UI_FRAME(SimpleInfoPanelIconArmor);
    UI_CHILD_VALUE(InfoPanelIconBackdrop, SimpleInfoPanelIconArmor, Texture, "InfoPanelIconArmorLarge", true);
    UI_CHILD_VALUE(InfoPanelIconLevel, SimpleInfoPanelIconArmor, Text, "1");
    UI_CHILD_VALUE(InfoPanelIconValue, SimpleInfoPanelIconArmor, Text, "4");
    UI_SetParent(SimpleInfoPanelIconArmor, parent);
    UI_SetPoint(SimpleInfoPanelIconArmor, FRAMEPOINT_TOPLEFT, parent, FRAMEPOINT_TOPLEFT, 0, -0.0745);
}

static void Init_SimpleInfoPanelIconHero(LPFRAMEDEF parent, LPEDICT unit) {
    int hero_str = UNIT_STRENGTH(unit->class_id) + UNIT_STRENGTH_PER_LEVEL(unit->class_id) * unit->hero.level;
    int hero_agi = UNIT_AGILITY(unit->class_id) + UNIT_AGILITY_PER_LEVEL(unit->class_id) * unit->hero.level;
    int hero_int = UNIT_INTELLIGENCE(unit->class_id) + UNIT_INTELLIGENCE_PER_LEVEL(unit->class_id) * unit->hero.level;
    LPCSTR primary = UNIT_PRIMARY_ATTRIBUTE(unit->class_id);
    if (primary == NULL || strlen(primary) < 3)
        return;
    char icon[256] = {0};
    sprintf(icon, "InfoPanelIconHeroIcon%s", primary);
    UI_FRAME(SimpleInfoPanelIconHero);
    UI_CHILD_VALUE(InfoPanelIconHeroIcon, SimpleInfoPanelIconHero, Texture, icon, true);
    UI_CHILD_VALUE(InfoPanelIconHeroStrengthValue, SimpleInfoPanelIconHero, Text, "%d", hero_str);
    UI_CHILD_VALUE(InfoPanelIconHeroAgilityValue, SimpleInfoPanelIconHero, Text, "%d", hero_agi);
    UI_CHILD_VALUE(InfoPanelIconHeroIntellectValue, SimpleInfoPanelIconHero, Text, "%d", hero_int);
    UI_SetParent(SimpleInfoPanelIconHero, parent);
    UI_SetPoint(SimpleInfoPanelIconHero, FRAMEPOINT_TOPLEFT, parent, FRAMEPOINT_TOPLEFT, 0.1, -0.037);
}

static void Init_SimpleInfoPanelIconFood(LPFRAMEDEF parent, LPEDICT unit) {
    UI_FRAME(SimpleInfoPanelIconFood);
    UI_CHILD_VALUE(InfoPanelIconBackdrop, SimpleInfoPanelIconFood, Texture, "InfoPanelIconArmorLarge", true);
    UI_CHILD_VALUE(InfoPanelIconLevel, SimpleInfoPanelIconFood, Text, "1");
    UI_CHILD_VALUE(InfoPanelIconValue, SimpleInfoPanelIconFood, Text, "4");
    UI_SetParent(SimpleInfoPanelIconFood, parent);
    UI_SetPoint(SimpleInfoPanelIconFood, FRAMEPOINT_TOPLEFT, parent, FRAMEPOINT_TOPLEFT, 0, -0.0345);
}

static void UI_WriteBuildQueue(LPFRAMEDEF infoPanel, LPEDICT unit) {
    UI_CHILD_FRAME(SimpleBuildTimeIndicator, infoPanel);
    UI_CHILD_VALUE(SimpleBuildingActionLabel, infoPanel, Text, "Constructing");
    FRAMEDEF firstItem, imageList;
    
    UI_InitFrame(&firstItem, FT_TEXTURE);
    UI_SetParent(&firstItem, infoPanel);
    UI_SetPoint(&firstItem, FRAMEPOINT_TOPLEFT, infoPanel, FRAMEPOINT_TOPLEFT, BUILD_QUEUE_POSITION);
    UI_SetSize(&firstItem, BUILD_QUEUE_SIZE);

    UI_CHILD_VALUE(SimpleBuildQueueBackdrop, infoPanel, Hidden, unit->currentmove->think == ai_birth);

    UI_InitFrame(&imageList, FT_BUILDQUEUE);
    imageList.BuildQueue.FirstItem = &firstItem;
    imageList.BuildQueue.BuildTimer = SimpleBuildTimeIndicator;
    imageList.BuildQueue.ItemOffset = BUILDQUEUE_OFFSET;
    for (LPEDICT queue = unit->build; queue; queue = queue->build) {
        LPCSTR buttonClassName = GetClassName(queue->class_id);
        LPCSTR buttonArt = FindConfigValue(buttonClassName, STR_ART);
        imageList.BuildQueue.Queue[imageList.BuildQueue.NumQueue].image = UI_LoadTexture(buttonArt, false);
        imageList.BuildQueue.Queue[imageList.BuildQueue.NumQueue].entity = queue->s.number;
        imageList.BuildQueue.NumQueue++;
        if (queue->build == queue || imageList.BuildQueue.NumQueue == MAX_BUILD_QUEUE)
            break;
    }

    UI_SetParent(&imageList, infoPanel);
    UI_SetPoint(&imageList, FRAMEPOINT_TOPLEFT, infoPanel, FRAMEPOINT_TOPLEFT, BUILD_QUEUE_IMAGES_POSITION);
    UI_SetSize(&imageList, BUILD_QUEUE_IMAGES_SIZE);
    
    UI_WriteFrame(&firstItem);
    UI_WriteFrame(&imageList);
}

static void Init_SimpleInfoPanelBuildingDetail(LPFRAMEDEF bottomPanel, LPEDICT unit) {
    UI_FRAME(SimpleInfoPanelBuildingDetail);
    UI_CHILD_VALUE(SimpleBuildingNameValue, SimpleInfoPanelBuildingDetail, Text, UNIT_NAME(unit->class_id));
    UI_CHILD_VALUE(SimpleBuildTimeIndicator, SimpleInfoPanelBuildingDetail, Texture, "SimpleBuildTimeIndicator", true);
    UI_CHILD_VALUE(SimpleBuildingActionLabel, SimpleInfoPanelBuildingDetail, Text, "Training");
    UI_CHILD_VALUE(SimpleBuildingDescriptionValue, SimpleInfoPanelBuildingDetail, Text, " ");
    UI_CHILD_VALUE(SimpleBuildQueueBackdrop, SimpleInfoPanelBuildingDetail, Hidden, false);
    
    UI_SetTexture2(SimpleBuildTimeIndicator, "SimpleBuildTimeIndicatorBorder", true);

    UI_WriteFrameWithChildren(SimpleInfoPanelBuildingDetail, bottomPanel);
//    Init_SimpleInfoPanelIconArmor(SimpleInfoPanelBuildingDetail, unit);
//    Init_SimpleInfoPanelIconFood(SimpleInfoPanelBuildingDetail, unit);
    UI_WriteBuildQueue(SimpleInfoPanelBuildingDetail, unit);
}

static void Init_SimpleInfoPanelUnitDetail(LPFRAMEDEF bottomPanel, LPEDICT unit) {
    LPCSTR unitTypeName = UNIT_NAME(unit->class_id);
    LPCSTR unitHeroName = UNIT_PROPER_NAMES(unit->class_id);
    
    UI_FRAME(SimpleInfoPanelUnitDetail);
    UI_CHILD_VALUE(SimpleNameValue, SimpleInfoPanelUnitDetail, Text, unitHeroName ? unitHeroName : unitTypeName);
    UI_CHILD_VALUE(SimpleClassValue, SimpleInfoPanelUnitDetail, Text, "Level %d %s", unit->hero.level, unitTypeName);
    
    Init_SimpleInfoPanelIconDamage(SimpleInfoPanelUnitDetail, unit);
    Init_SimpleInfoPanelIconArmor(SimpleInfoPanelUnitDetail, unit);
    Init_SimpleInfoPanelIconHero(SimpleInfoPanelUnitDetail, unit);
    
    UI_WriteFrameWithChildren(SimpleInfoPanelUnitDetail, bottomPanel);
}

static LPFRAMEDEF Init_BottomPanel(void) {
    static FRAMEDEF BottomPanel;
    UI_InitFrame(&BottomPanel, FT_SIMPLEFRAME);
    UI_SetSize(&BottomPanel, INFO_PANEL_UNIT_DETAIL);
    UI_SetPoint(&BottomPanel, FRAMEPOINT_BOTTOM, NULL, FRAMEPOINT_BOTTOM, 0, 0);
    return &BottomPanel;
}

LPCSTR GetBuildCommand(unitRace_t race) {
    switch (race) {
        case RACE_HUMAN: return STR_CmdBuildHuman;
        case RACE_ORC: return STR_CmdBuildOrc;
        case RACE_UNDEAD: return STR_CmdBuildUndead;
        case RACE_NIGHTELF: return STR_CmdBuildNightElf;
        default: return STR_CmdBuild;
    }
}

static LPCSTR remove_quotes(LPCSTR text) {
    if (*text != '"')
        return text;
    static char text2[1024] = { 0 };
    memset(text2, 0, sizeof(text2));
    memcpy(text2, text+1, strlen(text)-2);
    return text2;
}

static LPCSTR get_ability_art(LPCSTR code) {
    if (!strcmp(code, STR_CmdBuild)) {
        return GetBuildCommand(RACE_HUMAN);
    } else {
        return code;
    }
}

void UI_AddCommandButton(LPCSTR code) {
    DWORD ToolTipGoldIcon = UI_LoadTexture("ToolTipGoldIcon", true);
    DWORD ToolTipLumberIcon = UI_LoadTexture("ToolTipLumberIcon", true);
//    DWORD ToolTipStonesIcon = UI_LoadTexture("ToolTipStonesIcon", true);
//    DWORD ToolTipManaIcon = UI_LoadTexture("ToolTipManaIcon", true);
    DWORD ToolTipSupplyIcon = UI_LoadTexture("ToolTipSupplyIcon", true);
    LPCSTR altcode = get_ability_art(code);
    LPCSTR art = FindConfigValue(altcode, STR_ART);
    LPCSTR buttonpos = FindConfigValue(altcode, STR_BUTTONPOS);
    LPCSTR tip = FindConfigValue(altcode, STR_TIP);
    LPCSTR ubertip = FindConfigValue(altcode, STR_UBERTIP);
    FRAMEDEF button;
    if (!art) {
        gi.error("Not ART for %s", altcode);
        return;
    }
    if (!buttonpos) {
        gi.error("Not BUTTONPOS for %s", altcode);
        return;
    }
    char tooltip[1024] = { 0 };
    DWORD x, y;
    DWORD class_id = *(DWORD const*)code;
    DWORD gold_cost = UNIT_GOLD_COST(class_id);
    DWORD limber_cost = UNIT_LUMBER_COST(class_id);
    DWORD food_cost = UNIT_FOOD_USED(class_id);
    if (gold_cost > 0 || limber_cost > 0 || food_cost > 0) {
        // Calculate required space for the tooltip
        size_t tip_len = tip ? strlen(tip) : 0;
        size_t ubertip_len = ubertip ? strlen(remove_quotes(ubertip)) : 0;
        size_t remaining = sizeof(tooltip) - 1; // Reserve 1 byte for null terminator
        
        // Add tip with newline if there's enough space
        if (tip_len + 2 <= remaining) { // +2 for "|n"
            int written = snprintf(tooltip, remaining, "%s|n", tip);
            if (written > 0) remaining -= written;
        }
        
        // Add resource icons
        if (gold_cost > 0 && remaining > 20) { // Approximate space needed
            int written = snprintf(tooltip+strlen(tooltip), remaining, "<Icon,%d> %d   ", ToolTipGoldIcon, gold_cost);
            if (written > 0) remaining -= written;
        }
        if (limber_cost > 0 && remaining > 20) {
            int written = snprintf(tooltip+strlen(tooltip), remaining, "<Icon,%d> %d   ", ToolTipLumberIcon, limber_cost);
            if (written > 0) remaining -= written;
        }
        if (food_cost > 0 && remaining > 20) {
            int written = snprintf(tooltip+strlen(tooltip), remaining, "<Icon,%d> %d   ", ToolTipSupplyIcon, food_cost);
            if (written > 0) remaining -= written;
        }
        
        // Add ubertip if there's enough space
        if (ubertip_len + 2 <= remaining) { // +2 for "|n"
            snprintf(tooltip+strlen(tooltip), remaining, "|n%s", remove_quotes(ubertip));
        }
    } else if (tip || ubertip) {
        // Calculate required space
        size_t tip_len = tip ? strlen(tip) : 0;
        size_t ubertip_len = ubertip ? strlen(remove_quotes(ubertip)) : 0;
        
        // Check if we have enough space for both
        if (tip_len + ubertip_len + 2 <= sizeof(tooltip) - 1) { // +2 for "|n"
            snprintf(tooltip, sizeof(tooltip)-1, "%s|n%s", 
                    tip ? tip : "", 
                    ubertip ? remove_quotes(ubertip) : "");
        } else if (tip_len <= sizeof(tooltip) - 1) {
            // Just include tip if there's not enough space for both
            snprintf(tooltip, sizeof(tooltip)-1, "%s", tip);
        } else {
            // Truncate tip if it's too long
            snprintf(tooltip, sizeof(tooltip)-1, "%.1019s...", tip);
        }
    }
    sscanf(buttonpos, "%d,%d", &x, &y);
    VECTOR2 bpos = MAKE(VECTOR2, COMMAND_BUTTON_POSITION(x, y));
    UI_InitFrame(&button, FT_COMMANDBUTTON);
    UI_SetTexture(&button, art, true);
    UI_SetSize(&button, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE);
    UI_SetOnClick(&button, "button %s", code);
    UI_SetPoint(&button, FRAMEPOINT_CENTER, NULL, FRAMEPOINT_BOTTOM, bpos.x, bpos.y);
//    button.f.tex.index2 = UI_LoadTexture("CommandButtonActiveHighlight", true);
    button.Tooltip = tooltip;
    button.AlphaMode = x==0 && y==0;
    button.Stat = FindAbilityIndex(code);
    UI_WriteFrame(&button);
}

void ui_portrait(LPGAMECLIENT client) {
    LPEDICT ent = G_GetMainSelectedUnit(client);
    FRAMEDEF portrait;
    UI_InitFrame(&portrait, FT_PORTRAIT);
    portrait.Portrait.model = ent->s.model;
    portrait.Width = PORTRTAIT_SIZE;
    portrait.Height = PORTRTAIT_SIZE;
    UI_SetPoint(&portrait, FRAMEPOINT_BOTTOMLEFT, NULL, FRAMEPOINT_BOTTOMLEFT, PORTRTAIT_POSITION);
    UI_WriteFrame(&portrait);
}

void Get_Portrait_f(LPEDICT edict) {
    UI_WRITE_LAYER(edict, ui_portrait, LAYER_PORTRAIT);
}

void ui_unit_inventory(LPGAMECLIENT client) {
    LPEDICT ent = G_GetMainSelectedUnit(client);
    if (!ent)
        return;
    char tooltip[1024] = { 0 };
    FOR_LOOP(i, MAX_INVENTORY) {
        DWORD itemID = ent->inventory[i];
//        itemID = MAKEFOURCC('s','e','h','r');
        if (!itemID)
            continue;
        DWORD x = i % 2;
        DWORD y = i / 2;
        LPCSTR code = GetClassName(itemID);
        LPCSTR art = FindConfigValue(code, STR_ART);
        LPCSTR tip = FindConfigValue(code, STR_TIP);
        LPCSTR ubertip = FindConfigValue(code, STR_UBERTIP);
        FRAMEDEF button;
        
        // Calculate required space
        size_t tip_len = tip ? strlen(tip) : 0;
        size_t ubertip_len = ubertip ? strlen(remove_quotes(ubertip)) : 0;
        
        // Check if we have enough space for both
        if (tip_len + ubertip_len + 2 <= sizeof(tooltip) - 1) { // +2 for "|n"
            snprintf(tooltip, sizeof(tooltip)-1, "%s|n%s", 
                    tip ? tip : "", 
                    ubertip ? remove_quotes(ubertip) : "");
        } else if (tip_len <= sizeof(tooltip) - 1) {
            // Just include tip if there's not enough space for both
            snprintf(tooltip, sizeof(tooltip)-1, "%s", tip);
        } else {
            // Truncate tip if it's too long
            snprintf(tooltip, sizeof(tooltip)-1, "%.1019s...", tip);
        }
        VECTOR2 bpos = MAKE(VECTOR2, INVENTORY_BUTTON_POSITION(x, y));
        UI_InitFrame(&button, FT_COMMANDBUTTON);
        UI_SetTexture(&button, art, false);
        UI_SetSize(&button, INVENTORY_BUTTON_SIZE, INVENTORY_BUTTON_SIZE);
        UI_SetOnClick(&button, "inventory %s", code);
        UI_SetPoint(&button, FRAMEPOINT_CENTER, NULL, FRAMEPOINT_BOTTOM, bpos.x, bpos.y);
        button.Tooltip = tooltip;
        button.AlphaMode = x==0 && y==0;
        button.Stat = FindAbilityIndex(code);
        UI_WriteFrame(&button);

    }
}

void ui_unit_commands(LPGAMECLIENT client) {
    LPEDICT ent = G_GetMainSelectedUnit(client);
    if (!ent)
        return;
    if (ent->currentmove->think == ai_birth)
        return;
    LPCSTR abilities = UNIT_ABILITIES_NORMAL(ent->class_id);
    LPCSTR trains = UNIT_TRAINS(ent->class_id);
    if (UNIT_SPEED(ent->class_id) > 0) {
        UI_AddCommandButton(STR_CmdMove);
        UI_AddCommandButton(STR_CmdHoldPos);
        UI_AddCommandButton(STR_CmdPatrol);
        UI_AddCommandButton(STR_CmdStop);
    }
    if (UNIT_ATTACK1_DAMAGE_NUMBER_OF_DICE(ent->class_id) != 0) {
        UI_AddCommandButton(STR_CmdAttack);
    }
    if (UNIT_BUILDS(ent->class_id)) {
        UI_AddCommandButton(STR_CmdBuild);
    }
    if (abilities) {
        PARSE_LIST(abilities, abil, parse_segment) {
            LPCSTR code = gi.FindSheetCell(game.config.abilities, abil, "code");
            if (code && FindAbilityByClassname(code)) {
                UI_AddCommandButton(code);
            } else {
                gi.error("Ability not implemented: %s - %s", abil, gi.FindSheetCell(game.config.abilities, abil, "comments"));
            }
        }
    }
    if (trains) {
        PARSE_LIST(trains, unit, parse_segment) {
            UI_AddCommandButton(unit);
        }
    }
}

DWORD NumSelectedUnits(LPGAMECLIENT client) {
    DWORD selent = 0;
    FOR_SELECTED_UNITS(client, ent) {
        selent++;
    }
    return selent;
}

void Init_SimpleInfoPanelMultiselect(LPGAMECLIENT client) {
    FRAMEDEF multiselect;
    
    UI_InitFrame(&multiselect, FT_MULTISELECT);
    
    multiselect.Multiselect.HpBar = gi.ImageIndex("SimpleHpBarConsole");
    multiselect.Multiselect.ManaBar = gi.ImageIndex("SimpleManaBarConsole");
    multiselect.Multiselect.NumColumns = MULTISELECT_COLUMNS;
    multiselect.Multiselect.Offset = MAKE(VECTOR2, MULTISELECT_OFFSET);
    
    FOR_SELECTED_UNITS(client, ent) {
        uiBuildQueueItem_t *it = multiselect.Multiselect.Items + (multiselect.Multiselect.NumItems++);
        LPCSTR art = FindConfigValue(GetClassName(ent->class_id), STR_ART);
        it->image = gi.ImageIndex(art);
        it->entity = ent->s.number;
    }
    
    UI_SetSize(&multiselect, MULTISELECT_SIZE, MULTISELECT_SIZE);
    UI_SetPoint(&multiselect, FRAMEPOINT_CENTER, NULL, FRAMEPOINT_BOTTOMLEFT, MULTISELECT_POSITION);
    UI_WriteFrame(&multiselect);
}

void ui_unit_info(LPGAMECLIENT client) {
    if (NumSelectedUnits(client) > 1) {
        Init_SimpleInfoPanelMultiselect(client);
//        DWORD selent = 0;
//        FOR_SELECTED_UNITS(client, ent) {
//            HUD_MultiselectIcon(ent, selent++);
//        }
    } else if (NumSelectedUnits(client) > 0) {
        LPEDICT ent = G_GetMainSelectedUnit(client);
        LPFRAMEDEF bottomPanel = Init_BottomPanel();
        UI_WriteFrame(bottomPanel);
        if (ent->build) {
            Init_SimpleInfoPanelBuildingDetail(bottomPanel, ent);
        } else {
            Init_SimpleInfoPanelUnitDetail(bottomPanel, ent);
            Init_SimpleProgressIndicator();
        }
    }
}

void ui_cancel_only(LPGAMECLIENT client) {
    UI_AddCommandButton(STR_CmdCancel);
}

void Get_Commands_f(LPEDICT edict) {
    UI_WRITE_LAYER(edict, ui_unit_commands, LAYER_COMMANDBAR);
    UI_WRITE_LAYER(edict, ui_unit_info, LAYER_INFOPANEL);
    UI_WRITE_LAYER(edict, ui_unit_inventory, LAYER_INVENTORY);
    memset(&edict->client->menu, 0, sizeof(menu_t));
}

void UI_AddCancelButton(LPEDICT ent) {
    UI_WRITE_LAYER(ent, ui_cancel_only, LAYER_COMMANDBAR);
    memset(&ent->client->menu, 0, sizeof(menu_t));
}

void UI_ShowInterface(LPEDICT ent, BOOL flag, FLOAT fadeDuration) {
    if (flag) {
        ent->client->ps.uiflags = 1 << LAYER_CINEMATIC;
    } else {
        ent->client->ps.uiflags = ~(1 << LAYER_CINEMATIC);
    }
}