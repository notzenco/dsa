//! Stack (LIFO - Last In, First Out)
//!
//! A stack is a linear data structure that follows the LIFO principle.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Stack Structure:                                                          ║
//! ║  ┌─────────┐                                                               ║
//! ║  │    5    │ ◀── top (push/pop here)                                       ║
//! ║  ├─────────┤                                                               ║
//! ║  │    4    │                                                               ║
//! ║  ├─────────┤                                                               ║
//! ║  │    3    │                                                               ║
//! ║  ├─────────┤                                                               ║
//! ║  │    2    │                                                               ║
//! ║  ├─────────┤                                                               ║
//! ║  │    1    │ ◀── bottom                                                    ║
//! ║  └─────────┘                                                               ║
//! ║                                                                            ║
//! ║  Push Operation:              Pop Operation:                               ║
//! ║  ┌─────────┐                  ┌─────────┐                                  ║
//! ║  │   NEW   │ ◀── push(6)      │    X    │ ──▶ pop() returns 5              ║
//! ║  ├─────────┤                  ├─────────┤                                  ║
//! ║  │    5    │                  │    4    │ ◀── new top                      ║
//! ║  ├─────────┤                  ├─────────┤                                  ║
//! ║  │   ...   │                  │   ...   │                                  ║
//! ║  └─────────┘                  └─────────┘                                  ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation | Average | Worst | Space |
//! |-----------|---------|-------|-------|
//! | Push      | O(1)*   | O(n)  | O(1)  |
//! | Pop       | O(1)    | O(1)  | O(1)  |
//! | Peek      | O(1)    | O(1)  | O(1)  |
//! | Search    | O(n)    | O(n)  | O(1)  |
//! | Is Empty  | O(1)    | O(1)  | O(1)  |
//!
//! *Amortized O(1) due to occasional resizing
//!
//! ## LeetCode Problems
//!
//! - [#20 Valid Parentheses](https://leetcode.com/problems/valid-parentheses/)
//! - [#155 Min Stack](https://leetcode.com/problems/min-stack/)
//! - [#232 Implement Queue using Stacks](https://leetcode.com/problems/implement-queue-using-stacks/)
//! - [#150 Evaluate Reverse Polish Notation](https://leetcode.com/problems/evaluate-reverse-polish-notation/)
//! - [#739 Daily Temperatures](https://leetcode.com/problems/daily-temperatures/)
//! - [#84 Largest Rectangle in Histogram](https://leetcode.com/problems/largest-rectangle-in-histogram/)
//! - [#71 Simplify Path](https://leetcode.com/problems/simplify-path/)
//! - [#394 Decode String](https://leetcode.com/problems/decode-string/)
//!
//! ## Use Cases
//!
//! - Function call stack (recursion)
//! - Expression evaluation and parsing
//! - Undo/redo operations
//! - Backtracking algorithms
//! - Browser history (back button)
//! - Balanced parentheses checking
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::Stack;
//!
//! let mut stack = Stack::new();
//! stack.push(1);
//! stack.push(2);
//! stack.push(3);
//!
//! assert_eq!(stack.len(), 3);
//! assert_eq!(stack.peek(), Some(&3));
//! assert_eq!(stack.pop(), Some(3));
//! assert_eq!(stack.pop(), Some(2));
//! ```

use alloc::vec::Vec;

use dsa_core::{Container, DsaError, Result, Searchable};

/// A LIFO (Last In, First Out) stack implementation.
///
/// This implementation uses a `Vec` as the underlying storage,
/// providing amortized O(1) push and O(1) pop operations.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Stack<T> {
    data: Vec<T>,
}

