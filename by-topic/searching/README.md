# Searching Algorithms

Searching algorithms find elements or determine their absence in data structures, ranging from simple linear scans to sophisticated divide-and-conquer approaches.

## Implementations

| Algorithm | Python | C |
|-----------|--------|---|
| All Searching Algorithms | [searching.py](../../by-language/python/algorithms/searching.py) | [searching.c](../../by-language/c/algorithms/searching.c) |

## Complexity Comparison

```
┌─────────────────────┬─────────┬─────────┬─────────┐
│ Algorithm           │ Best    │ Average │ Worst   │
├─────────────────────┼─────────┼─────────┼─────────┤
│ Linear Search       │ O(1)    │ O(n)    │ O(n)    │
│ Binary Search       │ O(1)    │ O(log n)│ O(log n)│
│ Jump Search         │ O(1)    │ O(√n)   │ O(√n)   │
│ Interpolation Search│ O(1)    │O(log log n)│ O(n) │
│ Exponential Search  │ O(1)    │ O(log i)│ O(log n)│
└─────────────────────┴─────────┴─────────┴─────────┘
i = position of target
```

## Binary Search

The most important searching algorithm. Requires sorted data.

### Standard Template
```python
def binary_search(arr, target):
    left, right = 0, len(arr) - 1

    while left <= right:
        mid = left + (right - left) // 2

        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1  # Not found
```

### Lower Bound (First >= target)
```python
def lower_bound(arr, target):
    left, right = 0, len(arr)

    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] < target:
            left = mid + 1
        else:
            right = mid

    return left
```

### Upper Bound (First > target)
```python
def upper_bound(arr, target):
    left, right = 0, len(arr)

    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] <= target:
            left = mid + 1
        else:
            right = mid

    return left
```

## Two Pointers Technique

Use two indices to traverse data structures efficiently.

### Opposite Direction
```python
# Two Sum (sorted array)
def two_sum_sorted(arr, target):
    left, right = 0, len(arr) - 1

    while left < right:
        curr_sum = arr[left] + arr[right]
        if curr_sum == target:
            return [left, right]
        elif curr_sum < target:
            left += 1
        else:
            right -= 1

    return []
```

### Same Direction (Fast/Slow)
```python
# Remove duplicates in-place
def remove_duplicates(arr):
    if not arr:
        return 0

    slow = 0
    for fast in range(1, len(arr)):
        if arr[fast] != arr[slow]:
            slow += 1
            arr[slow] = arr[fast]

    return slow + 1
```

## Sliding Window Technique

Maintain a window over contiguous elements for subarray/substring problems.

### Fixed Size Window
```python
def max_sum_subarray(arr, k):
    if len(arr) < k:
        return None

    window_sum = sum(arr[:k])
    max_sum = window_sum

    for i in range(k, len(arr)):
        window_sum += arr[i] - arr[i - k]
        max_sum = max(max_sum, window_sum)

    return max_sum
```

### Variable Size Window
```python
def min_subarray_len(target, arr):
    left = 0
    curr_sum = 0
    min_len = float('inf')

    for right in range(len(arr)):
        curr_sum += arr[right]

        while curr_sum >= target:
            min_len = min(min_len, right - left + 1)
            curr_sum -= arr[left]
            left += 1

    return min_len if min_len != float('inf') else 0
```

### Sliding Window with Hash Map
```python
def longest_substring_without_repeat(s):
    char_index = {}
    left = 0
    max_len = 0

    for right, char in enumerate(s):
        if char in char_index and char_index[char] >= left:
            left = char_index[char] + 1
        char_index[char] = right
        max_len = max(max_len, right - left + 1)

    return max_len
```

## Binary Search Variants

### Search in Rotated Sorted Array
```python
def search_rotated(arr, target):
    left, right = 0, len(arr) - 1

    while left <= right:
        mid = (left + right) // 2

        if arr[mid] == target:
            return mid

        # Left half is sorted
        if arr[left] <= arr[mid]:
            if arr[left] <= target < arr[mid]:
                right = mid - 1
            else:
                left = mid + 1
        # Right half is sorted
        else:
            if arr[mid] < target <= arr[right]:
                left = mid + 1
            else:
                right = mid - 1

    return -1
```

### Binary Search on Answer
When the answer space is monotonic, binary search can find optimal values.

