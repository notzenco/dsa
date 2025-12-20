/**
 * GRAPH CORE DATA STRUCTURE
 *
 * Core graph structures and basic operations.
 * For algorithms (BFS, DFS, Dijkstra, etc.), include graph_algorithms.h
 *
 * See graph.h for full documentation.
 */

#ifndef GRAPH_CORE_H
#define GRAPH_CORE_H

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

/* ============== Result Free Functions ============== */

/**
 * Free traversal result.
 */
void traversal_result_free(TraversalResult *result);

/**
 * Free shortest path result.
 */
void shortest_path_result_free(ShortestPathResult *result);

/**
 * Free topological sort result.
 */
void topo_sort_result_free(TopoSortResult *result);

/**
 * Free MST result.
 */
void mst_result_free(MSTResult *result);

/**
 * Free SCC result.
 */
void scc_result_free(SCCResult *result);

#endif /* GRAPH_CORE_H */
