/**
 * Tests for Graph Data Structure and Algorithms
 */

#include "test_framework.h"
#include "../data-structures/graph.h"
#include <stdlib.h>

/* ============== Graph Creation Tests ============== */

TEST(graph_create_directed) {
    Graph *g = graph_create(5, true);
    ASSERT_NOT_NULL(g);
    ASSERT_EQ(5, graph_vertex_count(g));
    ASSERT_EQ(0, graph_edge_count(g));
    graph_destroy(g);
}

TEST(graph_create_undirected) {
    Graph *g = graph_create(5, false);
    ASSERT_NOT_NULL(g);
    ASSERT_EQ(5, graph_vertex_count(g));
    graph_destroy(g);
}

TEST(graph_add_edge_directed) {
    Graph *g = graph_create(5, true);
    ASSERT_TRUE(graph_add_edge(g, 0, 1, 10));
    ASSERT_TRUE(graph_has_edge(g, 0, 1));
    ASSERT_FALSE(graph_has_edge(g, 1, 0)); /* Directed */
    ASSERT_EQ(10, graph_get_weight(g, 0, 1));
    ASSERT_EQ(1, graph_edge_count(g));
    graph_destroy(g);
}

TEST(graph_add_edge_undirected) {
    Graph *g = graph_create(5, false);
    ASSERT_TRUE(graph_add_edge(g, 0, 1, 10));
    ASSERT_TRUE(graph_has_edge(g, 0, 1));
    ASSERT_TRUE(graph_has_edge(g, 1, 0)); /* Undirected */
    ASSERT_EQ(1, graph_edge_count(g));
    graph_destroy(g);
}

TEST(graph_degrees) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 3, 0, 1);

    ASSERT_EQ(2, graph_out_degree(g, 0));
    ASSERT_EQ(1, graph_in_degree(g, 0));
    ASSERT_EQ(1, graph_out_degree(g, 1));
    ASSERT_EQ(2, graph_in_degree(g, 2));
    graph_destroy(g);
}

/* ============== BFS Tests ============== */

TEST(bfs_simple) {
    Graph *g = graph_create(5, false);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 4, 1);

    TraversalResult *result = graph_bfs(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(5, result->order_count);
    ASSERT_EQ(0, result->distance[0]);
    ASSERT_EQ(1, result->distance[1]);
    ASSERT_EQ(1, result->distance[2]);
    ASSERT_EQ(2, result->distance[3]);
    ASSERT_EQ(2, result->distance[4]);

    traversal_result_free(result);
    graph_destroy(g);
}

TEST(bfs_disconnected) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 1);
    /* 2 and 3 are disconnected */

    TraversalResult *result = graph_bfs(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(2, result->order_count);
    ASSERT_EQ(-1, result->distance[2]);
    ASSERT_EQ(-1, result->distance[3]);

    traversal_result_free(result);
    graph_destroy(g);
}

/* ============== DFS Tests ============== */

TEST(dfs_simple) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 3, 1);

    TraversalResult *result = graph_dfs(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(0, result->order[0]); /* Starts at source */
    ASSERT_EQ(-1, result->parent[0]); /* Source has no parent */

    traversal_result_free(result);
    graph_destroy(g);
}

TEST(dfs_full) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    /* 2 -> 3 is separate component */
    graph_add_edge(g, 2, 3, 1);

    TraversalResult *result = graph_dfs_full(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(4, result->order_count);

    traversal_result_free(result);
    graph_destroy(g);
}

/* ============== Dijkstra Tests ============== */

TEST(dijkstra_simple) {
    Graph *g = graph_create(5, true);
    graph_add_edge(g, 0, 1, 4);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 2, 1, 2);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 3, 5);
    graph_add_edge(g, 3, 4, 3);

    ShortestPathResult *result = graph_dijkstra(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(0, result->distance[0]);
    ASSERT_EQ(3, result->distance[1]); /* 0->2->1 = 1+2 = 3 */
    ASSERT_EQ(1, result->distance[2]);
    ASSERT_EQ(4, result->distance[3]); /* 0->2->1->3 = 1+2+1 = 4 */
    ASSERT_EQ(7, result->distance[4]);

    shortest_path_result_free(result);
    graph_destroy(g);
}

