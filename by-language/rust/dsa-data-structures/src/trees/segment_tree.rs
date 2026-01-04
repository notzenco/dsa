//! Segment Tree
//!
//! A segment tree is a tree data structure for storing intervals or segments,
//! allowing efficient range queries and point/range updates.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Array: [1, 3, 5, 7, 9, 11]                                                ║
//! ║                                                                            ║
//! ║  Segment Tree (Sum):                                                       ║
//! ║                                                                            ║
//! ║                         ┌─────────┐                                        ║
//! ║                         │   36    │  [0,5] sum of all                      ║
//! ║                         └────┬────┘                                        ║
//! ║                    ┌─────────┴─────────┐                                   ║
//! ║                    ▼                   ▼                                   ║
//! ║              ┌─────────┐         ┌─────────┐                               ║
//! ║              │    9    │         │   27    │  [0,2] and [3,5]              ║
//! ║              └────┬────┘         └────┬────┘                               ║
//! ║           ┌───────┴───────┐   ┌───────┴───────┐                            ║
//! ║           ▼               ▼   ▼               ▼                            ║
//! ║      ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐              ║
//! ║      │    4    │    │    5    │    │   16    │    │   11    │              ║
//! ║      └────┬────┘    └─────────┘    └────┬────┘    └─────────┘              ║
//! ║      ┌────┴────┐                   ┌────┴────┐                             ║
//! ║      ▼         ▼                   ▼         ▼                             ║
//! ║  ┌───────┐ ┌───────┐           ┌───────┐ ┌───────┐                         ║
//! ║  │   1   │ │   3   │           │   7   │ │   9   │                         ║
//! ║  └───────┘ └───────┘           └───────┘ └───────┘                         ║
//! ║                                                                            ║
//! ║  Query range [1,4]: traverse nodes covering indices 1-4                    ║
//! ║  Update index 2: update leaf and propagate up                              ║
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
//! | Range Query   | O(log n)  | O(1)  |
//! | Range Update  | O(log n)  | O(1)  |
//!
//! ## LeetCode Problems
//!
//! - [#307 Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/)
//! - [#308 Range Sum Query 2D - Mutable](https://leetcode.com/problems/range-sum-query-2d-mutable/)
//! - [#315 Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/)
//! - [#327 Count of Range Sum](https://leetcode.com/problems/count-of-range-sum/)
//! - [#699 Falling Squares](https://leetcode.com/problems/falling-squares/)
//!
//! ## Use Cases
//!
//! - Range minimum/maximum queries
//! - Range sum queries
//! - Range GCD/LCM queries
//! - Computational geometry
//! - Database query optimization
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::SegmentTree;
//!
//! let arr = vec![1, 3, 5, 7, 9, 11];
//! let mut tree = SegmentTree::from_slice(&arr, |a, b| a + b, 0);
//!
//! // Range sum query [1, 4)
//! assert_eq!(tree.query(1, 4), 15); // 3 + 5 + 7
//!
//! // Update index 2 to value 10
//! tree.update(2, 10);
//! assert_eq!(tree.query(1, 4), 20); // 3 + 10 + 7
//! ```

use alloc::vec;
use alloc::vec::Vec;

/// A segment tree for efficient range queries and point updates.
///
/// The tree supports any associative binary operation (sum, min, max, gcd, etc.).
#[derive(Debug, Clone)]
pub struct SegmentTree<T, F>
where
    F: Fn(&T, &T) -> T,
{
    tree: Vec<T>,
    n: usize,
    merge: F,
    identity: T,
}

