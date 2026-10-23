"""
Tests for Red-Black Tree.
"""

import pytest
import random
from data_structures.red_black_tree import RedBlackTree, RBNode, Color


@pytest.mark.parametrize('values', [list(range(1, 7)), list(range(6, 0, -1))])
def test_delete_with_red_sibling_and_mirrored_case(values):
    tree = RedBlackTree()
    model = {}
    for key in values:
        tree.insert(key, str(key))
        model[key] = str(key)
    # Deleting the outer black leaf requires rotating its red sibling first.
    for key in values:
        assert tree.delete(key)
        del model[key]
        assert tree.inorder() == sorted(model.items())
        assert len(tree) == len(model)
        assert tree.is_valid()


def test_black_height_empty_single_and_balanced():
    tree = RedBlackTree()
    assert tree.black_height() == 0
    assert tree.height() == 0
    assert tree.is_valid()
    tree.insert(2, 'two')
    assert tree.black_height() == 1
    tree.insert(1, 'one')
    tree.insert(3, 'three')
    assert tree.black_height() == 1
    tree.insert(0, 'zero')
    assert tree.black_height() == 2
    assert tree.is_valid()


def test_none_values_have_independent_membership():
    tree = RedBlackTree()
    tree[1] = None
    assert 1 in tree
    assert tree[1] is None
    del tree[1]
    assert 1 not in tree
    with pytest.raises(KeyError):
        _ = tree[1]


def test_node_repr_reflects_color():
    node = RBNode(5, 'five')
    assert repr(node) == 'RBNode(5:R)'
    node.color = Color.BLACK
    assert repr(node) == 'RBNode(5:B)'


def test_validity_rejects_red_root():
    tree = RedBlackTree()
    tree.insert(1, 1)
    tree._root.color = Color.RED
    assert not tree.is_valid()


@pytest.mark.parametrize('side', ['left', 'right'])
def test_validity_rejects_red_parent_and_child(side):
    tree = RedBlackTree()
    tree.insert(10, 10)
    child = RBNode(5 if side == 'left' else 15, None, parent=tree._root)
    grandchild = RBNode(3 if side == 'left' else 17, None, parent=child)
    setattr(tree._root, side, child)
    setattr(child, side, grandchild)
    assert not tree.is_valid()


def test_validity_rejects_unequal_black_paths():
    tree = RedBlackTree()
    tree.insert(2, 2)
    tree.insert(1, 1)
    tree._root.left.color = Color.BLACK
    assert not tree.is_valid()


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
        seed = 20260417
        random.Random(seed).shuffle(values)

        for iteration, v in enumerate(values):
            rbt.insert(v, str(v))
            assert rbt.is_valid(), f"seed={seed}, iteration={iteration}, input={values!r}"

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


class TestRedBlackTreeClear:
    """Test clear operation."""

    def test_clear_rbt(self):
        """Test clearing Red-Black tree."""
        rbt = RedBlackTree()
        for i in range(10):
            rbt.insert(i, str(i))

        rbt.clear()
        assert len(rbt) == 0


class TestRepr:
    """Test string representations."""

    def test_rbt_repr(self):
        """Test Red-Black tree repr."""
        rbt = RedBlackTree()
        rbt.insert(10, "ten")
        s = repr(rbt)
        assert "RedBlackTree" in s
