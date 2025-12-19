"""
Tests for Queue implementations.
"""

import pytest
from data_structures.queue import (
    Queue, CircularQueue, Deque, MonotonicQueue,
    sliding_window_maximum
)


class TestQueueBasics:
    """Test basic queue operations."""

    def test_init(self):
        """Test initialization."""
        q = Queue()
        assert len(q) == 0
        assert q.is_empty

    def test_enqueue(self):
        """Test enqueue operation."""
        q = Queue()
        q.enqueue(10)
        q.enqueue(20)

        assert len(q) == 2
        assert not q.is_empty

    def test_dequeue(self):
        """Test dequeue operation (FIFO)."""
        q = Queue.from_list([10, 20, 30])
        assert q.dequeue() == 10
        assert q.dequeue() == 20
        assert q.dequeue() == 30

    def test_dequeue_empty_raises(self):
        """Test dequeue from empty queue."""
        q = Queue()
        with pytest.raises(IndexError):
            q.dequeue()

    def test_peek(self):
        """Test peek operation."""
        q = Queue.from_list([10, 20, 30])
        assert q.peek() == 10
        assert len(q) == 3  # Size unchanged

    def test_peek_empty_raises(self):
        """Test peek from empty queue."""
        q = Queue()
        with pytest.raises(IndexError):
            q.peek()

    def test_rear(self):
        """Test rear operation."""
        q = Queue.from_list([10, 20, 30])
        assert q.rear() == 30


class TestQueueUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing the queue."""
        q = Queue.from_list([1, 2, 3])
        q.clear()
        assert q.is_empty

    def test_to_list(self):
        """Test converting to list."""
        q = Queue.from_list([1, 2, 3])
        assert q.to_list() == [1, 2, 3]

    def test_from_list(self):
        """Test creating from list."""
        q = Queue.from_list([1, 2, 3])
        assert q.dequeue() == 1
        assert q.dequeue() == 2

    def test_iter(self):
        """Test iteration."""
        q = Queue.from_list([1, 2, 3])
        result = list(q)
        assert result == [1, 2, 3]

    def test_contains(self):
        """Test __contains__."""
        q = Queue.from_list([1, 2, 3])
        assert 2 in q
        assert 5 not in q

    def test_equality(self):
        """Test equality comparison."""
        q1 = Queue.from_list([1, 2, 3])
        q2 = Queue.from_list([1, 2, 3])
        q3 = Queue.from_list([1, 2, 4])

        assert q1 == q2
        assert q1 != q3


class TestCircularQueue:
    """Test circular queue operations."""

    def test_init(self):
        """Test initialization."""
        q = CircularQueue(5)
        assert len(q) == 0
        assert q.is_empty
        assert not q.is_full
        assert q.capacity == 5

    def test_init_invalid_capacity(self):
        """Test invalid capacity."""
        with pytest.raises(ValueError):
            CircularQueue(0)

    def test_enqueue(self):
        """Test enqueue operation."""
        q = CircularQueue(3)
        assert q.enqueue(10) is True
        assert q.enqueue(20) is True
        assert q.enqueue(30) is True
        assert q.is_full
        assert q.enqueue(40) is False  # Queue full

    def test_dequeue(self):
        """Test dequeue operation."""
        q = CircularQueue(3)
        q.enqueue(10)
        q.enqueue(20)

        assert q.dequeue() == 10
        assert q.dequeue() == 20
        assert q.dequeue() is None  # Empty

    def test_circular_behavior(self):
        """Test wrap-around behavior."""
        q = CircularQueue(3)

        # Fill queue
        q.enqueue(1)
        q.enqueue(2)
        q.enqueue(3)

        # Remove front
        assert q.dequeue() == 1

        # Add new element (should wrap)
        assert q.enqueue(4) is True

        # Verify order
        assert q.dequeue() == 2
        assert q.dequeue() == 3
        assert q.dequeue() == 4

    def test_peek_and_rear(self):
        """Test peek and rear."""
        q = CircularQueue(5)
        assert q.peek() is None
        assert q.rear() is None

        q.enqueue(10)
        q.enqueue(20)

        assert q.peek() == 10
        assert q.rear() == 20


class TestDeque:
    """Test double-ended queue operations."""

    def test_init(self):
        """Test initialization."""
        d = Deque()
        assert len(d) == 0
        assert d.is_empty

    def test_add_front(self):
        """Test adding to front."""
        d = Deque()
        d.add_front(20)
        d.add_front(10)

        assert d.to_list() == [10, 20]

    def test_add_rear(self):
        """Test adding to rear."""
        d = Deque()
        d.add_rear(10)
        d.add_rear(20)

        assert d.to_list() == [10, 20]

    def test_remove_front(self):
        """Test removing from front."""
        d = Deque()
        d.add_rear(10)
        d.add_rear(20)
        d.add_rear(30)

        assert d.remove_front() == 10
        assert d.to_list() == [20, 30]

    def test_remove_rear(self):
        """Test removing from rear."""
        d = Deque()
        d.add_rear(10)
        d.add_rear(20)
        d.add_rear(30)

        assert d.remove_rear() == 30
        assert d.to_list() == [10, 20]

    def test_remove_from_empty_raises(self):
        """Test removing from empty deque."""
        d = Deque()
        with pytest.raises(IndexError):
            d.remove_front()
        with pytest.raises(IndexError):
            d.remove_rear()

    def test_peek_front_and_rear(self):
        """Test peeking at both ends."""
        d = Deque()
        d.add_rear(10)
        d.add_rear(20)
        d.add_rear(30)

        assert d.peek_front() == 10
        assert d.peek_rear() == 30
        assert len(d) == 3  # Size unchanged

    def test_mixed_operations(self):
        """Test mixed front and rear operations."""
        d = Deque()

        d.add_rear(1)      # [1]
        d.add_rear(2)      # [1, 2]
        d.add_front(0)     # [0, 1, 2]
        d.add_rear(3)      # [0, 1, 2, 3]
        d.remove_front()   # [1, 2, 3]
        d.remove_rear()    # [1, 2]

        assert d.to_list() == [1, 2]

    def test_getitem(self):
        """Test index access."""
        d = Deque()
        d.add_rear(10)
        d.add_rear(20)
        d.add_rear(30)

        assert d[0] == 10
        assert d[1] == 20
        assert d[-1] == 30


class TestMonotonicQueue:
    """Test monotonic queue operations."""

    def test_basic(self):
        """Test basic operations."""
        mq = MonotonicQueue()
        mq.push(0, 3)
        assert mq.get_max() == 3

        mq.push(1, 1)  # 1 < 3, so 3 stays as max
        assert mq.get_max() == 3

        mq.push(2, 5)  # 5 > 3, so 5 becomes max
        assert mq.get_max() == 5

    def test_pop_front(self):
        """Test popping from front."""
        mq = MonotonicQueue()
        mq.push(0, 5)
        mq.push(1, 3)

        mq.pop(0)  # Remove element at index 0
        assert mq.get_max() == 3

    def test_empty(self):
        """Test empty queue."""
        mq = MonotonicQueue()
        assert mq.get_max() is None
        assert mq.is_empty


class TestSlidingWindowMaximum:
    """Test sliding window maximum utility."""

    def test_basic(self):
        """Test basic case."""
        nums = [1, 3, -1, -3, 5, 3, 6, 7]
        result = sliding_window_maximum(nums, 3)
        assert result == [3, 3, 5, 5, 6, 7]

    def test_window_size_one(self):
        """Test window size of 1."""
        nums = [1, 2, 3, 4, 5]
        result = sliding_window_maximum(nums, 1)
        assert result == [1, 2, 3, 4, 5]

    def test_window_equals_array(self):
        """Test window size equals array size."""
        nums = [1, 3, 2]
        result = sliding_window_maximum(nums, 3)
        assert result == [3]

    def test_decreasing_array(self):
        """Test decreasing array."""
        nums = [5, 4, 3, 2, 1]
        result = sliding_window_maximum(nums, 3)
        assert result == [5, 4, 3]

    def test_increasing_array(self):
        """Test increasing array."""
        nums = [1, 2, 3, 4, 5]
        result = sliding_window_maximum(nums, 3)
        assert result == [3, 4, 5]

    def test_empty_array(self):
        """Test empty array."""
        result = sliding_window_maximum([], 3)
        assert result == []

    def test_single_element(self):
        """Test single element array."""
        result = sliding_window_maximum([42], 1)
        assert result == [42]
