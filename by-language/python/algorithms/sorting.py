"""
Sorting Algorithms

╔════════════════════════════════════════════════════════════════════════════╗
║                         SORTING ALGORITHMS OVERVIEW                        ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Comparison-Based Sorts:                                                   ║
║  ┌─────────────────┬────────────┬────────────┬────────────┬────────┐       ║
║  │ Algorithm       │ Best       │ Average    │ Worst      │ Stable │       ║
║  ├─────────────────┼────────────┼────────────┼────────────┼────────┤       ║
║  │ Bubble Sort     │ O(n)       │ O(n²)      │ O(n²)      │ Yes    │       ║
║  │ Selection Sort  │ O(n²)      │ O(n²)      │ O(n²)      │ No     │       ║
║  │ Insertion Sort  │ O(n)       │ O(n²)      │ O(n²)      │ Yes    │       ║
║  │ Merge Sort      │ O(n log n) │ O(n log n) │ O(n log n) │ Yes    │       ║
║  │ Quick Sort      │ O(n log n) │ O(n log n) │ O(n²)      │ No     │       ║
║  │ Heap Sort       │ O(n log n) │ O(n log n) │ O(n log n) │ No     │       ║
║  │ Shell Sort      │ O(n log n) │ O(n^1.3)   │ O(n²)      │ No     │       ║
║  └─────────────────┴────────────┴────────────┴────────────┴────────┘       ║
║                                                                            ║
║  Non-Comparison Sorts:                                                     ║
║  ┌─────────────────┬────────────┬────────────┬────────────┬────────┐       ║
║  │ Algorithm       │ Time       │ Space      │ Constraint │ Stable │       ║
║  ├─────────────────┼────────────┼────────────┼────────────┼────────┤       ║
║  │ Counting Sort   │ O(n + k)   │ O(k)       │ Integers   │ Yes    │       ║
║  │ Radix Sort      │ O(d(n+k))  │ O(n + k)   │ Integers   │ Yes    │       ║
║  │ Bucket Sort     │ O(n + k)   │ O(n)       │ Uniform    │ Yes    │       ║
║  └─────────────────┴────────────┴────────────┴────────────┴────────┘       ║
║                                                                            ║
║  Bubble Sort Visualization:                                                ║
║  [5, 3, 8, 4, 2] → [3, 5, 4, 2, 8] → [3, 4, 2, 5, 8] → ... → [2,3,4,5,8]   ║
║    ↑ ↑ compare & swap if needed, largest bubbles to end                    ║
║                                                                            ║
║  Merge Sort Visualization:                                                 ║
║  [5, 3, 8, 4, 2, 7, 1, 6]                                                  ║
║        /               \                  Split                            ║
║  [5, 3, 8, 4]    [2, 7, 1, 6]                                              ║
║      /    \         /    \               Split more                        ║
║  [5,3]  [8,4]   [2,7]  [1,6]                                               ║
║    |      |       |      |               Merge pairs                       ║
║  [3,5]  [4,8]   [2,7]  [1,6]                                               ║
║      \    /         \    /               Merge                             ║
║  [3, 4, 5, 8]    [1, 2, 6, 7]                                              ║
║        \               /                 Final merge                       ║
║  [1, 2, 3, 4, 5, 6, 7, 8]                                                  ║
║                                                                            ║
║  Quick Sort (Partition):                                                   ║
║  [5, 3, 8, 4, 2, 7, 1, 6]  pivot=5                                         ║
║  [3, 4, 2, 1] [5] [8, 7, 6]  elements < 5 | pivot | elements > 5           ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

LEETCODE PROBLEMS:
- #912 Sort an Array
- #75 Sort Colors (Dutch National Flag)
- #148 Sort List (Merge Sort on Linked List)
- #215 Kth Largest Element (Quick Select)
- #347 Top K Frequent Elements
- #56 Merge Intervals (sort then merge)
- #179 Largest Number (custom comparator)
- #973 K Closest Points to Origin

USE CASES:
- Bubble/Selection/Insertion: Small arrays, nearly sorted data, educational
- Merge Sort: Stable sort needed, linked lists, external sorting
- Quick Sort: General purpose, in-place sorting, cache-friendly
- Heap Sort: Guaranteed O(n log n), memory-constrained
- Counting/Radix: Integer sorting, known range, linear time needed
- Bucket Sort: Uniformly distributed floating-point numbers
"""

