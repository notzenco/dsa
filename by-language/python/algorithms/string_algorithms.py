"""
String Algorithms.

This module contains implementations of string pattern matching and manipulation
algorithms with comprehensive documentation, complexity analysis, and LeetCode mappings.

╔═══════════════════════════════════════════════════════════════════════════════╗
║                           STRING ALGORITHMS                                   ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                                                               ║
║  PATTERN MATCHING:                                                            ║
║  ┌─────────────────────────────────────────────────────────────────┐         ║
║  │ Text:    "ABABDABACDABABCABAB"                                  │         ║
║  │ Pattern: "ABABCABAB"                                             │         ║
║  │                                                                  │         ║
║  │ Naive:     Compare at each position - O(n*m)                    │         ║
║  │ KMP:       Use failure function - O(n+m)                        │         ║
║  │ Rabin-Karp: Use rolling hash - O(n+m) average                   │         ║
║  │ Z-Algo:    Use Z-array - O(n+m)                                 │         ║
║  └─────────────────────────────────────────────────────────────────┘         ║
║                                                                               ║
║  KMP FAILURE FUNCTION (Partial Match Table):                                 ║
║  ┌───────────────────────────────────────────────────────────────┐           ║
║  │ Pattern: A B A B C A B A B                                     │           ║
║  │ Index:   0 1 2 3 4 5 6 7 8                                     │           ║
║  │ Failure: 0 0 1 2 0 1 2 3 4                                     │           ║
║  │                                                                │           ║
║  │ failure[i] = length of longest proper prefix                  │           ║
║  │              that is also a suffix of pattern[0..i]           │           ║
║  └───────────────────────────────────────────────────────────────┘           ║
║                                                                               ║
║  Z-ARRAY:                                                                     ║
║  ┌───────────────────────────────────────────────────────────────┐           ║
║  │ String: A A B A A C D                                          │           ║
║  │ Z:      X 1 0 2 1 0 0                                          │           ║
║  │                                                                │           ║
║  │ Z[i] = length of longest substring starting at i              │           ║
║  │        that is also a prefix of the string                    │           ║
║  └───────────────────────────────────────────────────────────────┘           ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY SUMMARY:
┌──────────────────┬────────────────┬──────────────┬─────────────────────────┐
│ Algorithm        │ Time           │ Space        │ Best For                │
├──────────────────┼────────────────┼──────────────┼─────────────────────────┤
│ Naive            │ O(n*m)         │ O(1)         │ Small inputs            │
│ KMP              │ O(n+m)         │ O(m)         │ Single pattern          │
│ Rabin-Karp       │ O(n+m) avg     │ O(1)         │ Multiple patterns       │
│ Z-Algorithm      │ O(n+m)         │ O(n+m)       │ All occurrences         │
│ Aho-Corasick     │ O(n+m+z)       │ O(m*k)       │ Multiple patterns       │
│ Manacher         │ O(n)           │ O(n)         │ Palindromes             │
│ Suffix Array     │ O(n log n)     │ O(n)         │ Multiple queries        │
└──────────────────┴────────────────┴──────────────┴─────────────────────────┘

LEETCODE PROBLEMS:
- #28 Find the Index of the First Occurrence in a String
- #459 Repeated Substring Pattern
- #214 Shortest Palindrome
- #5 Longest Palindromic Substring
- #647 Palindromic Substrings
- #1392 Longest Happy Prefix
"""

from typing import List, Optional


# =============================================================================
# NAIVE PATTERN MATCHING
# =============================================================================

def naive_search(text: str, pattern: str) -> List[int]:
    """
    Find all occurrences of pattern in text using naive approach.

    Time: O(n*m) where n=len(text), m=len(pattern)
    Space: O(1) excluding output

    Args:
        text: The text to search in
        pattern: The pattern to search for

    Returns:
        List of starting indices where pattern occurs
    """
    if not pattern or not text:
        return []

    n, m = len(text), len(pattern)
    if m > n:
        return []

    result = []
    for i in range(n - m + 1):
        match = True
        for j in range(m):
            if text[i + j] != pattern[j]:
                match = False
                break
        if match:
            result.append(i)

    return result


