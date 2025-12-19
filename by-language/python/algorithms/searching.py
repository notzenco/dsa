"""
Searching Algorithms Implementation.

This module contains various searching algorithms from basic to advanced,
including binary search variants, two-pointer techniques, and sliding window.

================================================================================
                        BINARY SEARCH VISUAL EXAMPLE
================================================================================

    Array: [1, 3, 5, 7, 9, 11, 13, 15]
    Target: 7

    Step 1:  [1, 3, 5, 7, 9, 11, 13, 15]
                       ^
              L=0     mid=3             R=7
              arr[3]=7 == 7  FOUND!

    Target: 5

    Step 1:  [1, 3, 5, 7, 9, 11, 13, 15]
                       ^
              L=0     mid=3             R=7
              arr[3]=7 > 5, search left

    Step 2:  [1, 3, 5, 7]
                 ^
              L=0 mid=1   R=2
              arr[1]=3 < 5, search right

    Step 3:  [5, 7]
              ^
              L=2=mid  R=2
              arr[2]=5 == 5  FOUND!

================================================================================

COMPLEXITY:
+----------------------+-------------+-------+
| Algorithm            | Time        | Space |
+----------------------+-------------+-------+
| Linear Search        | O(n)        | O(1)  |
| Binary Search        | O(log n)    | O(1)  |
| Jump Search          | O(sqrt(n))  | O(1)  |
| Interpolation Search | O(log log n)| O(1)  | (average, uniform data)
| Exponential Search   | O(log n)    | O(1)  |
| Ternary Search       | O(log n)    | O(1)  |
+----------------------+-------------+-------+

LEETCODE PROBLEMS:
- #704 Binary Search
- #35 Search Insert Position
- #34 Find First and Last Position
- #33 Search in Rotated Sorted Array
- #153 Find Minimum in Rotated Sorted Array
- #162 Find Peak Element
- #278 First Bad Version
- #69 Sqrt(x)
- #167 Two Sum II (Two Pointers)
- #3 Longest Substring Without Repeating (Sliding Window)
"""

from typing import List, Optional, Callable, TypeVar, Tuple, Any
import math

T = TypeVar('T')


# =============================================================================
# Basic Search Algorithms
# =============================================================================

def linear_search(arr: List[T], target: T) -> int:
    """
    Linear search - iterate through all elements.

    Args:
        arr: List to search in
        target: Element to find

    Returns:
        Index of target if found, -1 otherwise

    Time: O(n)
    Space: O(1)
    """
    for i, val in enumerate(arr):
        if val == target:
            return i
    return -1


def binary_search(arr: List[T], target: T) -> int:
    """
    Classic binary search on a sorted array.

    Args:
        arr: Sorted list to search in
        target: Element to find

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log n)
    Space: O(1)
    """
    left, right = 0, len(arr) - 1

    while left <= right:
        mid = left + (right - left) // 2

        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1


def binary_search_recursive(arr: List[T], target: T,
                            left: int = 0, right: int = -1) -> int:
    """
    Recursive binary search.

    Args:
        arr: Sorted list to search in
        target: Element to find
        left: Left boundary (inclusive)
        right: Right boundary (inclusive)

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log n)
    Space: O(log n) for recursion stack
    """
    if right == -1:
        right = len(arr) - 1

    if left > right:
        return -1

    mid = left + (right - left) // 2

    if arr[mid] == target:
        return mid
    elif arr[mid] < target:
        return binary_search_recursive(arr, target, mid + 1, right)
    else:
        return binary_search_recursive(arr, target, left, mid - 1)


# =============================================================================
# Binary Search Variants
# =============================================================================

def lower_bound(arr: List[T], target: T) -> int:
    """
    Find the leftmost position where target can be inserted (first >= target).

    Also known as bisect_left. Returns index of first element >= target,
    or len(arr) if all elements are < target.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Leftmost insertion point

    Time: O(log n)
    """
    left, right = 0, len(arr)

    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] < target:
            left = mid + 1
        else:
            right = mid

    return left


def upper_bound(arr: List[T], target: T) -> int:
    """
    Find the rightmost position where target can be inserted (first > target).

    Also known as bisect_right. Returns index of first element > target,
    or len(arr) if all elements are <= target.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Rightmost insertion point

    Time: O(log n)
    """
    left, right = 0, len(arr)

    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] <= target:
            left = mid + 1
        else:
            right = mid

    return left


def find_first_occurrence(arr: List[T], target: T) -> int:
    """
    Find the first occurrence of target in sorted array.

    Args:
        arr: Sorted list (may have duplicates)
        target: Target value

    Returns:
        Index of first occurrence, or -1 if not found

    Time: O(log n)
    """
    idx = lower_bound(arr, target)
    if idx < len(arr) and arr[idx] == target:
        return idx
    return -1


