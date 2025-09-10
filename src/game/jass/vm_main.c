#include "common/shared.h"
#include "g_local.h"
#include "vm_public.h"
#include "jass_parser.h"
#include "../parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>


#ifdef DEBUG_JASS
static __thread int depth = 0, callnum = 0;
#endif

#define F_END { NULL }
#define MAX_JASS_STACK 512
#define JASS_DELIM ",;()[]+-/*="
#define JASS_CONSTANT "constant"
#define JASS_ARRAY "array"
#define JASS_TIMER "timer"
#define JASS_NULL "null"
#define JASS_FALSE "false"
#define JASS_TRUE "true"
#define JASS_UNM "-"
#define JASS_COMMA ","
#define JASS_OPERATOR(NAME,FILE,LINE) { #NAME, NAME,FILE,LINE}
#define INF_LOOP_PROTECTION 1024

#define assert_type(var, type) assert(jass_checktype(var, type))
#define JASSALLOC(type) gi.MemAlloc(sizeof(type))


#define JASS_CP_SRCLOC(dest, src) \
do { \
    if (src) { \
        dest = JASSALLOC(SRCLOC); \
        dest->file = src->file; \
        dest->line = src->line; \
        dest->column = src->column; \
    } else { \
        dest = NULL; \
    } \
} while (0)

#define JASS_ADD_STACK(j, VAR, TYPE) \
LPJASSVAR VAR = &j->stack[j->num_stack++]; \
memset(VAR, 0, sizeof(*VAR)); \
VAR->type = &jass_types[TYPE];

#define JASS_SET_VALUE(VAR, VALUE, SIZE) \
jass_setnull(VAR); \
if (VALUE) { \
  (VAR)->value = gi.MemAlloc(SIZE); \
  memcpy((VAR)->value, VALUE, SIZE); \
}

#define JASS_CMPOP(NAME, OP) \
DWORD NAME(LPJASS j) { \
    return jass_pushboolean(j, jass_checknumber(j, 1) OP jass_checknumber(j, 2)); \
}

#define JASS_NUMOP(NAME, OP) \
DWORD NAME(LPJASS j) { \
    if (jass_gettype(j, 1) == jasstype_integer && jass_gettype(j, 2) == jasstype_integer) { \
        return jass_pushinteger(j, jass_checkinteger(j, 1) OP jass_checkinteger(j, 2)); \
    } else { \
        return jass_pushnumber(j, jass_checknumber(j, 1) OP jass_checknumber(j, 2)); \
    } \
}

KNOWN_AS(jass_array, JASSARRAY);
KNOWN_AS(jass_dict, JASSDICT);
KNOWN_AS(jass_arg, JASSARG);
KNOWN_AS(jass_env, JASSENV);

VMPROGRAM VM_Compile(LPCTOKEN token);
LPPLAYER currentplayer = NULL;
LPEDICT currentunit = NULL;

LPCSTR keywords[] = {
    "elseif", "else", "endif", "set", "endfunction", "local", "then", NULL
};

extern JASSMODULE jass_funcs[];

typedef struct {
    LPCSTR name;
    void (*func)(LPJASS, LPJASSENV, LPPARSER);
} parseStatement_t;

struct jass_var {
    LPCJASSTYPE type;
    HANDLE value;
    DWORD *refcount;
    BOOL constant;
    BOOL array;
    BOOL isnull; // for handle
    struct {
        LPJASSDICT locals;
        DWORD returnstack;
        BOOL done;
    } env;
    LPJASSARRAY _array;
    LPSRCLOC location;
};

struct jass_type {
    LPCJASSTYPE inherit;
    LPJASSTYPE next;
    LPCSTR name;
};

struct jass_arg {
    LPJASSARG next;
    LPCJASSTYPE type;
    LPCSTR name;
};

struct jass_function {
    LPJASSARG args;
    LPCJASSTYPE returns;
    LPJASSFUNC next;
    LPCSTR name;
    LPCTOKEN code;
    DWORD (*nativefunc)(LPJASS j);
    BOOL constant;
};

struct jass_array {
    LPJASSARRAY next;
    DWORD index;
    JASSVAR value;
    LPSRCLOC location;
};

struct jass_dict {
    LPJASSDICT next;
    LPCSTR key;
    JASSVAR value;
};

struct jass_s {
    LPJASSDICT globals;
    LPJASSTYPE types;
    LPJASSFUNC functions;
    JASSVAR stack[MAX_JASS_STACK];
    DWORD num_stack;
    DWORD num_stack_valid;
    LPJASSVAR stack_pointer;
    JASSCONTEXT context;
    int thread_id;
    LPCSTR thread_name;
};

JASSTYPE jass_types[] = {
    { NULL, NULL, "integer" },
    { NULL, NULL, "real" },
    { NULL, NULL, "string" },
    { NULL, NULL, "boolean" },
    { NULL, NULL, "code" },
    { NULL, NULL, "handle" },
    { NULL, NULL, "cfunction" },
};

static LPJASSVAR jass_stackvalue(LPJASS j, int index);
static JASSTYPEID jass_getvarbasetype(LPCJASSVAR var);
static DWORD jass_dotoken(LPJASS j, LPCTOKEN token);

BOOL atob(LPCSTR str) {
    return !strcmp(str, "true");
}

DWORD __unm(LPJASS j) {
    if (jass_gettype(j, 1) == jasstype_integer) {
        return jass_pushinteger(j, -jass_checkinteger(j, 1));
    } else {
        return jass_pushnumber(j, -jass_checknumber(j, 1));
    }
}

JASS_NUMOP(__add, +);
JASS_NUMOP(__sub, -);
JASS_NUMOP(__mul, *);
JASS_NUMOP(__div, /);
JASS_CMPOP(__le, <=);
JASS_CMPOP(__ge, >=);
JASS_CMPOP(__gt, >);
JASS_CMPOP(__lt, <);

