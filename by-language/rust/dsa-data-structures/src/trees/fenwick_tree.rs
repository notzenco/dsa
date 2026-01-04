//! Fenwick Tree (Binary Indexed Tree)
//!
//! A Fenwick tree is a data structure that efficiently supports prefix sum
//! queries and point updates in O(log n) time.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Original Array: [3, 2, -1, 6, 5, 4, -3, 3]                               ║
//! ║  Indices:         1  2   3  4  5  6   7  8  (1-indexed)                    ║
//! ║                                                                            ║
//! ║  Fenwick Tree Structure:                                                   ║
//! ║                                                                            ║
//! ║  Index:    1    2    3    4    5    6    7    8                           ║
//! ║  BIT:     [3]  [5]  [-1] [10] [5]  [9]  [-3] [19]                         ║
//! ║            │    │    │    │    │    │    │    │                           ║
//! ║            │    │    │    └────┴────┴────┴────┘ (sum of 1-8)              ║
//! ║            │    └────┘         └────┘           (sum of 1-2, 5-6)         ║
//! ║            │         │              │           (individual)              ║
//! ║                                                                            ║
//! ║  Responsibility ranges (each BIT[i] stores sum of specific range):        ║
//! ║  BIT[1] = arr[1]              (1 element)                                 ║
//! ║  BIT[2] = arr[1] + arr[2]     (2 elements)                                ║
//! ║  BIT[3] = arr[3]              (1 element)                                 ║
//! ║  BIT[4] = arr[1..4]           (4 elements)                                ║
//! ║  BIT[5] = arr[5]              (1 element)                                 ║
//! ║  BIT[6] = arr[5] + arr[6]     (2 elements)                                ║
//! ║  BIT[7] = arr[7]              (1 element)                                 ║
//! ║  BIT[8] = arr[1..8]           (8 elements)                                ║
//! ║                                                                            ║
//! ║  Key insight: i & (-i) gives the least significant bit                    ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation     | Time      | Space |
//! |---------------|-----------|-------|
//! | Build         | O(n)      | O(n)  |
//! | Point Update  | O(log n)  | O(1)  |
//! | Prefix Sum    | O(log n)  | O(1)  |
//! | Range Sum     | O(log n)  | O(1)  |
//!
//! ## LeetCode Problems
//!
//! - [#307 Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/)
//! - [#315 Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/)
//! - [#493 Reverse Pairs](https://leetcode.com/problems/reverse-pairs/)
//! - [#327 Count of Range Sum](https://leetcode.com/problems/count-of-range-sum/)
//!
//! ## Use Cases
//!
//! - Prefix sum queries with updates
//! - Counting inversions
//! - Range frequency queries
//! - Dynamic cumulative frequency tables
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::FenwickTree;
//!
//! let arr = vec![1, 2, 3, 4, 5];
//! let mut bit = FenwickTree::from_slice(&arr);
//!
//! // Prefix sum: sum of first 3 elements
//! assert_eq!(bit.prefix_sum(3), 6); // 1 + 2 + 3
//!
//! // Range sum: sum of elements from index 2 to 4
//! assert_eq!(bit.range_sum(2, 4), 9); // 2 + 3 + 4
//!
//! // Update: add 10 to element at index 2
//! bit.update(2, 10);
//! assert_eq!(bit.prefix_sum(3), 16); // 1 + 12 + 3
//! ```

use alloc::vec;
use alloc::vec::Vec;

use dsa_core::Container;

/// A Fenwick Tree (Binary Indexed Tree) for efficient prefix sum queries.
///
/// Uses 1-based indexing internally for cleaner bit manipulation.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct FenwickTree {
    tree: Vec<i64>,
    len: usize,
}

impl FenwickTree {
    /// Creates a new Fenwick tree of the specified size, initialized to zeros.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::FenwickTree;
    ///
    /// let bit = FenwickTree::new(5);
    /// assert_eq!(bit.len(), 5);
    /// assert_eq!(bit.prefix_sum(5), 0);
    /// ```
    #[must_use]
    pub fn new(size: usize) -> Self {
        FenwickTree {
            tree: vec![0; size + 1], // 1-indexed
            len: size,
        }
    }

