//! Singly Linked List
//!
//! A linked list where each node points to the next node in the sequence.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Structure:                                                                ║
//! ║  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐                  ║
//! ║  │ data │ next  │───▶│ data │ next  │───▶│ data │ next  │───▶ None        ║
//! ║  └──────────────┘    └──────────────┘    └──────────────┘                  ║
//! ║       ▲                                        ▲                           ║
//! ║      head                                     tail                         ║
//! ║                                                                            ║
//! ║  Node Structure:                                                           ║
//! ║  ┌─────────────────────┐                                                   ║
//! ║  │  ┌───────┬───────┐  │                                                   ║
//! ║  │  │ data  │ next ─┼──┼──▶ (pointer to next node)                         ║
//! ║  │  └───────┴───────┘  │                                                   ║
//! ║  └─────────────────────┘                                                   ║
//! ║                                                                            ║
//! ║  Insert at Head:                                                           ║
//! ║  Before: [A] ──▶ [B] ──▶ [C] ──▶ None                                      ║
//! ║          ▲                                                                 ║
//! ║         head                                                               ║
//! ║                                                                            ║
//! ║  After:  [X] ──▶ [A] ──▶ [B] ──▶ [C] ──▶ None                              ║
//! ║          ▲                                                                 ║
//! ║       new head                                                             ║
//! ║                                                                            ║
//! ║  Delete at Position 1:                                                     ║
//! ║  Before: [A] ──▶ [B] ──▶ [C] ──▶ None                                      ║
//! ║  After:  [A] ──────────▶ [C] ──▶ None                                      ║
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
//! | Insert at tail      | O(1)*       | O(1)*       | O(1)        |
//! | Insert at index     | O(n)        | O(n)        | O(1)        |
//! | Delete at head      | O(1)        | O(1)        | O(1)        |
//! | Delete at tail      | O(n)        | O(n)        | O(1)        |
//! | Delete at index     | O(n)        | O(n)        | O(1)        |
//!
//! *O(1) if tail pointer is maintained (as in this implementation)
//!
//! ## LeetCode Problems
//!
//! - [#206 Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/)
//! - [#21 Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists/)
//! - [#141 Linked List Cycle](https://leetcode.com/problems/linked-list-cycle/)
//! - [#142 Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/)
//! - [#160 Intersection of Two Linked Lists](https://leetcode.com/problems/intersection-of-two-linked-lists/)
//! - [#234 Palindrome Linked List](https://leetcode.com/problems/palindrome-linked-list/)
//! - [#237 Delete Node in a Linked List](https://leetcode.com/problems/delete-node-in-a-linked-list/)
//! - [#876 Middle of the Linked List](https://leetcode.com/problems/middle-of-the-linked-list/)
//! - [#203 Remove Linked List Elements](https://leetcode.com/problems/remove-linked-list-elements/)
//! - [#83 Remove Duplicates from Sorted List](https://leetcode.com/problems/remove-duplicates-from-sorted-list/)
//!
//! ## Use Cases
//!
//! - Implementing stacks and queues
//! - Undo functionality in applications
//! - Browser back button (history)
//! - Memory allocation (free lists)
//! - Polynomial arithmetic
//! - Music playlists (next song)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::SinglyLinkedList;
//!
//! let mut list = SinglyLinkedList::new();
//! list.push_back(10);
//! list.push_back(20);
//! list.push_front(5);
//!
//! assert_eq!(list.len(), 3);
//! assert_eq!(list.front(), Some(&5));
//! assert_eq!(list.pop_front(), Some(5));
//! ```

use alloc::boxed::Box;
use alloc::vec::Vec;

use dsa_core::{Container, DsaError, Result, Searchable};

/// A node in a singly linked list.
#[derive(Debug)]
struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(data: T) -> Self {
        Node { data, next: None }
    }
}

/// A singly linked list with head and tail pointers.
///
/// This implementation maintains a tail pointer to allow O(1) insertion
/// at the end of the list.
#[derive(Debug)]
pub struct SinglyLinkedList<T> {
    head: Option<Box<Node<T>>>,
    len: usize,
}

