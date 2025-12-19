"""
Dynamic Array (Resizable Array / ArrayList)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Logical View (size=5):                                                    ║
║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
║  │  10 │  20 │  30 │  40 │  50 │                                           ║
║  └─────┴─────┴─────┴─────┴─────┘                                           ║
║    [0]   [1]   [2]   [3]   [4]                                             ║
║                                                                            ║
║  Physical View (capacity=8):                                               ║
║  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐                         ║
║  │  10 │  20 │  30 │  40 │  50 │  -  │  -  │  -  │                         ║
║  └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘                         ║
║    [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]                           ║
║                                  ↑                                         ║
║                            unused capacity                                 ║
║                                                                            ║
║  Resize Operation (when size == capacity):                                 ║
║  Before: [1][2][3][4] → capacity: 4, size: 4                               ║
║  After:  [1][2][3][4][-][-][-][-] → capacity: 8, size: 4                   ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Average     │ Worst       │ Space       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Access by index     │ O(1)        │ O(1)        │ O(1)        │
│ Search              │ O(n)        │ O(n)        │ O(1)        │
│ Insert at end       │ O(1)*       │ O(n)        │ O(1)        │
│ Insert at index     │ O(n)        │ O(n)        │ O(1)        │
│ Delete at end       │ O(1)        │ O(1)        │ O(1)        │
│ Delete at index     │ O(n)        │ O(n)        │ O(1)        │
└─────────────────────┴─────────────┴─────────────┴─────────────┘
* Amortized O(1) due to occasional resizing

LEETCODE PROBLEMS:
- #1 Two Sum (array traversal)
- #26 Remove Duplicates from Sorted Array
- #27 Remove Element
- #88 Merge Sorted Array
- #283 Move Zeroes
- #448 Find All Numbers Disappeared in an Array

USE CASES:
- When you need fast random access by index
- When most operations are at the end of the list
- Implementing stacks, queues, and other data structures
- Storing collections where the size changes dynamically
"""

from typing import TypeVar, Generic, Iterator, Optional, List

T = TypeVar('T')


