/**
 * Tests for Bloom Filter
 */

#include "test_framework.h"
#include "../data-structures/bloom_filter.h"
#include <stdlib.h>
#include <string.h>

/* ============== Creation Tests ============== */

TEST(bloom_create_basic) {
    BloomFilter *bf = bloom_create(1000, 3);
    ASSERT_NOT_NULL(bf);
    ASSERT_EQ(1000, bloom_num_bits(bf));
    ASSERT_EQ(3, bloom_num_hashes(bf));
    ASSERT_EQ(0, bloom_count(bf));
    bloom_destroy(bf);
}

TEST(bloom_create_optimal) {
    BloomFilter *bf = bloom_create_optimal(100, 0.01);
    ASSERT_NOT_NULL(bf);
    ASSERT_TRUE(bloom_num_bits(bf) > 0);
    ASSERT_TRUE(bloom_num_hashes(bf) > 0);
    bloom_destroy(bf);
}

TEST(bloom_create_invalid) {
    ASSERT_TRUE(bloom_create(0, 3) == NULL);
    ASSERT_TRUE(bloom_create(100, 0) == NULL);
    ASSERT_TRUE(bloom_create_optimal(0, 0.01) == NULL);
    ASSERT_TRUE(bloom_create_optimal(100, 0) == NULL);
    ASSERT_TRUE(bloom_create_optimal(100, 1.0) == NULL);
}

/* ============== Add and Contains Tests ============== */

TEST(bloom_add_single) {
    BloomFilter *bf = bloom_create(1000, 3);
    bloom_add(bf, "hello");
    ASSERT_EQ(1, bloom_count(bf));
    ASSERT_TRUE(bloom_contains(bf, "hello"));
    bloom_destroy(bf);
}

TEST(bloom_add_multiple) {
    BloomFilter *bf = bloom_create(1000, 5);

    bloom_add(bf, "apple");
    bloom_add(bf, "banana");
    bloom_add(bf, "cherry");

    ASSERT_EQ(3, bloom_count(bf));
    ASSERT_TRUE(bloom_contains(bf, "apple"));
    ASSERT_TRUE(bloom_contains(bf, "banana"));
    ASSERT_TRUE(bloom_contains(bf, "cherry"));
    bloom_destroy(bf);
}

TEST(bloom_not_contains) {
    BloomFilter *bf = bloom_create(10000, 7);

    bloom_add(bf, "hello");
    bloom_add(bf, "world");

    /* With good parameters, these should not be found */
    ASSERT_FALSE(bloom_contains(bf, "xyz123"));
    ASSERT_FALSE(bloom_contains(bf, "notadded"));
    bloom_destroy(bf);
}

TEST(bloom_add_bytes) {
    BloomFilter *bf = bloom_create(1000, 3);

    int data1 = 12345;
    int data2 = 67890;

    bloom_add_bytes(bf, &data1, sizeof(data1));
    ASSERT_TRUE(bloom_contains_bytes(bf, &data1, sizeof(data1)));
    ASSERT_FALSE(bloom_contains_bytes(bf, &data2, sizeof(data2)));

    bloom_destroy(bf);
}

/* ============== No False Negatives Tests ============== */

TEST(bloom_no_false_negatives) {
    BloomFilter *bf = bloom_create(10000, 7);

    const char *words[] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "imbe", "jackfruit"
    };
    int n = 10;

    for (int i = 0; i < n; i++) {
        bloom_add(bf, words[i]);
    }

    /* All added words MUST be found (no false negatives) */
    for (int i = 0; i < n; i++) {
        ASSERT_TRUE(bloom_contains(bf, words[i]));
    }

    bloom_destroy(bf);
}

/* ============== Clear Tests ============== */

TEST(bloom_clear_test) {
    BloomFilter *bf = bloom_create(1000, 3);

    bloom_add(bf, "hello");
    bloom_add(bf, "world");
    ASSERT_EQ(2, bloom_count(bf));

    bloom_clear(bf);
    ASSERT_EQ(0, bloom_count(bf));
    ASSERT_FALSE(bloom_contains(bf, "hello"));
    ASSERT_FALSE(bloom_contains(bf, "world"));

    bloom_destroy(bf);
}

/* ============== Merge Tests ============== */

