/**
 * Tests for B-Tree
 */

#include "test_framework.h"
#include "../data-structures/b_tree.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(btree_create_default) {
    BTree *tree = btree_create_default();
    ASSERT_NOT_NULL(tree);
    ASSERT_EQ(0, btree_size(tree));
    ASSERT_TRUE(btree_is_empty(tree));
    ASSERT_TRUE(btree_validate(tree));
    btree_destroy(tree);
}

TEST(btree_create_with_degree) {
    BTree *tree = btree_create(2);  /* 2-3-4 tree */
    ASSERT_NOT_NULL(tree);
    ASSERT_TRUE(btree_validate(tree));
    btree_destroy(tree);

    tree = btree_create(5);
    ASSERT_NOT_NULL(tree);
    btree_destroy(tree);
}

TEST(btree_create_invalid) {
    ASSERT_TRUE(btree_create(0) == NULL);
    ASSERT_TRUE(btree_create(1) == NULL);
}

/* ============== Insert Tests ============== */

TEST(btree_insert_single) {
    BTree *tree = btree_create_default();
    ASSERT_TRUE(btree_insert(tree, 10, 100));
    ASSERT_EQ(1, btree_size(tree));
    ASSERT_TRUE(btree_contains(tree, 10));
    ASSERT_TRUE(btree_validate(tree));
    btree_destroy(tree);
}

TEST(btree_insert_multiple) {
    BTree *tree = btree_create_default();

    btree_insert(tree, 10, 100);
    btree_insert(tree, 5, 50);
    btree_insert(tree, 15, 150);

    ASSERT_EQ(3, btree_size(tree));
    ASSERT_TRUE(btree_contains(tree, 10));
    ASSERT_TRUE(btree_contains(tree, 5));
    ASSERT_TRUE(btree_contains(tree, 15));
    ASSERT_TRUE(btree_validate(tree));
    btree_destroy(tree);
}

TEST(btree_insert_duplicate) {
    BTree *tree = btree_create_default();

    ASSERT_TRUE(btree_insert(tree, 10, 100));   /* New key */
    ASSERT_FALSE(btree_insert(tree, 10, 200));  /* Update value */
    ASSERT_EQ(1, btree_size(tree));

    int value;
    btree_search(tree, 10, &value);
    ASSERT_EQ(200, value);  /* Value updated */
    ASSERT_TRUE(btree_validate(tree));
    btree_destroy(tree);
}

TEST(btree_insert_sorted_order) {
    BTree *tree = btree_create(2);  /* Low degree to force splits */

    for (int i = 1; i <= 20; i++) {
        btree_insert(tree, i, i * 10);
        ASSERT_TRUE(btree_validate(tree));
    }

    ASSERT_EQ(20, btree_size(tree));

    /* Verify all keys present */
    for (int i = 1; i <= 20; i++) {
        ASSERT_TRUE(btree_contains(tree, i));
    }

    btree_destroy(tree);
}

TEST(btree_insert_reverse_order) {
    BTree *tree = btree_create(2);

    for (int i = 20; i >= 1; i--) {
        btree_insert(tree, i, i * 10);
        ASSERT_TRUE(btree_validate(tree));
    }

    ASSERT_EQ(20, btree_size(tree));

    /* Verify sorted order */
    int keys[20];
    btree_inorder(tree, keys, 20);
    for (int i = 0; i < 20; i++) {
        ASSERT_EQ(i + 1, keys[i]);
    }

    btree_destroy(tree);
}

TEST(btree_insert_with_splits) {
    BTree *tree = btree_create(2);  /* Min keys: 1, Max keys: 3 */

    /* Insert enough to cause multiple splits */
    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    for (int i = 0; i < 10; i++) {
        btree_insert(tree, values[i], values[i] * 10);
        ASSERT_TRUE(btree_validate(tree));
    }

    ASSERT_EQ(10, btree_size(tree));

    /* Height should be > 1 */
    ASSERT_TRUE(btree_height(tree) > 1);

    btree_destroy(tree);
}

/* ============== Search Tests ============== */

TEST(btree_search_found) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 10, 100);
    btree_insert(tree, 20, 200);
    btree_insert(tree, 30, 300);

    int value;
    ASSERT_TRUE(btree_search(tree, 10, &value));
    ASSERT_EQ(100, value);

    ASSERT_TRUE(btree_search(tree, 20, &value));
    ASSERT_EQ(200, value);

    ASSERT_TRUE(btree_search(tree, 30, &value));
    ASSERT_EQ(300, value);

    btree_destroy(tree);
}

TEST(btree_search_not_found) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 10, 100);

    int value;
    ASSERT_FALSE(btree_search(tree, 5, &value));
    ASSERT_FALSE(btree_search(tree, 15, &value));

    btree_destroy(tree);
}

