"""Tests for B-Tree implementation."""

import random
import pytest
from data_structures.b_tree import BTree


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

        seed = 20260417
        random.Random(seed).shuffle(keys)
        model = set(keys)
        for iteration, k in enumerate(keys):
            message = f"seed={seed}, iteration={iteration}, input={keys!r}"
            assert bt.delete(k), message
            model.remove(k)
            assert bt.keys() == sorted(model), message
            assert len(bt) == len(model), message

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
        seed = 20260417
        random.Random(seed).shuffle(values)

        for v in values:
            bt.insert(v, str(v))

        message = f"seed={seed}, iteration={len(values) - 1}, input={values!r}"
        assert len(bt) == 1000, message
        assert bt.keys() == sorted(values), message

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

    def test_clear_btree(self):
        """Test clearing B-Tree."""
        bt = BTree(t=2)
        for i in range(10):
            bt.insert(i, str(i))

        bt.clear()
        assert len(bt) == 0


class TestRepr:
    """Test string representations."""

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


def test_none_value_has_independent_key_membership():
    tree = BTree()
    tree.insert(1, None)
    tree.insert(2, "two")
    assert 1 in tree
    assert tree[1] is None
    assert tree.delete(1)
    assert 1 not in tree
    with pytest.raises(KeyError):
        _ = tree[1]
