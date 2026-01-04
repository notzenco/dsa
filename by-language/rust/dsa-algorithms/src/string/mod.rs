//! # String Algorithms
//!
//! This module contains implementations of string matching and processing algorithms.
//!
//! ## Pattern Matching
//!
//! - [`kmp_search`] - Knuth-Morris-Pratt algorithm
//! - [`rabin_karp`] - Rabin-Karp algorithm with rolling hash
//! - [`z_algorithm`] - Z-function based matching
//!
//! ## String Processing
//!
//! - [`longest_palindromic_substring`] - Find longest palindrome
//! - [`is_palindrome`] - Check if string is a palindrome

use alloc::string::String;
use alloc::vec;
use alloc::vec::Vec;

// ============================================================================
// KMP Algorithm
// ============================================================================

/// Build the KMP failure function (prefix function).
///
/// For each position i, lps[i] is the length of the longest proper prefix
/// of pattern[0..=i] that is also a suffix.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::kmp_failure_function;
///
/// let lps = kmp_failure_function("ABABAC");
/// assert_eq!(lps, vec![0, 0, 1, 2, 3, 0]);
/// ```
pub fn kmp_failure_function(pattern: &str) -> Vec<usize> {
    let chars: Vec<char> = pattern.chars().collect();
    let m = chars.len();

    if m == 0 {
        return vec![];
    }

    let mut lps = vec![0usize; m];
    let mut len = 0;
    let mut i = 1;

    while i < m {
        if chars[i] == chars[len] {
            len += 1;
            lps[i] = len;
            i += 1;
        } else if len != 0 {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            i += 1;
        }
    }

    lps
}

/// KMP Search - Find all occurrences of pattern in text.
///
/// # Complexity
///
/// - Time: O(n + m)
/// - Space: O(m)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::kmp_search;
///
/// let positions = kmp_search("ABABDABACDABABCABAB", "ABABCABAB");
/// assert_eq!(positions, vec![10]);
///
/// let positions = kmp_search("AAAAAA", "AA");
/// assert_eq!(positions, vec![0, 1, 2, 3, 4]);
/// ```
pub fn kmp_search(text: &str, pattern: &str) -> Vec<usize> {
    let mut result = Vec::new();

    let text_chars: Vec<char> = text.chars().collect();
    let pattern_chars: Vec<char> = pattern.chars().collect();
    let n = text_chars.len();
    let m = pattern_chars.len();

    if m == 0 || m > n {
        return result;
    }

    let lps = kmp_failure_function(pattern);

    let mut i = 0; // text index
    let mut j = 0; // pattern index

    while i < n {
        if pattern_chars[j] == text_chars[i] {
            i += 1;
            j += 1;
        }

        if j == m {
            result.push(i - j);
            j = lps[j - 1];
        } else if i < n && pattern_chars[j] != text_chars[i] {
            if j != 0 {
                j = lps[j - 1];
            } else {
                i += 1;
            }
        }
    }

    result
}

// ============================================================================
// Rabin-Karp Algorithm
// ============================================================================

/// Rabin-Karp Search - Find all occurrences of pattern using rolling hash.
///
/// # Complexity
///
/// - Time: O(n + m) average, O(nm) worst case (many hash collisions)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::rabin_karp;
///
/// let positions = rabin_karp("GEEKS FOR GEEKS", "GEEK");
/// assert_eq!(positions, vec![0, 10]);
/// ```
pub fn rabin_karp(text: &str, pattern: &str) -> Vec<usize> {
    let mut result = Vec::new();

    let text_bytes = text.as_bytes();
    let pattern_bytes = pattern.as_bytes();
    let n = text_bytes.len();
    let m = pattern_bytes.len();

    if m == 0 || m > n {
        return result;
    }

    const BASE: u64 = 256;
    const MOD: u64 = 1_000_000_007;

    // Calculate h = BASE^(m-1) % MOD
    let mut h: u64 = 1;
    for _ in 0..m - 1 {
        h = (h * BASE) % MOD;
    }

    // Calculate initial hash values
    let mut pattern_hash: u64 = 0;
    let mut text_hash: u64 = 0;

    for i in 0..m {
        pattern_hash = (pattern_hash * BASE + pattern_bytes[i] as u64) % MOD;
        text_hash = (text_hash * BASE + text_bytes[i] as u64) % MOD;
    }

    // Slide the pattern over text
    for i in 0..=n - m {
        if pattern_hash == text_hash {
            // Verify character by character
            if text_bytes[i..i + m] == *pattern_bytes {
                result.push(i);
            }
        }

        // Calculate hash for next window
        if i < n - m {
            text_hash = (text_hash + MOD - (text_bytes[i] as u64 * h) % MOD) % MOD;
            text_hash = (text_hash * BASE + text_bytes[i + m] as u64) % MOD;
        }
    }

    result
}

// ============================================================================
// Z Algorithm
// ============================================================================

