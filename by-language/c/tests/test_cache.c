/**
 * Tests for LRU and LFU Caches
 */

#include "test_framework.h"
#include "../data-structures/lru_cache.h"
#include "../data-structures/lfu_cache.h"
#include <stdlib.h>

/* ============== LRU Cache Creation Tests ============== */

TEST(lru_create_empty) {
    LRUCache *cache = lru_create(3);
    ASSERT_NOT_NULL(cache);
    ASSERT_EQ(0, lru_size(cache));
    ASSERT_EQ(3, lru_capacity(cache));
    ASSERT_TRUE(lru_is_empty(cache));
    ASSERT_FALSE(lru_is_full(cache));
    lru_destroy(cache);
}

TEST(lru_create_zero_capacity) {
    LRUCache *cache = lru_create(0);
    ASSERT_TRUE(cache == NULL);
}

/* ============== LRU Cache Put/Get Tests ============== */

TEST(lru_put_single) {
    LRUCache *cache = lru_create(2);
    ASSERT_TRUE(lru_put(cache, 1, 100));
    ASSERT_EQ(1, lru_size(cache));

    int value;
    ASSERT_TRUE(lru_get(cache, 1, &value));
    ASSERT_EQ(100, value);
    lru_destroy(cache);
}

TEST(lru_put_update) {
    LRUCache *cache = lru_create(2);
    lru_put(cache, 1, 100);
    lru_put(cache, 1, 200);

    ASSERT_EQ(1, lru_size(cache));

    int value;
    ASSERT_TRUE(lru_get(cache, 1, &value));
    ASSERT_EQ(200, value);
    lru_destroy(cache);
}

TEST(lru_eviction_basic) {
    LRUCache *cache = lru_create(2);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);  /* Should evict key 1 */

    ASSERT_EQ(2, lru_size(cache));
    ASSERT_FALSE(lru_contains(cache, 1));
    ASSERT_TRUE(lru_contains(cache, 2));
    ASSERT_TRUE(lru_contains(cache, 3));
    lru_destroy(cache);
}

TEST(lru_get_updates_recency) {
    LRUCache *cache = lru_create(2);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);

    /* Access key 1 to make it most recent */
    int value;
    lru_get(cache, 1, &value);

    /* Now insert key 3, should evict key 2 */
    lru_put(cache, 3, 300);

    ASSERT_TRUE(lru_contains(cache, 1));
    ASSERT_FALSE(lru_contains(cache, 2));
    ASSERT_TRUE(lru_contains(cache, 3));
    lru_destroy(cache);
}

TEST(lru_get_not_found) {
    LRUCache *cache = lru_create(2);
    lru_put(cache, 1, 100);

    int value;
    ASSERT_FALSE(lru_get(cache, 2, &value));
    lru_destroy(cache);
}

/* ============== LRU Cache Delete Tests ============== */

TEST(lru_delete_existing) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);

    ASSERT_TRUE(lru_delete(cache, 1));
    ASSERT_EQ(1, lru_size(cache));
    ASSERT_FALSE(lru_contains(cache, 1));
    lru_destroy(cache);
}

TEST(lru_delete_not_found) {
    LRUCache *cache = lru_create(2);
    lru_put(cache, 1, 100);

    ASSERT_FALSE(lru_delete(cache, 2));
    ASSERT_EQ(1, lru_size(cache));
    lru_destroy(cache);
}

/* ============== LRU Cache Inspection Tests ============== */

TEST(lru_peek_newest) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);

    int key;
    ASSERT_TRUE(lru_peek_newest(cache, &key));
    ASSERT_EQ(3, key);

    /* Access key 1 */
    int value;
    lru_get(cache, 1, &value);
    ASSERT_TRUE(lru_peek_newest(cache, &key));
    ASSERT_EQ(1, key);
    lru_destroy(cache);
}

TEST(lru_peek_oldest) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);

    int key;
    ASSERT_TRUE(lru_peek_oldest(cache, &key));
    ASSERT_EQ(1, key);
    lru_destroy(cache);
}

