/**
 * Graph Data Structure and Algorithms Implementation
 * See graph.h for documentation
 */

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============== Helper Structures ============== */

/* Min-heap for Dijkstra/Prim */
typedef struct {
    int vertex;
    int distance;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int *pos;       /* Position of vertex in heap */
    int size;
    int capacity;
} MinPQ;

/* Union-Find for Kruskal */
typedef struct {
    int *parent;
    int *rank;
    int n;
} UnionFind;

/* Edge array for Kruskal */
typedef struct {
    int src;
    int dest;
    int weight;
} EdgeItem;

/* ============== Min Priority Queue ============== */

static MinPQ *minpq_create(int capacity) {
    MinPQ *pq = malloc(sizeof(MinPQ));
    if (pq == NULL) return NULL;

    pq->nodes = malloc(capacity * sizeof(HeapNode));
    pq->pos = malloc(capacity * sizeof(int));
    if (pq->nodes == NULL || pq->pos == NULL) {
        free(pq->nodes);
        free(pq->pos);
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

static void minpq_destroy(MinPQ *pq) {
    if (pq != NULL) {
        free(pq->nodes);
        free(pq->pos);
        free(pq);
    }
}

static void minpq_swap(MinPQ *pq, int i, int j) {
    pq->pos[pq->nodes[i].vertex] = j;
    pq->pos[pq->nodes[j].vertex] = i;
    HeapNode temp = pq->nodes[i];
    pq->nodes[i] = pq->nodes[j];
    pq->nodes[j] = temp;
}

static void minpq_sift_up(MinPQ *pq, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[parent].distance <= pq->nodes[i].distance) break;
        minpq_swap(pq, i, parent);
        i = parent;
    }
}

static void minpq_sift_down(MinPQ *pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->nodes[left].distance < pq->nodes[smallest].distance) {
        smallest = left;
    }
    if (right < pq->size && pq->nodes[right].distance < pq->nodes[smallest].distance) {
        smallest = right;
    }

    if (smallest != i) {
        minpq_swap(pq, i, smallest);
        minpq_sift_down(pq, smallest);
    }
}

static void minpq_insert(MinPQ *pq, int vertex, int distance) {
    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].distance = distance;
    pq->pos[vertex] = pq->size;
    minpq_sift_up(pq, pq->size);
    pq->size++;
}

static HeapNode minpq_extract_min(MinPQ *pq) {
    HeapNode min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->pos[pq->nodes[0].vertex] = 0;
    pq->size--;
    if (pq->size > 0) {
        minpq_sift_down(pq, 0);
    }
    return min;
}

static void minpq_decrease_key(MinPQ *pq, int vertex, int new_dist) {
    int i = pq->pos[vertex];
    pq->nodes[i].distance = new_dist;
    minpq_sift_up(pq, i);
}

static bool minpq_is_empty(const MinPQ *pq) {
    return pq->size == 0;
}

static bool minpq_contains(const MinPQ *pq, int vertex) {
    return pq->pos[vertex] < pq->size && pq->nodes[pq->pos[vertex]].vertex == vertex;
}

/* ============== Union-Find ============== */

static UnionFind *uf_create(int n) {
    UnionFind *uf = malloc(sizeof(UnionFind));
    if (uf == NULL) return NULL;

    uf->parent = malloc(n * sizeof(int));
    uf->rank = calloc(n, sizeof(int));
    if (uf->parent == NULL || uf->rank == NULL) {
        free(uf->parent);
        free(uf->rank);
        free(uf);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
    }
    uf->n = n;
    return uf;
}

static void uf_destroy(UnionFind *uf) {
    if (uf != NULL) {
        free(uf->parent);
        free(uf->rank);
        free(uf);
    }
}

static int uf_find(UnionFind *uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]); /* Path compression */
    }
    return uf->parent[x];
}

static bool uf_union(UnionFind *uf, int x, int y) {
    int px = uf_find(uf, x);
    int py = uf_find(uf, y);

    if (px == py) return false; /* Already in same set */

    /* Union by rank */
    if (uf->rank[px] < uf->rank[py]) {
        uf->parent[px] = py;
    } else if (uf->rank[px] > uf->rank[py]) {
        uf->parent[py] = px;
    } else {
        uf->parent[py] = px;
        uf->rank[px]++;
    }
    return true;
}