/* ============== Delete Tests ============== */

TEST(btree_delete_from_leaf) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 10, 100);
    btree_insert(tree, 5, 50);
    btree_insert(tree, 15, 150);

    ASSERT_TRUE(btree_delete(tree, 5));
    ASSERT_EQ(2, btree_size(tree));
    ASSERT_FALSE(btree_contains(tree, 5));
    ASSERT_TRUE(btree_validate(tree));

    btree_destroy(tree);
}

TEST(btree_delete_from_internal) {
    BTree *tree = btree_create(2);

    /* Build a tree with internal nodes */
    for (int i = 1; i <= 10; i++) {
        btree_insert(tree, i, i * 10);
    }

    ASSERT_TRUE(btree_validate(tree));

    /* Delete from internal nodes */
    btree_delete(tree, 5);
    ASSERT_TRUE(btree_validate(tree));
    ASSERT_FALSE(btree_contains(tree, 5));

    btree_destroy(tree);
}

TEST(btree_delete_not_found) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 10, 100);

    ASSERT_FALSE(btree_delete(tree, 20));
    ASSERT_EQ(1, btree_size(tree));

    btree_destroy(tree);
}

TEST(btree_delete_all) {
    BTree *tree = btree_create(2);

    for (int i = 1; i <= 10; i++) {
        btree_insert(tree, i, i * 10);
    }

    for (int i = 1; i <= 10; i++) {
        ASSERT_TRUE(btree_delete(tree, i));
        ASSERT_TRUE(btree_validate(tree));
    }

    ASSERT_EQ(0, btree_size(tree));
    ASSERT_TRUE(btree_is_empty(tree));

    btree_destroy(tree);
}

TEST(btree_delete_with_merge) {
    BTree *tree = btree_create(2);

    for (int i = 1; i <= 7; i++) {
        btree_insert(tree, i, i * 10);
    }

    /* Delete keys that force merges */
    for (int i = 7; i >= 1; i--) {
        ASSERT_TRUE(btree_delete(tree, i));
        ASSERT_TRUE(btree_validate(tree));
    }

    ASSERT_TRUE(btree_is_empty(tree));

    btree_destroy(tree);
}

/* ============== Min/Max Tests ============== */

TEST(btree_min_max) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 50, 500);
    btree_insert(tree, 10, 100);
    btree_insert(tree, 90, 900);
    btree_insert(tree, 30, 300);

    int min, max;
    ASSERT_TRUE(btree_min(tree, &min));
    ASSERT_EQ(10, min);

    ASSERT_TRUE(btree_max(tree, &max));
    ASSERT_EQ(90, max);

    btree_destroy(tree);
}

TEST(btree_min_max_empty) {
    BTree *tree = btree_create_default();
    int val;
    ASSERT_FALSE(btree_min(tree, &val));
    ASSERT_FALSE(btree_max(tree, &val));
    btree_destroy(tree);
}

/* ============== Traversal Tests ============== */

TEST(btree_inorder_test) {
    BTree *tree = btree_create_default();
    btree_insert(tree, 30, 300);
    btree_insert(tree, 10, 100);
    btree_insert(tree, 50, 500);
    btree_insert(tree, 20, 200);
    btree_insert(tree, 40, 400);

    int keys[10];
    size_t count = btree_inorder(tree, keys, 10);

    ASSERT_EQ(5, count);
    ASSERT_EQ(10, keys[0]);
    ASSERT_EQ(20, keys[1]);
    ASSERT_EQ(30, keys[2]);
    ASSERT_EQ(40, keys[3]);
    ASSERT_EQ(50, keys[4]);

    btree_destroy(tree);
}

TEST(btree_range_test) {
    BTree *tree = btree_create_default();
    for (int i = 1; i <= 10; i++) {
        btree_insert(tree, i * 10, i * 100);
    }

    int keys[10];
    size_t count = btree_range(tree, 30, 70, keys, 10);

    ASSERT_EQ(5, count);  /* 30, 40, 50, 60, 70 */
    ASSERT_EQ(30, keys[0]);
    ASSERT_EQ(70, keys[4]);

    btree_destroy(tree);
}

/* ============== Height Tests ============== */

TEST(btree_height_test) {
    BTree *tree = btree_create(2);

    ASSERT_EQ(0, btree_height(tree));

    btree_insert(tree, 1, 10);
    ASSERT_EQ(1, btree_height(tree));

    /* Insert more to increase height */
    for (int i = 2; i <= 10; i++) {
        btree_insert(tree, i, i * 10);
    }

    ASSERT_TRUE(btree_height(tree) >= 2);

    btree_destroy(tree);
}

/* ============== Clear Tests ============== */