TEST(lru_get_keys) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);

    int keys[3];
    size_t count = lru_get_keys(cache, keys, 3);
    ASSERT_EQ(3, count);
    ASSERT_EQ(3, keys[0]);  /* Most recent first */
    ASSERT_EQ(2, keys[1]);
    ASSERT_EQ(1, keys[2]);  /* Least recent last */
    lru_destroy(cache);
}

/* ============== LRU Cache Clear Tests ============== */

TEST(lru_clear_test) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);

    lru_clear(cache);
    ASSERT_EQ(0, lru_size(cache));
    ASSERT_TRUE(lru_is_empty(cache));
    ASSERT_FALSE(lru_contains(cache, 1));
    lru_destroy(cache);
}

/* ============== LRU Cache NULL Safety Tests ============== */

TEST(lru_null_safety) {
    int value;
    int keys[10];

    ASSERT_FALSE(lru_put(NULL, 1, 100));
    ASSERT_FALSE(lru_get(NULL, 1, &value));
    ASSERT_FALSE(lru_delete(NULL, 1));
    ASSERT_FALSE(lru_contains(NULL, 1));
    ASSERT_EQ(0, lru_size(NULL));
    ASSERT_EQ(0, lru_capacity(NULL));
    ASSERT_TRUE(lru_is_empty(NULL));
    ASSERT_FALSE(lru_is_full(NULL));
    ASSERT_FALSE(lru_peek_newest(NULL, &value));
    ASSERT_FALSE(lru_peek_oldest(NULL, &value));
    ASSERT_EQ(0, lru_get_keys(NULL, keys, 10));
}

/* ============== LRU LeetCode Test ============== */

TEST(lru_leetcode_example) {
    /* LeetCode #146 example */
    LRUCache *cache = lru_create(2);
    int value;

    lru_put(cache, 1, 1);
    lru_put(cache, 2, 2);

    ASSERT_TRUE(lru_get(cache, 1, &value));
    ASSERT_EQ(1, value);

    lru_put(cache, 3, 3);  /* Evicts key 2 */

    ASSERT_FALSE(lru_get(cache, 2, &value));

    lru_put(cache, 4, 4);  /* Evicts key 1 */

    ASSERT_FALSE(lru_get(cache, 1, &value));
    ASSERT_TRUE(lru_get(cache, 3, &value));
    ASSERT_EQ(3, value);
    ASSERT_TRUE(lru_get(cache, 4, &value));
    ASSERT_EQ(4, value);

    lru_destroy(cache);
}

/* ============== LFU Cache Creation Tests ============== */

TEST(lfu_create_empty) {
    LFUCache *cache = lfu_create(3);
    ASSERT_NOT_NULL(cache);
    ASSERT_EQ(0, lfu_size(cache));
    ASSERT_EQ(3, lfu_capacity(cache));
    ASSERT_TRUE(lfu_is_empty(cache));
    lfu_destroy(cache);
}

TEST(lfu_create_zero_capacity) {
    LFUCache *cache = lfu_create(0);
    ASSERT_TRUE(cache == NULL);
}

/* ============== LFU Cache Put/Get Tests ============== */

TEST(lfu_put_single) {
    LFUCache *cache = lfu_create(2);
    ASSERT_TRUE(lfu_put(cache, 1, 100));
    ASSERT_EQ(1, lfu_size(cache));

    int value;
    ASSERT_TRUE(lfu_get(cache, 1, &value));
    ASSERT_EQ(100, value);
    lfu_destroy(cache);
}

TEST(lfu_put_update) {
    LFUCache *cache = lfu_create(2);
    lfu_put(cache, 1, 100);
    lfu_put(cache, 1, 200);

    ASSERT_EQ(1, lfu_size(cache));

    int value;
    ASSERT_TRUE(lfu_get(cache, 1, &value));
    ASSERT_EQ(200, value);
    lfu_destroy(cache);
}

TEST(lfu_eviction_basic) {
    LFUCache *cache = lfu_create(2);
    lfu_put(cache, 1, 100);
    lfu_put(cache, 2, 200);
    lfu_put(cache, 3, 300);  /* Should evict key 1 (LRU among freq=1) */

    ASSERT_EQ(2, lfu_size(cache));
    ASSERT_FALSE(lfu_contains(cache, 1));
    ASSERT_TRUE(lfu_contains(cache, 2));
    ASSERT_TRUE(lfu_contains(cache, 3));
    lfu_destroy(cache);
}

