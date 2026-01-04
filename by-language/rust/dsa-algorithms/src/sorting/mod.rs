//! # Sorting Algorithms
//!
//! This module contains implementations of various sorting algorithms.
//!
//! ## Comparison Sorts
//!
//! - [`bubble_sort`] - O(n²) simple comparison sort
//! - [`selection_sort`] - O(n²) in-place comparison sort
//! - [`insertion_sort`] - O(n²) efficient for small or nearly sorted data
//! - [`merge_sort`] - O(n log n) stable divide-and-conquer sort
//! - [`quick_sort`] - O(n log n) average, fast in-place sort
//! - [`heap_sort`] - O(n log n) in-place comparison sort using heap
//!
//! ## Non-Comparison Sorts
//!
//! - [`counting_sort`] - O(n + k) integer sorting
//! - [`radix_sort`] - O(d(n + k)) digit-by-digit sorting
//!
//! ## Algorithm Comparison
//!
//! | Algorithm      | Best     | Average  | Worst    | Space  | Stable |
//! |----------------|----------|----------|----------|--------|--------|
//! | Bubble Sort    | O(n)     | O(n²)    | O(n²)    | O(1)   | Yes    |
//! | Selection Sort | O(n²)    | O(n²)    | O(n²)    | O(1)   | No     |
//! | Insertion Sort | O(n)     | O(n²)    | O(n²)    | O(1)   | Yes    |
//! | Merge Sort     | O(nlogn) | O(nlogn) | O(nlogn) | O(n)   | Yes    |
//! | Quick Sort     | O(nlogn) | O(nlogn) | O(n²)    | O(logn)| No     |
//! | Heap Sort      | O(nlogn) | O(nlogn) | O(nlogn) | O(1)   | No     |
//! | Counting Sort  | O(n+k)   | O(n+k)   | O(n+k)   | O(k)   | Yes    |
//! | Radix Sort     | O(d·n)   | O(d·n)   | O(d·n)   | O(n+k) | Yes    |

use alloc::vec::Vec;

/// Bubble Sort - Simple comparison-based sorting algorithm.
///
/// Repeatedly steps through the list, compares adjacent elements, and
/// swaps them if they are in the wrong order.
///
/// # Complexity
///
/// - Time: O(n²) average and worst, O(n) best (already sorted)
/// - Space: O(1)
/// - Stable: Yes
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::bubble_sort;
///
/// let mut arr = vec![64, 34, 25, 12, 22, 11, 90];
/// bubble_sort(&mut arr);
/// assert_eq!(arr, vec![11, 12, 22, 25, 34, 64, 90]);
/// ```
pub fn bubble_sort<T: Ord>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    for i in 0..n {
        let mut swapped = false;

        for j in 0..n - 1 - i {
            if arr[j] > arr[j + 1] {
                arr.swap(j, j + 1);
                swapped = true;
            }
        }

        // If no swapping occurred, array is sorted
        if !swapped {
            break;
        }
    }
}

/// Selection Sort - In-place comparison sorting algorithm.
///
/// Divides the input into a sorted and unsorted region, repeatedly
/// selecting the smallest element from the unsorted region.
///
/// # Complexity
///
/// - Time: O(n²) for all cases
/// - Space: O(1)
/// - Stable: No
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::selection_sort;
///
/// let mut arr = vec![64, 25, 12, 22, 11];
/// selection_sort(&mut arr);
/// assert_eq!(arr, vec![11, 12, 22, 25, 64]);
/// ```
pub fn selection_sort<T: Ord>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    for i in 0..n - 1 {
        let mut min_idx = i;

        for j in i + 1..n {
            if arr[j] < arr[min_idx] {
                min_idx = j;
            }
        }

        if min_idx != i {
            arr.swap(i, min_idx);
        }
    }
}

