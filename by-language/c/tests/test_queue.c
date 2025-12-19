/**
 * Tests for Queue, Circular Queue, and Deque
 */

#include "test_framework.h"
#include "../data-structures/queue.h"

/* ============== Queue Tests ============== */

TEST(queue_create_empty) {
    Queue *queue = queue_create();
    ASSERT_NOT_NULL(queue);
    ASSERT_EQ(0, queue_size(queue));
    ASSERT_TRUE(queue_is_empty(queue));
    queue_destroy(queue);
}

TEST(queue_enqueue_single) {
    Queue *queue = queue_create();
    ASSERT_TRUE(queue_enqueue(queue, 42));
    ASSERT_EQ(1, queue_size(queue));

    int value;
    ASSERT_TRUE(queue_front(queue, &value));
    ASSERT_EQ(42, value);
    queue_destroy(queue);
}

TEST(queue_enqueue_multiple) {
    Queue *queue = queue_create();
    queue_enqueue(queue, 10);
    queue_enqueue(queue, 20);
    queue_enqueue(queue, 30);

    ASSERT_EQ(3, queue_size(queue));

    int front, rear;
    queue_front(queue, &front);
    queue_rear(queue, &rear);
    ASSERT_EQ(10, front);
    ASSERT_EQ(30, rear);
    queue_destroy(queue);
}

