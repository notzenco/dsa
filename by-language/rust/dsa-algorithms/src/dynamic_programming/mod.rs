//! # Dynamic Programming Algorithms
//!
//! This module contains implementations of classic dynamic programming problems.
//!
//! ## Classic Problems
//!
//! - [`fibonacci`] - Nth Fibonacci number
//! - [`longest_common_subsequence`] - LCS of two sequences
//! - [`longest_increasing_subsequence`] - LIS length
//! - [`edit_distance`] - Levenshtein distance
//! - [`coin_change`] - Minimum coins for target amount
//! - [`knapsack_01`] - 0/1 Knapsack problem
//! - [`max_subarray_sum`] - Kadane's algorithm

use alloc::string::String;
use alloc::vec;
use alloc::vec::Vec;

/// Calculate the nth Fibonacci number.
///
/// Uses O(n) time and O(1) space with iterative approach.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::fibonacci;
///
/// assert_eq!(fibonacci(0), 0);
/// assert_eq!(fibonacci(1), 1);
/// assert_eq!(fibonacci(10), 55);
/// ```
pub fn fibonacci(n: u64) -> u64 {
    if n <= 1 {
        return n;
    }

    let mut prev2 = 0u64;
    let mut prev1 = 1u64;

    for _ in 2..=n {
        let curr = prev1.saturating_add(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    prev1
}

/// Calculate nth Fibonacci with memoization (returns entire sequence).
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::fibonacci_memo;
///
/// let fibs = fibonacci_memo(10);
/// assert_eq!(fibs[10], 55);
/// ```
pub fn fibonacci_memo(n: usize) -> Vec<u64> {
    let mut dp = vec![0u64; n + 1];

    if n >= 1 {
        dp[1] = 1;
    }

    for i in 2..=n {
        dp[i] = dp[i - 1].saturating_add(dp[i - 2]);
    }

    dp
}

/// Longest Common Subsequence - Find the length of the LCS of two strings.
///
/// # Complexity
///
/// - Time: O(m * n)
/// - Space: O(m * n)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::longest_common_subsequence;
///
/// assert_eq!(longest_common_subsequence("abcde", "ace"), 3);
/// assert_eq!(longest_common_subsequence("abc", "def"), 0);
/// ```
pub fn longest_common_subsequence(s1: &str, s2: &str) -> usize {
    let chars1: Vec<char> = s1.chars().collect();
    let chars2: Vec<char> = s2.chars().collect();
    let m = chars1.len();
    let n = chars2.len();

    if m == 0 || n == 0 {
        return 0;
    }

    let mut dp = vec![vec![0usize; n + 1]; m + 1];

    for i in 1..=m {
        for j in 1..=n {
            if chars1[i - 1] == chars2[j - 1] {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = dp[i - 1][j].max(dp[i][j - 1]);
            }
        }
    }

    dp[m][n]
}

/// Longest Common Subsequence - Returns the actual LCS string.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::lcs_string;
///
/// assert_eq!(lcs_string("abcde", "ace"), "ace");
/// ```
pub fn lcs_string(s1: &str, s2: &str) -> String {
    let chars1: Vec<char> = s1.chars().collect();
    let chars2: Vec<char> = s2.chars().collect();
    let m = chars1.len();
    let n = chars2.len();

    if m == 0 || n == 0 {
        return String::new();
    }

    let mut dp = vec![vec![0usize; n + 1]; m + 1];

    for i in 1..=m {
        for j in 1..=n {
            if chars1[i - 1] == chars2[j - 1] {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = dp[i - 1][j].max(dp[i][j - 1]);
            }
        }
    }

    // Backtrack to find the LCS
    let mut result = Vec::new();
    let mut i = m;
    let mut j = n;

    while i > 0 && j > 0 {
        if chars1[i - 1] == chars2[j - 1] {
            result.push(chars1[i - 1]);
            i -= 1;
            j -= 1;
        } else if dp[i - 1][j] > dp[i][j - 1] {
            i -= 1;
        } else {
            j -= 1;
        }
    }

    result.reverse();
    result.into_iter().collect()
}

/// Longest Increasing Subsequence - Find the length of the LIS.
///
/// Uses O(n log n) algorithm with binary search.
///
/// # Complexity
///
/// - Time: O(n log n)
/// - Space: O(n)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::longest_increasing_subsequence;
///
/// assert_eq!(longest_increasing_subsequence(&[10, 9, 2, 5, 3, 7, 101, 18]), 4);
/// assert_eq!(longest_increasing_subsequence(&[0, 1, 0, 3, 2, 3]), 4);
/// ```
pub fn longest_increasing_subsequence(arr: &[i32]) -> usize {
    if arr.is_empty() {
        return 0;
    }

    // tails[i] = smallest tail element for LIS of length i+1
    let mut tails: Vec<i32> = Vec::new();

    for &num in arr {
        // Binary search for position
        let pos = tails.partition_point(|&x| x < num);

        if pos == tails.len() {
            tails.push(num);
        } else {
            tails[pos] = num;
        }
    }

    tails.len()
}

/// LIS with O(n²) algorithm - easier to understand.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::lis_quadratic;
///
/// assert_eq!(lis_quadratic(&[10, 9, 2, 5, 3, 7, 101, 18]), 4);
/// ```
pub fn lis_quadratic(arr: &[i32]) -> usize {
    if arr.is_empty() {
        return 0;
    }

    let n = arr.len();
    let mut dp = vec![1usize; n];

    for i in 1..n {
        for j in 0..i {
            if arr[j] < arr[i] {
                dp[i] = dp[i].max(dp[j] + 1);
            }
        }
    }

    *dp.iter().max().unwrap()
}

/// Edit Distance (Levenshtein Distance) - Minimum operations to transform s1 to s2.
///
/// Operations: insert, delete, replace (each costs 1).
///
/// # Complexity
///
/// - Time: O(m * n)
/// - Space: O(m * n)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::edit_distance;
///
/// assert_eq!(edit_distance("horse", "ros"), 3);
/// assert_eq!(edit_distance("intention", "execution"), 5);
/// ```
pub fn edit_distance(s1: &str, s2: &str) -> usize {
    let chars1: Vec<char> = s1.chars().collect();
    let chars2: Vec<char> = s2.chars().collect();
    let m = chars1.len();
    let n = chars2.len();

    let mut dp = vec![vec![0usize; n + 1]; m + 1];

    // Base cases
    for i in 0..=m {
        dp[i][0] = i;
    }
    for j in 0..=n {
        dp[0][j] = j;
    }

    for i in 1..=m {
        for j in 1..=n {
            if chars1[i - 1] == chars2[j - 1] {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + dp[i - 1][j - 1].min(dp[i - 1][j]).min(dp[i][j - 1]);
            }
        }
    }

    dp[m][n]
}

/// Coin Change - Minimum number of coins to make up the amount.
///
/// Returns -1 if amount cannot be made with given coins.
///
/// # Complexity
///
/// - Time: O(amount * coins.len())
/// - Space: O(amount)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::coin_change;
///
/// assert_eq!(coin_change(&[1, 2, 5], 11), 3);  // 5 + 5 + 1
/// assert_eq!(coin_change(&[2], 3), -1);
/// ```
pub fn coin_change(coins: &[i32], amount: i32) -> i32 {
    if amount < 0 {
        return -1;
    }
    if amount == 0 {
        return 0;
    }

    let amount = amount as usize;
    let mut dp = vec![i32::MAX; amount + 1];
    dp[0] = 0;

    for i in 1..=amount {
        for &coin in coins {
            let coin = coin as usize;
            if coin <= i && dp[i - coin] != i32::MAX {
                dp[i] = dp[i].min(dp[i - coin] + 1);
            }
        }
    }

    if dp[amount] == i32::MAX {
        -1
    } else {
        dp[amount]
    }
}

/// Coin Change 2 - Count number of ways to make up the amount.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::coin_change_ways;
///
/// assert_eq!(coin_change_ways(&[1, 2, 5], 5), 4);  // 4 ways
/// ```
pub fn coin_change_ways(coins: &[i32], amount: i32) -> i32 {
    if amount < 0 {
        return 0;
    }

    let amount = amount as usize;
    let mut dp = vec![0i32; amount + 1];
    dp[0] = 1;

    for &coin in coins {
        let coin = coin as usize;
        for i in coin..=amount {
            dp[i] += dp[i - coin];
        }
    }

    dp[amount]
}

/// 0/1 Knapsack Problem - Maximum value with weight constraint.
///
/// # Complexity
///
/// - Time: O(n * capacity)
/// - Space: O(capacity)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::knapsack_01;
///
/// let values = vec![60, 100, 120];
/// let weights = vec![10, 20, 30];
/// assert_eq!(knapsack_01(&values, &weights, 50), 220);
/// ```
pub fn knapsack_01(values: &[i32], weights: &[i32], capacity: i32) -> i32 {
    if values.is_empty() || capacity <= 0 {
        return 0;
    }

    let capacity = capacity as usize;
    let n = values.len();
    let mut dp = vec![0i32; capacity + 1];

    for i in 0..n {
        let w = weights[i] as usize;
        for c in (w..=capacity).rev() {
            dp[c] = dp[c].max(dp[c - w] + values[i]);
        }
    }

    dp[capacity]
}

/// Unbounded Knapsack - Can use each item multiple times.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::knapsack_unbounded;
///
/// let values = vec![1, 30];
/// let weights = vec![1, 50];
/// assert_eq!(knapsack_unbounded(&values, &weights, 100), 100);
/// ```
pub fn knapsack_unbounded(values: &[i32], weights: &[i32], capacity: i32) -> i32 {
    if values.is_empty() || capacity <= 0 {
        return 0;
    }

    let capacity = capacity as usize;
    let mut dp = vec![0i32; capacity + 1];

    for c in 1..=capacity {
        for i in 0..values.len() {
            let w = weights[i] as usize;
            if w <= c {
                dp[c] = dp[c].max(dp[c - w] + values[i]);
            }
        }
    }

    dp[capacity]
}

/// Maximum Subarray Sum - Kadane's Algorithm.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::max_subarray_sum;
///
/// assert_eq!(max_subarray_sum(&[-2, 1, -3, 4, -1, 2, 1, -5, 4]), 6);
/// ```
pub fn max_subarray_sum(arr: &[i32]) -> i32 {
    if arr.is_empty() {
        return 0;
    }

    let mut max_ending_here = arr[0];
    let mut max_so_far = arr[0];

    for &num in arr.iter().skip(1) {
        max_ending_here = num.max(max_ending_here + num);
        max_so_far = max_so_far.max(max_ending_here);
    }

    max_so_far
}

/// Maximum Subarray Sum with indices.
///
/// Returns (max_sum, start_index, end_index).
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::max_subarray_with_indices;
///
/// let (sum, start, end) = max_subarray_with_indices(&[-2, 1, -3, 4, -1, 2, 1, -5, 4]);
/// assert_eq!(sum, 6);
/// assert_eq!(start, 3);
/// assert_eq!(end, 6);
/// ```
pub fn max_subarray_with_indices(arr: &[i32]) -> (i32, usize, usize) {
    if arr.is_empty() {
        return (0, 0, 0);
    }

    let mut max_ending_here = arr[0];
    let mut max_so_far = arr[0];
    let mut start = 0;
    let mut end = 0;
    let mut temp_start = 0;

    for (i, &num) in arr.iter().enumerate().skip(1) {
        if num > max_ending_here + num {
            max_ending_here = num;
            temp_start = i;
        } else {
            max_ending_here = max_ending_here + num;
        }

        if max_ending_here > max_so_far {
            max_so_far = max_ending_here;
            start = temp_start;
            end = i;
        }
    }

    (max_so_far, start, end)
}

/// Climbing Stairs - Count ways to climb n stairs (1 or 2 steps at a time).
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::climb_stairs;
///
/// assert_eq!(climb_stairs(2), 2);  // 1+1 or 2
/// assert_eq!(climb_stairs(3), 3);  // 1+1+1, 1+2, 2+1
/// ```
pub fn climb_stairs(n: i32) -> i32 {
    if n <= 2 {
        return n.max(0);
    }

    let mut prev2 = 1;
    let mut prev1 = 2;

    for _ in 3..=n {
        let curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    prev1
}

/// House Robber - Maximum money without robbing adjacent houses.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::house_robber;
///
/// assert_eq!(house_robber(&[1, 2, 3, 1]), 4);  // Rob house 0 and 2
/// assert_eq!(house_robber(&[2, 7, 9, 3, 1]), 12);  // Rob house 0, 2, 4
/// ```
pub fn house_robber(nums: &[i32]) -> i32 {
    if nums.is_empty() {
        return 0;
    }
    if nums.len() == 1 {
        return nums[0];
    }

    let mut prev2 = 0;
    let mut prev1 = 0;

    for &num in nums {
        let curr = prev1.max(prev2 + num);
        prev2 = prev1;
        prev1 = curr;
    }

    prev1
}

/// Word Break - Can string be segmented into dictionary words?
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::dynamic_programming::word_break;
///
/// let dict = vec!["leet", "code"];
/// assert!(word_break("leetcode", &dict));
///
/// let dict = vec!["cats", "dog", "sand", "and", "cat"];
/// assert!(!word_break("catsandog", &dict));
/// ```
pub fn word_break(s: &str, word_dict: &[&str]) -> bool {
    let n = s.len();
    let mut dp = vec![false; n + 1];
    dp[0] = true;

    for i in 1..=n {
        for &word in word_dict {
            let word_len = word.len();
            if word_len <= i && dp[i - word_len] && &s[i - word_len..i] == word {
                dp[i] = true;
                break;
            }
        }
    }

    dp[n]
}

#[cfg(test)]
mod tests {
    use super::*;

    mod fibonacci_tests {
        use super::*;

        #[test]
        fn test_fibonacci() {
            assert_eq!(fibonacci(0), 0);
            assert_eq!(fibonacci(1), 1);
            assert_eq!(fibonacci(2), 1);
            assert_eq!(fibonacci(10), 55);
            assert_eq!(fibonacci(20), 6765);
        }

        #[test]
        fn test_fibonacci_memo() {
            let fibs = fibonacci_memo(10);
            assert_eq!(fibs[0], 0);
            assert_eq!(fibs[1], 1);
            assert_eq!(fibs[10], 55);
        }
    }

    mod lcs_tests {
        use super::*;

        #[test]
        fn test_lcs_length() {
            assert_eq!(longest_common_subsequence("abcde", "ace"), 3);
            assert_eq!(longest_common_subsequence("abc", "abc"), 3);
            assert_eq!(longest_common_subsequence("abc", "def"), 0);
        }

        #[test]
        fn test_lcs_string() {
            assert_eq!(lcs_string("abcde", "ace"), "ace");
            assert_eq!(lcs_string("abc", "abc"), "abc");
        }

        #[test]
        fn test_lcs_empty() {
            assert_eq!(longest_common_subsequence("", "abc"), 0);
            assert_eq!(longest_common_subsequence("abc", ""), 0);
        }
    }

    mod lis_tests {
        use super::*;

        #[test]
        fn test_lis() {
            assert_eq!(longest_increasing_subsequence(&[10, 9, 2, 5, 3, 7, 101, 18]), 4);
            assert_eq!(longest_increasing_subsequence(&[0, 1, 0, 3, 2, 3]), 4);
            assert_eq!(longest_increasing_subsequence(&[7, 7, 7, 7, 7]), 1);
        }

        #[test]
        fn test_lis_empty() {
            assert_eq!(longest_increasing_subsequence(&[]), 0);
        }

        #[test]
        fn test_lis_quadratic() {
            assert_eq!(lis_quadratic(&[10, 9, 2, 5, 3, 7, 101, 18]), 4);
        }
    }

    mod edit_distance_tests {
        use super::*;

        #[test]
        fn test_edit_distance() {
            assert_eq!(edit_distance("horse", "ros"), 3);
            assert_eq!(edit_distance("intention", "execution"), 5);
        }

        #[test]
        fn test_edit_distance_empty() {
            assert_eq!(edit_distance("", "abc"), 3);
            assert_eq!(edit_distance("abc", ""), 3);
            assert_eq!(edit_distance("", ""), 0);
        }

        #[test]
        fn test_edit_distance_same() {
            assert_eq!(edit_distance("abc", "abc"), 0);
        }
    }

    mod coin_change_tests {
        use super::*;

        #[test]
        fn test_coin_change() {
            assert_eq!(coin_change(&[1, 2, 5], 11), 3);
            assert_eq!(coin_change(&[2], 3), -1);
            assert_eq!(coin_change(&[1], 0), 0);
        }

        #[test]
        fn test_coin_change_ways() {
            assert_eq!(coin_change_ways(&[1, 2, 5], 5), 4);
            assert_eq!(coin_change_ways(&[2], 3), 0);
        }
    }

    mod knapsack_tests {
        use super::*;

        #[test]
        fn test_knapsack_01() {
            let values = vec![60, 100, 120];
            let weights = vec![10, 20, 30];
            assert_eq!(knapsack_01(&values, &weights, 50), 220);
        }

        #[test]
        fn test_knapsack_01_empty() {
            assert_eq!(knapsack_01(&[], &[], 50), 0);
            assert_eq!(knapsack_01(&[10], &[5], 0), 0);
        }

        #[test]
        fn test_knapsack_unbounded() {
            let values = vec![1, 30];
            let weights = vec![1, 50];
            assert_eq!(knapsack_unbounded(&values, &weights, 100), 100);
        }
    }

    mod subarray_tests {
        use super::*;

        #[test]
        fn test_max_subarray_sum() {
            assert_eq!(max_subarray_sum(&[-2, 1, -3, 4, -1, 2, 1, -5, 4]), 6);
            assert_eq!(max_subarray_sum(&[1]), 1);
            assert_eq!(max_subarray_sum(&[-1, -2, -3]), -1);
        }

        #[test]
        fn test_max_subarray_with_indices() {
            let (sum, start, end) = max_subarray_with_indices(&[-2, 1, -3, 4, -1, 2, 1, -5, 4]);
            assert_eq!(sum, 6);
            assert_eq!(start, 3);
            assert_eq!(end, 6);
        }
    }

    mod other_tests {
        use super::*;

        #[test]
        fn test_climb_stairs() {
            assert_eq!(climb_stairs(2), 2);
            assert_eq!(climb_stairs(3), 3);
            assert_eq!(climb_stairs(5), 8);
        }

        #[test]
        fn test_house_robber() {
            assert_eq!(house_robber(&[1, 2, 3, 1]), 4);
            assert_eq!(house_robber(&[2, 7, 9, 3, 1]), 12);
        }

        #[test]
        fn test_word_break() {
            let dict = vec!["leet", "code"];
            assert!(word_break("leetcode", &dict));

            let dict = vec!["cats", "dog", "sand", "and", "cat"];
            assert!(!word_break("catsandog", &dict));
        }
    }
}
