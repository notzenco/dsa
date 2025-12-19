"""
Hash Table (Hash Map)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Chaining (Separate Chaining):                                             ║
║  ┌───────┬─────────────────────────────────────┐                           ║
║  │ Index │ Bucket (Linked List)                │                           ║
║  ├───────┼─────────────────────────────────────┤                           ║
║  │   0   │ → [("apple", 5)] → [("grape", 8)]   │                           ║
║  │   1   │ → [("banana", 3)]                   │                           ║
║  │   2   │ → None                              │                           ║
║  │   3   │ → [("cherry", 7)] → [("date", 2)]   │                           ║
║  │   4   │ → [("elderberry", 1)]               │                           ║
║  └───────┴─────────────────────────────────────┘                           ║
║                                                                            ║
║  Hash Function: hash(key) % capacity = index                               ║
║  Example: hash("apple") % 5 = 0                                            ║
║                                                                            ║
║  Open Addressing (Linear Probing):                                         ║
║  ┌───────┬─────────────────────┐                                           ║
║  │ Index │ (Key, Value)        │                                           ║
║  ├───────┼─────────────────────┤                                           ║
║  │   0   │ ("apple", 5)        │  ← hash("apple") = 0                      ║
║  │   1   │ ("grape", 8)        │  ← collision! probe to 1                  ║
║  │   2   │ None                │                                           ║
║  │   3   │ ("cherry", 7)       │                                           ║
║  │   4   │ DELETED             │  ← tombstone for lazy deletion            ║
║  └───────┴─────────────────────┘                                           ║
║                                                                            ║
║  Load Factor = n / capacity                                                ║
║  Resize when load factor > threshold (typically 0.75)                      ║
║                                                                            ║
║  Collision Resolution Strategies:                                          ║
║  1. Chaining: Store collisions in a linked list                            ║
║  2. Linear Probing: Check next slot (index + 1) % capacity                 ║
║  3. Quadratic Probing: Check (index + i²) % capacity                       ║
║  4. Double Hashing: Use second hash function for step size                 ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Average     │ Worst       │ Space       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Insert              │ O(1)        │ O(n)        │ O(n)        │
│ Delete              │ O(1)        │ O(n)        │ O(1)        │
│ Search              │ O(1)        │ O(n)        │ O(1)        │
│ Resize              │ O(n)        │ O(n)        │ O(n)        │
└─────────────────────┴─────────────┴─────────────┴─────────────┘
* Worst case occurs with many collisions (poor hash function or adversarial input)

LEETCODE PROBLEMS:
- #1 Two Sum
- #49 Group Anagrams
- #128 Longest Consecutive Sequence
- #146 LRU Cache
- #242 Valid Anagram
- #347 Top K Frequent Elements
- #380 Insert Delete GetRandom O(1)
- #383 Ransom Note
- #387 First Unique Character in a String
- #438 Find All Anagrams in a String
- #560 Subarray Sum Equals K
- #706 Design HashMap

USE CASES:
- Caching (memoization)
- Database indexing
- Symbol tables in compilers
- Counting frequencies
- Detecting duplicates
- Implementing sets
- Object property access in languages
"""

from typing import TypeVar, Generic, Iterator, Optional, List, Tuple, Callable
from dataclasses import dataclass

K = TypeVar('K')
V = TypeVar('V')


@dataclass
class HashEntry(Generic[K, V]):
    """An entry in the hash table."""
    key: K
    value: V

    def __repr__(self) -> str:
        return f"({self.key}: {self.value})"


