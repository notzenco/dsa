"""
Tests for AdjacencyMatrix implementation.
"""

import pytest
from data_structures.adjacency_matrix import AdjacencyMatrix


class TestAdjacencyMatrixBasics:
    """Test basic adjacency matrix operations."""

    def test_init_default(self):
        """Test default initialization."""
        g = AdjacencyMatrix(5)
        assert len(g) == 5
        assert g.num_vertices == 5
        assert not g.is_directed
        assert not g.is_weighted
        assert g.edge_count == 0

    def test_init_directed(self):
        """Test directed graph initialization."""
        g = AdjacencyMatrix(5, directed=True)
        assert g.is_directed

    def test_init_weighted(self):
        """Test weighted graph initialization."""
        g = AdjacencyMatrix(5, weighted=True)
        assert g.is_weighted

    def test_init_invalid(self):
        """Test invalid initialization."""
        with pytest.raises(ValueError):
            AdjacencyMatrix(-1)

    def test_contains(self):
        """Test vertex containment."""
        g = AdjacencyMatrix(5)
        assert 0 in g
        assert 4 in g
        assert 5 not in g
        assert -1 not in g

    def test_iter(self):
        """Test iteration over vertices."""
        g = AdjacencyMatrix(5)
        vertices = list(g)
        assert vertices == [0, 1, 2, 3, 4]


class TestEdgeOperations:
    """Test edge operations."""

    def test_add_edge_undirected(self):
        """Test adding edges to undirected graph."""
        g = AdjacencyMatrix(5)
        assert g.add_edge(0, 1)

        assert g.has_edge(0, 1)
        assert g.has_edge(1, 0)  # Undirected
        assert g.edge_count == 1

    def test_add_edge_directed(self):
        """Test adding edges to directed graph."""
        g = AdjacencyMatrix(5, directed=True)
        assert g.add_edge(0, 1)

        assert g.has_edge(0, 1)
        assert not g.has_edge(1, 0)  # Directed
        assert g.edge_count == 1

    def test_add_weighted_edge(self):
        """Test adding weighted edges."""
        g = AdjacencyMatrix(5, weighted=True)
        g.add_edge(0, 1, weight=5)

        assert g.get_weight(0, 1) == 5
        assert g.get_weight(1, 0) == 5

    def test_add_edge_invalid_vertex(self):
        """Test adding edge with invalid vertices."""
        g = AdjacencyMatrix(5)
        assert not g.add_edge(-1, 0)
        assert not g.add_edge(0, 5)
        assert not g.add_edge(10, 20)

    def test_add_edge_zero_weight(self):
        """Test adding edge with zero weight fails."""
        g = AdjacencyMatrix(5)
        assert not g.add_edge(0, 1, weight=0)

    def test_remove_edge(self):
        """Test removing edges."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(1, 2)

        assert g.remove_edge(0, 1)
        assert not g.has_edge(0, 1)
        assert not g.has_edge(1, 0)
        assert g.has_edge(1, 2)

    def test_remove_nonexistent_edge(self):
        """Test removing edge that doesn't exist."""
        g = AdjacencyMatrix(5)
        assert not g.remove_edge(0, 1)

    def test_set_weight(self):
        """Test setting edge weight."""
        g = AdjacencyMatrix(5, weighted=True)
        g.add_edge(0, 1, weight=5)
        g.set_weight(0, 1, 10)

        assert g.get_weight(0, 1) == 10


class TestVertexProperties:
    """Test vertex property methods."""

    def test_out_degree(self):
        """Test out-degree calculation."""
        g = AdjacencyMatrix(5, directed=True)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(0, 3)
        g.add_edge(1, 0)

        assert g.out_degree(0) == 3
        assert g.out_degree(1) == 1
        assert g.out_degree(2) == 0

    def test_in_degree(self):
        """Test in-degree calculation."""
        g = AdjacencyMatrix(5, directed=True)
        g.add_edge(0, 1)
        g.add_edge(2, 1)
        g.add_edge(3, 1)

        assert g.in_degree(1) == 3
        assert g.in_degree(0) == 0

    def test_degree_undirected(self):
        """Test degree in undirected graph."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(0, 3)

        assert g.degree(0) == 3
        assert g.degree(1) == 1

    def test_neighbors(self):
        """Test getting neighbors."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(0, 4)

        neighbors = g.neighbors(0)
        assert set(neighbors) == {1, 2, 4}

    def test_neighbors_weighted(self):
        """Test getting neighbors with weights."""
        g = AdjacencyMatrix(5, weighted=True)
        g.add_edge(0, 1, weight=5)
        g.add_edge(0, 2, weight=3)

        neighbors = g.neighbors_weighted(0)
        assert set(neighbors) == {(1, 5), (2, 3)}

    def test_neighbors_invalid_vertex(self):
        """Test neighbors of invalid vertex."""
        g = AdjacencyMatrix(5)
        assert g.neighbors(10) == []
        assert g.neighbors_weighted(10) == []


class TestGraphTraversal:
    """Test graph traversal algorithms."""

    def test_bfs(self):
        """Test BFS traversal."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(1, 3)
        g.add_edge(2, 4)

        result = g.bfs(0)
        assert result[0] == 0
        assert set(result) == {0, 1, 2, 3, 4}

    def test_bfs_disconnected(self):
        """Test BFS on disconnected graph."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(3, 4)

        result = g.bfs(0)
        assert set(result) == {0, 1}

    def test_bfs_invalid_source(self):
        """Test BFS with invalid source."""
        g = AdjacencyMatrix(5)
        assert g.bfs(10) == []

    def test_dfs(self):
        """Test DFS traversal."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(1, 3)
        g.add_edge(2, 4)

        result = g.dfs(0)
        assert result[0] == 0
        assert set(result) == {0, 1, 2, 3, 4}

    def test_dfs_recursive(self):
        """Test recursive DFS traversal."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(1, 3)
        g.add_edge(2, 4)

        result = g.dfs_recursive(0)
        assert result[0] == 0
        assert set(result) == {0, 1, 2, 3, 4}

    def test_dfs_invalid_source(self):
        """Test DFS with invalid source."""
        g = AdjacencyMatrix(5)
        assert g.dfs(10) == []


