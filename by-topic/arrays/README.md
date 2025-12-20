# Arrays

Arrays are contiguous blocks of memory that store elements of the same type, providing O(1) random access by index.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Dynamic Array | [dynamic_array.py](../../by-language/python/data_structures/dynamic_array.py) | [dynamic_array.c](../../by-language/c/data-structures/dynamic_array.c) |

## Complexity

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

## When to Use

- Need O(1) random access by index
- Data size is known or changes infrequently
- Memory locality matters (cache-friendly)
- Implementing other data structures (heaps, hash tables)

## When NOT to Use

- Frequent insertions/deletions in the middle
- Unknown or highly variable size
- Need to frequently search for values

## Common Patterns

### Two Pointers
Use two indices to traverse from both ends or at different speeds.
- Container with most water
- Remove duplicates in sorted array
- Three sum problems

### Sliding Window
Maintain a window over contiguous elements.
- Maximum sum subarray of size k
- Longest substring without repeating characters
- Minimum window substring

### Prefix Sum
Precompute cumulative sums for O(1) range queries.
- Range sum queries
- Subarray sum equals k
- Product of array except self

## Related LeetCode Problems

| # | Problem | Difficulty |
|---|---------|------------|
| 1 | Two Sum | Easy |
| 11 | Container With Most Water | Medium |
| 15 | 3Sum | Medium |
| 26 | Remove Duplicates from Sorted Array | Easy |
| 27 | Remove Element | Easy |
| 53 | Maximum Subarray | Medium |
| 121 | Best Time to Buy and Sell Stock | Easy |
| 238 | Product of Array Except Self | Medium |
| 283 | Move Zeroes | Easy |
| 560 | Subarray Sum Equals K | Medium |

## See Also

- [Searching Algorithms](../searching/README.md) - Binary search on sorted arrays
- [Sorting Algorithms](../sorting/README.md) - Array sorting techniques
- [Two Pointers & Sliding Window](../searching/README.md) - Array traversal patterns
