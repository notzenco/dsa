//! Doubly Linked List
//!
//! A linked list where each node has pointers to both next and previous nodes.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Structure:                                                                ║
//! ║       ┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐ ║
//! ║  None◀│prev│data│next│───▶◀───│prev│data│next│───▶◀───│prev│data│next│▶None║
//! ║       └──────────────────┘    └──────────────────┘    └──────────────────┘ ║
//! ║            ▲                                               ▲               ║
//! ║           head                                            tail             ║
//! ║                                                                            ║
//! ║  Node Structure:                                                           ║
//! ║  ┌─────────────────────────────┐                                           ║
//! ║  │  ┌──────┬───────┬───────┐   │                                           ║
//! ║  │  │ prev │ data  │ next  │   │                                           ║
//! ║  │  └──┼───┴───────┴───┼───┘   │                                           ║
//! ║  │     ▼               ▼       │                                           ║
//! ║  │  (previous)     (next node) │                                           ║
//! ║  └─────────────────────────────┘                                           ║
//! ║                                                                            ║
//! ║  Insert in Middle:                                                         ║
//! ║  Before: [A]◀──▶[C]                                                        ║
//! ║  After:  [A]◀──▶[B]◀──▶[C]                                                 ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation           | Average     | Worst       | Space       |
//! |---------------------|-------------|-------------|-------------|
//! | Access by index     | O(n)        | O(n)        | O(1)        |
//! | Search              | O(n)        | O(n)        | O(1)        |
//! | Insert at head      | O(1)        | O(1)        | O(1)        |
//! | Insert at tail      | O(1)        | O(1)        | O(1)        |
//! | Insert at index     | O(n)        | O(n)        | O(1)        |
//! | Delete at head      | O(1)        | O(1)        | O(1)        |
//! | Delete at tail      | O(1)        | O(1)        | O(1)        |
//! | Delete at index     | O(n)        | O(n)        | O(1)        |
//!
//! ## LeetCode Problems
//!
//! - [#146 LRU Cache](https://leetcode.com/problems/lru-cache/) (uses doubly linked list)
//! - [#460 LFU Cache](https://leetcode.com/problems/lfu-cache/)
//! - [#432 All O(1) Data Structure](https://leetcode.com/problems/all-oone-data-structure/)
//! - [#426 Convert BST to Sorted Doubly Linked List](https://leetcode.com/problems/convert-binary-search-tree-to-sorted-doubly-linked-list/)
//!
//! ## Use Cases
//!
//! - LRU/LFU cache implementations (O(1) removal from middle)
//! - Browser forward/back navigation
//! - Undo/redo functionality
//! - Music player (next/previous song)
//! - Text editors (cursor movement)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::DoublyLinkedList;
//!
//! let mut list = DoublyLinkedList::new();
//! list.push_back(10);
//! list.push_back(20);
//! list.push_front(5);
//!
//! assert_eq!(list.len(), 3);
//! assert_eq!(list.front(), Some(5));
//! assert_eq!(list.back(), Some(20));
//! assert_eq!(list.pop_back(), Some(20));
//! ```

use alloc::rc::Rc;
use alloc::vec::Vec;
use core::cell::RefCell;

use dsa_core::{Container, DsaError, Result, Searchable};

/// A node in a doubly linked list.
struct Node<T> {
    data: T,
    prev: Option<Rc<RefCell<Node<T>>>>,
    next: Option<Rc<RefCell<Node<T>>>>,
}

impl<T> Node<T> {
    fn new(data: T) -> Rc<RefCell<Self>> {
        Rc::new(RefCell::new(Node {
            data,
            prev: None,
            next: None,
        }))
    }
}

/// A doubly linked list with head and tail pointers.
///
/// This implementation uses `Rc<RefCell<Node<T>>>` for shared ownership
/// with interior mutability, allowing O(1) operations at both ends.
pub struct DoublyLinkedList<T> {
    head: Option<Rc<RefCell<Node<T>>>>,
    tail: Option<Rc<RefCell<Node<T>>>>,
    len: usize,
}

