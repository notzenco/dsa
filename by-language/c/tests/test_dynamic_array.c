/**
 * Tests for Dynamic Array
 */

#include "test_framework.h"
#include "../data-structures/dynamic_array.h"

/* Test creation */
TEST(da_create_default) {
    DynamicArray *arr = da_create();
    ASSERT_NOT_NULL(arr);
    ASSERT_EQ(0, da_size(arr));
    ASSERT_EQ(DA_INITIAL_CAPACITY, da_capacity(arr));
    ASSERT_TRUE(da_is_empty(arr));
    da_destroy(arr);
}

TEST(da_create_with_capacity) {
    DynamicArray *arr = da_create_with_capacity(100);
    ASSERT_NOT_NULL(arr);
    ASSERT_EQ(0, da_size(arr));
    ASSERT_EQ(100, da_capacity(arr));
    da_destroy(arr);
}

/* Test push_back */
TEST(da_push_back_single) {
    DynamicArray *arr = da_create();
    ASSERT_TRUE(da_push_back(arr, 42));
    ASSERT_EQ(1, da_size(arr));

    int value;
    ASSERT_TRUE(da_get(arr, 0, &value));
    ASSERT_EQ(42, value);
    da_destroy(arr);
}

TEST(da_push_back_multiple) {
    DynamicArray *arr = da_create();
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(da_push_back(arr, i * 10));
    }
    ASSERT_EQ(10, da_size(arr));

    int value;
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(da_get(arr, i, &value));
        ASSERT_EQ(i * 10, value);
    }
    da_destroy(arr);
}

TEST(da_push_back_resize) {
    DynamicArray *arr = da_create_with_capacity(2);
    ASSERT_TRUE(da_push_back(arr, 1));
    ASSERT_TRUE(da_push_back(arr, 2));
    ASSERT_EQ(2, da_capacity(arr));

    /* This should trigger a resize */
    ASSERT_TRUE(da_push_back(arr, 3));
    ASSERT_EQ(4, da_capacity(arr));
    ASSERT_EQ(3, da_size(arr));
    da_destroy(arr);
}

/* Test pop_back */
TEST(da_pop_back_single) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 42);

    int value;
    ASSERT_TRUE(da_pop_back(arr, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, da_size(arr));
    da_destroy(arr);
}

TEST(da_pop_back_empty) {
    DynamicArray *arr = da_create();
    int value;
    ASSERT_FALSE(da_pop_back(arr, &value));
    da_destroy(arr);
}

TEST(da_pop_back_null_out) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 42);
    ASSERT_TRUE(da_pop_back(arr, NULL));
    ASSERT_EQ(0, da_size(arr));
    da_destroy(arr);
}

/* Test get and set */
TEST(da_get_valid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    int value;
    ASSERT_TRUE(da_get(arr, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_TRUE(da_get(arr, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_TRUE(da_get(arr, 2, &value));
    ASSERT_EQ(30, value);
    da_destroy(arr);
}

TEST(da_get_invalid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);

    int value;
    ASSERT_FALSE(da_get(arr, 1, &value));
    ASSERT_FALSE(da_get(arr, 100, &value));
    da_destroy(arr);
}

TEST(da_set_valid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);

    ASSERT_TRUE(da_set(arr, 0, 42));

    int value;
    da_get(arr, 0, &value);
    ASSERT_EQ(42, value);
    da_destroy(arr);
}

TEST(da_set_invalid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);

    ASSERT_FALSE(da_set(arr, 1, 42));
    ASSERT_FALSE(da_set(arr, 100, 42));
    da_destroy(arr);
}

/* Test insert */
TEST(da_insert_at_beginning) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    ASSERT_TRUE(da_insert(arr, 0, 10));
    ASSERT_EQ(3, da_size(arr));

    int value;
    da_get(arr, 0, &value);
    ASSERT_EQ(10, value);
    da_get(arr, 1, &value);
    ASSERT_EQ(20, value);
    da_get(arr, 2, &value);
    ASSERT_EQ(30, value);
    da_destroy(arr);
}

TEST(da_insert_at_middle) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 30);

    ASSERT_TRUE(da_insert(arr, 1, 20));
    ASSERT_EQ(3, da_size(arr));

    int value;
    da_get(arr, 1, &value);
    ASSERT_EQ(20, value);
    da_destroy(arr);
}

TEST(da_insert_at_end) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);

    ASSERT_TRUE(da_insert(arr, 2, 30));
    ASSERT_EQ(3, da_size(arr));

    int value;
    da_get(arr, 2, &value);
    ASSERT_EQ(30, value);
    da_destroy(arr);
}

TEST(da_insert_invalid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);

    ASSERT_FALSE(da_insert(arr, 5, 20));
    da_destroy(arr);
}

/* Test remove */
TEST(da_remove_at_beginning) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    int value;
    ASSERT_TRUE(da_remove(arr, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_EQ(2, da_size(arr));

    da_get(arr, 0, &value);
    ASSERT_EQ(20, value);
    da_destroy(arr);
}

TEST(da_remove_at_middle) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    int value;
    ASSERT_TRUE(da_remove(arr, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_EQ(2, da_size(arr));

    da_get(arr, 1, &value);
    ASSERT_EQ(30, value);
    da_destroy(arr);
}

TEST(da_remove_at_end) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    int value;
    ASSERT_TRUE(da_remove(arr, 2, &value));
    ASSERT_EQ(30, value);
    ASSERT_EQ(2, da_size(arr));
    da_destroy(arr);
}

TEST(da_remove_invalid_index) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);

    int value;
    ASSERT_FALSE(da_remove(arr, 5, &value));
    da_destroy(arr);
}

