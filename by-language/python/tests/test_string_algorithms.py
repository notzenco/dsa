"""
Tests for String Algorithms.
"""

import pytest
from algorithms.string_algorithms import (
    # Naive
    naive_search,
    # KMP
    compute_lps, kmp_search, kmp_search_first,
    # Rabin-Karp
    rabin_karp_search, rabin_karp_multiple,
    # Z-Algorithm
    compute_z_array, z_algorithm_search,
    # Manacher
    manacher, longest_palindrome_manacher, count_palindromic_substrings,
    # Suffix Array
    build_suffix_array, build_suffix_array_optimized, build_lcp_array,
    search_with_suffix_array,
    # Utilities
    longest_common_prefix, repeated_substring_pattern,
    shortest_palindrome, longest_happy_prefix
)


class TestNaiveSearch:
    """Test naive pattern matching."""

    def test_basic(self):
        """Test basic pattern matching."""
        assert naive_search("hello world", "world") == [6]
        assert naive_search("ababab", "ab") == [0, 2, 4]

    def test_no_match(self):
        """Test when pattern not found."""
        assert naive_search("hello", "xyz") == []

    def test_empty(self):
        """Test empty inputs."""
        assert naive_search("", "abc") == []
        assert naive_search("abc", "") == []

    def test_pattern_longer_than_text(self):
        """Test pattern longer than text."""
        assert naive_search("ab", "abc") == []


class TestKMP:
    """Test KMP algorithm."""

    def test_compute_lps(self):
        """Test LPS array computation."""
        assert compute_lps("ABABCABAB") == [0, 0, 1, 2, 0, 1, 2, 3, 4]
        assert compute_lps("AAAA") == [0, 1, 2, 3]
        assert compute_lps("ABCD") == [0, 0, 0, 0]

    def test_kmp_search_basic(self):
        """Test basic KMP search."""
        assert kmp_search("ABABDABACDABABCABAB", "ABABCABAB") == [10]
        assert kmp_search("aaaa", "aa") == [0, 1, 2]

    def test_kmp_search_no_match(self):
        """Test KMP when pattern not found."""
        assert kmp_search("hello world", "xyz") == []

    def test_kmp_search_first(self):
        """Test finding first occurrence."""
        assert kmp_search_first("abcabc", "abc") == 0
        assert kmp_search_first("hello", "xyz") == -1

    def test_kmp_empty(self):
        """Test empty inputs."""
        assert kmp_search("abc", "") == [0]
        assert kmp_search("", "abc") == []


class TestRabinKarp:
    """Test Rabin-Karp algorithm."""

    def test_basic(self):
        """Test basic Rabin-Karp search."""
        assert rabin_karp_search("hello world", "world") == [6]
        assert rabin_karp_search("ababab", "ab") == [0, 2, 4]

    def test_no_match(self):
        """Test when pattern not found."""
        assert rabin_karp_search("hello", "xyz") == []

    def test_multiple_patterns(self):
        """Test searching multiple patterns."""
        text = "hello world hello"
        patterns = ["hello", "world", "xyz"]
        result = rabin_karp_multiple(text, patterns)

        assert result["hello"] == [0, 12]
        assert result["world"] == [6]
        assert result["xyz"] == []


class TestZAlgorithm:
    """Test Z-algorithm."""

    def test_compute_z_array(self):
        """Test Z-array computation."""
        z = compute_z_array("aabxaab")
        assert z[0] == 7  # Length of string
        assert z[1] == 1  # "a" matches prefix "a"
        assert z[2] == 0  # "bxaab" doesn't match prefix
        assert z[4] == 3  # "aab" matches prefix "aab"

    def test_z_search_basic(self):
        """Test Z-algorithm search."""
        assert z_algorithm_search("ABABDABACDABABCABAB", "ABABCABAB") == [10]
        assert z_algorithm_search("aaaa", "aa") == [0, 1, 2]

    def test_z_search_no_match(self):
        """Test when pattern not found."""
        assert z_algorithm_search("hello", "xyz") == []

    def test_z_empty(self):
        """Test empty inputs."""
        assert compute_z_array("") == []
        assert z_algorithm_search("", "abc") == []


