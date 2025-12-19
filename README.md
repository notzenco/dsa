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
│   ├── c/
│   ├── cpp/
│   ├── rust/
│   ├── python/
│   └── go/
├── by-topic/             # Cross-reference index by topic
├── COMPLEXITY.md         # Master complexity cheat sheet
└── LEETCODE_MAP.md       # DSA to LeetCode mappings
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
| AVL Tree | [x] | [ ] | [ ] | [ ] | [ ] |
| Red-Black Tree | [x] | [ ] | [ ] | [ ] | [ ] |
| B-Tree | [x] | [ ] | [ ] | [ ] | [ ] |
| Trie | [x] | [ ] | [ ] | [ ] | [ ] |
| Segment Tree | [x] | [ ] | [ ] | [ ] | [ ] |
| Fenwick Tree | [x] | [ ] | [ ] | [ ] | [ ] |

### Hashing
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Hash Table | [x] | [ ] | [ ] | [ ] | [ ] |
| Bloom Filter | [x] | [ ] | [ ] | [ ] | [ ] |

### Heaps
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Heap | [x] | [ ] | [ ] | [ ] | [ ] |
| Priority Queue | [x] | [ ] | [ ] | [ ] | [ ] |

### Graphs
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Adjacency List | [x] | [ ] | [ ] | [ ] | [ ] |
| Adjacency Matrix | [x] | [ ] | [ ] | [ ] | [ ] |
| Union-Find | [x] | [ ] | [ ] | [ ] | [ ] |

### Advanced
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| LRU Cache | [x] | [ ] | [ ] | [ ] | [ ] |
| LFU Cache | [x] | [ ] | [ ] | [ ] | [ ] |
| Skip List | [x] | [ ] | [ ] | [ ] | [ ] |

## Algorithms

### Sorting
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Bubble Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Selection Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Insertion Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Merge Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Quick Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Heap Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Counting Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Radix Sort | [x] | [ ] | [ ] | [ ] | [ ] |

### Searching
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Search | [x] | [ ] | [ ] | [ ] | [ ] |
| Two Pointers | [x] | [ ] | [ ] | [ ] | [ ] |
| Sliding Window | [x] | [ ] | [ ] | [ ] | [ ] |

### Graph
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| BFS | [x] | [ ] | [ ] | [ ] | [ ] |
| DFS | [x] | [ ] | [ ] | [ ] | [ ] |
| Dijkstra | [x] | [ ] | [ ] | [ ] | [ ] |
| Bellman-Ford | [x] | [ ] | [ ] | [ ] | [ ] |
| Floyd-Warshall | [x] | [ ] | [ ] | [ ] | [ ] |
| Prim's MST | [x] | [ ] | [ ] | [ ] | [ ] |
| Kruskal's MST | [x] | [ ] | [ ] | [ ] | [ ] |
| Topological Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Tarjan's SCC | [x] | [ ] | [ ] | [ ] | [ ] |

### Dynamic Programming
| Problem | Python | C | C++ | Rust | Go |
|---------|:------:|:-:|:---:|:----:|:--:|
| Fibonacci | [x] | [ ] | [ ] | [ ] | [ ] |
| Knapsack | [x] | [ ] | [ ] | [ ] | [ ] |
| LCS | [x] | [ ] | [ ] | [ ] | [ ] |
| LIS | [x] | [ ] | [ ] | [ ] | [ ] |
| Edit Distance | [x] | [ ] | [ ] | [ ] | [ ] |
| Coin Change | [x] | [ ] | [ ] | [ ] | [ ] |

### String
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| KMP | [x] | [ ] | [ ] | [ ] | [ ] |
| Rabin-Karp | [x] | [ ] | [ ] | [ ] | [ ] |
| Z-Algorithm | [x] | [ ] | [ ] | [ ] | [ ] |

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

## License

MIT
