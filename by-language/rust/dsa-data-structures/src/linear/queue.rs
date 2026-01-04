//! Queue (FIFO - First In, First Out)
//!
//! A queue is a linear data structure that follows the FIFO principle.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Queue Structure:                                                          ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
//! ║  │  1  │  2  │  3  │  4  │  5  │                                           ║
//! ║  └─────┴─────┴─────┴─────┴─────┘                                           ║
//! ║    ▲                       ▲                                               ║
//! ║  front                   rear                                              ║
//! ║  (dequeue)             (enqueue)                                           ║
//! ║                                                                            ║
//! ║  Enqueue Operation:                                                        ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┬─────┐                                     ║
//! ║  │  1  │  2  │  3  │  4  │  5  │ NEW │ ◀── enqueue(6)                       ║
//! ║  └─────┴─────┴─────┴─────┴─────┴─────┘                                     ║
//! ║                                                                            ║
//! ║  Dequeue Operation:                                                        ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
//! ║  │  X  │  2  │  3  │  4  │  5  │                                           ║
//! ║  └─────┴─────┴─────┴─────┴─────┘                                           ║
//! ║    ▲                                                                       ║
//! ║  dequeue() returns 1, new front is 2                                       ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation | Average | Worst | Space |
//! |-----------|---------|-------|-------|
//! | Enqueue   | O(1)*   | O(n)  | O(1)  |
//! | Dequeue   | O(1)*   | O(n)  | O(1)  |
//! | Peek      | O(1)    | O(1)  | O(1)  |
//! | Search    | O(n)    | O(n)  | O(1)  |
//! | Is Empty  | O(1)    | O(1)  | O(1)  |
//!
//! *Amortized O(1) using VecDeque
//!
//! ## LeetCode Problems
//!
//! - [#225 Implement Stack using Queues](https://leetcode.com/problems/implement-stack-using-queues/)
//! - [#622 Design Circular Queue](https://leetcode.com/problems/design-circular-queue/)
//! - [#933 Number of Recent Calls](https://leetcode.com/problems/number-of-recent-calls/)
//! - [#346 Moving Average from Data Stream](https://leetcode.com/problems/moving-average-from-data-stream/)
//! - [#102 Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/)
//! - [#994 Rotting Oranges](https://leetcode.com/problems/rotting-oranges/)
//!
//! ## Use Cases
//!
//! - BFS (Breadth-First Search)
//! - Task scheduling
//! - Print job spooling
//! - Message queues
//! - Request handling (web servers)
//! - Buffering (I/O operations)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::Queue;
//!
//! let mut queue = Queue::new();
//! queue.enqueue(1);
//! queue.enqueue(2);
//! queue.enqueue(3);
//!
//! assert_eq!(queue.len(), 3);
//! assert_eq!(queue.peek(), Some(&1));
//! assert_eq!(queue.dequeue(), Some(1));
//! assert_eq!(queue.dequeue(), Some(2));
//! ```

use alloc::collections::VecDeque;
use alloc::vec::Vec;

use dsa_core::{Container, Searchable};

/// A FIFO (First In, First Out) queue implementation.
///
/// This implementation uses `VecDeque` as the underlying storage,
/// providing amortized O(1) operations at both ends.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Queue<T> {
    data: VecDeque<T>,
}

impl<T> Queue<T> {
    /// Creates a new empty queue.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Queue;
    ///
    /// let queue: Queue<i32> = Queue::new();
    /// assert!(queue.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Queue {
            data: VecDeque::new(),
        }
    }

    /// Creates a new queue with the specified capacity.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn with_capacity(capacity: usize) -> Self {
        Queue {
            data: VecDeque::with_capacity(capacity),
        }
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

    /// Adds an element to the back of the queue.
    ///
    /// # Time Complexity
    /// O(1) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Queue;
    ///
    /// let mut queue = Queue::new();
    /// queue.enqueue(10);
    /// queue.enqueue(20);
    /// assert_eq!(queue.peek(), Some(&10));
    /// ```
    pub fn enqueue(&mut self, value: T) {
        self.data.push_back(value);
    }

    /// Removes and returns the front element from the queue.
    ///
    /// Returns `None` if the queue is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Queue;
    ///
    /// let mut queue = Queue::new();
    /// queue.enqueue(10);
    /// queue.enqueue(20);
    /// assert_eq!(queue.dequeue(), Some(10));
    /// assert_eq!(queue.dequeue(), Some(20));
    /// assert_eq!(queue.dequeue(), None);
    /// ```
    pub fn dequeue(&mut self) -> Option<T> {
        self.data.pop_front()
    }

    /// Returns a reference to the front element without removing it.
    ///
    /// Returns `None` if the queue is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Queue;
    ///
    /// let mut queue = Queue::new();
    /// queue.enqueue(10);
    /// assert_eq!(queue.peek(), Some(&10));
    /// assert_eq!(queue.len(), 1); // Element not removed
    /// ```
    #[must_use]
    pub fn peek(&self) -> Option<&T> {
        self.data.front()
    }

    /// Returns a mutable reference to the front element without removing it.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.data.front_mut()
    }

    /// Returns a reference to the back element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn back(&self) -> Option<&T> {
        self.data.back()
    }

    /// Clears the queue, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Returns an iterator over the queue from front to back.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter()
    }

    /// Converts the queue to a `Vec` (front to back).
    #[must_use]
    pub fn to_vec(&self) -> Vec<T>
    where
        T: Clone,
    {
        self.data.iter().cloned().collect()
    }

    /// Creates a queue from a `Vec` (first element is front).
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        Queue {
            data: vec.into_iter().collect(),
        }
    }
}

