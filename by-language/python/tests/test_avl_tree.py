"""
Tests for AVL Tree implementation.
"""

import pytest
import random
from data_structures.avl_tree import AVLTree, AVLTreeMap


class TestAVLTreeBasics:
    """Test basic AVL tree operations."""

    def test_init(self):
        """Test initialization."""
        tree = AVLTree()
        assert len(tree) == 0
        assert tree.height() == 0

    def test_insert_single(self):
        """Test inserting single element."""
        tree = AVLTree()
        tree.insert(5)

        assert len(tree) == 1
        assert tree.search(5)
        assert tree.height() == 1

    def test_insert_multiple(self):
        """Test inserting multiple elements."""
        tree = AVLTree()
        for val in [3, 1, 4, 1, 5, 9, 2, 6]:
            tree.insert(val)

        assert len(tree) == 7  # One duplicate
        assert tree.search(3)
        assert tree.search(9)

    def test_insert_duplicate(self):
        """Test inserting duplicate updates value."""
        tree = AVLTree()
        tree.insert(5, "first")
        tree.insert(5, "second")

        assert len(tree) == 1
        assert tree.get(5) == "second"

    def test_search_existing(self):
        """Test searching for existing key."""
        tree = AVLTree()
        tree.insert(10)
        tree.insert(5)
        tree.insert(15)

        assert tree.search(10)
        assert tree.search(5)
        assert tree.search(15)

    def test_search_nonexistent(self):
        """Test searching for non-existent key."""
        tree = AVLTree()
        tree.insert(10)

        assert not tree.search(5)
        assert not tree.search(15)


class TestAVLTreeDelete:
    """Test AVL tree deletion."""

    def test_delete_leaf(self):
        """Test deleting leaf node."""
        tree = AVLTree()
        tree.insert(10)
        tree.insert(5)
        tree.insert(15)

        assert tree.delete(5)
        assert not tree.search(5)
        assert len(tree) == 2

    def test_delete_node_one_child(self):
        """Test deleting node with one child."""
        tree = AVLTree()
        tree.insert(10)
        tree.insert(5)
        tree.insert(15)
        tree.insert(3)

        assert tree.delete(5)
        assert not tree.search(5)
        assert tree.search(3)

    def test_delete_node_two_children(self):
        """Test deleting node with two children."""
        tree = AVLTree()
        for val in [10, 5, 15, 3, 7, 12, 20]:
            tree.insert(val)

        assert tree.delete(5)
        assert not tree.search(5)
        assert tree.search(3)
        assert tree.search(7)

    def test_delete_root(self):
        """Test deleting root node."""
        tree = AVLTree()
        tree.insert(10)
        tree.insert(5)
        tree.insert(15)

        assert tree.delete(10)
        assert not tree.search(10)
        assert len(tree) == 2

    def test_delete_nonexistent(self):
        """Test deleting non-existent key."""
        tree = AVLTree()
        tree.insert(10)

        assert not tree.delete(5)
        assert len(tree) == 1


class TestAVLTreeBalance:
    """Test AVL tree balancing."""

    def test_ll_rotation(self):
        """Test left-left case triggers right rotation."""
        tree = AVLTree()
        # Insert in decreasing order to trigger LL case
        tree.insert(30)
        tree.insert(20)
        tree.insert(10)

        assert tree.is_balanced()
        # After rotation, 20 should be root
        assert tree._root.key == 20

    def test_rr_rotation(self):
        """Test right-right case triggers left rotation."""
        tree = AVLTree()
        # Insert in increasing order to trigger RR case
        tree.insert(10)
        tree.insert(20)
        tree.insert(30)

        assert tree.is_balanced()
        # After rotation, 20 should be root
        assert tree._root.key == 20

    def test_lr_rotation(self):
        """Test left-right case triggers LR rotation."""
        tree = AVLTree()
        tree.insert(30)
        tree.insert(10)
        tree.insert(20)

        assert tree.is_balanced()
        assert tree._root.key == 20

    def test_rl_rotation(self):
        """Test right-left case triggers RL rotation."""
        tree = AVLTree()
        tree.insert(10)
        tree.insert(30)
        tree.insert(20)

        assert tree.is_balanced()
        assert tree._root.key == 20

    def test_always_balanced(self):
        """Test tree stays balanced after many operations."""
        tree = AVLTree()

        # Insert many elements
        for i in range(100):
            tree.insert(i)
            assert tree.is_balanced()

        # Delete some elements
        for i in range(0, 100, 2):
            tree.delete(i)
            assert tree.is_balanced()

    def test_height_logarithmic(self):
        """Test height is O(log n)."""
        tree = AVLTree()
        n = 1000

        for i in range(n):
            tree.insert(i)

        # Height should be approximately log2(n)
        import math
        max_expected_height = 1.44 * math.log2(n + 1) + 1
        assert tree.height() <= max_expected_height


