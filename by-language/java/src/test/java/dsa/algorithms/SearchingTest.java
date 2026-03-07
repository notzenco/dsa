package dsa.algorithms;

import org.junit.jupiter.api.Test;

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
}
