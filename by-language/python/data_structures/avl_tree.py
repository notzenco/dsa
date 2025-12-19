"""
AVL Tree Implementation.

A self-balancing binary search tree where the heights of left and right
subtrees differ by at most 1 for every node.

================================================================================
                          AVL TREE VISUAL REPRESENTATION
================================================================================

    Unbalanced BST after inserting 1,2,3:    AVL Tree (auto-balanced):

          1                                       2
           \                                     / \
            2           --rotation-->           1   3
             \
              3

    Balance Factor (BF) = height(left) - height(right)
    Valid BF: -1, 0, +1

    Rotations:

    Right Rotation (LL case):        Left Rotation (RR case):

        z                                  z
       / \                                / \
      y   T4    -->      y               T1  y           -->        y
     / \                / \                 / \                    / \
    x   T3             x   z               T2  x                  z   x
   / \                / \ / \                 / \                / \ / \
  T1 T2              T1 T2 T3 T4             T3 T4              T1 T2 T3 T4

================================================================================

COMPLEXITY:
+-----------+----------+-------+
| Operation | Time     | Space |
+-----------+----------+-------+
| Search    | O(log n) | O(1)  |
| Insert    | O(log n) | O(1)  |
| Delete    | O(log n) | O(1)  |
| Min/Max   | O(log n) | O(1)  |
+-----------+----------+-------+

LEETCODE PROBLEMS:
- #110 Balanced Binary Tree
- #108 Convert Sorted Array to BST
- #1382 Balance a Binary Search Tree

USE CASES:
- Database indexing
- In-memory sorted containers
- When frequent insertions/deletions need guaranteed O(log n)
"""

from typing import TypeVar, Generic, Optional, List, Iterator, Tuple

T = TypeVar('T')


class AVLNode(Generic[T]):
    """A node in the AVL tree."""

    __slots__ = ['key', 'value', 'height', 'left', 'right']

    def __init__(self, key: T, value: any = None) -> None:
        """Initialize AVL node."""
        self.key = key
        self.value = value if value is not None else key
        self.height = 1
        self.left: Optional['AVLNode[T]'] = None
        self.right: Optional['AVLNode[T]'] = None


