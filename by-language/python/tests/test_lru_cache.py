"""
Tests for LRU Cache implementations.
"""

import pytest
import time
from data_structures.lru_cache import (
    LRUCache, LRUCacheSimple, LFUCache, TTLCache
)


class TestLRUCacheBasics:
    """Test basic LRU Cache operations."""

    def test_init(self):
        """Test initialization."""
        cache = LRUCache(3)
        assert len(cache) == 0
        assert cache.capacity == 3

    def test_init_invalid_capacity(self):
        """Test initialization with invalid capacity."""
        with pytest.raises(ValueError):
            LRUCache(0)
        with pytest.raises(ValueError):
            LRUCache(-1)

    def test_put_and_get(self):
        """Test basic put and get."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.get('a') == 1
        assert cache.get('b') == 2
        assert cache.get('c') is None

    def test_update_value(self):
        """Test updating existing key."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('a', 10)

        assert cache.get('a') == 10
        assert len(cache) == 1

    def test_eviction(self):
        """Test LRU eviction."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)  # Should evict 'a'

        assert cache.get('a') is None
        assert cache.get('b') == 2
        assert cache.get('c') == 3

    def test_access_updates_lru(self):
        """Test that get updates LRU order."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.get('a')  # Access 'a', making 'b' the LRU
        cache.put('c', 3)  # Should evict 'b'

        assert cache.get('a') == 1
        assert cache.get('b') is None
        assert cache.get('c') == 3

    def test_put_updates_lru(self):
        """Test that put updates LRU order."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('a', 10)  # Update 'a', making 'b' the LRU
        cache.put('c', 3)  # Should evict 'b'

        assert cache.get('a') == 10
        assert cache.get('b') is None
        assert cache.get('c') == 3


class TestLRUCacheDelete:
    """Test LRU Cache delete operations."""

    def test_delete_existing(self):
        """Test deleting existing key."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.delete('a') == 1
        assert cache.get('a') is None
        assert len(cache) == 1

    def test_delete_nonexistent(self):
        """Test deleting non-existent key."""
        cache = LRUCache(3)
        cache.put('a', 1)

        assert cache.delete('b') is None
        assert len(cache) == 1

    def test_delete_and_reinsert(self):
        """Test deleting and reinserting."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.delete('a')
        cache.put('c', 3)

        assert len(cache) == 2
        assert cache.get('b') == 2
        assert cache.get('c') == 3


class TestLRUCachePeek:
    """Test LRU Cache peek operations."""

    def test_peek_existing(self):
        """Test peek on existing key."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.peek('a') == 1

    def test_peek_does_not_update_order(self):
        """Test that peek doesn't update LRU order."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.peek('a')  # Should NOT update order
        cache.put('c', 3)  # Should evict 'a' (still LRU)

        assert cache.get('a') is None  # 'a' was evicted
        assert cache.get('b') == 2
        assert cache.get('c') == 3


class TestLRUCacheCapacity:
    """Test LRU Cache capacity operations."""

    def test_resize_larger(self):
        """Test resizing to larger capacity."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)

        cache.resize(4)
        cache.put('c', 3)
        cache.put('d', 4)

        assert len(cache) == 4
        assert cache.get('a') == 1

    def test_resize_smaller(self):
        """Test resizing to smaller capacity."""
        cache = LRUCache(4)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)
        cache.put('d', 4)

        cache.resize(2)

        assert len(cache) == 2
        # 'a' and 'b' should be evicted (LRU)
        assert cache.get('a') is None
        assert cache.get('b') is None

    def test_resize_invalid(self):
        """Test resizing to invalid capacity."""
        cache = LRUCache(3)
        with pytest.raises(ValueError):
            cache.resize(0)


class TestLRUCacheSpecialMethods:
    """Test LRU Cache special methods."""

    def test_get_lru_key(self):
        """Test getting LRU key."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)

        assert cache.get_lru_key() == 'a'

        cache.get('a')  # Access 'a'
        assert cache.get_lru_key() == 'b'

    def test_get_mru_key(self):
        """Test getting MRU key."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)

        assert cache.get_mru_key() == 'c'

        cache.get('a')  # Access 'a'
        assert cache.get_mru_key() == 'a'

    def test_get_lru_mru_empty(self):
        """Test LRU/MRU on empty cache."""
        cache = LRUCache(3)
        assert cache.get_lru_key() is None
        assert cache.get_mru_key() is None


