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
| Dynamic Array | [x] | [ ] | [ ] | [ ] | [ ] |
| Singly Linked List | [x] | [ ] | [ ] | [ ] | [ ] |
| Doubly Linked List | [x] | [ ] | [ ] | [ ] | [ ] |
| Stack | [x] | [ ] | [ ] | [ ] | [ ] |
| Queue | [x] | [ ] | [ ] | [ ] | [ ] |
| Deque | [x] | [ ] | [ ] | [ ] | [ ] |
| Circular Queue | [x] | [ ] | [ ] | [ ] | [ ] |
| Min/Max Stack | [x] | [ ] | [ ] | [ ] | [ ] |

### Trees
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Binary Search Tree | [x] | [ ] | [ ] | [ ] | [ ] |
| AVL Tree | [ ] | [ ] | [ ] | [ ] | [ ] |
| Red-Black Tree | [ ] | [ ] | [ ] | [ ] | [ ] |
| B-Tree | [ ] | [ ] | [ ] | [ ] | [ ] |
| Trie | [x] | [ ] | [ ] | [ ] | [ ] |
| Segment Tree | [ ] | [ ] | [ ] | [ ] | [ ] |
| Fenwick Tree | [ ] | [ ] | [ ] | [ ] | [ ] |

### Hashing
| Structure | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| Hash Table | [x] | [ ] | [ ] | [ ] | [ ] |
| Bloom Filter | [ ] | [ ] | [ ] | [ ] | [ ] |

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
| Skip List | [ ] | [ ] | [ ] | [ ] | [ ] |

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
| Floyd-Warshall | [ ] | [ ] | [ ] | [ ] | [ ] |
| Prim's MST | [ ] | [ ] | [ ] | [ ] | [ ] |
| Kruskal's MST | [ ] | [ ] | [ ] | [ ] | [ ] |
| Topological Sort | [x] | [ ] | [ ] | [ ] | [ ] |
| Tarjan's SCC | [ ] | [ ] | [ ] | [ ] | [ ] |

### Dynamic Programming
| Problem | Python | C | C++ | Rust | Go |
|---------|:------:|:-:|:---:|:----:|:--:|
| Fibonacci | [ ] | [ ] | [ ] | [ ] | [ ] |
| Knapsack | [ ] | [ ] | [ ] | [ ] | [ ] |
| LCS | [ ] | [ ] | [ ] | [ ] | [ ] |
| LIS | [ ] | [ ] | [ ] | [ ] | [ ] |
| Edit Distance | [ ] | [ ] | [ ] | [ ] | [ ] |
| Coin Change | [ ] | [ ] | [ ] | [ ] | [ ] |

### String
| Algorithm | Python | C | C++ | Rust | Go |
|-----------|:------:|:-:|:---:|:----:|:--:|
| KMP | [ ] | [ ] | [ ] | [ ] | [ ] |
| Rabin-Karp | [ ] | [ ] | [ ] | [ ] | [ ] |
| Z-Algorithm | [ ] | [ ] | [ ] | [ ] | [ ] |

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
