//! Dynamic Array (Resizable Array / ArrayList)
//!
//! A resizable array implementation that automatically grows and shrinks.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Logical View (size=5):                                                    ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┐                                           ║
//! ║  │  10 │  20 │  30 │  40 │  50 │                                           ║
//! ║  └─────┴─────┴─────┴─────┴─────┘                                           ║
//! ║    [0]   [1]   [2]   [3]   [4]                                             ║
//! ║                                                                            ║
//! ║  Physical View (capacity=8):                                               ║
//! ║  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐                         ║
//! ║  │  10 │  20 │  30 │  40 │  50 │  -  │  -  │  -  │                         ║
//! ║  └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘                         ║
//! ║    [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]                           ║
//! ║                                  ↑                                         ║
//! ║                            unused capacity                                 ║
//! ║                                                                            ║
//! ║  Resize Operation (when size == capacity):                                 ║
//! ║  Before: [1][2][3][4] → capacity: 4, size: 4                               ║
//! ║  After:  [1][2][3][4][-][-][-][-] → capacity: 8, size: 4                   ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation           | Average     | Worst       | Space       |
//! |---------------------|-------------|-------------|-------------|
//! | Access by index     | O(1)        | O(1)        | O(1)        |
//! | Search              | O(n)        | O(n)        | O(1)        |
//! | Insert at end       | O(1)*       | O(n)        | O(1)        |
//! | Insert at index     | O(n)        | O(n)        | O(1)        |
//! | Delete at end       | O(1)        | O(1)        | O(1)        |
//! | Delete at index     | O(n)        | O(n)        | O(1)        |
//!
//! *Amortized O(1) due to occasional resizing
//!
//! ## LeetCode Problems
//!
//! - [#1 Two Sum](https://leetcode.com/problems/two-sum/) (array traversal)
//! - [#26 Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array/)
//! - [#27 Remove Element](https://leetcode.com/problems/remove-element/)
//! - [#88 Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array/)
//! - [#283 Move Zeroes](https://leetcode.com/problems/move-zeroes/)
//! - [#448 Find All Numbers Disappeared in an Array](https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array/)
//!
//! ## Use Cases
//!
//! - When you need fast random access by index
//! - When most operations are at the end of the list
//! - Implementing stacks, queues, and other data structures
//! - Storing collections where the size changes dynamically
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::linear::DynamicArray;
//!
//! let mut arr = DynamicArray::new();
//! arr.push(10);
//! arr.push(20);
//! arr.push(30);
//!
//! assert_eq!(arr.len(), 3);
//! assert_eq!(arr.get(0), Some(&10));
//! assert_eq!(arr.pop(), Some(30));
//! ```

use alloc::vec::Vec;
use core::ops::{Index, IndexMut};

use dsa_core::{Container, DsaError, Result, Searchable};

/// Default initial capacity for the array.
const DEFAULT_CAPACITY: usize = 8;

/// Growth factor when resizing (doubles capacity).
const GROWTH_FACTOR: usize = 2;

/// Shrink threshold (shrinks when size <= capacity * 0.25).
const SHRINK_THRESHOLD: f64 = 0.25;

/// A resizable array implementation that automatically grows and shrinks.
///
/// This implementation uses a growth factor of 2x when expanding and
/// shrinks when the array is less than 25% full to optimize memory usage.
#[derive(Debug, Clone)]
pub struct DynamicArray<T> {
    data: Vec<T>,
}

