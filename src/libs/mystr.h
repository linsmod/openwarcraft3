#ifndef MSTR_H
#define MSTR_H

#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h> // 如果需要文件操作

// 字符串结构体
typedef struct {
    char* data;     // 字符串数据（以 null 结尾）
    size_t length;  // 当前字符串长度（不含 null 终止符）
    size_t capacity; // 分配的总容量（含 null 终止符）
} mstr_t;

// 创建和销毁
mstr_t* mstr_new(void);
mstr_t* mstr_new_with_capacity(size_t capacity);
mstr_t* mstr_new_from_cstr(const char* cstr);
void mstr_free(mstr_t* str);

// 基础操作
size_t mstr_length(const mstr_t* str);
const char* mstr_cstr(const mstr_t* str);
const char* mstr_cstr_dup(const mstr_t* str);
const char* mstr_cstr_dupfree(const mstr_t* str);
bool mstr_empty(const mstr_t* str);
void mstr_clear(mstr_t* str);

// 修改操作
int mstr_append(mstr_t* str, const char* format, ...);
int mstr_append_v(mstr_t* str, const char* format, va_list args);
int mstr_append_cstr(mstr_t* str, const char* cstr);
int mstr_append_char(mstr_t* str, char c);
int mstr_append_int(mstr_t* str, int value);
int mstr_append_double(mstr_t* str, double value);

// 实用功能
mstr_t* mstr_substring(const mstr_t* str, size_t start, size_t length);
int mstr_compare(const mstr_t* str1, const mstr_t* str2);
int mstr_compare_cstr(const mstr_t* str, const char* cstr);
bool mstr_starts_with(const mstr_t* str, const char* prefix);
bool mstr_ends_with(const mstr_t* str, const char* suffix);
mstr_t* mstr_join(const mstr_t* separator, const mstr_t** strings, size_t count);

// 转换操作
mstr_t* mstr_to_upper(mstr_t* str);
mstr_t* mstr_to_lower(mstr_t* str);
mstr_t* mstr_trim(mstr_t* str);

// 查找和替换
long mstr_find(const mstr_t* str, const char* substring, size_t start_pos);
int mstr_replace(mstr_t* str, const char* old_sub, const char* new_sub);

#endif // MSTR_H