from typing import TypeVar, List, Callable, Optional
import random

T = TypeVar('T')


# =============================================================================
# COMPARISON-BASED SORTING ALGORITHMS
# =============================================================================


def bubble_sort(arr: List[T]) -> List[T]:
    """
    Bubble Sort - repeatedly swap adjacent elements if in wrong order.

    Time Complexity: O(n²) average/worst, O(n) best (already sorted)
    Space Complexity: O(1)
    Stable: Yes

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    n = len(result)

    for i in range(n):
        swapped = False

        for j in range(0, n - i - 1):
            if result[j] > result[j + 1]:
                result[j], result[j + 1] = result[j + 1], result[j]
                swapped = True

        # Optimization: if no swaps, array is sorted
        if not swapped:
            break

    return result


def selection_sort(arr: List[T]) -> List[T]:
    """
    Selection Sort - find minimum and place at beginning.

    Time Complexity: O(n²) all cases
    Space Complexity: O(1)
    Stable: No (can be made stable with insertion instead of swap)

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    n = len(result)

    for i in range(n):
        min_idx = i

        for j in range(i + 1, n):
            if result[j] < result[min_idx]:
                min_idx = j

        result[i], result[min_idx] = result[min_idx], result[i]

    return result


def insertion_sort(arr: List[T]) -> List[T]:
    """
    Insertion Sort - build sorted portion one element at a time.

    Time Complexity: O(n²) average/worst, O(n) best (nearly sorted)
    Space Complexity: O(1)
    Stable: Yes

    Excellent for small arrays and nearly sorted data.
    Used as base case in hybrid sorts (Timsort, Introsort).

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    n = len(result)

    for i in range(1, n):
        key = result[i]
        j = i - 1

        while j >= 0 and result[j] > key:
            result[j + 1] = result[j]
            j -= 1

        result[j + 1] = key

    return result


def merge_sort(arr: List[T]) -> List[T]:
    """
    Merge Sort - divide and conquer with merging.

    Time Complexity: O(n log n) all cases
    Space Complexity: O(n)
    Stable: Yes

    Preferred for linked lists and when stability is required.

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    if len(arr) <= 1:
        return arr.copy()

    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])

    return _merge(left, right)


def _merge(left: List[T], right: List[T]) -> List[T]:
    """Merge two sorted lists into one sorted list."""
    result: List[T] = []
    i = j = 0

    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    result.extend(left[i:])
    result.extend(right[j:])

    return result


