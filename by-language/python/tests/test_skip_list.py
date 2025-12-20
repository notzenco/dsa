"""
Tests for Skip List implementation.
"""

import pytest
from data_structures.skip_list import SkipList


class TestSkipListBasics:
    """Test basic skip list operations."""

    def test_init(self):
        """Test skip list initialization."""
        sl = SkipList()
        assert len(sl) == 0
        assert sl.size == 0
        assert sl.is_empty()

    def test_init_custom_probability(self):
        """Test initialization with custom probability."""
        sl = SkipList(probability=0.25)
        assert sl.is_empty()

    def test_init_invalid_probability(self):
        """Test invalid probability is corrected."""
        sl = SkipList(probability=0)
        sl.insert(1, 'a')
        assert sl.search(1) == 'a'

    def test_insert_and_search(self):
        """Test basic insert and search."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert sl.search(1) == 'a'
        assert sl.search(2) == 'b'
        assert sl.search(3) == 'c'
        assert sl.size == 3

    def test_search_nonexistent(self):
        """Test searching for nonexistent key."""
        sl = SkipList()
        sl.insert(1, 'a')
        assert sl.search(2) is None

    def test_update_existing(self):
        """Test updating existing key."""
        sl = SkipList()
        assert sl.insert(1, 'a') is True  # New key
        assert sl.insert(1, 'b') is False  # Update
        assert sl.search(1) == 'b'
        assert sl.size == 1

    def test_contains(self):
        """Test containment check."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert 1 in sl
        assert 2 in sl
        assert 3 not in sl

    def test_get_with_default(self):
        """Test get with default value."""
        sl = SkipList()
        sl.insert(1, 'a')

        assert sl.get(1) == 'a'
        assert sl.get(2) is None
        assert sl.get(2, 'default') == 'default'


class TestSkipListDelete:
    """Test delete operations."""

    def test_delete_existing(self):
        """Test deleting existing key."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(2, 'b')
        sl.insert(3, 'c')

        assert sl.delete(2) is True
        assert sl.search(2) is None
        assert sl.size == 2
        assert sl.search(1) == 'a'
        assert sl.search(3) == 'c'

    def test_delete_nonexistent(self):
        """Test deleting nonexistent key."""
        sl = SkipList()
        sl.insert(1, 'a')

        assert sl.delete(2) is False
        assert sl.size == 1

    def test_delete_all(self):
        """Test deleting all elements."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(2, 'b')
        sl.insert(3, 'c')

        sl.delete(1)
        sl.delete(2)
        sl.delete(3)

        assert sl.is_empty()
        assert sl.size == 0