impl<T: PartialEq> Queue<T> {
    /// Returns `true` if the queue contains the specified value.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn contains(&self, value: &T) -> bool {
        self.data.contains(value)
    }
}

impl<T> Container for Queue<T> {
    fn len(&self) -> usize {
        self.data.len()
    }
}

impl<T: PartialEq> Searchable<T> for Queue<T> {
    fn search(&self, value: &T) -> Option<usize> {
        self.data.iter().position(|x| x == value)
    }
}

impl<T> Default for Queue<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> FromIterator<T> for Queue<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        Queue {
            data: iter.into_iter().collect(),
        }
    }
}

impl<T> IntoIterator for Queue<T> {
    type Item = T;
    type IntoIter = alloc::collections::vec_deque::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.into_iter()
    }
}

impl<'a, T> IntoIterator for &'a Queue<T> {
    type Item = &'a T;
    type IntoIter = alloc::collections::vec_deque::Iter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.iter()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let queue: Queue<i32> = Queue::new();
            assert!(queue.is_empty());
            assert_eq!(queue.len(), 0);
        }

        #[test]
        fn test_with_capacity() {
            let queue: Queue<i32> = Queue::with_capacity(10);
            assert!(queue.is_empty());
        }

        #[test]
        fn test_default() {
            let queue: Queue<i32> = Queue::default();
            assert!(queue.is_empty());
        }

        #[test]
        fn test_from_vec() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            assert_eq!(queue.len(), 3);
            assert_eq!(queue.peek(), Some(&1));
        }

        #[test]
        fn test_to_vec() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            assert_eq!(queue.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_clone() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            let cloned = queue.clone();
            assert_eq!(queue, cloned);
        }
    }

    mod enqueue_dequeue {
        use super::*;

        #[test]
        fn test_enqueue() {
            let mut queue = Queue::new();
            queue.enqueue(1);
            queue.enqueue(2);
            queue.enqueue(3);
            assert_eq!(queue.len(), 3);
            assert_eq!(queue.peek(), Some(&1));
        }

        #[test]
        fn test_dequeue() {
            let mut queue = Queue::from_vec(vec![1, 2, 3]);
            assert_eq!(queue.dequeue(), Some(1));
            assert_eq!(queue.dequeue(), Some(2));
            assert_eq!(queue.dequeue(), Some(3));
            assert_eq!(queue.dequeue(), None);
        }

        #[test]
        fn test_fifo_order() {
            let mut queue = Queue::new();
            queue.enqueue(1);
            queue.enqueue(2);
            queue.enqueue(3);

            // Should come out in same order (FIFO)
            assert_eq!(queue.dequeue(), Some(1));
            assert_eq!(queue.dequeue(), Some(2));
            assert_eq!(queue.dequeue(), Some(3));
        }
    }

    mod peek {
        use super::*;

        #[test]
        fn test_peek() {
            let mut queue = Queue::new();
            assert_eq!(queue.peek(), None);

            queue.enqueue(10);
            assert_eq!(queue.peek(), Some(&10));
            assert_eq!(queue.len(), 1); // Not removed

            queue.enqueue(20);
            assert_eq!(queue.peek(), Some(&10)); // Still front element
        }

        #[test]
        fn test_peek_mut() {
            let mut queue = Queue::from_vec(vec![1, 2, 3]);
            if let Some(front) = queue.peek_mut() {
                *front = 10;
            }
            assert_eq!(queue.peek(), Some(&10));
        }

        #[test]
        fn test_back() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            assert_eq!(queue.back(), Some(&3));
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_contains() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            assert!(queue.contains(&2));
            assert!(!queue.contains(&5));
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut queue = Queue::from_vec(vec![1, 2, 3]);
            queue.clear();
            assert!(queue.is_empty());
        }

        #[test]
        fn test_iter() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = queue.iter().cloned().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }

        #[test]
        fn test_from_iter() {
            let queue: Queue<i32> = (1..=5).collect();
            assert_eq!(queue.len(), 5);
            assert_eq!(queue.peek(), Some(&1));
        }

        #[test]
        fn test_into_iter() {
            let queue = Queue::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = queue.into_iter().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_empty_operations() {
            let mut queue: Queue<i32> = Queue::new();
            assert_eq!(queue.dequeue(), None);
            assert_eq!(queue.peek(), None);
            assert!(!queue.contains(&1));
        }

        #[test]
        fn test_single_element() {
            let mut queue = Queue::new();
            queue.enqueue(42);
            assert_eq!(queue.peek(), Some(&42));
            assert_eq!(queue.dequeue(), Some(42));
            assert!(queue.is_empty());
        }

        #[test]
        fn test_large_queue() {
            let mut queue = Queue::new();
            for i in 0..10000 {
                queue.enqueue(i);
            }
            assert_eq!(queue.len(), 10000);
            assert_eq!(queue.peek(), Some(&0));
        }

        #[test]
        fn test_interleaved_operations() {
            let mut queue = Queue::new();
            queue.enqueue(1);
            queue.enqueue(2);
            assert_eq!(queue.dequeue(), Some(1));
            queue.enqueue(3);
            assert_eq!(queue.dequeue(), Some(2));
            assert_eq!(queue.dequeue(), Some(3));
            assert!(queue.is_empty());
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let q1 = Queue::from_vec(vec![1, 2, 3]);
            let q2 = Queue::from_vec(vec![1, 2, 3]);
            let q3 = Queue::from_vec(vec![1, 2, 4]);
            assert_eq!(q1, q2);
            assert_ne!(q1, q3);
        }
    }
}
