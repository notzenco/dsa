/**
 * Tests for Monotonic Queue
 */

#include "test_framework.h"
#include "../data-structures/monotonic_queue.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(mq_create_empty) {
    MonotonicQueue *mq = mq_create();
    ASSERT_NOT_NULL(mq);
    ASSERT_EQ(0, mq_size(mq));
    ASSERT_TRUE(mq_is_empty(mq));
    mq_destroy(mq);
}

TEST(mq_create_with_capacity) {
    MonotonicQueue *mq = mq_create_with_capacity(100);
    ASSERT_NOT_NULL(mq);
    ASSERT_EQ(0, mq_size(mq));
    mq_destroy(mq);
}

/* ============== Push Tests ============== */

TEST(mq_push_single) {
    MonotonicQueue *mq = mq_create();
    ASSERT_TRUE(mq_push(mq, 0, 42));
    ASSERT_EQ(1, mq_size(mq));

    int value;
    ASSERT_TRUE(mq_get_max(mq, &value));
    ASSERT_EQ(42, value);
    mq_destroy(mq);
}

TEST(mq_push_increasing) {
    /* Push increasing values - each push removes all previous elements */
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 0, 10);
    mq_push(mq, 1, 20);
    mq_push(mq, 2, 30);

    /* Only the last (largest) should remain */
    ASSERT_EQ(1, mq_size(mq));

    int value;
    mq_get_max(mq, &value);
    ASSERT_EQ(30, value);
    mq_destroy(mq);
}

TEST(mq_push_decreasing) {
    /* Push decreasing values - all elements should remain */
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 0, 30);
    mq_push(mq, 1, 20);
    mq_push(mq, 2, 10);

    ASSERT_EQ(3, mq_size(mq));

    int value;
    mq_get_max(mq, &value);
    ASSERT_EQ(30, value);
    mq_destroy(mq);
}

TEST(mq_push_mixed) {
    /* [3, 1, 4, 1, 5, 9, 2, 6] */
    MonotonicQueue *mq = mq_create();

    mq_push(mq, 0, 3);  /* [3] */
    ASSERT_EQ(1, mq_size(mq));

    mq_push(mq, 1, 1);  /* [3, 1] */
    ASSERT_EQ(2, mq_size(mq));

    mq_push(mq, 2, 4);  /* [4] - removes 3, 1 */
    ASSERT_EQ(1, mq_size(mq));

    mq_push(mq, 3, 1);  /* [4, 1] */
    ASSERT_EQ(2, mq_size(mq));

    mq_push(mq, 4, 5);  /* [5] - removes 4, 1 */
    ASSERT_EQ(1, mq_size(mq));

    mq_push(mq, 5, 9);  /* [9] - removes 5 */
    ASSERT_EQ(1, mq_size(mq));

    int value;
    mq_get_max(mq, &value);
    ASSERT_EQ(9, value);

    mq_destroy(mq);
}

/* ============== Pop Tests ============== */

TEST(mq_pop_matching_index) {
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 0, 30);
    mq_push(mq, 1, 20);
    mq_push(mq, 2, 10);

    /* Pop front element with matching index */
    mq_pop(mq, 0);
    ASSERT_EQ(2, mq_size(mq));

    int value;
    mq_get_max(mq, &value);
    ASSERT_EQ(20, value);

    mq_destroy(mq);
}

TEST(mq_pop_non_matching_index) {
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 0, 30);
    mq_push(mq, 1, 20);

    /* Pop with non-matching index - should do nothing */
    mq_pop(mq, 5);
    ASSERT_EQ(2, mq_size(mq));

    mq_destroy(mq);
}

TEST(mq_pop_empty) {
    MonotonicQueue *mq = mq_create();
    /* Pop from empty queue - should not crash */
    mq_pop(mq, 0);
    ASSERT_TRUE(mq_is_empty(mq));
    mq_destroy(mq);
}

/* ============== Get Max Tests ============== */

TEST(mq_get_max_empty) {
    MonotonicQueue *mq = mq_create();
    int value;
    ASSERT_FALSE(mq_get_max(mq, &value));
    mq_destroy(mq);
}

TEST(mq_get_max_index) {
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 5, 100);
    mq_push(mq, 6, 50);

    int index;
    ASSERT_TRUE(mq_get_max_index(mq, &index));
    ASSERT_EQ(5, index);

    mq_destroy(mq);
}

/* ============== Clear Tests ============== */

TEST(mq_clear) {
    MonotonicQueue *mq = mq_create();
    mq_push(mq, 0, 10);
    mq_push(mq, 1, 20);
    mq_push(mq, 2, 30);

    mq_clear(mq);
    ASSERT_TRUE(mq_is_empty(mq));
    ASSERT_EQ(0, mq_size(mq));

    mq_destroy(mq);
}

/* ============== Sliding Window Maximum Tests ============== */

TEST(sliding_window_max_basic) {
    int nums[] = {1, 3, -1, -3, 5, 3, 6, 7};
    size_t n = 8;
    size_t k = 3;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(6, result_size);

    /* Expected: [3, 3, 5, 5, 6, 7] */
    ASSERT_EQ(3, result[0]);
    ASSERT_EQ(3, result[1]);
    ASSERT_EQ(5, result[2]);
    ASSERT_EQ(5, result[3]);
    ASSERT_EQ(6, result[4]);
    ASSERT_EQ(7, result[5]);

    free(result);
}