class TestLRUCacheDunder:
    """Test LRU Cache magic methods."""

    def test_len(self):
        """Test __len__."""
        cache = LRUCache(3)
        assert len(cache) == 0

        cache.put('a', 1)
        assert len(cache) == 1

        cache.put('b', 2)
        cache.put('c', 3)
        assert len(cache) == 3

    def test_contains(self):
        """Test __contains__."""
        cache = LRUCache(2)
        cache.put('a', 1)

        assert 'a' in cache
        assert 'b' not in cache

    def test_getitem(self):
        """Test __getitem__."""
        cache = LRUCache(2)
        cache.put('a', 1)

        assert cache['a'] == 1

    def test_getitem_keyerror(self):
        """Test __getitem__ raises KeyError."""
        cache = LRUCache(2)
        with pytest.raises(KeyError):
            _ = cache['missing']

    def test_setitem(self):
        """Test __setitem__."""
        cache = LRUCache(2)
        cache['a'] = 1
        cache['b'] = 2

        assert cache['a'] == 1
        assert cache['b'] == 2

    def test_delitem(self):
        """Test __delitem__."""
        cache = LRUCache(2)
        cache['a'] = 1
        del cache['a']

        assert 'a' not in cache

    def test_delitem_keyerror(self):
        """Test __delitem__ raises KeyError."""
        cache = LRUCache(2)
        with pytest.raises(KeyError):
            del cache['missing']

    def test_iter(self):
        """Test __iter__ (MRU to LRU order)."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)
        cache.get('a')  # Make 'a' MRU

        keys = list(cache)
        assert keys == ['a', 'c', 'b']

    def test_repr(self):
        """Test __repr__."""
        cache = LRUCache(3)
        cache.put('a', 1)

        s = repr(cache)
        assert "LRUCache" in s
        assert "3" in s  # capacity
        assert "1" in s  # size


class TestLRUCacheIterators:
    """Test LRU Cache iterator methods."""

    def test_items(self):
        """Test items iterator."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)

        items = list(cache.items())
        assert len(items) == 3
        assert items[0] == ('c', 3)  # MRU
        assert items[-1] == ('a', 1)  # LRU

    def test_keys(self):
        """Test keys iterator."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)

        keys = list(cache.keys())
        assert 'a' in keys
        assert 'b' in keys

    def test_values(self):
        """Test values iterator."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)

        values = list(cache.values())
        assert 1 in values
        assert 2 in values


class TestLRUCacheClear:
    """Test LRU Cache clear operation."""

    def test_clear(self):
        """Test clearing cache."""
        cache = LRUCache(3)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)

        cache.clear()

        assert len(cache) == 0
        assert cache.get('a') is None


