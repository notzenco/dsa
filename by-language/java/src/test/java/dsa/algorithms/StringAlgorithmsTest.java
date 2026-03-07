package dsa.algorithms;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class StringAlgorithmsTest {

    // --- naiveSearch ---
    @Test
    void testNaiveSearch() {
        assertEquals(List.of(0, 3, 6), StringAlgorithms.naiveSearch("abcabcabc", "abc"));
        assertEquals(List.of(), StringAlgorithms.naiveSearch("abcdef", "xyz"));
        assertEquals(List.of(0), StringAlgorithms.naiveSearch("hello", "hello"));
        assertEquals(List.of(), StringAlgorithms.naiveSearch("short", "toolongpattern"));
        assertEquals(List.of(), StringAlgorithms.naiveSearch("", "a"));
        assertEquals(List.of(0, 1, 2), StringAlgorithms.naiveSearch("aaaa", "aa"));
    }

    // --- kmpSearch ---
    @Test
    void testKmpSearch() {
        assertEquals(List.of(0, 3, 6), StringAlgorithms.kmpSearch("abcabcabc", "abc"));
        assertEquals(List.of(), StringAlgorithms.kmpSearch("abcdef", "xyz"));
        assertEquals(List.of(0), StringAlgorithms.kmpSearch("hello", "hello"));
        assertEquals(List.of(0, 1, 2), StringAlgorithms.kmpSearch("aaaa", "aa"));
        assertEquals(List.of(4), StringAlgorithms.kmpSearch("ababcababc", "cababc"));
    }

    // --- computeLPSArray ---
    @Test
    void testComputeLPSArray() {
        assertArrayEquals(new int[]{0, 0, 0, 1, 2, 3}, StringAlgorithms.computeLPSArray("abcabc"));
        assertArrayEquals(new int[]{0, 1, 2, 3}, StringAlgorithms.computeLPSArray("aaaa"));
        assertArrayEquals(new int[]{0, 0, 0}, StringAlgorithms.computeLPSArray("abc"));
        assertArrayEquals(new int[]{0, 0, 1, 0, 1, 2}, StringAlgorithms.computeLPSArray("abacab"));
    }

    // --- rabinKarp ---
    @Test
    void testRabinKarp() {
        assertEquals(List.of(0, 3, 6), StringAlgorithms.rabinKarp("abcabcabc", "abc"));
        assertEquals(List.of(), StringAlgorithms.rabinKarp("abcdef", "xyz"));
        assertEquals(List.of(0), StringAlgorithms.rabinKarp("hello", "hello"));
        assertEquals(List.of(0, 1, 2), StringAlgorithms.rabinKarp("aaaa", "aa"));
        assertEquals(List.of(), StringAlgorithms.rabinKarp("ab", "abc"));
    }

    // --- zAlgorithm ---
    @Test
    void testZAlgorithm() {
        assertEquals(List.of(0, 3, 6), StringAlgorithms.zAlgorithm("abcabcabc", "abc"));
        assertEquals(List.of(), StringAlgorithms.zAlgorithm("abcdef", "xyz"));
        assertEquals(List.of(0), StringAlgorithms.zAlgorithm("hello", "hello"));
        assertEquals(List.of(0, 1, 2), StringAlgorithms.zAlgorithm("aaaa", "aa"));
    }

    // --- computeZArray ---
    @Test
    void testComputeZArray() {
        int[] z = StringAlgorithms.computeZArray("aabxaa");
        assertEquals(6, z[0]); // z[0] is always length of string
        assertEquals(1, z[1]); // "a" matches prefix
        assertEquals(0, z[2]); // "bxaa" doesn't match prefix
        assertEquals(0, z[3]); // "xaa" doesn't match prefix
        assertEquals(2, z[4]); // "aa" matches prefix
        assertEquals(1, z[5]); // "a" matches prefix
    }

    // --- manacher ---
    @Test
    void testManacher() {
        String result1 = StringAlgorithms.manacher("babad");
        assertTrue("bab".equals(result1) || "aba".equals(result1));
        assertEquals("bb", StringAlgorithms.manacher("cbbd"));
        assertEquals("a", StringAlgorithms.manacher("a"));
        assertEquals("racecar", StringAlgorithms.manacher("racecar"));
        assertEquals("", StringAlgorithms.manacher(""));
        assertEquals("abacaba", StringAlgorithms.manacher("abacaba"));
    }

    // --- buildSuffixArray ---
    @Test
    void testBuildSuffixArray() {
        int[] sa = StringAlgorithms.buildSuffixArray("banana");
        // Suffixes sorted: a(5), ana(3), anana(1), banana(0), na(4), nana(2)
        assertArrayEquals(new int[]{5, 3, 1, 0, 4, 2}, sa);

        int[] sa2 = StringAlgorithms.buildSuffixArray("abc");
        assertArrayEquals(new int[]{0, 1, 2}, sa2);

        int[] sa3 = StringAlgorithms.buildSuffixArray("a");
        assertArrayEquals(new int[]{0}, sa3);
    }

    // --- levenshteinDistance ---
    @Test
    void testLevenshteinDistance() {
        assertEquals(3, StringAlgorithms.levenshteinDistance("kitten", "sitting"));
        assertEquals(1, StringAlgorithms.levenshteinDistance("cat", "bat"));
        assertEquals(0, StringAlgorithms.levenshteinDistance("same", "same"));
        assertEquals(3, StringAlgorithms.levenshteinDistance("", "abc"));
        assertEquals(4, StringAlgorithms.levenshteinDistance("abcd", ""));
        assertEquals(0, StringAlgorithms.levenshteinDistance("", ""));
        assertEquals(2, StringAlgorithms.levenshteinDistance("ab", "cd"));
    }

    // --- longestCommonPrefix ---
    @Test
    void testLongestCommonPrefix() {
        assertEquals("fl", StringAlgorithms.longestCommonPrefix(new String[]{"flower", "flow", "flight"}));
        assertEquals("", StringAlgorithms.longestCommonPrefix(new String[]{"dog", "racecar", "car"}));
        assertEquals("a", StringAlgorithms.longestCommonPrefix(new String[]{"a"}));
        assertEquals("", StringAlgorithms.longestCommonPrefix(new String[]{}));
        assertEquals("hello", StringAlgorithms.longestCommonPrefix(new String[]{"hello", "hello", "hello"}));
        assertEquals("", StringAlgorithms.longestCommonPrefix(new String[]{"", "abc"}));
    }

    // --- repeatedSubstringPattern ---
    @Test
    void testRepeatedSubstringPattern() {
        assertTrue(StringAlgorithms.repeatedSubstringPattern("abab"));
        assertFalse(StringAlgorithms.repeatedSubstringPattern("aba"));
        assertTrue(StringAlgorithms.repeatedSubstringPattern("abcabcabc"));
        assertTrue(StringAlgorithms.repeatedSubstringPattern("aaaa"));
        assertFalse(StringAlgorithms.repeatedSubstringPattern("a"));
        assertFalse(StringAlgorithms.repeatedSubstringPattern(""));
        assertTrue(StringAlgorithms.repeatedSubstringPattern("abaaba"));
        assertFalse(StringAlgorithms.repeatedSubstringPattern("abac"));
    }

    // --- Cross-algorithm consistency for pattern search ---
    @Test
    void testPatternSearchConsistency() {
        String text = "aabaacaadaabaaba";
        String pattern = "aaba";
        List<Integer> naive = StringAlgorithms.naiveSearch(text, pattern);
        List<Integer> kmp = StringAlgorithms.kmpSearch(text, pattern);
        List<Integer> rk = StringAlgorithms.rabinKarp(text, pattern);
        List<Integer> z = StringAlgorithms.zAlgorithm(text, pattern);
        assertEquals(naive, kmp, "KMP should match naive");
        assertEquals(naive, rk, "Rabin-Karp should match naive");
        assertEquals(naive, z, "Z-algorithm should match naive");
    }

    @Test
    void testPatternSearchSingleChar() {
        String text = "abcabc";
        String pattern = "a";
        assertEquals(List.of(0, 3), StringAlgorithms.naiveSearch(text, pattern));
        assertEquals(List.of(0, 3), StringAlgorithms.kmpSearch(text, pattern));
        assertEquals(List.of(0, 3), StringAlgorithms.rabinKarp(text, pattern));
        assertEquals(List.of(0, 3), StringAlgorithms.zAlgorithm(text, pattern));
    }

    @Test
    void testLevenshteinSymmetry() {
        assertEquals(StringAlgorithms.levenshteinDistance("abc", "def"),
                     StringAlgorithms.levenshteinDistance("def", "abc"));
    }
}
