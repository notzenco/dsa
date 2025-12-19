"""
Algorithm implementations in Python.

This module contains implementations of fundamental and advanced algorithms
with comprehensive documentation, complexity analysis, and LeetCode problem mappings.
"""

from .sorting import (
    # Comparison sorts
    bubble_sort,
    selection_sort,
    insertion_sort,
    merge_sort,
    quick_sort,
    heap_sort,
    shell_sort,
    # Non-comparison sorts
    counting_sort,
    radix_sort,
    bucket_sort,
    # Utilities
    quick_select,
    sort_with_comparator,
    is_sorted,
)

from .searching import (
    # Basic search
    linear_search,
    binary_search,
    binary_search_recursive,
    # Binary search variants
    lower_bound,
    upper_bound,
    find_first_occurrence,
    find_last_occurrence,
    count_occurrences,
    find_range,
    search_insert_position,
    # Specialized search
    jump_search,
    interpolation_search,
    exponential_search,
    ternary_search,
    # Special problems
    search_rotated_array,
    find_rotation_point,
    find_peak_element,
    sqrt_int,
    first_bad_version,
    # Two pointers
    two_sum_sorted,
    three_sum,
    container_with_most_water,
    remove_duplicates_sorted,
    # Sliding window
    max_sum_subarray,
    longest_substring_without_repeating,
    min_window_substring,
    longest_substring_k_distinct,
    find_all_anagrams,
    # Binary search on answer
    koko_eating_bananas,
    ship_within_days,
)

from .dynamic_programming import (
    # Fibonacci
    fibonacci_recursive,
    fibonacci_memoized,
    fibonacci_tabulation,
    fibonacci_optimized,
    fibonacci_matrix,
    # Knapsack
    knapsack_01,
    knapsack_01_optimized,
    knapsack_01_items,
    knapsack_unbounded,
    # LCS
    lcs_length,
    lcs_string,
    lcs_optimized,
    # LIS
    lis_length,
    lis_binary_search,
    lis_sequence,
    # Edit Distance
    edit_distance,
    edit_distance_optimized,
    # Coin Change
    coin_change_min,
    coin_change_ways,
    # Other DP
    climbing_stairs,
    house_robber,
    max_subarray_sum,
    longest_palindromic_substring,
    word_break,
    unique_paths,
    min_path_sum,
)

__all__ = [
    # Comparison sorts
    "bubble_sort",
    "selection_sort",
    "insertion_sort",
    "merge_sort",
    "quick_sort",
    "heap_sort",
    "shell_sort",
    # Non-comparison sorts
    "counting_sort",
    "radix_sort",
    "bucket_sort",
    # Utilities
    "quick_select",
    "sort_with_comparator",
    "is_sorted",
    # Basic search
    "linear_search",
    "binary_search",
    "binary_search_recursive",
    # Binary search variants
    "lower_bound",
    "upper_bound",
    "find_first_occurrence",
    "find_last_occurrence",
    "count_occurrences",
    "find_range",
    "search_insert_position",
    # Specialized search
    "jump_search",
    "interpolation_search",
    "exponential_search",
    "ternary_search",
    # Special problems
    "search_rotated_array",
    "find_rotation_point",
    "find_peak_element",
    "sqrt_int",
    "first_bad_version",
    # Two pointers
    "two_sum_sorted",
    "three_sum",
    "container_with_most_water",
    "remove_duplicates_sorted",
    # Sliding window
    "max_sum_subarray",
    "longest_substring_without_repeating",
    "min_window_substring",
    "longest_substring_k_distinct",
    "find_all_anagrams",
    # Binary search on answer
    "koko_eating_bananas",
    "ship_within_days",
    # Dynamic Programming - Fibonacci
    "fibonacci_recursive",
    "fibonacci_memoized",
    "fibonacci_tabulation",
    "fibonacci_optimized",
    "fibonacci_matrix",
    # Dynamic Programming - Knapsack
    "knapsack_01",
    "knapsack_01_optimized",
    "knapsack_01_items",
    "knapsack_unbounded",
    # Dynamic Programming - LCS/LIS
    "lcs_length",
    "lcs_string",
    "lcs_optimized",
    "lis_length",
    "lis_binary_search",
    "lis_sequence",
    # Dynamic Programming - Edit Distance
    "edit_distance",
    "edit_distance_optimized",
    # Dynamic Programming - Coin Change
    "coin_change_min",
    "coin_change_ways",
    # Dynamic Programming - Other
    "climbing_stairs",
    "house_robber",
    "max_subarray_sum",
    "longest_palindromic_substring",
    "word_break",
    "unique_paths",
    "min_path_sum",
]
