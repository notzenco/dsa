//! Deque (Double-Ended Queue)
//!
//! A deque is a linear data structure that allows insertion and deletion at both ends.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Deque Structure:                                                          ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
//! ║  │  1  │  2  │  3  │  4  │  5  │                                           ║
//! ║  └─────┴─────┴─────┴─────┴─────┘                                           ║
//! ║    ▲                       ▲                                               ║
//! ║  front                   back                                              ║
//! ║  (push/pop)           (push/pop)                                           ║
//! ║                                                                            ║
//! ║  Push Front:                                                               ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┬─────┐                                     ║
//! ║  │ NEW │  1  │  2  │  3  │  4  │  5  │ ◀── push_front(0)                   ║
//! ║  └─────┴─────┴─────┴─────┴─────┴─────┘                                     ║
//! ║                                                                            ║
//! ║  Push Back:                                                                ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┬─────┐                                     ║
//! ║  │  1  │  2  │  3  │  4  │  5  │ NEW │ ◀── push_back(6)                    ║
//! ║  └─────┴─────┴─────┴─────┴─────┴─────┘                                     ║
//! ║                                                                            ║
//! ║  Pop Operations:                                                           ║
//! ║  pop_front() ──▶ returns 1    pop_back() ──▶ returns 5                     ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation   | Average | Worst | Space |
//! |-------------|---------|-------|-------|
//! | Push Front  | O(1)*   | O(n)  | O(1)  |
//! | Push Back   | O(1)*   | O(n)  | O(1)  |
//! | Pop Front   | O(1)    | O(1)  | O(1)  |
//! | Pop Back    | O(1)    | O(1)  | O(1)  |
//! | Peek        | O(1)    | O(1)  | O(1)  |
//! | Access      | O(1)    | O(1)  | O(1)  |
//! | Search      | O(n)    | O(n)  | O(1)  |
//!
//! *Amortized O(1) using VecDeque
//!
//! ## LeetCode Problems
//!
//! - [#239 Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/)
//! - [#641 Design Circular Deque](https://leetcode.com/problems/design-circular-deque/)
//! - [#862 Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/)
//! - [#1438 Longest Continuous Subarray](https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/)
//!
//! ## Use Cases
//!
//! - Sliding window algorithms
//! - Palindrome checking
//! - Browser history (back/forward)
//! - Undo/redo operations
//! - Work stealing algorithms
//! - A-Steal task scheduler
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::Deque;
//!
//! let mut deque = Deque::new();
//! deque.push_back(1);
//! deque.push_back(2);
//! deque.push_front(0);
//!
//! assert_eq!(deque.len(), 3);
//! assert_eq!(deque.front(), Some(&0));
//! assert_eq!(deque.back(), Some(&2));
//! assert_eq!(deque.pop_front(), Some(0));
//! assert_eq!(deque.pop_back(), Some(2));
//! ```

use alloc::collections::VecDeque;
use alloc::vec::Vec;

use dsa_core::{Container, Searchable};

/// A double-ended queue implementation.
///
/// This implementation uses `VecDeque` as the underlying storage,
/// providing amortized O(1) operations at both ends.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Deque<T> {
    data: VecDeque<T>,
}