```python
# Find minimum capacity to ship within D days
def ship_within_days(weights, days):
    def can_ship(capacity):
        day_count = 1
        current_load = 0
        for weight in weights:
            if current_load + weight > capacity:
                day_count += 1
                current_load = weight
            else:
                current_load += weight
        return day_count <= days

    left = max(weights)
    right = sum(weights)

    while left < right:
        mid = (left + right) // 2
        if can_ship(mid):
            right = mid
        else:
            left = mid + 1

    return left
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 3 | Longest Substring Without Repeating | Medium | Sliding window |
| 4 | Median of Two Sorted Arrays | Hard | Binary search |
| 11 | Container With Most Water | Medium | Two pointers |
| 15 | 3Sum | Medium | Two pointers |
| 16 | 3Sum Closest | Medium | Two pointers |
| 26 | Remove Duplicates from Sorted Array | Easy | Two pointers |
| 27 | Remove Element | Easy | Two pointers |
| 33 | Search in Rotated Sorted Array | Medium | Binary search |
| 34 | Find First and Last Position | Medium | Binary search |
| 35 | Search Insert Position | Easy | Binary search |
| 42 | Trapping Rain Water | Hard | Two pointers |
| 69 | Sqrt(x) | Easy | Binary search |
| 74 | Search a 2D Matrix | Medium | Binary search |
| 76 | Minimum Window Substring | Hard | Sliding window |
| 81 | Search in Rotated Sorted Array II | Medium | Binary search |
| 153 | Find Minimum in Rotated Sorted Array | Medium | Binary search |
| 162 | Find Peak Element | Medium | Binary search |
| 167 | Two Sum II - Sorted | Medium | Two pointers |
| 209 | Minimum Size Subarray Sum | Medium | Sliding window |
| 240 | Search a 2D Matrix II | Medium | Binary search |
| 278 | First Bad Version | Easy | Binary search |
| 283 | Move Zeroes | Easy | Two pointers |
| 287 | Find the Duplicate Number | Medium | Two pointers |
| 340 | Longest Substring with At Most K Distinct | Medium | Sliding window |
| 349 | Intersection of Two Arrays | Easy | Two pointers |
| 374 | Guess Number Higher or Lower | Easy | Binary search |
| 392 | Is Subsequence | Easy | Two pointers |
| 410 | Split Array Largest Sum | Hard | Binary search |
| 424 | Longest Repeating Character Replacement | Medium | Sliding window |
| 438 | Find All Anagrams in a String | Medium | Sliding window |
| 454 | 4Sum II | Medium | Two pointers |
| 567 | Permutation in String | Medium | Sliding window |
| 611 | Valid Triangle Number | Medium | Two pointers |
| 633 | Sum of Square Numbers | Medium | Two pointers |
| 658 | Find K Closest Elements | Medium | Binary search |
| 704 | Binary Search | Easy | Binary search |
| 713 | Subarray Product Less Than K | Medium | Sliding window |
| 719 | Find Kth Smallest Pair Distance | Hard | Binary search |
| 852 | Peak Index in a Mountain Array | Medium | Binary search |
| 875 | Koko Eating Bananas | Medium | Binary search on answer |
| 904 | Fruit Into Baskets | Medium | Sliding window |
| 977 | Squares of a Sorted Array | Easy | Two pointers |
| 981 | Time Based Key-Value Store | Medium | Binary search |
| 1004 | Max Consecutive Ones III | Medium | Sliding window |
| 1011 | Capacity To Ship Packages | Medium | Binary search on answer |
| 1438 | Longest Subarray with Abs Diff | Medium | Sliding window |

## Choosing the Right Technique

| Problem Type | Technique |
|--------------|-----------|
| Find element in sorted array | Binary search |
| Find pair with target sum | Two pointers |
| Subarray/substring with constraint | Sliding window |
| Minimize/maximize with monotonic property | Binary search on answer |
| Linked list cycle detection | Fast/slow pointers |
| Merge two sorted arrays | Two pointers |
| Partition array | Two pointers |

## See Also

- [Arrays](../arrays/README.md) - Primary data structure for searching
- [Sorting](../sorting/README.md) - Binary search requires sorted data
- [Linked Lists](../linked-lists/README.md) - Fast/slow pointer technique
- [Strings](../strings/README.md) - Sliding window for substrings