/// Insertion Sort - Simple sorting algorithm that builds the sorted array one item at a time.
///
/// Efficient for small data sets and nearly sorted arrays.
///
/// # Complexity
///
/// - Time: O(n²) average and worst, O(n) best (already sorted)
/// - Space: O(1)
/// - Stable: Yes
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::insertion_sort;
///
/// let mut arr = vec![12, 11, 13, 5, 6];
/// insertion_sort(&mut arr);
/// assert_eq!(arr, vec![5, 6, 11, 12, 13]);
/// ```
pub fn insertion_sort<T: Ord>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    for i in 1..n {
        let mut j = i;
        while j > 0 && arr[j - 1] > arr[j] {
            arr.swap(j - 1, j);
            j -= 1;
        }
    }
}

/// Merge Sort - Efficient, stable, divide-and-conquer sorting algorithm.
///
/// Divides the array into halves, recursively sorts them, and merges
/// the sorted halves.
///
/// # Complexity
///
/// - Time: O(n log n) for all cases
/// - Space: O(n)
/// - Stable: Yes
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::merge_sort;
///
/// let mut arr = vec![38, 27, 43, 3, 9, 82, 10];
/// merge_sort(&mut arr);
/// assert_eq!(arr, vec![3, 9, 10, 27, 38, 43, 82]);
/// ```
pub fn merge_sort<T: Ord + Clone>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    let mid = n / 2;
    merge_sort(&mut arr[..mid]);
    merge_sort(&mut arr[mid..]);

    // Merge the two halves
    let left: Vec<T> = arr[..mid].to_vec();
    let right: Vec<T> = arr[mid..].to_vec();

    let mut i = 0;
    let mut j = 0;
    let mut k = 0;

    while i < left.len() && j < right.len() {
        if left[i] <= right[j] {
            arr[k] = left[i].clone();
            i += 1;
        } else {
            arr[k] = right[j].clone();
            j += 1;
        }
        k += 1;
    }

    while i < left.len() {
        arr[k] = left[i].clone();
        i += 1;
        k += 1;
    }

    while j < right.len() {
        arr[k] = right[j].clone();
        j += 1;
        k += 1;
    }
}

/// Quick Sort - Efficient divide-and-conquer sorting algorithm.
///
/// Selects a pivot element and partitions the array around it,
/// then recursively sorts the partitions.
///
/// # Complexity
///
/// - Time: O(n log n) average, O(n²) worst (rare with good pivot selection)
/// - Space: O(log n) for recursion stack
/// - Stable: No
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::quick_sort;
///
/// let mut arr = vec![10, 7, 8, 9, 1, 5];
/// quick_sort(&mut arr);
/// assert_eq!(arr, vec![1, 5, 7, 8, 9, 10]);
/// ```
pub fn quick_sort<T: Ord>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    quick_sort_helper(arr, 0, n - 1);
}

fn quick_sort_helper<T: Ord>(arr: &mut [T], low: usize, high: usize) {
    if low < high {
        let pivot_idx = partition(arr, low, high);

        if pivot_idx > 0 {
            quick_sort_helper(arr, low, pivot_idx - 1);
        }
        quick_sort_helper(arr, pivot_idx + 1, high);
    }
}

fn partition<T: Ord>(arr: &mut [T], low: usize, high: usize) -> usize {
    // Use median-of-three pivot selection for better performance
    let mid = low + (high - low) / 2;

    // Sort low, mid, high
    if arr[mid] < arr[low] {
        arr.swap(low, mid);
    }
    if arr[high] < arr[low] {
        arr.swap(low, high);
    }
    if arr[high] < arr[mid] {
        arr.swap(mid, high);
    }

    // Use mid as pivot (move to high-1)
    arr.swap(mid, high);

    let mut i = low;

    for j in low..high {
        if arr[j] <= arr[high] {
            arr.swap(i, j);
            i += 1;
        }
    }

    arr.swap(i, high);
    i
}

