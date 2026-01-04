//! AVL Tree (Adelson-Velsky and Landis Tree)
//!
//! An AVL tree is a self-balancing binary search tree where the heights of
//! the two child subtrees of any node differ by at most one.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  AVL Tree with Balance Factors:                                            ║
//! ║                                                                            ║
//! ║                    ┌───────┐                                               ║
//! ║                    │ 10(0) │ ◀─── balance factor = height(R) - height(L)   ║
//! ║                    └───┬───┘                                               ║
//! ║             ┌──────────┴──────────┐                                        ║
//! ║             ▼                     ▼                                        ║
//! ║         ┌───────┐             ┌───────┐                                    ║
//! ║         │  5(0) │             │ 15(1) │                                    ║
//! ║         └───┬───┘             └───┬───┘                                    ║
//! ║        ┌────┴────┐                └────┐                                   ║
//! ║        ▼         ▼                     ▼                                   ║
//! ║    ┌───────┐ ┌───────┐             ┌───────┐                               ║
//! ║    │  2(0) │ │  7(0) │             │ 20(0) │                               ║
//! ║    └───────┘ └───────┘             └───────┘                               ║
//! ║                                                                            ║
//! ║  Rotations:                                                                ║
//! ║                                                                            ║
//! ║  Right Rotation (LL case):     Left Rotation (RR case):                    ║
//! ║       z                              z                                     ║
//! ║      / \                            / \                                    ║
//! ║     y   T4                         T1  y                                   ║
//! ║    / \       ──▶   y                  / \      ──▶     y                   ║
//! ║   x   T3          / \                T2  x            / \                  ║
//! ║  / \             x   z                  / \          z   x                 ║
//! ║ T1 T2           / \ / \                T3 T4        / \ / \                ║
//! ║               T1 T2 T3 T4                          T1 T2 T3 T4             ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation | Average   | Worst     | Space |
//! |-----------|-----------|-----------|-------|
//! | Insert    | O(log n)  | O(log n)  | O(1)  |
//! | Delete    | O(log n)  | O(log n)  | O(1)  |
//! | Search    | O(log n)  | O(log n)  | O(1)  |
//! | Min/Max   | O(log n)  | O(log n)  | O(1)  |
//! | Traversal | O(n)      | O(n)      | O(n)  |
//!
//! Unlike unbalanced BST, AVL guarantees O(log n) for all operations.
//!
//! ## LeetCode Problems
//!
//! - [#110 Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree/)
//! - [#108 Convert Sorted Array to BST](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/)
//! - [#1382 Balance a Binary Search Tree](https://leetcode.com/problems/balance-a-binary-search-tree/)
//!
//! ## Use Cases
//!
//! - Databases (index structures)
//! - In-memory ordered dictionaries
//! - When guaranteed O(log n) performance is required
//! - File systems (directory structure)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::AVLTree;
//!
//! let mut avl = AVLTree::new();
//! avl.insert(10);
//! avl.insert(5);
//! avl.insert(15);
//! avl.insert(2);
//! avl.insert(7);
//!
//! assert!(avl.contains(&7));
//! assert!(avl.is_balanced());
//! assert_eq!(avl.min(), Some(&2));
//!
//! let sorted: Vec<_> = avl.inorder().collect();
//! assert_eq!(sorted, vec![&2, &5, &7, &10, &15]);
//! ```

use alloc::boxed::Box;
use alloc::collections::VecDeque;
use alloc::vec::Vec;

use dsa_core::Container;

/// A node in the AVL tree.
#[derive(Debug, Clone)]
struct Node<T> {
    value: T,
    height: i32,
    left: Option<Box<Node<T>>>,
    right: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(value: T) -> Self {
        Node {
            value,
            height: 1,
            left: None,
            right: None,
        }
    }
}

/// An AVL tree implementation (self-balancing BST).
///
/// The tree maintains the AVL property: for every node, the heights of
/// the left and right subtrees differ by at most 1.
#[derive(Debug, Clone)]
pub struct AVLTree<T> {
    root: Option<Box<Node<T>>>,
    size: usize,
}

