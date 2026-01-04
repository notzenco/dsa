//! Monotonic Queue
//!
//! A monotonic queue maintains elements in monotonically increasing or decreasing order.
//! It's particularly useful for sliding window problems.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Monotonic Decreasing Queue (Max Queue):                                   ║
//! ║                                                                            ║
//! ║  Input sequence: [3, 1, 4, 1, 5, 9, 2, 6]                                  ║
//! ║                                                                            ║
//! ║  Step-by-step (window size = 3):                                           ║
//! ║  ┌───────────────────────────────────────────────────────────────────┐     ║
//! ║  │ Window    │ Deque State      │ Max │ Comment                      │     ║
//! ║  ├───────────┼──────────────────┼─────┼──────────────────────────────┤     ║
//! ║  │ [3]       │ [3]              │  3  │ Add 3                        │     ║
//! ║  │ [3,1]     │ [3,1]            │  3  │ 1 < 3, keep both             │     ║
//! ║  │ [3,1,4]   │ [4]              │  4  │ 4 > all, clear and add       │     ║
//! ║  │ [1,4,1]   │ [4,1]            │  4  │ 3 leaves, add 1              │     ║
//! ║  │ [4,1,5]   │ [5]              │  5  │ 5 > all, clear and add       │     ║
//! ║  │ [1,5,9]   │ [9]              │  9  │ 9 > all, clear and add       │     ║
//! ║  │ [5,9,2]   │ [9,2]            │  9  │ 2 < 9, add                   │     ║
//! ║  │ [9,2,6]   │ [9,6]            │  9  │ 6 > 2, pop 2, add 6          │     ║
//! ║  └───────────────────────────────────────────────────────────────────┘     ║
//! ║                                                                            ║
//! ║  Invariant: Elements are always in decreasing order from front to back     ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation         | Average | Worst | Space |
//! |-------------------|---------|-------|-------|
//! | Push              | O(1)*   | O(n)  | O(1)  |
//! | Pop (by index)    | O(1)    | O(1)  | O(1)  |
//! | Get Max/Min       | O(1)    | O(1)  | O(1)  |
//! | Sliding Window    | O(n)    | O(n)  | O(k)  |
//!
//! *Amortized O(1) - each element is pushed and popped at most once
//!
//! ## LeetCode Problems
//!
//! - [#239 Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/)
//! - [#1425 Constrained Subsequence Sum](https://leetcode.com/problems/constrained-subsequence-sum/)
//! - [#1438 Longest Continuous Subarray](https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/)
//! - [#862 Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/)
//! - [#1499 Max Value of Equation](https://leetcode.com/problems/max-value-of-equation/)
//!
//! ## Use Cases
//!
//! - Sliding window maximum/minimum
//! - Stock span problem
//! - Constrained optimization problems
//! - Range maximum/minimum queries (with sliding windows)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::MonotonicQueue;
//!
//! // Create a max queue (monotonically decreasing)
//! let mut max_queue = MonotonicQueue::<i32>::new_max();
//! max_queue.push(3, 0);
//! max_queue.push(1, 1);
//! max_queue.push(4, 2);
//!
//! assert_eq!(max_queue.front(), Some(&4)); // Maximum is 4
//!
//! // Create a min queue (monotonically increasing)
//! let mut min_queue = MonotonicQueue::<i32>::new_min();
//! min_queue.push(3, 0);
//! min_queue.push(1, 1);
//! min_queue.push(4, 2);
//!
//! assert_eq!(min_queue.front(), Some(&1)); // Minimum is 1
//! ```

use alloc::collections::VecDeque;
use alloc::vec::Vec;

/// Comparison order for the monotonic queue.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MonotonicOrder {
    /// Decreasing order (front has maximum)
    Decreasing,
    /// Increasing order (front has minimum)
    Increasing,
}

