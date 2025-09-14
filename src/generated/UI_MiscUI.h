#pragma once
#include "../common/common.h"
typedef struct UI_MiscUI {
    union {
        LPCSTR QuickSaveFile;
        LPCSTR CreditsFile;
        LPCSTR Profanity;
        float BashTextHeight;
        float ManaBurnTextHeight;
        float CriticalStrikeTextHeight;
        float MissTextHeight;
        float BountyTextHeight;
        float LumberTextHeight;
        float GoldTextHeight;
        LPCSTR FontTexture;
    } Misc;
    union {
        LPCSTR ChatFont4;
        LPCSTR ChatFontDisplayName4;
        LPCSTR ChatFont3;
        LPCSTR ChatFontDisplayName3;
        LPCSTR ChatFont2;
        LPCSTR ChatFontDisplayName2;
        LPCSTR ChatFont1;
        LPCSTR ChatFontDisplayName1;
        LPCSTR ChatFont0;
        LPCSTR ChatFontDisplayName0;
        float ChatFontCount;
        float ChatFontEnabled;
    } ChatFonts;
    union {
        LPCSTR BattleNetLadder;
    } Websites;
    union {
        // score screen team number text
        float ScoreScreenTeam;
        // score screen large text
        float ScoreScreenLarge;
        // score screen normal text
        float ScoreScreenNormal;
        // map preference max item text
        float MapPreferenceItem;
        // map preference max players text
        float MapPreferenceMaxPlayers;
        // map list item text
        float MapListBoxItem;
        // map list max players text
        float MapListBoxMaxPlayers;
        // loading screen team header text
        float LoadingTeamHeader;
        // list box item text
        float ListBoxItem;
        // glue screen tooltip
        float GlueToolTip;
        // check box list item text
        float CheckBoxList;
        // b.net userlist level
        float BattleNetUserListLevel;
        // b.net userlist name
        float BattleNetUserListName;
        // Battle.net news items
        float BattleNetNewsBox;
        // items in the right-click context menu in Battle.net
        float BattleNetChatActionMenu;
        // team headers in the pre-game chatroom
        float ChatroomHeaders;
        // chat display edit box in the pre-game chatroom
        float ChatroomChatEditBox;
        // chat display text area in the pre-game chatroom
        float ChatroomTextArea;
        // chat display edit bxo in the Battle.net chatroom
        float BattleNetChatEditBox;
        // chat display text area in the Battle.net chatroom
        float BattleNetChatTextArea;
        // chat display edit bxo in the Battle.net chatroom
        float BattleNetTeamChatEditBox;
        // chat display text area in the Battle.net chatroom
        float BattleNetTeamChatTextArea;
        // description text on unit tooltip
        float UnitTipDesc;
        // unit name on unit tooltip
        float UnitTipUnitName;
        // player name on unit tooltip
        float UnitTipPlayerName;
        // quest description text in Quest Dialog
        float QuestDescription;
        // hit points and mana text below 3D unit portrait
        float PortraitStats;
        // leader board text
        float LeaderBoard;
        // "Inventory" text above your inventory
        float Inventory;
        // in-game chat text
        float WorldFrameChatMessage;
        // small text that is used for in-game trigger based dialog
        float WorldFrameUnitMessage;
        // large upkeep text that appears below the time of day indicator
        float WorldFrameTopMessage;
        // single line of error text that appears above the console
        float WorldFrameMessage;
        // little number at the bottom right of command buttons
        float CommandButtonNumber;
        // chat edit bar text
        float ChatEditBar;
        // tooltip cost value
        float ToolTipCost;
        // tooltip description and ubertip
        float ToolTipDesc;
        // tooltip name (first line)
        float ToolTipName;
    } FontHeights;
    union {
        char KEY_CANCEL;
        char KEY_DELETE;
        char GAMEOVER_LOAD;
        char GAMEOVER_REDUCE_DIFFICULTY;
        char GAMEOVER_RESTART;
        char GAMEOVER_QUIT_MISSION;
        char GAMEOVER_OK;
        char GAMEOVER_QUIT_GAME;
        char GAMEOVER_CONTINUE_OBSERVING;
        char GAMEOVER_CONTINUE_GAME;
        char GAMEOVER_CONTINUE;
        // Alt-F
        char FormationToggle;
        // Alt-A
        char MinimapColors;
        // Alt-T
        char MinimapTerrain;
        // Alt-G
        char MinimapSignal;
        // Ctrl-C and Alt-C
        char UnitCenter;
        // Ctrl-S
        char SfxToggle;
        // Ctrl-M
        char MusicToggle;
        // Alt-Q
        char QuickQuit;
        // Alt-O
        char QuickOptions;
        // Alt-H
        char QuickHelp;
        // Alt-L
        char QuickLoad;
        // Alt-S
        char QuickSave;
    } Hotkeys;
} UI_MiscUI;