TEST(lfu_eviction_by_frequency) {
    LFUCache *cache = lfu_create(2);
    lfu_put(cache, 1, 100);
    lfu_put(cache, 2, 200);

    /* Access key 1 to increase its frequency */
    int value;
    lfu_get(cache, 1, &value);

    /* Now insert key 3, should evict key 2 (freq=1) not key 1 (freq=2) */
    lfu_put(cache, 3, 300);

    ASSERT_TRUE(lfu_contains(cache, 1));
    ASSERT_FALSE(lfu_contains(cache, 2));
    ASSERT_TRUE(lfu_contains(cache, 3));
    lfu_destroy(cache);
}

TEST(lfu_frequency_tracking) {
    LFUCache *cache = lfu_create(3);
    lfu_put(cache, 1, 100);

    ASSERT_EQ(1, lfu_get_frequency(cache, 1));

    int value;
    lfu_get(cache, 1, &value);
    ASSERT_EQ(2, lfu_get_frequency(cache, 1));

    lfu_get(cache, 1, &value);
    ASSERT_EQ(3, lfu_get_frequency(cache, 1));

    lfu_destroy(cache);
}

TEST(lfu_min_frequency) {
    LFUCache *cache = lfu_create(3);

    ASSERT_EQ(0, lfu_min_frequency(cache));

    lfu_put(cache, 1, 100);
    ASSERT_EQ(1, lfu_min_frequency(cache));

    int value;
    lfu_get(cache, 1, &value);
    ASSERT_EQ(2, lfu_min_frequency(cache));

    lfu_put(cache, 2, 200);
    ASSERT_EQ(1, lfu_min_frequency(cache));

    lfu_destroy(cache);
}

/* ============== LFU Cache Delete Tests ============== */

TEST(lfu_delete_existing) {
    LFUCache *cache = lfu_create(3);
    lfu_put(cache, 1, 100);
    lfu_put(cache, 2, 200);

    ASSERT_TRUE(lfu_delete(cache, 1));
    ASSERT_EQ(1, lfu_size(cache));
    ASSERT_FALSE(lfu_contains(cache, 1));
    lfu_destroy(cache);
}

TEST(lfu_delete_not_found) {
    LFUCache *cache = lfu_create(2);
    lfu_put(cache, 1, 100);

    ASSERT_FALSE(lfu_delete(cache, 2));
    ASSERT_EQ(1, lfu_size(cache));
    lfu_destroy(cache);
}

/* ============== LFU Cache Clear Tests ============== */

TEST(lfu_clear_test) {
    LFUCache *cache = lfu_create(3);
    lfu_put(cache, 1, 100);
    lfu_put(cache, 2, 200);

    lfu_clear(cache);
    ASSERT_EQ(0, lfu_size(cache));
    ASSERT_TRUE(lfu_is_empty(cache));
    ASSERT_FALSE(lfu_contains(cache, 1));
    lfu_destroy(cache);
}

/* ============== LFU Cache NULL Safety Tests ============== */

TEST(lfu_null_safety) {
    int value;

    ASSERT_FALSE(lfu_put(NULL, 1, 100));
    ASSERT_FALSE(lfu_get(NULL, 1, &value));
    ASSERT_FALSE(lfu_delete(NULL, 1));
    ASSERT_FALSE(lfu_contains(NULL, 1));
    ASSERT_EQ(0, lfu_size(NULL));
    ASSERT_EQ(0, lfu_capacity(NULL));
    ASSERT_TRUE(lfu_is_empty(NULL));
    ASSERT_FALSE(lfu_is_full(NULL));
    ASSERT_EQ(-1, lfu_get_frequency(NULL, 1));
    ASSERT_EQ(0, lfu_min_frequency(NULL));
}

/* ============== LFU LeetCode Test ============== */

