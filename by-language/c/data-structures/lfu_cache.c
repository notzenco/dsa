/**
 * LFU Cache Implementation
 * See lfu_cache.h for documentation
 */

#include "lfu_cache.h"
#include <stdlib.h>
#include <string.h>

/* ============== Hash Functions ============== */

static size_t hash_key(int key, size_t num_buckets) {
    unsigned int ukey = (unsigned int)key;
    ukey = ((ukey >> 16) ^ ukey) * 0x45d9f3b;
    ukey = ((ukey >> 16) ^ ukey) * 0x45d9f3b;
    ukey = (ukey >> 16) ^ ukey;
    return ukey % num_buckets;
}

static size_t hash_freq(int freq, size_t num_buckets) {
    return (size_t)freq % num_buckets;
}

/* ============== Node Operations ============== */

static LFUNode *create_node(int key, int value) {
    LFUNode *node = malloc(sizeof(LFUNode));
    if (node) {
        node->key = key;
        node->value = value;
        node->freq = 1;
        node->prev = NULL;
        node->next = NULL;
    }
    return node;
}

static void remove_node(LFUNode *node) {
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;
}

/* ============== Frequency List Operations ============== */

static FreqList *create_freq_list(int freq) {
    FreqList *list = malloc(sizeof(FreqList));
    if (list == NULL) return NULL;

    list->freq = freq;
    list->head = malloc(sizeof(LFUNode));  /* Dummy head */
    list->tail = malloc(sizeof(LFUNode));  /* Dummy tail */

    if (list->head == NULL || list->tail == NULL) {
        free(list->head);
        free(list->tail);
        free(list);
        return NULL;
    }

    list->head->prev = NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->tail->next = NULL;
    list->size = 0;
    list->next = NULL;

    return list;
}

static void destroy_freq_list(FreqList *list) {
    if (list == NULL) return;

    LFUNode *node = list->head;
    while (node != NULL) {
        LFUNode *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

static void insert_node_front(FreqList *list, LFUNode *node) {
    node->prev = list->head;
    node->next = list->head->next;
    list->head->next->prev = node;
    list->head->next = node;
    list->size++;
}

static LFUNode *remove_tail_node(FreqList *list) {
    if (list->size == 0) return NULL;

    LFUNode *node = list->tail->prev;
    remove_node(node);
    list->size--;
    return node;
}

static void remove_node_from_list(FreqList *list, LFUNode *node) {
    remove_node(node);
    list->size--;
}

/* ============== Hash Map Operations ============== */

static LFUEntry *find_key_entry(const LFUCache *cache, int key) {
    size_t bucket = hash_key(key, cache->num_buckets);
    LFUEntry *entry = cache->key_map[bucket];

    while (entry != NULL) {
        if (entry->key == key) return entry;
        entry = entry->next;
    }
    return NULL;
}

static void insert_key_entry(LFUCache *cache, int key, LFUNode *node) {
    size_t bucket = hash_key(key, cache->num_buckets);

    LFUEntry *entry = malloc(sizeof(LFUEntry));
    if (entry == NULL) return;

    entry->key = key;
    entry->node = node;
    entry->next = cache->key_map[bucket];
    cache->key_map[bucket] = entry;
}

static void remove_key_entry(LFUCache *cache, int key) {
    size_t bucket = hash_key(key, cache->num_buckets);
    LFUEntry **pp = &cache->key_map[bucket];

    while (*pp != NULL) {
        if ((*pp)->key == key) {
            LFUEntry *to_delete = *pp;
            *pp = to_delete->next;
            free(to_delete);
            return;
        }
        pp = &(*pp)->next;
    }
}

static FreqList *find_freq_list(const LFUCache *cache, int freq) {
    size_t bucket = hash_freq(freq, cache->num_buckets);
    FreqList *list = cache->freq_map[bucket];

    while (list != NULL) {
        if (list->freq == freq) return list;
        list = list->next;
    }
    return NULL;
}

static FreqList *get_or_create_freq_list(LFUCache *cache, int freq) {
    FreqList *list = find_freq_list(cache, freq);
    if (list != NULL) return list;

    list = create_freq_list(freq);
    if (list == NULL) return NULL;

    size_t bucket = hash_freq(freq, cache->num_buckets);
    list->next = cache->freq_map[bucket];
    cache->freq_map[bucket] = list;

    return list;
}

/* ============== Creation/Destruction ============== */

LFUCache *lfu_create(size_t capacity) {
    if (capacity == 0) return NULL;

    LFUCache *cache = malloc(sizeof(LFUCache));
    if (cache == NULL) return NULL;

    cache->capacity = capacity;
    cache->size = 0;
    cache->min_freq = 0;
    cache->num_buckets = capacity * 2;

    cache->key_map = calloc(cache->num_buckets, sizeof(LFUEntry *));
    cache->freq_map = calloc(cache->num_buckets, sizeof(FreqList *));

    if (cache->key_map == NULL || cache->freq_map == NULL) {
        free(cache->key_map);
        free(cache->freq_map);
        free(cache);
        return NULL;
    }

    return cache;
}

void lfu_destroy(LFUCache *cache) {
    if (cache == NULL) return;

    /* Free key map entries */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        LFUEntry *entry = cache->key_map[i];
        while (entry != NULL) {
            LFUEntry *next = entry->next;
            free(entry);
            entry = next;
        }
    }

    /* Free freq map and all nodes */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        FreqList *list = cache->freq_map[i];
        while (list != NULL) {
            FreqList *next = list->next;
            destroy_freq_list(list);
            list = next;
        }
    }

    free(cache->key_map);
    free(cache->freq_map);
    free(cache);
}

