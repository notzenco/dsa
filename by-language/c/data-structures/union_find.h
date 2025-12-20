/**
 * UNION-FIND (DISJOINT SET UNION)
 *
 * A data structure that tracks elements partitioned into disjoint sets.
 * Supports near-constant time operations for union and find.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                        VISUAL REPRESENTATION                              ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Initial state (5 elements, each in own set):                             ║
 * ║                                                                           ║
 * ║      [0]   [1]   [2]   [3]   [4]                                          ║
 * ║                                                                           ║
 * ║  After union(0, 1) and union(2, 3):                                       ║
 * ║                                                                           ║
 * ║       0       2                                                           ║
 * ║       │       │                                                           ║
 * ║       1       3       [4]                                                 ║
 * ║                                                                           ║
 * ║  After union(0, 2):                                                       ║
 * ║                                                                           ║
 * ║            0                                                              ║
 * ║          / │                                                              ║
 * ║         1  2                                                              ║
 * ║            │                                                              ║
 * ║            3         [4]                                                  ║
 * ║                                                                           ║
 * ║  With path compression, after find(3):                                    ║
 * ║                                                                           ║
 * ║            0                                                              ║
 * ║         / │ \                                                             ║
 * ║        1  2  3       [4]                                                  ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation    │ Time (amortized)                                          ║
 * ║  ─────────────┼───────────────────────────────                            ║
 * ║  Find         │ O(α(n)) ≈ O(1)  (inverse Ackermann)                       ║
 * ║  Union        │ O(α(n)) ≈ O(1)                                            ║
 * ║  Connected    │ O(α(n)) ≈ O(1)                                            ║
 * ║  Space        │ O(n)                                                      ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #200 Number of Islands
 * - #547 Number of Provinces
 * - #684 Redundant Connection
 * - #721 Accounts Merge
 * - #990 Satisfiability of Equality Equations
 * - #1319 Number of Operations to Make Network Connected
 * - #1584 Min Cost to Connect All Points (Kruskal's MST)
 *
 * USE CASES:
 * - Network connectivity
 * - Image processing (connected components)
 * - Kruskal's minimum spanning tree
 * - Least common ancestor in trees
 * - Equivalence class detection
 */

#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Union-Find Structure ============== */

typedef struct {
    int *parent;        /* Parent of each element */
    int *rank;          /* Rank (approximate height) of each tree */
    int *size;          /* Size of each component (at root) */
    int n;              /* Number of elements */
    int num_components; /* Current number of disjoint sets */
} UnionFind;

/* ============== Creation/Destruction ============== */

/**
 * Create a new Union-Find structure.
 * Each element starts in its own singleton set.
 *
 * @param n Number of elements (0 to n-1)
 * @return New Union-Find structure or NULL on failure
 */
UnionFind *uf_create(int n);

/**
 * Destroy Union-Find structure and free memory.
 *
 * @param uf Union-Find structure to destroy
 */
void uf_destroy(UnionFind *uf);

/**
 * Clear and reinitialize Union-Find structure.
 * Each element returns to its own singleton set.
 *
 * @param uf Union-Find structure
 */
void uf_clear(UnionFind *uf);

/* ============== Core Operations ============== */

/**
 * Find the representative (root) of the set containing x.
 * Uses path compression for efficiency.
 *
 * @param uf Union-Find structure
 * @param x Element to find
 * @return Root of the set containing x, or -1 if x is invalid
 */
int uf_find(UnionFind *uf, int x);

/**
 * Union the sets containing x and y.
 * Uses union by rank for efficiency.
 *
 * @param uf Union-Find structure
 * @param x First element
 * @param y Second element
 * @return true if x and y were in different sets (union performed),
 *         false if already in same set or invalid elements
 */
bool uf_union(UnionFind *uf, int x, int y);

/**
 * Check if x and y are in the same set.
 *
 * @param uf Union-Find structure
 * @param x First element
 * @param y Second element
 * @return true if x and y are connected
 */
bool uf_connected(UnionFind *uf, int x, int y);

/* ============== Component Information ============== */

/**
 * Get the size of the set containing x.
 *
 * @param uf Union-Find structure
 * @param x Element
 * @return Size of the set containing x, or 0 if invalid
 */
int uf_component_size(UnionFind *uf, int x);

/**
 * Get the current number of disjoint sets.
 *
 * @param uf Union-Find structure
 * @return Number of components
 */
int uf_num_components(const UnionFind *uf);

/**
 * Get number of elements.
 *
 * @param uf Union-Find structure
 * @return Total number of elements
 */
int uf_size(const UnionFind *uf);

/* ============== Utility ============== */

/**
 * Get all elements in the same set as x.
 *
 * @param uf Union-Find structure
 * @param x Element
 * @param members Array to store members (caller allocates, size >= n)
 * @return Number of members written
 */
int uf_get_component(UnionFind *uf, int x, int *members);

/**
 * Get all root elements (component representatives).
 *
 * @param uf Union-Find structure
 * @param roots Array to store roots (caller allocates, size >= n)
 * @return Number of roots written
 */
int uf_get_roots(UnionFind *uf, int *roots);

/**
 * Check if an element is a root (representative of its set).
 *
 * @param uf Union-Find structure
 * @param x Element to check
 * @return true if x is a root
 */
bool uf_is_root(const UnionFind *uf, int x);

#endif /* UNION_FIND_H */
