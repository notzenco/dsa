# DSA Rust Implementation

A comprehensive collection of data structures and algorithms implemented in Rust.

## Features

- **No-std compatible**: Works in embedded and bare-metal environments
- **Comprehensive testing**: 840+ tests (697 unit tests, 143 doc tests)
- **Educational**: Detailed documentation with complexity analysis

## Project Structure

```
rust/
├── dsa-core/           # Core traits and error types
├── dsa-data-structures/ # All data structure implementations
└── dsa-algorithms/     # All algorithm implementations
```

## Data Structures (24)

### Linear Structures
- `DynamicArray` - Resizable array
- `SinglyLinkedList` - Forward-only linked list
- `DoublyLinkedList` - Bidirectional linked list
- `Stack` - LIFO structure
- `Queue` - FIFO circular buffer
- `Deque` - Double-ended queue
- `MonotonicQueue` - Sliding window min/max

### Trees
- `BinarySearchTree` - Basic BST
- `AVLTree` - Self-balancing tree
- `RedBlackTree` - Color-balanced tree
- `BTree` - Disk-optimized tree
- `Trie` - Prefix tree
- `SegmentTree` - Range queries
- `FenwickTree` - Binary indexed tree

### Hashing
- `HashTable` - Open addressing with linear probing
- `BloomFilter` - Probabilistic set membership

### Heaps
- `BinaryHeap` - Min/max heap

### Graphs
- `AdjacencyList` - Sparse graph representation
- `AdjacencyMatrix` - Dense graph representation
- `UnionFind` - Disjoint set

### Caches
- `LRUCache` - Least recently used
- `LFUCache` - Least frequently used

### Advanced
- `SkipList` - Probabilistic search structure

## Algorithms (50+)

### Sorting
- Bubble, Selection, Insertion (comparison)
- Merge, Quick, Heap (efficient)
- Counting, Radix (non-comparison)

### Searching
- Binary Search (and variants)
- Two Pointers
- Sliding Window

### Graph Algorithms
- BFS, DFS
- Dijkstra, Bellman-Ford, Floyd-Warshall
- Prim, Kruskal (MST)
- Topological Sort
- Kosaraju SCC

### Dynamic Programming
- Fibonacci
- Knapsack (0/1 and unbounded)
- LCS, LIS
- Edit Distance
- Coin Change
- Matrix Chain Multiplication
- Word Break

### String Algorithms
- KMP Pattern Matching
- Rabin-Karp
- Z-Algorithm
- Manacher (Palindromes)

## Usage

Add to your `Cargo.toml`:

```toml
[dependencies]
dsa-data-structures = { path = "path/to/dsa-data-structures" }
dsa-algorithms = { path = "path/to/dsa-algorithms" }
```

Example:

```rust
use dsa_data_structures::trees::AVLTree;
use dsa_algorithms::sorting::merge_sort;

// Use AVL tree
let mut tree = AVLTree::new();
tree.insert(5);
tree.insert(3);
tree.insert(7);
assert!(tree.contains(&5));

// Sort with merge sort
let mut arr = vec![5, 2, 8, 1, 9];
merge_sort(&mut arr);
assert_eq!(arr, vec![1, 2, 5, 8, 9]);
```

## Running Tests

```bash
# Run all tests
cargo test

# Run with output
cargo test -- --nocapture

# Run specific module tests
cargo test --lib trees::avl

# Run doc tests
cargo test --doc
```

## Complexity Reference

| Structure | Insert | Delete | Search | Space |
|-----------|--------|--------|--------|-------|
| DynamicArray | O(1)* | O(n) | O(n) | O(n) |
| LinkedList | O(1) | O(1) | O(n) | O(n) |
| HashTable | O(1)* | O(1)* | O(1)* | O(n) |
| BST | O(log n)* | O(log n)* | O(log n)* | O(n) |
| AVL/RB Tree | O(log n) | O(log n) | O(log n) | O(n) |
| B-Tree | O(log n) | O(log n) | O(log n) | O(n) |
| Heap | O(log n) | O(log n) | O(n) | O(n) |
| BloomFilter | O(k) | - | O(k) | O(m) |

*Amortized or average case

## License

MIT