/* ============== Edge Comparison for Kruskal ============== */

static int edge_compare(const void *a, const void *b) {
    return ((EdgeItem *)a)->weight - ((EdgeItem *)b)->weight;
}

/* ============== Graph Creation/Destruction ============== */

Graph *graph_create(int num_vertices, bool directed) {
    if (num_vertices <= 0) return NULL;

    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->adj_list = calloc(num_vertices, sizeof(Edge *));
    if (g->adj_list == NULL) {
        free(g);
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->directed = directed;
    return g;
}

void graph_destroy(Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->num_vertices; i++) {
        Edge *e = g->adj_list[i];
        while (e != NULL) {
            Edge *next = e->next;
            free(e);
            e = next;
        }
    }

    free(g->adj_list);
    free(g);
}

bool graph_add_edge(Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_vertices ||
        dest < 0 || dest >= g->num_vertices) {
        return false;
    }

    /* Add edge src -> dest */
    Edge *e = malloc(sizeof(Edge));
    if (e == NULL) return false;

    e->dest = dest;
    e->weight = weight;
    e->next = g->adj_list[src];
    g->adj_list[src] = e;

    /* If undirected, add reverse edge */
    if (!g->directed) {
        Edge *e2 = malloc(sizeof(Edge));
        if (e2 == NULL) return false;

        e2->dest = src;
        e2->weight = weight;
        e2->next = g->adj_list[dest];
        g->adj_list[dest] = e2;
    }

    return true;
}

bool graph_has_edge(const Graph *g, int src, int dest) {
    if (g == NULL || src < 0 || src >= g->num_vertices ||
        dest < 0 || dest >= g->num_vertices) {
        return false;
    }

    for (Edge *e = g->adj_list[src]; e != NULL; e = e->next) {
        if (e->dest == dest) return true;
    }
    return false;
}

int graph_get_weight(const Graph *g, int src, int dest) {
    if (g == NULL || src < 0 || src >= g->num_vertices ||
        dest < 0 || dest >= g->num_vertices) {
        return GRAPH_INF;
    }

    for (Edge *e = g->adj_list[src]; e != NULL; e = e->next) {
        if (e->dest == dest) return e->weight;
    }
    return GRAPH_INF;
}

int graph_vertex_count(const Graph *g) {
    return g != NULL ? g->num_vertices : 0;
}

int graph_edge_count(const Graph *g) {
    if (g == NULL) return 0;

    int count = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        for (Edge *e = g->adj_list[i]; e != NULL; e = e->next) {
            count++;
        }
    }
    return g->directed ? count : count / 2;
}

int graph_out_degree(const Graph *g, int v) {
    if (g == NULL || v < 0 || v >= g->num_vertices) return 0;

    int degree = 0;
    for (Edge *e = g->adj_list[v]; e != NULL; e = e->next) {
        degree++;
    }
    return degree;
}

int graph_in_degree(const Graph *g, int v) {
    if (g == NULL || v < 0 || v >= g->num_vertices) return 0;

    if (!g->directed) {
        return graph_out_degree(g, v);
    }

    int degree = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        for (Edge *e = g->adj_list[i]; e != NULL; e = e->next) {
            if (e->dest == v) degree++;
        }
    }
    return degree;
}

/* ============== Result Allocation Helpers ============== */

static TraversalResult *traversal_result_create(int n) {
    TraversalResult *result = malloc(sizeof(TraversalResult));
    if (result == NULL) return NULL;

    result->distance = malloc(n * sizeof(int));
    result->parent = malloc(n * sizeof(int));
    result->order = malloc(n * sizeof(int));

    if (result->distance == NULL || result->parent == NULL || result->order == NULL) {
        free(result->distance);
        free(result->parent);
        free(result->order);
        free(result);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        result->distance[i] = -1;
        result->parent[i] = -1;
    }
    result->order_count = 0;
    return result;
}

void traversal_result_free(TraversalResult *result) {
    if (result != NULL) {
        free(result->distance);
        free(result->parent);
        free(result->order);
        free(result);
    }
}

