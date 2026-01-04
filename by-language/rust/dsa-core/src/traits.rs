//! Core traits for data structures and algorithms.
//!
//! These traits define common interfaces that data structures implement,
//! enabling generic programming and consistent APIs across implementations.

use core::cmp::Ordering;

/// A trait for data structures that have a length/size.
pub trait Container {
    /// Returns the number of elements in the container.
    fn len(&self) -> usize;

    /// Returns `true` if the container contains no elements.
    fn is_empty(&self) -> bool {
        self.len() == 0
    }
}

/// A trait for data structures that support search operations.
pub trait Searchable<T> {
    /// Searches for a value and returns its index if found.
    fn search(&self, value: &T) -> Option<usize>;

    /// Returns `true` if the container contains the specified value.
    fn contains(&self, value: &T) -> bool {
        self.search(value).is_some()
    }
}

/// A trait for ordered data structures (e.g., BST, Heap).
pub trait Ordered<T: Ord>: Container {
    /// Returns a reference to the minimum element.
    fn min(&self) -> Option<&T>;

    /// Returns a reference to the maximum element.
    fn max(&self) -> Option<&T>;
}

/// A trait for tree-like data structures.
pub trait Tree<T> {
    /// Returns the height of the tree.
    fn height(&self) -> usize;

    /// Returns `true` if the tree is balanced.
    fn is_balanced(&self) -> bool;

    /// Returns the number of nodes in the tree.
    fn size(&self) -> usize;
}

/// A trait for sorting algorithms.
pub trait Sorter<T: Ord> {
    /// Sorts the slice in ascending order.
    fn sort(&self, data: &mut [T]);

    /// Sorts the slice using a custom comparator.
    fn sort_by<F>(&self, data: &mut [T], compare: F)
    where
        F: FnMut(&T, &T) -> Ordering;

    /// Returns `true` if this is a stable sorting algorithm.
    fn is_stable(&self) -> bool;

    /// Returns the name of the sorting algorithm.
    fn name(&self) -> &'static str;
}

/// A trait for graph data structures.
pub trait Graph {
    /// Returns the number of vertices in the graph.
    fn vertex_count(&self) -> usize;

    /// Returns the number of edges in the graph.
    fn edge_count(&self) -> usize;

    /// Returns `true` if the graph is directed.
    fn is_directed(&self) -> bool;

    /// Returns `true` if there is an edge from `u` to `v`.
    fn has_edge(&self, u: usize, v: usize) -> bool;

    /// Returns the neighbors of vertex `v`.
    fn neighbors(&self, v: usize) -> Vec<usize>;
}

/// A trait for cache data structures.
pub trait Cache<K, V> {
    /// Gets a value from the cache.
    fn get(&mut self, key: &K) -> Option<&V>;

    /// Puts a key-value pair into the cache.
    fn put(&mut self, key: K, value: V);

    /// Returns the capacity of the cache.
    fn capacity(&self) -> usize;

    /// Returns the current number of items in the cache.
    fn len(&self) -> usize;

    /// Returns `true` if the cache is empty.
    fn is_empty(&self) -> bool {
        self.len() == 0
    }
}
