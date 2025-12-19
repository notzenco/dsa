"""
Tests for Searching Algorithms.
"""

import pytest
from algorithms.searching import (
    # Basic search
    linear_search, binary_search, binary_search_recursive,
    # Binary search variants
    lower_bound, upper_bound, find_first_occurrence, find_last_occurrence,
    count_occurrences, find_range, search_insert_position,
    # Specialized search
    jump_search, interpolation_search, exponential_search, ternary_search,
    # Special problems
    search_rotated_array, find_rotation_point, find_peak_element,
    sqrt_int, first_bad_version,
    # Two pointers
    two_sum_sorted, three_sum, container_with_most_water, remove_duplicates_sorted,
    # Sliding window
    max_sum_subarray, longest_substring_without_repeating,
    min_window_substring, longest_substring_k_distinct, find_all_anagrams,
    # Binary search on answer
    koko_eating_bananas, ship_within_days
)


class TestBasicSearch:
    """Test basic search algorithms."""

    def test_linear_search_found(self):
        """Test linear search when element exists."""
        assert linear_search([1, 3, 5, 7, 9], 5) == 2
        assert linear_search([1, 3, 5, 7, 9], 1) == 0
        assert linear_search([1, 3, 5, 7, 9], 9) == 4

    def test_linear_search_not_found(self):
        """Test linear search when element doesn't exist."""
        assert linear_search([1, 3, 5, 7, 9], 4) == -1
        assert linear_search([], 5) == -1

    def test_binary_search_found(self):
        """Test binary search when element exists."""
        arr = [1, 3, 5, 7, 9, 11, 13]
        assert binary_search(arr, 7) == 3
        assert binary_search(arr, 1) == 0
        assert binary_search(arr, 13) == 6

    def test_binary_search_not_found(self):
        """Test binary search when element doesn't exist."""
        arr = [1, 3, 5, 7, 9]
        assert binary_search(arr, 4) == -1
        assert binary_search(arr, 0) == -1
        assert binary_search(arr, 10) == -1
        assert binary_search([], 5) == -1

    def test_binary_search_recursive(self):
        """Test recursive binary search."""
        arr = [1, 3, 5, 7, 9]
        assert binary_search_recursive(arr, 5) == 2
        assert binary_search_recursive(arr, 10) == -1


class TestBinarySearchVariants:
    """Test binary search variants."""

    def test_lower_bound(self):
        """Test lower_bound (first >= target)."""
        arr = [1, 2, 4, 4, 4, 6, 8]
        assert lower_bound(arr, 4) == 2  # First 4
        assert lower_bound(arr, 3) == 2  # Insert position for 3
        assert lower_bound(arr, 0) == 0  # Before all
        assert lower_bound(arr, 9) == 7  # After all

    def test_upper_bound(self):
        """Test upper_bound (first > target)."""
        arr = [1, 2, 4, 4, 4, 6, 8]
        assert upper_bound(arr, 4) == 5  # After last 4
        assert upper_bound(arr, 3) == 2  # Same as lower_bound for missing
        assert upper_bound(arr, 8) == 7  # After all

    def test_find_first_occurrence(self):
        """Test finding first occurrence."""
        arr = [1, 2, 4, 4, 4, 6, 8]
        assert find_first_occurrence(arr, 4) == 2
        assert find_first_occurrence(arr, 1) == 0
        assert find_first_occurrence(arr, 3) == -1

    def test_find_last_occurrence(self):
        """Test finding last occurrence."""
        arr = [1, 2, 4, 4, 4, 6, 8]
        assert find_last_occurrence(arr, 4) == 4
        assert find_last_occurrence(arr, 8) == 6
        assert find_last_occurrence(arr, 3) == -1

    def test_count_occurrences(self):
        """Test counting occurrences."""
        arr = [1, 2, 4, 4, 4, 6, 8]
        assert count_occurrences(arr, 4) == 3
        assert count_occurrences(arr, 1) == 1
        assert count_occurrences(arr, 3) == 0

    def test_find_range(self):
        """Test finding range (first and last position)."""
        arr = [5, 7, 7, 8, 8, 10]
        assert find_range(arr, 8) == (3, 4)
        assert find_range(arr, 6) == (-1, -1)
        assert find_range(arr, 5) == (0, 0)

    def test_search_insert_position(self):
        """Test search insert position."""
        arr = [1, 3, 5, 6]
        assert search_insert_position(arr, 5) == 2
        assert search_insert_position(arr, 2) == 1
        assert search_insert_position(arr, 7) == 4
        assert search_insert_position(arr, 0) == 0