TEST(btree_clear_test) {
    BTree *tree = btree_create_default();
    for (int i = 1; i <= 10; i++) {
        btree_insert(tree, i, i * 10);
    }

    btree_clear(tree);
    ASSERT_EQ(0, btree_size(tree));
    ASSERT_TRUE(btree_is_empty(tree));
    ASSERT_TRUE(btree_validate(tree));

    /* Can still insert after clear */
    btree_insert(tree, 100, 1000);
    ASSERT_EQ(1, btree_size(tree));

    btree_destroy(tree);
}

/* ============== NULL Safety Tests ============== */

TEST(btree_null_safety) {
    int val;
    int keys[10];

    ASSERT_FALSE(btree_insert(NULL, 10, 100));
    ASSERT_FALSE(btree_search(NULL, 10, &val));
    ASSERT_FALSE(btree_delete(NULL, 10));
    ASSERT_FALSE(btree_contains(NULL, 10));
    ASSERT_EQ(0, btree_size(NULL));
    ASSERT_TRUE(btree_is_empty(NULL));
    ASSERT_FALSE(btree_min(NULL, &val));
    ASSERT_FALSE(btree_max(NULL, &val));
    ASSERT_EQ(0, btree_inorder(NULL, keys, 10));
    ASSERT_EQ(0, btree_range(NULL, 0, 10, keys, 10));
    ASSERT_FALSE(btree_validate(NULL));
}

/* ============== Stress Tests ============== */

TEST(btree_stress_insert) {
    BTree *tree = btree_create(3);

    for (int i = 0; i < 1000; i++) {
        btree_insert(tree, i, i * 10);
    }

    ASSERT_EQ(1000, btree_size(tree));
    ASSERT_TRUE(btree_validate(tree));

    /* Verify all elements */
    for (int i = 0; i < 1000; i++) {
        int value;
        ASSERT_TRUE(btree_search(tree, i, &value));
        ASSERT_EQ(i * 10, value);
    }

    btree_destroy(tree);
}

TEST(btree_stress_delete) {
    BTree *tree = btree_create(3);

    for (int i = 0; i < 500; i++) {
        btree_insert(tree, i, i * 10);
    }

    /* Delete every other element */
    for (int i = 0; i < 500; i += 2) {
        ASSERT_TRUE(btree_delete(tree, i));
    }

    ASSERT_EQ(250, btree_size(tree));
    ASSERT_TRUE(btree_validate(tree));

    /* Verify correct elements remain */
    for (int i = 0; i < 500; i++) {
        if (i % 2 == 0) {
            ASSERT_FALSE(btree_contains(tree, i));
        } else {
            ASSERT_TRUE(btree_contains(tree, i));
        }
    }

    btree_destroy(tree);
}

TEST(btree_stress_mixed) {
    BTree *tree = btree_create(2);

    /* Mixed insertions and deletions */
    for (int i = 0; i < 100; i++) {
        btree_insert(tree, i, i * 10);
    }

    for (int i = 0; i < 50; i++) {
        btree_delete(tree, i * 2);
    }

    for (int i = 100; i < 150; i++) {
        btree_insert(tree, i, i * 10);
    }

    ASSERT_TRUE(btree_validate(tree));
    ASSERT_EQ(100, btree_size(tree));  /* 100 - 50 + 50 = 100 */

    btree_destroy(tree);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("B-Tree");

    /* Creation */
    RUN_TEST(btree_create_default);
    RUN_TEST(btree_create_with_degree);
    RUN_TEST(btree_create_invalid);

    /* Insert */
    RUN_TEST(btree_insert_single);
    RUN_TEST(btree_insert_multiple);
    RUN_TEST(btree_insert_duplicate);
    RUN_TEST(btree_insert_sorted_order);
    RUN_TEST(btree_insert_reverse_order);
    RUN_TEST(btree_insert_with_splits);

    /* Search */
    RUN_TEST(btree_search_found);
    RUN_TEST(btree_search_not_found);

    /* Delete */
    RUN_TEST(btree_delete_from_leaf);
    RUN_TEST(btree_delete_from_internal);
    RUN_TEST(btree_delete_not_found);
    RUN_TEST(btree_delete_all);
    RUN_TEST(btree_delete_with_merge);

    /* Min/Max */
    RUN_TEST(btree_min_max);
    RUN_TEST(btree_min_max_empty);

    /* Traversal */
    RUN_TEST(btree_inorder_test);
    RUN_TEST(btree_range_test);

    /* Height */
    RUN_TEST(btree_height_test);

    /* Clear */
    RUN_TEST(btree_clear_test);

    /* NULL Safety */
    RUN_TEST(btree_null_safety);

    /* Stress */
    RUN_TEST(btree_stress_insert);
    RUN_TEST(btree_stress_delete);
    RUN_TEST(btree_stress_mixed);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
