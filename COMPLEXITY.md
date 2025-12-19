# Data Structures & Algorithms Complexity Guide

A comprehensive reference for choosing the right data structure or algorithm for your problem.

## Table of Contents
- [Data Structures](#data-structures)
  - [Linear Structures](#linear-structures)
  - [Trees](#trees)
  - [Hash-Based](#hash-based)
  - [Graphs](#graphs)
  - [Advanced](#advanced)
- [Algorithms](#algorithms)
  - [Sorting](#sorting)
  - [Searching](#searching)
  - [Graph Algorithms](#graph-algorithms)
  - [Dynamic Programming](#dynamic-programming)
  - [String Algorithms](#string-algorithms)
- [Decision Flowcharts](#decision-flowcharts)
- [Quick Reference Tables](#quick-reference-tables)

---

## Data Structures

### Linear Structures

#### Array / Dynamic Array
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Access      │ O(1)    │ O(1)    │         │
│ Search      │ O(n)    │ O(n)    │         │
│ Insert End  │ O(1)*   │ O(n)    │ O(n)    │
│ Insert Mid  │ O(n)    │ O(n)    │         │
│ Delete End  │ O(1)    │ O(1)    │         │
│ Delete Mid  │ O(n)    │ O(n)    │         │
└─────────────┴─────────┴─────────┴─────────┘
* Amortized
```

**When to Use:**
- Need O(1) random access by index
- Data size is known or changes infrequently
- Memory locality matters (cache-friendly)
- Implementing other data structures (heaps, hash tables)

**When NOT to Use:**
- Frequent insertions/deletions in the middle
- Unknown or highly variable size
- Need to frequently search for values

---

#### Linked List (Singly/Doubly)
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Singly  │ Doubly  │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Access      │ O(n)    │ O(n)    │         │
│ Search      │ O(n)    │ O(n)    │         │
│ Insert Head │ O(1)    │ O(1)    │ O(n)    │
│ Insert Tail │ O(n)/O(1)*│ O(1)  │         │
│ Insert Mid  │ O(n)    │ O(n)    │         │
│ Delete Head │ O(1)    │ O(1)    │         │
│ Delete Tail │ O(n)    │ O(1)    │         │
│ Delete Mid  │ O(n)    │ O(n)    │         │
└─────────────┴─────────┴─────────┴─────────┘
* O(1) if tail pointer maintained
```

**When to Use:**
- Frequent insertions/deletions at ends
- Don't need random access
- Implementing stacks, queues
- Unknown data size that changes frequently
- Need to splice/merge lists efficiently

**When NOT to Use:**
- Need random access
- Memory is constrained (pointer overhead)
- Cache performance is critical

---

#### Stack (LIFO)
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Push        │ O(1)    │ O(n)    │
│ Pop         │ O(1)    │         │
│ Peek/Top    │ O(1)    │         │
│ IsEmpty     │ O(1)    │         │
└─────────────┴─────────┴─────────┘
```

**When to Use:**
- Undo/Redo functionality
- Expression evaluation & parsing
- Backtracking algorithms (DFS, maze solving)
- Function call management
- Balanced parentheses checking
- Browser history (back button)

**Variants:**
- **Min/Max Stack**: Track min/max in O(1) with extra O(n) space

---

#### Queue (FIFO)
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Enqueue     │ O(1)    │ O(n)    │
│ Dequeue     │ O(1)    │         │
│ Front/Peek  │ O(1)    │         │
│ IsEmpty     │ O(1)    │         │
└─────────────┴─────────┴─────────┘
```

**When to Use:**
- BFS traversal
- Task scheduling
- Buffering (I/O, print queues)
- Order processing (first-come-first-served)
- Rate limiting

**Variants:**
- **Deque**: O(1) operations at both ends
- **Circular Queue**: Fixed-size buffer, wraps around
- **Priority Queue**: See Heap section

---

### Trees

#### Binary Search Tree (BST)
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(log n)│ O(n)    │         │
│ Insert      │ O(log n)│ O(n)    │ O(n)    │
│ Delete      │ O(log n)│ O(n)    │         │
│ Min/Max     │ O(log n)│ O(n)    │         │
└─────────────┴─────────┴─────────┴─────────┘
Worst case occurs with skewed tree (sorted input)
```

**When to Use:**
- Need sorted data with dynamic updates
- In-order traversal needed
- Simple implementation acceptable
- Data is reasonably random

**When NOT to Use:**
- Data is sorted or nearly sorted (use balanced tree)
- Need guaranteed O(log n) operations

---

#### AVL Tree
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(log n)│ O(log n)│         │
│ Insert      │ O(log n)│ O(log n)│ O(n)    │
│ Delete      │ O(log n)│ O(log n)│         │
└─────────────┴─────────┴─────────┴─────────┘
Height: ≤ 1.44 * log(n)
```

**When to Use:**
- Read-heavy workloads (lookups frequent)
- Need strict balance guarantees
- Height must be minimal

**When NOT to Use:**
- Write-heavy workloads (many inserts/deletes)
- Simpler solution acceptable

---

#### Red-Black Tree
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(log n)│ O(log n)│         │
│ Insert      │ O(log n)│ O(log n)│ O(n)    │
│ Delete      │ O(log n)│ O(log n)│         │
└─────────────┴─────────┴─────────┴─────────┘
Height: ≤ 2 * log(n+1)
```

**When to Use:**
- Write-heavy workloads
- Need guaranteed O(log n) with fewer rotations
- Implementing ordered map/set (std::map, TreeMap)

**AVL vs Red-Black:**
| Aspect | AVL | Red-Black |
|--------|-----|-----------|
| Balance | Stricter | Looser |
| Height | ~1.44 log n | ~2 log n |
| Lookups | Faster | Slower |
| Inserts | More rotations | Fewer rotations |
| Use Case | Read-heavy | Write-heavy |

---

#### B-Tree
```
┌─────────────┬─────────────────┬─────────────┐
│ Operation   │ Time            │ Disk I/O    │
├─────────────┼─────────────────┼─────────────┤
│ Search      │ O(t * log_t(n)) │ O(log_t(n)) │
│ Insert      │ O(t * log_t(n)) │ O(log_t(n)) │
│ Delete      │ O(t * log_t(n)) │ O(log_t(n)) │
└─────────────┴─────────────────┴─────────────┘
t = minimum degree (branching factor)
```

**When to Use:**
- Database indexing
- File systems
- Disk-based storage (minimize I/O)
- Large datasets that don't fit in memory

**When NOT to Use:**
- In-memory only operations
- Small datasets

---

#### Trie (Prefix Tree)
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Search      │ O(m)    │         │
│ Insert      │ O(m)    │ O(m*n)  │
│ Delete      │ O(m)    │ to      │
│ Prefix      │ O(m)    │ O(ALPHABET*m*n) │
└─────────────┴─────────┴─────────┘
m = key length, n = number of keys
```

**When to Use:**
- Autocomplete/typeahead
- Spell checkers
- IP routing tables
- Word games (Boggle, Scrabble)
- Prefix-based searching

**When NOT to Use:**
- Memory constrained
- Keys are very long
- No prefix-based operations needed

---

#### Segment Tree
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Build       │ O(n)    │ O(n)    │
│ Query       │ O(log n)│         │
│ Update      │ O(log n)│         │
└─────────────┴─────────┴─────────┘
```

**When to Use:**
- Range queries (sum, min, max, GCD)
- Range updates (with lazy propagation)
- Competitive programming
- Computational geometry

---

#### Fenwick Tree (Binary Indexed Tree)
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Build       │ O(n)    │ O(n)    │
│ Query       │ O(log n)│         │
│ Update      │ O(log n)│         │
└─────────────┴─────────┴─────────┘
```

**Segment Tree vs Fenwick Tree:**
| Aspect | Segment Tree | Fenwick Tree |
|--------|--------------|--------------|
| Space | 4n | n |
| Code complexity | Higher | Lower |
| Range updates | With lazy prop | Limited |
| Operations | Any associative | Mainly sum |

---

### Hash-Based

#### Hash Table
```
┌─────────────┬─────────┬─────────┬─────────┐
│ Operation   │ Average │ Worst   │ Space   │
├─────────────┼─────────┼─────────┼─────────┤
│ Search      │ O(1)    │ O(n)    │         │
│ Insert      │ O(1)    │ O(n)    │ O(n)    │
│ Delete      │ O(1)    │ O(n)    │         │
└─────────────┴─────────┴─────────┴─────────┘
Worst case with poor hash function or many collisions
```

**When to Use:**
- Need O(1) average lookup/insert/delete
- Key-value associations
- Counting occurrences
- Detecting duplicates
- Caching

**When NOT to Use:**
- Need sorted order
- Need range queries
- Keys aren't hashable

**Collision Resolution:**
- **Chaining**: Linked list at each bucket, better for high load
- **Open Addressing**: Probe sequence, better cache performance

---

#### Bloom Filter
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Insert      │ O(k)    │ O(m)    │
│ Query       │ O(k)    │         │
└─────────────┴─────────┴─────────┘
k = number of hash functions
m = bit array size
False Positive Rate ≈ (1 - e^(-kn/m))^k
```

**When to Use:**
- Membership testing with space constraints
- Cache before expensive lookups
- Duplicate detection
- Network packet filtering
- Spell checkers

**Key Properties:**
- **No false negatives**: If says "not in set", definitely not
- **Possible false positives**: If says "in set", might be wrong
- **Cannot delete**: Use Counting Bloom Filter for deletions

---

### Graphs

#### Adjacency List vs Matrix
```
┌─────────────────┬─────────────────┬─────────────────┐
│ Operation       │ Adjacency List  │ Adjacency Matrix│
├─────────────────┼─────────────────┼─────────────────┤
│ Space           │ O(V + E)        │ O(V²)           │
│ Add Edge        │ O(1)            │ O(1)            │
│ Remove Edge     │ O(E)            │ O(1)            │
│ Check Edge      │ O(degree)       │ O(1)            │
│ Get Neighbors   │ O(1)            │ O(V)            │
│ Iterate Edges   │ O(E)            │ O(V²)           │
└─────────────────┴─────────────────┴─────────────────┘
```

**Use Adjacency List when:**
- Sparse graph (E << V²)
- Need to iterate neighbors frequently
- Memory is constrained

**Use Adjacency Matrix when:**
- Dense graph (E ≈ V²)
- Need O(1) edge existence checks
- Graph is small

---

#### Union-Find (Disjoint Set)
```
┌─────────────┬───────────────────┐
│ Operation   │ Time (amortized)  │
├─────────────┼───────────────────┤
│ Find        │ O(α(n)) ≈ O(1)    │
│ Union       │ O(α(n)) ≈ O(1)    │
│ Connected   │ O(α(n)) ≈ O(1)    │
└─────────────┴───────────────────┘
α(n) = inverse Ackermann function, practically constant
```

**When to Use:**
- Dynamic connectivity
- Kruskal's MST algorithm
- Cycle detection
- Connected components
- Equivalence relations

---

### Advanced

#### Heap / Priority Queue
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Insert      │ O(log n)│ O(n)    │
│ Extract Min │ O(log n)│         │
│ Peek Min    │ O(1)    │         │
│ Build       │ O(n)    │         │
│ Heapify     │ O(log n)│         │
└─────────────┴─────────┴─────────┘
```

**When to Use:**
- Priority scheduling
- Dijkstra's algorithm
- Finding k smallest/largest elements
- Merge k sorted lists
- Median maintenance (two heaps)

---

#### LRU Cache
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Get         │ O(1)    │ O(n)    │
│ Put         │ O(1)    │         │
└─────────────┴─────────┴─────────┘
Implementation: Hash Map + Doubly Linked List
```

**When to Use:**
- Caching with memory limit
- Page replacement
- Database query caching

---

#### Skip List
```
┌─────────────┬───────────┬─────────┐
│ Operation   │ Expected  │ Worst   │
├─────────────┼───────────┼─────────┤
│ Search      │ O(log n)  │ O(n)    │
│ Insert      │ O(log n)  │ O(n)    │
│ Delete      │ O(log n)  │ O(n)    │
└─────────────┴───────────┴─────────┘
```

**When to Use:**
- Alternative to balanced trees
- Lock-free concurrent data structures
- Range queries needed
- Simpler implementation than RB-tree

---

## Algorithms

### Sorting

```
┌──────────────────┬─────────┬─────────┬─────────┬────────┬────────────┐
│ Algorithm        │ Best    │ Average │ Worst   │ Space  │ Stable     │
├──────────────────┼─────────┼─────────┼─────────┼────────┼────────────┤
│ Bubble Sort      │ O(n)    │ O(n²)   │ O(n²)   │ O(1)   │ Yes        │
│ Selection Sort   │ O(n²)   │ O(n²)   │ O(n²)   │ O(1)   │ No         │
│ Insertion Sort   │ O(n)    │ O(n²)   │ O(n²)   │ O(1)   │ Yes        │
│ Merge Sort       │ O(n lg n)│O(n lg n)│O(n lg n)│ O(n)   │ Yes        │
│ Quick Sort       │ O(n lg n)│O(n lg n)│ O(n²)   │O(lg n) │ No         │
│ Heap Sort        │ O(n lg n)│O(n lg n)│O(n lg n)│ O(1)   │ No         │
│ Counting Sort    │ O(n+k)  │ O(n+k)  │ O(n+k)  │ O(k)   │ Yes        │
│ Radix Sort       │ O(nk)   │ O(nk)   │ O(nk)   │ O(n+k) │ Yes        │
│ Bucket Sort      │ O(n+k)  │ O(n+k)  │ O(n²)   │ O(n)   │ Yes        │
└──────────────────┴─────────┴─────────┴─────────┴────────┴────────────┘
n = number of elements, k = range of values
```

**Choosing a Sorting Algorithm:**

| Situation | Best Choice | Why |
|-----------|-------------|-----|
| Small arrays (n < 50) | Insertion Sort | Low overhead, good cache |
| Nearly sorted | Insertion Sort | O(n) best case |
| Need stability | Merge Sort | Guaranteed O(n log n) + stable |
| Memory constrained | Heap Sort | O(1) space |
| General purpose | Quick Sort | Fastest in practice |
| Integer keys, small range | Counting Sort | O(n+k) linear |
| Fixed-width integers | Radix Sort | O(nk) linear |
| Uniformly distributed | Bucket Sort | O(n) expected |

---

### Searching

```
┌─────────────────────┬─────────┬─────────┬─────────┐
│ Algorithm           │ Best    │ Average │ Worst   │
├─────────────────────┼─────────┼─────────┼─────────┤
│ Linear Search       │ O(1)    │ O(n)    │ O(n)    │
│ Binary Search       │ O(1)    │ O(log n)│ O(log n)│
│ Jump Search         │ O(1)    │ O(√n)   │ O(√n)   │
│ Interpolation Search│ O(1)    │ O(log log n)│ O(n)│
│ Exponential Search  │ O(1)    │ O(log i)│ O(log n)│
└─────────────────────┴─────────┴─────────┴─────────┘
i = position of target
```

**Binary Search Variants:**
- **Lower Bound**: First element ≥ target
- **Upper Bound**: First element > target
- **Search in Rotated Array**: Modified binary search
- **Binary Search on Answer**: When answer space is monotonic

---

### Graph Algorithms

```
┌────────────────────┬────────────────┬─────────────────┬─────────────┐
│ Algorithm          │ Time           │ Space           │ Use Case    │
├────────────────────┼────────────────┼─────────────────┼─────────────┤
│ BFS                │ O(V + E)       │ O(V)            │ Shortest path (unweighted) │
│ DFS                │ O(V + E)       │ O(V)            │ Traversal, cycle detection │
│ Dijkstra           │ O((V+E) log V) │ O(V)            │ Shortest path (non-negative)│
│ Bellman-Ford       │ O(V * E)       │ O(V)            │ Negative edges allowed │
│ Floyd-Warshall     │ O(V³)          │ O(V²)           │ All-pairs shortest path │
│ Prim's MST         │ O((V+E) log V) │ O(V)            │ Dense graphs │
│ Kruskal's MST      │ O(E log E)     │ O(V)            │ Sparse graphs │
│ Topological Sort   │ O(V + E)       │ O(V)            │ DAG ordering │
│ Tarjan's SCC       │ O(V + E)       │ O(V)            │ Strongly connected components │
└────────────────────┴────────────────┴─────────────────┴─────────────┘
```

**Shortest Path Algorithm Selection:**

| Condition | Best Algorithm |
|-----------|----------------|
| Unweighted graph | BFS |
| Non-negative weights | Dijkstra |
| Negative weights possible | Bellman-Ford |
| All pairs needed | Floyd-Warshall |
| Single source, dense graph | Dijkstra |
| Single source, sparse graph | Dijkstra with heap |

---

### Dynamic Programming

| Problem | Time | Space | Technique |
|---------|------|-------|-----------|
| Fibonacci | O(n) | O(1) | Space optimization |
| 0/1 Knapsack | O(nW) | O(W) | 1D DP |
| Unbounded Knapsack | O(nW) | O(W) | 1D DP |
| LCS | O(mn) | O(min(m,n)) | 2D DP |
| LIS | O(n log n) | O(n) | Binary search + DP |
| Edit Distance | O(mn) | O(min(m,n)) | 2D DP |
| Coin Change | O(nS) | O(S) | 1D DP |
| Matrix Chain | O(n³) | O(n²) | Interval DP |

**DP Patterns:**
1. **Linear DP**: Fibonacci, Climbing Stairs
2. **Knapsack**: 0/1, Unbounded, Bounded
3. **Grid DP**: Unique Paths, Min Path Sum
4. **String DP**: LCS, Edit Distance
5. **Interval DP**: Matrix Chain, Palindrome Partitioning
6. **Tree DP**: Max Path Sum, Tree Diameter

---

### String Algorithms

```
┌─────────────────┬────────────────┬────────────┐
│ Algorithm       │ Time           │ Space      │
├─────────────────┼────────────────┼────────────┤
│ Naive Search    │ O(nm)          │ O(1)       │
│ KMP             │ O(n + m)       │ O(m)       │
│ Rabin-Karp      │ O(n + m) avg   │ O(1)       │
│ Z-Algorithm     │ O(n + m)       │ O(n + m)   │
│ Manacher        │ O(n)           │ O(n)       │
│ Suffix Array    │ O(n log n)     │ O(n)       │
└─────────────────┴────────────────┴────────────┘
n = text length, m = pattern length
```

**Choosing a String Algorithm:**

| Use Case | Best Choice |
|----------|-------------|
| Single pattern search | KMP |
| Multiple patterns | Rabin-Karp or Aho-Corasick |
| All occurrences | Z-Algorithm |
| Longest palindrome | Manacher |
| Multiple queries | Suffix Array |

---

## Decision Flowcharts

### Choosing a Data Structure

```
Need to store data?
│
├─> Need key-value pairs?
│   ├─> Yes: Need ordering?
│   │   ├─> Yes: TreeMap (Red-Black) or Skip List
│   │   └─> No: Hash Table
│   └─> No: Continue below
│
├─> Need ordering/sorting?
│   ├─> Yes: Dynamic updates?
│   │   ├─> Yes: Balanced BST (AVL/Red-Black)
│   │   └─> No: Sorted Array
│   └─> No: Continue below
│
├─> Need LIFO access?
│   └─> Yes: Stack
│
├─> Need FIFO access?
│   └─> Yes: Queue
│
├─> Need priority ordering?
│   └─> Yes: Heap / Priority Queue
│
├─> Need prefix operations?
│   └─> Yes: Trie
│
├─> Need range queries?
│   └─> Yes: Segment Tree or Fenwick Tree
│
├─> Need membership test only?
│   └─> Yes: Bloom Filter (space-efficient) or Set
│
└─> Default: Array or Linked List
```

### Choosing a Sorting Algorithm

```
Sorting needed?
│
├─> Small array (n < 50)?
│   └─> Use Insertion Sort
│
├─> Nearly sorted?
│   └─> Use Insertion Sort
│
├─> Memory constrained?
│   └─> Use Heap Sort
│
├─> Stability required?
│   └─> Use Merge Sort
│
├─> Integer keys with small range?
│   └─> Use Counting Sort
│
├─> Fixed-width integers?
│   └─> Use Radix Sort
│
└─> General case?
    └─> Use Quick Sort
```

---

## Quick Reference Tables

### Space-Time Tradeoffs

| More Space | Less Time | Example |
|------------|-----------|---------|
| O(n) | O(1) lookup | Hash Table |
| O(n) | O(log n) range | Segment Tree |
| O(n) | O(1) min/max | Min Stack |
| O(V²) | O(1) edge check | Adjacency Matrix |

### Problem Pattern to Data Structure

| Pattern | Data Structure |
|---------|---------------|
| Top K elements | Heap |
| Sliding window max/min | Monotonic Deque |
| LRU Cache | HashMap + Doubly Linked List |
| Autocomplete | Trie |
| Range sum queries | Fenwick Tree |
| Connectivity queries | Union-Find |
| Shortest path | Graph + BFS/Dijkstra |

### Interview Tips

1. **Start simple**: Use basic structures first, optimize if needed
2. **State assumptions**: "Assuming the array is sorted..."
3. **Consider edge cases**: Empty input, single element, duplicates
4. **Analyze complexity**: Always discuss time AND space
5. **Trade-offs**: Explain why you chose one approach over another

---

## Additional Resources

- **LeetCode Problems**: See [LEETCODE_MAP.md](LEETCODE_MAP.md)
- **Implementations**: See language-specific folders in `by-language/`
- **Visualizations**: ASCII diagrams in each implementation file
