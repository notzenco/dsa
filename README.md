# Data Structures & Algorithms

A comprehensive collection of data structures and algorithms implemented in **C**, **Go**, **Java**, **Python**, **Rust**, and **TypeScript**.

Every language has:
- Unit tests for each data structure and algorithm
- Seeded randomized tests that compare implementations against simple reference models
- Time & space complexity notes

Most C, Python, and Rust sources also include ASCII diagrams and LeetCode problem
mappings in their header comments. The Go, Java, and TypeScript sources use shorter
doc comments.

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
├── COMPLEXITY.md         # Master complexity cheat sheet
└── REMEDIATION.md        # Boundary contracts and regression checks
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
| Circular Queue | ✓ | ✓ | ✓ | ✓ | - | ✓ |
| Min/Max Stack | ✓ | ✓ | ✓ | ✓ | - | ✓ |
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
| Adjacency Matrix | ✓ | ✓ | ✓ | ✓ | - | ✓ |
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
| Two Pointers | ✓ | ✓ | ✓ | ✓ | ✓ | - |
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
| Tarjan's SCC | ✓ | ✓ | ✓ | ✓ | - | ✓ |
| Kosaraju's SCC | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

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

### Go-only modules
The Go implementation also has algorithm modules that the other languages do not
have yet:

| Module | Contents |
|--------|----------|
| Backtracking | Permutations, subsets, combinations, combination sum, N-Queens, Sudoku, word search, generate parentheses |
| Greedy | Activity selection, interval problems, fractional knapsack, job sequencing, Huffman encoding, jump game, gas station, task scheduler |
| Math | GCD/LCM, primality and sieve, prime factors, fast power, Euler's totient, modular inverse, binomial and Catalan numbers, bit tricks |

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
./gradlew test
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
bun run typecheck
bun test
```

## Correctness and API contracts

The [remediation notes](REMEDIATION.md) record the boundary cases covered by
the regression suites and the commands used to verify them. Two path APIs now
require an explicit bound: C `shortest_path_reconstruct` takes the output array
capacity, and Rust `reconstruct_path` takes the source vertex. Callers of those
APIs should update their arguments before using the revised implementations.

### Git Hooks (Optional)
```bash
# Install pre-commit, pre-push, and commit-msg hooks
./.githooks/install.sh
```

The pre-commit hook checks Rust formatting and Clippy, Go formatting, and Python
lint when `ruff` or `black` is installed. The pre-push hook runs the suite for
each language whose toolchain is installed.

## Implementation Template

The C, Python, and Rust header comments follow this format:

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
