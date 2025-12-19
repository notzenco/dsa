"""
Tests for Hash Table implementations.
"""

import pytest
from data_structures.hash_table import (
    HashTableChaining, HashTableOpenAddressing, HashMap, HashSet
)


class TestHashTableChainingBasics:
    """Test basic operations for chaining hash table."""

    def test_init(self):
        """Test initialization."""
        ht = HashTableChaining()
        assert len(ht) == 0
        assert ht.is_empty
        assert ht.capacity == HashTableChaining.DEFAULT_CAPACITY

    def test_put_and_get(self):
        """Test basic put and get."""
        ht = HashTableChaining()
        ht.put("name", "Alice")
        ht.put("age", 30)

        assert ht.get("name") == "Alice"
        assert ht.get("age") == 30
        assert len(ht) == 2

    def test_update_existing(self):
        """Test updating existing key."""
        ht = HashTableChaining()
        ht.put("key", "value1")
        ht.put("key", "value2")

        assert ht.get("key") == "value2"
        assert len(ht) == 1

    def test_get_nonexistent(self):
        """Test getting non-existent key."""
        ht = HashTableChaining()
        assert ht.get("missing") is None
        assert ht.get("missing", "default") == "default"

    def test_remove(self):
        """Test removing keys."""
        ht = HashTableChaining()
        ht.put("a", 1)
        ht.put("b", 2)

        assert ht.remove("a") is True
        assert "a" not in ht
        assert len(ht) == 1

        assert ht.remove("missing") is False

    def test_contains(self):
        """Test __contains__."""
        ht = HashTableChaining()
        ht.put("key", "value")

        assert "key" in ht
        assert "missing" not in ht

    def test_bracket_access(self):
        """Test bracket notation."""
        ht = HashTableChaining()
        ht["key"] = "value"

        assert ht["key"] == "value"

        with pytest.raises(KeyError):
            _ = ht["missing"]

    def test_del_item(self):
        """Test del operator."""
        ht = HashTableChaining()
        ht["key"] = "value"
        del ht["key"]

        assert "key" not in ht

        with pytest.raises(KeyError):
            del ht["missing"]


class TestHashTableChainingIteration:
    """Test iteration methods."""

    def test_keys(self):
        """Test keys iteration."""
        ht = HashTableChaining()
        ht.put("a", 1)
        ht.put("b", 2)
        ht.put("c", 3)

        keys = set(ht.keys())
        assert keys == {"a", "b", "c"}

    def test_values(self):
        """Test values iteration."""
        ht = HashTableChaining()
        ht.put("a", 1)
        ht.put("b", 2)
        ht.put("c", 3)

        values = set(ht.values())
        assert values == {1, 2, 3}

    def test_items(self):
        """Test items iteration."""
        ht = HashTableChaining()
        ht.put("a", 1)
        ht.put("b", 2)

        items = set(ht.items())
        assert items == {("a", 1), ("b", 2)}

    def test_iter(self):
        """Test __iter__."""
        ht = HashTableChaining()
        ht.put("x", 1)
        ht.put("y", 2)

        keys = set(ht)
        assert keys == {"x", "y"}


class TestHashTableChainingResize:
    """Test resizing behavior."""

    def test_resize_on_load_factor(self):
        """Test automatic resizing."""
        ht = HashTableChaining(capacity=4)
        initial_capacity = ht.capacity

        # Add enough items to trigger resize
        for i in range(10):
            ht.put(f"key{i}", i)

        assert ht.capacity > initial_capacity
        assert len(ht) == 10

        # Verify all items still accessible
        for i in range(10):
            assert ht.get(f"key{i}") == i

    def test_load_factor(self):
        """Test load factor calculation."""
        ht = HashTableChaining(capacity=10)
        assert ht.load_factor == 0.0

        ht.put("a", 1)
        assert ht.load_factor == 0.1


class TestHashTableOpenAddressing:
    """Test open addressing implementation."""

    def test_init(self):
        """Test initialization."""
        ht = HashTableOpenAddressing()
        assert len(ht) == 0
        assert ht.is_empty

    def test_put_and_get(self):
        """Test basic put and get."""
        ht = HashTableOpenAddressing()
        ht.put("name", "Bob")
        ht.put("age", 25)

        assert ht.get("name") == "Bob"
        assert ht.get("age") == 25

    def test_collision_handling(self):
        """Test that collisions are handled."""
        ht = HashTableOpenAddressing(capacity=4)

        # Add multiple items (will cause collisions with small capacity)
        ht.put("a", 1)
        ht.put("b", 2)
        ht.put("c", 3)
        ht.put("d", 4)

        assert ht.get("a") == 1
        assert ht.get("b") == 2
        assert ht.get("c") == 3
        assert ht.get("d") == 4

    def test_remove_with_tombstone(self):
        """Test removal with tombstones."""
        ht = HashTableOpenAddressing()
        ht.put("a", 1)
        ht.put("b", 2)
        ht.put("c", 3)

        # Remove middle element
        assert ht.remove("b") is True
        assert "b" not in ht

        # Other elements still accessible
        assert ht.get("a") == 1
        assert ht.get("c") == 3

    def test_update_existing(self):
        """Test updating existing key."""
        ht = HashTableOpenAddressing()
        ht.put("key", "old")
        ht.put("key", "new")

        assert ht.get("key") == "new"
        assert len(ht) == 1

    def test_resize(self):
        """Test automatic resizing."""
        ht = HashTableOpenAddressing(capacity=4)

        for i in range(10):
            ht.put(f"key{i}", i)

        assert len(ht) == 10
        for i in range(10):
            assert ht.get(f"key{i}") == i


