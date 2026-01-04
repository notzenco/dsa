//! Binary Search Tree (BST)
//!
//! A binary search tree is a tree data structure where each node has at most two children,
//! and the left subtree contains only nodes with keys less than the node's key,
//! while the right subtree contains only nodes with keys greater than the node's key.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Binary Search Tree:                                                       ║
//! ║                                                                            ║
//! ║                    ┌───┐                                                   ║
//! ║                    │ 8 │ ◀─── root                                         ║
//! ║                    └─┬─┘                                                   ║
//! ║             ┌───────┴───────┐                                              ║
//! ║             ▼               ▼                                              ║
//! ║           ┌───┐           ┌───┐                                            ║
//! ║           │ 3 │           │ 10│                                            ║
//! ║           └─┬─┘           └─┬─┘                                            ║
//! ║        ┌────┴────┐          └────┐                                         ║
//! ║        ▼         ▼               ▼                                         ║
//! ║      ┌───┐     ┌───┐           ┌───┐                                       ║
//! ║      │ 1 │     │ 6 │           │ 14│                                       ║
//! ║      └───┘     └─┬─┘           └─┬─┘                                       ║
//! ║              ┌───┴───┐           └───┐                                     ║
//! ║              ▼       ▼               ▼                                     ║
//! ║            ┌───┐   ┌───┐           ┌───┐                                   ║
//! ║            │ 4 │   │ 7 │           │ 13│                                   ║
//! ║            └───┘   └───┘           └───┘                                   ║
//! ║                                                                            ║
//! ║  Property: left < parent < right                                           ║
//! ║                                                                            ║
//! ║  In-order traversal: 1, 3, 4, 6, 7, 8, 10, 13, 14 (sorted!)               ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation | Average   | Worst | Space |
//! |-----------|-----------|-------|-------|
//! | Insert    | O(log n)  | O(n)  | O(1)  |
//! | Delete    | O(log n)  | O(n)  | O(1)  |
//! | Search    | O(log n)  | O(n)  | O(1)  |
//! | Min/Max   | O(log n)  | O(n)  | O(1)  |
//! | Traversal | O(n)      | O(n)  | O(n)  |
//!
//! Worst case occurs when the tree degenerates to a linked list.
//!
//! ## LeetCode Problems
//!
//! - [#98 Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/)
//! - [#700 Search in a Binary Search Tree](https://leetcode.com/problems/search-in-a-binary-search-tree/)
//! - [#701 Insert into a Binary Search Tree](https://leetcode.com/problems/insert-into-a-binary-search-tree/)
//! - [#450 Delete Node in a BST](https://leetcode.com/problems/delete-node-in-a-bst/)
//! - [#230 Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst/)
//! - [#94 Binary Tree Inorder Traversal](https://leetcode.com/problems/binary-tree-inorder-traversal/)
//! - [#144 Binary Tree Preorder Traversal](https://leetcode.com/problems/binary-tree-preorder-traversal/)
//! - [#145 Binary Tree Postorder Traversal](https://leetcode.com/problems/binary-tree-postorder-traversal/)
//! - [#102 Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/)
//!
//! ## Use Cases
//!
//! - Implementing sorted collections (Set, Map)
//! - Database indexing
//! - Priority queues (when balanced)
//! - Expression parsing
//! - File system organization
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::BinarySearchTree;
//!
//! let mut bst = BinarySearchTree::new();
//! bst.insert(8);
//! bst.insert(3);
//! bst.insert(10);
//! bst.insert(1);
//! bst.insert(6);
//!
//! assert!(bst.contains(&6));
//! assert!(!bst.contains(&5));
//! assert_eq!(bst.min(), Some(&1));
//! assert_eq!(bst.max(), Some(&10));
//!
//! let inorder: Vec<_> = bst.inorder().collect();
//! assert_eq!(inorder, vec![&1, &3, &6, &8, &10]);
//! ```

