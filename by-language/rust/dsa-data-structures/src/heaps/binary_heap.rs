//! Binary Heap
//!
//! A binary heap is a complete binary tree that satisfies the heap property:
//! - Max-heap: parent >= children
//! - Min-heap: parent <= children
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Max-Heap:                          Min-Heap:                              ║
//! ║                                                                            ║
//! ║         ┌───┐                              ┌───┐                           ║
//! ║         │ 9 │                              │ 1 │                           ║
//! ║         └─┬─┘                              └─┬─┘                           ║
//! ║      ┌────┴────┐                       ┌────┴────┐                         ║
//! ║      ▼         ▼                       ▼         ▼                         ║
//! ║    ┌───┐     ┌───┐                   ┌───┐     ┌───┐                       ║
//! ║    │ 7 │     │ 8 │                   │ 3 │     │ 2 │                       ║
//! ║    └─┬─┘     └─┬─┘                   └─┬─┘     └─┬─┘                       ║
//! ║   ┌──┴──┐   ┌──┴──┐                 ┌──┴──┐   ┌──┴──┐                      ║
//! ║   ▼     ▼   ▼     ▼                 ▼     ▼   ▼     ▼                      ║
//! ║ ┌───┐ ┌───┐ ┌───┐ ┌───┐          ┌───┐ ┌───┐ ┌───┐ ┌───┐                  ║
//! ║ │ 5 │ │ 6 │ │ 3 │ │ 4 │          │ 7 │ │ 4 │ │ 5 │ │ 6 │                  ║
//! ║ └───┘ └───┘ └───┘ └───┘          └───┘ └───┘ └───┘ └───┘                  ║
//! ║                                                                            ║
//! ║  Array representation: [9, 7, 8, 5, 6, 3, 4]                              ║
//! ║  Parent of i: (i-1)/2   Children of i: 2i+1, 2i+2                         ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation  | Average   | Worst     | Space |
//! |------------|-----------|-----------|-------|
//! | Insert     | O(log n)  | O(log n)  | O(1)  |
//! | Extract    | O(log n)  | O(log n)  | O(1)  |
//! | Peek       | O(1)      | O(1)      | O(1)  |
//! | Heapify    | O(n)      | O(n)      | O(1)  |
//! | Search     | O(n)      | O(n)      | O(1)  |
//!
//! ## LeetCode Problems
//!
//! - [#215 Kth Largest Element](https://leetcode.com/problems/kth-largest-element-in-an-array/)
//! - [#347 Top K Frequent Elements](https://leetcode.com/problems/top-k-frequent-elements/)
//! - [#295 Find Median from Data Stream](https://leetcode.com/problems/find-median-from-data-stream/)
//! - [#23 Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists/)
//! - [#703 Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream/)
//! - [#1046 Last Stone Weight](https://leetcode.com/problems/last-stone-weight/)
//!
//! ## Use Cases
//!
//! - Priority queues
//! - Heap sort
//! - Graph algorithms (Dijkstra, Prim)
//! - Scheduling algorithms
//! - Median maintenance
//! - K-way merge
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::heaps::{MinHeap, MaxHeap};
//!
//! // Max heap
//! let mut max_heap = MaxHeap::new();
//! max_heap.push(3);
//! max_heap.push(1);
//! max_heap.push(4);
//! assert_eq!(max_heap.peek(), Some(&4));
//! assert_eq!(max_heap.pop(), Some(4));
//!
//! // Min heap
//! let mut min_heap = MinHeap::new();
//! min_heap.push(3);
//! min_heap.push(1);
//! min_heap.push(4);
//! assert_eq!(min_heap.peek(), Some(&1));
//! assert_eq!(min_heap.pop(), Some(1));
//! ```

use alloc::vec::Vec;
use core::cmp::Ordering;

use dsa_core::Container;

/// Comparison order for the heap.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HeapType {
    /// Max heap (largest element at root)
    Max,
    /// Min heap (smallest element at root)
    Min,
}

