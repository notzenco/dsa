/**
 * TTL CACHE (Time-To-Live Cache with LRU Eviction)
 *
 * A cache that combines time-based expiry with LRU eviction policy.
 * Entries expire after a specified TTL (time-to-live) duration.
 * When capacity is reached, the least recently used entry is evicted.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  TTL Cache combines two eviction strategies:                              ║
 * ║                                                                           ║
 * ║  1. Time-based expiry (TTL):                                              ║
 * ║     ┌─────────────────────────────────────────────┐                       ║
 * ║     │ Key: A │ Value: 100 │ Expires: t+5s │ VALID │                       ║
 * ║     │ Key: B │ Value: 200 │ Expires: t+2s │ VALID │                       ║
 * ║     │ Key: C │ Value: 300 │ Expires: t-1s │ EXPIRED ← Auto-removed       ║
 * ║     └─────────────────────────────────────────────┘                       ║
 * ║                                                                           ║
 * ║  2. LRU eviction (when at capacity):                                      ║
 * ║     HEAD (LRU)                               TAIL (MRU)                   ║
 * ║        │                                        │                         ║
 * ║        ▼                                        ▼                         ║
 * ║     ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐                             ║
 * ║     │ Old │ ←→ │     │ ←→ │     │ ←→ │ New │                             ║
 * ║     └─────┘    └─────┘    └─────┘    └─────┘                             ║
 * ║        ↑                                                                  ║
 * ║     Evicted first when capacity reached                                   ║
 * ║                                                                           ║
 * ║  Hash Table for O(1) lookup:                                              ║
 * ║     ┌───┬───┬───┬───┬───┬───┬───┬───┐                                    ║
 * ║     │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │  ← Buckets                         ║
 * ║     └─┬─┴───┴─┬─┴───┴───┴─┬─┴───┴───┘                                    ║
 * ║       ↓       ↓           ↓                                               ║
 * ║      [A]     [B]         [C]  ← Entries with hash chaining               ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Get                │ O(1)         │ O(n)       │ O(1)    │
 * │ Put                │ O(1)*        │ O(n)       │ O(1)    │
 * │ Delete             │ O(1)         │ O(n)       │ O(1)    │
 * │ Cleanup (expired)  │ O(n)         │ O(n)       │ O(1)    │
 * │ Space              │ -            │ -          │ O(n)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * O(n) when cleanup runs; cleanup is called periodically
 *
 * LEETCODE PROBLEMS:
 * - #146 LRU Cache (base concept)
 * - Cache with expiration (system design)
 *
 * USE CASES:
 * - Session management with timeout
 * - API rate limiting
 * - DNS caching
 * - Token caching with expiry
 * - Database query caching
 */

#ifndef TTL_CACHE_H
#define TTL_CACHE_H

#include <stddef.h>
#include <stdbool.h>

#define TTL_CACHE_INITIAL_BUCKETS 16
#define TTL_CACHE_LOAD_FACTOR 0.75

/**
 * Node in the TTL cache, stored in both hash table and LRU list
 */
typedef struct TTLCacheNode {
    int key;
    int value;
    double expire_time;              /* Absolute expiration time */
    struct TTLCacheNode *prev;       /* LRU list previous */
    struct TTLCacheNode *next;       /* LRU list next */
    struct TTLCacheNode *hash_next;  /* Hash chain next */
} TTLCacheNode;

/**
 * TTL Cache structure
 */
typedef struct {
    size_t capacity;                 /* Maximum number of entries */
    size_t size;                     /* Current number of entries */
    double default_ttl;              /* Default TTL in seconds */
    TTLCacheNode **buckets;          /* Hash table buckets */
    size_t num_buckets;              /* Number of buckets */
    TTLCacheNode *head;              /* LRU list head (dummy node) */
    TTLCacheNode *tail;              /* LRU list tail (dummy node) */
    double (*time_func)(void);       /* Function to get current time */
} TTLCache;

/* ============== Creation and Destruction ============== */

/**
 * Create a new TTL cache.
 *
 * @param capacity Maximum number of entries
 * @param ttl_seconds Default time-to-live in seconds
 * @return Pointer to new cache, or NULL on allocation failure
 */