impl<T> Stack<T> {
    /// Creates a new empty stack.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Stack;
    ///
    /// let stack: Stack<i32> = Stack::new();
    /// assert!(stack.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Stack { data: Vec::new() }
    }

    /// Creates a new stack with the specified capacity.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn with_capacity(capacity: usize) -> Self {
        Stack {
            data: Vec::with_capacity(capacity),
        }
    }

    /// Returns the number of elements in the stack.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns `true` if the stack contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Pushes an element onto the top of the stack.
    ///
    /// # Time Complexity
    /// O(1) amortized, O(n) worst case when resizing.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Stack;
    ///
    /// let mut stack = Stack::new();
    /// stack.push(10);
    /// stack.push(20);
    /// assert_eq!(stack.peek(), Some(&20));
    /// ```
    pub fn push(&mut self, value: T) {
        self.data.push(value);
    }

    /// Removes and returns the top element from the stack.
    ///
    /// Returns `None` if the stack is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Stack;
    ///
    /// let mut stack = Stack::new();
    /// stack.push(10);
    /// stack.push(20);
    /// assert_eq!(stack.pop(), Some(20));
    /// assert_eq!(stack.pop(), Some(10));
    /// assert_eq!(stack.pop(), None);
    /// ```
    pub fn pop(&mut self) -> Option<T> {
        self.data.pop()
    }

    /// Returns a reference to the top element without removing it.
    ///
    /// Returns `None` if the stack is empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Stack;
    ///
    /// let mut stack = Stack::new();
    /// stack.push(10);
    /// assert_eq!(stack.peek(), Some(&10));
    /// assert_eq!(stack.len(), 1); // Element not removed
    /// ```
    #[must_use]
    pub fn peek(&self) -> Option<&T> {
        self.data.last()
    }

    /// Returns a mutable reference to the top element without removing it.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.data.last_mut()
    }

    /// Clears the stack, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Returns an iterator over the stack from bottom to top.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter()
    }

    /// Returns an iterator over the stack from top to bottom.
    pub fn iter_from_top(&self) -> impl Iterator<Item = &T> {
        self.data.iter().rev()
    }

    /// Converts the stack to a `Vec` (bottom to top).
    #[must_use]
    pub fn to_vec(&self) -> Vec<T>
    where
        T: Clone,
    {
        self.data.clone()
    }

    /// Creates a stack from a `Vec` (first element is bottom).
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        Stack { data: vec }
    }
}

impl<T: PartialEq> Stack<T> {
    /// Searches for an element and returns its position from the top (1-indexed).
    ///
    /// Returns `None` if the element is not found.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::Stack;
    ///
    /// let stack = Stack::from_vec(vec![1, 2, 3, 4, 5]);
    /// assert_eq!(stack.search(&5), Some(1)); // Top element
    /// assert_eq!(stack.search(&1), Some(5)); // Bottom element
    /// assert_eq!(stack.search(&10), None);   // Not found
    /// ```
    #[must_use]
    pub fn search(&self, value: &T) -> Option<usize> {
        self.data
            .iter()
            .rev()
            .position(|x| x == value)
            .map(|pos| pos + 1)
    }

    /// Returns `true` if the stack contains the specified value.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn contains(&self, value: &T) -> bool {
        self.data.contains(value)
    }
}

impl<T> Container for Stack<T> {
    fn len(&self) -> usize {
        self.data.len()
    }
}

impl<T: PartialEq> Searchable<T> for Stack<T> {
    fn search(&self, value: &T) -> Option<usize> {
        // Returns 0-indexed position from bottom for trait compatibility
        self.data.iter().position(|x| x == value)
    }
}

impl<T> Default for Stack<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> FromIterator<T> for Stack<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        Stack {
            data: iter.into_iter().collect(),
        }
    }
}

impl<T> IntoIterator for Stack<T> {
    type Item = T;
    type IntoIter = alloc::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.into_iter()
    }
}