use alloc::boxed::Box;
use alloc::collections::VecDeque;
use alloc::vec::Vec;

use dsa_core::Container;

/// A node in the binary search tree.
#[derive(Debug, Clone)]
struct Node<T> {
    value: T,
    left: Option<Box<Node<T>>>,
    right: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(value: T) -> Self {
        Node {
            value,
            left: None,
            right: None,
        }
    }
}

/// A binary search tree implementation.
///
/// This is an unbalanced BST - for guaranteed O(log n) operations,
/// use AVL or Red-Black trees.
#[derive(Debug, Clone)]
pub struct BinarySearchTree<T> {
    root: Option<Box<Node<T>>>,
    size: usize,
}

impl<T: Ord> BinarySearchTree<T> {
    /// Creates a new empty binary search tree.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let bst: BinarySearchTree<i32> = BinarySearchTree::new();
    /// assert!(bst.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        BinarySearchTree {
            root: None,
            size: 0,
        }
    }

    /// Returns the number of elements in the tree.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.size
    }

    /// Returns `true` if the tree contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    /// Inserts a value into the tree.
    ///
    /// If the value already exists, it is not inserted (no duplicates).
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let mut bst = BinarySearchTree::new();
    /// bst.insert(5);
    /// bst.insert(3);
    /// bst.insert(7);
    /// assert_eq!(bst.len(), 3);
    /// ```
    pub fn insert(&mut self, value: T) {
        if Self::insert_node(&mut self.root, value) {
            self.size += 1;
        }
    }

    fn insert_node(node: &mut Option<Box<Node<T>>>, value: T) -> bool {
        match node {
            None => {
                *node = Some(Box::new(Node::new(value)));
                true
            }
            Some(n) => {
                use core::cmp::Ordering;
                match value.cmp(&n.value) {
                    Ordering::Less => Self::insert_node(&mut n.left, value),
                    Ordering::Greater => Self::insert_node(&mut n.right, value),
                    Ordering::Equal => false, // No duplicates
                }
            }
        }
    }

    /// Returns `true` if the tree contains the specified value.
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let mut bst = BinarySearchTree::new();
    /// bst.insert(5);
    /// assert!(bst.contains(&5));
    /// assert!(!bst.contains(&10));
    /// ```
    #[must_use]
    pub fn contains(&self, value: &T) -> bool {
        self.search(value).is_some()
    }

    /// Searches for a value and returns a reference to it if found.
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    #[must_use]
    pub fn search(&self, value: &T) -> Option<&T> {
        Self::search_node(&self.root, value)
    }

    fn search_node<'a>(node: &'a Option<Box<Node<T>>>, value: &T) -> Option<&'a T> {
        match node {
            None => None,
            Some(n) => {
                use core::cmp::Ordering;
                match value.cmp(&n.value) {
                    Ordering::Less => Self::search_node(&n.left, value),
                    Ordering::Greater => Self::search_node(&n.right, value),
                    Ordering::Equal => Some(&n.value),
                }
            }
        }
    }

    /// Removes a value from the tree.
    ///
    /// Returns `true` if the value was present and removed.
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let mut bst = BinarySearchTree::new();
    /// bst.insert(5);
    /// bst.insert(3);
    /// bst.insert(7);
    ///
    /// assert!(bst.remove(&5));
    /// assert!(!bst.contains(&5));
    /// assert_eq!(bst.len(), 2);
    /// ```
    pub fn remove(&mut self, value: &T) -> bool {
        let (new_root, removed) = Self::remove_node(self.root.take(), value);
        self.root = new_root;
        if removed {
            self.size -= 1;
        }
        removed
    }

    fn remove_node(node: Option<Box<Node<T>>>, value: &T) -> (Option<Box<Node<T>>>, bool) {
        match node {
            None => (None, false),
            Some(mut n) => {
                use core::cmp::Ordering;
                match value.cmp(&n.value) {
                    Ordering::Less => {
                        let (new_left, removed) = Self::remove_node(n.left.take(), value);
                        n.left = new_left;
                        (Some(n), removed)
                    }
                    Ordering::Greater => {
                        let (new_right, removed) = Self::remove_node(n.right.take(), value);
                        n.right = new_right;
                        (Some(n), removed)
                    }
                    Ordering::Equal => {
                        // Node to delete found
                        match (n.left.take(), n.right.take()) {
                            (None, None) => (None, true),
                            (Some(left), None) => (Some(left), true),
                            (None, Some(right)) => (Some(right), true),
                            (Some(left), Some(right)) => {
                                // Two children: replace with in-order successor
                                let (new_right, successor_val) = Self::extract_min(right);
                                n.value = successor_val;
                                n.left = Some(left);
                                n.right = new_right;
                                (Some(n), true)
                            }
                        }
                    }
                }
            }
        }
    }

    fn extract_min(mut node: Box<Node<T>>) -> (Option<Box<Node<T>>>, T) {
        match node.left.take() {
            None => (node.right, node.value),
            Some(left) => {
                let (new_left, min_val) = Self::extract_min(left);
                node.left = new_left;
                (Some(node), min_val)
            }
        }
    }

    /// Returns a reference to the minimum value in the tree.
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let mut bst = BinarySearchTree::new();
    /// bst.insert(5);
    /// bst.insert(3);
    /// bst.insert(7);
    /// assert_eq!(bst.min(), Some(&3));
    /// ```
    #[must_use]
    pub fn min(&self) -> Option<&T> {
        Self::find_min(&self.root)
    }

    fn find_min(node: &Option<Box<Node<T>>>) -> Option<&T> {
        match node {
            None => None,
            Some(n) => match &n.left {
                None => Some(&n.value),
                Some(_) => Self::find_min(&n.left),
            },
        }
    }

    /// Returns a reference to the maximum value in the tree.
    ///
    /// # Time Complexity
    /// O(log n) average, O(n) worst case
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BinarySearchTree;
    ///
    /// let mut bst = BinarySearchTree::new();
    /// bst.insert(5);
    /// bst.insert(3);
    /// bst.insert(7);
    /// assert_eq!(bst.max(), Some(&7));
    /// ```
    #[must_use]
    pub fn max(&self) -> Option<&T> {
        Self::find_max(&self.root)
    }

    fn find_max(node: &Option<Box<Node<T>>>) -> Option<&T> {
        match node {
            None => None,
            Some(n) => match &n.right {
                None => Some(&n.value),
                Some(_) => Self::find_max(&n.right),
            },
        }
    }

    /// Clears the tree, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.root = None;
        self.size = 0;
    }

    /// Returns the height of the tree.
    ///
    /// An empty tree has height 0, a tree with one node has height 1.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn height(&self) -> usize {
        Self::node_height(&self.root)
    }

    fn node_height(node: &Option<Box<Node<T>>>) -> usize {
        match node {
            None => 0,
            Some(n) => {
                1 + core::cmp::max(
                    Self::node_height(&n.left),
                    Self::node_height(&n.right),
                )
            }
        }
    }

    /// Returns an iterator that performs in-order traversal (sorted order).
    ///
    /// # Time Complexity
    /// O(n) for full traversal
    pub fn inorder(&self) -> InorderIterator<'_, T> {
        InorderIterator::new(&self.root)
    }

    /// Returns an iterator that performs pre-order traversal.
    ///
    /// # Time Complexity
    /// O(n) for full traversal
    pub fn preorder(&self) -> PreorderIterator<'_, T> {
        PreorderIterator::new(&self.root)
    }

    /// Returns an iterator that performs post-order traversal.
    ///
    /// # Time Complexity
    /// O(n) for full traversal
    pub fn postorder(&self) -> PostorderIterator<'_, T> {
        PostorderIterator::new(&self.root)
    }

    /// Returns an iterator that performs level-order (BFS) traversal.
    ///
    /// # Time Complexity
    /// O(n) for full traversal
    pub fn levelorder(&self) -> LevelOrderIterator<'_, T> {
        LevelOrderIterator::new(&self.root)
    }

    /// Converts the tree to a sorted Vec (in-order traversal).
    #[must_use]
    pub fn to_sorted_vec(&self) -> Vec<&T> {
        self.inorder().collect()
    }

    /// Creates a BST from a slice of values.
    #[must_use]
    pub fn from_slice(values: &[T]) -> Self
    where
        T: Clone,
    {
        let mut bst = BinarySearchTree::new();
        for value in values {
            bst.insert(value.clone());
        }
        bst
    }

    /// Returns the floor of a value (largest value <= given value).
    #[must_use]
    pub fn floor(&self, value: &T) -> Option<&T> {
        Self::find_floor(&self.root, value)
    }

    fn find_floor<'a>(node: &'a Option<Box<Node<T>>>, value: &T) -> Option<&'a T> {
        match node {
            None => None,
            Some(n) => {
                use core::cmp::Ordering;
                match value.cmp(&n.value) {
                    Ordering::Equal => Some(&n.value),
                    Ordering::Less => Self::find_floor(&n.left, value),
                    Ordering::Greater => {
                        Self::find_floor(&n.right, value).or(Some(&n.value))
                    }
                }
            }
        }
    }

    /// Returns the ceiling of a value (smallest value >= given value).
    #[must_use]
    pub fn ceiling(&self, value: &T) -> Option<&T> {
        Self::find_ceiling(&self.root, value)
    }

    fn find_ceiling<'a>(node: &'a Option<Box<Node<T>>>, value: &T) -> Option<&'a T> {
        match node {
            None => None,
            Some(n) => {
                use core::cmp::Ordering;
                match value.cmp(&n.value) {
                    Ordering::Equal => Some(&n.value),
                    Ordering::Greater => Self::find_ceiling(&n.right, value),
                    Ordering::Less => {
                        Self::find_ceiling(&n.left, value).or(Some(&n.value))
                    }
                }
            }
        }
    }

    /// Returns the kth smallest element (1-indexed).
    #[must_use]
    pub fn kth_smallest(&self, k: usize) -> Option<&T> {
        if k == 0 || k > self.size {
            return None;
        }
        self.inorder().nth(k - 1)
    }

    /// Checks if the tree is a valid BST.
    ///
    /// This is mainly for testing and debugging.
    #[must_use]
    pub fn is_valid(&self) -> bool {
        Self::is_valid_node(&self.root, None, None)
    }

    fn is_valid_node(
        node: &Option<Box<Node<T>>>,
        min: Option<&T>,
        max: Option<&T>,
    ) -> bool {
        match node {
            None => true,
            Some(n) => {
                if let Some(min_val) = min {
                    if n.value <= *min_val {
                        return false;
                    }
                }
                if let Some(max_val) = max {
                    if n.value >= *max_val {
                        return false;
                    }
                }
                Self::is_valid_node(&n.left, min, Some(&n.value))
                    && Self::is_valid_node(&n.right, Some(&n.value), max)
            }
        }
    }
}

