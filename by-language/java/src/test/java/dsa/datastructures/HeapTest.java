package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

class HeapTest {

    // ==================== MinHeap Tests ====================

    @Test
    void minHeapPushAndPeek() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        heap.push(1);
        assertEquals(1, heap.peek());
    }

    @Test
    void minHeapPop() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        heap.push(1);
        assertEquals(1, heap.pop());
        assertEquals(3, heap.pop());
        assertEquals(5, heap.pop());
        assertEquals(7, heap.pop());
    }

    @Test
    void minHeapPopEmpty() {
        var heap = new Heap.MinHeap<Integer>();
        assertThrows(NoSuchElementException.class, heap::pop);
    }

    @Test
    void minHeapPeekEmpty() {
        var heap = new Heap.MinHeap<Integer>();
        assertThrows(NoSuchElementException.class, heap::peek);
    }

    @Test
    void minHeapPushPop() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(5);
        heap.push(3);
        // pushpop with value smaller than min -> returns the value itself
        assertEquals(1, heap.pushpop(1));
        assertEquals(3, heap.peek());
        // pushpop with value larger than min -> returns old min, inserts new
        assertEquals(3, heap.pushpop(10));
        assertEquals(5, heap.peek());
    }

    @Test
    void minHeapReplace() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        assertEquals(3, heap.replace(4));
        assertEquals(4, heap.peek());
    }

    @Test
    void minHeapReplaceEmpty() {
        var heap = new Heap.MinHeap<Integer>();
        assertThrows(NoSuchElementException.class, () -> heap.replace(1));
    }

    @Test
    void minHeapSizeAndEmpty() {
        var heap = new Heap.MinHeap<Integer>();
        assertTrue(heap.isEmpty());
        assertEquals(0, heap.size());
        heap.push(1);
        assertFalse(heap.isEmpty());
        assertEquals(1, heap.size());
    }

    @Test
    void minHeapClear() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(1);
        heap.push(2);
        heap.clear();
        assertTrue(heap.isEmpty());
        assertEquals(0, heap.size());
    }

    @Test
    void minHeapToSortedList() {
        var heap = new Heap.MinHeap<Integer>();
        heap.push(5);
        heap.push(1);
        heap.push(3);
        heap.push(2);
        heap.push(4);
        List<Integer> sorted = heap.toSortedList();
        assertEquals(List.of(1, 2, 3, 4, 5), sorted);
        // Original heap should be unchanged
        assertEquals(5, heap.size());
    }

    @Test
    void minHeapHeapify() {
        var heap = new Heap.MinHeap<Integer>();
        heap.heapify(List.of(5, 3, 7, 1, 9, 2));
        assertEquals(1, heap.pop());
        assertEquals(2, heap.pop());
        assertEquals(3, heap.pop());
    }

    @Test
    void minHeapFromList() {
        var heap = Heap.MinHeap.fromList(List.of(5, 3, 7, 1, 9, 2));
        assertEquals(6, heap.size());
        assertEquals(1, heap.peek());
    }

    @Test
    void minHeapWithStrings() {
        var heap = new Heap.MinHeap<String>();
        heap.push("banana");
        heap.push("apple");
        heap.push("cherry");
        assertEquals("apple", heap.pop());
        assertEquals("banana", heap.pop());
        assertEquals("cherry", heap.pop());
    }

    // ==================== MaxHeap Tests ====================

    @Test
    void maxHeapPushAndPeek() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        heap.push(1);
        assertEquals(7, heap.peek());
    }

    @Test
    void maxHeapPop() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        heap.push(1);
        assertEquals(7, heap.pop());
        assertEquals(5, heap.pop());
        assertEquals(3, heap.pop());
        assertEquals(1, heap.pop());
    }

    @Test
    void maxHeapPopEmpty() {
        var heap = new Heap.MaxHeap<Integer>();
        assertThrows(NoSuchElementException.class, heap::pop);
    }

    @Test
    void maxHeapPushPop() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(5);
        heap.push(3);
        // pushpop with value larger than max -> returns the value itself
        assertEquals(10, heap.pushpop(10));
        assertEquals(5, heap.peek());
        // pushpop with value smaller than max -> returns old max, inserts new
        assertEquals(5, heap.pushpop(1));
        assertEquals(3, heap.peek());
    }

    @Test
    void maxHeapReplace() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(5);
        heap.push(3);
        heap.push(7);
        assertEquals(7, heap.replace(4));
        assertEquals(5, heap.peek());
    }

    @Test
    void maxHeapSizeAndEmpty() {
        var heap = new Heap.MaxHeap<Integer>();
        assertTrue(heap.isEmpty());
        heap.push(1);
        assertFalse(heap.isEmpty());
        assertEquals(1, heap.size());
    }

    @Test
    void maxHeapClear() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(1);
        heap.push(2);
        heap.clear();
        assertTrue(heap.isEmpty());
    }

    @Test
    void maxHeapToSortedList() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.push(5);
        heap.push(1);
        heap.push(3);
        heap.push(2);
        heap.push(4);
        List<Integer> sorted = heap.toSortedList();
        assertEquals(List.of(5, 4, 3, 2, 1), sorted);
        assertEquals(5, heap.size());
    }

    @Test
    void maxHeapFromList() {
        var heap = Heap.MaxHeap.fromList(List.of(5, 3, 7, 1, 9, 2));
        assertEquals(6, heap.size());
        assertEquals(9, heap.peek());
    }

    @Test
    void maxHeapHeapify() {
        var heap = new Heap.MaxHeap<Integer>();
        heap.heapify(List.of(5, 3, 7, 1, 9, 2));
        assertEquals(9, heap.pop());
        assertEquals(7, heap.pop());
        assertEquals(5, heap.pop());
    }

    // ==================== PriorityQueue Tests ====================

    @Test
    void priorityQueueNaturalOrder() {
        var pq = new Heap.PriorityQueue<Integer>();
        pq.enqueue(5);
        pq.enqueue(1);
        pq.enqueue(3);
        assertEquals(1, pq.dequeue());
        assertEquals(3, pq.dequeue());
        assertEquals(5, pq.dequeue());
    }

    @Test
    void priorityQueueCustomComparator() {
        // Reverse order (max first)
        var pq = new Heap.PriorityQueue<Integer>((a, b) -> b - a);
        pq.enqueue(5);
        pq.enqueue(1);
        pq.enqueue(3);
        assertEquals(5, pq.dequeue());
        assertEquals(3, pq.dequeue());
        assertEquals(1, pq.dequeue());
    }

    @Test
    void priorityQueuePeek() {
        var pq = new Heap.PriorityQueue<Integer>();
        pq.enqueue(3);
        pq.enqueue(1);
        assertEquals(1, pq.peek());
        assertEquals(2, pq.size()); // peek shouldn't remove
    }

    @Test
    void priorityQueueEmptyOperations() {
        var pq = new Heap.PriorityQueue<Integer>();
        assertTrue(pq.isEmpty());
        assertThrows(NoSuchElementException.class, pq::dequeue);
        assertThrows(NoSuchElementException.class, pq::peek);
    }

    @Test
    void priorityQueueSizeAndClear() {
        var pq = new Heap.PriorityQueue<String>();
        pq.enqueue("b");
        pq.enqueue("a");
        assertEquals(2, pq.size());
        pq.clear();
        assertEquals(0, pq.size());
        assertTrue(pq.isEmpty());
    }

    @Test
    void priorityQueueWithStringLength() {
        var pq = new Heap.PriorityQueue<String>((a, b) -> Integer.compare(a.length(), b.length()));
        pq.enqueue("medium");
        pq.enqueue("hi");
        pq.enqueue("superlongstring");
        assertEquals("hi", pq.dequeue());
        assertEquals("medium", pq.dequeue());
        assertEquals("superlongstring", pq.dequeue());
    }

    // ==================== Static Method Tests ====================

    @Test
    void heapSortBasic() {
        int[] arr = {5, 3, 8, 1, 2, 7, 4, 6};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{1, 2, 3, 4, 5, 6, 7, 8}, arr);
    }

    @Test
    void heapSortAlreadySorted() {
        int[] arr = {1, 2, 3, 4, 5};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{1, 2, 3, 4, 5}, arr);
    }

    @Test
    void heapSortReverseSorted() {
        int[] arr = {5, 4, 3, 2, 1};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{1, 2, 3, 4, 5}, arr);
    }

    @Test
    void heapSortSingleElement() {
        int[] arr = {42};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{42}, arr);
    }

    @Test
    void heapSortEmpty() {
        int[] arr = {};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{}, arr);
    }

    @Test
    void heapSortDuplicates() {
        int[] arr = {3, 1, 2, 1, 3, 2};
        Heap.heapSort(arr);
        assertArrayEquals(new int[]{1, 1, 2, 2, 3, 3}, arr);
    }

    @Test
    void nLargestBasic() {
        List<Integer> result = Heap.nLargest(3, List.of(5, 1, 8, 3, 9, 2, 7));
        assertEquals(3, result.size());
        assertEquals(List.of(9, 8, 7), result);
    }

    @Test
    void nLargestMoreThanAvailable() {
        List<Integer> result = Heap.nLargest(10, List.of(3, 1, 2));
        assertEquals(3, result.size());
        assertTrue(result.contains(1));
        assertTrue(result.contains(2));
        assertTrue(result.contains(3));
    }

    @Test
    void nSmallestBasic() {
        List<Integer> result = Heap.nSmallest(3, List.of(5, 1, 8, 3, 9, 2, 7));
        assertEquals(3, result.size());
        assertEquals(List.of(1, 2, 3), result);
    }

    @Test
    void nSmallestMoreThanAvailable() {
        List<Integer> result = Heap.nSmallest(10, List.of(3, 1, 2));
        assertEquals(3, result.size());
        assertTrue(result.contains(1));
        assertTrue(result.contains(2));
        assertTrue(result.contains(3));
    }

    @Test
    void nLargestWithStrings() {
        List<String> result = Heap.nLargest(2, List.of("banana", "apple", "cherry", "date"));
        assertEquals(2, result.size());
        assertEquals("date", result.get(0));
        assertEquals("cherry", result.get(1));
    }

    @Test
    void nSmallestWithStrings() {
        List<String> result = Heap.nSmallest(2, List.of("banana", "apple", "cherry", "date"));
        assertEquals(2, result.size());
        assertEquals("apple", result.get(0));
        assertEquals("banana", result.get(1));
    }

    // ==================== kWayMerge Tests ====================

    @Test
    void kWayMergeBasic() {
        List<List<Integer>> lists = List.of(
                List.of(1, 4, 7),
                List.of(2, 5, 8),
                List.of(3, 6, 9)
        );
        List<Integer> result = Heap.kWayMerge(lists);
        assertEquals(List.of(1, 2, 3, 4, 5, 6, 7, 8, 9), result);
    }

    @Test
    void kWayMergeDifferentLengths() {
        List<List<Integer>> lists = List.of(
                List.of(1, 3, 5, 7, 9),
                List.of(2),
                List.of(4, 6)
        );
        List<Integer> result = Heap.kWayMerge(lists);
        assertEquals(List.of(1, 2, 3, 4, 5, 6, 7, 9), result);
    }

    @Test
    void kWayMergeEmpty() {
        List<Integer> result = Heap.kWayMerge(List.of());
        assertTrue(result.isEmpty());
    }

    @Test
    void kWayMergeNull() {
        List<Integer> result = Heap.kWayMerge(null);
        assertTrue(result.isEmpty());
    }

    @Test
    void kWayMergeSingleList() {
        List<List<Integer>> lists = List.of(List.of(1, 2, 3));
        List<Integer> result = Heap.kWayMerge(lists);
        assertEquals(List.of(1, 2, 3), result);
    }

    @Test
    void kWayMergeWithStrings() {
        List<List<String>> lists = List.of(
                List.of("apple", "cherry"),
                List.of("banana", "date")
        );
        List<String> result = Heap.kWayMerge(lists);
        assertEquals(List.of("apple", "banana", "cherry", "date"), result);
    }

    // ==================== topKFrequent Tests ====================

    @Test
    void topKFrequentBasic() {
        int[] result = Heap.topKFrequent(new int[]{1, 1, 1, 2, 2, 3}, 2);
        assertEquals(2, result.length);
        // Result should contain 1 and 2 (most frequent)
        Arrays.sort(result);
        assertArrayEquals(new int[]{1, 2}, result);
    }

    @Test
    void topKFrequentSingleElement() {
        int[] result = Heap.topKFrequent(new int[]{1}, 1);
        assertArrayEquals(new int[]{1}, result);
    }

    @Test
    void topKFrequentAllSameFrequency() {
        int[] result = Heap.topKFrequent(new int[]{1, 2, 3}, 2);
        assertEquals(2, result.length);
    }

    @Test
    void topKFrequentAllElements() {
        int[] result = Heap.topKFrequent(new int[]{3, 3, 1, 1, 2, 2}, 3);
        assertEquals(3, result.length);
        Arrays.sort(result);
        assertArrayEquals(new int[]{1, 2, 3}, result);
    }

    // ==================== MedianFinder Tests ====================

    @Test
    void medianFinderBasic() {
        var mf = new Heap.MedianFinder();
        mf.addNum(1);
        assertEquals(1.0, mf.findMedian());
        mf.addNum(2);
        assertEquals(1.5, mf.findMedian());
        mf.addNum(3);
        assertEquals(2.0, mf.findMedian());
    }

    @Test
    void medianFinderDescendingOrder() {
        var mf = new Heap.MedianFinder();
        mf.addNum(5);
        mf.addNum(4);
        mf.addNum(3);
        mf.addNum(2);
        mf.addNum(1);
        assertEquals(3.0, mf.findMedian());
    }

    @Test
    void medianFinderEvenCount() {
        var mf = new Heap.MedianFinder();
        mf.addNum(1);
        mf.addNum(3);
        mf.addNum(2);
        mf.addNum(4);
        assertEquals(2.5, mf.findMedian());
    }

    @Test
    void medianFinderSize() {
        var mf = new Heap.MedianFinder();
        assertEquals(0, mf.size());
        mf.addNum(10);
        mf.addNum(20);
        assertEquals(2, mf.size());
    }

    @Test
    void medianFinderEmptyThrows() {
        var mf = new Heap.MedianFinder();
        assertThrows(NoSuchElementException.class, mf::findMedian);
    }

    @Test
    void medianFinderDuplicates() {
        var mf = new Heap.MedianFinder();
        mf.addNum(5);
        mf.addNum(5);
        mf.addNum(5);
        assertEquals(5.0, mf.findMedian());
    }

    // ==================== findMedianSortedArrays Tests ====================

    @Test
    void findMedianSortedArraysBasic() {
        assertEquals(2.0, Heap.findMedianSortedArrays(new int[]{1, 3}, new int[]{2}));
    }

    @Test
    void findMedianSortedArraysEvenTotal() {
        assertEquals(2.5, Heap.findMedianSortedArrays(new int[]{1, 2}, new int[]{3, 4}));
    }

    @Test
    void findMedianSortedArraysOneEmpty() {
        assertEquals(2.0, Heap.findMedianSortedArrays(new int[]{}, new int[]{1, 2, 3}));
    }

    @Test
    void findMedianSortedArraysSingleElements() {
        assertEquals(1.5, Heap.findMedianSortedArrays(new int[]{1}, new int[]{2}));
    }

    @Test
    void findMedianSortedArraysBothEmpty() {
        assertThrows(IllegalArgumentException.class,
                () -> Heap.findMedianSortedArrays(new int[]{}, new int[]{}));
    }
}
