/**
 * RED-BLACK TREE
 *
 * Self-balancing binary search tree with color properties.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         RED-BLACK TREE PROPERTIES                         ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  1. Every node is either RED or BLACK                                     ║
 * ║  2. Root is always BLACK                                                  ║
 * ║  3. All leaves (NIL) are BLACK                                            ║
 * ║  4. If a node is RED, both children are BLACK                             ║
 * ║  5. Every path from node to descendant NIL has same BLACK count           ║
 * ║                                                                           ║
 * ║  Height: h ≤ 2 * log(n + 1)                                               ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              COMPLEXITY                                   ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║  Operation      │ Average     │ Worst       │ Notes                       ║
 * ║  ───────────────┼─────────────┼─────────────┼──────────────────────────   ║
 * ║  Search         │ O(log n)    │ O(log n)    │ Guaranteed balance          ║
 * ║  Insert         │ O(log n)    │ O(log n)    │ At most 2 rotations         ║
 * ║  Delete         │ O(log n)    │ O(log n)    │ At most 3 rotations         ║
 * ║  Min/Max        │ O(log n)    │ O(log n)    │                             ║
 * ║  Space          │ O(n)        │ O(n)        │ +1 bit per node for color   ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPARISON WITH AVL:
 * ┌─────────────────┬─────────────────┬─────────────────┐
 * │ Aspect          │ AVL Tree        │ Red-Black Tree  │
 * ├─────────────────┼─────────────────┼─────────────────┤
 * │ Balance         │ Stricter        │ Looser          │
 * │ Height          │ ~1.44 log n     │ ~2 log n        │
 * │ Lookups         │ Faster          │ Slower          │
 * │ Insertions      │ More rotations  │ Fewer rotations │
 * │ Use Case        │ Read-heavy      │ Write-heavy     │
 * └─────────────────┴─────────────────┴─────────────────┘
 *
 * LEETCODE PROBLEMS:
 * - Used internally by std::map, std::set, TreeMap, TreeSet
 * - Foundation for interval trees, order statistic trees
 */

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stddef.h>
#include <stdbool.h>

/* Node colors */
typedef enum {
    RB_RED,
    RB_BLACK
} RBColor;

/* Red-Black Tree node */
typedef struct RBNode {
    int key;
    int value;
    RBColor color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

/* Red-Black Tree structure */
typedef struct {
    RBNode *root;
    RBNode *nil;    /* Sentinel node for leaves */
    size_t size;
} RBTree;

/* ============== Creation/Destruction ============== */

/**
 * Create a new Red-Black Tree.
 * @return New tree or NULL on failure
 */
RBTree *rbtree_create(void);

/**
 * Destroy tree and free all memory.
 * @param tree Tree to destroy
 */
void rbtree_destroy(RBTree *tree);

/**
 * Remove all nodes from tree.
 * @param tree Tree to clear
 */
void rbtree_clear(RBTree *tree);

/* ============== Basic Operations ============== */

/**
 * Insert a key-value pair.
 * @param tree Tree
 * @param key Key to insert
 * @param value Value to associate
 * @return true if new key inserted, false if key existed (value updated)
 */
bool rbtree_insert(RBTree *tree, int key, int value);

/**
 * Search for a key.
 * @param tree Tree
 * @param key Key to find
 * @param value_out Pointer to store value (can be NULL)
 * @return true if found
 */
bool rbtree_search(const RBTree *tree, int key, int *value_out);

/**
 * Delete a key.
 * @param tree Tree
 * @param key Key to delete
 * @return true if key existed and was deleted
 */
bool rbtree_delete(RBTree *tree, int key);

/**
 * Check if key exists.
 * @param tree Tree
 * @param key Key to check
 * @return true if key exists
 */
bool rbtree_contains(const RBTree *tree, int key);

/* ============== Size Operations ============== */

/**
 * Get number of nodes.
 * @param tree Tree
 * @return Number of nodes
 */
size_t rbtree_size(const RBTree *tree);

/**
 * Check if tree is empty.
 * @param tree Tree
 * @return true if empty
 */
bool rbtree_is_empty(const RBTree *tree);

/* ============== Min/Max Operations ============== */

/**
 * Get minimum key.
 * @param tree Tree
 * @param key_out Pointer to store minimum key
 * @return true if tree is not empty
 */
bool rbtree_min(const RBTree *tree, int *key_out);

/**
 * Get maximum key.
 * @param tree Tree
 * @param key_out Pointer to store maximum key
 * @return true if tree is not empty
 */
bool rbtree_max(const RBTree *tree, int *key_out);

/* ============== Order Statistics ============== */

/**
 * Get floor (largest key <= given key).
 * @param tree Tree
 * @param key Key to find floor of
 * @param result_out Pointer to store result
 * @return true if floor exists
 */
bool rbtree_floor(const RBTree *tree, int key, int *result_out);

/**
 * Get ceiling (smallest key >= given key).
 * @param tree Tree
 * @param key Key to find ceiling of
 * @param result_out Pointer to store result
 * @return true if ceiling exists
 */
bool rbtree_ceiling(const RBTree *tree, int key, int *result_out);

/* ============== Traversal ============== */

/**
 * Get keys in sorted order (in-order traversal).
 * @param tree Tree
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys stored
 */
size_t rbtree_inorder(const RBTree *tree, int *keys, size_t max_keys);

/**
 * Get keys in range [min_key, max_key].
 * @param tree Tree
 * @param min_key Minimum key (inclusive)
 * @param max_key Maximum key (inclusive)
 * @param keys Array to store keys
 * @param max_keys Maximum keys to retrieve
 * @return Number of keys in range
 */
size_t rbtree_range(const RBTree *tree, int min_key, int max_key,
                    int *keys, size_t max_keys);

/* ============== Validation ============== */

/**
 * Validate Red-Black Tree properties.
 * @param tree Tree to validate
 * @return true if all properties hold
 */
bool rbtree_validate(const RBTree *tree);

/**
 * Get black height of tree.
 * @param tree Tree
 * @return Black height (number of black nodes on any path to leaf)
 */
int rbtree_black_height(const RBTree *tree);

/**
 * Get actual height of tree.
 * @param tree Tree
 * @return Height
 */
int rbtree_height(const RBTree *tree);

/* ============== Debug ============== */

/**
 * Print tree structure.
 * @param tree Tree to print
 */
void rbtree_print(const RBTree *tree);

#endif /* RED_BLACK_TREE_H */
