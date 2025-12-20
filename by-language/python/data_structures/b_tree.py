"""
B-Tree Data Structure.

╔══════════════════════════════════════════════════════════════════════════════╗
║                                  B-TREE                                      ║
╠══════════════════════════════════════════════════════════════════════════════╣
║                                                                              ║
║  A self-balancing tree optimized for disk access, where each node can        ║
║  contain multiple keys and children.                                         ║
║                                                                              ║
║  B-Tree of order t (minimum degree):                                         ║
║  - Every node has at most 2t - 1 keys                                        ║
║  - Every node has at most 2t children                                        ║
║  - Every non-root node has at least t - 1 keys                              ║
║  - Root has at least 1 key (if non-empty)                                   ║
║  - All leaves are at the same level                                         ║
║                                                                              ║
║  Example B-Tree (t=2, so 1-3 keys per node, 2-4 children):                  ║
║                                                                              ║
║                           ┌─────────────┐                                   ║
║                           │   [10, 20]  │                                   ║
║                           └──────┬──────┘                                   ║
║               ┌──────────────────┼──────────────────┐                       ║
║               ▼                  ▼                  ▼                       ║
║        ┌──────────┐      ┌──────────┐      ┌─────────────┐                 ║
║        │ [3, 5, 7]│      │[12,15,17]│      │[25, 30, 40] │                 ║
║        └──────────┘      └──────────┘      └─────────────┘                 ║
║                                                                              ║
║  SPLIT OPERATION (when node is full):                                       ║
║                                                                              ║
║    Before:  [1, 2, 3, 4, 5]  (full node, t=3)                              ║
║                                                                              ║
║    After:        [3]         (middle key moves up)                          ║
║                 /   \                                                        ║
║            [1,2]    [4,5]                                                   ║
║                                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌───────────────┬─────────────────────┬───────────────┐
│ Operation     │ Time                │ Disk Accesses │
├───────────────┼─────────────────────┼───────────────┤
│ Search        │ O(t * log_t(n))     │ O(log_t(n))   │
│ Insert        │ O(t * log_t(n))     │ O(log_t(n))   │
│ Delete        │ O(t * log_t(n))     │ O(log_t(n))   │
│ Space         │ O(n)                │ -             │
└───────────────┴─────────────────────┴───────────────┘

where t = minimum degree, n = number of keys

WHY B-TREES?
- Optimized for systems that read/write large blocks of data
- Minimizes disk I/O by storing many keys per node
- Each node typically fits in one disk block/page

COMPARISON:
- B-Tree: Keys in all nodes, faster for random lookups
- B+ Tree: Keys only in leaves, better for range queries

USE CASES:
- Database indexes (MySQL, PostgreSQL)
- File systems (NTFS, ext4, HFS+)
- Key-value stores
"""

from typing import TypeVar, Generic, Optional, List, Tuple

K = TypeVar('K')
V = TypeVar('V')


class BTreeNode(Generic[K, V]):
    """Node in a B-Tree."""

    def __init__(self, leaf: bool = True) -> None:
        """
        Initialize B-Tree node.

        Args:
            leaf: True if this is a leaf node.
        """
        self.keys: List[K] = []
        self.values: List[V] = []
        self.children: List['BTreeNode[K, V]'] = []
        self.leaf = leaf

    def __repr__(self) -> str:
        return f"BTreeNode(keys={self.keys}, leaf={self.leaf})"


