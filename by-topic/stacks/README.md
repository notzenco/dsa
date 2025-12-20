# Stacks

A stack is a Last-In-First-Out (LIFO) data structure where elements are added and removed from the same end (the top).

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Stack | [stack.py](../../by-language/python/data_structures/stack.py) | [stack.c](../../by-language/c/data-structures/stack.c) |

## Complexity

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

## When to Use

- Undo/Redo functionality
- Expression evaluation & parsing
- Backtracking algorithms (DFS, maze solving)
- Function call management (call stack)
- Balanced parentheses checking
- Browser history (back button)
- Syntax parsing (compilers)

## Stack Variants

### Min/Max Stack
Track minimum/maximum in O(1) with extra O(n) space.
- Store pairs of (value, current_min) or (value, current_max)
- Or maintain auxiliary stack of min/max values

### Monotonic Stack
Maintain elements in sorted order.
- **Monotonically Increasing**: Pop larger elements before push
- **Monotonically Decreasing**: Pop smaller elements before push
- Used for next greater/smaller element problems

## Common Patterns

### Balanced Parentheses
Push opening brackets, pop and match closing brackets.
```
Input: "([{}])"
Stack: [ -> [( -> [([ -> [({ -> [([ -> [( -> [ -> empty ✓
```

### Expression Evaluation
- **Infix to Postfix**: Use operator precedence
- **Postfix Evaluation**: Push operands, pop for operators

### Monotonic Stack for Next Greater Element
```python
def next_greater(arr):
    result = [-1] * len(arr)
    stack = []
    for i in range(len(arr)):
        while stack and arr[stack[-1]] < arr[i]:
            result[stack.pop()] = arr[i]
        stack.append(i)
    return result
```

### Iterative DFS
Replace recursion with explicit stack.
```python
def dfs_iterative(root):
    stack = [root]
    while stack:
        node = stack.pop()
        process(node)
        for child in reversed(node.children):
            stack.append(child)
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 20 | Valid Parentheses | Easy | Matching |
| 32 | Longest Valid Parentheses | Hard | Stack + DP |
| 42 | Trapping Rain Water | Hard | Monotonic |
| 71 | Simplify Path | Medium | Stack |
| 84 | Largest Rectangle in Histogram | Hard | Monotonic |
| 85 | Maximal Rectangle | Hard | Monotonic |
| 94 | Binary Tree Inorder Traversal | Easy | Iterative DFS |
| 150 | Evaluate Reverse Polish Notation | Medium | Evaluation |
| 155 | Min Stack | Medium | Min Stack |
| 224 | Basic Calculator | Hard | Expression |
| 227 | Basic Calculator II | Medium | Expression |
| 316 | Remove Duplicate Letters | Medium | Monotonic |
| 394 | Decode String | Medium | Nested structure |
| 456 | 132 Pattern | Medium | Monotonic |
| 496 | Next Greater Element I | Easy | Monotonic |
| 503 | Next Greater Element II | Medium | Monotonic |
| 739 | Daily Temperatures | Medium | Monotonic |
| 844 | Backspace String Compare | Easy | Stack |
| 901 | Online Stock Span | Medium | Monotonic |
| 946 | Validate Stack Sequences | Medium | Simulation |

## Implementation Notes

### Array-based vs Linked List-based

| Aspect | Array-based | Linked List-based |
|--------|-------------|-------------------|
| Memory | Contiguous, may resize | Per-node allocation |
| Cache | Better locality | Pointer chasing |
| Size | Fixed or dynamic | Always dynamic |
| Simplicity | Simpler | More overhead |

## See Also

- [Queues](../queues/README.md) - FIFO counterpart
- [Trees](../trees/README.md) - Tree traversals use stacks
- [Graphs](../graphs/README.md) - DFS uses stack
