"""
Tests for Binary Search Tree implementation.
"""

import pytest
from data_structures.binary_search_tree import BinarySearchTree, TreeNode


class TestBSTBasics:
    """Test basic BST operations."""

    def test_init(self):
        """Test initialization."""
        bst = BinarySearchTree()
        assert len(bst) == 0
        assert bst.is_empty
        assert bst.root is None

    def test_insert_single(self):
        """Test inserting single element."""
        bst = BinarySearchTree()
        bst.insert(50)
        assert len(bst) == 1
        assert bst.root == 50

    def test_insert_multiple(self):
        """Test inserting multiple elements."""
        bst = BinarySearchTree()
        bst.insert(50)
        bst.insert(30)
        bst.insert(70)

        assert len(bst) == 3
        assert bst.root == 50

    def test_insert_maintains_bst_property(self):
        """Test that BST property is maintained."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])
        assert bst.is_valid_bst()


class TestBSTSearch:
    """Test search operations."""

    def test_search_existing(self):
        """Test searching for existing value."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        node = bst.search(30)
        assert node is not None
        assert node.value == 30

    def test_search_nonexistent(self):
        """Test searching for non-existent value."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        assert bst.search(999) is None

    def test_contains(self):
        """Test __contains__ method."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        assert 30 in bst
        assert 999 not in bst


class TestBSTDelete:
    """Test deletion operations."""

    def test_delete_leaf(self):
        """Test deleting a leaf node."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        result = bst.delete(30)

        assert result is True
        assert len(bst) == 2
        assert 30 not in bst
        assert bst.is_valid_bst()

    def test_delete_node_with_one_child(self):
        """Test deleting node with one child."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20])
        result = bst.delete(30)

        assert result is True
        assert 20 in bst
        assert bst.is_valid_bst()

    def test_delete_node_with_two_children(self):
        """Test deleting node with two children."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])
        result = bst.delete(30)

        assert result is True
        assert 20 in bst
        assert 40 in bst
        assert bst.is_valid_bst()

    def test_delete_root(self):
        """Test deleting root node."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        result = bst.delete(50)

        assert result is True
        assert len(bst) == 2
        assert bst.is_valid_bst()

    def test_delete_nonexistent(self):
        """Test deleting non-existent value."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        result = bst.delete(999)
        assert result is False
        assert len(bst) == 3


class TestBSTMinMax:
    """Test min/max operations."""

    def test_minimum(self):
        """Test finding minimum."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        assert bst.minimum() == 20

    def test_maximum(self):
        """Test finding maximum."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        assert bst.maximum() == 70

    def test_minimum_empty(self):
        """Test minimum on empty tree."""
        bst = BinarySearchTree()
        assert bst.minimum() is None

    def test_maximum_empty(self):
        """Test maximum on empty tree."""
        bst = BinarySearchTree()
        assert bst.maximum() is None


class TestBSTSuccessorPredecessor:
    """Test successor and predecessor."""

    def test_successor(self):
        """Test finding successor."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])

        assert bst.successor(30) == 40
        assert bst.successor(40) == 50
        assert bst.successor(50) == 60
        assert bst.successor(80) is None

    def test_predecessor(self):
        """Test finding predecessor."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])

        assert bst.predecessor(70) == 60
        assert bst.predecessor(60) == 50
        assert bst.predecessor(50) == 40
        assert bst.predecessor(20) is None


class TestBSTTraversal:
    """Test tree traversals."""

    def test_inorder(self):
        """Test in-order traversal (sorted)."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])
        result = list(bst.inorder())
        assert result == [20, 30, 40, 50, 60, 70, 80]

    def test_preorder(self):
        """Test pre-order traversal."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        result = list(bst.preorder())
        assert result == [50, 30, 20, 40, 70]

    def test_postorder(self):
        """Test post-order traversal."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        result = list(bst.postorder())
        assert result == [20, 40, 30, 70, 50]

    def test_levelorder(self):
        """Test level-order traversal."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        result = list(bst.levelorder())
        assert result == [50, 30, 70, 20, 40]

    def test_iter(self):
        """Test __iter__ (in-order)."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        result = list(bst)
        assert result == [30, 50, 70]


class TestBSTAdvanced:
    """Test advanced operations."""

    def test_height(self):
        """Test height calculation."""
        bst = BinarySearchTree()
        assert bst.height == -1

        bst.insert(50)
        assert bst.height == 0

        bst.insert(30)
        bst.insert(70)
        assert bst.height == 1

    def test_kth_smallest(self):
        """Test kth smallest element."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])

        assert bst.kth_smallest(1) == 20
        assert bst.kth_smallest(4) == 50
        assert bst.kth_smallest(7) == 80
        assert bst.kth_smallest(0) is None
        assert bst.kth_smallest(8) is None

    def test_floor(self):
        """Test floor operation."""
        bst = BinarySearchTree.from_list([10, 20, 30, 40, 50])

        assert bst.floor(25) == 20
        assert bst.floor(30) == 30
        assert bst.floor(5) is None

    def test_ceiling(self):
        """Test ceiling operation."""
        bst = BinarySearchTree.from_list([10, 20, 30, 40, 50])

        assert bst.ceiling(25) == 30
        assert bst.ceiling(30) == 30
        assert bst.ceiling(55) is None

    def test_range_query(self):
        """Test range query."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])
        result = bst.range_query(25, 65)
        assert result == [30, 40, 50, 60]

    def test_lowest_common_ancestor(self):
        """Test LCA."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40, 60, 80])

        assert bst.lowest_common_ancestor(20, 40) == 30
        assert bst.lowest_common_ancestor(20, 80) == 50
        assert bst.lowest_common_ancestor(60, 80) == 70


class TestBSTUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing the tree."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        bst.clear()
        assert bst.is_empty
        assert len(bst) == 0

    def test_to_list(self):
        """Test converting to sorted list."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        assert bst.to_list() == [20, 30, 40, 50, 70]

    def test_from_list(self):
        """Test creating from list."""
        bst = BinarySearchTree.from_list([50, 30, 70])
        assert len(bst) == 3
        assert bst.is_valid_bst()

    def test_from_sorted_list(self):
        """Test creating balanced BST from sorted list."""
        values = [10, 20, 30, 40, 50, 60, 70]
        bst = BinarySearchTree.from_sorted_list(values)

        assert len(bst) == 7
        assert bst.is_valid_bst()
        assert bst.height <= 3  # Should be balanced

    def test_is_valid_bst(self):
        """Test BST validation."""
        bst = BinarySearchTree.from_list([50, 30, 70, 20, 40])
        assert bst.is_valid_bst()


class TestTreeNode:
    """Test TreeNode class."""

    def test_node_creation(self):
        """Test creating a node."""
        node = TreeNode(42)
        assert node.value == 42
        assert node.left is None
        assert node.right is None
        assert node.parent is None
        assert node.is_leaf

    def test_node_properties(self):
        """Test node properties."""
        parent = TreeNode(50)
        left_child = TreeNode(30, parent=parent)
        right_child = TreeNode(70, parent=parent)
        parent.left = left_child
        parent.right = right_child

        assert parent.has_left_child
        assert parent.has_right_child
        assert not parent.is_leaf
        assert left_child.is_left_child
        assert right_child.is_right_child

    def test_node_repr(self):
        """Test node string representation."""
        node = TreeNode(42)
        assert repr(node) == "TreeNode(42)"