static ShortestPathResult *sp_result_create(int n) {
    ShortestPathResult *result = malloc(sizeof(ShortestPathResult));
    if (result == NULL) return NULL;

    result->distance = malloc(n * sizeof(int));
    result->parent = malloc(n * sizeof(int));

    if (result->distance == NULL || result->parent == NULL) {
        free(result->distance);
        free(result->parent);
        free(result);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        result->distance[i] = GRAPH_INF;
        result->parent[i] = -1;
    }
    result->has_negative_cycle = false;
    return result;
}

void shortest_path_result_free(ShortestPathResult *result) {
    if (result != NULL) {
        free(result->distance);
        free(result->parent);
        free(result);
    }
}

void topo_sort_result_free(TopoSortResult *result) {
    if (result != NULL) {
        free(result->order);
        free(result);
    }
}

void mst_result_free(MSTResult *result) {
    if (result != NULL) {
        if (result->edges != NULL) {
            for (int i = 0; i < result->num_edges; i++) {
                free(result->edges[i]);
            }
            free(result->edges);
        }
        free(result);
    }
}

void scc_result_free(SCCResult *result) {
    if (result != NULL) {
        free(result->component);
        free(result);
    }
}

/* ============== BFS ============== */

TraversalResult *graph_bfs(const Graph *g, int source) {
    if (g == NULL || source < 0 || source >= g->num_vertices) {
        return NULL;
    }

    int n = g->num_vertices;
    TraversalResult *result = traversal_result_create(n);
    if (result == NULL) return NULL;

    /* Queue for BFS */
    int *queue = malloc(n * sizeof(int));
    if (queue == NULL) {
        traversal_result_free(result);
        return NULL;
    }

    int front = 0, rear = 0;
    queue[rear++] = source;
    result->distance[source] = 0;

    while (front < rear) {
        int u = queue[front++];
        result->order[result->order_count++] = u;

        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            int v = e->dest;
            if (result->distance[v] == -1) {
                result->distance[v] = result->distance[u] + 1;
                result->parent[v] = u;
                queue[rear++] = v;
            }
        }
    }

    free(queue);
    return result;
}

/* ============== DFS ============== */

static void dfs_visit(const Graph *g, int u, bool *visited,
                      TraversalResult *result, int parent) {
    visited[u] = true;
    result->parent[u] = parent;
    result->order[result->order_count++] = u;

    for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
        if (!visited[e->dest]) {
            result->distance[e->dest] = result->distance[u] + 1;
            dfs_visit(g, e->dest, visited, result, u);
        }
    }
}

TraversalResult *graph_dfs(const Graph *g, int source) {
    if (g == NULL || source < 0 || source >= g->num_vertices) {
        return NULL;
    }

    int n = g->num_vertices;
    TraversalResult *result = traversal_result_create(n);
    if (result == NULL) return NULL;

    bool *visited = calloc(n, sizeof(bool));
    if (visited == NULL) {
        traversal_result_free(result);
        return NULL;
    }

    result->distance[source] = 0;
    dfs_visit(g, source, visited, result, -1);

    free(visited);
    return result;
}

TraversalResult *graph_dfs_full(const Graph *g) {
    if (g == NULL) return NULL;

    int n = g->num_vertices;
    TraversalResult *result = traversal_result_create(n);
    if (result == NULL) return NULL;

    bool *visited = calloc(n, sizeof(bool));
    if (visited == NULL) {
        traversal_result_free(result);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            result->distance[i] = 0;
            dfs_visit(g, i, visited, result, -1);
        }
    }

    free(visited);
    return result;
}

/* ============== Dijkstra ============== */

ShortestPathResult *graph_dijkstra(const Graph *g, int source) {
    if (g == NULL || source < 0 || source >= g->num_vertices) {
        return NULL;
    }

    int n = g->num_vertices;
    ShortestPathResult *result = sp_result_create(n);
    if (result == NULL) return NULL;

    MinPQ *pq = minpq_create(n);
    if (pq == NULL) {
        shortest_path_result_free(result);
        return NULL;
    }

    /* Initialize */
    for (int i = 0; i < n; i++) {
        minpq_insert(pq, i, i == source ? 0 : GRAPH_INF);
    }
    result->distance[source] = 0;

    while (!minpq_is_empty(pq)) {
        HeapNode min = minpq_extract_min(pq);
        int u = min.vertex;

        if (min.distance == GRAPH_INF) break;

        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            int v = e->dest;
            if (minpq_contains(pq, v)) {
                int new_dist = result->distance[u] + e->weight;
                if (new_dist < result->distance[v]) {
                    result->distance[v] = new_dist;
                    result->parent[v] = u;
                    minpq_decrease_key(pq, v, new_dist);
                }
            }
        }
    }

    minpq_destroy(pq);
    return result;
}

