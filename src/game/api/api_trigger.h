#include "g_local.h"
#include "jass/vm_public.h"
DWORD CreateTrigger(LPJASS j) {
    API_ALLOC(TRIGGER, trigger);
    return 1;
}
DWORD DestroyTrigger(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return 0;
}
DWORD ResetTrigger(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return 0;
}
DWORD EnableTrigger(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    whichTrigger->disabled = false;
    return 0;
}
DWORD DisableTrigger(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    whichTrigger->disabled = true;
    return 0;
}
DWORD IsTriggerEnabled(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return jass_pushboolean(j, !whichTrigger->disabled);
}
DWORD TriggerWaitOnSleeps(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    //BOOL flag = jass_checkboolean(j, 2);
    return 0;
}
DWORD IsTriggerWaitOnSleeps(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return jass_pushboolean(j, 0);
}
DWORD GetTriggeringTrigger(LPJASS j) {
    return jass_pushlighthandle(j, jass_getcontext(j)->trigger, "trigger");
}
DWORD GetTriggerEventId(LPJASS j) {
    return jass_pushnullhandle(j, "eventid");
}
DWORD GetTriggerEvalCount(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return jass_pushinteger(j, 0);
}
DWORD GetTriggerExecCount(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return jass_pushinteger(j, 0);
}
DWORD TriggerRegisterVariableEvent(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    //LPCSTR varName = jass_checkstring(j, 2);
    //HANDLE opcode = jass_checkhandle(j, 3, "limitop");
    //FLOAT limitval = jass_checknumber(j, 4);
    return jass_pushnullhandle(j, "event");
}
DWORD TriggerRegisterTimerEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    FLOAT timeout = jass_checknumber(j, 2);
    BOOL periodic = jass_checkboolean(j, 3);
    LPEVENT evt = G_MakeEvent(EVENT_GAME_TIMER_PERIADIC);
    evt->trigger = whichTrigger;
    LPTIMER timer = gi.MemAlloc(sizeof(TIMER));
    evt->timer = timer;
    timer->timeout = timeout;
    timer->elapsed = 0.0f;
    timer->remaining = timeout;
    timer->periodic = periodic;
    timer->paused = false;
    timer->active = true;
    // FIXME
    timer->handlerFunc = find_function(j, "ConditionalTriggerExecute"); 
    timer->destroyed = false;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterTimerExpireEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPTIMER t = jass_checkhandle(j, 2, "timer");
    LPEVENT evt = G_MakeEvent(EVENT_GAME_TIMER_EXPIRED);
    evt->trigger = whichTrigger;
    evt->timer = t;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterGameStateEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichState = jass_checkhandle(j, 2, "gamestate");
    HANDLE opcode = jass_checkhandle(j, 3, "limitop");
    FLOAT limitval = jass_checknumber(j, 4);
    LPEVENT evt = G_MakeEvent(EVENT_GAME_STATE_LIMIT);
    evt->trigger = whichTrigger;
    evt->gamestate = whichState;
    evt->limitop = opcode;
    evt->limitval = limitval;
    return jass_pushlighthandle(j, evt, "event");
}
// dialog click event
DWORD TriggerRegisterDialogEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichDialog = jass_checkhandle(j, 2, "dialog");
    LPEVENT evt = G_MakeEvent(EVENT_DIALOG_CLICK);
    evt->trigger = whichTrigger;
    evt->dialog = whichDialog;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterDialogButtonEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichButton = jass_checkhandle(j, 2, "button");
    LPEVENT evt = G_MakeEvent(EVENT_DIALOG_BUTTON_CLICK);
    evt->trigger = whichTrigger;
    evt->button = whichButton;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterGameEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    EVENTTYPE *whichGameEvent = jass_checkhandle(j, 2, "gameevent");
    LPEVENT evt = G_MakeEvent(*whichGameEvent);
    evt->trigger = whichTrigger;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterEnterRegion(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPREGION whichRegion = jass_checkhandle(j, 2, "region");
    LPEVENT evt = G_MakeEvent(EVENT_GAME_ENTER_REGION);
    evt->trigger = whichTrigger;
    evt->region = *whichRegion;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD GetTriggeringRegion(LPJASS j) {
    return jass_pushnullhandle(j, "region");
}
DWORD TriggerRegisterLeaveRegion(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPREGION whichRegion = jass_checkhandle(j, 2, "region");
    HANDLE filter = jass_checkhandle(j, 3, "boolexpr");
    LPEVENT evt = G_MakeEvent(EVENT_GAME_LEAVE_REGION);
    evt->trigger = whichTrigger;
    evt->region = *whichRegion;
    evt->filter = filter; 
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterTrackableHitEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE t = jass_checkhandle(j, 2, "trackable");
    LPEVENT evt = G_MakeEvent(EVENT_GAME_TRACKABLE_HIT);
    evt->trigger = whichTrigger;
    evt->trackable = t;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterTrackableTrackEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE t = jass_checkhandle(j, 2, "trackable");
    LPEVENT evt = G_MakeEvent(EVENT_GAME_TRACKABLE_TRACK);
    evt->trigger = whichTrigger;
    evt->trackable = t;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterGameVariableLimitEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPCSTR varName = jass_checkstring(j, 2);
    HANDLE opcode = jass_checkhandle(j, 3, "limitop");
    FLOAT limitval = jass_checknumber(j, 4);
    LPEVENT evt = G_MakeEvent(EVENT_GAME_VARIABLE_LIMIT);
    evt->trigger = whichTrigger;
    evt->varName = varName;
    evt->limitop = opcode;
    evt->limitval = limitval;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterPlayerEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPPLAYER whichPlayer = jass_checkhandle(j, 2, "player");
    EVENTTYPE *whichPlayerEvent = jass_checkhandle(j, 3, "playerevent");
    LPEVENT evt = G_MakeEvent(*whichPlayerEvent);
    evt->subject = PLAYER_ENT(whichPlayer);
    evt->trigger = whichTrigger;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD GetTriggerPlayer(LPJASS j) {
    return jass_pushnullhandle(j, "player");
}
DWORD TriggerRegisterPlayerUnitEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPPLAYER whichPlayer = jass_checkhandle(j, 2, "player");
    EVENTTYPE *whichPlayerUnitEvent = jass_checkhandle(j, 3, "playerunitevent");
    LPEVENT evt = G_MakeEvent(*whichPlayerUnitEvent);
    evt->subject = PLAYER_ENT(whichPlayer);
    evt->trigger = whichTrigger;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterPlayerAllianceChange(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPPLAYER whichPlayer = jass_checkhandle(j, 2, "player");
    HANDLE whichAlliance = jass_checkhandle(j, 3, "alliancetype");
    LPEVENT evt = G_MakeEvent(EVENT_PLAYER_ALLIANCE_CHANGED);
    evt->subject = PLAYER_ENT(whichPlayer);
    evt->trigger = whichTrigger;
    evt->gamestate = whichAlliance;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterPlayerStateEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPPLAYER whichPlayer = jass_checkhandle(j, 2, "player");
    HANDLE whichState = jass_checkhandle(j, 3, "playerstate");
    HANDLE opcode = jass_checkhandle(j, 4, "limitop");
    FLOAT limitval = jass_checknumber(j, 5);
    LPEVENT evt = G_MakeEvent(EVENT_PLAYER_STATE_LIMIT);
    evt->subject = PLAYER_ENT(whichPlayer);
    evt->trigger = whichTrigger;
    evt->gamestate = whichState;
    evt->limitop = opcode;
    evt->limitval = limitval;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterPlayerChatEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPPLAYER whichPlayer = jass_checkhandle(j, 2, "player");
    LPCSTR chatMessageToDetect = jass_checkstring(j, 3);
    BOOL exactMatchOnly = jass_checkboolean(j, 4);
    LPEVENT evt = G_MakeEvent(EVENT_PLAYER_CHAT);
    evt->subject = PLAYER_ENT(whichPlayer);
    evt->trigger = whichTrigger;
    evt->chatMessage = chatMessageToDetect;
    evt->exactMatchOnly = exactMatchOnly;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterDeathEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichWidget = jass_checkhandle(j, 2, "widget");
    LPEVENT evt = G_MakeEvent(EVENT_WIDGET_DEATH);
    evt->widget = whichWidget;
    evt->trigger = whichTrigger;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterUnitStateEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichUnit = jass_checkhandle(j, 2, "unit");
    HANDLE whichState = jass_checkhandle(j, 3, "unitstate");
    HANDLE opcode = jass_checkhandle(j, 4, "limitop");
    FLOAT limitval = jass_checknumber(j, 5);
    LPEVENT evt = G_MakeEvent(EVENT_UNIT_STATE_LIMIT);
    evt->subject = whichUnit;
    evt->trigger = whichTrigger;
    evt->gamestate = whichState;
    evt->limitop = opcode;
    evt->limitval = limitval;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterUnitEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPEDICT whichUnit = jass_checkhandle(j, 2, "unit");
    EVENTTYPE *whichEvent = jass_checkhandle(j, 3, "unitevent");
    LPEVENT evt = G_MakeEvent(*whichEvent);
    evt->subject = whichUnit;
    evt->trigger = whichTrigger;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterFilterUnitEvent(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    HANDLE whichUnit = jass_checkhandle(j, 2, "unit");
    EVENTTYPE *whichEvent = jass_checkhandle(j, 3, "unitevent");
    HANDLE filter = jass_checkhandle(j, 4, "boolexpr");
    LPEVENT evt = G_MakeEvent(*whichEvent);
    evt->subject = whichUnit;
    evt->trigger = whichTrigger;
    evt->filter = filter;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerRegisterUnitInRange(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    LPEDICT whichUnit = jass_checkhandle(j, 2, "unit");
    FLOAT range = jass_checknumber(j, 3);
    HANDLE filter = jass_checkhandle(j, 4, "boolexpr");
    LPEVENT evt = G_MakeEvent(EVENT_UNIT_IN_RANGE);
    evt->subject = whichUnit;
    evt->trigger = whichTrigger;
    evt->range = range;
    evt->filter = filter;
    return jass_pushlighthandle(j, evt, "event");
}
DWORD TriggerAddCondition(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    TRIGGERCONDITION *condition = gi.MemAlloc(sizeof(TRIGGERCONDITION));
    condition->expr = jass_checkhandle(j, 2, "boolexpr");
    ADD_TO_LIST(condition, whichTrigger->conditions);
    return jass_pushlighthandle(j, condition, "triggercondition");
}
DWORD TriggerRemoveCondition(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    TRIGGERCONDITION *whichCondition = jass_checkhandle(j, 2, "triggercondition");
    REMOVE_FROM_LIST(TRIGGERCONDITION, whichCondition, whichTrigger->conditions, gi.MemFree);
    return 0;
}
DWORD TriggerClearConditions(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    DELETE_LIST(TRIGGERCONDITION, whichTrigger->conditions, gi.MemFree);
    return 0;
}
DWORD TriggerAddAction(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    TRIGGERACTION *action = gi.MemAlloc(sizeof(TRIGGERACTION));
    action->func = jass_checkcode(j, 2);
    ADD_TO_LIST(action, whichTrigger->actions);
    return jass_pushlighthandle(j, action, "triggeraction");
}
DWORD TriggerRemoveAction(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    TRIGGERACTION *whichAction = jass_checkhandle(j, 2, "triggeraction");
    REMOVE_FROM_LIST(TRIGGERACTION, whichAction, whichTrigger->actions, gi.MemFree);
    return 0;
}
DWORD TriggerClearActions(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    DELETE_LIST(TRIGGERACTION, whichTrigger->actions, gi.MemFree);
    return 0;
}
DWORD TriggerSleepAction(LPJASS j) {
    FLOAT timeout = jass_checknumber(j, 1);
    gi.Sleep(timeout * 1000);
    return 0;
}
DWORD TriggerWaitForSound(LPJASS j) {
    gsound_t *s = jass_checkhandle(j, 1, "sound");
    FLOAT offset = jass_checknumber(j, 2);
    gi.Sleep(s->duration * 0.1 + offset * 1000);
    return 0;
}
DWORD TriggerEvaluate(LPJASS j) {
    LPSRCLOC loc = gi.MemAlloc(sizeof(SRCLOC));
    loc->file = __FILE__;
    loc->line = __LINE__;
    loc->column = 0;
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    FOR_EACH_LIST(TRIGGERCONDITION, cond, whichTrigger->conditions) {
        jass_pushfunction(j, cond->expr,loc);
        if (jass_call(j, 0) != 1 || !jass_popboolean(j)) {
            return jass_pushboolean(j, false);
        }
    }
    return jass_pushboolean(j, true);
}
DWORD TriggerExecute(LPJASS j) {
    LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    FOR_EACH_LIST(TRIGGERACTION, action, whichTrigger->actions) {
        jass_pushfunction(j, action->func,NULL);
        LPTRIGGER old_trigger = jass_getcontext(j)->trigger;
        jass_settriggeringtrigger(j, whichTrigger);
        jass_call(j, 0);
        jass_settriggeringtrigger(j, old_trigger);
    }
    return 0;
}
DWORD TriggerExecuteWait(LPJASS j) {
    //LPTRIGGER whichTrigger = jass_checkhandle(j, 1, "trigger");
    return 0;
}
DWORD GetTriggerUnit(LPJASS j) {
    return jass_pushlighthandle(j, jass_getcontext(j)->unit, "unit");
}
DWORD GetTriggerWidget(LPJASS j) {
    return jass_pushnullhandle(j, "widget");
}
