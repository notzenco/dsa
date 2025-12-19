"""
Binary Search Tree (BST)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  BST Property: For each node:                                              ║
║    - All values in left subtree < node value                               ║
║    - All values in right subtree > node value                              ║
║                                                                            ║
║  Example BST:                                                              ║
║                    ┌────────┐                                              ║
║                    │   50   │                                              ║
║                    └────────┘                                              ║
║                   /          \                                             ║
║           ┌────────┐      ┌────────┐                                       ║
║           │   30   │      │   70   │                                       ║
║           └────────┘      └────────┘                                       ║
║          /        \       /        \                                       ║
║    ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐                             ║
║    │   20   │ │   40   │ │   60   │ │   80   │                             ║
║    └────────┘ └────────┘ └────────┘ └────────┘                             ║
║                                                                            ║
║  Traversal Orders:                                                         ║
║  In-order   (L, Root, R): 20, 30, 40, 50, 60, 70, 80 (sorted!)             ║
║  Pre-order  (Root, L, R): 50, 30, 20, 40, 70, 60, 80                       ║
║  Post-order (L, R, Root): 20, 40, 30, 60, 80, 70, 50                       ║
║  Level-order (BFS):       50, 30, 70, 20, 40, 60, 80                       ║
║                                                                            ║
║  Insert 35:                                                                ║
║           50                                                               ║
║          /  \                                                              ║
║        30    70          35 < 50 → go left                                 ║
║       /  \               35 > 30 → go right                                ║
║     20   40              35 < 40 → go left                                 ║
║         /                Insert as left child of 40                        ║
║       [35]                                                                 ║
║                                                                            ║
║  Delete Node with 2 Children (replace with in-order successor):           ║
║  Delete 30: Replace with 40 (smallest in right subtree)                   ║
║           50                      50                                       ║
║          /  \        →           /  \                                      ║
║        30    70                40    70                                    ║
║       /  \                    /                                            ║
║     20   40                 20                                             ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Average     │ Worst       │ Space       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ Search              │ O(log n)    │ O(n)        │ O(1)        │
│ Insert              │ O(log n)    │ O(n)        │ O(1)        │
│ Delete              │ O(log n)    │ O(n)        │ O(1)        │
│ Min/Max             │ O(log n)    │ O(n)        │ O(1)        │
│ In-order traversal  │ O(n)        │ O(n)        │ O(h)        │
└─────────────────────┴─────────────┴─────────────┴─────────────┘
* Worst case occurs with skewed trees (essentially a linked list)
* h = height of tree (log n for balanced, n for skewed)

LEETCODE PROBLEMS:
- #700 Search in a Binary Search Tree
- #701 Insert into a Binary Search Tree
- #450 Delete Node in a BST
- #98 Validate Binary Search Tree
- #230 Kth Smallest Element in a BST
- #235 Lowest Common Ancestor of a BST
- #108 Convert Sorted Array to BST
- #173 Binary Search Tree Iterator
- #653 Two Sum IV - Input is a BST
- #538 Convert BST to Greater Tree
- #1038 Binary Search Tree to Greater Sum Tree

USE CASES:
- Maintaining sorted data with dynamic insertions/deletions
- Implementing sets and maps
- Priority queues (with modifications)
- Database indexing
- File system organization
- Expression parsing
"""

from typing import TypeVar, Generic, Iterator, Optional, List, Callable
from collections import deque

T = TypeVar('T')


class TreeNode(Generic[T]):
    """A node in a binary search tree."""

    __slots__ = ('value', 'left', 'right', 'parent')

    def __init__(
        self,
        value: T,
        left: Optional["TreeNode[T]"] = None,
        right: Optional["TreeNode[T]"] = None,
        parent: Optional["TreeNode[T]"] = None
    ) -> None:
        self.value: T = value
        self.left: Optional["TreeNode[T]"] = left
        self.right: Optional["TreeNode[T]"] = right
        self.parent: Optional["TreeNode[T]"] = parent

    def __repr__(self) -> str:
        return f"TreeNode({self.value})"

    @property
    def is_leaf(self) -> bool:
        """Return True if this is a leaf node."""
        return self.left is None and self.right is None

    @property
    def has_left_child(self) -> bool:
        """Return True if node has a left child."""
        return self.left is not None

    @property
    def has_right_child(self) -> bool:
        """Return True if node has a right child."""
        return self.right is not None

    @property
    def is_left_child(self) -> bool:
        """Return True if this is the left child of its parent."""
        return self.parent is not None and self.parent.left == self

    @property
    def is_right_child(self) -> bool:
        """Return True if this is the right child of its parent."""
        return self.parent is not None and self.parent.right == self


