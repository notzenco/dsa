/**
 * Tests for TTL Cache
 */

#include "test_framework.h"
#include "../data-structures/ttl_cache.h"
#include <stdlib.h>

/* Mock time for testing */
static double mock_time = 0.0;

static double get_mock_time(void) {
    return mock_time;
}

static void advance_time(double seconds) {
    mock_time += seconds;
}

static void reset_mock_time(void) {
    mock_time = 0.0;
}

/* ============== Creation Tests ============== */

TEST(ttl_create_empty) {
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);
    ASSERT_NOT_NULL(cache);
    ASSERT_EQ(0, ttl_size_dirty(cache));
    ASSERT_TRUE(ttl_is_empty(cache));
    ASSERT_EQ(10, ttl_capacity(cache));
    ttl_destroy(cache);
}

TEST(ttl_create_zero_capacity) {
    TTLCache *cache = ttl_create(0, 5.0);
    ASSERT_NULL(cache);
}

/* ============== Put and Get Tests ============== */

TEST(ttl_put_single) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ASSERT_TRUE(ttl_put(cache, 1, 100));
    ASSERT_EQ(1, ttl_size_dirty(cache));

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));
    ASSERT_EQ(100, value);

    ttl_destroy(cache);
}

TEST(ttl_put_multiple) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);
    ttl_put(cache, 3, 300);

    ASSERT_EQ(3, ttl_size_dirty(cache));

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));
    ASSERT_EQ(100, value);
    ASSERT_TRUE(ttl_get(cache, 2, &value));
    ASSERT_EQ(200, value);
    ASSERT_TRUE(ttl_get(cache, 3, &value));
    ASSERT_EQ(300, value);

    ttl_destroy(cache);
}

TEST(ttl_put_update) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 1, 200);

    ASSERT_EQ(1, ttl_size_dirty(cache));

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));
    ASSERT_EQ(200, value);

    ttl_destroy(cache);
}

TEST(ttl_get_nonexistent) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    int value;
    ASSERT_FALSE(ttl_get(cache, 42, &value));

    ttl_destroy(cache);
}

/* ============== Expiration Tests ============== */

TEST(ttl_entry_expires) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));

    /* Advance time past TTL */
    advance_time(6.0);

    ASSERT_FALSE(ttl_get(cache, 1, &value));
    ASSERT_EQ(0, ttl_size_dirty(cache));

    ttl_destroy(cache);
}

TEST(ttl_entry_not_expired) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);

    /* Advance time but not past TTL */
    advance_time(4.0);

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));
    ASSERT_EQ(100, value);

    ttl_destroy(cache);
}

TEST(ttl_custom_ttl) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 10.0, get_mock_time);

    /* Put with custom shorter TTL */
    ttl_put_with_ttl(cache, 1, 100, 2.0);

    advance_time(3.0);

    int value;
    ASSERT_FALSE(ttl_get(cache, 1, &value));

    ttl_destroy(cache);
}

TEST(ttl_update_resets_ttl) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);

    advance_time(4.0);

    /* Update resets TTL */
    ttl_put(cache, 1, 200);

    advance_time(4.0);

    /* Should still be valid (8 seconds total, but TTL reset at 4s) */
    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));
    ASSERT_EQ(200, value);

    ttl_destroy(cache);
}

/* ============== LRU Eviction Tests ============== */

TEST(ttl_evict_lru_on_capacity) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(3, 100.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);
    ttl_put(cache, 3, 300);

    /* Cache is full, adding new entry should evict LRU (key 1) */
    ttl_put(cache, 4, 400);

    ASSERT_EQ(3, ttl_size_dirty(cache));

    int value;
    ASSERT_FALSE(ttl_get(cache, 1, &value));  /* Evicted */
    ASSERT_TRUE(ttl_get(cache, 2, &value));
    ASSERT_TRUE(ttl_get(cache, 3, &value));
    ASSERT_TRUE(ttl_get(cache, 4, &value));

    ttl_destroy(cache);
}

TEST(ttl_access_updates_lru) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(3, 100.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);
    ttl_put(cache, 3, 300);

    /* Access key 1, making it most recently used */
    int value;
    ttl_get(cache, 1, &value);

    /* Add new entry, should evict key 2 (now LRU) */
    ttl_put(cache, 4, 400);

    ASSERT_TRUE(ttl_get(cache, 1, &value));   /* Still present */
    ASSERT_FALSE(ttl_get(cache, 2, &value));  /* Evicted */
    ASSERT_TRUE(ttl_get(cache, 3, &value));
    ASSERT_TRUE(ttl_get(cache, 4, &value));

    ttl_destroy(cache);
}

/* ============== Delete Tests ============== */

TEST(ttl_delete_existing) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ASSERT_TRUE(ttl_delete(cache, 1));
    ASSERT_EQ(0, ttl_size_dirty(cache));

    int value;
    ASSERT_FALSE(ttl_get(cache, 1, &value));

    ttl_destroy(cache);
}

TEST(ttl_delete_nonexistent) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ASSERT_FALSE(ttl_delete(cache, 42));

    ttl_destroy(cache);
}

/* ============== Contains Tests ============== */

TEST(ttl_contains_existing) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ASSERT_TRUE(ttl_contains(cache, 1));

    ttl_destroy(cache);
}

TEST(ttl_contains_expired) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    advance_time(6.0);

    ASSERT_FALSE(ttl_contains(cache, 1));

    ttl_destroy(cache);
}

/* ============== Cleanup Tests ============== */

