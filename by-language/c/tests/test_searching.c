/**
 * Tests for Searching Algorithms
 */

#include "test_framework.h"
#include "../algorithms/searching.h"
#include <stdlib.h>

/* ============== Linear Search Tests ============== */

TEST(linear_search_found) {
    int arr[] = {4, 2, 7, 1, 9, 3};
    ASSERT_EQ(2, linear_search(arr, 6, 7));
    ASSERT_EQ(0, linear_search(arr, 6, 4));
    ASSERT_EQ(5, linear_search(arr, 6, 3));
}

TEST(linear_search_not_found) {
    int arr[] = {4, 2, 7, 1, 9, 3};
    ASSERT_EQ(-1, linear_search(arr, 6, 5));
    ASSERT_EQ(-1, linear_search(arr, 6, 0));
}

TEST(linear_search_empty) {
    ASSERT_EQ(-1, linear_search(NULL, 0, 5));
}

/* ============== Binary Search Tests ============== */

TEST(binary_search_found) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_EQ(0, binary_search(arr, 9, 1));
    ASSERT_EQ(4, binary_search(arr, 9, 5));
    ASSERT_EQ(8, binary_search(arr, 9, 9));
}

TEST(binary_search_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(-1, binary_search(arr, 5, 2));
    ASSERT_EQ(-1, binary_search(arr, 5, 0));
    ASSERT_EQ(-1, binary_search(arr, 5, 10));
}

TEST(binary_search_single) {
    int arr[] = {5};
    ASSERT_EQ(0, binary_search(arr, 1, 5));
    ASSERT_EQ(-1, binary_search(arr, 1, 3));
}

TEST(binary_search_recursive_found) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_EQ(0, binary_search_recursive(arr, 9, 1));
    ASSERT_EQ(4, binary_search_recursive(arr, 9, 5));
    ASSERT_EQ(8, binary_search_recursive(arr, 9, 9));
}

/* ============== Lower/Upper Bound Tests ============== */

TEST(lower_bound_basic) {
    int arr[] = {1, 2, 2, 2, 3, 4, 5};
    ASSERT_EQ(1, lower_bound(arr, 7, 2)); /* First 2 at index 1 */
    ASSERT_EQ(0, lower_bound(arr, 7, 1));
    ASSERT_EQ(4, lower_bound(arr, 7, 3));
}

TEST(lower_bound_not_present) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(1, lower_bound(arr, 5, 2)); /* Would insert at index 1 */
    ASSERT_EQ(0, lower_bound(arr, 5, 0));
    ASSERT_EQ(5, lower_bound(arr, 5, 10)); /* Past end */
}

TEST(upper_bound_basic) {
    int arr[] = {1, 2, 2, 2, 3, 4, 5};
    ASSERT_EQ(4, upper_bound(arr, 7, 2)); /* First element > 2 at index 4 */
    ASSERT_EQ(1, upper_bound(arr, 7, 1));
    ASSERT_EQ(5, upper_bound(arr, 7, 3));
}

TEST(upper_bound_not_present) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(1, upper_bound(arr, 5, 2));
    ASSERT_EQ(0, upper_bound(arr, 5, 0));
    ASSERT_EQ(5, upper_bound(arr, 5, 10));
}

/* ============== Find First/Last Tests ============== */

TEST(find_first_basic) {
    int arr[] = {1, 2, 2, 2, 3, 4, 5};
    ASSERT_EQ(1, find_first(arr, 7, 2));
    ASSERT_EQ(0, find_first(arr, 7, 1));
    ASSERT_EQ(6, find_first(arr, 7, 5));
}

TEST(find_first_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(-1, find_first(arr, 5, 2));
}

TEST(find_last_basic) {
    int arr[] = {1, 2, 2, 2, 3, 4, 5};
    ASSERT_EQ(3, find_last(arr, 7, 2));
    ASSERT_EQ(0, find_last(arr, 7, 1));
    ASSERT_EQ(6, find_last(arr, 7, 5));
}

TEST(find_last_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(-1, find_last(arr, 5, 2));
}