static BOOL var_eq(LPCJASSVAR a, LPCJASSVAR b) {
    if (jass_getvarbasetype(a) != jass_getvarbasetype(b)) {
        return false;
    }
    switch ((a->value == NULL) + (b->value == NULL)) {
        case 2: return true;
        case 1: return false;
    }
    switch (jass_getvarbasetype(a)) {
        case jasstype_integer: return !memcmp(a->value, b->value, sizeof(LONG));
        case jasstype_real: return !memcmp(a->value, b->value, sizeof(FLOAT));
        case jasstype_string: return !strcmp(a->value, b->value);
        case jasstype_boolean: return !memcmp(a->value, b->value, sizeof(BOOL));
        case jasstype_code: return !memcmp(a->value, b->value, sizeof(HANDLE));
        case jasstype_cfunction: return !memcmp(a->value, b->value, sizeof(HANDLE));
        case jasstype_handle: return a->value==b->value;// return !memcmp(a->value, b->value, sizeof(HANDLE));
    }
    return false;
}


DWORD __eq(LPJASS j) {
    return jass_pushboolean(j, var_eq(jass_stackvalue(j, 1), jass_stackvalue(j, 2)));
}

DWORD __ne(LPJASS j) {
    return jass_pushboolean(j, !var_eq(jass_stackvalue(j, 1), jass_stackvalue(j, 2)));
}

DWORD __and(LPJASS j) {
    return jass_pushboolean(j, jass_toboolean(j, 1) && jass_toboolean(j, 2));
}

DWORD __or(LPJASS j) {
    return jass_pushboolean(j, jass_toboolean(j, 1) || jass_toboolean(j, 2));
}

DWORD __not(LPJASS j) {
    return jass_pushboolean(j, !jass_toboolean(j, 1));
}

JASSMODULE jass_operators[] = {
    JASS_OPERATOR(__add, __FILE__, __LINE__),
    JASS_OPERATOR(__sub, __FILE__, __LINE__),
    JASS_OPERATOR(__mul, __FILE__, __LINE__),
    JASS_OPERATOR(__div, __FILE__, __LINE__),
    JASS_OPERATOR(__ne, __FILE__, __LINE__),
    JASS_OPERATOR(__eq, __FILE__, __LINE__),
    JASS_OPERATOR(__ge, __FILE__, __LINE__),
    JASS_OPERATOR(__le, __FILE__, __LINE__),
    JASS_OPERATOR(__gt, __FILE__, __LINE__),
    JASS_OPERATOR(__lt, __FILE__, __LINE__),
    JASS_OPERATOR(__and, __FILE__, __LINE__),
    JASS_OPERATOR(__or, __FILE__, __LINE__),
    JASS_OPERATOR(__unm, __FILE__, __LINE__),
    JASS_OPERATOR(__not, __FILE__, __LINE__),
    { NULL },
};

void removeDoubleBackslashes(LPSTR str) {
    size_t len = strlen(str);
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        str[j++] = str[i];
        if (str[i] == '\\' && str[i + 1] == '\\') {
            i++; // Skip the second backslash
        }
    }
    str[j] = '\0'; // Null-terminate the modified string
}

BOOL is_integer(LPCSTR tok) {
    LPSTR endptr;
    strtol(tok, &endptr, 10);
    return *endptr == '\0';
}

BOOL is_float(LPCSTR tok) {
    LPSTR endptr;
    strtod(tok, &endptr);
    return *endptr == '\0';
}

BOOL is_fourcc(LPCSTR tok) {
    return *tok == '\'';
}

BOOL is_string(LPCSTR tok) {
    return *tok == '\"';
}

BOOL is_identifier(LPCSTR str) {
    if (!isalpha(*str) && *str != '_')
        return false;
    for (LPCSTR s = str; *s; ++s) {
        if (!isalnum(*s) && *s != '_')
            return false;
    }
    for (LPCSTR *kw = keywords; *kw; kw++) {
        if (!strcmp(str, *kw)) {
            return false;
        }
    }
    return true;
}

BOOL is_comma(LPCSTR str) {
    return !strcmp(str, ",");
}

static HANDLE RunAction(HANDLE handle) {
    
    LPJASS j = handle;
    prctl(PR_SET_NAME, j->thread_name);
    LPSRCLOC loc = gi.MemAlloc(sizeof(SRCLOC));
    loc->file = __FILE__;
    loc->line = __LINE__;
    loc->column = 0;

    jass_pushfunction(j, j->context.func,loc);
    jass_call(j, 0);

    if(j->context.func2) {
        jass_pushfunction(j, j->context.func2,loc);
        jass_call(j, 0);
    }
    gi.MemFree(handle);
    return NULL;
}

LPCJASSCONTEXT jass_getcontext(LPJASS j) {
    return &j->context;
}
void jass_settriggeringtrigger(LPJASS j, LPTRIGGER trigger) {
    j->context.trigger = trigger;
}

void jass_startthread(LPJASS j, LPCJASSCONTEXT context,LPCSTR thName) {
    static int thread_id = 0;
    LPJASS thread = jass_newstate();
    memcpy(thread, j, sizeof(JASS));
    memset(thread->stack, 0, sizeof(thread->stack));
    thread->stack_pointer = thread->stack;
    thread->num_stack = 0;
    thread->num_stack_valid=0;
    thread->context = *context;
    thread->thread_id = thread_id++;
    char text[1024] = { 0 };
    sprintf(text,"%s at JT%d",thName,thread->thread_id);
    thread->thread_name = strdup(text);
    DWORD id = gi.CreateThread(RunAction, thread);
}
BOOL jass_calltrigger(LPJASS j, LPTRIGGER trigger, LPEDICT unit) {
    if (trigger->disabled)
        return false;
    if (jass_evaluatetrigger(j, trigger, unit)) {
        jass_executetrigger(j, trigger, unit);
        return true;
    } else {
        return false;
    }
}
void jass_executetrigger(LPJASS j, LPTRIGGER trigger, LPEDICT unit) {
    FOR_EACH_LIST(TRIGGERACTION, action, trigger->actions) {
        LPPLAYER player = unit ? G_GetPlayerByNumber(unit->s.player) : NULL;
        jass_startthread(j, &MAKE(JASSCONTEXT,
                                  .trigger = trigger,
                                  .func = action->func,
                                  .unit = unit,
                                  .playerState = player,
                              ),"jass_executetrigger");
    }
}
BOOL jass_evaluatetrigger(LPJASS j, LPTRIGGER trigger, LPEDICT unit) {
    if (trigger->disabled)
        return false;
    JASS tmp_state;
    FOR_EACH_LIST(TRIGGERCONDITION, cond, trigger->conditions) {
        memcpy(&tmp_state, j, sizeof(struct jass_s));
        memset(tmp_state.stack, 0, sizeof(tmp_state.stack));
        tmp_state.num_stack = 0;
        tmp_state.num_stack_valid = 0;
        tmp_state.thread_id = j->thread_id;
        tmp_state.context.trigger = trigger;
        tmp_state.context.unit = unit;
        jass_pushfunction(&tmp_state, cond->expr,NULL);
        if (jass_call(&tmp_state, 0) != 1 || !jass_popboolean(&tmp_state)) {
            return false;
        }
    }
}

