/**
 * Tests for Doubly Linked List
 */

#include "test_framework.h"
#include "../data-structures/doubly_linked_list.h"

/* Test creation */
TEST(dll_create_empty) {
    DoublyLinkedList *list = dll_create();
    ASSERT_NOT_NULL(list);
    ASSERT_EQ(0, dll_size(list));
    ASSERT_TRUE(dll_is_empty(list));
    dll_destroy(list);
}

/* Test push_front */
TEST(dll_push_front_single) {
    DoublyLinkedList *list = dll_create();
    ASSERT_TRUE(dll_push_front(list, 42));
    ASSERT_EQ(1, dll_size(list));

    int value;
    ASSERT_TRUE(dll_peek_front(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_TRUE(dll_peek_back(list, &value));
    ASSERT_EQ(42, value);
    dll_destroy(list);
}

TEST(dll_push_front_multiple) {
    DoublyLinkedList *list = dll_create();
    dll_push_front(list, 30);
    dll_push_front(list, 20);
    dll_push_front(list, 10);

    ASSERT_EQ(3, dll_size(list));

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    dll_get(list, 2, &value);
    ASSERT_EQ(30, value);
    dll_destroy(list);
}

/* Test push_back */
TEST(dll_push_back_single) {
    DoublyLinkedList *list = dll_create();
    ASSERT_TRUE(dll_push_back(list, 42));
    ASSERT_EQ(1, dll_size(list));

    int value;
    ASSERT_TRUE(dll_peek_front(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_TRUE(dll_peek_back(list, &value));
    ASSERT_EQ(42, value);
    dll_destroy(list);
}

TEST(dll_push_back_multiple) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    ASSERT_EQ(3, dll_size(list));

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    dll_get(list, 2, &value);
    ASSERT_EQ(30, value);
    dll_destroy(list);
}

/* Test insert */
TEST(dll_insert_at_beginning) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    ASSERT_TRUE(dll_insert(list, 0, 10));
    ASSERT_EQ(3, dll_size(list));

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    dll_destroy(list);
}

TEST(dll_insert_at_middle) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 30);

    ASSERT_TRUE(dll_insert(list, 1, 20));
    ASSERT_EQ(3, dll_size(list));

    int value;
    dll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    dll_destroy(list);
}

TEST(dll_insert_at_end) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);

    ASSERT_TRUE(dll_insert(list, 2, 30));
    ASSERT_EQ(3, dll_size(list));

    int value;
    dll_peek_back(list, &value);
    ASSERT_EQ(30, value);
    dll_destroy(list);
}

TEST(dll_insert_invalid_index) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    ASSERT_FALSE(dll_insert(list, 5, 20));
    dll_destroy(list);
}

/* Test pop_front */
TEST(dll_pop_front_single) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 42);

    int value;
    ASSERT_TRUE(dll_pop_front(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, dll_size(list));
    ASSERT_TRUE(dll_is_empty(list));
    dll_destroy(list);
}

TEST(dll_pop_front_multiple) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    dll_pop_front(list, &value);
    ASSERT_EQ(10, value);
    dll_pop_front(list, &value);
    ASSERT_EQ(20, value);
    ASSERT_EQ(1, dll_size(list));
    dll_destroy(list);
}

TEST(dll_pop_front_empty) {
    DoublyLinkedList *list = dll_create();
    int value;
    ASSERT_FALSE(dll_pop_front(list, &value));
    dll_destroy(list);
}

/* Test pop_back */
TEST(dll_pop_back_single) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 42);

    int value;
    ASSERT_TRUE(dll_pop_back(list, &value));
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, dll_size(list));
    dll_destroy(list);
}

TEST(dll_pop_back_multiple) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    dll_pop_back(list, &value);
    ASSERT_EQ(30, value);
    dll_pop_back(list, &value);
    ASSERT_EQ(20, value);
    ASSERT_EQ(1, dll_size(list));
    dll_destroy(list);
}

TEST(dll_pop_back_empty) {
    DoublyLinkedList *list = dll_create();
    int value;
    ASSERT_FALSE(dll_pop_back(list, &value));
    dll_destroy(list);
}

