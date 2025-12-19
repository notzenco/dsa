"""
Queue (FIFO - First In, First Out)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Queue Structure:                                                          ║
║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
║  │  10 │  20 │  30 │  40 │  50 │                                           ║
║  └─────┴─────┴─────┴─────┴─────┘                                           ║
║    ▲                         ▲                                             ║
║   front                    rear                                            ║
║  (dequeue)               (enqueue)                                         ║
║                                                                            ║
║  Enqueue Operation (add to rear):                                          ║
║  Before: [10, 20, 30]                                                      ║
║  enqueue(40)                                                               ║
║  After:  [10, 20, 30, 40]                                                  ║
║                                                                            ║
║  Dequeue Operation (remove from front):                                    ║
║  Before: [10, 20, 30, 40]                                                  ║
║  dequeue() → returns 10                                                    ║
║  After:  [20, 30, 40]                                                      ║
║                                                                            ║
║  Circular Queue (size=4):                                                  ║
║  ┌─────┬─────┬─────┬─────┐                                                 ║
║  │  30 │  40 │     │  20 │                                                 ║
║  └─────┴─────┴─────┴─────┘                                                 ║
║    [0]   [1]   [2]   [3]                                                   ║
║                 ▲     ▲                                                    ║
║               rear  front                                                  ║
║                                                                            ║
║  Deque (Double-Ended Queue):                                               ║
║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
║  │  10 │  20 │  30 │  40 │  50 │                                           ║
║  └─────┴─────┴─────┴─────┴─────┘                                           ║
║    ▲                         ▲                                             ║
║   front                    rear                                            ║
║  (add/remove)          (add/remove)                                        ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Time        │ Space       │ Notes       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ enqueue             │ O(1)*       │ O(1)        │ *amortized  │
│ dequeue             │ O(1)        │ O(1)        │             │
│ peek/front          │ O(1)        │ O(1)        │             │
│ is_empty            │ O(1)        │ O(1)        │             │
│ size                │ O(1)        │ O(1)        │             │
│ Deque add/remove    │ O(1)        │ O(1)        │ both ends   │
└─────────────────────┴─────────────┴─────────────┴─────────────┘

LEETCODE PROBLEMS:
- #232 Implement Queue using Stacks
- #225 Implement Stack using Queues
- #622 Design Circular Queue
- #641 Design Circular Deque
- #346 Moving Average from Data Stream
- #933 Number of Recent Calls
- #239 Sliding Window Maximum (deque)
- #862 Shortest Subarray with Sum at Least K
- #1438 Longest Continuous Subarray With Absolute Diff

USE CASES:
- Task scheduling (CPU, printer queue)
- Breadth-first search (BFS)
- Buffer for data streams
- Handling requests in order
- Message queues
- Keyboard buffer
- Cache implementation
- Level-order tree traversal
"""

from typing import TypeVar, Generic, Iterator, Optional, List
from collections import deque

T = TypeVar('T')