/// Heap Sort - In-place comparison sorting algorithm using a binary heap.
///
/// Builds a max-heap from the array and repeatedly extracts the maximum.
///
/// # Complexity
///
/// - Time: O(n log n) for all cases
/// - Space: O(1)
/// - Stable: No
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::heap_sort;
///
/// let mut arr = vec![12, 11, 13, 5, 6, 7];
/// heap_sort(&mut arr);
/// assert_eq!(arr, vec![5, 6, 7, 11, 12, 13]);
/// ```
pub fn heap_sort<T: Ord>(arr: &mut [T]) {
    let n = arr.len();
    if n <= 1 {
        return;
    }

    // Build max heap
    for i in (0..n / 2).rev() {
        heapify(arr, n, i);
    }

    // Extract elements from heap one by one
    for i in (1..n).rev() {
        arr.swap(0, i);
        heapify(arr, i, 0);
    }
}

fn heapify<T: Ord>(arr: &mut [T], n: usize, i: usize) {
    let mut largest = i;
    let left = 2 * i + 1;
    let right = 2 * i + 2;

    if left < n && arr[left] > arr[largest] {
        largest = left;
    }

    if right < n && arr[right] > arr[largest] {
        largest = right;
    }

    if largest != i {
        arr.swap(i, largest);
        heapify(arr, n, largest);
    }
}

/// Counting Sort - Non-comparison integer sorting algorithm.
///
/// Counts occurrences of each value and uses arithmetic to determine positions.
///
/// # Complexity
///
/// - Time: O(n + k) where k is the range of input
/// - Space: O(k)
/// - Stable: Yes
///
/// # Note
///
/// Only works with non-negative integers.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::counting_sort;
///
/// let mut arr = vec![4, 2, 2, 8, 3, 3, 1];
/// counting_sort(&mut arr);
/// assert_eq!(arr, vec![1, 2, 2, 3, 3, 4, 8]);
/// ```
pub fn counting_sort(arr: &mut [u32]) {
    if arr.len() <= 1 {
        return;
    }

    // Find the maximum value
    let max = match arr.iter().max() {
        Some(&m) => m as usize,
        None => return,
    };

    // Create counting array
    let mut count = vec![0usize; max + 1];

    // Count occurrences
    for &val in arr.iter() {
        count[val as usize] += 1;
    }

    // Reconstruct sorted array
    let mut idx = 0;
    for (val, &cnt) in count.iter().enumerate() {
        for _ in 0..cnt {
            arr[idx] = val as u32;
            idx += 1;
        }
    }
}

/// Counting Sort for i32 - Handles negative numbers by shifting values.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::counting_sort_i32;
///
/// let mut arr = vec![4, -2, 2, 8, -3, 3, 1];
/// counting_sort_i32(&mut arr);
/// assert_eq!(arr, vec![-3, -2, 1, 2, 3, 4, 8]);
/// ```
pub fn counting_sort_i32(arr: &mut [i32]) {
    if arr.len() <= 1 {
        return;
    }

    let min = *arr.iter().min().unwrap();
    let max = *arr.iter().max().unwrap();
    let range = (max - min + 1) as usize;

    let mut count = vec![0usize; range];

    for &val in arr.iter() {
        count[(val - min) as usize] += 1;
    }

    let mut idx = 0;
    for (offset, &cnt) in count.iter().enumerate() {
        let val = min + offset as i32;
        for _ in 0..cnt {
            arr[idx] = val;
            idx += 1;
        }
    }
}

/// Radix Sort - Non-comparison integer sorting algorithm.
///
/// Sorts numbers digit by digit, from least significant to most significant.
///
/// # Complexity
///
/// - Time: O(d * n) where d is the number of digits
/// - Space: O(n + k) where k is the radix (10 for decimal)
/// - Stable: Yes
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::radix_sort;
///
/// let mut arr = vec![170, 45, 75, 90, 802, 24, 2, 66];
/// radix_sort(&mut arr);
/// assert_eq!(arr, vec![2, 24, 45, 66, 75, 90, 170, 802]);
/// ```
pub fn radix_sort(arr: &mut [u32]) {
    if arr.len() <= 1 {
        return;
    }

    // Find maximum to know number of digits
    let max = match arr.iter().max() {
        Some(&m) => m,
        None => return,
    };

    // Do counting sort for every digit
    let mut exp = 1u32;
    while max / exp > 0 {
        counting_sort_by_digit(arr, exp);
        exp *= 10;
    }
}

