# DSA Rust Implementation

A comprehensive collection of data structures and algorithms implemented in Rust.

## Features

- **No-std compatible**: All workspace crates compile with default features disabled
- **Comprehensive testing**: Unit and documentation tests across all crates
- **Educational**: Detailed documentation with complexity analysis

## Project Structure

```
rust/
├── dsa-core/           # Core traits and error types
├── dsa-data-structures/ # All data structure implementations
└── dsa-algorithms/     # All algorithm implementations
```

## Data Structures

### Linear Structures
- `DynamicArray` - Resizable array
- `SinglyLinkedList` - Forward-only linked list
- `DoublyLinkedList` - Bidirectional linked list
- `Stack` - LIFO structure
- `Queue` - FIFO circular buffer
- `Deque` - Double-ended queue
- `MonotonicQueue` - Sliding window min/max

### Trees
- `BinarySearchTree` - Basic BST with unique keys
- `AVLTree` - Self-balancing tree
- `RedBlackTree` - Color-balanced tree (no removal)
- `BTree` - Disk-optimized tree (keeps duplicate keys; no removal)
- `Trie` - Prefix tree
- `SegmentTree` - Range queries
- `FenwickTree` - Binary indexed tree

### Hashing
- `HashTable` - Open addressing with linear probing
- `BloomFilter` - Probabilistic set membership

### Heaps
- `BinaryHeap` - Min/max heap

### Graphs
- `Graph` - Adjacency list (`Vec<Vec<Edge>>` in `dsa_algorithms::graph`)
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
- Counting, Radix (non-comparison); counting sort falls back to
  `sort_unstable` when the value range is more than 16 times the input length

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

# Check all crates without the std feature
cargo check --offline --workspace --no-default-features

# Lints and documentation, as run in CI
cargo clippy --workspace --all-targets --all-features -- -D warnings
RUSTDOCFLAGS="-D warnings" cargo doc --workspace --no-deps
```

The `tests/models.rs` suites in `dsa-algorithms` and `dsa-data-structures`
compare each implementation against standard-library reference models with a
fixed seed. CI also checks the library crates on the declared minimum Rust
version, 1.70.

## Path reconstruction

`dsa_algorithms::graph::reconstruct_path` takes the predecessor array, the
source vertex, and the target vertex in that order. It returns `None` for an
invalid or unreachable target, including malformed parent chains. Callers of
the earlier two-argument form must pass the source explicitly.

## Complexity Reference

| Structure | Insert | Delete | Search | Space |
|-----------|--------|--------|--------|-------|
| DynamicArray | O(1)* | O(n) | O(n) | O(n) |
| LinkedList | O(1) | O(1) | O(n) | O(n) |
| HashTable | O(1)* | O(1)* | O(1)* | O(n) |
| BST | O(log n)* | O(log n)* | O(log n)* | O(n) |
| AVL Tree | O(log n) | O(log n) | O(log n) | O(n) |
| Red-Black Tree | O(log n) | - | O(log n) | O(n) |
| B-Tree | O(log n) | - | O(log n) | O(n) |
| Heap | O(log n) | O(log n) | O(n) | O(n) |
| BloomFilter | O(k) | - | O(k) | O(m) |

*Amortized or average case; - means the operation is not implemented

## License

MIT
