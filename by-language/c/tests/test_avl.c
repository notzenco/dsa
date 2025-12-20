/**
 * Tests for AVL Tree
 */

#include "test_framework.h"
#include "../data-structures/avl_tree.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(avl_create_empty) {
    AVLTree *tree = avl_create();
    ASSERT_NOT_NULL(tree);
    ASSERT_EQ(0, avl_size(tree));
    ASSERT_TRUE(avl_is_empty(tree));
    ASSERT_EQ(-1, avl_height(tree));
    avl_destroy(tree);
}

/* ============== Insert Tests ============== */

TEST(avl_insert_single) {
    AVLTree *tree = avl_create();
    ASSERT_TRUE(avl_insert(tree, 10));
    ASSERT_EQ(1, avl_size(tree));
    ASSERT_TRUE(avl_search(tree, 10));
    ASSERT_EQ(0, avl_height(tree));
    avl_destroy(tree);
}

TEST(avl_insert_multiple) {
    AVLTree *tree = avl_create();
    ASSERT_TRUE(avl_insert(tree, 10));
    ASSERT_TRUE(avl_insert(tree, 5));
    ASSERT_TRUE(avl_insert(tree, 15));
    ASSERT_EQ(3, avl_size(tree));
    ASSERT_TRUE(avl_search(tree, 10));
    ASSERT_TRUE(avl_search(tree, 5));
    ASSERT_TRUE(avl_search(tree, 15));
    avl_destroy(tree);
}

TEST(avl_insert_duplicate) {
    AVLTree *tree = avl_create();
    ASSERT_TRUE(avl_insert(tree, 10));
    ASSERT_FALSE(avl_insert(tree, 10));
    ASSERT_EQ(1, avl_size(tree));
    avl_destroy(tree);
}

TEST(avl_insert_left_left_rotation) {
    AVLTree *tree = avl_create();
    /* Insert in decreasing order to trigger LL rotation */
    avl_insert(tree, 30);
    avl_insert(tree, 20);
    avl_insert(tree, 10);

    /* After rotation: 20 should be root */
    ASSERT_EQ(3, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(1, avl_height(tree));
    avl_destroy(tree);
}

TEST(avl_insert_right_right_rotation) {
    AVLTree *tree = avl_create();
    /* Insert in increasing order to trigger RR rotation */
    avl_insert(tree, 10);
    avl_insert(tree, 20);
    avl_insert(tree, 30);

    ASSERT_EQ(3, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(1, avl_height(tree));
    avl_destroy(tree);
}

TEST(avl_insert_left_right_rotation) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 30);
    avl_insert(tree, 10);
    avl_insert(tree, 20);

    ASSERT_EQ(3, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(1, avl_height(tree));
    avl_destroy(tree);
}

TEST(avl_insert_right_left_rotation) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 30);
    avl_insert(tree, 20);

    ASSERT_EQ(3, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));
    ASSERT_EQ(1, avl_height(tree));
    avl_destroy(tree);
}

TEST(avl_insert_many_balanced) {
    AVLTree *tree = avl_create();

    /* Insert many elements */
    for (int i = 1; i <= 100; i++) {
        ASSERT_TRUE(avl_insert(tree, i));
    }

    ASSERT_EQ(100, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));

    /* Height should be O(log n) */
    int h = avl_height(tree);
    ASSERT_TRUE(h <= 7);  /* log2(100) ~= 6.6 */

    avl_destroy(tree);
}

/* ============== Delete Tests ============== */

TEST(avl_delete_leaf) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    ASSERT_TRUE(avl_delete(tree, 5));
    ASSERT_EQ(2, avl_size(tree));
    ASSERT_FALSE(avl_search(tree, 5));
    ASSERT_TRUE(avl_is_valid(tree));
    avl_destroy(tree);
}

TEST(avl_delete_node_with_one_child) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);

    ASSERT_TRUE(avl_delete(tree, 5));
    ASSERT_FALSE(avl_search(tree, 5));
    ASSERT_TRUE(avl_search(tree, 3));
    ASSERT_TRUE(avl_is_valid(tree));
    avl_destroy(tree);
}

TEST(avl_delete_node_with_two_children) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);
    avl_insert(tree, 7);

    ASSERT_TRUE(avl_delete(tree, 5));
    ASSERT_FALSE(avl_search(tree, 5));
    ASSERT_TRUE(avl_search(tree, 3));
    ASSERT_TRUE(avl_search(tree, 7));
    ASSERT_TRUE(avl_is_valid(tree));
    avl_destroy(tree);
}

TEST(avl_delete_root) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    ASSERT_TRUE(avl_delete(tree, 10));
    ASSERT_FALSE(avl_search(tree, 10));
    ASSERT_TRUE(avl_is_valid(tree));
    avl_destroy(tree);
}

TEST(avl_delete_not_found) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);

    ASSERT_FALSE(avl_delete(tree, 5));
    ASSERT_EQ(1, avl_size(tree));
    avl_destroy(tree);
}

