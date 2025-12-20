/**
 * Adjacency Matrix Implementation
 */

#include "adjacency_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ============== Creation/Destruction ============== */

AdjMatrix *adjmat_create(int num_vertices, bool directed, bool weighted) {
    if (num_vertices <= 0) return NULL;

    AdjMatrix *g = malloc(sizeof(AdjMatrix));
    if (!g) return NULL;

    g->matrix = malloc(sizeof(int *) * num_vertices);
    if (!g->matrix) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->matrix[i] = calloc(num_vertices, sizeof(int));
        if (!g->matrix[i]) {
            for (int j = 0; j < i; j++) {
                free(g->matrix[j]);
            }
            free(g->matrix);
            free(g);
            return NULL;
        }
    }

    g->num_vertices = num_vertices;
    g->directed = directed;
    g->weighted = weighted;

    return g;
}

void adjmat_destroy(AdjMatrix *g) {
    if (!g) return;

    if (g->matrix) {
        for (int i = 0; i < g->num_vertices; i++) {
            free(g->matrix[i]);
        }
        free(g->matrix);
    }

    free(g);
}

void adjmat_clear(AdjMatrix *g) {
    if (!g) return;

    for (int i = 0; i < g->num_vertices; i++) {
        memset(g->matrix[i], 0, sizeof(int) * g->num_vertices);
    }
}

/* ============== Edge Operations ============== */

bool adjmat_add_edge(AdjMatrix *g, int src, int dest, int weight) {
    if (!g) return false;
    if (src < 0 || src >= g->num_vertices) return false;
    if (dest < 0 || dest >= g->num_vertices) return false;
    if (weight == ADJMAT_NO_EDGE) return false;

    g->matrix[src][dest] = weight;

    if (!g->directed) {
        g->matrix[dest][src] = weight;
    }

    return true;
}

bool adjmat_remove_edge(AdjMatrix *g, int src, int dest) {
    if (!g) return false;
    if (src < 0 || src >= g->num_vertices) return false;
    if (dest < 0 || dest >= g->num_vertices) return false;

    bool had_edge = g->matrix[src][dest] != ADJMAT_NO_EDGE;

    g->matrix[src][dest] = ADJMAT_NO_EDGE;

    if (!g->directed) {
        g->matrix[dest][src] = ADJMAT_NO_EDGE;
    }

    return had_edge;
}

bool adjmat_has_edge(const AdjMatrix *g, int src, int dest) {
    if (!g) return false;
    if (src < 0 || src >= g->num_vertices) return false;
    if (dest < 0 || dest >= g->num_vertices) return false;

    return g->matrix[src][dest] != ADJMAT_NO_EDGE;
}

int adjmat_get_weight(const AdjMatrix *g, int src, int dest) {
    if (!g) return ADJMAT_NO_EDGE;
    if (src < 0 || src >= g->num_vertices) return ADJMAT_NO_EDGE;
    if (dest < 0 || dest >= g->num_vertices) return ADJMAT_NO_EDGE;

    return g->matrix[src][dest];
}

bool adjmat_set_weight(AdjMatrix *g, int src, int dest, int weight) {
    return adjmat_add_edge(g, src, dest, weight);
}

/* ============== Graph Properties ============== */

int adjmat_vertex_count(const AdjMatrix *g) {
    return g ? g->num_vertices : 0;
}

int adjmat_edge_count(const AdjMatrix *g) {
    if (!g) return 0;

    int count = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = 0; j < g->num_vertices; j++) {
            if (g->matrix[i][j] != ADJMAT_NO_EDGE) {
                count++;
            }
        }
    }

    /* For undirected graphs, edges are counted twice */
    if (!g->directed) {
        count /= 2;
    }

    return count;
}

int adjmat_out_degree(const AdjMatrix *g, int v) {
    if (!g) return 0;
    if (v < 0 || v >= g->num_vertices) return 0;

    int degree = 0;
    for (int j = 0; j < g->num_vertices; j++) {
        if (g->matrix[v][j] != ADJMAT_NO_EDGE) {
            degree++;
        }
    }

    return degree;
}

int adjmat_in_degree(const AdjMatrix *g, int v) {
    if (!g) return 0;
    if (v < 0 || v >= g->num_vertices) return 0;

    int degree = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->matrix[i][v] != ADJMAT_NO_EDGE) {
            degree++;
        }
    }

    return degree;
}

bool adjmat_is_directed(const AdjMatrix *g) {
    return g ? g->directed : false;
}

bool adjmat_is_weighted(const AdjMatrix *g) {
    return g ? g->weighted : false;
}

/* ============== Neighbor Operations ============== */

int adjmat_get_neighbors(const AdjMatrix *g, int v, int *neighbors, int max_neighbors) {
    if (!g || !neighbors || max_neighbors <= 0) return 0;
    if (v < 0 || v >= g->num_vertices) return 0;

    int count = 0;
    for (int j = 0; j < g->num_vertices && count < max_neighbors; j++) {
        if (g->matrix[v][j] != ADJMAT_NO_EDGE) {
            neighbors[count++] = j;
        }
    }

    return count;
}