void jass_updatetimer(LPJASS j, FLOAT frame_time) {
    LPSRCLOC loc = gi.MemAlloc(sizeof(SRCLOC));
    loc->file = __FILE__;
    loc->line = __LINE__;
    loc->column = 0;
    for (DWORD i = 0; i < num_timers; i++) {
        if(timers[i]->destroyed)
            continue;
        if (!timers[i]->destroyed && timers[i]->active && !timers[i]->paused) {
            timers[i]->elapsed += frame_time;
            timers[i]->remaining -= frame_time;
            if (timers[i]->remaining <= 0.0f) {
                if (timers[i]->handlerFunc != NULL) {
                    jass_pushfunction(j, timers[i]->handlerFunc,loc);
                    jass_call(j, 0);
                }
                if (timers[i]->periodic) {
                    timers[i]->remaining += timers[i]->timeout;
                } else {
                    timers[i]->active = false;
                }
            }
        }
    }
}

static LPCJASSMODULE find_cfunction(LPCJASS j, LPCSTR name) {
    for (LPCJASSMODULE m = jass_operators; m->name; m++) {
        if (!strcmp(m->name, name)) {
            return m;
        }
    }
    return NULL;
}

LPCJASSFUNC find_function(LPCJASS j, LPCSTR name) {
    FOR_EACH_LIST(JASSFUNC, func, j->functions) {
        if (!strcmp(func->name, name)) {
            return func;
        }
    }
    return NULL;
}

static LPJASSVAR find_dict(LPJASSDICT dict, LPCSTR name) {
    FOR_EACH_LIST(JASSDICT, item, dict) {
        if (!strcmp(item->key, name)) {
            return &item->value;
        }
    }
    return NULL;
}

static LPCJASSTYPE find_type(LPCJASS j, LPCSTR name) {
    FOR_LOOP(i, sizeof(jass_types)/sizeof(*jass_types)) {
        if (!strcmp(jass_types[i].name, name)) {
            return &jass_types[i];
        }
    }
    FOR_EACH_LIST(JASSTYPE, type, j->types) {
        if (!strcmp(type->name, name)) {
            return type;
        }
    }
    return NULL;
}

LPCJASSTYPE get_base_type(LPCJASSTYPE type) {
    while (type->inherit) {
        type = type->inherit;
    }
    return type;
}

void jass_setreturn(LPJASS j) {
    jass_stackvalue(j, 0)->env.done = true;
    jass_stackvalue(j, 0)->env.returnstack = j->num_stack;
    
}

BOOL jass_mustreturn(LPJASS j) {
    return jass_stackvalue(j, 0)->env.done;
}

DWORD jass_top(LPJASS j) {
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack !=0);
    return j->num_stack-1;
}

LPJASSVAR jass_topvalue(LPJASS j) {
    return j->stack+jass_top(j);
}

LPJASSVAR jass_stackvalue(LPJASS j, int index) {
    if (index < 0) {
        return j->stack + (j->num_stack + index);
    } else {
        return j->stack_pointer + index;
    }
}

JASSTYPEID jass_getvarbasetype(LPCJASSVAR var) {
    return (JASSTYPEID)(get_base_type(var->type) - jass_types);
}

JASSTYPEID jass_gettype(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    return jass_getvarbasetype(var);
}

BOOL jass_checktype(LPCJASSVAR var, JASSTYPEID type) {
    return get_base_type(var->type) == jass_types+type;
}
BOOL jass_checktype2(LPCJASSVAR var, LPCSTR type) {
    return var->type->name && !strcmp(var->type->name, type);
}

void jass_pop(LPJASS j, DWORD count) {
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=count);
    j->num_stack -= count;
}

static void jass_setnull(LPJASSVAR var);

static int dict_depth = 0;

static void jass_deletedict(LPJASSDICT dict) {
    dict_depth++;
    // fprintf(stdout, "jass_deletedict: %s\n", dict->key);
    SAFE_DELETE(dict->next, jass_deletedict);
    jass_setnull(&dict->value);
    gi.MemFree(dict);
}
void jass_setnull(LPJASSVAR var) {
    dict_depth =0;
    SAFE_DELETE(var->env.locals, jass_deletedict);
    switch (jass_getvarbasetype(var)) {
        case jasstype_handle:
            if (var->refcount && *var->refcount > 0) {
                (*var->refcount)--;
                var->value = NULL;
                var->location = NULL;
                var->refcount = NULL;
                if(var->value) {
#ifdef DEBUG_JASS
                INDENT(depth);
                fprintf(stdout, "DecRefCount of handle(%p) to %d\n", var->value, var->refcount ? *var->refcount : 0);
                    #endif
                }
            } else if(jass_checktype2(var,JASS_TIMER)) {
                // skip
            }
            else{
                if(var->value){
#ifdef DEBUG_JASS
                INDENT(depth);
                fprintf(stdout, "Destroy handle(%p) due to %d refcount!\n", var->value, var->refcount ? *var->refcount : 0);
                #endif
SAFE_DELETE(var->value, gi.MemFree);
                }
                SAFE_DELETE(var->location, gi.MemFree);
                SAFE_DELETE(var->refcount, gi.MemFree);
            }
            break;
        case jasstype_code:
        case jasstype_cfunction:
            // skip
            break;
        default:
            SAFE_DELETE(var->value, gi.MemFree);
            break;
    }
}