impl<T> Deque<T> {
    /// Creates a new empty deque.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let deque: Deque<i32> = Deque::new();
    /// assert!(deque.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Deque {
            data: VecDeque::new(),
        }
    }

    /// Creates a new deque with the specified capacity.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn with_capacity(capacity: usize) -> Self {
        Deque {
            data: VecDeque::with_capacity(capacity),
        }
    }

    /// Returns the number of elements in the deque.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns `true` if the deque contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Adds an element to the front of the deque.
    ///
    /// # Time Complexity
    /// O(1) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let mut deque = Deque::new();
    /// deque.push_front(1);
    /// deque.push_front(2);
    /// assert_eq!(deque.front(), Some(&2));
    /// ```
    pub fn push_front(&mut self, value: T) {
        self.data.push_front(value);
    }

    /// Adds an element to the back of the deque.
    ///
    /// # Time Complexity
    /// O(1) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let mut deque = Deque::new();
    /// deque.push_back(1);
    /// deque.push_back(2);
    /// assert_eq!(deque.back(), Some(&2));
    /// ```
    pub fn push_back(&mut self, value: T) {
        self.data.push_back(value);
    }

    /// Removes and returns the front element.
    ///
    /// Returns `None` if the deque is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let mut deque = Deque::from_vec(vec![1, 2, 3]);
    /// assert_eq!(deque.pop_front(), Some(1));
    /// assert_eq!(deque.pop_front(), Some(2));
    /// ```
    pub fn pop_front(&mut self) -> Option<T> {
        self.data.pop_front()
    }

    /// Removes and returns the back element.
    ///
    /// Returns `None` if the deque is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let mut deque = Deque::from_vec(vec![1, 2, 3]);
    /// assert_eq!(deque.pop_back(), Some(3));
    /// assert_eq!(deque.pop_back(), Some(2));
    /// ```
    pub fn pop_back(&mut self) -> Option<T> {
        self.data.pop_back()
    }

    /// Returns a reference to the front element.
    ///
    /// Returns `None` if the deque is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let deque = Deque::from_vec(vec![1, 2, 3]);
    /// assert_eq!(deque.front(), Some(&1));
    /// ```
    #[must_use]
    pub fn front(&self) -> Option<&T> {
        self.data.front()
    }

    /// Returns a mutable reference to the front element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn front_mut(&mut self) -> Option<&mut T> {
        self.data.front_mut()
    }

    /// Returns a reference to the back element.
    ///
    /// Returns `None` if the deque is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Deque;
    ///
    /// let deque = Deque::from_vec(vec![1, 2, 3]);
    /// assert_eq!(deque.back(), Some(&3));
    /// ```
    #[must_use]
    pub fn back(&self) -> Option<&T> {
        self.data.back()
    }

    /// Returns a mutable reference to the back element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn back_mut(&mut self) -> Option<&mut T> {
        self.data.back_mut()
    }

    /// Returns a reference to the element at the given index.
    ///
    /// Returns `None` if the index is out of bounds.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn get(&self, index: usize) -> Option<&T> {
        self.data.get(index)
    }

    /// Returns a mutable reference to the element at the given index.
    ///
    /// Returns `None` if the index is out of bounds.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn get_mut(&mut self, index: usize) -> Option<&mut T> {
        self.data.get_mut(index)
    }

    /// Clears the deque, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Returns an iterator over the deque from front to back.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter()
    }

    /// Converts the deque to a `Vec` (front to back).
    #[must_use]
    pub fn to_vec(&self) -> Vec<T>
    where
        T: Clone,
    {
        self.data.iter().cloned().collect()
    }

    /// Creates a deque from a `Vec` (first element is front).
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        Deque {
            data: vec.into_iter().collect(),
        }
    }

    /// Rotates the deque so that the first `n` elements move to the back.
    ///
    /// # Time Complexity
    /// O(min(n, len - n))
    pub fn rotate_left(&mut self, n: usize) {
        if !self.is_empty() {
            self.data.rotate_left(n % self.len());
        }
    }

    /// Rotates the deque so that the last `n` elements move to the front.
    ///
    /// # Time Complexity
    /// O(min(n, len - n))
    pub fn rotate_right(&mut self, n: usize) {
        if !self.is_empty() {
            self.data.rotate_right(n % self.len());
        }
    }

    /// Swaps elements at indices `i` and `j`.
    ///
    /// # Panics
    /// Panics if either index is out of bounds.
    pub fn swap(&mut self, i: usize, j: usize) {
        self.data.swap(i, j);
    }
}

impl<T: PartialEq> Deque<T> {
    /// Returns `true` if the deque contains the specified value.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn contains(&self, value: &T) -> bool {
        self.data.contains(value)
    }
}

impl<T> Container for Deque<T> {
    fn len(&self) -> usize {
        self.data.len()
    }
}

