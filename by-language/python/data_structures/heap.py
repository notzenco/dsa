"""
Binary Heap (Min-Heap and Max-Heap)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Min-Heap Property: Parent <= Children                                     ║
║  Max-Heap Property: Parent >= Children                                     ║
║                                                                            ║
║  Min-Heap Example:                                                         ║
║                        ┌─────┐                                             ║
║                        │  1  │  ← minimum at root                          ║
║                        └─────┘                                             ║
║                       /       \                                            ║
║                 ┌─────┐       ┌─────┐                                      ║
║                 │  3  │       │  2  │                                      ║
║                 └─────┘       └─────┘                                      ║
║                /     \        /     \                                      ║
║           ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐                                  ║
║           │  5  │ │  4  │ │  8  │ │  7  │                                  ║
║           └─────┘ └─────┘ └─────┘ └─────┘                                  ║
║                                                                            ║
║  Array Representation:                                                     ║
║  Index:   0    1    2    3    4    5    6                                  ║
║  Value: [ 1 ][ 3 ][ 2 ][ 5 ][ 4 ][ 8 ][ 7 ]                                ║
║                                                                            ║
║  Index Relationships (0-indexed):                                          ║
║  Parent(i)     = (i - 1) // 2                                              ║
║  Left Child(i)  = 2 * i + 1                                                ║
║  Right Child(i) = 2 * i + 2                                                ║
║                                                                            ║
║  Insert (Push):                                                            ║
║  1. Add element at end of array                                            ║
║  2. "Bubble up" - swap with parent while heap property violated            ║
║                                                                            ║
║  Before: [1, 3, 2, 5, 4]        After insert(0): [0, 1, 2, 5, 4, 3]        ║
║       1                              0                                     ║
║      / \                            / \                                    ║
║     3   2        →                 1   2                                   ║
║    / \                            / \   \                                  ║
║   5   4                          5   4   3                                 ║
║                                                                            ║
║  Extract Min/Max (Pop):                                                    ║
║  1. Replace root with last element                                         ║
║  2. "Bubble down" - swap with smaller/larger child while violated          ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Time        │ Space       │ Notes       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Insert (push)       │ O(log n)    │ O(1)        │             │
│ Extract min/max     │ O(log n)    │ O(1)        │             │
│ Peek min/max        │ O(1)        │ O(1)        │             │
│ Heapify (build)     │ O(n)        │ O(1)        │ Floyd's     │
│ Delete arbitrary    │ O(log n)    │ O(1)        │ with index  │
│ Decrease/Increase   │ O(log n)    │ O(1)        │ with index  │
│ Merge two heaps     │ O(n)        │ O(n)        │             │
└─────────────────────┴─────────────┴─────────────┴─────────────┘

LEETCODE PROBLEMS:
- #215 Kth Largest Element in an Array
- #347 Top K Frequent Elements
- #373 Find K Pairs with Smallest Sums
- #378 Kth Smallest Element in a Sorted Matrix
- #502 IPO
- #621 Task Scheduler
- #692 Top K Frequent Words
- #703 Kth Largest Element in a Stream
- #767 Reorganize String
- #973 K Closest Points to Origin
- #1046 Last Stone Weight
- #1337 The K Weakest Rows in a Matrix
- #23 Merge k Sorted Lists

USE CASES:
- Priority queues
- Dijkstra's algorithm
- Heap sort
- Median finding (two heaps)
- Task scheduling
- Event-driven simulation
- Huffman coding
- K-way merge
"""

from typing import TypeVar, Generic, Iterator, Optional, List, Callable, Tuple, Any

T = TypeVar('T')


