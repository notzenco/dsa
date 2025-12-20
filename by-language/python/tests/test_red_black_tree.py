"""
Tests for Red-Black Tree and B-Tree.
"""

import pytest
import random
from data_structures.red_black_tree import RedBlackTree, Color
from data_structures.b_tree import BTree


class TestRedBlackTreeBasics:
    """Test Red-Black Tree basic operations."""

    def test_empty_tree(self):
        """Test empty tree."""
        rbt = RedBlackTree()
        assert len(rbt) == 0
        assert 10 not in rbt

    def test_insert_single(self):
        """Test single insertion."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")

        assert len(rbt) == 1
        assert 10 in rbt
        assert rbt.search(10) == "ten"

    def test_insert_multiple(self):
        """Test multiple insertions."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")
        rbt.insert(5, "five")
        rbt.insert(15, "fifteen")

        assert len(rbt) == 3
        assert rbt.search(10) == "ten"
        assert rbt.search(5) == "five"
        assert rbt.search(15) == "fifteen"

    def test_insert_update(self):
        """Test updating existing key."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")
        rbt.insert(10, "TEN")

        assert len(rbt) == 1
        assert rbt.search(10) == "TEN"

    def test_bracket_notation(self):
        """Test bracket notation."""
        rbt = RedBlackTree()
        rbt[10] = "ten"
        rbt[20] = "twenty"

        assert rbt[10] == "ten"
        assert rbt[20] == "twenty"

    def test_getitem_keyerror(self):
        """Test KeyError on missing key."""
        rbt = RedBlackTree()
        rbt[10] = "ten"

        with pytest.raises(KeyError):
            _ = rbt[20]


class TestRedBlackTreeDeletion:
    """Test Red-Black Tree deletion."""

    def test_delete_leaf(self):
        """Test deleting leaf node."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")
        rbt.insert(5, "five")
        rbt.insert(15, "fifteen")

        assert rbt.delete(5)
        assert 5 not in rbt
        assert len(rbt) == 2

    def test_delete_not_found(self):
        """Test deleting non-existent key."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")

        assert not rbt.delete(20)
        assert len(rbt) == 1

    def test_delete_root(self):
        """Test deleting root."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")

        assert rbt.delete(10)
        assert len(rbt) == 0

    def test_delitem(self):
        """Test del notation."""
        rbt = RedBlackTree()
        rbt[10] = "ten"

        del rbt[10]
        assert 10 not in rbt

    def test_delitem_keyerror(self):
        """Test KeyError on delete."""
        rbt = RedBlackTree()
        with pytest.raises(KeyError):
            del rbt[10]


class TestRedBlackTreeProperties:
    """Test Red-Black Tree invariants."""

    def test_is_valid_after_inserts(self):
        """Tree should be valid after inserts."""
        rbt = RedBlackTree()
        for i in range(100):
            rbt.insert(i, str(i))
            assert rbt.is_valid()

    def test_is_valid_after_random_inserts(self):
        """Tree should be valid after random inserts."""
        rbt = RedBlackTree()
        values = list(range(100))
        random.shuffle(values)

        for v in values:
            rbt.insert(v, str(v))

        assert rbt.is_valid()

    def test_is_valid_after_deletes(self):
        """Tree should be valid after deletes."""
        rbt = RedBlackTree()
        for i in range(50):
            rbt.insert(i, str(i))

        for i in range(0, 50, 2):
            rbt.delete(i)
            assert rbt.is_valid()

    def test_height_bounded(self):
        """Height should be O(log n)."""
        rbt = RedBlackTree()
        for i in range(1000):
            rbt.insert(i, str(i))

        # Red-Black tree height <= 2 * log2(n + 1)
        import math
        max_height = 2 * math.log2(1001)
        assert rbt.height() <= max_height


