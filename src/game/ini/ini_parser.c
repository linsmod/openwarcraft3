#include "ini_parser.h"
#include "common/shared.h"
#include "parser.h"
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define ALLOC(type) gi.MemAlloc(sizeof(type))
#define FREE(val) SAFE_DELETE(val, gi.MemFree)
#define PARSER(NAME, ...) static LPTOKEN INI_##NAME(LPPARSER p, ##__VA_ARGS__)

#define PARSER_THROW(...) \
fprintf(stderr, __VA_ARGS__); \
fprintf(stderr, "\n"); \
init_parser_throw(); \
longjmp(exception_env, 1);

static jmp_buf exception_env;
typedef LPTOKEN (*LPGRAMMARFUNC)(LPPARSER);

typedef struct {
    LPCSTR name;
    LPGRAMMARFUNC func;
} parseClass_t;

BOOL is_integer(LPCSTR tok);
BOOL is_float(LPCSTR tok);
BOOL is_identifier(LPCSTR str);
BOOL is_string(LPCSTR tok);

static void init_parser_throw(void) {
    assert(false);
}




static parseClass_t function_keywords[] = {
    // { "set", statement_set },
    { NULL },
};

static LPSTR read_identifier(LPPARSER p) {
    if (is_identifier(peek_token(p))) {
        return strdup(parse_token(p));
    } else {
        return NULL;
    }
}

static LPGRAMMARFUNC eat_keyword(LPPARSER p, parseClass_t *keywords) {
    for (parseClass_t *cl = keywords; cl->name; cl++) {
        if (eat_token(p, cl->name)) {
            return cl->func;
        }
    }
    return NULL;
}

LPTOKEN ini_alloc_token(TOKENTYPE type, LPPARSER p) {
    LPTOKEN token = ALLOC(TOKEN);
    token->type = type;
    token->location = ALLOC(SRCLOC);
    token->location->file = p->location->file ;
    token->location->line = p->location->line;
    token->location->column = p->location->column;
    return token;
}
static void remove_quotes(LPSTR str, char quote) {
    size_t len = strlen(str);
    if (len >= 2 && str[0] == quote && str[len - 1] == quote) {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

LPTOKEN alloc_ident_section(LPPARSER p) {
    LPTOKEN t = ini_alloc_token(TT_SECTION,p);
    t->text = strdup(parse_token(p));
    return t;
}

PARSER(section_start) {
    LPTOKEN sections = NULL;
    while (!eat_token(p, "]")) {
        LPTOKEN token = alloc_ident_section(p);
        token->text = read_identifier(p);
        if (eat_token(p, "array")) {
            token->flags |= TF_ARRAY;
        }
        PUSH_BACK(TOKEN, token, sections);
    }
    return sections;
}


static parseClass_t global_keywords[] = {
    { "[", INI_section_start },
    { NULL },
};
LPTOKEN cell_last;
static BOOL parse_field(LPPARSER p, LPTOKEN token){
    LPTOKEN field = ini_alloc_token(TT_FIELD, p);
    field->text = read_identifier(p);
    if(!field->text){
        return 1;
    }
    printf(" %s %s\n",field->text,parser_sline(p));
    if(eat_token(p,"=") && p->location->line==field->location->line){
        LPTOKEN cell = ini_alloc_token(TT_TEXT, p);
        cell->text = parse_token_dup(p);
        cell_last= cell;
        if(is_integer(cell->text)){
            cell->type = TT_INTEGER;
        }
        else if(is_float(cell->text)){
            cell->type= TT_REAL;
        }
        assert(cell->text);
        PUSH_BACK(TOKEN, cell, field->field);

        while(eat_token(p, ",")||eat_token(p, ";")){
            cell = ini_alloc_token(TT_TEXT, p);
            cell->text = parse_token_dup(p);
            PUSH_BACK(TOKEN, cell, field->field);
        }
        LPCSTR comment = read_inlinecomment(p);
        if(comment){
            cell = ini_alloc_token(TT_COMMENT, p);
            cell->text = comment;
            PUSH_BACK(TOKEN, cell, field->field);
        }
    }
    PUSH_BACK(TOKEN, field, token->field);
    return field->text!=NULL;
}

LPTOKEN INI_ParseTokens(LPPARSER p) {
    LPTOKEN tokens = NULL;
    if (setjmp(exception_env) == 0) {
        while (*p->buffer && eat_token(p,"[")) {
            LPTOKEN section = ini_alloc_token(TT_SECTION, p);
            section->text = read_identifier(p);
            PUSH_BACK(TOKEN, section, tokens);
            fprintf(stdout, "[%s] at %s\n",section->text,parser_sline(p));
            if(eat_token(p, "]")){
               read_inlinecomment(p);
               while (*p->buffer && !peek_token_is(p,"[")) {
                    if(parse_field(p,section)){
                        read_inlinecomment(p);
                        continue;
                    }
                    else{
                        continue;
                    }
                    
                }
            }
            else{
                PARSER_THROW("bad syntax");
            }
        }
        return tokens;
    } else {
        FREE(tokens);
        fprintf(stderr, "Parser Error\n");
        return NULL;
    }
}