class TestGraphProperties:
    """Test graph property checks."""

    def test_is_connected_true(self):
        """Test connected graph."""
        g = AdjacencyMatrix(4)
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        assert g.is_connected()

    def test_is_connected_false(self):
        """Test disconnected graph."""
        g = AdjacencyMatrix(4)
        g.add_edge(0, 1)
        g.add_edge(2, 3)

        assert not g.is_connected()

    def test_is_connected_empty(self):
        """Test empty graph is connected."""
        g = AdjacencyMatrix(0)
        assert g.is_connected()


class TestFloydWarshall:
    """Test Floyd-Warshall algorithm."""

    def test_floyd_warshall_basic(self):
        """Test basic Floyd-Warshall."""
        g = AdjacencyMatrix(4, weighted=True)
        g.add_edge(0, 1, 3)
        g.add_edge(0, 2, 6)
        g.add_edge(1, 2, 2)
        g.add_edge(2, 3, 1)

        dist, success = g.floyd_warshall()
        assert success

        # Check distances
        assert dist[0][0] == 0
        assert dist[0][1] == 3
        assert dist[0][2] == 5  # Through vertex 1
        assert dist[0][3] == 6  # Through vertex 1, 2

    def test_floyd_warshall_directed(self):
        """Test Floyd-Warshall on directed graph."""
        g = AdjacencyMatrix(3, directed=True, weighted=True)
        g.add_edge(0, 1, 1)
        g.add_edge(1, 2, 2)

        dist, success = g.floyd_warshall()
        assert success

        assert dist[0][2] == 3
        assert dist[2][0] == float('inf')


class TestTransitiveClosure:
    """Test transitive closure algorithm."""

    def test_transitive_closure_basic(self):
        """Test basic transitive closure."""
        g = AdjacencyMatrix(4, directed=True)
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        closure = g.transitive_closure()

        # Diagonal should be True
        for i in range(4):
            assert closure[i][i]

        # Path from 0 to all
        assert closure[0][1]
        assert closure[0][2]
        assert closure[0][3]

        # No path backwards
        assert not closure[3][0]
        assert not closure[2][0]


class TestUtilityMethods:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing all edges."""
        g = AdjacencyMatrix(5)
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        g.add_edge(2, 3)

        g.clear()

        assert g.edge_count == 0
        assert not g.has_edge(0, 1)

    def test_copy(self):
        """Test copying graph."""
        g = AdjacencyMatrix(5, directed=True, weighted=True)
        g.add_edge(0, 1, 5)
        g.add_edge(1, 2, 3)

        copy = g.copy()

        assert copy.num_vertices == g.num_vertices
        assert copy.is_directed == g.is_directed
        assert copy.is_weighted == g.is_weighted
        assert copy.has_edge(0, 1)
        assert copy.get_weight(0, 1) == 5

        # Modify copy shouldn't affect original
        copy.remove_edge(0, 1)
        assert g.has_edge(0, 1)

    def test_transpose(self):
        """Test transposing directed graph."""
        g = AdjacencyMatrix(3, directed=True)
        g.add_edge(0, 1)
        g.add_edge(0, 2)
        g.add_edge(1, 2)

        trans = g.transpose()

        assert trans.has_edge(1, 0)
        assert trans.has_edge(2, 0)
        assert trans.has_edge(2, 1)
        assert not trans.has_edge(0, 1)

    def test_matrix_property(self):
        """Test getting matrix copy."""
        g = AdjacencyMatrix(3)
        g.add_edge(0, 1)

        matrix = g.matrix

        # Should be a copy
        matrix[0][2] = 1
        assert not g.has_edge(0, 2)

    def test_str(self):
        """Test string representation."""
        g = AdjacencyMatrix(3)
        g.add_edge(0, 1)

        s = str(g)
        assert "Adjacency Matrix" in s
        assert "3 vertices" in s

    def test_repr(self):
        """Test repr."""
        g = AdjacencyMatrix(3, directed=True, weighted=True)
        r = repr(g)

        assert "AdjacencyMatrix" in r
        assert "Directed" in r
        assert "Weighted" in r


class TestEdgeCases:
    """Test edge cases."""

    def test_empty_graph(self):
        """Test operations on empty graph."""
        g = AdjacencyMatrix(0)

        assert len(g) == 0
        assert g.edge_count == 0
        assert g.is_connected()
        assert list(g) == []

    def test_single_vertex(self):
        """Test single vertex graph."""
        g = AdjacencyMatrix(1)

        assert len(g) == 1
        assert 0 in g
        assert g.is_connected()
        assert g.bfs(0) == [0]
        assert g.dfs(0) == [0]

    def test_self_loop(self):
        """Test self loop."""
        g = AdjacencyMatrix(3)
        g.add_edge(0, 0, 1)

        assert g.has_edge(0, 0)
        assert g.edge_count == 1

    def test_complete_graph(self):
        """Test complete graph."""
        n = 5
        g = AdjacencyMatrix(n)

        for i in range(n):
            for j in range(i + 1, n):
                g.add_edge(i, j)

        # Complete graph has n*(n-1)/2 edges
        assert g.edge_count == n * (n - 1) // 2
        assert g.is_connected()