class AVLTree(Generic[T]):
    """
    AVL Tree - Self-balancing Binary Search Tree.

    Maintains balance by ensuring the height difference between
    left and right subtrees is at most 1 for every node.

    Example:
        >>> tree = AVLTree()
        >>> for i in [3, 1, 4, 1, 5, 9, 2, 6]:
        ...     tree.insert(i)
        >>> tree.search(5)
        True
        >>> tree.inorder()
        [1, 1, 2, 3, 4, 5, 6, 9]
    """

    def __init__(self) -> None:
        """Initialize empty AVL tree."""
        self._root: Optional[AVLNode[T]] = None
        self._size = 0

    def _height(self, node: Optional[AVLNode[T]]) -> int:
        """Get height of node (None has height 0)."""
        return node.height if node else 0

    def _balance_factor(self, node: Optional[AVLNode[T]]) -> int:
        """Get balance factor of node."""
        if not node:
            return 0
        return self._height(node.left) - self._height(node.right)

    def _update_height(self, node: AVLNode[T]) -> None:
        """Update height of node based on children."""
        node.height = 1 + max(self._height(node.left), self._height(node.right))

    def _rotate_right(self, z: AVLNode[T]) -> AVLNode[T]:
        """
        Right rotation for LL imbalance.

            z                y
           / \              / \
          y   T4    -->    x   z
         / \                  / \
        x   T3               T3  T4
        """
        y = z.left
        T3 = y.right

        y.right = z
        z.left = T3

        self._update_height(z)
        self._update_height(y)

        return y

    def _rotate_left(self, z: AVLNode[T]) -> AVLNode[T]:
        """
        Left rotation for RR imbalance.

          z                    y
         / \                  / \
        T1  y      -->       z   x
           / \              / \
          T2  x            T1  T2
        """
        y = z.right
        T2 = y.left

        y.left = z
        z.right = T2

        self._update_height(z)
        self._update_height(y)

        return y

    def _rebalance(self, node: AVLNode[T]) -> AVLNode[T]:
        """Rebalance node if needed."""
        self._update_height(node)
        balance = self._balance_factor(node)

        # Left heavy
        if balance > 1:
            # Left-Right case: first rotate left on left child
            if self._balance_factor(node.left) < 0:
                node.left = self._rotate_left(node.left)
            # Left-Left case
            return self._rotate_right(node)

        # Right heavy
        if balance < -1:
            # Right-Left case: first rotate right on right child
            if self._balance_factor(node.right) > 0:
                node.right = self._rotate_right(node.right)
            # Right-Right case
            return self._rotate_left(node)

        return node

    def insert(self, key: T, value: any = None) -> None:
        """
        Insert a key-value pair into the tree.

        Args:
            key: The key to insert
            value: Optional value (defaults to key)

        Time: O(log n)
        """
        self._root = self._insert(self._root, key, value)
        self._size += 1

    def _insert(self, node: Optional[AVLNode[T]], key: T, value: any) -> AVLNode[T]:
        """Recursive insert helper."""
        if not node:
            return AVLNode(key, value)

        if key < node.key:
            node.left = self._insert(node.left, key, value)
        elif key > node.key:
            node.right = self._insert(node.right, key, value)
        else:
            # Key exists, update value
            node.value = value if value is not None else key
            self._size -= 1  # Compensate for increment in insert()
            return node

        return self._rebalance(node)

    def delete(self, key: T) -> bool:
        """
        Delete a key from the tree.

        Args:
            key: The key to delete

        Returns:
            True if key was deleted, False if not found

        Time: O(log n)
        """
        if not self.search(key):
            return False

        self._root = self._delete(self._root, key)
        self._size -= 1
        return True

    def _delete(self, node: Optional[AVLNode[T]], key: T) -> Optional[AVLNode[T]]:
        """Recursive delete helper."""
        if not node:
            return None

        if key < node.key:
            node.left = self._delete(node.left, key)
        elif key > node.key:
            node.right = self._delete(node.right, key)
        else:
            # Node to delete found
            if not node.left:
                return node.right
            if not node.right:
                return node.left

            # Two children: replace with inorder successor
            successor = self._min_node(node.right)
            node.key = successor.key
            node.value = successor.value
            node.right = self._delete(node.right, successor.key)

        return self._rebalance(node)

    def _min_node(self, node: AVLNode[T]) -> AVLNode[T]:
        """Find node with minimum key."""
        while node.left:
            node = node.left
        return node

    def search(self, key: T) -> bool:
        """
        Check if key exists in tree.

        Time: O(log n)
        """
        return self._search(self._root, key) is not None

    def _search(self, node: Optional[AVLNode[T]], key: T) -> Optional[AVLNode[T]]:
        """Recursive search helper."""
        if not node:
            return None

        if key == node.key:
            return node
        elif key < node.key:
            return self._search(node.left, key)
        else:
            return self._search(node.right, key)

    def get(self, key: T) -> Optional[any]:
        """Get value for key."""
        node = self._search(self._root, key)
        return node.value if node else None

    def minimum(self) -> Optional[T]:
        """Get minimum key."""
        if not self._root:
            return None
        return self._min_node(self._root).key

    def maximum(self) -> Optional[T]:
        """Get maximum key."""
        if not self._root:
            return None
        node = self._root
        while node.right:
            node = node.right
        return node.key

    def inorder(self) -> List[T]:
        """Return keys in sorted order."""
        result = []
        self._inorder(self._root, result)
        return result

    def _inorder(self, node: Optional[AVLNode[T]], result: List[T]) -> None:
        """Recursive inorder traversal."""
        if node:
            self._inorder(node.left, result)
            result.append(node.key)
            self._inorder(node.right, result)

    def preorder(self) -> List[T]:
        """Return keys in preorder."""
        result = []
        self._preorder(self._root, result)
        return result

    def _preorder(self, node: Optional[AVLNode[T]], result: List[T]) -> None:
        """Recursive preorder traversal."""
        if node:
            result.append(node.key)
            self._preorder(node.left, result)
            self._preorder(node.right, result)

    def level_order(self) -> List[List[T]]:
        """Return keys level by level."""
        if not self._root:
            return []

        from collections import deque
        result = []
        queue = deque([self._root])

        while queue:
            level = []
            for _ in range(len(queue)):
                node = queue.popleft()
                level.append(node.key)
                if node.left:
                    queue.append(node.left)
                if node.right:
                    queue.append(node.right)
            result.append(level)

        return result

    def is_balanced(self) -> bool:
        """Check if tree is balanced (should always be True for AVL)."""
        return self._is_balanced(self._root)[0]

    def _is_balanced(self, node: Optional[AVLNode[T]]) -> Tuple[bool, int]:
        """Check balance and return (is_balanced, height)."""
        if not node:
            return (True, 0)

        left_balanced, left_height = self._is_balanced(node.left)
        right_balanced, right_height = self._is_balanced(node.right)

        balanced = (left_balanced and right_balanced and
                    abs(left_height - right_height) <= 1)

        return (balanced, 1 + max(left_height, right_height))

    def height(self) -> int:
        """Return height of tree."""
        return self._height(self._root)

    def clear(self) -> None:
        """Remove all elements."""
        self._root = None
        self._size = 0

    def __len__(self) -> int:
        """Return number of nodes."""
        return self._size

    def __contains__(self, key: T) -> bool:
        """Check if key exists."""
        return self.search(key)

    def __iter__(self) -> Iterator[T]:
        """Iterate over keys in sorted order."""
        return iter(self.inorder())

    def __repr__(self) -> str:
        """Return string representation."""
        return f"AVLTree(size={self._size}, height={self.height()})"