def find_last_occurrence(arr: List[T], target: T) -> int:
    """
    Find the last occurrence of target in sorted array.

    Args:
        arr: Sorted list (may have duplicates)
        target: Target value

    Returns:
        Index of last occurrence, or -1 if not found

    Time: O(log n)
    """
    idx = upper_bound(arr, target) - 1
    if idx >= 0 and arr[idx] == target:
        return idx
    return -1


def count_occurrences(arr: List[T], target: T) -> int:
    """
    Count occurrences of target in sorted array.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Number of occurrences

    Time: O(log n)
    """
    return upper_bound(arr, target) - lower_bound(arr, target)


def find_range(arr: List[T], target: T) -> Tuple[int, int]:
    """
    Find the starting and ending position of target.

    LeetCode #34

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Tuple (first_index, last_index), or (-1, -1) if not found

    Time: O(log n)
    """
    first = find_first_occurrence(arr, target)
    if first == -1:
        return (-1, -1)
    last = find_last_occurrence(arr, target)
    return (first, last)


def search_insert_position(arr: List[T], target: T) -> int:
    """
    Find index where target should be inserted.

    LeetCode #35

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Index where target is or should be inserted

    Time: O(log n)
    """
    return lower_bound(arr, target)


# =============================================================================
# Specialized Search Algorithms
# =============================================================================

def jump_search(arr: List[T], target: T) -> int:
    """
    Jump search - skip ahead by sqrt(n) steps, then linear search.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Index of target if found, -1 otherwise

    Time: O(sqrt(n))
    Space: O(1)
    """
    n = len(arr)
    if n == 0:
        return -1

    step = int(math.sqrt(n))
    prev = 0

    # Jump ahead
    while arr[min(step, n) - 1] < target:
        prev = step
        step += int(math.sqrt(n))
        if prev >= n:
            return -1

    # Linear search in block
    for i in range(prev, min(step, n)):
        if arr[i] == target:
            return i

    return -1


def interpolation_search(arr: List[int], target: int) -> int:
    """
    Interpolation search - estimate position based on value distribution.

    Best for uniformly distributed data. For non-uniform data,
    can degrade to O(n).

    Args:
        arr: Sorted list of integers
        target: Target value

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log log n) average for uniform data, O(n) worst case
    Space: O(1)
    """
    low, high = 0, len(arr) - 1

    while low <= high and arr[low] <= target <= arr[high]:
        if low == high:
            return low if arr[low] == target else -1

        # Estimate position using linear interpolation
        pos = low + ((target - arr[low]) * (high - low) //
                     (arr[high] - arr[low]))

        if arr[pos] == target:
            return pos
        elif arr[pos] < target:
            low = pos + 1
        else:
            high = pos - 1

    return -1


def exponential_search(arr: List[T], target: T) -> int:
    """
    Exponential search - find range then binary search.

    Useful when target is near the beginning of a large array.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log n)
    Space: O(1)
    """
    n = len(arr)
    if n == 0:
        return -1

    if arr[0] == target:
        return 0

    # Find range for binary search
    i = 1
    while i < n and arr[i] <= target:
        i *= 2

    # Binary search in range
    left = i // 2
    right = min(i, n - 1)

    while left <= right:
        mid = left + (right - left) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1


def ternary_search(arr: List[T], target: T) -> int:
    """
    Ternary search - divide into three parts.

    Similar to binary search but divides into three parts.
    Actually slightly slower than binary search due to more comparisons.

    Args:
        arr: Sorted list
        target: Target value

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log n)
    Space: O(1)
    """
    left, right = 0, len(arr) - 1

    while left <= right:
        mid1 = left + (right - left) // 3
        mid2 = right - (right - left) // 3

        if arr[mid1] == target:
            return mid1
        if arr[mid2] == target:
            return mid2

        if target < arr[mid1]:
            right = mid1 - 1
        elif target > arr[mid2]:
            left = mid2 + 1
        else:
            left = mid1 + 1
            right = mid2 - 1

    return -1


# =============================================================================
# Special Binary Search Problems
# =============================================================================

def search_rotated_array(arr: List[int], target: int) -> int:
    """
    Search in rotated sorted array.

    LeetCode #33

    Args:
        arr: Rotated sorted array (no duplicates)
        target: Target value

    Returns:
        Index of target if found, -1 otherwise

    Time: O(log n)
    """
    left, right = 0, len(arr) - 1

    while left <= right:
        mid = left + (right - left) // 2

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


def find_rotation_point(arr: List[int]) -> int:
    """
    Find the minimum element in rotated sorted array.

    LeetCode #153

    Args:
        arr: Rotated sorted array (no duplicates)

    Returns:
        Index of minimum element

    Time: O(log n)
    """
    left, right = 0, len(arr) - 1

    # Array is not rotated
    if arr[left] <= arr[right]:
        return 0

    while left < right:
        mid = left + (right - left) // 2

        if arr[mid] > arr[right]:
            left = mid + 1
        else:
            right = mid

    return left


