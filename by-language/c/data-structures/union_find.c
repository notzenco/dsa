/**
 * Union-Find Implementation
 * See union_find.h for documentation
 */

#include "union_find.h"
#include <stdlib.h>

/* ============== Creation/Destruction ============== */

UnionFind *uf_create(int n) {
    if (n <= 0) return NULL;

    UnionFind *uf = malloc(sizeof(UnionFind));
    if (uf == NULL) return NULL;

    uf->parent = malloc(n * sizeof(int));
    uf->rank = calloc(n, sizeof(int));
    uf->size = malloc(n * sizeof(int));

    if (uf->parent == NULL || uf->rank == NULL || uf->size == NULL) {
        free(uf->parent);
        free(uf->rank);
        free(uf->size);
        free(uf);
        return NULL;
    }

    /* Initialize: each element is its own parent with size 1 */
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->size[i] = 1;
    }

    uf->n = n;
    uf->num_components = n;

    return uf;
}

void uf_destroy(UnionFind *uf) {
    if (uf != NULL) {
        free(uf->parent);
        free(uf->rank);
        free(uf->size);
        free(uf);
    }
}

void uf_clear(UnionFind *uf) {
    if (uf == NULL) return;

    for (int i = 0; i < uf->n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
    uf->num_components = uf->n;
}

/* ============== Core Operations ============== */

int uf_find(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->n) {
        return -1;
    }

    /* Path compression: make every node point directly to root */
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

bool uf_union(UnionFind *uf, int x, int y) {
    if (uf == NULL) return false;

    int root_x = uf_find(uf, x);
    int root_y = uf_find(uf, y);

    if (root_x == -1 || root_y == -1) {
        return false; /* Invalid elements */
    }

    if (root_x == root_y) {
        return false; /* Already in same set */
    }

    /* Union by rank: attach smaller tree under larger tree */
    if (uf->rank[root_x] < uf->rank[root_y]) {
        uf->parent[root_x] = root_y;
        uf->size[root_y] += uf->size[root_x];
    } else if (uf->rank[root_x] > uf->rank[root_y]) {
        uf->parent[root_y] = root_x;
        uf->size[root_x] += uf->size[root_y];
    } else {
        /* Same rank: pick one as root and increment its rank */
        uf->parent[root_y] = root_x;
        uf->size[root_x] += uf->size[root_y];
        uf->rank[root_x]++;
    }

    uf->num_components--;
    return true;
}

bool uf_connected(UnionFind *uf, int x, int y) {
    int root_x = uf_find(uf, x);
    int root_y = uf_find(uf, y);

    if (root_x == -1 || root_y == -1) {
        return false;
    }

    return root_x == root_y;
}

/* ============== Component Information ============== */

int uf_component_size(UnionFind *uf, int x) {
    int root = uf_find(uf, x);
    if (root == -1) {
        return 0;
    }
    return uf->size[root];
}

int uf_num_components(const UnionFind *uf) {
    return uf ? uf->num_components : 0;
}

int uf_size(const UnionFind *uf) {
    return uf ? uf->n : 0;
}

/* ============== Utility ============== */

int uf_get_component(UnionFind *uf, int x, int *members) {
    if (uf == NULL || members == NULL) {
        return 0;
    }

    int root = uf_find(uf, x);
    if (root == -1) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < uf->n; i++) {
        if (uf_find(uf, i) == root) {
            members[count++] = i;
        }
    }

    return count;
}

int uf_get_roots(UnionFind *uf, int *roots) {
    if (uf == NULL || roots == NULL) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < uf->n; i++) {
        if (uf->parent[i] == i) {
            roots[count++] = i;
        }
    }

    return count;
}

bool uf_is_root(const UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->n) {
        return false;
    }
    return uf->parent[x] == x;
}
