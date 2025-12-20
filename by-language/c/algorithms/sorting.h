/**
 * SORTING ALGORITHMS
 *
 * Collection of comparison-based and non-comparison-based sorting algorithms.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         ALGORITHM COMPARISON                              ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Algorithm      │ Best     │ Average  │ Worst    │ Space  │ Stable       ║
 * ║  ───────────────┼──────────┼──────────┼──────────┼────────┼────────      ║
 * ║  Bubble Sort    │ O(n)     │ O(n²)    │ O(n²)    │ O(1)   │ Yes          ║
 * ║  Selection Sort │ O(n²)    │ O(n²)    │ O(n²)    │ O(1)   │ No           ║
 * ║  Insertion Sort │ O(n)     │ O(n²)    │ O(n²)    │ O(1)   │ Yes          ║
 * ║  Merge Sort     │ O(n lg n)│ O(n lg n)│ O(n lg n)│ O(n)   │ Yes          ║
 * ║  Quick Sort     │ O(n lg n)│ O(n lg n)│ O(n²)    │ O(lg n)│ No           ║
 * ║  Heap Sort      │ O(n lg n)│ O(n lg n)│ O(n lg n)│ O(1)   │ No           ║
 * ║  Counting Sort  │ O(n+k)   │ O(n+k)   │ O(n+k)   │ O(k)   │ Yes          ║
 * ║  Radix Sort     │ O(d·n)   │ O(d·n)   │ O(d·n)   │ O(n+k) │ Yes          ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #912 Sort an Array
 * - #88 Merge Sorted Array
 * - #75 Sort Colors (Dutch National Flag)
 * - #215 Kth Largest Element (Quick Select)
 * - #148 Sort List (Merge Sort on Linked List)
 *
 * USE CASES:
 * - Insertion Sort: Small arrays, nearly sorted data
 * - Merge Sort: Linked lists, stable sort needed, external sorting
 * - Quick Sort: General purpose, in-memory sorting
 * - Counting Sort: Small range of integers
 * - Radix Sort: Fixed-length integers or strings
 */

#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Bubble Sort - O(n²)
 * Repeatedly swaps adjacent elements if they are in wrong order.
 * @param arr Array to sort
 * @param n Size of array
 */
void bubble_sort(int *arr, size_t n);

/**
 * Selection Sort - O(n²)
 * Finds minimum element and places it at the beginning.
 * @param arr Array to sort
 * @param n Size of array
 */
void selection_sort(int *arr, size_t n);

/**
 * Insertion Sort - O(n²)
 * Builds sorted array one element at a time.
 * @param arr Array to sort
 * @param n Size of array
 */
void insertion_sort(int *arr, size_t n);

/**
 * Merge Sort - O(n log n)
 * Divide and conquer, stable sort.
 * @param arr Array to sort
 * @param n Size of array
 */
void merge_sort(int *arr, size_t n);

/**
 * Quick Sort - O(n log n) average
 * Divide and conquer using pivot partitioning.
 * @param arr Array to sort
 * @param n Size of array
 */
void quick_sort(int *arr, size_t n);

/**
 * Quick Sort with median-of-three pivot selection.
 * More robust against worst-case scenarios.
 * @param arr Array to sort
 * @param n Size of array
 */
void quick_sort_median(int *arr, size_t n);

/**
 * Counting Sort - O(n + k)
 * Non-comparison sort for integers in a known range.
 * @param arr Array to sort
 * @param n Size of array
 * Note: Handles negative numbers
 */
void counting_sort(int *arr, size_t n);

/**
 * Radix Sort (LSD) - O(d * n)
 * Non-comparison sort for non-negative integers.
 * @param arr Array to sort
 * @param n Size of array
 * Note: Only works for non-negative integers
 */
void radix_sort(int *arr, size_t n);

/**
 * Shell Sort - O(n log² n)
 * Generalization of insertion sort with gap sequence.
 * @param arr Array to sort
 * @param n Size of array
 */
void shell_sort(int *arr, size_t n);

/**
 * Dutch National Flag (3-way partitioning)
 * Sorts array containing only 0s, 1s, and 2s.
 * @param arr Array to sort
 * @param n Size of array
 */
void dutch_flag_sort(int *arr, size_t n);

/**
 * Quick Select - O(n) average
 * Find kth smallest element without fully sorting.
 * @param arr Array to search
 * @param n Size of array
 * @param k Position (0-indexed)
 * @return The kth smallest element
 */
int quick_select(int *arr, size_t n, size_t k);

/**
 * Check if array is sorted in ascending order.
 * @param arr Array to check
 * @param n Size of array
 * @return true if sorted, false otherwise
 */
bool is_sorted(const int *arr, size_t n);

/**
 * Check if array is sorted in descending order.
 * @param arr Array to check
 * @param n Size of array
 * @return true if sorted, false otherwise
 */
bool is_sorted_desc(const int *arr, size_t n);

#endif /* SORTING_H */
