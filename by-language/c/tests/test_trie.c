/**
 * Tests for Trie (Prefix Tree)
 */

#include "test_framework.h"
#include "../data-structures/trie.h"
#include <stdlib.h>
#include <string.h>

/* ============== Creation Tests ============== */

TEST(trie_create_empty) {
    Trie *trie = trie_create();
    ASSERT_NOT_NULL(trie);
    ASSERT_EQ(0, trie_word_count(trie));
    ASSERT_TRUE(trie_is_empty(trie));
    trie_destroy(trie);
}

/* ============== Insert Tests ============== */

TEST(trie_insert_single) {
    Trie *trie = trie_create();
    ASSERT_TRUE(trie_insert(trie, "hello"));
    ASSERT_EQ(1, trie_word_count(trie));
    ASSERT_TRUE(trie_search(trie, "hello"));
    trie_destroy(trie);
}

TEST(trie_insert_multiple) {
    Trie *trie = trie_create();
    ASSERT_TRUE(trie_insert(trie, "hello"));
    ASSERT_TRUE(trie_insert(trie, "world"));
    ASSERT_TRUE(trie_insert(trie, "help"));
    ASSERT_EQ(3, trie_word_count(trie));
    trie_destroy(trie);
}

TEST(trie_insert_duplicate) {
    Trie *trie = trie_create();
    ASSERT_TRUE(trie_insert(trie, "hello"));
    ASSERT_TRUE(trie_insert(trie, "hello")); /* Duplicate */
    ASSERT_EQ(1, trie_word_count(trie));
    trie_destroy(trie);
}

TEST(trie_insert_prefix) {
    Trie *trie = trie_create();
    ASSERT_TRUE(trie_insert(trie, "app"));
    ASSERT_TRUE(trie_insert(trie, "apple"));
    ASSERT_EQ(2, trie_word_count(trie));
    ASSERT_TRUE(trie_search(trie, "app"));
    ASSERT_TRUE(trie_search(trie, "apple"));
    trie_destroy(trie);
}

/* ============== Search Tests ============== */

TEST(trie_search_found) {
    Trie *trie = trie_create();
    trie_insert(trie, "hello");
    trie_insert(trie, "world");
    ASSERT_TRUE(trie_search(trie, "hello"));
    ASSERT_TRUE(trie_search(trie, "world"));
    trie_destroy(trie);
}

TEST(trie_search_not_found) {
    Trie *trie = trie_create();
    trie_insert(trie, "hello");
    ASSERT_FALSE(trie_search(trie, "hell"));
    ASSERT_FALSE(trie_search(trie, "helloworld"));
    ASSERT_FALSE(trie_search(trie, "world"));
    trie_destroy(trie);
}

TEST(trie_search_prefix_not_word) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");
    ASSERT_FALSE(trie_search(trie, "app")); /* Prefix but not inserted */
    ASSERT_TRUE(trie_search(trie, "apple"));
    trie_destroy(trie);
}

/* ============== Starts With Tests ============== */

TEST(trie_starts_with_true) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");
    trie_insert(trie, "application");
    ASSERT_TRUE(trie_starts_with(trie, "app"));
    ASSERT_TRUE(trie_starts_with(trie, "appl"));
    ASSERT_TRUE(trie_starts_with(trie, "apple"));
    trie_destroy(trie);
}

TEST(trie_starts_with_false) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");
    ASSERT_FALSE(trie_starts_with(trie, "apl"));
    ASSERT_FALSE(trie_starts_with(trie, "banana"));
    trie_destroy(trie);
}

/* ============== Delete Tests ============== */

TEST(trie_delete_single) {
    Trie *trie = trie_create();
    trie_insert(trie, "hello");
    ASSERT_TRUE(trie_delete(trie, "hello"));
    ASSERT_FALSE(trie_search(trie, "hello"));
    ASSERT_EQ(0, trie_word_count(trie));
    trie_destroy(trie);
}