//void jass_setenv(LPJASS j, int index, LPJASSDICT env) {
//    jass_stackvalue(j, index)->env.locals = env;
//}

BOOL is_handle_convertible(LPCJASSTYPE from, LPCJASSTYPE to) {
    if (from == to) {
        return true;
    } else if (from->inherit) {
        return is_handle_convertible(from->inherit, to);
    } else {
        return false;
    }
}

static LPJASSVAR ensure_array_value(LPJASS j, LPJASSVAR dest, DWORD index,LPSRCLOC location) {
    FOR_EACH_LIST(JASSARRAY, var, dest->_array) {
        if (var->index == index) {
            return &var->value;
        }
    }
    LPJASSARRAY jv = JASSALLOC(JASSARRAY);
    jv->value.type = dest->type;
    jv->index = index;
    jv->location = JASSALLOC(SRCLOC);
    jv->location->file = location->file;
    jv->location->line = location->line;
    jv->location->column = location->column;
    ADD_TO_LIST(jv, dest->_array);
    return &jv->value;
}
__attribute__((optimize("O0")))
void jass_copy(LPJASS j, LPJASSVAR var, LPCJASSVAR other) {
    FLOAT fval = 0;
    jass_setnull(var);
    
    if (other->_array) {
        var->type = other->type;
        FOR_EACH_LIST(JASSARRAY, srcar, other->_array) {
            jass_copy(j, ensure_array_value(j, var, srcar->index,srcar->location), &srcar->value);
        }
        
        return;
    } else if (!other->value) {
        return;
    } else {
        switch (jass_getvarbasetype(var)) {
            case jasstype_integer:
                assert(other->type == var->type);
                JASS_SET_VALUE(var, other->value, sizeof(LONG));
                break;
            case jasstype_handle:
                if (!is_handle_convertible(other->type, var->type)) {
                    fprintf(stderr, "Warning: Passing %s to %s type (at %s:%d:%d)\n", 
                    other->type->name, var->type->name, 
                    var->location->file ? var->location->file : "unknown", 
                    var->location->line,
                    var->location->column);
                }
                var->value = other->value;
                var->refcount = other->refcount;
                var->isnull = other->isnull;
                if (var->refcount) {
                    (*var->refcount)++;
                    #ifdef DEBUG_JASS
                    INDENT(depth);
                    fprintf(stdout, "(AddRefCount of handle(%p) to %d)\n", var->value, var->refcount ? *var->refcount : 0);
                    #endif
                }
                break;
            case jasstype_real:
                switch (jass_getvarbasetype(other)) {
                    case jasstype_real:
                        fval = *(FLOAT const *)other->value;
                        break;
                    case jasstype_integer:
                        fval = *(LONG const *)other->value;
                        break;
                    default:
                        assert(false);
                        return;
                }
                JASS_SET_VALUE(var, &fval, sizeof(FLOAT));
                break;
            case jasstype_boolean:
                assert(other->type == var->type);
                JASS_SET_VALUE(var, other->value, sizeof(BOOL));
                break;
            case jasstype_string:
                assert(other->type == var->type);
                JASS_SET_VALUE(var, other->value, strlen(other->value)+1);
                break;
            default:
                assert(false);
                break;
        }
    }

    JASS_CP_SRCLOC(var->location, other->location);
}

DWORD jass_pushnull(LPJASS j) {
    JASS_ADD_STACK(j, var, jasstype_handle);
    var->isnull = true;
    return 1;
}

DWORD jass_pushinteger(LPJASS j, LONG value) {
    JASS_ADD_STACK(j, var, jasstype_integer);
    JASS_SET_VALUE(var, &value, sizeof(value));
    return 1;
}

DWORD jass_pushhandle(LPJASS j, HANDLE value, LPCSTR type) {
    JASS_ADD_STACK(j, var, jasstype_handle);
    jass_setnull(var);
    var->type = find_type(j, type);
    if (value) {
        var->value = value;
        var->refcount = gi.MemAlloc(sizeof(DWORD));
#ifdef DEBUG_JASS
        INDENT(depth);
        fprintf(stdout, "SetRefCount of handle(%p) to %d\n", var->value, *var->refcount);
#endif
    }
    else{
        var->isnull = true;
    }
    return 1;
}

DWORD jass_pushnullhandle(LPJASS j, LPCSTR type) {
    return jass_pushhandle(j, 0, type);
}

HANDLE jass_newhandle(LPJASS j, DWORD size, LPCSTR type) {
    HANDLE data = size ? gi.MemAlloc(size) : NULL;
    jass_pushhandle(j, data, type);
    return data;
}

DWORD jass_pushlighthandle(LPJASS j, HANDLE value, LPCSTR type) {
    JASS_ADD_STACK(j, var, jasstype_handle);
    jass_setnull(var);
    var->type = find_type(j, type);
    var->value = value;
    var->refcount = gi.MemAlloc(sizeof(DWORD));
    *var->refcount = 1; // so that runtime won't ever delete it
    #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "SetRefCount of handle(%p) to %d\n", var->value, *var->refcount);
    #endif
    return 1;
}

DWORD jass_pushnumber(LPJASS j, FLOAT value) {
    JASS_ADD_STACK(j, var, jasstype_real);
    JASS_SET_VALUE(var, &value, sizeof(value));
    return 1;
}

DWORD jass_pushboolean(LPJASS j, BOOL value) {
    JASS_ADD_STACK(j, var, jasstype_boolean);
    JASS_SET_VALUE(var, &value, sizeof(value));
    return 1;
}

DWORD jass_pushstringlen(LPJASS j, LPCSTR value, DWORD len) {
    JASS_ADD_STACK(j, var, jasstype_string);
    JASS_SET_VALUE(var, value, len+1);
    ((LPSTR)var->value)[len] = '\0';
    removeDoubleBackslashes(var->value);
    return 1;
}