class TestRedBlackTreeTraversal:
    """Test Red-Black Tree traversal."""

    def test_inorder(self):
        """Test inorder traversal returns sorted order."""
        rbt = RedBlackTree()
        values = [5, 3, 7, 1, 9, 4, 6]
        for v in values:
            rbt.insert(v, str(v))

        result = rbt.inorder()
        keys = [k for k, _ in result]
        assert keys == sorted(values)

    def test_iter(self):
        """Test iteration."""
        rbt = RedBlackTree()
        for i in [5, 3, 7]:
            rbt.insert(i, str(i))

        assert list(rbt) == [3, 5, 7]

    def test_items(self):
        """Test items iteration."""
        rbt = RedBlackTree()
        rbt.insert(5, "five")
        rbt.insert(3, "three")

        items = list(rbt.items())
        assert (3, "three") in items
        assert (5, "five") in items

    def test_keys_values(self):
        """Test keys and values methods."""
        rbt = RedBlackTree()
        rbt.insert(3, "three")
        rbt.insert(1, "one")
        rbt.insert(2, "two")

        assert rbt.keys() == [1, 2, 3]
        assert rbt.values() == ["one", "two", "three"]


class TestRedBlackTreeMinMax:
    """Test min/max operations."""

    def test_min(self):
        """Test min."""
        rbt = RedBlackTree()
        rbt.insert(5, "five")
        rbt.insert(3, "three")
        rbt.insert(7, "seven")

        assert rbt.min() == (3, "three")

    def test_max(self):
        """Test max."""
        rbt = RedBlackTree()
        rbt.insert(5, "five")
        rbt.insert(3, "three")
        rbt.insert(7, "seven")

        assert rbt.max() == (7, "seven")

    def test_min_max_empty(self):
        """Test min/max on empty tree."""
        rbt = RedBlackTree()
        assert rbt.min() is None
        assert rbt.max() is None


class TestBTreeBasics:
    """Test B-Tree basic operations."""

    def test_empty_tree(self):
        """Test empty tree."""
        bt = BTree(t=2)
        assert len(bt) == 0
        assert 10 not in bt

    def test_insert_single(self):
        """Test single insertion."""
        bt = BTree(t=2)
        bt.insert(10, "ten")

        assert len(bt) == 1
        assert 10 in bt
        assert bt.search(10) == "ten"

    def test_insert_multiple(self):
        """Test multiple insertions."""
        bt = BTree(t=2)
        for i in range(10):
            bt.insert(i, str(i))

        assert len(bt) == 10
        for i in range(10):
            assert bt.search(i) == str(i)

    def test_insert_update(self):
        """Test updating existing key."""
        bt = BTree(t=2)
        bt.insert(10, "ten")
        bt.insert(10, "TEN")

        assert len(bt) == 1
        assert bt.search(10) == "TEN"

    def test_bracket_notation(self):
        """Test bracket notation."""
        bt = BTree(t=2)
        bt[10] = "ten"
        bt[20] = "twenty"

        assert bt[10] == "ten"
        assert bt[20] == "twenty"

    def test_getitem_keyerror(self):
        """Test KeyError on missing key."""
        bt = BTree(t=2)
        bt[10] = "ten"

        with pytest.raises(KeyError):
            _ = bt[20]


class TestBTreeDeletion:
    """Test B-Tree deletion."""

    def test_delete_from_leaf(self):
        """Test deleting from leaf."""
        bt = BTree(t=2)
        for i in range(5):
            bt.insert(i, str(i))

        assert bt.delete(2)
        assert 2 not in bt
        assert len(bt) == 4

    def test_delete_not_found(self):
        """Test deleting non-existent key."""
        bt = BTree(t=2)
        bt.insert(10, "ten")

        assert not bt.delete(20)

    def test_delete_all(self):
        """Test deleting all keys."""
        bt = BTree(t=2)
        keys = list(range(20))
        for k in keys:
            bt.insert(k, str(k))

        random.shuffle(keys)
        for k in keys:
            bt.delete(k)

        assert len(bt) == 0

    def test_delitem(self):
        """Test del notation."""
        bt = BTree(t=2)
        bt[10] = "ten"

        del bt[10]
        assert 10 not in bt


