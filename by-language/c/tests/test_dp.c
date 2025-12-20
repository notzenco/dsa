/**
 * Tests for Dynamic Programming Algorithms
 */

#include "test_framework.h"
#include "../algorithms/dynamic_programming.h"
#include <stdlib.h>
#include <string.h>

/* ============== Fibonacci Tests ============== */

TEST(fibonacci_basic) {
    ASSERT_EQ(0, fibonacci(0));
    ASSERT_EQ(1, fibonacci(1));
    ASSERT_EQ(1, fibonacci(2));
    ASSERT_EQ(2, fibonacci(3));
    ASSERT_EQ(5, fibonacci(5));
    ASSERT_EQ(55, fibonacci(10));
}

TEST(fibonacci_large) {
    ASSERT_EQ(6765, fibonacci(20));
    ASSERT_EQ(102334155, fibonacci(40));
}

TEST(climbing_stairs_basic) {
    ASSERT_EQ(1, climbing_stairs(1));
    ASSERT_EQ(2, climbing_stairs(2));
    ASSERT_EQ(3, climbing_stairs(3));
    ASSERT_EQ(5, climbing_stairs(4));
    ASSERT_EQ(8, climbing_stairs(5));
}

TEST(tribonacci_basic) {
    ASSERT_EQ(0, tribonacci(0));
    ASSERT_EQ(1, tribonacci(1));
    ASSERT_EQ(1, tribonacci(2));
    ASSERT_EQ(2, tribonacci(3));
    ASSERT_EQ(4, tribonacci(4));
    ASSERT_EQ(7, tribonacci(5));
}

/* ============== Knapsack Tests ============== */

TEST(knapsack_01_basic) {
    int weights[] = {1, 2, 3};
    int values[] = {6, 10, 12};
    ASSERT_EQ(22, knapsack_01(weights, values, 3, 5));
}

TEST(knapsack_01_exact_fit) {
    int weights[] = {2, 3, 4, 5};
    int values[] = {3, 4, 5, 6};
    ASSERT_EQ(7, knapsack_01(weights, values, 4, 5));
}

TEST(knapsack_unbounded_basic) {
    int weights[] = {1, 3, 4, 5};
    int values[] = {10, 40, 50, 70};
    /* Best: 1×5 + 1×3 = 70 + 40 = 110 with capacity 8 */
    ASSERT_EQ(110, knapsack_unbounded(weights, values, 4, 8));
}

TEST(subset_sum_true) {
    int nums[] = {3, 34, 4, 12, 5, 2};
    ASSERT_TRUE(subset_sum(nums, 6, 9));
    ASSERT_TRUE(subset_sum(nums, 6, 3));
}

TEST(subset_sum_false) {
    int nums[] = {3, 34, 4, 12, 5, 2};
    ASSERT_FALSE(subset_sum(nums, 6, 30));
}

TEST(can_partition_true) {
    int nums[] = {1, 5, 11, 5};
    ASSERT_TRUE(can_partition(nums, 4));
}

TEST(can_partition_false) {
    int nums[] = {1, 2, 3, 5};
    ASSERT_FALSE(can_partition(nums, 4));
}

TEST(target_sum_ways_basic) {
    int nums[] = {1, 1, 1, 1, 1};
    ASSERT_EQ(5, target_sum_ways(nums, 5, 3));
}

/* ============== String DP Tests ============== */

TEST(lcs_basic) {
    ASSERT_EQ(3, longest_common_subsequence("abcde", "ace"));
    ASSERT_EQ(3, longest_common_subsequence("abc", "abc"));
    ASSERT_EQ(0, longest_common_subsequence("abc", "def"));
}

TEST(lcs_empty) {
    ASSERT_EQ(0, longest_common_subsequence("", "abc"));
    ASSERT_EQ(0, longest_common_subsequence("abc", ""));
}

TEST(longest_common_substring_basic) {
    /* "Geeks" is common substring of length 5 */
    ASSERT_EQ(5, longest_common_substring("GeeksforGeeks", "GeeksQuiz"));
    /* "abcd" is common substring of length 4 */
    ASSERT_EQ(4, longest_common_substring("abcdxyz", "xyzabcd"));
}

TEST(edit_distance_basic) {
    ASSERT_EQ(3, edit_distance("horse", "ros"));
    ASSERT_EQ(5, edit_distance("intention", "execution"));
    ASSERT_EQ(0, edit_distance("abc", "abc"));
}

TEST(edit_distance_empty) {
    ASSERT_EQ(3, edit_distance("", "abc"));
    ASSERT_EQ(3, edit_distance("abc", ""));
}

