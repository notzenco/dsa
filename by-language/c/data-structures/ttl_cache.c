/**
 * TTL Cache Implementation
 *
 * Combines time-based expiry with LRU eviction.
 */

#include "ttl_cache.h"
#include <stdlib.h>
#include <time.h>

/* ============== Helper Functions ============== */

/**
 * Default time function using system clock.
 */
static double default_time_func(void) {
    return (double)time(NULL);
}

/**
 * Hash function for integer keys.
 */
static size_t ttl_hash(int key, size_t num_buckets) {
    /* Simple hash for integers */
    unsigned int k = (unsigned int)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b;
    k = ((k >> 16) ^ k) * 0x45d9f3b;
    k = (k >> 16) ^ k;
    return k % num_buckets;
}

/**
 * Create a new node.
 */
static TTLCacheNode *ttl_create_node(int key, int value, double expire_time) {
    TTLCacheNode *node = malloc(sizeof(TTLCacheNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->expire_time = expire_time;
    node->prev = NULL;
    node->next = NULL;
    node->hash_next = NULL;
    return node;
}

/**
 * Find node in hash table.
 */
static TTLCacheNode *ttl_find_node(TTLCache *cache, int key) {
    size_t bucket = ttl_hash(key, cache->num_buckets);
    TTLCacheNode *node = cache->buckets[bucket];

    while (node != NULL) {
        if (node->key == key) {
            return node;
        }
        node = node->hash_next;
    }
    return NULL;
}

/**
 * Remove node from LRU list (but not from hash table).
 */
static void ttl_remove_from_list(TTLCacheNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/**
 * Add node to tail of LRU list (most recently used).
 */
static void ttl_add_to_tail(TTLCache *cache, TTLCacheNode *node) {
    node->prev = cache->tail->prev;
    node->next = cache->tail;
    cache->tail->prev->next = node;
    cache->tail->prev = node;
}

/**
 * Move node to tail (mark as recently used).
 */
static void ttl_move_to_tail(TTLCache *cache, TTLCacheNode *node) {
    ttl_remove_from_list(node);
    ttl_add_to_tail(cache, node);
}

/**
 * Remove node from hash table.
 */
static void ttl_remove_from_hash(TTLCache *cache, TTLCacheNode *node) {
    size_t bucket = ttl_hash(node->key, cache->num_buckets);
    TTLCacheNode *curr = cache->buckets[bucket];
    TTLCacheNode *prev = NULL;

    while (curr != NULL) {
        if (curr == node) {
            if (prev == NULL) {
                cache->buckets[bucket] = curr->hash_next;
            } else {
                prev->hash_next = curr->hash_next;
            }
            return;
        }
        prev = curr;
        curr = curr->hash_next;
    }
}

/**
 * Add node to hash table.
 */
static void ttl_add_to_hash(TTLCache *cache, TTLCacheNode *node) {
    size_t bucket = ttl_hash(node->key, cache->num_buckets);
    node->hash_next = cache->buckets[bucket];
    cache->buckets[bucket] = node;
}

/**
 * Remove and free a node completely.
 */
static void ttl_remove_node(TTLCache *cache, TTLCacheNode *node) {
    ttl_remove_from_list(node);
    ttl_remove_from_hash(cache, node);
    free(node);
    cache->size--;
}

/**
 * Check if node is expired.
 */
static bool ttl_is_expired(TTLCache *cache, TTLCacheNode *node) {
    return cache->time_func() > node->expire_time;
}

/**
 * Evict LRU entry (head of list after dummy).
 */
static void ttl_evict_lru(TTLCache *cache) {
    if (cache->head->next != cache->tail) {
        ttl_remove_node(cache, cache->head->next);
    }
}

/* ============== Creation and Destruction ============== */

TTLCache *ttl_create(size_t capacity, double ttl_seconds) {
    return ttl_create_with_time_func(capacity, ttl_seconds, default_time_func);
}

TTLCache *ttl_create_with_time_func(size_t capacity, double ttl_seconds,
                                     double (*time_func)(void)) {
    if (capacity == 0) {
        return NULL;
    }

    TTLCache *cache = malloc(sizeof(TTLCache));
    if (cache == NULL) {
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->default_ttl = ttl_seconds;
    cache->time_func = time_func ? time_func : default_time_func;

    /* Allocate hash table */
    cache->num_buckets = TTL_CACHE_INITIAL_BUCKETS;
    while ((double)capacity / cache->num_buckets > TTL_CACHE_LOAD_FACTOR) {
        cache->num_buckets *= 2;
    }

    cache->buckets = calloc(cache->num_buckets, sizeof(TTLCacheNode *));
    if (cache->buckets == NULL) {
        free(cache);
        return NULL;
    }

    /* Create dummy head and tail nodes for LRU list */
    cache->head = malloc(sizeof(TTLCacheNode));
    cache->tail = malloc(sizeof(TTLCacheNode));
    if (cache->head == NULL || cache->tail == NULL) {
        free(cache->head);
        free(cache->tail);
        free(cache->buckets);
        free(cache);
        return NULL;
    }

    cache->head->prev = NULL;
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;
    cache->tail->next = NULL;

    return cache;
}

void ttl_destroy(TTLCache *cache) {
    if (cache == NULL) {
        return;
    }

    ttl_clear(cache);
    free(cache->head);
    free(cache->tail);
    free(cache->buckets);
    free(cache);
}

void ttl_clear(TTLCache *cache) {
    if (cache == NULL) {
        return;
    }

    /* Free all nodes in LRU list */
    TTLCacheNode *node = cache->head->next;
    while (node != cache->tail) {
        TTLCacheNode *next = node->next;
        free(node);
        node = next;
    }

    /* Reset list */
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;

    /* Clear hash table */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        cache->buckets[i] = NULL;
    }

    cache->size = 0;
}

/* ============== Core Operations ============== */

bool ttl_get(TTLCache *cache, int key, int *value) {
    if (cache == NULL || value == NULL) {
        return false;
    }

    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node == NULL) {
        return false;
    }

    /* Check expiration */
    if (ttl_is_expired(cache, node)) {
        ttl_remove_node(cache, node);
        return false;
    }

    /* Move to tail (most recently used) */
    ttl_move_to_tail(cache, node);
    *value = node->value;
    return true;
}

bool ttl_put(TTLCache *cache, int key, int value) {
    if (cache == NULL) {
        return false;
    }
    return ttl_put_with_ttl(cache, key, value, cache->default_ttl);
}

bool ttl_put_with_ttl(TTLCache *cache, int key, int value, double ttl_seconds) {
    if (cache == NULL) {
        return false;
    }

    double expire_time = cache->time_func() + ttl_seconds;

    /* Check if key already exists */
    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node != NULL) {
        /* Update existing entry */
        node->value = value;
        node->expire_time = expire_time;
        ttl_move_to_tail(cache, node);
        return true;
    }

    /* Run cleanup periodically (when at capacity) */
    if (cache->size >= cache->capacity) {
        ttl_cleanup(cache);
    }

    /* Evict LRU if still at capacity */
    while (cache->size >= cache->capacity) {
        ttl_evict_lru(cache);
    }

    /* Create new node */
    node = ttl_create_node(key, value, expire_time);
    if (node == NULL) {
        return false;
    }

    /* Add to hash table and LRU list */
    ttl_add_to_hash(cache, node);
    ttl_add_to_tail(cache, node);
    cache->size++;

    return true;
}

bool ttl_delete(TTLCache *cache, int key) {
    if (cache == NULL) {
        return false;
    }

    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node == NULL) {
        return false;
    }

    ttl_remove_node(cache, node);
    return true;
}