TEST(queue_dequeue_single) {
    Queue *queue = queue_create();
    queue_enqueue(queue, 42);

    int value;
    ASSERT_TRUE(queue_dequeue(queue, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, queue_size(queue));
    queue_destroy(queue);
}

TEST(queue_dequeue_multiple) {
    Queue *queue = queue_create();
    queue_enqueue(queue, 10);
    queue_enqueue(queue, 20);
    queue_enqueue(queue, 30);

    int value;
    queue_dequeue(queue, &value);
    ASSERT_EQ(10, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(20, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(30, value);

    ASSERT_TRUE(queue_is_empty(queue));
    queue_destroy(queue);
}

TEST(queue_dequeue_empty) {
    Queue *queue = queue_create();
    int value;
    ASSERT_FALSE(queue_dequeue(queue, &value));
    queue_destroy(queue);
}

TEST(queue_fifo_order) {
    Queue *queue = queue_create();

    /* Enqueue 1, 2, 3, 4, 5 */
    for (int i = 1; i <= 5; i++) {
        queue_enqueue(queue, i);
    }

    /* Dequeue should return 1, 2, 3, 4, 5 */
    int value;
    for (int i = 1; i <= 5; i++) {
        queue_dequeue(queue, &value);
        ASSERT_EQ(i, value);
    }

    queue_destroy(queue);
}

TEST(queue_interleaved_ops) {
    Queue *queue = queue_create();

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);

    int value;
    queue_dequeue(queue, &value);
    ASSERT_EQ(1, value);

    queue_enqueue(queue, 3);
    queue_enqueue(queue, 4);

    queue_dequeue(queue, &value);
    ASSERT_EQ(2, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(3, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(4, value);

    queue_destroy(queue);
}

TEST(queue_clear) {
    Queue *queue = queue_create();
    queue_enqueue(queue, 10);
    queue_enqueue(queue, 20);

    queue_clear(queue);
    ASSERT_EQ(0, queue_size(queue));
    ASSERT_TRUE(queue_is_empty(queue));
    queue_destroy(queue);
}

TEST(queue_resize) {
    Queue *queue = queue_create_with_capacity(2);
    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    queue_enqueue(queue, 3); /* Triggers resize */

    ASSERT_EQ(3, queue_size(queue));

    int value;
    queue_dequeue(queue, &value);
    ASSERT_EQ(1, value);
    queue_destroy(queue);
}

TEST(queue_wraparound) {
    Queue *queue = queue_create_with_capacity(4);

    /* Fill and partially empty to cause wraparound */
    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    queue_enqueue(queue, 3);

    int value;
    queue_dequeue(queue, &value);
    queue_dequeue(queue, &value);

    /* Now add more - should wrap around */
    queue_enqueue(queue, 4);
    queue_enqueue(queue, 5);

    queue_dequeue(queue, &value);
    ASSERT_EQ(3, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(4, value);
    queue_dequeue(queue, &value);
    ASSERT_EQ(5, value);

    queue_destroy(queue);
}

TEST(queue_null_safety) {
    ASSERT_EQ(0, queue_size(NULL));
    ASSERT_TRUE(queue_is_empty(NULL));
    ASSERT_FALSE(queue_enqueue(NULL, 42));
    ASSERT_FALSE(queue_dequeue(NULL, NULL));
}

/* ============== Circular Queue Tests ============== */

TEST(circular_queue_create) {
    CircularQueue *queue = circular_queue_create(5);
    ASSERT_NOT_NULL(queue);
    ASSERT_EQ(0, circular_queue_size(queue));
    ASSERT_TRUE(circular_queue_is_empty(queue));
    ASSERT_FALSE(circular_queue_is_full(queue));
    circular_queue_destroy(queue);
}

TEST(circular_queue_enqueue_dequeue) {
    CircularQueue *queue = circular_queue_create(3);

    ASSERT_TRUE(circular_queue_enqueue(queue, 10));
    ASSERT_TRUE(circular_queue_enqueue(queue, 20));
    ASSERT_TRUE(circular_queue_enqueue(queue, 30));

    ASSERT_TRUE(circular_queue_is_full(queue));
    ASSERT_FALSE(circular_queue_enqueue(queue, 40)); /* Should fail - full */

    int value;
    circular_queue_dequeue(queue, &value);
    ASSERT_EQ(10, value);

    ASSERT_TRUE(circular_queue_enqueue(queue, 40)); /* Now should work */

    circular_queue_destroy(queue);
}

TEST(circular_queue_front_rear) {
    CircularQueue *queue = circular_queue_create(5);
    circular_queue_enqueue(queue, 10);
    circular_queue_enqueue(queue, 20);
    circular_queue_enqueue(queue, 30);

    int front, rear;
    circular_queue_front(queue, &front);
    circular_queue_rear(queue, &rear);

    ASSERT_EQ(10, front);
    ASSERT_EQ(30, rear);

    circular_queue_destroy(queue);
}

TEST(circular_queue_wraparound) {
    CircularQueue *queue = circular_queue_create(3);

    circular_queue_enqueue(queue, 1);
    circular_queue_enqueue(queue, 2);
    circular_queue_enqueue(queue, 3);

    int value;
    circular_queue_dequeue(queue, &value);
    circular_queue_dequeue(queue, &value);

    circular_queue_enqueue(queue, 4);
    circular_queue_enqueue(queue, 5);

    /* Queue should now contain 3, 4, 5 */
    circular_queue_dequeue(queue, &value);
    ASSERT_EQ(3, value);
    circular_queue_dequeue(queue, &value);
    ASSERT_EQ(4, value);
    circular_queue_dequeue(queue, &value);
    ASSERT_EQ(5, value);

    circular_queue_destroy(queue);
}

/* ============== Deque Tests ============== */

TEST(deque_create_empty) {
    Deque *deque = deque_create();
    ASSERT_NOT_NULL(deque);
    ASSERT_EQ(0, deque_size(deque));
    ASSERT_TRUE(deque_is_empty(deque));
    deque_destroy(deque);
}

TEST(deque_push_front) {
    Deque *deque = deque_create();
    deque_push_front(deque, 30);
    deque_push_front(deque, 20);
    deque_push_front(deque, 10);

    ASSERT_EQ(3, deque_size(deque));

    int front, back;
    deque_front(deque, &front);
    deque_back(deque, &back);
    ASSERT_EQ(10, front);
    ASSERT_EQ(30, back);
    deque_destroy(deque);
}

TEST(deque_push_back) {
    Deque *deque = deque_create();
    deque_push_back(deque, 10);
    deque_push_back(deque, 20);
    deque_push_back(deque, 30);

    ASSERT_EQ(3, deque_size(deque));

    int front, back;
    deque_front(deque, &front);
    deque_back(deque, &back);
    ASSERT_EQ(10, front);
    ASSERT_EQ(30, back);
    deque_destroy(deque);
}

TEST(deque_pop_front) {
    Deque *deque = deque_create();
    deque_push_back(deque, 10);
    deque_push_back(deque, 20);
    deque_push_back(deque, 30);

    int value;
    deque_pop_front(deque, &value);
    ASSERT_EQ(10, value);
    deque_pop_front(deque, &value);
    ASSERT_EQ(20, value);
    deque_pop_front(deque, &value);
    ASSERT_EQ(30, value);

    ASSERT_TRUE(deque_is_empty(deque));
    deque_destroy(deque);
}

TEST(deque_pop_back) {
    Deque *deque = deque_create();
    deque_push_back(deque, 10);
    deque_push_back(deque, 20);
    deque_push_back(deque, 30);

    int value;
    deque_pop_back(deque, &value);
    ASSERT_EQ(30, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(20, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(10, value);

    ASSERT_TRUE(deque_is_empty(deque));
    deque_destroy(deque);
}

TEST(deque_mixed_ops) {
    Deque *deque = deque_create();

    deque_push_back(deque, 1);
    deque_push_front(deque, 2);
    deque_push_back(deque, 3);
    deque_push_front(deque, 4);

    /* Deque should be: 4, 2, 1, 3 */
    int value;
    deque_pop_front(deque, &value);
    ASSERT_EQ(4, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(3, value);
    deque_pop_front(deque, &value);
    ASSERT_EQ(2, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(1, value);

    deque_destroy(deque);
}

TEST(deque_get) {
    Deque *deque = deque_create();
    deque_push_back(deque, 10);
    deque_push_back(deque, 20);
    deque_push_back(deque, 30);

    int value;
    ASSERT_TRUE(deque_get(deque, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_TRUE(deque_get(deque, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_TRUE(deque_get(deque, 2, &value));
    ASSERT_EQ(30, value);
    ASSERT_FALSE(deque_get(deque, 3, &value));

    deque_destroy(deque);
}

TEST(deque_clear) {
    Deque *deque = deque_create();
    deque_push_back(deque, 10);
    deque_push_back(deque, 20);

    deque_clear(deque);
    ASSERT_EQ(0, deque_size(deque));
    ASSERT_TRUE(deque_is_empty(deque));
    deque_destroy(deque);
}

TEST(deque_resize) {
    Deque *deque = deque_create_with_capacity(2);
    deque_push_back(deque, 1);
    deque_push_back(deque, 2);
    deque_push_front(deque, 0); /* Triggers resize */

    ASSERT_EQ(3, deque_size(deque));

    int value;
    deque_pop_front(deque, &value);
    ASSERT_EQ(0, value);
    deque_destroy(deque);
}

TEST(deque_null_safety) {
    ASSERT_EQ(0, deque_size(NULL));
    ASSERT_TRUE(deque_is_empty(NULL));
    ASSERT_FALSE(deque_push_front(NULL, 42));
    ASSERT_FALSE(deque_push_back(NULL, 42));
    ASSERT_FALSE(deque_pop_front(NULL, NULL));
    ASSERT_FALSE(deque_pop_back(NULL, NULL));
}

TEST(deque_as_stack) {
    /* Deque can be used as a stack (LIFO) */
    Deque *deque = deque_create();

    deque_push_back(deque, 1);
    deque_push_back(deque, 2);
    deque_push_back(deque, 3);

    int value;
    deque_pop_back(deque, &value);
    ASSERT_EQ(3, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(2, value);
    deque_pop_back(deque, &value);
    ASSERT_EQ(1, value);

    deque_destroy(deque);
}

TEST(deque_as_queue) {
    /* Deque can be used as a queue (FIFO) */
    Deque *deque = deque_create();

    deque_push_back(deque, 1);
    deque_push_back(deque, 2);
    deque_push_back(deque, 3);

    int value;
    deque_pop_front(deque, &value);
    ASSERT_EQ(1, value);
    deque_pop_front(deque, &value);
    ASSERT_EQ(2, value);
    deque_pop_front(deque, &value);
    ASSERT_EQ(3, value);

    deque_destroy(deque);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Queue and Deque");

    /* Queue tests */
    RUN_TEST(queue_create_empty);
    RUN_TEST(queue_enqueue_single);
    RUN_TEST(queue_enqueue_multiple);
    RUN_TEST(queue_dequeue_single);
    RUN_TEST(queue_dequeue_multiple);
    RUN_TEST(queue_dequeue_empty);
    RUN_TEST(queue_fifo_order);
    RUN_TEST(queue_interleaved_ops);
    RUN_TEST(queue_clear);
    RUN_TEST(queue_resize);
    RUN_TEST(queue_wraparound);
    RUN_TEST(queue_null_safety);

    /* Circular Queue tests */
    RUN_TEST(circular_queue_create);
    RUN_TEST(circular_queue_enqueue_dequeue);
    RUN_TEST(circular_queue_front_rear);
    RUN_TEST(circular_queue_wraparound);

    /* Deque tests */
    RUN_TEST(deque_create_empty);
    RUN_TEST(deque_push_front);
    RUN_TEST(deque_push_back);
    RUN_TEST(deque_pop_front);
    RUN_TEST(deque_pop_back);
    RUN_TEST(deque_mixed_ops);
    RUN_TEST(deque_get);
    RUN_TEST(deque_clear);
    RUN_TEST(deque_resize);
    RUN_TEST(deque_null_safety);
    RUN_TEST(deque_as_stack);
    RUN_TEST(deque_as_queue);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