/// A binary heap implementation supporting both min and max variants.
#[derive(Debug, Clone)]
pub struct BinaryHeap<T> {
    data: Vec<T>,
    heap_type: HeapType,
}

impl<T: Ord> BinaryHeap<T> {
    /// Creates a new empty max heap.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::heaps::BinaryHeap;
    ///
    /// let heap: BinaryHeap<i32> = BinaryHeap::new();
    /// assert!(heap.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Self::new_max()
    }

    /// Creates a new empty max heap.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn new_max() -> Self {
        BinaryHeap {
            data: Vec::new(),
            heap_type: HeapType::Max,
        }
    }

    /// Creates a new empty min heap.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn new_min() -> Self {
        BinaryHeap {
            data: Vec::new(),
            heap_type: HeapType::Min,
        }
    }

    /// Creates a new heap with the specified capacity.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn with_capacity(capacity: usize, heap_type: HeapType) -> Self {
        BinaryHeap {
            data: Vec::with_capacity(capacity),
            heap_type,
        }
    }

    /// Creates a heap from a vector using heapify.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn from_vec(vec: Vec<T>, heap_type: HeapType) -> Self {
        let mut heap = BinaryHeap {
            data: vec,
            heap_type,
        };
        heap.heapify();
        heap
    }

    /// Returns the number of elements in the heap.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns `true` if the heap contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Returns the heap type (Min or Max).
    #[inline]
    #[must_use]
    pub fn heap_type(&self) -> HeapType {
        self.heap_type
    }

    /// Compares two elements according to heap type.
    fn compare(&self, a: &T, b: &T) -> Ordering {
        match self.heap_type {
            HeapType::Max => a.cmp(b),
            HeapType::Min => b.cmp(a),
        }
    }

    /// Returns the parent index of a node.
    #[inline]
    fn parent(i: usize) -> usize {
        (i - 1) / 2
    }

    /// Returns the left child index of a node.
    #[inline]
    fn left_child(i: usize) -> usize {
        2 * i + 1
    }

    /// Returns the right child index of a node.
    #[inline]
    fn right_child(i: usize) -> usize {
        2 * i + 2
    }

    /// Pushes an element onto the heap.
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::heaps::BinaryHeap;
    ///
    /// let mut heap = BinaryHeap::new_max();
    /// heap.push(3);
    /// heap.push(5);
    /// heap.push(1);
    /// assert_eq!(heap.peek(), Some(&5));
    /// ```
    pub fn push(&mut self, value: T) {
        self.data.push(value);
        self.sift_up(self.data.len() - 1);
    }

    /// Removes and returns the root element (max for max-heap, min for min-heap).
    ///
    /// # Time Complexity
    /// O(log n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::heaps::BinaryHeap;
    ///
    /// let mut heap = BinaryHeap::from_vec(vec![1, 5, 3], dsa_data_structures::heaps::HeapType::Max);
    /// assert_eq!(heap.pop(), Some(5));
    /// assert_eq!(heap.pop(), Some(3));
    /// assert_eq!(heap.pop(), Some(1));
    /// assert_eq!(heap.pop(), None);
    /// ```
    pub fn pop(&mut self) -> Option<T> {
        if self.is_empty() {
            return None;
        }

        let len = self.data.len();
        self.data.swap(0, len - 1);
        let result = self.data.pop();

        if !self.is_empty() {
            self.sift_down(0);
        }

        result
    }

    /// Returns a reference to the root element without removing it.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::heaps::BinaryHeap;
    ///
    /// let heap = BinaryHeap::from_vec(vec![1, 5, 3], dsa_data_structures::heaps::HeapType::Max);
    /// assert_eq!(heap.peek(), Some(&5));
    /// ```
    #[must_use]
    pub fn peek(&self) -> Option<&T> {
        self.data.first()
    }

    /// Returns a mutable reference to the root element.
    ///
    /// After modification, you may need to call `heapify()` to restore the heap property.
    #[must_use]
    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.data.first_mut()
    }

    /// Pushes an element and pops the root in one operation.
    ///
    /// More efficient than push + pop separately.
    ///
    /// # Time Complexity
    /// O(log n)
    pub fn push_pop(&mut self, mut value: T) -> T {
        if self.is_empty() || self.compare(&value, &self.data[0]) != Ordering::Less {
            return value;
        }

        core::mem::swap(&mut value, &mut self.data[0]);
        self.sift_down(0);
        value
    }

    /// Clears the heap, removing all elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Converts the heap into a sorted vector.
    ///
    /// # Time Complexity
    /// O(n log n)
    #[must_use]
    pub fn into_sorted_vec(mut self) -> Vec<T> {
        let mut result = Vec::with_capacity(self.len());
        while let Some(val) = self.pop() {
            result.push(val);
        }
        result
    }

    /// Restores the heap property (heapify).
    ///
    /// # Time Complexity
    /// O(n)
    fn heapify(&mut self) {
        let len = self.data.len();
        for i in (0..len / 2).rev() {
            self.sift_down(i);
        }
    }

    /// Moves an element up to maintain heap property.
    fn sift_up(&mut self, mut i: usize) {
        while i > 0 {
            let parent = Self::parent(i);
            if self.compare(&self.data[i], &self.data[parent]) == Ordering::Greater {
                self.data.swap(i, parent);
                i = parent;
            } else {
                break;
            }
        }
    }

    /// Moves an element down to maintain heap property.
    fn sift_down(&mut self, mut i: usize) {
        let len = self.data.len();
        loop {
            let left = Self::left_child(i);
            let right = Self::right_child(i);
            let mut largest = i;

            if left < len && self.compare(&self.data[left], &self.data[largest]) == Ordering::Greater {
                largest = left;
            }
            if right < len && self.compare(&self.data[right], &self.data[largest]) == Ordering::Greater {
                largest = right;
            }

            if largest != i {
                self.data.swap(i, largest);
                i = largest;
            } else {
                break;
            }
        }
    }

    /// Returns an iterator over the elements in arbitrary order.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter()
    }

    /// Returns the underlying vector.
    #[must_use]
    pub fn into_vec(self) -> Vec<T> {
        self.data
    }

    /// Checks if the heap property is maintained.
    #[must_use]
    pub fn is_valid(&self) -> bool {
        for i in 1..self.data.len() {
            let parent = Self::parent(i);
            if self.compare(&self.data[i], &self.data[parent]) == Ordering::Greater {
                return false;
            }
        }
        true
    }
}