class HashTableChaining(Generic[K, V]):
    """
    Hash table implementation using separate chaining.

    Collisions are handled by storing entries in a list at each bucket.
    """

    DEFAULT_CAPACITY = 16
    LOAD_FACTOR_THRESHOLD = 0.75

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:
        """
        Initialize hash table with given capacity.

        Args:
            capacity: Initial number of buckets.
        """
        self._capacity = max(1, capacity)
        self._size = 0
        self._buckets: List[List[HashEntry[K, V]]] = [[] for _ in range(self._capacity)]

    def __len__(self) -> int:
        """Return number of entries."""
        return self._size

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        index = self._hash(key)
        bucket = self._buckets[index]
        for entry in bucket:
            if entry.key == key:
                return True
        return False

    def __getitem__(self, key: K) -> V:
        """Get value by key."""
        index = self._hash(key)
        bucket = self._buckets[index]
        for entry in bucket:
            if entry.key == key:
                return entry.value
        raise KeyError(key)

    def __setitem__(self, key: K, value: V) -> None:
        """Set value for key."""
        self.put(key, value)

    def __delitem__(self, key: K) -> None:
        """Delete key."""
        if not self.remove(key):
            raise KeyError(key)

    def __iter__(self) -> Iterator[K]:
        """Iterate over keys."""
        for bucket in self._buckets:
            for entry in bucket:
                yield entry.key

    def __repr__(self) -> str:
        """String representation."""
        pairs = [f"{k}: {v}" for k, v in self.items()]
        return "{" + ", ".join(pairs) + "}"

    @property
    def capacity(self) -> int:
        """Return current capacity."""
        return self._capacity

    @property
    def load_factor(self) -> float:
        """Return current load factor."""
        return self._size / self._capacity

    @property
    def is_empty(self) -> bool:
        """Return True if empty."""
        return self._size == 0

    def _hash(self, key: K) -> int:
        """Compute bucket index for key."""
        return hash(key) % self._capacity

    def _resize(self, new_capacity: int) -> None:
        """Resize the hash table."""
        old_buckets = self._buckets
        self._capacity = new_capacity
        self._buckets = [[] for _ in range(new_capacity)]
        self._size = 0

        for bucket in old_buckets:
            for entry in bucket:
                self.put(entry.key, entry.value)

    def put(self, key: K, value: V) -> None:
        """
        Insert or update a key-value pair.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key.
            value: The value.
        """
        # Check if resize needed
        if self.load_factor >= self.LOAD_FACTOR_THRESHOLD:
            self._resize(self._capacity * 2)

        index = self._hash(key)
        bucket = self._buckets[index]

        # Check if key exists (update)
        for entry in bucket:
            if entry.key == key:
                entry.value = value
                return

        # Insert new entry
        bucket.append(HashEntry(key, value))
        self._size += 1

    def get(self, key: K, default: Optional[V] = None) -> Optional[V]:
        """
        Get value for key.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key to look up.
            default: Value to return if key not found.

        Returns:
            The value or default.
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        for entry in bucket:
            if entry.key == key:
                return entry.value

        return default

    def remove(self, key: K) -> bool:
        """
        Remove a key-value pair.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key to remove.

        Returns:
            True if key was found and removed.
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        for i, entry in enumerate(bucket):
            if entry.key == key:
                bucket.pop(i)
                self._size -= 1
                return True

        return False

    def clear(self) -> None:
        """Remove all entries."""
        self._buckets = [[] for _ in range(self._capacity)]
        self._size = 0

    def keys(self) -> Iterator[K]:
        """Iterate over keys."""
        return iter(self)

    def values(self) -> Iterator[V]:
        """Iterate over values."""
        for bucket in self._buckets:
            for entry in bucket:
                yield entry.value

    def items(self) -> Iterator[Tuple[K, V]]:
        """Iterate over key-value pairs."""
        for bucket in self._buckets:
            for entry in bucket:
                yield (entry.key, entry.value)