class Queue(Generic[T]):
    """
    A queue implementation using Python's deque for O(1) operations.

    Uses collections.deque which provides O(1) append and popleft.
    """

    def __init__(self) -> None:
        """Initialize an empty queue."""
        self._data: deque[T] = deque()

    def __len__(self) -> int:
        """Return the number of elements in the queue."""
        return len(self._data)

    def __iter__(self) -> Iterator[T]:
        """Iterate from front to rear."""
        return iter(self._data)

    def __repr__(self) -> str:
        """Return string representation."""
        return f"Queue({list(self._data)})"

    def __eq__(self, other: object) -> bool:
        """Check equality with another queue."""
        if not isinstance(other, Queue):
            return NotImplemented
        return list(self._data) == list(other._data)

    def __contains__(self, value: T) -> bool:
        """Check if value exists in queue."""
        return value in self._data

    @property
    def is_empty(self) -> bool:
        """Return True if queue is empty."""
        return len(self._data) == 0

    def enqueue(self, value: T) -> None:
        """
        Add an element to the rear of the queue.

        Time Complexity: O(1)

        Args:
            value: The value to add.
        """
        self._data.append(value)

    def dequeue(self) -> T:
        """
        Remove and return the front element.

        Time Complexity: O(1)

        Returns:
            The front element.

        Raises:
            IndexError: If the queue is empty.
        """
        if self.is_empty:
            raise IndexError("Dequeue from empty queue")
        return self._data.popleft()

    def peek(self) -> T:
        """
        Return the front element without removing it.

        Time Complexity: O(1)

        Returns:
            The front element.

        Raises:
            IndexError: If the queue is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty queue")
        return self._data[0]

    def rear(self) -> T:
        """
        Return the rear element without removing it.

        Time Complexity: O(1)

        Returns:
            The rear element.

        Raises:
            IndexError: If the queue is empty.
        """
        if self.is_empty:
            raise IndexError("Rear from empty queue")
        return self._data[-1]

    def clear(self) -> None:
        """Remove all elements from the queue."""
        self._data.clear()

    def to_list(self) -> List[T]:
        """Convert queue to list (front to rear)."""
        return list(self._data)

    @classmethod
    def from_list(cls, items: List[T]) -> "Queue[T]":
        """
        Create a queue from a list.

        First element becomes front of queue.

        Args:
            items: List of items.

        Returns:
            A new Queue.
        """
        queue: Queue[T] = cls()
        for item in items:
            queue.enqueue(item)
        return queue


class CircularQueue(Generic[T]):
    """
    A fixed-size circular queue implementation using an array.

    Efficient use of space by wrapping around when reaching the end.
    """

    def __init__(self, capacity: int) -> None:
        """
        Initialize a circular queue with fixed capacity.

        Args:
            capacity: Maximum number of elements.

        Raises:
            ValueError: If capacity is less than 1.
        """
        if capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity: int = capacity
        self._data: List[Optional[T]] = [None] * capacity
        self._front: int = 0
        self._rear: int = -1
        self._size: int = 0

    def __len__(self) -> int:
        """Return the number of elements."""
        return self._size

    def __repr__(self) -> str:
        """Return string representation."""
        elements = []
        if self._size > 0:
            idx = self._front
            for _ in range(self._size):
                elements.append(str(self._data[idx]))
                idx = (idx + 1) % self._capacity
        return f"CircularQueue([{', '.join(elements)}], capacity={self._capacity})"

    @property
    def is_empty(self) -> bool:
        """Return True if queue is empty."""
        return self._size == 0

    @property
    def is_full(self) -> bool:
        """Return True if queue is full."""
        return self._size == self._capacity

    @property
    def capacity(self) -> int:
        """Return the capacity of the queue."""
        return self._capacity

    def enqueue(self, value: T) -> bool:
        """
        Add an element to the rear.

        Time Complexity: O(1)

        Args:
            value: The value to add.

        Returns:
            True if successful, False if queue is full.
        """
        if self.is_full:
            return False

        self._rear = (self._rear + 1) % self._capacity
        self._data[self._rear] = value
        self._size += 1
        return True

    def dequeue(self) -> Optional[T]:
        """
        Remove and return the front element.

        Time Complexity: O(1)

        Returns:
            The front element, or None if empty.
        """
        if self.is_empty:
            return None

        value = self._data[self._front]
        self._data[self._front] = None
        self._front = (self._front + 1) % self._capacity
        self._size -= 1
        return value

    def peek(self) -> Optional[T]:
        """
        Return the front element without removing it.

        Time Complexity: O(1)

        Returns:
            The front element, or None if empty.
        """
        if self.is_empty:
            return None
        return self._data[self._front]

    def rear(self) -> Optional[T]:
        """
        Return the rear element without removing it.

        Time Complexity: O(1)

        Returns:
            The rear element, or None if empty.
        """
        if self.is_empty:
            return None
        return self._data[self._rear]


class Deque(Generic[T]):
    """
    A double-ended queue (deque) implementation.

    Supports O(1) insertion and deletion at both ends.
    """

    def __init__(self) -> None:
        """Initialize an empty deque."""
        self._data: deque[T] = deque()

    def __len__(self) -> int:
        """Return the number of elements."""
        return len(self._data)

    def __iter__(self) -> Iterator[T]:
        """Iterate from front to rear."""
        return iter(self._data)

    def __repr__(self) -> str:
        """Return string representation."""
        return f"Deque({list(self._data)})"

    def __getitem__(self, index: int) -> T:
        """Get element at index."""
        return self._data[index]

    @property
    def is_empty(self) -> bool:
        """Return True if deque is empty."""
        return len(self._data) == 0

    def add_front(self, value: T) -> None:
        """
        Add element to the front.

        Time Complexity: O(1)

        Args:
            value: The value to add.
        """
        self._data.appendleft(value)

    def add_rear(self, value: T) -> None:
        """
        Add element to the rear.

        Time Complexity: O(1)

        Args:
            value: The value to add.
        """
        self._data.append(value)

    def remove_front(self) -> T:
        """
        Remove and return the front element.

        Time Complexity: O(1)

        Returns:
            The front element.

        Raises:
            IndexError: If deque is empty.
        """
        if self.is_empty:
            raise IndexError("Remove from empty deque")
        return self._data.popleft()

    def remove_rear(self) -> T:
        """
        Remove and return the rear element.

        Time Complexity: O(1)

        Returns:
            The rear element.

        Raises:
            IndexError: If deque is empty.
        """
        if self.is_empty:
            raise IndexError("Remove from empty deque")
        return self._data.pop()

    def peek_front(self) -> T:
        """
        Return the front element without removing.

        Time Complexity: O(1)

        Returns:
            The front element.

        Raises:
            IndexError: If deque is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty deque")
        return self._data[0]

    def peek_rear(self) -> T:
        """
        Return the rear element without removing.

        Time Complexity: O(1)

        Returns:
            The rear element.

        Raises:
            IndexError: If deque is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty deque")
        return self._data[-1]

    def clear(self) -> None:
        """Remove all elements."""
        self._data.clear()

    def to_list(self) -> List[T]:
        """Convert to list."""
        return list(self._data)


class MonotonicQueue(Generic[T]):
    """
    A monotonic deque that maintains elements in sorted order.

    Useful for sliding window maximum/minimum problems.
    This implementation maintains a decreasing order (for maximum queries).
    """

    def __init__(self) -> None:
        """Initialize an empty monotonic queue."""
        self._data: deque[tuple[int, T]] = deque()  # (index, value)

    def __len__(self) -> int:
        """Return the number of elements."""
        return len(self._data)

    @property
    def is_empty(self) -> bool:
        """Return True if empty."""
        return len(self._data) == 0

    def push(self, index: int, value: T) -> None:
        """
        Add element maintaining decreasing order.

        Removes all elements smaller than the new value.

        Time Complexity: O(1) amortized

        Args:
            index: The index of the element.
            value: The value to add.
        """
        while self._data and self._data[-1][1] < value:  # type: ignore
            self._data.pop()
        self._data.append((index, value))

    def pop(self, index: int) -> None:
        """
        Remove element at index if it's at the front.

        Time Complexity: O(1)

        Args:
            index: The index to remove.
        """
        if self._data and self._data[0][0] == index:
            self._data.popleft()

    def get_max(self) -> Optional[T]:
        """
        Get the maximum element.

        Time Complexity: O(1)

        Returns:
            The maximum element, or None if empty.
        """
        if self.is_empty:
            return None
        return self._data[0][1]


def sliding_window_maximum(nums: List[int], k: int) -> List[int]:
    """
    Find maximum in each sliding window of size k.

    Time Complexity: O(n)
    Space Complexity: O(k)

    Args:
        nums: List of integers.
        k: Window size.

    Returns:
        List of maximums for each window position.

    Example:
        nums = [1,3,-1,-3,5,3,6,7], k = 3
        Output: [3,3,5,5,6,7]
    """
    if not nums or k <= 0:
        return []

    if k == 1:
        return nums.copy()

    result: List[int] = []
    mq = MonotonicQueue[int]()

    for i, num in enumerate(nums):
        mq.push(i, num)

        # Remove elements outside the window
        if i >= k:
            mq.pop(i - k)

        # Add maximum to result once window is formed
        if i >= k - 1:
            max_val = mq.get_max()
            if max_val is not None:
                result.append(max_val)

    return result
