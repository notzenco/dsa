"""
Tests for Sorting Algorithms.
"""

import pytest
import random
from algorithms.sorting import (
    bubble_sort, selection_sort, insertion_sort,
    merge_sort, quick_sort, heap_sort, shell_sort,
    counting_sort, radix_sort, bucket_sort,
    quick_select, sort_with_comparator, is_sorted
)


class TestComparisonSorts:
    """Test comparison-based sorting algorithms."""

    # Test data
    EMPTY = []
    SINGLE = [42]
    SORTED = [1, 2, 3, 4, 5]
    REVERSE = [5, 4, 3, 2, 1]
    RANDOM = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5]
    DUPLICATES = [5, 5, 5, 1, 1, 1]
    NEGATIVE = [-3, 1, -4, 1, 5, -9, 2, 6]

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_empty(self, sort_func):
        """Test sorting empty list."""
        assert sort_func(self.EMPTY) == []

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_single(self, sort_func):
        """Test sorting single element."""
        assert sort_func(self.SINGLE) == [42]

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_sorted(self, sort_func):
        """Test sorting already sorted list."""
        assert sort_func(self.SORTED) == [1, 2, 3, 4, 5]

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_reverse(self, sort_func):
        """Test sorting reverse sorted list."""
        assert sort_func(self.REVERSE) == [1, 2, 3, 4, 5]

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_random(self, sort_func):
        """Test sorting random list."""
        result = sort_func(self.RANDOM)
        assert result == sorted(self.RANDOM)

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_duplicates(self, sort_func):
        """Test sorting with duplicates."""
        result = sort_func(self.DUPLICATES)
        assert result == sorted(self.DUPLICATES)

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_negative(self, sort_func):
        """Test sorting with negative numbers."""
        result = sort_func(self.NEGATIVE)
        assert result == sorted(self.NEGATIVE)

    @pytest.mark.parametrize("sort_func", [
        bubble_sort, selection_sort, insertion_sort,
        merge_sort, quick_sort, heap_sort, shell_sort
    ])
    def test_does_not_modify_original(self, sort_func):
        """Test that original list is not modified."""
        original = [3, 1, 2]
        copy = original.copy()
        sort_func(original)
        assert original == copy


class TestStableSorts:
    """Test stability of stable sorting algorithms."""

    def test_merge_sort_stability(self):
        """Test merge sort is stable."""
        # Sort by first element, check second element order preserved
        data = [(1, 'a'), (2, 'b'), (1, 'c'), (2, 'd')]
        result = merge_sort(data)
        assert result == [(1, 'a'), (1, 'c'), (2, 'b'), (2, 'd')]

    def test_bubble_sort_stability(self):
        """Test bubble sort is stable."""
        data = [(1, 'a'), (2, 'b'), (1, 'c'), (2, 'd')]
        result = bubble_sort(data)
        assert result == [(1, 'a'), (1, 'c'), (2, 'b'), (2, 'd')]

    def test_insertion_sort_stability(self):
        """Test insertion sort is stable."""
        data = [(1, 'a'), (2, 'b'), (1, 'c'), (2, 'd')]
        result = insertion_sort(data)
        assert result == [(1, 'a'), (1, 'c'), (2, 'b'), (2, 'd')]


class TestNonComparisonSorts:
    """Test non-comparison sorting algorithms."""

    def test_counting_sort_basic(self):
        """Test counting sort basic case."""
        assert counting_sort([4, 2, 2, 8, 3, 3, 1]) == [1, 2, 2, 3, 3, 4, 8]

    def test_counting_sort_empty(self):
        """Test counting sort empty list."""
        assert counting_sort([]) == []

    def test_counting_sort_single(self):
        """Test counting sort single element."""
        assert counting_sort([42]) == [42]

    def test_counting_sort_negative(self):
        """Test counting sort with negative numbers."""
        assert counting_sort([-3, -1, -4, -1, 5, 9, 2, 6]) == sorted([-3, -1, -4, -1, 5, 9, 2, 6])

    def test_counting_sort_all_same(self):
        """Test counting sort all same values."""
        assert counting_sort([5, 5, 5, 5]) == [5, 5, 5, 5]

    def test_radix_sort_basic(self):
        """Test radix sort basic case."""
        assert radix_sort([170, 45, 75, 90, 802, 24, 2, 66]) == sorted([170, 45, 75, 90, 802, 24, 2, 66])

    def test_radix_sort_empty(self):
        """Test radix sort empty list."""
        assert radix_sort([]) == []

    def test_radix_sort_single(self):
        """Test radix sort single element."""
        assert radix_sort([42]) == [42]

    def test_radix_sort_negative(self):
        """Test radix sort with negative numbers."""
        arr = [-3, -1, 4, 1, -5, 9, -2, 6]
        assert radix_sort(arr) == sorted(arr)

    def test_bucket_sort_basic(self):
        """Test bucket sort basic case."""
        arr = [0.42, 0.32, 0.23, 0.52, 0.25, 0.47, 0.51]
        result = bucket_sort(arr)
        assert result == sorted(arr)

    def test_bucket_sort_empty(self):
        """Test bucket sort empty list."""
        assert bucket_sort([]) == []

    def test_bucket_sort_all_same(self):
        """Test bucket sort all same values."""
        assert bucket_sort([0.5, 0.5, 0.5]) == [0.5, 0.5, 0.5]


