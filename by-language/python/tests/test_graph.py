"""
Tests for Graph implementation.
"""

import pytest
from data_structures.graph import Graph


class TestGraphBasics:
    """Test basic graph operations."""

    def test_init_undirected(self):
        """Test undirected graph initialization."""
        g = Graph()
        assert len(g) == 0
        assert not g.is_directed

    def test_init_directed(self):
        """Test directed graph initialization."""
        g = Graph(directed=True)
        assert g.is_directed

    def test_add_vertex(self):
        """Test adding vertices."""
        g = Graph()
        g.add_vertex(1)
        g.add_vertex(2)

        assert 1 in g
        assert 2 in g
        assert 3 not in g

    def test_add_edge_undirected(self):
        """Test adding edges to undirected graph."""
        g = Graph()
        g.add_edge(1, 2)

        assert g.has_edge(1, 2)
        assert g.has_edge(2, 1)  # Undirected
        assert g.edge_count == 1

    def test_add_edge_directed(self):
        """Test adding edges to directed graph."""
        g = Graph(directed=True)
        g.add_edge(1, 2)

        assert g.has_edge(1, 2)
        assert not g.has_edge(2, 1)  # Directed
        assert g.edge_count == 1

    def test_add_weighted_edge(self):
        """Test adding weighted edges."""
        g = Graph()
        g.add_edge('A', 'B', weight=5.0)

        assert g.get_weight('A', 'B') == 5.0
        assert g.get_weight('B', 'A') == 5.0

    def test_remove_edge(self):
        """Test removing edges."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        assert g.remove_edge(1, 2)
        assert not g.has_edge(1, 2)
        assert not g.has_edge(2, 1)
        assert g.has_edge(2, 3)

    def test_remove_vertex(self):
        """Test removing vertices."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        g.add_edge(1, 3)

        assert g.remove_vertex(2)
        assert 2 not in g
        assert not g.has_edge(1, 2)
        assert g.has_edge(1, 3)

    def test_neighbors(self):
        """Test getting neighbors."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(1, 3)
        g.add_edge(1, 4)

        neighbors = g.neighbors(1)
        assert set(neighbors) == {2, 3, 4}

    def test_degree(self):
        """Test vertex degree."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(1, 3)
        g.add_edge(1, 4)

        assert g.degree(1) == 3
        assert g.degree(2) == 1


class TestGraphTraversal:
    """Test graph traversal algorithms."""

    def setup_method(self):
        """Set up test graph."""
        self.g = Graph()
        # Create: 1 - 2 - 3
        #         |   |
        #         4 - 5
        self.g.add_edge(1, 2)
        self.g.add_edge(2, 3)
        self.g.add_edge(1, 4)
        self.g.add_edge(2, 5)
        self.g.add_edge(4, 5)

    def test_bfs(self):
        """Test BFS traversal."""
        result = self.g.bfs(1)
        assert result[0] == 1
        assert set(result) == {1, 2, 3, 4, 5}

    def test_dfs(self):
        """Test DFS traversal."""
        result = self.g.dfs(1)
        assert result[0] == 1
        assert set(result) == {1, 2, 3, 4, 5}

    def test_dfs_recursive(self):
        """Test recursive DFS."""
        result = self.g.dfs_recursive(1)
        assert result[0] == 1
        assert set(result) == {1, 2, 3, 4, 5}

    def test_bfs_disconnected(self):
        """Test BFS on disconnected component."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(3, 4)

        result = g.bfs(1)
        assert set(result) == {1, 2}

    def test_traversal_empty(self):
        """Test traversal starting from non-existent vertex."""
        g = Graph()
        assert g.bfs(1) == []
        assert g.dfs(1) == []


class TestShortestPath:
    """Test shortest path algorithms."""

    def test_shortest_path_bfs(self):
        """Test unweighted shortest path."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        g.add_edge(1, 4)
        g.add_edge(4, 3)

        path = g.shortest_path_bfs(1, 3)
        assert path is not None
        assert len(path) == 3  # 1 -> 2 -> 3 or 1 -> 4 -> 3

    def test_shortest_path_bfs_no_path(self):
        """Test when no path exists."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_vertex(3)

        assert g.shortest_path_bfs(1, 3) is None

    def test_shortest_path_bfs_same_vertex(self):
        """Test path to same vertex."""
        g = Graph()
        g.add_vertex(1)

        assert g.shortest_path_bfs(1, 1) == [1]

    def test_dijkstra_basic(self):
        """Test Dijkstra's algorithm."""
        g = Graph()
        g.add_edge('A', 'B', 4)
        g.add_edge('A', 'C', 2)
        g.add_edge('B', 'C', 1)
        g.add_edge('B', 'D', 5)
        g.add_edge('C', 'D', 8)

        distances, _ = g.dijkstra('A')

        assert distances['A'] == 0
        assert distances['B'] == 3  # A -> C -> B
        assert distances['C'] == 2
        assert distances['D'] == 8  # A -> C -> B -> D

    def test_dijkstra_path(self):
        """Test Dijkstra path reconstruction."""
        g = Graph()
        g.add_edge('A', 'B', 1)
        g.add_edge('B', 'C', 1)
        g.add_edge('A', 'C', 5)

        result = g.shortest_path_dijkstra('A', 'C')
        assert result is not None
        distance, path = result
        assert distance == 2
        assert path == ['A', 'B', 'C']

    def test_bellman_ford(self):
        """Test Bellman-Ford algorithm."""
        g = Graph(directed=True)
        g.add_edge('A', 'B', 4)
        g.add_edge('A', 'C', 2)
        g.add_edge('B', 'C', -3)  # Negative weight
        g.add_edge('C', 'D', 2)

        result = g.bellman_ford('A')
        assert result is not None
        distances, _ = result
        assert distances['B'] == 4
        assert distances['C'] == 1  # A -> B -> C with negative edge


