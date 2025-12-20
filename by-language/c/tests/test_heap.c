/**
 * Tests for Heap and Priority Queue
 */

#include "test_framework.h"
#include "../data-structures/heap.h"
#include <stdlib.h>

/* ============== Min Heap Tests ============== */

TEST(min_heap_create_empty) {
    MinHeap *heap = min_heap_create();
    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(0, min_heap_size(heap));
    ASSERT_TRUE(min_heap_is_empty(heap));
    min_heap_destroy(heap);
}

TEST(min_heap_push_single) {
    MinHeap *heap = min_heap_create();
    ASSERT_TRUE(min_heap_push(heap, 42));
    ASSERT_EQ(1, min_heap_size(heap));

    int value;
    ASSERT_TRUE(min_heap_peek(heap, &value));
    ASSERT_EQ(42, value);
    min_heap_destroy(heap);
}

TEST(min_heap_push_multiple) {
    MinHeap *heap = min_heap_create();
    min_heap_push(heap, 5);
    min_heap_push(heap, 3);
    min_heap_push(heap, 7);
    min_heap_push(heap, 1);

    ASSERT_EQ(4, min_heap_size(heap));

    int value;
    min_heap_peek(heap, &value);
    ASSERT_EQ(1, value); /* Min should be at top */
    min_heap_destroy(heap);
}

TEST(min_heap_pop_order) {
    MinHeap *heap = min_heap_create();
    min_heap_push(heap, 5);
    min_heap_push(heap, 2);
    min_heap_push(heap, 8);
    min_heap_push(heap, 1);
    min_heap_push(heap, 9);

    int value;
    min_heap_pop(heap, &value);
    ASSERT_EQ(1, value);
    min_heap_pop(heap, &value);
    ASSERT_EQ(2, value);
    min_heap_pop(heap, &value);
    ASSERT_EQ(5, value);
    min_heap_pop(heap, &value);
    ASSERT_EQ(8, value);
    min_heap_pop(heap, &value);
    ASSERT_EQ(9, value);

    ASSERT_TRUE(min_heap_is_empty(heap));
    min_heap_destroy(heap);
}

TEST(min_heap_pop_empty) {
    MinHeap *heap = min_heap_create();
    int value;
    ASSERT_FALSE(min_heap_pop(heap, &value));
    min_heap_destroy(heap);
}

TEST(min_heap_from_array) {
    int arr[] = {5, 3, 7, 1, 4, 6, 2};
    MinHeap *heap = min_heap_from_array(arr, 7);

    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(7, min_heap_size(heap));

    int value;
    min_heap_peek(heap, &value);
    ASSERT_EQ(1, value);

    /* Verify heap property by extracting all */
    int prev = -1;
    while (!min_heap_is_empty(heap)) {
        min_heap_pop(heap, &value);
        ASSERT_TRUE(value >= prev);
        prev = value;
    }

    min_heap_destroy(heap);
}

TEST(min_heap_clear) {
    MinHeap *heap = min_heap_create();
    min_heap_push(heap, 1);
    min_heap_push(heap, 2);
    min_heap_push(heap, 3);

    min_heap_clear(heap);
    ASSERT_EQ(0, min_heap_size(heap));
    ASSERT_TRUE(min_heap_is_empty(heap));
    min_heap_destroy(heap);
}

TEST(min_heap_null_safety) {
    ASSERT_EQ(0, min_heap_size(NULL));
    ASSERT_TRUE(min_heap_is_empty(NULL));
    ASSERT_FALSE(min_heap_push(NULL, 42));
    ASSERT_FALSE(min_heap_pop(NULL, NULL));
    ASSERT_FALSE(min_heap_peek(NULL, NULL));
}

/* ============== Max Heap Tests ============== */

TEST(max_heap_create_empty) {
    MaxHeap *heap = max_heap_create();
    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(0, max_heap_size(heap));
    ASSERT_TRUE(max_heap_is_empty(heap));
    max_heap_destroy(heap);
}

TEST(max_heap_push_single) {
    MaxHeap *heap = max_heap_create();
    ASSERT_TRUE(max_heap_push(heap, 42));
    ASSERT_EQ(1, max_heap_size(heap));

    int value;
    ASSERT_TRUE(max_heap_peek(heap, &value));
    ASSERT_EQ(42, value);
    max_heap_destroy(heap);
}