impl<T> DynamicArray<T> {
    /// Creates a new empty `DynamicArray` with default capacity.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let arr: DynamicArray<i32> = DynamicArray::new();
    /// assert!(arr.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Self::with_capacity(DEFAULT_CAPACITY)
    }

    /// Creates a new empty `DynamicArray` with the specified capacity.
    ///
    /// # Arguments
    ///
    /// * `capacity` - The initial capacity of the array.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let arr: DynamicArray<i32> = DynamicArray::with_capacity(16);
    /// assert_eq!(arr.capacity(), 16);
    /// ```
    #[must_use]
    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Vec::with_capacity(capacity.max(1)),
        }
    }

    /// Returns the capacity of the array.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn capacity(&self) -> usize {
        self.data.capacity()
    }

    /// Returns the number of elements in the array.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Returns `true` if the array contains no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Appends an element to the end of the array.
    ///
    /// # Arguments
    ///
    /// * `value` - The value to append.
    ///
    /// # Time Complexity
    /// O(1) amortized, O(n) worst case when resizing.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let mut arr = DynamicArray::new();
    /// arr.push(10);
    /// arr.push(20);
    /// assert_eq!(arr.len(), 2);
    /// ```
    pub fn push(&mut self, value: T) {
        self.data.push(value);
    }

    /// Removes and returns the last element, or `None` if empty.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let mut arr = DynamicArray::from(vec![10, 20, 30]);
    /// assert_eq!(arr.pop(), Some(30));
    /// assert_eq!(arr.pop(), Some(20));
    /// ```
    pub fn pop(&mut self) -> Option<T> {
        let value = self.data.pop()?;

        // Shrink if below threshold
        let len = self.data.len();
        let capacity = self.data.capacity();
        if len > 0 && (len as f64) < (capacity as f64) * SHRINK_THRESHOLD {
            let new_capacity = (capacity / GROWTH_FACTOR).max(DEFAULT_CAPACITY);
            if new_capacity < capacity {
                self.data.shrink_to(new_capacity);
            }
        }

        Some(value)
    }

    /// Returns a reference to the element at the specified index.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the element.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let arr = DynamicArray::from(vec![10, 20, 30]);
    /// assert_eq!(arr.get(0), Some(&10));
    /// assert_eq!(arr.get(5), None);
    /// ```
    #[must_use]
    pub fn get(&self, index: usize) -> Option<&T> {
        self.data.get(index)
    }

    /// Returns a mutable reference to the element at the specified index.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn get_mut(&mut self, index: usize) -> Option<&mut T> {
        self.data.get_mut(index)
    }

    /// Sets the element at the specified index.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the element.
    /// * `value` - The new value.
    ///
    /// # Errors
    ///
    /// Returns `DsaError::IndexOutOfBounds` if the index is out of bounds.
    ///
    /// # Time Complexity
    /// O(1)
    pub fn set(&mut self, index: usize, value: T) -> Result<()> {
        if index >= self.data.len() {
            return Err(DsaError::IndexOutOfBounds {
                index,
                size: self.data.len(),
            });
        }
        self.data[index] = value;
        Ok(())
    }

    /// Inserts an element at the specified index.
    ///
    /// # Arguments
    ///
    /// * `index` - The index at which to insert.
    /// * `value` - The value to insert.
    ///
    /// # Errors
    ///
    /// Returns `DsaError::IndexOutOfBounds` if `index > len`.
    ///
    /// # Time Complexity
    /// O(n) due to shifting elements.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let mut arr = DynamicArray::from(vec![10, 30]);
    /// arr.insert(1, 20).unwrap();
    /// assert_eq!(arr.get(1), Some(&20));
    /// ```
    pub fn insert(&mut self, index: usize, value: T) -> Result<()> {
        if index > self.data.len() {
            return Err(DsaError::IndexOutOfBounds {
                index,
                size: self.data.len(),
            });
        }
        self.data.insert(index, value);
        Ok(())
    }

    /// Removes and returns the element at the specified index.
    ///
    /// # Arguments
    ///
    /// * `index` - The index of the element to remove.
    ///
    /// # Errors
    ///
    /// Returns `DsaError::IndexOutOfBounds` if the index is out of bounds.
    ///
    /// # Time Complexity
    /// O(n) due to shifting elements.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let mut arr = DynamicArray::from(vec![10, 20, 30]);
    /// assert_eq!(arr.remove(1).unwrap(), 20);
    /// assert_eq!(arr.len(), 2);
    /// ```
    pub fn remove(&mut self, index: usize) -> Result<T> {
        if index >= self.data.len() {
            return Err(DsaError::IndexOutOfBounds {
                index,
                size: self.data.len(),
            });
        }
        Ok(self.data.remove(index))
    }

    /// Clears all elements from the array.
    ///
    /// # Time Complexity
    /// O(n) for dropping elements.
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Reverses the array in place.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let mut arr = DynamicArray::from(vec![1, 2, 3]);
    /// arr.reverse();
    /// assert_eq!(arr.get(0), Some(&3));
    /// ```
    pub fn reverse(&mut self) {
        self.data.reverse();
    }

    /// Returns the first element, or `None` if empty.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn first(&self) -> Option<&T> {
        self.data.first()
    }

    /// Returns the last element, or `None` if empty.
    ///
    /// # Time Complexity
    /// O(1)
    #[must_use]
    pub fn last(&self) -> Option<&T> {
        self.data.last()
    }

    /// Returns an iterator over the array.
    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.data.iter()
    }

    /// Returns a mutable iterator over the array.
    pub fn iter_mut(&mut self) -> impl Iterator<Item = &mut T> {
        self.data.iter_mut()
    }

    /// Converts the `DynamicArray` into a `Vec`.
    #[must_use]
    pub fn into_vec(self) -> Vec<T> {
        self.data
    }

    /// Returns a slice of the array.
    #[must_use]
    pub fn as_slice(&self) -> &[T] {
        &self.data
    }

    /// Returns a mutable slice of the array.
    #[must_use]
    pub fn as_mut_slice(&mut self) -> &mut [T] {
        &mut self.data
    }
}