fn counting_sort_by_digit(arr: &mut [u32], exp: u32) {
    let n = arr.len();
    let mut output = vec![0u32; n];
    let mut count = [0usize; 10];

    // Count occurrences of each digit
    for &val in arr.iter() {
        let digit = ((val / exp) % 10) as usize;
        count[digit] += 1;
    }

    // Convert count to actual positions
    for i in 1..10 {
        count[i] += count[i - 1];
    }

    // Build output array (traverse in reverse for stability)
    for &val in arr.iter().rev() {
        let digit = ((val / exp) % 10) as usize;
        count[digit] -= 1;
        output[count[digit]] = val;
    }

    // Copy output back to arr
    arr.copy_from_slice(&output);
}

/// Check if an array is sorted in ascending order.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::sorting::is_sorted;
///
/// assert!(is_sorted(&[1, 2, 3, 4, 5]));
/// assert!(!is_sorted(&[1, 3, 2, 4, 5]));
/// assert!(is_sorted(&[1]));
/// assert!(is_sorted::<i32>(&[]));
/// ```
pub fn is_sorted<T: Ord>(arr: &[T]) -> bool {
    arr.windows(2).all(|w| w[0] <= w[1])
}

#[cfg(test)]
mod tests {
    use super::*;

    mod bubble_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![64, 34, 25, 12, 22, 11, 90];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![11, 12, 22, 25, 34, 64, 90]);
        }

        #[test]
        fn test_duplicates() {
            let mut arr = vec![3, 1, 4, 1, 5, 9, 2, 6, 5, 3];
            bubble_sort(&mut arr);
            assert_eq!(arr, vec![1, 1, 2, 3, 3, 4, 5, 5, 6, 9]);
        }
    }

    mod selection_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            selection_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            selection_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            selection_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            selection_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![64, 25, 12, 22, 11];
            selection_sort(&mut arr);
            assert_eq!(arr, vec![11, 12, 22, 25, 64]);
        }
    }

    mod insertion_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            insertion_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            insertion_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            insertion_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            insertion_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![12, 11, 13, 5, 6];
            insertion_sort(&mut arr);
            assert_eq!(arr, vec![5, 6, 11, 12, 13]);
        }
    }

    mod merge_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![38, 27, 43, 3, 9, 82, 10];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![3, 9, 10, 27, 38, 43, 82]);
        }

        #[test]
        fn test_stability() {
            // Merge sort should be stable
            let mut arr = vec![3, 1, 4, 1, 5, 9, 2, 6, 5, 3];
            merge_sort(&mut arr);
            assert_eq!(arr, vec![1, 1, 2, 3, 3, 4, 5, 5, 6, 9]);
        }
    }

    mod quick_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![10, 7, 8, 9, 1, 5];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1, 5, 7, 8, 9, 10]);
        }

        #[test]
        fn test_duplicates() {
            let mut arr = vec![3, 3, 3, 1, 1, 2, 2];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1, 1, 2, 2, 3, 3, 3]);
        }

        #[test]
        fn test_two_elements() {
            let mut arr = vec![2, 1];
            quick_sort(&mut arr);
            assert_eq!(arr, vec![1, 2]);
        }
    }

    mod heap_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<i32> = vec![];
            heap_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1];
            heap_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_sorted() {
            let mut arr = vec![1, 2, 3, 4, 5];
            heap_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_reverse() {
            let mut arr = vec![5, 4, 3, 2, 1];
            heap_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_random() {
            let mut arr = vec![12, 11, 13, 5, 6, 7];
            heap_sort(&mut arr);
            assert_eq!(arr, vec![5, 6, 7, 11, 12, 13]);
        }
    }

    mod counting_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<u32> = vec![];
            counting_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1u32];
            counting_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_basic() {
            let mut arr = vec![4u32, 2, 2, 8, 3, 3, 1];
            counting_sort(&mut arr);
            assert_eq!(arr, vec![1, 2, 2, 3, 3, 4, 8]);
        }

        #[test]
        fn test_all_same() {
            let mut arr = vec![5u32, 5, 5, 5, 5];
            counting_sort(&mut arr);
            assert_eq!(arr, vec![5, 5, 5, 5, 5]);
        }

        #[test]
        fn test_i32_with_negatives() {
            let mut arr = vec![4, -2, 2, 8, -3, 3, 1];
            counting_sort_i32(&mut arr);
            assert_eq!(arr, vec![-3, -2, 1, 2, 3, 4, 8]);
        }
    }

    mod radix_sort_tests {
        use super::*;

        #[test]
        fn test_empty() {
            let mut arr: Vec<u32> = vec![];
            radix_sort(&mut arr);
            assert_eq!(arr, vec![]);
        }

        #[test]
        fn test_single() {
            let mut arr = vec![1u32];
            radix_sort(&mut arr);
            assert_eq!(arr, vec![1]);
        }

        #[test]
        fn test_basic() {
            let mut arr = vec![170u32, 45, 75, 90, 802, 24, 2, 66];
            radix_sort(&mut arr);
            assert_eq!(arr, vec![2, 24, 45, 66, 75, 90, 170, 802]);
        }

        #[test]
        fn test_large_numbers() {
            let mut arr = vec![1000000u32, 100000, 10000, 1000, 100, 10, 1];
            radix_sort(&mut arr);
            assert_eq!(arr, vec![1, 10, 100, 1000, 10000, 100000, 1000000]);
        }

        #[test]
        fn test_same_digit_count() {
            let mut arr = vec![123u32, 456, 789, 234, 567, 890];
            radix_sort(&mut arr);
            assert_eq!(arr, vec![123, 234, 456, 567, 789, 890]);
        }
    }

    mod is_sorted_tests {
        use super::*;

        #[test]
        fn test_empty() {
            assert!(is_sorted::<i32>(&[]));
        }

        #[test]
        fn test_single() {
            assert!(is_sorted(&[1]));
        }

        #[test]
        fn test_sorted() {
            assert!(is_sorted(&[1, 2, 3, 4, 5]));
        }

        #[test]
        fn test_unsorted() {
            assert!(!is_sorted(&[1, 3, 2, 4, 5]));
        }

        #[test]
        fn test_reverse() {
            assert!(!is_sorted(&[5, 4, 3, 2, 1]));
        }

        #[test]
        fn test_duplicates() {
            assert!(is_sorted(&[1, 1, 2, 2, 3, 3]));
        }
    }

    mod comparative_tests {
        use super::*;

        fn test_all_sorts(arr: &[i32]) {
            let mut arr1 = arr.to_vec();
            let mut arr2 = arr.to_vec();
            let mut arr3 = arr.to_vec();
            let mut arr4 = arr.to_vec();
            let mut arr5 = arr.to_vec();
            let mut arr6 = arr.to_vec();

            bubble_sort(&mut arr1);
            selection_sort(&mut arr2);
            insertion_sort(&mut arr3);
            merge_sort(&mut arr4);
            quick_sort(&mut arr5);
            heap_sort(&mut arr6);

            // All should produce the same sorted result
            assert_eq!(arr1, arr2);
            assert_eq!(arr2, arr3);
            assert_eq!(arr3, arr4);
            assert_eq!(arr4, arr5);
            assert_eq!(arr5, arr6);

            // All should be sorted
            assert!(is_sorted(&arr1));
        }

        #[test]
        fn test_comparison_empty() {
            test_all_sorts(&[]);
        }

        #[test]
        fn test_comparison_single() {
            test_all_sorts(&[42]);
        }

        #[test]
        fn test_comparison_random() {
            test_all_sorts(&[64, 34, 25, 12, 22, 11, 90, 1, 100, 50]);
        }

        #[test]
        fn test_comparison_sorted() {
            test_all_sorts(&[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
        }

        #[test]
        fn test_comparison_reverse() {
            test_all_sorts(&[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]);
        }

        #[test]
        fn test_comparison_duplicates() {
            test_all_sorts(&[5, 2, 8, 2, 9, 1, 5, 8, 3, 1]);
        }
    }
}
