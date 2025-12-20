/**
 * Hash Table Implementation
 * See hash_table.h for documentation
 */

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============== String Key Hash Table (Separate Chaining) ============== */

/* FNV-1a hash function */
static size_t ht_hash(const char *key, size_t capacity) {
    size_t hash = 14695981039346656037ULL;
    while (*key) {
        hash ^= (unsigned char)*key++;
        hash *= 1099511628211ULL;
    }
    return hash % capacity;
}

static HTEntry *ht_create_entry(const char *key, int value) {
    HTEntry *entry = malloc(sizeof(HTEntry));
    if (entry == NULL) {
        return NULL;
    }

    entry->key = strdup(key);
    if (entry->key == NULL) {
        free(entry);
        return NULL;
    }

    entry->value = value;
    entry->next = NULL;
    return entry;
}

static void ht_free_entry(HTEntry *entry) {
    if (entry != NULL) {
        free(entry->key);
        free(entry);
    }
}

static bool ht_resize(HashTable *table, size_t new_capacity) {
    HTEntry **new_buckets = calloc(new_capacity, sizeof(HTEntry *));
    if (new_buckets == NULL) {
        return false;
    }

    /* Rehash all entries */
    for (size_t i = 0; i < table->capacity; i++) {
        HTEntry *entry = table->buckets[i];
        while (entry != NULL) {
            HTEntry *next = entry->next;
            size_t new_index = ht_hash(entry->key, new_capacity);
            entry->next = new_buckets[new_index];
            new_buckets[new_index] = entry;
            entry = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->capacity = new_capacity;
    return true;
}

HashTable *ht_create(void) {
    return ht_create_with_capacity(HT_INITIAL_CAPACITY);
}

HashTable *ht_create_with_capacity(size_t capacity) {
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = HT_INITIAL_CAPACITY;
    }

    table->buckets = calloc(capacity, sizeof(HTEntry *));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = capacity;
    return table;
}

void ht_destroy(HashTable *table) {
    if (table == NULL) {
        return;
    }

    ht_clear(table);
    free(table->buckets);
    free(table);
}

bool ht_put(HashTable *table, const char *key, int value) {
    if (table == NULL || key == NULL) {
        return false;
    }

    /* Check load factor and resize if needed */
    if ((double)(table->size + 1) / table->capacity > HT_LOAD_FACTOR_THRESHOLD) {
        if (!ht_resize(table, table->capacity * HT_GROWTH_FACTOR)) {
            return false;
        }
    }

    size_t index = ht_hash(key, table->capacity);
    HTEntry *entry = table->buckets[index];

    /* Check if key already exists */
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return true;
        }
        entry = entry->next;
    }

    /* Create new entry */
    HTEntry *new_entry = ht_create_entry(key, value);
    if (new_entry == NULL) {
        return false;
    }

    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->size++;
    return true;
}

bool ht_get(const HashTable *table, const char *key, int *out_value) {
    if (table == NULL || key == NULL) {
        return false;
    }

    size_t index = ht_hash(key, table->capacity);
    HTEntry *entry = table->buckets[index];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (out_value != NULL) {
                *out_value = entry->value;
            }
            return true;
        }
        entry = entry->next;
    }

    return false;
}

bool ht_contains(const HashTable *table, const char *key) {
    return ht_get(table, key, NULL);
}

bool ht_remove(HashTable *table, const char *key) {
    if (table == NULL || key == NULL) {
        return false;
    }

    size_t index = ht_hash(key, table->capacity);
    HTEntry *entry = table->buckets[index];
    HTEntry *prev = NULL;

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                table->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            ht_free_entry(entry);
            table->size--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }

    return false;
}

size_t ht_size(const HashTable *table) {
    return table != NULL ? table->size : 0;
}

bool ht_is_empty(const HashTable *table) {
    return table == NULL || table->size == 0;
}

void ht_clear(HashTable *table) {
    if (table == NULL) {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HTEntry *entry = table->buckets[i];
        while (entry != NULL) {
            HTEntry *next = entry->next;
            ht_free_entry(entry);
            entry = next;
        }
        table->buckets[i] = NULL;
    }

    table->size = 0;
}

bool ht_keys(const HashTable *table, char ***out_keys, size_t *out_count) {
    if (table == NULL || out_keys == NULL || out_count == NULL) {
        return false;
    }

    if (table->size == 0) {
        *out_keys = NULL;
        *out_count = 0;
        return true;
    }

    *out_keys = malloc(table->size * sizeof(char *));
    if (*out_keys == NULL) {
        return false;
    }

    size_t idx = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        HTEntry *entry = table->buckets[i];
        while (entry != NULL) {
            (*out_keys)[idx++] = entry->key;
            entry = entry->next;
        }
    }

    *out_count = table->size;
    return true;
}