TEST(longest_palindromic_subsequence_basic) {
    ASSERT_EQ(4, longest_palindromic_subsequence("bbbab"));
    ASSERT_EQ(2, longest_palindromic_subsequence("cbbd"));
    ASSERT_EQ(1, longest_palindromic_subsequence("a"));
}

TEST(longest_palindromic_substring_basic) {
    int start, len;
    longest_palindromic_substring("babad", &start, &len);
    ASSERT_EQ(3, len);

    longest_palindromic_substring("cbbd", &start, &len);
    ASSERT_EQ(2, len);
}

TEST(longest_palindromic_substring_single) {
    int start, len;
    longest_palindromic_substring("a", &start, &len);
    ASSERT_EQ(0, start);
    ASSERT_EQ(1, len);
}

/* ============== Sequence DP Tests ============== */

TEST(lis_basic) {
    int nums1[] = {10, 9, 2, 5, 3, 7, 101, 18};
    ASSERT_EQ(4, longest_increasing_subsequence(nums1, 8));

    int nums2[] = {0, 1, 0, 3, 2, 3};
    ASSERT_EQ(4, longest_increasing_subsequence(nums2, 6));
}

TEST(lis_all_same) {
    int nums[] = {7, 7, 7, 7, 7};
    ASSERT_EQ(1, longest_increasing_subsequence(nums, 5));
}

