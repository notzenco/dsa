/**
 * DYNAMIC PROGRAMMING ALGORITHMS
 *
 * Classic DP problems with multiple solution approaches.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         PROBLEM COMPLEXITY                                ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Problem          │ Time       │ Space    │ Approach                     ║
 * ║  ─────────────────┼────────────┼──────────┼──────────────────────        ║
 * ║  Fibonacci        │ O(n)       │ O(1)     │ Bottom-up, space optimized   ║
 * ║  0/1 Knapsack     │ O(n*W)     │ O(W)     │ 1D array optimization        ║
 * ║  LCS              │ O(m*n)     │ O(min)   │ Space optimized              ║
 * ║  LIS              │ O(n log n) │ O(n)     │ Binary search + patience     ║
 * ║  Edit Distance    │ O(m*n)     │ O(min)   │ Space optimized              ║
 * ║  Coin Change      │ O(n*amount)│ O(amount)│ Bottom-up                    ║
 * ║  Matrix Chain     │ O(n³)      │ O(n²)    │ Interval DP                  ║
 * ║  Rod Cutting      │ O(n²)      │ O(n)     │ Bottom-up                    ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #509 Fibonacci Number
 * - #70 Climbing Stairs
 * - #322 Coin Change
 * - #518 Coin Change 2
 * - #300 Longest Increasing Subsequence
 * - #1143 Longest Common Subsequence
 * - #72 Edit Distance
 * - #416 Partition Equal Subset Sum
 * - #494 Target Sum
 * - #62 Unique Paths
 * - #64 Minimum Path Sum
 * - #198 House Robber
 * - #213 House Robber II
 * - #121 Best Time to Buy and Sell Stock
 */

#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Fibonacci Variants ============== */

/**
 * Fibonacci number - O(n) time, O(1) space
 * @param n Index (0-indexed)
 * @return nth Fibonacci number
 */
long long fibonacci(int n);

/**
 * Climbing stairs - ways to climb n stairs (1 or 2 steps)
 * Same as Fibonacci.
 * @param n Number of stairs
 * @return Number of ways
 */
long long climbing_stairs(int n);

/**
 * Tribonacci - T(n) = T(n-1) + T(n-2) + T(n-3)
 * @param n Index
 * @return nth Tribonacci number
 */
long long tribonacci(int n);

/* ============== Knapsack Problems ============== */

/**
 * 0/1 Knapsack - O(n*W) time, O(W) space
 * @param weights Array of item weights
 * @param values Array of item values
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @return Maximum value achievable
 */
int knapsack_01(const int *weights, const int *values, int n, int capacity);

/**
 * Unbounded Knapsack - items can be used multiple times
 * @param weights Array of item weights
 * @param values Array of item values
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @return Maximum value achievable
 */
int knapsack_unbounded(const int *weights, const int *values, int n, int capacity);

/**
 * Subset sum - can we select items summing to target?
 * @param nums Array of numbers
 * @param n Size of array
 * @param target Target sum
 * @return true if subset exists
 */
bool subset_sum(const int *nums, int n, int target);

/**
 * Partition equal subset sum - can we partition into two equal subsets?
 * @param nums Array of numbers
 * @param n Size of array
 * @return true if partition exists
 */
bool can_partition(const int *nums, int n);

/**
 * Target sum - ways to assign +/- to reach target
 * @param nums Array of numbers
 * @param n Size of array
 * @param target Target sum
 * @return Number of ways
 */
int target_sum_ways(const int *nums, int n, int target);

/* ============== String DP ============== */

/**
 * Longest Common Subsequence - O(m*n) time, O(min(m,n)) space
 * @param s1 First string
 * @param s2 Second string
 * @return Length of LCS
 */
int longest_common_subsequence(const char *s1, const char *s2);

/**
 * Longest Common Substring
 * @param s1 First string
 * @param s2 Second string
 * @return Length of longest common substring
 */
int longest_common_substring(const char *s1, const char *s2);

/**
 * Edit Distance (Levenshtein) - O(m*n) time
 * @param s1 Source string
 * @param s2 Target string
 * @return Minimum edits to transform s1 to s2
 */
int edit_distance(const char *s1, const char *s2);

/**
 * Longest Palindromic Subsequence
 * @param s String
 * @return Length of longest palindromic subsequence
 */
