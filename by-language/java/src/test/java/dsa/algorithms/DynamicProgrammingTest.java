package dsa.algorithms;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DynamicProgrammingTest {

    // --- fibonacci ---
    @Test
    void testFibonacci() {
        assertEquals(0, DynamicProgramming.fibonacci(0));
        assertEquals(1, DynamicProgramming.fibonacci(1));
        assertEquals(1, DynamicProgramming.fibonacci(2));
        assertEquals(2, DynamicProgramming.fibonacci(3));
        assertEquals(5, DynamicProgramming.fibonacci(5));
        assertEquals(55, DynamicProgramming.fibonacci(10));
        assertEquals(6765, DynamicProgramming.fibonacci(20));
        assertEquals(832040, DynamicProgramming.fibonacci(30));
    }

    @Test
    void testFibonacciMemo() {
        assertEquals(0, DynamicProgramming.fibonacciMemo(0));
        assertEquals(1, DynamicProgramming.fibonacciMemo(1));
        assertEquals(1, DynamicProgramming.fibonacciMemo(2));
        assertEquals(5, DynamicProgramming.fibonacciMemo(5));
        assertEquals(55, DynamicProgramming.fibonacciMemo(10));
        assertEquals(6765, DynamicProgramming.fibonacciMemo(20));
        assertEquals(832040, DynamicProgramming.fibonacciMemo(30));
    }

    @Test
    void testFibonacciConsistency() {
        for (int i = 0; i <= 30; i++) {
            assertEquals(DynamicProgramming.fibonacci(i), DynamicProgramming.fibonacciMemo(i),
                    "fibonacci and fibonacciMemo should agree for n=" + i);
        }
    }

    // --- knapsack01 ---
    @Test
    void testKnapsack01() {
        int[] weights = {1, 3, 4, 5};
        int[] values = {1, 4, 5, 7};
        assertEquals(9, DynamicProgramming.knapsack01(weights, values, 7));

        assertEquals(0, DynamicProgramming.knapsack01(new int[]{}, new int[]{}, 10));
        assertEquals(0, DynamicProgramming.knapsack01(new int[]{5}, new int[]{10}, 4));
        assertEquals(10, DynamicProgramming.knapsack01(new int[]{5}, new int[]{10}, 5));
    }

    @Test
    void testKnapsack01Classic() {
        int[] weights = {2, 3, 4, 5};
        int[] values = {3, 4, 5, 6};
        assertEquals(7, DynamicProgramming.knapsack01(weights, values, 5));
    }

    // --- longestCommonSubsequence ---
    @Test
    void testLongestCommonSubsequence() {
        assertEquals(3, DynamicProgramming.longestCommonSubsequence("abcde", "ace"));
        assertEquals(0, DynamicProgramming.longestCommonSubsequence("abc", "def"));
        assertEquals(3, DynamicProgramming.longestCommonSubsequence("abc", "abc"));
        assertEquals(0, DynamicProgramming.longestCommonSubsequence("", "abc"));
        assertEquals(0, DynamicProgramming.longestCommonSubsequence("abc", ""));
        assertEquals(4, DynamicProgramming.longestCommonSubsequence("AGGTAB", "GXTXAYB"));
    }

    // --- lcsString ---
    @Test
    void testLcsString() {
        assertEquals("ace", DynamicProgramming.lcsString("abcde", "ace"));
        assertEquals("", DynamicProgramming.lcsString("abc", "def"));
        assertEquals("abc", DynamicProgramming.lcsString("abc", "abc"));
        assertEquals("GTAB", DynamicProgramming.lcsString("AGGTAB", "GXTXAYB"));
        assertEquals("", DynamicProgramming.lcsString("", "abc"));
    }

    // --- longestIncreasingSubsequence ---
    @Test
    void testLongestIncreasingSubsequence() {
        assertEquals(4, DynamicProgramming.longestIncreasingSubsequence(new int[]{10, 9, 2, 5, 3, 7, 101, 18}));
        assertEquals(4, DynamicProgramming.longestIncreasingSubsequence(new int[]{0, 1, 0, 3, 2, 3}));
        assertEquals(1, DynamicProgramming.longestIncreasingSubsequence(new int[]{7, 7, 7, 7}));
        assertEquals(0, DynamicProgramming.longestIncreasingSubsequence(new int[]{}));
        assertEquals(1, DynamicProgramming.longestIncreasingSubsequence(new int[]{5}));
        assertEquals(6, DynamicProgramming.longestIncreasingSubsequence(new int[]{1, 2, 3, 4, 5, 6}));
    }

    // --- editDistance ---
    @Test
    void testEditDistance() {
        assertEquals(3, DynamicProgramming.editDistance("kitten", "sitting"));
        assertEquals(1, DynamicProgramming.editDistance("cat", "bat"));
        assertEquals(0, DynamicProgramming.editDistance("same", "same"));
        assertEquals(3, DynamicProgramming.editDistance("", "abc"));
        assertEquals(4, DynamicProgramming.editDistance("abcd", ""));
        assertEquals(2, DynamicProgramming.editDistance("ab", "cd"));
    }

    // --- coinChange ---
    @Test
    void testCoinChange() {
        assertEquals(3, DynamicProgramming.coinChange(new int[]{1, 2, 5}, 11));
        assertEquals(-1, DynamicProgramming.coinChange(new int[]{2}, 3));
        assertEquals(0, DynamicProgramming.coinChange(new int[]{1}, 0));
        assertEquals(1, DynamicProgramming.coinChange(new int[]{1}, 1));
        assertEquals(2, DynamicProgramming.coinChange(new int[]{1, 3, 5}, 6));
    }

    // --- coinChangeWays ---
    @Test
    void testCoinChangeWays() {
        assertEquals(4, DynamicProgramming.coinChangeWays(new int[]{1, 2, 5}, 5));
        assertEquals(1, DynamicProgramming.coinChangeWays(new int[]{1}, 5));
        assertEquals(1, DynamicProgramming.coinChangeWays(new int[]{2, 5}, 0));
        assertEquals(0, DynamicProgramming.coinChangeWays(new int[]{5}, 3));
    }

    // --- climbingStairs ---
    @Test
    void testClimbingStairs() {
        assertEquals(1, DynamicProgramming.climbingStairs(0));
        assertEquals(1, DynamicProgramming.climbingStairs(1));
        assertEquals(2, DynamicProgramming.climbingStairs(2));
        assertEquals(3, DynamicProgramming.climbingStairs(3));
        assertEquals(5, DynamicProgramming.climbingStairs(4));
        assertEquals(8, DynamicProgramming.climbingStairs(5));
        assertEquals(89, DynamicProgramming.climbingStairs(10));
    }

    // --- maxSubarraySum ---
    @Test
    void testMaxSubarraySum() {
        assertEquals(6, DynamicProgramming.maxSubarraySum(new int[]{-2, 1, -3, 4, -1, 2, 1, -5, 4}));
        assertEquals(1, DynamicProgramming.maxSubarraySum(new int[]{1}));
        assertEquals(-1, DynamicProgramming.maxSubarraySum(new int[]{-1}));
        assertEquals(23, DynamicProgramming.maxSubarraySum(new int[]{5, 4, -1, 7, 8}));
        assertEquals(-1, DynamicProgramming.maxSubarraySum(new int[]{-3, -2, -1, -4}));
        assertEquals(0, DynamicProgramming.maxSubarraySum(new int[]{}));
    }

    // --- longestPalindromicSubstring ---
    @Test
    void testLongestPalindromicSubstring() {
        String result1 = DynamicProgramming.longestPalindromicSubstring("babad");
        assertTrue("bab".equals(result1) || "aba".equals(result1));
        assertEquals("bb", DynamicProgramming.longestPalindromicSubstring("cbbd"));
        assertEquals("a", DynamicProgramming.longestPalindromicSubstring("a"));
        assertEquals("racecar", DynamicProgramming.longestPalindromicSubstring("racecar"));
        assertEquals("", DynamicProgramming.longestPalindromicSubstring(""));
    }

    // --- uniquePaths ---
    @Test
    void testUniquePaths() {
        assertEquals(1, DynamicProgramming.uniquePaths(1, 1));
        assertEquals(1, DynamicProgramming.uniquePaths(1, 5));
        assertEquals(1, DynamicProgramming.uniquePaths(5, 1));
        assertEquals(28, DynamicProgramming.uniquePaths(3, 7));
        assertEquals(6, DynamicProgramming.uniquePaths(3, 3));
    }

    // --- minPathSum ---
    @Test
    void testMinPathSum() {
        int[][] grid1 = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
        assertEquals(7, DynamicProgramming.minPathSum(grid1));

        int[][] grid2 = {{1, 2, 3}, {4, 5, 6}};
        assertEquals(12, DynamicProgramming.minPathSum(grid2));

        int[][] grid3 = {{5}};
        assertEquals(5, DynamicProgramming.minPathSum(grid3));

        assertEquals(0, DynamicProgramming.minPathSum(new int[][]{}));
    }

    // --- wordBreak ---
    @Test
    void testWordBreak() {
        assertTrue(DynamicProgramming.wordBreak("leetcode", Arrays.asList("leet", "code")));
        assertTrue(DynamicProgramming.wordBreak("applepenapple", Arrays.asList("apple", "pen")));
        assertFalse(DynamicProgramming.wordBreak("catsandog", Arrays.asList("cats", "dog", "sand", "and", "cat")));
        assertTrue(DynamicProgramming.wordBreak("", List.of("a")));
        assertFalse(DynamicProgramming.wordBreak("a", List.of()));
    }

    // --- subsetSum ---
    @Test
    void testSubsetSum() {
        assertTrue(DynamicProgramming.subsetSum(new int[]{3, 34, 4, 12, 5, 2}, 9));
        assertFalse(DynamicProgramming.subsetSum(new int[]{3, 34, 4, 12, 5, 2}, 30));
        assertTrue(DynamicProgramming.subsetSum(new int[]{1, 2, 3}, 6));
        assertTrue(DynamicProgramming.subsetSum(new int[]{1, 2, 3}, 0));
        assertFalse(DynamicProgramming.subsetSum(new int[]{}, 1));
        assertTrue(DynamicProgramming.subsetSum(new int[]{}, 0));
        assertTrue(DynamicProgramming.subsetSum(new int[]{5}, 5));
        assertFalse(DynamicProgramming.subsetSum(new int[]{5}, 3));
    }

    // --- matrixChainMultiplication ---
    @Test
    void testMatrixChainMultiplication() {
        // Matrices: A1(10x30), A2(30x5), A3(5x60)
        assertEquals(4500, DynamicProgramming.matrixChainMultiplication(new int[]{10, 30, 5, 60}));
        // Matrices: A1(40x20), A2(20x30), A3(30x10), A4(10x30)
        assertEquals(26000, DynamicProgramming.matrixChainMultiplication(new int[]{40, 20, 30, 10, 30}));
        // Single matrix
        assertEquals(0, DynamicProgramming.matrixChainMultiplication(new int[]{10, 20}));
        // No matrices
        assertEquals(0, DynamicProgramming.matrixChainMultiplication(new int[]{10}));
    }
}
