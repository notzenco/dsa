//! # Red-Black Tree
//!
//! A self-balancing binary search tree with O(log n) operations.
//!
//! ## Properties
//!
//! 1. Every node is either red or black
//! 2. The root is black
//! 3. All leaves (NIL) are black
//! 4. If a node is red, both children are black
//! 5. Every path from root to leaves has the same number of black nodes
//!
//! ## Complexity Analysis
//!
//! | Operation | Time      | Space    |
//! |-----------|-----------|----------|
//! | Insert    | O(log n)  | O(1)     |
//! | Delete    | O(log n)  | O(1)     |
//! | Search    | O(log n)  | O(1)     |
//! | Space     | -         | O(n)     |
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::RedBlackTree;
//!
//! let mut tree = RedBlackTree::new();
//! tree.insert(10);
//! tree.insert(5);
//! tree.insert(15);
//!
//! assert!(tree.contains(&10));
//! assert_eq!(tree.len(), 3);
//! ```

use alloc::vec::Vec;

/// Node color in a Red-Black tree.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
enum Color {
    Red,
    Black,
}

/// A node in the Red-Black tree using arena allocation.
#[derive(Clone)]
struct Node<T: Clone> {
    value: T,
    color: Color,
    parent: Option<usize>,
    left: Option<usize>,
    right: Option<usize>,
}

/// A Red-Black tree implementation.
///
/// Uses arena allocation for safe memory management.
pub struct RedBlackTree<T: Ord + Clone> {
    nodes: Vec<Node<T>>,
    root: Option<usize>,
    len: usize,
}