    /// Creates a Fenwick tree from a slice of values.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::FenwickTree;
    ///
    /// let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
    /// assert_eq!(bit.prefix_sum(5), 15);
    /// ```
    #[must_use]
    pub fn from_slice(arr: &[i64]) -> Self {
        let n = arr.len();
        let mut tree = vec![0i64; n + 1];

        // Copy values to tree (1-indexed)
        for (i, &val) in arr.iter().enumerate() {
            tree[i + 1] = val;
        }

        // Build tree in O(n)
        for i in 1..=n {
            let parent = i + Self::lowbit(i);
            if parent <= n {
                tree[parent] += tree[i];
            }
        }

        FenwickTree { tree, len: n }
    }

    /// Returns the number of elements in the tree.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.len
    }

    /// Returns `true` if the tree is empty.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// Returns the lowest set bit of a number.
    #[inline]
    fn lowbit(x: usize) -> usize {
        x & x.wrapping_neg()
    }

    /// Adds a value to the element at the given index (1-indexed).
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::FenwickTree;
    ///
    /// let mut bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
    /// bit.update(3, 10); // Add 10 to element at index 3
    /// assert_eq!(bit.prefix_sum(3), 16); // 1 + 2 + 13
    /// ```
    pub fn update(&mut self, mut index: usize, delta: i64) {
        while index <= self.len {
            self.tree[index] += delta;
            index += Self::lowbit(index);
        }
    }

    /// Sets the element at the given index to a specific value.
    ///
    /// # Time Complexity
    /// O(log n)
    pub fn set(&mut self, index: usize, value: i64) {
        let current = self.get(index);
        self.update(index, value - current);
    }

    /// Gets the value at the given index.
    ///
    /// # Time Complexity
    /// O(log n)
    #[must_use]
    pub fn get(&self, index: usize) -> i64 {
        self.range_sum(index, index)
    }

    /// Returns the prefix sum from index 1 to the given index (inclusive).
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::FenwickTree;
    ///
    /// let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
    /// assert_eq!(bit.prefix_sum(1), 1);
    /// assert_eq!(bit.prefix_sum(3), 6);
    /// assert_eq!(bit.prefix_sum(5), 15);
    /// ```
    #[must_use]
    pub fn prefix_sum(&self, mut index: usize) -> i64 {
        let mut sum = 0;
        while index > 0 {
            sum += self.tree[index];
            index -= Self::lowbit(index);
        }
        sum
    }

    /// Returns the sum of elements in the range [left, right] (inclusive, 1-indexed).
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::FenwickTree;
    ///
    /// let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
    /// assert_eq!(bit.range_sum(2, 4), 9); // 2 + 3 + 4
    /// assert_eq!(bit.range_sum(1, 5), 15);
    /// ```
    #[must_use]
    pub fn range_sum(&self, left: usize, right: usize) -> i64 {
        if left > right || left == 0 {
            return 0;
        }
        self.prefix_sum(right) - self.prefix_sum(left - 1)
    }

    /// Finds the smallest index where prefix_sum(index) >= value.
    ///
    /// Useful for order statistics if values are frequencies.
    ///
    /// # Time Complexity
    /// O(log n)
    #[must_use]
    pub fn lower_bound(&self, mut value: i64) -> usize {
        if value <= 0 {
            return 0;
        }

        let mut pos = 0;
        let mut step = self.len.next_power_of_two();

        while step > 0 {
            if pos + step <= self.len && self.tree[pos + step] < value {
                pos += step;
                value -= self.tree[pos];
            }
            step /= 2;
        }

        pos + 1
    }
}

impl Container for FenwickTree {
    fn len(&self) -> usize {
        self.len
    }
}

impl Default for FenwickTree {
    fn default() -> Self {
        Self::new(0)
    }
}

/// A 2D Fenwick Tree for efficient 2D prefix sum queries.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct FenwickTree2D {
    tree: Vec<Vec<i64>>,
    rows: usize,
    cols: usize,
}