DWORD jass_pushstring(LPJASS j, LPCSTR value) {
    jass_pushstringlen(j, value, (DWORD)strlen(value));
    return 1;
}

DWORD jass_pushcfunction(LPJASS j, LPJASSCFUNCTION func,LPSRCLOC location) {
    JASS_ADD_STACK(j, var, jasstype_cfunction);
    JASS_SET_VALUE(var, &func, sizeof(LPJASSCFUNCTION));
    JASS_CP_SRCLOC(var->location, location);
    return 1;
}
__attribute__((optimize("O0")))
DWORD jass_pushfunction(LPJASS j, LPCJASSFUNC func, LPSRCLOC location) {
    if (func->nativefunc) {
        return jass_pushcfunction(j, func->nativefunc,location);
    } else {
        JASS_ADD_STACK(j, var, jasstype_code);
        var->value = (LPJASSFUNC)func;
         JASS_CP_SRCLOC(var->location, location);
        return 1;
    }
}

DWORD jass_pushvalue(LPJASS j, LPCJASSVAR other) {
    LPCJASSTYPE type = other->type;
    LPJASSVAR var = &j->stack[j->num_stack++];
    memset(var, 0, sizeof(*var));
    var->type = type;
    jass_copy(j, var, other);
    return 1;
}
LPCSTR jass_dumploc(LPSRCLOC loc) {
    static char buffer[1024];
    if (!loc || !loc->file) {
        return "unknown";
    }
    sprintf(buffer, "%s:%d:%d", loc->file, loc->line, loc->column);
    return buffer;
}
LPCSTR jass_dumpvar(LPCJASSVAR var) {
    static char buffer[1024];
    if (var->value == NULL) {
        return "null";
    }
    switch (jass_getvarbasetype(var)) {
        case jasstype_integer:
            sprintf(buffer, "%d", *(LONG *)var->value);
            break;
        case jasstype_real:
            sprintf(buffer, "%f", *(FLOAT *)var->value);
            break;
        case jasstype_string:
            sprintf(buffer, "\"%s\"", (char *)var->value);
            break;
        case jasstype_boolean:
            sprintf(buffer, "%s", *(BOOL *)var->value ? "true" : "false");
            break;
        case jasstype_code:
            sprintf(buffer, "code(%p)", var->value);
            break;
        case jasstype_handle:
            sprintf(buffer, "handle(%p) ref:%d", var->value, var->refcount ? *var->refcount : 0);
            break;
            case jasstype_cfunction:
            sprintf(buffer, "cfunction(%p)", *(LPJASSCFUNCTION *)var->value);
            break;
        default:
            sprintf(buffer, "unknown_jass_value(%p)", var->value);
    }
    return buffer;
}

LONG jass_checkinteger(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    assert_type(var, jasstype_integer);
    return var->value ? *(LONG *)var->value : 0;
}

FLOAT jass_checknumber(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    if (jass_checktype(var, jasstype_real)) {
        return var->value ? *(FLOAT *)var->value : 0;
    }
    if (jass_checktype(var, jasstype_integer)) {
        return var->value ? *(LONG *)var->value : 0;
    }
    assert(false);
}

BOOL jass_checkboolean(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    assert_type(var, jasstype_boolean);
    return var->value ? *(BOOL *)var->value : 0;
}

BOOL jass_toboolean(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    JASSTYPEID type = jass_getvarbasetype(var);
    if (var->value == NULL)
        return false;
    switch (type) {
        case jasstype_integer: return *(LONG *)var->value != 0;
        case jasstype_real: return *(FLOAT *)var->value != 0;
        case jasstype_string: return strlen(var->value) > 0;
        case jasstype_boolean: return *(BOOL *)var->value != 0;
        case jasstype_handle: return true;
        case jasstype_code: return true;
        default: return false;
    }
}

LPCSTR jass_checkstring(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    assert_type(var, jasstype_string);
    return var->value;
}

LPCJASSFUNC jass_checkcode(LPJASS j, int index) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    if(!var->isnull)
        assert_type(var, jasstype_code);
    return var->value;
}

HANDLE jass_checkhandle(LPJASS j, int index, LPCSTR type) {
    LPCJASSVAR var = jass_stackvalue(j, index);
    if(!var->isnull)
        assert(is_handle_convertible(var->type, find_type(j, type)));
    return var->value;
}

void jass_swap(LPJASS j, int a, int b) {
    JASSVAR tmp = *jass_stackvalue(j, a);
    *jass_stackvalue(j, a) = *jass_stackvalue(j, b);
    *jass_stackvalue(j, -2) = tmp;
}

BOOL jass_popboolean(LPJASS j) {
    BOOL value = jass_toboolean(j, -1);
    jass_pop(j, 1);
    return value;
}

DWORD jass_popinteger(LPJASS j) {
    DWORD value = jass_checkinteger(j, -1);
    jass_pop(j, 1);
    return value;
}

DWORD VM_EvalInteger(LPJASS j, LPCTOKEN token) {
        #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "push i.%d\n", atoi(token->primary));
#endif
    return jass_pushinteger(j, atoi(token->primary));
}

DWORD VM_EvalReal(LPJASS j, LPCTOKEN token) {
        #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "push real.%f\n", atof(token->primary));
#endif
    return jass_pushnumber(j, atof(token->primary));
}

DWORD VM_EvalString(LPJASS j, LPCTOKEN token) {   
     #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "push string.%s\n", token->primary);
#endif
    return jass_pushstring(j, token->primary);
}

DWORD VM_EvalBoolean(LPJASS j, LPCTOKEN token) {
        #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "push boolean.%s\n",token->primary);
#endif
    return jass_pushboolean(j, atob(token->primary));
}