class HashTableOpenAddressing(Generic[K, V]):
    """
    Hash table implementation using open addressing with linear probing.

    Collisions are resolved by probing for the next available slot.
    Uses tombstones for lazy deletion.
    """

    DEFAULT_CAPACITY = 16
    LOAD_FACTOR_THRESHOLD = 0.5  # Lower threshold for open addressing

    # Sentinel values
    _EMPTY = object()
    _DELETED = object()

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:
        """
        Initialize hash table.

        Args:
            capacity: Initial capacity.
        """
        self._capacity = max(1, capacity)
        self._size = 0
        self._keys: List = [self._EMPTY] * self._capacity
        self._values: List = [self._EMPTY] * self._capacity

    def __len__(self) -> int:
        """Return number of entries."""
        return self._size

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        index = self._find_slot(key)
        if index is None:
            return False
        current_key = self._keys[index]
        return current_key is not self._EMPTY and current_key is not self._DELETED and current_key == key

    def __getitem__(self, key: K) -> V:
        """Get value by key."""
        index = self._find_slot(key)
        if index is None:
            raise KeyError(key)
        current_key = self._keys[index]
        if current_key is self._EMPTY or current_key is self._DELETED or current_key != key:
            raise KeyError(key)
        return self._values[index]

    def __setitem__(self, key: K, value: V) -> None:
        """Set value for key."""
        self.put(key, value)

    def __delitem__(self, key: K) -> None:
        """Delete key."""
        if not self.remove(key):
            raise KeyError(key)

    def __iter__(self) -> Iterator[K]:
        """Iterate over keys."""
        for key in self._keys:
            if key is not self._EMPTY and key is not self._DELETED:
                yield key

    def __repr__(self) -> str:
        """String representation."""
        pairs = [f"{k}: {v}" for k, v in self.items()]
        return "{" + ", ".join(pairs) + "}"

    @property
    def capacity(self) -> int:
        """Return current capacity."""
        return self._capacity

    @property
    def load_factor(self) -> float:
        """Return current load factor."""
        return self._size / self._capacity

    @property
    def is_empty(self) -> bool:
        """Return True if empty."""
        return self._size == 0

    def _hash(self, key: K) -> int:
        """Compute initial index for key."""
        return hash(key) % self._capacity

    def _probe(self, index: int, i: int) -> int:
        """
        Linear probing: next index to try.

        Args:
            index: Initial hash index.
            i: Probe number (0, 1, 2, ...).

        Returns:
            Next index to probe.
        """
        return (index + i) % self._capacity

    def _find_slot(self, key: K) -> Optional[int]:
        """
        Find the slot for a key.

        Returns the index where the key is stored, or None if not found.
        For insertion, returns the first available slot.
        """
        index = self._hash(key)
        first_deleted: Optional[int] = None

        for i in range(self._capacity):
            probe_index = self._probe(index, i)
            current_key = self._keys[probe_index]

            if current_key is self._EMPTY:
                # Key not found, return first deleted slot or this empty slot
                return first_deleted if first_deleted is not None else probe_index

            if current_key is self._DELETED:
                # Remember first deleted slot for insertion
                if first_deleted is None:
                    first_deleted = probe_index
            elif current_key == key:
                # Found the key
                return probe_index

        # Table is full (shouldn't happen with proper resizing)
        return first_deleted

    def _resize(self, new_capacity: int) -> None:
        """Resize the hash table."""
        old_keys = self._keys
        old_values = self._values

        self._capacity = new_capacity
        self._keys = [self._EMPTY] * new_capacity
        self._values = [self._EMPTY] * new_capacity
        self._size = 0

        for i, key in enumerate(old_keys):
            if key is not self._EMPTY and key is not self._DELETED:
                self.put(key, old_values[i])

    def put(self, key: K, value: V) -> None:
        """
        Insert or update a key-value pair.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key.
            value: The value.
        """
        # Check if resize needed
        if self.load_factor >= self.LOAD_FACTOR_THRESHOLD:
            self._resize(self._capacity * 2)

        index = self._find_slot(key)

        if index is None:
            # Should not happen with proper resizing
            self._resize(self._capacity * 2)
            index = self._find_slot(key)

        # Check if this is a new key
        if self._keys[index] is self._EMPTY or self._keys[index] is self._DELETED:
            self._size += 1

        self._keys[index] = key
        self._values[index] = value

    def get(self, key: K, default: Optional[V] = None) -> Optional[V]:
        """
        Get value for key.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key to look up.
            default: Value to return if key not found.

        Returns:
            The value or default.
        """
        index = self._find_slot(key)

        if index is None:
            return default

        current_key = self._keys[index]
        if current_key is self._EMPTY or current_key is self._DELETED:
            return default

        if current_key == key:
            return self._values[index]

        return default

    def remove(self, key: K) -> bool:
        """
        Remove a key-value pair.

        Uses lazy deletion with tombstones.

        Time Complexity: O(1) average, O(n) worst case

        Args:
            key: The key to remove.

        Returns:
            True if key was found and removed.
        """
        index = self._hash(key)

        for i in range(self._capacity):
            probe_index = self._probe(index, i)
            current_key = self._keys[probe_index]

            if current_key is self._EMPTY:
                return False

            if current_key == key:
                self._keys[probe_index] = self._DELETED
                self._values[probe_index] = self._EMPTY
                self._size -= 1
                return True

        return False

    def clear(self) -> None:
        """Remove all entries."""
        self._keys = [self._EMPTY] * self._capacity
        self._values = [self._EMPTY] * self._capacity
        self._size = 0

    def keys(self) -> Iterator[K]:
        """Iterate over keys."""
        return iter(self)

    def values(self) -> Iterator[V]:
        """Iterate over values."""
        for i, key in enumerate(self._keys):
            if key is not self._EMPTY and key is not self._DELETED:
                yield self._values[i]

    def items(self) -> Iterator[Tuple[K, V]]:
        """Iterate over key-value pairs."""
        for i, key in enumerate(self._keys):
            if key is not self._EMPTY and key is not self._DELETED:
                yield (key, self._values[i])