int adjmat_get_neighbors_weighted(const AdjMatrix *g, int v,
                                   int *neighbors, int *weights, int max_neighbors) {
    if (!g || !neighbors || !weights || max_neighbors <= 0) return 0;
    if (v < 0 || v >= g->num_vertices) return 0;

    int count = 0;
    for (int j = 0; j < g->num_vertices && count < max_neighbors; j++) {
        if (g->matrix[v][j] != ADJMAT_NO_EDGE) {
            neighbors[count] = j;
            weights[count] = g->matrix[v][j];
            count++;
        }
    }

    return count;
}

/* ============== Graph Algorithms ============== */

int adjmat_bfs(const AdjMatrix *g, int source, int *order, int max_order) {
    if (!g || !order || max_order <= 0) return 0;
    if (source < 0 || source >= g->num_vertices) return 0;

    bool *visited = calloc(g->num_vertices, sizeof(bool));
    int *queue = malloc(sizeof(int) * g->num_vertices);
    if (!visited || !queue) {
        free(visited);
        free(queue);
        return 0;
    }

    int front = 0, rear = 0;
    int count = 0;

    queue[rear++] = source;
    visited[source] = true;

    while (front < rear && count < max_order) {
        int v = queue[front++];
        order[count++] = v;

        for (int j = 0; j < g->num_vertices; j++) {
            if (g->matrix[v][j] != ADJMAT_NO_EDGE && !visited[j]) {
                visited[j] = true;
                queue[rear++] = j;
            }
        }
    }

    free(visited);
    free(queue);

    return count;
}

static void dfs_helper(const AdjMatrix *g, int v, bool *visited,
                       int *order, int *count, int max_order) {
    if (*count >= max_order) return;

    visited[v] = true;
    order[(*count)++] = v;

    for (int j = 0; j < g->num_vertices && *count < max_order; j++) {
        if (g->matrix[v][j] != ADJMAT_NO_EDGE && !visited[j]) {
            dfs_helper(g, j, visited, order, count, max_order);
        }
    }
}

int adjmat_dfs(const AdjMatrix *g, int source, int *order, int max_order) {
    if (!g || !order || max_order <= 0) return 0;
    if (source < 0 || source >= g->num_vertices) return 0;

    bool *visited = calloc(g->num_vertices, sizeof(bool));
    if (!visited) return 0;

    int count = 0;
    dfs_helper(g, source, visited, order, &count, max_order);

    free(visited);
    return count;
}

bool adjmat_is_connected(const AdjMatrix *g) {
    if (!g || g->num_vertices == 0) return true;

    int *order = malloc(sizeof(int) * g->num_vertices);
    if (!order) return false;

    int visited = adjmat_bfs(g, 0, order, g->num_vertices);
    free(order);

    return visited == g->num_vertices;
}

bool adjmat_transitive_closure(const AdjMatrix *g, bool **closure) {
    if (!g || !closure) return false;

    int n = g->num_vertices;

    /* Initialize closure */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            closure[i][j] = (g->matrix[i][j] != ADJMAT_NO_EDGE) || (i == j);
        }
    }

    /* Floyd-Warshall for reachability */
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
            }
        }
    }

    return true;
}

bool adjmat_floyd_warshall(const AdjMatrix *g, int **dist) {
    if (!g || !dist) return false;

    int n = g->num_vertices;

    /* Initialize distances */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (g->matrix[i][j] != ADJMAT_NO_EDGE) {
                dist[i][j] = g->matrix[i][j];
            } else {
                dist[i][j] = ADJMAT_INF;
            }
        }
    }

    /* Floyd-Warshall */
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != ADJMAT_INF && dist[k][j] != ADJMAT_INF) {
                    int new_dist = dist[i][k] + dist[k][j];
                    if (new_dist < dist[i][j]) {
                        dist[i][j] = new_dist;
                    }
                }
            }
        }
    }

    /* Check for negative cycles */
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            return false;
        }
    }

    return true;
}

/* ============== Utility Functions ============== */

int **adjmat_get_matrix(const AdjMatrix *g) {
    return g ? g->matrix : NULL;
}

AdjMatrix *adjmat_copy(const AdjMatrix *g) {
    if (!g) return NULL;

    AdjMatrix *copy = adjmat_create(g->num_vertices, g->directed, g->weighted);
    if (!copy) return NULL;

    for (int i = 0; i < g->num_vertices; i++) {
        memcpy(copy->matrix[i], g->matrix[i], sizeof(int) * g->num_vertices);
    }

    return copy;
}

AdjMatrix *adjmat_transpose(const AdjMatrix *g) {
    if (!g) return NULL;

    AdjMatrix *trans = adjmat_create(g->num_vertices, g->directed, g->weighted);
    if (!trans) return NULL;

    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = 0; j < g->num_vertices; j++) {
            trans->matrix[j][i] = g->matrix[i][j];
        }
    }

    return trans;
}

void adjmat_print(const AdjMatrix *g) {
    if (!g) {
        printf("(null graph)\n");
        return;
    }

    printf("Adjacency Matrix (%d vertices, %s, %s):\n",
           g->num_vertices,
           g->directed ? "directed" : "undirected",
           g->weighted ? "weighted" : "unweighted");

    /* Print header */
    printf("    ");
    for (int j = 0; j < g->num_vertices; j++) {
        printf("%4d", j);
    }
    printf("\n");

    /* Print rows */
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%3d:", i);
        for (int j = 0; j < g->num_vertices; j++) {
            if (g->matrix[i][j] == ADJMAT_NO_EDGE) {
                printf("   .");
            } else {
                printf("%4d", g->matrix[i][j]);
            }
        }
        printf("\n");
    }
}