/* Test remove */
TEST(dll_remove_at_beginning) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    ASSERT_TRUE(dll_remove(list, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_EQ(2, dll_size(list));
    dll_destroy(list);
}

TEST(dll_remove_at_middle) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    ASSERT_TRUE(dll_remove(list, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_EQ(2, dll_size(list));
    dll_destroy(list);
}

TEST(dll_remove_at_end) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    ASSERT_TRUE(dll_remove(list, 2, &value));
    ASSERT_EQ(30, value);
    ASSERT_EQ(2, dll_size(list));
    dll_destroy(list);
}

TEST(dll_remove_invalid_index) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    int value;
    ASSERT_FALSE(dll_remove(list, 5, &value));
    dll_destroy(list);
}

/* Test remove_node (O(1) deletion) */
TEST(dll_remove_node_middle) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    DLLNode *node = dll_get_node(list, 1);
    int value;
    ASSERT_TRUE(dll_remove_node(list, node, &value));
    ASSERT_EQ(20, value);
    ASSERT_EQ(2, dll_size(list));

    /* Verify list integrity */
    dll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(30, value);
    dll_destroy(list);
}

/* Test remove_value */
TEST(dll_remove_value_exists) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    ASSERT_TRUE(dll_remove_value(list, 20));
    ASSERT_EQ(2, dll_size(list));
    ASSERT_FALSE(dll_contains(list, 20));
    dll_destroy(list);
}

TEST(dll_remove_value_not_exists) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    ASSERT_FALSE(dll_remove_value(list, 20));
    ASSERT_EQ(1, dll_size(list));
    dll_destroy(list);
}

/* Test get and set */
TEST(dll_get_valid_indices) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    int value;
    ASSERT_TRUE(dll_get(list, 0, &value));
    ASSERT_EQ(10, value);
    ASSERT_TRUE(dll_get(list, 1, &value));
    ASSERT_EQ(20, value);
    ASSERT_TRUE(dll_get(list, 2, &value));
    ASSERT_EQ(30, value);
    dll_destroy(list);
}

TEST(dll_get_invalid_index) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    int value;
    ASSERT_FALSE(dll_get(list, 1, &value));
    ASSERT_FALSE(dll_get(list, 100, &value));
    dll_destroy(list);
}

TEST(dll_set_valid_index) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    ASSERT_TRUE(dll_set(list, 0, 42));

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(42, value);
    dll_destroy(list);
}

/* Test find and contains */
TEST(dll_find_existing) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    ASSERT_EQ(0, dll_find(list, 10));
    ASSERT_EQ(1, dll_find(list, 20));
    ASSERT_EQ(2, dll_find(list, 30));
    dll_destroy(list);
}

TEST(dll_find_not_existing) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);

    ASSERT_EQ(-1, dll_find(list, 20));
    dll_destroy(list);
}

TEST(dll_contains_test) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);

    ASSERT_TRUE(dll_contains(list, 10));
    ASSERT_TRUE(dll_contains(list, 20));
    ASSERT_FALSE(dll_contains(list, 30));
    dll_destroy(list);
}

/* Test clear */
TEST(dll_clear_list) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    dll_clear(list);
    ASSERT_EQ(0, dll_size(list));
    ASSERT_TRUE(dll_is_empty(list));
    dll_destroy(list);
}

/* Test reverse */
TEST(dll_reverse_list) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    dll_reverse(list);

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(30, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(20, value);
    dll_get(list, 2, &value);
    ASSERT_EQ(10, value);
    dll_destroy(list);
}

TEST(dll_reverse_single) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 42);

    dll_reverse(list);

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(42, value);
    dll_destroy(list);
}

/* Test move_to_front */
TEST(dll_move_to_front_test) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    DLLNode *node = dll_get_node(list, 2); /* Node with 30 */
    dll_move_to_front(list, node);

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(30, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(10, value);
    dll_get(list, 2, &value);
    ASSERT_EQ(20, value);
    dll_destroy(list);
}

TEST(dll_move_to_front_already_front) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);

    DLLNode *node = dll_get_node(list, 0);
    dll_move_to_front(list, node);

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(10, value);
    dll_destroy(list);
}

/* Test move_to_back */
TEST(dll_move_to_back_test) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    DLLNode *node = dll_get_node(list, 0); /* Node with 10 */
    dll_move_to_back(list, node);

    int value;
    dll_get(list, 0, &value);
    ASSERT_EQ(20, value);
    dll_get(list, 1, &value);
    ASSERT_EQ(30, value);
    dll_get(list, 2, &value);
    ASSERT_EQ(10, value);
    dll_destroy(list);
}

