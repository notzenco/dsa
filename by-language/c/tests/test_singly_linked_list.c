/**
 * Tests for Singly Linked List
 */

#include "test_framework.h"
#include "../data-structures/singly_linked_list.h"
#include <stdlib.h>

/* Test creation */
TEST(sll_create_empty) {
    SinglyLinkedList *list = sll_create();
    ASSERT_NOT_NULL(list);
    ASSERT_EQ(0, sll_size(list));
    ASSERT_TRUE(sll_is_empty(list));
    sll_destroy(list);
}

/* Test push_front */
TEST(sll_push_front_single) {
    SinglyLinkedList *list = sll_create();
    ASSERT_TRUE(sll_push_front(list, 42));
    ASSERT_EQ(1, sll_size(list));

    int value;
    ASSERT_TRUE(sll_peek_front(list, &value));
    ASSERT_EQ(42, value);
    sll_destroy(list);
}

TEST(sll_push_front_multiple) {
    SinglyLinkedList *list = sll_create();
    sll_push_front(list, 30);
    sll_push_front(list, 20);
    sll_push_front(list, 10);

    ASSERT_EQ(3, sll_size(list));

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    sll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    sll_get(list, 2, &value);
    ASSERT_EQ(30, value);
    sll_destroy(list);
}

/* Test push_back */
TEST(sll_push_back_single) {
    SinglyLinkedList *list = sll_create();
    ASSERT_TRUE(sll_push_back(list, 42));
    ASSERT_EQ(1, sll_size(list));

    int value;
    ASSERT_TRUE(sll_peek_back(list, &value));
    ASSERT_EQ(42, value);
    sll_destroy(list);
}

TEST(sll_push_back_multiple) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    ASSERT_EQ(3, sll_size(list));

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    sll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    sll_get(list, 2, &value);
    ASSERT_EQ(30, value);
    sll_destroy(list);
}

/* Test insert */
TEST(sll_insert_at_beginning) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    ASSERT_TRUE(sll_insert(list, 0, 10));
    ASSERT_EQ(3, sll_size(list));

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    sll_destroy(list);
}

TEST(sll_insert_at_middle) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 30);

    ASSERT_TRUE(sll_insert(list, 1, 20));
    ASSERT_EQ(3, sll_size(list));

    int value;
    sll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    sll_destroy(list);
}

TEST(sll_insert_at_end) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);

    ASSERT_TRUE(sll_insert(list, 2, 30));
    ASSERT_EQ(3, sll_size(list));

    int value;
    sll_peek_back(list, &value);
    ASSERT_EQ(30, value);
    sll_destroy(list);
}

TEST(sll_insert_invalid_index) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    ASSERT_FALSE(sll_insert(list, 5, 20));
    sll_destroy(list);
}

/* Test pop_front */
TEST(sll_pop_front_single) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 42);

    int value;
    ASSERT_TRUE(sll_pop_front(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, sll_size(list));
    ASSERT_TRUE(sll_is_empty(list));
    sll_destroy(list);
}

TEST(sll_pop_front_multiple) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    sll_pop_front(list, &value);
    ASSERT_EQ(10, value);
    sll_pop_front(list, &value);
    ASSERT_EQ(20, value);
    ASSERT_EQ(1, sll_size(list));
    sll_destroy(list);
}

TEST(sll_pop_front_empty) {
    SinglyLinkedList *list = sll_create();
    int value;
    ASSERT_FALSE(sll_pop_front(list, &value));
    sll_destroy(list);
}

/* Test pop_back */
TEST(sll_pop_back_single) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 42);

    int value;
    ASSERT_TRUE(sll_pop_back(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, sll_size(list));
    sll_destroy(list);
}

TEST(sll_pop_back_multiple) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    sll_pop_back(list, &value);
    ASSERT_EQ(30, value);
    sll_pop_back(list, &value);
    ASSERT_EQ(20, value);
    ASSERT_EQ(1, sll_size(list));
    sll_destroy(list);
}

TEST(sll_pop_back_empty) {
    SinglyLinkedList *list = sll_create();
    int value;
    ASSERT_FALSE(sll_pop_back(list, &value));
    sll_destroy(list);
}

/* Test remove */
TEST(sll_remove_at_beginning) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    ASSERT_TRUE(sll_remove(list, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_EQ(2, sll_size(list));
    sll_destroy(list);
}

TEST(sll_remove_at_middle) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    ASSERT_TRUE(sll_remove(list, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_EQ(2, sll_size(list));
    sll_destroy(list);
}

TEST(sll_remove_at_end) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    ASSERT_TRUE(sll_remove(list, 2, &value));
    ASSERT_EQ(30, value);
    ASSERT_EQ(2, sll_size(list));
    sll_destroy(list);
}

TEST(sll_remove_invalid_index) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    int value;
    ASSERT_FALSE(sll_remove(list, 5, &value));
    sll_destroy(list);
}

