#ifndef ini_parser_h
#define ini_parser_h

#include "../g_local.h"
#include "common/shared.h"
KNOWN_AS(init_token, TOKEN);
typedef enum {
    TT_UNKNOWN,
    TT_VALUE,
    TT_FUNCTION,
    TT_TYPEDEF,
    TT_VARDECL,
    TT_SECTION,
    TT_IDENTIFIER,
    TT_FIELD,
    TT_TEXT,
    TT_COMMENT,
    TT_INTEGER,
    TT_REAL,
    TT_STRING,
    TT_FOURCC,
    TT_BOOLEAN,
    TT_IF,
    TT_SET,
    TT_LOOP,
    TT_ELSE,
    TT_EXITWHEN,
    TT_RETURN,
} TOKENTYPE;

enum {
    TF_NATIVE = 1,
    TF_CONSTANT = 2,
    TF_ARRAY = 4,
    TF_FUNCTION = 8,
};

struct init_token {
    LPCSTR text;
    LPTOKEN field;
    TOKENTYPE type;
    LPSRCLOC location;
    DWORD flags;
    LPTOKEN next;
};

LPTOKEN INI_ParseTokens(LPPARSER p);

#endif
