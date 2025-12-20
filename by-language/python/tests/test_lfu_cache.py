"""
Tests for LFU Cache implementation.
"""

import pytest
from data_structures.lfu_cache import LFUCache


class TestLFUCacheBasics:
    """Test basic LFU cache operations."""

    def test_init(self):
        """Test cache initialization."""
        cache = LFUCache(3)
        assert cache.capacity == 3
        assert cache.size == 0
        assert len(cache) == 0
        assert cache.is_empty()
        assert not cache.is_full()

    def test_init_invalid_capacity(self):
        """Test invalid capacity raises error."""
        with pytest.raises(ValueError):
            LFUCache(0)
        with pytest.raises(ValueError):
            LFUCache(-1)

    def test_put_and_get(self):
        """Test basic put and get operations."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')

        assert cache.get(1) == 'a'
        assert cache.get(2) == 'b'
        assert cache.get(3) == 'c'
        assert cache.size == 3

    def test_get_nonexistent_key(self):
        """Test getting a key that doesn't exist."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        assert cache.get(2) is None

    def test_update_existing_key(self):
        """Test updating an existing key."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(1, 'b')

        assert cache.get(1) == 'b'
        assert cache.size == 1

    def test_contains(self):
        """Test key containment check."""
        cache = LFUCache(3)
        cache.put(1, 'a')

        assert 1 in cache
        assert 2 not in cache


class TestLFUEviction:
    """Test LFU eviction behavior."""

    def test_eviction_single_frequency(self):
        """Test eviction when all items have same frequency."""
        cache = LFUCache(2)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')  # Should evict key 1 (LRU among freq=1)

        assert cache.get(1) is None
        assert cache.get(2) == 'b'
        assert cache.get(3) == 'c'

    def test_eviction_different_frequency(self):
        """Test eviction respects frequency."""
        cache = LFUCache(2)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.get(1)  # freq(1) = 2
        cache.put(3, 'c')  # Should evict key 2 (freq=1)

        assert cache.get(1) == 'a'
        assert cache.get(2) is None
        assert cache.get(3) == 'c'

    def test_eviction_lru_tiebreaker(self):
        """Test LRU is used as tiebreaker for same frequency."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')

        # Access all to get freq=2
        cache.get(1)
        cache.get(2)
        cache.get(3)

        # Access 1 and 3 more to get freq=3
        cache.get(1)
        cache.get(3)

        # Add new item - should evict key 2 (lowest freq=2, LRU)
        cache.put(4, 'd')

        assert cache.get(1) == 'a'
        assert cache.get(2) is None  # Evicted
        assert cache.get(3) == 'c'
        assert cache.get(4) == 'd'

    def test_eviction_returns_evicted(self):
        """Test that put returns evicted item."""
        cache = LFUCache(2)
        cache.put(1, 'a')
        cache.put(2, 'b')

        result = cache.put(3, 'c')
        assert result == (1, 'a')

    def test_update_no_eviction(self):
        """Test that updating existing key doesn't evict."""
        cache = LFUCache(2)
        cache.put(1, 'a')
        cache.put(2, 'b')

        result = cache.put(1, 'updated')
        assert result is None
        assert cache.size == 2


class TestLFUFrequency:
    """Test frequency tracking."""

    def test_initial_frequency(self):
        """Test that new items have frequency 1."""
        cache = LFUCache(3)
        cache.put(1, 'a')

        assert cache.get_frequency(1) == 1

    def test_get_increments_frequency(self):
        """Test that get increments frequency."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.get(1)
        cache.get(1)

        assert cache.get_frequency(1) == 3

    def test_put_update_increments_frequency(self):
        """Test that put on existing key increments frequency."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(1, 'b')

        assert cache.get_frequency(1) == 2

    def test_frequency_nonexistent_key(self):
        """Test frequency of nonexistent key."""
        cache = LFUCache(3)
        assert cache.get_frequency(1) == -1

    def test_min_frequency(self):
        """Test minimum frequency tracking."""
        cache = LFUCache(3)
        assert cache.min_frequency() == 0  # Empty

        cache.put(1, 'a')
        assert cache.min_frequency() == 1

        cache.get(1)
        assert cache.min_frequency() == 2

        cache.put(2, 'b')  # New item with freq=1
        assert cache.min_frequency() == 1


