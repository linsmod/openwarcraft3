#include "common/common.h"
#include "common/shared.h"
#include "common/zhash.h"
#include "r_local.h"
#include "stb/stb_truetype.h"
#include <StormPort.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#define MAX_GLYPHSET 256
#define FONT_SCALE 2
#define INV_SCALE(x) ((x) / (FONT_SCALE * 1000.f))

typedef struct {
    LPTEXTURE image;
    stbtt_bakedchar glyphs[MAX_GLYPHSET];
} glyphSet_t;

typedef struct  font {
    void *data;
    stbtt_fontinfo stbfont;
    glyphSet_t *sets[MAX_GLYPHSET];
    FLOAT size;
    int height;
} font_t;


// KNOWN_AS(ZHashTable, HASHTABLE);
// KNOWN_AS(ZHashEntry, HASHENTRY);
// #include "common/zhash.h"
// static LPHASHTABLE fontcache;

// void R_FontCacheInit(){
//     fontcache= zcreate_hash_table();
// }
// void R_FontCacheCleanup(){
//     zfree_hash_table(fontcache);
// }
// void R_FontCacheSet(LPSTR key,LPFONT font){
//     zhash_set(fontcache, strdup(key), font);
// }
// LPFONT R_FontCacheGet(LPSTR key){
//     return (LPFONT)zhash_get(fontcache, key);
// }

static const char* utf8_to_codepoint(const char *p, unsigned *dst) {
    unsigned res, n;
    switch (*p & 0xf0) {
        case 0xf0 :  res = *p & 0x07;  n = 3;  break;
        case 0xe0 :  res = *p & 0x0f;  n = 2;  break;
        case 0xd0 :
        case 0xc0 :  res = *p & 0x1f;  n = 1;  break;
        default   :  res = *p;         n = 0;  break;
    }
    while (n--) {
        res = (res << 6) | (*(++p) & 0x3f);
    }
    *dst = res;
    return p + 1;
}

static glyphSet_t* R_LoadGlyphSet(font_t *font, int idx) {
    glyphSet_t *set = ri.MemAlloc(sizeof(glyphSet_t));
    
    /* init image */
    int width = 128;
    int height = 128;
    uint8_t *fontimage;
    FLOAT s;
    int res;
    
retry:
    fontimage = ri.MemAlloc(width * height);
    /* load glyphs */
    s = stbtt_ScaleForMappingEmToPixels(&font->stbfont, 1) /
    stbtt_ScaleForPixelHeight(&font->stbfont, 1);
    res = stbtt_BakeFontBitmap(font->data, 0, font->size * s, fontimage,
                               width, height, idx * 256, 256, set->glyphs);
    
    /* retry with a larger image buffer if the buffer wasn't large enough */
    if (res < 0) {
        width *= 2;
        height *= 2;
        ri.MemFree(fontimage);
        goto retry;
    }
    
    /* adjust glyph yoffsets and xadvance */
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->stbfont, &ascent, &descent, &linegap);
    FLOAT scale = stbtt_ScaleForMappingEmToPixels(&font->stbfont, font->size);
    int scaled_ascent = ascent * scale + 0.5;
    for (int i = 0; i < 256; i++) {
        set->glyphs[i].yoff += scaled_ascent;
        set->glyphs[i].xadvance = floor(set->glyphs[i].xadvance);
    }
    
    LPCOLOR32 pixels = ri.MemAlloc(sizeof(COLOR32) * width * height);
    /* convert 8bit data to 32bit */
    for (int i = 0; i < width * height; i++) {
        uint8_t n = fontimage[i];
        pixels[i] = (COLOR32) { .r = 255, .g = 255, .b = 255, .a = n };
    }
    set->image = R_AllocateTexture(width, height);
    
    R_LoadTextureMipLevel(set->image, 0, pixels, width, height);
    
    return set;
}