/* Test remove_value */
TEST(sll_remove_value_exists) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    ASSERT_TRUE(sll_remove_value(list, 20));
    ASSERT_EQ(2, sll_size(list));
    ASSERT_FALSE(sll_contains(list, 20));
    sll_destroy(list);
}

TEST(sll_remove_value_head) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);

    ASSERT_TRUE(sll_remove_value(list, 10));
    ASSERT_EQ(1, sll_size(list));

    int value;
    sll_peek_front(list, &value);
    ASSERT_EQ(20, value);
    sll_destroy(list);
}

TEST(sll_remove_value_tail) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);

    ASSERT_TRUE(sll_remove_value(list, 20));
    ASSERT_EQ(1, sll_size(list));

    int value;
    sll_peek_back(list, &value);
    ASSERT_EQ(10, value);
    sll_destroy(list);
}

TEST(sll_remove_value_not_exists) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    ASSERT_FALSE(sll_remove_value(list, 20));
    ASSERT_EQ(1, sll_size(list));
    sll_destroy(list);
}

/* Test get and set */
TEST(sll_get_valid_indices) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    ASSERT_TRUE(sll_get(list, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_TRUE(sll_get(list, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_TRUE(sll_get(list, 2, &value));
    ASSERT_EQ(30, value);
    sll_destroy(list);
}

TEST(sll_get_invalid_index) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    int value;
    ASSERT_FALSE(sll_get(list, 1, &value));
    ASSERT_FALSE(sll_get(list, 100, &value));
    sll_destroy(list);
}

TEST(sll_set_valid_index) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    ASSERT_TRUE(sll_set(list, 0, 42));

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(42, value);
    sll_destroy(list);
}

TEST(sll_set_invalid_index) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    ASSERT_FALSE(sll_set(list, 1, 42));
    sll_destroy(list);
}

/* Test find and contains */
TEST(sll_find_existing) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    ASSERT_EQ(0, sll_find(list, 10));
    ASSERT_EQ(1, sll_find(list, 20));
    ASSERT_EQ(2, sll_find(list, 30));
    sll_destroy(list);
}

TEST(sll_find_not_existing) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);

    ASSERT_EQ(-1, sll_find(list, 20));
    sll_destroy(list);
}

TEST(sll_contains_test) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);

    ASSERT_TRUE(sll_contains(list, 10));
    ASSERT_TRUE(sll_contains(list, 20));
    ASSERT_FALSE(sll_contains(list, 30));
    sll_destroy(list);
}

/* Test clear */
TEST(sll_clear_list) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    sll_clear(list);
    ASSERT_EQ(0, sll_size(list));
    ASSERT_TRUE(sll_is_empty(list));
    sll_destroy(list);
}

/* Test reverse */
TEST(sll_reverse_list) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    sll_reverse(list);

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(30, value);
    sll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    sll_get(list, 2, &value);
    ASSERT_EQ(10, value);
    sll_destroy(list);
}

TEST(sll_reverse_single) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 42);

    sll_reverse(list);

    int value;
    sll_get(list, 0, &value);
    ASSERT_EQ(42, value);
    sll_destroy(list);
}

TEST(sll_reverse_empty) {
    SinglyLinkedList *list = sll_create();
    sll_reverse(list); /* Should not crash */
    ASSERT_EQ(0, sll_size(list));
    sll_destroy(list);
}

/* Test get_middle */
TEST(sll_get_middle_odd) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int value;
    ASSERT_TRUE(sll_get_middle(list, &value));
    ASSERT_EQ(20, value);
    sll_destroy(list);
}

TEST(sll_get_middle_even) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);
    sll_push_back(list, 40);

    int value;
    ASSERT_TRUE(sll_get_middle(list, &value));
    ASSERT_EQ(30, value); /* Second middle for even length */
    sll_destroy(list);
}

TEST(sll_get_middle_single) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 42);

    int value;
    ASSERT_TRUE(sll_get_middle(list, &value));
    ASSERT_EQ(42, value);
    sll_destroy(list);
}

/* Test has_cycle */
TEST(sll_has_cycle_no_cycle) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    ASSERT_FALSE(sll_has_cycle(list));
    sll_destroy(list);
}