impl<T: Ord> Default for BinarySearchTree<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Ord> Container for BinarySearchTree<T> {
    fn len(&self) -> usize {
        self.size
    }
}

impl<T: Ord> FromIterator<T> for BinarySearchTree<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let mut bst = BinarySearchTree::new();
        for value in iter {
            bst.insert(value);
        }
        bst
    }
}

/// In-order traversal iterator.
pub struct InorderIterator<'a, T> {
    stack: Vec<&'a Node<T>>,
    current: Option<&'a Node<T>>,
}

impl<'a, T> InorderIterator<'a, T> {
    fn new(root: &'a Option<Box<Node<T>>>) -> Self {
        InorderIterator {
            stack: Vec::new(),
            current: root.as_ref().map(|n| n.as_ref()),
        }
    }
}

impl<'a, T> Iterator for InorderIterator<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        while let Some(node) = self.current {
            self.stack.push(node);
            self.current = node.left.as_ref().map(|n| n.as_ref());
        }

        self.stack.pop().map(|node| {
            self.current = node.right.as_ref().map(|n| n.as_ref());
            &node.value
        })
    }
}

/// Pre-order traversal iterator.
pub struct PreorderIterator<'a, T> {
    stack: Vec<&'a Node<T>>,
}

impl<'a, T> PreorderIterator<'a, T> {
    fn new(root: &'a Option<Box<Node<T>>>) -> Self {
        let mut stack = Vec::new();
        if let Some(node) = root {
            stack.push(node.as_ref());
        }
        PreorderIterator { stack }
    }
}