# =============================================================================
# KMP (KNUTH-MORRIS-PRATT) ALGORITHM
# =============================================================================

def compute_lps(pattern: str) -> List[int]:
    """
    Compute the Longest Proper Prefix which is also Suffix (LPS) array.
    Also known as the failure function or partial match table.

    Time: O(m) where m = len(pattern)
    Space: O(m)

    Args:
        pattern: The pattern to compute LPS for

    Returns:
        LPS array where lps[i] = length of longest proper prefix
        of pattern[0..i] which is also a suffix
    """
    m = len(pattern)
    lps = [0] * m

    length = 0  # Length of previous longest prefix suffix
    i = 1

    while i < m:
        if pattern[i] == pattern[length]:
            length += 1
            lps[i] = length
            i += 1
        else:
            if length != 0:
                # Try shorter prefix
                length = lps[length - 1]
            else:
                lps[i] = 0
                i += 1

    return lps


def kmp_search(text: str, pattern: str) -> List[int]:
    """
    Find all occurrences of pattern in text using KMP algorithm.

    The key insight is that when a mismatch occurs, we don't need to
    re-examine characters that we know will match based on the LPS array.

    Time: O(n + m)
    Space: O(m)

    LeetCode: #28 Find the Index of the First Occurrence in a String

    Args:
        text: The text to search in
        pattern: The pattern to search for

    Returns:
        List of starting indices where pattern occurs
    """
    if not pattern:
        return [0] if text else []
    if not text:
        return []

    n, m = len(text), len(pattern)
    if m > n:
        return []

    lps = compute_lps(pattern)
    result = []

    i = 0  # Index for text
    j = 0  # Index for pattern

    while i < n:
        if text[i] == pattern[j]:
            i += 1
            j += 1

            if j == m:
                # Found a match
                result.append(i - m)
                j = lps[j - 1]
        else:
            if j != 0:
                # Use LPS to skip comparisons
                j = lps[j - 1]
            else:
                i += 1

    return result


def kmp_search_first(text: str, pattern: str) -> int:
    """
    Find the first occurrence of pattern in text using KMP.

    Time: O(n + m)
    Space: O(m)

    Args:
        text: The text to search in
        pattern: The pattern to search for

    Returns:
        Starting index of first occurrence, or -1 if not found
    """
    matches = kmp_search(text, pattern)
    return matches[0] if matches else -1


# =============================================================================
# RABIN-KARP ALGORITHM
# =============================================================================

def rabin_karp_search(text: str, pattern: str, prime: int = 101) -> List[int]:
    """
    Find all occurrences of pattern in text using Rabin-Karp algorithm.

    Uses a rolling hash to compute hash values in O(1) after initial computation.

    Time: O(n + m) average, O(n*m) worst case (many hash collisions)
    Space: O(1) excluding output

    Args:
        text: The text to search in
        pattern: The pattern to search for
        prime: Prime number for hash computation

    Returns:
        List of starting indices where pattern occurs
    """
    if not pattern or not text:
        return []

    n, m = len(text), len(pattern)
    if m > n:
        return []

    result = []
    d = 256  # Number of characters in alphabet

    # Calculate d^(m-1) % prime
    h = pow(d, m - 1, prime)

    # Calculate hash values for pattern and first window of text
    pattern_hash = 0
    text_hash = 0

    for i in range(m):
        pattern_hash = (d * pattern_hash + ord(pattern[i])) % prime
        text_hash = (d * text_hash + ord(text[i])) % prime

    # Slide the pattern over text
    for i in range(n - m + 1):
        # Check if hash values match
        if pattern_hash == text_hash:
            # Verify character by character (to handle hash collisions)
            if text[i:i + m] == pattern:
                result.append(i)

        # Calculate hash for next window
        if i < n - m:
            # Remove leading digit, add trailing digit
            text_hash = (d * (text_hash - ord(text[i]) * h) + ord(text[i + m])) % prime

            # Handle negative hash
            if text_hash < 0:
                text_hash += prime

    return result