impl<T: PartialEq> Searchable<T> for Deque<T> {
    fn search(&self, value: &T) -> Option<usize> {
        self.data.iter().position(|x| x == value)
    }
}

impl<T> Default for Deque<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> FromIterator<T> for Deque<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        Deque {
            data: iter.into_iter().collect(),
        }
    }
}

impl<T> IntoIterator for Deque<T> {
    type Item = T;
    type IntoIter = alloc::collections::vec_deque::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.into_iter()
    }
}

impl<'a, T> IntoIterator for &'a Deque<T> {
    type Item = &'a T;
    type IntoIter = alloc::collections::vec_deque::Iter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.iter()
    }
}

impl<T> core::ops::Index<usize> for Deque<T> {
    type Output = T;

    fn index(&self, index: usize) -> &Self::Output {
        &self.data[index]
    }
}

impl<T> core::ops::IndexMut<usize> for Deque<T> {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.data[index]
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let deque: Deque<i32> = Deque::new();
            assert!(deque.is_empty());
            assert_eq!(deque.len(), 0);
        }

        #[test]
        fn test_with_capacity() {
            let deque: Deque<i32> = Deque::with_capacity(10);
            assert!(deque.is_empty());
        }

        #[test]
        fn test_default() {
            let deque: Deque<i32> = Deque::default();
            assert!(deque.is_empty());
        }

        #[test]
        fn test_from_vec() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.len(), 3);
            assert_eq!(deque.front(), Some(&1));
            assert_eq!(deque.back(), Some(&3));
        }

        #[test]
        fn test_to_vec() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_clone() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            let cloned = deque.clone();
            assert_eq!(deque, cloned);
        }
    }

    mod push_pop {
        use super::*;

        #[test]
        fn test_push_front() {
            let mut deque = Deque::new();
            deque.push_front(3);
            deque.push_front(2);
            deque.push_front(1);
            assert_eq!(deque.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_push_back() {
            let mut deque = Deque::new();
            deque.push_back(1);
            deque.push_back(2);
            deque.push_back(3);
            assert_eq!(deque.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_pop_front() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.pop_front(), Some(1));
            assert_eq!(deque.pop_front(), Some(2));
            assert_eq!(deque.pop_front(), Some(3));
            assert_eq!(deque.pop_front(), None);
        }

        #[test]
        fn test_pop_back() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.pop_back(), Some(3));
            assert_eq!(deque.pop_back(), Some(2));
            assert_eq!(deque.pop_back(), Some(1));
            assert_eq!(deque.pop_back(), None);
        }

        #[test]
        fn test_mixed_operations() {
            let mut deque = Deque::new();
            deque.push_back(2);
            deque.push_front(1);
            deque.push_back(3);
            deque.push_front(0);
            assert_eq!(deque.to_vec(), vec![0, 1, 2, 3]);

            assert_eq!(deque.pop_front(), Some(0));
            assert_eq!(deque.pop_back(), Some(3));
            assert_eq!(deque.to_vec(), vec![1, 2]);
        }
    }

    mod peek {
        use super::*;

        #[test]
        fn test_front() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.front(), Some(&1));
        }

        #[test]
        fn test_front_empty() {
            let deque: Deque<i32> = Deque::new();
            assert_eq!(deque.front(), None);
        }

        #[test]
        fn test_front_mut() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            if let Some(front) = deque.front_mut() {
                *front = 10;
            }
            assert_eq!(deque.front(), Some(&10));
        }

        #[test]
        fn test_back() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.back(), Some(&3));
        }

        #[test]
        fn test_back_empty() {
            let deque: Deque<i32> = Deque::new();
            assert_eq!(deque.back(), None);
        }

        #[test]
        fn test_back_mut() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            if let Some(back) = deque.back_mut() {
                *back = 30;
            }
            assert_eq!(deque.back(), Some(&30));
        }
    }

    mod access {
        use super::*;

        #[test]
        fn test_get() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.get(0), Some(&1));
            assert_eq!(deque.get(1), Some(&2));
            assert_eq!(deque.get(2), Some(&3));
            assert_eq!(deque.get(3), None);
        }

        #[test]
        fn test_get_mut() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            if let Some(val) = deque.get_mut(1) {
                *val = 20;
            }
            assert_eq!(deque.get(1), Some(&20));
        }

        #[test]
        fn test_index() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque[0], 1);
            assert_eq!(deque[1], 2);
            assert_eq!(deque[2], 3);
        }

        #[test]
        fn test_index_mut() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            deque[1] = 20;
            assert_eq!(deque[1], 20);
        }
    }

    mod rotation {
        use super::*;

        #[test]
        fn test_rotate_left() {
            let mut deque = Deque::from_vec(vec![1, 2, 3, 4, 5]);
            deque.rotate_left(2);
            assert_eq!(deque.to_vec(), vec![3, 4, 5, 1, 2]);
        }

        #[test]
        fn test_rotate_right() {
            let mut deque = Deque::from_vec(vec![1, 2, 3, 4, 5]);
            deque.rotate_right(2);
            assert_eq!(deque.to_vec(), vec![4, 5, 1, 2, 3]);
        }

        #[test]
        fn test_rotate_empty() {
            let mut deque: Deque<i32> = Deque::new();
            deque.rotate_left(1); // Should not panic
            deque.rotate_right(1); // Should not panic
        }

        #[test]
        fn test_rotate_wrap() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            deque.rotate_left(4); // Same as rotate_left(1)
            assert_eq!(deque.to_vec(), vec![2, 3, 1]);
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_contains() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert!(deque.contains(&2));
            assert!(!deque.contains(&5));
        }

        #[test]
        fn test_search() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            assert_eq!(deque.search(&2), Some(1));
            assert_eq!(deque.search(&5), None);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut deque = Deque::from_vec(vec![1, 2, 3]);
            deque.clear();
            assert!(deque.is_empty());
        }

        #[test]
        fn test_iter() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = deque.iter().cloned().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }

        #[test]
        fn test_from_iter() {
            let deque: Deque<i32> = (1..=5).collect();
            assert_eq!(deque.len(), 5);
            assert_eq!(deque.front(), Some(&1));
            assert_eq!(deque.back(), Some(&5));
        }

        #[test]
        fn test_into_iter() {
            let deque = Deque::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = deque.into_iter().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }

        #[test]
        fn test_swap() {
            let mut deque = Deque::from_vec(vec![1, 2, 3, 4, 5]);
            deque.swap(0, 4);
            assert_eq!(deque.to_vec(), vec![5, 2, 3, 4, 1]);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_empty_operations() {
            let mut deque: Deque<i32> = Deque::new();
            assert_eq!(deque.pop_front(), None);
            assert_eq!(deque.pop_back(), None);
            assert_eq!(deque.front(), None);
            assert_eq!(deque.back(), None);
            assert!(!deque.contains(&1));
        }

        #[test]
        fn test_single_element() {
            let mut deque = Deque::new();
            deque.push_back(42);
            assert_eq!(deque.front(), Some(&42));
            assert_eq!(deque.back(), Some(&42));
            assert_eq!(deque.pop_front(), Some(42));
            assert!(deque.is_empty());
        }

        #[test]
        fn test_large_deque() {
            let mut deque = Deque::new();
            for i in 0..10000 {
                deque.push_back(i);
            }
            assert_eq!(deque.len(), 10000);
            assert_eq!(deque.front(), Some(&0));
            assert_eq!(deque.back(), Some(&9999));
        }

        #[test]
        fn test_alternating_ends() {
            let mut deque = Deque::new();
            for i in 0..100 {
                if i % 2 == 0 {
                    deque.push_front(i);
                } else {
                    deque.push_back(i);
                }
            }
            assert_eq!(deque.len(), 100);
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let d1 = Deque::from_vec(vec![1, 2, 3]);
            let d2 = Deque::from_vec(vec![1, 2, 3]);
            let d3 = Deque::from_vec(vec![1, 2, 4]);
            assert_eq!(d1, d2);
            assert_ne!(d1, d3);
        }
    }
}
