"""
Tests for Dynamic Programming Algorithms.
"""

import pytest
from algorithms.dynamic_programming import (
    # Fibonacci
    fibonacci_recursive, fibonacci_memoized, fibonacci_tabulation,
    fibonacci_optimized, fibonacci_matrix,
    # Knapsack
    knapsack_01, knapsack_01_optimized, knapsack_01_items, knapsack_unbounded,
    # LCS
    lcs_length, lcs_string, lcs_optimized,
    # LIS
    lis_length, lis_binary_search, lis_sequence,
    # Edit Distance
    edit_distance, edit_distance_optimized,
    # Coin Change
    coin_change_min, coin_change_ways,
    # Other
    climbing_stairs, house_robber, max_subarray_sum,
    longest_palindromic_substring, word_break, unique_paths, min_path_sum
)


class TestFibonacci:
    """Test Fibonacci implementations."""

    @pytest.mark.parametrize("fib_func", [
        fibonacci_memoized, fibonacci_tabulation,
        fibonacci_optimized, fibonacci_matrix
    ])
    def test_fibonacci_basic(self, fib_func):
        """Test basic Fibonacci values."""
        assert fib_func(0) == 0
        assert fib_func(1) == 1
        assert fib_func(2) == 1
        assert fib_func(5) == 5
        assert fib_func(10) == 55

    @pytest.mark.parametrize("fib_func", [
        fibonacci_memoized, fibonacci_tabulation,
        fibonacci_optimized, fibonacci_matrix
    ])
    def test_fibonacci_larger(self, fib_func):
        """Test larger Fibonacci numbers."""
        assert fib_func(20) == 6765
        assert fib_func(30) == 832040

    def test_fibonacci_recursive_small(self):
        """Test recursive version (only small inputs due to O(2^n))."""
        assert fibonacci_recursive(10) == 55


class TestKnapsack:
    """Test Knapsack implementations."""

    def test_knapsack_01_basic(self):
        """Test basic 0/1 knapsack."""
        weights = [1, 2, 3]
        values = [6, 10, 12]
        capacity = 5

        assert knapsack_01(weights, values, capacity) == 22
        assert knapsack_01_optimized(weights, values, capacity) == 22

    def test_knapsack_01_items(self):
        """Test knapsack with item reconstruction."""
        weights = [1, 2, 3]
        values = [6, 10, 12]
        capacity = 5

        max_val, items = knapsack_01_items(weights, values, capacity)
        assert max_val == 22
        # Should take items 1 and 2 (indices 1 and 2)

    def test_knapsack_01_empty(self):
        """Test empty knapsack."""
        assert knapsack_01([], [], 10) == 0

    def test_knapsack_01_no_capacity(self):
        """Test with zero capacity."""
        weights = [1, 2, 3]
        values = [10, 20, 30]
        assert knapsack_01(weights, values, 0) == 0

    def test_knapsack_unbounded(self):
        """Test unbounded knapsack."""
        weights = [1, 3, 4]
        values = [15, 50, 60]
        capacity = 8

        # Can use each item unlimited times
        # Optimal: 2 items of weight 3 (value 50) + 2 items of weight 1 (value 15) = 130
        result = knapsack_unbounded(weights, values, capacity)
        assert result == 130


class TestLCS:
    """Test Longest Common Subsequence."""

    def test_lcs_basic(self):
        """Test basic LCS."""
        assert lcs_length("abcde", "ace") == 3
        assert lcs_length("abc", "abc") == 3
        assert lcs_length("abc", "def") == 0

    def test_lcs_string(self):
        """Test LCS string reconstruction."""
        assert lcs_string("abcde", "ace") == "ace"
        assert lcs_string("AGGTAB", "GXTXAYB") == "GTAB"

    def test_lcs_optimized(self):
        """Test space-optimized LCS."""
        assert lcs_optimized("abcde", "ace") == 3
        assert lcs_optimized("abc", "def") == 0

    def test_lcs_empty(self):
        """Test LCS with empty strings."""
        assert lcs_length("", "abc") == 0
        assert lcs_length("abc", "") == 0
        assert lcs_length("", "") == 0


class TestLIS:
    """Test Longest Increasing Subsequence."""

    def test_lis_basic(self):
        """Test basic LIS."""
        assert lis_length([10, 9, 2, 5, 3, 7, 101, 18]) == 4
        assert lis_length([0, 1, 0, 3, 2, 3]) == 4
        assert lis_length([7, 7, 7, 7, 7]) == 1

    def test_lis_binary_search(self):
        """Test binary search LIS."""
        assert lis_binary_search([10, 9, 2, 5, 3, 7, 101, 18]) == 4
        assert lis_binary_search([0, 1, 0, 3, 2, 3]) == 4

    def test_lis_sequence(self):
        """Test LIS sequence reconstruction."""
        result = lis_sequence([10, 9, 2, 5, 3, 7, 101, 18])
        assert len(result) == 4
        # Check it's actually increasing
        for i in range(1, len(result)):
            assert result[i] > result[i - 1]

    def test_lis_empty(self):
        """Test LIS with empty array."""
        assert lis_length([]) == 0
        assert lis_sequence([]) == []

    def test_lis_single(self):
        """Test LIS with single element."""
        assert lis_length([42]) == 1


