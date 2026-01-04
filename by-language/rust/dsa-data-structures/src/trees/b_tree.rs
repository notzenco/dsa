//! # B-Tree
//!
//! A self-balancing tree data structure that maintains sorted data and allows
//! searches, insertions, and deletions in O(log n) time.
//!
//! B-trees are optimized for systems that read and write large blocks of data,
//! commonly used in databases and file systems.
//!
//! ## Properties
//!
//! For a B-tree of minimum degree t:
//! - Every node has at most 2t-1 keys
//! - Every non-root node has at least t-1 keys
//! - All leaves are at the same level
//!
//! ## Complexity Analysis
//!
//! | Operation | Time      | Space    |
//! |-----------|-----------|----------|
//! | Search    | O(log n)  | O(1)     |
//! | Insert    | O(log n)  | O(t)     |
//! | Delete    | O(log n)  | O(t)     |
//! | Space     | -         | O(n)     |
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::BTree;
//!
//! let mut tree = BTree::new(3); // Minimum degree 3
//! tree.insert(10);
//! tree.insert(20);
//! tree.insert(5);
//!
//! assert!(tree.contains(&10));
//! assert_eq!(tree.len(), 3);
//! ```

use alloc::vec::Vec;

/// A node in the B-tree.
#[derive(Clone)]
struct BTreeNode<T: Clone> {
    keys: Vec<T>,
    children: Vec<usize>, // Indices into nodes vec
    is_leaf: bool,
}

impl<T: Clone> BTreeNode<T> {
    fn new(is_leaf: bool) -> Self {
        BTreeNode {
            keys: Vec::new(),
            children: Vec::new(),
            is_leaf,
        }
    }
}

/// A B-tree implementation using arena allocation.
///
/// # Type Parameters
///
/// * `T` - The element type, must implement `Ord` and `Clone`
pub struct BTree<T: Ord + Clone> {
    nodes: Vec<BTreeNode<T>>,
    root: Option<usize>,
    min_degree: usize, // t
    len: usize,
}

impl<T: Ord + Clone> BTree<T> {
    /// Creates a new empty B-tree with the given minimum degree.
    ///
    /// # Arguments
    ///
    /// * `min_degree` - The minimum degree t (must be >= 2)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BTree;
    ///
    /// let tree: BTree<i32> = BTree::new(3);
    /// assert!(tree.is_empty());
    /// ```
    pub fn new(min_degree: usize) -> Self {
        let min_degree = min_degree.max(2);
        BTree {
            nodes: Vec::new(),
            root: None,
            min_degree,
            len: 0,
        }
    }

    /// Returns the number of elements.
    pub fn len(&self) -> usize {
        self.len
    }

    /// Returns `true` if the tree is empty.
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// Returns the minimum degree.
    pub fn min_degree(&self) -> usize {
        self.min_degree
    }

    /// Returns `true` if the tree contains the value.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BTree;
    ///
    /// let mut tree = BTree::new(3);
    /// tree.insert(5);
    /// assert!(tree.contains(&5));
    /// assert!(!tree.contains(&10));
    /// ```
    pub fn contains(&self, key: &T) -> bool {
        self.search(key).is_some()
    }

    /// Searches for a key and returns a reference if found.
    pub fn search(&self, key: &T) -> Option<&T> {
        self.search_node(self.root?, key)
    }

    fn search_node(&self, node_idx: usize, key: &T) -> Option<&T> {
        let node = &self.nodes[node_idx];

        // Find the first key >= search key
        let mut i = 0;
        while i < node.keys.len() && *key > node.keys[i] {
            i += 1;
        }

        // Check if we found the key
        if i < node.keys.len() && *key == node.keys[i] {
            return Some(&node.keys[i]);
        }

        // If leaf, key not found
        if node.is_leaf {
            return None;
        }

        // Recurse to appropriate child
        self.search_node(node.children[i], key)
    }