impl<T: Ord + Clone> RedBlackTree<T> {
    /// Creates a new empty Red-Black tree.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::RedBlackTree;
    ///
    /// let tree: RedBlackTree<i32> = RedBlackTree::new();
    /// assert!(tree.is_empty());
    /// ```
    pub fn new() -> Self {
        RedBlackTree {
            nodes: Vec::new(),
            root: None,
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

    /// Returns `true` if the tree contains the value.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::RedBlackTree;
    ///
    /// let mut tree = RedBlackTree::new();
    /// tree.insert(5);
    /// assert!(tree.contains(&5));
    /// assert!(!tree.contains(&10));
    /// ```
    pub fn contains(&self, value: &T) -> bool {
        self.find_node(value).is_some()
    }

    /// Finds a node with the given value.
    fn find_node(&self, value: &T) -> Option<usize> {
        let mut current = self.root;
        while let Some(idx) = current {
            match value.cmp(&self.nodes[idx].value) {
                core::cmp::Ordering::Less => current = self.nodes[idx].left,
                core::cmp::Ordering::Greater => current = self.nodes[idx].right,
                core::cmp::Ordering::Equal => return Some(idx),
            }
        }
        None
    }

    /// Gets a reference to a value.
    pub fn get(&self, value: &T) -> Option<&T> {
        self.find_node(value).map(|idx| &self.nodes[idx].value)
    }

    /// Inserts a value. Returns `true` if inserted, `false` if already exists.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::RedBlackTree;
    ///
    /// let mut tree = RedBlackTree::new();
    /// assert!(tree.insert(5));
    /// assert!(!tree.insert(5)); // Already exists
    /// ```
    pub fn insert(&mut self, value: T) -> bool {
        // Find insertion point
        let mut parent = None;
        let mut current = self.root;
        let mut go_left = false;

        while let Some(idx) = current {
            parent = current;
            match value.cmp(&self.nodes[idx].value) {
                core::cmp::Ordering::Less => {
                    current = self.nodes[idx].left;
                    go_left = true;
                }
                core::cmp::Ordering::Greater => {
                    current = self.nodes[idx].right;
                    go_left = false;
                }
                core::cmp::Ordering::Equal => return false, // Already exists
            }
        }

        // Create new node
        let new_idx = self.nodes.len();
        self.nodes.push(Node {
            value,
            color: Color::Red,
            parent,
            left: None,
            right: None,
        });

        // Link to parent
        if let Some(p_idx) = parent {
            if go_left {
                self.nodes[p_idx].left = Some(new_idx);
            } else {
                self.nodes[p_idx].right = Some(new_idx);
            }
        } else {
            self.root = Some(new_idx);
        }

        self.len += 1;

        // Fix Red-Black properties
        self.insert_fixup(new_idx);

        true
    }

    /// Fixes Red-Black properties after insertion.
    fn insert_fixup(&mut self, mut idx: usize) {
        while let Some(p_idx) = self.nodes[idx].parent {
            if self.nodes[p_idx].color != Color::Red {
                break;
            }

            let gp_idx = match self.nodes[p_idx].parent {
                Some(gp) => gp,
                None => break,
            };

            if Some(p_idx) == self.nodes[gp_idx].left {
                // Parent is left child
                let uncle = self.nodes[gp_idx].right;

                if uncle.map_or(false, |u| self.nodes[u].color == Color::Red) {
                    // Case 1: Uncle is red
                    self.nodes[p_idx].color = Color::Black;
                    if let Some(u) = uncle {
                        self.nodes[u].color = Color::Black;
                    }
                    self.nodes[gp_idx].color = Color::Red;
                    idx = gp_idx;
                } else {
                    // Uncle is black
                    if Some(idx) == self.nodes[p_idx].right {
                        // Case 2: Node is right child
                        idx = p_idx;
                        self.rotate_left(idx);
                    }
                    // Case 3: Node is left child
                    let p_idx = self.nodes[idx].parent.unwrap();
                    let gp_idx = self.nodes[p_idx].parent.unwrap();
                    self.nodes[p_idx].color = Color::Black;
                    self.nodes[gp_idx].color = Color::Red;
                    self.rotate_right(gp_idx);
                }
            } else {
                // Parent is right child (mirror cases)
                let uncle = self.nodes[gp_idx].left;

                if uncle.map_or(false, |u| self.nodes[u].color == Color::Red) {
                    // Case 1: Uncle is red
                    self.nodes[p_idx].color = Color::Black;
                    if let Some(u) = uncle {
                        self.nodes[u].color = Color::Black;
                    }
                    self.nodes[gp_idx].color = Color::Red;
                    idx = gp_idx;
                } else {
                    // Uncle is black
                    if Some(idx) == self.nodes[p_idx].left {
                        // Case 2: Node is left child
                        idx = p_idx;
                        self.rotate_right(idx);
                    }
                    // Case 3: Node is right child
                    let p_idx = self.nodes[idx].parent.unwrap();
                    let gp_idx = self.nodes[p_idx].parent.unwrap();
                    self.nodes[p_idx].color = Color::Black;
                    self.nodes[gp_idx].color = Color::Red;
                    self.rotate_left(gp_idx);
                }
            }
        }

        // Root is always black
        if let Some(root_idx) = self.root {
            self.nodes[root_idx].color = Color::Black;
        }
    }

    /// Left rotation around node x.
    fn rotate_left(&mut self, x: usize) {
        let y = match self.nodes[x].right {
            Some(y) => y,
            None => return,
        };

        // Turn y's left subtree into x's right subtree
        self.nodes[x].right = self.nodes[y].left;
        if let Some(yl) = self.nodes[y].left {
            self.nodes[yl].parent = Some(x);
        }

        // Link x's parent to y
        self.nodes[y].parent = self.nodes[x].parent;
        if let Some(p) = self.nodes[x].parent {
            if Some(x) == self.nodes[p].left {
                self.nodes[p].left = Some(y);
            } else {
                self.nodes[p].right = Some(y);
            }
        } else {
            self.root = Some(y);
        }

        // Put x on y's left
        self.nodes[y].left = Some(x);
        self.nodes[x].parent = Some(y);
    }

    /// Right rotation around node x.
    fn rotate_right(&mut self, x: usize) {
        let y = match self.nodes[x].left {
            Some(y) => y,
            None => return,
        };

        // Turn y's right subtree into x's left subtree
        self.nodes[x].left = self.nodes[y].right;
        if let Some(yr) = self.nodes[y].right {
            self.nodes[yr].parent = Some(x);
        }

        // Link x's parent to y
        self.nodes[y].parent = self.nodes[x].parent;
        if let Some(p) = self.nodes[x].parent {
            if Some(x) == self.nodes[p].left {
                self.nodes[p].left = Some(y);
            } else {
                self.nodes[p].right = Some(y);
            }
        } else {
            self.root = Some(y);
        }

        // Put x on y's right
        self.nodes[y].right = Some(x);
        self.nodes[x].parent = Some(y);
    }

    /// Returns the minimum value.
    pub fn min(&self) -> Option<&T> {
        self.min_node(self.root).map(|idx| &self.nodes[idx].value)
    }

    /// Finds the minimum node starting from a given node.
    fn min_node(&self, mut node: Option<usize>) -> Option<usize> {
        let mut result = None;
        while let Some(idx) = node {
            result = Some(idx);
            node = self.nodes[idx].left;
        }
        result
    }

    /// Returns the maximum value.
    pub fn max(&self) -> Option<&T> {
        self.max_node(self.root).map(|idx| &self.nodes[idx].value)
    }

    /// Finds the maximum node starting from a given node.
    fn max_node(&self, mut node: Option<usize>) -> Option<usize> {
        let mut result = None;
        while let Some(idx) = node {
            result = Some(idx);
            node = self.nodes[idx].right;
        }
        result
    }

    /// Returns an in-order iterator.
    pub fn iter(&self) -> RedBlackTreeIter<'_, T> {
        RedBlackTreeIter {
            tree: self,
            stack: Vec::new(),
            current: self.root,
        }
    }

    /// Clears the tree.
    pub fn clear(&mut self) {
        self.nodes.clear();
        self.root = None;
        self.len = 0;
    }

    /// Returns the height of the tree (black height).
    pub fn black_height(&self) -> usize {
        let mut height = 0;
        let mut current = self.root;
        while let Some(idx) = current {
            if self.nodes[idx].color == Color::Black {
                height += 1;
            }
            current = self.nodes[idx].left;
        }
        height
    }

    /// Validates Red-Black tree properties (for testing).
    #[cfg(test)]
    fn is_valid(&self) -> bool {
        if self.root.is_none() {
            return true;
        }

        let root_idx = self.root.unwrap();

        // Property 2: Root is black
        if self.nodes[root_idx].color != Color::Black {
            return false;
        }

        // Check properties 4 and 5 recursively
        self.validate_node(self.root, 0, &mut None)
    }

    #[cfg(test)]
    fn validate_node(
        &self,
        node: Option<usize>,
        black_count: usize,
        expected_black: &mut Option<usize>,
    ) -> bool {
        match node {
            None => {
                // Property 5: All paths have same black count
                match expected_black {
                    Some(expected) => black_count == *expected,
                    None => {
                        *expected_black = Some(black_count);
                        true
                    }
                }
            }
            Some(idx) => {
                let node = &self.nodes[idx];
                let new_black_count = if node.color == Color::Black {
                    black_count + 1
                } else {
                    // Property 4: Red node must have black children
                    if let Some(left) = node.left {
                        if self.nodes[left].color == Color::Red {
                            return false;
                        }
                    }
                    if let Some(right) = node.right {
                        if self.nodes[right].color == Color::Red {
                            return false;
                        }
                    }
                    black_count
                };

                self.validate_node(node.left, new_black_count, expected_black)
                    && self.validate_node(node.right, new_black_count, expected_black)
            }
        }
    }
}

impl<T: Ord + Clone> Default for RedBlackTree<T> {
    fn default() -> Self {
        Self::new()
    }
}

/// In-order iterator for Red-Black tree.
pub struct RedBlackTreeIter<'a, T: Ord + Clone> {
    tree: &'a RedBlackTree<T>,
    stack: Vec<usize>,
    current: Option<usize>,
}