TEST(dijkstra_unreachable) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    /* Vertex 2 is unreachable */

    ShortestPathResult *result = graph_dijkstra(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(0, result->distance[0]);
    ASSERT_EQ(1, result->distance[1]);
    ASSERT_EQ(GRAPH_INF, result->distance[2]);

    shortest_path_result_free(result);
    graph_destroy(g);
}

TEST(dijkstra_path_reconstruction) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 3, 1);

    ShortestPathResult *result = graph_dijkstra(g, 0);
    int path[4];
    int path_len;

    ASSERT_TRUE(shortest_path_reconstruct(result, 3, path, &path_len));
    ASSERT_EQ(4, path_len);
    ASSERT_EQ(0, path[0]);
    ASSERT_EQ(1, path[1]);
    ASSERT_EQ(2, path[2]);
    ASSERT_EQ(3, path[3]);

    shortest_path_result_free(result);
    graph_destroy(g);
}

/* ============== Bellman-Ford Tests ============== */

TEST(bellman_ford_simple) {
    Graph *g = graph_create(5, true);
    graph_add_edge(g, 0, 1, 4);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 2, 1, 2);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 3, 5);
    graph_add_edge(g, 3, 4, 3);

    ShortestPathResult *result = graph_bellman_ford(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_FALSE(result->has_negative_cycle);
    ASSERT_EQ(0, result->distance[0]);
    ASSERT_EQ(3, result->distance[1]);
    ASSERT_EQ(1, result->distance[2]);
    ASSERT_EQ(4, result->distance[3]);
    ASSERT_EQ(7, result->distance[4]);

    shortest_path_result_free(result);
    graph_destroy(g);
}

TEST(bellman_ford_negative_edges) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, -2);
    graph_add_edge(g, 2, 3, 1);

    ShortestPathResult *result = graph_bellman_ford(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_FALSE(result->has_negative_cycle);
    ASSERT_EQ(0, result->distance[0]);
    ASSERT_EQ(1, result->distance[1]);
    ASSERT_EQ(-1, result->distance[2]);
    ASSERT_EQ(0, result->distance[3]);

    shortest_path_result_free(result);
    graph_destroy(g);
}

TEST(bellman_ford_negative_cycle) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, -1);
    graph_add_edge(g, 2, 0, -1);

    ShortestPathResult *result = graph_bellman_ford(g, 0);
    ASSERT_NOT_NULL(result);
    ASSERT_TRUE(result->has_negative_cycle);

    shortest_path_result_free(result);
    graph_destroy(g);
}

/* ============== Topological Sort Tests ============== */

TEST(topo_sort_kahn_simple) {
    Graph *g = graph_create(6, true);
    graph_add_edge(g, 5, 2, 1);
    graph_add_edge(g, 5, 0, 1);
    graph_add_edge(g, 4, 0, 1);
    graph_add_edge(g, 4, 1, 1);
    graph_add_edge(g, 2, 3, 1);
    graph_add_edge(g, 3, 1, 1);

    TopoSortResult *result = graph_topo_sort_kahn(g);
    ASSERT_NOT_NULL(result);
    ASSERT_TRUE(result->is_dag);
    ASSERT_EQ(6, result->count);

    /* Verify topological order: all edges go forward */
    int pos[6];
    for (int i = 0; i < 6; i++) {
        pos[result->order[i]] = i;
    }
    ASSERT_TRUE(pos[5] < pos[2]);
    ASSERT_TRUE(pos[5] < pos[0]);
    ASSERT_TRUE(pos[2] < pos[3]);
    ASSERT_TRUE(pos[3] < pos[1]);

    topo_sort_result_free(result);
    graph_destroy(g);
}

TEST(topo_sort_kahn_cycle) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1); /* Cycle */

    TopoSortResult *result = graph_topo_sort_kahn(g);
    ASSERT_NOT_NULL(result);
    ASSERT_FALSE(result->is_dag);

    topo_sort_result_free(result);
    graph_destroy(g);
}