impl<T> DoublyLinkedList<T> {
    /// Creates a new empty `DoublyLinkedList`.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let list: DoublyLinkedList<i32> = DoublyLinkedList::new();
    /// assert!(list.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        DoublyLinkedList {
            head: None,
            tail: None,
            len: 0,
        }
    }

    /// Returns the number of elements in the list.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.len
    }

    /// Returns `true` if the list contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.head.is_none()
    }

    /// Adds an element to the front of the list.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_front(10);
    /// list.push_front(5);
    /// assert_eq!(list.front(), Some(5));
    /// ```
    pub fn push_front(&mut self, data: T) {
        let new_node = Node::new(data);

        match self.head.take() {
            Some(old_head) => {
                old_head.borrow_mut().prev = Some(Rc::clone(&new_node));
                new_node.borrow_mut().next = Some(old_head);
                self.head = Some(new_node);
            }
            None => {
                self.tail = Some(Rc::clone(&new_node));
                self.head = Some(new_node);
            }
        }
        self.len += 1;
    }

    /// Adds an element to the back of the list.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.back(), Some(20));
    /// ```
    pub fn push_back(&mut self, data: T) {
        let new_node = Node::new(data);

        match self.tail.take() {
            Some(old_tail) => {
                old_tail.borrow_mut().next = Some(Rc::clone(&new_node));
                new_node.borrow_mut().prev = Some(old_tail);
                self.tail = Some(new_node);
            }
            None => {
                self.head = Some(Rc::clone(&new_node));
                self.tail = Some(new_node);
            }
        }
        self.len += 1;
    }

    /// Removes and returns the element from the front of the list.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.pop_front(), Some(10));
    /// assert_eq!(list.pop_front(), Some(20));
    /// assert_eq!(list.pop_front(), None);
    /// ```
    pub fn pop_front(&mut self) -> Option<T> {
        self.head.take().map(|old_head| {
            match old_head.borrow_mut().next.take() {
                Some(new_head) => {
                    new_head.borrow_mut().prev = None;
                    self.head = Some(new_head);
                }
                None => {
                    self.tail = None;
                }
            }
            self.len -= 1;
            Rc::try_unwrap(old_head).ok().unwrap().into_inner().data
        })
    }

    /// Removes and returns the element from the back of the list.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.pop_back(), Some(20));
    /// assert_eq!(list.pop_back(), Some(10));
    /// ```
    pub fn pop_back(&mut self) -> Option<T> {
        self.tail.take().map(|old_tail| {
            match old_tail.borrow_mut().prev.take() {
                Some(new_tail) => {
                    new_tail.borrow_mut().next = None;
                    self.tail = Some(new_tail);
                }
                None => {
                    self.head = None;
                }
            }
            self.len -= 1;
            Rc::try_unwrap(old_tail).ok().unwrap().into_inner().data
        })
    }

    /// Returns the front element without removing it.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn front(&self) -> Option<T>
    where
        T: Clone,
    {
        self.head.as_ref().map(|node| node.borrow().data.clone())
    }

    /// Returns the back element without removing it.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn back(&self) -> Option<T>
    where
        T: Clone,
    {
        self.tail.as_ref().map(|node| node.borrow().data.clone())
    }

    /// Gets a clone of the element at the specified index.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// list.push_back(30);
    /// assert_eq!(list.get(1), Some(20));
    /// assert_eq!(list.get(5), None);
    /// ```
    #[must_use]
    pub fn get(&self, index: usize) -> Option<T>
    where
        T: Clone,
    {
        if index >= self.len {
            return None;
        }

        // Optimize by traversing from closer end
        if index < self.len / 2 {
            // Traverse from head
            let mut current = self.head.clone();
            for _ in 0..index {
                let next = current.as_ref()?.borrow().next.clone();
                current = next;
            }
            current.map(|node| node.borrow().data.clone())
        } else {
            // Traverse from tail
            let mut current = self.tail.clone();
            for _ in 0..(self.len - 1 - index) {
                let prev = current.as_ref()?.borrow().prev.clone();
                current = prev;
            }
            current.map(|node| node.borrow().data.clone())
        }
    }

    /// Inserts an element at the specified index.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Errors
    ///
    /// Returns `DsaError::IndexOutOfBounds` if `index > len`.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(30);
    /// list.insert(1, 20).unwrap();
    /// assert_eq!(list.get(1), Some(20));
    /// ```
    pub fn insert(&mut self, index: usize, data: T) -> Result<()> {
        if index > self.len {
            return Err(DsaError::IndexOutOfBounds {
                index,
                size: self.len,
            });
        }

        if index == 0 {
            self.push_front(data);
            return Ok(());
        }

        if index == self.len {
            self.push_back(data);
            return Ok(());
        }

        // Find the node at index
        let mut current = self.head.clone();
        for _ in 0..index {
            let next = current.as_ref().unwrap().borrow().next.clone();
            current = next;
        }

        let current_node = current.unwrap();
        let prev_node = current_node.borrow().prev.clone().unwrap();

        let new_node = Node::new(data);
        new_node.borrow_mut().prev = Some(Rc::clone(&prev_node));
        new_node.borrow_mut().next = Some(Rc::clone(&current_node));
        prev_node.borrow_mut().next = Some(Rc::clone(&new_node));
        current_node.borrow_mut().prev = Some(new_node);

        self.len += 1;
        Ok(())
    }

    /// Removes and returns the element at the specified index.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Errors
    ///
    /// Returns `DsaError::IndexOutOfBounds` if the index is out of bounds.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// list.push_back(30);
    /// assert_eq!(list.remove(1).unwrap(), 20);
    /// assert_eq!(list.len(), 2);
    /// ```
    pub fn remove(&mut self, index: usize) -> Result<T> {
        if index >= self.len {
            return Err(DsaError::IndexOutOfBounds {
                index,
                size: self.len,
            });
        }

        if index == 0 {
            return self.pop_front().ok_or(DsaError::EmptyContainer);
        }

        if index == self.len - 1 {
            return self.pop_back().ok_or(DsaError::EmptyContainer);
        }

        // Find the node at index
        let mut current = self.head.clone();
        for _ in 0..index {
            let next = current.as_ref().unwrap().borrow().next.clone();
            current = next;
        }

        let current_node = current.unwrap();
        let prev_node = current_node.borrow().prev.clone().unwrap();
        let next_node = current_node.borrow().next.clone().unwrap();

        prev_node.borrow_mut().next = Some(Rc::clone(&next_node));
        next_node.borrow_mut().prev = Some(prev_node);

        // Clear references to allow Rc to drop
        current_node.borrow_mut().prev = None;
        current_node.borrow_mut().next = None;

        self.len -= 1;
        Ok(Rc::try_unwrap(current_node)
            .ok()
            .unwrap()
            .into_inner()
            .data)
    }

    /// Clears the list, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        // Break circular references by clearing all links
        while self.pop_front().is_some() {}
    }

    /// Reverses the list in place.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DoublyLinkedList;
    ///
    /// let mut list = DoublyLinkedList::new();
    /// list.push_back(1);
    /// list.push_back(2);
    /// list.push_back(3);
    /// list.reverse();
    /// assert_eq!(list.front(), Some(3));
    /// ```
    pub fn reverse(&mut self)
    where
        T: Clone,
    {
        let mut current = self.head.clone();

        while let Some(node) = current {
            let next = node.borrow().next.clone();
            let prev = node.borrow().prev.clone();

            node.borrow_mut().next = prev;
            node.borrow_mut().prev = next.clone();

            current = next;
        }

        core::mem::swap(&mut self.head, &mut self.tail);
    }

    /// Converts the list to a `Vec`.
    #[must_use]
    pub fn to_vec(&self) -> Vec<T>
    where
        T: Clone,
    {
        let mut result = Vec::with_capacity(self.len);
        let mut current = self.head.clone();

        while let Some(node) = current {
            result.push(node.borrow().data.clone());
            current = node.borrow().next.clone();
        }

        result
    }

    /// Creates a `DoublyLinkedList` from a `Vec`.
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        let mut list = DoublyLinkedList::new();
        for item in vec {
            list.push_back(item);
        }
        list
    }
}

