//! Heap data structures.
//!
//! This module contains implementations of heap data structures:
//!
//! - [`BinaryHeap`] - Binary min/max heap

pub mod binary_heap;

pub use binary_heap::{BinaryHeap, MinHeap, MaxHeap, HeapType};