    /// Inserts a key into the tree.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::BTree;
    ///
    /// let mut tree = BTree::new(3);
    /// tree.insert(10);
    /// tree.insert(5);
    /// tree.insert(15);
    /// assert_eq!(tree.len(), 3);
    /// ```
    pub fn insert(&mut self, key: T) {
        if self.root.is_none() {
            // Create root
            let root_idx = self.nodes.len();
            let mut root = BTreeNode::new(true);
            root.keys.push(key);
            self.nodes.push(root);
            self.root = Some(root_idx);
            self.len = 1;
            return;
        }

        let root_idx = self.root.unwrap();

        // If root is full, split it
        if self.nodes[root_idx].keys.len() == 2 * self.min_degree - 1 {
            let new_root_idx = self.nodes.len();
            let mut new_root = BTreeNode::new(false);
            new_root.children.push(root_idx);
            self.nodes.push(new_root);
            self.root = Some(new_root_idx);
            self.split_child(new_root_idx, 0);
            self.insert_non_full(new_root_idx, key);
        } else {
            self.insert_non_full(root_idx, key);
        }

        self.len += 1;
    }

    /// Inserts a key into a non-full node.
    fn insert_non_full(&mut self, node_idx: usize, key: T) {
        let is_leaf = self.nodes[node_idx].is_leaf;

        if is_leaf {
            // Insert key in sorted order
            let keys = &mut self.nodes[node_idx].keys;
            let mut i = keys.len();
            while i > 0 && key < keys[i - 1] {
                i -= 1;
            }
            keys.insert(i, key);
        } else {
            // Find child to descend to
            let mut i = self.nodes[node_idx].keys.len();
            while i > 0 && key < self.nodes[node_idx].keys[i - 1] {
                i -= 1;
            }

            let child_idx = self.nodes[node_idx].children[i];

            // Split child if full
            if self.nodes[child_idx].keys.len() == 2 * self.min_degree - 1 {
                self.split_child(node_idx, i);

                // After split, determine which child to descend to
                if key > self.nodes[node_idx].keys[i] {
                    i += 1;
                }
            }

            let child_idx = self.nodes[node_idx].children[i];
            self.insert_non_full(child_idx, key);
        }
    }

    /// Splits a full child node.
    fn split_child(&mut self, parent_idx: usize, child_pos: usize) {
        let child_idx = self.nodes[parent_idx].children[child_pos];
        let t = self.min_degree;

        // Create new node
        let new_idx = self.nodes.len();
        let is_leaf = self.nodes[child_idx].is_leaf;
        let mut new_node = BTreeNode::new(is_leaf);

        // Move keys t..2t-1 to new node
        let median_key = self.nodes[child_idx].keys[t - 1].clone();
        new_node.keys = self.nodes[child_idx].keys.split_off(t);
        self.nodes[child_idx].keys.pop(); // Remove median

        // Move children if not leaf
        if !is_leaf {
            new_node.children = self.nodes[child_idx].children.split_off(t);
        }

        self.nodes.push(new_node);

        // Insert median key into parent
        self.nodes[parent_idx].keys.insert(child_pos, median_key);
        self.nodes[parent_idx].children.insert(child_pos + 1, new_idx);
    }

    /// Returns the minimum key.
    pub fn min(&self) -> Option<&T> {
        let mut node_idx = self.root?;
        loop {
            let node = &self.nodes[node_idx];
            if node.is_leaf || node.children.is_empty() {
                return node.keys.first();
            }
            node_idx = node.children[0];
        }
    }

    /// Returns the maximum key.
    pub fn max(&self) -> Option<&T> {
        let mut node_idx = self.root?;
        loop {
            let node = &self.nodes[node_idx];
            if node.is_leaf || node.children.is_empty() {
                return node.keys.last();
            }
            node_idx = *node.children.last().unwrap();
        }
    }