class TestGraphProperties:
    """Test graph property methods."""

    def test_is_connected(self):
        """Test connectivity check."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        assert g.is_connected()

        g.add_vertex(4)
        assert not g.is_connected()

    def test_connected_components(self):
        """Test finding connected components."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(3, 4)
        g.add_vertex(5)

        components = g.connected_components()
        assert len(components) == 3

    def test_has_cycle_undirected(self):
        """Test cycle detection in undirected graph."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        assert not g.has_cycle()

        g.add_edge(3, 1)
        assert g.has_cycle()

    def test_has_cycle_directed(self):
        """Test cycle detection in directed graph."""
        g = Graph(directed=True)
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        assert not g.has_cycle()

        g.add_edge(3, 1)
        assert g.has_cycle()

    def test_topological_sort(self):
        """Test topological sort."""
        g = Graph(directed=True)
        g.add_edge('A', 'B')
        g.add_edge('A', 'C')
        g.add_edge('B', 'D')
        g.add_edge('C', 'D')

        result = g.topological_sort()
        assert result is not None

        # A must come before B and C
        assert result.index('A') < result.index('B')
        assert result.index('A') < result.index('C')
        # B and C must come before D
        assert result.index('B') < result.index('D')
        assert result.index('C') < result.index('D')

    def test_topological_sort_cycle(self):
        """Test topological sort with cycle."""
        g = Graph(directed=True)
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        g.add_edge(3, 1)

        assert g.topological_sort() is None

    def test_topological_sort_undirected(self):
        """Test topological sort on undirected graph."""
        g = Graph()
        g.add_edge(1, 2)

        assert g.topological_sort() is None

    def test_is_bipartite(self):
        """Test bipartite check."""
        # Bipartite graph (path)
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        g.add_edge(3, 4)
        assert g.is_bipartite()

        # Non-bipartite (odd cycle)
        g.add_edge(1, 3)
        assert not g.is_bipartite()


class TestGraphUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing graph."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        g.clear()
        assert len(g) == 0

    def test_copy(self):
        """Test copying graph."""
        g = Graph()
        g.add_edge(1, 2, 5.0)
        g.add_edge(2, 3, 3.0)

        g2 = g.copy()
        assert g2.has_edge(1, 2)
        assert g2.get_weight(1, 2) == 5.0

        # Modify original, copy should be unchanged
        g.remove_edge(1, 2)
        assert g2.has_edge(1, 2)

    def test_to_adjacency_matrix(self):
        """Test conversion to adjacency matrix."""
        g = Graph()
        g.add_edge(0, 1, 2.0)
        g.add_edge(1, 2, 3.0)

        vertices, matrix = g.to_adjacency_matrix()

        idx = {v: i for i, v in enumerate(vertices)}
        assert matrix[idx[0]][idx[1]] == 2.0
        assert matrix[idx[1]][idx[2]] == 3.0

    def test_from_edge_list(self):
        """Test creating from edge list."""
        edges = [(1, 2, 1.0), (2, 3, 2.0), (3, 1, 3.0)]
        g = Graph.from_edge_list(edges)

        assert len(g) == 3
        assert g.has_edge(1, 2)
        assert g.get_weight(2, 3) == 2.0

    def test_from_edge_list_unweighted(self):
        """Test creating from unweighted edge list."""
        edges = [(1, 2), (2, 3)]
        g = Graph.from_edge_list(edges)

        assert g.has_edge(1, 2)
        assert g.get_weight(1, 2) == 1.0

    def test_vertices_property(self):
        """Test vertices property."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        assert set(g.vertices) == {1, 2, 3}

    def test_in_degree(self):
        """Test in-degree for directed graph."""
        g = Graph(directed=True)
        g.add_edge(1, 2)
        g.add_edge(3, 2)
        g.add_edge(4, 2)

        assert g.in_degree(2) == 3
        assert g.in_degree(1) == 0

    def test_iter(self):
        """Test iteration over vertices."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        vertices = list(g)
        assert set(vertices) == {1, 2, 3}


class TestEdgeCases:
    """Test edge cases."""

    def test_empty_graph(self):
        """Test operations on empty graph."""
        g = Graph()
        assert g.is_connected()
        assert g.connected_components() == []
        assert not g.has_cycle()
        assert g.is_bipartite()

    def test_single_vertex(self):
        """Test graph with single vertex."""
        g = Graph()
        g.add_vertex(1)

        assert g.bfs(1) == [1]
        assert g.is_connected()
        assert not g.has_cycle()

    def test_self_loop(self):
        """Test self-loop."""
        g = Graph()
        g.add_edge(1, 1)

        assert g.has_edge(1, 1)
        assert g.has_cycle()

    def test_repr(self):
        """Test string representation."""
        g = Graph()
        g.add_edge(1, 2)

        s = repr(g)
        assert "Undirected" in s
        assert "vertices=2" in s


class TestMinimumSpanningTree:
    """Test MST algorithms."""

    def setup_method(self):
        """Set up test graph for MST."""
        # Create weighted undirected graph:
        #       4
        #   A -------- B
        #   |\ 8     / |
        # 2 | \    / 2 | 7
        #   |  \ /     |
        #   C --D------E
        #     3    9
        self.g = Graph()
        self.g.add_edge('A', 'B', 4)
        self.g.add_edge('A', 'C', 2)
        self.g.add_edge('A', 'D', 8)
        self.g.add_edge('B', 'D', 2)
        self.g.add_edge('B', 'E', 7)
        self.g.add_edge('C', 'D', 3)
        self.g.add_edge('D', 'E', 9)

    def test_prim_mst(self):
        """Test Prim's algorithm."""
        edges, total_weight = self.g.prim_mst()

        # MST should have V-1 = 4 edges
        assert len(edges) == 4

        # Optimal MST weight: A-C(2) + C-D(3) + D-B(2) + B-E(7) = 14
        # or A-C(2) + A-B(4) + B-D(2) + B-E(7) = 15
        # Actually: A-C(2) + B-D(2) + C-D(3) + B-E(7) = 14
        assert total_weight == 14

    def test_kruskal_mst(self):
        """Test Kruskal's algorithm."""
        edges, total_weight = self.g.kruskal_mst()

        assert len(edges) == 4
        assert total_weight == 14

    def test_mst_same_result(self):
        """Both algorithms should produce same total weight."""
        prim_edges, prim_weight = self.g.prim_mst()
        kruskal_edges, kruskal_weight = self.g.kruskal_mst()

        assert prim_weight == kruskal_weight

    def test_mst_empty_graph(self):
        """Test MST on empty graph."""
        g = Graph()
        edges, weight = g.prim_mst()
        assert edges == []
        assert weight == 0.0

    def test_mst_single_vertex(self):
        """Test MST with single vertex."""
        g = Graph()
        g.add_vertex(1)
        edges, weight = g.prim_mst()
        assert edges == []
        assert weight == 0.0

    def test_mst_directed_error(self):
        """Test that MST raises error for directed graph."""
        g = Graph(directed=True)
        g.add_edge(1, 2, 1.0)

        with pytest.raises(ValueError):
            g.prim_mst()

        with pytest.raises(ValueError):
            g.kruskal_mst()