void Init_UI_MiscUI(UI_MiscUI *config);
void Shutdown_UI_MiscUI(UI_MiscUI *config);

void Init_UI_MiscUI(UI_MiscUI *config) {
    config->Misc.QuickSaveFile = "QuickSave"; 
    config->Misc.CreditsFile = "maps\\campaign\\warcraftiiicredits.w3m"а яеяl; (null)
    config->Misc.Profanity = ""; 
    config->Misc.BashTextHeight = 0.024; 
    config->Misc.ManaBurnTextHeight = 0.024; 
    config->Misc.CriticalStrikeTextHeight = 0.024; 
    config->Misc.MissTextHeight = 0.024; 
    config->Misc.BountyTextHeight = 0.024; 
    config->Misc.LumberTextHeight = 0.024; 
    config->Misc.GoldTextHeight = 0.024; 
    config->Misc.FontTexture = "dfst-m3u.ttf"; 
    config->ChatFonts.ChatFont4 = "Fonts\\dfst-m3u.ttf"aftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontDisplayName4 = "Simplified Chinese"aftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFont3 = "Fonts\\dffn_b31.ttf"aftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontDisplayName3 = "Traditional Chinese"ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFont2 = "Fonts\\DFHeiMd.ttf"e"ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontDisplayName2 = "Korean"FHeiMd.ttf"e"ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFont1 = "Fonts\\tt5500m_.ttf""ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontDisplayName1 = "Japanese"500m_.ttf""ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFont0 = "Fonts\\dfst-m3u.ttf""ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontDisplayName0 = "Default"st-m3u.ttf""ftiiicredits.w3m"а яеяl; (null)
    config->ChatFonts.ChatFontCount = 5; 
    config->ChatFonts.ChatFontEnabled = 1; 
    config->Websites.BattleNetLadder = "http://www.battle.net/war3/ladder"3m"а яеяl; //www.battle.net/war3/ladder"
    config->FontHeights.ScoreScreenTeam = 0.009; // score screen team number text
    config->FontHeights.ScoreScreenLarge = 0.011; // score screen large text
    config->FontHeights.ScoreScreenNormal = 0.011; // score screen normal text
    config->FontHeights.MapPreferenceItem = 0.013; // map preference max item text
    config->FontHeights.MapPreferenceMaxPlayers = 0.009; // map preference max players text
    config->FontHeights.MapListBoxItem = 0.013; // map list item text
    config->FontHeights.MapListBoxMaxPlayers = 0.009; // map list max players text
    config->FontHeights.LoadingTeamHeader = 0.013; // loading screen team header text
    config->FontHeights.ListBoxItem = 0.013; // list box item text
    config->FontHeights.GlueToolTip = 0.011; // glue screen tooltip
    config->FontHeights.CheckBoxList = 0.013; // check box list item text
    config->FontHeights.BattleNetUserListLevel = 0.011; // b.net userlist level
    config->FontHeights.BattleNetUserListName = 0.011; // b.net userlist name
    config->FontHeights.BattleNetNewsBox = 0.011; // Battle.net news items
    config->FontHeights.BattleNetChatActionMenu = 0.011; // items in the right-click context menu in Battle.net
    config->FontHeights.ChatroomHeaders = 0.011; // team headers in the pre-game chatroom
    config->FontHeights.ChatroomChatEditBox = 0.015; // chat display edit box in the pre-game chatroom
    config->FontHeights.ChatroomTextArea = 0.011; // chat display text area in the pre-game chatroom
    config->FontHeights.BattleNetChatEditBox = 0.015; // chat display edit bxo in the Battle.net chatroom
    config->FontHeights.BattleNetChatTextArea = 0.011; // chat display text area in the Battle.net chatroom
    config->FontHeights.BattleNetTeamChatEditBox = 0.015; // chat display edit bxo in the Battle.net chatroom
    config->FontHeights.BattleNetTeamChatTextArea = 0.011; // chat display text area in the Battle.net chatroom
    config->FontHeights.UnitTipDesc = 0.011; // description text on unit tooltip
    config->FontHeights.UnitTipUnitName = 0.011; // unit name on unit tooltip
    config->FontHeights.UnitTipPlayerName = 0.011; // player name on unit tooltip
    config->FontHeights.QuestDescription = 0.011; // quest description text in Quest Dialog
    config->FontHeights.PortraitStats = 0.011; // hit points and mana text below 3D unit portrait
    config->FontHeights.LeaderBoard = 0.011; // leader board text
    config->FontHeights.Inventory = 0.011; // "Inventory" text above your inventory
    config->FontHeights.WorldFrameChatMessage = 0.013; // in-game chat text
    config->FontHeights.WorldFrameUnitMessage = 0.015; // small text that is used for in-game trigger based dialog
    config->FontHeights.WorldFrameTopMessage = 0.024; // large upkeep text that appears below the time of day indicator
    config->FontHeights.WorldFrameMessage = 0.015; // single line of error text that appears above the console
    config->FontHeights.CommandButtonNumber = 0.009; // little number at the bottom right of command buttons
    config->FontHeights.ChatEditBar = 0.013; // chat edit bar text
    config->FontHeights.ToolTipCost = 0.011; // tooltip cost value
    config->FontHeights.ToolTipDesc = 0.011; // tooltip description and ubertip
    config->FontHeights.ToolTipName = 0.011; // tooltip name (first line)
    config->Hotkeys.KEY_CANCEL = 'A'; 
    config->Hotkeys.KEY_DELETE = 'D'; 
    config->Hotkeys.GAMEOVER_LOAD = 'L'; 
    config->Hotkeys.GAMEOVER_REDUCE_DIFFICULTY = 'D'; 
    config->Hotkeys.GAMEOVER_RESTART = 'R'; 
    config->Hotkeys.GAMEOVER_QUIT_MISSION = 'Q'; 
    config->Hotkeys.GAMEOVER_OK = 'O'; 
    config->Hotkeys.GAMEOVER_QUIT_GAME = 'Q'; 
    config->Hotkeys.GAMEOVER_CONTINUE_OBSERVING = 'O'; 
    config->Hotkeys.GAMEOVER_CONTINUE_GAME = 'C'; 
    config->Hotkeys.GAMEOVER_CONTINUE = 'C'; 
    config->Hotkeys.FormationToggle = 'F'; // Alt-F
    config->Hotkeys.MinimapColors = 'A'; // Alt-A
    config->Hotkeys.MinimapTerrain = 'T'; // Alt-T
    config->Hotkeys.MinimapSignal = 'G'; // Alt-G
    config->Hotkeys.UnitCenter = 'C'; // Ctrl-C and Alt-C
    config->Hotkeys.SfxToggle = 'S'; // Ctrl-S
    config->Hotkeys.MusicToggle = 'M'; // Ctrl-M
    config->Hotkeys.QuickQuit = 'Q'; // Alt-Q
    config->Hotkeys.QuickOptions = 'O'; // Alt-O
    config->Hotkeys.QuickHelp = 'H'; // Alt-H
    config->Hotkeys.QuickLoad = 'L'; // Alt-L
    config->Hotkeys.QuickSave = 'S'; // Alt-S
}

