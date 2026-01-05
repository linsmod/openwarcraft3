#ifndef STROP_H
#define STROP_H

#include <stddef.h>

/**
 * @file strop.h
 * @brief 字符串操作函数库
 * 
 * 提供各种字符串处理函数，包括修剪、去除空白字符等功能。
 */

/**
 * @brief 去除字符串两端的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针（与输入相同）
 * 
 * @note 支持处理前后空格、制表符、换行符等空白字符
 * @note 使用 isspace() 函数判断空白字符
 */
char* trim(char* str);

/**
 * @brief 去除字符串左侧的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针
 */
char* ltrim(char* str);

/**
 * @brief 去除字符串右侧的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针
 */
char* rtrim(char* str);

/**
 * @brief 创建字符串的修剪副本（不修改原字符串）
 * @param str 要处理的字符串
 * @return 修剪后的字符串副本，需要调用者释放
 * 
 * @note 如果 str 为 NULL，返回 NULL
 * @note 如果内存分配失败，返回 NULL
 */
char* trim_dup(const char* str);

/**
 * @brief 安全的字符串修剪函数（带长度限制）
 * @param str 要处理的字符串
 * @param max_len 最大长度限制
 * @return 处理后的字符串指针
 */
char* trim_safe(char* str, size_t max_len);

/**
 * @brief 扩展的空白字符定义（包含中文空格等）
 * @param c 要检查的字符
 * @return 如果字符是空白返回 true
 */
int is_whitespace_extended(int c);

/**
 * @brief 使用扩展空白字符定义的trim函数
 * @param str 要处理的字符串
 * @return 处理后的字符串指针
 */
char* trim_extended(char* str);

/**
 * @brief 去除所有空白字符（包括中间的）
 * @param str 要处理的字符串
 * @return 处理后的字符串指针
 */
char* remove_all_whitespace(char* str);

#endif /* STROP_H */