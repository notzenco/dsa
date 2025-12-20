/**
 * Tests for Red-Black Tree
 */

#include "test_framework.h"
#include "../data-structures/red_black_tree.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(rbtree_create_empty) {
    RBTree *tree = rbtree_create();
    ASSERT_NOT_NULL(tree);
    ASSERT_EQ(0, rbtree_size(tree));
    ASSERT_TRUE(rbtree_is_empty(tree));
    ASSERT_TRUE(rbtree_validate(tree));
    rbtree_destroy(tree);
}

/* ============== Insert Tests ============== */

TEST(rbtree_insert_single) {
    RBTree *tree = rbtree_create();
    ASSERT_TRUE(rbtree_insert(tree, 10, 100));
    ASSERT_EQ(1, rbtree_size(tree));
    ASSERT_TRUE(rbtree_contains(tree, 10));
    ASSERT_TRUE(rbtree_validate(tree));
    rbtree_destroy(tree);
}

TEST(rbtree_insert_multiple) {
    RBTree *tree = rbtree_create();

    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);

    ASSERT_EQ(3, rbtree_size(tree));
    ASSERT_TRUE(rbtree_contains(tree, 10));
    ASSERT_TRUE(rbtree_contains(tree, 5));
    ASSERT_TRUE(rbtree_contains(tree, 15));
    ASSERT_TRUE(rbtree_validate(tree));
    rbtree_destroy(tree);
}

TEST(rbtree_insert_duplicate) {
    RBTree *tree = rbtree_create();

    ASSERT_TRUE(rbtree_insert(tree, 10, 100));   /* New key */
    ASSERT_FALSE(rbtree_insert(tree, 10, 200));  /* Update value */
    ASSERT_EQ(1, rbtree_size(tree));

    int value;
    rbtree_search(tree, 10, &value);
    ASSERT_EQ(200, value);  /* Value updated */
    ASSERT_TRUE(rbtree_validate(tree));
    rbtree_destroy(tree);
}

TEST(rbtree_insert_sorted_order) {
    RBTree *tree = rbtree_create();

    /* Insert in sorted order - worst case for BST, but RB stays balanced */
    for (int i = 1; i <= 10; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    ASSERT_EQ(10, rbtree_size(tree));
    ASSERT_TRUE(rbtree_validate(tree));

    /* Height should be O(log n), not O(n) */
    int height = rbtree_height(tree);
    ASSERT_TRUE(height <= 8);  /* Should be much less than 10 */

    rbtree_destroy(tree);
}

TEST(rbtree_insert_reverse_order) {
    RBTree *tree = rbtree_create();

    /* Insert in reverse order */
    for (int i = 10; i >= 1; i--) {
        rbtree_insert(tree, i, i * 10);
    }

    ASSERT_EQ(10, rbtree_size(tree));
    ASSERT_TRUE(rbtree_validate(tree));

    /* Verify sorted order */
    int keys[10];
    rbtree_inorder(tree, keys, 10);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(i + 1, keys[i]);
    }

    rbtree_destroy(tree);
}

/* ============== Search Tests ============== */

TEST(rbtree_search_found) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 20, 200);
    rbtree_insert(tree, 30, 300);

    int value;
    ASSERT_TRUE(rbtree_search(tree, 10, &value));
    ASSERT_EQ(100, value);

    ASSERT_TRUE(rbtree_search(tree, 20, &value));
    ASSERT_EQ(200, value);

    ASSERT_TRUE(rbtree_search(tree, 30, &value));
    ASSERT_EQ(300, value);

    rbtree_destroy(tree);
}

TEST(rbtree_search_not_found) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);

    int value;
    ASSERT_FALSE(rbtree_search(tree, 5, &value));
    ASSERT_FALSE(rbtree_search(tree, 15, &value));

    rbtree_destroy(tree);
}

/* ============== Delete Tests ============== */

TEST(rbtree_delete_leaf) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);

    ASSERT_TRUE(rbtree_delete(tree, 5));
    ASSERT_EQ(2, rbtree_size(tree));
    ASSERT_FALSE(rbtree_contains(tree, 5));
    ASSERT_TRUE(rbtree_validate(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_delete_one_child) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);
    rbtree_insert(tree, 3, 30);

    ASSERT_TRUE(rbtree_delete(tree, 5));
    ASSERT_EQ(3, rbtree_size(tree));
    ASSERT_FALSE(rbtree_contains(tree, 5));
    ASSERT_TRUE(rbtree_contains(tree, 3));
    ASSERT_TRUE(rbtree_validate(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_delete_two_children) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);
    rbtree_insert(tree, 3, 30);
    rbtree_insert(tree, 7, 70);

    ASSERT_TRUE(rbtree_delete(tree, 5));
    ASSERT_EQ(4, rbtree_size(tree));
    ASSERT_FALSE(rbtree_contains(tree, 5));
    ASSERT_TRUE(rbtree_contains(tree, 3));
    ASSERT_TRUE(rbtree_contains(tree, 7));
    ASSERT_TRUE(rbtree_validate(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_delete_root) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);

    ASSERT_TRUE(rbtree_delete(tree, 10));
    ASSERT_EQ(2, rbtree_size(tree));
    ASSERT_FALSE(rbtree_contains(tree, 10));
    ASSERT_TRUE(rbtree_validate(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_delete_not_found) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);

    ASSERT_FALSE(rbtree_delete(tree, 20));
    ASSERT_EQ(1, rbtree_size(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_delete_all) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 5, 50);
    rbtree_insert(tree, 15, 150);

    rbtree_delete(tree, 10);
    rbtree_delete(tree, 5);
    rbtree_delete(tree, 15);

    ASSERT_EQ(0, rbtree_size(tree));
    ASSERT_TRUE(rbtree_is_empty(tree));
    ASSERT_TRUE(rbtree_validate(tree));

    rbtree_destroy(tree);
}

