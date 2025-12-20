"""
LFU Cache (Least Frequently Used Cache) Implementation.

A cache data structure that evicts the least frequently used items when full.
Ties are broken by LRU (least recently used among same frequency).

================================================================================
                         LFU CACHE VISUAL REPRESENTATION
================================================================================

    Structure: Hash Map + Frequency Map (each frequency -> Doubly Linked List)

    Key Map:
    +-------+-------+-------+-------+
    | key_A | key_B | key_C | key_D |  --> Points to nodes in frequency lists
    +-------+-------+-------+-------+

    Frequency Map (freq -> DLL of nodes with that frequency):

    freq=1: HEAD <-> [D] <-> TAIL              (D was just added)
    freq=2: HEAD <-> [C] <-> [B] <-> TAIL      (C more recent than B at freq 2)
    freq=3: HEAD <-> [A] <-> TAIL              (A accessed 3 times)

    min_freq = 1 (tracks minimum frequency for O(1) eviction)

    On eviction: Remove from tail of min_freq list (least recently used
                 among least frequently used)

================================================================================

COMPLEXITY:
+-----------+------+-------+
| Operation | Time | Space |
+-----------+------+-------+
| Get       | O(1) | O(1)  |
| Put       | O(1) | O(1)  |
| Delete    | O(1) | O(1)  |
+-----------+------+-------+
Space: O(capacity) for the cache

LEETCODE PROBLEMS:
- #460 LFU Cache

USE CASES:
- CPU cache replacement
- Database buffer pool
- Content caching with popularity tracking
- CDN caching
"""

from typing import TypeVar, Generic, Dict, Optional, Tuple
from collections import defaultdict

K = TypeVar('K')
V = TypeVar('V')


class LFUNode(Generic[K, V]):
    """A node in the doubly linked list for LFU cache."""

    __slots__ = ['key', 'value', 'freq', 'prev', 'next']

    def __init__(self, key: K, value: V, freq: int = 1) -> None:
        """Initialize node with key-value pair and frequency."""
        self.key = key
        self.value = value
        self.freq = freq
        self.prev: Optional['LFUNode[K, V]'] = None
        self.next: Optional['LFUNode[K, V]'] = None


class FrequencyList(Generic[K, V]):
    """Doubly linked list for nodes with the same frequency."""

    def __init__(self) -> None:
        """Initialize empty frequency list with dummy head/tail."""
        self.head: LFUNode[K, V] = LFUNode(None, None, 0)  # type: ignore
        self.tail: LFUNode[K, V] = LFUNode(None, None, 0)  # type: ignore
        self.head.next = self.tail
        self.tail.prev = self.head
        self.size = 0

    def add_front(self, node: LFUNode[K, V]) -> None:
        """Add node to front of list (most recently used)."""
        node.prev = self.head
        node.next = self.head.next
        self.head.next.prev = node  # type: ignore
        self.head.next = node
        self.size += 1

    def remove(self, node: LFUNode[K, V]) -> None:
        """Remove node from list."""
        if node.prev:
            node.prev.next = node.next
        if node.next:
            node.next.prev = node.prev
        node.prev = None
        node.next = None
        self.size -= 1

    def remove_tail(self) -> Optional[LFUNode[K, V]]:
        """Remove and return least recently used node (from tail)."""
        if self.size == 0:
            return None
        node = self.tail.prev
        self.remove(node)  # type: ignore
        return node

    def is_empty(self) -> bool:
        """Check if list is empty."""
        return self.size == 0


