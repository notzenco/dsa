"""
Tests for Heap implementations.
"""

import pytest
from data_structures.heap import (
    MinHeap, MaxHeap, PriorityQueue,
    heap_sort, nlargest, nsmallest
)


class TestMinHeapBasics:
    """Test basic MinHeap operations."""

    def test_init(self):
        """Test initialization."""
        heap = MinHeap()
        assert len(heap) == 0
        assert heap.is_empty
        assert not heap

    def test_push_single(self):
        """Test pushing single element."""
        heap = MinHeap()
        heap.push(5)
        assert len(heap) == 1
        assert heap.peek() == 5

    def test_push_multiple(self):
        """Test pushing multiple elements."""
        heap = MinHeap()
        heap.push(5)
        heap.push(3)
        heap.push(7)
        heap.push(1)

        assert len(heap) == 4
        assert heap.peek() == 1

    def test_pop(self):
        """Test popping elements."""
        heap = MinHeap()
        heap.push(5)
        heap.push(3)
        heap.push(7)
        heap.push(1)

        assert heap.pop() == 1
        assert heap.pop() == 3
        assert heap.pop() == 5
        assert heap.pop() == 7
        assert heap.is_empty

    def test_pop_empty_raises(self):
        """Test popping from empty heap."""
        heap = MinHeap()
        with pytest.raises(IndexError):
            heap.pop()

    def test_peek_empty_raises(self):
        """Test peeking empty heap."""
        heap = MinHeap()
        with pytest.raises(IndexError):
            heap.peek()


class TestMinHeapAdvanced:
    """Test advanced MinHeap operations."""

    def test_pushpop(self):
        """Test pushpop operation."""
        heap = MinHeap.from_list([2, 4, 6])

        # Push smaller, returns it immediately
        assert heap.pushpop(1) == 1
        assert heap.peek() == 2

        # Push larger, returns current min
        assert heap.pushpop(3) == 2
        assert heap.peek() == 3

    def test_replace(self):
        """Test replace operation."""
        heap = MinHeap.from_list([1, 3, 5])

        old_min = heap.replace(4)
        assert old_min == 1
        assert heap.peek() == 3

    def test_replace_empty_raises(self):
        """Test replace on empty heap."""
        heap = MinHeap()
        with pytest.raises(IndexError):
            heap.replace(1)

    def test_heapify(self):
        """Test heapify (Floyd's algorithm)."""
        heap = MinHeap.heapify([5, 3, 8, 1, 2, 9, 4])

        # Verify min-heap property
        result = []
        while heap:
            result.append(heap.pop())

        assert result == [1, 2, 3, 4, 5, 8, 9]

    def test_to_sorted_list(self):
        """Test to_sorted_list."""
        heap = MinHeap.from_list([5, 3, 8, 1, 2])
        sorted_list = heap.to_sorted_list()

        assert sorted_list == [1, 2, 3, 5, 8]
        # Original heap should be unchanged
        assert len(heap) == 5

    def test_contains(self):
        """Test __contains__."""
        heap = MinHeap.from_list([1, 2, 3])
        assert 2 in heap
        assert 5 not in heap

    def test_clear(self):
        """Test clear."""
        heap = MinHeap.from_list([1, 2, 3])
        heap.clear()
        assert heap.is_empty


class TestMaxHeapBasics:
    """Test basic MaxHeap operations."""

    def test_init(self):
        """Test initialization."""
        heap = MaxHeap()
        assert heap.is_empty

    def test_push_and_pop(self):
        """Test push and pop maintain max-heap property."""
        heap = MaxHeap()
        heap.push(5)
        heap.push(3)
        heap.push(7)
        heap.push(1)

        assert heap.pop() == 7
        assert heap.pop() == 5
        assert heap.pop() == 3
        assert heap.pop() == 1

    def test_peek(self):
        """Test peek returns maximum."""
        heap = MaxHeap.from_list([1, 5, 3])
        assert heap.peek() == 5

    def test_heapify(self):
        """Test heapify."""
        heap = MaxHeap.heapify([3, 1, 4, 1, 5, 9, 2, 6])

        result = []
        while heap:
            result.append(heap.pop())

        assert result == [9, 6, 5, 4, 3, 2, 1, 1]

    def test_pushpop(self):
        """Test pushpop."""
        heap = MaxHeap.from_list([4, 2, 6])

        # Push larger, returns it
        assert heap.pushpop(8) == 8
        assert heap.peek() == 6

        # Push smaller, returns current max
        assert heap.pushpop(3) == 6
        assert heap.peek() == 4

    def test_to_sorted_list(self):
        """Test to_sorted_list (descending)."""
        heap = MaxHeap.from_list([3, 1, 4, 1, 5])
        sorted_list = heap.to_sorted_list()

        assert sorted_list == [5, 4, 3, 1, 1]


