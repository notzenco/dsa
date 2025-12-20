"""
Skip List Implementation.

A probabilistic data structure that provides O(log n) search, insert, and delete.
An alternative to balanced trees with simpler implementation.

================================================================================
                         SKIP LIST VISUAL REPRESENTATION
================================================================================

    Level 3:  HEAD ─────────────────────────────────────────→ 25 ──→ NIL
               │                                              │
    Level 2:  HEAD ─────────→ 6 ─────────────────→ 17 ───→ 25 ──→ NIL
               │              │                    │         │
    Level 1:  HEAD ──→ 3 ──→ 6 ──→ 9 ─────→ 12 ─→ 17 ───→ 25 ──→ NIL
               │      │      │     │        │     │         │
    Level 0:  HEAD ─→ 3 ──→ 6 ──→ 7 ──→ 9 ─→ 12 → 17 → 19 → 25 → NIL

    Each node randomly decides its height (like flipping a coin)

================================================================================

COMPLEXITY:
+-------------------+-----------+---------+
| Operation         | Expected  | Worst   |
+-------------------+-----------+---------+
| Search            | O(log n)  | O(n)    |
| Insert            | O(log n)  | O(n)    |
| Delete            | O(log n)  | O(n)    |
| Space             | O(n)      | O(n lg n)|
+-------------------+-----------+---------+

ADVANTAGES OVER BALANCED TREES:
- Simpler implementation
- Easy to make lock-free for concurrency
- Good cache locality for forward traversal
- No complex rotation operations

LEETCODE PROBLEMS:
- #1206 Design Skiplist

USE CASES:
- In-memory databases (Redis sorted sets)
- Concurrent data structures
- Range queries
- Alternative to balanced BSTs
"""

from typing import TypeVar, Generic, Optional, List, Tuple, Iterator
import random

K = TypeVar('K')
V = TypeVar('V')

MAX_LEVEL = 32  # Supports up to 2^32 elements


class SkipListNode(Generic[K, V]):
    """A node in the skip list."""

    __slots__ = ['key', 'value', 'forward']

    def __init__(self, key: K, value: V, level: int) -> None:
        """Initialize node with key, value, and level."""
        self.key = key
        self.value = value
        self.forward: List[Optional['SkipListNode[K, V]']] = [None] * level