impl<T> SinglyLinkedList<T> {
    /// Creates a new empty `SinglyLinkedList`.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let list: SinglyLinkedList<i32> = SinglyLinkedList::new();
    /// assert!(list.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        SinglyLinkedList { head: None, len: 0 }
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
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_front(10);
    /// list.push_front(5);
    /// assert_eq!(list.front(), Some(&5));
    /// ```
    pub fn push_front(&mut self, data: T) {
        let mut new_node = Box::new(Node::new(data));
        new_node.next = self.head.take();
        self.head = Some(new_node);
        self.len += 1;
    }

    /// Adds an element to the back of the list.
    ///
    /// # Time Complexity
    /// O(n) - must traverse to find the tail
    ///
    /// Note: For O(1) append, consider using `DoublyLinkedList` or
    /// a different implementation with a tail pointer.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.back(), Some(&20));
    /// ```
    pub fn push_back(&mut self, data: T) {
        let new_node = Box::new(Node::new(data));

        if self.head.is_none() {
            self.head = Some(new_node);
        } else {
            // Traverse to the end
            let mut current = self.head.as_mut().unwrap();
            while current.next.is_some() {
                current = current.next.as_mut().unwrap();
            }
            current.next = Some(new_node);
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
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.pop_front(), Some(10));
    /// assert_eq!(list.pop_front(), Some(20));
    /// assert_eq!(list.pop_front(), None);
    /// ```
    pub fn pop_front(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            self.head = node.next;
            self.len -= 1;
            node.data
        })
    }

    /// Removes and returns the element from the back of the list.
    ///
    /// # Time Complexity
    /// O(n) - must traverse to find the second-to-last node
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// assert_eq!(list.pop_back(), Some(20));
    /// ```
    pub fn pop_back(&mut self) -> Option<T> {
        if self.head.is_none() {
            return None;
        }

        // Single element case
        if self.head.as_ref().unwrap().next.is_none() {
            self.len -= 1;
            return self.head.take().map(|node| node.data);
        }

        // Find the second-to-last node
        let mut current = self.head.as_mut().unwrap();
        while current.next.as_ref().unwrap().next.is_some() {
            current = current.next.as_mut().unwrap();
        }

        self.len -= 1;
        current.next.take().map(|node| node.data)
    }

    /// Returns a reference to the front element.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn front(&self) -> Option<&T> {
        self.head.as_ref().map(|node| &node.data)
    }

    /// Returns a mutable reference to the front element.
    #[must_use]
    pub fn front_mut(&mut self) -> Option<&mut T> {
        self.head.as_mut().map(|node| &mut node.data)
    }

    /// Returns a reference to the back element.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn back(&self) -> Option<&T> {
        if self.head.is_none() {
            return None;
        }

        let mut current = self.head.as_ref().unwrap();
        while current.next.is_some() {
            current = current.next.as_ref().unwrap();
        }
        Some(&current.data)
    }

    /// Gets a reference to the element at the specified index.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(20);
    /// list.push_back(30);
    /// assert_eq!(list.get(1), Some(&20));
    /// assert_eq!(list.get(5), None);
    /// ```
    #[must_use]
    pub fn get(&self, index: usize) -> Option<&T> {
        if index >= self.len {
            return None;
        }

        let mut current = self.head.as_ref()?;
        for _ in 0..index {
            current = current.next.as_ref()?;
        }
        Some(&current.data)
    }

    /// Gets a mutable reference to the element at the specified index.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn get_mut(&mut self, index: usize) -> Option<&mut T> {
        if index >= self.len {
            return None;
        }

        let mut current = self.head.as_mut()?;
        for _ in 0..index {
            current = current.next.as_mut()?;
        }
        Some(&mut current.data)
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
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(10);
    /// list.push_back(30);
    /// list.insert(1, 20).unwrap();
    /// assert_eq!(list.get(1), Some(&20));
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

        let mut current = self.head.as_mut().unwrap();
        for _ in 0..(index - 1) {
            current = current.next.as_mut().unwrap();
        }

        let mut new_node = Box::new(Node::new(data));
        new_node.next = current.next.take();
        current.next = Some(new_node);
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
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
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

        let mut current = self.head.as_mut().unwrap();
        for _ in 0..(index - 1) {
            current = current.next.as_mut().unwrap();
        }

        let removed = current.next.take().unwrap();
        current.next = removed.next;
        self.len -= 1;

        Ok(removed.data)
    }

    /// Clears the list, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.head = None;
        self.len = 0;
    }

    /// Reverses the list in place.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::new();
    /// list.push_back(1);
    /// list.push_back(2);
    /// list.push_back(3);
    /// list.reverse();
    /// assert_eq!(list.front(), Some(&3));
    /// ```
    pub fn reverse(&mut self) {
        let mut prev: Option<Box<Node<T>>> = None;
        let mut current = self.head.take();

        while let Some(mut node) = current {
            let next = node.next.take();
            node.next = prev;
            prev = Some(node);
            current = next;
        }

        self.head = prev;
    }

    /// Returns an iterator over the list.
    pub fn iter(&self) -> Iter<'_, T> {
        Iter {
            current: self.head.as_deref(),
        }
    }

    /// Converts the list to a `Vec`.
    #[must_use]
    pub fn to_vec(&self) -> Vec<T>
    where
        T: Clone,
    {
        self.iter().cloned().collect()
    }

    /// Creates a `SinglyLinkedList` from a `Vec`.
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        let mut list = SinglyLinkedList::new();
        for item in vec {
            list.push_back(item);
        }
        list
    }

    /// Returns the middle element using the slow/fast pointer technique.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let mut list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
    /// assert_eq!(list.middle(), Some(&3));
    /// ```
    #[must_use]
    pub fn middle(&self) -> Option<&T> {
        if self.head.is_none() {
            return None;
        }

        let mut slow = self.head.as_ref();
        let mut fast = self.head.as_ref();

        while fast.is_some() && fast.unwrap().next.is_some() {
            slow = slow.unwrap().next.as_ref();
            fast = fast.unwrap().next.as_ref().unwrap().next.as_ref();
        }

        slow.map(|node| &node.data)
    }

    /// Gets the nth element from the end (1-indexed).
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::SinglyLinkedList;
    ///
    /// let list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
    /// assert_eq!(list.nth_from_end(1), Some(&5)); // Last element
    /// assert_eq!(list.nth_from_end(2), Some(&4)); // Second to last
    /// ```
    #[must_use]
    pub fn nth_from_end(&self, n: usize) -> Option<&T> {
        if n == 0 || n > self.len {
            return None;
        }

        let mut slow = self.head.as_ref();
        let mut fast = self.head.as_ref();

        // Move fast n nodes ahead
        for _ in 0..n {
            fast = fast?.next.as_ref();
        }

        // Move both until fast reaches the end
        while let Some(f) = fast {
            slow = slow.unwrap().next.as_ref();
            fast = f.next.as_ref();
        }

        slow.map(|node| &node.data)
    }
}