TEST(trie_delete_preserves_prefix) {
    Trie *trie = trie_create();
    trie_insert(trie, "app");
    trie_insert(trie, "apple");
    ASSERT_TRUE(trie_delete(trie, "app"));
    ASSERT_FALSE(trie_search(trie, "app"));
    ASSERT_TRUE(trie_search(trie, "apple"));
    ASSERT_EQ(1, trie_word_count(trie));
    trie_destroy(trie);
}

TEST(trie_delete_not_found) {
    Trie *trie = trie_create();
    trie_insert(trie, "hello");
    ASSERT_FALSE(trie_delete(trie, "world"));
    ASSERT_EQ(1, trie_word_count(trie));
    trie_destroy(trie);
}

/* ============== Count Prefix Tests ============== */

TEST(trie_count_prefix_basic) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");
    trie_insert(trie, "application");
    trie_insert(trie, "apply");
    trie_insert(trie, "banana");
    ASSERT_EQ(3, trie_count_prefix(trie, "app"));
    ASSERT_EQ(1, trie_count_prefix(trie, "ban"));
    ASSERT_EQ(0, trie_count_prefix(trie, "xyz"));
    trie_destroy(trie);
}

/* ============== Autocomplete Tests ============== */

TEST(trie_autocomplete_basic) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");
    trie_insert(trie, "application");
    trie_insert(trie, "apply");
    trie_insert(trie, "banana");

    char *words[10];
    int count = trie_autocomplete(trie, "app", words, 10);
    ASSERT_EQ(3, count);

    /* Free allocated strings */
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    trie_destroy(trie);
}

TEST(trie_autocomplete_no_results) {
    Trie *trie = trie_create();
    trie_insert(trie, "apple");

    char *words[10];
    int count = trie_autocomplete(trie, "xyz", words, 10);
    ASSERT_EQ(0, count);
    trie_destroy(trie);
}

/* ============== Get All Words Tests ============== */

TEST(trie_get_all_words_basic) {
    Trie *trie = trie_create();
    trie_insert(trie, "cat");
    trie_insert(trie, "car");
    trie_insert(trie, "dog");

    char *words[10];
    int count = trie_get_all_words(trie, words, 10);
    ASSERT_EQ(3, count);

    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    trie_destroy(trie);
}

/* ============== Wildcard Search Tests ============== */

TEST(trie_wildcard_search_found) {
    Trie *trie = trie_create();
    trie_insert(trie, "bad");
    trie_insert(trie, "dad");
    trie_insert(trie, "mad");

    ASSERT_TRUE(trie_search_wildcard(trie, ".ad"));
    ASSERT_TRUE(trie_search_wildcard(trie, "b.."));
    ASSERT_TRUE(trie_search_wildcard(trie, "..."));
    trie_destroy(trie);
}

TEST(trie_wildcard_search_not_found) {
    Trie *trie = trie_create();
    trie_insert(trie, "bad");

    ASSERT_FALSE(trie_search_wildcard(trie, "...."));
    ASSERT_FALSE(trie_search_wildcard(trie, "b.x"));
    trie_destroy(trie);
}

/* ============== Longest Common Prefix Tests ============== */

TEST(trie_lcp_basic) {
    Trie *trie = trie_create();
    trie_insert(trie, "flower");
    trie_insert(trie, "flow");
    trie_insert(trie, "flight");

    char result[20];
    int len = trie_longest_common_prefix(trie, result, 20);
    ASSERT_EQ(2, len);
    ASSERT_EQ(0, strcmp("fl", result));
    trie_destroy(trie);
}

TEST(trie_lcp_no_common) {
    Trie *trie = trie_create();
    trie_insert(trie, "dog");
    trie_insert(trie, "cat");

    char result[20];
    int len = trie_longest_common_prefix(trie, result, 20);
    ASSERT_EQ(0, len);
    trie_destroy(trie);
}