impl<T: Ord> Default for BinaryHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Ord> Container for BinaryHeap<T> {
    fn len(&self) -> usize {
        self.data.len()
    }
}

impl<T: Ord> FromIterator<T> for BinaryHeap<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let vec: Vec<T> = iter.into_iter().collect();
        Self::from_vec(vec, HeapType::Max)
    }
}

/// A convenience type alias for a max heap.
pub type MaxHeap<T> = BinaryHeap<T>;

/// A min heap wrapper that provides a cleaner interface.
#[derive(Debug, Clone)]
pub struct MinHeap<T> {
    inner: BinaryHeap<T>,
}

impl<T: Ord> MinHeap<T> {
    /// Creates a new empty min heap.
    #[must_use]
    pub fn new() -> Self {
        MinHeap {
            inner: BinaryHeap::new_min(),
        }
    }

    /// Creates a min heap from a vector.
    #[must_use]
    pub fn from_vec(vec: Vec<T>) -> Self {
        MinHeap {
            inner: BinaryHeap::from_vec(vec, HeapType::Min),
        }
    }

    /// Returns the number of elements.
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.inner.len()
    }

    /// Returns `true` if empty.
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.inner.is_empty()
    }

    /// Pushes an element onto the heap.
    pub fn push(&mut self, value: T) {
        self.inner.push(value);
    }

    /// Removes and returns the minimum element.
    pub fn pop(&mut self) -> Option<T> {
        self.inner.pop()
    }

    /// Returns a reference to the minimum element.
    #[must_use]
    pub fn peek(&self) -> Option<&T> {
        self.inner.peek()
    }

    /// Clears the heap.
    pub fn clear(&mut self) {
        self.inner.clear();
    }
}

