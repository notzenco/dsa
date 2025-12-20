/**
 * STRING ALGORITHMS
 *
 * Pattern matching and string processing algorithms.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         ALGORITHM COMPARISON                              ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Algorithm      │ Preprocess │ Search    │ Space   │ Best For            ║
 * ║  ────────────────┼────────────┼───────────┼─────────┼──────────────────   ║
 * ║  Naive          │ O(1)       │ O(n*m)    │ O(1)    │ Short patterns      ║
 * ║  KMP            │ O(m)       │ O(n)      │ O(m)    │ General purpose     ║
 * ║  Rabin-Karp     │ O(m)       │ O(n+m)*   │ O(1)    │ Multiple patterns   ║
 * ║  Z-Algorithm    │ O(n+m)     │ O(n+m)    │ O(n+m)  │ All occurrences     ║
 * ║  Boyer-Moore    │ O(m+σ)     │ O(n/m)*   │ O(m+σ)  │ Long patterns       ║
 * ║                                                                           ║
 * ║  * = average case                                                         ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #28 Find the Index of the First Occurrence in a String
 * - #459 Repeated Substring Pattern
 * - #214 Shortest Palindrome (KMP)
 * - #686 Repeated String Match (Rabin-Karp)
 * - #1392 Longest Happy Prefix (Z/KMP)
 */

#ifndef STRING_ALGORITHMS_H
#define STRING_ALGORITHMS_H

#include <stddef.h>
#include <stdbool.h>

/* ============== Pattern Matching ============== */

/**
 * Naive pattern matching - O(n*m)
 * @param text Text to search in
 * @param pattern Pattern to find
 * @return Index of first occurrence or -1
 */
int naive_search(const char *text, const char *pattern);

/**
 * Find all occurrences using naive search
 * @param text Text to search in
 * @param pattern Pattern to find
 * @param indices Output array for indices
 * @param max_results Maximum results to store
 * @return Number of occurrences found
 */
int naive_search_all(const char *text, const char *pattern,
                     int *indices, int max_results);

/**
 * KMP (Knuth-Morris-Pratt) - O(n+m)
 * @param text Text to search in
 * @param pattern Pattern to find
 * @return Index of first occurrence or -1
 */
int kmp_search(const char *text, const char *pattern);

/**
 * Build KMP failure function (LPS array)
 * @param pattern Pattern string
 * @param lps Output array for LPS values
 * @param m Length of pattern
 */
void kmp_build_lps(const char *pattern, int *lps, size_t m);

/**
 * Find all occurrences using KMP
 * @param text Text to search in
 * @param pattern Pattern to find
 * @param indices Output array for indices
 * @param max_results Maximum results to store
 * @return Number of occurrences found
 */
int kmp_search_all(const char *text, const char *pattern,
                   int *indices, int max_results);

/**
 * Rabin-Karp - O(n+m) average
 * Uses rolling hash for efficient matching.
 * @param text Text to search in
 * @param pattern Pattern to find
 * @return Index of first occurrence or -1
 */
int rabin_karp_search(const char *text, const char *pattern);

/**
 * Find all occurrences using Rabin-Karp
 * @param text Text to search in
 * @param pattern Pattern to find
 * @param indices Output array for indices
 * @param max_results Maximum results to store
 * @return Number of occurrences found
 */
int rabin_karp_search_all(const char *text, const char *pattern,
                          int *indices, int max_results);

/**
 * Z-Algorithm - build Z array
 * Z[i] = length of longest substring starting at i that is also a prefix
 * @param s String to process
 * @param z Output Z array
 * @param n Length of string
 */
void z_function(const char *s, int *z, size_t n);

/**
 * Pattern matching using Z-Algorithm
 * @param text Text to search in
 * @param pattern Pattern to find
 * @return Index of first occurrence or -1
 */
int z_search(const char *text, const char *pattern);

/**
 * Find all occurrences using Z-Algorithm
 * @param text Text to search in
 * @param pattern Pattern to find
 * @param indices Output array for indices
 * @param max_results Maximum results to store
 * @return Number of occurrences found
 */
int z_search_all(const char *text, const char *pattern,
                 int *indices, int max_results);

/* ============== String Properties ============== */

/**
 * Check if string is palindrome
 * @param s String to check
 * @return true if palindrome
 */
bool is_palindrome(const char *s);

/**
 * Check if pattern is repeated substring of text
 * Uses KMP failure function.
 * @param s String to check
 * @return true if s = p + p + ... + p for some pattern p
 */
bool is_repeated_pattern(const char *s);

/**
 * Find longest happy prefix (longest proper prefix that is also suffix)
 * @param s String
 * @return Length of longest happy prefix
 */
int longest_happy_prefix(const char *s);

/**
 * Find shortest palindrome by prepending characters
 * @param s Original string
 * @param result Buffer for result (must be at least 2*strlen(s)+1)
 */
void shortest_palindrome(const char *s, char *result);

/* ============== String Transformation ============== */

/**
 * Reverse string in place
 * @param s String to reverse
 */
void string_reverse(char *s);

/**
 * Reverse words in string
 * @param s String to modify in place
 */
void reverse_words(char *s);

/**
 * Check if two strings are anagrams
 * @param s1 First string
 * @param s2 Second string
 * @return true if anagrams
 */
bool are_anagrams(const char *s1, const char *s2);

/**
 * Check if s1 is rotation of s2
 * @param s1 First string
 * @param s2 Second string
 * @return true if rotation
 */
bool is_rotation(const char *s1, const char *s2);

/* ============== Utility ============== */

/**
 * Count occurrences of pattern in text
 * @param text Text to search in
 * @param pattern Pattern to count
 * @return Number of occurrences
 */
int count_pattern(const char *text, const char *pattern);

/**
 * Find all indices where pattern occurs
 * @param text Text to search
 * @param pattern Pattern to find
 * @param count Output: number of occurrences
 * @return Dynamically allocated array of indices (caller must free)
 */
int *find_all_indices(const char *text, const char *pattern, int *count);

#endif /* STRING_ALGORITHMS_H */
