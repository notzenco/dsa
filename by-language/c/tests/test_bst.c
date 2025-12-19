/**
 * Tests for Binary Search Tree
 */

#include "test_framework.h"
#include "../data-structures/binary_search_tree.h"
#include <stdlib.h>

/* ============== Basic Tests ============== */

TEST(bst_create_empty) {
    BST *tree = bst_create();
    ASSERT_NOT_NULL(tree);
    ASSERT_EQ(0, bst_size(tree));
    ASSERT_TRUE(bst_is_empty(tree));
    bst_destroy(tree);
}

TEST(bst_insert_single) {
    BST *tree = bst_create();
    ASSERT_TRUE(bst_insert(tree, 50, 500));
    ASSERT_EQ(1, bst_size(tree));
    ASSERT_FALSE(bst_is_empty(tree));

    int value;
    ASSERT_TRUE(bst_search(tree, 50, &value));
    ASSERT_EQ(500, value);
    bst_destroy(tree);
}

TEST(bst_insert_multiple) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    ASSERT_EQ(5, bst_size(tree));
    ASSERT_TRUE(bst_contains(tree, 50));
    ASSERT_TRUE(bst_contains(tree, 30));
    ASSERT_TRUE(bst_contains(tree, 70));
    ASSERT_TRUE(bst_contains(tree, 20));
    ASSERT_TRUE(bst_contains(tree, 40));
    bst_destroy(tree);
}

TEST(bst_insert_duplicate) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 50, 999); /* Update value */

    ASSERT_EQ(1, bst_size(tree));

    int value;
    bst_search(tree, 50, &value);
    ASSERT_EQ(999, value);
    bst_destroy(tree);
}

TEST(bst_search_found) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    int value;
    ASSERT_TRUE(bst_search(tree, 30, &value));
    ASSERT_EQ(300, value);
    bst_destroy(tree);
}

TEST(bst_search_not_found) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);

    int value;
    ASSERT_FALSE(bst_search(tree, 100, &value));
    bst_destroy(tree);
}

/* ============== Delete Tests ============== */

TEST(bst_delete_leaf) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    ASSERT_TRUE(bst_delete(tree, 30));
    ASSERT_EQ(2, bst_size(tree));
    ASSERT_FALSE(bst_contains(tree, 30));
    ASSERT_TRUE(bst_contains(tree, 50));
    ASSERT_TRUE(bst_contains(tree, 70));
    bst_destroy(tree);
}

TEST(bst_delete_one_child) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 20, 200);

    ASSERT_TRUE(bst_delete(tree, 30));
    ASSERT_EQ(2, bst_size(tree));
    ASSERT_FALSE(bst_contains(tree, 30));
    ASSERT_TRUE(bst_contains(tree, 20));
    bst_destroy(tree);
}

TEST(bst_delete_two_children) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    ASSERT_TRUE(bst_delete(tree, 30));
    ASSERT_EQ(4, bst_size(tree));
    ASSERT_FALSE(bst_contains(tree, 30));
    ASSERT_TRUE(bst_contains(tree, 20));
    ASSERT_TRUE(bst_contains(tree, 40));
    ASSERT_TRUE(bst_is_valid(tree));
    bst_destroy(tree);
}

TEST(bst_delete_root) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    ASSERT_TRUE(bst_delete(tree, 50));
    ASSERT_EQ(2, bst_size(tree));
    ASSERT_FALSE(bst_contains(tree, 50));
    ASSERT_TRUE(bst_is_valid(tree));
    bst_destroy(tree);
}

TEST(bst_delete_not_found) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);

    ASSERT_FALSE(bst_delete(tree, 100));
    ASSERT_EQ(1, bst_size(tree));
    bst_destroy(tree);
}

/* ============== Min/Max Tests ============== */

TEST(bst_min_max) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 80, 800);

    int key, value;
    ASSERT_TRUE(bst_min(tree, &key, &value));
    ASSERT_EQ(20, key);
    ASSERT_EQ(200, value);

    ASSERT_TRUE(bst_max(tree, &key, &value));
    ASSERT_EQ(80, key);
    ASSERT_EQ(800, value);
    bst_destroy(tree);
}

TEST(bst_min_max_empty) {
    BST *tree = bst_create();
    int key;
    ASSERT_FALSE(bst_min(tree, &key, NULL));
    ASSERT_FALSE(bst_max(tree, &key, NULL));
    bst_destroy(tree);
}

