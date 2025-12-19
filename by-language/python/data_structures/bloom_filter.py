"""
Bloom Filter and Skip List Data Structures.

╔══════════════════════════════════════════════════════════════════════════════╗
║                              BLOOM FILTER                                    ║
╠══════════════════════════════════════════════════════════════════════════════╣
║                                                                              ║
║  A space-efficient probabilistic data structure for set membership.          ║
║                                                                              ║
║  Structure (m bits, k hash functions):                                       ║
║  ┌─────────────────────────────────────────────────────────────────────┐    ║
║  │ Bit Array: 0 0 1 0 1 1 0 0 1 0 0 1 0 1 0 0 1 0 0 1                 │    ║
║  │            ↑     ↑       ↑     ↑     ↑       ↑                      │    ║
║  │            └─────┴───────┴─────┴─────┴───────┘                      │    ║
║  │                  Bits set by inserted elements                      │    ║
║  └─────────────────────────────────────────────────────────────────────┘    ║
║                                                                              ║
║  Insert "hello":                                                             ║
║  ┌─────────────────────────────────────────────────────────────────────┐    ║
║  │  "hello" → h1(x)=2, h2(x)=5, h3(x)=8                               │    ║
║  │            Set bits 2, 5, and 8 to 1                                │    ║
║  └─────────────────────────────────────────────────────────────────────┘    ║
║                                                                              ║
║  Query "hello":                                                              ║
║  ┌─────────────────────────────────────────────────────────────────────┐    ║
║  │  Check bits 2, 5, 8                                                 │    ║
║  │  All are 1 → "Possibly in set" (might be false positive)           │    ║
║  │  Any is 0 → "Definitely not in set"                                │    ║
║  └─────────────────────────────────────────────────────────────────────┘    ║
║                                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌───────────────┬────────┬────────┐
│ Operation     │ Time   │ Space  │
├───────────────┼────────┼────────┤
│ Insert        │ O(k)   │ O(m)   │
│ Query         │ O(k)   │ O(1)   │
│ Space         │ -      │ O(m)   │
└───────────────┴────────┴────────┘

k = number of hash functions
m = size of bit array

FALSE POSITIVE RATE:
p ≈ (1 - e^(-kn/m))^k

where n = number of elements inserted

OPTIMAL k = (m/n) * ln(2) ≈ 0.693 * (m/n)

USE CASES:
- Checking if URL was crawled (web crawlers)
- Preventing duplicate cache lookups
- Database query optimization
- Spell checkers
- Network packet filtering
"""

import math
from typing import Any, List, Optional, Callable
import hashlib


class BloomFilter:
    """
    Standard Bloom Filter implementation.

    Supports configurable false positive rate.
    """

    def __init__(
        self,
        expected_elements: int = 1000,
        false_positive_rate: float = 0.01
    ) -> None:
        """
        Initialize Bloom filter with optimal parameters.

        Args:
            expected_elements: Expected number of elements to insert.
            false_positive_rate: Desired false positive probability.
        """
        # Calculate optimal bit array size
        # m = -(n * ln(p)) / (ln(2)^2)
        self._size = self._optimal_size(expected_elements, false_positive_rate)

        # Calculate optimal number of hash functions
        # k = (m/n) * ln(2)
        self._num_hashes = self._optimal_hashes(self._size, expected_elements)

        # Initialize bit array
        self._bit_array = [False] * self._size
        self._count = 0

    @staticmethod
    def _optimal_size(n: int, p: float) -> int:
        """Calculate optimal bit array size."""
        if p <= 0:
            p = 0.0001
        m = -(n * math.log(p)) / (math.log(2) ** 2)
        return max(int(m), 64)

    @staticmethod
    def _optimal_hashes(m: int, n: int) -> int:
        """Calculate optimal number of hash functions."""
        if n <= 0:
            n = 1
        k = (m / n) * math.log(2)
        return max(int(k), 1)

    def _hash(self, item: Any, seed: int) -> int:
        """
        Generate hash for item with given seed.

        Uses double hashing technique for efficiency:
        h_i(x) = h1(x) + i * h2(x)
        """
        data = str(item).encode('utf-8')

        # Primary hash
        h1 = int(hashlib.md5(data).hexdigest(), 16)
        # Secondary hash
        h2 = int(hashlib.sha256(data).hexdigest(), 16)

        return (h1 + seed * h2) % self._size

    def add(self, item: Any) -> None:
        """
        Add item to the filter.

        Time: O(k) where k = number of hash functions.
        """
        for i in range(self._num_hashes):
            index = self._hash(item, i)
            self._bit_array[index] = True
        self._count += 1

    def __contains__(self, item: Any) -> bool:
        """
        Check if item might be in the filter.

        Returns:
            True: Item is possibly in set (might be false positive).
            False: Item is definitely not in set.

        Time: O(k)
        """
        return all(
            self._bit_array[self._hash(item, i)]
            for i in range(self._num_hashes)
        )

    def contains(self, item: Any) -> bool:
        """Alias for __contains__."""
        return item in self

    @property
    def size(self) -> int:
        """Return size of bit array."""
        return self._size

    @property
    def num_hashes(self) -> int:
        """Return number of hash functions."""
        return self._num_hashes

    @property
    def count(self) -> int:
        """Return approximate number of items added."""
        return self._count

    def estimated_false_positive_rate(self) -> float:
        """
        Estimate current false positive rate.

        Formula: p = (1 - e^(-kn/m))^k
        """
        if self._count == 0:
            return 0.0
        exponent = -self._num_hashes * self._count / self._size
        return (1 - math.exp(exponent)) ** self._num_hashes

    def __repr__(self) -> str:
        return (f"BloomFilter(size={self._size}, hashes={self._num_hashes}, "
                f"count={self._count}, fp_rate={self.estimated_false_positive_rate():.4f})")


