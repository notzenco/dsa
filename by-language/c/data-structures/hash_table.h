/**
 * HASH TABLE (Hash Map)
 *
 * A data structure that maps keys to values using a hash function.
 * Uses separate chaining for collision resolution.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Hash Table with Separate Chaining:                                       ║
 * ║                                                                           ║
 * ║  Index   Bucket (Linked List)                                             ║
 * ║  ┌───┐                                                                    ║
 * ║  │ 0 │──► [key1:val1] ──► [key2:val2] ──► NULL                           ║
 * ║  ├───┤                                                                    ║
 * ║  │ 1 │──► NULL                                                            ║
 * ║  ├───┤                                                                    ║
 * ║  │ 2 │──► [key3:val3] ──► NULL                                           ║
 * ║  ├───┤                                                                    ║
 * ║  │ 3 │──► [key4:val4] ──► [key5:val5] ──► [key6:val6] ──► NULL           ║
 * ║  ├───┤                                                                    ║
 * ║  │...│                                                                    ║
 * ║  └───┘                                                                    ║
 * ║                                                                           ║
 * ║  Hash Function: index = hash(key) % capacity                              ║
 * ║  Load Factor: size / capacity (resize when > 0.75)                        ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Insert             │ O(1)         │ O(n)       │ O(1)    │
 * │ Search             │ O(1)         │ O(n)       │ O(1)    │
 * │ Delete             │ O(1)         │ O(n)       │ O(1)    │
 * │ Resize             │ O(n)         │ O(n)       │ O(n)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * Note: Worst case occurs with many collisions
 *
 * LEETCODE PROBLEMS:
 * - #1 Two Sum
 * - #49 Group Anagrams
 * - #128 Longest Consecutive Sequence
 * - #146 LRU Cache
 * - #380 Insert Delete GetRandom O(1)
 *
 * USE CASES:
 * - Symbol tables in compilers
 * - Database indexing
 * - Caching (memoization)
 * - Counting frequencies
 * - Detecting duplicates
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>

#define HT_INITIAL_CAPACITY 16
#define HT_LOAD_FACTOR_THRESHOLD 0.75
#define HT_GROWTH_FACTOR 2

/**
 * Hash table entry (for separate chaining)
 */
typedef struct HTEntry {
    char *key;
    int value;
    struct HTEntry *next;
} HTEntry;

/**
 * Hash Table structure
 */
typedef struct {
    HTEntry **buckets;
    size_t size;
    size_t capacity;
} HashTable;

/**
 * Create a new empty hash table.
 * @return Pointer to new hash table, or NULL on allocation failure
 */
HashTable *ht_create(void);

/**
 * Create a new hash table with specified capacity.
 * @param capacity Initial capacity
 * @return Pointer to new hash table, or NULL on allocation failure
 */
HashTable *ht_create_with_capacity(size_t capacity);

/**
 * Free all memory associated with the hash table.
 * @param table Pointer to the hash table
 */
void ht_destroy(HashTable *table);

/**
 * Insert or update a key-value pair.
 * @param table Pointer to the hash table
 * @param key Key string (will be copied)
 * @param value Value to store
 * @return true if successful, false on allocation failure
 */
bool ht_put(HashTable *table, const char *key, int value);

/**
 * Get the value associated with a key.
 * @param table Pointer to the hash table
 * @param key Key to look up
 * @param out_value Pointer to store the value
 * @return true if found, false otherwise
 */
bool ht_get(const HashTable *table, const char *key, int *out_value);

/**
 * Check if a key exists in the hash table.
 * @param table Pointer to the hash table
 * @param key Key to check
 * @return true if exists, false otherwise
 */
bool ht_contains(const HashTable *table, const char *key);

/**
 * Remove a key-value pair.
 * @param table Pointer to the hash table
 * @param key Key to remove
 * @return true if removed, false if key not found
 */
bool ht_remove(HashTable *table, const char *key);

/**
 * Get the number of entries in the hash table.
 * @param table Pointer to the hash table
 * @return Number of entries
 */
size_t ht_size(const HashTable *table);

/**
 * Check if the hash table is empty.
 * @param table Pointer to the hash table
 * @return true if empty, false otherwise
 */
bool ht_is_empty(const HashTable *table);

/**
 * Clear all entries from the hash table.
 * @param table Pointer to the hash table
 */
void ht_clear(HashTable *table);

/**
 * Get all keys in the hash table.
 * @param table Pointer to the hash table
 * @param out_keys Pointer to store allocated array of key pointers
 * @param out_count Pointer to store the count
 * @return true if successful, false on allocation failure
 */
bool ht_keys(const HashTable *table, char ***out_keys, size_t *out_count);

/**
 * Get all values in the hash table.
 * @param table Pointer to the hash table
 * @param out_values Pointer to store allocated array of values
 * @param out_count Pointer to store the count
 * @return true if successful, false on allocation failure
 */
bool ht_values(const HashTable *table, int **out_values, size_t *out_count);

/**
 * Get the current load factor.
 * @param table Pointer to the hash table
 * @return Load factor (size / capacity)
 */
double ht_load_factor(const HashTable *table);

/**
 * Print the hash table contents (for debugging).
 * @param table Pointer to the hash table
 */
void ht_print(const HashTable *table);

/* ============== Integer Key Hash Table ============== */

/**
 * Integer key hash table entry
 */
typedef struct HTIntEntry {
    int key;
    int value;
    bool occupied;
    bool deleted;
} HTIntEntry;

/**
 * Integer Key Hash Table (Open Addressing with Linear Probing)
 */
typedef struct {
    HTIntEntry *entries;
    size_t size;
    size_t capacity;
} HashTableInt;

/**
 * Create a new integer key hash table.
 * @return Pointer to new hash table, or NULL on allocation failure
 */
HashTableInt *ht_int_create(void);

/**
 * Free all memory associated with the integer hash table.
 * @param table Pointer to the hash table
 */
void ht_int_destroy(HashTableInt *table);

/**
 * Insert or update a key-value pair.
 * @param table Pointer to the hash table
 * @param key Integer key
 * @param value Value to store
 * @return true if successful, false on allocation failure
 */
bool ht_int_put(HashTableInt *table, int key, int value);

/**
 * Get the value associated with a key.
 * @param table Pointer to the hash table
 * @param key Key to look up
 * @param out_value Pointer to store the value
 * @return true if found, false otherwise
 */
bool ht_int_get(const HashTableInt *table, int key, int *out_value);

/**
 * Check if a key exists.
 * @param table Pointer to the hash table
 * @param key Key to check
 * @return true if exists, false otherwise
 */
bool ht_int_contains(const HashTableInt *table, int key);

/**
 * Remove a key-value pair.
 * @param table Pointer to the hash table
 * @param key Key to remove
 * @return true if removed, false if not found
 */
bool ht_int_remove(HashTableInt *table, int key);

/**
 * Get the number of entries.
 * @param table Pointer to the hash table
 * @return Number of entries
 */
size_t ht_int_size(const HashTableInt *table);

/**
 * Check if empty.
 * @param table Pointer to the hash table
 * @return true if empty, false otherwise
 */
bool ht_int_is_empty(const HashTableInt *table);

/**
 * Clear all entries.
 * @param table Pointer to the hash table
 */
void ht_int_clear(HashTableInt *table);

#endif /* HASH_TABLE_H */
