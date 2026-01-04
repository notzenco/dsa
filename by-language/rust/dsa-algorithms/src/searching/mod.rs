//! # Searching Algorithms
//!
//! This module contains implementations of searching algorithms and patterns.
//!
//! ## Binary Search Variants
//!
//! - [`binary_search`] - Standard binary search
//! - [`lower_bound`] - First position where value could be inserted
//! - [`upper_bound`] - Last position where value could be inserted
//! - [`binary_search_first`] - First occurrence of target
//! - [`binary_search_last`] - Last occurrence of target
//!
//! ## Two Pointers Techniques
//!
//! - [`two_sum_sorted`] - Find pair with target sum
//! - [`three_sum`] - Find triplets with target sum
//! - [`remove_duplicates`] - Remove duplicates in-place
//! - [`container_with_most_water`] - Maximum area between lines
//!
//! ## Sliding Window Techniques
//!
//! - [`max_sum_subarray`] - Maximum sum of fixed-size window
//! - [`min_window_substring`] - Minimum window containing all chars
//! - [`longest_substring_without_repeating`] - Longest unique character substring

use alloc::collections::BTreeMap;
use alloc::string::String;
use alloc::vec;
use alloc::vec::Vec;

// ============================================================================
// Binary Search Variants
// ============================================================================

/// Standard binary search for a sorted array.
///
/// Returns the index of the target if found, `None` otherwise.
///
/// # Complexity
///
/// - Time: O(log n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::binary_search;
///
/// let arr = vec![1, 3, 5, 7, 9, 11, 13];
/// assert_eq!(binary_search(&arr, &7), Some(3));
/// assert_eq!(binary_search(&arr, &6), None);
/// ```
pub fn binary_search<T: Ord>(arr: &[T], target: &T) -> Option<usize> {
    if arr.is_empty() {
        return None;
    }

    let mut left = 0;
    let mut right = arr.len() - 1;

    while left <= right {
        let mid = left + (right - left) / 2;

        match arr[mid].cmp(target) {
            core::cmp::Ordering::Equal => return Some(mid),
            core::cmp::Ordering::Less => left = mid + 1,
            core::cmp::Ordering::Greater => {
                if mid == 0 {
                    break;
                }
                right = mid - 1;
            }
        }
    }

    None
}