class CountingBloomFilter:
    """
    Counting Bloom Filter - supports deletions.

    Instead of bits, uses counters that can be incremented and decremented.
    """

    def __init__(
        self,
        expected_elements: int = 1000,
        false_positive_rate: float = 0.01,
        counter_bits: int = 4
    ) -> None:
        """
        Initialize counting Bloom filter.

        Args:
            expected_elements: Expected number of elements.
            false_positive_rate: Desired false positive probability.
            counter_bits: Bits per counter (4 allows counts up to 15).
        """
        self._size = BloomFilter._optimal_size(expected_elements, false_positive_rate)
        self._num_hashes = BloomFilter._optimal_hashes(self._size, expected_elements)
        self._max_count = (1 << counter_bits) - 1
        self._counters = [0] * self._size
        self._count = 0

    def _hash(self, item: Any, seed: int) -> int:
        """Generate hash for item."""
        data = str(item).encode('utf-8')
        h1 = int(hashlib.md5(data).hexdigest(), 16)
        h2 = int(hashlib.sha256(data).hexdigest(), 16)
        return (h1 + seed * h2) % self._size

    def add(self, item: Any) -> bool:
        """
        Add item to the filter.

        Returns:
            True if added successfully, False if any counter would overflow.
        """
        indices = [self._hash(item, i) for i in range(self._num_hashes)]

        # Check for overflow
        if any(self._counters[idx] >= self._max_count for idx in indices):
            return False

        for idx in indices:
            self._counters[idx] += 1
        self._count += 1
        return True

    def remove(self, item: Any) -> bool:
        """
        Remove item from the filter.

        Returns:
            True if removed, False if item not in filter.
        """
        if item not in self:
            return False

        for i in range(self._num_hashes):
            idx = self._hash(item, i)
            if self._counters[idx] > 0:
                self._counters[idx] -= 1
        self._count -= 1
        return True

    def __contains__(self, item: Any) -> bool:
        """Check if item might be in the filter."""
        return all(
            self._counters[self._hash(item, i)] > 0
            for i in range(self._num_hashes)
        )

    @property
    def count(self) -> int:
        """Return number of items."""
        return self._count


class ScalableBloomFilter:
    """
    Scalable Bloom Filter - grows as needed.

    Creates new Bloom filters as capacity is reached,
    maintaining the overall false positive rate.
    """

    def __init__(
        self,
        initial_capacity: int = 1000,
        false_positive_rate: float = 0.01,
        growth_rate: int = 2
    ) -> None:
        """
        Initialize scalable Bloom filter.

        Args:
            initial_capacity: Initial capacity of first filter.
            false_positive_rate: Target overall false positive rate.
            growth_rate: Factor by which each new filter grows.
        """
        self._initial_capacity = initial_capacity
        self._fp_rate = false_positive_rate
        self._growth_rate = growth_rate
        self._filters: List[BloomFilter] = []
        self._count = 0

        # Each filter gets tighter fp rate to maintain overall rate
        # Sum of geometric series: fp = r + r^2 + r^3 + ...
        # Where r = fp_rate * (1 - fp_rate)
        self._fp_ratio = 0.9  # Each filter's fp rate relative to target

        self._add_filter()

    def _add_filter(self) -> None:
        """Add a new Bloom filter with appropriate parameters."""
        capacity = self._initial_capacity * (self._growth_rate ** len(self._filters))
        # Tighter fp rate for each subsequent filter
        fp = self._fp_rate * (self._fp_ratio ** len(self._filters))
        self._filters.append(BloomFilter(capacity, fp))

    def add(self, item: Any) -> None:
        """Add item to the filter."""
        if item in self:
            return

        # If current filter is at capacity, add new one
        current = self._filters[-1]
        if current.count >= self._initial_capacity * (self._growth_rate ** (len(self._filters) - 1)):
            self._add_filter()
            current = self._filters[-1]

        current.add(item)
        self._count += 1

    def __contains__(self, item: Any) -> bool:
        """Check if item might be in any filter."""
        return any(item in f for f in self._filters)

    @property
    def count(self) -> int:
        """Return total count."""
        return self._count

    @property
    def num_filters(self) -> int:
        """Return number of sub-filters."""
        return len(self._filters)