class TestEditDistance:
    """Test Edit Distance."""

    def test_edit_distance_basic(self):
        """Test basic edit distance."""
        assert edit_distance("horse", "ros") == 3
        assert edit_distance("intention", "execution") == 5

    def test_edit_distance_optimized(self):
        """Test space-optimized edit distance."""
        assert edit_distance_optimized("horse", "ros") == 3
        assert edit_distance_optimized("intention", "execution") == 5

    def test_edit_distance_same(self):
        """Test identical strings."""
        assert edit_distance("abc", "abc") == 0

    def test_edit_distance_empty(self):
        """Test with empty strings."""
        assert edit_distance("", "abc") == 3
        assert edit_distance("abc", "") == 3
        assert edit_distance("", "") == 0


class TestCoinChange:
    """Test Coin Change problems."""

    def test_coin_change_min(self):
        """Test minimum coins."""
        assert coin_change_min([1, 2, 5], 11) == 3  # 5+5+1
        assert coin_change_min([2], 3) == -1
        assert coin_change_min([1], 0) == 0

    def test_coin_change_ways(self):
        """Test number of ways."""
        assert coin_change_ways([1, 2, 5], 5) == 4
        assert coin_change_ways([2], 3) == 0
        assert coin_change_ways([10], 10) == 1


class TestClimbingStairs:
    """Test Climbing Stairs."""

    def test_climbing_stairs(self):
        """Test climbing stairs."""
        assert climbing_stairs(1) == 1
        assert climbing_stairs(2) == 2
        assert climbing_stairs(3) == 3
        assert climbing_stairs(4) == 5
        assert climbing_stairs(5) == 8


class TestHouseRobber:
    """Test House Robber."""

    def test_house_robber(self):
        """Test house robber."""
        assert house_robber([1, 2, 3, 1]) == 4
        assert house_robber([2, 7, 9, 3, 1]) == 12

    def test_house_robber_empty(self):
        """Test with empty array."""
        assert house_robber([]) == 0
        assert house_robber([5]) == 5


class TestMaxSubarraySum:
    """Test Maximum Subarray Sum (Kadane's)."""

    def test_max_subarray_sum(self):
        """Test max subarray sum."""
        assert max_subarray_sum([-2, 1, -3, 4, -1, 2, 1, -5, 4]) == 6
        assert max_subarray_sum([1]) == 1
        assert max_subarray_sum([5, 4, -1, 7, 8]) == 23

    def test_max_subarray_all_negative(self):
        """Test with all negative numbers."""
        assert max_subarray_sum([-3, -2, -1]) == -1


class TestLongestPalindromicSubstring:
    """Test Longest Palindromic Substring."""

    def test_longest_palindrome(self):
        """Test longest palindrome."""
        result = longest_palindromic_substring("babad")
        assert result in ["bab", "aba"]

        assert longest_palindromic_substring("cbbd") == "bb"

    def test_longest_palindrome_single(self):
        """Test single character."""
        assert longest_palindromic_substring("a") == "a"

    def test_longest_palindrome_empty(self):
        """Test empty string."""
        assert longest_palindromic_substring("") == ""


class TestWordBreak:
    """Test Word Break."""

    def test_word_break_true(self):
        """Test cases where break is possible."""
        assert word_break("leetcode", ["leet", "code"])
        assert word_break("applepenapple", ["apple", "pen"])

    def test_word_break_false(self):
        """Test cases where break is impossible."""
        assert not word_break("catsandog", ["cats", "dog", "sand", "and", "cat"])


class TestUniquePaths:
    """Test Unique Paths."""

    def test_unique_paths(self):
        """Test unique paths."""
        assert unique_paths(3, 7) == 28
        assert unique_paths(3, 2) == 3
        assert unique_paths(1, 1) == 1


class TestMinPathSum:
    """Test Minimum Path Sum."""

    def test_min_path_sum(self):
        """Test minimum path sum."""
        grid = [
            [1, 3, 1],
            [1, 5, 1],
            [4, 2, 1]
        ]
        assert min_path_sum(grid) == 7  # 1->3->1->1->1

    def test_min_path_sum_single(self):
        """Test single cell."""
        assert min_path_sum([[5]]) == 5

    def test_min_path_sum_empty(self):
        """Test empty grid."""
        assert min_path_sum([]) == 0
        assert min_path_sum([[]]) == 0
