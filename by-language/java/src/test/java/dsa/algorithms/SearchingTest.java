package dsa.algorithms;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class SearchingTest {

    private static final int[] SORTED = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    private static final int[] EMPTY = {};
    private static final int[] SINGLE = {42};
    private static final int[] DUPS = {1, 2, 2, 2, 3, 4, 4, 5};

    // --- linearSearch ---
    @Test
    void testLinearSearch() {
        assertEquals(0, Searching.linearSearch(SORTED, 1));
        assertEquals(4, Searching.linearSearch(SORTED, 9));
        assertEquals(9, Searching.linearSearch(SORTED, 19));
        assertEquals(-1, Searching.linearSearch(SORTED, 6));
        assertEquals(-1, Searching.linearSearch(EMPTY, 5));
        assertEquals(0, Searching.linearSearch(SINGLE, 42));
        assertEquals(-1, Searching.linearSearch(SINGLE, 10));
    }

    // --- binarySearch ---
    @Test
    void testBinarySearch() {
        assertEquals(0, Searching.binarySearch(SORTED, 1));
        assertEquals(4, Searching.binarySearch(SORTED, 9));
        assertEquals(9, Searching.binarySearch(SORTED, 19));
        assertEquals(-1, Searching.binarySearch(SORTED, 6));
        assertEquals(-1, Searching.binarySearch(EMPTY, 5));
        assertEquals(0, Searching.binarySearch(SINGLE, 42));
        assertEquals(-1, Searching.binarySearch(SINGLE, 10));
    }

    // --- binarySearchRecursive ---
    @Test
    void testBinarySearchRecursive() {
        assertEquals(0, Searching.binarySearchRecursive(SORTED, 1));
        assertEquals(4, Searching.binarySearchRecursive(SORTED, 9));
        assertEquals(9, Searching.binarySearchRecursive(SORTED, 19));
        assertEquals(-1, Searching.binarySearchRecursive(SORTED, 6));
        assertEquals(-1, Searching.binarySearchRecursive(EMPTY, 5));
        assertEquals(0, Searching.binarySearchRecursive(SINGLE, 42));
    }

    // --- lowerBound ---
    @Test
    void testLowerBound() {
        // {1, 3, 5, 7, 9, 11, 13, 15, 17, 19}
        assertEquals(0, Searching.lowerBound(SORTED, 1));   // exact match at start
        assertEquals(0, Searching.lowerBound(SORTED, 0));   // before all
        assertEquals(10, Searching.lowerBound(SORTED, 20)); // after all
        assertEquals(2, Searching.lowerBound(SORTED, 4));   // between 3 and 5 -> index 2
        assertEquals(2, Searching.lowerBound(SORTED, 5));   // exact match
        // With duplicates: {1, 2, 2, 2, 3, 4, 4, 5}
        assertEquals(1, Searching.lowerBound(DUPS, 2));     // first occurrence of 2
    }

    // --- upperBound ---
    @Test
    void testUpperBound() {
        assertEquals(1, Searching.upperBound(SORTED, 1));    // first index > 1
        assertEquals(0, Searching.upperBound(SORTED, 0));    // first index > 0
        assertEquals(10, Searching.upperBound(SORTED, 19));  // past end
        assertEquals(10, Searching.upperBound(SORTED, 20));  // past end
        // With duplicates
        assertEquals(4, Searching.upperBound(DUPS, 2));      // first index > 2
    }

    // --- jumpSearch ---
    @Test
    void testJumpSearch() {
        assertEquals(0, Searching.jumpSearch(SORTED, 1));
        assertEquals(4, Searching.jumpSearch(SORTED, 9));
        assertEquals(9, Searching.jumpSearch(SORTED, 19));
        assertEquals(-1, Searching.jumpSearch(SORTED, 6));
        assertEquals(-1, Searching.jumpSearch(EMPTY, 5));
        assertEquals(0, Searching.jumpSearch(SINGLE, 42));
        assertEquals(-1, Searching.jumpSearch(SINGLE, 10));
    }

    // --- interpolationSearch ---
    @Test
    void testInterpolationSearch() {
        assertEquals(0, Searching.interpolationSearch(SORTED, 1));
        assertEquals(4, Searching.interpolationSearch(SORTED, 9));
        assertEquals(9, Searching.interpolationSearch(SORTED, 19));
        assertEquals(-1, Searching.interpolationSearch(SORTED, 6));
        assertEquals(-1, Searching.interpolationSearch(EMPTY, 5));
        assertEquals(0, Searching.interpolationSearch(SINGLE, 42));
        assertEquals(-1, Searching.interpolationSearch(SINGLE, 10));
        assertEquals(0, Searching.interpolationSearch(new int[]{1, 1}, 1));
    }

    // --- exponentialSearch ---
    @Test
    void testExponentialSearch() {
        assertEquals(0, Searching.exponentialSearch(SORTED, 1));
        assertEquals(4, Searching.exponentialSearch(SORTED, 9));
        assertEquals(9, Searching.exponentialSearch(SORTED, 19));
        assertEquals(-1, Searching.exponentialSearch(SORTED, 6));
        assertEquals(-1, Searching.exponentialSearch(EMPTY, 5));
        assertEquals(0, Searching.exponentialSearch(SINGLE, 42));
        assertEquals(-1, Searching.exponentialSearch(SINGLE, 10));
    }

    // --- ternarySearch ---
    @Test
    void testTernarySearch() {
        assertEquals(0, Searching.ternarySearch(SORTED, 1));
        assertEquals(4, Searching.ternarySearch(SORTED, 9));
        assertEquals(9, Searching.ternarySearch(SORTED, 19));
        assertEquals(-1, Searching.ternarySearch(SORTED, 6));
        assertEquals(-1, Searching.ternarySearch(EMPTY, 5));
        assertEquals(0, Searching.ternarySearch(SINGLE, 42));
        assertEquals(-1, Searching.ternarySearch(SINGLE, 10));
    }

    // --- findFirstOccurrence ---
    @Test
    void testFindFirstOccurrence() {
        // {1, 2, 2, 2, 3, 4, 4, 5}
        assertEquals(1, Searching.findFirstOccurrence(DUPS, 2));
        assertEquals(0, Searching.findFirstOccurrence(DUPS, 1));
        assertEquals(5, Searching.findFirstOccurrence(DUPS, 4));
        assertEquals(7, Searching.findFirstOccurrence(DUPS, 5));
        assertEquals(-1, Searching.findFirstOccurrence(DUPS, 6));
        assertEquals(-1, Searching.findFirstOccurrence(EMPTY, 1));
    }

    // --- findLastOccurrence ---
    @Test
    void testFindLastOccurrence() {
        // {1, 2, 2, 2, 3, 4, 4, 5}
        assertEquals(3, Searching.findLastOccurrence(DUPS, 2));
        assertEquals(0, Searching.findLastOccurrence(DUPS, 1));
        assertEquals(6, Searching.findLastOccurrence(DUPS, 4));
        assertEquals(7, Searching.findLastOccurrence(DUPS, 5));
        assertEquals(-1, Searching.findLastOccurrence(DUPS, 6));
    }

    // --- countOccurrences ---
    @Test
    void testCountOccurrences() {
        // {1, 2, 2, 2, 3, 4, 4, 5}
        assertEquals(3, Searching.countOccurrences(DUPS, 2));
        assertEquals(1, Searching.countOccurrences(DUPS, 1));
        assertEquals(2, Searching.countOccurrences(DUPS, 4));
        assertEquals(0, Searching.countOccurrences(DUPS, 6));
        assertEquals(0, Searching.countOccurrences(EMPTY, 1));
    }

    // --- searchInsertPosition ---
    @Test
    void testSearchInsertPosition() {
        // {1, 3, 5, 7, 9, 11, 13, 15, 17, 19}
        assertEquals(0, Searching.searchInsertPosition(SORTED, 1));   // exact
        assertEquals(0, Searching.searchInsertPosition(SORTED, 0));   // before all
        assertEquals(10, Searching.searchInsertPosition(SORTED, 20)); // after all
        assertEquals(2, Searching.searchInsertPosition(SORTED, 4));   // between 3 and 5
        assertEquals(2, Searching.searchInsertPosition(SORTED, 5));   // exact
        assertEquals(0, Searching.searchInsertPosition(EMPTY, 5));
    }

    // --- findPeakElement ---
    @Test
    void testFindPeakElement() {
        int[] arr1 = {1, 3, 5, 4, 2};
        int peak1 = Searching.findPeakElement(arr1);
        assertTrue(peak1 >= 0 && peak1 < arr1.length);
        // Verify it's actually a peak
        if (peak1 > 0) assertTrue(arr1[peak1] > arr1[peak1 - 1]);
        if (peak1 < arr1.length - 1) assertTrue(arr1[peak1] > arr1[peak1 + 1]);

        int[] arr2 = {1, 2, 3, 4, 5};
        int peak2 = Searching.findPeakElement(arr2);
        assertEquals(4, peak2); // last element is a peak

        int[] arr3 = {5, 4, 3, 2, 1};
        int peak3 = Searching.findPeakElement(arr3);
        assertEquals(0, peak3); // first element is a peak

        assertEquals(0, Searching.findPeakElement(SINGLE));
        assertEquals(-1, Searching.findPeakElement(EMPTY));

        int[] arr4 = {1, 3, 2};
        assertEquals(1, Searching.findPeakElement(arr4));
    }

    // --- searchInRotatedArray ---
    @Test
    void testSearchInRotatedArray() {
        int[] rotated = {4, 5, 6, 7, 0, 1, 2};
        assertEquals(0, Searching.searchInRotatedArray(rotated, 4));
        assertEquals(1, Searching.searchInRotatedArray(rotated, 5));
        assertEquals(4, Searching.searchInRotatedArray(rotated, 0));
        assertEquals(6, Searching.searchInRotatedArray(rotated, 2));
        assertEquals(-1, Searching.searchInRotatedArray(rotated, 3));
        assertEquals(-1, Searching.searchInRotatedArray(EMPTY, 5));

        int[] notRotated = {1, 2, 3, 4, 5};
        assertEquals(0, Searching.searchInRotatedArray(notRotated, 1));
        assertEquals(4, Searching.searchInRotatedArray(notRotated, 5));
        assertEquals(-1, Searching.searchInRotatedArray(notRotated, 6));

        int[] singleRotated = {2, 1};
        assertEquals(0, Searching.searchInRotatedArray(singleRotated, 2));
        assertEquals(1, Searching.searchInRotatedArray(singleRotated, 1));
        assertEquals(-1, Searching.searchInRotatedArray(singleRotated, 3));
    }

    // --- Additional edge case tests ---
    @Test
    void testSearchAllElementsPresent() {
        for (int val : SORTED) {
            assertNotEquals(-1, Searching.binarySearch(SORTED, val));
            assertNotEquals(-1, Searching.binarySearchRecursive(SORTED, val));
            assertNotEquals(-1, Searching.linearSearch(SORTED, val));
            assertNotEquals(-1, Searching.jumpSearch(SORTED, val));
            assertNotEquals(-1, Searching.exponentialSearch(SORTED, val));
            assertNotEquals(-1, Searching.ternarySearch(SORTED, val));
            assertNotEquals(-1, Searching.interpolationSearch(SORTED, val));
        }
    }

    @Test
    void testTwoElementArray() {
        int[] two = {10, 20};
        assertEquals(0, Searching.binarySearch(two, 10));
        assertEquals(1, Searching.binarySearch(two, 20));
        assertEquals(-1, Searching.binarySearch(two, 15));
        assertEquals(0, Searching.jumpSearch(two, 10));
        assertEquals(1, Searching.jumpSearch(two, 20));
    }

    @Test
    void testInterpolationSearchUniformDistribution() {
        int[] uniform = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        assertEquals(0, Searching.interpolationSearch(uniform, 10));
        assertEquals(4, Searching.interpolationSearch(uniform, 50));
        assertEquals(9, Searching.interpolationSearch(uniform, 100));
        assertEquals(-1, Searching.interpolationSearch(uniform, 55));
    }

    // --- twoSumSorted ---
    @Test
    void testTwoSumSortedFound() {
        int[] arr = {1, 2, 3, 4, 6};
        int[] result = Searching.twoSumSorted(arr, 6);
        assertEquals(2, result.length);
        assertEquals(arr[result[0]] + arr[result[1]], 6);
    }

    @Test
    void testTwoSumSortedNotFound() {
        int[] arr = {1, 2, 3, 4, 6};
        int[] result = Searching.twoSumSorted(arr, 20);
        assertEquals(0, result.length);
    }

    @Test
    void testTwoSumSortedWithDuplicates() {
        int[] arr = {1, 1, 2, 3, 4};
        int[] result = Searching.twoSumSorted(arr, 2);
        assertEquals(2, result.length);
        assertEquals(0, result[0]);
        assertEquals(1, result[1]);
    }

    // --- threeSum ---
    @Test
    void testThreeSumMultipleTriplets() {
        int[] nums = {-1, 0, 1, 2, -1, -4};
        List<List<Integer>> result = Searching.threeSum(nums);
        assertEquals(2, result.size());
        assertTrue(result.contains(List.of(-1, -1, 2)));
        assertTrue(result.contains(List.of(-1, 0, 1)));
    }

    @Test
    void testThreeSumNoSolution() {
        int[] nums = {1, 2, 3};
        List<List<Integer>> result = Searching.threeSum(nums);
        assertTrue(result.isEmpty());
    }

    @Test
    void testThreeSumAllZeros() {
        int[] nums = {0, 0, 0, 0};
        List<List<Integer>> result = Searching.threeSum(nums);
        assertEquals(1, result.size());
        assertEquals(List.of(0, 0, 0), result.get(0));
    }

    @Test
    void testThreeSumDuplicatesHandling() {
        int[] nums = {-2, 0, 0, 2, 2};
        List<List<Integer>> result = Searching.threeSum(nums);
        assertEquals(1, result.size());
        assertEquals(List.of(-2, 0, 2), result.get(0));
    }

    // --- containerWithMostWater ---
    @Test
    void testContainerWithMostWaterBasic() {
        int[] height = {1, 8, 6, 2, 5, 4, 8, 3, 7};
        assertEquals(49, Searching.containerWithMostWater(height));
    }

    @Test
    void testContainerWithMostWaterSingleHeight() {
        int[] height = {1, 1};
        assertEquals(1, Searching.containerWithMostWater(height));
    }

    @Test
    void testContainerWithMostWaterEqualHeights() {
        int[] height = {5, 5, 5, 5};
        assertEquals(15, Searching.containerWithMostWater(height));
    }

    // --- removeDuplicatesSorted ---
    @Test
    void testRemoveDuplicatesSorted() {
        int[] arr = {1, 1, 2, 2, 3, 4, 4, 5};
        int newLen = Searching.removeDuplicatesSorted(arr);
        assertEquals(5, newLen);
        assertEquals(1, arr[0]);
        assertEquals(2, arr[1]);
        assertEquals(3, arr[2]);
        assertEquals(4, arr[3]);
        assertEquals(5, arr[4]);
    }

    @Test
    void testRemoveDuplicatesSortedEmpty() {
        assertEquals(0, Searching.removeDuplicatesSorted(new int[]{}));
    }

    @Test
    void testRemoveDuplicatesSortedNoDups() {
        int[] arr = {1, 2, 3};
        assertEquals(3, Searching.removeDuplicatesSorted(arr));
    }

    // --- longestSubstringWithoutRepeating ---
    @Test
    void testLongestSubstringWithoutRepeating() {
        assertEquals(3, Searching.longestSubstringWithoutRepeating("abcabcbb"));
        assertEquals(1, Searching.longestSubstringWithoutRepeating("bbbbb"));
        assertEquals(0, Searching.longestSubstringWithoutRepeating(""));
        assertEquals(3, Searching.longestSubstringWithoutRepeating("pwwkew"));
    }

    // --- minWindowSubstring ---
    @Test
    void testMinWindowSubstringBasic() {
        assertEquals("BANC", Searching.minWindowSubstring("ADOBECODEBANC", "ABC"));
    }

    @Test
    void testMinWindowSubstringNoValid() {
        assertEquals("", Searching.minWindowSubstring("abc", "xyz"));
    }

    @Test
    void testMinWindowSubstringExactMatch() {
        assertEquals("a", Searching.minWindowSubstring("a", "a"));
    }

    // --- findAllAnagrams ---
    @Test
    void testFindAllAnagrams() {
        assertEquals(List.of(0, 6), Searching.findAllAnagrams("cbaebabacd", "abc"));
    }

    @Test
    void testFindAllAnagramsNoAnagrams() {
        assertEquals(List.of(), Searching.findAllAnagrams("abcdef", "xyz"));
    }

    @Test
    void testFindAllAnagramsAllMatch() {
        assertEquals(List.of(0, 1, 2), Searching.findAllAnagrams("abab", "ab"));
    }

    // --- maxSumSubarray ---
    @Test
    void testMaxSumSubarray() {
        assertEquals(12, Searching.maxSumSubarray(new int[]{1, 2, 3, 4, 5}, 3));
        assertEquals(15, Searching.maxSumSubarray(new int[]{1, 2, 3, 4, 5}, 5));
        assertEquals(5, Searching.maxSumSubarray(new int[]{5, 1, 2, 3, 4}, 1));
    }

    // --- longestSubstringKDistinct ---
    @Test
    void testLongestSubstringKDistinct() {
        assertEquals(3, Searching.longestSubstringKDistinct("eceba", 2));
        assertEquals(4, Searching.longestSubstringKDistinct("eceba", 3));
        assertEquals(0, Searching.longestSubstringKDistinct("abc", 0));
        assertEquals(1, Searching.longestSubstringKDistinct("abc", 1));
    }

    // --- findRotationPoint ---
    @Test
    void testFindRotationPointRotated() {
        assertEquals(4, Searching.findRotationPoint(new int[]{4, 5, 6, 7, 0, 1, 2}));
    }

    @Test
    void testFindRotationPointNotRotated() {
        assertEquals(0, Searching.findRotationPoint(new int[]{1, 2, 3, 4, 5}));
    }

    @Test
    void testFindRotationPointSingleElement() {
        assertEquals(0, Searching.findRotationPoint(new int[]{1}));
    }

    @Test
    void testFindRotationPointEmpty() {
        assertEquals(-1, Searching.findRotationPoint(new int[]{}));
    }

    @Test
    void testFindRotationPointTwoElements() {
        assertEquals(1, Searching.findRotationPoint(new int[]{2, 1}));
    }

    // --- sqrtInt ---
    @Test
    void testSqrtIntPerfectSquares() {
        assertEquals(0, Searching.sqrtInt(0));
        assertEquals(1, Searching.sqrtInt(1));
        assertEquals(2, Searching.sqrtInt(4));
        assertEquals(3, Searching.sqrtInt(9));
        assertEquals(10, Searching.sqrtInt(100));
    }

    @Test
    void testSqrtIntNonPerfect() {
        assertEquals(1, Searching.sqrtInt(2));
        assertEquals(1, Searching.sqrtInt(3));
        assertEquals(2, Searching.sqrtInt(8));
        assertEquals(3, Searching.sqrtInt(10));
        assertEquals(4, Searching.sqrtInt(24));
    }

    @Test
    void testSqrtIntLargeValue() {
        assertEquals(46340, Searching.sqrtInt(Integer.MAX_VALUE));
    }
}