TEST(max_heap_push_multiple) {
    MaxHeap *heap = max_heap_create();
    max_heap_push(heap, 5);
    max_heap_push(heap, 3);
    max_heap_push(heap, 7);
    max_heap_push(heap, 1);

    ASSERT_EQ(4, max_heap_size(heap));

    int value;
    max_heap_peek(heap, &value);
    ASSERT_EQ(7, value); /* Max should be at top */
    max_heap_destroy(heap);
}

TEST(max_heap_pop_order) {
    MaxHeap *heap = max_heap_create();
    max_heap_push(heap, 5);
    max_heap_push(heap, 2);
    max_heap_push(heap, 8);
    max_heap_push(heap, 1);
    max_heap_push(heap, 9);

    int value;
    max_heap_pop(heap, &value);
    ASSERT_EQ(9, value);
    max_heap_pop(heap, &value);
    ASSERT_EQ(8, value);
    max_heap_pop(heap, &value);
    ASSERT_EQ(5, value);
    max_heap_pop(heap, &value);
    ASSERT_EQ(2, value);
    max_heap_pop(heap, &value);
    ASSERT_EQ(1, value);

    ASSERT_TRUE(max_heap_is_empty(heap));
    max_heap_destroy(heap);
}

TEST(max_heap_pop_empty) {
    MaxHeap *heap = max_heap_create();
    int value;
    ASSERT_FALSE(max_heap_pop(heap, &value));
    max_heap_destroy(heap);
}

TEST(max_heap_from_array) {
    int arr[] = {5, 3, 7, 1, 4, 6, 2};
    MaxHeap *heap = max_heap_from_array(arr, 7);

    ASSERT_NOT_NULL(heap);
    ASSERT_EQ(7, max_heap_size(heap));

    int value;
    max_heap_peek(heap, &value);
    ASSERT_EQ(7, value);

    /* Verify heap property by extracting all */
    int prev = 100;
    while (!max_heap_is_empty(heap)) {
        max_heap_pop(heap, &value);
        ASSERT_TRUE(value <= prev);
        prev = value;
    }

    max_heap_destroy(heap);
}

TEST(max_heap_clear) {
    MaxHeap *heap = max_heap_create();
    max_heap_push(heap, 1);
    max_heap_push(heap, 2);
    max_heap_push(heap, 3);

    max_heap_clear(heap);
    ASSERT_EQ(0, max_heap_size(heap));
    ASSERT_TRUE(max_heap_is_empty(heap));
    max_heap_destroy(heap);
}

TEST(max_heap_null_safety) {
    ASSERT_EQ(0, max_heap_size(NULL));
    ASSERT_TRUE(max_heap_is_empty(NULL));
    ASSERT_FALSE(max_heap_push(NULL, 42));
    ASSERT_FALSE(max_heap_pop(NULL, NULL));
    ASSERT_FALSE(max_heap_peek(NULL, NULL));
}

/* ============== Priority Queue Tests ============== */

