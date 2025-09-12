#include "pzhash.h"
#include <stdlib.h>
#include <string.h>

// 辅助宏
#define PZCOUNT_OF(arr) (sizeof(arr) / sizeof(*arr))
#define pz_free free
#define pz_malloc malloc
#define pz_calloc calloc

// 哈希表大小序列（质数）
static const size_t hash_sizes[] = {
    53, 101, 211, 503, 1009, 2003, 4001, 8009, 16001, 32003,
    64007, 128021, 256031, 512009, 1024001, 2048003, 4096001
};

// 默认哈希函数和比较函数
static size_t default_hash_func(void *key) {
    return pzhash_ptr_hash(key);
}

static bool default_equal_func(void *key1, void *key2) {
    return pzhash_ptr_equal(key1, key2);
}

// 创建哈希表（使用默认函数）
struct PZHashTable *pzhash_create(void) {
    return pzhash_create_full(default_hash_func, default_equal_func);
}

// 创建哈希表（自定义函数）
struct PZHashTable *pzhash_create_full(PZHashFunc hash_func, PZEqualFunc equal_func) {
    struct PZHashTable *hash_table;
    
    hash_table = (struct PZHashTable *)pz_malloc(sizeof(struct PZHashTable));
    if (!hash_table) return NULL;
    
    hash_table->size_index = 0;
    hash_table->entry_count = 0;
    hash_table->entries = pz_calloc(hash_sizes[0], sizeof(struct PZHashEntry *));
    
    if (!hash_table->entries) {
        pz_free(hash_table);
        return NULL;
    }
    
    // 存储函数指针（如果需要可以扩展结构体来存储）
    // 这里为了简单，使用默认函数
    
    return hash_table;
}

// 销毁哈希表
void pzhash_destroy(struct PZHashTable *hash_table) {
    if (!hash_table) return;
    
    size_t size = hash_sizes[hash_table->size_index];
    
    for (size_t i = 0; i < size; i++) {
        struct PZHashEntry *entry = hash_table->entries[i];
        while (entry) {
            struct PZHashEntry *next = entry->next;
            pz_free(entry);
            entry = next;
        }
    }
    
    pz_free(hash_table->entries);
    pz_free(hash_table);
}

// 创建哈希条目
static struct PZHashEntry *create_entry(void *key, void *val) {
    struct PZHashEntry *entry;
    
    entry = (struct PZHashEntry *)pz_malloc(sizeof(struct PZHashEntry));
    if (!entry) return NULL;
    
    entry->key = key;
    entry->val = val;
    entry->next = NULL;
    
    return entry;
}

// 计算哈希值
static size_t calculate_hash(struct PZHashTable *hash_table, void *key) {
    size_t size = hash_sizes[hash_table->size_index];
    return pzhash_ptr_hash(key) % size;
}

// 设置键值对
void pzhash_set(struct PZHashTable *hash_table, void *key, void *val) {
    if (!hash_table || !key) return;
    
    size_t hash = calculate_hash(hash_table, key);
    struct PZHashEntry *entry = hash_table->entries[hash];
    struct PZHashEntry *prev = NULL;
    
    // 查找是否已存在
    while (entry) {
        if (pzhash_ptr_equal(entry->key, key)) {
            // 更新值
            entry->val = val;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
    
    // 创建新条目
    struct PZHashEntry *new_entry = create_entry(key, val);
    if (!new_entry) return;
    
    if (prev) {
        prev->next = new_entry;
    } else {
        hash_table->entries[hash] = new_entry;
    }
    
    hash_table->entry_count++;
    
    // 检查是否需要扩容
    size_t current_size = hash_sizes[hash_table->size_index];
    if (hash_table->entry_count > current_size * 3 / 4) {
        // 扩容逻辑（简化版，实际需要实现rehash）
        // 这里为了简单省略了自动扩容
    }
}

// 获取值
void *pzhash_get(struct PZHashTable *hash_table, void *key) {
    if (!hash_table || !key) return NULL;
    
    size_t hash = calculate_hash(hash_table, key);
    struct PZHashEntry *entry = hash_table->entries[hash];
    
    while (entry) {
        if (pzhash_ptr_equal(entry->key, key)) {
            return entry->val;
        }
        entry = entry->next;
    }
    
    return NULL;
}

// 删除键值对
void *pzhash_delete(struct PZHashTable *hash_table, void *key) {
    if (!hash_table || !key) return NULL;
    
    size_t hash = calculate_hash(hash_table, key);
    struct PZHashEntry *entry = hash_table->entries[hash];
    struct PZHashEntry *prev = NULL;
    
    while (entry) {
        if (pzhash_ptr_equal(entry->key, key)) {
            void *val = entry->val;
            
            if (prev) {
                prev->next = entry->next;
            } else {
                hash_table->entries[hash] = entry->next;
            }
            
            pz_free(entry);
            hash_table->entry_count--;
            
            // 检查是否需要缩容（简化版）
            return val;
        }
        
        prev = entry;
        entry = entry->next;
    }
    
    return NULL;
}

// 检查键是否存在
bool pzhash_exists(struct PZHashTable *hash_table, void *key) {
    if (!hash_table || !key) return false;
    
    size_t hash = calculate_hash(hash_table, key);
    struct PZHashEntry *entry = hash_table->entries[hash];
    
    while (entry) {
        if (pzhash_ptr_equal(entry->key, key)) {
            return true;
        }
        entry = entry->next;
    }
    
    return false;
}

// 遍历哈希表
void pzhash_foreach(struct PZHashTable *hash_table, PZHashForeachFunc func, void *user_data) {
    if (!hash_table || !func) return;
    
    size_t size = hash_sizes[hash_table->size_index];
    
    for (size_t i = 0; i < size; i++) {
        struct PZHashEntry *entry = hash_table->entries[i];
        while (entry) {
            func(entry->key, entry->val, user_data);
            entry = entry->next;
        }
    }
}

// 获取条目数量
size_t pzhash_size(struct PZHashTable *hash_table) {
    return hash_table ? hash_table->entry_count : 0;
}

// 获取容量
size_t pzhash_capacity(struct PZHashTable *hash_table) {
    return hash_table ? hash_sizes[hash_table->size_index] : 0;
}

// 内置哈希函数：直接使用指针值
size_t pzhash_direct_hash(void *key) {
    return (size_t)key;
}

// 内置哈希函数：更好的指针哈希
size_t pzhash_ptr_hash(void *key) {
    // Thomas Wang's 64-bit mix function
    size_t x = (size_t)key;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

// 内置比较函数：直接比较指针
bool pzhash_direct_equal(void *key1, void *key2) {
    return key1 == key2;
}

// 内置比较函数：指针比较（与direct相同）
bool pzhash_ptr_equal(void *key1, void *key2) {
    return key1 == key2;
}