impl<'a, T> Iterator for PreorderIterator<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.stack.pop().map(|node| {
            if let Some(right) = &node.right {
                self.stack.push(right.as_ref());
            }
            if let Some(left) = &node.left {
                self.stack.push(left.as_ref());
            }
            &node.value
        })
    }
}

/// Post-order traversal iterator.
pub struct PostorderIterator<'a, T> {
    stack: Vec<(&'a Node<T>, bool)>,
}

impl<'a, T> PostorderIterator<'a, T> {
    fn new(root: &'a Option<Box<Node<T>>>) -> Self {
        let mut stack = Vec::new();
        if let Some(node) = root {
            stack.push((node.as_ref(), false));
        }
        PostorderIterator { stack }
    }
}

impl<'a, T> Iterator for PostorderIterator<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        while let Some((node, visited)) = self.stack.pop() {
            if visited {
                return Some(&node.value);
            }

            // Push back as visited
            self.stack.push((node, true));

            // Push children (right first, then left, so left is processed first)
            if let Some(right) = &node.right {
                self.stack.push((right.as_ref(), false));
            }
            if let Some(left) = &node.left {
                self.stack.push((left.as_ref(), false));
            }
        }
        None
    }
}

/// Level-order (BFS) traversal iterator.
pub struct LevelOrderIterator<'a, T> {
    queue: VecDeque<&'a Node<T>>,
}

