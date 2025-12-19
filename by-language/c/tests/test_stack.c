/**
 * Tests for Stack, MinStack, and MaxStack
 */

#include "test_framework.h"
#include "../data-structures/stack.h"

/* ============== Basic Stack Tests ============== */

TEST(stack_create_empty) {
    Stack *stack = stack_create();
    ASSERT_NOT_NULL(stack);
    ASSERT_EQ(0, stack_size(stack));
    ASSERT_TRUE(stack_is_empty(stack));
    stack_destroy(stack);
}

TEST(stack_push_single) {
    Stack *stack = stack_create();
    ASSERT_TRUE(stack_push(stack, 42));
    ASSERT_EQ(1, stack_size(stack));
    ASSERT_FALSE(stack_is_empty(stack));

    int value;
    ASSERT_TRUE(stack_peek(stack, &value));
    ASSERT_EQ(42, value);
    stack_destroy(stack);
}

TEST(stack_push_multiple) {
    Stack *stack = stack_create();
    stack_push(stack, 10);
    stack_push(stack, 20);
    stack_push(stack, 30);

    ASSERT_EQ(3, stack_size(stack));

    int value;
    stack_peek(stack, &value);
    ASSERT_EQ(30, value); /* Top should be 30 */
    stack_destroy(stack);
}