TEST(avl_delete_triggers_rebalance) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 20);
    avl_insert(tree, 15);
    avl_insert(tree, 25);

    ASSERT_TRUE(avl_delete(tree, 5));
    ASSERT_TRUE(avl_is_valid(tree));
    avl_destroy(tree);
}

/* ============== Search Tests ============== */

TEST(avl_search_found) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    ASSERT_TRUE(avl_search(tree, 10));
    ASSERT_TRUE(avl_search(tree, 5));
    ASSERT_TRUE(avl_search(tree, 15));
    avl_destroy(tree);
}

TEST(avl_search_not_found) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);

    ASSERT_FALSE(avl_search(tree, 5));
    ASSERT_FALSE(avl_search(tree, 15));
    avl_destroy(tree);
}

TEST(avl_contains_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);

    ASSERT_TRUE(avl_contains(tree, 10));
    ASSERT_FALSE(avl_contains(tree, 5));
    avl_destroy(tree);
}

/* ============== Min/Max Tests ============== */

TEST(avl_minimum_test) {
    AVLTree *tree = avl_create();
    int min;

    ASSERT_FALSE(avl_minimum(tree, &min));

    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);

    ASSERT_TRUE(avl_minimum(tree, &min));
    ASSERT_EQ(3, min);
    avl_destroy(tree);
}

TEST(avl_maximum_test) {
    AVLTree *tree = avl_create();
    int max;

    ASSERT_FALSE(avl_maximum(tree, &max));

    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 20);

    ASSERT_TRUE(avl_maximum(tree, &max));
    ASSERT_EQ(20, max);
    avl_destroy(tree);
}

/* ============== Traversal Tests ============== */

TEST(avl_inorder_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);
    avl_insert(tree, 7);

    int result[10];
    size_t count = avl_inorder(tree, result, 10);

    ASSERT_EQ(5, count);
    ASSERT_EQ(3, result[0]);
    ASSERT_EQ(5, result[1]);
    ASSERT_EQ(7, result[2]);
    ASSERT_EQ(10, result[3]);
    ASSERT_EQ(15, result[4]);
    avl_destroy(tree);
}

TEST(avl_preorder_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    int result[10];
    size_t count = avl_preorder(tree, result, 10);

    ASSERT_EQ(3, count);
    ASSERT_EQ(10, result[0]);  /* Root first */
    avl_destroy(tree);
}

TEST(avl_postorder_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    int result[10];
    size_t count = avl_postorder(tree, result, 10);

    ASSERT_EQ(3, count);
    ASSERT_EQ(10, result[2]);  /* Root last */
    avl_destroy(tree);
}

TEST(avl_level_order_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    int result[10];
    size_t count = avl_level_order(tree, result, 10);

    ASSERT_EQ(3, count);
    ASSERT_EQ(10, result[0]);  /* Root first */
    avl_destroy(tree);
}

/* ============== Advanced Operations Tests ============== */

TEST(avl_successor_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 12);

    int succ;
    ASSERT_TRUE(avl_successor(tree, 10, &succ));
    ASSERT_EQ(12, succ);

    ASSERT_TRUE(avl_successor(tree, 5, &succ));
    ASSERT_EQ(10, succ);

    ASSERT_FALSE(avl_successor(tree, 15, &succ));
    avl_destroy(tree);
}

TEST(avl_predecessor_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 7);

    int pred;
    ASSERT_TRUE(avl_predecessor(tree, 10, &pred));
    ASSERT_EQ(7, pred);

    ASSERT_TRUE(avl_predecessor(tree, 15, &pred));
    ASSERT_EQ(10, pred);

    ASSERT_FALSE(avl_predecessor(tree, 5, &pred));
    avl_destroy(tree);
}

TEST(avl_kth_smallest_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);
    avl_insert(tree, 7);

    int result;
    ASSERT_TRUE(avl_kth_smallest(tree, 1, &result));
    ASSERT_EQ(3, result);

    ASSERT_TRUE(avl_kth_smallest(tree, 3, &result));
    ASSERT_EQ(7, result);

    ASSERT_TRUE(avl_kth_smallest(tree, 5, &result));
    ASSERT_EQ(15, result);

    ASSERT_FALSE(avl_kth_smallest(tree, 6, &result));
    ASSERT_FALSE(avl_kth_smallest(tree, 0, &result));
    avl_destroy(tree);
}

TEST(avl_floor_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    int result;
    ASSERT_TRUE(avl_floor(tree, 10, &result));
    ASSERT_EQ(10, result);

    ASSERT_TRUE(avl_floor(tree, 7, &result));
    ASSERT_EQ(5, result);

    ASSERT_TRUE(avl_floor(tree, 12, &result));
    ASSERT_EQ(10, result);

    ASSERT_FALSE(avl_floor(tree, 3, &result));
    avl_destroy(tree);
}

TEST(avl_ceiling_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    int result;
    ASSERT_TRUE(avl_ceiling(tree, 10, &result));
    ASSERT_EQ(10, result);

    ASSERT_TRUE(avl_ceiling(tree, 7, &result));
    ASSERT_EQ(10, result);

    ASSERT_TRUE(avl_ceiling(tree, 12, &result));
    ASSERT_EQ(15, result);

    ASSERT_FALSE(avl_ceiling(tree, 20, &result));
    avl_destroy(tree);
}