class TestManacher:
    """Test Manacher's algorithm."""

    def test_longest_palindrome(self):
        """Test finding longest palindrome."""
        assert longest_palindrome_manacher("babad") in ["bab", "aba"]
        assert longest_palindrome_manacher("cbbd") == "bb"
        assert longest_palindrome_manacher("a") == "a"
        assert longest_palindrome_manacher("racecar") == "racecar"

    def test_longest_palindrome_empty(self):
        """Test empty string."""
        assert longest_palindrome_manacher("") == ""

    def test_count_palindromes(self):
        """Test counting palindromic substrings."""
        # "abc": "a", "b", "c" = 3
        assert count_palindromic_substrings("abc") == 3
        # "aaa": "a"(3), "aa"(2), "aaa"(1) = 6
        assert count_palindromic_substrings("aaa") == 6

    def test_manacher_array(self):
        """Test Manacher array computation."""
        p = manacher("aba")
        # Transformed: "#a#b#a#"
        # Palindrome radii should show 3 at center (the 'b')
        assert max(p) == 3


class TestSuffixArray:
    """Test suffix array construction and usage."""

    def test_build_suffix_array(self):
        """Test suffix array construction."""
        sa = build_suffix_array("banana")
        # Suffixes sorted: "a", "ana", "anana", "banana", "na", "nana"
        # Indices: 5, 3, 1, 0, 4, 2
        assert sa == [5, 3, 1, 0, 4, 2]

    def test_build_suffix_array_optimized(self):
        """Test optimized suffix array construction."""
        sa = build_suffix_array_optimized("banana")
        assert sa == [5, 3, 1, 0, 4, 2]

    def test_build_lcp_array(self):
        """Test LCP array construction."""
        s = "banana"
        sa = build_suffix_array(s)
        lcp = build_lcp_array(s, sa)
        # LCP between adjacent suffixes in sorted order:
        # "a" vs "ana" -> 1
        # "ana" vs "anana" -> 3
        # "anana" vs "banana" -> 0
        # "banana" vs "na" -> 0
        # "na" vs "nana" -> 2
        assert lcp == [0, 1, 3, 0, 0, 2]

    def test_search_with_suffix_array(self):
        """Test pattern search using suffix array."""
        text = "banana"
        sa = build_suffix_array(text)

        assert search_with_suffix_array(text, "ana", sa) == [1, 3]
        assert search_with_suffix_array(text, "nan", sa) == [2]
        assert search_with_suffix_array(text, "xyz", sa) == []

    def test_suffix_array_empty(self):
        """Test empty string."""
        assert build_suffix_array("") == []
        assert build_suffix_array_optimized("") == []


class TestUtilities:
    """Test utility functions."""

    def test_longest_common_prefix(self):
        """Test longest common prefix."""
        assert longest_common_prefix(["flower", "flow", "flight"]) == "fl"
        assert longest_common_prefix(["dog", "racecar", "car"]) == ""
        assert longest_common_prefix(["abc"]) == "abc"
        assert longest_common_prefix([]) == ""

    def test_repeated_substring_pattern(self):
        """Test repeated substring pattern."""
        assert repeated_substring_pattern("abab") == True
        assert repeated_substring_pattern("aba") == False
        assert repeated_substring_pattern("abcabcabcabc") == True

    def test_shortest_palindrome(self):
        """Test shortest palindrome."""
        assert shortest_palindrome("aacecaaa") == "aaacecaaa"
        assert shortest_palindrome("abcd") == "dcbabcd"
        assert shortest_palindrome("") == ""

    def test_longest_happy_prefix(self):
        """Test longest happy prefix."""
        assert longest_happy_prefix("level") == "l"
        assert longest_happy_prefix("ababab") == "abab"
        assert longest_happy_prefix("abc") == ""


class TestCompareAlgorithms:
    """Test that all algorithms give same results."""

    @pytest.mark.parametrize("text,pattern,expected", [
        ("hello world", "world", [6]),
        ("ababab", "ab", [0, 2, 4]),
        ("aaaaaa", "aa", [0, 1, 2, 3, 4]),
        ("abcdef", "xyz", []),
        ("abcabc", "abc", [0, 3]),
    ])
    def test_all_algorithms_match(self, text, pattern, expected):
        """Verify all search algorithms produce same results."""
        assert naive_search(text, pattern) == expected
        assert kmp_search(text, pattern) == expected
        assert rabin_karp_search(text, pattern) == expected
        assert z_algorithm_search(text, pattern) == expected