def quick_sort(arr: List[T]) -> List[T]:
    """
    Quick Sort - divide and conquer with partitioning.

    Time Complexity: O(n log n) average, O(n²) worst (sorted input with bad pivot)
    Space Complexity: O(log n) average (recursion stack)
    Stable: No

    Generally fastest comparison sort in practice.
    Uses randomized pivot to avoid worst case.

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    _quick_sort_helper(result, 0, len(result) - 1)
    return result


def _quick_sort_helper(arr: List[T], low: int, high: int) -> None:
    """Helper function for in-place quick sort."""
    if low < high:
        pivot_idx = _partition(arr, low, high)
        _quick_sort_helper(arr, low, pivot_idx - 1)
        _quick_sort_helper(arr, pivot_idx + 1, high)


def _partition(arr: List[T], low: int, high: int) -> int:
    """Partition array around a random pivot (Lomuto scheme)."""
    # Randomize pivot to avoid worst case
    pivot_idx = random.randint(low, high)
    arr[pivot_idx], arr[high] = arr[high], arr[pivot_idx]

    pivot = arr[high]
    i = low - 1

    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]

    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1


def heap_sort(arr: List[T]) -> List[T]:
    """
    Heap Sort - build max-heap and extract elements.

    Time Complexity: O(n log n) all cases
    Space Complexity: O(1)
    Stable: No

    Guaranteed O(n log n) but typically slower than quicksort due to cache.

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    n = len(result)

    # Build max-heap
    for i in range(n // 2 - 1, -1, -1):
        _heapify(result, n, i)

    # Extract elements
    for i in range(n - 1, 0, -1):
        result[0], result[i] = result[i], result[0]
        _heapify(result, i, 0)

    return result


def _heapify(arr: List[T], n: int, i: int) -> None:
    """Maintain max-heap property."""
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n and arr[left] > arr[largest]:
        largest = left

    if right < n and arr[right] > arr[largest]:
        largest = right

    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        _heapify(arr, n, largest)


def shell_sort(arr: List[T]) -> List[T]:
    """
    Shell Sort - generalization of insertion sort with gap sequence.

    Time Complexity: O(n²) worst, O(n log n) to O(n^1.3) average
    Space Complexity: O(1)
    Stable: No

    Uses Knuth's gap sequence: 1, 4, 13, 40, 121, ...

    Args:
        arr: List to sort.

    Returns:
        New sorted list.
    """
    result = arr.copy()
    n = len(result)

    # Generate Knuth's gap sequence
    gap = 1
    while gap < n // 3:
        gap = 3 * gap + 1

    while gap > 0:
        for i in range(gap, n):
            temp = result[i]
            j = i

            while j >= gap and result[j - gap] > temp:
                result[j] = result[j - gap]
                j -= gap

            result[j] = temp

        gap //= 3

    return result


# =============================================================================
# NON-COMPARISON SORTING ALGORITHMS
# =============================================================================


def counting_sort(arr: List[int]) -> List[int]:
    """
    Counting Sort - count occurrences, then reconstruct.

    Time Complexity: O(n + k) where k is range of values
    Space Complexity: O(k)
    Stable: Yes

    Only works for integers with known, reasonable range.

    Args:
        arr: List of integers to sort.

    Returns:
        New sorted list.
    """
    if not arr:
        return []

    min_val = min(arr)
    max_val = max(arr)
    range_size = max_val - min_val + 1

    # Count occurrences
    count = [0] * range_size
    for num in arr:
        count[num - min_val] += 1

    # Compute cumulative counts
    for i in range(1, range_size):
        count[i] += count[i - 1]

    # Build output array (iterate backwards for stability)
    result = [0] * len(arr)
    for num in reversed(arr):
        idx = count[num - min_val] - 1
        result[idx] = num
        count[num - min_val] -= 1

    return result


def radix_sort(arr: List[int]) -> List[int]:
    """
    Radix Sort - sort digit by digit using counting sort.

    Time Complexity: O(d * (n + k)) where d is digits, k is base
    Space Complexity: O(n + k)
    Stable: Yes

    Works for non-negative integers.

    Args:
        arr: List of non-negative integers to sort.

    Returns:
        New sorted list.
    """
    if not arr:
        return []

    # Handle negative numbers by separating them
    negatives = [-x for x in arr if x < 0]
    non_negatives = [x for x in arr if x >= 0]

    def radix_sort_positive(nums: List[int]) -> List[int]:
        if not nums:
            return []

        max_val = max(nums)
        result = nums.copy()
        exp = 1

        while max_val // exp > 0:
            result = _counting_sort_by_digit(result, exp)
            exp *= 10

        return result

    sorted_negatives = [-x for x in reversed(radix_sort_positive(negatives))]
    sorted_non_negatives = radix_sort_positive(non_negatives)

    return sorted_negatives + sorted_non_negatives


def _counting_sort_by_digit(arr: List[int], exp: int) -> List[int]:
    """Counting sort by a specific digit."""
    n = len(arr)
    result = [0] * n
    count = [0] * 10

    for num in arr:
        digit = (num // exp) % 10
        count[digit] += 1

    for i in range(1, 10):
        count[i] += count[i - 1]

    for num in reversed(arr):
        digit = (num // exp) % 10
        idx = count[digit] - 1
        result[idx] = num
        count[digit] -= 1

    return result


def bucket_sort(arr: List[float], num_buckets: int = 10) -> List[float]:
    """
    Bucket Sort - distribute into buckets, sort each, concatenate.

    Time Complexity: O(n + k) average, O(n²) worst
    Space Complexity: O(n)
    Stable: Yes (if using stable sort for buckets)

    Best for uniformly distributed floating-point numbers in [0, 1).

    Args:
        arr: List of floats in [0, 1) to sort.
        num_buckets: Number of buckets.

    Returns:
        New sorted list.
    """
    if not arr:
        return []

    # Create buckets
    buckets: List[List[float]] = [[] for _ in range(num_buckets)]

    # Find min and max for scaling
    min_val = min(arr)
    max_val = max(arr)
    range_val = max_val - min_val

    if range_val == 0:
        return arr.copy()

    # Distribute elements into buckets
    for num in arr:
        # Scale to [0, num_buckets)
        idx = int((num - min_val) / range_val * (num_buckets - 1))
        buckets[idx].append(num)

    # Sort each bucket using insertion sort
    result: List[float] = []
    for bucket in buckets:
        # Use insertion sort for small buckets
        bucket.sort()
        result.extend(bucket)

    return result


# =============================================================================
# QUICK SELECT (Related algorithm)
# =============================================================================


def quick_select(arr: List[T], k: int) -> T:
    """
    Quick Select - find kth smallest element.

    Time Complexity: O(n) average, O(n²) worst
    Space Complexity: O(1)

    Uses randomized partitioning.

    Args:
        arr: List to search.
        k: Position (1-indexed, k=1 means smallest).

    Returns:
        The kth smallest element.

    Raises:
        ValueError: If k is out of range.
    """
    if k < 1 or k > len(arr):
        raise ValueError(f"k={k} out of range for array of size {len(arr)}")

    result = arr.copy()
    return _quick_select_helper(result, 0, len(result) - 1, k - 1)


def _quick_select_helper(arr: List[T], low: int, high: int, k: int) -> T:
    """Helper for quick select."""
    if low == high:
        return arr[low]

    pivot_idx = _partition(arr, low, high)

    if k == pivot_idx:
        return arr[k]
    elif k < pivot_idx:
        return _quick_select_helper(arr, low, pivot_idx - 1, k)
    else:
        return _quick_select_helper(arr, pivot_idx + 1, high, k)


# =============================================================================
# SORTING WITH CUSTOM COMPARATOR
# =============================================================================


def sort_with_comparator(
    arr: List[T],
    comparator: Callable[[T, T], int]
) -> List[T]:
    """
    Sort using a custom comparator function.

    Uses merge sort for stability.

    Args:
        arr: List to sort.
        comparator: Function that returns:
                   - negative if a < b
                   - zero if a == b
                   - positive if a > b

    Returns:
        New sorted list.
    """
    if len(arr) <= 1:
        return arr.copy()

    mid = len(arr) // 2
    left = sort_with_comparator(arr[:mid], comparator)
    right = sort_with_comparator(arr[mid:], comparator)

    return _merge_with_comparator(left, right, comparator)


def _merge_with_comparator(
    left: List[T],
    right: List[T],
    comparator: Callable[[T, T], int]
) -> List[T]:
    """Merge with custom comparator."""
    result: List[T] = []
    i = j = 0

    while i < len(left) and j < len(right):
        if comparator(left[i], right[j]) <= 0:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    result.extend(left[i:])
    result.extend(right[j:])

    return result


# =============================================================================
# UTILITY FUNCTIONS
# =============================================================================


def is_sorted(arr: List[T], reverse: bool = False) -> bool:
    """
    Check if array is sorted.

    Args:
        arr: List to check.
        reverse: If True, check for descending order.

    Returns:
        True if sorted.
    """
    if len(arr) <= 1:
        return True

    if reverse:
        return all(arr[i] >= arr[i + 1] for i in range(len(arr) - 1))
    else:
        return all(arr[i] <= arr[i + 1] for i in range(len(arr) - 1))
