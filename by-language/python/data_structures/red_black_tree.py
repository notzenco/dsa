"""
Red-Black Tree Data Structure.

╔══════════════════════════════════════════════════════════════════════════════╗
║                              RED-BLACK TREE                                  ║
╠══════════════════════════════════════════════════════════════════════════════╣
║                                                                              ║
║  A self-balancing binary search tree with the following properties:          ║
║  1. Every node is either RED or BLACK                                        ║
║  2. Root is always BLACK                                                     ║
║  3. All leaves (NIL) are BLACK                                               ║
║  4. If a node is RED, both its children are BLACK                           ║
║  5. Every path from root to leaves has the same number of BLACK nodes       ║
║                                                                              ║
║  Example Tree (B=Black, R=Red):                                             ║
║                                                                              ║
║                        ┌───────(13B)───────┐                                ║
║                        │                   │                                ║
║                   ┌──(8R)──┐          ┌──(17R)──┐                           ║
║                   │        │          │         │                           ║
║               ┌─(1B)─┐  (11B)      (15B)    ┌─(25B)─┐                       ║
║               │      │                      │       │                       ║
║              NIL   (6R)                   (22R)   (27R)                      ║
║                                                                              ║
║  ROTATIONS:                                                                  ║
║                                                                              ║
║  Left Rotation:          Right Rotation:                                    ║
║       x                        y                                            ║
║      / \                      / \                                           ║
║     a   y        =>          x   c                                          ║
║        / \                  / \                                             ║
║       b   c                a   b                                            ║
║                                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌───────────────┬─────────────┬─────────────┐
│ Operation     │ Average     │ Worst       │
├───────────────┼─────────────┼─────────────┤
│ Search        │ O(log n)    │ O(log n)    │
│ Insert        │ O(log n)    │ O(log n)    │
│ Delete        │ O(log n)    │ O(log n)    │
│ Space         │ O(n)        │ O(n)        │
└───────────────┴─────────────┴─────────────┘

COMPARISON WITH AVL:
- AVL: More strictly balanced (height diff ≤ 1), faster lookups
- Red-Black: Less strictly balanced, faster inserts/deletes
- Red-Black height ≤ 2 * log(n+1), AVL height ≤ 1.44 * log(n)

LEETCODE PROBLEMS:
- Used internally by TreeMap/TreeSet in Java, std::map in C++

USE CASES:
- Linux kernel (CFS scheduler, memory management)
- Java TreeMap, TreeSet
- C++ std::map, std::set
- Databases (indexing)
"""

from typing import TypeVar, Generic, Optional, List, Iterator, Tuple
from enum import Enum

K = TypeVar('K')
V = TypeVar('V')


class Color(Enum):
    """Node color in Red-Black tree."""
    RED = 0
    BLACK = 1


class RBNode(Generic[K, V]):
    """Node in a Red-Black tree."""

    def __init__(
        self,
        key: K,
        value: V,
        color: Color = Color.RED,
        parent: Optional['RBNode[K, V]'] = None
    ) -> None:
        self.key = key
        self.value = value
        self.color = color
        self.parent = parent
        self.left: Optional['RBNode[K, V]'] = None
        self.right: Optional['RBNode[K, V]'] = None

    def __repr__(self) -> str:
        c = 'R' if self.color == Color.RED else 'B'
        return f"RBNode({self.key}:{c})"