impl FenwickTree2D {
    /// Creates a new 2D Fenwick tree of the specified dimensions.
    ///
    /// # Time Complexity
    /// O(rows * cols)
    #[must_use]
    pub fn new(rows: usize, cols: usize) -> Self {
        FenwickTree2D {
            tree: vec![vec![0; cols + 1]; rows + 1],
            rows,
            cols,
        }
    }

    /// Creates a 2D Fenwick tree from a 2D array.
    ///
    /// # Time Complexity
    /// O(rows * cols * log(rows) * log(cols))
    #[must_use]
    pub fn from_matrix(matrix: &[Vec<i64>]) -> Self {
        if matrix.is_empty() {
            return Self::new(0, 0);
        }

        let rows = matrix.len();
        let cols = matrix[0].len();
        let mut bit = Self::new(rows, cols);

        for (i, row) in matrix.iter().enumerate() {
            for (j, &val) in row.iter().enumerate() {
                bit.update(i + 1, j + 1, val);
            }
        }

        bit
    }

    #[inline]
    fn lowbit(x: usize) -> usize {
        x & x.wrapping_neg()
    }

    /// Adds a value to the element at position (row, col) (1-indexed).
    ///
    /// # Time Complexity
    /// O(log(rows) * log(cols))
    pub fn update(&mut self, mut row: usize, col: usize, delta: i64) {
        while row <= self.rows {
            let mut c = col;
            while c <= self.cols {
                self.tree[row][c] += delta;
                c += Self::lowbit(c);
            }
            row += Self::lowbit(row);
        }
    }

    /// Returns the prefix sum from (1,1) to (row, col).
    ///
    /// # Time Complexity
    /// O(log(rows) * log(cols))
    #[must_use]
    pub fn prefix_sum(&self, mut row: usize, col: usize) -> i64 {
        let mut sum = 0;
        while row > 0 {
            let mut c = col;
            while c > 0 {
                sum += self.tree[row][c];
                c -= Self::lowbit(c);
            }
            row -= Self::lowbit(row);
        }
        sum
    }

    /// Returns the sum of elements in the rectangle from (r1, c1) to (r2, c2).
    ///
    /// # Time Complexity
    /// O(log(rows) * log(cols))
    #[must_use]
    pub fn range_sum(&self, r1: usize, c1: usize, r2: usize, c2: usize) -> i64 {
        self.prefix_sum(r2, c2) - self.prefix_sum(r1 - 1, c2) - self.prefix_sum(r2, c1 - 1)
            + self.prefix_sum(r1 - 1, c1 - 1)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let bit = FenwickTree::new(5);
            assert_eq!(bit.len(), 5);
            assert_eq!(bit.prefix_sum(5), 0);
        }