class TestSpecializedSearch:
    """Test specialized search algorithms."""

    def test_jump_search(self):
        """Test jump search."""
        arr = list(range(0, 100, 5))  # [0, 5, 10, ..., 95]
        assert jump_search(arr, 25) == 5
        assert jump_search(arr, 0) == 0
        assert jump_search(arr, 95) == 19
        assert jump_search(arr, 7) == -1

    def test_interpolation_search(self):
        """Test interpolation search."""
        arr = list(range(10, 101, 10))  # [10, 20, ..., 100]
        assert interpolation_search(arr, 50) == 4
        assert interpolation_search(arr, 10) == 0
        assert interpolation_search(arr, 100) == 9
        assert interpolation_search(arr, 55) == -1

    def test_exponential_search(self):
        """Test exponential search."""
        arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        assert exponential_search(arr, 3) == 2
        assert exponential_search(arr, 1) == 0
        assert exponential_search(arr, 10) == 9
        assert exponential_search(arr, 11) == -1

    def test_ternary_search(self):
        """Test ternary search."""
        arr = [1, 3, 5, 7, 9, 11, 13, 15]
        assert ternary_search(arr, 7) == 3
        assert ternary_search(arr, 1) == 0
        assert ternary_search(arr, 15) == 7
        assert ternary_search(arr, 6) == -1


class TestSpecialProblems:
    """Test special binary search problems."""

    def test_search_rotated_array(self):
        """Test search in rotated sorted array."""
        arr = [4, 5, 6, 7, 0, 1, 2]
        assert search_rotated_array(arr, 0) == 4
        assert search_rotated_array(arr, 4) == 0
        assert search_rotated_array(arr, 3) == -1

    def test_find_rotation_point(self):
        """Test finding rotation point (minimum)."""
        assert find_rotation_point([3, 4, 5, 1, 2]) == 3
        assert find_rotation_point([4, 5, 6, 7, 0, 1, 2]) == 4
        assert find_rotation_point([1, 2, 3, 4, 5]) == 0  # Not rotated

    def test_find_peak_element(self):
        """Test finding peak element."""
        arr = [1, 2, 3, 1]
        peak = find_peak_element(arr)
        assert peak == 2 or (peak > 0 and arr[peak] > arr[peak - 1])

        arr = [1, 2, 1, 3, 5, 6, 4]
        peak = find_peak_element(arr)
        assert arr[peak] >= arr[max(0, peak - 1)]
        assert arr[peak] >= arr[min(len(arr) - 1, peak + 1)]

    def test_sqrt_int(self):
        """Test integer square root."""
        assert sqrt_int(4) == 2
        assert sqrt_int(8) == 2  # Floor
        assert sqrt_int(9) == 3
        assert sqrt_int(0) == 0
        assert sqrt_int(1) == 1
        assert sqrt_int(100) == 10

    def test_first_bad_version(self):
        """Test first bad version."""
        def is_bad_4(n):
            return n >= 4

        assert first_bad_version(5, is_bad_4) == 4
        assert first_bad_version(10, is_bad_4) == 4

        def is_bad_1(n):
            return n >= 1

        assert first_bad_version(5, is_bad_1) == 1


class TestTwoPointers:
    """Test two pointers technique."""

    def test_two_sum_sorted(self):
        """Test two sum in sorted array."""
        arr = [2, 7, 11, 15]
        result = two_sum_sorted(arr, 9)
        assert result == (0, 1)

        result = two_sum_sorted(arr, 18)
        assert result == (1, 2)

        assert two_sum_sorted(arr, 100) is None

    def test_three_sum(self):
        """Test three sum."""
        result = three_sum([-1, 0, 1, 2, -1, -4])
        assert [-1, -1, 2] in result
        assert [-1, 0, 1] in result
        assert len(result) == 2

        assert three_sum([0, 0, 0]) == [[0, 0, 0]]
        assert three_sum([0, 1, 1]) == []

    def test_container_with_most_water(self):
        """Test container with most water."""
        assert container_with_most_water([1, 8, 6, 2, 5, 4, 8, 3, 7]) == 49
        assert container_with_most_water([1, 1]) == 1
        assert container_with_most_water([4, 3, 2, 1, 4]) == 16

    def test_remove_duplicates_sorted(self):
        """Test removing duplicates from sorted array."""
        arr = [1, 1, 2]
        k = remove_duplicates_sorted(arr)
        assert k == 2
        assert arr[:k] == [1, 2]

        arr = [0, 0, 1, 1, 1, 2, 2, 3, 3, 4]
        k = remove_duplicates_sorted(arr)
        assert k == 5
        assert arr[:k] == [0, 1, 2, 3, 4]