TEST(lfu_leetcode_example) {
    /* LeetCode #460 example */
    LFUCache *cache = lfu_create(2);
    int value;

    lfu_put(cache, 1, 1);
    lfu_put(cache, 2, 2);

    ASSERT_TRUE(lfu_get(cache, 1, &value));  /* key 1 freq=2 */
    ASSERT_EQ(1, value);

    lfu_put(cache, 3, 3);  /* Evicts key 2 (freq=1, LRU) */

    ASSERT_FALSE(lfu_get(cache, 2, &value));

    ASSERT_TRUE(lfu_get(cache, 3, &value));  /* key 3 freq=2 */
    ASSERT_EQ(3, value);

    lfu_put(cache, 4, 4);  /* Evicts key 1 or 3 (both freq=2, key 1 is LRU) */

    ASSERT_FALSE(lfu_get(cache, 1, &value));
    ASSERT_TRUE(lfu_get(cache, 3, &value));
    ASSERT_EQ(3, value);
    ASSERT_TRUE(lfu_get(cache, 4, &value));
    ASSERT_EQ(4, value);

    lfu_destroy(cache);
}

/* ============== LRU Stress Test ============== */

TEST(lru_stress_test) {
    LRUCache *cache = lru_create(100);
    int value;

    /* Insert many items */
    for (int i = 0; i < 1000; i++) {
        lru_put(cache, i, i * 10);
    }

    ASSERT_EQ(100, lru_size(cache));
    ASSERT_TRUE(lru_is_full(cache));

    /* Only last 100 should be present */
    for (int i = 0; i < 900; i++) {
        ASSERT_FALSE(lru_contains(cache, i));
    }
    for (int i = 900; i < 1000; i++) {
        ASSERT_TRUE(lru_get(cache, i, &value));
        ASSERT_EQ(i * 10, value);
    }

    lru_destroy(cache);
}

/* ============== LFU Stress Test ============== */

TEST(lfu_stress_test) {
    LFUCache *cache = lfu_create(100);
    int value;

    /* Insert many items */
    for (int i = 0; i < 1000; i++) {
        lfu_put(cache, i, i * 10);
    }

    ASSERT_EQ(100, lfu_size(cache));
    ASSERT_TRUE(lfu_is_full(cache));

    /* Only last 100 should be present (all have same freq) */
    for (int i = 0; i < 900; i++) {
        ASSERT_FALSE(lfu_contains(cache, i));
    }
    for (int i = 900; i < 1000; i++) {
        ASSERT_TRUE(lfu_get(cache, i, &value));
    }

    lfu_destroy(cache);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("LRU/LFU Cache");

    /* LRU Creation */
    RUN_TEST(lru_create_empty);
    RUN_TEST(lru_create_zero_capacity);

    /* LRU Put/Get */
    RUN_TEST(lru_put_single);
    RUN_TEST(lru_put_update);
    RUN_TEST(lru_eviction_basic);
    RUN_TEST(lru_get_updates_recency);
    RUN_TEST(lru_get_not_found);

    /* LRU Delete */
    RUN_TEST(lru_delete_existing);
    RUN_TEST(lru_delete_not_found);

    /* LRU Inspection */
    RUN_TEST(lru_peek_newest);
    RUN_TEST(lru_peek_oldest);
    RUN_TEST(lru_get_keys);

    /* LRU Clear */
    RUN_TEST(lru_clear_test);

    /* LRU NULL Safety */
    RUN_TEST(lru_null_safety);

    /* LRU LeetCode */
    RUN_TEST(lru_leetcode_example);

    /* LFU Creation */
    RUN_TEST(lfu_create_empty);
    RUN_TEST(lfu_create_zero_capacity);

    /* LFU Put/Get */
    RUN_TEST(lfu_put_single);
    RUN_TEST(lfu_put_update);
    RUN_TEST(lfu_eviction_basic);
    RUN_TEST(lfu_eviction_by_frequency);
    RUN_TEST(lfu_frequency_tracking);
    RUN_TEST(lfu_min_frequency);

    /* LFU Delete */
    RUN_TEST(lfu_delete_existing);
    RUN_TEST(lfu_delete_not_found);

    /* LFU Clear */
    RUN_TEST(lfu_clear_test);

    /* LFU NULL Safety */
    RUN_TEST(lfu_null_safety);

    /* LFU LeetCode */
    RUN_TEST(lfu_leetcode_example);

    /* Stress Tests */
    RUN_TEST(lru_stress_test);
    RUN_TEST(lfu_stress_test);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
