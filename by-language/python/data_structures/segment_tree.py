"""
Segment Tree Implementation.

A data structure for efficient range queries and point/range updates.

================================================================================
                      SEGMENT TREE VISUAL REPRESENTATION
================================================================================

    Array: [1, 3, 5, 7, 9, 11]

    Segment Tree (sum):
                        [36]                    <- sum of [0,5]
                       /    \
                    [9]      [27]               <- sum of [0,2], [3,5]
                   /   \     /   \
                 [4]   [5] [16]  [11]           <- sum of [0,1],[2,2],[3,4],[5,5]
                /   \      /   \
              [1]   [3]  [7]   [9]              <- leaf nodes

    Range Query [1,4]:
    - Need nodes covering indices 1,2,3,4
    - Result = arr[1] + arr[2] + arr[3] + arr[4] = 3+5+7+9 = 24

================================================================================

COMPLEXITY:
+-------------------+---------------+
| Operation         | Time          |
+-------------------+---------------+
| Build             | O(n)          |
| Point Update      | O(log n)      |
| Range Query       | O(log n)      |
| Range Update*     | O(log n)      |
| Space             | O(n)          |
+-------------------+---------------+
* With lazy propagation

LEETCODE PROBLEMS:
- #307 Range Sum Query - Mutable
- #315 Count of Smaller Numbers After Self
- #327 Count of Range Sum
- #493 Reverse Pairs
- #1649 Create Sorted Array through Instructions

USE CASES:
- Range sum/min/max queries
- Range updates with lazy propagation
- Counting inversions
- Interval scheduling
"""

from typing import List, Callable


class SegmentTree:
    """
    Segment Tree for range queries and point updates.

    Supports any associative operation (sum, min, max, gcd, etc.)

    Example:
        >>> st = SegmentTree([1, 3, 5, 7, 9, 11])
        >>> st.query(1, 4)  # Sum of indices 1-4
        24
        >>> st.update(2, 10)  # Set index 2 to 10
        >>> st.query(1, 4)
        29
    """

    def __init__(self, arr: List[int],
                 operation: Callable[[int, int], int] = lambda a, b: a + b,
                 identity: int = 0) -> None:
        """
        Initialize segment tree.

        Args:
            arr: Input array
            operation: Associative operation (default: sum)
            identity: Identity element for operation (default: 0 for sum)
        """
        self._n = len(arr)
        self._op = operation
        self._identity = identity
        self._tree = [identity] * (4 * self._n) if self._n > 0 else []

        if self._n > 0:
            self._build(arr, 0, 0, self._n - 1)

    def _build(self, arr: List[int], node: int, start: int, end: int) -> None:
        """Build segment tree recursively."""
        if start == end:
            self._tree[node] = arr[start]
        else:
            mid = (start + end) // 2
            left_child = 2 * node + 1
            right_child = 2 * node + 2

            self._build(arr, left_child, start, mid)
            self._build(arr, right_child, mid + 1, end)

            self._tree[node] = self._op(self._tree[left_child], self._tree[right_child])

    def update(self, idx: int, value: int) -> None:
        """
        Update value at index.

        Args:
            idx: Index to update
            value: New value

        Time: O(log n)
        """
        self._update(0, 0, self._n - 1, idx, value)

    def _update(self, node: int, start: int, end: int, idx: int, value: int) -> None:
        """Recursive update helper."""
        if start == end:
            self._tree[node] = value
        else:
            mid = (start + end) // 2
            left_child = 2 * node + 1
            right_child = 2 * node + 2

            if idx <= mid:
                self._update(left_child, start, mid, idx, value)
            else:
                self._update(right_child, mid + 1, end, idx, value)

            self._tree[node] = self._op(self._tree[left_child], self._tree[right_child])

    def query(self, left: int, right: int) -> int:
        """
        Query range [left, right].

        Args:
            left: Left boundary (inclusive)
            right: Right boundary (inclusive)

        Returns:
            Result of operation over range

        Time: O(log n)
        """
        return self._query(0, 0, self._n - 1, left, right)

    def _query(self, node: int, start: int, end: int, left: int, right: int) -> int:
        """Recursive query helper."""
        # No overlap
        if right < start or left > end:
            return self._identity

        # Complete overlap
        if left <= start and end <= right:
            return self._tree[node]

        # Partial overlap
        mid = (start + end) // 2
        left_result = self._query(2 * node + 1, start, mid, left, right)
        right_result = self._query(2 * node + 2, mid + 1, end, left, right)

        return self._op(left_result, right_result)


