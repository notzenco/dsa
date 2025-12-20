# Trees

Trees are hierarchical data structures consisting of nodes connected by edges, with a single root node and no cycles.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Binary Search Tree | [binary_search_tree.py](../../by-language/python/data_structures/binary_search_tree.py) | [binary_search_tree.c](../../by-language/c/data-structures/binary_search_tree.c) |
| AVL Tree | [avl_tree.py](../../by-language/python/data_structures/avl_tree.py) | [avl_tree.c](../../by-language/c/data-structures/avl_tree.c) |
| Red-Black Tree | [red_black_tree.py](../../by-language/python/data_structures/red_black_tree.py) | [red_black_tree.c](../../by-language/c/data-structures/red_black_tree.c) |
| B-Tree | [b_tree.py](../../by-language/python/data_structures/b_tree.py) | [b_tree.c](../../by-language/c/data-structures/b_tree.c) |
| Trie | [trie.py](../../by-language/python/data_structures/trie.py) | [trie.c](../../by-language/c/data-structures/trie.c) |
| Segment Tree | [segment_tree.py](../../by-language/python/data_structures/segment_tree.py) | [segment_tree.c](../../by-language/c/data-structures/segment_tree.c) |
| Fenwick Tree | [fenwick_tree.py](../../by-language/python/data_structures/fenwick_tree.py) | [fenwick_tree.c](../../by-language/c/data-structures/fenwick_tree.c) |

## Complexity

### Binary Search Tree
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(log n)│ O(n)    │         │
│ Insert      │ O(log n)│ O(n)    │ O(n)    │
│ Delete      │ O(log n)│ O(n)    │         │
│ Min/Max     │ O(log n)│ O(n)    │         │
└─────────────┴─────────┴─────────┴─────────┘
```

### Balanced Trees (AVL, Red-Black)
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(log n)│ O(log n)│         │
│ Insert      │ O(log n)│ O(log n)│ O(n)    │
│ Delete      │ O(log n)│ O(log n)│         │
└─────────────┴─────────┴─────────┴─────────┘
```

### Trie
```
┌─────────────┬─────────┬─────────────────────┐
│ Operation   │ Time    │ Space               │
├─────────────┼─────────┼─────────────────────┤
│ Search      │ O(m)    │                     │
│ Insert      │ O(m)    │ O(ALPHABET * m * n) │
│ Delete      │ O(m)    │                     │
│ Prefix      │ O(m)    │                     │
└─────────────┴─────────┴─────────────────────┘
m = key length, n = number of keys
```

### Segment Tree / Fenwick Tree
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Build       │ O(n)    │ O(n)    │
│ Query       │ O(log n)│         │
│ Update      │ O(log n)│         │
└─────────────┴─────────┴─────────┘
```

## Tree Types Comparison

### AVL vs Red-Black Tree
| Aspect | AVL | Red-Black |
|--------|-----|-----------|
| Balance | Stricter | Looser |
| Height | ~1.44 log n | ~2 log n |
| Lookups | Faster | Slower |
| Inserts | More rotations | Fewer rotations |
| Use Case | Read-heavy | Write-heavy |

### Segment Tree vs Fenwick Tree
| Aspect | Segment Tree | Fenwick Tree |
|--------|--------------|--------------|
| Space | 4n | n |
| Code complexity | Higher | Lower |
| Range updates | With lazy prop | Limited |
| Operations | Any associative | Mainly sum |

## Tree Traversals

### Depth-First Traversals
```
        1
       / \
      2   3
     / \
    4   5

Preorder  (Root-Left-Right): 1, 2, 4, 5, 3
Inorder   (Left-Root-Right): 4, 2, 5, 1, 3
Postorder (Left-Right-Root): 4, 5, 2, 3, 1
```

### Breadth-First (Level Order)
```
Level 0: 1
Level 1: 2, 3
Level 2: 4, 5
```

## Common Patterns

### Recursive DFS Template
```python
def dfs(node):
    if not node:
        return base_case

    left = dfs(node.left)
    right = dfs(node.right)

    return combine(node.val, left, right)
```

### Iterative Traversals
```python
# Inorder (using stack)
def inorder(root):
    result, stack = [], []
    curr = root
    while curr or stack:
        while curr:
            stack.append(curr)
            curr = curr.left
        curr = stack.pop()
        result.append(curr.val)
        curr = curr.right
    return result
```

### Morris Traversal (O(1) space)
Thread the tree using null pointers for traversal without stack.

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 94 | Binary Tree Inorder Traversal | Easy | Traversal |
| 98 | Validate Binary Search Tree | Medium | BST property |
| 100 | Same Tree | Easy | Recursion |
| 101 | Symmetric Tree | Easy | Mirror check |
| 102 | Binary Tree Level Order Traversal | Medium | BFS |
| 104 | Maximum Depth of Binary Tree | Easy | DFS |
| 105 | Construct BT from Preorder and Inorder | Medium | Construction |
| 108 | Convert Sorted Array to BST | Easy | Construction |
| 110 | Balanced Binary Tree | Easy | Height check |
| 112 | Path Sum | Easy | DFS |
| 114 | Flatten Binary Tree to Linked List | Medium | Morris-like |
| 124 | Binary Tree Maximum Path Sum | Hard | Tree DP |
| 144 | Binary Tree Preorder Traversal | Easy | Traversal |
| 145 | Binary Tree Postorder Traversal | Easy | Traversal |
| 173 | Binary Search Tree Iterator | Medium | Controlled traversal |
| 199 | Binary Tree Right Side View | Medium | BFS/DFS |
| 208 | Implement Trie | Medium | Trie |
| 211 | Design Add and Search Words | Medium | Trie + DFS |
| 212 | Word Search II | Hard | Trie + backtracking |
| 226 | Invert Binary Tree | Easy | Recursion |
| 230 | Kth Smallest Element in BST | Medium | Inorder |
| 235 | LCA of BST | Medium | BST property |
| 236 | Lowest Common Ancestor | Medium | DFS |
| 297 | Serialize and Deserialize BT | Hard | Traversal |
| 307 | Range Sum Query - Mutable | Medium | Segment/Fenwick |
| 315 | Count of Smaller Numbers After Self | Hard | Segment tree |
| 337 | House Robber III | Medium | Tree DP |
| 450 | Delete Node in BST | Medium | BST operations |
| 543 | Diameter of Binary Tree | Easy | Tree DP |
| 572 | Subtree of Another Tree | Easy | Matching |
| 617 | Merge Two Binary Trees | Easy | Recursion |
| 662 | Maximum Width of Binary Tree | Medium | BFS |
| 863 | All Nodes Distance K | Medium | Graph conversion |

## Specialized Trees

### Trie (Prefix Tree)
**Use Cases**: Autocomplete, spell checkers, IP routing, word games

### Segment Tree
**Use Cases**: Range queries (sum, min, max, GCD), range updates

### Fenwick Tree (Binary Indexed Tree)
**Use Cases**: Prefix sums, point updates, simpler than segment tree

### B-Tree
**Use Cases**: Database indexing, file systems, disk-based storage

## See Also

- [Graphs](../graphs/README.md) - Trees are acyclic graphs
- [Heaps](../heaps/README.md) - Complete binary tree structure
- [Stacks](../stacks/README.md) - Tree traversal with stack
- [Queues](../queues/README.md) - Level order traversal
