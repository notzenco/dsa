/**
 * Tests for Hash Table
 */

#include "test_framework.h"
#include "../data-structures/hash_table.h"
#include <stdlib.h>
#include <string.h>

/* ============== String Key Hash Table Tests ============== */

TEST(ht_create_empty) {
    HashTable *table = ht_create();
    ASSERT_NOT_NULL(table);
    ASSERT_EQ(0, ht_size(table));
    ASSERT_TRUE(ht_is_empty(table));
    ht_destroy(table);
}

TEST(ht_put_single) {
    HashTable *table = ht_create();
    ASSERT_TRUE(ht_put(table, "key1", 100));
    ASSERT_EQ(1, ht_size(table));

    int value;
    ASSERT_TRUE(ht_get(table, "key1", &value));
    ASSERT_EQ(100, value);
    ht_destroy(table);
}

TEST(ht_put_multiple) {
    HashTable *table = ht_create();
    ht_put(table, "apple", 1);
    ht_put(table, "banana", 2);
    ht_put(table, "cherry", 3);

    ASSERT_EQ(3, ht_size(table));

    int value;
    ASSERT_TRUE(ht_get(table, "apple", &value));
    ASSERT_EQ(1, value);
    ASSERT_TRUE(ht_get(table, "banana", &value));
    ASSERT_EQ(2, value);
    ASSERT_TRUE(ht_get(table, "cherry", &value));
    ASSERT_EQ(3, value);
    ht_destroy(table);
}

TEST(ht_put_update) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);
    ht_put(table, "key1", 200);

    ASSERT_EQ(1, ht_size(table));

    int value;
    ht_get(table, "key1", &value);
    ASSERT_EQ(200, value);
    ht_destroy(table);
}

TEST(ht_get_not_found) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);

    int value;
    ASSERT_FALSE(ht_get(table, "key2", &value));
    ht_destroy(table);
}

TEST(ht_contains_test) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);

    ASSERT_TRUE(ht_contains(table, "key1"));
    ASSERT_FALSE(ht_contains(table, "key2"));
    ht_destroy(table);
}

TEST(ht_remove_existing) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);
    ht_put(table, "key2", 200);

    ASSERT_TRUE(ht_remove(table, "key1"));
    ASSERT_EQ(1, ht_size(table));
    ASSERT_FALSE(ht_contains(table, "key1"));
    ASSERT_TRUE(ht_contains(table, "key2"));
    ht_destroy(table);
}

TEST(ht_remove_not_found) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);

    ASSERT_FALSE(ht_remove(table, "key2"));
    ASSERT_EQ(1, ht_size(table));
    ht_destroy(table);
}

TEST(ht_clear_test) {
    HashTable *table = ht_create();
    ht_put(table, "key1", 100);
    ht_put(table, "key2", 200);
    ht_put(table, "key3", 300);

    ht_clear(table);
    ASSERT_EQ(0, ht_size(table));
    ASSERT_TRUE(ht_is_empty(table));
    ASSERT_FALSE(ht_contains(table, "key1"));
    ht_destroy(table);
}

TEST(ht_resize_on_load) {
    HashTable *table = ht_create_with_capacity(4);

    /* Insert enough to trigger resize */
    for (int i = 0; i < 10; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        ht_put(table, key, i * 10);
    }

    ASSERT_EQ(10, ht_size(table));

    /* Verify all entries still accessible */
    for (int i = 0; i < 10; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        int value;
        ASSERT_TRUE(ht_get(table, key, &value));
        ASSERT_EQ(i * 10, value);
    }

    ht_destroy(table);
}

TEST(ht_collision_handling) {
    HashTable *table = ht_create_with_capacity(4);

    /* These will likely collide in a small table */
    ht_put(table, "a", 1);
    ht_put(table, "b", 2);
    ht_put(table, "c", 3);
    ht_put(table, "d", 4);

    int value;
    ASSERT_TRUE(ht_get(table, "a", &value));
    ASSERT_EQ(1, value);
    ASSERT_TRUE(ht_get(table, "b", &value));
    ASSERT_EQ(2, value);
    ASSERT_TRUE(ht_get(table, "c", &value));
    ASSERT_EQ(3, value);
    ASSERT_TRUE(ht_get(table, "d", &value));
    ASSERT_EQ(4, value);

    ht_destroy(table);
}

