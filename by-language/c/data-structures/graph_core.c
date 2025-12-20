/**
 * Graph Core Implementation
 * See graph_core.h for documentation
 */

#include "graph_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* ============== Result Free Functions ============== */

void traversal_result_free(TraversalResult *result) {
    if (result != NULL) {
        free(result->distance);
        free(result->parent);
        free(result->order);
        free(result);
    }
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