# Convenience alias
HashMap = HashTableChaining


class HashSet(Generic[K]):
    """
    A hash set implementation using a hash table.

    Stores unique keys only, no associated values.
    """

    def __init__(self) -> None:
        """Initialize empty hash set."""
        self._table: HashTableChaining[K, bool] = HashTableChaining()

    def __len__(self) -> int:
        """Return number of elements."""
        return len(self._table)

    def __contains__(self, key: K) -> bool:
        """Check if key is in set."""
        return key in self._table

    def __iter__(self) -> Iterator[K]:
        """Iterate over elements."""
        return iter(self._table)

    def __repr__(self) -> str:
        """String representation."""
        return "{" + ", ".join(str(k) for k in self) + "}"

    @property
    def is_empty(self) -> bool:
        """Return True if empty."""
        return len(self._table) == 0

    def add(self, key: K) -> None:
        """Add an element to the set."""
        self._table.put(key, True)

    def remove(self, key: K) -> bool:
        """Remove an element from the set."""
        return self._table.remove(key)

    def discard(self, key: K) -> None:
        """Remove element if present (no error if not)."""
        self._table.remove(key)

    def clear(self) -> None:
        """Remove all elements."""
        self._table.clear()

    def union(self, other: "HashSet[K]") -> "HashSet[K]":
        """Return union of two sets."""
        result: HashSet[K] = HashSet()
        for key in self:
            result.add(key)
        for key in other:
            result.add(key)
        return result

    def intersection(self, other: "HashSet[K]") -> "HashSet[K]":
        """Return intersection of two sets."""
        result: HashSet[K] = HashSet()
        for key in self:
            if key in other:
                result.add(key)
        return result

    def difference(self, other: "HashSet[K]") -> "HashSet[K]":
        """Return difference of two sets (self - other)."""
        result: HashSet[K] = HashSet()
        for key in self:
            if key not in other:
                result.add(key)
        return result

    def to_list(self) -> List[K]:
        """Convert to list."""
        return list(self)

    @classmethod
    def from_list(cls, items: List[K]) -> "HashSet[K]":
        """Create from list."""
        s: HashSet[K] = cls()
        for item in items:
            s.add(item)
        return s