/// A monotonic queue that maintains elements in sorted order.
///
/// Each element is stored with its index for efficient window-based removal.
#[derive(Debug, Clone)]
pub struct MonotonicQueue<T> {
    /// The internal deque storing (value, index) pairs
    data: VecDeque<(T, usize)>,
    /// The ordering to maintain
    order: MonotonicOrder,
}

impl<T: PartialOrd> MonotonicQueue<T> {
    /// Creates a new monotonic queue with the specified ordering.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn new(order: MonotonicOrder) -> Self {
        MonotonicQueue {
            data: VecDeque::new(),
            order,
        }
    }

    /// Creates a new max queue (monotonically decreasing).
    ///
    /// The front element is always the maximum.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::MonotonicQueue;
    ///
    /// let mut queue = MonotonicQueue::<i32>::new_max();
    /// queue.push(1, 0);
    /// queue.push(3, 1);
    /// queue.push(2, 2);
    /// assert_eq!(queue.front(), Some(&3)); // Max element
    /// ```
    #[must_use]
    pub fn new_max() -> Self {
        Self::new(MonotonicOrder::Decreasing)
    }

    /// Creates a new min queue (monotonically increasing).
    ///
    /// The front element is always the minimum.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::MonotonicQueue;
    ///
    /// let mut queue = MonotonicQueue::<i32>::new_min();
    /// queue.push(3, 0);
    /// queue.push(1, 1);
    /// queue.push(2, 2);
    /// assert_eq!(queue.front(), Some(&1)); // Min element
    /// ```
    #[must_use]
    pub fn new_min() -> Self {
        Self::new(MonotonicOrder::Increasing)
    }

    /// Returns the number of elements in the queue.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns `true` if the queue contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Pushes an element with its index, maintaining monotonic order.
    ///
    /// Elements that violate the monotonic property are removed from the back.
    ///
    /// # Time Complexity
    /// O(1) amortized (each element is pushed and popped at most once)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::MonotonicQueue;
    ///
    /// let mut queue = MonotonicQueue::<i32>::new_max();
    /// queue.push(2, 0);
    /// queue.push(1, 1); // 1 < 2, added after 2
    /// queue.push(3, 2); // 3 > 2 and 1, they are removed
    /// assert_eq!(queue.len(), 1);
    /// assert_eq!(queue.front(), Some(&3));
    /// ```
    pub fn push(&mut self, value: T, index: usize) {
        match self.order {
            MonotonicOrder::Decreasing => {
                // Remove elements smaller than the new value
                while let Some((back_val, _)) = self.data.back() {
                    if *back_val <= value {
                        self.data.pop_back();
                    } else {
                        break;
                    }
                }
            }
            MonotonicOrder::Increasing => {
                // Remove elements larger than the new value
                while let Some((back_val, _)) = self.data.back() {
                    if *back_val >= value {
                        self.data.pop_back();
                    } else {
                        break;
                    }
                }
            }
        }
        self.data.push_back((value, index));
    }

    /// Removes elements from the front that are outside the given window.
    ///
    /// # Time Complexity
    /// O(1) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::MonotonicQueue;
    ///
    /// let mut queue = MonotonicQueue::<i32>::new_max();
    /// queue.push(5, 0);
    /// queue.push(3, 1);
    /// queue.push(4, 2);
    ///
    /// // Remove elements with index < 1 (i.e., index 0)
    /// queue.pop_front_if_before(1);
    /// assert_eq!(queue.front(), Some(&4));
    /// ```
    pub fn pop_front_if_before(&mut self, min_index: usize) {
        while let Some((_, idx)) = self.data.front() {
            if *idx < min_index {
                self.data.pop_front();
            } else {
                break;
            }
        }
    }

    /// Returns a reference to the front (maximum or minimum) element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn front(&self) -> Option<&T> {
        self.data.front().map(|(v, _)| v)
    }

    /// Returns a reference to the front element with its index.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn front_with_index(&self) -> Option<(&T, usize)> {
        self.data.front().map(|(v, idx)| (v, *idx))
    }

    /// Returns a reference to the back element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn back(&self) -> Option<&T> {
        self.data.back().map(|(v, _)| v)
    }

    /// Returns a reference to the back element with its index.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn back_with_index(&self) -> Option<(&T, usize)> {
        self.data.back().map(|(v, idx)| (v, *idx))
    }

    /// Clears the queue, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Returns an iterator over the elements from front to back.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter().map(|(v, _)| v)
    }

    /// Returns an iterator over (value, index) pairs from front to back.
    pub fn iter_with_index(&self) -> impl Iterator<Item = (&T, usize)> {
        self.data.iter().map(|(v, idx)| (v, *idx))
    }
}