bool ht_values(const HashTable *table, int **out_values, size_t *out_count) {
    if (table == NULL || out_values == NULL || out_count == NULL) {
        return false;
    }

    if (table->size == 0) {
        *out_values = NULL;
        *out_count = 0;
        return true;
    }

    *out_values = malloc(table->size * sizeof(int));
    if (*out_values == NULL) {
        return false;
    }

    size_t idx = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        HTEntry *entry = table->buckets[i];
        while (entry != NULL) {
            (*out_values)[idx++] = entry->value;
            entry = entry->next;
        }
    }

    *out_count = table->size;
    return true;
}

double ht_load_factor(const HashTable *table) {
    if (table == NULL || table->capacity == 0) {
        return 0.0;
    }
    return (double)table->size / table->capacity;
}

void ht_print(const HashTable *table) {
    if (table == NULL) {
        printf("NULL\n");
        return;
    }

    printf("HashTable (size=%lu, capacity=%lu, load=%.2f):\n",
           (unsigned long)table->size, (unsigned long)table->capacity,
           ht_load_factor(table));

    for (size_t i = 0; i < table->capacity; i++) {
        if (table->buckets[i] != NULL) {
            printf("  [%lu]: ", (unsigned long)i);
            HTEntry *entry = table->buckets[i];
            while (entry != NULL) {
                printf("(%s:%d)", entry->key, entry->value);
                if (entry->next != NULL) {
                    printf(" -> ");
                }
                entry = entry->next;
            }
            printf("\n");
        }
    }
}

/* ============== Integer Key Hash Table (Open Addressing) ============== */

static size_t ht_int_hash(int key, size_t capacity) {
    /* Simple integer hash */
    size_t hash = (size_t)key;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;
    return hash % capacity;
}

static bool ht_int_resize(HashTableInt *table, size_t new_capacity) {
    HTIntEntry *new_entries = calloc(new_capacity, sizeof(HTIntEntry));
    if (new_entries == NULL) {
        return false;
    }

    /* Rehash all entries */
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].occupied && !table->entries[i].deleted) {
            int key = table->entries[i].key;
            int value = table->entries[i].value;

            size_t index = ht_int_hash(key, new_capacity);
            while (new_entries[index].occupied) {
                index = (index + 1) % new_capacity;
            }

            new_entries[index].key = key;
            new_entries[index].value = value;
            new_entries[index].occupied = true;
            new_entries[index].deleted = false;
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

HashTableInt *ht_int_create(void) {
    HashTableInt *table = malloc(sizeof(HashTableInt));
    if (table == NULL) {
        return NULL;
    }

    table->entries = calloc(HT_INITIAL_CAPACITY, sizeof(HTIntEntry));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = HT_INITIAL_CAPACITY;
    return table;
}

void ht_int_destroy(HashTableInt *table) {
    if (table != NULL) {
        free(table->entries);
        free(table);
    }
}

bool ht_int_put(HashTableInt *table, int key, int value) {
    if (table == NULL) {
        return false;
    }

    /* Check load factor */
    if ((double)(table->size + 1) / table->capacity > HT_LOAD_FACTOR_THRESHOLD) {
        if (!ht_int_resize(table, table->capacity * HT_GROWTH_FACTOR)) {
            return false;
        }
    }

    size_t index = ht_int_hash(key, table->capacity);
    size_t start = index;

    do {
        if (!table->entries[index].occupied || table->entries[index].deleted) {
            table->entries[index].key = key;
            table->entries[index].value = value;
            table->entries[index].occupied = true;
            table->entries[index].deleted = false;
            table->size++;
            return true;
        }

        if (table->entries[index].key == key) {
            table->entries[index].value = value;
            return true;
        }

        index = (index + 1) % table->capacity;
    } while (index != start);

    return false;
}

bool ht_int_get(const HashTableInt *table, int key, int *out_value) {
    if (table == NULL) {
        return false;
    }

    size_t index = ht_int_hash(key, table->capacity);
    size_t start = index;

    do {
        if (!table->entries[index].occupied) {
            return false;
        }

        if (!table->entries[index].deleted && table->entries[index].key == key) {
            if (out_value != NULL) {
                *out_value = table->entries[index].value;
            }
            return true;
        }

        index = (index + 1) % table->capacity;
    } while (index != start);

    return false;
}

bool ht_int_contains(const HashTableInt *table, int key) {
    return ht_int_get(table, key, NULL);
}

bool ht_int_remove(HashTableInt *table, int key) {
    if (table == NULL) {
        return false;
    }

    size_t index = ht_int_hash(key, table->capacity);
    size_t start = index;

    do {
        if (!table->entries[index].occupied) {
            return false;
        }

        if (!table->entries[index].deleted && table->entries[index].key == key) {
            table->entries[index].deleted = true;
            table->size--;
            return true;
        }

        index = (index + 1) % table->capacity;
    } while (index != start);

    return false;
}

size_t ht_int_size(const HashTableInt *table) {
    return table != NULL ? table->size : 0;
}

bool ht_int_is_empty(const HashTableInt *table) {
    return table == NULL || table->size == 0;
}

void ht_int_clear(HashTableInt *table) {
    if (table == NULL) {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        table->entries[i].occupied = false;
        table->entries[i].deleted = false;
    }

    table->size = 0;
}