TEST(ttl_cleanup_expired) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);

    advance_time(3.0);
    ttl_put(cache, 3, 300);  /* Added later, expires later */

    advance_time(3.0);  /* Total: 6 seconds */

    /* Keys 1 and 2 should be expired, key 3 still valid */
    size_t removed = ttl_cleanup(cache);
    ASSERT_EQ(2, removed);
    ASSERT_EQ(1, ttl_size_dirty(cache));

    int value;
    ASSERT_FALSE(ttl_get(cache, 1, &value));
    ASSERT_FALSE(ttl_get(cache, 2, &value));
    ASSERT_TRUE(ttl_get(cache, 3, &value));

    ttl_destroy(cache);
}

/* ============== TTL Query Tests ============== */

TEST(ttl_get_remaining_ttl) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 10.0, get_mock_time);

    ttl_put(cache, 1, 100);
    advance_time(3.0);

    double remaining;
    ASSERT_TRUE(ttl_get_ttl(cache, 1, &remaining));
    ASSERT_EQ(7.0, remaining);

    ttl_destroy(cache);
}

TEST(ttl_refresh) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    advance_time(4.0);

    ASSERT_TRUE(ttl_refresh(cache, 1));

    /* TTL should be reset to 5 seconds from now */
    advance_time(4.0);

    int value;
    ASSERT_TRUE(ttl_get(cache, 1, &value));

    ttl_destroy(cache);
}

/* ============== Clear Tests ============== */

TEST(ttl_clear) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);
    ttl_put(cache, 3, 300);

    ttl_clear(cache);

    ASSERT_TRUE(ttl_is_empty(cache));
    ASSERT_EQ(0, ttl_size_dirty(cache));

    /* Should be able to add new entries after clear */
    ASSERT_TRUE(ttl_put(cache, 4, 400));

    int value;
    ASSERT_TRUE(ttl_get(cache, 4, &value));
    ASSERT_EQ(400, value);

    ttl_destroy(cache);
}

/* ============== Edge Cases ============== */

TEST(ttl_null_cache) {
    int value;
    ASSERT_FALSE(ttl_get(NULL, 1, &value));
    ASSERT_FALSE(ttl_put(NULL, 1, 100));
    ASSERT_FALSE(ttl_delete(NULL, 1));
    ASSERT_FALSE(ttl_contains(NULL, 1));
    ASSERT_EQ(0, ttl_cleanup(NULL));
}

TEST(ttl_negative_keys) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(10, 5.0, get_mock_time);

    ttl_put(cache, -1, 100);
    ttl_put(cache, -100, 200);

    int value;
    ASSERT_TRUE(ttl_get(cache, -1, &value));
    ASSERT_EQ(100, value);
    ASSERT_TRUE(ttl_get(cache, -100, &value));
    ASSERT_EQ(200, value);

    ttl_destroy(cache);
}

TEST(ttl_single_capacity) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(1, 5.0, get_mock_time);

    ttl_put(cache, 1, 100);
    ttl_put(cache, 2, 200);

    ASSERT_EQ(1, ttl_size_dirty(cache));

    int value;
    ASSERT_FALSE(ttl_get(cache, 1, &value));  /* Evicted */
    ASSERT_TRUE(ttl_get(cache, 2, &value));
    ASSERT_EQ(200, value);

    ttl_destroy(cache);
}

TEST(ttl_many_entries) {
    reset_mock_time();
    TTLCache *cache = ttl_create_with_time_func(100, 10.0, get_mock_time);

    for (int i = 0; i < 100; i++) {
        ASSERT_TRUE(ttl_put(cache, i, i * 10));
    }

    ASSERT_EQ(100, ttl_size_dirty(cache));
    ASSERT_TRUE(ttl_is_full(cache));

    int value;
    for (int i = 0; i < 100; i++) {
        ASSERT_TRUE(ttl_get(cache, i, &value));
        ASSERT_EQ(i * 10, value);
    }

    ttl_destroy(cache);
}

/* ============== Main ============== */

int main(void) {
    TEST_SUITE_START("TTL Cache Tests");

    /* Creation tests */
    RUN_TEST(ttl_create_empty);
    RUN_TEST(ttl_create_zero_capacity);

    /* Put and get tests */
    RUN_TEST(ttl_put_single);
    RUN_TEST(ttl_put_multiple);
    RUN_TEST(ttl_put_update);
    RUN_TEST(ttl_get_nonexistent);

    /* Expiration tests */
    RUN_TEST(ttl_entry_expires);
    RUN_TEST(ttl_entry_not_expired);
    RUN_TEST(ttl_custom_ttl);
    RUN_TEST(ttl_update_resets_ttl);

    /* LRU eviction tests */
    RUN_TEST(ttl_evict_lru_on_capacity);
    RUN_TEST(ttl_access_updates_lru);

    /* Delete tests */
    RUN_TEST(ttl_delete_existing);
    RUN_TEST(ttl_delete_nonexistent);

    /* Contains tests */
    RUN_TEST(ttl_contains_existing);
    RUN_TEST(ttl_contains_expired);

    /* Cleanup tests */
    RUN_TEST(ttl_cleanup_expired);

    /* TTL query tests */
    RUN_TEST(ttl_get_remaining_ttl);
    RUN_TEST(ttl_refresh);

    /* Clear tests */
    RUN_TEST(ttl_clear);

    /* Edge cases */
    RUN_TEST(ttl_null_cache);
    RUN_TEST(ttl_negative_keys);
    RUN_TEST(ttl_single_capacity);
    RUN_TEST(ttl_many_entries);

    TEST_SUITE_END();
}