/* Test to_array */
TEST(sll_to_array_test) {
    SinglyLinkedList *list = sll_create();
    sll_push_back(list, 10);
    sll_push_back(list, 20);
    sll_push_back(list, 30);

    int *arr;
    size_t size;
    ASSERT_TRUE(sll_to_array(list, &arr, &size));
    ASSERT_EQ(3, size);
    ASSERT_EQ(10, arr[0]);
    ASSERT_EQ(20, arr[1]);
    ASSERT_EQ(30, arr[2]);

    free(arr);
    sll_destroy(list);
}

TEST(sll_to_array_empty) {
    SinglyLinkedList *list = sll_create();

    int *arr;
    size_t size;
    ASSERT_TRUE(sll_to_array(list, &arr, &size));
    ASSERT_EQ(0, size);
    ASSERT_NULL(arr);
    sll_destroy(list);
}

/* Test null safety */
TEST(sll_null_safety) {
    ASSERT_EQ(0, sll_size(NULL));
    ASSERT_TRUE(sll_is_empty(NULL));
    ASSERT_FALSE(sll_push_front(NULL, 42));
    ASSERT_FALSE(sll_push_back(NULL, 42));
    ASSERT_FALSE(sll_pop_front(NULL, NULL));
    ASSERT_FALSE(sll_pop_back(NULL, NULL));
    ASSERT_EQ(-1, sll_find(NULL, 42));
    ASSERT_FALSE(sll_contains(NULL, 42));
}

/* Test tail pointer integrity */
TEST(sll_tail_integrity_push_pop) {
    SinglyLinkedList *list = sll_create();

    /* Push back should update tail */
    sll_push_back(list, 10);
    int value;
    sll_peek_back(list, &value);
    ASSERT_EQ(10, value);

    sll_push_back(list, 20);
    sll_peek_back(list, &value);
    ASSERT_EQ(20, value);

    /* Pop back should update tail */
    sll_pop_back(list, &value);
    sll_peek_back(list, &value);
    ASSERT_EQ(10, value);

    sll_destroy(list);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Singly Linked List");

    /* Creation tests */
    RUN_TEST(sll_create_empty);

    /* Push front tests */
    RUN_TEST(sll_push_front_single);
    RUN_TEST(sll_push_front_multiple);

    /* Push back tests */
    RUN_TEST(sll_push_back_single);
    RUN_TEST(sll_push_back_multiple);

    /* Insert tests */
    RUN_TEST(sll_insert_at_beginning);
    RUN_TEST(sll_insert_at_middle);
    RUN_TEST(sll_insert_at_end);
    RUN_TEST(sll_insert_invalid_index);

    /* Pop front tests */
    RUN_TEST(sll_pop_front_single);
    RUN_TEST(sll_pop_front_multiple);
    RUN_TEST(sll_pop_front_empty);

    /* Pop back tests */
    RUN_TEST(sll_pop_back_single);
    RUN_TEST(sll_pop_back_multiple);
    RUN_TEST(sll_pop_back_empty);

    /* Remove tests */
    RUN_TEST(sll_remove_at_beginning);
    RUN_TEST(sll_remove_at_middle);
    RUN_TEST(sll_remove_at_end);
    RUN_TEST(sll_remove_invalid_index);

    /* Remove value tests */
    RUN_TEST(sll_remove_value_exists);
    RUN_TEST(sll_remove_value_head);
    RUN_TEST(sll_remove_value_tail);
    RUN_TEST(sll_remove_value_not_exists);

    /* Get and set tests */
    RUN_TEST(sll_get_valid_indices);
    RUN_TEST(sll_get_invalid_index);
    RUN_TEST(sll_set_valid_index);
    RUN_TEST(sll_set_invalid_index);

    /* Find and contains tests */
    RUN_TEST(sll_find_existing);
    RUN_TEST(sll_find_not_existing);
    RUN_TEST(sll_contains_test);

    /* Clear test */
    RUN_TEST(sll_clear_list);

    /* Reverse tests */
    RUN_TEST(sll_reverse_list);
    RUN_TEST(sll_reverse_single);
    RUN_TEST(sll_reverse_empty);

    /* Middle tests */
    RUN_TEST(sll_get_middle_odd);
    RUN_TEST(sll_get_middle_even);
    RUN_TEST(sll_get_middle_single);

    /* Cycle test */
    RUN_TEST(sll_has_cycle_no_cycle);

    /* To array tests */
    RUN_TEST(sll_to_array_test);
    RUN_TEST(sll_to_array_empty);

    /* Null safety test */
    RUN_TEST(sll_null_safety);

    /* Tail integrity test */
    RUN_TEST(sll_tail_integrity_push_pop);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