TEST(sliding_window_max_k_equals_n) {
    int nums[] = {1, 3, 5, 2, 8};
    size_t n = 5;
    size_t k = 5;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(1, result_size);
    ASSERT_EQ(8, result[0]);

    free(result);
}

TEST(sliding_window_max_k_equals_1) {
    int nums[] = {1, 3, 5, 2, 8};
    size_t n = 5;
    size_t k = 1;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(5, result_size);

    /* Each element is its own max */
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(3, result[1]);
    ASSERT_EQ(5, result[2]);
    ASSERT_EQ(2, result[3]);
    ASSERT_EQ(8, result[4]);

    free(result);
}

TEST(sliding_window_max_all_same) {
    int nums[] = {5, 5, 5, 5, 5};
    size_t n = 5;
    size_t k = 3;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result_size);

    ASSERT_EQ(5, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(5, result[2]);

    free(result);
}

TEST(sliding_window_max_decreasing) {
    int nums[] = {9, 8, 7, 6, 5};
    size_t n = 5;
    size_t k = 2;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(4, result_size);

    ASSERT_EQ(9, result[0]);
    ASSERT_EQ(8, result[1]);
    ASSERT_EQ(7, result[2]);
    ASSERT_EQ(6, result[3]);

    free(result);
}

TEST(sliding_window_max_increasing) {
    int nums[] = {1, 2, 3, 4, 5};
    size_t n = 5;
    size_t k = 2;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(4, result_size);

    ASSERT_EQ(2, result[0]);
    ASSERT_EQ(3, result[1]);
    ASSERT_EQ(4, result[2]);
    ASSERT_EQ(5, result[3]);

    free(result);
}

/* ============== Sliding Window Minimum Tests ============== */

TEST(sliding_window_min_basic) {
    int nums[] = {1, 3, -1, -3, 5, 3, 6, 7};
    size_t n = 8;
    size_t k = 3;
    size_t result_size;

    int *result = mq_sliding_window_min(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(6, result_size);

    /* Expected: [-1, -3, -3, -3, 3, 3] */
    ASSERT_EQ(-1, result[0]);
    ASSERT_EQ(-3, result[1]);
    ASSERT_EQ(-3, result[2]);
    ASSERT_EQ(-3, result[3]);
    ASSERT_EQ(3, result[4]);
    ASSERT_EQ(3, result[5]);

    free(result);
}

/* ============== Edge Cases ============== */

TEST(sliding_window_max_null_input) {
    size_t result_size;
    int *result = mq_sliding_window_max(NULL, 5, 3, &result_size);
    ASSERT_NULL(result);
    ASSERT_EQ(0, result_size);
}

TEST(sliding_window_max_k_greater_than_n) {
    int nums[] = {1, 2, 3};
    size_t result_size;
    int *result = mq_sliding_window_max(nums, 3, 5, &result_size);
    ASSERT_NULL(result);
    ASSERT_EQ(0, result_size);
}

TEST(sliding_window_max_k_zero) {
    int nums[] = {1, 2, 3};
    size_t result_size;
    int *result = mq_sliding_window_max(nums, 3, 0, &result_size);
    ASSERT_NULL(result);
    ASSERT_EQ(0, result_size);
}

TEST(sliding_window_max_with_negatives) {
    int nums[] = {-7, -8, -6, -10, -11};
    size_t n = 5;
    size_t k = 3;
    size_t result_size;

    int *result = mq_sliding_window_max(nums, n, k, &result_size);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(3, result_size);

    ASSERT_EQ(-6, result[0]);
    ASSERT_EQ(-6, result[1]);
    ASSERT_EQ(-6, result[2]);

    free(result);
}

/* ============== Main ============== */

int main(void) {
    TEST_SUITE_START("Monotonic Queue Tests");

    /* Creation tests */
    RUN_TEST(mq_create_empty);
    RUN_TEST(mq_create_with_capacity);

    /* Push tests */
    RUN_TEST(mq_push_single);
    RUN_TEST(mq_push_increasing);
    RUN_TEST(mq_push_decreasing);
    RUN_TEST(mq_push_mixed);

    /* Pop tests */
    RUN_TEST(mq_pop_matching_index);
    RUN_TEST(mq_pop_non_matching_index);
    RUN_TEST(mq_pop_empty);

    /* Get max tests */
    RUN_TEST(mq_get_max_empty);
    RUN_TEST(mq_get_max_index);

    /* Clear tests */
    RUN_TEST(mq_clear);

    /* Sliding window maximum tests */
    RUN_TEST(sliding_window_max_basic);
    RUN_TEST(sliding_window_max_k_equals_n);
    RUN_TEST(sliding_window_max_k_equals_1);
    RUN_TEST(sliding_window_max_all_same);
    RUN_TEST(sliding_window_max_decreasing);
    RUN_TEST(sliding_window_max_increasing);

    /* Sliding window minimum tests */
    RUN_TEST(sliding_window_min_basic);

    /* Edge cases */
    RUN_TEST(sliding_window_max_null_input);
    RUN_TEST(sliding_window_max_k_greater_than_n);
    RUN_TEST(sliding_window_max_k_zero);
    RUN_TEST(sliding_window_max_with_negatives);

    TEST_SUITE_END();
}