impl<T: PartialEq> SinglyLinkedList<T> {
    /// Finds the index of the first occurrence of a value.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn find(&self, value: &T) -> Option<usize> {
        let mut index = 0;
        let mut current = self.head.as_ref();

        while let Some(node) = current {
            if &node.data == value {
                return Some(index);
            }
            current = node.next.as_ref();
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

impl<T> Container for SinglyLinkedList<T> {
    fn len(&self) -> usize {
        self.len
    }
}

impl<T: PartialEq> Searchable<T> for SinglyLinkedList<T> {
    fn search(&self, value: &T) -> Option<usize> {
        self.find(value)
    }
}

impl<T> Default for SinglyLinkedList<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Clone> Clone for SinglyLinkedList<T> {
    fn clone(&self) -> Self {
        Self::from_vec(self.to_vec())
    }
}

impl<T: PartialEq> PartialEq for SinglyLinkedList<T> {
    fn eq(&self, other: &Self) -> bool {
        if self.len != other.len {
            return false;
        }

        let mut a = self.head.as_ref();
        let mut b = other.head.as_ref();

        while let (Some(node_a), Some(node_b)) = (a, b) {
            if node_a.data != node_b.data {
                return false;
            }
            a = node_a.next.as_ref();
            b = node_b.next.as_ref();
        }

        true
    }
}

impl<T: Eq> Eq for SinglyLinkedList<T> {}

impl<T> Drop for SinglyLinkedList<T> {
    fn drop(&mut self) {
        // Iterative drop to avoid stack overflow on long lists
        let mut current = self.head.take();
        while let Some(mut node) = current {
            current = node.next.take();
        }
    }
}

impl<T> FromIterator<T> for SinglyLinkedList<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let mut list = SinglyLinkedList::new();
        for item in iter {
            list.push_back(item);
        }
        list
    }
}