class TestAVLTreeTraversal:
    """Test AVL tree traversals."""

    def test_inorder(self):
        """Test inorder traversal returns sorted keys."""
        tree = AVLTree()
        values = [5, 3, 7, 1, 4, 6, 8]
        for v in values:
            tree.insert(v)

        assert tree.inorder() == sorted(values)

    def test_preorder(self):
        """Test preorder traversal."""
        tree = AVLTree()
        tree.insert(2)
        tree.insert(1)
        tree.insert(3)

        preorder = tree.preorder()
        assert preorder[0] == 2  # Root first

    def test_level_order(self):
        """Test level order traversal."""
        tree = AVLTree()
        tree.insert(2)
        tree.insert(1)
        tree.insert(3)

        levels = tree.level_order()
        assert levels[0] == [2]  # Root level
        assert set(levels[1]) == {1, 3}  # Second level


class TestAVLTreeMinMax:
    """Test minimum/maximum operations."""

    def test_minimum(self):
        """Test finding minimum."""
        tree = AVLTree()
        values = [5, 3, 7, 1, 4, 6, 8]
        for v in values:
            tree.insert(v)

        assert tree.minimum() == 1

    def test_maximum(self):
        """Test finding maximum."""
        tree = AVLTree()
        values = [5, 3, 7, 1, 4, 6, 8]
        for v in values:
            tree.insert(v)

        assert tree.maximum() == 8

    def test_min_max_empty(self):
        """Test min/max on empty tree."""
        tree = AVLTree()
        assert tree.minimum() is None
        assert tree.maximum() is None


class TestAVLTreeDunder:
    """Test magic methods."""

    def test_len(self):
        """Test __len__."""
        tree = AVLTree()
        assert len(tree) == 0

        tree.insert(1)
        tree.insert(2)
        tree.insert(3)
        assert len(tree) == 3

    def test_contains(self):
        """Test __contains__."""
        tree = AVLTree()
        tree.insert(5)

        assert 5 in tree
        assert 10 not in tree

    def test_iter(self):
        """Test __iter__."""
        tree = AVLTree()
        values = [3, 1, 4, 1, 5]
        for v in values:
            tree.insert(v)

        assert list(tree) == sorted(set(values))

    def test_repr(self):
        """Test __repr__."""
        tree = AVLTree()
        tree.insert(1)

        s = repr(tree)
        assert "AVLTree" in s
        assert "1" in s


