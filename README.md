# Data Structures & Algorithms

A comprehensive collection of data structures and algorithms implemented in **C**, **Go**, **Java**, **Python**, **Rust**, and **TypeScript**.

Each implementation includes:
- Detailed ASCII visual diagrams
- Time & space complexity analysis
- LeetCode problem mappings
- Comprehensive unit tests

## Language Requirements

| Language | Version | Build Tool |
|----------|---------|------------|
| C | C11 | gcc/make |
| Go | 1.21+ | go test |
| Java | 17+ | gradle |
| Python | 3.10+ | pytest |
| Rust | 1.70+ | cargo |
| TypeScript | ES2022 | bun |

## Repository Structure

```
dsa/
├── by-language/          # Implementations organized by language
│   ├── c/                # Complete C implementations
│   ├── go/               # Complete Go implementations
│   ├── java/             # Complete Java implementations
│   ├── python/           # Complete Python implementations
│   ├── rust/             # Complete Rust implementations
│   └── typescript/       # Complete TypeScript implementations
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
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Dynamic Array | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Singly Linked List | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Doubly Linked List | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Stack | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Queue | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Deque | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Circular Queue | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Min/Max Stack | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Monotonic Queue | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Trees
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Binary Search Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| AVL Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Red-Black Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| B-Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Trie | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Segment Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Fenwick Tree | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Hashing
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Hash Table | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Bloom Filter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Heaps
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Binary Heap | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Priority Queue | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Graphs
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Adjacency List | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Adjacency Matrix | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Union-Find | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Caches
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| LRU Cache | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| LFU Cache | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| TTL Cache | ✓ | ✓ | ✓ | ✓ | - | ✓ |

### Advanced
| Structure | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Skip List | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

## Algorithms

### Sorting
| Algorithm | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Bubble Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Selection Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Insertion Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Merge Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Quick Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Heap Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Counting Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Radix Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Searching
| Algorithm | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Binary Search | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Two Pointers | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Sliding Window | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Graph
| Algorithm | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| BFS | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| DFS | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Dijkstra | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Bellman-Ford | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Floyd-Warshall | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Prim's MST | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Kruskal's MST | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Topological Sort | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Tarjan's SCC | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### Dynamic Programming
| Problem | C | Go | Java | Python | Rust | TypeScript |
|---------|:-:|:--:|:----:|:------:|:----:|:----------:|
| Fibonacci | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Knapsack | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| LCS | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| LIS | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Edit Distance | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Coin Change | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

### String
| Algorithm | C | Go | Java | Python | Rust | TypeScript |
|-----------|:-:|:--:|:----:|:------:|:----:|:----------:|
| KMP | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Rabin-Karp | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Z-Algorithm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

## Quick Start

### C
```bash
cd by-language/c
make test
```

### Go
```bash
cd by-language/go
go test ./...
```

### Java
```bash
cd by-language/java
gradle test
```

### Python
```bash
cd by-language/python
pip install pytest
pytest tests/
```

### Rust
```bash
cd by-language/rust
cargo test
```

### TypeScript
```bash
cd by-language/typescript
bun install
bun test
```

### Git Hooks (Optional)
```bash
# Install pre-commit and pre-push hooks
./.githooks/install.sh
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