impl<'a, T> IntoIterator for &'a Stack<T> {
    type Item = &'a T;
    type IntoIter = core::slice::Iter<'a, T>;

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
            let stack: Stack<i32> = Stack::new();
            assert!(stack.is_empty());
            assert_eq!(stack.len(), 0);
        }

        #[test]
        fn test_with_capacity() {
            let stack: Stack<i32> = Stack::with_capacity(10);
            assert!(stack.is_empty());
        }

        #[test]
        fn test_default() {
            let stack: Stack<i32> = Stack::default();
            assert!(stack.is_empty());
        }

        #[test]
        fn test_from_vec() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            assert_eq!(stack.len(), 3);
            assert_eq!(stack.peek(), Some(&3));
        }

        #[test]
        fn test_to_vec() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            assert_eq!(stack.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_clone() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            let cloned = stack.clone();
            assert_eq!(stack, cloned);
        }
    }

    mod push_pop {
        use super::*;

        #[test]
        fn test_push() {
            let mut stack = Stack::new();
            stack.push(1);
            stack.push(2);
            stack.push(3);
            assert_eq!(stack.len(), 3);
            assert_eq!(stack.peek(), Some(&3));
        }

        #[test]
        fn test_pop() {
            let mut stack = Stack::from_vec(vec![1, 2, 3]);
            assert_eq!(stack.pop(), Some(3));
            assert_eq!(stack.pop(), Some(2));
            assert_eq!(stack.pop(), Some(1));
            assert_eq!(stack.pop(), None);
        }

        #[test]
        fn test_lifo_order() {
            let mut stack = Stack::new();
            stack.push(1);
            stack.push(2);
            stack.push(3);

            // Should come out in reverse order (LIFO)
            assert_eq!(stack.pop(), Some(3));
            assert_eq!(stack.pop(), Some(2));
            assert_eq!(stack.pop(), Some(1));
        }
    }

    mod peek {
        use super::*;

        #[test]
        fn test_peek() {
            let mut stack = Stack::new();
            assert_eq!(stack.peek(), None);

            stack.push(10);
            assert_eq!(stack.peek(), Some(&10));
            assert_eq!(stack.len(), 1); // Not removed

            stack.push(20);
            assert_eq!(stack.peek(), Some(&20));
        }

        #[test]
        fn test_peek_mut() {
            let mut stack = Stack::from_vec(vec![1, 2, 3]);
            if let Some(top) = stack.peek_mut() {
                *top = 30;
            }
            assert_eq!(stack.peek(), Some(&30));
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_search() {
            let stack = Stack::from_vec(vec![1, 2, 3, 4, 5]);
            assert_eq!(stack.search(&5), Some(1)); // Top
            assert_eq!(stack.search(&4), Some(2));
            assert_eq!(stack.search(&1), Some(5)); // Bottom
            assert_eq!(stack.search(&10), None);
        }

        #[test]
        fn test_contains() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            assert!(stack.contains(&2));
            assert!(!stack.contains(&5));
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut stack = Stack::from_vec(vec![1, 2, 3]);
            stack.clear();
            assert!(stack.is_empty());
        }

        #[test]
        fn test_iter() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = stack.iter().cloned().collect();
            assert_eq!(collected, vec![1, 2, 3]); // Bottom to top
        }

        #[test]
        fn test_iter_from_top() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = stack.iter_from_top().cloned().collect();
            assert_eq!(collected, vec![3, 2, 1]); // Top to bottom
        }

        #[test]
        fn test_from_iter() {
            let stack: Stack<i32> = (1..=5).collect();
            assert_eq!(stack.len(), 5);
            assert_eq!(stack.peek(), Some(&5));
        }

        #[test]
        fn test_into_iter() {
            let stack = Stack::from_vec(vec![1, 2, 3]);
            let collected: Vec<_> = stack.into_iter().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_empty_operations() {
            let mut stack: Stack<i32> = Stack::new();
            assert_eq!(stack.pop(), None);
            assert_eq!(stack.peek(), None);
            assert!(!stack.contains(&1));
        }

        #[test]
        fn test_single_element() {
            let mut stack = Stack::new();
            stack.push(42);
            assert_eq!(stack.peek(), Some(&42));
            assert_eq!(stack.pop(), Some(42));
            assert!(stack.is_empty());
        }

        #[test]
        fn test_large_stack() {
            let mut stack = Stack::new();
            for i in 0..10000 {
                stack.push(i);
            }
            assert_eq!(stack.len(), 10000);
            assert_eq!(stack.peek(), Some(&9999));
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let s1 = Stack::from_vec(vec![1, 2, 3]);
            let s2 = Stack::from_vec(vec![1, 2, 3]);
            let s3 = Stack::from_vec(vec![1, 2, 4]);
            assert_eq!(s1, s2);
            assert_ne!(s1, s3);
        }
    }
}