impl<T: PartialEq + Clone> DoublyLinkedList<T> {
    /// Finds the index of the first occurrence of a value.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn find(&self, value: &T) -> Option<usize> {
        let mut current = self.head.clone();
        let mut index = 0;

        while let Some(node) = current {
            if &node.borrow().data == value {
                return Some(index);
            }
            current = node.borrow().next.clone();
            index += 1;
        }

        None
    }

    /// Removes the first occurrence of a value.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Returns
    ///
    /// `true` if the value was found and removed, `false` otherwise.
    pub fn remove_value(&mut self, value: &T) -> bool {
        if let Some(index) = self.find(value) {
            self.remove(index).is_ok()
        } else {
            false
        }
    }
}

impl<T> Container for DoublyLinkedList<T> {
    fn len(&self) -> usize {
        self.len
    }
}

impl<T: PartialEq + Clone> Searchable<T> for DoublyLinkedList<T> {
    fn search(&self, value: &T) -> Option<usize> {
        self.find(value)
    }
}

impl<T> Default for DoublyLinkedList<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Clone> Clone for DoublyLinkedList<T> {
    fn clone(&self) -> Self {
        Self::from_vec(self.to_vec())
    }
}

impl<T: PartialEq + Clone> PartialEq for DoublyLinkedList<T> {
    fn eq(&self, other: &Self) -> bool {
        if self.len != other.len {
            return false;
        }
        self.to_vec() == other.to_vec()
    }
}