/* ============== Bellman-Ford ============== */

ShortestPathResult *graph_bellman_ford(const Graph *g, int source) {
    if (g == NULL || source < 0 || source >= g->num_vertices) {
        return NULL;
    }

    int n = g->num_vertices;
    ShortestPathResult *result = sp_result_create(n);
    if (result == NULL) return NULL;

    result->distance[source] = 0;

    /* Relax all edges V-1 times */
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            if (result->distance[u] == GRAPH_INF) continue;

            for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
                int v = e->dest;
                int new_dist = result->distance[u] + e->weight;
                if (new_dist < result->distance[v]) {
                    result->distance[v] = new_dist;
                    result->parent[v] = u;
                }
            }
        }
    }

    /* Check for negative cycles */
    for (int u = 0; u < n; u++) {
        if (result->distance[u] == GRAPH_INF) continue;

        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            int v = e->dest;
            if (result->distance[u] + e->weight < result->distance[v]) {
                result->has_negative_cycle = true;
                return result;
            }
        }
    }

    return result;
}

bool shortest_path_reconstruct(const ShortestPathResult *result, int dest,
                               int *path_out, int *path_len_out) {
    if (result == NULL || path_out == NULL || path_len_out == NULL) {
        return false;
    }

    if (result->distance[dest] == GRAPH_INF) {
        *path_len_out = 0;
        return false;
    }

    /* Build path backwards */
    int len = 0;
    int current = dest;
    while (current != -1) {
        path_out[len++] = current;
        current = result->parent[current];
    }

    /* Reverse path */
    for (int i = 0; i < len / 2; i++) {
        int temp = path_out[i];
        path_out[i] = path_out[len - 1 - i];
        path_out[len - 1 - i] = temp;
    }

    *path_len_out = len;
    return true;
}

/* ============== Topological Sort (Kahn's) ============== */

TopoSortResult *graph_topo_sort_kahn(const Graph *g) {
    if (g == NULL || !g->directed) return NULL;

    int n = g->num_vertices;
    TopoSortResult *result = malloc(sizeof(TopoSortResult));
    if (result == NULL) return NULL;

    result->order = malloc(n * sizeof(int));
    if (result->order == NULL) {
        free(result);
        return NULL;
    }
    result->count = 0;
    result->is_dag = true;

    /* Calculate in-degrees */
    int *in_degree = calloc(n, sizeof(int));
    int *queue = malloc(n * sizeof(int));
    if (in_degree == NULL || queue == NULL) {
        free(in_degree);
        free(queue);
        topo_sort_result_free(result);
        return NULL;
    }

    for (int u = 0; u < n; u++) {
        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            in_degree[e->dest]++;
        }
    }

    /* Add all vertices with in-degree 0 to queue */
    int front = 0, rear = 0;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            queue[rear++] = i;
        }
    }

    /* Process queue */
    while (front < rear) {
        int u = queue[front++];
        result->order[result->count++] = u;

        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            in_degree[e->dest]--;
            if (in_degree[e->dest] == 0) {
                queue[rear++] = e->dest;
            }
        }
    }

    /* Check if all vertices processed (no cycle) */
    if (result->count != n) {
        result->is_dag = false;
    }

    free(in_degree);
    free(queue);
    return result;
}

/* ============== Topological Sort (DFS) ============== */

typedef enum { WHITE, GRAY, BLACK } Color;

static bool topo_dfs(const Graph *g, int u, Color *color, int *stack, int *top) {
    color[u] = GRAY;

    for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
        if (color[e->dest] == GRAY) {
            return false; /* Back edge = cycle */
        }
        if (color[e->dest] == WHITE) {
            if (!topo_dfs(g, e->dest, color, stack, top)) {
                return false;
            }
        }
    }

    color[u] = BLACK;
    stack[(*top)++] = u;
    return true;
}