class BinarySearchTree(Generic[T]):
    """
    A binary search tree implementation.

    Supports duplicate values (stored in right subtree).
    Maintains parent pointers for efficient operations.
    """

    def __init__(self) -> None:
        """Initialize an empty BST."""
        self._root: Optional[TreeNode[T]] = None
        self._size: int = 0

    def __len__(self) -> int:
        """Return the number of nodes in the tree."""
        return self._size

    def __contains__(self, value: T) -> bool:
        """Check if value exists in the tree."""
        return self.search(value) is not None

    def __iter__(self) -> Iterator[T]:
        """Iterate over values in-order (sorted)."""
        return self.inorder()

    def __repr__(self) -> str:
        """Return string representation."""
        values = list(self.inorder())
        return f"BinarySearchTree({values})"

    @property
    def is_empty(self) -> bool:
        """Return True if tree is empty."""
        return self._root is None

    @property
    def root(self) -> Optional[T]:
        """Return root value."""
        return self._root.value if self._root else None

    @property
    def height(self) -> int:
        """
        Return the height of the tree.

        Height is -1 for empty tree, 0 for single node.
        """
        return self._height(self._root)

    def _height(self, node: Optional[TreeNode[T]]) -> int:
        """Calculate height recursively."""
        if node is None:
            return -1
        return 1 + max(self._height(node.left), self._height(node.right))

    def insert(self, value: T) -> TreeNode[T]:
        """
        Insert a value into the BST.

        Time Complexity: O(log n) average, O(n) worst case

        Args:
            value: Value to insert.

        Returns:
            The newly created node.
        """
        new_node = TreeNode(value)

        if self._root is None:
            self._root = new_node
            self._size += 1
            return new_node

        current = self._root
        while True:
            if value < current.value:
                if current.left is None:
                    current.left = new_node
                    new_node.parent = current
                    break
                current = current.left
            else:
                if current.right is None:
                    current.right = new_node
                    new_node.parent = current
                    break
                current = current.right

        self._size += 1
        return new_node

    def search(self, value: T) -> Optional[TreeNode[T]]:
        """
        Search for a value in the BST.

        Time Complexity: O(log n) average, O(n) worst case

        Args:
            value: Value to search for.

        Returns:
            The node containing the value, or None if not found.
        """
        current = self._root

        while current is not None:
            if value == current.value:
                return current
            elif value < current.value:
                current = current.left
            else:
                current = current.right

        return None

    def delete(self, value: T) -> bool:
        """
        Delete a value from the BST.

        Time Complexity: O(log n) average, O(n) worst case

        Args:
            value: Value to delete.

        Returns:
            True if value was found and deleted, False otherwise.
        """
        node = self.search(value)
        if node is None:
            return False

        self._delete_node(node)
        self._size -= 1
        return True

    def _delete_node(self, node: TreeNode[T]) -> None:
        """Delete a specific node from the tree."""
        # Case 1: Node is a leaf
        if node.is_leaf:
            self._replace_node(node, None)

        # Case 2: Node has one child
        elif node.left is None:
            self._replace_node(node, node.right)
        elif node.right is None:
            self._replace_node(node, node.left)

        # Case 3: Node has two children
        else:
            # Find in-order successor (minimum in right subtree)
            successor = self._minimum_node(node.right)

            # If successor is not the right child, replace it with its right child
            if successor.parent != node:
                self._replace_node(successor, successor.right)
                successor.right = node.right
                successor.right.parent = successor

            # Replace node with successor
            self._replace_node(node, successor)
            successor.left = node.left
            successor.left.parent = successor

    def _replace_node(
        self,
        node: TreeNode[T],
        replacement: Optional[TreeNode[T]]
    ) -> None:
        """Replace a node with another node (or None)."""
        if node.parent is None:
            self._root = replacement
        elif node.is_left_child:
            node.parent.left = replacement
        else:
            node.parent.right = replacement

        if replacement is not None:
            replacement.parent = node.parent

    def minimum(self) -> Optional[T]:
        """
        Find the minimum value in the tree.

        Time Complexity: O(log n) average, O(n) worst case

        Returns:
            The minimum value, or None if tree is empty.
        """
        if self._root is None:
            return None
        return self._minimum_node(self._root).value

    def maximum(self) -> Optional[T]:
        """
        Find the maximum value in the tree.

        Time Complexity: O(log n) average, O(n) worst case

        Returns:
            The maximum value, or None if tree is empty.
        """
        if self._root is None:
            return None
        return self._maximum_node(self._root).value

    def _minimum_node(self, node: TreeNode[T]) -> TreeNode[T]:
        """Find the node with minimum value in subtree."""
        current = node
        while current.left is not None:
            current = current.left
        return current

    def _maximum_node(self, node: TreeNode[T]) -> TreeNode[T]:
        """Find the node with maximum value in subtree."""
        current = node
        while current.right is not None:
            current = current.right
        return current

    def successor(self, value: T) -> Optional[T]:
        """
        Find the in-order successor of a value.

        The successor is the smallest value greater than the given value.

        Args:
            value: Value to find successor of.

        Returns:
            The successor value, or None if not found.
        """
        node = self.search(value)
        if node is None:
            return None

        # If right subtree exists, successor is minimum in right subtree
        if node.right is not None:
            return self._minimum_node(node.right).value

        # Otherwise, go up until we find a node that is a left child
        current = node
        parent = node.parent
        while parent is not None and current.is_right_child:
            current = parent
            parent = parent.parent

        return parent.value if parent else None

    def predecessor(self, value: T) -> Optional[T]:
        """
        Find the in-order predecessor of a value.

        The predecessor is the largest value smaller than the given value.

        Args:
            value: Value to find predecessor of.

        Returns:
            The predecessor value, or None if not found.
        """
        node = self.search(value)
        if node is None:
            return None

        # If left subtree exists, predecessor is maximum in left subtree
        if node.left is not None:
            return self._maximum_node(node.left).value

        # Otherwise, go up until we find a node that is a right child
        current = node
        parent = node.parent
        while parent is not None and current.is_left_child:
            current = parent
            parent = parent.parent

        return parent.value if parent else None

    def inorder(self) -> Iterator[T]:
        """
        Iterate values in-order (sorted order).

        Time Complexity: O(n)
        Space Complexity: O(h) where h is height

        Yields:
            Values in ascending order.
        """
        stack: List[TreeNode[T]] = []
        current = self._root

        while stack or current:
            # Go to leftmost node
            while current:
                stack.append(current)
                current = current.left

            # Process current node
            current = stack.pop()
            yield current.value

            # Move to right subtree
            current = current.right

    def preorder(self) -> Iterator[T]:
        """
        Iterate values in pre-order (root, left, right).

        Time Complexity: O(n)
        Space Complexity: O(h)

        Yields:
            Values in pre-order.
        """
        if self._root is None:
            return

        stack: List[TreeNode[T]] = [self._root]

        while stack:
            node = stack.pop()
            yield node.value

            # Push right first so left is processed first
            if node.right:
                stack.append(node.right)
            if node.left:
                stack.append(node.left)

    def postorder(self) -> Iterator[T]:
        """
        Iterate values in post-order (left, right, root).

        Time Complexity: O(n)
        Space Complexity: O(h)

        Yields:
            Values in post-order.
        """
        if self._root is None:
            return

        stack1: List[TreeNode[T]] = [self._root]
        stack2: List[TreeNode[T]] = []

        while stack1:
            node = stack1.pop()
            stack2.append(node)

            if node.left:
                stack1.append(node.left)
            if node.right:
                stack1.append(node.right)

        while stack2:
            yield stack2.pop().value

    def levelorder(self) -> Iterator[T]:
        """
        Iterate values in level-order (BFS).

        Time Complexity: O(n)
        Space Complexity: O(w) where w is max width

        Yields:
            Values level by level, left to right.
        """
        if self._root is None:
            return

        queue: deque[TreeNode[T]] = deque([self._root])

        while queue:
            node = queue.popleft()
            yield node.value

            if node.left:
                queue.append(node.left)
            if node.right:
                queue.append(node.right)

    def is_valid_bst(self) -> bool:
        """
        Check if the tree is a valid BST.

        Returns:
            True if valid BST, False otherwise.
        """
        def validate(
            node: Optional[TreeNode[T]],
            min_val: Optional[T],
            max_val: Optional[T]
        ) -> bool:
            if node is None:
                return True

            if min_val is not None and node.value <= min_val:
                return False
            if max_val is not None and node.value >= max_val:
                return False

            return (validate(node.left, min_val, node.value) and
                    validate(node.right, node.value, max_val))

        return validate(self._root, None, None)

    def kth_smallest(self, k: int) -> Optional[T]:
        """
        Find the kth smallest element (1-indexed).

        Time Complexity: O(h + k)
        Space Complexity: O(h)

        Args:
            k: The k value (1 = smallest).

        Returns:
            The kth smallest value, or None if k is invalid.
        """
        if k < 1 or k > self._size:
            return None

        count = 0
        for value in self.inorder():
            count += 1
            if count == k:
                return value

        return None

    def floor(self, value: T) -> Optional[T]:
        """
        Find the largest value <= given value.

        Args:
            value: The target value.

        Returns:
            Floor value, or None if no floor exists.
        """
        result: Optional[T] = None
        current = self._root

        while current is not None:
            if current.value == value:
                return value
            elif current.value < value:
                result = current.value
                current = current.right
            else:
                current = current.left

        return result

    def ceiling(self, value: T) -> Optional[T]:
        """
        Find the smallest value >= given value.

        Args:
            value: The target value.

        Returns:
            Ceiling value, or None if no ceiling exists.
        """
        result: Optional[T] = None
        current = self._root

        while current is not None:
            if current.value == value:
                return value
            elif current.value > value:
                result = current.value
                current = current.left
            else:
                current = current.right

        return result

    def range_query(self, low: T, high: T) -> List[T]:
        """
        Find all values in range [low, high].

        Time Complexity: O(h + m) where m is number of values in range
        Space Complexity: O(h + m)

        Args:
            low: Lower bound (inclusive).
            high: Upper bound (inclusive).

        Returns:
            List of values in range, sorted.
        """
        result: List[T] = []

        def traverse(node: Optional[TreeNode[T]]) -> None:
            if node is None:
                return

            if low <= node.value:
                traverse(node.left)

            if low <= node.value <= high:
                result.append(node.value)

            if node.value <= high:
                traverse(node.right)

        traverse(self._root)
        return result

    def clear(self) -> None:
        """Remove all nodes from the tree."""
        self._root = None
        self._size = 0

    def to_list(self) -> List[T]:
        """Convert to sorted list (in-order)."""
        return list(self.inorder())

    @classmethod
    def from_list(cls, values: List[T]) -> "BinarySearchTree[T]":
        """
        Create a BST from a list of values.

        Args:
            values: Values to insert.

        Returns:
            A new BST containing all values.
        """
        bst: BinarySearchTree[T] = cls()
        for value in values:
            bst.insert(value)
        return bst

    @classmethod
    def from_sorted_list(cls, values: List[T]) -> "BinarySearchTree[T]":
        """
        Create a balanced BST from a sorted list.

        Time Complexity: O(n)

        Args:
            values: Sorted list of values.

        Returns:
            A balanced BST.
        """
        bst: BinarySearchTree[T] = cls()

        def build(left: int, right: int, parent: Optional[TreeNode[T]]) -> Optional[TreeNode[T]]:
            if left > right:
                return None

            mid = (left + right) // 2
            node = TreeNode(values[mid], parent=parent)
            bst._size += 1

            node.left = build(left, mid - 1, node)
            node.right = build(mid + 1, right, node)

            return node

        bst._root = build(0, len(values) - 1, None)
        return bst

    def lowest_common_ancestor(self, val1: T, val2: T) -> Optional[T]:
        """
        Find the lowest common ancestor of two values.

        Time Complexity: O(h)

        Args:
            val1: First value.
            val2: Second value.

        Returns:
            LCA value, or None if either value doesn't exist.
        """
        # Ensure both values exist
        if self.search(val1) is None or self.search(val2) is None:
            return None

        current = self._root

        while current is not None:
            if val1 < current.value and val2 < current.value:
                current = current.left
            elif val1 > current.value and val2 > current.value:
                current = current.right
            else:
                return current.value

        return None
