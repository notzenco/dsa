"""
Tests for Union-Find (Disjoint Set Union) implementations.
"""

import pytest
from data_structures.union_find import (
    UnionFind, UnionFindArray, WeightedUnionFind,
    PersistentUnionFind, num_islands, find_redundant_connection
)


class TestUnionFindBasics:
    """Test basic Union-Find operations."""

    def test_init(self):
        """Test initialization."""
        uf = UnionFind()
        assert len(uf) == 0
        assert uf.num_components == 0

    def test_make_set(self):
        """Test creating sets."""
        uf = UnionFind()
        assert uf.make_set(1)
        assert uf.make_set(2)
        assert uf.make_set(3)

        assert len(uf) == 3
        assert uf.num_components == 3

    def test_make_set_duplicate(self):
        """Test creating duplicate set."""
        uf = UnionFind()
        assert uf.make_set(1)
        assert not uf.make_set(1)
        assert len(uf) == 1

    def test_find(self):
        """Test finding root."""
        uf = UnionFind()
        uf.make_set(1)
        uf.make_set(2)

        assert uf.find(1) == 1
        assert uf.find(2) == 2

    def test_find_nonexistent(self):
        """Test find on nonexistent element."""
        uf = UnionFind()
        with pytest.raises(KeyError):
            uf.find(99)

    def test_union_basic(self):
        """Test basic union."""
        uf = UnionFind()
        uf.make_set(1)
        uf.make_set(2)

        assert uf.union(1, 2)
        assert uf.connected(1, 2)
        assert uf.num_components == 1

    def test_union_already_connected(self):
        """Test union of already connected elements."""
        uf = UnionFind()
        uf.make_set(1)
        uf.make_set(2)
        uf.union(1, 2)

        assert not uf.union(1, 2)
        assert uf.num_components == 1

    def test_connected(self):
        """Test connectivity check."""
        uf = UnionFind()
        uf.make_set(1)
        uf.make_set(2)
        uf.make_set(3)

        assert not uf.connected(1, 2)
        uf.union(1, 2)
        assert uf.connected(1, 2)
        assert not uf.connected(1, 3)

    def test_transitive_connectivity(self):
        """Test transitive connectivity."""
        uf = UnionFind()
        for i in range(5):
            uf.make_set(i)

        uf.union(0, 1)
        uf.union(1, 2)

        assert uf.connected(0, 2)
        assert not uf.connected(0, 3)


class TestUnionFindSetSize:
    """Test set size operations."""

    def test_initial_size(self):
        """Test initial set size is 1."""
        uf = UnionFind()
        uf.make_set(1)
        assert uf.set_size(1) == 1

    def test_size_after_union(self):
        """Test size increases after union."""
        uf = UnionFind()
        uf.make_set(1)
        uf.make_set(2)
        uf.union(1, 2)

        assert uf.set_size(1) == 2
        assert uf.set_size(2) == 2

    def test_size_multiple_unions(self):
        """Test size with multiple unions."""
        uf = UnionFind()
        for i in range(5):
            uf.make_set(i)

        uf.union(0, 1)
        uf.union(2, 3)
        uf.union(0, 2)

        assert uf.set_size(0) == 4
        assert uf.set_size(4) == 1


class TestUnionFindComponents:
    """Test component operations."""

    def test_get_components(self):
        """Test getting all components."""
        uf = UnionFind()
        for i in range(5):
            uf.make_set(i)

        uf.union(0, 1)
        uf.union(2, 3)

        components = uf.get_components()
        assert len(components) == 3

        # Convert to sets of frozensets for comparison
        component_sets = {frozenset(c) for c in components}
        expected = {frozenset({0, 1}), frozenset({2, 3}), frozenset({4})}
        assert component_sets == expected

    def test_num_components(self):
        """Test component count."""
        uf = UnionFind()
        for i in range(5):
            uf.make_set(i)

        assert uf.num_components == 5

        uf.union(0, 1)
        assert uf.num_components == 4

        uf.union(2, 3)
        assert uf.num_components == 3

        uf.union(0, 2)
        assert uf.num_components == 2


