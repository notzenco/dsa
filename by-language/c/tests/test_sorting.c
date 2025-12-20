/**
 * Tests for Sorting Algorithms
 */

#include "test_framework.h"
#include "../algorithms/sorting.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* ============== Helper Functions ============== */

static int* create_array(const int *src, size_t n) {
    int *arr = malloc(n * sizeof(int));
    if (arr != NULL) {
        memcpy(arr, src, n * sizeof(int));
    }
    return arr;
}

static bool arrays_equal(const int *a, const int *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

/* ============== Bubble Sort Tests ============== */

TEST(bubble_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9};
    bubble_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(9, arr[4]);
}

TEST(bubble_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    bubble_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(bubble_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    bubble_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(bubble_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    bubble_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(bubble_sort_single) {
    int arr[] = {42};
    bubble_sort(arr, 1);
    ASSERT_EQ(42, arr[0]);
}

TEST(bubble_sort_empty) {
    bubble_sort(NULL, 0);
    /* Should not crash */
}

/* ============== Selection Sort Tests ============== */

TEST(selection_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9};
    selection_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(selection_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    selection_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(selection_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    selection_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(selection_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    selection_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

/* ============== Insertion Sort Tests ============== */

TEST(insertion_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9};
    insertion_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(insertion_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    insertion_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(insertion_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    insertion_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(insertion_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    insertion_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

/* ============== Merge Sort Tests ============== */

TEST(merge_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    merge_sort(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(merge_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    merge_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(merge_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    merge_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(merge_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    merge_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(merge_sort_large) {
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 100 - i;
    }
    merge_sort(arr, 100);
    ASSERT_TRUE(is_sorted(arr, 100));
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(100, arr[99]);
}

/* ============== Quick Sort Tests ============== */

TEST(quick_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quick_sort(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(quick_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    quick_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(quick_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    quick_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(quick_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    quick_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(quick_sort_large) {
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 100 - i;
    }
    quick_sort(arr, 100);
    ASSERT_TRUE(is_sorted(arr, 100));
}

/* ============== Quick Sort Median Tests ============== */

TEST(quick_sort_median_basic) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quick_sort_median(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(quick_sort_median_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    quick_sort_median(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(quick_sort_median_reverse) {
    int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    quick_sort_median(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(quick_sort_median_large) {
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 100 - i;
    }
    quick_sort_median(arr, 100);
    ASSERT_TRUE(is_sorted(arr, 100));
}

/* ============== Counting Sort Tests ============== */

TEST(counting_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    counting_sort(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(counting_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    counting_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
}

TEST(counting_sort_negative) {
    int arr[] = {3, -1, 4, -1, 5, -9, 2, 6, -5, 3};
    counting_sort(arr, 10);
    ASSERT_TRUE(is_sorted(arr, 10));
    ASSERT_EQ(-9, arr[0]);
}

TEST(counting_sort_all_same) {
    int arr[] = {5, 5, 5, 5, 5};
    counting_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

/* ============== Radix Sort Tests ============== */

TEST(radix_sort_basic) {
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    radix_sort(arr, 8);
    ASSERT_TRUE(is_sorted(arr, 8));
}

TEST(radix_sort_single_digit) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    radix_sort(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(radix_sort_large_numbers) {
    int arr[] = {12345, 67890, 11111, 99999, 54321};
    radix_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(radix_sort_duplicates) {
    int arr[] = {100, 200, 100, 300, 200, 100};
    radix_sort(arr, 6);
    ASSERT_TRUE(is_sorted(arr, 6));
}

/* ============== Shell Sort Tests ============== */

TEST(shell_sort_basic) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    shell_sort(arr, 9);
    ASSERT_TRUE(is_sorted(arr, 9));
}

TEST(shell_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    shell_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(shell_sort_reverse) {
    int arr[] = {5, 4, 3, 2, 1};
    shell_sort(arr, 5);
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(shell_sort_large) {
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 100 - i;
    }
    shell_sort(arr, 100);
    ASSERT_TRUE(is_sorted(arr, 100));
}

/* ============== Dutch National Flag Tests ============== */

TEST(dutch_flag_basic) {
    int arr[] = {2, 0, 1, 2, 1, 0};
    dutch_flag_sort(arr, 6);
    int expected[] = {0, 0, 1, 1, 2, 2};
    ASSERT_TRUE(arrays_equal(arr, expected, 6));
}

TEST(dutch_flag_all_zeros) {
    int arr[] = {0, 0, 0, 0};
    dutch_flag_sort(arr, 4);
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(0, arr[i]);
    }
}

TEST(dutch_flag_all_ones) {
    int arr[] = {1, 1, 1, 1};
    dutch_flag_sort(arr, 4);
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(1, arr[i]);
    }
}

TEST(dutch_flag_all_twos) {
    int arr[] = {2, 2, 2, 2};
    dutch_flag_sort(arr, 4);
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(2, arr[i]);
    }
}

TEST(dutch_flag_single_each) {
    int arr[] = {2, 1, 0};
    dutch_flag_sort(arr, 3);
    ASSERT_EQ(0, arr[0]);
    ASSERT_EQ(1, arr[1]);
    ASSERT_EQ(2, arr[2]);
}

TEST(dutch_flag_already_sorted) {
    int arr[] = {0, 0, 1, 1, 2, 2};
    dutch_flag_sort(arr, 6);
    int expected[] = {0, 0, 1, 1, 2, 2};
    ASSERT_TRUE(arrays_equal(arr, expected, 6));
}

/* ============== Quick Select Tests ============== */

TEST(quick_select_basic) {
    int arr[] = {5, 2, 8, 1, 9};
    ASSERT_EQ(1, quick_select(arr, 5, 0)); /* 0th smallest = 1 */
    ASSERT_EQ(2, quick_select(arr, 5, 1)); /* 1st smallest = 2 */
    ASSERT_EQ(5, quick_select(arr, 5, 2)); /* 2nd smallest = 5 */
    ASSERT_EQ(8, quick_select(arr, 5, 3)); /* 3rd smallest = 8 */
    ASSERT_EQ(9, quick_select(arr, 5, 4)); /* 4th smallest = 9 */
}

TEST(quick_select_duplicates) {
    int arr[] = {3, 1, 4, 1, 5};
    ASSERT_EQ(1, quick_select(arr, 5, 0));
    ASSERT_EQ(1, quick_select(arr, 5, 1));
    ASSERT_EQ(3, quick_select(arr, 5, 2));
}

TEST(quick_select_single) {
    int arr[] = {42};
    ASSERT_EQ(42, quick_select(arr, 1, 0));
}

TEST(quick_select_median) {
    int arr[] = {9, 1, 5, 3, 7};
    ASSERT_EQ(5, quick_select(arr, 5, 2)); /* Median */
}

TEST(quick_select_invalid) {
    int arr[] = {1, 2, 3};
    ASSERT_EQ(INT_MIN, quick_select(arr, 3, 5)); /* k out of range */
    ASSERT_EQ(INT_MIN, quick_select(NULL, 3, 0)); /* NULL array */
}

/* ============== is_sorted Tests ============== */

TEST(is_sorted_asc) {
    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_TRUE(is_sorted(arr, 5));
}

TEST(is_sorted_not_sorted) {
    int arr[] = {1, 3, 2, 4, 5};
    ASSERT_FALSE(is_sorted(arr, 5));
}

TEST(is_sorted_desc) {
    int arr[] = {5, 4, 3, 2, 1};
    ASSERT_TRUE(is_sorted_desc(arr, 5));
}

TEST(is_sorted_desc_not_sorted) {
    int arr[] = {5, 3, 4, 2, 1};
    ASSERT_FALSE(is_sorted_desc(arr, 5));
}

TEST(is_sorted_single) {
    int arr[] = {42};
    ASSERT_TRUE(is_sorted(arr, 1));
    ASSERT_TRUE(is_sorted_desc(arr, 1));
}

TEST(is_sorted_empty) {
    ASSERT_TRUE(is_sorted(NULL, 0));
    ASSERT_TRUE(is_sorted_desc(NULL, 0));
}

/* ============== Cross-Algorithm Consistency Tests ============== */

TEST(all_sorts_produce_same_result) {
    int original[] = {64, 34, 25, 12, 22, 11, 90, 1, 45, 33};
    int expected[] = {1, 11, 12, 22, 25, 33, 34, 45, 64, 90};
    size_t n = 10;

    int *arr1 = create_array(original, n);
    int *arr2 = create_array(original, n);
    int *arr3 = create_array(original, n);
    int *arr4 = create_array(original, n);
    int *arr5 = create_array(original, n);
    int *arr6 = create_array(original, n);
    int *arr7 = create_array(original, n);
    int *arr8 = create_array(original, n);

    bubble_sort(arr1, n);
    selection_sort(arr2, n);
    insertion_sort(arr3, n);
    merge_sort(arr4, n);
    quick_sort(arr5, n);
    quick_sort_median(arr6, n);
    counting_sort(arr7, n);
    shell_sort(arr8, n);

    ASSERT_TRUE(arrays_equal(arr1, expected, n));
    ASSERT_TRUE(arrays_equal(arr2, expected, n));
    ASSERT_TRUE(arrays_equal(arr3, expected, n));
    ASSERT_TRUE(arrays_equal(arr4, expected, n));
    ASSERT_TRUE(arrays_equal(arr5, expected, n));
    ASSERT_TRUE(arrays_equal(arr6, expected, n));
    ASSERT_TRUE(arrays_equal(arr7, expected, n));
    ASSERT_TRUE(arrays_equal(arr8, expected, n));

    free(arr1); free(arr2); free(arr3); free(arr4);
    free(arr5); free(arr6); free(arr7); free(arr8);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Sorting Algorithms");

    /* Bubble Sort */
    RUN_TEST(bubble_sort_basic);
    RUN_TEST(bubble_sort_already_sorted);
    RUN_TEST(bubble_sort_reverse);
    RUN_TEST(bubble_sort_duplicates);
    RUN_TEST(bubble_sort_single);
    RUN_TEST(bubble_sort_empty);

    /* Selection Sort */
    RUN_TEST(selection_sort_basic);
    RUN_TEST(selection_sort_already_sorted);
    RUN_TEST(selection_sort_reverse);
    RUN_TEST(selection_sort_duplicates);

    /* Insertion Sort */
    RUN_TEST(insertion_sort_basic);
    RUN_TEST(insertion_sort_already_sorted);
    RUN_TEST(insertion_sort_reverse);
    RUN_TEST(insertion_sort_duplicates);

    /* Merge Sort */
    RUN_TEST(merge_sort_basic);
    RUN_TEST(merge_sort_already_sorted);
    RUN_TEST(merge_sort_reverse);
    RUN_TEST(merge_sort_duplicates);
    RUN_TEST(merge_sort_large);

    /* Quick Sort */
    RUN_TEST(quick_sort_basic);
    RUN_TEST(quick_sort_already_sorted);
    RUN_TEST(quick_sort_reverse);
    RUN_TEST(quick_sort_duplicates);
    RUN_TEST(quick_sort_large);

    /* Quick Sort Median */
    RUN_TEST(quick_sort_median_basic);
    RUN_TEST(quick_sort_median_already_sorted);
    RUN_TEST(quick_sort_median_reverse);
    RUN_TEST(quick_sort_median_large);

    /* Counting Sort */
    RUN_TEST(counting_sort_basic);
    RUN_TEST(counting_sort_duplicates);
    RUN_TEST(counting_sort_negative);
    RUN_TEST(counting_sort_all_same);

    /* Radix Sort */
    RUN_TEST(radix_sort_basic);
    RUN_TEST(radix_sort_single_digit);
    RUN_TEST(radix_sort_large_numbers);
    RUN_TEST(radix_sort_duplicates);

    /* Shell Sort */
    RUN_TEST(shell_sort_basic);
    RUN_TEST(shell_sort_already_sorted);
    RUN_TEST(shell_sort_reverse);
    RUN_TEST(shell_sort_large);

    /* Dutch National Flag */
    RUN_TEST(dutch_flag_basic);
    RUN_TEST(dutch_flag_all_zeros);
    RUN_TEST(dutch_flag_all_ones);
    RUN_TEST(dutch_flag_all_twos);
    RUN_TEST(dutch_flag_single_each);
    RUN_TEST(dutch_flag_already_sorted);

    /* Quick Select */
    RUN_TEST(quick_select_basic);
    RUN_TEST(quick_select_duplicates);
    RUN_TEST(quick_select_single);
    RUN_TEST(quick_select_median);
    RUN_TEST(quick_select_invalid);

    /* is_sorted */
    RUN_TEST(is_sorted_asc);
    RUN_TEST(is_sorted_not_sorted);
    RUN_TEST(is_sorted_desc);
    RUN_TEST(is_sorted_desc_not_sorted);
    RUN_TEST(is_sorted_single);
    RUN_TEST(is_sorted_empty);

    /* Cross-algorithm */
    RUN_TEST(all_sorts_produce_same_result);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