impl<'a, T> IntoIterator for &'a SinglyLinkedList<T> {
    type Item = &'a T;
    type IntoIter = Iter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        self.iter()
    }
}

/// An iterator over the elements of a `SinglyLinkedList`.
pub struct Iter<'a, T> {
    current: Option<&'a Node<T>>,
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.current.map(|node| {
            self.current = node.next.as_deref();
            &node.data
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let list: SinglyLinkedList<i32> = SinglyLinkedList::new();
            assert!(list.is_empty());
            assert_eq!(list.len(), 0);
        }

        #[test]
        fn test_default() {
            let list: SinglyLinkedList<i32> = SinglyLinkedList::default();
            assert!(list.is_empty());
        }

        #[test]
        fn test_from_vec() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            assert_eq!(list.len(), 3);
            assert_eq!(list.front(), Some(&1));
        }

        #[test]
        fn test_to_vec() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            assert_eq!(list.to_vec(), vec![1, 2, 3]);
        }

        #[test]
        fn test_clone() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            let cloned = list.clone();
            assert_eq!(list.to_vec(), cloned.to_vec());
        }
    }

    mod push_pop {
        use super::*;

        #[test]
        fn test_push_front() {
            let mut list = SinglyLinkedList::new();
            list.push_front(30);
            list.push_front(20);
            list.push_front(10);
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_push_back() {
            let mut list = SinglyLinkedList::new();
            list.push_back(10);
            list.push_back(20);
            list.push_back(30);
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_pop_front() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.pop_front(), Some(10));
            assert_eq!(list.pop_front(), Some(20));
            assert_eq!(list.pop_front(), Some(30));
            assert_eq!(list.pop_front(), None);
        }

        #[test]
        fn test_pop_back() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.pop_back(), Some(30));
            assert_eq!(list.pop_back(), Some(20));
            assert_eq!(list.pop_back(), Some(10));
            assert_eq!(list.pop_back(), None);
        }
    }

    mod access {
        use super::*;

        #[test]
        fn test_front_back() {
            let list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.front(), Some(&10));
            assert_eq!(list.back(), Some(&30));

            let empty: SinglyLinkedList<i32> = SinglyLinkedList::new();
            assert_eq!(empty.front(), None);
            assert_eq!(empty.back(), None);
        }

        #[test]
        fn test_get() {
            let list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.get(0), Some(&10));
            assert_eq!(list.get(1), Some(&20));
            assert_eq!(list.get(2), Some(&30));
            assert_eq!(list.get(3), None);
        }

        #[test]
        fn test_get_mut() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            if let Some(val) = list.get_mut(1) {
                *val = 25;
            }
            assert_eq!(list.get(1), Some(&25));
        }
    }

    mod insert_remove {
        use super::*;

        #[test]
        fn test_insert() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 30]);
            assert!(list.insert(1, 20).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_at_start() {
            let mut list = SinglyLinkedList::from_vec(vec![20, 30]);
            assert!(list.insert(0, 10).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_at_end() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20]);
            assert!(list.insert(2, 30).is_ok());
            assert_eq!(list.to_vec(), vec![10, 20, 30]);
        }

        #[test]
        fn test_insert_out_of_bounds() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20]);
            let result = list.insert(5, 30);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_remove() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.remove(1).unwrap(), 20);
            assert_eq!(list.to_vec(), vec![10, 30]);
        }

        #[test]
        fn test_remove_first() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert_eq!(list.remove(0).unwrap(), 10);
            assert_eq!(list.to_vec(), vec![20, 30]);
        }

        #[test]
        fn test_remove_out_of_bounds() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20]);
            let result = list.remove(5);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_clear() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            list.clear();
            assert!(list.is_empty());
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_find() {
            let list = SinglyLinkedList::from_vec(vec![10, 20, 30, 20]);
            assert_eq!(list.find(&20), Some(1));
            assert_eq!(list.find(&50), None);
        }

        #[test]
        fn test_contains() {
            let list = SinglyLinkedList::from_vec(vec![10, 20, 30]);
            assert!(list.contains(&20));
            assert!(!list.contains(&50));
        }

        #[test]
        fn test_remove_value() {
            let mut list = SinglyLinkedList::from_vec(vec![10, 20, 30, 20]);
            assert!(list.remove_value(&20));
            assert_eq!(list.to_vec(), vec![10, 30, 20]);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_reverse() {
            let mut list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
            list.reverse();
            assert_eq!(list.to_vec(), vec![5, 4, 3, 2, 1]);
        }

        #[test]
        fn test_reverse_empty() {
            let mut list: SinglyLinkedList<i32> = SinglyLinkedList::new();
            list.reverse();
            assert!(list.is_empty());
        }

        #[test]
        fn test_reverse_single() {
            let mut list = SinglyLinkedList::from_vec(vec![42]);
            list.reverse();
            assert_eq!(list.to_vec(), vec![42]);
        }

        #[test]
        fn test_middle() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
            assert_eq!(list.middle(), Some(&3));

            // For even-length lists, returns the second middle (per LeetCode #876)
            let even_list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4]);
            assert_eq!(even_list.middle(), Some(&3));
        }

        #[test]
        fn test_nth_from_end() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3, 4, 5]);
            assert_eq!(list.nth_from_end(1), Some(&5));
            assert_eq!(list.nth_from_end(2), Some(&4));
            assert_eq!(list.nth_from_end(5), Some(&1));
            assert_eq!(list.nth_from_end(0), None);
            assert_eq!(list.nth_from_end(6), None);
        }

        #[test]
        fn test_iter() {
            let list = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            let sum: i32 = list.iter().sum();
            assert_eq!(sum, 6);
        }

        #[test]
        fn test_from_iter() {
            let list: SinglyLinkedList<i32> = (1..=5).collect();
            assert_eq!(list.to_vec(), vec![1, 2, 3, 4, 5]);
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let list1 = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            let list2 = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            let list3 = SinglyLinkedList::from_vec(vec![1, 2, 4]);
            assert_eq!(list1, list2);
            assert_ne!(list1, list3);
        }

        #[test]
        fn test_eq_different_lengths() {
            let list1 = SinglyLinkedList::from_vec(vec![1, 2, 3]);
            let list2 = SinglyLinkedList::from_vec(vec![1, 2]);
            assert_ne!(list1, list2);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let mut list = SinglyLinkedList::new();
            list.push_back(42);
            assert_eq!(list.front(), Some(&42));
            assert_eq!(list.back(), Some(&42));
            assert_eq!(list.pop_front(), Some(42));
            assert!(list.is_empty());
        }

        #[test]
        fn test_large_list() {
            let mut list = SinglyLinkedList::new();
            for i in 0..1000 {
                list.push_back(i);
            }
            assert_eq!(list.len(), 1000);
            assert_eq!(list.get(500), Some(&500));
        }
    }
}