impl<T: PartialEq> DynamicArray<T> {
    /// Finds the index of the first occurrence of a value.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::linear::DynamicArray;
    ///
    /// let arr = DynamicArray::from(vec![10, 20, 30, 20]);
    /// assert_eq!(arr.find(&20), Some(1));
    /// assert_eq!(arr.find(&50), None);
    /// ```
    #[must_use]
    pub fn find(&self, value: &T) -> Option<usize> {
        self.data.iter().position(|x| x == value)
    }

    /// Removes the first occurrence of a value.
    ///
    /// # Time Complexity
    /// O(n) for searching and shifting.
    ///
    /// # Returns
    ///
    /// `true` if the value was found and removed, `false` otherwise.
    pub fn remove_value(&mut self, value: &T) -> bool {
        if let Some(index) = self.find(value) {
            self.data.remove(index);
            true
        } else {
            false
        }
    }
}

impl<T> Container for DynamicArray<T> {
    fn len(&self) -> usize {
        self.data.len()
    }
}

impl<T: PartialEq> Searchable<T> for DynamicArray<T> {
    fn search(&self, value: &T) -> Option<usize> {
        self.find(value)
    }
}

impl<T> Default for DynamicArray<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> From<Vec<T>> for DynamicArray<T> {
    fn from(vec: Vec<T>) -> Self {
        Self { data: vec }
    }
}

impl<T: Clone> From<&[T]> for DynamicArray<T> {
    fn from(slice: &[T]) -> Self {
        Self {
            data: slice.to_vec(),
        }
    }
}

impl<T> FromIterator<T> for DynamicArray<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        Self {
            data: iter.into_iter().collect(),
        }
    }
}

impl<T> IntoIterator for DynamicArray<T> {
    type Item = T;
    type IntoIter = alloc::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.into_iter()
    }
}

impl<'a, T> IntoIterator for &'a DynamicArray<T> {
    type Item = &'a T;
    type IntoIter = core::slice::Iter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        self.data.iter()
    }
}

impl<T> Index<usize> for DynamicArray<T> {
    type Output = T;

    fn index(&self, index: usize) -> &Self::Output {
        &self.data[index]
    }
}

impl<T> IndexMut<usize> for DynamicArray<T> {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.data[index]
    }
}

impl<T: PartialEq> PartialEq for DynamicArray<T> {
    fn eq(&self, other: &Self) -> bool {
        self.data == other.data
    }
}

