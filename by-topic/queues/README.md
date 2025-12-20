# Queues

A queue is a First-In-First-Out (FIFO) data structure where elements are added at the rear and removed from the front.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Queue | [queue.py](../../by-language/python/data_structures/queue.py) | [queue.c](../../by-language/c/data-structures/queue.c) |

## Complexity

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

## When to Use

- BFS traversal
- Task scheduling (CPU, I/O)
- Buffering (print queues, message queues)
- Order processing (first-come-first-served)
- Rate limiting
- Level-order tree traversal

## Queue Variants

### Deque (Double-Ended Queue)
O(1) operations at both ends.
- Can be used as both stack and queue
- Useful for sliding window problems

### Circular Queue
Fixed-size buffer that wraps around.
- Efficient memory usage
- Used in buffer implementations

### Priority Queue
Elements dequeued by priority, not insertion order.
- See [Heaps](../heaps/README.md) for implementation

### Monotonic Deque
Maintain elements in sorted order for sliding window max/min.

## Common Patterns

### BFS Template
```python
from collections import deque

def bfs(start):
    queue = deque([start])
    visited = {start}

    while queue:
        node = queue.popleft()
        process(node)

        for neighbor in get_neighbors(node):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)
```

### Level Order Traversal
```python
def level_order(root):
    if not root:
        return []

    result = []
    queue = deque([root])

    while queue:
        level_size = len(queue)
        level = []

        for _ in range(level_size):
            node = queue.popleft()
            level.append(node.val)

            if node.left:
                queue.append(node.left)
            if node.right:
                queue.append(node.right)

        result.append(level)

    return result
```

### Sliding Window Maximum (Monotonic Deque)
```python
from collections import deque

def max_sliding_window(nums, k):
    result = []
    dq = deque()  # stores indices

    for i, num in enumerate(nums):
        # Remove elements outside window
        while dq and dq[0] < i - k + 1:
            dq.popleft()

        # Remove smaller elements
        while dq and nums[dq[-1]] < num:
            dq.pop()

        dq.append(i)

        if i >= k - 1:
            result.append(nums[dq[0]])

    return result
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 102 | Binary Tree Level Order Traversal | Medium | Level order |
| 103 | Binary Tree Zigzag Level Order | Medium | Level order |
| 107 | Binary Tree Level Order II | Medium | Level order |
| 116 | Populating Next Right Pointers | Medium | Level order |
| 199 | Binary Tree Right Side View | Medium | Level order |
| 225 | Implement Stack using Queues | Easy | Queue ops |
| 232 | Implement Queue using Stacks | Easy | Stack ops |
| 239 | Sliding Window Maximum | Hard | Monotonic deque |
| 279 | Perfect Squares | Medium | BFS |
| 286 | Walls and Gates | Medium | Multi-source BFS |
| 297 | Serialize and Deserialize Binary Tree | Hard | Level order |
| 346 | Moving Average from Data Stream | Easy | Circular queue |
| 362 | Design Hit Counter | Medium | Queue |
| 429 | N-ary Tree Level Order Traversal | Medium | Level order |
| 542 | 01 Matrix | Medium | Multi-source BFS |
| 622 | Design Circular Queue | Medium | Circular queue |
| 641 | Design Circular Deque | Medium | Circular deque |
| 752 | Open the Lock | Medium | BFS |
| 862 | Shortest Subarray with Sum at Least K | Hard | Monotonic deque |
| 933 | Number of Recent Calls | Easy | Queue |
| 934 | Shortest Bridge | Medium | BFS |
| 994 | Rotting Oranges | Medium | Multi-source BFS |
| 1091 | Shortest Path in Binary Matrix | Medium | BFS |

## Implementation Notes

### Array-based Circular Queue
```
Front: Index where next dequeue happens
Rear: Index where next enqueue happens
Size tracking: (rear - front + capacity) % capacity

     front         rear
       ↓            ↓
┌───┬───┬───┬───┬───┬───┬───┬───┐
│   │ A │ B │ C │ D │   │   │   │
└───┴───┴───┴───┴───┴───┴───┴───┘
```

### Linked List-based Queue
- Front: Head of list (dequeue)
- Rear: Tail of list (enqueue)
- Maintain both head and tail pointers

## See Also

- [Stacks](../stacks/README.md) - LIFO counterpart
- [Heaps](../heaps/README.md) - Priority queue implementation
- [Graphs](../graphs/README.md) - BFS uses queue
- [Trees](../trees/README.md) - Level order traversal