class TestPriorityQueue:
    """Test PriorityQueue."""

    def test_init(self):
        """Test initialization."""
        pq = PriorityQueue()
        assert pq.is_empty

    def test_enqueue_dequeue(self):
        """Test basic enqueue and dequeue."""
        pq = PriorityQueue()
        pq.enqueue(5)
        pq.enqueue(1)
        pq.enqueue(3)

        assert pq.dequeue() == 1
        assert pq.dequeue() == 3
        assert pq.dequeue() == 5

    def test_with_key_function(self):
        """Test with custom key function."""
        # Priority by string length
        pq = PriorityQueue(key=len)
        pq.enqueue("hello")      # len=5
        pq.enqueue("hi")         # len=2
        pq.enqueue("hey")        # len=3

        assert pq.dequeue() == "hi"
        assert pq.dequeue() == "hey"
        assert pq.dequeue() == "hello"

    def test_with_tuple_priority(self):
        """Test with tuple elements (priority, value)."""
        pq = PriorityQueue(key=lambda x: x[0])
        pq.enqueue((3, "low"))
        pq.enqueue((1, "high"))
        pq.enqueue((2, "medium"))

        assert pq.dequeue() == (1, "high")
        assert pq.dequeue() == (2, "medium")
        assert pq.dequeue() == (3, "low")

    def test_stable_ordering(self):
        """Test that equal priorities maintain insertion order."""
        pq = PriorityQueue(key=lambda x: x[0])
        pq.enqueue((1, "first"))
        pq.enqueue((1, "second"))
        pq.enqueue((1, "third"))

        assert pq.dequeue() == (1, "first")
        assert pq.dequeue() == (1, "second")
        assert pq.dequeue() == (1, "third")

    def test_peek(self):
        """Test peek."""
        pq = PriorityQueue()
        pq.enqueue(5)
        pq.enqueue(1)

        assert pq.peek() == 1
        assert len(pq) == 2

    def test_dequeue_empty_raises(self):
        """Test dequeue from empty queue."""
        pq = PriorityQueue()
        with pytest.raises(IndexError):
            pq.dequeue()

    def test_clear(self):
        """Test clear."""
        pq = PriorityQueue()
        pq.enqueue(1)
        pq.enqueue(2)
        pq.clear()
        assert pq.is_empty


class TestHeapSort:
    """Test heap_sort function."""

    def test_empty(self):
        """Test sorting empty list."""
        assert heap_sort([]) == []

    def test_single(self):
        """Test sorting single element."""
        assert heap_sort([42]) == [42]

    def test_sorted(self):
        """Test sorting already sorted list."""
        assert heap_sort([1, 2, 3, 4, 5]) == [1, 2, 3, 4, 5]

    def test_reverse(self):
        """Test sorting reverse sorted list."""
        assert heap_sort([5, 4, 3, 2, 1]) == [1, 2, 3, 4, 5]

    def test_random(self):
        """Test sorting random list."""
        assert heap_sort([3, 1, 4, 1, 5, 9, 2, 6]) == [1, 1, 2, 3, 4, 5, 6, 9]

    def test_duplicates(self):
        """Test sorting with duplicates."""
        assert heap_sort([5, 5, 5, 1, 1]) == [1, 1, 5, 5, 5]


class TestNLargestNSmallest:
    """Test nlargest and nsmallest functions."""

    def test_nlargest_basic(self):
        """Test nlargest basic case."""
        result = nlargest(3, [1, 5, 2, 8, 3, 9, 4])
        assert result == [9, 8, 5]

    def test_nlargest_all(self):
        """Test nlargest when n >= len."""
        result = nlargest(10, [3, 1, 2])
        assert result == [3, 2, 1]

    def test_nlargest_zero(self):
        """Test nlargest with n=0."""
        assert nlargest(0, [1, 2, 3]) == []

    def test_nsmallest_basic(self):
        """Test nsmallest basic case."""
        result = nsmallest(3, [1, 5, 2, 8, 3, 9, 4])
        assert result == [1, 2, 3]

    def test_nsmallest_all(self):
        """Test nsmallest when n >= len."""
        result = nsmallest(10, [3, 1, 2])
        assert result == [1, 2, 3]

    def test_nsmallest_zero(self):
        """Test nsmallest with n=0."""
        assert nsmallest(0, [1, 2, 3]) == []


class TestHeapEdgeCases:
    """Test edge cases."""

    def test_large_heap(self):
        """Test with many elements."""
        heap = MinHeap()
        for i in range(1000, 0, -1):
            heap.push(i)

        result = []
        while heap:
            result.append(heap.pop())

        assert result == list(range(1, 1001))

    def test_duplicate_values(self):
        """Test with duplicate values."""
        heap = MinHeap.from_list([3, 1, 3, 1, 3])

        result = []
        while heap:
            result.append(heap.pop())

        assert result == [1, 1, 3, 3, 3]

    def test_negative_values(self):
        """Test with negative values."""
        heap = MinHeap.from_list([0, -5, 3, -2, 1])

        assert heap.pop() == -5
        assert heap.pop() == -2
        assert heap.pop() == 0

    def test_bool_conversion(self):
        """Test bool conversion."""
        heap = MinHeap()
        assert not heap
        heap.push(1)
        assert heap
