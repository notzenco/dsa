"""
Singly Linked List

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Structure:                                                                ║
║  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐                  ║
║  │ data │ next  │───▶│ data │ next  │───▶│ data │ next  │───▶ None        ║
║  └──────────────┘    └──────────────┘    └──────────────┘                  ║
║       ▲                                        ▲                           ║
║      head                                     tail                         ║
║                                                                            ║
║  Node Structure:                                                           ║
║  ┌─────────────────────┐                                                   ║
║  │  ┌───────┬───────┐  │                                                   ║
║  │  │ data  │ next ─┼──┼──▶ (pointer to next node)                         ║
║  │  └───────┴───────┘  │                                                   ║
║  └─────────────────────┘                                                   ║
║                                                                            ║
║  Insert at Head:                                                           ║
║  Before: [A] ──▶ [B] ──▶ [C] ──▶ None                                      ║
║          ▲                                                                 ║
║         head                                                               ║
║                                                                            ║
║  After:  [X] ──▶ [A] ──▶ [B] ──▶ [C] ──▶ None                              ║
║          ▲                                                                 ║
║       new head                                                             ║
║                                                                            ║
║  Delete at Position 1:                                                     ║
║  Before: [A] ──▶ [B] ──▶ [C] ──▶ None                                      ║
║  After:  [A] ──────────▶ [C] ──▶ None                                      ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Average     │ Worst       │ Space       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Access by index     │ O(n)        │ O(n)        │ O(1)        │
│ Search              │ O(n)        │ O(n)        │ O(1)        │
│ Insert at head      │ O(1)        │ O(1)        │ O(1)        │
│ Insert at tail      │ O(1)*       │ O(1)*       │ O(1)        │
│ Insert at index     │ O(n)        │ O(n)        │ O(1)        │
│ Delete at head      │ O(1)        │ O(1)        │ O(1)        │
│ Delete at tail      │ O(n)        │ O(n)        │ O(1)        │
│ Delete at index     │ O(n)        │ O(n)        │ O(1)        │
└─────────────────────┴─────────────┴─────────────┴─────────────┘
* O(1) if tail pointer is maintained (as in this implementation)

LEETCODE PROBLEMS:
- #206 Reverse Linked List
- #21 Merge Two Sorted Lists
- #141 Linked List Cycle
- #142 Linked List Cycle II
- #160 Intersection of Two Linked Lists
- #234 Palindrome Linked List
- #237 Delete Node in a Linked List
- #876 Middle of the Linked List
- #203 Remove Linked List Elements
- #83 Remove Duplicates from Sorted List

USE CASES:
- Implementing stacks and queues
- Undo functionality in applications
- Browser back button (history)
- Memory allocation (free lists)
- Polynomial arithmetic
- Music playlists (next song)
"""

from typing import TypeVar, Generic, Iterator, Optional, List, Callable

T = TypeVar('T')


class Node(Generic[T]):
    """A node in a singly linked list."""

    __slots__ = ('data', 'next')

    def __init__(self, data: T, next_node: Optional["Node[T]"] = None) -> None:
        self.data: T = data
        self.next: Optional["Node[T]"] = next_node

    def __repr__(self) -> str:
        return f"Node({self.data})"