class TestSkipListRange:
    """Test range operations."""

    def test_range_basic(self):
        """Test basic range query."""
        sl = SkipList()
        for i in range(10):
            sl.insert(i, str(i))

        result = sl.range(3, 7)
        assert result == [(3, '3'), (4, '4'), (5, '5'), (6, '6'), (7, '7')]

    def test_range_empty_result(self):
        """Test range with no elements in range."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(10, 'b')

        result = sl.range(5, 8)
        assert result == []

    def test_range_invalid(self):
        """Test range with min > max."""
        sl = SkipList()
        sl.insert(1, 'a')

        result = sl.range(10, 5)
        assert result == []

    def test_ceiling(self):
        """Test ceiling operation."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(3, 'c')
        sl.insert(5, 'e')

        assert sl.ceiling(2) == 3
        assert sl.ceiling(3) == 3
        assert sl.ceiling(0) == 1
        assert sl.ceiling(6) is None

    def test_floor(self):
        """Test floor operation."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(3, 'c')
        sl.insert(5, 'e')

        assert sl.floor(4) == 3
        assert sl.floor(3) == 3
        assert sl.floor(6) == 5
        assert sl.floor(0) is None


class TestSkipListMinMax:
    """Test min/max operations."""

    def test_min(self):
        """Test getting minimum."""
        sl = SkipList()
        sl.insert(5, 'e')
        sl.insert(2, 'b')
        sl.insert(8, 'h')

        result = sl.min()
        assert result == (2, 'b')

    def test_max(self):
        """Test getting maximum."""
        sl = SkipList()
        sl.insert(5, 'e')
        sl.insert(2, 'b')
        sl.insert(8, 'h')

        result = sl.max()
        assert result == (8, 'h')

    def test_min_empty(self):
        """Test min on empty list."""
        sl = SkipList()
        assert sl.min() is None

    def test_max_empty(self):
        """Test max on empty list."""
        sl = SkipList()
        assert sl.max() is None


class TestSkipListIteration:
    """Test iteration operations."""

    def test_keys(self):
        """Test iterating over keys."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert list(sl.keys()) == [1, 2, 3]

    def test_values(self):
        """Test iterating over values."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert list(sl.values()) == ['a', 'b', 'c']

    def test_items(self):
        """Test iterating over items."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert list(sl.items()) == [(1, 'a'), (2, 'b'), (3, 'c')]

    def test_iter(self):
        """Test __iter__."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')
        sl.insert(2, 'b')

        assert list(sl) == [1, 2, 3]

    def test_to_list(self):
        """Test to_list conversion."""
        sl = SkipList()
        sl.insert(3, 'c')
        sl.insert(1, 'a')

        assert sl.to_list() == [(1, 'a'), (3, 'c')]


class TestSkipListClear:
    """Test clear operation."""

    def test_clear(self):
        """Test clearing the skip list."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(2, 'b')
        sl.insert(3, 'c')

        sl.clear()

        assert sl.is_empty()
        assert sl.size == 0
        assert sl.search(1) is None

    def test_reuse_after_clear(self):
        """Test that skip list can be reused after clear."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.clear()
        sl.insert(2, 'b')

        assert sl.size == 1
        assert sl.search(2) == 'b'


class TestSkipListEdgeCases:
    """Test edge cases."""

    def test_single_element(self):
        """Test with single element."""
        sl = SkipList()
        sl.insert(1, 'a')

        assert sl.size == 1
        assert sl.search(1) == 'a'
        assert sl.min() == (1, 'a')
        assert sl.max() == (1, 'a')

    def test_duplicate_inserts(self):
        """Test multiple inserts of same key."""
        sl = SkipList()
        sl.insert(1, 'a')
        sl.insert(1, 'b')
        sl.insert(1, 'c')

        assert sl.size == 1
        assert sl.search(1) == 'c'

    def test_ordered_insert(self):
        """Test inserting in order."""
        sl = SkipList()
        for i in range(100):
            sl.insert(i, str(i))

        assert sl.size == 100
        assert sl.min() == (0, '0')
        assert sl.max() == (99, '99')

    def test_reverse_ordered_insert(self):
        """Test inserting in reverse order."""
        sl = SkipList()
        for i in range(99, -1, -1):
            sl.insert(i, str(i))

        assert sl.size == 100
        assert sl.min() == (0, '0')
        assert sl.max() == (99, '99')

    def test_string_keys(self):
        """Test with string keys."""
        sl: SkipList[str, int] = SkipList()
        sl.insert('cherry', 3)
        sl.insert('apple', 1)
        sl.insert('banana', 2)

        assert list(sl.keys()) == ['apple', 'banana', 'cherry']
        assert sl.search('banana') == 2

    def test_repr(self):
        """Test string representation."""
        sl = SkipList()
        sl.insert(1, 'a')

        r = repr(sl)
        assert 'SkipList' in r
        assert 'size=1' in r


class TestSkipListStress:
    """Stress tests."""

    def test_many_operations(self):
        """Test many insert/search/delete operations."""
        sl = SkipList()
        n = 1000

        # Insert
        for i in range(n):
            sl.insert(i, i * 2)

        assert sl.size == n

        # Search
        for i in range(n):
            assert sl.search(i) == i * 2

        # Delete half
        for i in range(0, n, 2):
            sl.delete(i)

        assert sl.size == n // 2

        # Verify remaining
        for i in range(n):
            if i % 2 == 0:
                assert sl.search(i) is None
            else:
                assert sl.search(i) == i * 2

    def test_random_operations(self):
        """Test random insert/delete pattern."""
        import random
        random.seed(42)

        sl = SkipList()
        reference = {}

        for _ in range(500):
            op = random.choice(['insert', 'delete', 'search'])
            key = random.randint(0, 100)

            if op == 'insert':
                value = random.randint(0, 1000)
                sl.insert(key, value)
                reference[key] = value
            elif op == 'delete':
                sl.delete(key)
                reference.pop(key, None)
            else:
                result = sl.search(key)
                expected = reference.get(key)
                assert result == expected

        # Final verification
        assert sl.size == len(reference)
        for key, value in reference.items():
            assert sl.search(key) == value
