/**
 * BINARY SEARCH TREE (BST)
 *
 * A binary tree where each node's left subtree contains only values less than
 * the node's value, and the right subtree contains only values greater.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  BST Property: left < parent < right                                      ║
 * ║                                                                           ║
 * ║                        ┌───────┐                                          ║
 * ║                        │  50   │ (root)                                   ║
 * ║                        └───┬───┘                                          ║
 * ║                    ┌───────┴───────┐                                      ║
 * ║                ┌───┴───┐       ┌───┴───┐                                  ║
 * ║                │  30   │       │  70   │                                  ║
 * ║                └───┬───┘       └───┬───┘                                  ║
 * ║              ┌─────┴─────┐   ┌─────┴─────┐                                ║
 * ║          ┌───┴───┐   ┌───┴───┐   ┌───┴───┐   ┌───┴───┐                   ║
 * ║          │  20   │   │  40   │   │  60   │   │  80   │                   ║
 * ║          └───────┘   └───────┘   └───────┘   └───────┘                   ║
 * ║                                                                           ║
 * ║  Traversals:                                                              ║
 * ║  - Inorder (sorted):  20, 30, 40, 50, 60, 70, 80                         ║
 * ║  - Preorder (root-first): 50, 30, 20, 40, 70, 60, 80                     ║
 * ║  - Postorder (root-last): 20, 40, 30, 60, 80, 70, 50                     ║
 * ║  - Level-order (BFS): 50, 30, 70, 20, 40, 60, 80                         ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Search             │ O(log n)     │ O(n)       │ O(1)    │
 * │ Insert             │ O(log n)     │ O(n)       │ O(1)    │
 * │ Delete             │ O(log n)     │ O(n)       │ O(1)    │
 * │ Min/Max            │ O(log n)     │ O(n)       │ O(1)    │
 * │ Traversal          │ O(n)         │ O(n)       │ O(h)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * Note: Worst case occurs when tree degenerates to a linked list
 *
 * LEETCODE PROBLEMS:
 * - #700 Search in a BST
 * - #701 Insert into a BST
 * - #450 Delete Node in a BST
 * - #98 Validate Binary Search Tree
 * - #230 Kth Smallest Element in a BST
 * - #235 Lowest Common Ancestor of a BST
 *
 * USE CASES:
 * - Dictionary/Symbol table implementations
 * - Database indexing
 * - Priority queues (when balanced)
 * - Expression trees
 */

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * BST Node structure
 */
typedef struct BSTNode {
    int key;
    int value;
    struct BSTNode *left;
    struct BSTNode *right;
    struct BSTNode *parent;
} BSTNode;

/**
 * Binary Search Tree structure
 */
typedef struct {
    BSTNode *root;
    size_t size;
} BST;

/**
 * Create a new empty BST.
 * @return Pointer to new BST, or NULL on allocation failure
 */
BST *bst_create(void);

/**
 * Free all memory associated with the BST.
 * @param tree Pointer to the BST
 */
void bst_destroy(BST *tree);

/**
 * Insert a key-value pair into the BST.
 * @param tree Pointer to the BST
 * @param key Key to insert
 * @param value Value associated with the key
 * @return true if inserted, false if key already exists or allocation failure
 */
bool bst_insert(BST *tree, int key, int value);

/**
 * Search for a key in the BST.
 * @param tree Pointer to the BST
 * @param key Key to search for
 * @param out_value Pointer to store the value if found
 * @return true if found, false otherwise
 */
bool bst_search(const BST *tree, int key, int *out_value);

/**
 * Check if a key exists in the BST.
 * @param tree Pointer to the BST
 * @param key Key to check
 * @return true if exists, false otherwise
 */
bool bst_contains(const BST *tree, int key);

/**
 * Delete a key from the BST.
 * @param tree Pointer to the BST
 * @param key Key to delete
 * @return true if deleted, false if key not found
 */
bool bst_delete(BST *tree, int key);

/**
 * Get the minimum key in the BST.
 * @param tree Pointer to the BST
 * @param out_key Pointer to store the minimum key
 * @param out_value Pointer to store the value (can be NULL)
 * @return true if found, false if tree is empty
 */