"""
╔══════════════════════════════════════════════════════════════════════════════╗
║                                SKIP LIST                                     ║
╠══════════════════════════════════════════════════════════════════════════════╣
║                                                                              ║
║  A probabilistic data structure for ordered data with O(log n) operations.   ║
║                                                                              ║
║  Structure:                                                                  ║
║  ┌────────────────────────────────────────────────────────────────────────┐ ║
║  │ Level 3: HEAD ─────────────────────────────→ 50 ─────────────────→ NIL │ ║
║  │ Level 2: HEAD ────→ 10 ─────────────────────→ 50 ─────────────────→ NIL │ ║
║  │ Level 1: HEAD ────→ 10 ─────→ 30 ───────────→ 50 ────→ 70 ────────→ NIL │ ║
║  │ Level 0: HEAD ─→ 10 ─→ 20 ─→ 30 ─→ 40 ─→ 50 ─→ 60 ─→ 70 ─→ 80 ─→ NIL  │ ║
║  └────────────────────────────────────────────────────────────────────────┘ ║
║                                                                              ║
║  Search for 40:                                                              ║
║  1. Start at HEAD, Level 3                                                   ║
║  2. 50 > 40, go down to Level 2                                             ║
║  3. 10 < 40, go right; 50 > 40, go down to Level 1                          ║
║  4. 10 < 40, go right; 30 < 40, go right; 50 > 40, go down to Level 0       ║
║  5. 30 < 40, go right; 40 == 40, FOUND!                                     ║
║                                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY (expected):
┌───────────────┬────────────┬───────────┐
│ Operation     │ Average    │ Worst     │
├───────────────┼────────────┼───────────┤
│ Search        │ O(log n)   │ O(n)      │
│ Insert        │ O(log n)   │ O(n)      │
│ Delete        │ O(log n)   │ O(n)      │
│ Space         │ O(n)       │ O(n log n)│
└───────────────┴────────────┴───────────┘

USE CASES:
- Redis sorted sets (ZSET)
- LevelDB / RocksDB memtable
- Lock-free concurrent data structures
- Range queries
"""

import random
from typing import TypeVar, Generic, Iterator, Tuple

K = TypeVar('K')
V = TypeVar('V')


class SkipListNode(Generic[K, V]):
    """Node in a skip list."""

    def __init__(self, key: K, value: V, level: int) -> None:
        self.key = key
        self.value = value
        # Forward pointers for each level
        self.forward: List[Optional['SkipListNode[K, V]']] = [None] * (level + 1)

    def __repr__(self) -> str:
        return f"SkipListNode({self.key}: {self.value}, levels={len(self.forward)})"