TEST(ht_keys_test) {
    HashTable *table = ht_create();
    ht_put(table, "apple", 1);
    ht_put(table, "banana", 2);
    ht_put(table, "cherry", 3);

    char **keys;
    size_t count;
    ASSERT_TRUE(ht_keys(table, &keys, &count));
    ASSERT_EQ(3, count);

    /* Check all keys present (order not guaranteed) */
    int found = 0;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(keys[i], "apple") == 0 ||
            strcmp(keys[i], "banana") == 0 ||
            strcmp(keys[i], "cherry") == 0) {
            found++;
        }
    }
    ASSERT_EQ(3, found);

    free(keys);
    ht_destroy(table);
}

TEST(ht_values_test) {
    HashTable *table = ht_create();
    ht_put(table, "a", 10);
    ht_put(table, "b", 20);
    ht_put(table, "c", 30);

    int *values;
    size_t count;
    ASSERT_TRUE(ht_values(table, &values, &count));
    ASSERT_EQ(3, count);

    /* Check sum of values */
    int sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += values[i];
    }
    ASSERT_EQ(60, sum);

    free(values);
    ht_destroy(table);
}

TEST(ht_load_factor_test) {
    HashTable *table = ht_create_with_capacity(10);
    ASSERT_TRUE(ht_load_factor(table) < 0.01);

    for (int i = 0; i < 5; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        ht_put(table, key, i);
    }

    double lf = ht_load_factor(table);
    ASSERT_TRUE(lf >= 0.49 && lf <= 0.51);

    ht_destroy(table);
}

TEST(ht_null_safety) {
    ASSERT_EQ(0, ht_size(NULL));
    ASSERT_TRUE(ht_is_empty(NULL));
    ASSERT_FALSE(ht_put(NULL, "key", 100));
    ASSERT_FALSE(ht_get(NULL, "key", NULL));
    ASSERT_FALSE(ht_contains(NULL, "key"));
    ASSERT_FALSE(ht_remove(NULL, "key"));
}

TEST(ht_null_key) {
    HashTable *table = ht_create();
    ASSERT_FALSE(ht_put(table, NULL, 100));
    ASSERT_FALSE(ht_get(table, NULL, NULL));
    ht_destroy(table);
}

/* ============== Integer Key Hash Table Tests ============== */

TEST(ht_int_create_empty) {
    HashTableInt *table = ht_int_create();
    ASSERT_NOT_NULL(table);
    ASSERT_EQ(0, ht_int_size(table));
    ASSERT_TRUE(ht_int_is_empty(table));
    ht_int_destroy(table);
}

TEST(ht_int_put_single) {
    HashTableInt *table = ht_int_create();
    ASSERT_TRUE(ht_int_put(table, 42, 100));
    ASSERT_EQ(1, ht_int_size(table));

    int value;
    ASSERT_TRUE(ht_int_get(table, 42, &value));
    ASSERT_EQ(100, value);
    ht_int_destroy(table);
}

TEST(ht_int_put_multiple) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 1, 10);
    ht_int_put(table, 2, 20);
    ht_int_put(table, 3, 30);

    ASSERT_EQ(3, ht_int_size(table));

    int value;
    ASSERT_TRUE(ht_int_get(table, 1, &value));
    ASSERT_EQ(10, value);
    ASSERT_TRUE(ht_int_get(table, 2, &value));
    ASSERT_EQ(20, value);
    ASSERT_TRUE(ht_int_get(table, 3, &value));
    ASSERT_EQ(30, value);
    ht_int_destroy(table);
}

TEST(ht_int_put_update) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 42, 100);
    ht_int_put(table, 42, 200);

    ASSERT_EQ(1, ht_int_size(table));

    int value;
    ht_int_get(table, 42, &value);
    ASSERT_EQ(200, value);
    ht_int_destroy(table);
}

TEST(ht_int_put_negative_keys) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, -5, 50);
    ht_int_put(table, -10, 100);
    ht_int_put(table, 0, 0);

    int value;
    ASSERT_TRUE(ht_int_get(table, -5, &value));
    ASSERT_EQ(50, value);
    ASSERT_TRUE(ht_int_get(table, -10, &value));
    ASSERT_EQ(100, value);
    ASSERT_TRUE(ht_int_get(table, 0, &value));
    ASSERT_EQ(0, value);
    ht_int_destroy(table);
}

TEST(ht_int_get_not_found) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 42, 100);

    int value;
    ASSERT_FALSE(ht_int_get(table, 99, &value));
    ht_int_destroy(table);
}