impl<T: Ord> AVLTree<T> {
    /// Creates a new empty AVL tree.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::AVLTree;
    ///
    /// let avl: AVLTree<i32> = AVLTree::new();
    /// assert!(avl.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        AVLTree {
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

    /// Returns the height of a node (or 0 for None).
    fn node_height(node: &Option<Box<Node<T>>>) -> i32 {
        node.as_ref().map_or(0, |n| n.height)
    }

    /// Returns the balance factor of a node.
    fn balance_factor(node: &Node<T>) -> i32 {
        Self::node_height(&node.right) - Self::node_height(&node.left)
    }

    /// Updates the height of a node based on its children.
    fn update_height(node: &mut Node<T>) {
        node.height = 1 + core::cmp::max(
            Self::node_height(&node.left),
            Self::node_height(&node.right),
        );
    }

    /// Right rotation (for LL case).
    fn rotate_right(mut y: Box<Node<T>>) -> Box<Node<T>> {
        let mut x = y.left.take().expect("Left child must exist for right rotation");
        y.left = x.right.take();
        Self::update_height(&mut y);
        x.right = Some(y);
        Self::update_height(&mut x);
        x
    }

    /// Left rotation (for RR case).
    fn rotate_left(mut y: Box<Node<T>>) -> Box<Node<T>> {
        let mut x = y.right.take().expect("Right child must exist for left rotation");
        y.right = x.left.take();
        Self::update_height(&mut y);
        x.left = Some(y);
        Self::update_height(&mut x);
        x
    }

    /// Rebalances a node if necessary.
    fn rebalance(mut node: Box<Node<T>>) -> Box<Node<T>> {
        Self::update_height(&mut node);
        let balance = Self::balance_factor(&node);

        // Left heavy
        if balance < -1 {
            if Self::balance_factor(node.left.as_ref().unwrap()) > 0 {
                // LR case: left rotate left child first
                node.left = Some(Self::rotate_left(node.left.take().unwrap()));
            }
            // LL case
            return Self::rotate_right(node);
        }

        // Right heavy
        if balance > 1 {
            if Self::balance_factor(node.right.as_ref().unwrap()) < 0 {
                // RL case: right rotate right child first
                node.right = Some(Self::rotate_right(node.right.take().unwrap()));
            }
            // RR case
            return Self::rotate_left(node);
        }

        node
    }

    /// Inserts a value into the tree.
    ///
    /// If the value already exists, it is not inserted (no duplicates).
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::AVLTree;
    ///
    /// let mut avl = AVLTree::new();
    /// avl.insert(10);
    /// avl.insert(5);
    /// avl.insert(15);
    /// assert_eq!(avl.len(), 3);
    /// assert!(avl.is_balanced());
    /// ```
    pub fn insert(&mut self, value: T) {
        let (new_root, inserted) = Self::insert_node(self.root.take(), value);
        self.root = new_root;
        if inserted {
            self.size += 1;
        }
    }

    fn insert_node(node: Option<Box<Node<T>>>, value: T) -> (Option<Box<Node<T>>>, bool) {
        match node {
            None => (Some(Box::new(Node::new(value))), true),
            Some(mut n) => {
                use core::cmp::Ordering;
                let inserted = match value.cmp(&n.value) {
                    Ordering::Less => {
                        let (new_left, ins) = Self::insert_node(n.left.take(), value);
                        n.left = new_left;
                        ins
                    }
                    Ordering::Greater => {
                        let (new_right, ins) = Self::insert_node(n.right.take(), value);
                        n.right = new_right;
                        ins
                    }
                    Ordering::Equal => false,
                };
                if inserted {
                    (Some(Self::rebalance(n)), true)
                } else {
                    (Some(n), false)
                }
            }
        }
    }

    /// Returns `true` if the tree contains the specified value.
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::AVLTree;
    ///
    /// let mut avl = AVLTree::new();
    /// avl.insert(5);
    /// assert!(avl.contains(&5));
    /// assert!(!avl.contains(&10));
    /// ```
    #[must_use]
    pub fn contains(&self, value: &T) -> bool {
        Self::search_node(&self.root, value).is_some()
    }

