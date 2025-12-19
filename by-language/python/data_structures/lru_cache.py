"""
LRU Cache (Least Recently Used Cache) Implementation.

A cache data structure that evicts the least recently used items when full.
Combines a hash map with a doubly linked list for O(1) operations.

================================================================================
                         LRU CACHE VISUAL REPRESENTATION
================================================================================

    Structure: Hash Map + Doubly Linked List

    Hash Map:
    +-------+-------+-------+-------+
    | key_A | key_B | key_C | key_D |  --> Points to nodes in linked list
    +-------+-------+-------+-------+

    Doubly Linked List (Most Recent <--> Least Recent):

    HEAD <-> [A] <-> [B] <-> [C] <-> [D] <-> TAIL
             ^                        ^
             |                        |
          Most Recent           Least Recent
          (just used)           (evict first)

    Operations:
    - get(B):  Move B to head --> HEAD <-> [B] <-> [A] <-> [C] <-> [D] <-> TAIL
    - put(E):  Add E to head, evict D if at capacity

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
- #146 LRU Cache
- #460 LFU Cache (variation)

USE CASES:
- Web browser cache
- Database query cache
- Operating system page replacement
- Content delivery networks (CDN)
- API response caching
"""

from typing import TypeVar, Generic, Dict, Optional, Callable, Iterator, Tuple, Any
from collections import OrderedDict

K = TypeVar('K')
V = TypeVar('V')


class LRUNode(Generic[K, V]):
    """A node in the doubly linked list for LRU cache."""

    __slots__ = ['key', 'value', 'prev', 'next']

    def __init__(self, key: K, value: V) -> None:
        """Initialize node with key-value pair."""
        self.key = key
        self.value = value
        self.prev: Optional['LRUNode[K, V]'] = None
        self.next: Optional['LRUNode[K, V]'] = None


