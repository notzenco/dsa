/**
 * LFU CACHE (Least Frequently Used)
 *
 * A cache that evicts the least frequently used item when capacity is exceeded.
 * Ties are broken by LRU (least recently used among same frequency).
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Frequency Map                 Key-Node Map                              ║
 * ║   ┌─────────────┐               ┌─────────────┐                          ║
 * ║   │ freq=1 ─────┼──→ DLL        │ key1 ───────┼──→ Node                  ║
 * ║   │ freq=2 ─────┼──→ DLL        │ key2 ───────┼──→ Node                  ║
 * ║   │ freq=3 ─────┼──→ DLL        │ key3 ───────┼──→ Node                  ║
 * ║   └─────────────┘               └─────────────┘                          ║
 * ║                                                                           ║
 * ║   min_freq tracks the minimum frequency for O(1) eviction                 ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Time   │ Space                                      ║
 * ║  ───────────────────┼────────┼──────────                                  ║
 * ║  Get                │ O(1)   │ O(1)                                       ║
 * ║  Put                │ O(1)   │ O(1)                                       ║
 * ║  Delete             │ O(1)   │ O(1)                                       ║
 * ║  Space              │   -    │ O(capacity)                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #460 LFU Cache
 */

#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <stddef.h>
#include <stdbool.h>

/* ============== LFU Node ============== */

typedef struct LFUNode {
    int key;
    int value;
    int freq;
    struct LFUNode *prev;
    struct LFUNode *next;
} LFUNode;

/* ============== Frequency List ============== */

typedef struct FreqList {
    int freq;
    LFUNode *head;
    LFUNode *tail;
    size_t size;
    struct FreqList *next;  /* For hash chaining */
} FreqList;

/* ============== Hash Entry for Key Map ============== */

typedef struct LFUEntry {
    int key;
    LFUNode *node;
    struct LFUEntry *next;
} LFUEntry;

/* ============== LFU Cache ============== */

typedef struct {
    size_t capacity;
    size_t size;
    int min_freq;
    LFUEntry **key_map;       /* Key -> Node mapping */
    FreqList **freq_map;      /* Frequency -> List mapping */
    size_t num_buckets;
} LFUCache;

/* ============== Creation/Destruction ============== */

/**
 * Create a new LFU cache.
 * @param capacity Maximum number of items
 * @return New cache or NULL on failure
 */
LFUCache *lfu_create(size_t capacity);

/**
 * Destroy an LFU cache and free all memory.
 * @param cache Cache to destroy
 */
void lfu_destroy(LFUCache *cache);

/**
 * Clear all items from cache.
 * @param cache Cache to clear
 */
void lfu_clear(LFUCache *cache);

/* ============== Core Operations ============== */

/**
 * Get a value from the cache.
 * Increments the frequency count of the item.
 * @param cache Cache
 * @param key Key to look up
 * @param value Pointer to store the value
 * @return true if key exists
 */
bool lfu_get(LFUCache *cache, int key, int *value);

/**
 * Put a key-value pair into the cache.
 * If key exists, updates value and increments frequency.
 * If at capacity, evicts least frequently used item.
 * @param cache Cache
 * @param key Key
 * @param value Value
 * @return true on success
 */
bool lfu_put(LFUCache *cache, int key, int value);

/**
 * Delete a key from the cache.
 * @param cache Cache
 * @param key Key to delete
 * @return true if key was found and deleted
 */
bool lfu_delete(LFUCache *cache, int key);

/**
 * Check if a key exists in the cache.
 * Does NOT increment frequency.
 * @param cache Cache
 * @param key Key to check
 * @return true if key exists
 */
bool lfu_contains(const LFUCache *cache, int key);

/* ============== Cache Properties ============== */

/**
 * Get current number of items in cache.
 * @param cache Cache
 * @return Number of items
 */
size_t lfu_size(const LFUCache *cache);

/**
 * Get cache capacity.
 * @param cache Cache
 * @return Capacity
 */
size_t lfu_capacity(const LFUCache *cache);

/**
 * Check if cache is empty.
 * @param cache Cache
 * @return true if empty
 */
bool lfu_is_empty(const LFUCache *cache);

/**
 * Check if cache is full.
 * @param cache Cache
 * @return true if full
 */
bool lfu_is_full(const LFUCache *cache);

/* ============== Inspection ============== */

/**
 * Get frequency of a key.
 * @param cache Cache
 * @param key Key to check
 * @return Frequency count, or -1 if key not found
 */
int lfu_get_frequency(const LFUCache *cache, int key);

/**
 * Get the minimum frequency in the cache.
 * @param cache Cache
 * @return Minimum frequency, or 0 if empty
 */
int lfu_min_frequency(const LFUCache *cache);

#endif /* LFU_CACHE_H */