class AVLTreeMap(Generic[T]):
    """
    AVL Tree based Map/Dictionary.

    Provides dictionary-like interface with guaranteed O(log n) operations.

    Example:
        >>> m = AVLTreeMap()
        >>> m['b'] = 2
        >>> m['a'] = 1
        >>> m['c'] = 3
        >>> list(m.keys())
        ['a', 'b', 'c']
    """

    def __init__(self) -> None:
        """Initialize empty map."""
        self._tree = AVLTree()

    def __setitem__(self, key: T, value: any) -> None:
        """Set key-value pair."""
        self._tree.insert(key, value)

    def __getitem__(self, key: T) -> any:
        """Get value for key."""
        value = self._tree.get(key)
        if value is None and not self._tree.search(key):
            raise KeyError(key)
        return value

    def __delitem__(self, key: T) -> None:
        """Delete key."""
        if not self._tree.delete(key):
            raise KeyError(key)

    def __contains__(self, key: T) -> bool:
        """Check if key exists."""
        return self._tree.search(key)

    def __len__(self) -> int:
        """Return number of items."""
        return len(self._tree)

    def __iter__(self) -> Iterator[T]:
        """Iterate over keys in sorted order."""
        return iter(self._tree)

    def get(self, key: T, default: any = None) -> any:
        """Get value with default."""
        value = self._tree.get(key)
        if value is None and not self._tree.search(key):
            return default
        return value

    def keys(self) -> List[T]:
        """Return sorted keys."""
        return self._tree.inorder()

    def items(self) -> List[Tuple[T, any]]:
        """Return sorted (key, value) pairs."""
        result = []
        self._collect_items(self._tree._root, result)
        return result

    def _collect_items(self, node: Optional[AVLNode[T]], result: List) -> None:
        """Collect key-value pairs in order."""
        if node:
            self._collect_items(node.left, result)
            result.append((node.key, node.value))
            self._collect_items(node.right, result)

    def floor(self, key: T) -> Optional[T]:
        """Find largest key <= given key."""
        result = None
        node = self._tree._root

        while node:
            if node.key == key:
                return key
            elif node.key < key:
                result = node.key
                node = node.right
            else:
                node = node.left

        return result

    def ceiling(self, key: T) -> Optional[T]:
        """Find smallest key >= given key."""
        result = None
        node = self._tree._root

        while node:
            if node.key == key:
                return key
            elif node.key > key:
                result = node.key
                node = node.left
            else:
                node = node.right

        return result

    def range_query(self, low: T, high: T) -> List[T]:
        """Return all keys in range [low, high]."""
        result = []
        self._range_query(self._tree._root, low, high, result)
        return result

    def _range_query(self, node: Optional[AVLNode[T]], low: T, high: T,
                     result: List[T]) -> None:
        """Collect keys in range."""
        if not node:
            return

        if low < node.key:
            self._range_query(node.left, low, high, result)

        if low <= node.key <= high:
            result.append(node.key)

        if high > node.key:
            self._range_query(node.right, low, high, result)