TEST(lis_increasing) {
    int nums[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(5, longest_increasing_subsequence(nums, 5));
}

TEST(max_subarray_sum_basic) {
    int nums[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    ASSERT_EQ(6, max_subarray_sum(nums, 9));
}

TEST(max_subarray_sum_all_negative) {
    int nums[] = {-2, -3, -1, -5};
    ASSERT_EQ(-1, max_subarray_sum(nums, 4));
}

TEST(max_product_subarray_basic) {
    int nums1[] = {2, 3, -2, 4};
    ASSERT_EQ(6, max_product_subarray(nums1, 4));

    int nums2[] = {-2, 0, -1};
    ASSERT_EQ(0, max_product_subarray(nums2, 3));
}

/* ============== Coin Change Tests ============== */

TEST(coin_change_min_basic) {
    int coins[] = {1, 2, 5};
    ASSERT_EQ(3, coin_change_min(coins, 3, 11));
    ASSERT_EQ(0, coin_change_min(coins, 3, 0));
}

TEST(coin_change_min_impossible) {
    int coins[] = {2};
    ASSERT_EQ(-1, coin_change_min(coins, 1, 3));
}

TEST(coin_change_ways_basic) {
    int coins[] = {1, 2, 5};
    ASSERT_EQ(4, coin_change_ways(coins, 3, 5));
}

TEST(coin_change_ways_single) {
    int coins[] = {2};
    ASSERT_EQ(1, coin_change_ways(coins, 1, 2));
    ASSERT_EQ(0, coin_change_ways(coins, 1, 3));
}

/* ============== Grid DP Tests ============== */

TEST(unique_paths_basic) {
    ASSERT_EQ(28, unique_paths(3, 7));
    ASSERT_EQ(3, unique_paths(3, 2));
    ASSERT_EQ(1, unique_paths(1, 1));
}

TEST(unique_paths_obstacles_basic) {
    int grid[] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    ASSERT_EQ(2, unique_paths_obstacles(grid, 3, 3));
}

TEST(unique_paths_obstacles_blocked) {
    int grid[] = {1, 0, 0, 0};
    ASSERT_EQ(0, unique_paths_obstacles(grid, 2, 2));
}

TEST(min_path_sum_basic) {
    int grid[] = {1, 3, 1, 1, 5, 1, 4, 2, 1};
    ASSERT_EQ(7, min_path_sum(grid, 3, 3));
}

/* ============== House Robber Tests ============== */

TEST(house_robber_basic) {
    int nums1[] = {1, 2, 3, 1};
    ASSERT_EQ(4, house_robber(nums1, 4));

    int nums2[] = {2, 7, 9, 3, 1};
    ASSERT_EQ(12, house_robber(nums2, 5));
}

TEST(house_robber_single) {
    int nums[] = {5};
    ASSERT_EQ(5, house_robber(nums, 1));
}

TEST(house_robber_circular_basic) {
    int nums1[] = {2, 3, 2};
    ASSERT_EQ(3, house_robber_circular(nums1, 3));

    int nums2[] = {1, 2, 3, 1};
    ASSERT_EQ(4, house_robber_circular(nums2, 4));
}

/* ============== Stock Tests ============== */

TEST(max_profit_one_txn_basic) {
    int prices1[] = {7, 1, 5, 3, 6, 4};
    ASSERT_EQ(5, max_profit_one_txn(prices1, 6));

    int prices2[] = {7, 6, 4, 3, 1};
    ASSERT_EQ(0, max_profit_one_txn(prices2, 5));
}

TEST(max_profit_unlimited_basic) {
    int prices1[] = {7, 1, 5, 3, 6, 4};
    ASSERT_EQ(7, max_profit_unlimited(prices1, 6));

    int prices2[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(4, max_profit_unlimited(prices2, 5));
}

TEST(max_profit_cooldown_basic) {
    int prices[] = {1, 2, 3, 0, 2};
    ASSERT_EQ(3, max_profit_cooldown(prices, 5));
}

/* ============== Classic DP Tests ============== */

TEST(rod_cutting_basic) {
    int prices[] = {1, 5, 8, 9, 10, 17, 17, 20};
    ASSERT_EQ(22, rod_cutting(prices, 8));
}

TEST(rod_cutting_small) {
    int prices[] = {2, 5, 7, 8};
    ASSERT_EQ(10, rod_cutting(prices, 4));
}

TEST(matrix_chain_basic) {
    int dims[] = {10, 30, 5, 60};
    ASSERT_EQ(4500, matrix_chain_multiply(dims, 3));
}

TEST(matrix_chain_four) {
    int dims[] = {40, 20, 30, 10, 30};
    ASSERT_EQ(26000, matrix_chain_multiply(dims, 4));
}

TEST(word_break_true) {
    const char *dict[] = {"leet", "code"};
    ASSERT_TRUE(word_break("leetcode", dict, 2));

    const char *dict2[] = {"apple", "pen"};
    ASSERT_TRUE(word_break("applepenapple", dict2, 2));
}

TEST(word_break_false) {
    const char *dict[] = {"cats", "dog", "sand", "and", "cat"};
    ASSERT_FALSE(word_break("catsandog", dict, 5));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Dynamic Programming Algorithms");

    /* Fibonacci */
    RUN_TEST(fibonacci_basic);
    RUN_TEST(fibonacci_large);
    RUN_TEST(climbing_stairs_basic);
    RUN_TEST(tribonacci_basic);

    /* Knapsack */
    RUN_TEST(knapsack_01_basic);
    RUN_TEST(knapsack_01_exact_fit);
    RUN_TEST(knapsack_unbounded_basic);
    RUN_TEST(subset_sum_true);
    RUN_TEST(subset_sum_false);
    RUN_TEST(can_partition_true);
    RUN_TEST(can_partition_false);
    RUN_TEST(target_sum_ways_basic);

    /* String DP */
    RUN_TEST(lcs_basic);
    RUN_TEST(lcs_empty);
    RUN_TEST(longest_common_substring_basic);
    RUN_TEST(edit_distance_basic);
    RUN_TEST(edit_distance_empty);
    RUN_TEST(longest_palindromic_subsequence_basic);
    RUN_TEST(longest_palindromic_substring_basic);
    RUN_TEST(longest_palindromic_substring_single);

    /* Sequence DP */
    RUN_TEST(lis_basic);
    RUN_TEST(lis_all_same);
    RUN_TEST(lis_increasing);
    RUN_TEST(max_subarray_sum_basic);
    RUN_TEST(max_subarray_sum_all_negative);
    RUN_TEST(max_product_subarray_basic);

    /* Coin Change */
    RUN_TEST(coin_change_min_basic);
    RUN_TEST(coin_change_min_impossible);
    RUN_TEST(coin_change_ways_basic);
    RUN_TEST(coin_change_ways_single);

    /* Grid DP */
    RUN_TEST(unique_paths_basic);
    RUN_TEST(unique_paths_obstacles_basic);
    RUN_TEST(unique_paths_obstacles_blocked);
    RUN_TEST(min_path_sum_basic);

    /* House Robber */
    RUN_TEST(house_robber_basic);
    RUN_TEST(house_robber_single);
    RUN_TEST(house_robber_circular_basic);

    /* Stock */
    RUN_TEST(max_profit_one_txn_basic);
    RUN_TEST(max_profit_unlimited_basic);
    RUN_TEST(max_profit_cooldown_basic);

    /* Classic DP */
    RUN_TEST(rod_cutting_basic);
    RUN_TEST(rod_cutting_small);
    RUN_TEST(matrix_chain_basic);
    RUN_TEST(matrix_chain_four);
    RUN_TEST(word_break_true);
    RUN_TEST(word_break_false);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
