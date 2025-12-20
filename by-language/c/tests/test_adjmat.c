/**
 * Tests for Adjacency Matrix
 */

#include "test_framework.h"
#include "../data-structures/adjacency_matrix.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(adjmat_create_basic) {
    AdjMatrix *g = adjmat_create(5, false, false);
    ASSERT_NOT_NULL(g);
    ASSERT_EQ(5, adjmat_vertex_count(g));
    ASSERT_EQ(0, adjmat_edge_count(g));
    ASSERT_FALSE(adjmat_is_directed(g));
    ASSERT_FALSE(adjmat_is_weighted(g));
    adjmat_destroy(g);
}

TEST(adjmat_create_directed) {
    AdjMatrix *g = adjmat_create(5, true, false);
    ASSERT_NOT_NULL(g);
    ASSERT_TRUE(adjmat_is_directed(g));
    adjmat_destroy(g);
}

TEST(adjmat_create_weighted) {
    AdjMatrix *g = adjmat_create(5, false, true);
    ASSERT_NOT_NULL(g);
    ASSERT_TRUE(adjmat_is_weighted(g));
    adjmat_destroy(g);
}

TEST(adjmat_create_invalid) {
    ASSERT_TRUE(adjmat_create(0, false, false) == NULL);
    ASSERT_TRUE(adjmat_create(-1, false, false) == NULL);
}

/* ============== Edge Tests ============== */

TEST(adjmat_add_edge_undirected) {
    AdjMatrix *g = adjmat_create(5, false, false);

    ASSERT_TRUE(adjmat_add_edge(g, 0, 1, 1));
    ASSERT_TRUE(adjmat_has_edge(g, 0, 1));
    ASSERT_TRUE(adjmat_has_edge(g, 1, 0));  /* Undirected */
    ASSERT_EQ(1, adjmat_edge_count(g));

    adjmat_destroy(g);
}

TEST(adjmat_add_edge_directed) {
    AdjMatrix *g = adjmat_create(5, true, false);

    ASSERT_TRUE(adjmat_add_edge(g, 0, 1, 1));
    ASSERT_TRUE(adjmat_has_edge(g, 0, 1));
    ASSERT_FALSE(adjmat_has_edge(g, 1, 0));  /* Directed */
    ASSERT_EQ(1, adjmat_edge_count(g));

    adjmat_destroy(g);
}

TEST(adjmat_add_edge_weighted) {
    AdjMatrix *g = adjmat_create(5, false, true);

    ASSERT_TRUE(adjmat_add_edge(g, 0, 1, 5));
    ASSERT_EQ(5, adjmat_get_weight(g, 0, 1));
    ASSERT_EQ(5, adjmat_get_weight(g, 1, 0));

    adjmat_destroy(g);
}