class SkipList(Generic[K, V]):
    """
    Skip List implementation.

    A probabilistic data structure that maintains elements in sorted order
    and provides expected O(log n) time for search, insert, and delete operations.

    Example:
        >>> sl = SkipList()
        >>> sl.insert(3, 'c')
        >>> sl.insert(1, 'a')
        >>> sl.insert(2, 'b')
        >>> sl.search(2)
        'b'
        >>> list(sl.keys())
        [1, 2, 3]
    """

    def __init__(self, probability: float = 0.5) -> None:
        """
        Initialize skip list.

        Args:
            probability: Probability for level increase (0 < p < 1).
                        Default 0.5 gives expected height of 2.
        """
        if not (0 < probability < 1):
            probability = 0.5

        self._probability = probability
        self._level = 1
        self._size = 0
        # Head node with max level, no actual key/value
        self._head: SkipListNode[K, V] = SkipListNode(None, None, MAX_LEVEL)  # type: ignore

    def __len__(self) -> int:
        """Return number of elements."""
        return self._size

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return self.contains(key)

    def __iter__(self) -> Iterator[K]:
        """Iterate over keys in sorted order."""
        return self.keys()

    def __repr__(self) -> str:
        """String representation."""
        return f"SkipList(size={self._size}, level={self._level})"

    @property
    def size(self) -> int:
        """Return number of elements."""
        return self._size

    def _random_level(self) -> int:
        """Generate random level for a new node."""
        level = 1
        while random.random() < self._probability and level < MAX_LEVEL:
            level += 1
        return level

    def insert(self, key: K, value: V) -> bool:
        """
        Insert or update a key-value pair.

        Args:
            key: Key to insert (must be comparable).
            value: Value to associate with key.

        Returns:
            True if new key was inserted, False if existing key was updated.

        Time: O(log n) expected
        """
        update: List[SkipListNode[K, V]] = [self._head] * MAX_LEVEL
        current = self._head

        # Find position at each level
        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key < key:
                current = current.forward[i]
            update[i] = current

        current = current.forward[0]

        # Key already exists - update value
        if current is not None and current.key == key:
            current.value = value
            return False

        # Generate random level for new node
        new_level = self._random_level()

        # Update list level if needed
        if new_level > self._level:
            for i in range(self._level, new_level):
                update[i] = self._head
            self._level = new_level

        # Create and insert new node
        new_node: SkipListNode[K, V] = SkipListNode(key, value, new_level)

        for i in range(new_level):
            new_node.forward[i] = update[i].forward[i]
            update[i].forward[i] = new_node

        self._size += 1
        return True

    def search(self, key: K) -> Optional[V]:
        """
        Search for a key.

        Args:
            key: Key to search for.

        Returns:
            Value if found, None otherwise.

        Time: O(log n) expected
        """
        current = self._head

        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key < key:
                current = current.forward[i]

        current = current.forward[0]

        if current is not None and current.key == key:
            return current.value

        return None

    def delete(self, key: K) -> bool:
        """
        Delete a key.

        Args:
            key: Key to delete.

        Returns:
            True if key was found and deleted.

        Time: O(log n) expected
        """
        update: List[SkipListNode[K, V]] = [self._head] * MAX_LEVEL
        current = self._head

        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key < key:
                current = current.forward[i]
            update[i] = current

        current = current.forward[0]

        if current is None or current.key != key:
            return False

        # Update forward pointers
        for i in range(self._level):
            if update[i].forward[i] != current:
                break
            update[i].forward[i] = current.forward[i]

        # Update list level
        while self._level > 1 and self._head.forward[self._level - 1] is None:
            self._level -= 1

        self._size -= 1
        return True

    def contains(self, key: K) -> bool:
        """
        Check if key exists.

        Args:
            key: Key to check.

        Returns:
            True if key exists.

        Time: O(log n) expected
        """
        return self.search(key) is not None

    def get(self, key: K, default: Optional[V] = None) -> Optional[V]:
        """
        Get value for key with default.

        Args:
            key: Key to look up.
            default: Default value if key not found.

        Returns:
            Value if found, default otherwise.
        """
        result = self.search(key)
        return result if result is not None else default

    # =========================================================================
    # RANGE OPERATIONS
    # =========================================================================

    def range(self, min_key: K, max_key: K) -> List[Tuple[K, V]]:
        """
        Get all key-value pairs in range [min_key, max_key].

        Args:
            min_key: Minimum key (inclusive).
            max_key: Maximum key (inclusive).

        Returns:
            List of (key, value) tuples in range.

        Time: O(log n + k) where k is the number of elements in range
        """
        if min_key > max_key:  # type: ignore
            return []

        result: List[Tuple[K, V]] = []
        current = self._head

        # Find first node >= min_key
        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key < min_key:
                current = current.forward[i]

        current = current.forward[0]

        # Collect keys in range
        while current is not None and current.key <= max_key:  # type: ignore
            result.append((current.key, current.value))
            current = current.forward[0]

        return result

    def ceiling(self, key: K) -> Optional[K]:
        """
        Get smallest key >= given key.

        Args:
            key: Key to find ceiling for.

        Returns:
            Ceiling key if found, None otherwise.

        Time: O(log n) expected
        """
        current = self._head

        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key < key:
                current = current.forward[i]

        current = current.forward[0]

        if current is not None:
            return current.key

        return None

    def floor(self, key: K) -> Optional[K]:
        """
        Get largest key <= given key.

        Args:
            key: Key to find floor for.

        Returns:
            Floor key if found, None otherwise.

        Time: O(log n) expected
        """
        current = self._head
        floor_node: Optional[SkipListNode[K, V]] = None

        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None and current.forward[i].key <= key:
                current = current.forward[i]
            if current is not self._head and current.key <= key:  # type: ignore
                floor_node = current

        if floor_node is not None:
            return floor_node.key

        return None

    # =========================================================================
    # UTILITY METHODS
    # =========================================================================

    def is_empty(self) -> bool:
        """Check if skip list is empty."""
        return self._size == 0

    def min(self) -> Optional[Tuple[K, V]]:
        """
        Get minimum key-value pair.

        Returns:
            (key, value) tuple or None if empty.

        Time: O(1)
        """
        if self._size == 0:
            return None

        node = self._head.forward[0]
        if node is not None:
            return (node.key, node.value)
        return None

    def max(self) -> Optional[Tuple[K, V]]:
        """
        Get maximum key-value pair.

        Returns:
            (key, value) tuple or None if empty.

        Time: O(log n) expected
        """
        if self._size == 0:
            return None

        current = self._head
        for i in range(self._level - 1, -1, -1):
            while current.forward[i] is not None:
                current = current.forward[i]

        if current is not self._head:
            return (current.key, current.value)
        return None

    def keys(self) -> Iterator[K]:
        """
        Iterate over keys in sorted order.

        Yields:
            Keys in ascending order.
        """
        current = self._head.forward[0]
        while current is not None:
            yield current.key
            current = current.forward[0]

    def values(self) -> Iterator[V]:
        """
        Iterate over values in key order.

        Yields:
            Values in ascending key order.
        """
        current = self._head.forward[0]
        while current is not None:
            yield current.value
            current = current.forward[0]

    def items(self) -> Iterator[Tuple[K, V]]:
        """
        Iterate over key-value pairs in sorted order.

        Yields:
            (key, value) tuples in ascending order.
        """
        current = self._head.forward[0]
        while current is not None:
            yield (current.key, current.value)
            current = current.forward[0]

    def clear(self) -> None:
        """Remove all elements."""
        for i in range(MAX_LEVEL):
            self._head.forward[i] = None
        self._level = 1
        self._size = 0

    def to_list(self) -> List[Tuple[K, V]]:
        """
        Convert to list of key-value pairs.

        Returns:
            List of (key, value) tuples in sorted order.
        """
        return list(self.items())