class SegmentTreeLazy:
    """
    Segment Tree with Lazy Propagation for range updates.

    Supports both point and range updates efficiently.

    Example:
        >>> st = SegmentTreeLazy([1, 3, 5, 7, 9, 11])
        >>> st.range_update(1, 4, 10)  # Add 10 to indices 1-4
        >>> st.query(1, 4)
        64
    """

    def __init__(self, arr: List[int]) -> None:
        """Initialize segment tree with lazy propagation."""
        self._n = len(arr)
        self._tree = [0] * (4 * self._n) if self._n > 0 else []
        self._lazy = [0] * (4 * self._n) if self._n > 0 else []

        if self._n > 0:
            self._build(arr, 0, 0, self._n - 1)

    def _build(self, arr: List[int], node: int, start: int, end: int) -> None:
        """Build segment tree."""
        if start == end:
            self._tree[node] = arr[start]
        else:
            mid = (start + end) // 2
            self._build(arr, 2 * node + 1, start, mid)
            self._build(arr, 2 * node + 2, mid + 1, end)
            self._tree[node] = self._tree[2 * node + 1] + self._tree[2 * node + 2]

    def _push_down(self, node: int, start: int, end: int) -> None:
        """Push lazy value to children."""
        if self._lazy[node] != 0:
            mid = (start + end) // 2
            left_child = 2 * node + 1
            right_child = 2 * node + 2

            # Update children
            self._tree[left_child] += self._lazy[node] * (mid - start + 1)
            self._tree[right_child] += self._lazy[node] * (end - mid)

            # Propagate lazy to children
            self._lazy[left_child] += self._lazy[node]
            self._lazy[right_child] += self._lazy[node]

            self._lazy[node] = 0

    def range_update(self, left: int, right: int, value: int) -> None:
        """
        Add value to all elements in range [left, right].

        Time: O(log n)
        """
        self._range_update(0, 0, self._n - 1, left, right, value)

    def _range_update(self, node: int, start: int, end: int,
                      left: int, right: int, value: int) -> None:
        """Recursive range update with lazy propagation."""
        # No overlap
        if right < start or left > end:
            return

        # Complete overlap
        if left <= start and end <= right:
            self._tree[node] += value * (end - start + 1)
            self._lazy[node] += value
            return

        # Partial overlap - push down lazy first
        self._push_down(node, start, end)

        mid = (start + end) // 2
        self._range_update(2 * node + 1, start, mid, left, right, value)
        self._range_update(2 * node + 2, mid + 1, end, left, right, value)

        self._tree[node] = self._tree[2 * node + 1] + self._tree[2 * node + 2]

    def query(self, left: int, right: int) -> int:
        """
        Query sum of range [left, right].

        Time: O(log n)
        """
        return self._query(0, 0, self._n - 1, left, right)

    def _query(self, node: int, start: int, end: int, left: int, right: int) -> int:
        """Recursive query with lazy propagation."""
        if right < start or left > end:
            return 0

        if left <= start and end <= right:
            return self._tree[node]

        self._push_down(node, start, end)

        mid = (start + end) // 2
        return (self._query(2 * node + 1, start, mid, left, right) +
                self._query(2 * node + 2, mid + 1, end, left, right))


# For backwards compatibility, also export Fenwick Tree classes
# (Users who import from segment_tree will still have access)
from data_structures.fenwick_tree import FenwickTree, FenwickTree2D, count_inversions, range_sum_query_mutable

__all__ = [
    'SegmentTree',
    'SegmentTreeLazy',
    # Re-exported for backwards compatibility
    'FenwickTree',
    'FenwickTree2D',
    'count_inversions',
    'range_sum_query_mutable',
]