class LRUCache(Generic[K, V]):
    """
    LRU (Least Recently Used) Cache implementation.

    A cache with a fixed capacity that evicts the least recently used item
    when inserting a new item would exceed capacity.

    Implementation uses a hash map for O(1) key lookup combined with a
    doubly linked list to track access order.

    Example:
        >>> cache = LRUCache(2)
        >>> cache.put(1, 'a')
        >>> cache.put(2, 'b')
        >>> cache.get(1)
        'a'
        >>> cache.put(3, 'c')  # Evicts key 2
        >>> cache.get(2)  # Returns None (evicted)
    """

    def __init__(self, capacity: int) -> None:
        """
        Initialize LRU cache with given capacity.

        Args:
            capacity: Maximum number of items to store

        Raises:
            ValueError: If capacity < 1
        """
        if capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity = capacity
        self._cache: Dict[K, LRUNode[K, V]] = {}

        # Dummy head and tail nodes to simplify edge cases
        self._head: LRUNode[K, V] = LRUNode(None, None)  # type: ignore
        self._tail: LRUNode[K, V] = LRUNode(None, None)  # type: ignore
        self._head.next = self._tail
        self._tail.prev = self._head

    def get(self, key: K) -> Optional[V]:
        """
        Get value for key and mark as recently used.

        Args:
            key: The key to look up

        Returns:
            The value if found, None otherwise

        Time: O(1)
        """
        if key not in self._cache:
            return None

        node = self._cache[key]
        self._move_to_front(node)
        return node.value

    def put(self, key: K, value: V) -> Optional[Tuple[K, V]]:
        """
        Insert or update key-value pair.

        If key exists, updates value and marks as recently used.
        If key doesn't exist and cache is full, evicts LRU item.

        Args:
            key: The key to insert/update
            value: The value to store

        Returns:
            Tuple of (evicted_key, evicted_value) if eviction occurred, None otherwise

        Time: O(1)
        """
        evicted = None

        if key in self._cache:
            # Update existing
            node = self._cache[key]
            node.value = value
            self._move_to_front(node)
        else:
            # Insert new
            if len(self._cache) >= self._capacity:
                evicted = self._evict_lru()

            node = LRUNode(key, value)
            self._cache[key] = node
            self._add_to_front(node)

        return evicted

    def delete(self, key: K) -> Optional[V]:
        """
        Delete key from cache.

        Args:
            key: The key to delete

        Returns:
            The deleted value if found, None otherwise

        Time: O(1)
        """
        if key not in self._cache:
            return None

        node = self._cache[key]
        self._remove_node(node)
        del self._cache[key]
        return node.value

    def peek(self, key: K) -> Optional[V]:
        """
        Get value without marking as recently used.

        Args:
            key: The key to look up

        Returns:
            The value if found, None otherwise

        Time: O(1)
        """
        if key not in self._cache:
            return None
        return self._cache[key].value

    def contains(self, key: K) -> bool:
        """Check if key exists in cache."""
        return key in self._cache

    def clear(self) -> None:
        """Clear all items from cache."""
        self._cache.clear()
        self._head.next = self._tail
        self._tail.prev = self._head

    @property
    def capacity(self) -> int:
        """Return the cache capacity."""
        return self._capacity

    def resize(self, new_capacity: int) -> None:
        """
        Resize cache to new capacity.

        If new capacity is smaller, evicts LRU items as needed.

        Args:
            new_capacity: The new capacity

        Raises:
            ValueError: If new_capacity < 1
        """
        if new_capacity < 1:
            raise ValueError("Capacity must be at least 1")

        while len(self._cache) > new_capacity:
            self._evict_lru()

        self._capacity = new_capacity

    def get_lru_key(self) -> Optional[K]:
        """Get the least recently used key without evicting."""
        if not self._cache:
            return None
        return self._tail.prev.key

    def get_mru_key(self) -> Optional[K]:
        """Get the most recently used key."""
        if not self._cache:
            return None
        return self._head.next.key

    def _add_to_front(self, node: LRUNode[K, V]) -> None:
        """Add node right after head (most recently used position)."""
        node.prev = self._head
        node.next = self._head.next
        self._head.next.prev = node
        self._head.next = node

    def _remove_node(self, node: LRUNode[K, V]) -> None:
        """Remove node from its current position."""
        node.prev.next = node.next
        node.next.prev = node.prev

    def _move_to_front(self, node: LRUNode[K, V]) -> None:
        """Move existing node to front (most recently used)."""
        self._remove_node(node)
        self._add_to_front(node)

    def _evict_lru(self) -> Tuple[K, V]:
        """Evict least recently used item."""
        lru_node = self._tail.prev
        self._remove_node(lru_node)
        del self._cache[lru_node.key]
        return (lru_node.key, lru_node.value)

    def __len__(self) -> int:
        """Return number of items in cache."""
        return len(self._cache)

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return key in self._cache

    def __getitem__(self, key: K) -> V:
        """Get item, raises KeyError if not found."""
        value = self.get(key)
        if value is None and key not in self._cache:
            raise KeyError(key)
        return value  # type: ignore

    def __setitem__(self, key: K, value: V) -> None:
        """Set item."""
        self.put(key, value)

    def __delitem__(self, key: K) -> None:
        """Delete item, raises KeyError if not found."""
        if self.delete(key) is None:
            raise KeyError(key)

    def __iter__(self) -> Iterator[K]:
        """Iterate over keys from most to least recently used."""
        node = self._head.next
        while node != self._tail:
            yield node.key
            node = node.next

    def __repr__(self) -> str:
        """Return string representation."""
        return f"LRUCache(capacity={self._capacity}, size={len(self._cache)})"

    def items(self) -> Iterator[Tuple[K, V]]:
        """Iterate over (key, value) pairs from MRU to LRU."""
        node = self._head.next
        while node != self._tail:
            yield (node.key, node.value)
            node = node.next

    def keys(self) -> Iterator[K]:
        """Iterate over keys from MRU to LRU."""
        return iter(self)

    def values(self) -> Iterator[V]:
        """Iterate over values from MRU to LRU."""
        node = self._head.next
        while node != self._tail:
            yield node.value
            node = node.next


class LRUCacheSimple(Generic[K, V]):
    """
    Simplified LRU Cache using Python's OrderedDict.

    Functionally equivalent to LRUCache but uses Python's built-in
    OrderedDict for simpler implementation.

    Example:
        >>> cache = LRUCacheSimple(2)
        >>> cache.put('a', 1)
        >>> cache.put('b', 2)
        >>> cache.get('a')
        1
    """

    def __init__(self, capacity: int) -> None:
        """Initialize with given capacity."""
        if capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity = capacity
        self._cache: OrderedDict[K, V] = OrderedDict()

    def get(self, key: K) -> Optional[V]:
        """Get value and mark as recently used."""
        if key not in self._cache:
            return None

        # Move to end (most recently used)
        self._cache.move_to_end(key)
        return self._cache[key]

    def put(self, key: K, value: V) -> None:
        """Insert or update key-value pair."""
        if key in self._cache:
            self._cache.move_to_end(key)
        else:
            if len(self._cache) >= self._capacity:
                # Remove first item (least recently used)
                self._cache.popitem(last=False)

        self._cache[key] = value

    def delete(self, key: K) -> Optional[V]:
        """Delete key from cache."""
        return self._cache.pop(key, None)

    def __len__(self) -> int:
        """Return number of items."""
        return len(self._cache)

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return key in self._cache