class TestFloydWarshall:
    """Test Floyd-Warshall algorithm."""

    def test_floyd_warshall_basic(self):
        """Test basic Floyd-Warshall."""
        g = Graph(directed=True)
        g.add_edge(1, 2, 3)
        g.add_edge(2, 3, 1)
        g.add_edge(1, 3, 7)

        vertices, dist, pred = g.floyd_warshall()
        idx = {v: i for i, v in enumerate(vertices)}

        assert dist[idx[1]][idx[2]] == 3
        assert dist[idx[1]][idx[3]] == 4  # 1 -> 2 -> 3 = 4, not direct 7
        assert dist[idx[2]][idx[3]] == 1

    def test_floyd_warshall_path(self):
        """Test Floyd-Warshall path reconstruction."""
        g = Graph()
        g.add_edge('A', 'B', 1)
        g.add_edge('B', 'C', 2)
        g.add_edge('A', 'C', 10)

        result = g.floyd_warshall_path('A', 'C')
        assert result is not None
        distance, path = result
        assert distance == 3  # A -> B -> C
        assert path == ['A', 'B', 'C']

    def test_floyd_warshall_no_path(self):
        """Test Floyd-Warshall when no path exists."""
        g = Graph(directed=True)
        g.add_edge(1, 2, 1)
        g.add_vertex(3)

        result = g.floyd_warshall_path(1, 3)
        assert result is None


