"""
Tests for Segment Tree and Fenwick Tree implementations.
"""

import pytest
from data_structures.segment_tree import (
    SegmentTree, SegmentTreeLazy, FenwickTree, FenwickTree2D,
    count_inversions, range_sum_query_mutable
)


class TestSegmentTreeBasics:
    """Test basic segment tree operations."""

    def test_init(self):
        """Test initialization."""
        st = SegmentTree([1, 3, 5, 7, 9, 11])
        assert st.query(0, 5) == 36

    def test_init_empty(self):
        """Test initialization with empty array."""
        st = SegmentTree([])
        # Should not raise

    def test_init_single(self):
        """Test initialization with single element."""
        st = SegmentTree([42])
        assert st.query(0, 0) == 42

    def test_query_single_element(self):
        """Test query for single element."""
        st = SegmentTree([1, 3, 5, 7, 9, 11])
        assert st.query(2, 2) == 5
        assert st.query(0, 0) == 1
        assert st.query(5, 5) == 11

    def test_query_range(self):
        """Test query for range."""
        st = SegmentTree([1, 3, 5, 7, 9, 11])
        assert st.query(0, 2) == 9  # 1+3+5
        assert st.query(1, 4) == 24  # 3+5+7+9
        assert st.query(0, 5) == 36  # All

    def test_update(self):
        """Test point update."""
        st = SegmentTree([1, 3, 5, 7, 9, 11])
        st.update(2, 10)

        assert st.query(2, 2) == 10
        assert st.query(0, 5) == 41  # 36 - 5 + 10

    def test_multiple_updates(self):
        """Test multiple updates."""
        st = SegmentTree([1, 2, 3, 4, 5])

        st.update(0, 10)
        st.update(4, 50)

        assert st.query(0, 4) == 10 + 2 + 3 + 4 + 50


class TestSegmentTreeOperations:
    """Test segment tree with different operations."""

    def test_min_query(self):
        """Test segment tree for minimum."""
        st = SegmentTree([5, 2, 8, 1, 9, 3],
                        operation=min,
                        identity=float('inf'))

        assert st.query(0, 5) == 1
        assert st.query(0, 2) == 2
        assert st.query(3, 5) == 1

    def test_max_query(self):
        """Test segment tree for maximum."""
        st = SegmentTree([5, 2, 8, 1, 9, 3],
                        operation=max,
                        identity=float('-inf'))

        assert st.query(0, 5) == 9
        assert st.query(0, 2) == 8
        assert st.query(3, 5) == 9

    def test_gcd_query(self):
        """Test segment tree for GCD."""
        import math
        st = SegmentTree([12, 18, 24, 36],
                        operation=math.gcd,
                        identity=0)

        assert st.query(0, 3) == 6  # GCD of all
        assert st.query(0, 1) == 6  # GCD(12, 18)
        assert st.query(2, 3) == 12  # GCD(24, 36)


class TestSegmentTreeLazy:
    """Test segment tree with lazy propagation."""

    def test_init(self):
        """Test initialization."""
        st = SegmentTreeLazy([1, 3, 5, 7, 9, 11])
        assert st.query(0, 5) == 36

    def test_range_update(self):
        """Test range update."""
        st = SegmentTreeLazy([1, 3, 5, 7, 9, 11])
        st.range_update(1, 4, 10)  # Add 10 to indices 1-4

        # New values: [1, 13, 15, 17, 19, 11]
        assert st.query(1, 4) == 13 + 15 + 17 + 19

    def test_multiple_range_updates(self):
        """Test multiple range updates."""
        st = SegmentTreeLazy([0, 0, 0, 0, 0])

        st.range_update(0, 2, 1)  # [1, 1, 1, 0, 0]
        st.range_update(2, 4, 2)  # [1, 1, 3, 2, 2]

        assert st.query(0, 4) == 9
        assert st.query(0, 1) == 2
        assert st.query(2, 4) == 7

    def test_query_after_range_update(self):
        """Test querying different ranges after update."""
        st = SegmentTreeLazy([1, 2, 3, 4, 5])

        st.range_update(0, 4, 10)  # Add 10 to all

        assert st.query(0, 0) == 11
        assert st.query(2, 2) == 13
        assert st.query(0, 4) == 65  # 15 + 11 + 12 + 13 + 14


class TestFenwickTreeBasics:
    """Test basic Fenwick tree operations."""

    def test_init(self):
        """Test initialization."""
        ft = FenwickTree([3, 2, -1, 6, 5, 4, -3, 3])
        assert ft.prefix_sum(7) == 19  # Sum of all

    def test_init_empty(self):
        """Test initialization with empty array."""
        ft = FenwickTree([])
        assert len(ft) == 0

    def test_prefix_sum(self):
        """Test prefix sum."""
        ft = FenwickTree([1, 2, 3, 4, 5])

        assert ft.prefix_sum(0) == 1
        assert ft.prefix_sum(1) == 3
        assert ft.prefix_sum(2) == 6
        assert ft.prefix_sum(4) == 15

    def test_range_sum(self):
        """Test range sum."""
        ft = FenwickTree([1, 2, 3, 4, 5])

        assert ft.range_sum(0, 4) == 15
        assert ft.range_sum(1, 3) == 9  # 2+3+4
        assert ft.range_sum(2, 2) == 3

    def test_update(self):
        """Test update (add delta)."""
        ft = FenwickTree([1, 2, 3, 4, 5])

        ft.update(2, 10)  # Add 10 to index 2

        assert ft.range_sum(2, 2) == 13
        assert ft.prefix_sum(4) == 25

    def test_set(self):
        """Test set (change value)."""
        ft = FenwickTree([1, 2, 3, 4, 5])

        ft.set(2, 10)  # Set index 2 to 10

        assert ft.range_sum(2, 2) == 10
        assert ft.prefix_sum(4) == 22  # 1+2+10+4+5

    def test_negative_values(self):
        """Test with negative values."""
        ft = FenwickTree([3, -2, 5, -1, 4])

        assert ft.prefix_sum(4) == 9
        assert ft.range_sum(1, 3) == 2  # -2+5+-1