/* ============== Height Tests ============== */

TEST(bst_height_single) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    ASSERT_EQ(1, bst_height(tree));
    bst_destroy(tree);
}

TEST(bst_height_balanced) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);
    bst_insert(tree, 60, 600);
    bst_insert(tree, 80, 800);

    ASSERT_EQ(3, bst_height(tree));
    bst_destroy(tree);
}

TEST(bst_height_degenerate) {
    BST *tree = bst_create();
    /* Insert in sorted order - creates a linked list */
    for (int i = 1; i <= 5; i++) {
        bst_insert(tree, i, i * 10);
    }
    ASSERT_EQ(5, bst_height(tree));
    bst_destroy(tree);
}

/* ============== Traversal Tests ============== */

static int inorder_result[100];
static int inorder_count;

static void inorder_visitor(int key, int value, void *user_data) {
    (void)value;
    (void)user_data;
    inorder_result[inorder_count++] = key;
}

TEST(bst_inorder_traversal) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    inorder_count = 0;
    bst_inorder(tree, inorder_visitor, NULL);

    ASSERT_EQ(5, inorder_count);
    ASSERT_EQ(20, inorder_result[0]);
    ASSERT_EQ(30, inorder_result[1]);
    ASSERT_EQ(40, inorder_result[2]);
    ASSERT_EQ(50, inorder_result[3]);
    ASSERT_EQ(70, inorder_result[4]);
    bst_destroy(tree);
}

TEST(bst_preorder_traversal) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    inorder_count = 0;
    bst_preorder(tree, inorder_visitor, NULL);

    ASSERT_EQ(3, inorder_count);
    ASSERT_EQ(50, inorder_result[0]); /* Root first */
    ASSERT_EQ(30, inorder_result[1]);
    ASSERT_EQ(70, inorder_result[2]);
    bst_destroy(tree);
}

TEST(bst_postorder_traversal) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    inorder_count = 0;
    bst_postorder(tree, inorder_visitor, NULL);

    ASSERT_EQ(3, inorder_count);
    ASSERT_EQ(30, inorder_result[0]);
    ASSERT_EQ(70, inorder_result[1]);
    ASSERT_EQ(50, inorder_result[2]); /* Root last */
    bst_destroy(tree);
}

TEST(bst_levelorder_traversal) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    inorder_count = 0;
    bst_levelorder(tree, inorder_visitor, NULL);

    ASSERT_EQ(5, inorder_count);
    ASSERT_EQ(50, inorder_result[0]); /* Level 0 */
    ASSERT_EQ(30, inorder_result[1]); /* Level 1 */
    ASSERT_EQ(70, inorder_result[2]); /* Level 1 */
    ASSERT_EQ(20, inorder_result[3]); /* Level 2 */
    ASSERT_EQ(40, inorder_result[4]); /* Level 2 */
    bst_destroy(tree);
}

/* ============== Successor/Predecessor Tests ============== */

TEST(bst_successor_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    int succ;
    ASSERT_TRUE(bst_successor(tree, 30, &succ));
    ASSERT_EQ(40, succ);

    ASSERT_TRUE(bst_successor(tree, 40, &succ));
    ASSERT_EQ(50, succ);

    ASSERT_FALSE(bst_successor(tree, 70, &succ)); /* No successor */
    bst_destroy(tree);
}

TEST(bst_predecessor_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    int pred;
    ASSERT_TRUE(bst_predecessor(tree, 50, &pred));
    ASSERT_EQ(40, pred);

    ASSERT_TRUE(bst_predecessor(tree, 30, &pred));
    ASSERT_EQ(20, pred);

    ASSERT_FALSE(bst_predecessor(tree, 20, &pred)); /* No predecessor */
    bst_destroy(tree);
}

/* ============== Kth Smallest Test ============== */

TEST(bst_kth_smallest_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    int key;
    ASSERT_TRUE(bst_kth_smallest(tree, 1, &key));
    ASSERT_EQ(20, key);

    ASSERT_TRUE(bst_kth_smallest(tree, 3, &key));
    ASSERT_EQ(40, key);

    ASSERT_TRUE(bst_kth_smallest(tree, 5, &key));
    ASSERT_EQ(70, key);

    ASSERT_FALSE(bst_kth_smallest(tree, 6, &key)); /* Out of bounds */
    bst_destroy(tree);
}

