/**
 * ADJACENCY MATRIX GRAPH REPRESENTATION
 *
 * Graph using a 2D matrix for edge representation.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         ADJACENCY MATRIX                                  ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  For a graph with vertices {0, 1, 2, 3}:                                  ║
 * ║                                                                           ║
 * ║      0 ──5── 1                    0   1   2   3                           ║
 * ║      │       │                 0 [ 0   5   0   2 ]                        ║
 * ║      2       3                 1 [ 5   0   3   0 ]                        ║
 * ║      │       │                 2 [ 0   3   0   0 ]                        ║
 * ║      3 ──────┘                 3 [ 2   0   0   0 ]                        ║
 * ║                                                                           ║
 * ║  0 = no edge, value = edge weight                                         ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                    COMPARISON WITH ADJACENCY LIST                         ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║  Operation         │ Adjacency Matrix │ Adjacency List                    ║
 * ║  ──────────────────┼──────────────────┼────────────────────────────       ║
 * ║  Space             │ O(V²)            │ O(V + E)                          ║
 * ║  Add Edge          │ O(1)             │ O(1)                              ║
 * ║  Remove Edge       │ O(1)             │ O(E)                              ║
 * ║  Check Edge        │ O(1)             │ O(degree)                         ║
 * ║  Get Neighbors     │ O(V)             │ O(degree)                         ║
 * ║  Iterate All Edges │ O(V²)            │ O(E)                              ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * USE CASES:
 * - Dense graphs (E ≈ V²)
 * - Need O(1) edge existence checks
 * - Small to medium sized graphs
 * - Floyd-Warshall algorithm
 * - Transitive closure
 *
 * LEETCODE PROBLEMS:
 * - #785 Is Graph Bipartite?
 * - #797 All Paths From Source to Target
 * - #1557 Minimum Number of Vertices to Reach All Nodes
 */

#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#define ADJMAT_NO_EDGE 0
#define ADJMAT_INF INT_MAX

/* Adjacency Matrix Graph structure */
typedef struct {
    int **matrix;           /* 2D adjacency matrix */
    int num_vertices;       /* Number of vertices */
    bool directed;          /* True for directed graph */
    bool weighted;          /* True for weighted graph */
} AdjMatrix;

/* ============== Creation/Destruction ============== */

/**
 * Create a new adjacency matrix graph.
 * @param num_vertices Number of vertices
 * @param directed True for directed graph
 * @param weighted True for weighted graph
 * @return New graph or NULL on failure
 */
AdjMatrix *adjmat_create(int num_vertices, bool directed, bool weighted);

/**
 * Destroy graph and free memory.
 * @param g Graph to destroy
 */
void adjmat_destroy(AdjMatrix *g);

/**
 * Clear all edges from graph.
 * @param g Graph to clear
 */
void adjmat_clear(AdjMatrix *g);

/* ============== Edge Operations ============== */

/**
 * Add an edge to the graph.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @param weight Edge weight (use 1 for unweighted)
 * @return true on success
 */
bool adjmat_add_edge(AdjMatrix *g, int src, int dest, int weight);

/**
 * Remove an edge from the graph.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @return true if edge existed
 */
bool adjmat_remove_edge(AdjMatrix *g, int src, int dest);

/**
 * Check if edge exists.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @return true if edge exists
 */
bool adjmat_has_edge(const AdjMatrix *g, int src, int dest);

/**
 * Get edge weight.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @return Edge weight or 0 if no edge
 */
int adjmat_get_weight(const AdjMatrix *g, int src, int dest);

/**
 * Set edge weight (edge must exist or will be created).
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @param weight New weight
 * @return true on success
 */
bool adjmat_set_weight(AdjMatrix *g, int src, int dest, int weight);

/* ============== Graph Properties ============== */

/**
 * Get number of vertices.
 */
int adjmat_vertex_count(const AdjMatrix *g);

/**
 * Get number of edges.
 */
int adjmat_edge_count(const AdjMatrix *g);

/**
 * Get out-degree of a vertex.
 */
int adjmat_out_degree(const AdjMatrix *g, int v);

/**
 * Get in-degree of a vertex.
 */
int adjmat_in_degree(const AdjMatrix *g, int v);

/**
 * Check if graph is directed.
 */
bool adjmat_is_directed(const AdjMatrix *g);

/**
 * Check if graph is weighted.
 */
bool adjmat_is_weighted(const AdjMatrix *g);

/* ============== Neighbor Operations ============== */

/**
 * Get neighbors of a vertex.
 * @param g Graph
 * @param v Vertex
 * @param neighbors Array to store neighbors
 * @param max_neighbors Maximum neighbors to return
 * @return Number of neighbors
 */
int adjmat_get_neighbors(const AdjMatrix *g, int v, int *neighbors, int max_neighbors);

/**
 * Get neighbors with weights.
 * @param g Graph
 * @param v Vertex
 * @param neighbors Array to store neighbors
 * @param weights Array to store weights
 * @param max_neighbors Maximum neighbors to return
 * @return Number of neighbors
 */
int adjmat_get_neighbors_weighted(const AdjMatrix *g, int v,
                                   int *neighbors, int *weights, int max_neighbors);

/* ============== Graph Algorithms ============== */

/**
 * BFS traversal from source.
 * @param g Graph
 * @param source Starting vertex
 * @param order Array to store visit order
 * @param max_order Maximum vertices to return
 * @return Number of vertices visited
 */
int adjmat_bfs(const AdjMatrix *g, int source, int *order, int max_order);

/**
 * DFS traversal from source.
 * @param g Graph
 * @param source Starting vertex
 * @param order Array to store visit order
 * @param max_order Maximum vertices to return
 * @return Number of vertices visited
 */
int adjmat_dfs(const AdjMatrix *g, int source, int *order, int max_order);

/**
 * Check if graph is connected (undirected) or weakly connected (directed).
 * @param g Graph
 * @return true if connected
 */
bool adjmat_is_connected(const AdjMatrix *g);

/**
 * Compute transitive closure using Floyd-Warshall.
 * @param g Graph
 * @param closure 2D array to store closure (caller allocates V*V)
 * @return true on success
 */
bool adjmat_transitive_closure(const AdjMatrix *g, bool **closure);

/**
 * Floyd-Warshall all-pairs shortest path.
 * @param g Graph
 * @param dist 2D array to store distances (caller allocates V*V)
 * @return true on success (false if negative cycle)
 */
bool adjmat_floyd_warshall(const AdjMatrix *g, int **dist);

/* ============== Utility Functions ============== */

/**
 * Get direct access to matrix (for algorithms that need it).
 * @param g Graph
 * @return Pointer to 2D matrix
 */
int **adjmat_get_matrix(const AdjMatrix *g);

/**
 * Copy the adjacency matrix.
 * @param g Graph to copy
 * @return Copy of graph or NULL on failure
 */
AdjMatrix *adjmat_copy(const AdjMatrix *g);

/**
 * Get transpose of directed graph.
 * @param g Graph
 * @return Transposed graph or NULL on failure
 */
AdjMatrix *adjmat_transpose(const AdjMatrix *g);

/**
 * Print graph matrix.
 * @param g Graph to print
 */
void adjmat_print(const AdjMatrix *g);

#endif /* ADJACENCY_MATRIX_H */
