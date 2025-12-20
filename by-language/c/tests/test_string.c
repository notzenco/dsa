/**
 * Tests for String Algorithms
 */

#include "test_framework.h"
#include "../algorithms/string_algorithms.h"
#include <stdlib.h>
#include <string.h>

/* ============== Naive Search Tests ============== */

TEST(naive_search_found) {
    ASSERT_EQ(0, naive_search("hello world", "hello"));
    ASSERT_EQ(6, naive_search("hello world", "world"));
    ASSERT_EQ(2, naive_search("abcabc", "cab"));
}

TEST(naive_search_not_found) {
    ASSERT_EQ(-1, naive_search("hello world", "xyz"));
    ASSERT_EQ(-1, naive_search("abc", "abcd"));
}

TEST(naive_search_empty) {
    ASSERT_EQ(0, naive_search("hello", ""));
    ASSERT_EQ(-1, naive_search("", "hello"));
}

TEST(naive_search_all_basic) {
    int indices[10];
    int count = naive_search_all("abababa", "aba", indices, 10);
    ASSERT_EQ(3, count);
    ASSERT_EQ(0, indices[0]);
    ASSERT_EQ(2, indices[1]);
    ASSERT_EQ(4, indices[2]);
}

/* ============== KMP Tests ============== */

TEST(kmp_search_found) {
    ASSERT_EQ(0, kmp_search("hello world", "hello"));
    ASSERT_EQ(6, kmp_search("hello world", "world"));
    ASSERT_EQ(2, kmp_search("abcabc", "cab"));
}

TEST(kmp_search_not_found) {
    ASSERT_EQ(-1, kmp_search("hello world", "xyz"));
    ASSERT_EQ(-1, kmp_search("abc", "abcd"));
}

TEST(kmp_search_pattern_match) {
    /* First occurrence of "aaba" is at index 0 */
    ASSERT_EQ(0, kmp_search("aabaacaadaabaaabaa", "aaba"));
}

TEST(kmp_search_all_basic) {
    int indices[10];
    int count = kmp_search_all("abababa", "aba", indices, 10);
    ASSERT_EQ(3, count);
    ASSERT_EQ(0, indices[0]);
    ASSERT_EQ(2, indices[1]);
    ASSERT_EQ(4, indices[2]);
}

TEST(kmp_search_all_repeated) {
    int indices[10];
    int count = kmp_search_all("aaaaaa", "aa", indices, 10);
    ASSERT_EQ(5, count);
}

TEST(kmp_build_lps_basic) {
    int lps[6];
    kmp_build_lps("aabaab", lps, 6);
    ASSERT_EQ(0, lps[0]);
    ASSERT_EQ(1, lps[1]);
    ASSERT_EQ(0, lps[2]);
    ASSERT_EQ(1, lps[3]);
    ASSERT_EQ(2, lps[4]);
    ASSERT_EQ(3, lps[5]);
}

/* ============== Rabin-Karp Tests ============== */

TEST(rabin_karp_search_found) {
    ASSERT_EQ(0, rabin_karp_search("hello world", "hello"));
    ASSERT_EQ(6, rabin_karp_search("hello world", "world"));
}

TEST(rabin_karp_search_not_found) {
    ASSERT_EQ(-1, rabin_karp_search("hello world", "xyz"));
}

TEST(rabin_karp_search_all_basic) {
    int indices[10];
    int count = rabin_karp_search_all("abababa", "aba", indices, 10);
    ASSERT_EQ(3, count);
}

/* ============== Z-Algorithm Tests ============== */

TEST(z_function_basic) {
    int z[7];
    z_function("aabxaab", z, 7);
    /* z[0] is typically 0 or undefined */
    ASSERT_EQ(1, z[1]);  /* "ab..." vs "aabxaab" */
    ASSERT_EQ(0, z[2]);
    ASSERT_EQ(0, z[3]);
    ASSERT_EQ(3, z[4]);  /* "aab" matches prefix */
}

TEST(z_search_found) {
    ASSERT_EQ(0, z_search("hello world", "hello"));
    ASSERT_EQ(6, z_search("hello world", "world"));
}