impl<T: Ord> Default for MinHeap<T> {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new_max() {
            let heap: BinaryHeap<i32> = BinaryHeap::new_max();
            assert!(heap.is_empty());
            assert_eq!(heap.heap_type(), HeapType::Max);
        }

        #[test]
        fn test_new_min() {
            let heap: BinaryHeap<i32> = BinaryHeap::new_min();
            assert!(heap.is_empty());
            assert_eq!(heap.heap_type(), HeapType::Min);
        }

        #[test]
        fn test_default() {
            let heap: BinaryHeap<i32> = BinaryHeap::default();
            assert!(heap.is_empty());
            assert_eq!(heap.heap_type(), HeapType::Max);
        }

        #[test]
        fn test_from_vec_max() {
            let heap = BinaryHeap::from_vec(vec![3, 1, 4, 1, 5], HeapType::Max);
            assert_eq!(heap.len(), 5);
            assert_eq!(heap.peek(), Some(&5));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_from_vec_min() {
            let heap = BinaryHeap::from_vec(vec![3, 1, 4, 1, 5], HeapType::Min);
            assert_eq!(heap.len(), 5);
            assert_eq!(heap.peek(), Some(&1));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_from_iter() {
            let heap: BinaryHeap<i32> = (1..=5).collect();
            assert_eq!(heap.len(), 5);
            assert_eq!(heap.peek(), Some(&5));
        }
    }

    mod push_pop {
        use super::*;

        #[test]
        fn test_push_max_heap() {
            let mut heap = BinaryHeap::new_max();
            heap.push(3);
            heap.push(5);
            heap.push(1);
            assert_eq!(heap.peek(), Some(&5));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_push_min_heap() {
            let mut heap = BinaryHeap::new_min();
            heap.push(3);
            heap.push(5);
            heap.push(1);
            assert_eq!(heap.peek(), Some(&1));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_pop_max_heap() {
            let mut heap = BinaryHeap::from_vec(vec![1, 5, 3, 7, 2], HeapType::Max);
            assert_eq!(heap.pop(), Some(7));
            assert_eq!(heap.pop(), Some(5));
            assert_eq!(heap.pop(), Some(3));
            assert_eq!(heap.pop(), Some(2));
            assert_eq!(heap.pop(), Some(1));
            assert_eq!(heap.pop(), None);
        }

        #[test]
        fn test_pop_min_heap() {
            let mut heap = BinaryHeap::from_vec(vec![1, 5, 3, 7, 2], HeapType::Min);
            assert_eq!(heap.pop(), Some(1));
            assert_eq!(heap.pop(), Some(2));
            assert_eq!(heap.pop(), Some(3));
            assert_eq!(heap.pop(), Some(5));
            assert_eq!(heap.pop(), Some(7));
            assert_eq!(heap.pop(), None);
        }

        #[test]
        fn test_push_pop_maintains_validity() {
            let mut heap = BinaryHeap::new_max();
            for i in [5, 3, 7, 1, 9, 2, 8, 4, 6] {
                heap.push(i);
                assert!(heap.is_valid());
            }
            while !heap.is_empty() {
                heap.pop();
                assert!(heap.is_valid());
            }
        }
    }

    mod peek {
        use super::*;

        #[test]
        fn test_peek() {
            let heap = BinaryHeap::from_vec(vec![1, 5, 3], HeapType::Max);
            assert_eq!(heap.peek(), Some(&5));
        }

        #[test]
        fn test_peek_empty() {
            let heap: BinaryHeap<i32> = BinaryHeap::new();
            assert_eq!(heap.peek(), None);
        }

        #[test]
        fn test_peek_mut() {
            let mut heap = BinaryHeap::from_vec(vec![1, 5, 3], HeapType::Max);
            if let Some(val) = heap.peek_mut() {
                *val = 10;
            }
            // Note: after mutation, heap property may be violated
            assert_eq!(heap.peek(), Some(&10));
        }
    }

    mod push_pop_combined {
        use super::*;

        #[test]
        fn test_push_pop_larger() {
            let mut heap = BinaryHeap::from_vec(vec![1, 5, 3], HeapType::Max);
            let result = heap.push_pop(10);
            assert_eq!(result, 10); // 10 is larger than max, returned immediately
            assert_eq!(heap.peek(), Some(&5));
        }

        #[test]
        fn test_push_pop_smaller() {
            let mut heap = BinaryHeap::from_vec(vec![1, 5, 3], HeapType::Max);
            let result = heap.push_pop(2);
            assert_eq!(result, 5); // 5 was the max
            assert_eq!(heap.peek(), Some(&3));
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut heap = BinaryHeap::from_vec(vec![1, 2, 3], HeapType::Max);
            heap.clear();
            assert!(heap.is_empty());
        }

        #[test]
        fn test_into_sorted_vec() {
            let heap = BinaryHeap::from_vec(vec![3, 1, 4, 1, 5], HeapType::Max);
            let sorted = heap.into_sorted_vec();
            assert_eq!(sorted, vec![5, 4, 3, 1, 1]);
        }

        #[test]
        fn test_into_vec() {
            let heap = BinaryHeap::from_vec(vec![3, 1, 4], HeapType::Max);
            let vec = heap.into_vec();
            assert_eq!(vec.len(), 3);
        }

        #[test]
        fn test_iter() {
            let heap = BinaryHeap::from_vec(vec![1, 2, 3], HeapType::Max);
            let sum: i32 = heap.iter().sum();
            assert_eq!(sum, 6);
        }
    }

    mod min_heap_wrapper {
        use super::*;

        #[test]
        fn test_min_heap_new() {
            let heap: MinHeap<i32> = MinHeap::new();
            assert!(heap.is_empty());
        }

        #[test]
        fn test_min_heap_push_pop() {
            let mut heap = MinHeap::new();
            heap.push(3);
            heap.push(1);
            heap.push(4);
            assert_eq!(heap.peek(), Some(&1));
            assert_eq!(heap.pop(), Some(1));
            assert_eq!(heap.pop(), Some(3));
        }

        #[test]
        fn test_min_heap_from_vec() {
            let heap = MinHeap::from_vec(vec![5, 3, 7, 1, 9]);
            assert_eq!(heap.peek(), Some(&1));
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let mut heap = BinaryHeap::from_vec(vec![42], HeapType::Max);
            assert_eq!(heap.peek(), Some(&42));
            assert_eq!(heap.pop(), Some(42));
            assert!(heap.is_empty());
        }

        #[test]
        fn test_duplicates() {
            let mut heap = BinaryHeap::from_vec(vec![5, 5, 5, 5], HeapType::Max);
            assert_eq!(heap.pop(), Some(5));
            assert_eq!(heap.pop(), Some(5));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_negative_values() {
            let mut heap = BinaryHeap::from_vec(vec![-5, -3, -7, -1, -9], HeapType::Max);
            assert_eq!(heap.pop(), Some(-1));
            assert_eq!(heap.pop(), Some(-3));
        }

        #[test]
        fn test_large_heap() {
            let mut heap = BinaryHeap::new_max();
            for i in 0..1000 {
                heap.push(i);
            }
            assert_eq!(heap.len(), 1000);
            assert_eq!(heap.peek(), Some(&999));
            assert!(heap.is_valid());
        }

        #[test]
        fn test_ascending_order() {
            let mut heap = BinaryHeap::new_max();
            for i in 1..=10 {
                heap.push(i);
            }
            let mut prev = heap.pop().unwrap();
            while let Some(val) = heap.pop() {
                assert!(prev >= val);
                prev = val;
            }
        }

        #[test]
        fn test_descending_order() {
            let mut heap = BinaryHeap::new_max();
            for i in (1..=10).rev() {
                heap.push(i);
            }
            let mut prev = heap.pop().unwrap();
            while let Some(val) = heap.pop() {
                assert!(prev >= val);
                prev = val;
            }
        }
    }
}
