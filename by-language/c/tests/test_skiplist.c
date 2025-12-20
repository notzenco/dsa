/**
 * Tests for Skip List
 */

#include "test_framework.h"
#include "../data-structures/skip_list.h"
#include <stdlib.h>

/* ============== Creation Tests ============== */

TEST(skiplist_create_empty) {
    SkipList *sl = skiplist_create();
    ASSERT_NOT_NULL(sl);
    ASSERT_EQ(0, skiplist_size(sl));
    ASSERT_TRUE(skiplist_is_empty(sl));
    skiplist_destroy(sl);
}

TEST(skiplist_create_with_prob) {
    SkipList *sl = skiplist_create_with_prob(0.25);
    ASSERT_NOT_NULL(sl);
    skiplist_destroy(sl);
}

/* ============== Insert Tests ============== */

TEST(skiplist_insert_single) {
    SkipList *sl = skiplist_create();
    ASSERT_TRUE(skiplist_insert(sl, 10, 100));
    ASSERT_EQ(1, skiplist_size(sl));
    ASSERT_TRUE(skiplist_contains(sl, 10));
    skiplist_destroy(sl);
}

TEST(skiplist_insert_multiple) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 5, 50);
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 15, 150);

    ASSERT_EQ(3, skiplist_size(sl));
    ASSERT_TRUE(skiplist_contains(sl, 5));
    ASSERT_TRUE(skiplist_contains(sl, 10));
    ASSERT_TRUE(skiplist_contains(sl, 15));
    skiplist_destroy(sl);
}

TEST(skiplist_insert_duplicate) {
    SkipList *sl = skiplist_create();
    ASSERT_TRUE(skiplist_insert(sl, 10, 100));  /* New key */
    ASSERT_FALSE(skiplist_insert(sl, 10, 200)); /* Existing key - update */
    ASSERT_EQ(1, skiplist_size(sl));

    int value;
    skiplist_search(sl, 10, &value);
    ASSERT_EQ(200, value);  /* Value updated */
    skiplist_destroy(sl);
}

TEST(skiplist_insert_sorted_order) {
    SkipList *sl = skiplist_create();

    /* Insert in sorted order */
    for (int i = 1; i <= 10; i++) {
        skiplist_insert(sl, i, i * 10);
    }

    ASSERT_EQ(10, skiplist_size(sl));

    /* Verify all present */
    for (int i = 1; i <= 10; i++) {
        ASSERT_TRUE(skiplist_contains(sl, i));
    }
    skiplist_destroy(sl);
}

TEST(skiplist_insert_reverse_order) {
    SkipList *sl = skiplist_create();

    /* Insert in reverse order */
    for (int i = 10; i >= 1; i--) {
        skiplist_insert(sl, i, i * 10);
    }

    ASSERT_EQ(10, skiplist_size(sl));

    int keys[10];
    skiplist_get_keys(sl, keys, 10);

    /* Keys should be in sorted order */
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(i + 1, keys[i]);
    }
    skiplist_destroy(sl);
}

/* ============== Search Tests ============== */

TEST(skiplist_search_found) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 20, 200);
    skiplist_insert(sl, 30, 300);

    int value;
    ASSERT_TRUE(skiplist_search(sl, 10, &value));
    ASSERT_EQ(100, value);

    ASSERT_TRUE(skiplist_search(sl, 20, &value));
    ASSERT_EQ(200, value);

    ASSERT_TRUE(skiplist_search(sl, 30, &value));
    ASSERT_EQ(300, value);
    skiplist_destroy(sl);
}

TEST(skiplist_search_not_found) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);

    int value;
    ASSERT_FALSE(skiplist_search(sl, 5, &value));
    ASSERT_FALSE(skiplist_search(sl, 15, &value));
    skiplist_destroy(sl);
}

/* ============== Delete Tests ============== */

TEST(skiplist_delete_existing) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 20, 200);
    skiplist_insert(sl, 30, 300);

    ASSERT_TRUE(skiplist_delete(sl, 20));
    ASSERT_EQ(2, skiplist_size(sl));
    ASSERT_FALSE(skiplist_contains(sl, 20));
    ASSERT_TRUE(skiplist_contains(sl, 10));
    ASSERT_TRUE(skiplist_contains(sl, 30));
    skiplist_destroy(sl);
}