class TestSlidingWindow:
    """Test sliding window technique."""

    def test_max_sum_subarray(self):
        """Test max sum subarray of size k."""
        assert max_sum_subarray([1, 4, 2, 10, 23, 3, 1, 0, 20], 4) == 39
        assert max_sum_subarray([1, 2, 3, 4, 5], 2) == 9
        assert max_sum_subarray([1, 2], 5) is None

    def test_longest_substring_without_repeating(self):
        """Test longest substring without repeating characters."""
        assert longest_substring_without_repeating("abcabcbb") == 3
        assert longest_substring_without_repeating("bbbbb") == 1
        assert longest_substring_without_repeating("pwwkew") == 3
        assert longest_substring_without_repeating("") == 0

    def test_min_window_substring(self):
        """Test minimum window substring."""
        assert min_window_substring("ADOBECODEBANC", "ABC") == "BANC"
        assert min_window_substring("a", "a") == "a"
        assert min_window_substring("a", "aa") == ""

    def test_longest_substring_k_distinct(self):
        """Test longest substring with k distinct characters."""
        assert longest_substring_k_distinct("eceba", 2) == 3  # "ece"
        assert longest_substring_k_distinct("aa", 1) == 2
        assert longest_substring_k_distinct("abc", 0) == 0

    def test_find_all_anagrams(self):
        """Test finding all anagrams."""
        result = find_all_anagrams("cbaebabacd", "abc")
        assert result == [0, 6]

        result = find_all_anagrams("abab", "ab")
        assert result == [0, 1, 2]


class TestBinarySearchOnAnswer:
    """Test binary search on answer pattern."""

    def test_koko_eating_bananas(self):
        """Test Koko eating bananas."""
        assert koko_eating_bananas([3, 6, 7, 11], 8) == 4
        assert koko_eating_bananas([30, 11, 23, 4, 20], 5) == 30
        assert koko_eating_bananas([30, 11, 23, 4, 20], 6) == 23

    def test_ship_within_days(self):
        """Test ship packages within days."""
        assert ship_within_days([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 5) == 15
        assert ship_within_days([3, 2, 2, 4, 1, 4], 3) == 6
        assert ship_within_days([1, 2, 3, 1, 1], 4) == 3


class TestEdgeCases:
    """Test edge cases."""

    def test_empty_array(self):
        """Test search on empty array."""
        assert binary_search([], 5) == -1
        assert linear_search([], 5) == -1
        assert lower_bound([], 5) == 0
        assert upper_bound([], 5) == 0

    def test_single_element(self):
        """Test search on single element array."""
        assert binary_search([5], 5) == 0
        assert binary_search([5], 3) == -1
        assert lower_bound([5], 5) == 0
        assert upper_bound([5], 5) == 1

    def test_all_same_elements(self):
        """Test array with all same elements."""
        arr = [5, 5, 5, 5, 5]
        assert find_first_occurrence(arr, 5) == 0
        assert find_last_occurrence(arr, 5) == 4
        assert count_occurrences(arr, 5) == 5
        assert find_range(arr, 5) == (0, 4)

    def test_large_array(self):
        """Test with large array."""
        arr = list(range(1000000))
        assert binary_search(arr, 500000) == 500000
        assert binary_search(arr, 999999) == 999999
        assert binary_search(arr, 1000001) == -1

    def test_negative_numbers(self):
        """Test with negative numbers."""
        arr = [-10, -5, 0, 5, 10]
        assert binary_search(arr, -5) == 1
        assert binary_search(arr, 0) == 2
        assert lower_bound(arr, -7) == 1