TEST(z_search_not_found) {
    ASSERT_EQ(-1, z_search("hello world", "xyz"));
}

TEST(z_search_all_basic) {
    int indices[10];
    int count = z_search_all("abababa", "aba", indices, 10);
    ASSERT_EQ(3, count);
}

/* ============== String Properties Tests ============== */

TEST(is_palindrome_true) {
    ASSERT_TRUE(is_palindrome("racecar"));
    ASSERT_TRUE(is_palindrome("a"));
    ASSERT_TRUE(is_palindrome(""));
    ASSERT_TRUE(is_palindrome("abba"));
}

TEST(is_palindrome_false) {
    ASSERT_FALSE(is_palindrome("hello"));
    ASSERT_FALSE(is_palindrome("ab"));
}

TEST(is_repeated_pattern_true) {
    ASSERT_TRUE(is_repeated_pattern("abab"));
    ASSERT_TRUE(is_repeated_pattern("abcabc"));
    ASSERT_TRUE(is_repeated_pattern("aaaa"));
}

TEST(is_repeated_pattern_false) {
    ASSERT_FALSE(is_repeated_pattern("aba"));
    ASSERT_FALSE(is_repeated_pattern("abcd"));
    ASSERT_FALSE(is_repeated_pattern("a"));
}

TEST(longest_happy_prefix_basic) {
    /* "level" - only "l" matches as prefix and suffix */
    ASSERT_EQ(1, longest_happy_prefix("level"));
    /* "ababab" - "abab" is prefix and suffix */
    ASSERT_EQ(4, longest_happy_prefix("ababab"));
    ASSERT_EQ(0, longest_happy_prefix("abc"));
}

TEST(shortest_palindrome_basic) {
    char result[20];
    shortest_palindrome("aacecaaa", result);
    ASSERT_EQ(0, strcmp("aaacecaaa", result));
}

TEST(shortest_palindrome_already) {
    char result[20];
    shortest_palindrome("aba", result);
    ASSERT_EQ(0, strcmp("aba", result));
}

/* ============== String Transformation Tests ============== */

TEST(string_reverse_basic) {
    char s[] = "hello";
    string_reverse(s);
    ASSERT_EQ(0, strcmp("olleh", s));
}

TEST(string_reverse_single) {
    char s[] = "a";
    string_reverse(s);
    ASSERT_EQ(0, strcmp("a", s));
}

TEST(string_reverse_empty) {
    char s[] = "";
    string_reverse(s);
    ASSERT_EQ(0, strcmp("", s));
}

TEST(reverse_words_basic) {
    char s[] = "hello world";
    reverse_words(s);
    ASSERT_EQ(0, strcmp("world hello", s));
}

TEST(reverse_words_single) {
    char s[] = "hello";
    reverse_words(s);
    ASSERT_EQ(0, strcmp("hello", s));
}

TEST(are_anagrams_true) {
    ASSERT_TRUE(are_anagrams("listen", "silent"));
    ASSERT_TRUE(are_anagrams("anagram", "nagaram"));
    ASSERT_TRUE(are_anagrams("", ""));
}

TEST(are_anagrams_false) {
    ASSERT_FALSE(are_anagrams("hello", "world"));
    ASSERT_FALSE(are_anagrams("abc", "ab"));
}

TEST(is_rotation_true) {
    ASSERT_TRUE(is_rotation("abcde", "cdeab"));
    ASSERT_TRUE(is_rotation("waterbottle", "erbottlewat"));
    ASSERT_TRUE(is_rotation("", ""));
}

TEST(is_rotation_false) {
    ASSERT_FALSE(is_rotation("abcde", "abced"));
    ASSERT_FALSE(is_rotation("abc", "ab"));
}

/* ============== Utility Tests ============== */

TEST(count_pattern_basic) {
    ASSERT_EQ(3, count_pattern("abababa", "aba"));
    ASSERT_EQ(2, count_pattern("hello hello", "hello"));
    ASSERT_EQ(0, count_pattern("abc", "xyz"));
}

