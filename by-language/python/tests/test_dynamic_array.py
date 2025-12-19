"""
Tests for Dynamic Array implementation.
"""

import pytest
from data_structures.dynamic_array import DynamicArray


class TestDynamicArrayBasics:
    """Test basic operations of DynamicArray."""

    def test_init_default(self):
        """Test default initialization."""
        arr = DynamicArray()
        assert len(arr) == 0
        assert arr.capacity == DynamicArray.DEFAULT_CAPACITY
        assert arr.is_empty

    def test_init_custom_capacity(self):
        """Test initialization with custom capacity."""
        arr = DynamicArray(16)
        assert arr.capacity == 16

    def test_init_invalid_capacity(self):
        """Test initialization with invalid capacity raises error."""
        with pytest.raises(ValueError):
            DynamicArray(0)
        with pytest.raises(ValueError):
            DynamicArray(-1)

    def test_append_single(self):
        """Test appending a single element."""
        arr = DynamicArray()
        arr.append(42)
        assert len(arr) == 1
        assert arr[0] == 42
        assert not arr.is_empty

    def test_append_multiple(self):
        """Test appending multiple elements."""
        arr = DynamicArray()
        for i in range(5):
            arr.append(i * 10)

        assert len(arr) == 5
        assert arr[0] == 0
        assert arr[4] == 40


class TestDynamicArrayAccess:
    """Test access operations of DynamicArray."""

    def test_getitem_positive_index(self):
        """Test getting items with positive index."""
        arr = DynamicArray.from_list([10, 20, 30])
        assert arr[0] == 10
        assert arr[1] == 20
        assert arr[2] == 30

    def test_getitem_negative_index(self):
        """Test getting items with negative index."""
        arr = DynamicArray.from_list([10, 20, 30])
        assert arr[-1] == 30
        assert arr[-2] == 20
        assert arr[-3] == 10

    def test_getitem_out_of_bounds(self):
        """Test getting items with out of bounds index."""
        arr = DynamicArray.from_list([10, 20, 30])
        with pytest.raises(IndexError):
            _ = arr[3]
        with pytest.raises(IndexError):
            _ = arr[-4]

    def test_setitem(self):
        """Test setting items."""
        arr = DynamicArray.from_list([10, 20, 30])
        arr[1] = 99
        assert arr[1] == 99

    def test_setitem_negative_index(self):
        """Test setting items with negative index."""
        arr = DynamicArray.from_list([10, 20, 30])
        arr[-1] = 99
        assert arr[2] == 99


class TestDynamicArrayInsertDelete:
    """Test insert and delete operations."""

    def test_insert_at_beginning(self):
        """Test inserting at the beginning."""
        arr = DynamicArray.from_list([20, 30])
        arr.insert(0, 10)
        assert arr.to_list() == [10, 20, 30]

    def test_insert_at_middle(self):
        """Test inserting in the middle."""
        arr = DynamicArray.from_list([10, 30])
        arr.insert(1, 20)
        assert arr.to_list() == [10, 20, 30]

    def test_insert_at_end(self):
        """Test inserting at the end."""
        arr = DynamicArray.from_list([10, 20])
        arr.insert(2, 30)
        assert arr.to_list() == [10, 20, 30]

    def test_pop_default(self):
        """Test popping last element."""
        arr = DynamicArray.from_list([10, 20, 30])
        value = arr.pop()
        assert value == 30
        assert arr.to_list() == [10, 20]

    def test_pop_at_index(self):
        """Test popping at specific index."""
        arr = DynamicArray.from_list([10, 20, 30])
        value = arr.pop(1)
        assert value == 20
        assert arr.to_list() == [10, 30]

    def test_pop_empty_raises(self):
        """Test popping from empty array raises error."""
        arr = DynamicArray()
        with pytest.raises(IndexError):
            arr.pop()

    def test_remove_existing(self):
        """Test removing existing element."""
        arr = DynamicArray.from_list([10, 20, 30, 20])
        result = arr.remove(20)
        assert result is True
        assert arr.to_list() == [10, 30, 20]  # Only first 20 removed

    def test_remove_nonexistent(self):
        """Test removing non-existent element."""
        arr = DynamicArray.from_list([10, 20, 30])
        result = arr.remove(99)
        assert result is False
        assert len(arr) == 3