TEST(pq_create_empty) {
    PriorityQueue *pq = pq_create();
    ASSERT_NOT_NULL(pq);
    ASSERT_EQ(0, pq_size(pq));
    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

TEST(pq_push_single) {
    PriorityQueue *pq = pq_create();
    ASSERT_TRUE(pq_push(pq, 100, 1));
    ASSERT_EQ(1, pq_size(pq));

    int value;
    ASSERT_TRUE(pq_peek(pq, &value));
    ASSERT_EQ(100, value);
    pq_destroy(pq);
}

TEST(pq_push_priority_order) {
    PriorityQueue *pq = pq_create();
    pq_push(pq, 100, 5);  /* Low priority */
    pq_push(pq, 200, 1);  /* High priority */
    pq_push(pq, 300, 3);  /* Medium priority */

    int value;
    pq_peek(pq, &value);
    ASSERT_EQ(200, value); /* Highest priority (lowest number) at top */
    pq_destroy(pq);
}

TEST(pq_pop_order) {
    PriorityQueue *pq = pq_create();
    pq_push(pq, 100, 5);
    pq_push(pq, 200, 1);
    pq_push(pq, 300, 3);
    pq_push(pq, 400, 2);

    int value;
    pq_pop(pq, &value);
    ASSERT_EQ(200, value); /* Priority 1 */
    pq_pop(pq, &value);
    ASSERT_EQ(400, value); /* Priority 2 */
    pq_pop(pq, &value);
    ASSERT_EQ(300, value); /* Priority 3 */
    pq_pop(pq, &value);
    ASSERT_EQ(100, value); /* Priority 5 */

    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

TEST(pq_pop_empty) {
    PriorityQueue *pq = pq_create();
    int value;
    ASSERT_FALSE(pq_pop(pq, &value));
    pq_destroy(pq);
}

TEST(pq_same_priority) {
    PriorityQueue *pq = pq_create();
    pq_push(pq, 100, 1);
    pq_push(pq, 200, 1);
    pq_push(pq, 300, 1);

    ASSERT_EQ(3, pq_size(pq));

    /* All have same priority, order may vary */
    int sum = 0;
    int value;
    while (!pq_is_empty(pq)) {
        pq_pop(pq, &value);
        sum += value;
    }
    ASSERT_EQ(600, sum);

    pq_destroy(pq);
}

TEST(pq_clear) {
    PriorityQueue *pq = pq_create();
    pq_push(pq, 100, 1);
    pq_push(pq, 200, 2);
    pq_push(pq, 300, 3);

    pq_clear(pq);
    ASSERT_EQ(0, pq_size(pq));
    ASSERT_TRUE(pq_is_empty(pq));
    pq_destroy(pq);
}

TEST(pq_null_safety) {
    ASSERT_EQ(0, pq_size(NULL));
    ASSERT_TRUE(pq_is_empty(NULL));
    ASSERT_FALSE(pq_push(NULL, 100, 1));
    ASSERT_FALSE(pq_pop(NULL, NULL));
    ASSERT_FALSE(pq_peek(NULL, NULL));
}

/* ============== Heap Sort Tests ============== */

TEST(heap_sort_asc_test) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    heap_sort_asc(arr, 9);

    for (int i = 0; i < 8; i++) {
        ASSERT_TRUE(arr[i] <= arr[i + 1]);
    }
    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(9, arr[8]);
}

TEST(heap_sort_desc_test) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    heap_sort_desc(arr, 9);

    for (int i = 0; i < 8; i++) {
        ASSERT_TRUE(arr[i] >= arr[i + 1]);
    }
    ASSERT_EQ(9, arr[0]);
    ASSERT_EQ(1, arr[8]);
}

TEST(heap_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    heap_sort_asc(arr, 5);

    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(5, arr[4]);
}

TEST(heap_sort_reverse_sorted) {
    int arr[] = {5, 4, 3, 2, 1};
    heap_sort_asc(arr, 5);

    ASSERT_EQ(1, arr[0]);
    ASSERT_EQ(5, arr[4]);
}

TEST(heap_sort_single) {
    int arr[] = {42};
    heap_sort_asc(arr, 1);
    ASSERT_EQ(42, arr[0]);
}

TEST(heap_sort_duplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    heap_sort_asc(arr, 10);

    for (int i = 0; i < 9; i++) {
        ASSERT_TRUE(arr[i] <= arr[i + 1]);
    }
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Heap and Priority Queue");

    /* Min Heap tests */
    RUN_TEST(min_heap_create_empty);
    RUN_TEST(min_heap_push_single);
    RUN_TEST(min_heap_push_multiple);
    RUN_TEST(min_heap_pop_order);
    RUN_TEST(min_heap_pop_empty);
    RUN_TEST(min_heap_from_array);
    RUN_TEST(min_heap_clear);
    RUN_TEST(min_heap_null_safety);

    /* Max Heap tests */
    RUN_TEST(max_heap_create_empty);
    RUN_TEST(max_heap_push_single);
    RUN_TEST(max_heap_push_multiple);
    RUN_TEST(max_heap_pop_order);
    RUN_TEST(max_heap_pop_empty);
    RUN_TEST(max_heap_from_array);
    RUN_TEST(max_heap_clear);
    RUN_TEST(max_heap_null_safety);

    /* Priority Queue tests */
    RUN_TEST(pq_create_empty);
    RUN_TEST(pq_push_single);
    RUN_TEST(pq_push_priority_order);
    RUN_TEST(pq_pop_order);
    RUN_TEST(pq_pop_empty);
    RUN_TEST(pq_same_priority);
    RUN_TEST(pq_clear);
    RUN_TEST(pq_null_safety);

    /* Heap Sort tests */
    RUN_TEST(heap_sort_asc_test);
    RUN_TEST(heap_sort_desc_test);
    RUN_TEST(heap_sort_already_sorted);
    RUN_TEST(heap_sort_reverse_sorted);
    RUN_TEST(heap_sort_single);
    RUN_TEST(heap_sort_duplicates);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
