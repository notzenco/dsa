/**
 * GRAPH DATA STRUCTURE AND ALGORITHMS
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
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         ALGORITHM COMPLEXITY                              ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Algorithm          │ Time         │ Space   │ Notes                      ║
 * ║  ───────────────────┼──────────────┼─────────┼─────────────────────       ║
 * ║  BFS                │ O(V + E)     │ O(V)    │ Unweighted shortest path   ║
 * ║  DFS                │ O(V + E)     │ O(V)    │ Cycle detection, topo sort ║
 * ║  Dijkstra           │ O((V+E)logV) │ O(V)    │ Non-negative weights       ║
 * ║  Bellman-Ford       │ O(V * E)     │ O(V)    │ Handles negative weights   ║
 * ║  Topological Sort   │ O(V + E)     │ O(V)    │ DAG only                   ║
 * ║  Kruskal's MST      │ O(E log E)   │ O(V)    │ Uses Union-Find            ║
 * ║  Prim's MST         │ O((V+E)logV) │ O(V)    │ Uses Min-Heap              ║
 * ║  Tarjan's SCC       │ O(V + E)     │ O(V)    │ Strongly connected comp.   ║
 * ║  Kosaraju's SCC     │ O(V + E)     │ O(V)    │ Two DFS passes             ║
 * ║  Floyd-Warshall     │ O(V³)        │ O(V²)   │ All-pairs shortest path    ║
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

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#define GRAPH_INF INT_MAX

/* ============== Edge Structure ============== */

typedef struct Edge {
    int dest;           /* Destination vertex */
    int weight;         /* Edge weight (1 for unweighted) */
    struct Edge *next;  /* Next edge in adjacency list */
} Edge;

/* ============== Graph Structure ============== */

typedef struct {
    int num_vertices;
    bool directed;
    Edge **adj_list;    /* Array of adjacency lists */
} Graph;

/* ============== Graph Result Structures ============== */

/* BFS/DFS result */
typedef struct {
    int *distance;      /* Distance from source */
    int *parent;        /* Parent in traversal tree */
    int *order;         /* Traversal order */
    int order_count;    /* Number of vertices visited */
} TraversalResult;

/* Shortest path result */
typedef struct {
    int *distance;      /* Distance from source */
    int *parent;        /* Parent in shortest path tree */
    bool has_negative_cycle;
} ShortestPathResult;

/* Topological sort result */
typedef struct {
    int *order;         /* Topological order */
    int count;          /* Number of vertices in order */
    bool is_dag;        /* True if graph is a DAG */
} TopoSortResult;

/* MST result */
typedef struct {
    int **edges;        /* Array of [u, v, weight] */
    int num_edges;      /* Number of edges in MST */
    int total_weight;   /* Total MST weight */
} MSTResult;

/* SCC result */
typedef struct {
    int *component;     /* Component ID for each vertex */
    int num_components; /* Number of strongly connected components */
} SCCResult;

/* ============== Graph Creation/Destruction ============== */

/**
 * Create a new graph.
 * @param num_vertices Number of vertices
 * @param directed True for directed graph
 * @return New graph or NULL on failure
 */
Graph *graph_create(int num_vertices, bool directed);

/**
 * Destroy a graph and free memory.
 * @param g Graph to destroy
 */
void graph_destroy(Graph *g);

/**
 * Add an edge to the graph.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @param weight Edge weight (use 1 for unweighted)
 * @return true on success
 */
bool graph_add_edge(Graph *g, int src, int dest, int weight);

/**
 * Check if edge exists.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @return true if edge exists
 */
bool graph_has_edge(const Graph *g, int src, int dest);

/**
 * Get edge weight.
 * @param g Graph
 * @param src Source vertex
 * @param dest Destination vertex
 * @return Edge weight or GRAPH_INF if no edge
 */
int graph_get_weight(const Graph *g, int src, int dest);

/**
 * Get number of vertices.
 */
int graph_vertex_count(const Graph *g);

/**
 * Get number of edges.
 */
int graph_edge_count(const Graph *g);

/**
 * Get out-degree of a vertex.
 */
int graph_out_degree(const Graph *g, int v);

/**
 * Get in-degree of a vertex (for directed graphs).
 */
int graph_in_degree(const Graph *g, int v);

/* ============== Traversal Algorithms ============== */

