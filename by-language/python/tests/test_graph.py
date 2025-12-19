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