impl<T: Eq + Clone> Eq for DoublyLinkedList<T> {}

impl<T> Drop for DoublyLinkedList<T> {
    fn drop(&mut self) {
        self.clear();
    }
}

impl<T> FromIterator<T> for DoublyLinkedList<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let mut list = DoublyLinkedList::new();
        for item in iter {
            list.push_back(item);
        }
        list
    }
}

// Debug implementation
impl<T: core::fmt::Debug + Clone> core::fmt::Debug for DoublyLinkedList<T> {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        f.debug_list().entries(self.to_vec().iter()).finish()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let list: DoublyLinkedList<i32> = DoublyLinkedList::new();
            assert!(list.is_empty());
            assert_eq!(list.len(), 0);
        }

        #[test]
        fn test_default() {
            let list: DoublyLinkedList<i32> = DoublyLinkedList::default();
            assert!(list.is_empty());
        }

        #[test]
        fn test_from_vec() {
            let list = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            assert_eq!(list.len(), 3);
            assert_eq!(list.front(), Some(1));
        }

        #[test]
        fn test_to_vec() {
            let list = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            assert_eq!(list.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_clone() {
            let list = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            let cloned = list.clone();
            assert_eq!(list.to_vec(), cloned.to_vec());
        }
    }

    mod push_pop {
        use super::*;

        #[test]
        fn test_push_front() {
            let mut list = DoublyLinkedList::new();
            list.push_front(30);
            list.push_front(20);
            list.push_front(10);
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_push_back() {
            let mut list = DoublyLinkedList::new();
            list.push_back(10);
            list.push_back(20);
            list.push_back(30);
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_pop_front() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.pop_front(), Some(10));
            assert_eq!(list.pop_front(), Some(20));
            assert_eq!(list.pop_front(), Some(30));
            assert_eq!(list.pop_front(), None);
        }

        #[test]
        fn test_pop_back() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.pop_back(), Some(30));
            assert_eq!(list.pop_back(), Some(20));
            assert_eq!(list.pop_back(), Some(10));
            assert_eq!(list.pop_back(), None);
        }

        #[test]
        fn test_mixed_push_pop() {
            let mut list = DoublyLinkedList::new();
            list.push_back(2);
            list.push_front(1);
            list.push_back(3);
            assert_eq!(list.to_vec(), vec![1, 2, 3]);
            assert_eq!(list.pop_front(), Some(1));
            assert_eq!(list.pop_back(), Some(3));
            assert_eq!(list.pop_front(), Some(2));
        }
    }

    mod access {
        use super::*;

        #[test]
        fn test_front_back() {
            let list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.front(), Some(10));
            assert_eq!(list.back(), Some(30));

            let empty: DoublyLinkedList<i32> = DoublyLinkedList::new();
            assert_eq!(empty.front(), None);
            assert_eq!(empty.back(), None);
        }

        #[test]
        fn test_get() {
            let list = DoublyLinkedList::from_vec(vec![10, 20, 30, 40, 50]);
            assert_eq!(list.get(0), Some(10));
            assert_eq!(list.get(2), Some(30));
            assert_eq!(list.get(4), Some(50));
            assert_eq!(list.get(5), None);
        }

        #[test]
        fn test_get_optimized_traversal() {
            // Test that get() traverses from the closer end
            let list = DoublyLinkedList::from_vec(vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
            // These should traverse from head
            assert_eq!(list.get(0), Some(1));
            assert_eq!(list.get(4), Some(5));
            // These should traverse from tail
            assert_eq!(list.get(5), Some(6));
            assert_eq!(list.get(9), Some(10));
        }
    }

    mod insert_remove {
        use super::*;

        #[test]
        fn test_insert() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 30]);
            assert!(list.insert(1, 20).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_at_start() {
            let mut list = DoublyLinkedList::from_vec(vec![20, 30]);
            assert!(list.insert(0, 10).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_at_end() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20]);
            assert!(list.insert(2, 30).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_out_of_bounds() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20]);
            let result = list.insert(5, 30);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_remove() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.remove(1).unwrap(), 20);
            assert_eq!(list.to_vec(), vec![10, 30]);
        }

        #[test]
        fn test_remove_first() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.remove(0).unwrap(), 10);
            assert_eq!(list.to_vec(), vec![20, 30]);
        }

        #[test]
        fn test_remove_last() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.remove(2).unwrap(), 30);
            assert_eq!(list.to_vec(), vec![10, 20]);
        }

        #[test]
        fn test_remove_out_of_bounds() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20]);
            let result = list.remove(5);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_clear() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            list.clear();
            assert!(list.is_empty());
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_find() {
            let list = DoublyLinkedList::from_vec(vec![10, 20, 30, 20]);
            assert_eq!(list.find(&20), Some(1));
            assert_eq!(list.find(&50), None);
        }

        #[test]
        fn test_contains() {
            let list = DoublyLinkedList::from_vec(vec![10, 20, 30]);
            assert!(list.contains(&20));
            assert!(!list.contains(&50));
        }

        #[test]
        fn test_remove_value() {
            let mut list = DoublyLinkedList::from_vec(vec![10, 20, 30, 20]);
            assert!(list.remove_value(&20));
            assert_eq!(list.to_vec(), vec![10, 30, 20]);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_reverse() {
            let mut list = DoublyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
            list.reverse();
            assert_eq!(list.to_vec(), vec![5, 4, 3, 2, 1]);
        }

        #[test]
        fn test_reverse_empty() {
            let mut list: DoublyLinkedList<i32> = DoublyLinkedList::new();
            list.reverse();
            assert!(list.is_empty());
        }

        #[test]
        fn test_reverse_single() {
            let mut list = DoublyLinkedList::from_vec(vec![42]);
            list.reverse();
            assert_eq!(list.to_vec(), vec![42]);
        }

        #[test]
        fn test_from_iter() {
            let list: DoublyLinkedList<i32> = (1..=5).collect();
            assert_eq!(list.to_vec(), vec![1, 2, 3, 4, 5]);
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let list1 = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            let list2 = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            let list3 = DoublyLinkedList::from_vec(vec![1, 2, 4]);
            assert_eq!(list1, list2);
            assert_ne!(list1, list3);
        }

        #[test]
        fn test_eq_different_lengths() {
            let list1 = DoublyLinkedList::from_vec(vec![1, 2, 3]);
            let list2 = DoublyLinkedList::from_vec(vec![1, 2]);
            assert_ne!(list1, list2);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let mut list = DoublyLinkedList::new();
            list.push_back(42);
            assert_eq!(list.front(), Some(42));
            assert_eq!(list.back(), Some(42));
            assert_eq!(list.pop_front(), Some(42));
            assert!(list.is_empty());
        }

        #[test]
        fn test_two_elements() {
            let mut list = DoublyLinkedList::new();
            list.push_back(1);
            list.push_back(2);
            assert_eq!(list.pop_back(), Some(2));
            assert_eq!(list.pop_front(), Some(1));
            assert!(list.is_empty());
        }

        #[test]
        fn test_large_list() {
            let mut list = DoublyLinkedList::new();
            for i in 0..1000 {
                list.push_back(i);
            }
            assert_eq!(list.len(), 1000);
            assert_eq!(list.get(500), Some(500));
        }
    }
}
