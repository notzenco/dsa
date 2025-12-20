/**
 * AVL TREE (Self-Balancing Binary Search Tree)
 *
 * A height-balanced BST where the heights of left and right subtrees
 * differ by at most 1 for every node.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║                          [30]                                             ║
 * ║                         /    \                                            ║
 * ║                      [20]    [40]                                         ║
 * ║                     /   \       \                                         ║
 * ║                  [10]  [25]    [50]                                       ║
 * ║                                                                           ║
 * ║   Balance Factor = Height(Left) - Height(Right)                          ║
 * ║   Valid range: -1, 0, +1                                                  ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            ROTATIONS                                      ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Right Rotation (LL case):    Left Rotation (RR case):                   ║
 * ║         y                x           x                y                   ║
 * ║        / \              / \         / \              / \                  ║
 * ║       x   C    ->      A   y       A   y    ->      x   C                 ║
 * ║      / \                  / \         / \          / \                    ║
 * ║     A   B                B   C       B   C        A   B                   ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Time          │ Space                               ║
 * ║  ───────────────────┼───────────────┼──────────                           ║
 * ║  Insert             │ O(log n)      │ O(log n)                            ║
 * ║  Delete             │ O(log n)      │ O(log n)                            ║
 * ║  Search             │ O(log n)      │ O(1)                                ║
 * ║  Min/Max            │ O(log n)      │ O(1)                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #110 Balanced Binary Tree
 * - #108 Convert Sorted Array to BST
 * - #1382 Balance a BST
 * - #220 Contains Duplicate III
 */

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stddef.h>
#include <stdbool.h>

/* ============== AVL Node ============== */

typedef struct AVLNode {
    int data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

/* ============== AVL Tree Structure ============== */

typedef struct {
    AVLNode *root;
    size_t size;
} AVLTree;

/* ============== Creation/Destruction ============== */

/**
 * Create a new AVL tree.
 * @return New tree or NULL on failure
 */
AVLTree *avl_create(void);

/**
 * Destroy an AVL tree and free all memory.
 * @param tree Tree to destroy
 */
void avl_destroy(AVLTree *tree);

/**
 * Remove all nodes from tree.
 * @param tree Tree to clear
 */
void avl_clear(AVLTree *tree);

/* ============== Basic Operations ============== */

/**
 * Insert a value into the AVL tree.
 * @param tree Tree
 * @param value Value to insert
 * @return true on success
 */
bool avl_insert(AVLTree *tree, int value);

/**
 * Delete a value from the AVL tree.
 * @param tree Tree
 * @param value Value to delete
 * @return true if value was found and deleted
 */
bool avl_delete(AVLTree *tree, int value);

/**
 * Search for a value in the tree.
 * @param tree Tree
 * @param value Value to search for
 * @return true if value exists
 */
bool avl_search(const AVLTree *tree, int value);

/**
 * Check if tree contains a value.
 * @param tree Tree
 * @param value Value to check
 * @return true if value exists
 */
bool avl_contains(const AVLTree *tree, int value);

/* ============== Tree Properties ============== */

/**
 * Get the minimum value.
 * @param tree Tree
 * @param result Pointer to store minimum value
 * @return true if tree is not empty
 */
bool avl_minimum(const AVLTree *tree, int *result);

/**
 * Get the maximum value.
 * @param tree Tree
 * @param result Pointer to store maximum value
 * @return true if tree is not empty
 */
bool avl_maximum(const AVLTree *tree, int *result);

/**
 * Get the height of the tree.
 * @param tree Tree
 * @return Height (-1 for empty tree)
 */
int avl_height(const AVLTree *tree);

/**
 * Get number of nodes in tree.
 * @param tree Tree
 * @return Number of nodes
 */
size_t avl_size(const AVLTree *tree);

/**
 * Check if tree is empty.
 * @param tree Tree
 * @return true if empty
 */
bool avl_is_empty(const AVLTree *tree);

/* ============== Traversal ============== */

/**
 * Get values in sorted order (inorder traversal).
 * @param tree Tree
 * @param result Array to store values
 * @param max_size Maximum values to store
 * @return Number of values stored
 */
size_t avl_inorder(const AVLTree *tree, int *result, size_t max_size);

/**
 * Get values in preorder traversal.
 * @param tree Tree
 * @param result Array to store values
 * @param max_size Maximum values to store
 * @return Number of values stored
 */
size_t avl_preorder(const AVLTree *tree, int *result, size_t max_size);

/**
 * Get values in postorder traversal.
 * @param tree Tree
 * @param result Array to store values
 * @param max_size Maximum values to store
 * @return Number of values stored
 */
size_t avl_postorder(const AVLTree *tree, int *result, size_t max_size);

/**
 * Get values in level order traversal.
 * @param tree Tree
 * @param result Array to store values
 * @param max_size Maximum values to store
 * @return Number of values stored
 */
size_t avl_level_order(const AVLTree *tree, int *result, size_t max_size);

/* ============== Advanced Operations ============== */

/**
 * Get in-order successor.
 * @param tree Tree
 * @param value Value to find successor for
 * @param result Pointer to store successor
 * @return true if successor exists
 */
bool avl_successor(const AVLTree *tree, int value, int *result);

/**
 * Get in-order predecessor.
 * @param tree Tree
 * @param value Value to find predecessor for
 * @param result Pointer to store predecessor
 * @return true if predecessor exists
 */
bool avl_predecessor(const AVLTree *tree, int value, int *result);

/**
 * Get the kth smallest element (1-indexed).
 * @param tree Tree
 * @param k Position (1 = smallest)
 * @param result Pointer to store result
 * @return true if k is valid
 */
bool avl_kth_smallest(const AVLTree *tree, size_t k, int *result);

/**
 * Find floor (largest value <= given value).
 * @param tree Tree
 * @param value Value to find floor for
 * @param result Pointer to store floor
 * @return true if floor exists
 */
bool avl_floor(const AVLTree *tree, int value, int *result);

/**
 * Find ceiling (smallest value >= given value).
 * @param tree Tree
 * @param value Value to find ceiling for
 * @param result Pointer to store ceiling
 * @return true if ceiling exists
 */
bool avl_ceiling(const AVLTree *tree, int value, int *result);

/**
 * Count values in range [low, high].
 * @param tree Tree
 * @param low Lower bound (inclusive)
 * @param high Upper bound (inclusive)
 * @return Number of values in range
 */
size_t avl_count_range(const AVLTree *tree, int low, int high);

/* ============== Validation ============== */

/**
 * Check if tree is a valid AVL tree.
 * @param tree Tree
 * @return true if valid AVL tree
 */
bool avl_is_valid(const AVLTree *tree);

/**
 * Get balance factor of root node.
 * @param tree Tree
 * @return Balance factor (height(left) - height(right))
 */
int avl_balance_factor(const AVLTree *tree);

#endif /* AVL_TREE_H */