static glyphSet_t* R_GetGlyphSet(font_t *font, int codepoint) {
    int idx = (codepoint >> 8) % MAX_GLYPHSET;
    if (!font->sets[idx]) {
        font->sets[idx] = R_LoadGlyphSet(font, idx);
    }
    return font->sets[idx];
}
#define SUBSTR_TO(buf, start, end) do { \
    int len = (end) - (start); \
    if (len < 0) len = 0; \
    if (len >= (int)sizeof(buf)-1) len = (int)sizeof(buf)-1; \
    memcpy(buf, start, len); \
    buf[len] = '\0'; \
} while(0)

#define STR_APPENDLINE(dst, chars) do { \
    char* _new_ptr = NULL; \
    size_t _old_len = (dst) ? strlen(dst) : 0; \
    size_t _add_len = (chars) ? strlen(chars) : 0; \
    size_t _new_len = _old_len + 1 + _add_len + 1; /* +1: '\n', +1: '\0' */ \
    \
    _new_ptr = (char*)realloc((dst), _new_len); \
    if (_new_ptr == NULL) { \
        /* realloc 失败，保留原指针（系统未释放） */ \
        break; \
    } \
    (dst) = _new_ptr; /* 更新原指针 */ \
    if (_old_len > 0) { \
        (dst)[_old_len] = '\n'; /* 添加换行 */ \
    } \
    memcpy((dst) + _old_len + 1, (chars), _add_len); \
    (dst)[_old_len + 1 + _add_len] = '\0'; /* 确保结尾 */ \
} while(0)

// 遍历每一行，start 和 end 指向行的开始和 \n 位置
#define FOR_EACH_LINE(start, end, input) \
    char *start, *end; \
    for (char* _pos = (input), *_next = NULL; \
         (_pos ? ((start) = _pos, \
            (_next = strchr(_pos, '\n')) ? ((end) = _next) : ((end) = _pos + strlen(_pos)), \
            1) : 0); \
         _pos = (_next && *(_next)) ? _next + 1 : NULL \
        )

// 判断是否为字体文件（不区分大小写）
static BOOL IsFontFile(LPCSTR filename) {
    int len = strlen(filename);
    if (len < 4) return false;
    
    const char* ext = filename + len - 4;
    if (_stricmp(ext, ".ttf") == 0) return true;
    if (_stricmp(ext, ".otf") == 0) return true;
    
    if (len >= 5) {
        ext = filename + len - 5;
        if (_stricmp(ext, ".ttc") == 0) return true;  // TrueType Collection
    }
    return false;
}
void loadAvialbeFonts(){
    DWORD buf_size;
    char* buffer= ri.ReadText("(listfile)",&buf_size);
    if(!buffer)
        return;
    printf("AailableFonts:\n");
    FOR_EACH_LINE(start, end, buffer) {
        char line[512];
        SUBSTR_TO(line, start, end);

        // 去除可能的 \r
        char* cr = strchr(line, '\r');
        if (cr) *cr = '\0';

        // 判断是否为字体文件
        int len = strlen(line);
        if (len >= 4) {
            const char* ext = line + len - 4;
            if (_stricmp(ext, ".ttf") == 0 || 
                _stricmp(ext, ".otf") == 0 ||
                _stricmp(ext, ".TTF") == 0 ||
                _stricmp(ext, ".OTF") == 0) {
                // Fonts\dffn_b31.ttf
                // Fonts\DFHeiMd.ttf
                // Fonts\dfst-m3u.ttf
                // Fonts\FRIZQT__.TTF
                // Fonts\NIM_____.ttf
                // Fonts\tt5500m_.ttf
                printf("loadAvialbeFont: %s\n", line);
                if(_stricmp("Fonts\\dfst-m3u.ttf",line)){
                    R_FontCacheSet("Arial", DEFAULT_TEXTFONT_SIZE,line);
                }
                else if(_stricmp("Fonts\\dfst-m3u.ttf",line)){
                    R_FontCacheSet("Arial", DEFAULT_TEXTFONT_SIZE,line);
                    R_FontCacheSet("sans-serif", DEFAULT_TEXTFONT_SIZE,line);
                }
            }
        }
    }
}
/* Font cache structure */
typedef struct {
    char family[64];
    int size;
    FONT* font;
    LPCSTR filename;
    LPSTR alias;
} font_cache_entry_t;
static font_cache_entry_t font_cache[16] = {0};
static int font_cache_count = 0;
/**
 * @brief Get font from cache or load it
 */
