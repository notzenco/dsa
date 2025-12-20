"""
Fenwick Tree (Binary Indexed Tree) Implementation.

A data structure for efficient prefix sum queries and point updates.
More space-efficient than segment trees but less flexible.

================================================================================
                      FENWICK TREE VISUAL REPRESENTATION
================================================================================

    Array index:      1    2    3    4    5    6    7    8
    Array values:    [3]  [2]  [-1] [6]  [5]  [4]  [-3] [3]

    BIT array:       [3]  [5]  [-1] [10] [5]  [9]  [-3] [19]
                      |    |    |    |    |    |    |    |
                      1    2    3    4    5    6    7    8

    Each BIT[i] stores sum of range based on lowest set bit of i:
    - BIT[1] = arr[1]                    (1 = 0001, covers 1 element)
    - BIT[2] = arr[1] + arr[2]           (2 = 0010, covers 2 elements)
    - BIT[4] = arr[1..4]                 (4 = 0100, covers 4 elements)
    - BIT[8] = arr[1..8]                 (8 = 1000, covers 8 elements)

    Key insight: i & (-i) gives the lowest set bit of i

================================================================================

COMPLEXITY:
+-------------------+---------------+
| Operation         | Time          |
+-------------------+---------------+
| Build             | O(n log n)    |
| Point Update      | O(log n)      |
| Prefix Sum        | O(log n)      |
| Range Sum         | O(log n)      |
| Space             | O(n)          |
+-------------------+---------------+

LEETCODE PROBLEMS:
- #307 Range Sum Query - Mutable
- #315 Count of Smaller Numbers After Self
- #327 Count of Range Sum
- #493 Reverse Pairs

USE CASES:
- Prefix sum queries
- Counting inversions
- Range frequency queries
- 2D range sum queries
"""

from typing import List


class FenwickTree:
    """
    Fenwick Tree (Binary Indexed Tree) for prefix sums.

    More space-efficient than segment tree but less flexible.

    Example:
        >>> ft = FenwickTree([3, 2, -1, 6, 5, 4, -3, 3])
        >>> ft.prefix_sum(5)  # Sum of indices 0-4
        15
        >>> ft.range_sum(2, 5)  # Sum of indices 2-5
        14
        >>> ft.update(3, 4)  # Add 4 to index 3
    """

    def __init__(self, arr: List[int]) -> None:
        """
        Initialize Fenwick tree.

        Args:
            arr: Input array (0-indexed)

        Time: O(n log n)
        """
        self._n = len(arr)
        self._tree = [0] * (self._n + 1)  # 1-indexed

        for i, val in enumerate(arr):
            self._add(i + 1, val)

    def _add(self, i: int, delta: int) -> None:
        """Add delta to index i (1-indexed internal)."""
        while i <= self._n:
            self._tree[i] += delta
            i += i & (-i)  # Add lowest set bit

    def update(self, idx: int, delta: int) -> None:
        """
        Add delta to element at index.

        Args:
            idx: Index (0-indexed)
            delta: Value to add

        Time: O(log n)
        """
        self._add(idx + 1, delta)

    def set(self, idx: int, value: int) -> None:
        """
        Set element at index to value.

        Args:
            idx: Index (0-indexed)
            value: New value

        Time: O(log n)
        """
        current = self.range_sum(idx, idx)
        self.update(idx, value - current)

    def prefix_sum(self, idx: int) -> int:
        """
        Get sum of elements from 0 to idx (inclusive).

        Args:
            idx: End index (0-indexed)

        Returns:
            Sum of arr[0..idx]

        Time: O(log n)
        """
        i = idx + 1  # Convert to 1-indexed
        total = 0
        while i > 0:
            total += self._tree[i]
            i -= i & (-i)  # Remove lowest set bit
        return total

    def range_sum(self, left: int, right: int) -> int:
        """
        Get sum of elements from left to right (inclusive).

        Args:
            left: Start index (0-indexed)
            right: End index (0-indexed)

        Returns:
            Sum of arr[left..right]

        Time: O(log n)
        """
        right_sum = self.prefix_sum(right)
        left_sum = self.prefix_sum(left - 1) if left > 0 else 0
        return right_sum - left_sum

    def __len__(self) -> int:
        """Return size of underlying array."""
        return self._n


class FenwickTree2D:
    """
    2D Fenwick Tree for 2D range sum queries.

    Example:
        >>> matrix = [[1, 2], [3, 4]]
        >>> ft = FenwickTree2D(matrix)
        >>> ft.query(0, 0, 1, 1)  # Sum of entire matrix
        10
    """

    def __init__(self, matrix: List[List[int]]) -> None:
        """Initialize 2D Fenwick tree."""
        if not matrix or not matrix[0]:
            self._rows = self._cols = 0
            self._tree: List[List[int]] = []
            return

        self._rows = len(matrix)
        self._cols = len(matrix[0])
        self._tree = [[0] * (self._cols + 1) for _ in range(self._rows + 1)]

        for i in range(self._rows):
            for j in range(self._cols):
                self._add(i + 1, j + 1, matrix[i][j])

    def _add(self, row: int, col: int, delta: int) -> None:
        """Add delta to position (row, col) - 1-indexed."""
        i = row
        while i <= self._rows:
            j = col
            while j <= self._cols:
                self._tree[i][j] += delta
                j += j & (-j)
            i += i & (-i)

    def update(self, row: int, col: int, delta: int) -> None:
        """Add delta to position (row, col) - 0-indexed."""
        self._add(row + 1, col + 1, delta)

    def _prefix_sum(self, row: int, col: int) -> int:
        """Get prefix sum up to (row, col) - 1-indexed."""
        total = 0
        i = row
        while i > 0:
            j = col
            while j > 0:
                total += self._tree[i][j]
                j -= j & (-j)
            i -= i & (-i)
        return total

    def query(self, r1: int, c1: int, r2: int, c2: int) -> int:
        """
        Get sum of submatrix from (r1,c1) to (r2,c2) - 0-indexed.

        Time: O(log(rows) * log(cols))
        """
        if self._rows == 0 or self._cols == 0:
            return 0

        r1 += 1
        c1 += 1
        r2 += 1
        c2 += 1

        return (self._prefix_sum(r2, c2)
                - self._prefix_sum(r1 - 1, c2)
                - self._prefix_sum(r2, c1 - 1)
                + self._prefix_sum(r1 - 1, c1 - 1))


# Utility functions using Fenwick Trees

def count_inversions(arr: List[int]) -> int:
    """
    Count inversions in array using Fenwick tree.

    An inversion is a pair (i, j) where i < j and arr[i] > arr[j].

    Time: O(n log n)
    """
    if not arr:
        return 0

    # Coordinate compression
    sorted_unique = sorted(set(arr))
    rank = {v: i + 1 for i, v in enumerate(sorted_unique)}

    n = len(sorted_unique)
    ft = FenwickTree([0] * n)
    inversions = 0

    for val in reversed(arr):
        r = rank[val]
        inversions += ft.prefix_sum(r - 2) if r > 1 else 0
        ft.update(r - 1, 1)

    return inversions


def range_sum_query_mutable(arr: List[int]) -> tuple:
    """
    Create mutable range sum query structure.

    Returns (update_fn, query_fn) tuple.

    LeetCode #307
    """
    ft = FenwickTree(arr)
    original = arr.copy()

    def update(idx: int, val: int) -> None:
        delta = val - original[idx]
        original[idx] = val
        ft.update(idx, delta)

    def query(left: int, right: int) -> int:
        return ft.range_sum(left, right)

    return (update, query)