def rabin_karp_multiple(text: str, patterns: List[str]) -> dict:
    """
    Find all occurrences of multiple patterns in text.

    This is where Rabin-Karp shines - we can check multiple patterns
    at each position using a hash set.

    Time: O(n * k + m) where k = number of patterns, m = total pattern length
    Space: O(k)

    Args:
        text: The text to search in
        patterns: List of patterns to search for

    Returns:
        Dictionary mapping each pattern to list of starting indices
    """
    result = {pattern: [] for pattern in patterns}

    if not text or not patterns:
        return result

    n = len(text)
    prime = 101
    d = 256

    # Group patterns by length and compute their hashes
    length_groups = {}
    for pattern in patterns:
        m = len(pattern)
        if m > n:
            continue
        if m not in length_groups:
            length_groups[m] = {}

        # Compute pattern hash
        pattern_hash = 0
        for char in pattern:
            pattern_hash = (d * pattern_hash + ord(char)) % prime

        if pattern_hash not in length_groups[m]:
            length_groups[m][pattern_hash] = []
        length_groups[m][pattern_hash].append(pattern)

    # For each pattern length, slide window over text
    for m, hash_to_patterns in length_groups.items():
        h = pow(d, m - 1, prime)
        text_hash = 0

        # Initial hash
        for i in range(m):
            text_hash = (d * text_hash + ord(text[i])) % prime

        # Slide window
        for i in range(n - m + 1):
            if text_hash in hash_to_patterns:
                # Check each pattern with this hash
                for pattern in hash_to_patterns[text_hash]:
                    if text[i:i + m] == pattern:
                        result[pattern].append(i)

            # Roll hash
            if i < n - m:
                text_hash = (d * (text_hash - ord(text[i]) * h) + ord(text[i + m])) % prime
                if text_hash < 0:
                    text_hash += prime

    return result


# =============================================================================
# Z-ALGORITHM
# =============================================================================

def compute_z_array(s: str) -> List[int]:
    """
    Compute the Z-array for string s.

    Z[i] = length of longest substring starting at i that is also a prefix of s.
    Z[0] is undefined (set to 0 or len(s) depending on convention).

    Time: O(n)
    Space: O(n)

    LeetCode: #1392 Longest Happy Prefix

    Args:
        s: Input string

    Returns:
        Z-array
    """
    n = len(s)
    if n == 0:
        return []

    z = [0] * n
    z[0] = n  # Convention: z[0] = length of string

    # [l, r] is the rightmost Z-box found so far
    l, r = 0, 0

    for i in range(1, n):
        if i < r:
            # i is inside the current Z-box
            # z[i] is at least min(z[i-l], r-i)
            z[i] = min(z[i - l], r - i)

        # Try to extend z[i]
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1

        # Update Z-box if needed
        if i + z[i] > r:
            l, r = i, i + z[i]

    return z


def z_algorithm_search(text: str, pattern: str) -> List[int]:
    """
    Find all occurrences of pattern in text using Z-algorithm.

    Concatenate pattern + '$' + text and compute Z-array.
    Positions where Z[i] == len(pattern) are matches.

    Time: O(n + m)
    Space: O(n + m)

    Args:
        text: The text to search in
        pattern: The pattern to search for

    Returns:
        List of starting indices where pattern occurs
    """
    if not pattern or not text:
        return []

    m = len(pattern)
    if m > len(text):
        return []

    # Concatenate pattern + separator + text
    concat = pattern + '$' + text
    z = compute_z_array(concat)

    result = []
    for i in range(m + 1, len(concat)):
        if z[i] == m:
            result.append(i - m - 1)

    return result


# =============================================================================
# MANACHER'S ALGORITHM (Longest Palindromic Substring)
# =============================================================================