TEST(topo_sort_dfs_simple) {
    Graph *g = graph_create(4, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 0, 2, 1);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 3, 1);

    TopoSortResult *result = graph_topo_sort_dfs(g);
    ASSERT_NOT_NULL(result);
    ASSERT_TRUE(result->is_dag);
    ASSERT_EQ(4, result->count);

    /* Verify: 0 before 1 and 2, both before 3 */
    int pos[4];
    for (int i = 0; i < 4; i++) {
        pos[result->order[i]] = i;
    }
    ASSERT_TRUE(pos[0] < pos[1]);
    ASSERT_TRUE(pos[0] < pos[2]);
    ASSERT_TRUE(pos[1] < pos[3]);
    ASSERT_TRUE(pos[2] < pos[3]);

    topo_sort_result_free(result);
    graph_destroy(g);
}

/* ============== MST Tests ============== */

TEST(mst_kruskal_simple) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 10);
    graph_add_edge(g, 0, 2, 6);
    graph_add_edge(g, 0, 3, 5);
    graph_add_edge(g, 1, 3, 15);
    graph_add_edge(g, 2, 3, 4);

    MSTResult *result = graph_mst_kruskal(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result->num_edges);
    ASSERT_EQ(19, result->total_weight); /* 4 + 5 + 10 = 19 */

    mst_result_free(result);
    graph_destroy(g);
}

TEST(mst_prim_simple) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 10);
    graph_add_edge(g, 0, 2, 6);
    graph_add_edge(g, 0, 3, 5);
    graph_add_edge(g, 1, 3, 15);
    graph_add_edge(g, 2, 3, 4);

    MSTResult *result = graph_mst_prim(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result->num_edges);
    ASSERT_EQ(19, result->total_weight);

    mst_result_free(result);
    graph_destroy(g);
}

TEST(mst_complete_graph) {
    /* K4 complete graph */
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 0, 2, 2);
    graph_add_edge(g, 0, 3, 3);
    graph_add_edge(g, 1, 2, 4);
    graph_add_edge(g, 1, 3, 5);
    graph_add_edge(g, 2, 3, 6);

    MSTResult *kruskal = graph_mst_kruskal(g);
    MSTResult *prim = graph_mst_prim(g);

    ASSERT_EQ(kruskal->total_weight, prim->total_weight);
    ASSERT_EQ(6, kruskal->total_weight); /* 1 + 2 + 3 = 6 */

    mst_result_free(kruskal);
    mst_result_free(prim);
    graph_destroy(g);
}

/* ============== SCC Tests ============== */

TEST(scc_tarjan_simple) {
    Graph *g = graph_create(5, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1); /* SCC: {0,1,2} */
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 3, 4, 1); /* Separate: {3}, {4} */

    SCCResult *result = graph_scc_tarjan(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result->num_components);

    /* Vertices 0, 1, 2 should be in same component */
    ASSERT_EQ(result->component[0], result->component[1]);
    ASSERT_EQ(result->component[1], result->component[2]);

    /* 3 and 4 in different components */
    ASSERT_TRUE(result->component[3] != result->component[0]);
    ASSERT_TRUE(result->component[4] != result->component[3]);

    scc_result_free(result);
    graph_destroy(g);
}

TEST(scc_kosaraju_simple) {
    Graph *g = graph_create(5, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 3, 4, 1);

    SCCResult *result = graph_scc_kosaraju(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result->num_components);

    ASSERT_EQ(result->component[0], result->component[1]);
    ASSERT_EQ(result->component[1], result->component[2]);

    scc_result_free(result);
    graph_destroy(g);
}

TEST(scc_all_separate) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    /* No back edges, all vertices in separate SCCs */

    SCCResult *result = graph_scc_tarjan(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result->num_components);

    scc_result_free(result);
    graph_destroy(g);
}

TEST(scc_single_component) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1);

    SCCResult *result = graph_scc_tarjan(g);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(1, result->num_components);

    scc_result_free(result);
    graph_destroy(g);
}

/* ============== Cycle Detection Tests ============== */

TEST(cycle_detection_directed_has_cycle) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1);

    ASSERT_TRUE(graph_has_cycle_directed(g));
    graph_destroy(g);
}

TEST(cycle_detection_directed_no_cycle) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);

    ASSERT_FALSE(graph_has_cycle_directed(g));
    graph_destroy(g);
}

