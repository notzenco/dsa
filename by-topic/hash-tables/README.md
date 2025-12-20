# Hash Tables

Hash tables provide O(1) average-case lookup, insertion, and deletion by mapping keys to array indices using hash functions.

## Implementations

| Structure | Python | C |
|-----------|--------|---|
| Hash Table | [hash_table.py](../../by-language/python/data_structures/hash_table.py) | [hash_table.c](../../by-language/c/data-structures/hash_table.c) |
| Bloom Filter | [bloom_filter.py](../../by-language/python/data_structures/bloom_filter.py) | [bloom_filter.c](../../by-language/c/data-structures/bloom_filter.c) |
| LRU Cache | [lru_cache.py](../../by-language/python/data_structures/lru_cache.py) | [lru_cache.c](../../by-language/c/data-structures/lru_cache.c) |
| LFU Cache | [lfu_cache.py](../../by-language/python/data_structures/lfu_cache.py) | [lfu_cache.c](../../by-language/c/data-structures/lfu_cache.c) |

## Complexity

### Hash Table
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

### Bloom Filter
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Insert      │ O(k)    │ O(m)    │
│ Query       │ O(k)    │         │
└─────────────┴─────────┴─────────┘
k = number of hash functions, m = bit array size
```

### LRU/LFU Cache
```
┌─────────────┬─────────┬─────────┐
│ Operation   │ Time    │ Space   │
├─────────────┼─────────┼─────────┤
│ Get         │ O(1)    │ O(n)    │
│ Put         │ O(1)    │         │
└─────────────┴─────────┴─────────┘
```

## When to Use

- Need O(1) average lookup/insert/delete
- Key-value associations
- Counting occurrences
- Detecting duplicates
- Caching
- Grouping elements by property

## When NOT to Use

- Need sorted order (use BST or sorted array)
- Need range queries
- Keys aren't hashable
- Need guaranteed O(1) (worst case is O(n))

## Collision Resolution

### Chaining
Each bucket contains a linked list of entries.
- **Pros**: Simple, handles high load gracefully
- **Cons**: Pointer overhead, cache unfriendly

### Open Addressing
Probe for next available slot.
- **Linear Probing**: Check next slot sequentially
- **Quadratic Probing**: Check i² slots away
- **Double Hashing**: Use second hash function

| Aspect | Chaining | Open Addressing |
|--------|----------|-----------------|
| Load factor | Can exceed 1 | Must be < 1 |
| Memory | Pointer overhead | No extra pointers |
| Cache | Poor locality | Better locality |
| Deletion | Simple | Requires tombstones |

## Common Patterns

### Two Sum Pattern
Use hash map to find complement in O(1).
```python
def two_sum(nums, target):
    seen = {}
    for i, num in enumerate(nums):
        complement = target - num
        if complement in seen:
            return [seen[complement], i]
        seen[num] = i
    return []
```

### Frequency Counting
```python
from collections import Counter
freq = Counter(arr)
most_common = freq.most_common(k)
```

### Group Anagrams
```python
from collections import defaultdict
def group_anagrams(strs):
    groups = defaultdict(list)
    for s in strs:
        key = tuple(sorted(s))
        groups[key].append(s)
    return list(groups.values())
```

### LRU Cache Implementation
Combine hash map with doubly linked list:
- Hash map: O(1) access to nodes
- Doubly linked list: O(1) move to front, remove from end

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 1 | Two Sum | Easy | Complement lookup |
| 3 | Longest Substring Without Repeating | Medium | Sliding window + hash |
| 36 | Valid Sudoku | Medium | Set checking |
| 49 | Group Anagrams | Medium | Grouping |
| 76 | Minimum Window Substring | Hard | Sliding window + hash |
| 128 | Longest Consecutive Sequence | Medium | Set lookup |
| 136 | Single Number | Easy | XOR (or hash) |
| 146 | LRU Cache | Medium | Hash + DLL |
| 149 | Max Points on a Line | Hard | Slope counting |
| 166 | Fraction to Recurring Decimal | Medium | Cycle detection |
| 187 | Repeated DNA Sequences | Medium | Rolling hash |
| 202 | Happy Number | Easy | Cycle detection |
| 205 | Isomorphic Strings | Easy | Two-way mapping |
| 217 | Contains Duplicate | Easy | Set |
| 219 | Contains Duplicate II | Easy | Hash with index |
| 242 | Valid Anagram | Easy | Frequency count |
| 290 | Word Pattern | Easy | Bijection |
| 347 | Top K Frequent Elements | Medium | Frequency + heap |
| 350 | Intersection of Two Arrays II | Easy | Frequency count |
| 380 | Insert Delete GetRandom O(1) | Medium | Hash + array |
| 387 | First Unique Character | Easy | Frequency count |
| 438 | Find All Anagrams | Medium | Sliding window |
| 454 | 4Sum II | Medium | Hash counting |
| 460 | LFU Cache | Hard | Hash + freq buckets |
| 523 | Continuous Subarray Sum | Medium | Prefix sum mod |
| 560 | Subarray Sum Equals K | Medium | Prefix sum + hash |
| 763 | Partition Labels | Medium | Last occurrence |

## Bloom Filter Details

### Properties
- **No false negatives**: If "not in set", definitely not
- **Possible false positives**: If "in set", might be wrong
- **Cannot delete**: Use Counting Bloom Filter for deletions

### Use Cases
- Membership testing with space constraints
- Cache before expensive lookups
- Duplicate detection
- Network packet filtering

### False Positive Rate
```
FPR ≈ (1 - e^(-kn/m))^k
```
Where: k = hash functions, n = items, m = bits

## See Also

- [Trees](../trees/README.md) - Ordered alternatives (BST, B-Tree)
- [Strings](../strings/README.md) - String hashing algorithms
- [Arrays](../arrays/README.md) - Two sum and related patterns