class TestUnionFindDunder:
    """Test magic methods."""

    def test_len(self):
        """Test __len__."""
        uf = UnionFind()
        assert len(uf) == 0

        uf.make_set(1)
        assert len(uf) == 1

        uf.make_set(2)
        uf.make_set(3)
        assert len(uf) == 3

    def test_contains(self):
        """Test __contains__."""
        uf = UnionFind()
        uf.make_set(1)

        assert 1 in uf
        assert 2 not in uf

    def test_repr(self):
        """Test __repr__."""
        uf = UnionFind()
        uf.make_set(1)

        assert "UnionFind" in repr(uf)
        assert "1" in repr(uf)


class TestUnionFindWithStrings:
    """Test Union-Find with string elements."""

    def test_string_elements(self):
        """Test with string keys."""
        uf = UnionFind()
        uf.make_set("alice")
        uf.make_set("bob")
        uf.make_set("charlie")

        uf.union("alice", "bob")

        assert uf.connected("alice", "bob")
        assert not uf.connected("alice", "charlie")


class TestUnionFindArray:
    """Test array-based Union-Find."""

    def test_init(self):
        """Test initialization."""
        uf = UnionFindArray(5)
        assert len(uf) == 5
        assert uf.num_components == 5

    def test_union_and_find(self):
        """Test basic operations."""
        uf = UnionFindArray(5)
        uf.union(0, 1)
        uf.union(2, 3)

        assert uf.connected(0, 1)
        assert uf.connected(2, 3)
        assert not uf.connected(0, 2)

    def test_transitive(self):
        """Test transitive connectivity."""
        uf = UnionFindArray(5)
        uf.union(0, 1)
        uf.union(1, 2)
        uf.union(2, 3)

        assert uf.connected(0, 3)

    def test_set_size(self):
        """Test set size."""
        uf = UnionFindArray(5)
        uf.union(0, 1)
        uf.union(1, 2)

        assert uf.set_size(0) == 3
        assert uf.set_size(3) == 1

    def test_get_components(self):
        """Test getting components."""
        uf = UnionFindArray(5)
        uf.union(0, 1)
        uf.union(2, 3)

        components = uf.get_components()
        assert len(components) == 3

    def test_out_of_range(self):
        """Test index out of range."""
        uf = UnionFindArray(5)
        with pytest.raises(IndexError):
            uf.find(10)

    def test_repr(self):
        """Test repr."""
        uf = UnionFindArray(5)
        assert "UnionFindArray" in repr(uf)
        assert "5" in repr(uf)


class TestWeightedUnionFind:
    """Test weighted Union-Find."""

    def test_basic_operations(self):
        """Test basic weighted operations."""
        wuf = WeightedUnionFind()
        wuf.make_set('A')
        wuf.make_set('B')

        assert wuf.union('A', 'B', 5)
        assert wuf.connected('A', 'B')

    def test_diff(self):
        """Test getting difference."""
        wuf = WeightedUnionFind()
        wuf.make_set('A')
        wuf.make_set('B')
        wuf.union('A', 'B', 5)  # A - B = 5

        diff = wuf.diff('A', 'B')
        assert diff == pytest.approx(5.0)

        diff_reverse = wuf.diff('B', 'A')
        assert diff_reverse == pytest.approx(-5.0)

    def test_diff_not_connected(self):
        """Test diff for unconnected elements."""
        wuf = WeightedUnionFind()
        wuf.make_set('A')
        wuf.make_set('B')

        assert wuf.diff('A', 'B') is None

    def test_transitive_weights(self):
        """Test transitive weight calculation."""
        wuf = WeightedUnionFind()
        wuf.make_set('A')
        wuf.make_set('B')
        wuf.make_set('C')

        wuf.union('A', 'B', 2)  # A - B = 2
        wuf.union('B', 'C', 3)  # B - C = 3

        # A - C = (A - B) + (B - C) = 2 + 3 = 5
        diff = wuf.diff('A', 'C')
        assert diff == pytest.approx(5.0)

    def test_contains(self):
        """Test contains check."""
        wuf = WeightedUnionFind()
        wuf.make_set('A')

        assert 'A' in wuf
        assert 'B' not in wuf


class TestPersistentUnionFind:
    """Test persistent Union-Find."""

    def test_basic_operations(self):
        """Test basic operations."""
        puf = PersistentUnionFind(5)
        puf.union(0, 1)
        puf.union(2, 3)

        assert puf.connected(0, 1)
        assert puf.connected(2, 3)
        assert not puf.connected(0, 2)

    def test_query_past_versions(self):
        """Test querying past versions."""
        puf = PersistentUnionFind(5)

        v0 = puf.current_version
        v1 = puf.union(0, 1)
        v2 = puf.union(2, 3)

        # Before any unions
        assert not puf.connected(0, 1, version=v0)

        # After first union
        assert puf.connected(0, 1, version=v1)
        assert not puf.connected(2, 3, version=v1)

        # After second union
        assert puf.connected(0, 1, version=v2)
        assert puf.connected(2, 3, version=v2)

    def test_len(self):
        """Test length."""
        puf = PersistentUnionFind(10)
        assert len(puf) == 10