class TestHashSet:
    """Test HashSet implementation."""

    def test_init(self):
        """Test initialization."""
        s = HashSet()
        assert len(s) == 0
        assert s.is_empty

    def test_add_and_contains(self):
        """Test add and contains."""
        s = HashSet()
        s.add(1)
        s.add(2)
        s.add(3)

        assert 1 in s
        assert 2 in s
        assert 4 not in s

    def test_add_duplicates(self):
        """Test adding duplicates."""
        s = HashSet()
        s.add(1)
        s.add(1)
        s.add(1)

        assert len(s) == 1

    def test_remove(self):
        """Test remove."""
        s = HashSet()
        s.add(1)
        s.add(2)

        assert s.remove(1) is True
        assert 1 not in s
        assert s.remove(99) is False

    def test_discard(self):
        """Test discard (no error)."""
        s = HashSet()
        s.add(1)

        s.discard(1)
        assert 1 not in s

        # Should not raise
        s.discard(99)

    def test_union(self):
        """Test union operation."""
        s1 = HashSet.from_list([1, 2, 3])
        s2 = HashSet.from_list([3, 4, 5])

        result = s1.union(s2)
        assert set(result.to_list()) == {1, 2, 3, 4, 5}

    def test_intersection(self):
        """Test intersection operation."""
        s1 = HashSet.from_list([1, 2, 3, 4])
        s2 = HashSet.from_list([3, 4, 5, 6])

        result = s1.intersection(s2)
        assert set(result.to_list()) == {3, 4}

    def test_difference(self):
        """Test difference operation."""
        s1 = HashSet.from_list([1, 2, 3, 4])
        s2 = HashSet.from_list([3, 4, 5])

        result = s1.difference(s2)
        assert set(result.to_list()) == {1, 2}

    def test_from_list(self):
        """Test creating from list."""
        s = HashSet.from_list([1, 2, 2, 3, 3, 3])
        assert len(s) == 3

    def test_clear(self):
        """Test clear."""
        s = HashSet.from_list([1, 2, 3])
        s.clear()
        assert s.is_empty


class TestHashMapAlias:
    """Test HashMap alias."""

    def test_hashmap_is_chaining(self):
        """Test that HashMap is an alias for HashTableChaining."""
        assert HashMap is HashTableChaining

    def test_hashmap_works(self):
        """Test HashMap basic usage."""
        m = HashMap()
        m["key"] = "value"
        assert m["key"] == "value"


class TestHashTableEdgeCases:
    """Test edge cases."""

    def test_none_as_key(self):
        """Test None as key."""
        ht = HashTableChaining()
        ht.put(None, "value")
        assert ht.get(None) == "value"

    def test_none_as_value(self):
        """Test None as value."""
        ht = HashTableChaining()
        ht.put("key", None)
        assert ht.get("key") is None
        assert "key" in ht

    def test_various_key_types(self):
        """Test various key types."""
        ht = HashTableChaining()
        ht.put(1, "int")
        ht.put("str", "string")
        ht.put((1, 2), "tuple")
        ht.put(3.14, "float")

        assert ht.get(1) == "int"
        assert ht.get("str") == "string"
        assert ht.get((1, 2)) == "tuple"
        assert ht.get(3.14) == "float"

    def test_large_number_of_items(self):
        """Test with many items."""
        ht = HashTableChaining()

        for i in range(1000):
            ht.put(f"key_{i}", i)

        assert len(ht) == 1000

        for i in range(1000):
            assert ht.get(f"key_{i}") == i

    def test_clear(self):
        """Test clear method."""
        ht = HashTableChaining()
        ht.put("a", 1)
        ht.put("b", 2)

        ht.clear()
        assert ht.is_empty
        assert "a" not in ht

    def test_repr(self):
        """Test string representation."""
        ht = HashTableChaining()
        ht.put("a", 1)
        s = repr(ht)
        assert "a" in s
        assert "1" in s