class TestBTreeTraversal:
    """Test B-Tree traversal."""

    def test_inorder(self):
        """Test inorder returns sorted order."""
        bt = BTree(t=2)
        values = [5, 3, 7, 1, 9, 4, 6, 8, 2]
        for v in values:
            bt.insert(v, str(v))

        result = bt.inorder()
        keys = [k for k, _ in result]
        assert keys == sorted(values)

    def test_iter(self):
        """Test iteration."""
        bt = BTree(t=2)
        for i in [5, 3, 7, 1]:
            bt.insert(i, str(i))

        assert list(bt) == [1, 3, 5, 7]

    def test_keys_values(self):
        """Test keys and values methods."""
        bt = BTree(t=2)
        bt.insert(3, "three")
        bt.insert(1, "one")
        bt.insert(2, "two")

        assert bt.keys() == [1, 2, 3]
        assert bt.values() == ["one", "two", "three"]


class TestBTreeMinMax:
    """Test B-Tree min/max."""

    def test_min(self):
        """Test min."""
        bt = BTree(t=2)
        for i in [5, 3, 7, 1]:
            bt.insert(i, str(i))

        assert bt.min() == (1, "1")

    def test_max(self):
        """Test max."""
        bt = BTree(t=2)
        for i in [5, 3, 7, 1]:
            bt.insert(i, str(i))

        assert bt.max() == (7, "7")

    def test_min_max_empty(self):
        """Test min/max on empty tree."""
        bt = BTree(t=2)
        assert bt.min() is None
        assert bt.max() is None


class TestBTreeProperties:
    """Test B-Tree properties."""

    def test_different_degrees(self):
        """Test with different minimum degrees."""
        for t in [2, 3, 5, 10]:
            bt = BTree(t=t)
            for i in range(100):
                bt.insert(i, str(i))

            assert len(bt) == 100
            for i in range(100):
                assert i in bt

    def test_large_dataset(self):
        """Test with large dataset."""
        bt = BTree(t=10)
        values = list(range(1000))
        random.shuffle(values)

        for v in values:
            bt.insert(v, str(v))

        assert len(bt) == 1000
        assert bt.keys() == sorted(values)

    def test_height(self):
        """Test height is logarithmic."""
        bt = BTree(t=10)
        for i in range(10000):
            bt.insert(i, str(i))

        # Height should be O(log_t(n))
        import math
        max_height = math.ceil(math.log(10001, 10))
        assert bt.height() <= max_height + 1

    def test_invalid_degree(self):
        """Test invalid minimum degree."""
        with pytest.raises(ValueError):
            BTree(t=1)


class TestBTreeClear:
    """Test clear operation."""

    def test_clear_rbt(self):
        """Test clearing Red-Black tree."""
        rbt = RedBlackTree()
        for i in range(10):
            rbt.insert(i, str(i))

        rbt.clear()
        assert len(rbt) == 0

    def test_clear_btree(self):
        """Test clearing B-Tree."""
        bt = BTree(t=2)
        for i in range(10):
            bt.insert(i, str(i))

        bt.clear()
        assert len(bt) == 0


class TestRepr:
    """Test string representations."""

    def test_rbt_repr(self):
        """Test Red-Black tree repr."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")
        s = repr(rbt)
        assert "RedBlackTree" in s

    def test_btree_repr(self):
        """Test B-Tree repr."""
        bt = BTree(t=2)
        bt.insert(10, "ten")
        s = repr(bt)
        assert "BTree" in s

    def test_btree_display(self):
        """Test B-Tree display."""
        bt = BTree(t=2)
        for i in range(5):
            bt.insert(i, str(i))

        display = bt.display()
        assert len(display) > 0
