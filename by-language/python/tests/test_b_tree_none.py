import pytest

from data_structures.b_tree import BTree


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