class LFUCache(Generic[K, V]):
    """
    LFU (Least Frequently Used) Cache implementation.

    A cache with a fixed capacity that evicts the least frequently used item
    when inserting a new item would exceed capacity. Ties are broken by LRU
    (evict the least recently used among items with the same frequency).

    Implementation uses:
    - Hash map for O(1) key lookup
    - Hash map of frequency -> doubly linked list for O(1) frequency tracking
    - min_freq tracking for O(1) eviction

    Example:
        >>> cache = LFUCache(2)
        >>> cache.put(1, 'a')
        >>> cache.put(2, 'b')
        >>> cache.get(1)  # freq(1) = 2
        'a'
        >>> cache.put(3, 'c')  # Evicts key 2 (freq=1, LRU)
        >>> cache.get(2)  # Returns None (evicted)
    """

    def __init__(self, capacity: int) -> None:
        """
        Initialize LFU cache with given capacity.

        Args:
            capacity: Maximum number of items to store

        Raises:
            ValueError: If capacity < 1
        """
        if capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity = capacity
        self._size = 0
        self._min_freq = 0
        self._key_map: Dict[K, LFUNode[K, V]] = {}
        self._freq_map: Dict[int, FrequencyList[K, V]] = defaultdict(FrequencyList)

    def __len__(self) -> int:
        """Return number of items in cache."""
        return self._size

    def __contains__(self, key: K) -> bool:
        """Check if key exists in cache (doesn't update frequency)."""
        return key in self._key_map

    def __repr__(self) -> str:
        """String representation."""
        return f"LFUCache(capacity={self._capacity}, size={self._size})"

    @property
    def capacity(self) -> int:
        """Return cache capacity."""
        return self._capacity

    @property
    def size(self) -> int:
        """Return current number of items."""
        return self._size

    def get(self, key: K) -> Optional[V]:
        """
        Get value for key and increment frequency.

        Args:
            key: The key to look up

        Returns:
            The value if found, None otherwise

        Time: O(1)
        """
        if key not in self._key_map:
            return None

        node = self._key_map[key]
        self._update_frequency(node)
        return node.value

    def put(self, key: K, value: V) -> Optional[Tuple[K, V]]:
        """
        Insert or update key-value pair.

        If key exists, updates value and increments frequency.
        If key doesn't exist and cache is full, evicts LFU item.

        Args:
            key: The key to insert/update
            value: The value to store

        Returns:
            Tuple of (evicted_key, evicted_value) if eviction occurred, None otherwise

        Time: O(1)
        """
        evicted = None

        if key in self._key_map:
            # Update existing key
            node = self._key_map[key]
            node.value = value
            self._update_frequency(node)
            return None

        # Evict if at capacity
        if self._size >= self._capacity:
            evicted = self._evict()

        # Create new node with frequency 1
        node = LFUNode(key, value, 1)
        self._key_map[key] = node
        self._freq_map[1].add_front(node)
        self._min_freq = 1
        self._size += 1

        return evicted

    def delete(self, key: K) -> bool:
        """
        Delete a key from the cache.

        Args:
            key: The key to delete

        Returns:
            True if key was found and deleted

        Time: O(1)
        """
        if key not in self._key_map:
            return False

        node = self._key_map[key]
        freq_list = self._freq_map[node.freq]
        freq_list.remove(node)

        del self._key_map[key]
        self._size -= 1

        return True

    def peek(self, key: K) -> Optional[V]:
        """
        Get value without updating frequency.

        Args:
            key: The key to look up

        Returns:
            The value if found, None otherwise

        Time: O(1)
        """
        if key not in self._key_map:
            return None
        return self._key_map[key].value

    def get_frequency(self, key: K) -> int:
        """
        Get the access frequency of a key.

        Args:
            key: The key to check

        Returns:
            Frequency count, or -1 if key not found

        Time: O(1)
        """
        if key not in self._key_map:
            return -1
        return self._key_map[key].freq

    def min_frequency(self) -> int:
        """
        Get the minimum frequency in the cache.

        Returns:
            Minimum frequency, or 0 if cache is empty

        Time: O(1)
        """
        if self._size == 0:
            return 0
        return self._min_freq

    def is_empty(self) -> bool:
        """Check if cache is empty."""
        return self._size == 0

    def is_full(self) -> bool:
        """Check if cache is at capacity."""
        return self._size >= self._capacity

    def clear(self) -> None:
        """Remove all items from cache."""
        self._key_map.clear()
        self._freq_map.clear()
        self._size = 0
        self._min_freq = 0

    def _update_frequency(self, node: LFUNode[K, V]) -> None:
        """Move node to next frequency list."""
        old_freq = node.freq
        new_freq = old_freq + 1

        # Remove from old frequency list
        old_list = self._freq_map[old_freq]
        old_list.remove(node)

        # Update min_freq if needed
        if old_list.is_empty() and self._min_freq == old_freq:
            self._min_freq = new_freq

        # Add to new frequency list
        node.freq = new_freq
        self._freq_map[new_freq].add_front(node)

    def _evict(self) -> Optional[Tuple[K, V]]:
        """Evict the LFU item (LRU among same frequency)."""
        if self._size == 0:
            return None

        min_list = self._freq_map[self._min_freq]
        node = min_list.remove_tail()

        if node is None:
            return None

        evicted_key = node.key
        evicted_value = node.value
        del self._key_map[evicted_key]
        self._size -= 1

        return (evicted_key, evicted_value)
