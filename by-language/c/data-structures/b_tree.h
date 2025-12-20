/**
 * B-TREE
 *
 * Self-balancing tree optimized for disk-based storage systems.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                           B-TREE PROPERTIES                               ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  For a B-Tree of minimum degree t:                                        ║
 * ║  1. Every node has at most 2t - 1 keys                                    ║
 * ║  2. Every node (except root) has at least t - 1 keys                      ║
 * ║  3. Root has at least 1 key (if non-empty)                                ║
 * ║  4. All leaves appear at the same level                                   ║
 * ║  5. A node with n keys has n + 1 children                                 ║
 * ║                                                                           ║
 * ║  Example (t=2, also called 2-3-4 tree):                                   ║
 * ║                      [10, 20]                                             ║
 * ║                    /    |    \                                            ║
 * ║              [5,7]    [15]   [25,30,35]                                   ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              COMPLEXITY                                   ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║  Operation      │ Time            │ Disk I/O        │ Notes              ║
 * ║  ───────────────┼─────────────────┼─────────────────┼─────────────────   ║
 * ║  Search         │ O(t * log_t(n)) │ O(log_t(n))     │ t = min degree     ║
 * ║  Insert         │ O(t * log_t(n)) │ O(log_t(n))     │ May split nodes    ║
 * ║  Delete         │ O(t * log_t(n)) │ O(log_t(n))     │ May merge nodes    ║
 * ║  Space          │ O(n)            │                 │                    ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * USE CASES:
 * - Database indexing (most RDBMS use B+ trees)
 * - File systems (NTFS, ext4, HFS+)
 * - Key-value stores
 * - Any application with slow disk I/O
 *
 * LEETCODE PROBLEMS:
 * - Rarely tested directly, but concepts used in system design
 */

#ifndef B_TREE_H
#define B_TREE_H

#include <stddef.h>
#include <stdbool.h>

/* Default minimum degree (t=3 means 2-5 keys per node) */
#define BTREE_DEFAULT_MIN_DEGREE 3

/* B-Tree node structure */
typedef struct BTreeNode {
    int *keys;              /* Array of keys */
    int *values;            /* Array of values */
    struct BTreeNode **children;  /* Array of child pointers */
    int num_keys;           /* Current number of keys */
    bool is_leaf;           /* True if this is a leaf node */
} BTreeNode;

/* B-Tree structure */
typedef struct {
    BTreeNode *root;
    int min_degree;         /* Minimum degree (t) */
    size_t size;            /* Total number of keys */
} BTree;

/* ============== Creation/Destruction ============== */

/**
 * Create a new B-Tree with specified minimum degree.
 * @param min_degree Minimum degree t (must be >= 2)
 * @return New tree or NULL on failure
 */
BTree *btree_create(int min_degree);

/**
 * Create a new B-Tree with default minimum degree.
 * @return New tree or NULL on failure
 */
BTree *btree_create_default(void);

/**
 * Destroy tree and free all memory.
 * @param tree Tree to destroy
 */
void btree_destroy(BTree *tree);

/**
 * Remove all keys from tree.
 * @param tree Tree to clear
 */
void btree_clear(BTree *tree);

/* ============== Basic Operations ============== */

/**
 * Insert a key-value pair.
 * @param tree Tree
 * @param key Key to insert
 * @param value Value to associate
 * @return true if new key inserted, false if key existed (value updated)
 */
bool btree_insert(BTree *tree, int key, int value);

/**
 * Search for a key.
 * @param tree Tree
 * @param key Key to find
 * @param value_out Pointer to store value (can be NULL)
 * @return true if found
 */
bool btree_search(const BTree *tree, int key, int *value_out);

/**
 * Delete a key.
 * @param tree Tree
 * @param key Key to delete
 * @return true if key existed and was deleted
 */
bool btree_delete(BTree *tree, int key);

/**
 * Check if key exists.
 * @param tree Tree
 * @param key Key to check
 * @return true if key exists
 */
bool btree_contains(const BTree *tree, int key);

/* ============== Size Operations ============== */

/**
 * Get number of keys.
 * @param tree Tree
 * @return Number of keys
 */
size_t btree_size(const BTree *tree);

/**
 * Check if tree is empty.
 * @param tree Tree
 * @return true if empty
 */
bool btree_is_empty(const BTree *tree);

/**
 * Get height of tree.
 * @param tree Tree
 * @return Height (0 for empty tree)
 */
int btree_height(const BTree *tree);

/* ============== Min/Max Operations ============== */

/**
 * Get minimum key.
 * @param tree Tree
 * @param key_out Pointer to store minimum key
 * @return true if tree is not empty
 */
bool btree_min(const BTree *tree, int *key_out);

/**
 * Get maximum key.
 * @param tree Tree
 * @param key_out Pointer to store maximum key
 * @return true if tree is not empty
 */
bool btree_max(const BTree *tree, int *key_out);

/* ============== Traversal ============== */

/**
 * Get keys in sorted order (in-order traversal).
 * @param tree Tree
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys stored
 */
size_t btree_inorder(const BTree *tree, int *keys, size_t max_keys);

/**
 * Get keys in range [min_key, max_key].
 * @param tree Tree
 * @param min_key Minimum key (inclusive)
 * @param max_key Maximum key (inclusive)
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys in range
 */
size_t btree_range(const BTree *tree, int min_key, int max_key,
                   int *keys, size_t max_keys);

/* ============== Validation ============== */

/**
 * Validate B-Tree properties.
 * @param tree Tree to validate
 * @return true if all properties hold
 */
bool btree_validate(const BTree *tree);

/* ============== Debug ============== */

/**
 * Print tree structure.
 * @param tree Tree to print
 */
void btree_print(const BTree *tree);

#endif /* B_TREE_H */
