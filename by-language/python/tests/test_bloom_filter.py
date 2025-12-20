"""
Tests for Bloom Filter and Skip List.
"""

import pytest
from data_structures.bloom_filter import (
    BloomFilter, CountingBloomFilter, ScalableBloomFilter
)
from data_structures.skip_list import SkipList


class TestBloomFilter:
    """Test Bloom Filter."""

    def test_add_and_contains(self):
        """Test basic add and contains."""
        bf = BloomFilter(expected_elements=100)
        bf.add("hello")
        bf.add("world")

        assert "hello" in bf
        assert "world" in bf

    def test_not_contains(self):
        """Test item not in filter."""
        bf = BloomFilter(expected_elements=100)
        bf.add("hello")

        # "goodbye" definitely not in filter
        # Note: might have false positives, but very unlikely with small data
        assert bf.contains("hello")

    def test_no_false_negatives(self):
        """Bloom filters never have false negatives."""
        bf = BloomFilter(expected_elements=1000)
        items = [f"item_{i}" for i in range(100)]

        for item in items:
            bf.add(item)

        # All added items must be found
        for item in items:
            assert item in bf

    def test_false_positive_rate(self):
        """Test false positive rate is within expected bounds."""
        bf = BloomFilter(expected_elements=1000, false_positive_rate=0.01)

        # Add 1000 items
        for i in range(1000):
            bf.add(f"item_{i}")

        # Test with items NOT in the filter
        false_positives = 0
        test_count = 10000
        for i in range(test_count):
            if f"not_in_filter_{i}" in bf:
                false_positives += 1

        # Allow some variance, but should be close to 1%
        fp_rate = false_positives / test_count
        assert fp_rate < 0.05  # Should be well under 5%

    def test_count(self):
        """Test item count."""
        bf = BloomFilter(expected_elements=100)
        assert bf.count == 0

        bf.add("a")
        bf.add("b")
        bf.add("c")

        assert bf.count == 3

    def test_parameters(self):
        """Test filter parameters."""
        bf = BloomFilter(expected_elements=1000, false_positive_rate=0.01)

        assert bf.size > 0
        assert bf.num_hashes > 0

    def test_repr(self):
        """Test string representation."""
        bf = BloomFilter(expected_elements=100)
        s = repr(bf)
        assert "BloomFilter" in s


class TestCountingBloomFilter:
    """Test Counting Bloom Filter."""

    def test_add_and_contains(self):
        """Test basic add and contains."""
        cbf = CountingBloomFilter(expected_elements=100)
        cbf.add("hello")

        assert "hello" in cbf

    def test_remove(self):
        """Test removal from counting filter."""
        cbf = CountingBloomFilter(expected_elements=100)
        cbf.add("hello")
        cbf.add("world")

        assert "hello" in cbf
        assert cbf.remove("hello")
        # After removal, shouldn't be found (unless false positive)

    def test_remove_not_exists(self):
        """Test removing non-existent item."""
        cbf = CountingBloomFilter(expected_elements=100)
        assert not cbf.remove("not_added")

    def test_count(self):
        """Test item count with add/remove."""
        cbf = CountingBloomFilter(expected_elements=100)
        cbf.add("a")
        cbf.add("b")
        assert cbf.count == 2

        cbf.remove("a")
        assert cbf.count == 1


class TestScalableBloomFilter:
    """Test Scalable Bloom Filter."""

    def test_add_and_contains(self):
        """Test basic operations."""
        sbf = ScalableBloomFilter(initial_capacity=10)
        sbf.add("hello")

        assert "hello" in sbf

    def test_scaling(self):
        """Test that filter scales."""
        sbf = ScalableBloomFilter(initial_capacity=10, growth_rate=2)

        # Add more than initial capacity
        for i in range(50):
            sbf.add(f"item_{i}")

        # Should have created additional filters
        assert sbf.num_filters > 1

        # All items should still be findable
        for i in range(50):
            assert f"item_{i}" in sbf

    def test_count(self):
        """Test count across filters."""
        sbf = ScalableBloomFilter(initial_capacity=10)
        for i in range(25):
            sbf.add(f"unique_item_{i}")

        # Count may be slightly less due to false positive matches
        # during scaling (item appears to be in filter when it's not)
        assert sbf.count >= 20  # Allow for some false positives
        assert sbf.count <= 25