impl<T: PartialOrd + Clone> MonotonicQueue<T> {
    /// Computes the maximum (or minimum) for each sliding window.
    ///
    /// # Arguments
    /// * `arr` - The input array
    /// * `k` - Window size
    ///
    /// # Time Complexity
    /// O(n) where n is the length of the array
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::MonotonicQueue;
    ///
    /// let arr = vec![1, 3, -1, -3, 5, 3, 6, 7];
    /// let max_queue = MonotonicQueue::<i32>::new_max();
    /// let result = max_queue.sliding_window_extremes(&arr, 3);
    /// assert_eq!(result, vec![3, 3, 5, 5, 6, 7]);
    /// ```
    #[must_use]
    pub fn sliding_window_extremes(&self, arr: &[T], k: usize) -> Vec<T> {
        if arr.is_empty() || k == 0 || k > arr.len() {
            return Vec::new();
        }

        let mut queue = MonotonicQueue::new(self.order);
        let mut result = Vec::with_capacity(arr.len() - k + 1);

        for (i, item) in arr.iter().enumerate() {
            queue.push(item.clone(), i);

            // Remove elements outside the window
            if i >= k {
                queue.pop_front_if_before(i - k + 1);
            }

            // Once we have a full window, record the extreme
            if i >= k - 1 {
                if let Some(val) = queue.front() {
                    result.push(val.clone());
                }
            }
        }

        result
    }
}

/// Computes the sliding window maximum for a given array.
///
/// # Arguments
/// * `arr` - The input array
/// * `k` - Window size
///
/// # Time Complexity
/// O(n) where n is the length of the array
///
/// # Example
///
/// ```rust
/// use dsa_data_structures::linear::sliding_window_maximum;
///
/// let arr = vec![1, 3, -1, -3, 5, 3, 6, 7];
/// let result = sliding_window_maximum(&arr, 3);
/// assert_eq!(result, vec![3, 3, 5, 5, 6, 7]);
/// ```
pub fn sliding_window_maximum<T: PartialOrd + Clone>(arr: &[T], k: usize) -> Vec<T> {
    let queue = MonotonicQueue::<T>::new_max();
    queue.sliding_window_extremes(arr, k)
}