        #[test]
        fn test_from_slice() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.len(), 5);
            assert_eq!(bit.prefix_sum(5), 15);
        }

        #[test]
        fn test_default() {
            let bit = FenwickTree::default();
            assert!(bit.is_empty());
        }
    }

    mod prefix_sum {
        use super::*;

        #[test]
        fn test_prefix_sum() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.prefix_sum(1), 1);
            assert_eq!(bit.prefix_sum(2), 3);
            assert_eq!(bit.prefix_sum(3), 6);
            assert_eq!(bit.prefix_sum(4), 10);
            assert_eq!(bit.prefix_sum(5), 15);
        }

        #[test]
        fn test_prefix_sum_zero() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.prefix_sum(0), 0);
        }

        #[test]
        fn test_prefix_sum_with_negatives() {
            let bit = FenwickTree::from_slice(&[1, -2, 3, -4, 5]);
            assert_eq!(bit.prefix_sum(5), 3); // 1 - 2 + 3 - 4 + 5
        }
    }

    mod range_sum {
        use super::*;

        #[test]
        fn test_range_sum() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.range_sum(2, 4), 9); // 2 + 3 + 4
            assert_eq!(bit.range_sum(1, 5), 15);
            assert_eq!(bit.range_sum(3, 3), 3);
        }

        #[test]
        fn test_range_sum_single() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.range_sum(3, 3), 3);
        }

        #[test]
        fn test_range_sum_invalid() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.range_sum(4, 2), 0); // left > right
            assert_eq!(bit.range_sum(0, 3), 0); // left == 0
        }
    }

    mod update {
        use super::*;

        #[test]
        fn test_update() {
            let mut bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            bit.update(3, 10);
            assert_eq!(bit.prefix_sum(3), 16); // 1 + 2 + 13
            assert_eq!(bit.prefix_sum(5), 25);
        }

        #[test]
        fn test_update_negative() {
            let mut bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            bit.update(3, -5);
            assert_eq!(bit.prefix_sum(3), 1); // 1 + 2 + (-2)
        }

        #[test]
        fn test_set() {
            let mut bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            bit.set(3, 10);
            assert_eq!(bit.get(3), 10);
            assert_eq!(bit.prefix_sum(3), 13);
        }

        #[test]
        fn test_get() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.get(1), 1);
            assert_eq!(bit.get(3), 3);
            assert_eq!(bit.get(5), 5);
        }
    }

    mod lower_bound {
        use super::*;

        #[test]
        fn test_lower_bound() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            // prefix_sum: [1, 3, 6, 10, 15]
            assert_eq!(bit.lower_bound(1), 1); // prefix_sum(1) = 1 >= 1
            assert_eq!(bit.lower_bound(3), 2); // prefix_sum(2) = 3 >= 3
            assert_eq!(bit.lower_bound(4), 3); // prefix_sum(3) = 6 >= 4
            assert_eq!(bit.lower_bound(6), 3); // prefix_sum(3) = 6 >= 6
            assert_eq!(bit.lower_bound(15), 5); // prefix_sum(5) = 15 >= 15
        }

        #[test]
        fn test_lower_bound_zero() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.lower_bound(0), 0);
            assert_eq!(bit.lower_bound(-5), 0);
        }

        #[test]
        fn test_lower_bound_beyond() {
            let bit = FenwickTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(bit.lower_bound(16), 6); // Beyond all elements
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let bit = FenwickTree::from_slice(&[42]);
            assert_eq!(bit.prefix_sum(1), 42);
            assert_eq!(bit.range_sum(1, 1), 42);
        }

        #[test]
        fn test_all_zeros() {
            let bit = FenwickTree::from_slice(&[0, 0, 0, 0, 0]);
            assert_eq!(bit.prefix_sum(5), 0);
        }

        #[test]
        fn test_large_values() {
            let bit = FenwickTree::from_slice(&[1_000_000_000, 2_000_000_000]);
            assert_eq!(bit.prefix_sum(2), 3_000_000_000);
        }

        #[test]
        fn test_many_updates() {
            let mut bit = FenwickTree::new(1000);
            for i in 1..=1000 {
                bit.update(i, i as i64);
            }
            assert_eq!(bit.prefix_sum(1000), 500500); // Sum of 1 to 1000
        }
    }

    mod fenwick_2d {
        use super::*;

        #[test]
        fn test_new_2d() {
            let bit = FenwickTree2D::new(3, 3);
            assert_eq!(bit.prefix_sum(3, 3), 0);
        }

        #[test]
        fn test_from_matrix() {
            let matrix = vec![vec![1, 2, 3], vec![4, 5, 6], vec![7, 8, 9]];
            let bit = FenwickTree2D::from_matrix(&matrix);
            assert_eq!(bit.prefix_sum(3, 3), 45);
        }

        #[test]
        fn test_update_2d() {
            let mut bit = FenwickTree2D::new(3, 3);
            bit.update(2, 2, 5);
            assert_eq!(bit.prefix_sum(2, 2), 5);
            assert_eq!(bit.prefix_sum(3, 3), 5);
        }

        #[test]
        fn test_range_sum_2d() {
            let matrix = vec![vec![1, 2, 3], vec![4, 5, 6], vec![7, 8, 9]];
            let bit = FenwickTree2D::from_matrix(&matrix);
            // Sum of center element
            assert_eq!(bit.range_sum(2, 2, 2, 2), 5);
            // Sum of bottom-right 2x2
            assert_eq!(bit.range_sum(2, 2, 3, 3), 28); // 5 + 6 + 8 + 9
        }
    }
}