TEST(find_all_indices_basic) {
    int count;
    int *indices = find_all_indices("abababa", "aba", &count);
    ASSERT_NOT_NULL(indices);
    ASSERT_EQ(3, count);
    ASSERT_EQ(0, indices[0]);
    ASSERT_EQ(2, indices[1]);
    ASSERT_EQ(4, indices[2]);
    free(indices);
}

TEST(find_all_indices_not_found) {
    int count;
    int *indices = find_all_indices("abc", "xyz", &count);
    ASSERT_EQ(0, count);
    ASSERT_TRUE(indices == NULL);
}

/* ============== Cross-Algorithm Consistency ============== */

TEST(all_search_algorithms_agree) {
    const char *text = "abcabcabcabc";
    const char *pattern = "abc";

    int naive = naive_search(text, pattern);
    int kmp = kmp_search(text, pattern);
    int rk = rabin_karp_search(text, pattern);
    int z = z_search(text, pattern);

    ASSERT_EQ(naive, kmp);
    ASSERT_EQ(kmp, rk);
    ASSERT_EQ(rk, z);
    ASSERT_EQ(0, naive);
}

TEST(all_search_all_agree) {
    const char *text = "abababa";
    const char *pattern = "aba";

    int naive_idx[10], kmp_idx[10], rk_idx[10], z_idx[10];

    int naive_count = naive_search_all(text, pattern, naive_idx, 10);
    int kmp_count = kmp_search_all(text, pattern, kmp_idx, 10);
    int rk_count = rabin_karp_search_all(text, pattern, rk_idx, 10);
    int z_count = z_search_all(text, pattern, z_idx, 10);

    ASSERT_EQ(naive_count, kmp_count);
    ASSERT_EQ(kmp_count, rk_count);
    ASSERT_EQ(rk_count, z_count);
    ASSERT_EQ(3, naive_count);
}

/* Main test runner */
int main(void) {
    TEST_SUITE_START("String Algorithms");

    /* Naive Search */
    RUN_TEST(naive_search_found);
    RUN_TEST(naive_search_not_found);
    RUN_TEST(naive_search_empty);
    RUN_TEST(naive_search_all_basic);

    /* KMP */
    RUN_TEST(kmp_search_found);
    RUN_TEST(kmp_search_not_found);
    RUN_TEST(kmp_search_pattern_match);
    RUN_TEST(kmp_search_all_basic);
    RUN_TEST(kmp_search_all_repeated);
    RUN_TEST(kmp_build_lps_basic);

    /* Rabin-Karp */
    RUN_TEST(rabin_karp_search_found);
    RUN_TEST(rabin_karp_search_not_found);
    RUN_TEST(rabin_karp_search_all_basic);

    /* Z-Algorithm */
    RUN_TEST(z_function_basic);
    RUN_TEST(z_search_found);
    RUN_TEST(z_search_not_found);
    RUN_TEST(z_search_all_basic);

    /* String Properties */
    RUN_TEST(is_palindrome_true);
    RUN_TEST(is_palindrome_false);
    RUN_TEST(is_repeated_pattern_true);
    RUN_TEST(is_repeated_pattern_false);
    RUN_TEST(longest_happy_prefix_basic);
    RUN_TEST(shortest_palindrome_basic);
    RUN_TEST(shortest_palindrome_already);

    /* String Transformation */
    RUN_TEST(string_reverse_basic);
    RUN_TEST(string_reverse_single);
    RUN_TEST(string_reverse_empty);
    RUN_TEST(reverse_words_basic);
    RUN_TEST(reverse_words_single);
    RUN_TEST(are_anagrams_true);
    RUN_TEST(are_anagrams_false);
    RUN_TEST(is_rotation_true);
    RUN_TEST(is_rotation_false);

    /* Utility */
    RUN_TEST(count_pattern_basic);
    RUN_TEST(find_all_indices_basic);
    RUN_TEST(find_all_indices_not_found);

    /* Cross-algorithm consistency */
    RUN_TEST(all_search_algorithms_agree);
    RUN_TEST(all_search_all_agree);

    TEST_SUITE_END();

    return GET_FAILED_COUNT() > 0 ? 1 : 0;
}