def find_peak_element(arr: List[int]) -> int:
    """
    Find any peak element (element greater than its neighbors).

    LeetCode #162

    Args:
        arr: List of integers

    Returns:
        Index of a peak element

    Time: O(log n)
    """
    left, right = 0, len(arr) - 1

    while left < right:
        mid = left + (right - left) // 2

        if arr[mid] > arr[mid + 1]:
            right = mid
        else:
            left = mid + 1

    return left


def sqrt_int(x: int) -> int:
    """
    Integer square root using binary search.

    LeetCode #69

    Args:
        x: Non-negative integer

    Returns:
        Floor of square root of x

    Time: O(log x)
    """
    if x < 2:
        return x

    left, right = 1, x // 2

    while left <= right:
        mid = left + (right - left) // 2
        square = mid * mid

        if square == x:
            return mid
        elif square < x:
            left = mid + 1
        else:
            right = mid - 1

    return right


def first_bad_version(n: int, is_bad: Callable[[int], bool]) -> int:
    """
    Find the first bad version using binary search.

    LeetCode #278

    Args:
        n: Total number of versions (1 to n)
        is_bad: Function that returns True if version is bad

    Returns:
        First bad version number

    Time: O(log n)
    """
    left, right = 1, n

    while left < right:
        mid = left + (right - left) // 2

        if is_bad(mid):
            right = mid
        else:
            left = mid + 1

    return left


# =============================================================================
# Two Pointers Technique
# =============================================================================

def two_sum_sorted(arr: List[int], target: int) -> Optional[Tuple[int, int]]:
    """
    Find two numbers that add up to target in sorted array.

    LeetCode #167

    Args:
        arr: Sorted list of integers
        target: Target sum

    Returns:
        Tuple of indices, or None if not found

    Time: O(n)
    Space: O(1)
    """
    left, right = 0, len(arr) - 1

    while left < right:
        current_sum = arr[left] + arr[right]

        if current_sum == target:
            return (left, right)
        elif current_sum < target:
            left += 1
        else:
            right -= 1

    return None


def three_sum(arr: List[int]) -> List[List[int]]:
    """
    Find all unique triplets that sum to zero.

    LeetCode #15

    Args:
        arr: List of integers

    Returns:
        List of triplets

    Time: O(n^2)
    Space: O(1) excluding output
    """
    arr = sorted(arr)
    n = len(arr)
    result = []

    for i in range(n - 2):
        # Skip duplicates
        if i > 0 and arr[i] == arr[i - 1]:
            continue

        left, right = i + 1, n - 1
        target = -arr[i]

        while left < right:
            current_sum = arr[left] + arr[right]

            if current_sum == target:
                result.append([arr[i], arr[left], arr[right]])

                # Skip duplicates
                while left < right and arr[left] == arr[left + 1]:
                    left += 1
                while left < right and arr[right] == arr[right - 1]:
                    right -= 1

                left += 1
                right -= 1
            elif current_sum < target:
                left += 1
            else:
                right -= 1

    return result


def container_with_most_water(heights: List[int]) -> int:
    """
    Find max water that can be contained between two lines.

    LeetCode #11

    Args:
        heights: List of line heights

    Returns:
        Maximum water volume

    Time: O(n)
    Space: O(1)
    """
    max_area = 0
    left, right = 0, len(heights) - 1

    while left < right:
        width = right - left
        height = min(heights[left], heights[right])
        area = width * height
        max_area = max(max_area, area)

        if heights[left] < heights[right]:
            left += 1
        else:
            right -= 1

    return max_area


def remove_duplicates_sorted(arr: List[T]) -> int:
    """
    Remove duplicates from sorted array in-place.

    LeetCode #26

    Args:
        arr: Sorted list (modified in-place)

    Returns:
        Length of array after removing duplicates

    Time: O(n)
    Space: O(1)
    """
    if not arr:
        return 0

    write_idx = 1

    for read_idx in range(1, len(arr)):
        if arr[read_idx] != arr[read_idx - 1]:
            arr[write_idx] = arr[read_idx]
            write_idx += 1

    return write_idx


# =============================================================================
# Sliding Window Technique
# =============================================================================

def max_sum_subarray(arr: List[int], k: int) -> Optional[int]:
    """
    Find maximum sum of any contiguous subarray of size k.

    Args:
        arr: List of integers
        k: Subarray size

    Returns:
        Maximum sum, or None if array too small

    Time: O(n)
    Space: O(1)
    """
    n = len(arr)
    if n < k:
        return None

    # Sum of first window
    window_sum = sum(arr[:k])
    max_sum = window_sum

    # Slide window
    for i in range(k, n):
        window_sum = window_sum + arr[i] - arr[i - k]
        max_sum = max(max_sum, window_sum)

    return max_sum