class RedBlackTree(Generic[K, V]):
    """
    Red-Black Tree implementation.

    A self-balancing binary search tree that guarantees O(log n)
    operations through color-based balancing.
    """

    def __init__(self) -> None:
        """Initialize empty Red-Black tree."""
        self._root: Optional[RBNode[K, V]] = None
        self._size = 0

    def __len__(self) -> int:
        """Return number of nodes."""
        return self._size

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return self._find_node(key) is not None

    def _find_node(self, key: K) -> Optional[RBNode[K, V]]:
        """Find node with given key."""
        node = self._root
        while node is not None:
            if key < node.key:
                node = node.left
            elif key > node.key:
                node = node.right
            else:
                return node
        return None

    def search(self, key: K) -> Optional[V]:
        """
        Search for key and return value.

        Time: O(log n)
        """
        node = self._find_node(key)
        return node.value if node else None

    def __getitem__(self, key: K) -> V:
        """Get value by key."""
        node = self._find_node(key)
        if node is None:
            raise KeyError(key)
        return node.value

    # =========================================================================
    # ROTATIONS
    # =========================================================================

    def _rotate_left(self, x: RBNode[K, V]) -> None:
        """
        Left rotation around node x.

            x                y
           / \              / \
          a   y    =>      x   c
             / \          / \
            b   c        a   b
        """
        y = x.right
        if y is None:
            return

        # Turn y's left subtree into x's right subtree
        x.right = y.left
        if y.left is not None:
            y.left.parent = x

        # Link x's parent to y
        y.parent = x.parent
        if x.parent is None:
            self._root = y
        elif x == x.parent.left:
            x.parent.left = y
        else:
            x.parent.right = y

        # Put x on y's left
        y.left = x
        x.parent = y

    def _rotate_right(self, y: RBNode[K, V]) -> None:
        """
        Right rotation around node y.

              y              x
             / \            / \
            x   c    =>    a   y
           / \                / \
          a   b              b   c
        """
        x = y.left
        if x is None:
            return

        # Turn x's right subtree into y's left subtree
        y.left = x.right
        if x.right is not None:
            x.right.parent = y

        # Link y's parent to x
        x.parent = y.parent
        if y.parent is None:
            self._root = x
        elif y == y.parent.right:
            y.parent.right = x
        else:
            y.parent.left = x

        # Put y on x's right
        x.right = y
        y.parent = x

    # =========================================================================
    # INSERTION
    # =========================================================================

    def insert(self, key: K, value: V) -> None:
        """
        Insert key-value pair.

        Time: O(log n)
        """
        # Standard BST insert
        new_node: RBNode[K, V] = RBNode(key, value, Color.RED)

        parent: Optional[RBNode[K, V]] = None
        current = self._root

        while current is not None:
            parent = current
            if key < current.key:
                current = current.left
            elif key > current.key:
                current = current.right
            else:
                # Key exists, update value
                current.value = value
                return

        new_node.parent = parent

        if parent is None:
            self._root = new_node
        elif key < parent.key:
            parent.left = new_node
        else:
            parent.right = new_node

        self._size += 1

        # Fix Red-Black properties
        self._insert_fixup(new_node)

    def _insert_fixup(self, z: RBNode[K, V]) -> None:
        """Fix Red-Black tree properties after insertion."""
        while z.parent is not None and z.parent.color == Color.RED:
            if z.parent.parent is None:
                break

            if z.parent == z.parent.parent.left:
                # Parent is left child of grandparent
                uncle = z.parent.parent.right

                if uncle is not None and uncle.color == Color.RED:
                    # Case 1: Uncle is red
                    z.parent.color = Color.BLACK
                    uncle.color = Color.BLACK
                    z.parent.parent.color = Color.RED
                    z = z.parent.parent
                else:
                    if z == z.parent.right:
                        # Case 2: z is right child
                        z = z.parent
                        self._rotate_left(z)

                    # Case 3: z is left child
                    if z.parent is not None:
                        z.parent.color = Color.BLACK
                        if z.parent.parent is not None:
                            z.parent.parent.color = Color.RED
                            self._rotate_right(z.parent.parent)
            else:
                # Parent is right child of grandparent (mirror cases)
                uncle = z.parent.parent.left

                if uncle is not None and uncle.color == Color.RED:
                    # Case 1: Uncle is red
                    z.parent.color = Color.BLACK
                    uncle.color = Color.BLACK
                    z.parent.parent.color = Color.RED
                    z = z.parent.parent
                else:
                    if z == z.parent.left:
                        # Case 2: z is left child
                        z = z.parent
                        self._rotate_right(z)

                    # Case 3: z is right child
                    if z.parent is not None:
                        z.parent.color = Color.BLACK
                        if z.parent.parent is not None:
                            z.parent.parent.color = Color.RED
                            self._rotate_left(z.parent.parent)

        # Root must be black
        if self._root is not None:
            self._root.color = Color.BLACK

    def __setitem__(self, key: K, value: V) -> None:
        """Set value by key."""
        self.insert(key, value)

    # =========================================================================
    # DELETION
    # =========================================================================

    def _transplant(self, u: RBNode[K, V], v: Optional[RBNode[K, V]]) -> None:
        """Replace subtree rooted at u with subtree rooted at v."""
        if u.parent is None:
            self._root = v
        elif u == u.parent.left:
            u.parent.left = v
        else:
            u.parent.right = v

        if v is not None:
            v.parent = u.parent

    def _minimum(self, node: RBNode[K, V]) -> RBNode[K, V]:
        """Find minimum node in subtree."""
        while node.left is not None:
            node = node.left
        return node

    def delete(self, key: K) -> bool:
        """
        Delete node with given key.

        Time: O(log n)

        Returns:
            True if key was found and deleted.
        """
        z = self._find_node(key)
        if z is None:
            return False

        y = z
        y_original_color = y.color
        x: Optional[RBNode[K, V]] = None
        x_parent: Optional[RBNode[K, V]] = None

        if z.left is None:
            x = z.right
            x_parent = z.parent
            self._transplant(z, z.right)
        elif z.right is None:
            x = z.left
            x_parent = z.parent
            self._transplant(z, z.left)
        else:
            y = self._minimum(z.right)
            y_original_color = y.color
            x = y.right

            if y.parent == z:
                x_parent = y
            else:
                x_parent = y.parent
                self._transplant(y, y.right)
                y.right = z.right
                if y.right is not None:
                    y.right.parent = y

            self._transplant(z, y)
            y.left = z.left
            if y.left is not None:
                y.left.parent = y
            y.color = z.color

        self._size -= 1

        if y_original_color == Color.BLACK:
            self._delete_fixup(x, x_parent)

        return True

    def _delete_fixup(
        self,
        x: Optional[RBNode[K, V]],
        x_parent: Optional[RBNode[K, V]]
    ) -> None:
        """Fix Red-Black tree properties after deletion."""
        while x != self._root and (x is None or x.color == Color.BLACK):
            if x_parent is None:
                break

            if x == x_parent.left:
                w = x_parent.right  # Sibling

                if w is not None and w.color == Color.RED:
                    # Case 1: Sibling is red
                    w.color = Color.BLACK
                    x_parent.color = Color.RED
                    self._rotate_left(x_parent)
                    w = x_parent.right

                if w is None:
                    x = x_parent
                    x_parent = x.parent
                    continue

                left_black = w.left is None or w.left.color == Color.BLACK
                right_black = w.right is None or w.right.color == Color.BLACK

                if left_black and right_black:
                    # Case 2: Both of sibling's children are black
                    w.color = Color.RED
                    x = x_parent
                    x_parent = x.parent
                else:
                    if right_black:
                        # Case 3: Sibling's right child is black
                        if w.left is not None:
                            w.left.color = Color.BLACK
                        w.color = Color.RED
                        self._rotate_right(w)
                        w = x_parent.right

                    # Case 4: Sibling's right child is red
                    if w is not None:
                        w.color = x_parent.color
                        if w.right is not None:
                            w.right.color = Color.BLACK
                    x_parent.color = Color.BLACK
                    self._rotate_left(x_parent)
                    x = self._root
                    break
            else:
                # Mirror cases
                w = x_parent.left

                if w is not None and w.color == Color.RED:
                    w.color = Color.BLACK
                    x_parent.color = Color.RED
                    self._rotate_right(x_parent)
                    w = x_parent.left

                if w is None:
                    x = x_parent
                    x_parent = x.parent
                    continue

                left_black = w.left is None or w.left.color == Color.BLACK
                right_black = w.right is None or w.right.color == Color.BLACK

                if left_black and right_black:
                    w.color = Color.RED
                    x = x_parent
                    x_parent = x.parent
                else:
                    if left_black:
                        if w.right is not None:
                            w.right.color = Color.BLACK
                        w.color = Color.RED
                        self._rotate_left(w)
                        w = x_parent.left

                    if w is not None:
                        w.color = x_parent.color
                        if w.left is not None:
                            w.left.color = Color.BLACK
                    x_parent.color = Color.BLACK
                    self._rotate_right(x_parent)
                    x = self._root
                    break

        if x is not None:
            x.color = Color.BLACK

    def __delitem__(self, key: K) -> None:
        """Delete by key."""
        if not self.delete(key):
            raise KeyError(key)

    # =========================================================================
    # TRAVERSAL
    # =========================================================================

    def inorder(self) -> List[Tuple[K, V]]:
        """Return inorder traversal (sorted order)."""
        result: List[Tuple[K, V]] = []

        def _inorder(node: Optional[RBNode[K, V]]) -> None:
            if node is not None:
                _inorder(node.left)
                result.append((node.key, node.value))
                _inorder(node.right)

        _inorder(self._root)
        return result

    def __iter__(self) -> Iterator[K]:
        """Iterate keys in sorted order."""
        for key, _ in self.inorder():
            yield key

    def items(self) -> Iterator[Tuple[K, V]]:
        """Iterate (key, value) pairs in sorted order."""
        for item in self.inorder():
            yield item

    def keys(self) -> List[K]:
        """Return all keys in sorted order."""
        return [k for k, _ in self.inorder()]

    def values(self) -> List[V]:
        """Return all values in key-sorted order."""
        return [v for _, v in self.inorder()]

    # =========================================================================
    # UTILITY
    # =========================================================================

    def min(self) -> Optional[Tuple[K, V]]:
        """Return minimum key-value pair."""
        if self._root is None:
            return None
        node = self._minimum(self._root)
        return (node.key, node.value)

    def max(self) -> Optional[Tuple[K, V]]:
        """Return maximum key-value pair."""
        if self._root is None:
            return None
        node = self._root
        while node.right is not None:
            node = node.right
        return (node.key, node.value)

    def height(self) -> int:
        """Return height of tree."""
        def _height(node: Optional[RBNode[K, V]]) -> int:
            if node is None:
                return 0
            return 1 + max(_height(node.left), _height(node.right))
        return _height(self._root)

    def black_height(self) -> int:
        """Return black height (number of black nodes to any leaf)."""
        count = 0
        node = self._root
        while node is not None:
            if node.color == Color.BLACK:
                count += 1
            node = node.left
        return count

    def is_valid(self) -> bool:
        """
        Check if tree satisfies Red-Black properties.

        Returns:
            True if valid Red-Black tree.
        """
        if self._root is None:
            return True

        # Property 2: Root is black
        if self._root.color != Color.BLACK:
            return False

        # Check all other properties
        def check(node: Optional[RBNode[K, V]], black_count: int, path_black: List[int]) -> bool:
            if node is None:
                # Property 5: All paths have same black count
                if not path_black:
                    path_black.append(black_count)
                return black_count == path_black[0]

            # Property 4: Red node has black children
            if node.color == Color.RED:
                if node.left is not None and node.left.color == Color.RED:
                    return False
                if node.right is not None and node.right.color == Color.RED:
                    return False

            if node.color == Color.BLACK:
                black_count += 1

            return (check(node.left, black_count, path_black) and
                    check(node.right, black_count, path_black))

        return check(self._root, 0, [])

    def clear(self) -> None:
        """Remove all nodes."""
        self._root = None
        self._size = 0

    def __repr__(self) -> str:
        return f"RedBlackTree(size={self._size}, height={self.height()})"
