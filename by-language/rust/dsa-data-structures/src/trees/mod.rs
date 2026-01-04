//! Tree data structures.
//!
//! This module contains implementations of tree data structures:
//!
//! - [`BinarySearchTree`] - Basic binary search tree
//! - `AVLTree` - Self-balancing AVL tree
//! - `RedBlackTree` - Self-balancing red-black tree
//! - `BTree` - B-tree for disk-based storage
//! - `Trie` - Prefix tree for string operations
//! - `SegmentTree` - Range query data structure
//! - `FenwickTree` - Binary indexed tree for prefix sums

pub mod avl_tree;
pub mod b_tree;
pub mod binary_search_tree;
pub mod fenwick_tree;
pub mod red_black_tree;
pub mod segment_tree;
pub mod trie;

pub use avl_tree::AVLTree;
pub use b_tree::BTree;
pub use binary_search_tree::BinarySearchTree;
pub use fenwick_tree::{FenwickTree, FenwickTree2D};
pub use red_black_tree::RedBlackTree;
pub use segment_tree::{
    max_segment_tree, min_segment_tree, sum_segment_tree, LazySegmentTree, SegmentTree,
};
pub use trie::Trie;