    /// Searches for a value and returns a reference to it if found.
    ///
    /// # Time Complexity
    /// O(log n)
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
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::AVLTree;
    ///
    /// let mut avl = AVLTree::new();
    /// avl.insert(5);
    /// avl.insert(3);
    /// avl.insert(7);
    ///
    /// assert!(avl.remove(&5));
    /// assert!(!avl.contains(&5));
    /// assert!(avl.is_balanced());
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
                        if removed {
                            (Some(Self::rebalance(n)), true)
                        } else {
                            (Some(n), false)
                        }
                    }
                    Ordering::Greater => {
                        let (new_right, removed) = Self::remove_node(n.right.take(), value);
                        n.right = new_right;
                        if removed {
                            (Some(Self::rebalance(n)), true)
                        } else {
                            (Some(n), false)
                        }
                    }
                    Ordering::Equal => {
                        match (n.left.take(), n.right.take()) {
                            (None, None) => (None, true),
                            (Some(left), None) => (Some(left), true),
                            (None, Some(right)) => (Some(right), true),
                            (Some(left), Some(right)) => {
                                let (new_right, successor_val) = Self::extract_min(right);
                                n.value = successor_val;
                                n.left = Some(left);
                                n.right = new_right;
                                (Some(Self::rebalance(n)), true)
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
                (Some(Self::rebalance(node)), min_val)
            }
        }
    }

    /// Returns a reference to the minimum value in the tree.
    ///
    /// # Time Complexity
    /// O(log n)
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
    /// O(log n)
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
    /// # Time Complexity
    /// O(1) - height is stored in nodes
    #[must_use]
    pub fn height(&self) -> usize {
        Self::node_height(&self.root) as usize
    }

    /// Checks if the tree is balanced (AVL property holds).
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn is_balanced(&self) -> bool {
        Self::check_balanced(&self.root)
    }

    fn check_balanced(node: &Option<Box<Node<T>>>) -> bool {
        match node {
            None => true,
            Some(n) => {
                let balance = Self::balance_factor(n);
                balance >= -1 && balance <= 1
                    && Self::check_balanced(&n.left)
                    && Self::check_balanced(&n.right)
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
    pub fn preorder(&self) -> PreorderIterator<'_, T> {
        PreorderIterator::new(&self.root)
    }

    /// Returns an iterator that performs level-order (BFS) traversal.
    pub fn levelorder(&self) -> LevelOrderIterator<'_, T> {
        LevelOrderIterator::new(&self.root)
    }

    /// Converts the tree to a sorted Vec (in-order traversal).
    #[must_use]
    pub fn to_sorted_vec(&self) -> Vec<&T> {
        self.inorder().collect()
    }

    /// Creates an AVL tree from a slice of values.
    #[must_use]
    pub fn from_slice(values: &[T]) -> Self
    where
        T: Clone,
    {
        let mut avl = AVLTree::new();
        for value in values {
            avl.insert(value.clone());
        }
        avl
    }

    /// Checks if the tree is a valid BST and maintains AVL property.
    #[must_use]
    pub fn is_valid(&self) -> bool {
        Self::is_valid_node(&self.root, None, None) && self.is_balanced()
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

impl<T: Ord> Default for AVLTree<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Ord> Container for AVLTree<T> {
    fn len(&self) -> usize {
        self.size
    }
}

impl<T: Ord> FromIterator<T> for AVLTree<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let mut avl = AVLTree::new();
        for value in iter {
            avl.insert(value);
        }
        avl
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
            let avl: AVLTree<i32> = AVLTree::new();
            assert!(avl.is_empty());
            assert_eq!(avl.len(), 0);
        }

        #[test]
        fn test_default() {
            let avl: AVLTree<i32> = AVLTree::default();
            assert!(avl.is_empty());
        }

        #[test]
        fn test_from_slice() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(avl.len(), 5);
            assert!(avl.is_valid());
        }

        #[test]
        fn test_from_iter() {
            let avl: AVLTree<i32> = (1..=5).collect();
            assert_eq!(avl.len(), 5);
            assert!(avl.is_valid());
        }
    }

    mod insert {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut avl = AVLTree::new();
            avl.insert(5);
            assert_eq!(avl.len(), 1);
            assert!(avl.contains(&5));
            assert!(avl.is_balanced());
        }

        #[test]
        fn test_insert_multiple() {
            let mut avl = AVLTree::new();
            avl.insert(5);
            avl.insert(3);
            avl.insert(7);
            avl.insert(1);
            avl.insert(9);
            assert_eq!(avl.len(), 5);
            assert!(avl.is_valid());
        }

        #[test]
        fn test_insert_duplicate() {
            let mut avl = AVLTree::new();
            avl.insert(5);
            avl.insert(5);
            assert_eq!(avl.len(), 1);
        }

        #[test]
        fn test_insert_ascending_stays_balanced() {
            let mut avl = AVLTree::new();
            for i in 1..=10 {
                avl.insert(i);
                assert!(avl.is_balanced(), "Tree unbalanced after inserting {}", i);
            }
            assert_eq!(avl.len(), 10);
            assert!(avl.is_valid());
        }

        #[test]
        fn test_insert_descending_stays_balanced() {
            let mut avl = AVLTree::new();
            for i in (1..=10).rev() {
                avl.insert(i);
                assert!(avl.is_balanced(), "Tree unbalanced after inserting {}", i);
            }
            assert_eq!(avl.len(), 10);
            assert!(avl.is_valid());
        }

        #[test]
        fn test_ll_rotation() {
            // Insert in descending order to trigger LL rotations
            let mut avl = AVLTree::new();
            avl.insert(30);
            avl.insert(20);
            avl.insert(10);
            assert!(avl.is_balanced());
            assert!(avl.is_valid());
        }

        #[test]
        fn test_rr_rotation() {
            // Insert in ascending order to trigger RR rotations
            let mut avl = AVLTree::new();
            avl.insert(10);
            avl.insert(20);
            avl.insert(30);
            assert!(avl.is_balanced());
            assert!(avl.is_valid());
        }

        #[test]
        fn test_lr_rotation() {
            let mut avl = AVLTree::new();
            avl.insert(30);
            avl.insert(10);
            avl.insert(20);
            assert!(avl.is_balanced());
            assert!(avl.is_valid());
        }

        #[test]
        fn test_rl_rotation() {
            let mut avl = AVLTree::new();
            avl.insert(10);
            avl.insert(30);
            avl.insert(20);
            assert!(avl.is_balanced());
            assert!(avl.is_valid());
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_contains() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert!(avl.contains(&5));
            assert!(avl.contains(&3));
            assert!(avl.contains(&7));
            assert!(!avl.contains(&4));
            assert!(!avl.contains(&10));
        }

        #[test]
        fn test_search() {
            let avl = AVLTree::from_slice(&[5, 3, 7]);
            assert_eq!(avl.search(&5), Some(&5));
            assert_eq!(avl.search(&3), Some(&3));
            assert_eq!(avl.search(&4), None);
        }

        #[test]
        fn test_search_empty() {
            let avl: AVLTree<i32> = AVLTree::new();
            assert_eq!(avl.search(&5), None);
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove_leaf() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7]);
            assert!(avl.remove(&3));
            assert!(!avl.contains(&3));
            assert_eq!(avl.len(), 2);
            assert!(avl.is_valid());
        }

        #[test]
        fn test_remove_one_child() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7, 1]);
            assert!(avl.remove(&3));
            assert!(!avl.contains(&3));
            assert!(avl.contains(&1));
            assert!(avl.is_valid());
        }

        #[test]
        fn test_remove_two_children() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            assert!(avl.remove(&5));
            assert!(!avl.contains(&5));
            assert!(avl.is_valid());
        }

        #[test]
        fn test_remove_root() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7]);
            assert!(avl.remove(&5));
            assert!(!avl.contains(&5));
            assert!(avl.is_valid());
        }

        #[test]
        fn test_remove_nonexistent() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7]);
            assert!(!avl.remove(&10));
            assert_eq!(avl.len(), 3);
        }

        #[test]
        fn test_remove_maintains_balance() {
            let mut avl = AVLTree::new();
            for i in 1..=20 {
                avl.insert(i);
            }

            for i in 1..=10 {
                assert!(avl.remove(&i));
                assert!(avl.is_balanced(), "Tree unbalanced after removing {}", i);
                assert!(avl.is_valid());
            }
        }

        #[test]
        fn test_remove_all() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7]);
            assert!(avl.remove(&5));
            assert!(avl.remove(&3));
            assert!(avl.remove(&7));
            assert!(avl.is_empty());
        }
    }

    mod min_max {
        use super::*;

        #[test]
        fn test_min() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(avl.min(), Some(&1));
        }

        #[test]
        fn test_max() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(avl.max(), Some(&9));
        }

        #[test]
        fn test_min_empty() {
            let avl: AVLTree<i32> = AVLTree::new();
            assert_eq!(avl.min(), None);
        }

        #[test]
        fn test_max_empty() {
            let avl: AVLTree<i32> = AVLTree::new();
            assert_eq!(avl.max(), None);
        }
    }

    mod traversal {
        use super::*;

        #[test]
        fn test_inorder() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
            let result: Vec<_> = avl.inorder().cloned().collect();
            assert_eq!(result, vec![1, 3, 4, 5, 6, 7, 9]);
        }

        #[test]
        fn test_to_sorted_vec() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert_eq!(avl.to_sorted_vec(), vec![&1, &3, &5, &7, &9]);
        }

        #[test]
        fn test_empty_traversal() {
            let avl: AVLTree<i32> = AVLTree::new();
            assert_eq!(avl.inorder().count(), 0);
            assert_eq!(avl.preorder().count(), 0);
            assert_eq!(avl.levelorder().count(), 0);
        }
    }

    mod height {
        use super::*;

        #[test]
        fn test_height_empty() {
            let avl: AVLTree<i32> = AVLTree::new();
            assert_eq!(avl.height(), 0);
        }

        #[test]
        fn test_height_single() {
            let avl = AVLTree::from_slice(&[5]);
            assert_eq!(avl.height(), 1);
        }

        #[test]
        fn test_height_balanced() {
            let avl = AVLTree::from_slice(&[5, 3, 7]);
            assert_eq!(avl.height(), 2);
        }

        #[test]
        fn test_height_many_elements() {
            let mut avl = AVLTree::new();
            for i in 1..=100 {
                avl.insert(i);
            }
            // AVL tree with 100 elements should have height around log2(100) ≈ 7
            let h = avl.height();
            assert!(h <= 10, "Height {} is too large for 100 elements", h);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut avl = AVLTree::from_slice(&[5, 3, 7]);
            avl.clear();
            assert!(avl.is_empty());
            assert_eq!(avl.height(), 0);
        }

        #[test]
        fn test_is_valid() {
            let avl = AVLTree::from_slice(&[5, 3, 7, 1, 9]);
            assert!(avl.is_valid());
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let avl = AVLTree::from_slice(&[42]);
            assert_eq!(avl.len(), 1);
            assert!(avl.contains(&42));
            assert_eq!(avl.min(), Some(&42));
            assert_eq!(avl.max(), Some(&42));
            assert!(avl.is_balanced());
        }

        #[test]
        fn test_negative_values() {
            let avl = AVLTree::from_slice(&[-5, -3, -7, -1, -9]);
            assert_eq!(avl.min(), Some(&-9));
            assert_eq!(avl.max(), Some(&-1));
            assert!(avl.is_valid());
        }

        #[test]
        fn test_large_tree() {
            let mut avl = AVLTree::new();
            for i in 0..1000 {
                avl.insert(i);
            }
            assert_eq!(avl.len(), 1000);
            assert!(avl.is_valid());
            assert_eq!(avl.min(), Some(&0));
            assert_eq!(avl.max(), Some(&999));
        }

        #[test]
        fn test_random_operations() {
            let mut avl = AVLTree::new();
            let values = [50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 78, 90];

            for &v in &values {
                avl.insert(v);
                assert!(avl.is_valid(), "Invalid after inserting {}", v);
            }

            for &v in &values[..7] {
                avl.remove(&v);
                assert!(avl.is_valid(), "Invalid after removing {}", v);
            }
        }
    }
}
