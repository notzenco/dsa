/**
 * SEGMENT TREE
 *
 * A tree data structure for efficient range queries and point updates.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Array: [1, 3, 5, 7, 9, 11]                                              ║
 * ║                                                                           ║
 * ║                        [36]           (sum of entire range)               ║
 * ║                       /    \                                              ║
 * ║                    [9]      [27]      (sum of left/right halves)          ║
 * ║                   /   \    /    \                                         ║
 * ║                 [4]  [5] [16]  [11]   (sum of quarters)                   ║
 * ║                /  \      /  \                                             ║
 * ║              [1] [3]   [7] [9]        (leaf nodes = array elements)       ║
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
 * ║  Range Query        │ O(log n)  │ O(log n)                                ║
 * ║  Point Update       │ O(log n)  │ O(log n)                                ║
 * ║  Range Update*      │ O(log n)  │ O(log n)                                ║
 * ║                     *with lazy propagation                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #307 Range Sum Query - Mutable
 * - #315 Count of Smaller Numbers After Self
 * - #327 Count of Range Sum
 * - #493 Reverse Pairs
 */

#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Query Types ============== */

typedef enum {
    SEG_SUM,    /* Range sum */
    SEG_MIN,    /* Range minimum */
    SEG_MAX,    /* Range maximum */
    SEG_GCD     /* Range GCD */
} SegmentTreeType;

/* ============== Segment Tree Structure ============== */

typedef struct {
    int *tree;           /* Tree array (size 4*n) */
    int *lazy;           /* Lazy propagation array */
    size_t size;         /* Number of elements */
    SegmentTreeType type;
    int identity;        /* Identity element for the operation */
} SegmentTree;

/* ============== Creation/Destruction ============== */

/**
 * Create a segment tree from an array.
 * @param arr Input array
 * @param n Array size
 * @param type Type of query (SUM, MIN, MAX, GCD)
 * @return New segment tree or NULL on failure
 */
SegmentTree *segtree_create(const int *arr, size_t n, SegmentTreeType type);

/**
 * Create an empty segment tree.
 * @param n Size
 * @param type Type of query
 * @return New segment tree or NULL on failure
 */
SegmentTree *segtree_create_empty(size_t n, SegmentTreeType type);

/**
 * Destroy a segment tree.
 * @param tree Tree to destroy
 */
void segtree_destroy(SegmentTree *tree);

/* ============== Core Operations ============== */

/**
 * Query a range [left, right] inclusive.
 * @param tree Segment tree
 * @param left Left index (0-indexed)
 * @param right Right index (0-indexed)
 * @return Query result
 */
int segtree_query(SegmentTree *tree, size_t left, size_t right);

/**
 * Update a single element.
 * @param tree Segment tree
 * @param index Index to update (0-indexed)
 * @param value New value
 */
void segtree_update(SegmentTree *tree, size_t index, int value);

/**
 * Add a value to a single element.
 * @param tree Segment tree
 * @param index Index to update (0-indexed)
 * @param delta Value to add
 */
void segtree_add(SegmentTree *tree, size_t index, int delta);

/**
 * Update a range [left, right] with lazy propagation.
 * Adds delta to all elements in range.
 * @param tree Segment tree
 * @param left Left index
 * @param right Right index
 * @param delta Value to add
 */
void segtree_range_update(SegmentTree *tree, size_t left, size_t right, int delta);

/* ============== Utility ============== */

/**
 * Get the size of the segment tree.
 * @param tree Segment tree
 * @return Number of elements
 */
size_t segtree_size(const SegmentTree *tree);

/**
 * Get element at index.
 * @param tree Segment tree
 * @param index Index (0-indexed)
 * @return Element value
 */
int segtree_get(SegmentTree *tree, size_t index);

#endif /* SEGMENT_TREE_H */