LPFONT R_FontCacheGet(LPCSTR family, DWORD size)
{
    for (int i = 0; i < font_cache_count; i++) {
        if (strcmp(font_cache[i].family, family) == 0 && 
            font_cache[i].size == size) {
            return font_cache[i].font;
        }
        FOR_EACH_LINE(start, end, font_cache[i].alias){
            char line[512];
            SUBSTR_TO(line, start, end);
            if(_stricmp(line,family)==0){
                return font_cache[i].font;
                break;
            }
        }
    }
    return NULL;
}
LPFONT R_FontCacheSet(LPCSTR family, DWORD size,LPCSTR filename){
    for (int i = 0; i < font_cache_count; i++) {
        if (strcmp(font_cache[i].filename, filename) == 0 && 
            font_cache[i].size == size) {
                BOOL exists = 0;
                FOR_EACH_LINE(start, end, font_cache[i].alias){
                    char line[512];
                    SUBSTR_TO(line, start, end);
                    if(_stricmp(line,family)==0){
                        exists=1;
                        break;
                    }
                }
                if(!exists){
                    STR_APPENDLINE(font_cache[i].alias, family);
                }
            return font_cache[i].font;
        }
    }
    FONT* new_font = R_LoadFont(filename, size);
    if (new_font) {
        font_cache_entry_t *entry = &font_cache[font_cache_count++];
        strncpy(entry->family, family, sizeof(entry->family)-1);
        entry->size = size;
        entry->font = new_font;
        entry->filename = strdup(filename);
        return new_font;
    }
    else{
        fprintf(stdout, "warn: R_FontCacheMapFont bad font %s", filename);
    }
}
/**
 * @brief Cleanup font cache
 */
void R_CleanupFontCache(void)
{
    for (int i = 0; i < font_cache_count; i++) {
        if (font_cache[i].font) {
            R_ReleaseFont(font_cache[i].font);
        }
    }
    font_cache_count = 0;
}

LPFONT R_LoadFont(LPCSTR filename, DWORD fontsize) {
    fprintf(stdout, "R_LoadFont %s\n", filename);
    fontsize = MAX(9, fontsize);
    font_t *font = ri.MemAlloc(sizeof(font_t));
    font->size = fontsize * FONT_SCALE;
    
    /* load font into buffer */
    HANDLE file = ri.FileOpen(filename);
    if (!file) { 
        return NULL; 
    }
    /* get size */
    DWORD buf_size = SFileGetFileSize(file, NULL);
    /* load */
    font->data = ri.MemAlloc(buf_size);
    SFileReadFile(file, font->data, buf_size, NULL, NULL);
    ri.FileClose(file);
    
    /* init stbfont */
    int ok = stbtt_InitFont(&font->stbfont, font->data, 0);
    if (!ok) { goto fail; }
    
    /* get height and scale */
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->stbfont, &ascent, &descent, &linegap);
    FLOAT scale = stbtt_ScaleForMappingEmToPixels(&font->stbfont, fontsize);
    font->height = (ascent - descent + linegap) * scale + 0.5;
    
    /* make tab and newline glyphs invisible */
    stbtt_bakedchar *g = R_GetGlyphSet(font, '\n')->glyphs;
    g['\t'].x1 = g['\t'].x0;
    g['\n'].x1 = g['\n'].x0;
    
    return font;
    
fail:
    if (font) { ri.MemFree(font->data); }
    ri.MemFree(font);
    return NULL;
}

void R_ReleaseFont(LPFONT font) {
    for (int i = 0; i < MAX_GLYPHSET; i++) {
        glyphSet_t *set = font->sets[i];
        if (set) {
            R_ReleaseTexture(set->image);
            ri.MemFree(set);
        }
    }
    ri.MemFree(font->data);
    ri.MemFree(font);
}

FLOAT R_GetFontWidth(LPFONT font, LPCSTR text) {
    FLOAT x = 0;
    LPCSTR p = text;
    unsigned codepoint;
    while (*p) {
        p = utf8_to_codepoint(p, &codepoint);
        glyphSet_t *set = R_GetGlyphSet(font, codepoint);
        stbtt_bakedchar *g = &set->glyphs[codepoint & 0xff];
        x += INV_SCALE(g->xadvance);
    }
    return x;
}