TEST(ht_int_contains_test) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 42, 100);

    ASSERT_TRUE(ht_int_contains(table, 42));
    ASSERT_FALSE(ht_int_contains(table, 99));
    ht_int_destroy(table);
}

TEST(ht_int_remove_existing) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 1, 10);
    ht_int_put(table, 2, 20);

    ASSERT_TRUE(ht_int_remove(table, 1));
    ASSERT_EQ(1, ht_int_size(table));
    ASSERT_FALSE(ht_int_contains(table, 1));
    ASSERT_TRUE(ht_int_contains(table, 2));
    ht_int_destroy(table);
}

TEST(ht_int_remove_not_found) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 42, 100);

    ASSERT_FALSE(ht_int_remove(table, 99));
    ASSERT_EQ(1, ht_int_size(table));
    ht_int_destroy(table);
}

TEST(ht_int_clear_test) {
    HashTableInt *table = ht_int_create();
    ht_int_put(table, 1, 10);
    ht_int_put(table, 2, 20);
    ht_int_put(table, 3, 30);

    ht_int_clear(table);
    ASSERT_EQ(0, ht_int_size(table));
    ASSERT_TRUE(ht_int_is_empty(table));
    ASSERT_FALSE(ht_int_contains(table, 1));
    ht_int_destroy(table);
}

TEST(ht_int_linear_probing) {
    HashTableInt *table = ht_int_create();

    /* Insert many items to test collision handling */
    for (int i = 0; i < 50; i++) {
        ht_int_put(table, i, i * 10);
    }

    ASSERT_EQ(50, ht_int_size(table));

    /* Verify all entries */
    for (int i = 0; i < 50; i++) {
        int value;
        ASSERT_TRUE(ht_int_get(table, i, &value));
        ASSERT_EQ(i * 10, value);
    }

    ht_int_destroy(table);
}

TEST(ht_int_remove_and_reinsert) {
    HashTableInt *table = ht_int_create();

    ht_int_put(table, 1, 10);
    ht_int_put(table, 2, 20);
    ht_int_put(table, 3, 30);

    ht_int_remove(table, 2);
    ASSERT_FALSE(ht_int_contains(table, 2));

    /* Reinsert at same key */
    ht_int_put(table, 2, 200);
    ASSERT_TRUE(ht_int_contains(table, 2));

    int value;
    ht_int_get(table, 2, &value);
    ASSERT_EQ(200, value);

    ht_int_destroy(table);
}

TEST(ht_int_null_safety) {
    ASSERT_EQ(0, ht_int_size(NULL));
    ASSERT_TRUE(ht_int_is_empty(NULL));
    ASSERT_FALSE(ht_int_put(NULL, 42, 100));
    ASSERT_FALSE(ht_int_get(NULL, 42, NULL));
    ASSERT_FALSE(ht_int_contains(NULL, 42));
    ASSERT_FALSE(ht_int_remove(NULL, 42));
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Hash Table");

    /* String key hash table tests */
    RUN_TEST(ht_create_empty);
    RUN_TEST(ht_put_single);
    RUN_TEST(ht_put_multiple);
    RUN_TEST(ht_put_update);
    RUN_TEST(ht_get_not_found);
    RUN_TEST(ht_contains_test);
    RUN_TEST(ht_remove_existing);
    RUN_TEST(ht_remove_not_found);
    RUN_TEST(ht_clear_test);
    RUN_TEST(ht_resize_on_load);
    RUN_TEST(ht_collision_handling);
    RUN_TEST(ht_keys_test);
    RUN_TEST(ht_values_test);
    RUN_TEST(ht_load_factor_test);
    RUN_TEST(ht_null_safety);
    RUN_TEST(ht_null_key);

    /* Integer key hash table tests */
    RUN_TEST(ht_int_create_empty);
    RUN_TEST(ht_int_put_single);
    RUN_TEST(ht_int_put_multiple);
    RUN_TEST(ht_int_put_update);
    RUN_TEST(ht_int_put_negative_keys);
    RUN_TEST(ht_int_get_not_found);
    RUN_TEST(ht_int_contains_test);
    RUN_TEST(ht_int_remove_existing);
    RUN_TEST(ht_int_remove_not_found);
    RUN_TEST(ht_int_clear_test);
    RUN_TEST(ht_int_linear_probing);
    RUN_TEST(ht_int_remove_and_reinsert);
    RUN_TEST(ht_int_null_safety);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