__attribute__((optimize("O0")))
DWORD VM_EvalIdentifier(LPJASS j, LPCTOKEN token) {
    LPCJASSFUNC f = NULL;
    LPCJASSVAR v = NULL;
    if (token->flags & TF_FUNCTION) {
        if ((f = find_function(j, token->primary))) {
            return jass_pushfunction(j, f,token->location);
        } else {
#ifdef DEBUG_JASS
            fprintf(stderr, "WARN: Not a function? %s\n", token->primary);
#endif
            return jass_pushnull(j);
        }
    } else if ((v = find_dict(j->globals, token->primary))) {
#ifdef DEBUG_JASS
            INDENT(depth);
            fprintf(stdout, "push %s: %s\n",token->primary, jass_dumpvar(v));
#endif
        return jass_pushvalue(j, v);
    } else if ((v = find_dict(jass_stackvalue(j, 0)->env.locals, token->primary))) {
#ifdef DEBUG_JASS
            INDENT(depth);
            fprintf(stdout, "push %s: %s\n",token->primary, jass_dumpvar(v));
#endif
        return jass_pushvalue(j, v);
    } else {
#ifdef DEBUG_JASS
 INDENT(depth);
            fprintf(stdout, "push null\n");
#endif
        return jass_pushnull(j);
    }
}

DWORD VM_EvalArrayAccess(LPJASS j, LPCTOKEN token) {
    assert(jass_dotoken(j, token->index) == 1);
    DWORD index_val = jass_popinteger(j);
   
    VM_EvalIdentifier(j, token);
    LPJASSVAR var = jass_stackvalue(j, -1);
#ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "ArrayAccess: [%d]\n", index_val);
#endif
    LPJASSVAR item = ensure_array_value(j, var, index_val, token->location);
    jass_pop(j, 1);
    JASSVAR tmp;
    memcpy(&tmp, item, sizeof(JASSVAR));
#ifdef DEBUG_JASS
    INDENT(depth);
            fprintf(stdout, "push %s: %s\n",token->primary, jass_dumpvar(&tmp));
#endif
    jass_pushvalue(j, &tmp);
    return 1;
}

DWORD VM_EvalFourCC(LPJASS j, LPCTOKEN token) {
    #ifdef DEBUG_JASS
     INDENT(depth);
    fprintf(stdout, "push fourcc.%d\n", atoi(token->primary));
#endif
    return jass_pushinteger(j, *(DWORD *)token->primary);
}

DWORD VM_EvalCall(LPJASS j, LPCTOKEN token) {
    LPCJASSFUNC f = NULL;
    LPCJASSMODULE model = NULL;
    
    DWORD stacksize = j->num_stack;
    LPSRCLOC location = token->location;
    if (!strcmp(token->primary, "CommentString") && token->args) {
        fprintf(stdout, "CommentString: %s\n", token->args->primary);
        return 0;
    } else if ((f = find_function(j, token->primary))) {
        DWORD args = 0;
        jass_pushfunction(j, f,token->location);
        FOR_EACH_LIST(TOKEN, arg, token->args) {
            jass_dotoken(j, arg);
            args++;
        }
        jass_call(j, args);
        assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=stacksize);
        return j->num_stack - stacksize;
    } else if ((model = find_cfunction(j, token->primary))) {
        DWORD args = 0;
        jass_pushcfunction(j, model->func,token->location);
        FOR_EACH_LIST(TOKEN, arg, token->args) {
            jass_dotoken(j, arg);
            args++;
        }
        jass_call(j, args);
        assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=stacksize);
        return j->num_stack - stacksize;
    } else {
        fprintf(stdout, "Can't find function %s\n", token->primary);
        assert(false);
        return 0;
    }
}

static struct {
    TOKENTYPE tokentype;
    DWORD (*func)(LPJASS j, LPCTOKEN token);
} token_types[] = {
    { TT_INTEGER, VM_EvalInteger },
    { TT_REAL, VM_EvalReal },
    { TT_STRING, VM_EvalString },
    { TT_BOOLEAN, VM_EvalBoolean },
    { TT_IDENTIFIER, VM_EvalIdentifier },
    { TT_ARRAYACCESS, VM_EvalArrayAccess },
    { TT_FOURCC, VM_EvalFourCC },
    { TT_CALL, VM_EvalCall },
};

DWORD jass_dotoken(LPJASS j, LPCTOKEN token) {
    if (!token)
        return 0;
    FOR_LOOP(idx, sizeof(token_types)/sizeof(*token_types)) {
        if (token_types[idx].tokentype == token->type) {
            return token_types[idx].func(j, token);
        }
    }
    assert(false);
    return 0;
}

static void jass_set_value(LPJASS j, LPJASSVAR dest, LPCTOKEN init) {
    DWORD stack = jass_dotoken(j, init);
    assert(stack == 1); // likely init points to undefined, check what's inside
    jass_copy(j, dest, j->stack + jass_top(j));
    jass_pop(j, 1);
}

__attribute__((optimize("O0")))
static void jass_set_array_value(LPJASS j, LPJASSVAR dest, LPCTOKEN index, LPCTOKEN init) {
    assert(jass_dotoken(j, index) == 1);
    DWORD index_val = jass_popinteger(j);
    LPJASSVAR index_dest = ensure_array_value(j, dest, index_val,index->location);
    assert(jass_dotoken(j, init) == 1);
    jass_copy(j, index_dest, j->stack + jass_top(j));
    jass_pop(j, 1);
}

LPJASSDICT parse_dict(LPJASS j, LPCTOKEN token) {
    LPJASSDICT item = JASSALLOC(JASSDICT);
    item->value.constant = token->flags & TF_CONSTANT;
    item->value.array = token->flags & TF_ARRAY;
    item->value.type = find_type(j, token->primary);
    item->key = token->secondary;
    JASS_CP_SRCLOC(item->value.location, token->location);
#ifdef DEBUG_JASS
INDENT(depth);
        fprintf(stdout, "decl: %s %s\n", token->primary, item->key);
#endif
    if (token->init) {
        jass_set_value(j, &item->value, token->init);
    }
    return item;
}

#define TOKENFUNC(NAME) __attribute__((optimize("O0"))) void eval_##NAME(LPJASS j, LPCTOKEN token)
#define TOKENEVAL(NAME) { #NAME, TT_##NAME, eval_##NAME }