TEST(skiplist_delete_not_found) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);

    ASSERT_FALSE(skiplist_delete(sl, 20));
    ASSERT_EQ(1, skiplist_size(sl));
    skiplist_destroy(sl);
}

TEST(skiplist_delete_all) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 20, 200);
    skiplist_insert(sl, 30, 300);

    skiplist_delete(sl, 10);
    skiplist_delete(sl, 20);
    skiplist_delete(sl, 30);

    ASSERT_EQ(0, skiplist_size(sl));
    ASSERT_TRUE(skiplist_is_empty(sl));
    skiplist_destroy(sl);
}

/* ============== Min/Max Tests ============== */

TEST(skiplist_min_max) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 50, 500);
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 90, 900);
    skiplist_insert(sl, 30, 300);

    int min, max;
    ASSERT_TRUE(skiplist_min(sl, &min));
    ASSERT_EQ(10, min);

    ASSERT_TRUE(skiplist_max(sl, &max));
    ASSERT_EQ(90, max);
    skiplist_destroy(sl);
}

TEST(skiplist_min_max_empty) {
    SkipList *sl = skiplist_create();
    int val;
    ASSERT_FALSE(skiplist_min(sl, &val));
    ASSERT_FALSE(skiplist_max(sl, &val));
    skiplist_destroy(sl);
}

/* ============== Range Query Tests ============== */

TEST(skiplist_range_basic) {
    SkipList *sl = skiplist_create();
    for (int i = 1; i <= 10; i++) {
        skiplist_insert(sl, i * 10, i * 100);
    }

    int keys[10];
    size_t count = skiplist_range(sl, 30, 70, keys, 10);

    ASSERT_EQ(5, count);  /* 30, 40, 50, 60, 70 */
    ASSERT_EQ(30, keys[0]);
    ASSERT_EQ(70, keys[4]);
    skiplist_destroy(sl);
}

TEST(skiplist_range_empty) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 90, 900);

    int keys[10];
    size_t count = skiplist_range(sl, 20, 80, keys, 10);

    ASSERT_EQ(0, count);
    skiplist_destroy(sl);
}

/* ============== Floor/Ceiling Tests ============== */

