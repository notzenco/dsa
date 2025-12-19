"""
Doubly Linked List

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Structure:                                                                ║
║         ┌────────────────┐    ┌────────────────┐    ┌────────────────┐     ║
║  None ◀─│prev│data│next │◀──▶│prev│data│next │◀──▶│prev│data│next │──▶None║
║         └────────────────┘    └────────────────┘    └────────────────┘     ║
║              ▲                                            ▲                ║
║            head                                         tail               ║
║                                                                            ║
║  Node Structure:                                                           ║
║  ┌───────────────────────────────┐                                         ║
║  │  ┌──────┬───────┬──────┐     │                                         ║
║  │  │ prev │ data  │ next │     │                                         ║
║  │  └──┬───┴───────┴───┬──┘     │                                         ║
║  │     │               │        │                                         ║
║  │     ▼               ▼        │                                         ║
║  │  previous        next        │                                         ║
║  │   node           node        │                                         ║
║  └───────────────────────────────┘                                         ║
║                                                                            ║
║  Insert After Node B:                                                      ║
║  Before: [A] ◀──▶ [B] ◀──▶ [C]                                             ║
║  After:  [A] ◀──▶ [B] ◀──▶ [X] ◀──▶ [C]                                    ║
║                                                                            ║
║  Delete Node B:                                                            ║
║  Before: [A] ◀──▶ [B] ◀──▶ [C]                                             ║
║  After:  [A] ◀──────────▶ [C]                                              ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Average     │ Worst       │ Space       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Access by index     │ O(n)        │ O(n)        │ O(1)        │
│ Search              │ O(n)        │ O(n)        │ O(1)        │
│ Insert at head      │ O(1)        │ O(1)        │ O(1)        │
│ Insert at tail      │ O(1)        │ O(1)        │ O(1)        │
│ Insert at index     │ O(n)        │ O(n)        │ O(1)        │
│ Delete at head      │ O(1)        │ O(1)        │ O(1)        │
│ Delete at tail      │ O(1)        │ O(1)        │ O(1)        │
│ Delete at index     │ O(n)        │ O(n)        │ O(1)        │
│ Delete given node   │ O(1)        │ O(1)        │ O(1)        │
└─────────────────────┴─────────────┴─────────────┴─────────────┘

LEETCODE PROBLEMS:
- #146 LRU Cache (doubly linked list + hash map)
- #460 LFU Cache
- #432 All O(1) Data Structure
- #706 Design HashMap
- #707 Design Linked List

USE CASES:
- LRU/LFU Cache implementation
- Browser forward/back navigation
- Undo/Redo with bidirectional traversal
- Music player (next/previous)
- Text editors (cursor movement)
- Deque implementation
"""

from typing import TypeVar, Generic, Iterator, Optional, List, Callable

T = TypeVar('T')


class DNode(Generic[T]):
    """A node in a doubly linked list."""

    __slots__ = ('data', 'prev', 'next')

    def __init__(
        self,
        data: T,
        prev_node: Optional["DNode[T]"] = None,
        next_node: Optional["DNode[T]"] = None
    ) -> None:
        self.data: T = data
        self.prev: Optional["DNode[T]"] = prev_node
        self.next: Optional["DNode[T]"] = next_node

    def __repr__(self) -> str:
        return f"DNode({self.data})"