class MinHeap(Generic[T]):
    """
    A min-heap implementation using an array.

    The smallest element is always at the root (index 0).
    """

    def __init__(self) -> None:
        """Initialize an empty min-heap."""
        self._data: List[T] = []

    def __len__(self) -> int:
        """Return number of elements."""
        return len(self._data)

    def __bool__(self) -> bool:
        """Return True if heap is not empty."""
        return len(self._data) > 0

    def __iter__(self) -> Iterator[T]:
        """Iterate over elements (not in sorted order)."""
        return iter(self._data)

    def __repr__(self) -> str:
        """String representation."""
        return f"MinHeap({self._data})"

    def __contains__(self, value: T) -> bool:
        """Check if value exists in heap."""
        return value in self._data

    @property
    def is_empty(self) -> bool:
        """Return True if heap is empty."""
        return len(self._data) == 0

    def _parent(self, i: int) -> int:
        """Get parent index."""
        return (i - 1) // 2

    def _left_child(self, i: int) -> int:
        """Get left child index."""
        return 2 * i + 1

    def _right_child(self, i: int) -> int:
        """Get right child index."""
        return 2 * i + 2

    def _has_left_child(self, i: int) -> bool:
        """Check if node has left child."""
        return self._left_child(i) < len(self._data)

    def _has_right_child(self, i: int) -> bool:
        """Check if node has right child."""
        return self._right_child(i) < len(self._data)

    def _swap(self, i: int, j: int) -> None:
        """Swap elements at indices i and j."""
        self._data[i], self._data[j] = self._data[j], self._data[i]

    def _bubble_up(self, index: int) -> None:
        """Move element up to restore heap property."""
        while index > 0:
            parent = self._parent(index)
            if self._data[index] < self._data[parent]:
                self._swap(index, parent)
                index = parent
            else:
                break

    def _bubble_down(self, index: int) -> None:
        """Move element down to restore heap property."""
        while self._has_left_child(index):
            smallest = index
            left = self._left_child(index)
            right = self._right_child(index)

            if self._data[left] < self._data[smallest]:
                smallest = left

            if self._has_right_child(index) and self._data[right] < self._data[smallest]:
                smallest = right

            if smallest != index:
                self._swap(index, smallest)
                index = smallest
            else:
                break

    def push(self, value: T) -> None:
        """
        Insert a value into the heap.

        Time Complexity: O(log n)

        Args:
            value: Value to insert.
        """
        self._data.append(value)
        self._bubble_up(len(self._data) - 1)

    def pop(self) -> T:
        """
        Remove and return the minimum element.

        Time Complexity: O(log n)

        Returns:
            The minimum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Pop from empty heap")

        min_val = self._data[0]

        # Move last element to root and bubble down
        last = self._data.pop()
        if self._data:
            self._data[0] = last
            self._bubble_down(0)

        return min_val

    def peek(self) -> T:
        """
        Return the minimum element without removing it.

        Time Complexity: O(1)

        Returns:
            The minimum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty heap")
        return self._data[0]

    def pushpop(self, value: T) -> T:
        """
        Push value and pop minimum in one operation (more efficient).

        Time Complexity: O(log n)

        Args:
            value: Value to push.

        Returns:
            The minimum element (may be the pushed value).
        """
        if self.is_empty or value <= self._data[0]:
            return value

        min_val = self._data[0]
        self._data[0] = value
        self._bubble_down(0)
        return min_val

    def replace(self, value: T) -> T:
        """
        Pop minimum and push value in one operation.

        Time Complexity: O(log n)

        Args:
            value: Value to push after popping.

        Returns:
            The previous minimum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Replace on empty heap")

        min_val = self._data[0]
        self._data[0] = value
        self._bubble_down(0)
        return min_val

    def clear(self) -> None:
        """Remove all elements."""
        self._data.clear()

    def to_sorted_list(self) -> List[T]:
        """
        Return elements in sorted order.

        Time Complexity: O(n log n)

        Note: Does not modify the heap.
        """
        # Create a copy and extract all elements
        temp = MinHeap()
        temp._data = self._data.copy()

        result: List[T] = []
        while temp:
            result.append(temp.pop())
        return result

    @classmethod
    def heapify(cls, items: List[T]) -> "MinHeap[T]":
        """
        Create a heap from a list in O(n) time.

        Uses Floyd's heap construction algorithm.

        Args:
            items: List of items.

        Returns:
            A new MinHeap containing all items.
        """
        heap: MinHeap[T] = cls()
        heap._data = items.copy()

        # Start from last non-leaf node and bubble down
        for i in range(len(heap._data) // 2 - 1, -1, -1):
            heap._bubble_down(i)

        return heap

    @classmethod
    def from_list(cls, items: List[T]) -> "MinHeap[T]":
        """Alias for heapify."""
        return cls.heapify(items)


class MaxHeap(Generic[T]):
    """
    A max-heap implementation using an array.

    The largest element is always at the root (index 0).
    """

    def __init__(self) -> None:
        """Initialize an empty max-heap."""
        self._data: List[T] = []

    def __len__(self) -> int:
        """Return number of elements."""
        return len(self._data)

    def __bool__(self) -> bool:
        """Return True if heap is not empty."""
        return len(self._data) > 0

    def __iter__(self) -> Iterator[T]:
        """Iterate over elements (not in sorted order)."""
        return iter(self._data)

    def __repr__(self) -> str:
        """String representation."""
        return f"MaxHeap({self._data})"

    def __contains__(self, value: T) -> bool:
        """Check if value exists in heap."""
        return value in self._data

    @property
    def is_empty(self) -> bool:
        """Return True if heap is empty."""
        return len(self._data) == 0

    def _parent(self, i: int) -> int:
        """Get parent index."""
        return (i - 1) // 2

    def _left_child(self, i: int) -> int:
        """Get left child index."""
        return 2 * i + 1

    def _right_child(self, i: int) -> int:
        """Get right child index."""
        return 2 * i + 2

    def _has_left_child(self, i: int) -> bool:
        """Check if node has left child."""
        return self._left_child(i) < len(self._data)

    def _has_right_child(self, i: int) -> bool:
        """Check if node has right child."""
        return self._right_child(i) < len(self._data)

    def _swap(self, i: int, j: int) -> None:
        """Swap elements at indices i and j."""
        self._data[i], self._data[j] = self._data[j], self._data[i]

    def _bubble_up(self, index: int) -> None:
        """Move element up to restore heap property."""
        while index > 0:
            parent = self._parent(index)
            if self._data[index] > self._data[parent]:
                self._swap(index, parent)
                index = parent
            else:
                break

    def _bubble_down(self, index: int) -> None:
        """Move element down to restore heap property."""
        while self._has_left_child(index):
            largest = index
            left = self._left_child(index)
            right = self._right_child(index)

            if self._data[left] > self._data[largest]:
                largest = left

            if self._has_right_child(index) and self._data[right] > self._data[largest]:
                largest = right

            if largest != index:
                self._swap(index, largest)
                index = largest
            else:
                break

    def push(self, value: T) -> None:
        """
        Insert a value into the heap.

        Time Complexity: O(log n)

        Args:
            value: Value to insert.
        """
        self._data.append(value)
        self._bubble_up(len(self._data) - 1)

    def pop(self) -> T:
        """
        Remove and return the maximum element.

        Time Complexity: O(log n)

        Returns:
            The maximum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Pop from empty heap")

        max_val = self._data[0]

        last = self._data.pop()
        if self._data:
            self._data[0] = last
            self._bubble_down(0)

        return max_val

    def peek(self) -> T:
        """
        Return the maximum element without removing it.

        Time Complexity: O(1)

        Returns:
            The maximum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty heap")
        return self._data[0]

    def pushpop(self, value: T) -> T:
        """
        Push value and pop maximum in one operation.

        Time Complexity: O(log n)

        Args:
            value: Value to push.

        Returns:
            The maximum element (may be the pushed value).
        """
        if self.is_empty or value >= self._data[0]:
            return value

        max_val = self._data[0]
        self._data[0] = value
        self._bubble_down(0)
        return max_val

    def replace(self, value: T) -> T:
        """
        Pop maximum and push value in one operation.

        Time Complexity: O(log n)

        Args:
            value: Value to push after popping.

        Returns:
            The previous maximum element.

        Raises:
            IndexError: If heap is empty.
        """
        if self.is_empty:
            raise IndexError("Replace on empty heap")

        max_val = self._data[0]
        self._data[0] = value
        self._bubble_down(0)
        return max_val

    def clear(self) -> None:
        """Remove all elements."""
        self._data.clear()

    def to_sorted_list(self) -> List[T]:
        """
        Return elements in descending sorted order.

        Time Complexity: O(n log n)
        """
        temp = MaxHeap()
        temp._data = self._data.copy()

        result: List[T] = []
        while temp:
            result.append(temp.pop())
        return result

    @classmethod
    def heapify(cls, items: List[T]) -> "MaxHeap[T]":
        """
        Create a heap from a list in O(n) time.

        Args:
            items: List of items.

        Returns:
            A new MaxHeap containing all items.
        """
        heap: MaxHeap[T] = cls()
        heap._data = items.copy()

        for i in range(len(heap._data) // 2 - 1, -1, -1):
            heap._bubble_down(i)

        return heap

    @classmethod
    def from_list(cls, items: List[T]) -> "MaxHeap[T]":
        """Alias for heapify."""
        return cls.heapify(items)


class PriorityQueue(Generic[T]):
    """
    A priority queue implementation using a min-heap.

    Elements with lower priority values are dequeued first.
    Supports custom priority functions.
    """

    def __init__(self, key: Optional[Callable[[T], Any]] = None) -> None:
        """
        Initialize priority queue.

        Args:
            key: Optional function to extract priority from elements.
                 If None, elements themselves are used as priorities.
        """
        self._heap: List[Tuple[Any, int, T]] = []
        self._key = key if key else lambda x: x
        self._counter = 0  # For stable ordering of equal priorities

    def __len__(self) -> int:
        """Return number of elements."""
        return len(self._heap)

    def __bool__(self) -> bool:
        """Return True if queue is not empty."""
        return len(self._heap) > 0

    def __repr__(self) -> str:
        """String representation."""
        items = [item for _, _, item in self._heap]
        return f"PriorityQueue({items})"

    @property
    def is_empty(self) -> bool:
        """Return True if queue is empty."""
        return len(self._heap) == 0

    def _parent(self, i: int) -> int:
        return (i - 1) // 2

    def _left_child(self, i: int) -> int:
        return 2 * i + 1

    def _right_child(self, i: int) -> int:
        return 2 * i + 2

    def _swap(self, i: int, j: int) -> None:
        self._heap[i], self._heap[j] = self._heap[j], self._heap[i]

    def _bubble_up(self, index: int) -> None:
        while index > 0:
            parent = self._parent(index)
            if self._heap[index] < self._heap[parent]:
                self._swap(index, parent)
                index = parent
            else:
                break

    def _bubble_down(self, index: int) -> None:
        size = len(self._heap)
        while self._left_child(index) < size:
            smallest = index
            left = self._left_child(index)
            right = self._right_child(index)

            if self._heap[left] < self._heap[smallest]:
                smallest = left

            if right < size and self._heap[right] < self._heap[smallest]:
                smallest = right

            if smallest != index:
                self._swap(index, smallest)
                index = smallest
            else:
                break

    def enqueue(self, item: T) -> None:
        """
        Add an item to the queue.

        Time Complexity: O(log n)

        Args:
            item: Item to add.
        """
        priority = self._key(item)
        entry = (priority, self._counter, item)
        self._counter += 1
        self._heap.append(entry)
        self._bubble_up(len(self._heap) - 1)

    def dequeue(self) -> T:
        """
        Remove and return the highest priority item.

        Time Complexity: O(log n)

        Returns:
            The item with lowest priority value.

        Raises:
            IndexError: If queue is empty.
        """
        if self.is_empty:
            raise IndexError("Dequeue from empty queue")

        item = self._heap[0][2]

        last = self._heap.pop()
        if self._heap:
            self._heap[0] = last
            self._bubble_down(0)

        return item

    def peek(self) -> T:
        """
        Return highest priority item without removing.

        Time Complexity: O(1)

        Returns:
            The item with lowest priority value.

        Raises:
            IndexError: If queue is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty queue")
        return self._heap[0][2]

    def clear(self) -> None:
        """Remove all items."""
        self._heap.clear()
        self._counter = 0