bool ttl_contains(TTLCache *cache, int key) {
    if (cache == NULL) {
        return false;
    }

    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node == NULL) {
        return false;
    }

    if (ttl_is_expired(cache, node)) {
        ttl_remove_node(cache, node);
        return false;
    }

    return true;
}

/* ============== Maintenance ============== */

size_t ttl_cleanup(TTLCache *cache) {
    if (cache == NULL) {
        return 0;
    }

    size_t removed = 0;
    TTLCacheNode *node = cache->head->next;

    while (node != cache->tail) {
        TTLCacheNode *next = node->next;
        if (ttl_is_expired(cache, node)) {
            ttl_remove_node(cache, node);
            removed++;
        }
        node = next;
    }

    return removed;
}

bool ttl_get_ttl(TTLCache *cache, int key, double *remaining) {
    if (cache == NULL || remaining == NULL) {
        return false;
    }

    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node == NULL) {
        return false;
    }

    double now = cache->time_func();
    if (now > node->expire_time) {
        ttl_remove_node(cache, node);
        return false;
    }

    *remaining = node->expire_time - now;
    return true;
}

bool ttl_refresh(TTLCache *cache, int key) {
    if (cache == NULL) {
        return false;
    }

    TTLCacheNode *node = ttl_find_node(cache, key);
    if (node == NULL) {
        return false;
    }

    if (ttl_is_expired(cache, node)) {
        ttl_remove_node(cache, node);
        return false;
    }

    node->expire_time = cache->time_func() + cache->default_ttl;
    ttl_move_to_tail(cache, node);
    return true;
}

/* ============== Properties ============== */

size_t ttl_size(TTLCache *cache) {
    if (cache == NULL) {
        return 0;
    }
    ttl_cleanup(cache);
    return cache->size;
}

size_t ttl_size_dirty(const TTLCache *cache) {
    if (cache == NULL) {
        return 0;
    }
    return cache->size;
}

size_t ttl_capacity(const TTLCache *cache) {
    if (cache == NULL) {
        return 0;
    }
    return cache->capacity;
}

bool ttl_is_empty(const TTLCache *cache) {
    return cache == NULL || cache->size == 0;
}

bool ttl_is_full(const TTLCache *cache) {
    return cache != NULL && cache->size >= cache->capacity;
}