/// Lower bound - first position where value could be inserted maintaining sorted order.
///
/// Returns the index of the first element >= target, or arr.len() if none.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::lower_bound;
///
/// let arr = vec![1, 2, 4, 4, 4, 6, 8];
/// assert_eq!(lower_bound(&arr, &4), 2);  // First 4
/// assert_eq!(lower_bound(&arr, &5), 5);  // Would insert at 5
/// assert_eq!(lower_bound(&arr, &0), 0);  // Before all
/// ```
pub fn lower_bound<T: Ord>(arr: &[T], target: &T) -> usize {
    let mut left = 0;
    let mut right = arr.len();

    while left < right {
        let mid = left + (right - left) / 2;

        if arr[mid] < *target {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    left
}

/// Upper bound - first position where value is greater than target.
///
/// Returns the index of the first element > target, or arr.len() if none.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::upper_bound;
///
/// let arr = vec![1, 2, 4, 4, 4, 6, 8];
/// assert_eq!(upper_bound(&arr, &4), 5);  // After last 4
/// assert_eq!(upper_bound(&arr, &3), 2);  // Would insert at 2
/// assert_eq!(upper_bound(&arr, &10), 7); // After all
/// ```
pub fn upper_bound<T: Ord>(arr: &[T], target: &T) -> usize {
    let mut left = 0;
    let mut right = arr.len();

    while left < right {
        let mid = left + (right - left) / 2;

        if arr[mid] <= *target {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    left
}

/// Find the first occurrence of target in a sorted array with duplicates.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::binary_search_first;
///
/// let arr = vec![1, 2, 4, 4, 4, 6, 8];
/// assert_eq!(binary_search_first(&arr, &4), Some(2));
/// assert_eq!(binary_search_first(&arr, &5), None);
/// ```
pub fn binary_search_first<T: Ord>(arr: &[T], target: &T) -> Option<usize> {
    let idx = lower_bound(arr, target);
    if idx < arr.len() && arr[idx] == *target {
        Some(idx)
    } else {
        None
    }
}

/// Find the last occurrence of target in a sorted array with duplicates.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::binary_search_last;
///
/// let arr = vec![1, 2, 4, 4, 4, 6, 8];
/// assert_eq!(binary_search_last(&arr, &4), Some(4));
/// assert_eq!(binary_search_last(&arr, &5), None);
/// ```
pub fn binary_search_last<T: Ord>(arr: &[T], target: &T) -> Option<usize> {
    let idx = upper_bound(arr, target);
    if idx > 0 && arr[idx - 1] == *target {
        Some(idx - 1)
    } else {
        None
    }
}

/// Count occurrences of target in a sorted array.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::count_occurrences;
///
/// let arr = vec![1, 2, 4, 4, 4, 6, 8];
/// assert_eq!(count_occurrences(&arr, &4), 3);
/// assert_eq!(count_occurrences(&arr, &5), 0);
/// ```
pub fn count_occurrences<T: Ord>(arr: &[T], target: &T) -> usize {
    let lower = lower_bound(arr, target);
    let upper = upper_bound(arr, target);
    upper - lower
}

// ============================================================================
// Two Pointers Techniques
// ============================================================================

/// Find two numbers in a sorted array that sum to target.
///
/// Returns indices of the two numbers, or `None` if not found.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::two_sum_sorted;
///
/// let arr = vec![2, 7, 11, 15];
/// assert_eq!(two_sum_sorted(&arr, 9), Some((0, 1)));
/// assert_eq!(two_sum_sorted(&arr, 10), None);
/// ```
pub fn two_sum_sorted(arr: &[i32], target: i32) -> Option<(usize, usize)> {
    if arr.len() < 2 {
        return None;
    }

    let mut left = 0;
    let mut right = arr.len() - 1;

    while left < right {
        let sum = arr[left] + arr[right];

        match sum.cmp(&target) {
            core::cmp::Ordering::Equal => return Some((left, right)),
            core::cmp::Ordering::Less => left += 1,
            core::cmp::Ordering::Greater => right -= 1,
        }
    }

    None
}

/// Find all unique triplets that sum to target.
///
/// # Complexity
///
/// - Time: O(n²)
/// - Space: O(1) excluding output
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::three_sum;
///
/// let mut arr = vec![-1, 0, 1, 2, -1, -4];
/// let result = three_sum(&mut arr, 0);
/// assert!(result.contains(&vec![-1, -1, 2]));
/// assert!(result.contains(&vec![-1, 0, 1]));
/// ```
pub fn three_sum(arr: &mut [i32], target: i32) -> Vec<Vec<i32>> {
    let mut result = Vec::new();

    if arr.len() < 3 {
        return result;
    }

    arr.sort();
    let n = arr.len();

    for i in 0..n - 2 {
        // Skip duplicates
        if i > 0 && arr[i] == arr[i - 1] {
            continue;
        }

        let mut left = i + 1;
        let mut right = n - 1;

        while left < right {
            let sum = arr[i] + arr[left] + arr[right];

            match sum.cmp(&target) {
                core::cmp::Ordering::Equal => {
                    result.push(vec![arr[i], arr[left], arr[right]]);

                    // Skip duplicates
                    while left < right && arr[left] == arr[left + 1] {
                        left += 1;
                    }
                    while left < right && arr[right] == arr[right - 1] {
                        right -= 1;
                    }

                    left += 1;
                    right -= 1;
                }
                core::cmp::Ordering::Less => left += 1,
                core::cmp::Ordering::Greater => right -= 1,
            }
        }
    }

    result
}

/// Remove duplicates from sorted array in-place, returns new length.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::remove_duplicates;
///
/// let mut arr = vec![1, 1, 2, 2, 3, 4, 4, 5];
/// let len = remove_duplicates(&mut arr);
/// assert_eq!(len, 5);
/// assert_eq!(&arr[..len], &[1, 2, 3, 4, 5]);
/// ```
pub fn remove_duplicates<T: Ord>(arr: &mut [T]) -> usize {
    if arr.is_empty() {
        return 0;
    }

    let mut write = 1;

    for read in 1..arr.len() {
        if arr[read] != arr[write - 1] {
            arr.swap(write, read);
            write += 1;
        }
    }

    write
}

/// Container with most water - find max area between two lines.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::container_with_most_water;
///
/// let heights = vec![1, 8, 6, 2, 5, 4, 8, 3, 7];
/// assert_eq!(container_with_most_water(&heights), 49);
/// ```
pub fn container_with_most_water(heights: &[i32]) -> i32 {
    if heights.len() < 2 {
        return 0;
    }

    let mut left = 0;
    let mut right = heights.len() - 1;
    let mut max_area = 0;

    while left < right {
        let width = (right - left) as i32;
        let height = heights[left].min(heights[right]);
        max_area = max_area.max(width * height);

        if heights[left] < heights[right] {
            left += 1;
        } else {
            right -= 1;
        }
    }

    max_area
}

/// Trapping rain water - calculate water trapped between bars.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::trap_water;
///
/// let heights = vec![0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1];
/// assert_eq!(trap_water(&heights), 6);
/// ```
pub fn trap_water(heights: &[i32]) -> i32 {
    if heights.len() < 3 {
        return 0;
    }

    let mut left = 0;
    let mut right = heights.len() - 1;
    let mut left_max = 0;
    let mut right_max = 0;
    let mut water = 0;

    while left < right {
        if heights[left] < heights[right] {
            if heights[left] >= left_max {
                left_max = heights[left];
            } else {
                water += left_max - heights[left];
            }
            left += 1;
        } else {
            if heights[right] >= right_max {
                right_max = heights[right];
            } else {
                water += right_max - heights[right];
            }
            right -= 1;
        }
    }

    water
}

// ============================================================================
// Sliding Window Techniques
// ============================================================================

/// Maximum sum of a subarray of fixed size k.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::max_sum_subarray;
///
/// let arr = vec![1, 4, 2, 10, 23, 3, 1, 0, 20];
/// assert_eq!(max_sum_subarray(&arr, 4), Some(39)); // [10, 23, 3, 1] or [3, 1, 0, 20]
/// ```
pub fn max_sum_subarray(arr: &[i32], k: usize) -> Option<i32> {
    if k == 0 || arr.len() < k {
        return None;
    }

    // Calculate first window sum
    let mut window_sum: i32 = arr[..k].iter().sum();
    let mut max_sum = window_sum;

    // Slide the window
    for i in k..arr.len() {
        window_sum = window_sum + arr[i] - arr[i - k];
        max_sum = max_sum.max(window_sum);
    }

    Some(max_sum)
}

/// Longest substring without repeating characters.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(min(n, alphabet_size))
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::longest_substring_without_repeating;
///
/// assert_eq!(longest_substring_without_repeating("abcabcbb"), 3); // "abc"
/// assert_eq!(longest_substring_without_repeating("bbbbb"), 1);    // "b"
/// assert_eq!(longest_substring_without_repeating("pwwkew"), 3);   // "wke"
/// ```
pub fn longest_substring_without_repeating(s: &str) -> usize {
    let chars: Vec<char> = s.chars().collect();
    let mut char_index: BTreeMap<char, usize> = BTreeMap::new();
    let mut max_len = 0;
    let mut start = 0;

    for (i, &c) in chars.iter().enumerate() {
        if let Some(&prev_idx) = char_index.get(&c) {
            if prev_idx >= start {
                start = prev_idx + 1;
            }
        }
        char_index.insert(c, i);
        max_len = max_len.max(i - start + 1);
    }

    max_len
}

/// Minimum window substring containing all characters of pattern.
///
/// # Complexity
///
/// - Time: O(n + m)
/// - Space: O(k) where k is unique chars in pattern
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::min_window_substring;
///
/// assert_eq!(min_window_substring("ADOBECODEBANC", "ABC"), Some("BANC".to_string()));
/// assert_eq!(min_window_substring("a", "a"), Some("a".to_string()));
/// assert_eq!(min_window_substring("a", "aa"), None);
/// ```
pub fn min_window_substring(s: &str, t: &str) -> Option<String> {
    if t.is_empty() || s.len() < t.len() {
        return None;
    }

    let s_chars: Vec<char> = s.chars().collect();
    let t_chars: Vec<char> = t.chars().collect();

    // Count characters needed
    let mut need: BTreeMap<char, i32> = BTreeMap::new();
    for &c in &t_chars {
        *need.entry(c).or_insert(0) += 1;
    }

    let mut have: BTreeMap<char, i32> = BTreeMap::new();
    let mut formed = 0;
    let required = need.len();

    let mut left = 0;
    let mut min_len = usize::MAX;
    let mut result_start = 0;

    for right in 0..s_chars.len() {
        let c = s_chars[right];
        *have.entry(c).or_insert(0) += 1;

        if need.contains_key(&c) && have.get(&c) == need.get(&c) {
            formed += 1;
        }

        // Try to shrink window
        while formed == required {
            let window_len = right - left + 1;
            if window_len < min_len {
                min_len = window_len;
                result_start = left;
            }

            let left_char = s_chars[left];
            *have.get_mut(&left_char).unwrap() -= 1;

            if need.contains_key(&left_char) && have.get(&left_char) < need.get(&left_char) {
                formed -= 1;
            }

            left += 1;
        }
    }

    if min_len == usize::MAX {
        None
    } else {
        Some(s_chars[result_start..result_start + min_len].iter().collect())
    }
}

/// Find longest substring with at most k distinct characters.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(k)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::searching::longest_substring_k_distinct;
///
/// assert_eq!(longest_substring_k_distinct("eceba", 2), 3);  // "ece"
/// assert_eq!(longest_substring_k_distinct("aa", 1), 2);     // "aa"
/// ```
pub fn longest_substring_k_distinct(s: &str, k: usize) -> usize {
    if k == 0 {
        return 0;
    }

    let chars: Vec<char> = s.chars().collect();
    let mut count: BTreeMap<char, usize> = BTreeMap::new();
    let mut max_len = 0;
    let mut left = 0;

    for right in 0..chars.len() {
        *count.entry(chars[right]).or_insert(0) += 1;

        while count.len() > k {
            let left_char = chars[left];
            *count.get_mut(&left_char).unwrap() -= 1;
            if count[&left_char] == 0 {
                count.remove(&left_char);
            }
            left += 1;
        }

        max_len = max_len.max(right - left + 1);
    }

    max_len
}

#[cfg(test)]
mod tests {
    use super::*;

    mod binary_search_tests {
        use super::*;

        #[test]
        fn test_binary_search_found() {
            let arr = vec![1, 3, 5, 7, 9, 11, 13];
            assert_eq!(binary_search(&arr, &7), Some(3));
            assert_eq!(binary_search(&arr, &1), Some(0));
            assert_eq!(binary_search(&arr, &13), Some(6));
        }

        #[test]
        fn test_binary_search_not_found() {
            let arr = vec![1, 3, 5, 7, 9, 11, 13];
            assert_eq!(binary_search(&arr, &6), None);
            assert_eq!(binary_search(&arr, &0), None);
            assert_eq!(binary_search(&arr, &14), None);
        }

        #[test]
        fn test_binary_search_empty() {
            let arr: Vec<i32> = vec![];
            assert_eq!(binary_search(&arr, &5), None);
        }

        #[test]
        fn test_binary_search_single() {
            let arr = vec![5];
            assert_eq!(binary_search(&arr, &5), Some(0));
            assert_eq!(binary_search(&arr, &3), None);
        }

        #[test]
        fn test_lower_bound() {
            let arr = vec![1, 2, 4, 4, 4, 6, 8];
            assert_eq!(lower_bound(&arr, &4), 2);
            assert_eq!(lower_bound(&arr, &5), 5);
            assert_eq!(lower_bound(&arr, &0), 0);
            assert_eq!(lower_bound(&arr, &10), 7);
        }

        #[test]
        fn test_upper_bound() {
            let arr = vec![1, 2, 4, 4, 4, 6, 8];
            assert_eq!(upper_bound(&arr, &4), 5);
            assert_eq!(upper_bound(&arr, &3), 2);
            assert_eq!(upper_bound(&arr, &10), 7);
        }

        #[test]
        fn test_first_last_occurrence() {
            let arr = vec![1, 2, 4, 4, 4, 6, 8];
            assert_eq!(binary_search_first(&arr, &4), Some(2));
            assert_eq!(binary_search_last(&arr, &4), Some(4));
            assert_eq!(binary_search_first(&arr, &5), None);
            assert_eq!(binary_search_last(&arr, &5), None);
        }

        #[test]
        fn test_count_occurrences() {
            let arr = vec![1, 2, 4, 4, 4, 6, 8];
            assert_eq!(count_occurrences(&arr, &4), 3);
            assert_eq!(count_occurrences(&arr, &1), 1);
            assert_eq!(count_occurrences(&arr, &5), 0);
        }
    }

    mod two_pointers_tests {
        use super::*;

        #[test]
        fn test_two_sum_sorted() {
            let arr = vec![2, 7, 11, 15];
            assert_eq!(two_sum_sorted(&arr, 9), Some((0, 1)));
            assert_eq!(two_sum_sorted(&arr, 18), Some((1, 2)));
            assert_eq!(two_sum_sorted(&arr, 10), None);
        }

        #[test]
        fn test_three_sum() {
            let mut arr = vec![-1, 0, 1, 2, -1, -4];
            let result = three_sum(&mut arr, 0);
            assert_eq!(result.len(), 2);
            assert!(result.contains(&vec![-1, -1, 2]));
            assert!(result.contains(&vec![-1, 0, 1]));
        }

        #[test]
        fn test_three_sum_no_result() {
            let mut arr = vec![1, 2, 3];
            let result = three_sum(&mut arr, 0);
            assert!(result.is_empty());
        }

        #[test]
        fn test_remove_duplicates() {
            let mut arr = vec![1, 1, 2, 2, 3, 4, 4, 5];
            let len = remove_duplicates(&mut arr);
            assert_eq!(len, 5);
            assert_eq!(&arr[..len], &[1, 2, 3, 4, 5]);
        }

        #[test]
        fn test_remove_duplicates_all_same() {
            let mut arr = vec![1, 1, 1, 1];
            let len = remove_duplicates(&mut arr);
            assert_eq!(len, 1);
        }

        #[test]
        fn test_container_with_most_water() {
            let heights = vec![1, 8, 6, 2, 5, 4, 8, 3, 7];
            assert_eq!(container_with_most_water(&heights), 49);
        }

        #[test]
        fn test_trap_water() {
            let heights = vec![0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1];
            assert_eq!(trap_water(&heights), 6);
        }

        #[test]
        fn test_trap_water_empty() {
            assert_eq!(trap_water(&[]), 0);
            assert_eq!(trap_water(&[1, 2]), 0);
        }
    }

    mod sliding_window_tests {
        use super::*;

        #[test]
        fn test_max_sum_subarray() {
            let arr = vec![1, 4, 2, 10, 23, 3, 1, 0, 20];
            assert_eq!(max_sum_subarray(&arr, 4), Some(39));
        }

        #[test]
        fn test_max_sum_subarray_edge() {
            assert_eq!(max_sum_subarray(&[], 1), None);
            assert_eq!(max_sum_subarray(&[1, 2, 3], 0), None);
            assert_eq!(max_sum_subarray(&[1, 2], 5), None);
        }

        #[test]
        fn test_longest_substring_without_repeating() {
            assert_eq!(longest_substring_without_repeating("abcabcbb"), 3);
            assert_eq!(longest_substring_without_repeating("bbbbb"), 1);
            assert_eq!(longest_substring_without_repeating("pwwkew"), 3);
            assert_eq!(longest_substring_without_repeating(""), 0);
        }

        #[test]
        fn test_min_window_substring() {
            assert_eq!(
                min_window_substring("ADOBECODEBANC", "ABC"),
                Some("BANC".to_string())
            );
            assert_eq!(min_window_substring("a", "a"), Some("a".to_string()));
            assert_eq!(min_window_substring("a", "aa"), None);
        }

        #[test]
        fn test_longest_substring_k_distinct() {
            assert_eq!(longest_substring_k_distinct("eceba", 2), 3);
            assert_eq!(longest_substring_k_distinct("aa", 1), 2);
            assert_eq!(longest_substring_k_distinct("abc", 0), 0);
        }
    }
}