/* Test bidirectional traversal */
TEST(dll_bidirectional_traversal) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    /* Forward traversal */
    DLLNode *current = list->head;
    ASSERT_EQ(10, current->data);
    current = current->next;
    ASSERT_EQ(20, current->data);
    current = current->next;
    ASSERT_EQ(30, current->data);

    /* Backward traversal from tail */
    current = list->tail;
    ASSERT_EQ(30, current->data);
    current = current->prev;
    ASSERT_EQ(20, current->data);
    current = current->prev;
    ASSERT_EQ(10, current->data);

    dll_destroy(list);
}

/* Test null safety */
TEST(dll_null_safety) {
    ASSERT_EQ(0, dll_size(NULL));
    ASSERT_TRUE(dll_is_empty(NULL));
    ASSERT_FALSE(dll_push_front(NULL, 42));
    ASSERT_FALSE(dll_push_back(NULL, 42));
    ASSERT_FALSE(dll_pop_front(NULL, NULL));
    ASSERT_FALSE(dll_pop_back(NULL, NULL));
    ASSERT_EQ(-1, dll_find(NULL, 42));
    ASSERT_FALSE(dll_contains(NULL, 42));
}

/* Test pointer integrity after operations */
TEST(dll_pointer_integrity) {
    DoublyLinkedList *list = dll_create();
    dll_push_back(list, 10);
    dll_push_back(list, 20);
    dll_push_back(list, 30);

    /* Verify prev/next pointers */
    ASSERT_NULL(list->head->prev);
    ASSERT_NOT_NULL(list->head->next);
    ASSERT_NOT_NULL(list->tail->prev);
    ASSERT_NULL(list->tail->next);

    DLLNode *middle = dll_get_node(list, 1);
    ASSERT_EQ(list->head, middle->prev);
    ASSERT_EQ(list->tail, middle->next);

    dll_destroy(list);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Doubly Linked List");

    /* Creation tests */
    RUN_TEST(dll_create_empty);

    /* Push front tests */
    RUN_TEST(dll_push_front_single);
    RUN_TEST(dll_push_front_multiple);

    /* Push back tests */
    RUN_TEST(dll_push_back_single);
    RUN_TEST(dll_push_back_multiple);

    /* Insert tests */
    RUN_TEST(dll_insert_at_beginning);
    RUN_TEST(dll_insert_at_middle);
    RUN_TEST(dll_insert_at_end);
    RUN_TEST(dll_insert_invalid_index);

    /* Pop front tests */
    RUN_TEST(dll_pop_front_single);
    RUN_TEST(dll_pop_front_multiple);
    RUN_TEST(dll_pop_front_empty);

    /* Pop back tests */
    RUN_TEST(dll_pop_back_single);
    RUN_TEST(dll_pop_back_multiple);
    RUN_TEST(dll_pop_back_empty);

    /* Remove tests */
    RUN_TEST(dll_remove_at_beginning);
    RUN_TEST(dll_remove_at_middle);
    RUN_TEST(dll_remove_at_end);
    RUN_TEST(dll_remove_invalid_index);

    /* Remove node tests */
    RUN_TEST(dll_remove_node_middle);

    /* Remove value tests */
    RUN_TEST(dll_remove_value_exists);
    RUN_TEST(dll_remove_value_not_exists);

    /* Get and set tests */
    RUN_TEST(dll_get_valid_indices);
    RUN_TEST(dll_get_invalid_index);
    RUN_TEST(dll_set_valid_index);

    /* Find and contains tests */
    RUN_TEST(dll_find_existing);
    RUN_TEST(dll_find_not_existing);
    RUN_TEST(dll_contains_test);

    /* Clear test */
    RUN_TEST(dll_clear_list);

    /* Reverse tests */
    RUN_TEST(dll_reverse_list);
    RUN_TEST(dll_reverse_single);

    /* Move to front tests */
    RUN_TEST(dll_move_to_front_test);
    RUN_TEST(dll_move_to_front_already_front);

    /* Move to back test */
    RUN_TEST(dll_move_to_back_test);

    /* Bidirectional traversal test */
    RUN_TEST(dll_bidirectional_traversal);

    /* Null safety test */
    RUN_TEST(dll_null_safety);

    /* Pointer integrity test */
    RUN_TEST(dll_pointer_integrity);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
