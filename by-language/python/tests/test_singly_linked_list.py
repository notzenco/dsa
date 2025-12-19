"""
Tests for Singly Linked List implementation.
"""

import pytest
from data_structures.singly_linked_list import SinglyLinkedList, Node


class TestSinglyLinkedListBasics:
    """Test basic operations."""

    def test_init(self):
        """Test initialization."""
        ll = SinglyLinkedList()
        assert len(ll) == 0
        assert ll.is_empty
        assert ll.head is None
        assert ll.tail is None

    def test_append(self):
        """Test appending elements."""
        ll = SinglyLinkedList()
        ll.append(10)
        ll.append(20)
        ll.append(30)

        assert len(ll) == 3
        assert ll.head == 10
        assert ll.tail == 30
        assert ll.to_list() == [10, 20, 30]

    def test_prepend(self):
        """Test prepending elements."""
        ll = SinglyLinkedList()
        ll.prepend(30)
        ll.prepend(20)
        ll.prepend(10)

        assert ll.to_list() == [10, 20, 30]
        assert ll.head == 10
        assert ll.tail == 30


class TestSinglyLinkedListAccess:
    """Test access operations."""

    def test_getitem(self):
        """Test getting items by index."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        assert ll[0] == 10
        assert ll[1] == 20
        assert ll[2] == 30

    def test_getitem_negative(self):
        """Test getting items with negative index."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        assert ll[-1] == 30
        assert ll[-2] == 20

    def test_getitem_out_of_bounds(self):
        """Test index out of bounds."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        with pytest.raises(IndexError):
            _ = ll[3]

    def test_iter(self):
        """Test iteration."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        result = list(ll)
        assert result == [10, 20, 30]


class TestSinglyLinkedListInsert:
    """Test insertion operations."""

    def test_insert_at_beginning(self):
        """Test inserting at beginning."""
        ll = SinglyLinkedList.from_list([20, 30])
        ll.insert(0, 10)
        assert ll.to_list() == [10, 20, 30]
        assert ll.head == 10

    def test_insert_at_middle(self):
        """Test inserting in middle."""
        ll = SinglyLinkedList.from_list([10, 30])
        ll.insert(1, 20)
        assert ll.to_list() == [10, 20, 30]

    def test_insert_at_end(self):
        """Test inserting at end."""
        ll = SinglyLinkedList.from_list([10, 20])
        ll.insert(2, 30)
        assert ll.to_list() == [10, 20, 30]
        assert ll.tail == 30


class TestSinglyLinkedListDelete:
    """Test deletion operations."""

    def test_pop_first(self):
        """Test popping first element."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        value = ll.pop_first()
        assert value == 10
        assert ll.to_list() == [20, 30]
        assert ll.head == 20

    def test_pop_last(self):
        """Test popping last element."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        value = ll.pop_last()
        assert value == 30
        assert ll.to_list() == [10, 20]
        assert ll.tail == 20

    def test_pop_single_element(self):
        """Test popping from single element list."""
        ll = SinglyLinkedList.from_list([42])
        value = ll.pop_first()
        assert value == 42
        assert ll.is_empty
        assert ll.head is None
        assert ll.tail is None

    def test_pop_at_index(self):
        """Test popping at specific index."""
        ll = SinglyLinkedList.from_list([10, 20, 30, 40])
        value = ll.pop(1)
        assert value == 20
        assert ll.to_list() == [10, 30, 40]

    def test_pop_empty_raises(self):
        """Test popping from empty list raises error."""
        ll = SinglyLinkedList()
        with pytest.raises(IndexError):
            ll.pop_first()

    def test_remove_existing(self):
        """Test removing existing value."""
        ll = SinglyLinkedList.from_list([10, 20, 30, 20])
        result = ll.remove(20)
        assert result is True
        assert ll.to_list() == [10, 30, 20]

    def test_remove_head(self):
        """Test removing head value."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        result = ll.remove(10)
        assert result is True
        assert ll.head == 20

    def test_remove_tail(self):
        """Test removing tail value."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        result = ll.remove(30)
        assert result is True
        assert ll.tail == 20

    def test_remove_nonexistent(self):
        """Test removing non-existent value."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        result = ll.remove(99)
        assert result is False
        assert len(ll) == 3


class TestSinglyLinkedListSearch:
    """Test search operations."""

    def test_find_existing(self):
        """Test finding existing element."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        assert ll.find(20) == 1

    def test_find_nonexistent(self):
        """Test finding non-existent element."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        assert ll.find(99) == -1

    def test_contains(self):
        """Test __contains__ method."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        assert 20 in ll
        assert 99 not in ll


class TestSinglyLinkedListUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing the list."""
        ll = SinglyLinkedList.from_list([10, 20, 30])
        ll.clear()
        assert ll.is_empty
        assert ll.head is None
        assert ll.tail is None

    def test_reverse(self):
        """Test reversing the list."""
        ll = SinglyLinkedList.from_list([10, 20, 30, 40])
        ll.reverse()
        assert ll.to_list() == [40, 30, 20, 10]
        assert ll.head == 40
        assert ll.tail == 10

    def test_reverse_empty(self):
        """Test reversing empty list."""
        ll = SinglyLinkedList()
        ll.reverse()
        assert ll.to_list() == []

    def test_reverse_single(self):
        """Test reversing single element."""
        ll = SinglyLinkedList.from_list([42])
        ll.reverse()
        assert ll.to_list() == [42]

    def test_repr(self):
        """Test string representation."""
        ll = SinglyLinkedList.from_list([1, 2, 3])
        assert "1 -> 2 -> 3" in repr(ll)

    def test_equality(self):
        """Test equality comparison."""
        ll1 = SinglyLinkedList.from_list([1, 2, 3])
        ll2 = SinglyLinkedList.from_list([1, 2, 3])
        ll3 = SinglyLinkedList.from_list([1, 2, 4])

        assert ll1 == ll2
        assert ll1 != ll3


class TestSinglyLinkedListAdvanced:
    """Test advanced operations."""

    def test_get_middle_odd(self):
        """Test getting middle element (odd length)."""
        ll = SinglyLinkedList.from_list([1, 2, 3, 4, 5])
        assert ll.get_middle() == 3

    def test_get_middle_even(self):
        """Test getting middle element (even length)."""
        ll = SinglyLinkedList.from_list([1, 2, 3, 4])
        assert ll.get_middle() == 2

    def test_get_middle_empty(self):
        """Test getting middle of empty list."""
        ll = SinglyLinkedList()
        assert ll.get_middle() is None

    def test_get_nth_from_end(self):
        """Test getting nth element from end."""
        ll = SinglyLinkedList.from_list([1, 2, 3, 4, 5])
        assert ll.get_nth_from_end(1) == 5  # Last
        assert ll.get_nth_from_end(2) == 4  # Second last
        assert ll.get_nth_from_end(5) == 1  # First

    def test_get_nth_from_end_invalid(self):
        """Test getting nth from end with invalid n."""
        ll = SinglyLinkedList.from_list([1, 2, 3])
        assert ll.get_nth_from_end(0) is None
        assert ll.get_nth_from_end(4) is None

    def test_remove_duplicates(self):
        """Test removing duplicates."""
        ll = SinglyLinkedList.from_list([1, 2, 2, 3, 3, 3, 4])
        ll.remove_duplicates()
        assert ll.to_list() == [1, 2, 3, 4]

    def test_remove_duplicates_all_same(self):
        """Test removing duplicates when all same."""
        ll = SinglyLinkedList.from_list([5, 5, 5, 5])
        ll.remove_duplicates()
        assert ll.to_list() == [5]

    def test_map(self):
        """Test map operation."""
        ll = SinglyLinkedList.from_list([1, 2, 3])
        result = ll.map(lambda x: x * 2)
        assert result.to_list() == [2, 4, 6]

    def test_filter(self):
        """Test filter operation."""
        ll = SinglyLinkedList.from_list([1, 2, 3, 4, 5])
        result = ll.filter(lambda x: x % 2 == 0)
        assert result.to_list() == [2, 4]


class TestNodeClass:
    """Test the Node class."""

    def test_node_creation(self):
        """Test creating a node."""
        node = Node(42)
        assert node.data == 42
        assert node.next is None

    def test_node_with_next(self):
        """Test creating a node with next pointer."""
        node2 = Node(2)
        node1 = Node(1, node2)
        assert node1.next == node2

    def test_node_repr(self):
        """Test node string representation."""
        node = Node(42)
        assert repr(node) == "Node(42)"