impl<T, F> SegmentTree<T, F>
where
    T: Clone,
    F: Fn(&T, &T) -> T,
{
    /// Creates a new segment tree from a slice with a custom merge function.
    ///
    /// # Arguments
    /// * `arr` - The input array
    /// * `merge` - The associative binary operation (e.g., `|a, b| a + b` for sum)
    /// * `identity` - The identity element for the operation (0 for sum, i64::MAX for min)
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::SegmentTree;
    ///
    /// // Sum segment tree
    /// let sum_tree = SegmentTree::from_slice(&[1, 2, 3, 4], |a, b| a + b, 0);
    /// assert_eq!(sum_tree.query(0, 4), 10);
    ///
    /// // Min segment tree
    /// let min_tree = SegmentTree::from_slice(&[3, 1, 4, 1, 5], |a, b| *a.min(b), i32::MAX);
    /// assert_eq!(min_tree.query(0, 5), 1);
    /// ```
    #[must_use]
    pub fn from_slice(arr: &[T], merge: F, identity: T) -> Self {
        let n = arr.len();
        if n == 0 {
            return SegmentTree {
                tree: vec![identity.clone()],
                n: 0,
                merge,
                identity,
            };
        }

        let mut tree = vec![identity.clone(); 2 * n];

        // Fill leaves
        for (i, val) in arr.iter().enumerate() {
            tree[n + i] = val.clone();
        }

        // Build internal nodes
        for i in (1..n).rev() {
            tree[i] = merge(&tree[2 * i], &tree[2 * i + 1]);
        }

        SegmentTree {
            tree,
            n,
            merge,
            identity,
        }
    }

    /// Returns the number of elements in the original array.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.n
    }

    /// Returns `true` if the tree is empty.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.n == 0
    }

    /// Updates the value at the given index.
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::SegmentTree;
    ///
    /// let mut tree = SegmentTree::from_slice(&[1, 2, 3, 4], |a, b| a + b, 0);
    /// tree.update(2, 10);
    /// assert_eq!(tree.query(0, 4), 17); // 1 + 2 + 10 + 4
    /// ```
    pub fn update(&mut self, index: usize, value: T) {
        let mut i = self.n + index;
        self.tree[i] = value;

        while i > 1 {
            i /= 2;
            self.tree[i] = (self.merge)(&self.tree[2 * i], &self.tree[2 * i + 1]);
        }
    }

    /// Queries the range [left, right) using the merge operation.
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::SegmentTree;
    ///
    /// let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
    /// assert_eq!(tree.query(1, 4), 9); // 2 + 3 + 4
    /// assert_eq!(tree.query(0, 5), 15); // sum of all
    /// ```
    #[must_use]
    pub fn query(&self, left: usize, right: usize) -> T {
        if left >= right || left >= self.n {
            return self.identity.clone();
        }

        let right = right.min(self.n);
        let mut left_idx = self.n + left;
        let mut right_idx = self.n + right;

        let mut left_result = self.identity.clone();
        let mut right_result = self.identity.clone();

        while left_idx < right_idx {
            if left_idx % 2 == 1 {
                left_result = (self.merge)(&left_result, &self.tree[left_idx]);
                left_idx += 1;
            }
            if right_idx % 2 == 1 {
                right_idx -= 1;
                right_result = (self.merge)(&self.tree[right_idx], &right_result);
            }
            left_idx /= 2;
            right_idx /= 2;
        }

        (self.merge)(&left_result, &right_result)
    }

    /// Gets the value at a specific index.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn get(&self, index: usize) -> Option<&T> {
        if index < self.n {
            Some(&self.tree[self.n + index])
        } else {
            None
        }
    }
}

/// A segment tree optimized for range minimum queries.
pub type MinSegmentTree = SegmentTree<i64, fn(&i64, &i64) -> i64>;

/// A segment tree optimized for range maximum queries.
pub type MaxSegmentTree = SegmentTree<i64, fn(&i64, &i64) -> i64>;

/// A segment tree optimized for range sum queries.
pub type SumSegmentTree = SegmentTree<i64, fn(&i64, &i64) -> i64>;

/// Creates a sum segment tree from a slice.
#[must_use]
pub fn sum_segment_tree(arr: &[i64]) -> SumSegmentTree {
    SegmentTree::from_slice(arr, |a, b| a + b, 0)
}

/// Creates a min segment tree from a slice.
#[must_use]
pub fn min_segment_tree(arr: &[i64]) -> MinSegmentTree {
    SegmentTree::from_slice(arr, |a, b| *a.min(b), i64::MAX)
}

/// Creates a max segment tree from a slice.
#[must_use]
pub fn max_segment_tree(arr: &[i64]) -> MaxSegmentTree {
    SegmentTree::from_slice(arr, |a, b| *a.max(b), i64::MIN)
}

/// A segment tree with lazy propagation for efficient range updates.
#[derive(Debug, Clone)]
pub struct LazySegmentTree {
    tree: Vec<i64>,
    lazy: Vec<i64>,
    n: usize,
}

