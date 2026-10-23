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
        assertEquals(List.of(0, 1), StringAlgorithms.zAlgorithm("$$", "$"));
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

    // --- boyerMoore ---
    @Test
    void testBoyerMooreBasic() {
        assertEquals(List.of(0, 3, 6), StringAlgorithms.boyerMoore("abcabcabc", "abc"));
    }

    @Test
    void testBoyerMooreNoMatch() {
        assertEquals(List.of(), StringAlgorithms.boyerMoore("abcdef", "xyz"));
    }

    @Test
    void testBoyerMooreOverlapping() {
        assertEquals(List.of(0, 1, 2), StringAlgorithms.boyerMoore("aaaa", "aa"));
    }

    @Test
    void testBoyerMooreSingleChar() {
        assertEquals(List.of(0, 3), StringAlgorithms.boyerMoore("abcabc", "a"));
    }

    @Test
    void testBoyerMooreConsistency() {
        String text = "aabaacaadaabaaba";
        String pattern = "aaba";
        List<Integer> naive = StringAlgorithms.naiveSearch(text, pattern);
        List<Integer> bm = StringAlgorithms.boyerMoore(text, pattern);
        assertEquals(naive, bm, "Boyer-Moore should match naive");
    }

    // --- shortestPalindrome ---
    @Test
    void testShortestPalindrome() {
        assertEquals("aaacecaaa", StringAlgorithms.shortestPalindrome("aacecaaa"));
        assertEquals("dcbabcd", StringAlgorithms.shortestPalindrome("abcd"));
        assertEquals("a", StringAlgorithms.shortestPalindrome("a"));
        assertEquals("", StringAlgorithms.shortestPalindrome(""));
        assertEquals("a#a", StringAlgorithms.shortestPalindrome("a#a"));
    }

    // --- longestHappyPrefix ---
    @Test
    void testLongestHappyPrefix() {
        assertEquals("l", StringAlgorithms.longestHappyPrefix("level"));
        assertEquals("abab", StringAlgorithms.longestHappyPrefix("ababab"));
        assertEquals("", StringAlgorithms.longestHappyPrefix("abc"));
        assertEquals("", StringAlgorithms.longestHappyPrefix("a"));
    }

    // --- countPalindromicSubstrings ---
    @Test
    void testCountPalindromicSubstrings() {
        assertEquals(3, StringAlgorithms.countPalindromicSubstrings("abc"));
        assertEquals(6, StringAlgorithms.countPalindromicSubstrings("aaa"));
        assertEquals(1, StringAlgorithms.countPalindromicSubstrings("a"));
        assertEquals(0, StringAlgorithms.countPalindromicSubstrings(""));
    }

    // --- computeLCPArray ---
    @Test
    void testComputeLCPArray() {
        String s = "banana";
        int[] sa = StringAlgorithms.buildSuffixArray(s);
        int[] lcp = StringAlgorithms.computeLCPArray(s, sa);
        // sa = [5, 3, 1, 0, 4, 2]
        // suffixes: a, ana, anana, banana, na, nana
        // LCP: [0, 1, 3, 0, 0, 2]
        assertEquals(0, lcp[0]); // first element is always 0
        assertEquals(1, lcp[1]); // a vs ana
        assertEquals(3, lcp[2]); // ana vs anana
        assertEquals(0, lcp[3]); // anana vs banana
        assertEquals(0, lcp[4]); // banana vs na
        assertEquals(2, lcp[5]); // na vs nana
    }

    // --- searchWithSuffixArray ---
    @Test
    void testSearchWithSuffixArray() {
        List<Integer> result = StringAlgorithms.searchWithSuffixArray("banana", "ana");
        assertEquals(2, result.size());
        assertTrue(result.contains(1));
        assertTrue(result.contains(3));
    }

    @Test
    void testSearchWithSuffixArrayNotFound() {
        List<Integer> result = StringAlgorithms.searchWithSuffixArray("banana", "xyz");
        assertTrue(result.isEmpty());
    }

    // --- rabinKarpMultiple ---
    @Test
    void testRabinKarpMultiple() {
        List<Integer> result = StringAlgorithms.rabinKarpMultiple("abcabcabc", new String[]{"abc", "cab"});
        assertTrue(result.contains(0));
        assertTrue(result.contains(2));
        assertTrue(result.contains(3));
        assertTrue(result.contains(5));
        assertTrue(result.contains(6));
    }

    @Test
    void testRabinKarpMultipleNoMatch() {
        List<Integer> result = StringAlgorithms.rabinKarpMultiple("hello", new String[]{"xyz"});
        assertTrue(result.isEmpty());
    }

    // --- isSubsequence ---
    @Test
    void testIsSubsequence() {
        assertTrue(StringAlgorithms.isSubsequence("ace", "abcde"));
        assertFalse(StringAlgorithms.isSubsequence("aec", "abcde"));
        assertTrue(StringAlgorithms.isSubsequence("", "abcde"));
        assertTrue(StringAlgorithms.isSubsequence("", ""));
        assertFalse(StringAlgorithms.isSubsequence("a", ""));
        assertTrue(StringAlgorithms.isSubsequence("abc", "abc"));
    }

    // --- longestRepeatingSubstring ---
    @Test
    void testLongestRepeatingSubstring() {
        assertEquals(3, StringAlgorithms.longestRepeatingSubstring("banana"));
        assertEquals(2, StringAlgorithms.longestRepeatingSubstring("abab"));
        assertEquals(0, StringAlgorithms.longestRepeatingSubstring("abc"));
        assertEquals(0, StringAlgorithms.longestRepeatingSubstring("a"));
        assertEquals(0, StringAlgorithms.longestRepeatingSubstring(""));
    }
}