class SinglyLinkedList(Generic[T]):
    """
    A singly linked list implementation with head and tail pointers.

    This implementation maintains both head and tail pointers to allow
    O(1) insertion at both ends.
    """

    def __init__(self) -> None:
        """Initialize an empty linked list."""
        self._head: Optional[Node[T]] = None
        self._tail: Optional[Node[T]] = None
        self._size: int = 0

    def __len__(self) -> int:
        """Return the number of elements in the list."""
        return self._size

    def __iter__(self) -> Iterator[T]:
        """Iterate over the elements in the list."""
        current = self._head
        while current is not None:
            yield current.data
            current = current.next

    def __repr__(self) -> str:
        """Return a string representation of the list."""
        elements = " -> ".join(str(x) for x in self)
        return f"SinglyLinkedList([{elements}])"

    def __getitem__(self, index: int) -> T:
        """
        Get element at the specified index.

        Time Complexity: O(n)

        Args:
            index: The index of the element to retrieve.

        Returns:
            The element at the specified index.

        Raises:
            IndexError: If the index is out of bounds.
        """
        node = self._get_node(index)
        return node.data

    def __contains__(self, value: T) -> bool:
        """Check if value exists in the list."""
        return self.find(value) != -1

    def __eq__(self, other: object) -> bool:
        """Check equality with another SinglyLinkedList."""
        if not isinstance(other, SinglyLinkedList):
            return NotImplemented

        if self._size != other._size:
            return False

        current1, current2 = self._head, other._head
        while current1 is not None:
            if current1.data != current2.data:  # type: ignore
                return False
            current1 = current1.next
            current2 = current2.next  # type: ignore

        return True

    @property
    def is_empty(self) -> bool:
        """Return True if the list is empty."""
        return self._size == 0

    @property
    def head(self) -> Optional[T]:
        """Return the first element, or None if empty."""
        return self._head.data if self._head else None

    @property
    def tail(self) -> Optional[T]:
        """Return the last element, or None if empty."""
        return self._tail.data if self._tail else None

    def _get_node(self, index: int) -> Node[T]:
        """
        Get the node at the specified index.

        Args:
            index: The index of the node.

        Returns:
            The node at the specified index.

        Raises:
            IndexError: If the index is out of bounds.
        """
        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        current = self._head
        for _ in range(index):
            current = current.next  # type: ignore

        return current  # type: ignore

    def prepend(self, value: T) -> None:
        """
        Add an element to the beginning of the list.

        Time Complexity: O(1)

        Args:
            value: The value to add.
        """
        new_node = Node(value, self._head)
        self._head = new_node

        if self._tail is None:
            self._tail = new_node

        self._size += 1

    def append(self, value: T) -> None:
        """
        Add an element to the end of the list.

        Time Complexity: O(1)

        Args:
            value: The value to add.
        """
        new_node = Node(value)

        if self._tail is None:
            self._head = new_node
            self._tail = new_node
        else:
            self._tail.next = new_node
            self._tail = new_node

        self._size += 1

    def insert(self, index: int, value: T) -> None:
        """
        Insert an element at the specified index.

        Time Complexity: O(n)

        Args:
            index: The index at which to insert.
            value: The value to insert.

        Raises:
            IndexError: If the index is out of bounds.
        """
        if index < 0:
            index = self._size + index + 1

        if index < 0 or index > self._size:
            raise IndexError("Index out of bounds")

        if index == 0:
            self.prepend(value)
        elif index == self._size:
            self.append(value)
        else:
            prev_node = self._get_node(index - 1)
            new_node = Node(value, prev_node.next)
            prev_node.next = new_node
            self._size += 1

    def pop_first(self) -> T:
        """
        Remove and return the first element.

        Time Complexity: O(1)

        Returns:
            The first element.

        Raises:
            IndexError: If the list is empty.
        """
        if self._head is None:
            raise IndexError("Pop from empty list")

        value = self._head.data
        self._head = self._head.next

        if self._head is None:
            self._tail = None

        self._size -= 1
        return value

    def pop_last(self) -> T:
        """
        Remove and return the last element.

        Time Complexity: O(n) - must traverse to find second-to-last node

        Returns:
            The last element.

        Raises:
            IndexError: If the list is empty.
        """
        if self._tail is None:
            raise IndexError("Pop from empty list")

        if self._head == self._tail:
            value = self._head.data
            self._head = None
            self._tail = None
            self._size -= 1
            return value

        # Find second-to-last node
        current = self._head
        while current.next != self._tail:  # type: ignore
            current = current.next  # type: ignore

        value = self._tail.data
        current.next = None  # type: ignore
        self._tail = current
        self._size -= 1
        return value

    def pop(self, index: int = -1) -> T:
        """
        Remove and return the element at the specified index.

        Time Complexity: O(n)

        Args:
            index: The index of the element to remove. Defaults to -1 (last).

        Returns:
            The removed element.

        Raises:
            IndexError: If the list is empty or index is out of bounds.
        """
        if self._size == 0:
            raise IndexError("Pop from empty list")

        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        if index == 0:
            return self.pop_first()

        if index == self._size - 1:
            return self.pop_last()

        prev_node = self._get_node(index - 1)
        target_node = prev_node.next
        prev_node.next = target_node.next  # type: ignore
        self._size -= 1
        return target_node.data  # type: ignore

    def remove(self, value: T) -> bool:
        """
        Remove the first occurrence of a value.

        Time Complexity: O(n)

        Args:
            value: The value to remove.

        Returns:
            True if the value was found and removed, False otherwise.
        """
        if self._head is None:
            return False

        if self._head.data == value:
            self.pop_first()
            return True

        current = self._head
        while current.next is not None:
            if current.next.data == value:
                if current.next == self._tail:
                    self._tail = current
                current.next = current.next.next
                self._size -= 1
                return True
            current = current.next

        return False

    def find(self, value: T) -> int:
        """
        Find the index of the first occurrence of a value.

        Time Complexity: O(n)

        Args:
            value: The value to search for.

        Returns:
            The index of the value, or -1 if not found.
        """
        current = self._head
        index = 0

        while current is not None:
            if current.data == value:
                return index
            current = current.next
            index += 1

        return -1

    def clear(self) -> None:
        """
        Remove all elements from the list.

        Time Complexity: O(1)
        """
        self._head = None
        self._tail = None
        self._size = 0

    def reverse(self) -> None:
        """
        Reverse the list in place.

        Time Complexity: O(n)
        Space Complexity: O(1)
        """
        if self._size <= 1:
            return

        self._tail = self._head
        prev: Optional[Node[T]] = None
        current = self._head

        while current is not None:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node

        self._head = prev

    def to_list(self) -> List[T]:
        """Convert the linked list to a Python list."""
        return list(self)

    @classmethod
    def from_list(cls, items: List[T]) -> "SinglyLinkedList[T]":
        """
        Create a SinglyLinkedList from a Python list.

        Args:
            items: The list of items to add.

        Returns:
            A new SinglyLinkedList containing all items.
        """
        linked_list: SinglyLinkedList[T] = cls()
        for item in items:
            linked_list.append(item)
        return linked_list

    def get_middle(self) -> Optional[T]:
        """
        Find the middle element using slow/fast pointer technique.

        Time Complexity: O(n)
        Space Complexity: O(1)

        Returns:
            The middle element, or None if list is empty.
        """
        if self._head is None:
            return None

        slow = self._head
        fast = self._head

        while fast.next is not None and fast.next.next is not None:
            slow = slow.next  # type: ignore
            fast = fast.next.next

        return slow.data  # type: ignore

    def has_cycle(self) -> bool:
        """
        Detect if the list has a cycle using Floyd's algorithm.

        Time Complexity: O(n)
        Space Complexity: O(1)

        Returns:
            True if the list has a cycle, False otherwise.
        """
        if self._head is None:
            return False

        slow = self._head
        fast = self._head

        while fast is not None and fast.next is not None:
            slow = slow.next  # type: ignore
            fast = fast.next.next

            if slow == fast:
                return True

        return False

    def get_nth_from_end(self, n: int) -> Optional[T]:
        """
        Get the nth element from the end (1-indexed).

        Time Complexity: O(n)
        Space Complexity: O(1)

        Args:
            n: Position from end (1 = last element).

        Returns:
            The nth element from end, or None if n is invalid.
        """
        if n <= 0 or n > self._size:
            return None

        slow = self._head
        fast = self._head

        # Move fast n nodes ahead
        for _ in range(n):
            if fast is None:
                return None
            fast = fast.next

        # Move both until fast reaches end
        while fast is not None:
            slow = slow.next  # type: ignore
            fast = fast.next

        return slow.data if slow else None  # type: ignore

    def remove_duplicates(self) -> None:
        """
        Remove duplicate values from the list.

        Time Complexity: O(n)
        Space Complexity: O(n) for the set

        Note: Preserves first occurrence of each value.
        """
        if self._head is None:
            return

        seen: set = set()
        current = self._head
        seen.add(current.data)

        while current.next is not None:
            if current.next.data in seen:
                if current.next == self._tail:
                    self._tail = current
                current.next = current.next.next
                self._size -= 1
            else:
                seen.add(current.next.data)
                current = current.next

    def map(self, func: Callable[[T], T]) -> "SinglyLinkedList[T]":
        """
        Apply a function to each element and return a new list.

        Args:
            func: Function to apply to each element.

        Returns:
            A new SinglyLinkedList with transformed elements.
        """
        result: SinglyLinkedList[T] = SinglyLinkedList()
        for item in self:
            result.append(func(item))
        return result

    def filter(self, predicate: Callable[[T], bool]) -> "SinglyLinkedList[T]":
        """
        Filter elements based on a predicate.

        Args:
            predicate: Function that returns True for elements to keep.

        Returns:
            A new SinglyLinkedList with filtered elements.
        """
        result: SinglyLinkedList[T] = SinglyLinkedList()
        for item in self:
            if predicate(item):
                result.append(item)
        return result