TEST(bloom_merge_test) {
    BloomFilter *bf1 = bloom_create(1000, 3);
    BloomFilter *bf2 = bloom_create(1000, 3);

    bloom_add(bf1, "hello");
    bloom_add(bf2, "world");

    ASSERT_TRUE(bloom_merge(bf1, bf2));

    ASSERT_TRUE(bloom_contains(bf1, "hello"));
    ASSERT_TRUE(bloom_contains(bf1, "world"));

    bloom_destroy(bf1);
    bloom_destroy(bf2);
}

TEST(bloom_merge_incompatible) {
    BloomFilter *bf1 = bloom_create(1000, 3);
    BloomFilter *bf2 = bloom_create(2000, 3);  /* Different size */

    ASSERT_FALSE(bloom_merge(bf1, bf2));

    bloom_destroy(bf1);
    bloom_destroy(bf2);
}

/* ============== Statistics Tests ============== */

TEST(bloom_bits_set) {
    BloomFilter *bf = bloom_create(100, 3);

    ASSERT_EQ(0, bloom_bits_set(bf));

    bloom_add(bf, "hello");
    ASSERT_TRUE(bloom_bits_set(bf) > 0);
    ASSERT_TRUE(bloom_bits_set(bf) <= 3);  /* At most 3 bits per element */

    bloom_destroy(bf);
}

TEST(bloom_false_positive_rate) {
    BloomFilter *bf = bloom_create_optimal(100, 0.01);

    /* Initially 0% FP rate */
    ASSERT_TRUE(bloom_false_positive_rate(bf) < 0.001);

    /* Add elements */
    for (int i = 0; i < 50; i++) {
        char buf[20];
        snprintf(buf, sizeof(buf), "word%d", i);
        bloom_add(bf, buf);
    }

    /* FP rate should still be reasonable */
    ASSERT_TRUE(bloom_false_positive_rate(bf) < 0.1);

    bloom_destroy(bf);
}

/* ============== NULL Safety Tests ============== */

TEST(bloom_null_safety) {
    ASSERT_FALSE(bloom_contains(NULL, "test"));
    ASSERT_EQ(0, bloom_count(NULL));
    ASSERT_EQ(0, bloom_num_bits(NULL));
    ASSERT_EQ(0, bloom_num_hashes(NULL));
    ASSERT_EQ(0, bloom_bits_set(NULL));
    ASSERT_FALSE(bloom_merge(NULL, NULL));
}

/* ============== Stress Tests ============== */

TEST(bloom_stress_test) {
    BloomFilter *bf = bloom_create_optimal(10000, 0.01);

    /* Add many elements */
    for (int i = 0; i < 10000; i++) {
        char buf[20];
        snprintf(buf, sizeof(buf), "item%d", i);
        bloom_add(bf, buf);
    }

    ASSERT_EQ(10000, bloom_count(bf));

    /* Check all are found (no false negatives) */
    for (int i = 0; i < 10000; i++) {
        char buf[20];
        snprintf(buf, sizeof(buf), "item%d", i);
        ASSERT_TRUE(bloom_contains(bf, buf));
    }

    /* Count false positives on items not added */
    int false_positives = 0;
    for (int i = 10000; i < 11000; i++) {
        char buf[20];
        snprintf(buf, sizeof(buf), "item%d", i);
        if (bloom_contains(bf, buf)) {
            false_positives++;
        }
    }

    /* False positive rate should be around 1% (allow some margin) */
    ASSERT_TRUE(false_positives < 50);  /* < 5% */

    bloom_destroy(bf);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Bloom Filter");

    /* Creation */
    RUN_TEST(bloom_create_basic);
    RUN_TEST(bloom_create_optimal);
    RUN_TEST(bloom_create_invalid);

    /* Add and Contains */
    RUN_TEST(bloom_add_single);
    RUN_TEST(bloom_add_multiple);
    RUN_TEST(bloom_not_contains);
    RUN_TEST(bloom_add_bytes);

    /* No False Negatives */
    RUN_TEST(bloom_no_false_negatives);

    /* Clear */
    RUN_TEST(bloom_clear_test);

    /* Merge */
    RUN_TEST(bloom_merge_test);
    RUN_TEST(bloom_merge_incompatible);

    /* Statistics */
    RUN_TEST(bloom_bits_set);
    RUN_TEST(bloom_false_positive_rate);

    /* NULL Safety */
    RUN_TEST(bloom_null_safety);

    /* Stress */
    RUN_TEST(bloom_stress_test);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