/* ============== LCA Test ============== */

TEST(bst_lca_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);
    bst_insert(tree, 60, 600);
    bst_insert(tree, 80, 800);

    int lca;
    ASSERT_TRUE(bst_lca(tree, 20, 40, &lca));
    ASSERT_EQ(30, lca);

    ASSERT_TRUE(bst_lca(tree, 60, 80, &lca));
    ASSERT_EQ(70, lca);

    ASSERT_TRUE(bst_lca(tree, 20, 70, &lca));
    ASSERT_EQ(50, lca);

    ASSERT_TRUE(bst_lca(tree, 20, 30, &lca));
    ASSERT_EQ(30, lca);
    bst_destroy(tree);
}

/* ============== To Array Test ============== */

TEST(bst_to_array_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    int *arr;
    size_t size;
    ASSERT_TRUE(bst_to_array(tree, &arr, &size));

    ASSERT_EQ(5, size);
    ASSERT_EQ(20, arr[0]);
    ASSERT_EQ(30, arr[1]);
    ASSERT_EQ(40, arr[2]);
    ASSERT_EQ(50, arr[3]);
    ASSERT_EQ(70, arr[4]);

    free(arr);
    bst_destroy(tree);
}

/* ============== Validation Test ============== */

TEST(bst_is_valid_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);
    bst_insert(tree, 20, 200);
    bst_insert(tree, 40, 400);

    ASSERT_TRUE(bst_is_valid(tree));
    bst_destroy(tree);
}

/* ============== Clear Test ============== */

TEST(bst_clear_test) {
    BST *tree = bst_create();
    bst_insert(tree, 50, 500);
    bst_insert(tree, 30, 300);
    bst_insert(tree, 70, 700);

    bst_clear(tree);
    ASSERT_EQ(0, bst_size(tree));
    ASSERT_TRUE(bst_is_empty(tree));

    /* Should be able to insert again */
    ASSERT_TRUE(bst_insert(tree, 100, 1000));
    ASSERT_EQ(1, bst_size(tree));
    bst_destroy(tree);
}

/* ============== Null Safety Test ============== */

TEST(bst_null_safety) {
    ASSERT_EQ(0, bst_size(NULL));
    ASSERT_TRUE(bst_is_empty(NULL));
    ASSERT_FALSE(bst_insert(NULL, 50, 500));
    ASSERT_FALSE(bst_search(NULL, 50, NULL));
    ASSERT_FALSE(bst_delete(NULL, 50));
    ASSERT_FALSE(bst_contains(NULL, 50));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Binary Search Tree");

    /* Basic tests */
    RUN_TEST(bst_create_empty);
    RUN_TEST(bst_insert_single);
    RUN_TEST(bst_insert_multiple);
    RUN_TEST(bst_insert_duplicate);
    RUN_TEST(bst_search_found);
    RUN_TEST(bst_search_not_found);

    /* Delete tests */
    RUN_TEST(bst_delete_leaf);
    RUN_TEST(bst_delete_one_child);
    RUN_TEST(bst_delete_two_children);
    RUN_TEST(bst_delete_root);
    RUN_TEST(bst_delete_not_found);

    /* Min/Max tests */
    RUN_TEST(bst_min_max);
    RUN_TEST(bst_min_max_empty);

    /* Height tests */
    RUN_TEST(bst_height_single);
    RUN_TEST(bst_height_balanced);
    RUN_TEST(bst_height_degenerate);

    /* Traversal tests */
    RUN_TEST(bst_inorder_traversal);
    RUN_TEST(bst_preorder_traversal);
    RUN_TEST(bst_postorder_traversal);
    RUN_TEST(bst_levelorder_traversal);

    /* Successor/Predecessor tests */
    RUN_TEST(bst_successor_test);
    RUN_TEST(bst_predecessor_test);

    /* Kth smallest test */
    RUN_TEST(bst_kth_smallest_test);

    /* LCA test */
    RUN_TEST(bst_lca_test);

    /* To array test */
    RUN_TEST(bst_to_array_test);

    /* Validation test */
    RUN_TEST(bst_is_valid_test);

    /* Clear test */
    RUN_TEST(bst_clear_test);

    /* Null safety test */
    RUN_TEST(bst_null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