TEST(cycle_detection_undirected_has_cycle) {
    Graph *g = graph_create(3, false);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1);

    ASSERT_TRUE(graph_has_cycle_undirected(g));
    graph_destroy(g);
}

TEST(cycle_detection_undirected_no_cycle) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 3, 1);

    ASSERT_FALSE(graph_has_cycle_undirected(g));
    graph_destroy(g);
}

/* ============== Utility Tests ============== */

TEST(graph_is_connected_true) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 3, 1);

    ASSERT_TRUE(graph_is_connected(g));
    graph_destroy(g);
}

TEST(graph_is_connected_false) {
    Graph *g = graph_create(4, false);
    graph_add_edge(g, 0, 1, 1);
    /* 2, 3 disconnected */

    ASSERT_FALSE(graph_is_connected(g));
    graph_destroy(g);
}

TEST(graph_is_dag_true) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);

    ASSERT_TRUE(graph_is_dag(g));
    graph_destroy(g);
}

TEST(graph_is_dag_false) {
    Graph *g = graph_create(3, true);
    graph_add_edge(g, 0, 1, 1);
    graph_add_edge(g, 1, 2, 1);
    graph_add_edge(g, 2, 0, 1);

    ASSERT_FALSE(graph_is_dag(g));
    graph_destroy(g);
}

/* ============== NULL Safety Tests ============== */

TEST(null_safety) {
    ASSERT_EQ(0, graph_vertex_count(NULL));
    ASSERT_EQ(0, graph_edge_count(NULL));
    ASSERT_FALSE(graph_add_edge(NULL, 0, 1, 1));
    ASSERT_FALSE(graph_has_edge(NULL, 0, 1));
    ASSERT_EQ(GRAPH_INF, graph_get_weight(NULL, 0, 1));
    ASSERT_EQ(NULL, graph_bfs(NULL, 0));
    ASSERT_EQ(NULL, graph_dfs(NULL, 0));
    ASSERT_EQ(NULL, graph_dijkstra(NULL, 0));
    ASSERT_EQ(NULL, graph_bellman_ford(NULL, 0));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Graph Data Structure and Algorithms");

    /* Creation tests */
    RUN_TEST(graph_create_directed);
    RUN_TEST(graph_create_undirected);
    RUN_TEST(graph_add_edge_directed);
    RUN_TEST(graph_add_edge_undirected);
    RUN_TEST(graph_degrees);

    /* BFS tests */
    RUN_TEST(bfs_simple);
    RUN_TEST(bfs_disconnected);

    /* DFS tests */
    RUN_TEST(dfs_simple);
    RUN_TEST(dfs_full);

    /* Dijkstra tests */
    RUN_TEST(dijkstra_simple);
    RUN_TEST(dijkstra_unreachable);
    RUN_TEST(dijkstra_path_reconstruction);

    /* Bellman-Ford tests */
    RUN_TEST(bellman_ford_simple);
    RUN_TEST(bellman_ford_negative_edges);
    RUN_TEST(bellman_ford_negative_cycle);

    /* Topological sort tests */
    RUN_TEST(topo_sort_kahn_simple);
    RUN_TEST(topo_sort_kahn_cycle);
    RUN_TEST(topo_sort_dfs_simple);

    /* MST tests */
    RUN_TEST(mst_kruskal_simple);
    RUN_TEST(mst_prim_simple);
    RUN_TEST(mst_complete_graph);

    /* SCC tests */
    RUN_TEST(scc_tarjan_simple);
    RUN_TEST(scc_kosaraju_simple);
    RUN_TEST(scc_all_separate);
    RUN_TEST(scc_single_component);

    /* Cycle detection tests */
    RUN_TEST(cycle_detection_directed_has_cycle);
    RUN_TEST(cycle_detection_directed_no_cycle);
    RUN_TEST(cycle_detection_undirected_has_cycle);
    RUN_TEST(cycle_detection_undirected_no_cycle);

    /* Utility tests */
    RUN_TEST(graph_is_connected_true);
    RUN_TEST(graph_is_connected_false);
    RUN_TEST(graph_is_dag_true);
    RUN_TEST(graph_is_dag_false);

    /* NULL safety */
    RUN_TEST(null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
