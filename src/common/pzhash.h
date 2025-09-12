#ifndef PZHASH_H
#define PZHASH_H

#include <stdbool.h>
#include <stddef.h>

// 指针哈希表结构
struct PZHashEntry {
    void *key;           // 指针 key
    void *val;           // 值
    struct PZHashEntry *next;
};

struct PZHashTable {
    struct PZHashEntry **entries;
    size_t size_index;   // 当前使用的哈希表大小索引
    size_t entry_count;  // 条目数量
};

// 哈希函数类型定义
typedef size_t (*PZHashFunc)(void *key);
typedef bool (*PZEqualFunc)(void *key1, void *key2);

// 创建和销毁
struct PZHashTable *pzhash_create(void);
struct PZHashTable *pzhash_create_full(PZHashFunc hash_func, PZEqualFunc equal_func);
void pzhash_destroy(struct PZHashTable *hash_table);

// 基本操作
void pzhash_set(struct PZHashTable *hash_table, void *key, void *val);
void *pzhash_get(struct PZHashTable *hash_table, void *key);
void *pzhash_delete(struct PZHashTable *hash_table, void *key);
bool pzhash_exists(struct PZHashTable *hash_table, void *key);

// 遍历相关
typedef void (*PZHashForeachFunc)(void *key, void *val, void *user_data);
void pzhash_foreach(struct PZHashTable *hash_table, PZHashForeachFunc func, void *user_data);

// 统计信息
size_t pzhash_size(struct PZHashTable *hash_table);
size_t pzhash_capacity(struct PZHashTable *hash_table);

// 内置哈希函数
size_t pzhash_direct_hash(void *key);
size_t pzhash_ptr_hash(void *key);

// 内置比较函数
bool pzhash_direct_equal(void *key1, void *key2);
bool pzhash_ptr_equal(void *key1, void *key2);

#endif  // PZHASH_H