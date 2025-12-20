# Heaps

A heap is a complete binary tree that satisfies the heap property: in a max-heap, each parent is greater than or equal to its children; in a min-heap, each parent is less than or equal to its children.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Binary Heap | [heap.py](../../by-language/python/data_structures/heap.py) | [heap.c](../../by-language/c/data-structures/heap.c) |

## Complexity

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

## When to Use

- Priority scheduling
- Dijkstra's algorithm
- Finding k smallest/largest elements
- Merge k sorted lists
- Median maintenance (two heaps)
- Event-driven simulation
- Huffman coding

## Heap Operations Visualized

### Insert (Bubble Up)
```
Insert 2 into min-heap:

    1              1              1
   / \            / \            / \
  3   4    →     3   4    →     2   4
 /              / \            / \
5              5   2          5   3

Add at end    Compare with   Swap if needed
              parent         (bubble up)
```

### Extract Min (Bubble Down)
```
Extract from min-heap:

    1              5              3
   / \            / \            / \
  3   4    →     3   4    →     5   4
 /
5

Remove root   Move last      Swap with smallest
              to root        child (bubble down)
```

## Common Patterns

### Top K Elements
```python
import heapq

# K largest: use min-heap of size k
def k_largest(nums, k):
    heap = nums[:k]
    heapq.heapify(heap)
    for num in nums[k:]:
        if num > heap[0]:
            heapq.heapreplace(heap, num)
    return heap

# K smallest: use max-heap of size k (negate values)
def k_smallest(nums, k):
    heap = [-x for x in nums[:k]]
    heapq.heapify(heap)
    for num in nums[k:]:
        if -num > heap[0]:
            heapq.heapreplace(heap, -num)
    return [-x for x in heap]
```

### Running Median (Two Heaps)
```python
class MedianFinder:
    def __init__(self):
        self.small = []  # max-heap (negated)
        self.large = []  # min-heap

    def add(self, num):
        heapq.heappush(self.small, -num)
        heapq.heappush(self.large, -heapq.heappop(self.small))

        if len(self.large) > len(self.small):
            heapq.heappush(self.small, -heapq.heappop(self.large))

    def median(self):
        if len(self.small) > len(self.large):
            return -self.small[0]
        return (-self.small[0] + self.large[0]) / 2
```

### Merge K Sorted Lists
```python
import heapq

def merge_k_lists(lists):
    heap = []
    for i, lst in enumerate(lists):
        if lst:
            heapq.heappush(heap, (lst[0], i, 0))

    result = []
    while heap:
        val, list_idx, elem_idx = heapq.heappop(heap)
        result.append(val)

        if elem_idx + 1 < len(lists[list_idx]):
            next_val = lists[list_idx][elem_idx + 1]
            heapq.heappush(heap, (next_val, list_idx, elem_idx + 1))

    return result
```

### Dijkstra's Shortest Path
```python
import heapq

def dijkstra(graph, start):
    dist = {start: 0}
    heap = [(0, start)]

    while heap:
        d, u = heapq.heappop(heap)
        if d > dist.get(u, float('inf')):
            continue

        for v, weight in graph[u]:
            new_dist = d + weight
            if new_dist < dist.get(v, float('inf')):
                dist[v] = new_dist
                heapq.heappush(heap, (new_dist, v))

    return dist
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 23 | Merge k Sorted Lists | Hard | K-way merge |
| 215 | Kth Largest Element in Array | Medium | Top K |
| 218 | The Skyline Problem | Hard | Sweep line + heap |
| 239 | Sliding Window Maximum | Hard | Monotonic deque |
| 253 | Meeting Rooms II | Medium | Event scheduling |
| 264 | Ugly Number II | Medium | Min-heap generation |
| 295 | Find Median from Data Stream | Hard | Two heaps |
| 313 | Super Ugly Number | Medium | Min-heap |
| 347 | Top K Frequent Elements | Medium | Top K |
| 355 | Design Twitter | Medium | Merge K feeds |
| 373 | Find K Pairs with Smallest Sums | Medium | Min-heap |
| 378 | Kth Smallest Element in Sorted Matrix | Medium | Min-heap |
| 407 | Trapping Rain Water II | Hard | Min-heap BFS |
| 451 | Sort Characters By Frequency | Medium | Frequency + heap |
| 502 | IPO | Hard | Two heaps |
| 621 | Task Scheduler | Medium | Max-heap |
| 630 | Course Schedule III | Hard | Max-heap |
| 632 | Smallest Range Covering K Lists | Hard | K-way merge |
| 658 | Find K Closest Elements | Medium | Binary search + heap |
| 703 | Kth Largest Element in Stream | Easy | Min-heap |
| 743 | Network Delay Time | Medium | Dijkstra |
| 767 | Reorganize String | Medium | Max-heap |
| 778 | Swim in Rising Water | Hard | Min-heap BFS |
| 787 | Cheapest Flights Within K Stops | Medium | Modified Dijkstra |
| 857 | Minimum Cost to Hire K Workers | Hard | Sorting + max-heap |
| 973 | K Closest Points to Origin | Medium | Top K |
| 1046 | Last Stone Weight | Easy | Max-heap |
| 1167 | Minimum Cost to Connect Sticks | Medium | Min-heap |
| 1354 | Construct Target Array | Hard | Max-heap |
| 1642 | Furthest Building You Can Reach | Medium | Min-heap |

## Heap Variants

### D-ary Heap
- Each node has d children instead of 2
- Shallower tree (height = log_d(n))
- More comparisons per level
- Better cache performance for large d

### Fibonacci Heap
- Amortized O(1) decrease-key
- Better for Dijkstra's algorithm theoretically
- Complex implementation, rarely used in practice

### Indexed Priority Queue
- Support decrease-key operation
- Needed for proper Dijkstra implementation

## Implementation Notes

### Array Representation
```
For 0-indexed array:
- Parent of i: (i - 1) / 2
- Left child of i: 2 * i + 1
- Right child of i: 2 * i + 2

        0
       / \
      1   2
     / \ / \
    3  4 5  6

Array: [0, 1, 2, 3, 4, 5, 6]
```

### Python heapq Module
- Only provides min-heap
- For max-heap, negate values
- `heapify()` - O(n) build
- `heappush()` - O(log n) insert
- `heappop()` - O(log n) extract min
- `heapreplace()` - O(log n) pop then push
- `nlargest(k, iterable)` - top k largest
- `nsmallest(k, iterable)` - top k smallest

## See Also

- [Sorting](../sorting/README.md) - Heap sort algorithm
- [Graphs](../graphs/README.md) - Dijkstra's algorithm uses heaps
- [Trees](../trees/README.md) - Heap is a complete binary tree