/// Computes the sliding window minimum for a given array.
///
/// # Arguments
/// * `arr` - The input array
/// * `k` - Window size
///
/// # Time Complexity
/// O(n) where n is the length of the array
///
/// # Example
///
/// ```rust
/// use dsa_data_structures::linear::sliding_window_minimum;
///
/// let arr = vec![1, 3, -1, -3, 5, 3, 6, 7];
/// let result = sliding_window_minimum(&arr, 3);
/// assert_eq!(result, vec![-1, -3, -3, -3, 3, 3]);
/// ```
pub fn sliding_window_minimum<T: PartialOrd + Clone>(arr: &[T], k: usize) -> Vec<T> {
    let queue = MonotonicQueue::<T>::new_min();
    queue.sliding_window_extremes(arr, k)
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new_max() {
            let queue = MonotonicQueue::<i32>::new_max();
            assert!(queue.is_empty());
            assert_eq!(queue.len(), 0);
        }

        #[test]
        fn test_new_min() {
            let queue = MonotonicQueue::<i32>::new_min();
            assert!(queue.is_empty());
            assert_eq!(queue.len(), 0);
        }

        #[test]
        fn test_order() {
            let max_queue = MonotonicQueue::<i32>::new_max();
            let min_queue = MonotonicQueue::<i32>::new_min();
            assert_eq!(max_queue.order, MonotonicOrder::Decreasing);
            assert_eq!(min_queue.order, MonotonicOrder::Increasing);
        }
    }

    mod max_queue {
        use super::*;

        #[test]
        fn test_push_increasing() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(1, 0);
            queue.push(2, 1);
            queue.push(3, 2);
            // All previous elements are removed
            assert_eq!(queue.len(), 1);
            assert_eq!(queue.front(), Some(&3));
        }

        #[test]
        fn test_push_decreasing() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(3, 0);
            queue.push(2, 1);
            queue.push(1, 2);
            // All elements are kept
            assert_eq!(queue.len(), 3);
            assert_eq!(queue.front(), Some(&3));
            assert_eq!(queue.back(), Some(&1));
        }

        #[test]
        fn test_push_mixed() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(2, 0);
            queue.push(1, 1);
            queue.push(3, 2);
            // 3 removes 1 and 2
            assert_eq!(queue.len(), 1);
            assert_eq!(queue.front(), Some(&3));
        }

        #[test]
        fn test_front_with_index() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(3, 1);
            assert_eq!(queue.front_with_index(), Some((&5, 0)));
        }
    }

    mod min_queue {
        use super::*;

        #[test]
        fn test_push_decreasing() {
            let mut queue = MonotonicQueue::<i32>::new_min();
            queue.push(3, 0);
            queue.push(2, 1);
            queue.push(1, 2);
            // All previous elements are removed
            assert_eq!(queue.len(), 1);
            assert_eq!(queue.front(), Some(&1));
        }

        #[test]
        fn test_push_increasing() {
            let mut queue = MonotonicQueue::<i32>::new_min();
            queue.push(1, 0);
            queue.push(2, 1);
            queue.push(3, 2);
            // All elements are kept
            assert_eq!(queue.len(), 3);
            assert_eq!(queue.front(), Some(&1));
            assert_eq!(queue.back(), Some(&3));
        }

        #[test]
        fn test_push_mixed() {
            let mut queue = MonotonicQueue::<i32>::new_min();
            queue.push(2, 0);
            queue.push(3, 1);
            queue.push(1, 2);
            // 1 removes 2 and 3
            assert_eq!(queue.len(), 1);
            assert_eq!(queue.front(), Some(&1));
        }
    }

    mod pop_front_if_before {
        use super::*;

        #[test]
        fn test_pop_old_elements() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(4, 1);
            queue.push(3, 2);

            queue.pop_front_if_before(1);
            assert_eq!(queue.front(), Some(&4));

            queue.pop_front_if_before(2);
            assert_eq!(queue.front(), Some(&3));
        }

        #[test]
        fn test_pop_nothing() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(4, 1);

            queue.pop_front_if_before(0);
            assert_eq!(queue.len(), 2);
        }

        #[test]
        fn test_pop_all() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(4, 1);

            queue.pop_front_if_before(10);
            assert!(queue.is_empty());
        }
    }

    mod sliding_window {
        use super::*;

        #[test]
        fn test_sliding_window_maximum() {
            let arr = vec![1, 3, -1, -3, 5, 3, 6, 7];
            let result = sliding_window_maximum(&arr, 3);
            assert_eq!(result, vec![3, 3, 5, 5, 6, 7]);
        }

        #[test]
        fn test_sliding_window_minimum() {
            let arr = vec![1, 3, -1, -3, 5, 3, 6, 7];
            let result = sliding_window_minimum(&arr, 3);
            assert_eq!(result, vec![-1, -3, -3, -3, 3, 3]);
        }

        #[test]
        fn test_window_size_one() {
            let arr = vec![1, 3, -1, -3, 5];
            let max_result = sliding_window_maximum(&arr, 1);
            let min_result = sliding_window_minimum(&arr, 1);
            assert_eq!(max_result, vec![1, 3, -1, -3, 5]);
            assert_eq!(min_result, vec![1, 3, -1, -3, 5]);
        }

        #[test]
        fn test_window_size_equals_array() {
            let arr = vec![1, 3, -1, -3, 5];
            let max_result = sliding_window_maximum(&arr, 5);
            let min_result = sliding_window_minimum(&arr, 5);
            assert_eq!(max_result, vec![5]);
            assert_eq!(min_result, vec![-3]);
        }

        #[test]
        fn test_empty_array() {
            let arr: Vec<i32> = vec![];
            let result = sliding_window_maximum(&arr, 3);
            assert!(result.is_empty());
        }

        #[test]
        fn test_window_larger_than_array() {
            let arr = vec![1, 2, 3];
            let result = sliding_window_maximum(&arr, 5);
            assert!(result.is_empty());
        }

        #[test]
        fn test_window_size_zero() {
            let arr = vec![1, 2, 3];
            let result = sliding_window_maximum(&arr, 0);
            assert!(result.is_empty());
        }

        #[test]
        fn test_leetcode_239_example() {
            // LeetCode #239: Sliding Window Maximum
            let nums = vec![1, 3, -1, -3, 5, 3, 6, 7];
            let k = 3;
            let expected = vec![3, 3, 5, 5, 6, 7];
            assert_eq!(sliding_window_maximum(&nums, k), expected);
        }

        #[test]
        fn test_all_same_elements() {
            let arr = vec![5, 5, 5, 5, 5];
            let max_result = sliding_window_maximum(&arr, 3);
            let min_result = sliding_window_minimum(&arr, 3);
            assert_eq!(max_result, vec![5, 5, 5]);
            assert_eq!(min_result, vec![5, 5, 5]);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(1, 0);
            queue.push(2, 1);
            queue.clear();
            assert!(queue.is_empty());
        }

        #[test]
        fn test_iter() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(3, 1);
            queue.push(1, 2);
            let collected: Vec<_> = queue.iter().cloned().collect();
            assert_eq!(collected, vec![5, 3, 1]);
        }

        #[test]
        fn test_iter_with_index() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(3, 1);
            let collected: Vec<_> = queue.iter_with_index().map(|(v, i)| (*v, i)).collect();
            assert_eq!(collected, vec![(5, 0), (3, 1)]);
        }

        #[test]
        fn test_back() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(3, 1);
            assert_eq!(queue.back(), Some(&3));
            assert_eq!(queue.back_with_index(), Some((&3, 1)));
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_empty_operations() {
            let queue = MonotonicQueue::<i32>::new_max();
            assert_eq!(queue.front(), None);
            assert_eq!(queue.back(), None);
            assert_eq!(queue.front_with_index(), None);
            assert_eq!(queue.back_with_index(), None);
        }

        #[test]
        fn test_single_element() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(42, 0);
            assert_eq!(queue.front(), Some(&42));
            assert_eq!(queue.back(), Some(&42));
            assert_eq!(queue.len(), 1);
        }

        #[test]
        fn test_duplicate_values() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            queue.push(5, 0);
            queue.push(5, 1);
            queue.push(5, 2);
            // Equal values are removed (<=)
            assert_eq!(queue.len(), 1);
            assert_eq!(queue.front_with_index(), Some((&5, 2)));
        }

        #[test]
        fn test_negative_values() {
            let arr = vec![-5, -3, -1, -4, -2];
            let max_result = sliding_window_maximum(&arr, 2);
            let min_result = sliding_window_minimum(&arr, 2);
            assert_eq!(max_result, vec![-3, -1, -1, -2]);
            assert_eq!(min_result, vec![-5, -3, -4, -4]);
        }

        #[test]
        fn test_large_window() {
            let mut queue = MonotonicQueue::<i32>::new_max();
            for i in 0..10000 {
                queue.push(i, i as usize);
            }
            // Last element should be the maximum
            assert_eq!(queue.front(), Some(&9999));
        }
    }
}