def longest_substring_without_repeating(s: str) -> int:
    """
    Find length of longest substring without repeating characters.

    LeetCode #3

    Args:
        s: Input string

    Returns:
        Length of longest substring without repeats

    Time: O(n)
    Space: O(min(n, alphabet_size))
    """
    char_index = {}
    max_length = 0
    left = 0

    for right, char in enumerate(s):
        if char in char_index and char_index[char] >= left:
            left = char_index[char] + 1

        char_index[char] = right
        max_length = max(max_length, right - left + 1)

    return max_length


def min_window_substring(s: str, t: str) -> str:
    """
    Find minimum window substring containing all characters of t.

    LeetCode #76

    Args:
        s: Source string
        t: Target string (characters to include)

    Returns:
        Minimum window substring, or empty string if not found

    Time: O(n + m)
    Space: O(k) where k is unique characters in t
    """
    if not s or not t:
        return ""

    from collections import Counter

    target_count = Counter(t)
    required = len(target_count)

    window_count: dict = {}
    formed = 0
    left = 0

    min_len = float('inf')
    min_left = 0

    for right, char in enumerate(s):
        window_count[char] = window_count.get(char, 0) + 1

        if char in target_count and window_count[char] == target_count[char]:
            formed += 1

        # Try to contract window
        while formed == required:
            if right - left + 1 < min_len:
                min_len = right - left + 1
                min_left = left

            left_char = s[left]
            window_count[left_char] -= 1

            if left_char in target_count and window_count[left_char] < target_count[left_char]:
                formed -= 1

            left += 1

    return "" if min_len == float('inf') else s[min_left:min_left + min_len]


def longest_substring_k_distinct(s: str, k: int) -> int:
    """
    Find length of longest substring with at most k distinct characters.

    LeetCode #340

    Args:
        s: Input string
        k: Maximum distinct characters

    Returns:
        Length of longest valid substring

    Time: O(n)
    Space: O(k)
    """
    if k == 0:
        return 0

    char_count: dict = {}
    max_length = 0
    left = 0

    for right, char in enumerate(s):
        char_count[char] = char_count.get(char, 0) + 1

        while len(char_count) > k:
            left_char = s[left]
            char_count[left_char] -= 1
            if char_count[left_char] == 0:
                del char_count[left_char]
            left += 1

        max_length = max(max_length, right - left + 1)

    return max_length


def find_all_anagrams(s: str, p: str) -> List[int]:
    """
    Find all start indices of p's anagrams in s.

    LeetCode #438

    Args:
        s: Source string
        p: Pattern string

    Returns:
        List of starting indices

    Time: O(n)
    Space: O(1) - fixed alphabet size
    """
    if len(p) > len(s):
        return []

    from collections import Counter

    p_count = Counter(p)
    window_count: dict = {}
    result = []

    for i, char in enumerate(s):
        window_count[char] = window_count.get(char, 0) + 1

        # Remove leftmost character when window exceeds size
        if i >= len(p):
            left_char = s[i - len(p)]
            window_count[left_char] -= 1
            if window_count[left_char] == 0:
                del window_count[left_char]

        if window_count == p_count:
            result.append(i - len(p) + 1)

    return result


# =============================================================================
# Binary Search on Answer
# =============================================================================

def koko_eating_bananas(piles: List[int], h: int) -> int:
    """
    Find minimum eating speed to finish all bananas in h hours.

    LeetCode #875

    Args:
        piles: List of banana pile sizes
        h: Hours available

    Returns:
        Minimum eating speed

    Time: O(n * log(max_pile))
    """
    def can_finish(speed: int) -> bool:
        hours = sum((pile + speed - 1) // speed for pile in piles)
        return hours <= h

    left, right = 1, max(piles)

    while left < right:
        mid = left + (right - left) // 2

        if can_finish(mid):
            right = mid
        else:
            left = mid + 1

    return left


def ship_within_days(weights: List[int], days: int) -> int:
    """
    Find minimum ship capacity to ship all packages within days.

    LeetCode #1011

    Args:
        weights: Package weights
        days: Days available

    Returns:
        Minimum ship capacity

    Time: O(n * log(sum_weights))
    """
    def can_ship(capacity: int) -> bool:
        day_count = 1
        current_load = 0

        for weight in weights:
            if current_load + weight > capacity:
                day_count += 1
                current_load = weight
            else:
                current_load += weight

        return day_count <= days

    left, right = max(weights), sum(weights)

    while left < right:
        mid = left + (right - left) // 2

        if can_ship(mid):
            right = mid
        else:
            left = mid + 1

    return left