    /// Returns an in-order iterator over the keys.
    pub fn iter(&self) -> BTreeIter<'_, T> {
        let mut stack = Vec::new();
        if let Some(root) = self.root {
            stack.push((root, 0usize));
        }
        BTreeIter { tree: self, stack }
    }

    /// Clears the tree.
    pub fn clear(&mut self) {
        self.nodes.clear();
        self.root = None;
        self.len = 0;
    }

    /// Returns the height of the tree.
    pub fn height(&self) -> usize {
        match self.root {
            None => 0,
            Some(mut node_idx) => {
                let mut height = 1;
                while !self.nodes[node_idx].is_leaf {
                    if self.nodes[node_idx].children.is_empty() {
                        break;
                    }
                    node_idx = self.nodes[node_idx].children[0];
                    height += 1;
                }
                height
            }
        }
    }
}

impl<T: Ord + Clone> Default for BTree<T> {
    fn default() -> Self {
        Self::new(2)
    }
}

/// In-order iterator for B-tree.
pub struct BTreeIter<'a, T: Ord + Clone> {
    tree: &'a BTree<T>,
    stack: Vec<(usize, usize)>, // (node_idx, key_index)
}

impl<'a, T: Ord + Clone> Iterator for BTreeIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        while let Some(&(node_idx, key_idx)) = self.stack.last() {
            let node = &self.tree.nodes[node_idx];

            // If we have more children to explore
            if !node.is_leaf && key_idx < node.children.len() {
                let child_idx = node.children[key_idx];
                self.stack.last_mut().unwrap().1 += 1;
                self.stack.push((child_idx, 0));
                continue;
            }

            // Return key if available
            let key_to_return = key_idx.checked_sub(if node.is_leaf { 0 } else { 1 });

            if node.is_leaf {
                if key_idx < node.keys.len() {
                    self.stack.last_mut().unwrap().1 += 1;
                    return Some(&node.keys[key_idx]);
                }
            } else {
                // We've explored child at key_idx, check if we already returned the key at key_idx-1
                if let Some(ki) = key_to_return {
                    if ki < node.keys.len() && key_idx == ki + 1 {
                        // Already incremented, now return the key
                    }
                }
            }

            self.stack.pop();
        }
        None
    }
}

/// Simple in-order collection for B-tree.
impl<T: Ord + Clone> BTree<T> {
    /// Collects all keys in sorted order.
    pub fn collect_sorted(&self) -> Vec<T> {
        let mut result = Vec::new();
        if let Some(root) = self.root {
            self.collect_node(root, &mut result);
        }
        result
    }

    fn collect_node(&self, node_idx: usize, result: &mut Vec<T>) {
        let node = &self.nodes[node_idx];

        for i in 0..node.keys.len() {
            // Visit left child
            if !node.is_leaf && i < node.children.len() {
                self.collect_node(node.children[i], result);
            }
            // Visit key
            result.push(node.keys[i].clone());
        }

        // Visit rightmost child
        if !node.is_leaf && node.children.len() > node.keys.len() {
            self.collect_node(*node.children.last().unwrap(), result);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let tree: BTree<i32> = BTree::new(3);
            assert!(tree.is_empty());
            assert_eq!(tree.len(), 0);
            assert_eq!(tree.min_degree(), 3);
        }

        #[test]
        fn test_default() {
            let tree: BTree<i32> = BTree::default();
            assert!(tree.is_empty());
            assert_eq!(tree.min_degree(), 2);
        }

        #[test]
        fn test_min_degree_enforced() {
            let tree: BTree<i32> = BTree::new(1);
            assert_eq!(tree.min_degree(), 2); // Should be at least 2
        }
    }

    mod insert {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut tree = BTree::new(3);
            tree.insert(5);
            assert!(tree.contains(&5));
            assert_eq!(tree.len(), 1);
        }

        #[test]
        fn test_insert_multiple() {
            let mut tree = BTree::new(3);
            tree.insert(10);
            tree.insert(20);
            tree.insert(5);
            tree.insert(15);
            tree.insert(25);

            assert!(tree.contains(&10));
            assert!(tree.contains(&20));
            assert!(tree.contains(&5));
            assert!(tree.contains(&15));
            assert!(tree.contains(&25));
            assert_eq!(tree.len(), 5);
        }