int longest_palindromic_subsequence(const char *s);

/**
 * Longest Palindromic Substring
 * @param s String
 * @param start_out Output: start index of palindrome
 * @param len_out Output: length of palindrome
 */
void longest_palindromic_substring(const char *s, int *start_out, int *len_out);

/* ============== Sequence DP ============== */

/**
 * Longest Increasing Subsequence - O(n log n)
 * @param nums Array of numbers
 * @param n Size of array
 * @return Length of LIS
 */
int longest_increasing_subsequence(const int *nums, int n);

/**
 * Maximum Subarray Sum (Kadane's) - O(n)
 * @param nums Array of numbers
 * @param n Size of array
 * @return Maximum subarray sum
 */
int max_subarray_sum(const int *nums, int n);

/**
 * Maximum Product Subarray - O(n)
 * @param nums Array of numbers
 * @param n Size of array
 * @return Maximum product of subarray
 */
int max_product_subarray(const int *nums, int n);

/* ============== Coin/Change Problems ============== */

/**
 * Coin Change - minimum coins to make amount
 * @param coins Array of coin denominations
 * @param n Number of coin types
 * @param amount Target amount
 * @return Minimum coins needed, or -1 if impossible
 */
int coin_change_min(const int *coins, int n, int amount);

/**
 * Coin Change 2 - number of ways to make amount
 * @param coins Array of coin denominations
 * @param n Number of coin types
 * @param amount Target amount
 * @return Number of combinations
 */
int coin_change_ways(const int *coins, int n, int amount);

/* ============== Grid DP ============== */

/**
 * Unique Paths - ways to reach bottom-right
 * @param m Number of rows
 * @param n Number of columns
 * @return Number of unique paths
 */
long long unique_paths(int m, int n);

/**
 * Unique Paths with Obstacles
 * @param grid Obstacle grid (1 = obstacle)
 * @param m Number of rows
 * @param n Number of columns
 * @return Number of unique paths
 */
int unique_paths_obstacles(const int *grid, int m, int n);

/**
 * Minimum Path Sum
 * @param grid Cost grid
 * @param m Number of rows
 * @param n Number of columns
 * @return Minimum path sum from top-left to bottom-right
 */
int min_path_sum(const int *grid, int m, int n);

/* ============== House Robber ============== */

/**
 * House Robber - max money without robbing adjacent
 * @param nums Array of money in each house
 * @param n Number of houses
 * @return Maximum money
 */
int house_robber(const int *nums, int n);

/**
 * House Robber II - houses in a circle
 * @param nums Array of money in each house
 * @param n Number of houses
 * @return Maximum money
 */
int house_robber_circular(const int *nums, int n);

/* ============== Stock Problems ============== */

/**
 * Best Time to Buy and Sell Stock - one transaction
 * @param prices Array of prices
 * @param n Number of days
 * @return Maximum profit
 */
int max_profit_one_txn(const int *prices, int n);

/**
 * Best Time to Buy and Sell Stock II - unlimited transactions
 * @param prices Array of prices
 * @param n Number of days
 * @return Maximum profit
 */
int max_profit_unlimited(const int *prices, int n);

/**
 * Best Time to Buy and Sell Stock with Cooldown
 * @param prices Array of prices
 * @param n Number of days
 * @return Maximum profit
 */
int max_profit_cooldown(const int *prices, int n);

/* ============== Classic DP ============== */

/**
 * Rod Cutting - maximize profit by cutting rod
 * @param prices prices[i] = price of rod of length i+1
 * @param n Length of rod
 * @return Maximum obtainable value
 */
int rod_cutting(const int *prices, int n);

/**
 * Matrix Chain Multiplication - minimum scalar multiplications
 * @param dims Array of matrix dimensions
 * @param n Number of matrices (dims has n+1 elements)
 * @return Minimum multiplications
 */
int matrix_chain_multiply(const int *dims, int n);

/**
 * Word Break - can string be segmented into dictionary words?
 * @param s String to segment
 * @param word_dict Array of dictionary words
 * @param dict_size Number of words in dictionary
 * @return true if can be segmented
 */
bool word_break(const char *s, const char **word_dict, int dict_size);

#endif /* DYNAMIC_PROGRAMMING_H */
