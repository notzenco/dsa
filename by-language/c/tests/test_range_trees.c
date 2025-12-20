/**
 * Tests for Segment Tree and Fenwick Tree
 */

#include "test_framework.h"
#include "../data-structures/segment_tree.h"
#include "../data-structures/fenwick_tree.h"
#include <stdlib.h>

/* ============== Segment Tree Creation Tests ============== */

TEST(segtree_create_sum) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);
    ASSERT_NOT_NULL(st);
    ASSERT_EQ(6, segtree_size(st));
    segtree_destroy(st);
}

TEST(segtree_create_min) {
    int arr[] = {3, 1, 4, 1, 5, 9};
    SegmentTree *st = segtree_create(arr, 6, SEG_MIN);
    ASSERT_NOT_NULL(st);
    segtree_destroy(st);
}

TEST(segtree_create_max) {
    int arr[] = {3, 1, 4, 1, 5, 9};
    SegmentTree *st = segtree_create(arr, 6, SEG_MAX);
    ASSERT_NOT_NULL(st);
    segtree_destroy(st);
}

/* ============== Segment Tree Sum Query Tests ============== */

TEST(segtree_sum_query_full) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);
    ASSERT_EQ(36, segtree_query(st, 0, 5));
    segtree_destroy(st);
}

TEST(segtree_sum_query_partial) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);
    ASSERT_EQ(9, segtree_query(st, 0, 2));   /* 1+3+5 */
    ASSERT_EQ(27, segtree_query(st, 3, 5));  /* 7+9+11 */
    ASSERT_EQ(15, segtree_query(st, 1, 3));  /* 3+5+7 */
    segtree_destroy(st);
}

TEST(segtree_sum_query_single) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);
    ASSERT_EQ(1, segtree_query(st, 0, 0));
    ASSERT_EQ(5, segtree_query(st, 2, 2));
    ASSERT_EQ(11, segtree_query(st, 5, 5));
    segtree_destroy(st);
}

/* ============== Segment Tree Min Query Tests ============== */

TEST(segtree_min_query) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    SegmentTree *st = segtree_create(arr, 6, SEG_MIN);
    ASSERT_EQ(1, segtree_query(st, 0, 5));
    ASSERT_EQ(2, segtree_query(st, 0, 2));
    ASSERT_EQ(1, segtree_query(st, 2, 4));
    segtree_destroy(st);
}

/* ============== Segment Tree Max Query Tests ============== */

TEST(segtree_max_query) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    SegmentTree *st = segtree_create(arr, 6, SEG_MAX);
    ASSERT_EQ(9, segtree_query(st, 0, 5));
    ASSERT_EQ(8, segtree_query(st, 0, 2));
    ASSERT_EQ(9, segtree_query(st, 3, 5));
    segtree_destroy(st);
}

/* ============== Segment Tree Update Tests ============== */

TEST(segtree_update_sum) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);

    segtree_update(st, 2, 10);  /* Change 5 to 10 */
    ASSERT_EQ(10, segtree_get(st, 2));
    ASSERT_EQ(41, segtree_query(st, 0, 5));  /* 36 - 5 + 10 = 41 */
    segtree_destroy(st);
}

TEST(segtree_add) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);

    segtree_add(st, 2, 5);  /* Add 5 to index 2: 5 + 5 = 10 */
    ASSERT_EQ(10, segtree_get(st, 2));
    ASSERT_EQ(41, segtree_query(st, 0, 5));
    segtree_destroy(st);
}

/* ============== Segment Tree Range Update Tests ============== */

TEST(segtree_range_update) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);

    segtree_range_update(st, 1, 3, 10);  /* Add 10 to indices 1-3 */
    /* New array: 1, 13, 15, 17, 9, 11 = 66 */
    ASSERT_EQ(66, segtree_query(st, 0, 5));
    segtree_destroy(st);
}

/* ============== Fenwick Tree Creation Tests ============== */

TEST(fenwick_create_from_array) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);
    ASSERT_NOT_NULL(ft);
    ASSERT_EQ(6, fenwick_size(ft));
    fenwick_destroy(ft);
}

TEST(fenwick_create_empty) {
    FenwickTree *ft = fenwick_create_empty(10);
    ASSERT_NOT_NULL(ft);
    ASSERT_EQ(10, fenwick_size(ft));
    ASSERT_EQ(0, fenwick_prefix_sum(ft, 9));
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Prefix Sum Tests ============== */

TEST(fenwick_prefix_sum_basic) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);

    ASSERT_EQ(1, fenwick_prefix_sum(ft, 0));   /* 1 */
    ASSERT_EQ(4, fenwick_prefix_sum(ft, 1));   /* 1+3 */
    ASSERT_EQ(9, fenwick_prefix_sum(ft, 2));   /* 1+3+5 */
    ASSERT_EQ(16, fenwick_prefix_sum(ft, 3));  /* 1+3+5+7 */
    ASSERT_EQ(36, fenwick_prefix_sum(ft, 5));  /* all */
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Range Sum Tests ============== */

TEST(fenwick_range_sum_basic) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);

    ASSERT_EQ(36, fenwick_range_sum(ft, 0, 5));  /* all */
    ASSERT_EQ(9, fenwick_range_sum(ft, 0, 2));   /* 1+3+5 */
    ASSERT_EQ(27, fenwick_range_sum(ft, 3, 5));  /* 7+9+11 */
    ASSERT_EQ(15, fenwick_range_sum(ft, 1, 3));  /* 3+5+7 */
    ASSERT_EQ(5, fenwick_range_sum(ft, 2, 2));   /* just 5 */
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Get Tests ============== */

