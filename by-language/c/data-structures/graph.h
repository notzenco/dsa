/**
 * GRAPH DATA STRUCTURE AND ALGORITHMS
 *
 * Umbrella header that includes both core graph operations and algorithms.
 * For backward compatibility, include this file to get all graph functionality.
 *
 * Weighted directed graph using adjacency list representation.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                           GRAPH REPRESENTATION                            ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Adjacency List (used here):                                              ║
 * ║    Space: O(V + E)                                                        ║
 * ║    Add edge: O(1)                                                         ║
 * ║    Check edge: O(degree)                                                  ║
 * ║    Iterate neighbors: O(degree)                                           ║
 * ║                                                                           ║
 * ║  Adjacency Matrix:                                                        ║
 * ║    Space: O(V²)                                                           ║
 * ║    Add edge: O(1)                                                         ║
 * ║    Check edge: O(1)                                                       ║
 * ║    Iterate neighbors: O(V)                                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #200 Number of Islands (BFS/DFS)
 * - #207 Course Schedule (Topological Sort)
 * - #210 Course Schedule II
 * - #743 Network Delay Time (Dijkstra)
 * - #787 Cheapest Flights Within K Stops (Bellman-Ford)
 * - #1584 Min Cost to Connect All Points (MST)
 */

#ifndef GRAPH_H
#define GRAPH_H

/* Core graph structures and operations */
#include "graph_core.h"

/* Graph algorithms (traversal, shortest path, MST, SCC, etc.) */
#include "graph_algorithms.h"

#endif /* GRAPH_H */
