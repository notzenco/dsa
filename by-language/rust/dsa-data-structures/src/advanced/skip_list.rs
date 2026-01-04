//! # Skip List
//!
//! A Skip List is a probabilistic data structure that allows O(log n) average
//! search, insertion, and deletion operations.
//!
//! ## Complexity Analysis
//!
//! | Operation | Average    | Worst Case |
//! |-----------|------------|------------|
//! | Search    | O(log n)   | O(n)       |
//! | Insert    | O(log n)   | O(n)       |
//! | Delete    | O(log n)   | O(n)       |
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::advanced::SkipList;
//!
//! let mut list = SkipList::new();
//! list.insert(3);
//! list.insert(1);
//! list.insert(2);
//!
//! assert!(list.contains(&2));
//! assert_eq!(list.len(), 3);
//!
//! list.remove(&2);
//! assert!(!list.contains(&2));
//! ```

use alloc::vec;
use alloc::vec::Vec;

const MAX_LEVEL: usize = 16;

/// A node in the skip list.
#[derive(Clone)]
struct Node<T: Clone> {
    value: T,
    forward: Vec<Option<usize>>, // Indices into nodes vec
}

/// A probabilistic search structure with O(log n) average operations.
///
/// This implementation uses arena allocation (Vec-based) for safety.
pub struct SkipList<T: Ord + Clone> {
    nodes: Vec<Node<T>>,
    head_forward: Vec<Option<usize>>,
    level: usize,
    rand_state: u64,
}

impl<T: Ord + Clone> SkipList<T> {
    /// Creates a new empty skip list.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::advanced::SkipList;
    ///
    /// let list: SkipList<i32> = SkipList::new();
    /// assert!(list.is_empty());
    /// ```
    pub fn new() -> Self {
        SkipList {
            nodes: Vec::new(),
            head_forward: vec![None; MAX_LEVEL + 1],
            level: 0,
            rand_state: 0x853c49e6748fea9b,
        }
    }

    /// Returns the number of elements.
    pub fn len(&self) -> usize {
        self.nodes.len()
    }

    /// Returns `true` if empty.
    pub fn is_empty(&self) -> bool {
        self.nodes.is_empty()
    }

    fn random(&mut self) -> u64 {
        let mut x = self.rand_state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        self.rand_state = x;
        x
    }

    fn random_level(&mut self) -> usize {
        let mut level = 0;
        while level < MAX_LEVEL && (self.random() % 2 == 0) {
            level += 1;
        }
        level
    }

    /// Inserts a value. Returns `true` if inserted, `false` if already exists.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::advanced::SkipList;
    ///
    /// let mut list = SkipList::new();
    /// assert!(list.insert(5));
    /// assert!(!list.insert(5)); // Already exists
    /// ```
    pub fn insert(&mut self, value: T) -> bool {
        let mut update: Vec<Option<usize>> = vec![None; MAX_LEVEL + 1];
        let mut current: Option<usize> = None;
        let mut current_forward = &self.head_forward;

        // Find position
        for i in (0..=self.level).rev() {
            loop {
                if let Some(next_idx) = current_forward[i] {
                    match self.nodes[next_idx].value.cmp(&value) {
                        core::cmp::Ordering::Less => {
                            current = Some(next_idx);
                            current_forward = &self.nodes[next_idx].forward;
                        }
                        core::cmp::Ordering::Equal => return false,
                        core::cmp::Ordering::Greater => break,
                    }
                } else {
                    break;
                }
            }
            update[i] = current;
        }

        let new_level = self.random_level();

        if new_level > self.level {
            for i in (self.level + 1)..=new_level {
                update[i] = None; // Head
            }
            self.level = new_level;
        }

        // Create new node
        let new_idx = self.nodes.len();
        let mut new_forward = vec![None; new_level + 1];

        for i in 0..=new_level {
            if let Some(prev_idx) = update[i] {
                new_forward[i] = self.nodes[prev_idx].forward[i];
            } else {
                new_forward[i] = self.head_forward[i];
            }
        }

        self.nodes.push(Node {
            value,
            forward: new_forward,
        });

        // Update forward pointers
        for i in 0..=new_level {
            if let Some(prev_idx) = update[i] {
                self.nodes[prev_idx].forward[i] = Some(new_idx);
            } else {
                self.head_forward[i] = Some(new_idx);
            }
        }

        true
    }