class TestQuickSelect:
    """Test quick select algorithm."""

    def test_kth_smallest(self):
        """Test finding kth smallest."""
        arr = [3, 1, 4, 1, 5, 9, 2, 6]

        assert quick_select(arr, 1) == 1  # 1st smallest
        assert quick_select(arr, 2) == 1  # 2nd smallest (duplicate)
        assert quick_select(arr, 8) == 9  # 8th smallest (largest)

    def test_median(self):
        """Test finding median."""
        arr = [3, 1, 4, 1, 5]
        # Sorted: [1, 1, 3, 4, 5], median is 3 (3rd element)
        assert quick_select(arr, 3) == 3

    def test_single_element(self):
        """Test single element."""
        assert quick_select([42], 1) == 42

    def test_k_out_of_range(self):
        """Test k out of range."""
        with pytest.raises(ValueError):
            quick_select([1, 2, 3], 0)
        with pytest.raises(ValueError):
            quick_select([1, 2, 3], 4)


class TestSortWithComparator:
    """Test sorting with custom comparator."""

    def test_reverse_order(self):
        """Test sorting in reverse order."""
        arr = [1, 3, 2, 5, 4]
        # Comparator for descending order
        result = sort_with_comparator(arr, lambda a, b: b - a)
        assert result == [5, 4, 3, 2, 1]

    def test_by_absolute_value(self):
        """Test sorting by absolute value."""
        arr = [-5, 2, -1, 4, -3]
        result = sort_with_comparator(arr, lambda a, b: abs(a) - abs(b))
        assert [abs(x) for x in result] == [1, 2, 3, 4, 5]

    def test_by_second_element(self):
        """Test sorting tuples by second element."""
        arr = [(1, 5), (2, 3), (3, 4), (4, 1), (5, 2)]
        result = sort_with_comparator(arr, lambda a, b: a[1] - b[1])
        assert result == [(4, 1), (5, 2), (2, 3), (3, 4), (1, 5)]


class TestIsSorted:
    """Test is_sorted utility."""

    def test_sorted_ascending(self):
        """Test ascending sorted array."""
        assert is_sorted([1, 2, 3, 4, 5])
        assert is_sorted([1, 1, 2, 2, 3])

    def test_sorted_descending(self):
        """Test descending sorted array."""
        assert is_sorted([5, 4, 3, 2, 1], reverse=True)
        assert is_sorted([3, 3, 2, 2, 1], reverse=True)

    def test_unsorted(self):
        """Test unsorted array."""
        assert not is_sorted([1, 3, 2])
        assert not is_sorted([5, 4, 3, 2, 1])

    def test_empty_and_single(self):
        """Test empty and single element."""
        assert is_sorted([])
        assert is_sorted([42])
        assert is_sorted([], reverse=True)


class TestLargeInputs:
    """Test with larger inputs."""

    def test_large_random(self):
        """Test sorting large random array."""
        arr = [random.randint(-1000, 1000) for _ in range(1000)]

        # Test all algorithms produce correct result
        expected = sorted(arr)

        assert merge_sort(arr) == expected
        assert quick_sort(arr) == expected
        assert heap_sort(arr) == expected

    def test_large_nearly_sorted(self):
        """Test sorting large nearly sorted array."""
        arr = list(range(1000))
        # Swap a few elements
        for _ in range(10):
            i, j = random.randint(0, 999), random.randint(0, 999)
            arr[i], arr[j] = arr[j], arr[i]

        expected = sorted(arr)
        assert insertion_sort(arr) == expected