impl<'a, T> LevelOrderIterator<'a, T> {
    fn new(root: &'a Option<Box<Node<T>>>) -> Self {
        let mut queue = VecDeque::new();
        if let Some(node) = root {
            queue.push_back(node.as_ref());
        }
        LevelOrderIterator { queue }
    }
}

impl<'a, T> Iterator for LevelOrderIterator<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.queue.pop_front().map(|node| {
            if let Some(left) = &node.left {
                self.queue.push_back(left.as_ref());
            }
            if let Some(right) = &node.right {
                self.queue.push_back(right.as_ref());
            }
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
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert!(bst.is_empty());
            assert_eq!(bst.len(), 0);
        }

        #[test]
        fn test_default() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::default();
            assert!(bst.is_empty());
        }

        #[test]
        fn test_from_slice() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.len(), 5);
            assert!(bst.is_valid());
        }

        #[test]
        fn test_from_iter() {
            let bst: BinarySearchTree<i32> = (1..=5).collect();
            assert_eq!(bst.len(), 5);
            assert!(bst.is_valid());
        }
    }

    mod insert {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut bst = BinarySearchTree::new();
            bst.insert(5);
            assert_eq!(bst.len(), 1);
            assert!(bst.contains(&5));
        }

        #[test]
        fn test_insert_multiple() {
            let mut bst = BinarySearchTree::new();
            bst.insert(5);
            bst.insert(3);
            bst.insert(7);
            bst.insert(1);
            bst.insert(9);
            assert_eq!(bst.len(), 5);
            assert!(bst.is_valid());
        }

        #[test]
        fn test_insert_duplicate() {
            let mut bst = BinarySearchTree::new();
            bst.insert(5);
            bst.insert(5);
            assert_eq!(bst.len(), 1);
        }

        #[test]
        fn test_insert_ascending() {
            let mut bst = BinarySearchTree::new();
            for i in 1..=10 {
                bst.insert(i);
            }
            assert_eq!(bst.len(), 10);
            assert!(bst.is_valid());
        }

        #[test]
        fn test_insert_descending() {
            let mut bst = BinarySearchTree::new();
            for i in (1..=10).rev() {
                bst.insert(i);
            }
            assert_eq!(bst.len(), 10);
            assert!(bst.is_valid());
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_contains() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert!(bst.contains(&5));
            assert!(bst.contains(&3));
            assert!(bst.contains(&7));
            assert!(!bst.contains(&4));
            assert!(!bst.contains(&10));
        }

        #[test]
        fn test_search() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert_eq!(bst.search(&5), Some(&5));
            assert_eq!(bst.search(&3), Some(&3));
            assert_eq!(bst.search(&4), None);
        }

        #[test]
        fn test_search_empty() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert_eq!(bst.search(&5), None);
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove_leaf() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert!(bst.remove(&3));
            assert!(!bst.contains(&3));
            assert_eq!(bst.len(), 2);
            assert!(bst.is_valid());
        }

        #[test]
        fn test_remove_one_child() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7, 1]);
            assert!(bst.remove(&3));
            assert!(!bst.contains(&3));
            assert!(bst.contains(&1));
            assert!(bst.is_valid());
        }

        #[test]
        fn test_remove_two_children() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            assert!(bst.remove(&5));
            assert!(!bst.contains(&5));
            assert_eq!(bst.len(), 6);
            assert!(bst.is_valid());
        }

        #[test]
        fn test_remove_root() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert!(bst.remove(&5));
            assert!(!bst.contains(&5));
            assert!(bst.is_valid());
        }

        #[test]
        fn test_remove_nonexistent() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert!(!bst.remove(&10));
            assert_eq!(bst.len(), 3);
        }

        #[test]
        fn test_remove_all() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert!(bst.remove(&5));
            assert!(bst.remove(&3));
            assert!(bst.remove(&7));
            assert!(bst.is_empty());
        }
    }

    mod min_max {
        use super::*;

        #[test]
        fn test_min() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.min(), Some(&1));
        }

        #[test]
        fn test_max() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.max(), Some(&9));
        }

        #[test]
        fn test_min_empty() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert_eq!(bst.min(), None);
        }

        #[test]
        fn test_max_empty() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert_eq!(bst.max(), None);
        }

        #[test]
        fn test_min_max_single() {
            let bst = BinarySearchTree::from_slice(&[5]);
            assert_eq!(bst.min(), Some(&5));
            assert_eq!(bst.max(), Some(&5));
        }
    }

    mod floor_ceiling {
        use super::*;

        #[test]
        fn test_floor() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.floor(&4), Some(&3));
            assert_eq!(bst.floor(&5), Some(&5));
            assert_eq!(bst.floor(&8), Some(&7));
            assert_eq!(bst.floor(&0), None);
        }

        #[test]
        fn test_ceiling() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.ceiling(&4), Some(&5));
            assert_eq!(bst.ceiling(&5), Some(&5));
            assert_eq!(bst.ceiling(&6), Some(&7));
            assert_eq!(bst.ceiling(&10), None);
        }
    }

    mod traversal {
        use super::*;

        #[test]
        fn test_inorder() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            let result: Vec<_> = bst.inorder().cloned().collect();
            assert_eq!(result, vec![1, 3, 4, 5, 6, 7, 9]);
        }

        #[test]
        fn test_preorder() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            let result: Vec<_> = bst.preorder().cloned().collect();
            assert_eq!(result, vec![5, 3, 1, 4, 7, 6, 9]);
        }

        #[test]
        fn test_postorder() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            let result: Vec<_> = bst.postorder().cloned().collect();
            assert_eq!(result, vec![1, 4, 3, 6, 9, 7, 5]);
        }

        #[test]
        fn test_levelorder() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            let result: Vec<_> = bst.levelorder().cloned().collect();
            assert_eq!(result, vec![5, 3, 7, 1, 4, 6, 9]);
        }

        #[test]
        fn test_to_sorted_vec() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.to_sorted_vec(), vec![&1, &3, &5, &7, &9]);
        }

        #[test]
        fn test_empty_traversal() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert_eq!(bst.inorder().count(), 0);
            assert_eq!(bst.preorder().count(), 0);
            assert_eq!(bst.postorder().count(), 0);
            assert_eq!(bst.levelorder().count(), 0);
        }
    }

    mod height {
        use super::*;

        #[test]
        fn test_height_empty() {
            let bst: BinarySearchTree<i32> = BinarySearchTree::new();
            assert_eq!(bst.height(), 0);
        }

        #[test]
        fn test_height_single() {
            let bst = BinarySearchTree::from_slice(&[5]);
            assert_eq!(bst.height(), 1);
        }

        #[test]
        fn test_height_balanced() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert_eq!(bst.height(), 2);
        }

        #[test]
        fn test_height_unbalanced() {
            let mut bst = BinarySearchTree::new();
            for i in 1..=5 {
                bst.insert(i);
            }
            assert_eq!(bst.height(), 5); // Degenerate to linked list
        }
    }

    mod kth_smallest {
        use super::*;

        #[test]
        fn test_kth_smallest() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(bst.kth_smallest(1), Some(&1));
            assert_eq!(bst.kth_smallest(2), Some(&3));
            assert_eq!(bst.kth_smallest(3), Some(&5));
            assert_eq!(bst.kth_smallest(4), Some(&7));
            assert_eq!(bst.kth_smallest(5), Some(&9));
        }

        #[test]
        fn test_kth_smallest_invalid() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            assert_eq!(bst.kth_smallest(0), None);
            assert_eq!(bst.kth_smallest(4), None);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut bst = BinarySearchTree::from_slice(&[5, 3, 7]);
            bst.clear();
            assert!(bst.is_empty());
            assert_eq!(bst.height(), 0);
        }

        #[test]
        fn test_is_valid() {
            let bst = BinarySearchTree::from_slice(&[5, 3, 7, 1, 9]);
            assert!(bst.is_valid());
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let bst = BinarySearchTree::from_slice(&[42]);
            assert_eq!(bst.len(), 1);
            assert!(bst.contains(&42));
            assert_eq!(bst.min(), Some(&42));
            assert_eq!(bst.max(), Some(&42));
            assert_eq!(bst.height(), 1);
        }

        #[test]
        fn test_negative_values() {
            let bst = BinarySearchTree::from_slice(&[-5, -3, -7, -1, -9]);
            assert_eq!(bst.min(), Some(&-9));
            assert_eq!(bst.max(), Some(&-1));
            let sorted: Vec<_> = bst.inorder().cloned().collect();
            assert_eq!(sorted, vec![-9, -7, -5, -3, -1]);
        }

        #[test]
        fn test_strings() {
            let mut bst = BinarySearchTree::new();
            bst.insert("banana");
            bst.insert("apple");
            bst.insert("cherry");
            assert_eq!(bst.min(), Some(&"apple"));
            assert_eq!(bst.max(), Some(&"cherry"));
        }

        #[test]
        fn test_large_tree() {
            let mut bst = BinarySearchTree::new();
            for i in 0..1000 {
                bst.insert(i);
            }
            assert_eq!(bst.len(), 1000);
            assert!(bst.is_valid());
            assert_eq!(bst.min(), Some(&0));
            assert_eq!(bst.max(), Some(&999));
        }
    }
}