TopoSortResult *graph_topo_sort_dfs(const Graph *g) {
    if (g == NULL || !g->directed) return NULL;

    int n = g->num_vertices;
    TopoSortResult *result = malloc(sizeof(TopoSortResult));
    if (result == NULL) return NULL;

    result->order = malloc(n * sizeof(int));
    if (result->order == NULL) {
        free(result);
        return NULL;
    }
    result->is_dag = true;

    Color *color = calloc(n, sizeof(Color));
    int *stack = malloc(n * sizeof(int));
    if (color == NULL || stack == NULL) {
        free(color);
        free(stack);
        topo_sort_result_free(result);
        return NULL;
    }

    int top = 0;
    for (int i = 0; i < n; i++) {
        if (color[i] == WHITE) {
            if (!topo_dfs(g, i, color, stack, &top)) {
                result->is_dag = false;
                result->count = 0;
                free(color);
                free(stack);
                return result;
            }
        }
    }

    /* Reverse stack to get topological order */
    result->count = n;
    for (int i = 0; i < n; i++) {
        result->order[i] = stack[n - 1 - i];
    }

    free(color);
    free(stack);
    return result;
}

/* ============== Kruskal's MST ============== */

MSTResult *graph_mst_kruskal(const Graph *g) {
    if (g == NULL) return NULL;

    int n = g->num_vertices;
    MSTResult *result = malloc(sizeof(MSTResult));
    if (result == NULL) return NULL;

    result->edges = NULL;
    result->num_edges = 0;
    result->total_weight = 0;

    /* Count edges and create edge array */
    int edge_count = graph_edge_count(g);
    if (edge_count == 0) return result;

    EdgeItem *edges = malloc(edge_count * sizeof(EdgeItem));
    if (edges == NULL) {
        free(result);
        return NULL;
    }

    int idx = 0;
    for (int u = 0; u < n; u++) {
        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            /* For undirected, only add edge once (u < v) */
            if (!g->directed && u > e->dest) continue;
            edges[idx].src = u;
            edges[idx].dest = e->dest;
            edges[idx].weight = e->weight;
            idx++;
        }
    }

    /* Sort edges by weight */
    qsort(edges, idx, sizeof(EdgeItem), edge_compare);

    /* Allocate MST edges array */
    result->edges = malloc((n - 1) * sizeof(int *));
    if (result->edges == NULL) {
        free(edges);
        free(result);
        return NULL;
    }

    /* Union-Find for cycle detection */
    UnionFind *uf = uf_create(n);
    if (uf == NULL) {
        free(edges);
        free(result->edges);
        free(result);
        return NULL;
    }

    /* Build MST */
    for (int i = 0; i < idx && result->num_edges < n - 1; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;

        if (uf_union(uf, u, v)) {
            result->edges[result->num_edges] = malloc(3 * sizeof(int));
            if (result->edges[result->num_edges] == NULL) {
                /* Cleanup on failure */
                for (int j = 0; j < result->num_edges; j++) {
                    free(result->edges[j]);
                }
                free(result->edges);
                free(edges);
                uf_destroy(uf);
                free(result);
                return NULL;
            }
            result->edges[result->num_edges][0] = u;
            result->edges[result->num_edges][1] = v;
            result->edges[result->num_edges][2] = edges[i].weight;
            result->total_weight += edges[i].weight;
            result->num_edges++;
        }
    }

    free(edges);
    uf_destroy(uf);
    return result;
}

/* ============== Prim's MST ============== */