impl<'a, T: Ord + Clone> Iterator for RedBlackTreeIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        // Go left as far as possible
        while let Some(idx) = self.current {
            self.stack.push(idx);
            self.current = self.tree.nodes[idx].left;
        }

        // Pop from stack
        self.stack.pop().map(|idx| {
            self.current = self.tree.nodes[idx].right;
            &self.tree.nodes[idx].value
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
            let tree: RedBlackTree<i32> = RedBlackTree::new();
            assert!(tree.is_empty());
            assert_eq!(tree.len(), 0);
        }

        #[test]
        fn test_default() {
            let tree: RedBlackTree<i32> = RedBlackTree::default();
            assert!(tree.is_empty());
        }
    }

    mod insert {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut tree = RedBlackTree::new();
            assert!(tree.insert(5));
            assert!(tree.contains(&5));
            assert!(tree.is_valid());
        }

        #[test]
        fn test_insert_multiple() {
            let mut tree = RedBlackTree::new();
            tree.insert(10);
            tree.insert(5);
            tree.insert(15);
            tree.insert(3);
            tree.insert(7);

            assert!(tree.contains(&10));
            assert!(tree.contains(&5));
            assert!(tree.contains(&15));
            assert!(tree.contains(&3));
            assert!(tree.contains(&7));
            assert_eq!(tree.len(), 5);
            assert!(tree.is_valid());
        }

        #[test]
        fn test_insert_duplicate() {
            let mut tree = RedBlackTree::new();
            assert!(tree.insert(5));
            assert!(!tree.insert(5));
            assert_eq!(tree.len(), 1);
        }

        #[test]
        fn test_insert_ascending() {
            let mut tree = RedBlackTree::new();
            for i in 1..=10 {
                tree.insert(i);
            }
            assert_eq!(tree.len(), 10);
            assert!(tree.is_valid());
        }

        #[test]
        fn test_insert_descending() {
            let mut tree = RedBlackTree::new();
            for i in (1..=10).rev() {
                tree.insert(i);
            }
            assert_eq!(tree.len(), 10);
            assert!(tree.is_valid());
        }
    }

    mod contains_and_get {
        use super::*;

        #[test]
        fn test_contains() {
            let mut tree = RedBlackTree::new();
            tree.insert(5);
            tree.insert(10);
            tree.insert(3);

            assert!(tree.contains(&5));
            assert!(tree.contains(&10));
            assert!(tree.contains(&3));
            assert!(!tree.contains(&1));
            assert!(!tree.contains(&100));
        }

        #[test]
        fn test_get() {
            let mut tree = RedBlackTree::new();
            tree.insert(5);
            assert_eq!(tree.get(&5), Some(&5));
            assert_eq!(tree.get(&10), None);
        }
    }

    mod min_max {
        use super::*;

        #[test]
        fn test_min_max() {
            let mut tree = RedBlackTree::new();
            assert_eq!(tree.min(), None);
            assert_eq!(tree.max(), None);

            tree.insert(10);
            tree.insert(5);
            tree.insert(15);
            tree.insert(3);
            tree.insert(20);

            assert_eq!(tree.min(), Some(&3));
            assert_eq!(tree.max(), Some(&20));
        }
    }

    mod iter {
        use super::*;

        #[test]
        fn test_iter_inorder() {
            let mut tree = RedBlackTree::new();
            tree.insert(5);
            tree.insert(3);
            tree.insert(7);
            tree.insert(1);
            tree.insert(9);

            let values: Vec<_> = tree.iter().cloned().collect();
            assert_eq!(values, vec![1, 3, 5, 7, 9]);
        }

        #[test]
        fn test_iter_empty() {
            let tree: RedBlackTree<i32> = RedBlackTree::new();
            let values: Vec<_> = tree.iter().collect();
            assert!(values.is_empty());
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut tree = RedBlackTree::new();
            tree.insert(5);
            tree.insert(10);
            tree.clear();

            assert!(tree.is_empty());
            assert!(!tree.contains(&5));
        }
    }

    mod stress {
        use super::*;

        #[test]
        fn test_many_insertions() {
            let mut tree = RedBlackTree::new();
            for i in 0..100 {
                tree.insert(i);
            }

            assert_eq!(tree.len(), 100);
            assert!(tree.is_valid());

            for i in 0..100 {
                assert!(tree.contains(&i));
            }
        }

        #[test]
        fn test_random_insertions() {
            let mut tree = RedBlackTree::new();
            let values = [42, 17, 89, 3, 56, 91, 28, 64, 5, 73];

            for &v in &values {
                tree.insert(v);
            }

            assert_eq!(tree.len(), 10);
            assert!(tree.is_valid());

            let sorted: Vec<_> = tree.iter().cloned().collect();
            let mut expected = values.to_vec();
            expected.sort();
            assert_eq!(sorted, expected);
        }
    }

    mod black_height {
        use super::*;

        #[test]
        fn test_black_height() {
            let mut tree = RedBlackTree::new();
            assert_eq!(tree.black_height(), 0);

            for i in 1..=15 {
                tree.insert(i);
            }

            // Black height should be reasonable
            assert!(tree.black_height() > 0);
            assert!(tree.black_height() <= 5); // log2(15) + 1
        }
    }
}
