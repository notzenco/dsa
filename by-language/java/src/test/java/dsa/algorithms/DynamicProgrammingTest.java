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

    // --- houseRobber ---
    @Test
    void testHouseRobber() {
        assertEquals(4, DynamicProgramming.houseRobber(new int[]{1, 2, 3, 1}));
        assertEquals(12, DynamicProgramming.houseRobber(new int[]{2, 7, 9, 3, 1}));
        assertEquals(0, DynamicProgramming.houseRobber(new int[]{}));
    }

    @Test
    void testHouseRobberSingleHouse() {
        assertEquals(5, DynamicProgramming.houseRobber(new int[]{5}));
    }

    @Test
    void testHouseRobberTwoHouses() {
        assertEquals(7, DynamicProgramming.houseRobber(new int[]{3, 7}));
    }

    @Test
    void testHouseRobberAllSameValues() {
        assertEquals(9, DynamicProgramming.houseRobber(new int[]{3, 3, 3, 3, 3}));
    }

    // --- houseRobberII ---
    @Test
    void testHouseRobberII() {
        assertEquals(3, DynamicProgramming.houseRobberII(new int[]{2, 3, 2}));
        assertEquals(4, DynamicProgramming.houseRobberII(new int[]{1, 2, 3, 1}));
        assertEquals(3, DynamicProgramming.houseRobberII(new int[]{1, 2, 3}));
        assertEquals(0, DynamicProgramming.houseRobberII(new int[]{}));
        assertEquals(5, DynamicProgramming.houseRobberII(new int[]{5}));
    }

    // --- unboundedKnapsack ---
    @Test
    void testUnboundedKnapsack() {
        int[] weights = {1, 3, 4, 5};
        int[] values = {10, 40, 50, 70};
        assertEquals(110, DynamicProgramming.unboundedKnapsack(weights, values, 8));
    }

    @Test
    void testUnboundedKnapsackMultipleCopies() {
        int[] weights = {2};
        int[] values = {5};
        assertEquals(15, DynamicProgramming.unboundedKnapsack(weights, values, 6));
    }

    @Test
    void testUnboundedKnapsackEmpty() {
        assertEquals(0, DynamicProgramming.unboundedKnapsack(new int[]{}, new int[]{}, 10));
    }

    // --- longestPalindromicSubsequence ---
    @Test
    void testLongestPalindromicSubsequence() {
        assertEquals(4, DynamicProgramming.longestPalindromicSubsequence("bbbab"));
        assertEquals(2, DynamicProgramming.longestPalindromicSubsequence("cbbd"));
    }

    @Test
    void testLongestPalindromicSubsequenceSingleChar() {
        assertEquals(1, DynamicProgramming.longestPalindromicSubsequence("a"));
    }

    @Test
    void testLongestPalindromicSubsequenceAllSame() {
        assertEquals(5, DynamicProgramming.longestPalindromicSubsequence("aaaaa"));
    }

    @Test
    void testLongestPalindromicSubsequenceEmpty() {
        assertEquals(0, DynamicProgramming.longestPalindromicSubsequence(""));
    }

    // --- fibonacciOptimized ---
    @Test
    void testFibonacciOptimized() {
        for (int i = 0; i <= 30; i++) {
            assertEquals(DynamicProgramming.fibonacci(i), DynamicProgramming.fibonacciOptimized(i),
                    "fibonacciOptimized should match fibonacci for n=" + i);
        }
    }

    // --- lisWithSequence ---
    @Test
    void testLisWithSequence() {
        int[] result = DynamicProgramming.lisWithSequence(new int[]{10, 9, 2, 5, 3, 7, 101, 18});
        assertEquals(4, result.length);
        // Verify the sequence is increasing
        for (int i = 1; i < result.length; i++) {
            assertTrue(result[i] > result[i - 1]);
        }
    }

    @Test
    void testLisWithSequenceEmpty() {
        assertArrayEquals(new int[]{}, DynamicProgramming.lisWithSequence(new int[]{}));
    }

    @Test
    void testLisWithSequenceSingle() {
        assertArrayEquals(new int[]{5}, DynamicProgramming.lisWithSequence(new int[]{5}));
    }

    @Test
    void testLisWithSequenceAlreadySorted() {
        assertArrayEquals(new int[]{1, 2, 3, 4, 5}, DynamicProgramming.lisWithSequence(new int[]{1, 2, 3, 4, 5}));
    }

    // --- canPartition ---
    @Test
    void testCanPartition() {
        assertTrue(DynamicProgramming.canPartition(new int[]{1, 5, 11, 5}));
        assertFalse(DynamicProgramming.canPartition(new int[]{1, 2, 3, 5}));
        assertTrue(DynamicProgramming.canPartition(new int[]{1, 1}));
        assertFalse(DynamicProgramming.canPartition(new int[]{1}));
    }

    // --- minCostClimbingStairs ---
    @Test
    void testMinCostClimbingStairs() {
        assertEquals(15, DynamicProgramming.minCostClimbingStairs(new int[]{10, 15, 20}));
        assertEquals(6, DynamicProgramming.minCostClimbingStairs(new int[]{1, 100, 1, 1, 1, 100, 1, 1, 100, 1}));
        assertEquals(0, DynamicProgramming.minCostClimbingStairs(new int[]{}));
    }

    // --- numDecodings ---
    @Test
    void testNumDecodings() {
        assertEquals(2, DynamicProgramming.numDecodings("12"));
        assertEquals(3, DynamicProgramming.numDecodings("226"));
        assertEquals(0, DynamicProgramming.numDecodings("0"));
        assertEquals(0, DynamicProgramming.numDecodings("06"));
        assertEquals(1, DynamicProgramming.numDecodings("10"));
        assertEquals(1, DynamicProgramming.numDecodings("27"));
    }

    // --- maximalSquare ---
    @Test
    void testMaximalSquare() {
        int[][] matrix = {
            {1, 0, 1, 0, 0},
            {1, 0, 1, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 1, 0}
        };
        assertEquals(4, DynamicProgramming.maximalSquare(matrix));
    }

    @Test
    void testMaximalSquareAllOnes() {
        int[][] matrix = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        };
        assertEquals(9, DynamicProgramming.maximalSquare(matrix));
    }

    @Test
    void testMaximalSquareSingleRow() {
        int[][] matrix = {{1, 0, 1, 1}};
        assertEquals(1, DynamicProgramming.maximalSquare(matrix));
    }

    @Test
    void testMaximalSquareEmpty() {
        assertEquals(0, DynamicProgramming.maximalSquare(new int[][]{}));
    }

    // --- lengthOfLIS_DP ---
    @Test
    void testLengthOfLIS_DP() {
        // Should agree with longestIncreasingSubsequence
        int[][] testCases = {
            {10, 9, 2, 5, 3, 7, 101, 18},
            {0, 1, 0, 3, 2, 3},
            {7, 7, 7, 7},
            {1, 2, 3, 4, 5, 6}
        };
        for (int[] tc : testCases) {
            assertEquals(DynamicProgramming.longestIncreasingSubsequence(tc),
                         DynamicProgramming.lengthOfLIS_DP(tc),
                         "LIS_DP should match LIS for input " + Arrays.toString(tc));
        }
    }

    @Test
    void testLengthOfLIS_DPEmpty() {
        assertEquals(0, DynamicProgramming.lengthOfLIS_DP(new int[]{}));
    }

    @Test
    void testLengthOfLIS_DPSingle() {
        assertEquals(1, DynamicProgramming.lengthOfLIS_DP(new int[]{5}));
    }
}