TEST(count_occurrences_basic) {
    int arr[] = {1, 2, 2, 2, 3, 4, 5};
    ASSERT_EQ(3, count_occurrences(arr, 7, 2));
    ASSERT_EQ(1, count_occurrences(arr, 7, 1));
    ASSERT_EQ(0, count_occurrences(arr, 7, 6));
}

TEST(search_insert_basic) {
    int arr[] = {1, 3, 5, 6};
    ASSERT_EQ(2, search_insert(arr, 4, 5)); /* Exists */
    ASSERT_EQ(1, search_insert(arr, 4, 2)); /* Insert between 1 and 3 */
    ASSERT_EQ(4, search_insert(arr, 4, 7)); /* Insert at end */
    ASSERT_EQ(0, search_insert(arr, 4, 0)); /* Insert at beginning */
}

/* ============== Advanced Search Tests ============== */

TEST(interpolation_search_found) {
    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    ASSERT_EQ(4, interpolation_search(arr, 10, 50));
    ASSERT_EQ(0, interpolation_search(arr, 10, 10));
    ASSERT_EQ(9, interpolation_search(arr, 10, 100));
}

TEST(interpolation_search_not_found) {
    int arr[] = {10, 20, 30, 40, 50};
    ASSERT_EQ(-1, interpolation_search(arr, 5, 25));
    ASSERT_EQ(-1, interpolation_search(arr, 5, 5));
}

TEST(exponential_search_found) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    ASSERT_EQ(0, exponential_search(arr, 15, 1));
    ASSERT_EQ(7, exponential_search(arr, 15, 8));
    ASSERT_EQ(14, exponential_search(arr, 15, 15));
}

TEST(exponential_search_not_found) {
    int arr[] = {2, 4, 6, 8, 10};
    ASSERT_EQ(-1, exponential_search(arr, 5, 5));
}

TEST(jump_search_found) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    ASSERT_EQ(0, jump_search(arr, 16, 1));
    ASSERT_EQ(7, jump_search(arr, 16, 8));
    ASSERT_EQ(15, jump_search(arr, 16, 16));
}

TEST(jump_search_not_found) {
    int arr[] = {1, 3, 5, 7, 9};
    ASSERT_EQ(-1, jump_search(arr, 5, 4));
}

/* ============== Ternary Search Tests ============== */

TEST(ternary_search_min_basic) {
    /* Unimodal: decreases then increases */
    int arr[] = {9, 7, 5, 3, 1, 2, 4, 6, 8};
    ASSERT_EQ(4, ternary_search_min(arr, 9)); /* Minimum is 1 at index 4 */
}

TEST(ternary_search_min_at_start) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(0, ternary_search_min(arr, 5));
}

TEST(ternary_search_min_at_end) {
    int arr[] = {5, 4, 3, 2, 1};
    ASSERT_EQ(4, ternary_search_min(arr, 5));
}

TEST(ternary_search_max_basic) {
    /* Unimodal: increases then decreases */
    int arr[] = {1, 3, 5, 7, 9, 8, 6, 4, 2};
    ASSERT_EQ(4, ternary_search_max(arr, 9)); /* Maximum is 9 at index 4 */
}

TEST(ternary_search_max_at_start) {
    int arr[] = {5, 4, 3, 2, 1};
    ASSERT_EQ(0, ternary_search_max(arr, 5));
}

TEST(ternary_search_max_at_end) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(4, ternary_search_max(arr, 5));
}

/* ============== Rotated Array Tests ============== */

TEST(search_rotated_found) {
    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    ASSERT_EQ(0, search_rotated(arr, 7, 4));
    ASSERT_EQ(4, search_rotated(arr, 7, 0));
    ASSERT_EQ(6, search_rotated(arr, 7, 2));
}

TEST(search_rotated_not_found) {
    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    ASSERT_EQ(-1, search_rotated(arr, 7, 3));
}

TEST(search_rotated_no_rotation) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(2, search_rotated(arr, 5, 3));
}

TEST(find_rotation_point_basic) {
    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    ASSERT_EQ(4, find_rotation_point(arr, 7)); /* 0 is at index 4 */
}