/// Z-function - For each position i, z[i] is the length of the longest
/// substring starting at i that matches a prefix of the string.
///
/// # Complexity
///
/// - Time: O(n)
/// - Space: O(n)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::z_function;
///
/// let z = z_function("aabxaab");
/// assert_eq!(z, vec![7, 1, 0, 0, 3, 1, 0]);
/// ```
pub fn z_function(s: &str) -> Vec<usize> {
    let chars: Vec<char> = s.chars().collect();
    let n = chars.len();

    if n == 0 {
        return vec![];
    }

    let mut z = vec![0usize; n];
    z[0] = n;

    let mut l = 0;
    let mut r = 0;

    for i in 1..n {
        if i < r {
            z[i] = (r - i).min(z[i - l]);
        }

        while i + z[i] < n && chars[z[i]] == chars[i + z[i]] {
            z[i] += 1;
        }

        if i + z[i] > r {
            l = i;
            r = i + z[i];
        }
    }

    z
}

/// Z-Algorithm Search - Find all occurrences of pattern in text.
///
/// # Complexity
///
/// - Time: O(n + m)
/// - Space: O(n + m)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::z_search;
///
/// let positions = z_search("GEEKS FOR GEEKS", "GEEK");
/// assert_eq!(positions, vec![0, 10]);
/// ```
pub fn z_search(text: &str, pattern: &str) -> Vec<usize> {
    let mut result = Vec::new();

    let pattern_len = pattern.chars().count();
    let text_len = text.chars().count();

    if pattern_len == 0 || pattern_len > text_len {
        return result;
    }

    // Concatenate pattern + special char + text
    let concat = format!("{}\0{}", pattern, text);
    let z = z_function(&concat);

    // Find matches
    for i in pattern_len + 1..z.len() {
        if z[i] == pattern_len {
            result.push(i - pattern_len - 1);
        }
    }

    result
}

// ============================================================================
// Palindrome Algorithms
// ============================================================================

/// Check if a string is a palindrome.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::is_palindrome;
///
/// assert!(is_palindrome("racecar"));
/// assert!(is_palindrome("A man a plan a canal Panama"));
/// assert!(!is_palindrome("hello"));
/// ```
pub fn is_palindrome(s: &str) -> bool {
    let chars: Vec<char> = s
        .chars()
        .filter(|c| c.is_alphanumeric())
        .map(|c| c.to_ascii_lowercase())
        .collect();

    if chars.is_empty() {
        return true;
    }

    let mut left = 0;
    let mut right = chars.len() - 1;

    while left < right {
        if chars[left] != chars[right] {
            return false;
        }
        left += 1;
        right -= 1;
    }

    true
}

/// Find the longest palindromic substring using expand around center.
///
/// # Complexity
///
/// - Time: O(n²)
/// - Space: O(1)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::longest_palindromic_substring;
///
/// assert_eq!(longest_palindromic_substring("babad"), "bab");
/// assert_eq!(longest_palindromic_substring("cbbd"), "bb");
/// ```
pub fn longest_palindromic_substring(s: &str) -> String {
    let chars: Vec<char> = s.chars().collect();
    let n = chars.len();

    if n <= 1 {
        return s.to_string();
    }

    let mut start = 0;
    let mut max_len = 1;

    let expand = |left: i32, right: i32| -> (usize, usize) {
        let mut l = left;
        let mut r = right;

        while l >= 0 && (r as usize) < n && chars[l as usize] == chars[r as usize] {
            l -= 1;
            r += 1;
        }

        ((l + 1) as usize, (r - l - 1) as usize)
    };

    for i in 0..n {
        // Odd length palindrome
        let (s1, len1) = expand(i as i32, i as i32);
        if len1 > max_len {
            start = s1;
            max_len = len1;
        }

        // Even length palindrome
        if i + 1 < n {
            let (s2, len2) = expand(i as i32, (i + 1) as i32);
            if len2 > max_len {
                start = s2;
                max_len = len2;
            }
        }
    }

    chars[start..start + max_len].iter().collect()
}

/// Find all palindromic substrings.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::count_palindromic_substrings;
///
/// assert_eq!(count_palindromic_substrings("abc"), 3);  // a, b, c
/// assert_eq!(count_palindromic_substrings("aaa"), 6);  // a, a, a, aa, aa, aaa
/// ```
pub fn count_palindromic_substrings(s: &str) -> usize {
    let chars: Vec<char> = s.chars().collect();
    let n = chars.len();

    if n == 0 {
        return 0;
    }

    let mut count = 0;

    let count_from_center = |left: i32, right: i32| -> usize {
        let mut cnt = 0;
        let mut l = left;
        let mut r = right;

        while l >= 0 && (r as usize) < n && chars[l as usize] == chars[r as usize] {
            cnt += 1;
            l -= 1;
            r += 1;
        }

        cnt
    };

    for i in 0..n {
        count += count_from_center(i as i32, i as i32); // Odd
        count += count_from_center(i as i32, (i + 1) as i32); // Even
    }

    count
}

