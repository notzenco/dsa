# Data Structures & Algorithms

A comprehensive collection of data structures and algorithms implemented in **5 languages**: C, C++, Rust, Python, and Go.

Each implementation includes:
- Detailed ASCII visual diagrams
- Time & space complexity analysis
- LeetCode problem mappings
- Comprehensive unit tests

## Repository Structure

```
dsa/
├── by-language/          # Implementations organized by language
│   ├── c/                # Complete C implementations
│   ├── python/           # Complete Python implementations
│   ├── cpp/              # (Planned)
│   ├── rust/             # (Planned)
│   └── go/               # (Planned)
├── by-topic/             # Cross-reference index by topic
│   ├── arrays/           # Dynamic arrays, two pointers, sliding window
│   ├── linked-lists/     # Singly/doubly linked lists
│   ├── stacks/           # Stack operations, monotonic stacks
│   ├── queues/           # Queues, deques, circular queues
│   ├── hash-tables/      # Hash maps, bloom filters, caches
│   ├── trees/            # BST, AVL, Red-Black, Trie, Segment Tree
│   ├── heaps/            # Binary heaps, priority queues
│   ├── graphs/           # Graph representations and algorithms
│   ├── sorting/          # All sorting algorithms
│   ├── searching/        # Binary search, two pointers, sliding window
│   ├── strings/          # KMP, Rabin-Karp, Z-algorithm
│   └── dynamic-programming/  # DP patterns and problems
└── COMPLEXITY.md         # Master complexity cheat sheet
```

## Data Structures

### Linear
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Dynamic Array | [x] | [x] | [ ] | [ ] | [ ] |
| Singly Linked List | [x] | [x] | [ ] | [ ] | [ ] |
| Doubly Linked List | [x] | [x] | [ ] | [ ] | [ ] |
| Stack | [x] | [x] | [ ] | [ ] | [ ] |
| Queue | [x] | [x] | [ ] | [ ] | [ ] |
| Deque | [x] | [x] | [ ] | [ ] | [ ] |
| Circular Queue | [x] | [x] | [ ] | [ ] | [ ] |
| Min/Max Stack | [x] | [x] | [ ] | [ ] | [ ] |

### Trees
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Search Tree | [x] | [x] | [ ] | [ ] | [ ] |
| AVL Tree | [x] | [x] | [ ] | [ ] | [ ] |
| Red-Black Tree | [x] | [x] | [ ] | [ ] | [ ] |
| B-Tree | [x] | [x] | [ ] | [ ] | [ ] |
| Trie | [x] | [x] | [ ] | [ ] | [ ] |
| Segment Tree | [x] | [x] | [ ] | [ ] | [ ] |
| Fenwick Tree | [x] | [x] | [ ] | [ ] | [ ] |

### Hashing
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Hash Table | [x] | [x] | [ ] | [ ] | [ ] |
| Bloom Filter | [x] | [x] | [ ] | [ ] | [ ] |

### Heaps
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Heap | [x] | [x] | [ ] | [ ] | [ ] |
| Priority Queue | [x] | [x] | [ ] | [ ] | [ ] |

### Graphs
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Adjacency List | [x] | [x] | [ ] | [ ] | [ ] |
| Adjacency Matrix | [x] | [x] | [ ] | [ ] | [ ] |
| Union-Find | [x] | [x] | [ ] | [ ] | [ ] |

### Advanced
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| LRU Cache | [x] | [x] | [ ] | [ ] | [ ] |
| LFU Cache | [x] | [x] | [ ] | [ ] | [ ] |
| TTL Cache | [x] | [x] | [ ] | [ ] | [ ] |
| Skip List | [x] | [x] | [ ] | [ ] | [ ] |
| Monotonic Queue | [x] | [x] | [ ] | [ ] | [ ] |

## Algorithms

### Sorting
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Bubble Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Selection Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Insertion Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Merge Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Quick Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Heap Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Counting Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Radix Sort | [x] | [x] | [ ] | [ ] | [ ] |

### Searching
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Search | [x] | [x] | [ ] | [ ] | [ ] |
| Two Pointers | [x] | [x] | [ ] | [ ] | [ ] |
| Sliding Window | [x] | [x] | [ ] | [ ] | [ ] |