TEST(adjmat_remove_edge) {
    AdjMatrix *g = adjmat_create(5, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    ASSERT_TRUE(adjmat_remove_edge(g, 0, 1));
    ASSERT_FALSE(adjmat_has_edge(g, 0, 1));
    ASSERT_FALSE(adjmat_has_edge(g, 1, 0));
    ASSERT_EQ(0, adjmat_edge_count(g));

    adjmat_destroy(g);
}

TEST(adjmat_remove_edge_not_found) {
    AdjMatrix *g = adjmat_create(5, false, false);
    ASSERT_FALSE(adjmat_remove_edge(g, 0, 1));
    adjmat_destroy(g);
}

TEST(adjmat_add_edge_invalid) {
    AdjMatrix *g = adjmat_create(5, false, false);

    ASSERT_FALSE(adjmat_add_edge(g, -1, 1, 1));
    ASSERT_FALSE(adjmat_add_edge(g, 0, 5, 1));
    ASSERT_FALSE(adjmat_add_edge(g, 0, 1, 0));  /* Weight 0 = no edge */

    adjmat_destroy(g);
}

/* ============== Degree Tests ============== */

TEST(adjmat_degree) {
    AdjMatrix *g = adjmat_create(5, true, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 0, 2, 1);
    adjmat_add_edge(g, 1, 0, 1);

    ASSERT_EQ(2, adjmat_out_degree(g, 0));
    ASSERT_EQ(1, adjmat_in_degree(g, 0));
    ASSERT_EQ(1, adjmat_out_degree(g, 1));
    ASSERT_EQ(1, adjmat_in_degree(g, 1));

    adjmat_destroy(g);
}

/* ============== Neighbor Tests ============== */

TEST(adjmat_get_neighbors) {
    AdjMatrix *g = adjmat_create(5, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 0, 3, 1);

    int neighbors[5];
    int count = adjmat_get_neighbors(g, 0, neighbors, 5);

    ASSERT_EQ(2, count);
    ASSERT_TRUE(neighbors[0] == 1 || neighbors[1] == 1);
    ASSERT_TRUE(neighbors[0] == 3 || neighbors[1] == 3);

    adjmat_destroy(g);
}

TEST(adjmat_get_neighbors_weighted) {
    AdjMatrix *g = adjmat_create(5, false, true);

    adjmat_add_edge(g, 0, 1, 5);
    adjmat_add_edge(g, 0, 2, 10);

    int neighbors[5], weights[5];
    int count = adjmat_get_neighbors_weighted(g, 0, neighbors, weights, 5);

    ASSERT_EQ(2, count);

    adjmat_destroy(g);
}

/* ============== BFS Tests ============== */

TEST(adjmat_bfs_basic) {
    AdjMatrix *g = adjmat_create(5, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 0, 2, 1);
    adjmat_add_edge(g, 1, 3, 1);
    adjmat_add_edge(g, 2, 4, 1);

    int order[5];
    int count = adjmat_bfs(g, 0, order, 5);

    ASSERT_EQ(5, count);
    ASSERT_EQ(0, order[0]);  /* Start vertex first */

    adjmat_destroy(g);
}

TEST(adjmat_bfs_disconnected) {
    AdjMatrix *g = adjmat_create(5, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    /* Vertices 2, 3, 4 are disconnected */

    int order[5];
    int count = adjmat_bfs(g, 0, order, 5);

    ASSERT_EQ(2, count);

    adjmat_destroy(g);
}

/* ============== DFS Tests ============== */

TEST(adjmat_dfs_basic) {
    AdjMatrix *g = adjmat_create(5, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 0, 2, 1);
    adjmat_add_edge(g, 1, 3, 1);
    adjmat_add_edge(g, 2, 4, 1);

    int order[5];
    int count = adjmat_dfs(g, 0, order, 5);

    ASSERT_EQ(5, count);
    ASSERT_EQ(0, order[0]);  /* Start vertex first */

    adjmat_destroy(g);
}

/* ============== Connectivity Tests ============== */

TEST(adjmat_is_connected) {
    AdjMatrix *g = adjmat_create(4, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 1, 2, 1);
    adjmat_add_edge(g, 2, 3, 1);

    ASSERT_TRUE(adjmat_is_connected(g));

    adjmat_destroy(g);
}

TEST(adjmat_is_not_connected) {
    AdjMatrix *g = adjmat_create(4, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    /* 2 and 3 are disconnected */

    ASSERT_FALSE(adjmat_is_connected(g));

    adjmat_destroy(g);
}

/* ============== Floyd-Warshall Tests ============== */

TEST(adjmat_floyd_warshall_basic) {
    AdjMatrix *g = adjmat_create(4, true, true);

    adjmat_add_edge(g, 0, 1, 3);
    adjmat_add_edge(g, 0, 2, 8);
    adjmat_add_edge(g, 1, 2, 2);
    adjmat_add_edge(g, 2, 3, 1);

    int **dist = malloc(sizeof(int *) * 4);
    for (int i = 0; i < 4; i++) {
        dist[i] = malloc(sizeof(int) * 4);
    }

    ASSERT_TRUE(adjmat_floyd_warshall(g, dist));

    ASSERT_EQ(0, dist[0][0]);
    ASSERT_EQ(3, dist[0][1]);
    ASSERT_EQ(5, dist[0][2]);  /* 0->1->2 = 3+2 = 5 */
    ASSERT_EQ(6, dist[0][3]);  /* 0->1->2->3 = 3+2+1 = 6 */

    for (int i = 0; i < 4; i++) {
        free(dist[i]);
    }
    free(dist);

    adjmat_destroy(g);
}

/* ============== Transitive Closure Tests ============== */

TEST(adjmat_transitive_closure_test) {
    AdjMatrix *g = adjmat_create(4, true, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 1, 2, 1);
    adjmat_add_edge(g, 2, 3, 1);

    bool **closure = malloc(sizeof(bool *) * 4);
    for (int i = 0; i < 4; i++) {
        closure[i] = malloc(sizeof(bool) * 4);
    }

    ASSERT_TRUE(adjmat_transitive_closure(g, closure));

    /* 0 can reach all vertices */
    ASSERT_TRUE(closure[0][1]);
    ASSERT_TRUE(closure[0][2]);
    ASSERT_TRUE(closure[0][3]);

    /* 3 cannot reach any other vertex */
    ASSERT_FALSE(closure[3][0]);
    ASSERT_FALSE(closure[3][1]);
    ASSERT_FALSE(closure[3][2]);

    for (int i = 0; i < 4; i++) {
        free(closure[i]);
    }
    free(closure);

    adjmat_destroy(g);
}

/* ============== Utility Tests ============== */

TEST(adjmat_copy_test) {
    AdjMatrix *g = adjmat_create(4, true, true);

    adjmat_add_edge(g, 0, 1, 5);
    adjmat_add_edge(g, 1, 2, 3);

    AdjMatrix *copy = adjmat_copy(g);
    ASSERT_NOT_NULL(copy);
    ASSERT_EQ(adjmat_vertex_count(g), adjmat_vertex_count(copy));
    ASSERT_TRUE(adjmat_has_edge(copy, 0, 1));
    ASSERT_EQ(5, adjmat_get_weight(copy, 0, 1));

    adjmat_destroy(copy);
    adjmat_destroy(g);
}

TEST(adjmat_transpose_test) {
    AdjMatrix *g = adjmat_create(3, true, true);

    adjmat_add_edge(g, 0, 1, 5);
    adjmat_add_edge(g, 0, 2, 3);

    AdjMatrix *trans = adjmat_transpose(g);
    ASSERT_NOT_NULL(trans);
    ASSERT_TRUE(adjmat_has_edge(trans, 1, 0));
    ASSERT_TRUE(adjmat_has_edge(trans, 2, 0));
    ASSERT_FALSE(adjmat_has_edge(trans, 0, 1));

    adjmat_destroy(trans);
    adjmat_destroy(g);
}

TEST(adjmat_clear_test) {
    AdjMatrix *g = adjmat_create(4, false, false);

    adjmat_add_edge(g, 0, 1, 1);
    adjmat_add_edge(g, 1, 2, 1);

    adjmat_clear(g);
    ASSERT_EQ(0, adjmat_edge_count(g));
    ASSERT_FALSE(adjmat_has_edge(g, 0, 1));

    adjmat_destroy(g);
}

/* ============== NULL Safety Tests ============== */

TEST(adjmat_null_safety) {
    int neighbors[5], weights[5], order[5];

    ASSERT_FALSE(adjmat_add_edge(NULL, 0, 1, 1));
    ASSERT_FALSE(adjmat_remove_edge(NULL, 0, 1));
    ASSERT_FALSE(adjmat_has_edge(NULL, 0, 1));
    ASSERT_EQ(0, adjmat_get_weight(NULL, 0, 1));
    ASSERT_EQ(0, adjmat_vertex_count(NULL));
    ASSERT_EQ(0, adjmat_edge_count(NULL));
    ASSERT_EQ(0, adjmat_out_degree(NULL, 0));
    ASSERT_EQ(0, adjmat_in_degree(NULL, 0));
    ASSERT_EQ(0, adjmat_get_neighbors(NULL, 0, neighbors, 5));
    ASSERT_EQ(0, adjmat_get_neighbors_weighted(NULL, 0, neighbors, weights, 5));
    ASSERT_EQ(0, adjmat_bfs(NULL, 0, order, 5));
    ASSERT_EQ(0, adjmat_dfs(NULL, 0, order, 5));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Adjacency Matrix");

    /* Creation */
    RUN_TEST(adjmat_create_basic);
    RUN_TEST(adjmat_create_directed);
    RUN_TEST(adjmat_create_weighted);
    RUN_TEST(adjmat_create_invalid);

    /* Edge Operations */
    RUN_TEST(adjmat_add_edge_undirected);
    RUN_TEST(adjmat_add_edge_directed);
    RUN_TEST(adjmat_add_edge_weighted);
    RUN_TEST(adjmat_remove_edge);
    RUN_TEST(adjmat_remove_edge_not_found);
    RUN_TEST(adjmat_add_edge_invalid);

    /* Degree */
    RUN_TEST(adjmat_degree);

    /* Neighbors */
    RUN_TEST(adjmat_get_neighbors);
    RUN_TEST(adjmat_get_neighbors_weighted);

    /* BFS */
    RUN_TEST(adjmat_bfs_basic);
    RUN_TEST(adjmat_bfs_disconnected);

    /* DFS */
    RUN_TEST(adjmat_dfs_basic);

    /* Connectivity */
    RUN_TEST(adjmat_is_connected);
    RUN_TEST(adjmat_is_not_connected);

    /* Floyd-Warshall */
    RUN_TEST(adjmat_floyd_warshall_basic);

    /* Transitive Closure */
    RUN_TEST(adjmat_transitive_closure_test);

    /* Utility */
    RUN_TEST(adjmat_copy_test);
    RUN_TEST(adjmat_transpose_test);
    RUN_TEST(adjmat_clear_test);

    /* NULL Safety */
    RUN_TEST(adjmat_null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