/* ============== Shortest Unique Prefix Tests ============== */

TEST(trie_shortest_unique_prefix_basic) {
    Trie *trie = trie_create();
    trie_insert(trie, "zebra");
    trie_insert(trie, "dog");
    trie_insert(trie, "duck");
    trie_insert(trie, "dove");

    char result[20];
    int len = trie_shortest_unique_prefix(trie, "zebra", result, 20);
    ASSERT_EQ(1, len);
    ASSERT_EQ(0, strcmp("z", result));

    len = trie_shortest_unique_prefix(trie, "dog", result, 20);
    ASSERT_EQ(3, len);
    ASSERT_EQ(0, strcmp("dog", result));
    trie_destroy(trie);
}

/* ============== Clear Tests ============== */

TEST(trie_clear_test) {
    Trie *trie = trie_create();
    trie_insert(trie, "hello");
    trie_insert(trie, "world");
    ASSERT_EQ(2, trie_word_count(trie));

    trie_clear(trie);
    ASSERT_EQ(0, trie_word_count(trie));
    ASSERT_TRUE(trie_is_empty(trie));
    ASSERT_FALSE(trie_search(trie, "hello"));
    trie_destroy(trie);
}

/* ============== NULL Safety Tests ============== */

TEST(trie_null_safety) {
    ASSERT_FALSE(trie_insert(NULL, "test"));
    ASSERT_FALSE(trie_search(NULL, "test"));
    ASSERT_FALSE(trie_delete(NULL, "test"));
    ASSERT_FALSE(trie_starts_with(NULL, "test"));
    ASSERT_EQ(0, trie_count_prefix(NULL, "test"));
    ASSERT_EQ(0, trie_word_count(NULL));
    ASSERT_TRUE(trie_is_empty(NULL));
}

/* ============== Case Sensitivity Tests ============== */

TEST(trie_case_insensitive) {
    Trie *trie = trie_create();
    trie_insert(trie, "Hello");
    ASSERT_TRUE(trie_search(trie, "hello"));
    ASSERT_TRUE(trie_search(trie, "HELLO"));
    ASSERT_TRUE(trie_search(trie, "HeLLo"));
    trie_destroy(trie);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("Trie (Prefix Tree)");

    /* Creation */
    RUN_TEST(trie_create_empty);

    /* Insert */
    RUN_TEST(trie_insert_single);
    RUN_TEST(trie_insert_multiple);
    RUN_TEST(trie_insert_duplicate);
    RUN_TEST(trie_insert_prefix);

    /* Search */
    RUN_TEST(trie_search_found);
    RUN_TEST(trie_search_not_found);
    RUN_TEST(trie_search_prefix_not_word);

    /* Starts With */
    RUN_TEST(trie_starts_with_true);
    RUN_TEST(trie_starts_with_false);

    /* Delete */
    RUN_TEST(trie_delete_single);
    RUN_TEST(trie_delete_preserves_prefix);
    RUN_TEST(trie_delete_not_found);

    /* Count Prefix */
    RUN_TEST(trie_count_prefix_basic);

    /* Autocomplete */
    RUN_TEST(trie_autocomplete_basic);
    RUN_TEST(trie_autocomplete_no_results);

    /* Get All Words */
    RUN_TEST(trie_get_all_words_basic);

    /* Wildcard Search */
    RUN_TEST(trie_wildcard_search_found);
    RUN_TEST(trie_wildcard_search_not_found);

    /* Longest Common Prefix */
    RUN_TEST(trie_lcp_basic);
    RUN_TEST(trie_lcp_no_common);

    /* Shortest Unique Prefix */
    RUN_TEST(trie_shortest_unique_prefix_basic);

    /* Clear */
    RUN_TEST(trie_clear_test);

    /* NULL Safety */
    RUN_TEST(trie_null_safety);

    /* Case */
    RUN_TEST(trie_case_insensitive);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