/// Manacher's Algorithm - Find longest palindromic substring in O(n).
///
/// Returns (center, length) of the longest palindromic substring.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::string::manacher;
///
/// let (center, len) = manacher("babad");
/// assert!(len >= 3); // "bab" or "aba"
/// ```
pub fn manacher(s: &str) -> (usize, usize) {
    if s.is_empty() {
        return (0, 0);
    }

    // Transform string: "abc" -> "#a#b#c#"
    let chars: Vec<char> = s.chars().collect();
    let mut t = vec!['#'];
    for &c in &chars {
        t.push(c);
        t.push('#');
    }
    let n = t.len();

    let mut p = vec![0usize; n]; // Palindrome radii
    let mut center = 0;
    let mut right = 0;

    for i in 0..n {
        if i < right {
            p[i] = (right - i).min(p[2 * center - i]);
        }

        // Expand around center
        while i >= p[i] + 1 && i + p[i] + 1 < n && t[i - p[i] - 1] == t[i + p[i] + 1] {
            p[i] += 1;
        }

        if i + p[i] > right {
            center = i;
            right = i + p[i];
        }
    }

    // Find maximum
    let max_idx = p.iter().enumerate().max_by_key(|&(_, &v)| v).unwrap().0;
    let max_len = p[max_idx];

    // Convert back to original indices
    let start = (max_idx - max_len) / 2;
    let length = max_len;

    (start, length)
}

#[cfg(test)]
mod tests {
    use super::*;

    mod kmp_tests {
        use super::*;

        #[test]
        fn test_failure_function() {
            assert_eq!(kmp_failure_function("ABABAC"), vec![0, 0, 1, 2, 3, 0]);
            assert_eq!(kmp_failure_function("AAAA"), vec![0, 1, 2, 3]);
            assert_eq!(kmp_failure_function("ABCD"), vec![0, 0, 0, 0]);
        }

        #[test]
        fn test_kmp_search() {
            assert_eq!(kmp_search("ABABDABACDABABCABAB", "ABABCABAB"), vec![10]);
            assert_eq!(kmp_search("AAAAAA", "AA"), vec![0, 1, 2, 3, 4]);
            assert_eq!(kmp_search("ABCDEF", "XYZ"), vec![]);
        }

        #[test]
        fn test_kmp_empty() {
            assert_eq!(kmp_search("ABCDEF", ""), vec![]);
            assert_eq!(kmp_search("", "ABC"), vec![]);
        }

        #[test]
        fn test_kmp_pattern_longer() {
            assert_eq!(kmp_search("ABC", "ABCDEF"), vec![]);
        }
    }

    mod rabin_karp_tests {
        use super::*;

        #[test]
        fn test_rabin_karp() {
            assert_eq!(rabin_karp("GEEKS FOR GEEKS", "GEEK"), vec![0, 10]);
            assert_eq!(rabin_karp("AABAACAADAABAABA", "AABA"), vec![0, 9, 12]);
        }

        #[test]
        fn test_rabin_karp_no_match() {
            assert_eq!(rabin_karp("ABCDEF", "XYZ"), vec![]);
        }

        #[test]
        fn test_rabin_karp_empty() {
            assert_eq!(rabin_karp("ABCDEF", ""), vec![]);
        }
    }

    mod z_tests {
        use super::*;

        #[test]
        fn test_z_function() {
            assert_eq!(z_function("aabxaab"), vec![7, 1, 0, 0, 3, 1, 0]);
            assert_eq!(z_function("aaaaa"), vec![5, 4, 3, 2, 1]);
        }

        #[test]
        fn test_z_search() {
            assert_eq!(z_search("GEEKS FOR GEEKS", "GEEK"), vec![0, 10]);
            assert_eq!(z_search("AABAACAADAABAABA", "AABA"), vec![0, 9, 12]);
        }

        #[test]
        fn test_z_empty() {
            assert_eq!(z_function(""), vec![]);
            assert_eq!(z_search("ABC", ""), vec![]);
        }
    }

    mod palindrome_tests {
        use super::*;

        #[test]
        fn test_is_palindrome() {
            assert!(is_palindrome("racecar"));
            assert!(is_palindrome("A man a plan a canal Panama"));
            assert!(is_palindrome(""));
            assert!(!is_palindrome("hello"));
        }

        #[test]
        fn test_longest_palindromic_substring() {
            let result = longest_palindromic_substring("babad");
            assert!(result == "bab" || result == "aba");

            assert_eq!(longest_palindromic_substring("cbbd"), "bb");
            assert_eq!(longest_palindromic_substring("a"), "a");
        }

        #[test]
        fn test_count_palindromic_substrings() {
            assert_eq!(count_palindromic_substrings("abc"), 3);
            assert_eq!(count_palindromic_substrings("aaa"), 6);
        }

        #[test]
        fn test_manacher() {
            let (_, len) = manacher("babad");
            assert!(len >= 3);

            let (_, len) = manacher("cbbd");
            assert_eq!(len, 2);
        }
    }
}
