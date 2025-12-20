/**
 * LRU Cache Implementation
 * See lru_cache.h for documentation
 */

#include "lru_cache.h"
#include <stdlib.h>
#include <string.h>

/* ============== Hash Function ============== */

static size_t hash_key(int key, size_t num_buckets) {
    /* Simple hash for integers */
    unsigned int ukey = (unsigned int)key;
    ukey = ((ukey >> 16) ^ ukey) * 0x45d9f3b;
    ukey = ((ukey >> 16) ^ ukey) * 0x45d9f3b;
    ukey = (ukey >> 16) ^ ukey;
    return ukey % num_buckets;
}

/* ============== Node Operations ============== */

static LRUNode *create_node(int key, int value) {
    LRUNode *node = malloc(sizeof(LRUNode));
    if (node) {
        node->key = key;
        node->value = value;
        node->prev = NULL;
        node->next = NULL;
    }
    return node;
}

static void remove_node(LRUNode *node) {
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
}

static void insert_after(LRUNode *prev_node, LRUNode *new_node) {
    new_node->prev = prev_node;
    new_node->next = prev_node->next;
    if (prev_node->next) prev_node->next->prev = new_node;
    prev_node->next = new_node;
}

/* ============== Hash Table Operations ============== */

static LRUEntry *find_entry(const LRUCache *cache, int key) {
    size_t bucket = hash_key(key, cache->num_buckets);
    LRUEntry *entry = cache->buckets[bucket];

    while (entry != NULL) {
        if (entry->key == key) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static void insert_entry(LRUCache *cache, int key, LRUNode *node) {
    size_t bucket = hash_key(key, cache->num_buckets);

    LRUEntry *entry = malloc(sizeof(LRUEntry));
    if (entry == NULL) return;

    entry->key = key;
    entry->node = node;
    entry->next = cache->buckets[bucket];
    cache->buckets[bucket] = entry;
}

static void remove_entry(LRUCache *cache, int key) {
    size_t bucket = hash_key(key, cache->num_buckets);
    LRUEntry **pp = &cache->buckets[bucket];

    while (*pp != NULL) {
        if ((*pp)->key == key) {
            LRUEntry *to_delete = *pp;
            *pp = to_delete->next;
            free(to_delete);
            return;
        }
        pp = &(*pp)->next;
    }
}

/* ============== Creation/Destruction ============== */

LRUCache *lru_create(size_t capacity) {
    if (capacity == 0) return NULL;

    LRUCache *cache = malloc(sizeof(LRUCache));
    if (cache == NULL) return NULL;

    cache->capacity = capacity;
    cache->size = 0;

    /* Create dummy head and tail */
    cache->head = create_node(0, 0);
    cache->tail = create_node(0, 0);

    if (cache->head == NULL || cache->tail == NULL) {
        free(cache->head);
        free(cache->tail);
        free(cache);
        return NULL;
    }

    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;

    /* Create hash table */
    cache->num_buckets = capacity * 2;  /* Load factor 0.5 */
    cache->buckets = calloc(cache->num_buckets, sizeof(LRUEntry *));

    if (cache->buckets == NULL) {
        free(cache->head);
        free(cache->tail);
        free(cache);
        return NULL;
    }

    return cache;
}

void lru_destroy(LRUCache *cache) {
    if (cache == NULL) return;

    /* Free all nodes */
    LRUNode *node = cache->head;
    while (node != NULL) {
        LRUNode *next = node->next;
        free(node);
        node = next;
    }

    /* Free hash table entries */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        LRUEntry *entry = cache->buckets[i];
        while (entry != NULL) {
            LRUEntry *next = entry->next;
            free(entry);
            entry = next;
        }
    }

    free(cache->buckets);
    free(cache);
}

void lru_clear(LRUCache *cache) {
    if (cache == NULL) return;

    /* Free all nodes except dummies */
    LRUNode *node = cache->head->next;
    while (node != cache->tail) {
        LRUNode *next = node->next;
        free(node);
        node = next;
    }

    /* Reset dummy links */
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;

    /* Clear hash table */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        LRUEntry *entry = cache->buckets[i];
        while (entry != NULL) {
            LRUEntry *next = entry->next;
            free(entry);
            entry = next;
        }
        cache->buckets[i] = NULL;
    }

    cache->size = 0;
}

/* ============== Core Operations ============== */

bool lru_get(LRUCache *cache, int key, int *value) {
    if (cache == NULL || value == NULL) return false;

    LRUEntry *entry = find_entry(cache, key);
    if (entry == NULL) return false;

    *value = entry->node->value;

    /* Move to front (most recently used) */
    remove_node(entry->node);
    insert_after(cache->head, entry->node);

    return true;
}

bool lru_put(LRUCache *cache, int key, int value) {
    if (cache == NULL) return false;

    LRUEntry *entry = find_entry(cache, key);

    if (entry != NULL) {
        /* Update existing key */
        entry->node->value = value;
        remove_node(entry->node);
        insert_after(cache->head, entry->node);
        return true;
    }

    /* Check capacity and evict if needed */
    if (cache->size >= cache->capacity) {
        /* Remove least recently used (before tail) */
        LRUNode *lru = cache->tail->prev;
        if (lru != cache->head) {
            remove_node(lru);
            remove_entry(cache, lru->key);
            free(lru);
            cache->size--;
        }
    }

    /* Create new node */
    LRUNode *node = create_node(key, value);
    if (node == NULL) return false;

    /* Insert at front */
    insert_after(cache->head, node);
    insert_entry(cache, key, node);
    cache->size++;

    return true;
}

bool lru_delete(LRUCache *cache, int key) {
    if (cache == NULL) return false;

    LRUEntry *entry = find_entry(cache, key);
    if (entry == NULL) return false;

    remove_node(entry->node);
    free(entry->node);
    remove_entry(cache, key);
    cache->size--;

    return true;
}

bool lru_contains(const LRUCache *cache, int key) {
    if (cache == NULL) return false;
    return find_entry(cache, key) != NULL;
}

/* ============== Cache Properties ============== */

size_t lru_size(const LRUCache *cache) {
    return cache ? cache->size : 0;
}

size_t lru_capacity(const LRUCache *cache) {
    return cache ? cache->capacity : 0;
}

bool lru_is_empty(const LRUCache *cache) {
    return cache == NULL || cache->size == 0;
}

bool lru_is_full(const LRUCache *cache) {
    return cache != NULL && cache->size >= cache->capacity;
}

/* ============== Inspection ============== */

bool lru_peek_newest(const LRUCache *cache, int *key) {
    if (cache == NULL || key == NULL || cache->size == 0) return false;

    *key = cache->head->next->key;
    return true;
}

bool lru_peek_oldest(const LRUCache *cache, int *key) {
    if (cache == NULL || key == NULL || cache->size == 0) return false;

    *key = cache->tail->prev->key;
    return true;
}

size_t lru_get_keys(const LRUCache *cache, int *keys, size_t max_keys) {
    if (cache == NULL || keys == NULL || max_keys == 0) return 0;

    size_t count = 0;
    LRUNode *node = cache->head->next;

    while (node != cache->tail && count < max_keys) {
        keys[count++] = node->key;
        node = node->next;
    }

    return count;
}
