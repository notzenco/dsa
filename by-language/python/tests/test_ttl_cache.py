"""Tests for TTL Cache implementation."""

import time
from data_structures.ttl_cache import TTLCache


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


