#include "mystr.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MSTR_DEFAULT_CAPACITY 32
#define MSTR_GROWTH_FACTOR 2

// 确保有足够容量（内部函数）
static bool mstr_ensure_capacity(mstr_t* str, size_t needed) {
    if (str == NULL) return false;
    
    // 需要额外空间给 null 终止符
    size_t required = str->length + needed + 1;
    
    if (required > str->capacity) {
        size_t new_capacity = str->capacity;
        while (new_capacity < required) {
            new_capacity *= MSTR_GROWTH_FACTOR;
        }
        
        char* new_data = realloc(str->data, new_capacity);
        if (new_data == NULL) {
            return false;
        }
        
        str->data = new_data;
        str->capacity = new_capacity;
    }
    return true;
}

// 创建新字符串
mstr_t* mstr_new(void) {
    return mstr_new_with_capacity(MSTR_DEFAULT_CAPACITY);
}

mstr_t* mstr_new_with_capacity(size_t capacity) {
    mstr_t* str = malloc(sizeof(mstr_t));
    if (str == NULL) return NULL;
    
    str->data = malloc(capacity);
    if (str->data == NULL) {
        free(str);
        return NULL;
    }
    
    str->data[0] = '\0';
    str->length = 0;
    str->capacity = capacity;
    return str;
}

mstr_t* mstr_new_from_cstr(const char* cstr) {
    if (cstr == NULL) return mstr_new();
    
    size_t len = strlen(cstr);
    mstr_t* str = mstr_new_with_capacity(len + 1);
    if (str == NULL) return NULL;
    
    memcpy(str->data, cstr, len + 1);
    str->length = len;
    return str;
}

void mstr_free(mstr_t* str) {
    if (str) {
        free(str->data);
        free(str);
    }
}

// 基础操作
size_t mstr_length(const mstr_t* str) {
    return str ? str->length : 0;
}

const char* mstr_cstr(const mstr_t* str) {
    return str ? str->data : "";
}

const char* mstr_cstr_dup(const mstr_t* str) {
    return str ? strdup(str->data) : "";
}

const char* mstr_cstr_dupfree(const mstr_t* str) {
    const char* ret= str ? strdup(str->data) : "";
    mstr_free((mstr_t*)str);
    return ret;
}

bool mstr_empty(const mstr_t* str) {
    return str == NULL || str->length == 0;
}

void mstr_clear(mstr_t* str) {
    if (str) {
        str->data[0] = '\0';
        str->length = 0;
    }
}

// 修改操作 - 核心函数
int mstr_append(mstr_t* str, const char* format, ...) {
    if (str == NULL || format == NULL) return -1;
    
    va_list args;
    int needed;
    
    // 计算需要多少空间
    va_start(args, format);
    needed = vsnprintf(NULL, 0, format, args);
    va_end(args);
    
    if (needed < 0) return -1;
    if (!mstr_ensure_capacity(str, needed)) return -1;
    
    // 执行实际的格式化
    va_start(args, format);
    int written = vsnprintf(str->data + str->length, 
                          str->capacity - str->length, 
                          format, args);
    va_end(args);
    
    if (written > 0) {
        str->length += written;
    }
    
    return written;
}

int mstr_append_v(mstr_t* str, const char* format, va_list args) {
    if (str == NULL || format == NULL) return -1;
    
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    if (needed < 0) return -1;
    if (!mstr_ensure_capacity(str, needed)) return -1;
    
    int written = vsnprintf(str->data + str->length, 
                          str->capacity - str->length, 
                          format, args);
    
    if (written > 0) {
        str->length += written;
    }
    
    return written;
}

int mstr_append_cstr(mstr_t* str, const char* cstr) {
    if (str == NULL || cstr == NULL) return -1;
    
    size_t len = strlen(cstr);
    if (!mstr_ensure_capacity(str, len)) return -1;
    
    memcpy(str->data + str->length, cstr, len + 1);
    str->length += len;
    return len;
}

int mstr_append_char(mstr_t* str, char c) {
    if (str == NULL) return -1;
    
    if (!mstr_ensure_capacity(str, 1)) return -1;
    
    str->data[str->length] = c;
    str->data[str->length + 1] = '\0';
    str->length += 1;
    return 1;
}

// 其他函数的实现（简化示例）
mstr_t* mstr_substring(const mstr_t* str, size_t start, size_t length) {
    if (str == NULL || start >= str->length) return mstr_new();
    
    size_t actual_length = length;
    if (start + length > str->length) {
        actual_length = str->length - start;
    }
    
    mstr_t* result = mstr_new_with_capacity(actual_length + 1);
    if (result == NULL) return NULL;
    
    memcpy(result->data, str->data + start, actual_length);
    result->data[actual_length] = '\0';
    result->length = actual_length;
    
    return result;
}

// 