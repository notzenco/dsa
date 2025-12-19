"""
Tests for Doubly Linked List implementation.
"""

import pytest
from data_structures.doubly_linked_list import DoublyLinkedList, DNode


class TestDoublyLinkedListBasics:
    """Test basic operations."""

    def test_init(self):
        """Test initialization."""
        dll = DoublyLinkedList()
        assert len(dll) == 0
        assert dll.is_empty
        assert dll.head is None
        assert dll.tail is None

    def test_append(self):
        """Test appending elements."""
        dll = DoublyLinkedList()
        dll.append(10)
        dll.append(20)
        dll.append(30)

        assert len(dll) == 3
        assert dll.head == 10
        assert dll.tail == 30
        assert dll.to_list() == [10, 20, 30]

    def test_prepend(self):
        """Test prepending elements."""
        dll = DoublyLinkedList()
        dll.prepend(30)
        dll.prepend(20)
        dll.prepend(10)

        assert dll.to_list() == [10, 20, 30]
        assert dll.head == 10
        assert dll.tail == 30


class TestDoublyLinkedListAccess:
    """Test access operations."""

    def test_getitem(self):
        """Test getting items by index."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert dll[0] == 10
        assert dll[1] == 20
        assert dll[2] == 30

    def test_getitem_negative(self):
        """Test negative indexing."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert dll[-1] == 30
        assert dll[-2] == 20
        assert dll[-3] == 10

    def test_setitem(self):
        """Test setting items."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        dll[1] = 99
        assert dll[1] == 99

    def test_iter_forward(self):
        """Test forward iteration."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert list(dll) == [10, 20, 30]

    def test_iter_backward(self):
        """Test backward iteration."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert list(reversed(dll)) == [30, 20, 10]


class TestDoublyLinkedListInsert:
    """Test insertion operations."""

    def test_insert_at_beginning(self):
        """Test inserting at beginning."""
        dll = DoublyLinkedList.from_list([20, 30])
        dll.insert(0, 10)
        assert dll.to_list() == [10, 20, 30]
        assert dll.head == 10

    def test_insert_at_middle(self):
        """Test inserting in middle."""
        dll = DoublyLinkedList.from_list([10, 30])
        dll.insert(1, 20)
        assert dll.to_list() == [10, 20, 30]

    def test_insert_at_end(self):
        """Test inserting at end."""
        dll = DoublyLinkedList.from_list([10, 20])
        dll.insert(2, 30)
        assert dll.to_list() == [10, 20, 30]
        assert dll.tail == 30

    def test_insert_after_node(self):
        """Test inserting after a specific node."""
        dll = DoublyLinkedList()
        node_a = dll.append(10)
        dll.append(30)

        dll.insert_after(node_a, 20)
        assert dll.to_list() == [10, 20, 30]

    def test_insert_before_node(self):
        """Test inserting before a specific node."""
        dll = DoublyLinkedList()
        dll.append(10)
        node_c = dll.append(30)

        dll.insert_before(node_c, 20)
        assert dll.to_list() == [10, 20, 30]


class TestDoublyLinkedListDelete:
    """Test deletion operations."""

    def test_pop_first(self):
        """Test popping first element."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        value = dll.pop_first()
        assert value == 10
        assert dll.to_list() == [20, 30]
        assert dll.head == 20

    def test_pop_last(self):
        """Test popping last element."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        value = dll.pop_last()
        assert value == 30
        assert dll.to_list() == [10, 20]
        assert dll.tail == 20

    def test_pop_first_single_element(self):
        """Test popping from single element list."""
        dll = DoublyLinkedList.from_list([42])
        value = dll.pop_first()
        assert value == 42
        assert dll.is_empty
        assert dll.head is None
        assert dll.tail is None

    def test_pop_last_single_element(self):
        """Test popping last from single element list."""
        dll = DoublyLinkedList.from_list([42])
        value = dll.pop_last()
        assert value == 42
        assert dll.is_empty

    def test_pop_at_index(self):
        """Test popping at index."""
        dll = DoublyLinkedList.from_list([10, 20, 30, 40])
        value = dll.pop(1)
        assert value == 20
        assert dll.to_list() == [10, 30, 40]

    def test_remove_node(self):
        """Test removing a specific node."""
        dll = DoublyLinkedList()
        dll.append(10)
        node_b = dll.append(20)
        dll.append(30)

        value = dll.remove_node(node_b)
        assert value == 20
        assert dll.to_list() == [10, 30]

    def test_remove_value(self):
        """Test removing by value."""
        dll = DoublyLinkedList.from_list([10, 20, 30, 20])
        result = dll.remove(20)
        assert result is True
        assert dll.to_list() == [10, 30, 20]

    def test_remove_nonexistent(self):
        """Test removing non-existent value."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        result = dll.remove(99)
        assert result is False