TOKENFUNC(TOKENS);
TOKENFUNC(SINGLETOKEN);

TOKENFUNC(TYPEDEF) {
    LPJASSTYPE type = JASSALLOC(JASSTYPE);
    type->name = token->primary;
    type->inherit = find_type(j, token->secondary);
    ADD_TO_LIST(type, j->types);
    #ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "typedef %s inherits %s\n", type->name, type->inherit ? type->inherit->name : "null");
    #endif
}

BOOL uses_localplayer(LPCTOKEN token) {
    if (token->type == TT_CALL && !strcmp(token->primary, "GetLocalPlayer")) {
        return true;
    }
    FOR_EACH_LIST(TOKEN, arg, token->args) {
        if (uses_localplayer(arg)) {
            return true;
        }
    }
    return false;
}

TOKENFUNC(IF) {
    if (uses_localplayer(token->condition)) {
        FOR_LOOP(i, MAX_PLAYERS) {
            currentplayer = G_GetPlayerByNumber(i);
            jass_dotoken(j, token->condition);
            if (jass_popboolean(j)) {
                eval_TOKENS(j, token->body);
            }
            currentplayer = NULL;
        }
        assert(!token->elseblock);
    } else while (token) {
        if (!token->condition) {
            eval_TOKENS(j, token->body);
            return;
        }
        jass_dotoken(j, token->condition);
        if (jass_popboolean(j)) {
            eval_TOKENS(j, token->body);
            return;
        }
        token = token->elseblock;
    }
}

TOKENFUNC(SET) {
    LPJASSVAR v = NULL;
    if ((v = find_dict(j->globals, token->secondary))) {
        if (token->index) {
            return jass_set_array_value(j, v, token->index, token->init);
        } else {
            return jass_set_value(j, v, token->init);
        }
    } else if ((v = find_dict(jass_stackvalue(j, 0)->env.locals, token->secondary))) {
        if (token->index) {
            return jass_set_array_value(j, v, token->index, token->init);
        } else {
            return jass_set_value(j, v, token->init);
        }
    } else {
        fprintf(stdout, "Can't find variable %s\n", token->primary);
    }
}

TOKENFUNC(VARDECL) {
    LPJASSDICT vardecl = parse_dict(j, token);
    ADD_TO_LIST(vardecl, jass_stackvalue(j, 0)->env.locals);
}

TOKENFUNC(GLOBAL) {
    LPJASSDICT global = parse_dict(j, token);
    ADD_TO_LIST(global, j->globals);
}

TOKENFUNC(FUNCTION) {
    LPJASSFUNC func = JASSALLOC(JASSFUNC);
    func->name = token->primary;
    func->code = token->body;
    func->returns = find_type(j, token->secondary);
    FOR_EACH_LIST(TOKEN, arg, token->args) {
        LPJASSARG jarg = JASSALLOC(JASSARG);
        jarg->name = arg->secondary;
        jarg->type = find_type(j, arg->primary);
        PUSH_BACK(JASSARG, jarg, func->args);
    }
    if (token->flags & TF_NATIVE) {
        LPJASSMODULE mod = find_in_array(jass_funcs, sizeof(JASSMODULE), func->name);
        if (mod) {
            func->nativefunc = mod->func;
        }
    }
    ADD_TO_LIST(func, j->functions);
}

TOKENFUNC(CALL) {
    jass_dotoken(j, token);
}

TOKENFUNC(LOOP) {
    for (DWORD i = 0;; i++) {
        FOR_EACH_LIST(TOKEN const, tok, token->body) {
            if (jass_mustreturn(j)) {
                return;
            } else if (token->type == TT_RETURN) {
#ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "prepare return value:\n");
#endif
                jass_setreturn(j);
                jass_dotoken(j, token->body);
#ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "return.\n");
#endif
                return;
            } else if (tok->type == TT_EXITWHEN) {
                jass_dotoken(j, tok->condition);
                if (jass_popboolean(j)) {
                    return;
                }
            } else {
                eval_SINGLETOKEN(j, tok);
            }
        }
        assert(i < INF_LOOP_PROTECTION);
    }
}

struct {
    LPCSTR name;
    TOKENTYPE type;
    void (*func)(LPJASS, LPCTOKEN);
} token_eval[] = {
    TOKENEVAL(TYPEDEF),
    TOKENEVAL(FUNCTION),
    TOKENEVAL(VARDECL),
    TOKENEVAL(GLOBAL),
    TOKENEVAL(CALL),
    TOKENEVAL(IF),
    TOKENEVAL(SET),
    TOKENEVAL(LOOP),
};

TOKENFUNC(SINGLETOKEN) {
    FOR_LOOP(index, sizeof(token_eval) / sizeof(*token_eval)) {
        if (token->type == token_eval[index].type) {
            token_eval[index].func(j, token);
            return;
        }
    }
    fprintf(stdout, "Can't evaluate token of type %d\n", token->type);
    assert(false);
}

TOKENFUNC(TOKENS) {
    FOR_EACH_LIST(TOKEN const, tok, token) {
        if (jass_mustreturn(j)) {
            return;
        } else if (tok->type == TT_RETURN) {
#ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "prepare return value:\n");
#endif
                jass_setreturn(j);
                jass_dotoken(j, tok->body);
#ifdef DEBUG_JASS
    INDENT(depth);
    fprintf(stdout, "return.\n");
#endif
        } else {
            eval_SINGLETOKEN(j, tok);
        }
    }
}

BOOL jass_dobuffer(LPJASS j, LPSTR buffer2,LPSTR fileName) {
    LPSTR buffer = buffer2;
    gi.TextRemoveComments(buffer);
    gi.TextRemoveBom(buffer);
    LPSRCLOC location= JASSALLOC(SRCLOC);
    location->file = fileName;
    location->line = 1;
    location->column = 1;
    LPTOKEN program = JASS_ParseTokens(&MAKE(PARSER, 
        .buffer = buffer, 
        .delimiters = JASS_DELIM,
        .location = location));
//    VM_Compile(program);
    eval_TOKENS(j, program);
    return true;
}