bool bst_min(const BST *tree, int *out_key, int *out_value);

/**
 * Get the maximum key in the BST.
 * @param tree Pointer to the BST
 * @param out_key Pointer to store the maximum key
 * @param out_value Pointer to store the value (can be NULL)
 * @return true if found, false if tree is empty
 */
bool bst_max(const BST *tree, int *out_key, int *out_value);

/**
 * Get the number of nodes in the BST.
 * @param tree Pointer to the BST
 * @return Number of nodes
 */
size_t bst_size(const BST *tree);

/**
 * Check if the BST is empty.
 * @param tree Pointer to the BST
 * @return true if empty, false otherwise
 */
bool bst_is_empty(const BST *tree);

/**
 * Get the height of the BST.
 * @param tree Pointer to the BST
 * @return Height of the tree (0 for empty tree)
 */
int bst_height(const BST *tree);

/**
 * Clear all nodes from the BST.
 * @param tree Pointer to the BST
 */
void bst_clear(BST *tree);

/**
 * Validate that the tree satisfies BST property.
 * @param tree Pointer to the BST
 * @return true if valid BST, false otherwise
 */
bool bst_is_valid(const BST *tree);

/* ============== Traversal Functions ============== */

/**
 * Callback function type for traversal
 */
typedef void (*bst_visit_fn)(int key, int value, void *user_data);

/**
 * Perform inorder traversal (sorted order).
 * @param tree Pointer to the BST
 * @param visit Callback function for each node
 * @param user_data User data passed to callback
 */
void bst_inorder(const BST *tree, bst_visit_fn visit, void *user_data);

/**
 * Perform preorder traversal (root first).
 * @param tree Pointer to the BST
 * @param visit Callback function for each node
 * @param user_data User data passed to callback
 */
void bst_preorder(const BST *tree, bst_visit_fn visit, void *user_data);

/**
 * Perform postorder traversal (root last).
 * @param tree Pointer to the BST
 * @param visit Callback function for each node
 * @param user_data User data passed to callback
 */
void bst_postorder(const BST *tree, bst_visit_fn visit, void *user_data);

/**
 * Perform level-order traversal (BFS).
 * @param tree Pointer to the BST
 * @param visit Callback function for each node
 * @param user_data User data passed to callback
 */
void bst_levelorder(const BST *tree, bst_visit_fn visit, void *user_data);

/* ============== Additional Operations ============== */

/**
 * Get the inorder successor of a key.
 * @param tree Pointer to the BST
 * @param key Key to find successor for
 * @param out_key Pointer to store the successor key
 * @return true if successor found, false otherwise
 */
bool bst_successor(const BST *tree, int key, int *out_key);

/**
 * Get the inorder predecessor of a key.
 * @param tree Pointer to the BST
 * @param key Key to find predecessor for
 * @param out_key Pointer to store the predecessor key
 * @return true if predecessor found, false otherwise
 */
bool bst_predecessor(const BST *tree, int key, int *out_key);

/**
 * Get the kth smallest element.
 * @param tree Pointer to the BST
 * @param k The k value (1-indexed)
 * @param out_key Pointer to store the kth smallest key
 * @return true if found, false if k > size
 */
bool bst_kth_smallest(const BST *tree, size_t k, int *out_key);

/**
 * Find the lowest common ancestor of two keys.
 * @param tree Pointer to the BST
 * @param key1 First key
 * @param key2 Second key
 * @param out_key Pointer to store the LCA key
 * @return true if LCA found, false if either key not in tree
 */
bool bst_lca(const BST *tree, int key1, int key2, int *out_key);

/**
 * Get all keys in sorted order as an array.
 * @param tree Pointer to the BST
 * @param out_array Pointer to store allocated array
 * @param out_size Pointer to store array size
 * @return true if successful, false on allocation failure
 */
bool bst_to_array(const BST *tree, int **out_array, size_t *out_size);

/**
 * Print the BST structure (for debugging).
 * @param tree Pointer to the BST
 */
void bst_print(const BST *tree);

#endif /* BINARY_SEARCH_TREE_H */