class TestDoublyLinkedListSearch:
    """Test search operations."""

    def test_find(self):
        """Test finding element."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert dll.find(20) == 1
        assert dll.find(99) == -1

    def test_find_node(self):
        """Test finding node."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        node = dll.find_node(20)
        assert node is not None
        assert node.data == 20

    def test_contains(self):
        """Test contains."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert 20 in dll
        assert 99 not in dll


class TestDoublyLinkedListUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        dll.clear()
        assert dll.is_empty
        assert dll.head is None
        assert dll.tail is None

    def test_reverse(self):
        """Test reversing."""
        dll = DoublyLinkedList.from_list([10, 20, 30, 40])
        dll.reverse()
        assert dll.to_list() == [40, 30, 20, 10]
        assert dll.head == 40
        assert dll.tail == 10

    def test_reverse_single(self):
        """Test reversing single element."""
        dll = DoublyLinkedList.from_list([42])
        dll.reverse()
        assert dll.to_list() == [42]

    def test_to_list_reversed(self):
        """Test to_list_reversed."""
        dll = DoublyLinkedList.from_list([10, 20, 30])
        assert dll.to_list_reversed() == [30, 20, 10]

    def test_equality(self):
        """Test equality."""
        dll1 = DoublyLinkedList.from_list([1, 2, 3])
        dll2 = DoublyLinkedList.from_list([1, 2, 3])
        dll3 = DoublyLinkedList.from_list([1, 2, 4])

        assert dll1 == dll2
        assert dll1 != dll3


class TestDoublyLinkedListLRUOperations:
    """Test operations useful for LRU cache."""

    def test_move_to_front(self):
        """Test moving node to front."""
        dll = DoublyLinkedList()
        dll.append(1)
        node_2 = dll.append(2)
        dll.append(3)

        dll.move_to_front(node_2)
        assert dll.to_list() == [2, 1, 3]
        assert dll.head == 2

    def test_move_to_front_already_front(self):
        """Test moving front node to front."""
        dll = DoublyLinkedList()
        node_1 = dll.append(1)
        dll.append(2)

        dll.move_to_front(node_1)
        assert dll.to_list() == [1, 2]

    def test_move_to_front_tail(self):
        """Test moving tail to front."""
        dll = DoublyLinkedList()
        dll.append(1)
        dll.append(2)
        node_3 = dll.append(3)

        dll.move_to_front(node_3)
        assert dll.to_list() == [3, 1, 2]
        assert dll.tail == 2

    def test_move_to_back(self):
        """Test moving node to back."""
        dll = DoublyLinkedList()
        dll.append(1)
        node_2 = dll.append(2)
        dll.append(3)

        dll.move_to_back(node_2)
        assert dll.to_list() == [1, 3, 2]
        assert dll.tail == 2


class TestDNodeClass:
    """Test DNode class."""

    def test_node_creation(self):
        """Test creating a node."""
        node = DNode(42)
        assert node.data == 42
        assert node.prev is None
        assert node.next is None

    def test_node_with_links(self):
        """Test creating linked nodes."""
        node1 = DNode(1)
        node2 = DNode(2, prev_node=node1)
        node1.next = node2

        assert node1.next == node2
        assert node2.prev == node1

    def test_node_repr(self):
        """Test node representation."""
        node = DNode(42)
        assert repr(node) == "DNode(42)"