/* ============== Min/Max Tests ============== */

TEST(rbtree_min_max) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 50, 500);
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 90, 900);
    rbtree_insert(tree, 30, 300);

    int min, max;
    ASSERT_TRUE(rbtree_min(tree, &min));
    ASSERT_EQ(10, min);

    ASSERT_TRUE(rbtree_max(tree, &max));
    ASSERT_EQ(90, max);

    rbtree_destroy(tree);
}

TEST(rbtree_min_max_empty) {
    RBTree *tree = rbtree_create();
    int val;
    ASSERT_FALSE(rbtree_min(tree, &val));
    ASSERT_FALSE(rbtree_max(tree, &val));
    rbtree_destroy(tree);
}

/* ============== Floor/Ceiling Tests ============== */

TEST(rbtree_floor_basic) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 30, 300);
    rbtree_insert(tree, 50, 500);

    int result;
    ASSERT_TRUE(rbtree_floor(tree, 30, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(rbtree_floor(tree, 35, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(rbtree_floor(tree, 50, &result));
    ASSERT_EQ(50, result);

    ASSERT_FALSE(rbtree_floor(tree, 5, &result));

    rbtree_destroy(tree);
}

TEST(rbtree_ceiling_basic) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 30, 300);
    rbtree_insert(tree, 50, 500);

    int result;
    ASSERT_TRUE(rbtree_ceiling(tree, 30, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(rbtree_ceiling(tree, 25, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(rbtree_ceiling(tree, 10, &result));
    ASSERT_EQ(10, result);

    ASSERT_FALSE(rbtree_ceiling(tree, 55, &result));

    rbtree_destroy(tree);
}

/* ============== Traversal Tests ============== */

TEST(rbtree_inorder_test) {
    RBTree *tree = rbtree_create();
    rbtree_insert(tree, 30, 300);
    rbtree_insert(tree, 10, 100);
    rbtree_insert(tree, 50, 500);
    rbtree_insert(tree, 20, 200);
    rbtree_insert(tree, 40, 400);

    int keys[10];
    size_t count = rbtree_inorder(tree, keys, 10);

    ASSERT_EQ(5, count);
    ASSERT_EQ(10, keys[0]);
    ASSERT_EQ(20, keys[1]);
    ASSERT_EQ(30, keys[2]);
    ASSERT_EQ(40, keys[3]);
    ASSERT_EQ(50, keys[4]);

    rbtree_destroy(tree);
}

TEST(rbtree_range_test) {
    RBTree *tree = rbtree_create();
    for (int i = 1; i <= 10; i++) {
        rbtree_insert(tree, i * 10, i * 100);
    }

    int keys[10];
    size_t count = rbtree_range(tree, 30, 70, keys, 10);

    ASSERT_EQ(5, count);  /* 30, 40, 50, 60, 70 */
    ASSERT_EQ(30, keys[0]);
    ASSERT_EQ(70, keys[4]);

    rbtree_destroy(tree);
}

/* ============== Clear Tests ============== */

TEST(rbtree_clear_test) {
    RBTree *tree = rbtree_create();
    for (int i = 1; i <= 10; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    rbtree_clear(tree);
    ASSERT_EQ(0, rbtree_size(tree));
    ASSERT_TRUE(rbtree_is_empty(tree));
    ASSERT_TRUE(rbtree_validate(tree));

    /* Can still insert after clear */
    rbtree_insert(tree, 100, 1000);
    ASSERT_EQ(1, rbtree_size(tree));

    rbtree_destroy(tree);
}

/* ============== Validation Tests ============== */

TEST(rbtree_validate_after_operations) {
    RBTree *tree = rbtree_create();

    /* Insert many elements */
    for (int i = 0; i < 100; i++) {
        rbtree_insert(tree, i, i * 10);
        ASSERT_TRUE(rbtree_validate(tree));
    }

    /* Delete some elements */
    for (int i = 0; i < 100; i += 2) {
        rbtree_delete(tree, i);
        ASSERT_TRUE(rbtree_validate(tree));
    }

    ASSERT_EQ(50, rbtree_size(tree));

    rbtree_destroy(tree);
}

TEST(rbtree_black_height) {
    RBTree *tree = rbtree_create();

    for (int i = 1; i <= 15; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    int bh = rbtree_black_height(tree);
    ASSERT_TRUE(bh >= 1);

    rbtree_destroy(tree);
}

/* ============== NULL Safety Tests ============== */

TEST(rbtree_null_safety) {
    int val;
    int keys[10];

    ASSERT_FALSE(rbtree_insert(NULL, 10, 100));
    ASSERT_FALSE(rbtree_search(NULL, 10, &val));
    ASSERT_FALSE(rbtree_delete(NULL, 10));
    ASSERT_FALSE(rbtree_contains(NULL, 10));
    ASSERT_EQ(0, rbtree_size(NULL));
    ASSERT_TRUE(rbtree_is_empty(NULL));
    ASSERT_FALSE(rbtree_min(NULL, &val));
    ASSERT_FALSE(rbtree_max(NULL, &val));
    ASSERT_EQ(0, rbtree_inorder(NULL, keys, 10));
    ASSERT_EQ(0, rbtree_range(NULL, 0, 10, keys, 10));
    ASSERT_FALSE(rbtree_validate(NULL));
}

/* ============== Stress Tests ============== */

TEST(rbtree_stress_insert) {
    RBTree *tree = rbtree_create();

    for (int i = 0; i < 1000; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    ASSERT_EQ(1000, rbtree_size(tree));
    ASSERT_TRUE(rbtree_validate(tree));

    /* Height should be O(log n) */
    int height = rbtree_height(tree);
    ASSERT_TRUE(height <= 20);  /* 2 * log2(1000) ≈ 20 */

    rbtree_destroy(tree);
}

TEST(rbtree_stress_delete) {
    RBTree *tree = rbtree_create();

    for (int i = 0; i < 1000; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    /* Delete every other element */
    for (int i = 0; i < 1000; i += 2) {
        ASSERT_TRUE(rbtree_delete(tree, i));
        ASSERT_TRUE(rbtree_validate(tree));
    }

    ASSERT_EQ(500, rbtree_size(tree));

    /* Verify correct elements remain */
    for (int i = 0; i < 1000; i++) {
        if (i % 2 == 0) {
            ASSERT_FALSE(rbtree_contains(tree, i));
        } else {
            ASSERT_TRUE(rbtree_contains(tree, i));
        }
    }

    rbtree_destroy(tree);
}

TEST(rbtree_stress_mixed) {
    RBTree *tree = rbtree_create();

    /* Mixed operations */
    for (int i = 0; i < 500; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    for (int i = 0; i < 250; i++) {
        rbtree_delete(tree, i * 2);
    }

    for (int i = 500; i < 750; i++) {
        rbtree_insert(tree, i, i * 10);
    }

    ASSERT_TRUE(rbtree_validate(tree));
    ASSERT_EQ(500, rbtree_size(tree));  /* 500 - 250 + 250 = 500 */

    rbtree_destroy(tree);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Red-Black Tree");

    /* Creation */
    RUN_TEST(rbtree_create_empty);

    /* Insert */
    RUN_TEST(rbtree_insert_single);
    RUN_TEST(rbtree_insert_multiple);
    RUN_TEST(rbtree_insert_duplicate);
    RUN_TEST(rbtree_insert_sorted_order);
    RUN_TEST(rbtree_insert_reverse_order);

    /* Search */
    RUN_TEST(rbtree_search_found);
    RUN_TEST(rbtree_search_not_found);

    /* Delete */
    RUN_TEST(rbtree_delete_leaf);
    RUN_TEST(rbtree_delete_one_child);
    RUN_TEST(rbtree_delete_two_children);
    RUN_TEST(rbtree_delete_root);
    RUN_TEST(rbtree_delete_not_found);
    RUN_TEST(rbtree_delete_all);

    /* Min/Max */
    RUN_TEST(rbtree_min_max);
    RUN_TEST(rbtree_min_max_empty);

    /* Floor/Ceiling */
    RUN_TEST(rbtree_floor_basic);
    RUN_TEST(rbtree_ceiling_basic);

    /* Traversal */
    RUN_TEST(rbtree_inorder_test);
    RUN_TEST(rbtree_range_test);

    /* Clear */
    RUN_TEST(rbtree_clear_test);

    /* Validation */
    RUN_TEST(rbtree_validate_after_operations);
    RUN_TEST(rbtree_black_height);

    /* NULL Safety */
    RUN_TEST(rbtree_null_safety);

    /* Stress */
    RUN_TEST(rbtree_stress_insert);
    RUN_TEST(rbtree_stress_delete);
    RUN_TEST(rbtree_stress_mixed);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