FLOAT R_GetFontHeight(LPFONT font) {
    return FONT_SCALE * INV_SCALE(font->height);
}

BOOL will_word_fit(LPCSTR text, FLOAT width, LPCFONT font) {
    for (LPCSTR p = text; *p && !isspace(*p) && *p != '|';) {
        unsigned codepoint;
        p = utf8_to_codepoint(p, &codepoint);
        glyphSet_t *set = R_GetGlyphSet((LPFONT)font, codepoint);
        stbtt_bakedchar *g = &set->glyphs[codepoint & 0xff];
        width -= INV_SCALE(g->xadvance);
    }
    return width >= -0.001;
}

static VECTOR2 get_position(LPCDRAWTEXT arg) {
    VECTOR2 pos = { 0 };
    VECTOR2 size = R_GetTextSize(arg);
    switch (arg->halign) {
        case FONT_JUSTIFYRIGHT: pos.x = arg->rect.x + arg->rect.w - size.x; break;
        case FONT_JUSTIFYCENTER: pos.x = arg->rect.x + (arg->rect.w - size.x) / 2; break;
        case FONT_JUSTIFYLEFT: pos.x = arg->rect.x; break;
    }
    switch (arg->valign) {
        case FONT_JUSTIFYBOTTOM: pos.y = arg->rect.y + arg->rect.h - size.y; break;
        case FONT_JUSTIFYMIDDLE: pos.y = arg->rect.y + (arg->rect.h - size.y) / 2; break;
        case FONT_JUSTIFYTOP: pos.y = arg->rect.y; break;
    }
    return pos;
}

static RECT get_uvrect(stbtt_bakedchar *g, FLOAT h, FLOAT w) {
    RECT const uv_rect = {
        .x = g->x0 / w,
        .y = g->y0 / h,
        .w = (g->x1 - g->x0) / w,
        .h = (g->y1 - g->y0) / h,
    };
    return uv_rect;
}

static RECT get_screenrect(LPCVECTOR2 cursor, stbtt_bakedchar *g) {
    RECT const screen = {
        .x = cursor->x + INV_SCALE(g->xoff),
        .y = cursor->y + INV_SCALE(g->yoff),
        .w = INV_SCALE(g->x1 - g->x0),
        .h = INV_SCALE(g->y1 - g->y0),
    };
    return screen;
}