class TestDynamicArrayResize:
    """Test automatic resizing behavior."""

    def test_grow_on_append(self):
        """Test array grows when capacity is reached."""
        arr = DynamicArray(2)
        arr.append(1)
        arr.append(2)
        assert arr.capacity == 2

        arr.append(3)  # Should trigger resize
        assert arr.capacity == 4
        assert arr.to_list() == [1, 2, 3]

    def test_shrink_on_pop(self):
        """Test array shrinks when utilization is low."""
        arr = DynamicArray(4)
        for i in range(4):
            arr.append(i)

        # Pop until shrink threshold
        arr.pop()
        arr.pop()
        arr.pop()  # size=1, capacity=4 (25%)

        # Capacity should shrink but not below DEFAULT_CAPACITY
        assert arr.capacity <= 4


class TestDynamicArraySearch:
    """Test search operations."""

    def test_find_existing(self):
        """Test finding existing element."""
        arr = DynamicArray.from_list([10, 20, 30, 20])
        assert arr.find(20) == 1  # First occurrence

    def test_find_nonexistent(self):
        """Test finding non-existent element."""
        arr = DynamicArray.from_list([10, 20, 30])
        assert arr.find(99) == -1

    def test_contains(self):
        """Test __contains__ method."""
        arr = DynamicArray.from_list([10, 20, 30])
        assert 20 in arr
        assert 99 not in arr


class TestDynamicArrayUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing the array."""
        arr = DynamicArray.from_list([10, 20, 30])
        arr.clear()
        assert len(arr) == 0
        assert arr.is_empty
        assert arr.capacity == DynamicArray.DEFAULT_CAPACITY

    def test_reverse(self):
        """Test reversing the array."""
        arr = DynamicArray.from_list([10, 20, 30, 40])
        arr.reverse()
        assert arr.to_list() == [40, 30, 20, 10]

    def test_reverse_empty(self):
        """Test reversing empty array."""
        arr = DynamicArray()
        arr.reverse()
        assert arr.to_list() == []

    def test_reverse_single(self):
        """Test reversing single element array."""
        arr = DynamicArray.from_list([42])
        arr.reverse()
        assert arr.to_list() == [42]

    def test_iter(self):
        """Test iteration."""
        arr = DynamicArray.from_list([10, 20, 30])
        result = [x for x in arr]
        assert result == [10, 20, 30]

    def test_repr(self):
        """Test string representation."""
        arr = DynamicArray.from_list([1, 2, 3])
        assert repr(arr) == "DynamicArray([1, 2, 3])"

    def test_equality(self):
        """Test equality comparison."""
        arr1 = DynamicArray.from_list([1, 2, 3])
        arr2 = DynamicArray.from_list([1, 2, 3])
        arr3 = DynamicArray.from_list([1, 2, 4])

        assert arr1 == arr2
        assert arr1 != arr3

    def test_from_list(self):
        """Test creating from list."""
        arr = DynamicArray.from_list([10, 20, 30])
        assert arr.to_list() == [10, 20, 30]

    def test_to_list(self):
        """Test converting to list."""
        arr = DynamicArray()
        arr.append(10)
        arr.append(20)
        assert arr.to_list() == [10, 20]


class TestDynamicArrayEdgeCases:
    """Test edge cases."""

    def test_large_array(self):
        """Test with large number of elements."""
        arr = DynamicArray()
        n = 10000

        for i in range(n):
            arr.append(i)

        assert len(arr) == n
        assert arr[0] == 0
        assert arr[n - 1] == n - 1

    def test_mixed_types(self):
        """Test with mixed types (when not using generics strictly)."""
        arr: DynamicArray = DynamicArray()
        arr.append(1)
        arr.append("two")
        arr.append(3.0)

        assert arr.to_list() == [1, "two", 3.0]

    def test_none_values(self):
        """Test storing None values."""
        arr: DynamicArray = DynamicArray()
        arr.append(None)
        arr.append(1)
        arr.append(None)

        assert arr.to_list() == [None, 1, None]
        assert arr.find(None) == 0