impl<T: Eq> Eq for DynamicArray<T> {}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let arr: DynamicArray<i32> = DynamicArray::new();
            assert!(arr.is_empty());
            assert_eq!(arr.len(), 0);
        }

        #[test]
        fn test_with_capacity() {
            let arr: DynamicArray<i32> = DynamicArray::with_capacity(16);
            assert!(arr.is_empty());
            assert!(arr.capacity() >= 16);
        }

        #[test]
        fn test_from_vec() {
            let arr = DynamicArray::from(vec![1, 2, 3]);
            assert_eq!(arr.len(), 3);
            assert_eq!(arr.get(0), Some(&1));
        }

        #[test]
        fn test_from_slice() {
            let slice = &[1, 2, 3][..];
            let arr = DynamicArray::from(slice);
            assert_eq!(arr.len(), 3);
        }

        #[test]
        fn test_default() {
            let arr: DynamicArray<i32> = DynamicArray::default();
            assert!(arr.is_empty());
        }
    }

    mod access {
        use super::*;

        #[test]
        fn test_get() {
            let arr = DynamicArray::from(vec![10, 20, 30]);
            assert_eq!(arr.get(0), Some(&10));
            assert_eq!(arr.get(1), Some(&20));
            assert_eq!(arr.get(2), Some(&30));
            assert_eq!(arr.get(3), None);
        }

        #[test]
        fn test_get_mut() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            if let Some(val) = arr.get_mut(1) {
                *val = 25;
            }
            assert_eq!(arr.get(1), Some(&25));
        }

        #[test]
        fn test_set() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            assert!(arr.set(1, 25).is_ok());
            assert_eq!(arr.get(1), Some(&25));
        }

        #[test]
        fn test_set_out_of_bounds() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            let result = arr.set(5, 50);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_first_last() {
            let arr = DynamicArray::from(vec![10, 20, 30]);
            assert_eq!(arr.first(), Some(&10));
            assert_eq!(arr.last(), Some(&30));

            let empty: DynamicArray<i32> = DynamicArray::new();
            assert_eq!(empty.first(), None);
            assert_eq!(empty.last(), None);
        }

        #[test]
        fn test_index() {
            let arr = DynamicArray::from(vec![10, 20, 30]);
            assert_eq!(arr[0], 10);
            assert_eq!(arr[1], 20);
            assert_eq!(arr[2], 30);
        }

        #[test]
        fn test_index_mut() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            arr[1] = 25;
            assert_eq!(arr[1], 25);
        }
    }

    mod insert_delete {
        use super::*;

        #[test]
        fn test_push() {
            let mut arr = DynamicArray::new();
            arr.push(10);
            arr.push(20);
            arr.push(30);
            assert_eq!(arr.len(), 3);
            assert_eq!(arr.get(2), Some(&30));
        }

        #[test]
        fn test_pop() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            assert_eq!(arr.pop(), Some(30));
            assert_eq!(arr.pop(), Some(20));
            assert_eq!(arr.pop(), Some(10));
            assert_eq!(arr.pop(), None);
        }

        #[test]
        fn test_insert() {
            let mut arr = DynamicArray::from(vec![10, 30]);
            assert!(arr.insert(1, 20).is_ok());
            assert_eq!(arr.len(), 3);
            assert_eq!(arr.get(1), Some(&20));
        }

        #[test]
        fn test_insert_at_start() {
            let mut arr = DynamicArray::from(vec![20, 30]);
            assert!(arr.insert(0, 10).is_ok());
            assert_eq!(arr.get(0), Some(&10));
        }

        #[test]
        fn test_insert_at_end() {
            let mut arr = DynamicArray::from(vec![10, 20]);
            assert!(arr.insert(2, 30).is_ok());
            assert_eq!(arr.get(2), Some(&30));
        }

        #[test]
        fn test_insert_out_of_bounds() {
            let mut arr = DynamicArray::from(vec![10, 20]);
            let result = arr.insert(5, 30);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_remove() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            assert_eq!(arr.remove(1).unwrap(), 20);
            assert_eq!(arr.len(), 2);
            assert_eq!(arr.get(1), Some(&30));
        }

        #[test]
        fn test_remove_out_of_bounds() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            let result = arr.remove(5);
            assert!(matches!(result, Err(DsaError::IndexOutOfBounds { .. })));
        }

        #[test]
        fn test_clear() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            arr.clear();
            assert!(arr.is_empty());
            assert_eq!(arr.len(), 0);
        }
    }

    mod search {
        use super::*;

        #[test]
        fn test_find() {
            let arr = DynamicArray::from(vec![10, 20, 30, 20]);
            assert_eq!(arr.find(&20), Some(1));
            assert_eq!(arr.find(&50), None);
        }

        #[test]
        fn test_contains() {
            let arr = DynamicArray::from(vec![10, 20, 30]);
            assert!(arr.contains(&20));
            assert!(!arr.contains(&50));
        }

        #[test]
        fn test_remove_value() {
            let mut arr = DynamicArray::from(vec![10, 20, 30, 20]);
            assert!(arr.remove_value(&20));
            assert_eq!(arr.len(), 3);
            assert_eq!(arr.find(&20), Some(2)); // Second 20 is now at index 2
        }

        #[test]
        fn test_remove_value_not_found() {
            let mut arr = DynamicArray::from(vec![10, 20, 30]);
            assert!(!arr.remove_value(&50));
            assert_eq!(arr.len(), 3);
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_reverse() {
            let mut arr = DynamicArray::from(vec![1, 2, 3, 4, 5]);
            arr.reverse();
            assert_eq!(arr.as_slice(), &[5, 4, 3, 2, 1]);
        }

        #[test]
        fn test_iter() {
            let arr = DynamicArray::from(vec![1, 2, 3]);
            let sum: i32 = arr.iter().sum();
            assert_eq!(sum, 6);
        }

        #[test]
        fn test_iter_mut() {
            let mut arr = DynamicArray::from(vec![1, 2, 3]);
            for x in arr.iter_mut() {
                *x *= 2;
            }
            assert_eq!(arr.as_slice(), &[2, 4, 6]);
        }

        #[test]
        fn test_into_iter() {
            let arr = DynamicArray::from(vec![1, 2, 3]);
            let collected: Vec<i32> = arr.into_iter().collect();
            assert_eq!(collected, vec![1, 2, 3]);
        }

        #[test]
        fn test_from_iter() {
            let arr: DynamicArray<i32> = (1..=5).collect();
            assert_eq!(arr.len(), 5);
            assert_eq!(arr.as_slice(), &[1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_into_vec() {
            let arr = DynamicArray::from(vec![1, 2, 3]);
            let vec = arr.into_vec();
            assert_eq!(vec, vec![1, 2, 3]);
        }

        #[test]
        fn test_as_slice() {
            let arr = DynamicArray::from(vec![1, 2, 3]);
            assert_eq!(arr.as_slice(), &[1, 2, 3]);
        }
    }

    mod equality {
        use super::*;

        #[test]
        fn test_eq() {
            let arr1 = DynamicArray::from(vec![1, 2, 3]);
            let arr2 = DynamicArray::from(vec![1, 2, 3]);
            let arr3 = DynamicArray::from(vec![1, 2, 4]);
            assert_eq!(arr1, arr2);
            assert_ne!(arr1, arr3);
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_empty_operations() {
            let mut arr: DynamicArray<i32> = DynamicArray::new();
            assert!(arr.is_empty());
            assert_eq!(arr.pop(), None);
            assert_eq!(arr.first(), None);
            assert_eq!(arr.last(), None);
            assert_eq!(arr.find(&10), None);
        }

        #[test]
        fn test_single_element() {
            let mut arr = DynamicArray::new();
            arr.push(42);
            assert_eq!(arr.len(), 1);
            assert_eq!(arr.first(), Some(&42));
            assert_eq!(arr.last(), Some(&42));
            assert_eq!(arr.pop(), Some(42));
            assert!(arr.is_empty());
        }

        #[test]
        fn test_large_array() {
            let mut arr = DynamicArray::new();
            for i in 0..10000 {
                arr.push(i);
            }
            assert_eq!(arr.len(), 10000);
            assert_eq!(arr.get(5000), Some(&5000));
        }
    }
}