TTLCache *ttl_create(size_t capacity, double ttl_seconds);

/**
 * Create a TTL cache with custom time function (for testing).
 *
 * @param capacity Maximum number of entries
 * @param ttl_seconds Default time-to-live in seconds
 * @param time_func Function that returns current time in seconds
 * @return Pointer to new cache, or NULL on allocation failure
 */
TTLCache *ttl_create_with_time_func(size_t capacity, double ttl_seconds,
                                     double (*time_func)(void));

/**
 * Destroy cache and free all memory.
 *
 * @param cache Cache to destroy
 */
void ttl_destroy(TTLCache *cache);

/**
 * Remove all entries from cache.
 *
 * @param cache Cache to clear
 */
void ttl_clear(TTLCache *cache);

/* ============== Core Operations ============== */

/**
 * Get value for key.
 * Returns false if key not found or expired.
 * Updates LRU position on successful access.
 *
 * @param cache Cache to query
 * @param key Key to look up
 * @param value Output parameter for value
 * @return true if found and not expired, false otherwise
 */
bool ttl_get(TTLCache *cache, int key, int *value);

/**
 * Put key-value pair with default TTL.
 * If key exists, updates value and resets TTL.
 * If at capacity, evicts LRU entry first.
 *
 * @param cache Cache to update
 * @param key Key to insert/update
 * @param value Value to store
 * @return true on success, false on allocation failure
 */
bool ttl_put(TTLCache *cache, int key, int value);

/**
 * Put key-value pair with custom TTL.
 *
 * @param cache Cache to update
 * @param key Key to insert/update
 * @param value Value to store
 * @param ttl_seconds Custom TTL for this entry
 * @return true on success, false on allocation failure
 */
bool ttl_put_with_ttl(TTLCache *cache, int key, int value, double ttl_seconds);

/**
 * Delete entry by key.
 *
 * @param cache Cache to update
 * @param key Key to delete
 * @return true if key was found and deleted, false otherwise
 */
bool ttl_delete(TTLCache *cache, int key);

/**
 * Check if key exists and is not expired.
 *
 * @param cache Cache to query
 * @param key Key to check
 * @return true if key exists and not expired, false otherwise
 */
bool ttl_contains(TTLCache *cache, int key);

/* ============== Maintenance ============== */

/**
 * Remove all expired entries from cache.
 * Called automatically during put operations.
 *
 * @param cache Cache to clean
 * @return Number of entries removed
 */
size_t ttl_cleanup(TTLCache *cache);

/**
 * Get remaining TTL for a key.
 *
 * @param cache Cache to query
 * @param key Key to check
 * @param remaining Output parameter for remaining TTL in seconds
 * @return true if key exists and not expired, false otherwise
 */
bool ttl_get_ttl(TTLCache *cache, int key, double *remaining);

/**
 * Reset TTL for existing key without changing value.
 *
 * @param cache Cache to update
 * @param key Key to refresh
 * @return true if key exists and was refreshed, false otherwise
 */
bool ttl_refresh(TTLCache *cache, int key);

/* ============== Properties ============== */

/**
 * Get number of non-expired entries.
 * Note: This runs cleanup first, so may be O(n).
 *
 * @param cache Cache to query
 * @return Number of valid entries
 */
size_t ttl_size(TTLCache *cache);

/**
 * Get number of entries without cleanup.
 * May include expired entries.
 *
 * @param cache Cache to query
 * @return Number of entries (including expired)
 */
size_t ttl_size_dirty(const TTLCache *cache);

/**
 * Get capacity of cache.
 *
 * @param cache Cache to query
 * @return Maximum capacity
 */
size_t ttl_capacity(const TTLCache *cache);

/**
 * Check if cache is empty.
 *
 * @param cache Cache to check
 * @return true if empty, false otherwise
 */
bool ttl_is_empty(const TTLCache *cache);

/**
 * Check if cache is full.
 *
 * @param cache Cache to check
 * @return true if at capacity, false otherwise
 */
bool ttl_is_full(const TTLCache *cache);

#endif /* TTL_CACHE_H */