class DoublyLinkedList(Generic[T]):
    """
    A doubly linked list implementation with head and tail pointers.

    Supports O(1) operations at both ends and O(1) deletion given a node.
    """

    def __init__(self) -> None:
        """Initialize an empty doubly linked list."""
        self._head: Optional[DNode[T]] = None
        self._tail: Optional[DNode[T]] = None
        self._size: int = 0

    def __len__(self) -> int:
        """Return the number of elements."""
        return self._size

    def __iter__(self) -> Iterator[T]:
        """Iterate forward through the list."""
        current = self._head
        while current is not None:
            yield current.data
            current = current.next

    def __reversed__(self) -> Iterator[T]:
        """Iterate backward through the list."""
        current = self._tail
        while current is not None:
            yield current.data
            current = current.prev

    def __repr__(self) -> str:
        """Return string representation."""
        elements = " <-> ".join(str(x) for x in self)
        return f"DoublyLinkedList([{elements}])"

    def __getitem__(self, index: int) -> T:
        """Get element at index."""
        node = self._get_node(index)
        return node.data

    def __setitem__(self, index: int, value: T) -> None:
        """Set element at index."""
        node = self._get_node(index)
        node.data = value

    def __contains__(self, value: T) -> bool:
        """Check if value exists."""
        return self.find(value) != -1

    def __eq__(self, other: object) -> bool:
        """Check equality."""
        if not isinstance(other, DoublyLinkedList):
            return NotImplemented

        if self._size != other._size:
            return False

        c1, c2 = self._head, other._head
        while c1 is not None:
            if c1.data != c2.data:  # type: ignore
                return False
            c1 = c1.next
            c2 = c2.next  # type: ignore

        return True

    @property
    def is_empty(self) -> bool:
        """Return True if list is empty."""
        return self._size == 0

    @property
    def head(self) -> Optional[T]:
        """Return first element."""
        return self._head.data if self._head else None

    @property
    def tail(self) -> Optional[T]:
        """Return last element."""
        return self._tail.data if self._tail else None

    def _get_node(self, index: int) -> DNode[T]:
        """
        Get node at index with optimization for traversal direction.

        Uses forward traversal for first half, backward for second half.
        """
        if index < 0:
            index = self._size + index

        if index < 0 or index >= self._size:
            raise IndexError("Index out of bounds")

        # Optimize: traverse from closer end
        if index <= self._size // 2:
            current = self._head
            for _ in range(index):
                current = current.next  # type: ignore
        else:
            current = self._tail
            for _ in range(self._size - 1 - index):
                current = current.prev  # type: ignore

        return current  # type: ignore

    def prepend(self, value: T) -> DNode[T]:
        """
        Add element to the beginning.

        Time Complexity: O(1)

        Args:
            value: Value to add.

        Returns:
            The created node.
        """
        new_node = DNode(value, next_node=self._head)

        if self._head is not None:
            self._head.prev = new_node

        self._head = new_node

        if self._tail is None:
            self._tail = new_node

        self._size += 1
        return new_node

    def append(self, value: T) -> DNode[T]:
        """
        Add element to the end.

        Time Complexity: O(1)

        Args:
            value: Value to add.

        Returns:
            The created node.
        """
        new_node = DNode(value, prev_node=self._tail)

        if self._tail is not None:
            self._tail.next = new_node

        self._tail = new_node

        if self._head is None:
            self._head = new_node

        self._size += 1
        return new_node

    def insert(self, index: int, value: T) -> None:
        """
        Insert element at index.

        Time Complexity: O(n)

        Args:
            index: Position to insert at.
            value: Value to insert.

        Raises:
            IndexError: If index is out of bounds.
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
            next_node = self._get_node(index)
            prev_node = next_node.prev
            new_node = DNode(value, prev_node, next_node)
            prev_node.next = new_node  # type: ignore
            next_node.prev = new_node
            self._size += 1

    def insert_after(self, node: DNode[T], value: T) -> DNode[T]:
        """
        Insert value after a given node.

        Time Complexity: O(1)

        Args:
            node: The node after which to insert.
            value: Value to insert.

        Returns:
            The created node.
        """
        new_node = DNode(value, prev_node=node, next_node=node.next)

        if node.next is not None:
            node.next.prev = new_node
        else:
            self._tail = new_node

        node.next = new_node
        self._size += 1
        return new_node

    def insert_before(self, node: DNode[T], value: T) -> DNode[T]:
        """
        Insert value before a given node.

        Time Complexity: O(1)

        Args:
            node: The node before which to insert.
            value: Value to insert.

        Returns:
            The created node.
        """
        new_node = DNode(value, prev_node=node.prev, next_node=node)

        if node.prev is not None:
            node.prev.next = new_node
        else:
            self._head = new_node

        node.prev = new_node
        self._size += 1
        return new_node

    def pop_first(self) -> T:
        """
        Remove and return first element.

        Time Complexity: O(1)

        Returns:
            The first element.

        Raises:
            IndexError: If list is empty.
        """
        if self._head is None:
            raise IndexError("Pop from empty list")

        value = self._head.data
        self._head = self._head.next

        if self._head is not None:
            self._head.prev = None
        else:
            self._tail = None

        self._size -= 1
        return value

    def pop_last(self) -> T:
        """
        Remove and return last element.

        Time Complexity: O(1)

        Returns:
            The last element.

        Raises:
            IndexError: If list is empty.
        """
        if self._tail is None:
            raise IndexError("Pop from empty list")

        value = self._tail.data
        self._tail = self._tail.prev

        if self._tail is not None:
            self._tail.next = None
        else:
            self._head = None

        self._size -= 1
        return value

    def pop(self, index: int = -1) -> T:
        """
        Remove and return element at index.

        Time Complexity: O(n)

        Args:
            index: Index to remove. Defaults to -1 (last).

        Returns:
            The removed element.
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

        node = self._get_node(index)
        return self.remove_node(node)

    def remove_node(self, node: DNode[T]) -> T:
        """
        Remove a specific node from the list.

        Time Complexity: O(1)

        Args:
            node: The node to remove.

        Returns:
            The data from the removed node.
        """
        if node.prev is not None:
            node.prev.next = node.next
        else:
            self._head = node.next

        if node.next is not None:
            node.next.prev = node.prev
        else:
            self._tail = node.prev

        self._size -= 1
        return node.data

    def remove(self, value: T) -> bool:
        """
        Remove first occurrence of value.

        Time Complexity: O(n)

        Args:
            value: Value to remove.

        Returns:
            True if found and removed.
        """
        current = self._head

        while current is not None:
            if current.data == value:
                self.remove_node(current)
                return True
            current = current.next

        return False

    def find(self, value: T) -> int:
        """
        Find index of first occurrence.

        Time Complexity: O(n)

        Args:
            value: Value to find.

        Returns:
            Index or -1 if not found.
        """
        current = self._head
        index = 0

        while current is not None:
            if current.data == value:
                return index
            current = current.next
            index += 1

        return -1

    def find_node(self, value: T) -> Optional[DNode[T]]:
        """
        Find node containing value.

        Time Complexity: O(n)

        Args:
            value: Value to find.

        Returns:
            The node or None if not found.
        """
        current = self._head

        while current is not None:
            if current.data == value:
                return current
            current = current.next

        return None

    def clear(self) -> None:
        """Remove all elements."""
        self._head = None
        self._tail = None
        self._size = 0

    def reverse(self) -> None:
        """
        Reverse the list in place.

        Time Complexity: O(n)
        Space Complexity: O(1)
        """
        current = self._head

        while current is not None:
            current.prev, current.next = current.next, current.prev
            current = current.prev  # Move to original next

        self._head, self._tail = self._tail, self._head

    def to_list(self) -> List[T]:
        """Convert to Python list."""
        return list(self)

    def to_list_reversed(self) -> List[T]:
        """Convert to reversed Python list."""
        return list(reversed(self))

    @classmethod
    def from_list(cls, items: List[T]) -> "DoublyLinkedList[T]":
        """Create from Python list."""
        dll: DoublyLinkedList[T] = cls()
        for item in items:
            dll.append(item)
        return dll

    def move_to_front(self, node: DNode[T]) -> None:
        """
        Move a node to the front (useful for LRU cache).

        Time Complexity: O(1)

        Args:
            node: Node to move to front.
        """
        if node == self._head:
            return

        # Remove from current position
        if node.prev is not None:
            node.prev.next = node.next

        if node.next is not None:
            node.next.prev = node.prev
        else:
            self._tail = node.prev

        # Insert at front
        node.prev = None
        node.next = self._head

        if self._head is not None:
            self._head.prev = node

        self._head = node

    def move_to_back(self, node: DNode[T]) -> None:
        """
        Move a node to the back.

        Time Complexity: O(1)

        Args:
            node: Node to move to back.
        """
        if node == self._tail:
            return

        # Remove from current position
        if node.next is not None:
            node.next.prev = node.prev

        if node.prev is not None:
            node.prev.next = node.next
        else:
            self._head = node.next

        # Insert at back
        node.next = None
        node.prev = self._tail

        if self._tail is not None:
            self._tail.next = node

        self._tail = node