void lfu_clear(LFUCache *cache) {
    if (cache == NULL) return;

    /* Free key map entries */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        LFUEntry *entry = cache->key_map[i];
        while (entry != NULL) {
            LFUEntry *next = entry->next;
            free(entry);
            entry = next;
        }
        cache->key_map[i] = NULL;
    }

    /* Free freq map and all nodes */
    for (size_t i = 0; i < cache->num_buckets; i++) {
        FreqList *list = cache->freq_map[i];
        while (list != NULL) {
            FreqList *next = list->next;
            destroy_freq_list(list);
            list = next;
        }
        cache->freq_map[i] = NULL;
    }

    cache->size = 0;
    cache->min_freq = 0;
}

/* ============== Core Operations ============== */

static void update_frequency(LFUCache *cache, LFUNode *node) {
    int old_freq = node->freq;
    int new_freq = old_freq + 1;

    /* Remove from old frequency list */
    FreqList *old_list = find_freq_list(cache, old_freq);
    if (old_list != NULL) {
        remove_node_from_list(old_list, node);

        /* Update min_freq if needed */
        if (old_list->size == 0 && cache->min_freq == old_freq) {
            cache->min_freq = new_freq;
        }
    }

    /* Add to new frequency list */
    node->freq = new_freq;
    FreqList *new_list = get_or_create_freq_list(cache, new_freq);
    if (new_list != NULL) {
        insert_node_front(new_list, node);
    }
}

bool lfu_get(LFUCache *cache, int key, int *value) {
    if (cache == NULL || value == NULL) return false;

    LFUEntry *entry = find_key_entry(cache, key);
    if (entry == NULL) return false;

    *value = entry->node->value;
    update_frequency(cache, entry->node);

    return true;
}

bool lfu_put(LFUCache *cache, int key, int value) {
    if (cache == NULL) return false;

    LFUEntry *entry = find_key_entry(cache, key);

    if (entry != NULL) {
        /* Update existing key */
        entry->node->value = value;
        update_frequency(cache, entry->node);
        return true;
    }

    /* Evict if at capacity */
    if (cache->size >= cache->capacity) {
        FreqList *min_list = find_freq_list(cache, cache->min_freq);
        if (min_list != NULL && min_list->size > 0) {
            LFUNode *lfu_node = remove_tail_node(min_list);
            if (lfu_node != NULL) {
                remove_key_entry(cache, lfu_node->key);
                free(lfu_node);
                cache->size--;
            }
        }
    }

    /* Create new node */
    LFUNode *node = create_node(key, value);
    if (node == NULL) return false;

    /* Add to frequency 1 list */
    FreqList *list = get_or_create_freq_list(cache, 1);
    if (list == NULL) {
        free(node);
        return false;
    }

    insert_node_front(list, node);
    insert_key_entry(cache, key, node);
    cache->min_freq = 1;
    cache->size++;

    return true;
}

bool lfu_delete(LFUCache *cache, int key) {
    if (cache == NULL) return false;

    LFUEntry *entry = find_key_entry(cache, key);
    if (entry == NULL) return false;

    LFUNode *node = entry->node;
    FreqList *list = find_freq_list(cache, node->freq);

    if (list != NULL) {
        remove_node_from_list(list, node);
    }

    remove_key_entry(cache, key);
    free(node);
    cache->size--;

    return true;
}

bool lfu_contains(const LFUCache *cache, int key) {
    if (cache == NULL) return false;
    return find_key_entry(cache, key) != NULL;
}

/* ============== Cache Properties ============== */

size_t lfu_size(const LFUCache *cache) {
    return cache ? cache->size : 0;
}

size_t lfu_capacity(const LFUCache *cache) {
    return cache ? cache->capacity : 0;
}

bool lfu_is_empty(const LFUCache *cache) {
    return cache == NULL || cache->size == 0;
}

bool lfu_is_full(const LFUCache *cache) {
    return cache != NULL && cache->size >= cache->capacity;
}

/* ============== Inspection ============== */

int lfu_get_frequency(const LFUCache *cache, int key) {
    if (cache == NULL) return -1;

    LFUEntry *entry = find_key_entry(cache, key);
    if (entry == NULL) return -1;

    return entry->node->freq;
}

int lfu_min_frequency(const LFUCache *cache) {
    if (cache == NULL || cache->size == 0) return 0;
    return cache->min_freq;
}
