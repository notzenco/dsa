/**
 * SEARCHING ALGORITHMS
 *
 * Collection of searching algorithms for arrays.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         ALGORITHM COMPARISON                              ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Algorithm           │ Time (Avg) │ Time (Worst)│ Prerequisite            ║
 * ║  ────────────────────┼────────────┼─────────────┼────────────────────     ║
 * ║  Linear Search       │ O(n)       │ O(n)        │ None                    ║
 * ║  Binary Search       │ O(log n)   │ O(log n)    │ Sorted array            ║
 * ║  Interpolation Search│ O(log log n)│O(n)        │ Sorted, uniform dist    ║
 * ║  Exponential Search  │ O(log n)   │ O(log n)    │ Sorted, unbounded       ║
 * ║  Ternary Search      │ O(log n)   │ O(log n)    │ Unimodal function       ║
 * ║  Jump Search         │ O(√n)      │ O(√n)       │ Sorted array            ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #704 Binary Search
 * - #35 Search Insert Position
 * - #34 Find First and Last Position of Element
 * - #33 Search in Rotated Sorted Array
 * - #153 Find Minimum in Rotated Sorted Array
 * - #162 Find Peak Element
 * - #69 Sqrt(x)
 *
 * USE CASES:
 * - Linear Search: Unsorted data, small arrays
 * - Binary Search: Sorted arrays, decision problems
 * - Interpolation Search: Uniformly distributed sorted data
 * - Exponential Search: Unbounded/infinite arrays
 * - Ternary Search: Finding extrema in unimodal functions
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Basic Search ============== */

/**
 * Linear search - O(n)
 * Works on unsorted arrays.
 * @param arr Array to search
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int linear_search(const int *arr, size_t n, int target);

/**
 * Binary search - O(log n)
 * Requires sorted array.
 * @param arr Sorted array to search
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int binary_search(const int *arr, size_t n, int target);

/**
 * Binary search (recursive) - O(log n)
 * @param arr Sorted array to search
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int binary_search_recursive(const int *arr, size_t n, int target);

/* ============== Binary Search Variants ============== */

/**
 * Lower bound - O(log n)
 * Find index of first element >= target.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of first element >= target, or n if all elements < target
 */
size_t lower_bound(const int *arr, size_t n, int target);

/**
 * Upper bound - O(log n)
 * Find index of first element > target.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of first element > target, or n if all elements <= target
 */
size_t upper_bound(const int *arr, size_t n, int target);

/**
 * Find first occurrence - O(log n)
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of first occurrence or -1 if not found
 */
int find_first(const int *arr, size_t n, int target);

/**
 * Find last occurrence - O(log n)
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of last occurrence or -1 if not found
 */
int find_last(const int *arr, size_t n, int target);

/**
 * Count occurrences - O(log n)
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to count
 * @return Number of occurrences of target
 */
size_t count_occurrences(const int *arr, size_t n, int target);

/**
 * Search insert position - O(log n)
 * Find index where target should be inserted to maintain sorted order.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to insert
 * @return Index where target should be inserted
 */
size_t search_insert(const int *arr, size_t n, int target);

/* ============== Advanced Search ============== */

/**
 * Interpolation search - O(log log n) average, O(n) worst
 * Best for uniformly distributed sorted data.
 * @param arr Sorted array with uniform distribution
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int interpolation_search(const int *arr, size_t n, int target);

/**
 * Exponential search - O(log n)
 * Good for unbounded or very large sorted arrays.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int exponential_search(const int *arr, size_t n, int target);

/**
 * Jump search - O(√n)
 * Block-based search for sorted arrays.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int jump_search(const int *arr, size_t n, int target);

/**
 * Ternary search for minimum - O(log n)
 * Find minimum in a unimodal function (decreases then increases).
 * @param arr Array representing unimodal function
 * @param n Size of array
 * @return Index of minimum element
 */
size_t ternary_search_min(const int *arr, size_t n);

/**
 * Ternary search for maximum - O(log n)
 * Find maximum in a unimodal function (increases then decreases).
 * @param arr Array representing unimodal function
 * @param n Size of array
 * @return Index of maximum element
 */
size_t ternary_search_max(const int *arr, size_t n);

/* ============== Rotated Array Search ============== */

/**
 * Search in rotated sorted array - O(log n)
 * @param arr Rotated sorted array
 * @param n Size of array
 * @param target Value to find
 * @return Index of target or -1 if not found
 */
int search_rotated(const int *arr, size_t n, int target);

/**
 * Find minimum in rotated sorted array - O(log n)
 * @param arr Rotated sorted array
 * @param n Size of array
 * @return Index of minimum element
 */
size_t find_rotation_point(const int *arr, size_t n);

/**
 * Find peak element - O(log n)
 * @param arr Array where arr[i] != arr[i+1]
 * @param n Size of array
 * @return Index of any peak element
 */
size_t find_peak(const int *arr, size_t n);

/* ============== 2D Search ============== */

/**
 * Search in row-wise and column-wise sorted 2D matrix - O(m + n)
 * @param matrix 2D array (flattened row-major)
 * @param rows Number of rows
 * @param cols Number of columns
 * @param target Value to find
 * @param row_out Output parameter for row index
 * @param col_out Output parameter for column index
 * @return true if found, false otherwise
 */
bool search_matrix(const int *matrix, size_t rows, size_t cols, int target,
                   size_t *row_out, size_t *col_out);