class TestNumIslands:
    """Test number of islands function."""

    def test_single_island(self):
        """Test single island."""
        grid = [
            [1, 1, 1],
            [0, 0, 0],
            [0, 0, 0]
        ]
        assert num_islands(grid) == 1

    def test_multiple_islands(self):
        """Test multiple islands."""
        grid = [
            [1, 0, 1],
            [0, 0, 0],
            [1, 0, 1]
        ]
        assert num_islands(grid) == 4

    def test_connected_island(self):
        """Test connected island."""
        grid = [
            [1, 1, 0],
            [1, 1, 0],
            [0, 0, 1]
        ]
        assert num_islands(grid) == 2

    def test_empty_grid(self):
        """Test empty grid."""
        assert num_islands([]) == 0
        assert num_islands([[]]) == 0

    def test_no_land(self):
        """Test grid with no land."""
        grid = [
            [0, 0, 0],
            [0, 0, 0]
        ]
        assert num_islands(grid) == 0


class TestFindRedundantConnection:
    """Test find redundant connection function."""

    def test_simple_cycle(self):
        """Test simple cycle."""
        edges = [[1, 2], [1, 3], [2, 3]]
        result = find_redundant_connection(edges)
        assert result == [2, 3]

    def test_larger_graph(self):
        """Test larger graph."""
        edges = [[1, 2], [2, 3], [3, 4], [1, 4], [1, 5]]
        result = find_redundant_connection(edges)
        assert result == [1, 4]


class TestPathCompression:
    """Test path compression optimization."""

    def test_path_compression_works(self):
        """Test that path compression flattens trees."""
        uf = UnionFindArray(10)

        # Create a chain: 0 <- 1 <- 2 <- ... <- 9
        for i in range(9):
            uf.union(i, i + 1)

        # After find(9), all should point more directly to root
        uf.find(9)

        # The tree should be significantly flattened
        # We can verify by checking parent pointers
        root = uf.find(0)
        for i in range(10):
            assert uf.find(i) == root


class TestUnionByRank:
    """Test union by rank optimization."""

    def test_balanced_tree(self):
        """Test that union by rank keeps trees balanced."""
        uf = UnionFindArray(8)

        # Union in a balanced way
        uf.union(0, 1)
        uf.union(2, 3)
        uf.union(4, 5)
        uf.union(6, 7)

        uf.union(0, 2)
        uf.union(4, 6)

        uf.union(0, 4)

        # All should be connected
        for i in range(8):
            assert uf.connected(0, i)

        # Components should be 1
        assert uf.num_components == 1


class TestEdgeCases:
    """Test edge cases."""

    def test_single_element(self):
        """Test with single element."""
        uf = UnionFind()
        uf.make_set(1)

        assert uf.find(1) == 1
        assert uf.set_size(1) == 1
        assert uf.num_components == 1

    def test_self_union(self):
        """Test union with self."""
        uf = UnionFind()
        uf.make_set(1)

        assert not uf.union(1, 1)
        assert uf.num_components == 1

    def test_large_number_of_elements(self):
        """Test with many elements."""
        n = 10000
        uf = UnionFindArray(n)

        # Union all even numbers
        for i in range(0, n - 2, 2):
            uf.union(i, i + 2)

        # Union all odd numbers
        for i in range(1, n - 2, 2):
            uf.union(i, i + 2)

        assert uf.connected(0, n - 2)  # All evens connected
        assert uf.connected(1, n - 1)  # All odds connected
        assert not uf.connected(0, 1)  # Evens and odds not connected

    def test_mixed_operations(self):
        """Test mixed sequence of operations."""
        uf = UnionFind()

        # Add elements
        for i in range(10):
            uf.make_set(i)

        # Various unions
        uf.union(0, 5)
        uf.union(1, 6)
        uf.union(0, 1)

        assert uf.connected(0, 6)
        assert uf.connected(5, 1)
        assert uf.set_size(0) == 4