TEST(find_rotation_point_no_rotation) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(0, find_rotation_point(arr, 5));
}

TEST(find_rotation_point_single_rotation) {
    int arr[] = {2, 1};
    ASSERT_EQ(1, find_rotation_point(arr, 2));
}

TEST(find_peak_basic) {
    int arr[] = {1, 2, 3, 1};
    size_t peak = find_peak(arr, 4);
    ASSERT_TRUE(peak == 2); /* 3 is the peak */
}

TEST(find_peak_at_start) {
    int arr[] = {5, 4, 3, 2, 1};
    ASSERT_EQ(0, find_peak(arr, 5));
}

TEST(find_peak_at_end) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(4, find_peak(arr, 5));
}

/* ============== 2D Matrix Tests ============== */

TEST(search_matrix_found) {
    /* Row-wise and column-wise sorted */
    int matrix[] = {
        1, 4, 7, 11,
        2, 5, 8, 12,
        3, 6, 9, 16,
        10, 13, 14, 17
    };
    size_t row, col;

    ASSERT_TRUE(search_matrix(matrix, 4, 4, 5, &row, &col));
    ASSERT_EQ(1, row);
    ASSERT_EQ(1, col);

    ASSERT_TRUE(search_matrix(matrix, 4, 4, 1, &row, &col));
    ASSERT_EQ(0, row);
    ASSERT_EQ(0, col);
}

TEST(search_matrix_not_found) {
    int matrix[] = {
        1, 4, 7, 11,
        2, 5, 8, 12,
        3, 6, 9, 16,
        10, 13, 14, 17
    };
    size_t row, col;

    ASSERT_FALSE(search_matrix(matrix, 4, 4, 15, &row, &col));
    ASSERT_FALSE(search_matrix(matrix, 4, 4, 0, &row, &col));
}

TEST(search_matrix_sorted_found) {
    /* Fully sorted matrix */
    int matrix[] = {
        1, 3, 5,
        7, 9, 11,
        13, 15, 17
    };
    size_t row, col;

    ASSERT_TRUE(search_matrix_sorted(matrix, 3, 3, 9, &row, &col));
    ASSERT_EQ(1, row);
    ASSERT_EQ(1, col);

    ASSERT_TRUE(search_matrix_sorted(matrix, 3, 3, 1, &row, &col));
    ASSERT_EQ(0, row);
    ASSERT_EQ(0, col);
}

TEST(search_matrix_sorted_not_found) {
    int matrix[] = {
        1, 3, 5,
        7, 9, 11,
        13, 15, 17
    };
    size_t row, col;

    ASSERT_FALSE(search_matrix_sorted(matrix, 3, 3, 10, &row, &col));
}

/* ============== Special Search Tests ============== */

TEST(sqrt_binary_basic) {
    ASSERT_EQ(0, sqrt_binary(0));
    ASSERT_EQ(1, sqrt_binary(1));
    ASSERT_EQ(2, sqrt_binary(4));
    ASSERT_EQ(3, sqrt_binary(9));
    ASSERT_EQ(4, sqrt_binary(16));
}

TEST(sqrt_binary_non_perfect) {
    ASSERT_EQ(2, sqrt_binary(8)); /* floor(sqrt(8)) = 2 */
    ASSERT_EQ(3, sqrt_binary(15)); /* floor(sqrt(15)) = 3 */
    ASSERT_EQ(10, sqrt_binary(100));
    ASSERT_EQ(31, sqrt_binary(1000));
}

TEST(sqrt_binary_large) {
    ASSERT_EQ(46340, sqrt_binary(2147395600));
}

TEST(kth_missing_basic) {
    int arr[] = {2, 3, 4, 7, 11};
    ASSERT_EQ(1, kth_missing(arr, 5, 1)); /* 1 is first missing */
    ASSERT_EQ(5, kth_missing(arr, 5, 2)); /* 5 is second missing */
    ASSERT_EQ(6, kth_missing(arr, 5, 3)); /* 6 is third missing */
}

TEST(kth_missing_at_start) {
    int arr[] = {5, 6, 7, 8, 9};
    ASSERT_EQ(1, kth_missing(arr, 5, 1));
    ASSERT_EQ(4, kth_missing(arr, 5, 4));
}

