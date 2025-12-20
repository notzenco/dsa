/**
 * SKIP LIST
 *
 * A probabilistic data structure that provides O(log n) search, insert, and delete.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Level 3:  HEAD ─────────────────────────────────────────→ 25 ──→ NIL   ║
 * ║              │                                              │             ║
 * ║   Level 2:  HEAD ─────────→ 6 ─────────────────→ 17 ───→ 25 ──→ NIL     ║
 * ║              │              │                    │         │             ║
 * ║   Level 1:  HEAD ──→ 3 ──→ 6 ──→ 9 ─────→ 12 ─→ 17 ───→ 25 ──→ NIL     ║
 * ║              │      │      │     │        │     │         │             ║
 * ║   Level 0:  HEAD ─→ 3 ──→ 6 ──→ 7 ──→ 9 ─→ 12 → 17 → 19 → 25 → NIL     ║
 * ║                                                                           ║
 * ║   Each node randomly decides its height (like flipping a coin)            ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Expected  │ Worst   │ Space                         ║
 * ║  ───────────────────┼───────────┼─────────┼──────────                     ║
 * ║  Search             │ O(log n)  │ O(n)    │ O(1)                          ║
 * ║  Insert             │ O(log n)  │ O(n)    │ O(log n)                      ║
 * ║  Delete             │ O(log n)  │ O(n)    │ O(1)                          ║
 * ║  Total Space        │ O(n)      │ O(n log n) │                            ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ADVANTAGES OVER BALANCED TREES:
 * - Simpler implementation
 * - Easy to make lock-free for concurrency
 * - Good cache locality for forward traversal
 */

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <stddef.h>
#include <stdbool.h>

#define SKIPLIST_MAX_LEVEL 32  /* Supports up to 2^32 elements */

/* ============== Skip List Node ============== */

typedef struct SkipListNode {
    int key;
    int value;
    int level;  /* Height of this node */
    struct SkipListNode **forward;  /* Array of forward pointers */
} SkipListNode;

/* ============== Skip List Structure ============== */

typedef struct {
    SkipListNode *head;
    int level;       /* Current max level of list */
    size_t size;     /* Number of elements */
    double prob;     /* Probability for level increase (typically 0.5) */
} SkipList;

/* ============== Creation/Destruction ============== */

/**
 * Create a new skip list.
 * @return New skip list or NULL on failure
 */
SkipList *skiplist_create(void);

/**
 * Create a skip list with custom probability.
 * @param prob Probability for level increase (0 < prob < 1)
 * @return New skip list or NULL on failure
 */
SkipList *skiplist_create_with_prob(double prob);

/**
 * Destroy a skip list.
 * @param list Skip list to destroy
 */
void skiplist_destroy(SkipList *list);

/**
 * Clear all elements from skip list.
 * @param list Skip list to clear
 */
void skiplist_clear(SkipList *list);

/* ============== Core Operations ============== */

/**
 * Insert a key-value pair.
 * @param list Skip list
 * @param key Key
 * @param value Value
 * @return true if inserted (new key), false if updated (existing key)
 */
bool skiplist_insert(SkipList *list, int key, int value);

/**
 * Search for a key.
 * @param list Skip list
 * @param key Key to search
 * @param value Pointer to store value if found
 * @return true if found
 */
bool skiplist_search(const SkipList *list, int key, int *value);

/**
 * Delete a key.
 * @param list Skip list
 * @param key Key to delete
 * @return true if found and deleted
 */
bool skiplist_delete(SkipList *list, int key);

/**
 * Check if key exists.
 * @param list Skip list
 * @param key Key to check
 * @return true if exists
 */
bool skiplist_contains(const SkipList *list, int key);

/* ============== Range Operations ============== */

/**
 * Get all keys in range [min_key, max_key].
 * @param list Skip list
 * @param min_key Minimum key (inclusive)
 * @param max_key Maximum key (inclusive)
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys found
 */
size_t skiplist_range(const SkipList *list, int min_key, int max_key,
                      int *keys, size_t max_keys);

/**
 * Get smallest key >= given key (ceiling).
 * @param list Skip list
 * @param key Key
 * @param result Pointer to store result
 * @return true if found
 */
bool skiplist_ceiling(const SkipList *list, int key, int *result);

/**
 * Get largest key <= given key (floor).
 * @param list Skip list
 * @param key Key
 * @param result Pointer to store result
 * @return true if found
 */
bool skiplist_floor(const SkipList *list, int key, int *result);

/* ============== Utility ============== */

/**
 * Get number of elements.
 * @param list Skip list
 * @return Number of elements
 */
size_t skiplist_size(const SkipList *list);

/**
 * Check if empty.
 * @param list Skip list
 * @return true if empty
 */
bool skiplist_is_empty(const SkipList *list);

/**
 * Get minimum key.
 * @param list Skip list
 * @param key Pointer to store minimum key
 * @return true if not empty
 */
bool skiplist_min(const SkipList *list, int *key);

/**
 * Get maximum key.
 * @param list Skip list
 * @param key Pointer to store maximum key
 * @return true if not empty
 */
bool skiplist_max(const SkipList *list, int *key);

/**
 * Get all keys in sorted order.
 * @param list Skip list
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys retrieved
 */
size_t skiplist_get_keys(const SkipList *list, int *keys, size_t max_keys);

#endif /* SKIP_LIST_H */