/**
 * Breadth-First Search from source vertex.
 * @param g Graph
 * @param source Starting vertex
 * @return Traversal result (caller must free)
 */
TraversalResult *graph_bfs(const Graph *g, int source);

/**
 * Depth-First Search from source vertex.
 * @param g Graph
 * @param source Starting vertex
 * @return Traversal result (caller must free)
 */
TraversalResult *graph_dfs(const Graph *g, int source);

/**
 * DFS visiting all vertices (forest traversal).
 * @param g Graph
 * @return Traversal result (caller must free)
 */
TraversalResult *graph_dfs_full(const Graph *g);

/**
 * Free traversal result.
 */
void traversal_result_free(TraversalResult *result);

/* ============== Shortest Path Algorithms ============== */

/**
 * Dijkstra's algorithm for shortest paths.
 * Requires non-negative edge weights.
 * @param g Graph
 * @param source Starting vertex
 * @return Shortest path result (caller must free)
 */
ShortestPathResult *graph_dijkstra(const Graph *g, int source);

/**
 * Bellman-Ford algorithm for shortest paths.
 * Handles negative edge weights, detects negative cycles.
 * @param g Graph
 * @param source Starting vertex
 * @return Shortest path result (caller must free)
 */
ShortestPathResult *graph_bellman_ford(const Graph *g, int source);

/**
 * Free shortest path result.
 */
void shortest_path_result_free(ShortestPathResult *result);

/**
 * Reconstruct path from source to destination.
 * @param result Shortest path result
 * @param dest Destination vertex
 * @param path_out Array to store path
 * @param path_len_out Pointer to store path length
 * @return true if path exists
 */
bool shortest_path_reconstruct(const ShortestPathResult *result, int dest,
                               int *path_out, int *path_len_out);

/* ============== Topological Sort ============== */

/**
 * Topological sort using Kahn's algorithm (BFS-based).
 * @param g Graph (must be directed)
 * @return Topological sort result (caller must free)
 */
TopoSortResult *graph_topo_sort_kahn(const Graph *g);

/**
 * Topological sort using DFS.
 * @param g Graph (must be directed)
 * @return Topological sort result (caller must free)
 */
TopoSortResult *graph_topo_sort_dfs(const Graph *g);

/**
 * Free topological sort result.
 */
void topo_sort_result_free(TopoSortResult *result);

/* ============== Minimum Spanning Tree ============== */

/**
 * Kruskal's algorithm for MST.
 * Uses Union-Find for cycle detection.
 * @param g Graph (should be undirected)
 * @return MST result (caller must free)
 */
MSTResult *graph_mst_kruskal(const Graph *g);

/**
 * Prim's algorithm for MST.
 * Uses min-heap for efficient implementation.
 * @param g Graph (should be undirected)
 * @return MST result (caller must free)
 */
MSTResult *graph_mst_prim(const Graph *g);

/**
 * Free MST result.
 */
void mst_result_free(MSTResult *result);

/* ============== Strongly Connected Components ============== */

/**
 * Tarjan's algorithm for SCC.
 * @param g Graph (must be directed)
 * @return SCC result (caller must free)
 */
SCCResult *graph_scc_tarjan(const Graph *g);

/**
 * Kosaraju's algorithm for SCC.
 * @param g Graph (must be directed)
 * @return SCC result (caller must free)
 */
SCCResult *graph_scc_kosaraju(const Graph *g);

/**
 * Free SCC result.
 */
void scc_result_free(SCCResult *result);

/* ============== Cycle Detection ============== */

/**
 * Check if directed graph has a cycle.
 * @param g Graph (must be directed)
 * @return true if cycle exists
 */
bool graph_has_cycle_directed(const Graph *g);

/**
 * Check if undirected graph has a cycle.
 * @param g Graph (must be undirected)
 * @return true if cycle exists
 */
bool graph_has_cycle_undirected(const Graph *g);

/* ============== Utility Functions ============== */

/**
 * Check if graph is connected (undirected) or weakly connected (directed).
 * @param g Graph
 * @return true if connected
 */
bool graph_is_connected(const Graph *g);

/**
 * Check if directed graph is a DAG.
 * @param g Graph
 * @return true if DAG
 */
bool graph_is_dag(const Graph *g);

/**
 * Print graph to stdout.
 * @param g Graph
 */
void graph_print(const Graph *g);

#endif /* GRAPH_H */