TEST(avl_count_range_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);
    avl_insert(tree, 3);
    avl_insert(tree, 7);
    avl_insert(tree, 12);
    avl_insert(tree, 20);

    ASSERT_EQ(4, avl_count_range(tree, 5, 12));  /* 5, 7, 10, 12 */
    ASSERT_EQ(7, avl_count_range(tree, 0, 100));
    ASSERT_EQ(0, avl_count_range(tree, 100, 200));
    avl_destroy(tree);
}

/* ============== Clear Tests ============== */

TEST(avl_clear_test) {
    AVLTree *tree = avl_create();
    avl_insert(tree, 10);
    avl_insert(tree, 5);
    avl_insert(tree, 15);

    avl_clear(tree);

    ASSERT_EQ(0, avl_size(tree));
    ASSERT_TRUE(avl_is_empty(tree));
    ASSERT_FALSE(avl_search(tree, 10));
    avl_destroy(tree);
}

/* ============== Balance Factor Tests ============== */

TEST(avl_balance_factor_test) {
    AVLTree *tree = avl_create();

    ASSERT_EQ(0, avl_balance_factor(tree));

    avl_insert(tree, 10);
    ASSERT_EQ(0, avl_balance_factor(tree));

    avl_insert(tree, 5);
    ASSERT_TRUE(avl_balance_factor(tree) >= -1);
    ASSERT_TRUE(avl_balance_factor(tree) <= 1);
    avl_destroy(tree);
}

/* ============== NULL Safety Tests ============== */

TEST(avl_null_safety) {
    int result;
    int arr[10];

    ASSERT_FALSE(avl_insert(NULL, 10));
    ASSERT_FALSE(avl_delete(NULL, 10));
    ASSERT_FALSE(avl_search(NULL, 10));
    ASSERT_FALSE(avl_contains(NULL, 10));
    ASSERT_FALSE(avl_minimum(NULL, &result));
    ASSERT_FALSE(avl_maximum(NULL, &result));
    ASSERT_EQ(0, avl_size(NULL));
    ASSERT_TRUE(avl_is_empty(NULL));
    ASSERT_EQ(0, avl_inorder(NULL, arr, 10));
    ASSERT_TRUE(avl_is_valid(NULL));
}

/* ============== Stress Tests ============== */

TEST(avl_stress_insert_delete) {
    AVLTree *tree = avl_create();

    /* Insert 1000 elements */
    for (int i = 0; i < 1000; i++) {
        avl_insert(tree, i);
    }
    ASSERT_EQ(1000, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));

    /* Delete half */
    for (int i = 0; i < 500; i++) {
        avl_delete(tree, i);
    }
    ASSERT_EQ(500, avl_size(tree));
    ASSERT_TRUE(avl_is_valid(tree));

    /* Verify remaining elements */
    for (int i = 500; i < 1000; i++) {
        ASSERT_TRUE(avl_search(tree, i));
    }

    avl_destroy(tree);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("AVL Tree");

    /* Creation */
    RUN_TEST(avl_create_empty);

    /* Insert */
    RUN_TEST(avl_insert_single);
    RUN_TEST(avl_insert_multiple);
    RUN_TEST(avl_insert_duplicate);
    RUN_TEST(avl_insert_left_left_rotation);
    RUN_TEST(avl_insert_right_right_rotation);
    RUN_TEST(avl_insert_left_right_rotation);
    RUN_TEST(avl_insert_right_left_rotation);
    RUN_TEST(avl_insert_many_balanced);

    /* Delete */
    RUN_TEST(avl_delete_leaf);
    RUN_TEST(avl_delete_node_with_one_child);
    RUN_TEST(avl_delete_node_with_two_children);
    RUN_TEST(avl_delete_root);
    RUN_TEST(avl_delete_not_found);
    RUN_TEST(avl_delete_triggers_rebalance);

    /* Search */
    RUN_TEST(avl_search_found);
    RUN_TEST(avl_search_not_found);
    RUN_TEST(avl_contains_test);

    /* Min/Max */
    RUN_TEST(avl_minimum_test);
    RUN_TEST(avl_maximum_test);

    /* Traversal */
    RUN_TEST(avl_inorder_test);
    RUN_TEST(avl_preorder_test);
    RUN_TEST(avl_postorder_test);
    RUN_TEST(avl_level_order_test);

    /* Advanced Operations */
    RUN_TEST(avl_successor_test);
    RUN_TEST(avl_predecessor_test);
    RUN_TEST(avl_kth_smallest_test);
    RUN_TEST(avl_floor_test);
    RUN_TEST(avl_ceiling_test);
    RUN_TEST(avl_count_range_test);

    /* Clear */
    RUN_TEST(avl_clear_test);

    /* Balance Factor */
    RUN_TEST(avl_balance_factor_test);

    /* NULL Safety */
    RUN_TEST(avl_null_safety);

    /* Stress */
    RUN_TEST(avl_stress_insert_delete);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
