"""
Tests for Fenwick Tree implementation.
"""

import pytest
from data_structures.fenwick_tree import (
    FenwickTree, FenwickTree2D, count_inversions, range_sum_query_mutable
)


class TestFenwickTreeBasics:
    """Test basic Fenwick tree operations."""

    def test_init(self):
        """Test initialization."""
        ft = FenwickTree([3, 2, -1, 6, 5, 4, -3, 3])
        assert len(ft) == 8

    def test_init_empty(self):
        """Test empty initialization."""
        ft = FenwickTree([])
        assert len(ft) == 0

    def test_prefix_sum(self):
        """Test prefix sum queries."""
        ft = FenwickTree([3, 2, -1, 6, 5, 4, -3, 3])
        assert ft.prefix_sum(0) == 3
        assert ft.prefix_sum(1) == 5
        assert ft.prefix_sum(2) == 4
        assert ft.prefix_sum(3) == 10
        assert ft.prefix_sum(7) == 19

    def test_range_sum(self):
        """Test range sum queries."""
        ft = FenwickTree([3, 2, -1, 6, 5, 4, -3, 3])
        assert ft.range_sum(0, 3) == 10
        assert ft.range_sum(2, 5) == 14
        assert ft.range_sum(0, 7) == 19
        assert ft.range_sum(4, 4) == 5

    def test_update(self):
        """Test update operation."""
        ft = FenwickTree([1, 2, 3, 4, 5])
        ft.update(2, 10)  # Add 10 to index 2
        assert ft.range_sum(2, 2) == 13
        assert ft.prefix_sum(4) == 25

    def test_set(self):
        """Test set operation."""
        ft = FenwickTree([1, 2, 3, 4, 5])
        ft.set(2, 10)  # Set index 2 to 10
        assert ft.range_sum(2, 2) == 10
        assert ft.prefix_sum(4) == 22

    def test_negative_values(self):
        """Test with negative values."""
        ft = FenwickTree([-1, -2, -3, -4, -5])
        assert ft.prefix_sum(4) == -15
        assert ft.range_sum(1, 3) == -9


class TestFenwickTree2D:
    """Test 2D Fenwick tree operations."""

    def test_init(self):
        """Test 2D initialization."""
        matrix = [
            [1, 2, 3],
            [4, 5, 6],
            [7, 8, 9]
        ]
        ft = FenwickTree2D(matrix)
        assert ft.query(0, 0, 2, 2) == 45

    def test_init_empty(self):
        """Test empty 2D initialization."""
        ft = FenwickTree2D([])
        assert ft.query(0, 0, 0, 0) == 0

    def test_query_submatrix(self):
        """Test submatrix query."""
        matrix = [
            [1, 2, 3],
            [4, 5, 6],
            [7, 8, 9]
        ]
        ft = FenwickTree2D(matrix)

        assert ft.query(0, 0, 0, 0) == 1
        assert ft.query(0, 0, 1, 1) == 12
        assert ft.query(1, 1, 2, 2) == 28

    def test_update(self):
        """Test 2D update."""
        matrix = [
            [1, 2],
            [3, 4]
        ]
        ft = FenwickTree2D(matrix)
        ft.update(0, 0, 10)

        assert ft.query(0, 0, 1, 1) == 20


class TestCountInversions:
    """Test inversion counting."""

    def test_no_inversions(self):
        """Test sorted array (no inversions)."""
        assert count_inversions([1, 2, 3, 4, 5]) == 0

    def test_all_inversions(self):
        """Test reverse sorted array (maximum inversions)."""
        assert count_inversions([5, 4, 3, 2, 1]) == 10

    def test_some_inversions(self):
        """Test array with some inversions."""
        assert count_inversions([2, 4, 1, 3, 5]) == 3

    def test_empty(self):
        """Test empty array."""
        assert count_inversions([]) == 0

    def test_single(self):
        """Test single element."""
        assert count_inversions([1]) == 0

    def test_duplicates(self):
        """Test with duplicates."""
        assert count_inversions([2, 1, 1, 3]) == 2


class TestRangeSumQueryMutable:
    """Test mutable range sum query."""

    def test_basic_operations(self):
        """Test basic update and query."""
        update, query = range_sum_query_mutable([1, 2, 3, 4, 5])

        assert query(0, 4) == 15
        update(2, 10)
        assert query(0, 4) == 22

    def test_leetcode_example(self):
        """Test LeetCode #307 example."""
        update, query = range_sum_query_mutable([1, 3, 5])

        assert query(0, 2) == 9
        update(1, 2)
        assert query(0, 2) == 8


class TestFenwickTreeEdgeCases:
    """Test edge cases."""

    def test_single_element(self):
        """Test single element operations."""
        ft = FenwickTree([42])
        assert ft.prefix_sum(0) == 42
        assert ft.range_sum(0, 0) == 42

        ft.update(0, 8)
        assert ft.prefix_sum(0) == 50

    def test_large_values(self):
        """Test with large values."""
        ft = FenwickTree([10**9, 10**9, 10**9])
        assert ft.prefix_sum(2) == 3 * 10**9

    def test_many_updates(self):
        """Test many updates."""
        ft = FenwickTree([0] * 100)

        for i in range(100):
            ft.update(i, i + 1)

        assert ft.prefix_sum(99) == 5050

    def test_alternating_operations(self):
        """Test alternating updates and queries."""
        ft = FenwickTree([1, 2, 3, 4, 5])

        for i in range(5):
            ft.update(i, 1)
            assert ft.range_sum(0, i) == sum(range(1, i + 2)) + (i + 1)