class SkipList(Generic[K, V]):
    """
    Skip List implementation.

    A probabilistic alternative to balanced trees.

    LeetCode: #1206 Design Skiplist
    """

    def __init__(self, max_level: int = 16, p: float = 0.5) -> None:
        """
        Initialize skip list.

        Args:
            max_level: Maximum number of levels.
            p: Probability of promotion to next level.
        """
        self._max_level = max_level
        self._p = p
        self._level = 0  # Current highest level
        # Head node with maximum levels
        self._head: SkipListNode[K, V] = SkipListNode(None, None, max_level)  # type: ignore
        self._size = 0

    def _random_level(self) -> int:
        """Generate random level for new node."""
        level = 0
        while random.random() < self._p and level < self._max_level:
            level += 1
        return level

    def search(self, key: K) -> Optional[V]:
        """
        Search for key and return value.

        Time: O(log n) expected.
        """
        current = self._head

        # Start from highest level and work down
        for i in range(self._level, -1, -1):
            while (current.forward[i] is not None and
                   current.forward[i].key < key):  # type: ignore
                current = current.forward[i]

        # Move to potential match
        current = current.forward[0]

        if current is not None and current.key == key:
            return current.value
        return None

    def __getitem__(self, key: K) -> V:
        """Get value by key, raises KeyError if not found."""
        result = self.search(key)
        if result is None:
            # Check if key exists with None value
            current = self._head
            for i in range(self._level, -1, -1):
                while (current.forward[i] is not None and
                       current.forward[i].key < key):  # type: ignore
                    current = current.forward[i]
            current = current.forward[0]
            if current is None or current.key != key:
                raise KeyError(key)
        return result  # type: ignore

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        current = self._head
        for i in range(self._level, -1, -1):
            while (current.forward[i] is not None and
                   current.forward[i].key < key):  # type: ignore
                current = current.forward[i]
        current = current.forward[0]
        return current is not None and current.key == key

    def insert(self, key: K, value: V) -> None:
        """
        Insert key-value pair.

        Time: O(log n) expected.
        """
        # Track nodes that need updating at each level
        update: List[SkipListNode[K, V]] = [self._head] * (self._max_level + 1)
        current = self._head

        # Find position and track update path
        for i in range(self._level, -1, -1):
            while (current.forward[i] is not None and
                   current.forward[i].key < key):  # type: ignore
                current = current.forward[i]
            update[i] = current

        current = current.forward[0]

        # Update existing key
        if current is not None and current.key == key:
            current.value = value
            return

        # Generate random level for new node
        new_level = self._random_level()

        # Update list level if necessary
        if new_level > self._level:
            for i in range(self._level + 1, new_level + 1):
                update[i] = self._head
            self._level = new_level

        # Create new node
        new_node: SkipListNode[K, V] = SkipListNode(key, value, new_level)

        # Insert by updating forward pointers
        for i in range(new_level + 1):
            new_node.forward[i] = update[i].forward[i]
            update[i].forward[i] = new_node

        self._size += 1

    def __setitem__(self, key: K, value: V) -> None:
        """Set value by key."""
        self.insert(key, value)

    def delete(self, key: K) -> bool:
        """
        Delete key from skip list.

        Time: O(log n) expected.

        Returns:
            True if key was found and deleted.
        """
        update: List[SkipListNode[K, V]] = [self._head] * (self._max_level + 1)
        current = self._head

        # Find position and track update path
        for i in range(self._level, -1, -1):
            while (current.forward[i] is not None and
                   current.forward[i].key < key):  # type: ignore
                current = current.forward[i]
            update[i] = current

        current = current.forward[0]

        # Key not found
        if current is None or current.key != key:
            return False

        # Update forward pointers
        for i in range(self._level + 1):
            if update[i].forward[i] != current:
                break
            update[i].forward[i] = current.forward[i]

        # Decrease level if necessary
        while self._level > 0 and self._head.forward[self._level] is None:
            self._level -= 1

        self._size -= 1
        return True

    def __delitem__(self, key: K) -> None:
        """Delete by key, raises KeyError if not found."""
        if not self.delete(key):
            raise KeyError(key)

    def __len__(self) -> int:
        """Return number of elements."""
        return self._size

    def __iter__(self) -> Iterator[K]:
        """Iterate over keys in sorted order."""
        current = self._head.forward[0]
        while current is not None:
            yield current.key
            current = current.forward[0]

    def items(self) -> Iterator[Tuple[K, V]]:
        """Iterate over (key, value) pairs in sorted order."""
        current = self._head.forward[0]
        while current is not None:
            yield (current.key, current.value)
            current = current.forward[0]

    def range(self, start: K, end: K) -> Iterator[Tuple[K, V]]:
        """
        Get all key-value pairs in range [start, end).

        Time: O(log n + k) where k = number of elements in range.
        """
        # Find start position
        current = self._head
        for i in range(self._level, -1, -1):
            while (current.forward[i] is not None and
                   current.forward[i].key < start):  # type: ignore
                current = current.forward[i]

        current = current.forward[0]

        # Yield elements in range
        while current is not None and current.key < end:  # type: ignore
            yield (current.key, current.value)
            current = current.forward[0]

    def min(self) -> Optional[Tuple[K, V]]:
        """Get minimum key-value pair."""
        if self._head.forward[0] is None:
            return None
        node = self._head.forward[0]
        return (node.key, node.value)

    def max(self) -> Optional[Tuple[K, V]]:
        """Get maximum key-value pair."""
        if self._size == 0:
            return None

        current = self._head
        for i in range(self._level, -1, -1):
            while current.forward[i] is not None:
                current = current.forward[i]

        return (current.key, current.value)

    @property
    def level(self) -> int:
        """Return current highest level."""
        return self._level

    def __repr__(self) -> str:
        return f"SkipList(size={self._size}, level={self._level})"

    def display(self) -> str:
        """Return visual representation of skip list."""
        if self._size == 0:
            return "Empty SkipList"

        lines = []
        for i in range(self._level, -1, -1):
            line = f"L{i}: HEAD"
            current = self._head.forward[i]
            while current is not None:
                line += f" -> {current.key}"
                current = current.forward[i]
            line += " -> NIL"
            lines.append(line)

        return "\n".join(lines)
