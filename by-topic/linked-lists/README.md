# Linked Lists

Linked lists are linear data structures where elements are stored in nodes, with each node containing data and a reference to the next (and optionally previous) node.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Singly Linked List | [singly_linked_list.py](../../by-language/python/data_structures/singly_linked_list.py) | [singly_linked_list.c](../../by-language/c/data-structures/singly_linked_list.c) |
| Doubly Linked List | [doubly_linked_list.py](../../by-language/python/data_structures/doubly_linked_list.py) | [doubly_linked_list.c](../../by-language/c/data-structures/doubly_linked_list.c) |

## Complexity

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

## When to Use

- Frequent insertions/deletions at ends
- Don't need random access
- Implementing stacks, queues
- Unknown data size that changes frequently
- Need to splice/merge lists efficiently

## When NOT to Use

- Need random access by index
- Memory is constrained (pointer overhead)
- Cache performance is critical

## Singly vs Doubly Linked List

| Aspect | Singly | Doubly |
|--------|--------|--------|
| Memory | 1 pointer/node | 2 pointers/node |
| Traverse backward | O(n) from head | O(1) from any node |
| Delete given node | Need previous | O(1) |
| Implementation | Simpler | More complex |

## Common Patterns

### Fast and Slow Pointers (Floyd's)
Use two pointers moving at different speeds.
- Detect cycle in linked list
- Find middle of linked list
- Find start of cycle

### Dummy Head Node
Use a sentinel node to simplify edge cases.
- Merge sorted lists
- Remove elements
- Partition list

### Reverse Linked List
Iterative or recursive reversal techniques.
- Reverse entire list
- Reverse in groups of k
- Reverse between positions

## Related LeetCode Problems

| # | Problem | Difficulty |
|---|---------|------------|
| 2 | Add Two Numbers | Medium |
| 19 | Remove Nth Node From End of List | Medium |
| 21 | Merge Two Sorted Lists | Easy |
| 23 | Merge k Sorted Lists | Hard |
| 25 | Reverse Nodes in k-Group | Hard |
| 83 | Remove Duplicates from Sorted List | Easy |
| 92 | Reverse Linked List II | Medium |
| 141 | Linked List Cycle | Easy |
| 142 | Linked List Cycle II | Medium |
| 143 | Reorder List | Medium |
| 146 | LRU Cache | Medium |
| 160 | Intersection of Two Linked Lists | Easy |
| 206 | Reverse Linked List | Easy |
| 234 | Palindrome Linked List | Easy |
| 328 | Odd Even Linked List | Medium |

## See Also

- [Stacks](../stacks/README.md) - Can be implemented with linked lists
- [Queues](../queues/README.md) - Can be implemented with linked lists
- [Hash Tables](../hash-tables/README.md) - LRU Cache uses doubly linked list
