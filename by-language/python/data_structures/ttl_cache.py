"""
TTL Cache (Time-To-Live Cache) Implementation.

A cache that combines time-based expiry with LRU eviction policy.
Items automatically expire after a specified duration.

================================================================================
                         TTL CACHE VISUAL REPRESENTATION
================================================================================

    Structure: Hash Map + Ordered Dict + Expiration Times

    Cache State:
    +-------+-------------------+-------------+
    | Key   | Value             | Expires At  |
    +-------+-------------------+-------------+
    | key_A | "value_a"         | t + 60s     |
    | key_B | "value_b"         | t + 30s     | <-- Will expire first
    | key_C | "value_c"         | t + 90s     |
    +-------+-------------------+-------------+

    LRU Order (for capacity eviction):
    HEAD <-> [A] <-> [B] <-> [C] <-> TAIL
             ^                ^
             |                |
          Most Recent    Least Recent

    Eviction Priority:
    1. Expired items (time-based)
    2. LRU items (when at capacity)

================================================================================

COMPLEXITY:
+-----------+------+-------+
| Operation | Time | Space |
+-----------+------+-------+
| Get       | O(1) | O(1)  |
| Put       | O(1)*| O(1)  |
| Contains  | O(1) | O(1)  |
| Cleanup   | O(n) | O(1)  |
+-----------+------+-------+
* O(n) if cleanup triggers during put
Space: O(capacity) for the cache

USE CASES:
- Session management with timeout
- API rate limiting
- DNS caching
- Token caching with expiry
- Database query caching with freshness
- Web browser cache with max-age
"""

from typing import TypeVar, Generic, Dict, Optional, Tuple
from collections import OrderedDict

K = TypeVar('K')
V = TypeVar('V')


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