class TestStronglyConnectedComponents:
    """Test SCC algorithms."""

    def setup_method(self):
        """Set up test graph for SCC."""
        # Create directed graph with 3 SCCs:
        #   1 -> 2 -> 3
        #   ^    |    |
        #   |    v    v
        #   4 <- 5    6 -> 7
        #             ^    |
        #             +----+
        self.g = Graph(directed=True)
        self.g.add_edge(1, 2)
        self.g.add_edge(2, 3)
        self.g.add_edge(2, 5)
        self.g.add_edge(3, 6)
        self.g.add_edge(5, 4)
        self.g.add_edge(4, 1)
        self.g.add_edge(6, 7)
        self.g.add_edge(7, 6)

    def test_tarjan_scc(self):
        """Test Tarjan's SCC algorithm."""
        sccs = self.g.tarjan_scc()

        # Should find 3 SCCs: {1,2,4,5}, {3}, {6,7}
        assert len(sccs) == 3

        scc_sets = [set(scc) for scc in sccs]
        assert {1, 2, 4, 5} in scc_sets
        assert {3} in scc_sets
        assert {6, 7} in scc_sets

    def test_kosaraju_scc(self):
        """Test Kosaraju's SCC algorithm."""
        sccs = self.g.kosaraju_scc()

        assert len(sccs) == 3

        scc_sets = [set(scc) for scc in sccs]
        assert {1, 2, 4, 5} in scc_sets
        assert {3} in scc_sets
        assert {6, 7} in scc_sets

    def test_scc_same_result(self):
        """Both SCC algorithms should find same components."""
        tarjan = self.g.tarjan_scc()
        kosaraju = self.g.kosaraju_scc()

        tarjan_sets = {frozenset(scc) for scc in tarjan}
        kosaraju_sets = {frozenset(scc) for scc in kosaraju}

        assert tarjan_sets == kosaraju_sets

    def test_scc_undirected(self):
        """Test SCC on undirected graph (should use connected components)."""
        g = Graph()
        g.add_edge(1, 2)
        g.add_edge(3, 4)

        sccs = g.tarjan_scc()
        assert len(sccs) == 2

    def test_scc_single_vertex(self):
        """Test SCC with isolated vertices."""
        g = Graph(directed=True)
        g.add_vertex(1)
        g.add_vertex(2)
        g.add_edge(3, 4)

        sccs = g.tarjan_scc()
        # {1}, {2}, {3}, {4} - 4 single-vertex SCCs
        # Since 3->4 is directed without cycle back, they're separate SCCs
        assert len(sccs) == 4

    def test_scc_strongly_connected(self):
        """Test fully strongly connected graph."""
        g = Graph(directed=True)
        g.add_edge(1, 2)
        g.add_edge(2, 3)
        g.add_edge(3, 1)

        sccs = g.tarjan_scc()
        assert len(sccs) == 1
        assert set(sccs[0]) == {1, 2, 3}