def manacher(s: str) -> List[int]:
    """
    Compute palindrome radii for all centers using Manacher's algorithm.

    Transforms string to handle even-length palindromes uniformly.
    "abc" -> "#a#b#c#"

    Time: O(n)
    Space: O(n)

    Args:
        s: Input string

    Returns:
        Array p where p[i] = radius of palindrome centered at i in transformed string
    """
    if not s:
        return []

    # Transform string
    t = '#' + '#'.join(s) + '#'
    n = len(t)
    p = [0] * n

    center = 0  # Center of rightmost palindrome
    right = 0   # Right edge of rightmost palindrome

    for i in range(n):
        # Use mirror if inside current palindrome
        if i < right:
            mirror = 2 * center - i
            p[i] = min(right - i, p[mirror])

        # Try to expand
        while (i - p[i] - 1 >= 0 and
               i + p[i] + 1 < n and
               t[i - p[i] - 1] == t[i + p[i] + 1]):
            p[i] += 1

        # Update center and right if needed
        if i + p[i] > right:
            center = i
            right = i + p[i]

    return p


def longest_palindrome_manacher(s: str) -> str:
    """
    Find the longest palindromic substring using Manacher's algorithm.

    Time: O(n)
    Space: O(n)

    LeetCode: #5 Longest Palindromic Substring

    Args:
        s: Input string

    Returns:
        Longest palindromic substring
    """
    if not s:
        return ""

    p = manacher(s)

    # Find the maximum radius and its center
    max_radius = 0
    max_center = 0
    for i, radius in enumerate(p):
        if radius > max_radius:
            max_radius = radius
            max_center = i

    # Convert back to original indices
    # In transformed string "#a#b#c#", index i corresponds to
    # original index (i - 1) // 2
    start = (max_center - max_radius) // 2
    length = max_radius

    return s[start:start + length]


def count_palindromic_substrings(s: str) -> int:
    """
    Count all palindromic substrings using Manacher's algorithm.

    Time: O(n)
    Space: O(n)

    LeetCode: #647 Palindromic Substrings

    Args:
        s: Input string

    Returns:
        Number of palindromic substrings
    """
    if not s:
        return 0

    p = manacher(s)

    # Each palindrome of radius r contributes (r + 1) // 2 palindromic substrings
    # in the original string (for '#' positions) or (r // 2) + 1 for character positions
    count = 0
    for i, radius in enumerate(p):
        # For each center, count number of palindromes
        # radius = max expansion, so there are (radius + 1) // 2 palindromes
        # centered here in the original string
        count += (radius + 1) // 2

    return count


# =============================================================================
# SUFFIX ARRAY
# =============================================================================

def build_suffix_array(s: str) -> List[int]:
    """
    Build suffix array for string s.

    Suffix array contains starting indices of all suffixes in sorted order.

    Time: O(n log n) using sorting
    Space: O(n)

    Args:
        s: Input string

    Returns:
        Suffix array
    """
    n = len(s)
    if n == 0:
        return []

    # Create list of (suffix, starting index)
    suffixes = [(s[i:], i) for i in range(n)]

    # Sort by suffix
    suffixes.sort(key=lambda x: x[0])

    return [idx for _, idx in suffixes]


def build_suffix_array_optimized(s: str) -> List[int]:
    """
    Build suffix array using prefix doubling (O(n log n) comparisons).

    More efficient for large strings.

    Time: O(n log^2 n) with sorting, O(n log n) with radix sort
    Space: O(n)

    Args:
        s: Input string

    Returns:
        Suffix array
    """
    n = len(s)
    if n == 0:
        return []
    if n == 1:
        return [0]

    # Initialize with single character ranks
    rank = [ord(c) for c in s]
    sa = list(range(n))
    tmp = [0] * n

    k = 1
    while k < n:
        # Sort by (rank[i], rank[i+k])
        def key(i):
            return (rank[i], rank[i + k] if i + k < n else -1)

        sa.sort(key=key)

        # Compute new ranks
        tmp[sa[0]] = 0
        for i in range(1, n):
            tmp[sa[i]] = tmp[sa[i - 1]]
            if key(sa[i]) != key(sa[i - 1]):
                tmp[sa[i]] += 1

        rank = tmp[:]

        # Early termination if all ranks are unique
        if rank[sa[n - 1]] == n - 1:
            break

        k *= 2

    return sa