static VECTOR2 process_text(LPCDRAWTEXT arg, BOOL draw) {
    if (!arg->font) {
        return MAKE(VECTOR2, 0, 0);
    }
    VECTOR2 pos = draw ? get_position(arg) : MAKE(VECTOR2, 0, 0);
    COLOR32 color = arg->color;
    VECTOR2 cursor = pos;
    FLOAT maxwidth = 0;
    // FLOAT linesize = 0.5 * arg->font->size / 1000.f;
    FLOAT linesize = R_GetFontHeight((LPFONT)arg->font);
    for (LPCSTR p = arg->text; *p;) {
        if (*p == '\n') {
            cursor.x = pos.x;
            cursor.y += linesize * arg->lineHeight * 1.1;
            p++;
            continue;
        }
        if (!strncmp(p, "|n", 2) || !strncmp(p, "|N", 2)) {
        // next_line:
            cursor.x = pos.x;
            cursor.y += linesize * arg->lineHeight * 1.1;
            p += 2;
            continue;
        }
        if (*p == '<') {
            DWORD icon = atoi(p+6);
            switch (*(DWORD*)(p+1)) {
                case MAKEFOURCC('I', 'c', 'o', 'n'):
                    if (draw && arg->icons[icon]) {
                        R_DrawImage(arg->icons[icon],
                                    &MAKE(RECT, cursor.x, cursor.y + linesize * 0.1, linesize, linesize),
                                    &MAKE(RECT, 0, 0, 1, 1),
                                    COLOR32_WHITE);
                    }
                    cursor.x += linesize;
                    break;
            }
            p = strchr(p + 1, '>') + 1;
            continue;;
        }
        if (!strncmp(p, "|r", 2) || !strncmp(p, "|R", 2)) {
            color = COLOR32_WHITE;
            p += 2;
            continue;
        }
        if (!strncmp(p, "|c", 2) || !strncmp(p, "|C", 2)) {
            COLOR32 c;
            sscanf(p+2, "%08x", (DWORD *)&c);
            color.a = c.a;
            color.b = c.r;
            color.g = c.g;
            color.r = c.b;
            p += 10;
            continue;
        }
        
        // if (arg->wordWrap && cursor.x > pos.x && !will_word_fit(p, arg->textWidth - (cursor.x - pos.x), arg->font)) {
        //     cursor.x = pos.x;
        //     cursor.y += linesize * arg->lineHeight;
        // }
       
        unsigned codepoint;
        p = utf8_to_codepoint(p, &codepoint);
        glyphSet_t *set = R_GetGlyphSet((LPFONT)arg->font, codepoint);
        stbtt_bakedchar *g = &set->glyphs[codepoint & 0xff];
        FLOAT charWidth = INV_SCALE(g->xadvance);
        if (arg->wordWrap && ((cursor.x + charWidth) - (pos.x + arg->textWidth))> 0.001) {
            cursor.x = pos.x;
            cursor.y += linesize * arg->lineHeight;
        }
        if (draw) {
            FLOAT const w = set->image->width;
            FLOAT const h = set->image->height;
            RECT const uv_rect = get_uvrect(g, h, w);
            RECT const screen = get_screenrect(&cursor, g);
            R_DrawImage(set->image, &screen, &uv_rect, color);
        }
        cursor.x += charWidth;
        maxwidth = MAX(maxwidth, cursor.x);
    }
    return MAKE(VECTOR2, maxwidth, cursor.y + R_GetFontHeight((LPFONT)arg->font));
}


void R_DrawUtf8TextEx(LPCDRAWTEXT arg) {
    process_text(arg, true);
    
//    R_DrawWireRect(&arg->rect, MAKE(COLOR32, 255, 0, 255, 255));
}

LPFONT g_default_text_font;



// 初始化默认字体
void R_InitDefaultFonts(void) {
    loadAvialbeFonts();
    if (!g_default_text_font) {
        // 尝试加载默认字体
        // 这里可以使用系统字体或War3的默认字体
        g_default_text_font = R_FontCacheSet(DEFAULT_TEXTFONT_NAME, 14,"Fonts\\dfst-m3u.ttf"); // 黑体，14px
        if (!g_default_text_font) {
            printf("Warning: Failed to load default font\n");
        }
        printf("Default font loaded\n");
    }
}

// 清理默认字体
void R_ReleaseDefaultFonts() {
    if (g_default_text_font) {
        R_ReleaseFont(g_default_text_font);
        g_default_text_font = NULL;
        printf("Default font cleaned up\n");
    }
}

void R_DrawUtf8Text(LPCSTR string, DWORD x, DWORD y, COLOR32 color){
    assert(g_default_text_font);

    DRAWTEXT arg;
    arg.color = color;
    arg.rect = MAKE(RECT,x,y);
    arg.halign= FONT_JUSTIFYLEFT;
    arg.wordWrap = 1;
    arg.text = string;
    arg.valign = FONT_JUSTIFYBOTTOM;
    arg.model_matrix = NULL;
    arg.font = g_default_text_font;
    process_text(&arg, true);
}

void R_DrawUtf8Text2(LPCSTR string, DWORD x, DWORD y, COLOR32 color,LPMATRIX4 transform){
    assert(g_default_text_font);
    
    DRAWTEXT arg;
    arg.color = color;
    arg.rect = MAKE(RECT,x,y);
    arg.halign= FONT_JUSTIFYLEFT;
    arg.wordWrap = 1;
    arg.text = string;
    arg.valign = FONT_JUSTIFYBOTTOM;
    arg.model_matrix = transform;
    arg.font = g_default_text_font;
    process_text(&arg, true);
}

VECTOR2 R_GetTextSize(LPCDRAWTEXT arg) {
    return process_text(arg, false);
}