impl LazySegmentTree {
    /// Creates a new lazy segment tree for range sum queries.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn new(size: usize) -> Self {
        let n = size.next_power_of_two();
        LazySegmentTree {
            tree: vec![0; 2 * n],
            lazy: vec![0; 2 * n],
            n,
        }
    }

    /// Creates a lazy segment tree from a slice.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn from_slice(arr: &[i64]) -> Self {
        let size = arr.len();
        let n = size.next_power_of_two();
        let mut tree = vec![0i64; 2 * n];
        let lazy = vec![0i64; 2 * n];

        // Fill leaves
        for (i, &val) in arr.iter().enumerate() {
            tree[n + i] = val;
        }

        // Build internal nodes
        for i in (1..n).rev() {
            tree[i] = tree[2 * i] + tree[2 * i + 1];
        }

        LazySegmentTree { tree, lazy, n }
    }

    /// Returns the size of the underlying array.
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.n
    }

    /// Returns `true` if the tree is empty.
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.n == 0
    }

    fn push_down(&mut self, node: usize, left_size: usize, right_size: usize) {
        if self.lazy[node] != 0 {
            self.tree[2 * node] += self.lazy[node] * left_size as i64;
            self.tree[2 * node + 1] += self.lazy[node] * right_size as i64;
            self.lazy[2 * node] += self.lazy[node];
            self.lazy[2 * node + 1] += self.lazy[node];
            self.lazy[node] = 0;
        }
    }

    /// Adds a value to all elements in range [left, right).
    ///
    /// # Time Complexity
    /// O(log n)
    pub fn range_add(&mut self, left: usize, right: usize, delta: i64) {
        self.range_add_helper(1, 0, self.n, left, right, delta);
    }

    fn range_add_helper(
        &mut self,
        node: usize,
        node_left: usize,
        node_right: usize,
        left: usize,
        right: usize,
        delta: i64,
    ) {
        if left >= node_right || right <= node_left {
            return;
        }

        if left <= node_left && node_right <= right {
            self.tree[node] += delta * (node_right - node_left) as i64;
            self.lazy[node] += delta;
            return;
        }

        let mid = (node_left + node_right) / 2;
        self.push_down(node, mid - node_left, node_right - mid);

        self.range_add_helper(2 * node, node_left, mid, left, right, delta);
        self.range_add_helper(2 * node + 1, mid, node_right, left, right, delta);

        self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1];
    }

    /// Queries the sum of range [left, right).
    ///
    /// # Time Complexity
    /// O(log n)
    #[must_use]
    pub fn query(&mut self, left: usize, right: usize) -> i64 {
        self.query_helper(1, 0, self.n, left, right)
    }

    fn query_helper(
        &mut self,
        node: usize,
        node_left: usize,
        node_right: usize,
        left: usize,
        right: usize,
    ) -> i64 {
        if left >= node_right || right <= node_left {
            return 0;
        }

        if left <= node_left && node_right <= right {
            return self.tree[node];
        }

        let mid = (node_left + node_right) / 2;
        self.push_down(node, mid - node_left, node_right - mid);

        self.query_helper(2 * node, node_left, mid, left, right)
            + self.query_helper(2 * node + 1, mid, node_right, left, right)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_from_slice_sum() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4], |a, b| a + b, 0);
            assert_eq!(tree.len(), 4);
            assert_eq!(tree.query(0, 4), 10);
        }

        #[test]
        fn test_from_slice_min() {
            let tree = SegmentTree::from_slice(&[3, 1, 4, 1, 5], |a, b| *a.min(b), i32::MAX);
            assert_eq!(tree.query(0, 5), 1);
        }

        #[test]
        fn test_from_slice_max() {
            let tree = SegmentTree::from_slice(&[3, 1, 4, 1, 5], |a, b| *a.max(b), i32::MIN);
            assert_eq!(tree.query(0, 5), 5);
        }

        #[test]
        fn test_empty() {
            let tree: SegmentTree<i32, _> = SegmentTree::from_slice(&[], |a, b| a + b, 0);
            assert!(tree.is_empty());
            assert_eq!(tree.query(0, 0), 0);
        }
    }

    mod query {
        use super::*;

        #[test]
        fn test_query_full_range() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.query(0, 5), 15);
        }

        #[test]
        fn test_query_partial_range() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.query(1, 4), 9); // 2 + 3 + 4
            assert_eq!(tree.query(0, 3), 6); // 1 + 2 + 3
            assert_eq!(tree.query(2, 5), 12); // 3 + 4 + 5
        }

        #[test]
        fn test_query_single_element() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.query(2, 3), 3);
        }

        #[test]
        fn test_query_empty_range() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.query(3, 3), 0);
            assert_eq!(tree.query(4, 2), 0);
        }

        #[test]
        fn test_query_out_of_bounds() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.query(0, 10), 15); // Clamped to valid range
            assert_eq!(tree.query(10, 15), 0);
        }
    }

    mod update {
        use super::*;

        #[test]
        fn test_update() {
            let mut tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            tree.update(2, 10);
            assert_eq!(tree.query(0, 5), 22); // 1 + 2 + 10 + 4 + 5
            assert_eq!(tree.query(2, 3), 10);
        }

        #[test]
        fn test_multiple_updates() {
            let mut tree = SegmentTree::from_slice(&[1, 1, 1, 1, 1], |a, b| a + b, 0);
            tree.update(0, 5);
            tree.update(2, 5);
            tree.update(4, 5);
            assert_eq!(tree.query(0, 5), 17); // 5 + 1 + 5 + 1 + 5
        }

        #[test]
        fn test_update_min_tree() {
            let mut tree = SegmentTree::from_slice(&[5, 3, 7, 1, 9], |a, b| *a.min(b), i32::MAX);
            assert_eq!(tree.query(0, 5), 1);
            tree.update(3, 10);
            assert_eq!(tree.query(0, 5), 3);
        }
    }

    mod get {
        use super::*;

        #[test]
        fn test_get() {
            let tree = SegmentTree::from_slice(&[1, 2, 3, 4, 5], |a, b| a + b, 0);
            assert_eq!(tree.get(0), Some(&1));
            assert_eq!(tree.get(2), Some(&3));
            assert_eq!(tree.get(4), Some(&5));
            assert_eq!(tree.get(5), None);
        }
    }

    mod helper_functions {
        use super::*;

        #[test]
        fn test_sum_segment_tree() {
            let tree = sum_segment_tree(&[1, 2, 3, 4, 5]);
            assert_eq!(tree.query(0, 5), 15);
        }

        #[test]
        fn test_min_segment_tree() {
            let tree = min_segment_tree(&[5, 3, 7, 1, 9]);
            assert_eq!(tree.query(0, 5), 1);
        }

        #[test]
        fn test_max_segment_tree() {
            let tree = max_segment_tree(&[5, 3, 7, 1, 9]);
            assert_eq!(tree.query(0, 5), 9);
        }
    }

    mod lazy_segment_tree {
        use super::*;

        #[test]
        fn test_new() {
            let tree = LazySegmentTree::new(8);
            assert_eq!(tree.len(), 8);
        }

        #[test]
        fn test_from_slice() {
            let mut tree = LazySegmentTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(tree.query(0, 5), 15);
        }

        #[test]
        fn test_range_add() {
            let mut tree = LazySegmentTree::from_slice(&[1, 2, 3, 4, 5]);
            tree.range_add(1, 4, 10); // Add 10 to indices 1, 2, 3
            assert_eq!(tree.query(0, 5), 45); // 1 + 12 + 13 + 14 + 5
            assert_eq!(tree.query(1, 4), 39); // 12 + 13 + 14
        }

        #[test]
        fn test_multiple_range_adds() {
            let mut tree = LazySegmentTree::from_slice(&[0, 0, 0, 0, 0]);
            tree.range_add(0, 5, 1); // All become 1
            tree.range_add(2, 4, 2); // Indices 2,3 become 3
            assert_eq!(tree.query(0, 5), 9); // 1 + 1 + 3 + 3 + 1
        }

        #[test]
        fn test_mixed_operations() {
            let mut tree = LazySegmentTree::from_slice(&[1, 2, 3, 4, 5]);
            assert_eq!(tree.query(0, 3), 6);
            tree.range_add(0, 3, 5);
            assert_eq!(tree.query(0, 3), 21);
            assert_eq!(tree.query(3, 5), 9);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let tree = SegmentTree::from_slice(&[42], |a, b| a + b, 0);
            assert_eq!(tree.query(0, 1), 42);
        }

        #[test]
        fn test_two_elements() {
            let tree = SegmentTree::from_slice(&[1, 2], |a, b| a + b, 0);
            assert_eq!(tree.query(0, 2), 3);
            assert_eq!(tree.query(0, 1), 1);
            assert_eq!(tree.query(1, 2), 2);
        }

        #[test]
        fn test_large_array() {
            let arr: Vec<i64> = (1..=1000).collect();
            let tree = sum_segment_tree(&arr);
            assert_eq!(tree.query(0, 1000), 500500);
        }

        #[test]
        fn test_negative_values() {
            let tree = SegmentTree::from_slice(&[-5, -3, -1, 2, 4], |a, b| a + b, 0);
            assert_eq!(tree.query(0, 5), -3);
            assert_eq!(tree.query(0, 3), -9);
        }

        #[test]
        fn test_gcd_tree() {
            fn gcd(a: &i64, b: &i64) -> i64 {
                let (mut a, mut b) = (*a, *b);
                while b != 0 {
                    let t = b;
                    b = a % b;
                    a = t;
                }
                a
            }
            let tree = SegmentTree::from_slice(&[12, 18, 24, 36], gcd, 0);
            assert_eq!(tree.query(0, 4), 6);
            assert_eq!(tree.query(0, 2), 6);
            assert_eq!(tree.query(2, 4), 12);
        }
    }
}