LPJASS jass_newstate(void) {
    LPJASS j = JASSALLOC(JASS);
    j->stack_pointer = j->stack;
    return j;
}

void jass_close(LPJASS j) {
    gi.MemFree(j);
}

BOOL jass_dofile(LPJASS j, LPCSTR fileName) {
    LPSTR buffer = gi.ReadFileIntoString(fileName);
    if (buffer) {
        BOOL success = jass_dobuffer(j, buffer,strdup( fileName));
        gi.MemFree(buffer);
        return success;
    } else {
        return false;
    }
}

//BOOL jass_dofilenative(LPJASS j, LPCSTR fileName) {
//    FILE *file = fopen(fileName, "rb");
//    if (file == NULL) {
//        fprintf(stderr, "Error opening the file.\n");
//        return false;
//    }
//    fseek(file, 0, SEEK_END);
//    long file_size = ftell(file);
//    fseek(file, 0, SEEK_SET);
//    LPSTR buffer = gi.MemAlloc(file_size + 1); // +1 for null-terminator
//    if (buffer == NULL) {
//        fprintf(stderr, "Memory allocation failed.\n");
//        fclose(file);
//        return false;
//    }
//    fread(buffer, 1, file_size, file);
//    buffer[file_size] = '\0';
//
//    fclose(file);
//
//    BOOL success = jass_dobuffer(j, fileName, buffer);
//
//    // Free the buffer memory
//    gi.MemFree(buffer);
//
//    return success;
//}

DWORD jass_call(LPJASS j, DWORD args) {
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=args+1);
    LPJASSVAR root = &j->stack[j->num_stack - args - 1];
    LPJASSVAR old_stack_pointer = j->stack_pointer;
    DWORD ret = 0;
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=args+1);
    j->stack_pointer = &j->stack[j->num_stack - args - 1];
    assert(j->num_stack <= MAX_JASS_STACK);
    j->num_stack_valid = j->num_stack;
#ifdef DEBUG_JASS
    callnum++;
    depth++;
    INDENT(depth);
    printf("call ");
#endif
    if (jass_getvarbasetype(root) == jasstype_cfunction) {
        LPJASSCFUNCTION func = *(LPJASSCFUNCTION *)root->value;
#ifdef DEBUG_JASS
        for (DWORD i = 0; jass_funcs[i].name; i++) {
            if (jass_funcs[i].func == func) {
                printf("%s (native)", jass_funcs[i].name);
                break;
            }
        }
        for (DWORD i = 0; jass_operators[i].name; i++) {
            if (jass_operators[i].func == func) {
                printf("%s (native)", jass_operators[i].name);
                break;
            }
        }
        printf("%d.args at %s\n",args,jass_dumploc(root->location));
#endif
        assert(j->num_stack <= MAX_JASS_STACK);
        DWORD valid = j->num_stack;
        if(j->num_stack_valid==21845){
            ret = func(j);
        }
        else{
            ret = func(j);
        }
        assert(j->num_stack <= MAX_JASS_STACK);
    } else {
        LPCJASSFUNC func = root->value;
        LPJASSDICT locals = NULL;
        DWORD argnum = 1;

        FOR_EACH_LIST(JASSARG, arg, func->args) {
            LPJASSDICT local = JASSALLOC(JASSDICT);
            local->key = arg->name;
            local->value.type = arg->type;
            assert(j->num_stack <= MAX_JASS_STACK);
            jass_copy(j, &local->value, &j->stack_pointer[argnum]);
            assert(j->num_stack <= MAX_JASS_STACK);
            PUSH_BACK(JASSDICT, local, locals);
            argnum++;
        }
#ifdef DEBUG_JASS
        printf("%s %d.args at %s\n ", func->name,argnum-1,jass_dumploc(root->location));
#endif
        root->env.done = false;
        root->env.returnstack = -1;
        root->env.locals = locals;
        eval_TOKENS(j, func->code);
        if (root->env.returnstack != -1) {
            assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=root->env.returnstack);
            ret = j->num_stack - root->env.returnstack;
            assert(j->num_stack <= MAX_JASS_STACK);
        }
    }
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=ret);
    LPJASSVAR last = &j->stack[j->num_stack - ret];
    for (LPJASSVAR it = root; it < last; it++) jass_setnull(it);
    for (LPJASSVAR it = root; it < last; it++) it->isnull = true;
    memmove(root, last, ret * sizeof(JASSVAR));
    int delta = last - root;
    assert(j->num_stack <= MAX_JASS_STACK && j->num_stack >=0 && j->num_stack>=delta);
    j->num_stack -= last - root;
    j->stack_pointer = old_stack_pointer;
#ifdef DEBUG_JASS
    depth--;
#endif
    return ret;
}

void jass_callbyname(LPJASS j, LPCSTR name, BOOL async) {
    LPSRCLOC loc = gi.MemAlloc(sizeof(SRCLOC));
    loc->file = __FILE__;
    loc->line = __LINE__;
    loc->column = 0;
    LPCJASSFUNC func = find_function(j, name);
    if (!func) {
        fprintf(stderr, "Function not found %s\n", name);
        return;
    }
    if (async) {
        jass_startthread(j, &MAKE(JASSCONTEXT, .func = func,.func2 = NULL,.trigger = NULL),"jass_callbyname");
    } else {
        jass_pushfunction(j, func,loc);
        jass_call(j, 0);
    }
}
void jass_callbyname_sequenced_async(LPJASS j, LPCSTR name,LPCSTR name2) {
    LPCJASSFUNC func = find_function(j, name);
    if (!func) {
        fprintf(stderr, "Function not found %s\n", name);
        return;
    }
    LPCJASSFUNC func2 = find_function(j, name2);
    if (!func2) {
        fprintf(stderr, "Function not found %s\n", name2);
        return;
    }
    jass_startthread(j, &MAKE(JASSCONTEXT, .func = func,.func2 = func2,.trigger = NULL),"jass_callbyname2");
}
