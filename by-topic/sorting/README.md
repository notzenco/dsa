# Sorting Algorithms

Sorting algorithms arrange elements in a specific order (ascending or descending), fundamental to many other algorithms and data processing tasks.

## Implementations

| Algorithm | Python | C |
|-----------|--------|---|
| All Sorting Algorithms | [sorting.py](../../by-language/python/algorithms/sorting.py) | [sorting.c](../../by-language/c/algorithms/sorting.c) |

## Complexity Comparison

```
┌──────────────────┬─────────┬─────────┬─────────┬────────┬────────┐
│ Algorithm        │ Best    │ Average │ Worst   │ Space  │ Stable │
├──────────────────┼─────────┼─────────┼─────────┼────────┼────────┤
│ Bubble Sort      │ O(n)    │ O(n²)   │ O(n²)   │ O(1)   │ Yes    │
│ Selection Sort   │ O(n²)   │ O(n²)   │ O(n²)   │ O(1)   │ No     │
│ Insertion Sort   │ O(n)    │ O(n²)   │ O(n²)   │ O(1)   │ Yes    │
│ Merge Sort       │ O(n lg n)│O(n lg n)│O(n lg n)│ O(n)   │ Yes    │
│ Quick Sort       │ O(n lg n)│O(n lg n)│ O(n²)   │O(lg n) │ No     │
│ Heap Sort        │ O(n lg n)│O(n lg n)│O(n lg n)│ O(1)   │ No     │
│ Counting Sort    │ O(n+k)  │ O(n+k)  │ O(n+k)  │ O(k)   │ Yes    │
│ Radix Sort       │ O(nk)   │ O(nk)   │ O(nk)   │ O(n+k) │ Yes    │
│ Bucket Sort      │ O(n+k)  │ O(n+k)  │ O(n²)   │ O(n)   │ Yes    │
└──────────────────┴─────────┴─────────┴─────────┴────────┴────────┘
n = number of elements, k = range of values
```

## Algorithm Selection Guide

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

## Algorithm Details

### Comparison-Based Sorts

#### Bubble Sort
Repeatedly swap adjacent elements if out of order.
- **Best for**: Educational purposes, detecting if already sorted
- **Optimization**: Early exit if no swaps in a pass

#### Selection Sort
Find minimum and swap to front, repeat for remaining.
- **Best for**: When memory writes are expensive
- **Note**: Always O(n²), even for sorted input

#### Insertion Sort
Insert each element into its correct position in sorted portion.
- **Best for**: Small arrays, nearly sorted data, online sorting
- **Used in**: Hybrid sorts (Timsort, Introsort) for small subarrays

#### Merge Sort
Divide array in half, sort recursively, merge sorted halves.
- **Best for**: When stability is required, linked lists
- **Note**: Consistent O(n log n), requires O(n) extra space

#### Quick Sort
Choose pivot, partition around it, sort partitions recursively.
- **Best for**: General purpose, in-memory sorting
- **Optimization**: Randomized pivot, 3-way partition for duplicates

#### Heap Sort
Build max-heap, repeatedly extract maximum.
- **Best for**: When O(1) space is required
- **Note**: Poor cache performance compared to Quick Sort

### Non-Comparison Sorts

#### Counting Sort
Count occurrences, calculate positions, place elements.
- **Best for**: Small integer range, many duplicates
- **Requirement**: Know range of values

#### Radix Sort
Sort by each digit, from least to most significant.
- **Best for**: Fixed-length integers, strings
- **Variants**: LSD (least significant digit first), MSD

#### Bucket Sort
Distribute into buckets, sort each bucket, concatenate.
- **Best for**: Uniformly distributed floating-point numbers
- **Note**: Degrades to O(n²) with poor distribution

## Common Patterns

### Quick Sort Partition (Lomuto)
```python
def partition(arr, low, high):
    pivot = arr[high]
    i = low - 1

    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]

    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1
```

### Merge Operation
```python
def merge(arr, left, mid, right):
    L = arr[left:mid + 1]
    R = arr[mid + 1:right + 1]

    i = j = 0
    k = left

    while i < len(L) and j < len(R):
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1

    while i < len(L):
        arr[k] = L[i]
        i += 1
        k += 1

    while j < len(R):
        arr[k] = R[j]
        j += 1
        k += 1
```

### Custom Comparators
```python
# Sort by multiple keys
arr.sort(key=lambda x: (x[0], -x[1]))

# Sort objects
students.sort(key=lambda s: s.grade, reverse=True)

# Using functools for complex comparisons
from functools import cmp_to_key
arr.sort(key=cmp_to_key(compare_function))
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 56 | Merge Intervals | Medium | Sort + merge |
| 75 | Sort Colors | Medium | Dutch flag |
| 88 | Merge Sorted Array | Easy | Merge |
| 147 | Insertion Sort List | Medium | Insertion sort |
| 148 | Sort List | Medium | Merge sort |
| 164 | Maximum Gap | Medium | Bucket/radix sort |
| 179 | Largest Number | Medium | Custom comparator |
| 215 | Kth Largest Element | Medium | Quick select |
| 242 | Valid Anagram | Easy | Counting sort |
| 274 | H-Index | Medium | Counting sort |
| 280 | Wiggle Sort | Medium | Custom ordering |
| 324 | Wiggle Sort II | Medium | Quick select |
| 347 | Top K Frequent | Medium | Bucket sort |
| 349 | Intersection of Two Arrays | Easy | Sort + pointers |
| 350 | Intersection II | Easy | Sort + pointers |
| 451 | Sort Characters By Frequency | Medium | Counting |
| 493 | Reverse Pairs | Hard | Merge sort |
| 524 | Longest Word in Dictionary | Medium | Custom sort |
| 561 | Array Partition | Easy | Sort |
| 581 | Shortest Unsorted Subarray | Medium | Sort comparison |
| 912 | Sort an Array | Medium | Any O(n log n) |
| 969 | Pancake Sorting | Medium | Custom sort |
| 973 | K Closest Points | Medium | Quick select |
| 1051 | Height Checker | Easy | Counting sort |
| 1122 | Relative Sort Array | Easy | Custom order |
| 1288 | Remove Covered Intervals | Medium | Sort + greedy |
| 1329 | Sort Matrix Diagonally | Medium | Multi-sort |
| 1636 | Sort Array by Increasing Frequency | Easy | Custom sort |

## Stability in Sorting

A sorting algorithm is **stable** if equal elements maintain their relative order.

```
Before: [(3,'a'), (1,'b'), (3,'c'), (2,'d')]
Stable:   [(1,'b'), (2,'d'), (3,'a'), (3,'c')]  ← 'a' before 'c'
Unstable: [(1,'b'), (2,'d'), (3,'c'), (3,'a')]  ← order may change
```

**Stable**: Bubble, Insertion, Merge, Counting, Radix, Bucket
**Unstable**: Selection, Quick, Heap

## Hybrid Algorithms

### Timsort (Python's default)
- Merge sort + insertion sort
- Exploits existing order (natural runs)
- O(n log n) worst, O(n) best

### Introsort (C++'s std::sort)
- Quick sort + heap sort + insertion sort
- Switches to heap sort if recursion too deep
- Guaranteed O(n log n) worst case

## See Also

- [Arrays](../arrays/README.md) - Data structure being sorted
- [Heaps](../heaps/README.md) - Heap sort implementation
- [Searching](../searching/README.md) - Binary search requires sorted data