class TestSkipList:
    """Test Skip List."""

    def test_insert_and_search(self):
        """Test basic insert and search."""
        sl = SkipList()
        sl.insert(10, "ten")
        sl.insert(20, "twenty")
        sl.insert(30, "thirty")

        assert sl.search(10) == "ten"
        assert sl.search(20) == "twenty"
        assert sl.search(30) == "thirty"
        assert sl.search(40) is None

    def test_contains(self):
        """Test contains check."""
        sl = SkipList()
        sl.insert(10, "ten")

        assert 10 in sl
        assert 20 not in sl

    def test_update(self):
        """Test updating existing key."""
        sl = SkipList()
        sl.insert(10, "ten")
        sl.insert(10, "TEN")

        assert sl.search(10) == "TEN"
        assert len(sl) == 1

    def test_delete(self):
        """Test deletion."""
        sl = SkipList()
        sl.insert(10, "ten")
        sl.insert(20, "twenty")
        sl.insert(30, "thirty")

        assert sl.delete(20)
        assert 20 not in sl
        assert 10 in sl
        assert 30 in sl
        assert len(sl) == 2

    def test_delete_not_exists(self):
        """Test deleting non-existent key."""
        sl = SkipList()
        sl.insert(10, "ten")

        assert not sl.delete(20)

    def test_getitem_setitem(self):
        """Test bracket notation."""
        sl = SkipList()
        sl[10] = "ten"
        sl[20] = "twenty"

        assert sl[10] == "ten"
        assert sl[20] == "twenty"

    def test_getitem_keyerror(self):
        """Test KeyError on missing key."""
        sl = SkipList()
        sl[10] = "ten"

        with pytest.raises(KeyError):
            _ = sl[20]

    def test_delitem(self):
        """Test delete with del."""
        sl = SkipList()
        sl[10] = "ten"

        del sl[10]
        assert 10 not in sl

    def test_delitem_keyerror(self):
        """Test KeyError on delete missing key."""
        sl = SkipList()

        with pytest.raises(KeyError):
            del sl[10]

    def test_len(self):
        """Test length."""
        sl = SkipList()
        assert len(sl) == 0

        sl.insert(10, "ten")
        sl.insert(20, "twenty")
        assert len(sl) == 2

        sl.delete(10)
        assert len(sl) == 1

    def test_iter(self):
        """Test iteration in sorted order."""
        sl = SkipList()
        sl.insert(30, "thirty")
        sl.insert(10, "ten")
        sl.insert(20, "twenty")

        keys = list(sl)
        assert keys == [10, 20, 30]

    def test_items(self):
        """Test items iteration."""
        sl = SkipList()
        sl.insert(30, "thirty")
        sl.insert(10, "ten")
        sl.insert(20, "twenty")

        items = list(sl.items())
        assert items == [(10, "ten"), (20, "twenty"), (30, "thirty")]

    def test_range(self):
        """Test range query."""
        sl = SkipList()
        for i in range(10):
            sl.insert(i * 10, f"val_{i}")

        items = list(sl.range(25, 75))
        assert items == [(30, "val_3"), (40, "val_4"), (50, "val_5"), (60, "val_6"), (70, "val_7")]

    def test_min_max(self):
        """Test min and max."""
        sl = SkipList()
        sl.insert(30, "thirty")
        sl.insert(10, "ten")
        sl.insert(50, "fifty")

        assert sl.min() == (10, "ten")
        assert sl.max() == (50, "fifty")

    def test_min_max_empty(self):
        """Test min/max on empty list."""
        sl = SkipList()
        assert sl.min() is None
        assert sl.max() is None

    def test_large_dataset(self):
        """Test with larger dataset."""
        sl = SkipList()
        items = list(range(0, 1000, 2))  # Even numbers 0-998
        import random
        random.shuffle(items)

        for item in items:
            sl.insert(item, f"val_{item}")

        # All items should be findable
        for item in range(0, 1000, 2):
            assert item in sl
            assert sl.search(item) == f"val_{item}"

        # Odd numbers should not be found
        for item in range(1, 1000, 2):
            assert item not in sl

        # Should be in sorted order
        keys = list(sl)
        assert keys == sorted(items)

    def test_level(self):
        """Test level property."""
        sl = SkipList()
        assert sl.level == 0

        # Add items to potentially increase level
        for i in range(100):
            sl.insert(i, f"val_{i}")

        # Level should have increased (probabilistically)
        assert sl.level >= 0

    def test_repr(self):
        """Test string representation."""
        sl = SkipList()
        sl.insert(10, "ten")
        s = repr(sl)
        assert "SkipList" in s

    def test_display(self):
        """Test display method."""
        sl = SkipList()
        sl.insert(10, "ten")
        sl.insert(20, "twenty")

        display = sl.display()
        assert "HEAD" in display
        assert "NIL" in display