class TestAVLTreeMap:
    """Test AVL Tree based map."""

    def test_setitem_getitem(self):
        """Test setting and getting items."""
        m = AVLTreeMap()
        m['b'] = 2
        m['a'] = 1
        m['c'] = 3

        assert m['a'] == 1
        assert m['b'] == 2
        assert m['c'] == 3

    def test_getitem_keyerror(self):
        """Test getting non-existent key raises KeyError."""
        m = AVLTreeMap()
        with pytest.raises(KeyError):
            _ = m['missing']

    def test_delitem(self):
        """Test deleting item."""
        m = AVLTreeMap()
        m['a'] = 1
        del m['a']

        assert 'a' not in m

    def test_contains(self):
        """Test membership check."""
        m = AVLTreeMap()
        m['a'] = 1

        assert 'a' in m
        assert 'b' not in m

    def test_keys_sorted(self):
        """Test keys are returned sorted."""
        m = AVLTreeMap()
        m['c'] = 3
        m['a'] = 1
        m['b'] = 2

        assert m.keys() == ['a', 'b', 'c']

    def test_items(self):
        """Test items returns sorted pairs."""
        m = AVLTreeMap()
        m['b'] = 2
        m['a'] = 1

        assert m.items() == [('a', 1), ('b', 2)]

    def test_get_with_default(self):
        """Test get with default value."""
        m = AVLTreeMap()
        m['a'] = 1

        assert m.get('a') == 1
        assert m.get('b') is None
        assert m.get('b', 'default') == 'default'

    def test_floor(self):
        """Test floor operation."""
        m = AVLTreeMap()
        for i in [10, 20, 30, 40]:
            m[i] = i

        assert m.floor(25) == 20
        assert m.floor(30) == 30
        assert m.floor(5) is None

    def test_ceiling(self):
        """Test ceiling operation."""
        m = AVLTreeMap()
        for i in [10, 20, 30, 40]:
            m[i] = i

        assert m.ceiling(25) == 30
        assert m.ceiling(20) == 20
        assert m.ceiling(50) is None

    def test_range_query(self):
        """Test range query."""
        m = AVLTreeMap()
        for i in range(1, 11):
            m[i] = i

        assert m.range_query(3, 7) == [3, 4, 5, 6, 7]
        assert m.range_query(5, 5) == [5]
        assert m.range_query(20, 30) == []


class TestAVLTreeStress:
    """Stress tests for AVL tree."""

    def test_random_operations(self):
        """Test random insert/delete operations."""
        tree = AVLTree()
        inserted = set()

        for _ in range(500):
            val = random.randint(1, 1000)
            tree.insert(val)
            inserted.add(val)
            assert tree.is_balanced()

        for val in list(inserted)[:250]:
            tree.delete(val)
            inserted.discard(val)
            assert tree.is_balanced()

        for val in inserted:
            assert tree.search(val)

    def test_sequential_insert_delete(self):
        """Test sequential insertions and deletions."""
        tree = AVLTree()

        # Insert 1 to 1000
        for i in range(1, 1001):
            tree.insert(i)

        assert len(tree) == 1000
        assert tree.is_balanced()

        # Delete all even numbers
        for i in range(2, 1001, 2):
            tree.delete(i)

        assert len(tree) == 500
        assert tree.is_balanced()

        # Verify odd numbers remain
        for i in range(1, 1001, 2):
            assert tree.search(i)


class TestAVLTreeEdgeCases:
    """Test edge cases."""

    def test_empty_operations(self):
        """Test operations on empty tree."""
        tree = AVLTree()

        assert not tree.search(1)
        assert not tree.delete(1)
        assert tree.inorder() == []
        assert tree.minimum() is None
        assert tree.maximum() is None

    def test_single_element(self):
        """Test operations on single element tree."""
        tree = AVLTree()
        tree.insert(42)

        assert tree.search(42)
        assert tree.minimum() == 42
        assert tree.maximum() == 42
        assert tree.inorder() == [42]

        tree.delete(42)
        assert len(tree) == 0

    def test_negative_keys(self):
        """Test with negative keys."""
        tree = AVLTree()
        for v in [-5, -3, -1, 0, 1, 3, 5]:
            tree.insert(v)

        assert tree.minimum() == -5
        assert tree.maximum() == 5
        assert tree.inorder() == [-5, -3, -1, 0, 1, 3, 5]

    def test_string_keys(self):
        """Test with string keys."""
        tree = AVLTree()
        tree.insert("banana")
        tree.insert("apple")
        tree.insert("cherry")

        assert tree.inorder() == ["apple", "banana", "cherry"]
        assert tree.search("banana")

    def test_clear(self):
        """Test clearing tree."""
        tree = AVLTree()
        for i in range(100):
            tree.insert(i)

        tree.clear()

        assert len(tree) == 0
        assert tree.height() == 0