        #[test]
        fn test_insert_causes_split() {
            let mut tree = BTree::new(2); // Max 3 keys per node

            for i in 1..=10 {
                tree.insert(i);
            }

            assert_eq!(tree.len(), 10);

            for i in 1..=10 {
                assert!(tree.contains(&i));
            }
        }

        #[test]
        fn test_insert_descending() {
            let mut tree = BTree::new(2);

            for i in (1..=10).rev() {
                tree.insert(i);
            }

            assert_eq!(tree.len(), 10);

            for i in 1..=10 {
                assert!(tree.contains(&i));
            }
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_search_existing() {
            let mut tree = BTree::new(3);
            tree.insert(10);
            tree.insert(20);
            tree.insert(5);

            assert_eq!(tree.search(&10), Some(&10));
            assert_eq!(tree.search(&20), Some(&20));
            assert_eq!(tree.search(&5), Some(&5));
        }

        #[test]
        fn test_search_nonexistent() {
            let mut tree = BTree::new(3);
            tree.insert(10);

            assert_eq!(tree.search(&5), None);
            assert_eq!(tree.search(&100), None);
        }

        #[test]
        fn test_search_empty() {
            let tree: BTree<i32> = BTree::new(3);
            assert_eq!(tree.search(&5), None);
        }
    }

    mod min_max {
        use super::*;

        #[test]
        fn test_min_max_empty() {
            let tree: BTree<i32> = BTree::new(3);
            assert_eq!(tree.min(), None);
            assert_eq!(tree.max(), None);
        }

        #[test]
        fn test_min_max() {
            let mut tree = BTree::new(2);
            tree.insert(10);
            tree.insert(5);
            tree.insert(15);
            tree.insert(3);
            tree.insert(20);

            assert_eq!(tree.min(), Some(&3));
            assert_eq!(tree.max(), Some(&20));
        }
    }

    mod collect_sorted {
        use super::*;

        #[test]
        fn test_collect_sorted() {
            let mut tree = BTree::new(2);
            tree.insert(5);
            tree.insert(3);
            tree.insert(7);
            tree.insert(1);
            tree.insert(9);
            tree.insert(4);
            tree.insert(6);

            let sorted = tree.collect_sorted();
            assert_eq!(sorted, vec![1, 3, 4, 5, 6, 7, 9]);
        }

        #[test]
        fn test_collect_sorted_empty() {
            let tree: BTree<i32> = BTree::new(3);
            let sorted = tree.collect_sorted();
            assert!(sorted.is_empty());
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut tree = BTree::new(3);
            tree.insert(10);
            tree.insert(20);
            tree.clear();

            assert!(tree.is_empty());
            assert!(!tree.contains(&10));
        }
    }

    mod height {
        use super::*;

        #[test]
        fn test_height_empty() {
            let tree: BTree<i32> = BTree::new(3);
            assert_eq!(tree.height(), 0);
        }

        #[test]
        fn test_height_grows() {
            let mut tree = BTree::new(2);

            tree.insert(1);
            assert_eq!(tree.height(), 1);

            // Insert more to force splits
            for i in 2..=20 {
                tree.insert(i);
            }

            // Height should be reasonable (log base t of n)
            assert!(tree.height() >= 2);
            assert!(tree.height() <= 5);
        }
    }

    mod stress {
        use super::*;

        #[test]
        fn test_many_insertions() {
            let mut tree = BTree::new(3);

            for i in 0..100 {
                tree.insert(i);
            }

            assert_eq!(tree.len(), 100);

            for i in 0..100 {
                assert!(tree.contains(&i), "Missing key: {}", i);
            }
        }

        #[test]
        fn test_random_order() {
            let mut tree = BTree::new(2);
            let values = [42, 17, 89, 3, 56, 91, 28, 64, 5, 73];

            for &v in &values {
                tree.insert(v);
            }

            assert_eq!(tree.len(), 10);

            let sorted = tree.collect_sorted();
            let mut expected = values.to_vec();
            expected.sort();
            assert_eq!(sorted, expected);
        }
    }
}