/* Test find and contains */
TEST(da_find_existing) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    ASSERT_EQ(0, da_find(arr, 10));
    ASSERT_EQ(1, da_find(arr, 20));
    ASSERT_EQ(2, da_find(arr, 30));
    da_destroy(arr);
}

TEST(da_find_not_existing) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);

    ASSERT_EQ(-1, da_find(arr, 30));
    da_destroy(arr);
}

TEST(da_contains) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);

    ASSERT_TRUE(da_contains(arr, 10));
    ASSERT_TRUE(da_contains(arr, 20));
    ASSERT_FALSE(da_contains(arr, 30));
    da_destroy(arr);
}

/* Test clear */
TEST(da_clear_array) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    da_clear(arr);
    ASSERT_EQ(0, da_size(arr));
    ASSERT_TRUE(da_is_empty(arr));
    da_destroy(arr);
}

/* Test reserve */
TEST(da_reserve_larger) {
    DynamicArray *arr = da_create_with_capacity(10);
    ASSERT_TRUE(da_reserve(arr, 100));
    ASSERT_EQ(100, da_capacity(arr));
    da_destroy(arr);
}

TEST(da_reserve_smaller) {
    DynamicArray *arr = da_create_with_capacity(100);
    ASSERT_TRUE(da_reserve(arr, 10));
    ASSERT_EQ(100, da_capacity(arr)); /* Should not shrink */
    da_destroy(arr);
}

/* Test shrink_to_fit */
TEST(da_shrink_to_fit_test) {
    DynamicArray *arr = da_create_with_capacity(100);
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);

    ASSERT_TRUE(da_shrink_to_fit(arr));
    ASSERT_EQ(3, da_capacity(arr));
    ASSERT_EQ(3, da_size(arr));
    da_destroy(arr);
}

/* Test reverse */
TEST(da_reverse_array) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 10);
    da_push_back(arr, 20);
    da_push_back(arr, 30);
    da_push_back(arr, 40);

    da_reverse(arr);

    int value;
    da_get(arr, 0, &value);
    ASSERT_EQ(40, value);
    da_get(arr, 1, &value);
    ASSERT_EQ(30, value);
    da_get(arr, 2, &value);
    ASSERT_EQ(20, value);
    da_get(arr, 3, &value);
    ASSERT_EQ(10, value);
    da_destroy(arr);
}

TEST(da_reverse_single) {
    DynamicArray *arr = da_create();
    da_push_back(arr, 42);

    da_reverse(arr);

    int value;
    da_get(arr, 0, &value);
    ASSERT_EQ(42, value);
    da_destroy(arr);
}

TEST(da_reverse_empty) {
    DynamicArray *arr = da_create();
    da_reverse(arr); /* Should not crash */
    ASSERT_EQ(0, da_size(arr));
    da_destroy(arr);
}

/* Test null safety */
TEST(da_null_safety) {
    ASSERT_EQ(0, da_size(NULL));
    ASSERT_EQ(0, da_capacity(NULL));
    ASSERT_TRUE(da_is_empty(NULL));
    ASSERT_FALSE(da_push_back(NULL, 42));
    ASSERT_FALSE(da_pop_back(NULL, NULL));
    ASSERT_EQ(-1, da_find(NULL, 42));
    ASSERT_FALSE(da_contains(NULL, 42));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Dynamic Array");

    /* Creation tests */
    RUN_TEST(da_create_default);
    RUN_TEST(da_create_with_capacity);

    /* Push back tests */
    RUN_TEST(da_push_back_single);
    RUN_TEST(da_push_back_multiple);
    RUN_TEST(da_push_back_resize);

    /* Pop back tests */
    RUN_TEST(da_pop_back_single);
    RUN_TEST(da_pop_back_empty);
    RUN_TEST(da_pop_back_null_out);

    /* Get and set tests */
    RUN_TEST(da_get_valid_index);
    RUN_TEST(da_get_invalid_index);
    RUN_TEST(da_set_valid_index);
    RUN_TEST(da_set_invalid_index);

    /* Insert tests */
    RUN_TEST(da_insert_at_beginning);
    RUN_TEST(da_insert_at_middle);
    RUN_TEST(da_insert_at_end);
    RUN_TEST(da_insert_invalid_index);

    /* Remove tests */
    RUN_TEST(da_remove_at_beginning);
    RUN_TEST(da_remove_at_middle);
    RUN_TEST(da_remove_at_end);
    RUN_TEST(da_remove_invalid_index);

    /* Find and contains tests */
    RUN_TEST(da_find_existing);
    RUN_TEST(da_find_not_existing);
    RUN_TEST(da_contains);

    /* Clear test */
    RUN_TEST(da_clear_array);

    /* Reserve tests */
    RUN_TEST(da_reserve_larger);
    RUN_TEST(da_reserve_smaller);

    /* Shrink to fit test */
    RUN_TEST(da_shrink_to_fit_test);

    /* Reverse tests */
    RUN_TEST(da_reverse_array);
    RUN_TEST(da_reverse_single);
    RUN_TEST(da_reverse_empty);

    /* Null safety test */
    RUN_TEST(da_null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