    /// Returns `true` if the value exists.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::advanced::SkipList;
    ///
    /// let mut list = SkipList::new();
    /// list.insert(5);
    /// assert!(list.contains(&5));
    /// assert!(!list.contains(&10));
    /// ```
    pub fn contains(&self, value: &T) -> bool {
        let mut current_forward = &self.head_forward;

        for i in (0..=self.level).rev() {
            loop {
                if let Some(next_idx) = current_forward[i] {
                    match self.nodes[next_idx].value.cmp(value) {
                        core::cmp::Ordering::Less => {
                            current_forward = &self.nodes[next_idx].forward;
                        }
                        core::cmp::Ordering::Equal => return true,
                        core::cmp::Ordering::Greater => break,
                    }
                } else {
                    break;
                }
            }
        }

        false
    }

    /// Removes a value. Returns `true` if removed.
    ///
    /// Note: This implementation marks nodes as removed but doesn't compact.
    /// For a production implementation, periodic compaction would be needed.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::advanced::SkipList;
    ///
    /// let mut list = SkipList::new();
    /// list.insert(5);
    /// assert!(list.remove(&5));
    /// assert!(!list.contains(&5));
    /// ```
    pub fn remove(&mut self, value: &T) -> bool {
        let mut update: Vec<Option<usize>> = vec![None; MAX_LEVEL + 1];
        let mut current: Option<usize> = None;
        let mut current_forward = &self.head_forward;
        let mut found_idx: Option<usize> = None;

        for i in (0..=self.level).rev() {
            loop {
                if let Some(next_idx) = current_forward[i] {
                    match self.nodes[next_idx].value.cmp(value) {
                        core::cmp::Ordering::Less => {
                            current = Some(next_idx);
                            current_forward = &self.nodes[next_idx].forward;
                        }
                        core::cmp::Ordering::Equal => {
                            found_idx = Some(next_idx);
                            break;
                        }
                        core::cmp::Ordering::Greater => break,
                    }
                } else {
                    break;
                }
            }
            update[i] = current;
        }

        if let Some(target_idx) = found_idx {
            // Update forward pointers
            for i in 0..=self.level {
                if let Some(prev_idx) = update[i] {
                    if self.nodes[prev_idx].forward[i] == Some(target_idx) {
                        let target_forward = if i < self.nodes[target_idx].forward.len() {
                            self.nodes[target_idx].forward[i]
                        } else {
                            None
                        };
                        self.nodes[prev_idx].forward[i] = target_forward;
                    }
                } else if self.head_forward[i] == Some(target_idx) {
                    let target_forward = if i < self.nodes[target_idx].forward.len() {
                        self.nodes[target_idx].forward[i]
                    } else {
                        None
                    };
                    self.head_forward[i] = target_forward;
                }
            }

            // Update level
            while self.level > 0 && self.head_forward[self.level].is_none() {
                self.level -= 1;
            }

            return true;
        }

        false
    }

    /// Returns the first (minimum) element.
    pub fn first(&self) -> Option<&T> {
        self.head_forward[0].map(|idx| &self.nodes[idx].value)
    }

    /// Returns an iterator over elements in sorted order.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::advanced::SkipList;
    ///
    /// let mut list = SkipList::new();
    /// list.insert(3);
    /// list.insert(1);
    /// list.insert(2);
    ///
    /// let sorted: Vec<_> = list.iter().cloned().collect();
    /// assert_eq!(sorted, vec![1, 2, 3]);
    /// ```
    pub fn iter(&self) -> SkipListIter<'_, T> {
        SkipListIter {
            list: self,
            current: self.head_forward[0],
        }
    }

    /// Clears the skip list.
    pub fn clear(&mut self) {
        self.nodes.clear();
        self.head_forward = vec![None; MAX_LEVEL + 1];
        self.level = 0;
    }
}

