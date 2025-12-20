/**
 * FENWICK TREE (Binary Indexed Tree)
 *
 * A data structure for efficient prefix sums and point updates.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Index:    1    2    3    4    5    6    7    8                          ║
 * ║   Array:  [ 1,   3,   5,   7,   9,  11,  13,  15 ]                        ║
 * ║                                                                           ║
 * ║   BIT:    [ 1,   4,   5,  16,   9,  20,  13,  64 ]                        ║
 * ║              │    │    │    │    │    │    │    │                         ║
 * ║              └─1  └─2  └─1  └─4  └─1  └─2  └─1  └─8  (range covered)      ║
 * ║                                                                           ║
 * ║   bit[i] stores sum of elements from (i - lowbit(i) + 1) to i             ║
 * ║   lowbit(i) = i & (-i) = rightmost set bit                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Time      │ Space                                   ║
 * ║  ───────────────────┼───────────┼──────────                               ║
 * ║  Build              │ O(n)      │ O(n)                                    ║
 * ║  Point Update       │ O(log n)  │ O(1)                                    ║
 * ║  Prefix Sum         │ O(log n)  │ O(1)                                    ║
 * ║  Range Sum          │ O(log n)  │ O(1)                                    ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #307 Range Sum Query - Mutable
 * - #315 Count of Smaller Numbers After Self
 * - #493 Reverse Pairs
 */

#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Fenwick Tree Structure ============== */

typedef struct {
    int *tree;       /* BIT array (1-indexed internally) */
    size_t size;     /* Number of elements */
} FenwickTree;

/* ============== Creation/Destruction ============== */

/**
 * Create a Fenwick tree from an array.
 * @param arr Input array
 * @param n Array size
 * @return New Fenwick tree or NULL on failure
 */
FenwickTree *fenwick_create(const int *arr, size_t n);

/**
 * Create an empty Fenwick tree.
 * @param n Size
 * @return New Fenwick tree or NULL on failure
 */
FenwickTree *fenwick_create_empty(size_t n);

/**
 * Destroy a Fenwick tree.
 * @param tree Tree to destroy
 */
void fenwick_destroy(FenwickTree *tree);

/* ============== Core Operations ============== */

/**
 * Add a value to element at index.
 * @param tree Fenwick tree
 * @param index Index (0-indexed)
 * @param delta Value to add
 */
void fenwick_add(FenwickTree *tree, size_t index, int delta);

/**
 * Update element at index to a new value.
 * @param tree Fenwick tree
 * @param index Index (0-indexed)
 * @param value New value
 */
void fenwick_update(FenwickTree *tree, size_t index, int value);

/**
 * Get prefix sum from index 0 to index (inclusive).
 * @param tree Fenwick tree
 * @param index Index (0-indexed)
 * @return Prefix sum
 */
int fenwick_prefix_sum(const FenwickTree *tree, size_t index);

/**
 * Get range sum from left to right (inclusive).
 * @param tree Fenwick tree
 * @param left Left index (0-indexed)
 * @param right Right index (0-indexed)
 * @return Range sum
 */
int fenwick_range_sum(const FenwickTree *tree, size_t left, size_t right);

/**
 * Get element at index.
 * @param tree Fenwick tree
 * @param index Index (0-indexed)
 * @return Element value
 */
int fenwick_get(const FenwickTree *tree, size_t index);

/* ============== Utility ============== */

/**
 * Get the size of the Fenwick tree.
 * @param tree Fenwick tree
 * @return Number of elements
 */
size_t fenwick_size(const FenwickTree *tree);

/**
 * Find smallest index with prefix sum >= value.
 * @param tree Fenwick tree
 * @param value Target value
 * @return Index or size if not found
 */
size_t fenwick_lower_bound(const FenwickTree *tree, int value);

#endif /* FENWICK_TREE_H */