### Graph
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| BFS | [x] | [x] | [ ] | [ ] | [ ] |
| DFS | [x] | [x] | [ ] | [ ] | [ ] |
| Dijkstra | [x] | [x] | [ ] | [ ] | [ ] |
| Bellman-Ford | [x] | [x] | [ ] | [ ] | [ ] |
| Floyd-Warshall | [x] | [x] | [ ] | [ ] | [ ] |
| Prim's MST | [x] | [x] | [ ] | [ ] | [ ] |
| Kruskal's MST | [x] | [x] | [ ] | [ ] | [ ] |
| Topological Sort | [x] | [x] | [ ] | [ ] | [ ] |
| Tarjan's SCC | [x] | [x] | [ ] | [ ] | [ ] |

### Dynamic Programming
| Problem | Python | C | C++ | Rust | Go |
|---------|:------:|:-:|:---:|:----:|:--:|
| Fibonacci | [x] | [x] | [ ] | [ ] | [ ] |
| Knapsack | [x] | [x] | [ ] | [ ] | [ ] |
| LCS | [x] | [x] | [ ] | [ ] | [ ] |
| LIS | [x] | [x] | [ ] | [ ] | [ ] |
| Edit Distance | [x] | [x] | [ ] | [ ] | [ ] |
| Coin Change | [x] | [x] | [ ] | [ ] | [ ] |

### String
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| KMP | [x] | [x] | [ ] | [ ] | [ ] |
| Rabin-Karp | [x] | [x] | [ ] | [ ] | [ ] |
| Z-Algorithm | [x] | [x] | [ ] | [ ] | [ ] |

## Quick Start

### Python
```bash
cd by-language/python
pip install -e ".[dev]"
pytest tests/
```

### Rust
```bash
cd by-language/rust
cargo test
```

### Go
```bash
cd by-language/go
go test ./...
```

### C++
```bash
cd by-language/cpp
mkdir build && cd build
cmake .. && make
ctest
```

### C
```bash
cd by-language/c
make test
```

## Implementation Template

Each implementation follows a consistent format:

```
/**
 * [DATA STRUCTURE/ALGORITHM NAME]
 *
 * ╔═══════════════════════════════════════╗
 * ║         VISUAL REPRESENTATION         ║
 * ╚═══════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌───────────┬──────┬───────┐
 * │ Operation │ Time │ Space │
 * ├───────────┼──────┼───────┤
 * │ ...       │ O(?) │ O(?)  │
 * └───────────┴──────┴───────┘
 *
 * LEETCODE: #xxx Problem Name
 *
 * USE CASES: ...
 */
```

## Browse by Topic

Each topic includes complexity analysis, common patterns, implementation tips, and related LeetCode problems:

| Topic | Description |
|-------|-------------|
| [Arrays](by-topic/arrays/) | Dynamic arrays, two pointers, sliding window, prefix sums |
| [Linked Lists](by-topic/linked-lists/) | Singly/doubly linked, fast/slow pointers, reversal |
| [Stacks](by-topic/stacks/) | LIFO operations, monotonic stacks, expression parsing |
| [Queues](by-topic/queues/) | FIFO operations, deques, BFS patterns |
| [Hash Tables](by-topic/hash-tables/) | Hash maps, bloom filters, LRU/LFU caches |
| [Trees](by-topic/trees/) | BST, AVL, Red-Black, B-Tree, Trie, Segment Tree |
| [Heaps](by-topic/heaps/) | Binary heaps, priority queues, top-K problems |
| [Graphs](by-topic/graphs/) | Representations, BFS, DFS, shortest paths, MST |
| [Sorting](by-topic/sorting/) | Comparison and non-comparison sorting algorithms |
| [Searching](by-topic/searching/) | Binary search, two pointers, sliding window |
| [Strings](by-topic/strings/) | KMP, Rabin-Karp, Z-algorithm, pattern matching |
| [Dynamic Programming](by-topic/dynamic-programming/) | DP patterns: linear, knapsack, string, grid, interval |

## License

MIT