impl<T: Ord + Clone> Default for SkipList<T> {
    fn default() -> Self {
        Self::new()
    }
}

/// Iterator over skip list elements.
pub struct SkipListIter<'a, T: Ord + Clone> {
    list: &'a SkipList<T>,
    current: Option<usize>,
}

impl<'a, T: Ord + Clone> Iterator for SkipListIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.current.map(|idx| {
            let node = &self.list.nodes[idx];
            self.current = node.forward.first().copied().flatten();
            &node.value
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
            let list: SkipList<i32> = SkipList::new();
            assert!(list.is_empty());
        }

        #[test]
        fn test_default() {
            let list: SkipList<i32> = SkipList::default();
            assert!(list.is_empty());
        }
    }

    mod insert {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut list = SkipList::new();
            assert!(list.insert(5));
            assert!(list.contains(&5));
        }

        #[test]
        fn test_insert_multiple() {
            let mut list = SkipList::new();
            list.insert(3);
            list.insert(1);
            list.insert(2);

            assert!(list.contains(&1));
            assert!(list.contains(&2));
            assert!(list.contains(&3));
        }

        #[test]
        fn test_insert_duplicate() {
            let mut list = SkipList::new();
            assert!(list.insert(5));
            assert!(!list.insert(5));
        }

        #[test]
        fn test_insert_sorted() {
            let mut list = SkipList::new();
            list.insert(5);
            list.insert(1);
            list.insert(3);

            let sorted: Vec<_> = list.iter().cloned().collect();
            assert_eq!(sorted, vec![1, 3, 5]);
        }
    }

    mod contains {
        use super::*;

        #[test]
        fn test_contains_existing() {
            let mut list = SkipList::new();
            list.insert(5);
            assert!(list.contains(&5));
        }

        #[test]
        fn test_contains_nonexistent() {
            let mut list = SkipList::new();
            list.insert(5);
            assert!(!list.contains(&10));
        }

        #[test]
        fn test_contains_empty() {
            let list: SkipList<i32> = SkipList::new();
            assert!(!list.contains(&5));
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove_existing() {
            let mut list = SkipList::new();
            list.insert(5);
            assert!(list.remove(&5));
            assert!(!list.contains(&5));
        }

        #[test]
        fn test_remove_nonexistent() {
            let mut list = SkipList::new();
            list.insert(5);
            assert!(!list.remove(&10));
        }

        #[test]
        fn test_remove_middle() {
            let mut list = SkipList::new();
            list.insert(1);
            list.insert(2);
            list.insert(3);

            assert!(list.remove(&2));
            assert!(!list.contains(&2));
            assert!(list.contains(&1));
            assert!(list.contains(&3));
        }
    }

    mod first_and_iter {
        use super::*;

        #[test]
        fn test_first() {
            let mut list = SkipList::new();
            assert_eq!(list.first(), None);

            list.insert(3);
            list.insert(1);
            list.insert(2);

            assert_eq!(list.first(), Some(&1));
        }

        #[test]
        fn test_iter() {
            let mut list = SkipList::new();
            list.insert(5);
            list.insert(1);
            list.insert(3);

            let items: Vec<_> = list.iter().cloned().collect();
            assert_eq!(items, vec![1, 3, 5]);
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut list = SkipList::new();
            list.insert(1);
            list.insert(2);
            list.clear();

            assert!(list.is_empty());
        }
    }

    mod stress {
        use super::*;

        #[test]
        fn test_many_insertions() {
            let mut list = SkipList::new();

            for i in 0..100 {
                list.insert(i);
            }

            for i in 0..100 {
                assert!(list.contains(&i));
            }
        }
    }
}
