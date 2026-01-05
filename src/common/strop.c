#include <ctype.h>
#include <string.h>
#include <stdlib.h>
/**
 * @brief 去除字符串两端的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针（与输入相同）
 * 
 * @note 支持处理前后空格、制表符、换行符等空白字符
 * @note 使用 isspace() 函数判断空白字符
 */
char* trim(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }
    
    char* start = str;
    char* end = str + strlen(str) - 1;
    
    // 去除前导空白
    while (isspace((unsigned char)*start) && *start != '\0') {
        start++;
    }
    
    // 如果整个字符串都是空白
    if (*start == '\0') {
        *str = '\0';
        return str;
    }
    
    // 去除尾部空白
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    
    // 移动处理后的字符串到开头
    if (start != str) {
        char* p = str;
        while (start <= end) {
            *p++ = *start++;
        }
        *p = '\0';
    } else {
        // 直接在原位置添加结束符
        *(end + 1) = '\0';
    }
    
    return str;
}

/**
 * @brief 去除字符串左侧的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针
 */
char* ltrim(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }
    
    char* start = str;
    
    // 跳过前导空白
    while (isspace((unsigned char)*start) && *start != '\0') {
        start++;
    }
    
    // 移动字符串到开头
    if (start != str) {
        char* p = str;
        while (*start != '\0') {
            *p++ = *start++;
        }
        *p = '\0';
    }
    
    return str;
}

/**
 * @brief 去除字符串右侧的空白字符
 * @param str 要处理的字符串（原地修改）
 * @return 处理后的字符串指针
 */
char* rtrim(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }
    
    char* end = str + strlen(str) - 1;
    
    // 从尾部向前查找非空白字符
    while (end >= str && isspace((unsigned char)*end)) {
        end--;
    }
    
    // 设置结束符
    *(end + 1) = '\0';
    
    return str;
}

/**
 * @brief 创建字符串的修剪副本（不修改原字符串）
 * @param str 要处理的字符串
 * @return 修剪后的字符串副本，需要调用者释放
 * 
 * @note 如果 str 为 NULL，返回 NULL
 * @note 如果内存分配失败，返回 NULL
 */
char* trim_dup(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    
    // 分配内存
    char* result = strdup(str);
    if (result == NULL) {
        return NULL;
    }
    
    // 修剪
    return trim(result);
}

/**
 * @brief 安全的字符串修剪函数（带长度限制）
 * @param str 要处理的字符串
 * @param max_len 最大长度限制
 * @return 处理后的字符串指针
 */
char* trim_safe(char* str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return str;
    }
    
    // 确保字符串以null结尾
    if (strlen(str) >= max_len) {
        str[max_len - 1] = '\0';
    }
    
    return trim(str);
}

/**
 * @brief 扩展的空白字符定义（包含中文空格等）
 * @param c 要检查的字符
 * @return 如果字符是空白返回 true
 */
int is_whitespace_extended(int c) {
    // 标准空白字符
    if (isspace(c)) {
        return 1;
    }
    
    // 中文空格（全角空格）
    if (c == 0x3000) {
        return 1;
    }
    
    // 不间断空格（&nbsp;）
    if (c == 0xA0) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief 使用扩展空白字符定义的trim函数
 * @param str 要处理的字符串
 * @return 处理后的字符串指针
 */
char* trim_extended(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }
    
    char* start = str;
    char* end = str + strlen(str) - 1;
    
    // 去除前导空白
    while (is_whitespace_extended((unsigned char)*start) && *start != '\0') {
        start++;
    }
    
    // 如果整个字符串都是空白
    if (*start == '\0') {
        *str = '\0';
        return str;
    }
    
    // 去除尾部空白
    while (end > start && is_whitespace_extended((unsigned char)*end)) {
        end--;
    }
    
    // 移动处理后的字符串到开头
    if (start != str) {
        char* p = str;
        while (start <= end) {
            *p++ = *start++;
        }
        *p = '\0';
    } else {
        *(end + 1) = '\0';
    }
    
    return str;
}

/**
 * @brief 去除所有空白字符（包括中间的）
 * @param str 要处理的字符串
 * @return 处理后的字符串指针
 */
char* remove_all_whitespace(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }
    
    char* dest = str;
    char* src = str;
    
    while (*src != '\0') {
        if (!isspace((unsigned char)*src)) {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
    
    return str;
}