class BTree(Generic[K, V]):
    """
    B-Tree implementation.

    A self-balancing tree where each node can have multiple keys
    and children, optimized for disk-based storage systems.
    """

    def __init__(self, t: int = 2) -> None:
        """
        Initialize B-Tree.

        Args:
            t: Minimum degree. Each node has:
               - At most 2t - 1 keys
               - At least t - 1 keys (except root)
               - At most 2t children
        """
        if t < 2:
            raise ValueError("Minimum degree t must be at least 2")

        self._t = t
        self._root: BTreeNode[K, V] = BTreeNode(leaf=True)
        self._size = 0

    @property
    def order(self) -> int:
        """Return order (maximum children per node)."""
        return 2 * self._t

    @property
    def min_degree(self) -> int:
        """Return minimum degree."""
        return self._t

    def __len__(self) -> int:
        """Return number of keys."""
        return self._size

    # =========================================================================
    # SEARCH
    # =========================================================================

    def search(self, key: K) -> Optional[V]:
        """
        Search for key in tree.

        Time: O(t * log_t(n))

        Args:
            key: Key to search for.

        Returns:
            Value if found, None otherwise.
        """
        return self._search(self._root, key)

    def _search(self, node: BTreeNode[K, V], key: K) -> Optional[V]:
        """Recursive search in subtree."""
        # Find first key >= search key
        i = 0
        while i < len(node.keys) and key > node.keys[i]:
            i += 1

        # Check if key found
        if i < len(node.keys) and key == node.keys[i]:
            return node.values[i]

        # If leaf, key not in tree
        if node.leaf:
            return None

        # Recurse to appropriate child
        return self._search(node.children[i], key)

    def __contains__(self, key: K) -> bool:
        """Check if key exists."""
        return self.search(key) is not None

    def __getitem__(self, key: K) -> V:
        """Get value by key."""
        result = self.search(key)
        if result is None:
            raise KeyError(key)
        return result

    # =========================================================================
    # INSERTION
    # =========================================================================

    def insert(self, key: K, value: V) -> None:
        """
        Insert key-value pair.

        Time: O(t * log_t(n))
        """
        root = self._root

        # If root is full, split it first
        if len(root.keys) == 2 * self._t - 1:
            new_root: BTreeNode[K, V] = BTreeNode(leaf=False)
            new_root.children.append(self._root)
            self._split_child(new_root, 0)
            self._root = new_root

        self._insert_non_full(self._root, key, value)
        self._size += 1

    def _split_child(self, parent: BTreeNode[K, V], i: int) -> None:
        """
        Split the i-th child of parent.

        The child must be full (have 2t-1 keys).
        """
        t = self._t
        full_child = parent.children[i]

        # Save original data
        original_keys = full_child.keys[:]
        original_values = full_child.values[:]
        original_children = full_child.children[:]

        # The median is at index t-1
        median_key = original_keys[t - 1]
        median_value = original_values[t - 1]

        # Create new node to hold right half
        new_node: BTreeNode[K, V] = BTreeNode(leaf=full_child.leaf)

        # Left child keeps first t-1 keys
        full_child.keys = original_keys[:t - 1]
        full_child.values = original_values[:t - 1]

        # New node gets keys after median (indices t to 2t-2)
        new_node.keys = original_keys[t:]
        new_node.values = original_values[t:]

        # Move children if not leaf
        if not full_child.leaf:
            full_child.children = original_children[:t]
            new_node.children = original_children[t:]

        # Insert median into parent
        parent.keys.insert(i, median_key)
        parent.values.insert(i, median_value)
        parent.children.insert(i + 1, new_node)

    def _insert_non_full(self, node: BTreeNode[K, V], key: K, value: V) -> None:
        """Insert into a node that is not full."""
        i = len(node.keys) - 1

        if node.leaf:
            # Find position and insert
            while i >= 0 and key < node.keys[i]:
                i -= 1

            # Check for duplicate key
            if i >= 0 and node.keys[i] == key:
                node.values[i] = value  # Update value
                self._size -= 1  # Didn't actually add new key
                return

            node.keys.insert(i + 1, key)
            node.values.insert(i + 1, value)
        else:
            # Find child to recurse into
            while i >= 0 and key < node.keys[i]:
                i -= 1

            # Check for duplicate key in this node
            if i >= 0 and node.keys[i] == key:
                node.values[i] = value
                self._size -= 1
                return

            i += 1

            # Split child if full
            if len(node.children[i].keys) == 2 * self._t - 1:
                self._split_child(node, i)
                if key > node.keys[i]:
                    i += 1
                elif key == node.keys[i]:
                    node.values[i] = value
                    self._size -= 1
                    return

            self._insert_non_full(node.children[i], key, value)

    def __setitem__(self, key: K, value: V) -> None:
        """Set value by key."""
        self.insert(key, value)

    # =========================================================================
    # DELETION
    # =========================================================================

    def delete(self, key: K) -> bool:
        """
        Delete key from tree.

        Time: O(t * log_t(n))

        Returns:
            True if key was found and deleted.
        """
        if key not in self:
            return False

        self._delete(self._root, key)
        self._size -= 1

        # If root is empty but has children, make first child new root
        if len(self._root.keys) == 0 and not self._root.leaf:
            self._root = self._root.children[0]

        return True

    def _delete(self, node: BTreeNode[K, V], key: K) -> None:
        """Delete key from subtree rooted at node."""
        t = self._t
        i = 0

        # Find key or child to recurse into
        while i < len(node.keys) and key > node.keys[i]:
            i += 1

        if node.leaf:
            # Case 1: Key is in leaf node
            if i < len(node.keys) and node.keys[i] == key:
                node.keys.pop(i)
                node.values.pop(i)
            return

        if i < len(node.keys) and node.keys[i] == key:
            # Case 2: Key is in internal node
            self._delete_internal(node, i)
        else:
            # Case 3: Key is in subtree
            self._delete_subtree(node, i, key)

    def _delete_internal(self, node: BTreeNode[K, V], i: int) -> None:
        """Delete key at index i from internal node."""
        t = self._t
        key = node.keys[i]

        if len(node.children[i].keys) >= t:
            # Case 2a: Left child has enough keys
            predecessor = self._get_predecessor(node, i)
            node.keys[i] = predecessor[0]
            node.values[i] = predecessor[1]
            self._delete(node.children[i], predecessor[0])
        elif len(node.children[i + 1].keys) >= t:
            # Case 2b: Right child has enough keys
            successor = self._get_successor(node, i)
            node.keys[i] = successor[0]
            node.values[i] = successor[1]
            self._delete(node.children[i + 1], successor[0])
        else:
            # Case 2c: Both children have t-1 keys, merge
            self._merge(node, i)
            self._delete(node.children[i], key)

    def _delete_subtree(self, node: BTreeNode[K, V], i: int, key: K) -> None:
        """Delete key from subtree at child i."""
        t = self._t

        if len(node.children[i].keys) < t:
            # Child has minimum keys, need to fix first
            if i > 0 and len(node.children[i - 1].keys) >= t:
                # Borrow from left sibling
                self._borrow_from_left(node, i)
            elif i < len(node.children) - 1 and len(node.children[i + 1].keys) >= t:
                # Borrow from right sibling
                self._borrow_from_right(node, i)
            else:
                # Merge with sibling
                if i < len(node.children) - 1:
                    self._merge(node, i)
                else:
                    self._merge(node, i - 1)
                    i -= 1

        self._delete(node.children[i], key)

    def _get_predecessor(self, node: BTreeNode[K, V], i: int) -> Tuple[K, V]:
        """Get predecessor (rightmost key in left subtree)."""
        current = node.children[i]
        while not current.leaf:
            current = current.children[-1]
        return (current.keys[-1], current.values[-1])

    def _get_successor(self, node: BTreeNode[K, V], i: int) -> Tuple[K, V]:
        """Get successor (leftmost key in right subtree)."""
        current = node.children[i + 1]
        while not current.leaf:
            current = current.children[0]
        return (current.keys[0], current.values[0])

    def _borrow_from_left(self, node: BTreeNode[K, V], i: int) -> None:
        """Borrow a key from left sibling."""
        child = node.children[i]
        left_sibling = node.children[i - 1]

        # Move key from parent to child
        child.keys.insert(0, node.keys[i - 1])
        child.values.insert(0, node.values[i - 1])

        # Move key from sibling to parent
        node.keys[i - 1] = left_sibling.keys.pop()
        node.values[i - 1] = left_sibling.values.pop()

        # Move child pointer if not leaf
        if not child.leaf:
            child.children.insert(0, left_sibling.children.pop())

    def _borrow_from_right(self, node: BTreeNode[K, V], i: int) -> None:
        """Borrow a key from right sibling."""
        child = node.children[i]
        right_sibling = node.children[i + 1]

        # Move key from parent to child
        child.keys.append(node.keys[i])
        child.values.append(node.values[i])

        # Move key from sibling to parent
        node.keys[i] = right_sibling.keys.pop(0)
        node.values[i] = right_sibling.values.pop(0)

        # Move child pointer if not leaf
        if not child.leaf:
            child.children.append(right_sibling.children.pop(0))

    def _merge(self, node: BTreeNode[K, V], i: int) -> None:
        """Merge child i with child i+1."""
        left = node.children[i]
        right = node.children[i + 1]

        # Move separator key from parent to left child
        left.keys.append(node.keys[i])
        left.values.append(node.values[i])

        # Move all keys from right to left
        left.keys.extend(right.keys)
        left.values.extend(right.values)

        # Move children if not leaf
        if not left.leaf:
            left.children.extend(right.children)

        # Remove separator and right child from parent
        node.keys.pop(i)
        node.values.pop(i)
        node.children.pop(i + 1)

    def __delitem__(self, key: K) -> None:
        """Delete by key."""
        if not self.delete(key):
            raise KeyError(key)

    # =========================================================================
    # TRAVERSAL
    # =========================================================================

    def inorder(self) -> List[Tuple[K, V]]:
        """Return all key-value pairs in sorted order."""
        result: List[Tuple[K, V]] = []
        self._inorder(self._root, result)
        return result

    def _inorder(self, node: BTreeNode[K, V], result: List[Tuple[K, V]]) -> None:
        """Recursive inorder traversal."""
        for i in range(len(node.keys)):
            if not node.leaf:
                self._inorder(node.children[i], result)
            result.append((node.keys[i], node.values[i]))

        if not node.leaf and node.children:
            self._inorder(node.children[-1], result)

    def __iter__(self):
        """Iterate keys in sorted order."""
        for key, _ in self.inorder():
            yield key

    def items(self):
        """Iterate (key, value) pairs."""
        for item in self.inorder():
            yield item

    def keys(self) -> List[K]:
        """Return all keys in sorted order."""
        return [k for k, _ in self.inorder()]

    def values(self) -> List[V]:
        """Return all values."""
        return [v for _, v in self.inorder()]

    # =========================================================================
    # UTILITY
    # =========================================================================

    def min(self) -> Optional[Tuple[K, V]]:
        """Return minimum key-value pair."""
        if len(self._root.keys) == 0:
            return None

        node = self._root
        while not node.leaf:
            node = node.children[0]
        return (node.keys[0], node.values[0])

    def max(self) -> Optional[Tuple[K, V]]:
        """Return maximum key-value pair."""
        if len(self._root.keys) == 0:
            return None

        node = self._root
        while not node.leaf:
            node = node.children[-1]
        return (node.keys[-1], node.values[-1])

    def height(self) -> int:
        """Return height of tree (all leaves at same level)."""
        height = 0
        node = self._root
        while not node.leaf:
            height += 1
            node = node.children[0]
        return height

    def clear(self) -> None:
        """Remove all keys."""
        self._root = BTreeNode(leaf=True)
        self._size = 0

    def __repr__(self) -> str:
        return f"BTree(t={self._t}, size={self._size}, height={self.height()})"

    def display(self) -> str:
        """Return string representation of tree structure."""
        lines: List[str] = []
        self._display(self._root, "", True, lines)
        return "\n".join(lines)

    def _display(
        self,
        node: BTreeNode[K, V],
        prefix: str,
        is_last: bool,
        lines: List[str]
    ) -> None:
        """Helper for display."""
        connector = "└── " if is_last else "├── "
        lines.append(prefix + connector + str(node.keys))

        if not node.leaf:
            child_prefix = prefix + ("    " if is_last else "│   ")
            for i, child in enumerate(node.children):
                self._display(child, child_prefix, i == len(node.children) - 1, lines)