class LFUCache(Generic[K, V]):
    """
    LFU (Least Frequently Used) Cache implementation.

    Evicts the least frequently used item. If multiple items have
    the same frequency, evicts the least recently used among them.

    LeetCode #460

    Example:
        >>> cache = LFUCache(2)
        >>> cache.put(1, 'a')
        >>> cache.put(2, 'b')
        >>> cache.get(1)  # freq[1] = 2
        'a'
        >>> cache.put(3, 'c')  # Evicts key 2 (lower frequency)
        >>> cache.get(2)
    """

    def __init__(self, capacity: int) -> None:
        """Initialize LFU cache."""
        if capacity < 1:
            raise ValueError("Capacity must be at least 1")

        self._capacity = capacity
        self._cache: Dict[K, V] = {}
        self._freq: Dict[K, int] = {}
        self._freq_to_keys: Dict[int, OrderedDict[K, None]] = {}
        self._min_freq = 0

    def get(self, key: K) -> Optional[V]:
        """Get value and increment frequency."""
        if key not in self._cache:
            return None

        self._increment_freq(key)
        return self._cache[key]

    def put(self, key: K, value: V) -> None:
        """Insert or update key-value pair."""
        if key in self._cache:
            self._cache[key] = value
            self._increment_freq(key)
            return

        # Evict if at capacity
        if len(self._cache) >= self._capacity:
            self._evict()

        # Insert new key
        self._cache[key] = value
        self._freq[key] = 1
        self._min_freq = 1

        if 1 not in self._freq_to_keys:
            self._freq_to_keys[1] = OrderedDict()
        self._freq_to_keys[1][key] = None

    def _increment_freq(self, key: K) -> None:
        """Increment frequency of key."""
        freq = self._freq[key]
        self._freq[key] = freq + 1

        # Remove from current frequency bucket
        del self._freq_to_keys[freq][key]

        # Update min_freq if this was the last key at min_freq
        if not self._freq_to_keys[freq] and freq == self._min_freq:
            self._min_freq += 1

        # Add to new frequency bucket
        new_freq = freq + 1
        if new_freq not in self._freq_to_keys:
            self._freq_to_keys[new_freq] = OrderedDict()
        self._freq_to_keys[new_freq][key] = None

    def _evict(self) -> None:
        """Evict least frequently used key."""
        # Get LFU keys and remove the LRU among them
        lfu_keys = self._freq_to_keys[self._min_freq]
        evict_key = next(iter(lfu_keys))

        del lfu_keys[evict_key]
        del self._cache[evict_key]
        del self._freq[evict_key]

    def __len__(self) -> int:
        """Return number of items."""
        return len(self._cache)

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return key in self._cache


class TTLCache(Generic[K, V]):
    """
    Cache with Time-To-Live (TTL) expiration.

    Items automatically expire after a specified duration.

    Example:
        >>> import time
        >>> cache = TTLCache(capacity=100, ttl=1.0)  # 1 second TTL
        >>> cache.put('key', 'value')
        >>> cache.get('key')
        'value'
        >>> time.sleep(1.1)
        >>> cache.get('key')  # Expired
    """

    def __init__(self, capacity: int, ttl: float) -> None:
        """
        Initialize TTL cache.

        Args:
            capacity: Maximum number of items
            ttl: Time-to-live in seconds
        """
        import time
        self._time = time.time

        if capacity < 1:
            raise ValueError("Capacity must be at least 1")
        if ttl <= 0:
            raise ValueError("TTL must be positive")

        self._capacity = capacity
        self._ttl = ttl
        self._cache: Dict[K, Tuple[V, float]] = {}
        self._lru: OrderedDict[K, None] = OrderedDict()

    def get(self, key: K) -> Optional[V]:
        """Get value if not expired."""
        if key not in self._cache:
            return None

        value, expire_time = self._cache[key]
        if self._time() > expire_time:
            # Expired
            self._remove(key)
            return None

        # Move to end (most recently used)
        self._lru.move_to_end(key)
        return value

    def put(self, key: K, value: V, ttl: Optional[float] = None) -> None:
        """
        Insert or update with optional custom TTL.

        Args:
            key: The key
            value: The value
            ttl: Optional custom TTL for this item
        """
        # Clean expired entries
        self._cleanup()

        if key in self._cache:
            self._lru.move_to_end(key)
        else:
            if len(self._cache) >= self._capacity:
                # Evict oldest
                oldest_key = next(iter(self._lru))
                self._remove(oldest_key)

            self._lru[key] = None

        expire_time = self._time() + (ttl if ttl else self._ttl)
        self._cache[key] = (value, expire_time)

    def _remove(self, key: K) -> None:
        """Remove key from cache."""
        if key in self._cache:
            del self._cache[key]
        if key in self._lru:
            del self._lru[key]

    def _cleanup(self) -> None:
        """Remove expired entries."""
        current_time = self._time()
        expired = [k for k, (_, exp) in self._cache.items() if current_time > exp]
        for k in expired:
            self._remove(k)

    def __len__(self) -> int:
        """Return number of non-expired items."""
        self._cleanup()
        return len(self._cache)

    def __contains__(self, key: K) -> bool:
        """Check if key exists and is not expired."""
        return self.get(key) is not None