def build_lcp_array(s: str, sa: List[int]) -> List[int]:
    """
    Build LCP (Longest Common Prefix) array from suffix array.

    LCP[i] = length of longest common prefix between suffix[sa[i]] and suffix[sa[i-1]]

    Time: O(n) using Kasai's algorithm
    Space: O(n)

    Args:
        s: Input string
        sa: Suffix array of s

    Returns:
        LCP array
    """
    n = len(s)
    if n == 0:
        return []

    # Compute inverse suffix array (rank of each suffix)
    rank = [0] * n
    for i, suffix_idx in enumerate(sa):
        rank[suffix_idx] = i

    lcp = [0] * n
    k = 0

    for i in range(n):
        if rank[i] == 0:
            k = 0
            continue

        j = sa[rank[i] - 1]

        while i + k < n and j + k < n and s[i + k] == s[j + k]:
            k += 1

        lcp[rank[i]] = k

        if k > 0:
            k -= 1

    return lcp


def search_with_suffix_array(text: str, pattern: str, sa: List[int]) -> List[int]:
    """
    Search for pattern in text using precomputed suffix array.

    Time: O(m log n) where m = len(pattern), n = len(text)
    Space: O(1) excluding input

    Args:
        text: The text to search in
        pattern: The pattern to search for
        sa: Precomputed suffix array of text

    Returns:
        List of starting indices where pattern occurs
    """
    if not pattern or not text:
        return []

    n = len(text)
    m = len(pattern)

    # Binary search for leftmost occurrence
    left = 0
    right = n
    while left < right:
        mid = (left + right) // 2
        suffix = text[sa[mid]:]
        if suffix[:m] < pattern:
            left = mid + 1
        else:
            right = mid

    start = left

    # Binary search for rightmost occurrence
    right = n
    while left < right:
        mid = (left + right) // 2
        suffix = text[sa[mid]:]
        if suffix[:m] <= pattern:
            left = mid + 1
        else:
            right = mid

    end = left

    return sorted([sa[i] for i in range(start, end)])


# =============================================================================
# UTILITY FUNCTIONS
# =============================================================================

def longest_common_prefix(strs: List[str]) -> str:
    """
    Find the longest common prefix among array of strings.

    Time: O(S) where S = sum of all string lengths
    Space: O(1)

    LeetCode: #14 Longest Common Prefix

    Args:
        strs: List of strings

    Returns:
        Longest common prefix
    """
    if not strs:
        return ""

    # Use the first string as reference
    prefix = strs[0]

    for s in strs[1:]:
        # Trim prefix until it matches
        while not s.startswith(prefix):
            prefix = prefix[:-1]
            if not prefix:
                return ""

    return prefix


def repeated_substring_pattern(s: str) -> bool:
    """
    Check if string can be constructed by repeating a substring.

    Trick: If s can be formed by repeating pattern p, then s + s contains
    another occurrence of s (other than the one starting at index 0 and len(s)).

    Time: O(n)
    Space: O(n)

    LeetCode: #459 Repeated Substring Pattern

    Args:
        s: Input string

    Returns:
        True if s can be formed by repeating a substring
    """
    if not s:
        return False

    doubled = (s + s)[1:-1]  # Remove first and last char
    return s in doubled


def shortest_palindrome(s: str) -> str:
    """
    Find the shortest palindrome by adding characters in front.

    Uses KMP to find the longest palindromic prefix.

    Time: O(n)
    Space: O(n)

    LeetCode: #214 Shortest Palindrome

    Args:
        s: Input string

    Returns:
        Shortest palindrome formed by adding characters in front
    """
    if not s:
        return ""

    # Create pattern: s + '#' + reversed(s)
    rev = s[::-1]
    concat = s + '#' + rev

    # Compute LPS array
    lps = compute_lps(concat)

    # Length of longest palindromic prefix
    palindrome_len = lps[-1]

    # Add the non-palindromic suffix in reverse
    return rev[:len(s) - palindrome_len] + s


def longest_happy_prefix(s: str) -> str:
    """
    Find the longest happy prefix (prefix that is also a suffix).

    Time: O(n)
    Space: O(n)

    LeetCode: #1392 Longest Happy Prefix

    Args:
        s: Input string

    Returns:
        Longest happy prefix
    """
    if not s:
        return ""

    lps = compute_lps(s)
    return s[:lps[-1]]