MSTResult *graph_mst_prim(const Graph *g) {
    if (g == NULL) return NULL;

    int n = g->num_vertices;
    MSTResult *result = malloc(sizeof(MSTResult));
    if (result == NULL) return NULL;

    result->edges = malloc((n - 1) * sizeof(int *));
    result->num_edges = 0;
    result->total_weight = 0;

    if (result->edges == NULL) {
        free(result);
        return NULL;
    }

    int *key = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));
    bool *in_mst = calloc(n, sizeof(bool));
    MinPQ *pq = minpq_create(n);

    if (key == NULL || parent == NULL || in_mst == NULL || pq == NULL) {
        free(key);
        free(parent);
        free(in_mst);
        minpq_destroy(pq);
        free(result->edges);
        free(result);
        return NULL;
    }

    /* Initialize */
    for (int i = 0; i < n; i++) {
        key[i] = GRAPH_INF;
        parent[i] = -1;
        minpq_insert(pq, i, i == 0 ? 0 : GRAPH_INF);
    }
    key[0] = 0;

    while (!minpq_is_empty(pq)) {
        HeapNode min = minpq_extract_min(pq);
        int u = min.vertex;

        if (min.distance == GRAPH_INF) break;

        in_mst[u] = true;

        /* Add edge to MST */
        if (parent[u] != -1) {
            result->edges[result->num_edges] = malloc(3 * sizeof(int));
            if (result->edges[result->num_edges] != NULL) {
                result->edges[result->num_edges][0] = parent[u];
                result->edges[result->num_edges][1] = u;
                result->edges[result->num_edges][2] = key[u];
                result->total_weight += key[u];
                result->num_edges++;
            }
        }

        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            int v = e->dest;
            if (!in_mst[v] && e->weight < key[v]) {
                key[v] = e->weight;
                parent[v] = u;
                if (minpq_contains(pq, v)) {
                    minpq_decrease_key(pq, v, e->weight);
                }
            }
        }
    }

    free(key);
    free(parent);
    free(in_mst);
    minpq_destroy(pq);
    return result;
}

/* ============== Tarjan's SCC ============== */

typedef struct {
    int *disc;
    int *low;
    bool *on_stack;
    int *stack;
    int stack_top;
    int time;
    int *component;
    int num_components;
} TarjanState;

static void tarjan_dfs(const Graph *g, int u, TarjanState *state) {
    state->disc[u] = state->low[u] = state->time++;
    state->stack[state->stack_top++] = u;
    state->on_stack[u] = true;

    for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
        int v = e->dest;
        if (state->disc[v] == -1) {
            tarjan_dfs(g, v, state);
            if (state->low[v] < state->low[u]) {
                state->low[u] = state->low[v];
            }
        } else if (state->on_stack[v]) {
            if (state->disc[v] < state->low[u]) {
                state->low[u] = state->disc[v];
            }
        }
    }

    /* If u is root of SCC */
    if (state->low[u] == state->disc[u]) {
        int v;
        do {
            v = state->stack[--state->stack_top];
            state->on_stack[v] = false;
            state->component[v] = state->num_components;
        } while (v != u);
        state->num_components++;
    }
}

SCCResult *graph_scc_tarjan(const Graph *g) {
    if (g == NULL || !g->directed) return NULL;

    int n = g->num_vertices;
    SCCResult *result = malloc(sizeof(SCCResult));
    if (result == NULL) return NULL;

    result->component = malloc(n * sizeof(int));
    if (result->component == NULL) {
        free(result);
        return NULL;
    }
    result->num_components = 0;

    TarjanState state;
    state.disc = malloc(n * sizeof(int));
    state.low = malloc(n * sizeof(int));
    state.on_stack = calloc(n, sizeof(bool));
    state.stack = malloc(n * sizeof(int));
    state.component = result->component;

    if (state.disc == NULL || state.low == NULL ||
        state.on_stack == NULL || state.stack == NULL) {
        free(state.disc);
        free(state.low);
        free(state.on_stack);
        free(state.stack);
        scc_result_free(result);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        state.disc[i] = -1;
    }
    state.stack_top = 0;
    state.time = 0;
    state.num_components = 0;

    for (int i = 0; i < n; i++) {
        if (state.disc[i] == -1) {
            tarjan_dfs(g, i, &state);
        }
    }

    result->num_components = state.num_components;

    free(state.disc);
    free(state.low);
    free(state.on_stack);
    free(state.stack);
    return result;
}

/* ============== Kosaraju's SCC ============== */

static void kosaraju_dfs1(const Graph *g, int u, bool *visited, int *stack, int *top) {
    visited[u] = true;
    for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
        if (!visited[e->dest]) {
            kosaraju_dfs1(g, e->dest, visited, stack, top);
        }
    }
    stack[(*top)++] = u;
}

static void kosaraju_dfs2(const Graph *g_rev, int u, bool *visited,
                          int *component, int comp_id) {
    visited[u] = true;
    component[u] = comp_id;
    for (Edge *e = g_rev->adj_list[u]; e != NULL; e = e->next) {
        if (!visited[e->dest]) {
            kosaraju_dfs2(g_rev, e->dest, visited, component, comp_id);
        }
    }
}