class DynamicArray(Generic[T]):
    """
    A resizable array implementation that automatically grows and shrinks.

    This implementation uses a growth factor of 2x when expanding and
    shrinks when the array is less than 25% full to optimize memory usage.
    """

    DEFAULT_CAPACITY = 8
    GROWTH_FACTOR = 2
    SHRINK_THRESHOLD = 0.25

    def __init__(self, initial_capacity: int = DEFAULT_CAPACITY) -> None:
        """
        Initialize a new dynamic array.

        Args:
            initial_capacity: Starting capacity of the internal array.
                              Must be positive.

        Raises:
            ValueError: If initial_capacity is less than 1.
        """
        if initial_capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity: int = initial_capacity
        self._size: int = 0
        self._data: List[Optional[T]] = [None] * self._capacity

    def __len__(self) -> int:
        """Return the number of elements in the array."""
        return self._size

    def __getitem__(self, index: int) -> T:
        """
        Get element at the specified index.

        Args:
            index: The index of the element to retrieve.
                   Supports negative indexing.

        Returns:
            The element at the specified index.

        Raises:
            IndexError: If the index is out of bounds.
        """
        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        return self._data[index]  # type: ignore

    def __setitem__(self, index: int, value: T) -> None:
        """
        Set element at the specified index.

        Args:
            index: The index of the element to set.
                   Supports negative indexing.
            value: The value to set.

        Raises:
            IndexError: If the index is out of bounds.
        """
        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        self._data[index] = value

    def __iter__(self) -> Iterator[T]:
        """Return an iterator over the array elements."""
        for i in range(self._size):
            yield self._data[i]  # type: ignore

    def __repr__(self) -> str:
        """Return a string representation of the array."""
        elements = [str(self._data[i]) for i in range(self._size)]
        return f"DynamicArray([{', '.join(elements)}])"

    def __eq__(self, other: object) -> bool:
        """Check equality with another DynamicArray."""
        if not isinstance(other, DynamicArray):
            return NotImplemented

        if self._size != other._size:
            return False

        for i in range(self._size):
            if self._data[i] != other._data[i]:
                return False

        return True

    def __contains__(self, value: T) -> bool:
        """Check if value exists in the array."""
        return self.find(value) != -1

    @property
    def capacity(self) -> int:
        """Return the current capacity of the internal array."""
        return self._capacity

    @property
    def is_empty(self) -> bool:
        """Return True if the array contains no elements."""
        return self._size == 0

    def _resize(self, new_capacity: int) -> None:
        """
        Resize the internal array to the new capacity.

        Args:
            new_capacity: The new capacity for the internal array.
        """
        new_data: List[Optional[T]] = [None] * new_capacity

        for i in range(self._size):
            new_data[i] = self._data[i]

        self._data = new_data
        self._capacity = new_capacity

    def append(self, value: T) -> None:
        """
        Add an element to the end of the array.

        Time Complexity: O(1) amortized, O(n) worst case when resizing.

        Args:
            value: The value to append.
        """
        if self._size == self._capacity:
            self._resize(self._capacity * self.GROWTH_FACTOR)

        self._data[self._size] = value
        self._size += 1

    def insert(self, index: int, value: T) -> None:
        """
        Insert an element at the specified index.

        Time Complexity: O(n) due to shifting elements.

        Args:
            index: The index at which to insert the element.
            value: The value to insert.

        Raises:
            IndexError: If the index is out of bounds.
        """
        if index < 0:
            index = self._size + index + 1

        if index < 0 or index > self._size:
            raise IndexError("Index out of bounds")

        if self._size == self._capacity:
            self._resize(self._capacity * self.GROWTH_FACTOR)

        # Shift elements to the right
        for i in range(self._size, index, -1):
            self._data[i] = self._data[i - 1]

        self._data[index] = value
        self._size += 1

    def pop(self, index: int = -1) -> T:
        """
        Remove and return the element at the specified index.

        Time Complexity: O(1) for last element, O(n) for other indices.

        Args:
            index: The index of the element to remove. Defaults to -1 (last).

        Returns:
            The removed element.

        Raises:
            IndexError: If the array is empty or index is out of bounds.
        """
        if self._size == 0:
            raise IndexError("Pop from empty array")

        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        value = self._data[index]

        # Shift elements to the left
        for i in range(index, self._size - 1):
            self._data[i] = self._data[i + 1]

        self._data[self._size - 1] = None
        self._size -= 1

        # Shrink if necessary
        if self._size > 0 and self._size <= self._capacity * self.SHRINK_THRESHOLD:
            new_capacity = max(self.DEFAULT_CAPACITY, self._capacity // self.GROWTH_FACTOR)
            if new_capacity < self._capacity:
                self._resize(new_capacity)

        return value  # type: ignore

    def remove(self, value: T) -> bool:
        """
        Remove the first occurrence of a value from the array.

        Time Complexity: O(n) for searching and shifting.

        Args:
            value: The value to remove.

        Returns:
            True if the value was found and removed, False otherwise.
        """
        index = self.find(value)

        if index == -1:
            return False

        self.pop(index)
        return True

    def find(self, value: T) -> int:
        """
        Find the index of the first occurrence of a value.

        Time Complexity: O(n)

        Args:
            value: The value to search for.

        Returns:
            The index of the value, or -1 if not found.
        """
        for i in range(self._size):
            if self._data[i] == value:
                return i
        return -1

    def clear(self) -> None:
        """
        Remove all elements from the array.

        Time Complexity: O(1)
        """
        self._data = [None] * self.DEFAULT_CAPACITY
        self._capacity = self.DEFAULT_CAPACITY
        self._size = 0

    def reverse(self) -> None:
        """
        Reverse the array in place.

        Time Complexity: O(n)
        Space Complexity: O(1)
        """
        left, right = 0, self._size - 1

        while left < right:
            self._data[left], self._data[right] = self._data[right], self._data[left]
            left += 1
            right -= 1

    def to_list(self) -> List[T]:
        """
        Convert the dynamic array to a Python list.

        Returns:
            A new list containing all elements.
        """
        return [self._data[i] for i in range(self._size)]  # type: ignore

    @classmethod
    def from_list(cls, items: List[T]) -> "DynamicArray[T]":
        """
        Create a DynamicArray from a Python list.

        Args:
            items: The list of items to add.

        Returns:
            A new DynamicArray containing all items.
        """
        capacity = max(cls.DEFAULT_CAPACITY, len(items))
        arr: DynamicArray[T] = cls(capacity)

        for item in items:
            arr.append(item)

        return arr