TEST(kth_missing_past_array) {
    int arr[] = {1, 2, 3};
    ASSERT_EQ(4, kth_missing(arr, 3, 1));
    ASSERT_EQ(5, kth_missing(arr, 3, 2));
}

/* ============== NULL Safety Tests ============== */

TEST(null_safety) {
    ASSERT_EQ(-1, linear_search(NULL, 5, 1));
    ASSERT_EQ(-1, binary_search(NULL, 5, 1));
    ASSERT_EQ(-1, binary_search_recursive(NULL, 5, 1));
    ASSERT_EQ(0, lower_bound(NULL, 5, 1));
    ASSERT_EQ(0, upper_bound(NULL, 5, 1));
    ASSERT_EQ(-1, find_first(NULL, 5, 1));
    ASSERT_EQ(-1, find_last(NULL, 5, 1));
    ASSERT_EQ(-1, interpolation_search(NULL, 5, 1));
    ASSERT_EQ(-1, exponential_search(NULL, 5, 1));
    ASSERT_EQ(-1, jump_search(NULL, 5, 1));
    ASSERT_EQ(-1, search_rotated(NULL, 5, 1));
    ASSERT_FALSE(search_matrix(NULL, 5, 5, 1, NULL, NULL));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Searching Algorithms");

    /* Linear Search */
    RUN_TEST(linear_search_found);
    RUN_TEST(linear_search_not_found);
    RUN_TEST(linear_search_empty);

    /* Binary Search */
    RUN_TEST(binary_search_found);
    RUN_TEST(binary_search_not_found);
    RUN_TEST(binary_search_single);
    RUN_TEST(binary_search_recursive_found);

    /* Lower/Upper Bound */
    RUN_TEST(lower_bound_basic);
    RUN_TEST(lower_bound_not_present);
    RUN_TEST(upper_bound_basic);
    RUN_TEST(upper_bound_not_present);

    /* Find First/Last */
    RUN_TEST(find_first_basic);
    RUN_TEST(find_first_not_found);
    RUN_TEST(find_last_basic);
    RUN_TEST(find_last_not_found);
    RUN_TEST(count_occurrences_basic);
    RUN_TEST(search_insert_basic);

    /* Advanced Search */
    RUN_TEST(interpolation_search_found);
    RUN_TEST(interpolation_search_not_found);
    RUN_TEST(exponential_search_found);
    RUN_TEST(exponential_search_not_found);
    RUN_TEST(jump_search_found);
    RUN_TEST(jump_search_not_found);

    /* Ternary Search */
    RUN_TEST(ternary_search_min_basic);
    RUN_TEST(ternary_search_min_at_start);
    RUN_TEST(ternary_search_min_at_end);
    RUN_TEST(ternary_search_max_basic);
    RUN_TEST(ternary_search_max_at_start);
    RUN_TEST(ternary_search_max_at_end);

    /* Rotated Array */
    RUN_TEST(search_rotated_found);
    RUN_TEST(search_rotated_not_found);
    RUN_TEST(search_rotated_no_rotation);
    RUN_TEST(find_rotation_point_basic);
    RUN_TEST(find_rotation_point_no_rotation);
    RUN_TEST(find_rotation_point_single_rotation);
    RUN_TEST(find_peak_basic);
    RUN_TEST(find_peak_at_start);
    RUN_TEST(find_peak_at_end);

    /* 2D Matrix */
    RUN_TEST(search_matrix_found);
    RUN_TEST(search_matrix_not_found);
    RUN_TEST(search_matrix_sorted_found);
    RUN_TEST(search_matrix_sorted_not_found);

    /* Special Search */
    RUN_TEST(sqrt_binary_basic);
    RUN_TEST(sqrt_binary_non_perfect);
    RUN_TEST(sqrt_binary_large);
    RUN_TEST(kth_missing_basic);
    RUN_TEST(kth_missing_at_start);
    RUN_TEST(kth_missing_past_array);

    /* NULL Safety */
    RUN_TEST(null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