TEST(fenwick_get_basic) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);

    ASSERT_EQ(1, fenwick_get(ft, 0));
    ASSERT_EQ(3, fenwick_get(ft, 1));
    ASSERT_EQ(5, fenwick_get(ft, 2));
    ASSERT_EQ(7, fenwick_get(ft, 3));
    ASSERT_EQ(9, fenwick_get(ft, 4));
    ASSERT_EQ(11, fenwick_get(ft, 5));
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Add Tests ============== */

TEST(fenwick_add_basic) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);

    fenwick_add(ft, 2, 10);  /* 5 + 10 = 15 */
    ASSERT_EQ(15, fenwick_get(ft, 2));
    ASSERT_EQ(46, fenwick_range_sum(ft, 0, 5));  /* 36 + 10 */
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Update Tests ============== */

TEST(fenwick_update_basic) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    FenwickTree *ft = fenwick_create(arr, 6);

    fenwick_update(ft, 2, 100);  /* Set index 2 to 100 */
    ASSERT_EQ(100, fenwick_get(ft, 2));
    ASSERT_EQ(131, fenwick_range_sum(ft, 0, 5));  /* 36 - 5 + 100 */
    fenwick_destroy(ft);
}

/* ============== Fenwick Tree Lower Bound Tests ============== */

TEST(fenwick_lower_bound_basic) {
    int arr[] = {1, 2, 3, 4, 5};  /* Prefix sums: 1, 3, 6, 10, 15 */
    FenwickTree *ft = fenwick_create(arr, 5);

    ASSERT_EQ(1, fenwick_lower_bound(ft, 2));   /* First prefix sum >= 2 is at index 1 (sum=3) */
    ASSERT_EQ(2, fenwick_lower_bound(ft, 5));   /* First prefix sum >= 5 is at index 2 (sum=6) */
    fenwick_destroy(ft);
}

/* ============== Consistency Tests ============== */

TEST(segtree_fenwick_consistency) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    SegmentTree *st = segtree_create(arr, 6, SEG_SUM);
    FenwickTree *ft = fenwick_create(arr, 6);

    /* Both should give same results for range sums */
    for (int i = 0; i < 6; i++) {
        for (int j = i; j < 6; j++) {
            int st_sum = segtree_query(st, i, j);
            int ft_sum = fenwick_range_sum(ft, i, j);
            ASSERT_EQ(st_sum, ft_sum);
        }
    }

    segtree_destroy(st);
    fenwick_destroy(ft);
}

/* ============== NULL Safety Tests ============== */

TEST(segtree_null_safety) {
    ASSERT_EQ(0, segtree_query(NULL, 0, 5));
    ASSERT_EQ(0, segtree_size(NULL));
}

TEST(fenwick_null_safety) {
    ASSERT_EQ(0, fenwick_prefix_sum(NULL, 5));
    ASSERT_EQ(0, fenwick_range_sum(NULL, 0, 5));
    ASSERT_EQ(0, fenwick_size(NULL));
}

/* ============== Stress Tests ============== */

TEST(segtree_stress) {
    int arr[1000];
    for (int i = 0; i < 1000; i++) arr[i] = i + 1;

    SegmentTree *st = segtree_create(arr, 1000, SEG_SUM);
    ASSERT_NOT_NULL(st);

    /* Sum of 1 to 1000 = 500500 */
    ASSERT_EQ(500500, segtree_query(st, 0, 999));

    segtree_destroy(st);
}

TEST(fenwick_stress) {
    int arr[1000];
    for (int i = 0; i < 1000; i++) arr[i] = i + 1;

    FenwickTree *ft = fenwick_create(arr, 1000);
    ASSERT_NOT_NULL(ft);

    ASSERT_EQ(500500, fenwick_prefix_sum(ft, 999));

    fenwick_destroy(ft);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Segment Tree & Fenwick Tree");

    /* Segment Tree Creation */
    RUN_TEST(segtree_create_sum);
    RUN_TEST(segtree_create_min);
    RUN_TEST(segtree_create_max);

    /* Segment Tree Sum Query */
    RUN_TEST(segtree_sum_query_full);
    RUN_TEST(segtree_sum_query_partial);
    RUN_TEST(segtree_sum_query_single);

    /* Segment Tree Min/Max Query */
    RUN_TEST(segtree_min_query);
    RUN_TEST(segtree_max_query);

    /* Segment Tree Update */
    RUN_TEST(segtree_update_sum);
    RUN_TEST(segtree_add);
    RUN_TEST(segtree_range_update);

    /* Fenwick Tree Creation */
    RUN_TEST(fenwick_create_from_array);
    RUN_TEST(fenwick_create_empty);

    /* Fenwick Tree Queries */
    RUN_TEST(fenwick_prefix_sum_basic);
    RUN_TEST(fenwick_range_sum_basic);
    RUN_TEST(fenwick_get_basic);

    /* Fenwick Tree Update */
    RUN_TEST(fenwick_add_basic);
    RUN_TEST(fenwick_update_basic);
    RUN_TEST(fenwick_lower_bound_basic);

    /* Consistency */
    RUN_TEST(segtree_fenwick_consistency);

    /* NULL Safety */
    RUN_TEST(segtree_null_safety);
    RUN_TEST(fenwick_null_safety);

    /* Stress Tests */
    RUN_TEST(segtree_stress);
    RUN_TEST(fenwick_stress);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