def heap_sort(arr: List[T]) -> List[T]:
    """
    Sort a list using heap sort.

    Time Complexity: O(n log n)
    Space Complexity: O(n)

    Args:
        arr: List to sort.

    Returns:
        New sorted list in ascending order.
    """
    heap = MinHeap.heapify(arr)
    return heap.to_sorted_list()


def nlargest(n: int, items: List[T]) -> List[T]:
    """
    Find the n largest elements.

    Time Complexity: O(n log k) where k = n

    Args:
        n: Number of largest elements to find.
        items: List of items.

    Returns:
        List of n largest elements in descending order.
    """
    if n <= 0:
        return []

    if n >= len(items):
        return sorted(items, reverse=True)

    # Use min-heap of size n
    heap = MinHeap[T]()

    for item in items:
        if len(heap) < n:
            heap.push(item)
        elif item > heap.peek():
            heap.replace(item)

    return heap.to_sorted_list()[::-1]


def nsmallest(n: int, items: List[T]) -> List[T]:
    """
    Find the n smallest elements.

    Time Complexity: O(n log k) where k = n

    Args:
        n: Number of smallest elements to find.
        items: List of items.

    Returns:
        List of n smallest elements in ascending order.
    """
    if n <= 0:
        return []

    if n >= len(items):
        return sorted(items)

    # Use max-heap of size n
    heap = MaxHeap[T]()

    for item in items:
        if len(heap) < n:
            heap.push(item)
        elif item < heap.peek():
            heap.replace(item)

    return heap.to_sorted_list()[::-1]