TEST(skiplist_floor_basic) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 30, 300);
    skiplist_insert(sl, 50, 500);

    int result;
    ASSERT_TRUE(skiplist_floor(sl, 30, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(skiplist_floor(sl, 35, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(skiplist_floor(sl, 50, &result));
    ASSERT_EQ(50, result);

    ASSERT_FALSE(skiplist_floor(sl, 5, &result));
    skiplist_destroy(sl);
}

TEST(skiplist_ceiling_basic) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 30, 300);
    skiplist_insert(sl, 50, 500);

    int result;
    ASSERT_TRUE(skiplist_ceiling(sl, 30, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(skiplist_ceiling(sl, 25, &result));
    ASSERT_EQ(30, result);

    ASSERT_TRUE(skiplist_ceiling(sl, 10, &result));
    ASSERT_EQ(10, result);

    ASSERT_FALSE(skiplist_ceiling(sl, 55, &result));
    skiplist_destroy(sl);
}

/* ============== Clear Tests ============== */

TEST(skiplist_clear_test) {
    SkipList *sl = skiplist_create();
    for (int i = 1; i <= 10; i++) {
        skiplist_insert(sl, i, i * 10);
    }

    skiplist_clear(sl);
    ASSERT_EQ(0, skiplist_size(sl));
    ASSERT_TRUE(skiplist_is_empty(sl));

    /* Can still insert after clear */
    skiplist_insert(sl, 100, 1000);
    ASSERT_EQ(1, skiplist_size(sl));
    skiplist_destroy(sl);
}

/* ============== Get Keys Tests ============== */

TEST(skiplist_get_keys_test) {
    SkipList *sl = skiplist_create();
    skiplist_insert(sl, 30, 300);
    skiplist_insert(sl, 10, 100);
    skiplist_insert(sl, 50, 500);
    skiplist_insert(sl, 20, 200);
    skiplist_insert(sl, 40, 400);

    int keys[10];
    size_t count = skiplist_get_keys(sl, keys, 10);

    ASSERT_EQ(5, count);
    /* Keys should be in sorted order */
    ASSERT_EQ(10, keys[0]);
    ASSERT_EQ(20, keys[1]);
    ASSERT_EQ(30, keys[2]);
    ASSERT_EQ(40, keys[3]);
    ASSERT_EQ(50, keys[4]);
    skiplist_destroy(sl);
}

/* ============== NULL Safety Tests ============== */

TEST(skiplist_null_safety) {
    int val;
    int keys[10];

    ASSERT_FALSE(skiplist_insert(NULL, 10, 100));
    ASSERT_FALSE(skiplist_search(NULL, 10, &val));
    ASSERT_FALSE(skiplist_delete(NULL, 10));
    ASSERT_FALSE(skiplist_contains(NULL, 10));
    ASSERT_EQ(0, skiplist_size(NULL));
    ASSERT_TRUE(skiplist_is_empty(NULL));
    ASSERT_FALSE(skiplist_min(NULL, &val));
    ASSERT_FALSE(skiplist_max(NULL, &val));
    ASSERT_EQ(0, skiplist_range(NULL, 0, 10, keys, 10));
    ASSERT_EQ(0, skiplist_get_keys(NULL, keys, 10));
}

/* ============== Stress Tests ============== */

TEST(skiplist_stress_insert) {
    SkipList *sl = skiplist_create();

    for (int i = 0; i < 1000; i++) {
        skiplist_insert(sl, i, i * 10);
    }

    ASSERT_EQ(1000, skiplist_size(sl));

    /* Verify all elements */
    for (int i = 0; i < 1000; i++) {
        int value;
        ASSERT_TRUE(skiplist_search(sl, i, &value));
        ASSERT_EQ(i * 10, value);
    }

    skiplist_destroy(sl);
}

TEST(skiplist_stress_delete) {
    SkipList *sl = skiplist_create();

    for (int i = 0; i < 1000; i++) {
        skiplist_insert(sl, i, i * 10);
    }

    /* Delete every other element */
    for (int i = 0; i < 1000; i += 2) {
        ASSERT_TRUE(skiplist_delete(sl, i));
    }

    ASSERT_EQ(500, skiplist_size(sl));

    /* Verify correct elements remain */
    for (int i = 0; i < 1000; i++) {
        if (i % 2 == 0) {
            ASSERT_FALSE(skiplist_contains(sl, i));
        } else {
            ASSERT_TRUE(skiplist_contains(sl, i));
        }
    }

    skiplist_destroy(sl);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Skip List");

    /* Creation */
    RUN_TEST(skiplist_create_empty);
    RUN_TEST(skiplist_create_with_prob);

    /* Insert */
    RUN_TEST(skiplist_insert_single);
    RUN_TEST(skiplist_insert_multiple);
    RUN_TEST(skiplist_insert_duplicate);
    RUN_TEST(skiplist_insert_sorted_order);
    RUN_TEST(skiplist_insert_reverse_order);

    /* Search */
    RUN_TEST(skiplist_search_found);
    RUN_TEST(skiplist_search_not_found);

    /* Delete */
    RUN_TEST(skiplist_delete_existing);
    RUN_TEST(skiplist_delete_not_found);
    RUN_TEST(skiplist_delete_all);

    /* Min/Max */
    RUN_TEST(skiplist_min_max);
    RUN_TEST(skiplist_min_max_empty);

    /* Range */
    RUN_TEST(skiplist_range_basic);
    RUN_TEST(skiplist_range_empty);

    /* Floor/Ceiling */
    RUN_TEST(skiplist_floor_basic);
    RUN_TEST(skiplist_ceiling_basic);

    /* Clear */
    RUN_TEST(skiplist_clear_test);

    /* Get Keys */
    RUN_TEST(skiplist_get_keys_test);

    /* NULL Safety */
    RUN_TEST(skiplist_null_safety);

    /* Stress */
    RUN_TEST(skiplist_stress_insert);
    RUN_TEST(skiplist_stress_delete);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