class TestFenwickTree2D:
    """Test 2D Fenwick tree."""

    def test_init(self):
        """Test initialization."""
        matrix = [
            [1, 2, 3],
            [4, 5, 6],
            [7, 8, 9]
        ]
        ft = FenwickTree2D(matrix)
        assert ft.query(0, 0, 2, 2) == 45  # Sum of all

    def test_init_empty(self):
        """Test empty matrix."""
        ft = FenwickTree2D([])
        # Should not raise

    def test_query_submatrix(self):
        """Test query for submatrix."""
        matrix = [
            [1, 2, 3],
            [4, 5, 6],
            [7, 8, 9]
        ]
        ft = FenwickTree2D(matrix)

        assert ft.query(0, 0, 0, 0) == 1
        assert ft.query(1, 1, 1, 1) == 5
        assert ft.query(0, 0, 1, 1) == 12  # 1+2+4+5
        assert ft.query(1, 1, 2, 2) == 28  # 5+6+8+9

    def test_update(self):
        """Test point update."""
        matrix = [
            [1, 2],
            [3, 4]
        ]
        ft = FenwickTree2D(matrix)

        ft.update(0, 0, 9)  # Add 9 to (0,0)

        assert ft.query(0, 0, 0, 0) == 10
        assert ft.query(0, 0, 1, 1) == 19


class TestCountInversions:
    """Test inversion counting."""

    def test_no_inversions(self):
        """Test sorted array (no inversions)."""
        assert count_inversions([1, 2, 3, 4, 5]) == 0

    def test_all_inversions(self):
        """Test reverse sorted array (max inversions)."""
        # [5,4,3,2,1] has 10 inversions
        assert count_inversions([5, 4, 3, 2, 1]) == 10

    def test_some_inversions(self):
        """Test array with some inversions."""
        # [2,4,1,3,5] inversions: (2,1), (4,1), (4,3) = 3
        assert count_inversions([2, 4, 1, 3, 5]) == 3

    def test_empty(self):
        """Test empty array."""
        assert count_inversions([]) == 0

    def test_single(self):
        """Test single element."""
        assert count_inversions([42]) == 0

    def test_duplicates(self):
        """Test with duplicates."""
        # [2, 1, 1] inversions: (2,1), (2,1) = 2
        assert count_inversions([2, 1, 1]) == 2


class TestRangeSumQueryMutable:
    """Test mutable range sum query structure."""

    def test_basic_operations(self):
        """Test basic update and query."""
        update, query = range_sum_query_mutable([1, 3, 5])

        assert query(0, 2) == 9

        update(1, 2)
        assert query(0, 2) == 8

    def test_leetcode_example(self):
        """Test LeetCode example."""
        update, query = range_sum_query_mutable([1, 3, 5])

        assert query(0, 2) == 9
        update(1, 2)
        assert query(0, 2) == 8


class TestSegmentTreeEdgeCases:
    """Test edge cases."""

    def test_single_element_operations(self):
        """Test all operations on single element."""
        st = SegmentTree([42])

        assert st.query(0, 0) == 42
        st.update(0, 100)
        assert st.query(0, 0) == 100

    def test_two_elements(self):
        """Test with two elements."""
        st = SegmentTree([10, 20])

        assert st.query(0, 0) == 10
        assert st.query(1, 1) == 20
        assert st.query(0, 1) == 30

    def test_large_array(self):
        """Test with large array."""
        n = 10000
        arr = list(range(n))
        st = SegmentTree(arr)

        # Sum of 0 to n-1 = n*(n-1)/2
        assert st.query(0, n - 1) == n * (n - 1) // 2

    def test_fenwick_large_array(self):
        """Test Fenwick tree with large array."""
        n = 10000
        arr = list(range(1, n + 1))
        ft = FenwickTree(arr)

        # Sum of 1 to n = n*(n+1)/2
        assert ft.prefix_sum(n - 1) == n * (n + 1) // 2

    def test_alternating_updates_queries(self):
        """Test alternating updates and queries."""
        st = SegmentTree([1, 2, 3, 4, 5])

        assert st.query(0, 4) == 15
        st.update(2, 10)
        assert st.query(0, 4) == 22
        st.update(0, 5)
        assert st.query(0, 4) == 26
        assert st.query(1, 3) == 16  # 2 + 10 + 4
