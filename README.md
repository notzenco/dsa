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
| Skip List | [x] | [x] | [ ] | [ ] | [ ] |

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

## License

MIT
