/**
 * LRU CACHE (Least Recently Used)
 *
 * A cache that evicts the least recently used item when capacity is exceeded.
 * Uses a hash table for O(1) lookup and doubly linked list for O(1) updates.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Hash Table                    Doubly Linked List                        ║
 * ║   ┌─────────┐                                                             ║
 * ║   │ key1 ───┼───────→  [HEAD] ←→ [Node1] ←→ [Node2] ←→ [TAIL]            ║
 * ║   │ key2 ───┼───────→            Most Recent ──→ Least Recent             ║
 * ║   │ key3 ───┼───────→                                                     ║
 * ║   └─────────┘                                                             ║
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
 * - #146 LRU Cache
 * - #432 All O(1) Data Structure
 * - #460 LFU Cache
 */

#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>
#include <stdbool.h>

/* ============== LRU Node ============== */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode *prev;
    struct LRUNode *next;
} LRUNode;

/* ============== Hash Entry ============== */

typedef struct LRUEntry {
    int key;
    LRUNode *node;
    struct LRUEntry *next;  /* For chaining */
} LRUEntry;

/* ============== LRU Cache ============== */

typedef struct {
    size_t capacity;
    size_t size;
    LRUNode *head;  /* Dummy head (most recent) */
    LRUNode *tail;  /* Dummy tail (least recent) */
    LRUEntry **buckets;
    size_t num_buckets;
} LRUCache;

/* ============== Creation/Destruction ============== */

/**
 * Create a new LRU cache.
 * @param capacity Maximum number of items
 * @return New cache or NULL on failure
 */
LRUCache *lru_create(size_t capacity);

/**
 * Destroy an LRU cache and free all memory.
 * @param cache Cache to destroy
 */
void lru_destroy(LRUCache *cache);

/**
 * Clear all items from cache.
 * @param cache Cache to clear
 */
void lru_clear(LRUCache *cache);

/* ============== Core Operations ============== */

/**
 * Get a value from the cache.
 * Marks the item as recently used.
 * @param cache Cache
 * @param key Key to look up
 * @param value Pointer to store the value
 * @return true if key exists
 */
bool lru_get(LRUCache *cache, int key, int *value);

/**
 * Put a key-value pair into the cache.
 * If key exists, updates the value.
 * If at capacity, evicts least recently used item.
 * @param cache Cache
 * @param key Key
 * @param value Value
 * @return true on success
 */
bool lru_put(LRUCache *cache, int key, int value);

/**
 * Delete a key from the cache.
 * @param cache Cache
 * @param key Key to delete
 * @return true if key was found and deleted
 */
bool lru_delete(LRUCache *cache, int key);

/**
 * Check if a key exists in the cache.
 * Does NOT mark as recently used.
 * @param cache Cache
 * @param key Key to check
 * @return true if key exists
 */
bool lru_contains(const LRUCache *cache, int key);

/* ============== Cache Properties ============== */

/**
 * Get current number of items in cache.
 * @param cache Cache
 * @return Number of items
 */
size_t lru_size(const LRUCache *cache);

/**
 * Get cache capacity.
 * @param cache Cache
 * @return Capacity
 */
size_t lru_capacity(const LRUCache *cache);

/**
 * Check if cache is empty.
 * @param cache Cache
 * @return true if empty
 */
bool lru_is_empty(const LRUCache *cache);

/**
 * Check if cache is full.
 * @param cache Cache
 * @return true if full
 */
bool lru_is_full(const LRUCache *cache);

/* ============== Inspection ============== */

/**
 * Get the most recently used key.
 * @param cache Cache
 * @param key Pointer to store the key
 * @return true if cache is not empty
 */
bool lru_peek_newest(const LRUCache *cache, int *key);

/**
 * Get the least recently used key.
 * @param cache Cache
 * @param key Pointer to store the key
 * @return true if cache is not empty
 */
bool lru_peek_oldest(const LRUCache *cache, int *key);

/**
 * Get all keys in order from most to least recently used.
 * @param cache Cache
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys retrieved
 */
size_t lru_get_keys(const LRUCache *cache, int *keys, size_t max_keys);

#endif /* LRU_CACHE_H */
