package dsa.algorithms;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Random;
import java.util.function.Function;

import static org.junit.jupiter.api.Assertions.*;

class SortingTest {

    private static final int[] EMPTY = {};
    private static final int[] SINGLE = {42};
    private static final int[] SORTED = {1, 2, 3, 4, 5};
    private static final int[] REVERSE = {5, 4, 3, 2, 1};
    private static final int[] DUPLICATES = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    private static final int[] RANDOM = {38, 27, 43, 3, 9, 82, 10};
    private static final int[] ALL_SAME = {7, 7, 7, 7, 7};
    private static final int[] TWO_ELEMENTS = {2, 1};

    private void assertSortsCorrectly(Function<int[], int[]> sortFn, int[] input) {
        int[] original = input.clone();
        int[] result = sortFn.apply(input);
        // Input should not be modified
        assertArrayEquals(original, input, "Input array should not be modified");
        // Result should be sorted
        assertTrue(Sorting.isSorted(result), "Result should be sorted");
        // Result should have same length
        assertEquals(input.length, result.length, "Result should have same length");
        // Result should contain same elements
        int[] expectedSorted = input.clone();
        Arrays.sort(expectedSorted);
        assertArrayEquals(expectedSorted, result, "Result should match Arrays.sort output");
    }

    private void testSortAlgorithm(Function<int[], int[]> sortFn) {
        assertSortsCorrectly(sortFn, EMPTY);
        assertSortsCorrectly(sortFn, SINGLE);
        assertSortsCorrectly(sortFn, SORTED);
        assertSortsCorrectly(sortFn, REVERSE);
        assertSortsCorrectly(sortFn, DUPLICATES);
        assertSortsCorrectly(sortFn, RANDOM);
        assertSortsCorrectly(sortFn, ALL_SAME);
        assertSortsCorrectly(sortFn, TWO_ELEMENTS);
    }

    // Non-negative only sort algorithms
    private void testNonNegativeSortAlgorithm(Function<int[], int[]> sortFn) {
        assertSortsCorrectly(sortFn, EMPTY);
        assertSortsCorrectly(sortFn, SINGLE);
        assertSortsCorrectly(sortFn, SORTED);
        assertSortsCorrectly(sortFn, REVERSE);
        assertSortsCorrectly(sortFn, DUPLICATES);
        assertSortsCorrectly(sortFn, RANDOM);
        assertSortsCorrectly(sortFn, ALL_SAME);
        assertSortsCorrectly(sortFn, TWO_ELEMENTS);
        assertSortsCorrectly(sortFn, new int[]{0, 0, 0});
        assertSortsCorrectly(sortFn, new int[]{100, 0, 50, 25, 75});
    }

    @Test
    void testBubbleSort() {
        testSortAlgorithm(Sorting::bubbleSort);
    }

    @Test
    void testSelectionSort() {
        testSortAlgorithm(Sorting::selectionSort);
    }

    @Test
    void testInsertionSort() {
        testSortAlgorithm(Sorting::insertionSort);
    }

    @Test
    void testMergeSort() {
        testSortAlgorithm(Sorting::mergeSort);
    }

    @Test
    void testQuickSort() {
        testSortAlgorithm(Sorting::quickSort);
    }

    @Test
    void testHeapSort() {
        testSortAlgorithm(Sorting::heapSort);
    }

    @Test
    void testRadixSort() {
        testNonNegativeSortAlgorithm(Sorting::radixSort);
    }

    @Test
    void testCountingSort() {
        testNonNegativeSortAlgorithm(Sorting::countingSort);
    }

    @Test
    void testShellSort() {
        testSortAlgorithm(Sorting::shellSort);
    }

    @Test
    void testIsSorted() {
        assertTrue(Sorting.isSorted(EMPTY));
        assertTrue(Sorting.isSorted(SINGLE));
        assertTrue(Sorting.isSorted(SORTED));
        assertFalse(Sorting.isSorted(REVERSE));
        assertFalse(Sorting.isSorted(DUPLICATES));
        assertTrue(Sorting.isSorted(ALL_SAME));
        assertTrue(Sorting.isSorted(new int[]{1, 1, 2, 3, 3, 4}));
        assertFalse(Sorting.isSorted(new int[]{1, 3, 2}));
    }

    @Test
    void testBubbleSortDoesNotModifyInput() {
        int[] input = {5, 3, 1, 4, 2};
        int[] copy = input.clone();
        Sorting.bubbleSort(input);
        assertArrayEquals(copy, input);
    }

    @Test
    void testLargerRandomArray() {
        Random rng = new Random(12345);
        int[] large = new int[200];
        for (int i = 0; i < large.length; i++) {
            large[i] = rng.nextInt(1000);
        }
        assertSortsCorrectly(Sorting::mergeSort, large);
        assertSortsCorrectly(Sorting::quickSort, large);
        assertSortsCorrectly(Sorting::heapSort, large);
        assertSortsCorrectly(Sorting::shellSort, large);
    }

    @Test
    void testNegativeNumbers() {
        int[] negatives = {-3, -1, -7, -4, -2};
        assertSortsCorrectly(Sorting::bubbleSort, negatives);
        assertSortsCorrectly(Sorting::selectionSort, negatives);
        assertSortsCorrectly(Sorting::insertionSort, negatives);
        assertSortsCorrectly(Sorting::mergeSort, negatives);
        assertSortsCorrectly(Sorting::quickSort, negatives);
        assertSortsCorrectly(Sorting::heapSort, negatives);
        assertSortsCorrectly(Sorting::shellSort, negatives);
    }

    @Test
    void testMixedPositiveNegative() {
        int[] mixed = {-5, 3, -1, 0, 7, -3, 2};
        assertSortsCorrectly(Sorting::bubbleSort, mixed);
        assertSortsCorrectly(Sorting::mergeSort, mixed);
        assertSortsCorrectly(Sorting::quickSort, mixed);
        assertSortsCorrectly(Sorting::heapSort, mixed);
    }
}