TEST(stack_pop_single) {
    Stack *stack = stack_create();
    stack_push(stack, 42);

    int value;
    ASSERT_TRUE(stack_pop(stack, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, stack_size(stack));
    ASSERT_TRUE(stack_is_empty(stack));
    stack_destroy(stack);
}

TEST(stack_pop_multiple) {
    Stack *stack = stack_create();
    stack_push(stack, 10);
    stack_push(stack, 20);
    stack_push(stack, 30);

    int value;
    stack_pop(stack, &value);
    ASSERT_EQ(30, value);
    stack_pop(stack, &value);
    ASSERT_EQ(20, value);
    stack_pop(stack, &value);
    ASSERT_EQ(10, value);

    ASSERT_TRUE(stack_is_empty(stack));
    stack_destroy(stack);
}

TEST(stack_pop_empty) {
    Stack *stack = stack_create();
    int value;
    ASSERT_FALSE(stack_pop(stack, &value));
    stack_destroy(stack);
}

TEST(stack_peek_empty) {
    Stack *stack = stack_create();
    int value;
    ASSERT_FALSE(stack_peek(stack, &value));
    stack_destroy(stack);
}

TEST(stack_lifo_order) {
    Stack *stack = stack_create();

    /* Push 1, 2, 3, 4, 5 */
    for (int i = 1; i <= 5; i++) {
        stack_push(stack, i);
    }

    /* Pop should return 5, 4, 3, 2, 1 */
    int value;
    for (int i = 5; i >= 1; i--) {
        stack_pop(stack, &value);
        ASSERT_EQ(i, value);
    }

    stack_destroy(stack);
}

TEST(stack_clear) {
    Stack *stack = stack_create();
    stack_push(stack, 10);
    stack_push(stack, 20);
    stack_push(stack, 30);

    stack_clear(stack);
    ASSERT_EQ(0, stack_size(stack));
    ASSERT_TRUE(stack_is_empty(stack));
    stack_destroy(stack);
}

TEST(stack_resize_grow) {
    Stack *stack = stack_create_with_capacity(2);
    stack_push(stack, 1);
    stack_push(stack, 2);
    stack_push(stack, 3); /* Should trigger resize */

    ASSERT_EQ(3, stack_size(stack));

    int value;
    stack_pop(stack, &value);
    ASSERT_EQ(3, value);
    stack_destroy(stack);
}

TEST(stack_null_safety) {
    ASSERT_EQ(0, stack_size(NULL));
    ASSERT_TRUE(stack_is_empty(NULL));
    ASSERT_FALSE(stack_push(NULL, 42));
    ASSERT_FALSE(stack_pop(NULL, NULL));
    ASSERT_FALSE(stack_peek(NULL, NULL));
}

/* ============== Min Stack Tests ============== */

TEST(min_stack_create_empty) {
    MinStack *stack = min_stack_create();
    ASSERT_NOT_NULL(stack);
    ASSERT_EQ(0, min_stack_size(stack));
    ASSERT_TRUE(min_stack_is_empty(stack));
    min_stack_destroy(stack);
}

TEST(min_stack_push_pop) {
    MinStack *stack = min_stack_create();
    min_stack_push(stack, 5);
    min_stack_push(stack, 3);
    min_stack_push(stack, 7);

    ASSERT_EQ(3, min_stack_size(stack));

    int value;
    min_stack_pop(stack, &value);
    ASSERT_EQ(7, value);
    ASSERT_EQ(2, min_stack_size(stack));
    min_stack_destroy(stack);
}

TEST(min_stack_get_min_simple) {
    MinStack *stack = min_stack_create();
    min_stack_push(stack, 5);

    int min;
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(5, min);
    min_stack_destroy(stack);
}

TEST(min_stack_get_min_decreasing) {
    MinStack *stack = min_stack_create();
    min_stack_push(stack, 5);
    min_stack_push(stack, 3);
    min_stack_push(stack, 1);

    int min;
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(1, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(3, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(5, min);

    min_stack_destroy(stack);
}

TEST(min_stack_get_min_increasing) {
    MinStack *stack = min_stack_create();
    min_stack_push(stack, 1);
    min_stack_push(stack, 3);
    min_stack_push(stack, 5);

    int min;
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(1, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(1, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(1, min);

    min_stack_destroy(stack);
}

TEST(min_stack_get_min_mixed) {
    MinStack *stack = min_stack_create();
    min_stack_push(stack, 2);
    min_stack_push(stack, 0);
    min_stack_push(stack, 3);
    min_stack_push(stack, 0);

    int min;
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(0, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(0, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(0, min);

    min_stack_pop(stack, NULL);
    ASSERT_TRUE(min_stack_get_min(stack, &min));
    ASSERT_EQ(2, min);

    min_stack_destroy(stack);
}

TEST(min_stack_empty_get_min) {
    MinStack *stack = min_stack_create();
    int min;
    ASSERT_FALSE(min_stack_get_min(stack, &min));
    min_stack_destroy(stack);
}

/* ============== Max Stack Tests ============== */

TEST(max_stack_create_empty) {
    MaxStack *stack = max_stack_create();
    ASSERT_NOT_NULL(stack);
    ASSERT_EQ(0, max_stack_size(stack));
    ASSERT_TRUE(max_stack_is_empty(stack));
    max_stack_destroy(stack);
}

TEST(max_stack_push_pop) {
    MaxStack *stack = max_stack_create();
    max_stack_push(stack, 5);
    max_stack_push(stack, 3);
    max_stack_push(stack, 7);

    ASSERT_EQ(3, max_stack_size(stack));

    int value;
    max_stack_pop(stack, &value);
    ASSERT_EQ(7, value);
    ASSERT_EQ(2, max_stack_size(stack));
    max_stack_destroy(stack);
}

TEST(max_stack_get_max_simple) {
    MaxStack *stack = max_stack_create();
    max_stack_push(stack, 5);

    int max;
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);
    max_stack_destroy(stack);
}

TEST(max_stack_get_max_increasing) {
    MaxStack *stack = max_stack_create();
    max_stack_push(stack, 1);
    max_stack_push(stack, 3);
    max_stack_push(stack, 5);

    int max;
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(3, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(1, max);

    max_stack_destroy(stack);
}

TEST(max_stack_get_max_decreasing) {
    MaxStack *stack = max_stack_create();
    max_stack_push(stack, 5);
    max_stack_push(stack, 3);
    max_stack_push(stack, 1);

    int max;
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_destroy(stack);
}

TEST(max_stack_get_max_mixed) {
    MaxStack *stack = max_stack_create();
    max_stack_push(stack, 2);
    max_stack_push(stack, 5);
    max_stack_push(stack, 3);
    max_stack_push(stack, 5);

    int max;
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(5, max);

    max_stack_pop(stack, NULL);
    ASSERT_TRUE(max_stack_get_max(stack, &max));
    ASSERT_EQ(2, max);

    max_stack_destroy(stack);
}

TEST(max_stack_empty_get_max) {
    MaxStack *stack = max_stack_create();
    int max;
    ASSERT_FALSE(max_stack_get_max(stack, &max));
    max_stack_destroy(stack);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Stack");

    /* Basic Stack tests */
    RUN_TEST(stack_create_empty);
    RUN_TEST(stack_push_single);
    RUN_TEST(stack_push_multiple);
    RUN_TEST(stack_pop_single);
    RUN_TEST(stack_pop_multiple);
    RUN_TEST(stack_pop_empty);
    RUN_TEST(stack_peek_empty);
    RUN_TEST(stack_lifo_order);
    RUN_TEST(stack_clear);
    RUN_TEST(stack_resize_grow);
    RUN_TEST(stack_null_safety);

    /* Min Stack tests */
    RUN_TEST(min_stack_create_empty);
    RUN_TEST(min_stack_push_pop);
    RUN_TEST(min_stack_get_min_simple);
    RUN_TEST(min_stack_get_min_decreasing);
    RUN_TEST(min_stack_get_min_increasing);
    RUN_TEST(min_stack_get_min_mixed);
    RUN_TEST(min_stack_empty_get_min);

    /* Max Stack tests */
    RUN_TEST(max_stack_create_empty);
    RUN_TEST(max_stack_push_pop);
    RUN_TEST(max_stack_get_max_simple);
    RUN_TEST(max_stack_get_max_increasing);
    RUN_TEST(max_stack_get_max_decreasing);
    RUN_TEST(max_stack_get_max_mixed);
    RUN_TEST(max_stack_empty_get_max);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