class TestLFUDelete:
    """Test delete operations."""

    def test_delete_existing_key(self):
        """Test deleting an existing key."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(2, 'b')

        assert cache.delete(1)
        assert cache.get(1) is None
        assert cache.size == 1

    def test_delete_nonexistent_key(self):
        """Test deleting a nonexistent key."""
        cache = LFUCache(3)
        cache.put(1, 'a')

        assert not cache.delete(2)
        assert cache.size == 1


class TestLFUPeek:
    """Test peek operations."""

    def test_peek_existing_key(self):
        """Test peeking at an existing key."""
        cache = LFUCache(3)
        cache.put(1, 'a')

        assert cache.peek(1) == 'a'
        assert cache.get_frequency(1) == 1  # Frequency unchanged

    def test_peek_nonexistent_key(self):
        """Test peeking at a nonexistent key."""
        cache = LFUCache(3)
        assert cache.peek(1) is None


class TestLFUClear:
    """Test clear operations."""

    def test_clear(self):
        """Test clearing the cache."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')

        cache.clear()

        assert cache.size == 0
        assert cache.is_empty()
        assert cache.get(1) is None


class TestLFUEdgeCases:
    """Test edge cases."""

    def test_capacity_one(self):
        """Test cache with capacity of 1."""
        cache = LFUCache(1)
        cache.put(1, 'a')
        assert cache.get(1) == 'a'

        cache.put(2, 'b')
        assert cache.get(1) is None
        assert cache.get(2) == 'b'

    def test_multiple_evictions(self):
        """Test multiple evictions."""
        cache = LFUCache(2)
        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')  # Evicts 1
        cache.put(4, 'd')  # Evicts 2

        assert cache.get(1) is None
        assert cache.get(2) is None
        assert cache.get(3) == 'c'
        assert cache.get(4) == 'd'

    def test_leetcode_example(self):
        """Test LeetCode #460 example."""
        cache = LFUCache(2)

        cache.put(1, 1)
        cache.put(2, 2)
        assert cache.get(1) == 1

        cache.put(3, 3)  # Evicts key 2
        assert cache.get(2) is None
        assert cache.get(3) == 3

        cache.put(4, 4)  # Evicts key 1
        assert cache.get(1) is None
        assert cache.get(3) == 3
        assert cache.get(4) == 4

    def test_different_types(self):
        """Test cache with different key/value types."""
        cache: LFUCache[str, int] = LFUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)

        assert cache.get('a') == 1
        assert cache.get('b') == 2
        assert cache.get('c') == 3

    def test_repr(self):
        """Test string representation."""
        cache = LFUCache(3)
        cache.put(1, 'a')
        cache.put(2, 'b')

        r = repr(cache)
        assert 'LFUCache' in r
        assert 'capacity=3' in r
        assert 'size=2' in r


class TestLFUStress:
    """Stress tests for LFU cache."""

    def test_many_operations(self):
        """Test many put/get operations."""
        cache = LFUCache(100)

        # Fill cache
        for i in range(100):
            cache.put(i, str(i))

        # Access some items multiple times
        for i in range(50):
            for _ in range(i):
                cache.get(i)

        # Add more items - should evict least frequently used
        for i in range(100, 150):
            cache.put(i, str(i))

        # Lower frequency items should be evicted
        assert cache.size == 100

    def test_frequency_ordering(self):
        """Test that frequency ordering is maintained."""
        cache = LFUCache(3)

        cache.put(1, 'a')
        cache.put(2, 'b')
        cache.put(3, 'c')

        # Make freq: 1->5, 2->3, 3->1
        for _ in range(4):
            cache.get(1)
        for _ in range(2):
            cache.get(2)

        # Evict key 3 (freq=1)
        cache.put(4, 'd')
        assert cache.get(3) is None

        # Evict key 4 (freq=1)
        cache.put(5, 'e')
        assert cache.get(4) is None

        # Keys 1 and 2 should still exist
        assert cache.get(1) == 'a'
        assert cache.get(2) == 'b'