class TestLRUCacheSimple:
    """Test simple OrderedDict-based LRU Cache."""

    def test_basic_operations(self):
        """Test basic operations."""
        cache = LRUCacheSimple(2)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.get('a') == 1
        assert cache.get('b') == 2

    def test_eviction(self):
        """Test LRU eviction."""
        cache = LRUCacheSimple(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.get('a')  # Access 'a'
        cache.put('c', 3)  # Evicts 'b'

        assert cache.get('a') == 1
        assert cache.get('b') is None
        assert cache.get('c') == 3

    def test_delete(self):
        """Test delete."""
        cache = LRUCacheSimple(2)
        cache.put('a', 1)
        assert cache.delete('a') == 1
        assert 'a' not in cache

    def test_contains(self):
        """Test contains."""
        cache = LRUCacheSimple(2)
        cache.put('a', 1)
        assert 'a' in cache
        assert 'b' not in cache


class TestLFUCache:
    """Test LFU Cache implementation."""

    def test_basic_operations(self):
        """Test basic put and get."""
        cache = LFUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.get('a') == 1
        assert cache.get('b') == 2

    def test_frequency_based_eviction(self):
        """Test that LFU evicts least frequently used."""
        cache = LFUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.get('a')  # freq[a] = 2, freq[b] = 1
        cache.put('c', 3)  # Evicts 'b' (lower frequency)

        assert cache.get('a') == 1
        assert cache.get('b') is None
        assert cache.get('c') == 3

    def test_tie_breaker_lru(self):
        """Test LRU tie-breaking when frequencies equal."""
        cache = LFUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        # Both have freq = 1, 'a' is LRU
        cache.put('c', 3)  # Evicts 'a'

        assert cache.get('a') is None
        assert cache.get('b') == 2

    def test_update_value(self):
        """Test updating value increases frequency."""
        cache = LFUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('a', 10)  # Update, freq[a] = 2
        cache.put('c', 3)  # Evicts 'b'

        assert cache.get('a') == 10
        assert cache.get('b') is None

    def test_len_contains(self):
        """Test len and contains."""
        cache = LFUCache(2)
        cache.put('a', 1)

        assert len(cache) == 1
        assert 'a' in cache
        assert 'b' not in cache


class TestTTLCache:
    """Test TTL Cache implementation."""

    def test_basic_operations(self):
        """Test basic put and get."""
        cache = TTLCache(capacity=2, ttl=10.0)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.get('a') == 1
        assert cache.get('b') == 2

    def test_expiration(self):
        """Test item expiration."""
        cache = TTLCache(capacity=2, ttl=0.1)
        cache.put('a', 1)

        assert cache.get('a') == 1
        time.sleep(0.15)
        assert cache.get('a') is None

    def test_custom_ttl(self):
        """Test custom TTL per item."""
        cache = TTLCache(capacity=2, ttl=10.0)
        cache.put('short', 1, ttl=0.1)
        cache.put('long', 2, ttl=10.0)

        time.sleep(0.15)
        assert cache.get('short') is None
        assert cache.get('long') == 2

    def test_capacity_eviction(self):
        """Test capacity-based eviction."""
        cache = TTLCache(capacity=2, ttl=10.0)
        cache.put('a', 1)
        cache.put('b', 2)
        cache.put('c', 3)  # Evicts 'a'

        assert cache.get('a') is None
        assert cache.get('b') == 2
        assert cache.get('c') == 3

    def test_len_with_expiration(self):
        """Test len accounts for expired items."""
        cache = TTLCache(capacity=3, ttl=0.1)
        cache.put('a', 1)
        cache.put('b', 2)

        assert len(cache) == 2
        time.sleep(0.15)
        assert len(cache) == 0

    def test_contains_with_expiration(self):
        """Test contains checks expiration."""
        cache = TTLCache(capacity=2, ttl=0.1)
        cache.put('a', 1)

        assert 'a' in cache
        time.sleep(0.15)
        assert 'a' not in cache


class TestLRUCacheEdgeCases:
    """Test edge cases."""

    def test_capacity_one(self):
        """Test cache with capacity 1."""
        cache = LRUCache(1)
        cache.put('a', 1)
        cache.put('b', 2)

        assert cache.get('a') is None
        assert cache.get('b') == 2

    def test_none_values(self):
        """Test storing None values."""
        cache = LRUCache(2)
        cache.put('a', None)

        assert 'a' in cache
        assert cache.get('a') is None  # Returns None but key exists

    def test_eviction_returns_evicted(self):
        """Test that put returns evicted item."""
        cache = LRUCache(2)
        cache.put('a', 1)
        cache.put('b', 2)
        evicted = cache.put('c', 3)

        assert evicted == ('a', 1)

    def test_no_eviction_returns_none(self):
        """Test that put returns None when no eviction."""
        cache = LRUCache(3)
        evicted = cache.put('a', 1)

        assert evicted is None

    def test_large_cache(self):
        """Test cache with many items."""
        cache = LRUCache(1000)
        for i in range(1000):
            cache.put(i, i * 10)

        assert len(cache) == 1000
        assert cache.get(999) == 9990

        cache.put(1000, 10000)  # Evicts key 0
        assert cache.get(0) is None