/**
 * Search in fully sorted 2D matrix - O(log(m*n))
 * Matrix where first element of each row > last of previous row.
 * @param matrix 2D array (flattened row-major)
 * @param rows Number of rows
 * @param cols Number of columns
 * @param target Value to find
 * @param row_out Output parameter for row index
 * @param col_out Output parameter for column index
 * @return true if found, false otherwise
 */
bool search_matrix_sorted(const int *matrix, size_t rows, size_t cols, int target,
                          size_t *row_out, size_t *col_out);

/* ============== Special Search ============== */

/**
 * Binary search for square root - O(log n)
 * Find floor of square root of n.
 * @param n Number to find square root of
 * @return Floor of sqrt(n)
 */
int sqrt_binary(int n);

/**
 * Find kth missing positive number - O(log n)
 * @param arr Sorted array of positive integers
 * @param n Size of array
 * @param k The kth missing number to find
 * @return The kth missing positive number
 */
int kth_missing(const int *arr, size_t n, int k);

/* ============== Two Pointers Techniques ============== */

/**
 * Two Sum (sorted array) - O(n)
 * Find two numbers that sum to target.
 * @param arr Sorted array
 * @param n Size of array
 * @param target Target sum
 * @param i_out Index of first number
 * @param j_out Index of second number
 * @return true if pair found
 */
bool two_sum_sorted(const int *arr, size_t n, int target, size_t *i_out, size_t *j_out);

/**
 * Three Sum - O(n²)
 * Find all unique triplets that sum to target.
 * @param arr Array (will be sorted in place)
 * @param n Size of array
 * @param target Target sum
 * @param triplets Output array of [i,j,k] indices (caller allocates)
 * @param max_triplets Maximum triplets to find
 * @return Number of triplets found
 */
size_t three_sum(int *arr, size_t n, int target, int (*triplets)[3], size_t max_triplets);

/**
 * Remove duplicates from sorted array - O(n)
 * Modifies array in place.
 * @param arr Sorted array
 * @param n Size of array
 * @return New length after removing duplicates
 */
size_t remove_duplicates(int *arr, size_t n);

/**
 * Move zeros to end - O(n)
 * Modifies array in place, preserving relative order.
 * @param arr Array
 * @param n Size of array
 */
void move_zeros(int *arr, size_t n);

/**
 * Container with most water - O(n)
 * Find max area formed by two lines.
 * @param heights Array of heights
 * @param n Size of array
 * @return Maximum water that can be contained
 */
int max_water_container(const int *heights, size_t n);

/**
 * Valid palindrome (ignoring non-alphanumeric) - O(n)
 * @param s String to check
 * @return true if palindrome
 */
bool is_palindrome_str(const char *s);

/**
 * Reverse array in place - O(n)
 * @param arr Array to reverse
 * @param n Size of array
 */
void reverse_array(int *arr, size_t n);

/**
 * Dutch National Flag / Sort Colors - O(n)
 * Sort array containing only 0, 1, 2.
 * @param arr Array to sort
 * @param n Size of array
 */
void sort_colors(int *arr, size_t n);

/**
 * Partition array around pivot - O(n)
 * @param arr Array to partition
 * @param n Size of array
 * @param pivot Pivot value
 * @return Index where pivot would be
 */
size_t partition_array(int *arr, size_t n, int pivot);

/* ============== Sliding Window Techniques ============== */

/**
 * Maximum sum subarray of size k - O(n)
 * @param arr Array
 * @param n Size of array
 * @param k Window size
 * @return Maximum sum of any k consecutive elements
 */
int max_sum_subarray_k(const int *arr, size_t n, size_t k);

/**
 * Minimum size subarray with sum >= target - O(n)
 * @param arr Array of positive integers
 * @param n Size of array
 * @param target Target sum
 * @return Minimum length of subarray with sum >= target, or 0 if none exists
 */
size_t min_subarray_sum(const int *arr, size_t n, int target);

/**
 * Longest substring without repeating characters - O(n)
 * @param s String
 * @return Length of longest substring with unique characters
 */
size_t longest_unique_substring(const char *s);

/**
 * Count subarrays with sum equal to k - O(n)
 * @param arr Array
 * @param n Size of array
 * @param k Target sum
 * @return Number of subarrays with sum equal to k
 */
size_t count_subarrays_sum(const int *arr, size_t n, int k);

/**
 * Maximum of all subarrays of size k - O(n)
 * Uses deque-based sliding window.
 * @param arr Array
 * @param n Size of array
 * @param k Window size
 * @param result Output array (size n-k+1, caller allocates)
 * @return Number of maximums written
 */
size_t sliding_window_max(const int *arr, size_t n, size_t k, int *result);

/**
 * Find all anagrams of pattern in string - O(n)
 * @param s String to search in
 * @param p Pattern to find anagrams of
 * @param indices Output array for starting indices (caller allocates)
 * @param max_indices Maximum indices to return
 * @return Number of anagram positions found
 */
size_t find_anagrams(const char *s, const char *p, size_t *indices, size_t max_indices);

/**
 * Minimum window substring - O(n)
 * Find minimum window in s containing all characters of t.
 * @param s Source string
 * @param t Target characters
 * @param start_out Starting index of minimum window
 * @param len_out Length of minimum window
 * @return true if window found
 */
bool min_window_substring(const char *s, const char *t, size_t *start_out, size_t *len_out);

#endif /* SEARCHING_H */