SCCResult *graph_scc_kosaraju(const Graph *g) {
    if (g == NULL || !g->directed) return NULL;

    int n = g->num_vertices;
    SCCResult *result = malloc(sizeof(SCCResult));
    if (result == NULL) return NULL;

    result->component = malloc(n * sizeof(int));
    if (result->component == NULL) {
        free(result);
        return NULL;
    }
    result->num_components = 0;

    /* Create reverse graph */
    Graph *g_rev = graph_create(n, true);
    if (g_rev == NULL) {
        scc_result_free(result);
        return NULL;
    }

    for (int u = 0; u < n; u++) {
        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            graph_add_edge(g_rev, e->dest, u, e->weight);
        }
    }

    bool *visited = calloc(n, sizeof(bool));
    int *stack = malloc(n * sizeof(int));
    if (visited == NULL || stack == NULL) {
        free(visited);
        free(stack);
        graph_destroy(g_rev);
        scc_result_free(result);
        return NULL;
    }

    /* First DFS to fill stack */
    int top = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            kosaraju_dfs1(g, i, visited, stack, &top);
        }
    }

    /* Reset visited */
    memset(visited, false, n * sizeof(bool));

    /* Second DFS on reverse graph in stack order */
    while (top > 0) {
        int u = stack[--top];
        if (!visited[u]) {
            kosaraju_dfs2(g_rev, u, visited, result->component, result->num_components);
            result->num_components++;
        }
    }

    free(visited);
    free(stack);
    graph_destroy(g_rev);
    return result;
}

/* ============== Cycle Detection ============== */

static bool has_cycle_dfs(const Graph *g, int u, Color *color, int parent) {
    color[u] = GRAY;

    for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
        if (color[e->dest] == GRAY) {
            if (g->directed || e->dest != parent) {
                return true;
            }
        }
        if (color[e->dest] == WHITE) {
            if (has_cycle_dfs(g, e->dest, color, u)) {
                return true;
            }
        }
    }

    color[u] = BLACK;
    return false;
}

bool graph_has_cycle_directed(const Graph *g) {
    if (g == NULL || !g->directed) return false;

    int n = g->num_vertices;
    Color *color = calloc(n, sizeof(Color));
    if (color == NULL) return false;

    bool has_cycle = false;
    for (int i = 0; i < n && !has_cycle; i++) {
        if (color[i] == WHITE) {
            has_cycle = has_cycle_dfs(g, i, color, -1);
        }
    }

    free(color);
    return has_cycle;
}

bool graph_has_cycle_undirected(const Graph *g) {
    if (g == NULL || g->directed) return false;

    int n = g->num_vertices;
    UnionFind *uf = uf_create(n);
    if (uf == NULL) return false;

    bool has_cycle = false;
    for (int u = 0; u < n && !has_cycle; u++) {
        for (Edge *e = g->adj_list[u]; e != NULL; e = e->next) {
            if (u < e->dest) { /* Process each edge once */
                if (!uf_union(uf, u, e->dest)) {
                    has_cycle = true;
                    break;
                }
            }
        }
    }

    uf_destroy(uf);
    return has_cycle;
}

/* ============== Utility Functions ============== */

bool graph_is_connected(const Graph *g) {
    if (g == NULL || g->num_vertices == 0) return true;

    TraversalResult *result = graph_bfs(g, 0);
    if (result == NULL) return false;

    bool connected = (result->order_count == g->num_vertices);
    traversal_result_free(result);
    return connected;
}

bool graph_is_dag(const Graph *g) {
    if (g == NULL || !g->directed) return false;
    return !graph_has_cycle_directed(g);
}

void graph_print(const Graph *g) {
    if (g == NULL) {
        printf("(null graph)\n");
        return;
    }

    printf("Graph (%s, %d vertices):\n",
           g->directed ? "directed" : "undirected", g->num_vertices);

    for (int i = 0; i < g->num_vertices; i++) {
        printf("  %d:", i);
        for (Edge *e = g->adj_list[i]; e != NULL; e = e->next) {
            printf(" -> %d(%d)", e->dest, e->weight);
        }
        printf("\n");
    }
}
