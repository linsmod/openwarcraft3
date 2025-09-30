#include "parser.h"
#include "common/shared.h"
#include "g_local.h"
#include "libs/mystr.h"
#include <string.h>
#define MAX_SEGMENT_SIZE 1024

BOOL eat_token(LPPARSER p, LPCSTR value) {
    LPCSTR tok = peek_token(p);
    if(!tok)
        return false;
    if (!strcmp(tok, value)) {
        parse_token(p);
        return true;
    } else {
        return false;
    }
}

void skipWs(LPPARSER p){
    while (isspace(*p->buffer))
            ++p->buffer;
}

void setln(LPPARSER p, LPCSTR start) {
    if (!p || !p->buffer || !start) return;
    
    skipWs(p);
    // 直接遍历从 buffer 到 start 的字符
    const char* current = start+1;
    const char* end = p->buffer;
    
    while (current < end && *current != '\0') {
        if (*current == '\n') {
            p->location->line++;
            p->location->column = 1;
        } else {
            p->location->column++;
        }
        current++;
    }
}

LPCSTR read_inlinecomment(LPPARSER p) {
    LPCSTR start = p->buffer;
    size_t segmentLength = 0;
    if (*p->buffer == '/' && *(p->buffer+1) == '/') {
        p->buffer+=2;
        mstr_t* str = mstr_new_with_capacity(MAX_SEGMENT_SIZE);
        while (*p->buffer!='\n' && *p->buffer!='\0') {
            mstr_append_char(str, *(p->buffer++));
        }
        LPCSTR ret = mstr_cstr(str);
        mstr_free(str);
        setln(p, start);
        return ret;
    }
    return NULL;
}
LPCSTR parser_sline(LPPARSER p){
    mstr_t* at =mstr_new_from_cstr(p->location->file);
    mstr_append(at, ":%d:%d", p->location->line,p->location->column);
    return mstr_cstr_dupfree(at);
}
LPCSTR parse_token_dup(LPPARSER p) {
    LPCSTR tok = parse_token(p);
    return tok?strdup(tok):NULL;
}
LPCSTR parse_token(LPPARSER p) {
    static char word[MAX_SEGMENT_SIZE];
    LPCSTR start = p->buffer;
    if(!p->buffer || !*p->buffer)
        return "";
    skipWs(p);
    if (*p->buffer == '\"') {
        LPCSTR closingQuote = strchr(p->buffer+1, '"');
        size_t stringLength = closingQuote-p->buffer+1;
        if (p->eat_quotes) {
            p->buffer++;
            stringLength -= 2;
        }
        memcpy(word, p->buffer, stringLength);
        word[stringLength] = '\0';
        p->buffer = ++closingQuote;
        setln(p, start);
//        printf("%s\n", word);
        return word;
    } else if (strchr(p->delimiters, *p->buffer)) {
        word[0] = *(p->buffer++);
        word[1] = '\0';
        setln(p, start);
        return word;
    } else {
        size_t segmentLength = 0;
        while (*p->buffer &&
           (!isspace(*p->buffer) && strchr(p->delimiters, *p->buffer) == NULL) &&
               segmentLength < MAX_SEGMENT_SIZE - 1) {
            word[segmentLength++] = *(p->buffer++);
        }
        word[segmentLength] = '\0'; // Null-terminate the segment
        setln(p, start);
        return word;
    }
}

LPCSTR peek_token(LPPARSER p) {
    PARSER tmp = *p;
    LPCSTR token = parse_token(&tmp);
    return token;
}
BOOL peek_token_is(LPPARSER p,LPCSTR str) {
    PARSER tmp = *p;
    LPCSTR token = parse_token(&tmp);
    return token && !strcmp(str,token);
}
LPCSTR parse_segment(LPPARSER p) {
    static char segment[MAX_SEGMENT_SIZE];
    memset(segment, 0, MAX_SEGMENT_SIZE);
    if (*p->buffer == '\0')
        return NULL;
    skipWs(p);
    LPCSTR start = p->buffer;
    if (*p->buffer == '\"') {
        ++start;
        p->buffer = strchr(start, '\"');
        memcpy(segment, start, p->buffer - start);
        segment[p->buffer - start] = '\0';
        p->buffer = strchr(p->buffer, ',');
    } else {
        p->buffer = strchr(p->buffer, ',');
        if (p->buffer) {
            memcpy(segment, start, p->buffer - start);
            segment[p->buffer - start] = '\0'; // Null-terminate the segment
        }
        else {
            strcpy(segment, start);
            p->buffer = start + strlen(start);
            return segment;
        }
    }
    ++p->buffer;
    return segment;
}

LPCSTR parse_segment2(LPPARSER p) {
    static char segment[MAX_SEGMENT_SIZE];
    memset(segment, 0, MAX_SEGMENT_SIZE);
    if (*p->buffer == '\0')
        return NULL;
    while (isspace(*p->buffer)){
        ++p->buffer;
    }
    DWORD num_quotes = 0;
    for (LPSTR out = segment; *p->buffer; ++p->buffer, ++out) {
        if (*p->buffer == ',' && (num_quotes & 1) == 0) {
            ++p->buffer;
            break;
        }
        if (*p->buffer == '"')
            ++num_quotes;
        *out = *p->buffer;
    }
    return segment;
}



void parser_error(LPPARSER parser) {
    parser->error = true;
}

void *find_in_array(void *array, long sizeofelem, LPCSTR name) {
    LPSTR str = array;
    while (*(LPCSTR *)str) {
        LPCSTR value = *(LPCSTR *)str;
        if (!strcmp(value, name)) {
            return str;
        }
        str += sizeofelem;
    }
    return NULL;
}

