# String Algorithms

String algorithms handle text processing, pattern matching, and string manipulation - fundamental to text editors, search engines, and bioinformatics.

## Implementations

| Algorithm | Python | C |
|-----------|--------|---|
| All String Algorithms | [string_algorithms.py](../../by-language/python/algorithms/string_algorithms.py) | [string_algorithms.c](../../by-language/c/algorithms/string_algorithms.c) |
| Trie | [trie.py](../../by-language/python/data_structures/trie.py) | [trie.c](../../by-language/c/data-structures/trie.c) |

## Complexity Comparison

```
┌─────────────────┬────────────────┬────────────┐
│ Algorithm       │ Time           │ Space      │
├─────────────────┼────────────────┼────────────┤
│ Naive Search    │ O(nm)          │ O(1)       │
│ KMP             │ O(n + m)       │ O(m)       │
│ Rabin-Karp      │ O(n + m) avg   │ O(1)       │
│ Z-Algorithm     │ O(n + m)       │ O(n + m)   │
│ Manacher        │ O(n)           │ O(n)       │
│ Suffix Array    │ O(n log n)     │ O(n)       │
└─────────────────┴────────────────┴────────────┘
n = text length, m = pattern length
```

## Pattern Matching Algorithms

### KMP (Knuth-Morris-Pratt)
Uses failure function to skip unnecessary comparisons.

```python
def compute_lps(pattern):
    """Compute Longest Proper Prefix which is also Suffix"""
    m = len(pattern)
    lps = [0] * m
    length = 0
    i = 1

    while i < m:
        if pattern[i] == pattern[length]:
            length += 1
            lps[i] = length
            i += 1
        elif length != 0:
            length = lps[length - 1]
        else:
            lps[i] = 0
            i += 1

    return lps

def kmp_search(text, pattern):
    n, m = len(text), len(pattern)
    lps = compute_lps(pattern)

    matches = []
    i = j = 0

    while i < n:
        if text[i] == pattern[j]:
            i += 1
            j += 1
            if j == m:
                matches.append(i - j)
                j = lps[j - 1]
        elif j != 0:
            j = lps[j - 1]
        else:
            i += 1

    return matches
```

**Best for**: Single pattern search, when pattern is fixed

### Rabin-Karp
Uses rolling hash for efficient comparison.

```python
def rabin_karp(text, pattern):
    n, m = len(text), len(pattern)
    if m > n:
        return []

    base = 256
    mod = 10**9 + 7
    h = pow(base, m - 1, mod)

    pattern_hash = 0
    text_hash = 0
    for i in range(m):
        pattern_hash = (pattern_hash * base + ord(pattern[i])) % mod
        text_hash = (text_hash * base + ord(text[i])) % mod

    matches = []
    for i in range(n - m + 1):
        if pattern_hash == text_hash:
            if text[i:i + m] == pattern:  # Verify match
                matches.append(i)

        if i < n - m:
            text_hash = (base * (text_hash - ord(text[i]) * h) + ord(text[i + m])) % mod
            if text_hash < 0:
                text_hash += mod

    return matches
```

**Best for**: Multiple pattern search, plagiarism detection

### Z-Algorithm
Computes Z-array where Z[i] is length of longest substring starting at i that matches prefix.

```python
def z_function(s):
    n = len(s)
    z = [0] * n
    z[0] = n
    l = r = 0

    for i in range(1, n):
        if i < r:
            z[i] = min(r - i, z[i - l])
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
        if i + z[i] > r:
            l, r = i, i + z[i]

    return z

def z_search(text, pattern):
    combined = pattern + "$" + text
    z = z_function(combined)
    m = len(pattern)

    return [i - m - 1 for i in range(m + 1, len(combined)) if z[i] == m]
```

**Best for**: Finding all occurrences, string compression

### Manacher's Algorithm
Finds longest palindromic substring in O(n).

```python
def manacher(s):
    # Transform: "abc" -> "^#a#b#c#$"
    t = "^#" + "#".join(s) + "#$"
    n = len(t)
    p = [0] * n
    center = right = 0

    for i in range(1, n - 1):
        if i < right:
            p[i] = min(right - i, p[2 * center - i])

        while t[i + p[i] + 1] == t[i - p[i] - 1]:
            p[i] += 1

        if i + p[i] > right:
            center, right = i, i + p[i]

    max_len = max(p)
    center_idx = p.index(max_len)
    start = (center_idx - max_len) // 2

    return s[start:start + max_len]
```

## Common String Patterns

### Anagram Detection
```python
from collections import Counter

def is_anagram(s1, s2):
    return Counter(s1) == Counter(s2)

def find_anagrams(s, p):
    """Find all anagram start indices of p in s"""
    result = []
    p_count = Counter(p)
    s_count = Counter()

    for i, char in enumerate(s):
        s_count[char] += 1

        if i >= len(p):
            left_char = s[i - len(p)]
            s_count[left_char] -= 1
            if s_count[left_char] == 0:
                del s_count[left_char]

        if s_count == p_count:
            result.append(i - len(p) + 1)

    return result
```

### Palindrome Check
```python
def is_palindrome(s):
    left, right = 0, len(s) - 1
    while left < right:
        if s[left] != s[right]:
            return False
        left += 1
        right -= 1
    return True

def longest_palindrome_dp(s):
    n = len(s)
    dp = [[False] * n for _ in range(n)]
    start = 0
    max_len = 1

    # All substrings of length 1
    for i in range(n):
        dp[i][i] = True

    # Check length 2
    for i in range(n - 1):
        if s[i] == s[i + 1]:
            dp[i][i + 1] = True
            start = i
            max_len = 2

    # Check length 3 and above
    for length in range(3, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            if s[i] == s[j] and dp[i + 1][j - 1]:
                dp[i][j] = True
                start = i
                max_len = length

    return s[start:start + max_len]
```

### String Hashing
```python
def polynomial_hash(s, base=31, mod=10**9 + 9):
    hash_val = 0
    power = 1
    for char in s:
        hash_val = (hash_val + (ord(char) - ord('a') + 1) * power) % mod
        power = (power * base) % mod
    return hash_val
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 3 | Longest Substring Without Repeating | Medium | Sliding window |
| 5 | Longest Palindromic Substring | Medium | DP/Manacher |
| 10 | Regular Expression Matching | Hard | DP |
| 14 | Longest Common Prefix | Easy | Comparison |
| 22 | Generate Parentheses | Medium | Backtracking |
| 28 | Find Index of First Occurrence | Easy | KMP/Z-algo |
| 32 | Longest Valid Parentheses | Hard | Stack/DP |
| 44 | Wildcard Matching | Hard | DP |
| 49 | Group Anagrams | Medium | Hashing |
| 65 | Valid Number | Hard | State machine |
| 72 | Edit Distance | Medium | DP |
| 76 | Minimum Window Substring | Hard | Sliding window |
| 91 | Decode Ways | Medium | DP |
| 115 | Distinct Subsequences | Hard | DP |
| 125 | Valid Palindrome | Easy | Two pointers |
| 131 | Palindrome Partitioning | Medium | Backtracking + DP |
| 139 | Word Break | Medium | DP |
| 140 | Word Break II | Hard | DP + backtrack |
| 151 | Reverse Words in a String | Medium | String manipulation |
| 186 | Reverse Words in a String II | Medium | In-place reversal |
| 187 | Repeated DNA Sequences | Medium | Rolling hash |
| 208 | Implement Trie | Medium | Trie |
| 211 | Design Add and Search Words | Medium | Trie + DFS |
| 212 | Word Search II | Hard | Trie + backtrack |
| 214 | Shortest Palindrome | Hard | KMP |
| 242 | Valid Anagram | Easy | Counting |
| 271 | Encode and Decode Strings | Medium | Encoding |
| 290 | Word Pattern | Easy | Hash mapping |
| 336 | Palindrome Pairs | Hard | Trie/Hash |
| 383 | Ransom Note | Easy | Counting |
| 387 | First Unique Character | Easy | Counting |
| 438 | Find All Anagrams | Medium | Sliding window |
| 459 | Repeated Substring Pattern | Easy | KMP/String |
| 516 | Longest Palindromic Subsequence | Medium | DP |
| 520 | Detect Capital | Easy | String rules |
| 647 | Palindromic Substrings | Medium | DP/Manacher |
| 680 | Valid Palindrome II | Easy | Two pointers |
| 686 | Repeated String Match | Medium | Rabin-Karp |
| 727 | Minimum Window Subsequence | Hard | DP |
| 1143 | Longest Common Subsequence | Medium | DP |
| 1392 | Longest Happy Prefix | Hard | KMP/Z-algo |

## Algorithm Selection Guide

| Use Case | Best Choice |
|----------|-------------|
| Single pattern search | KMP |
| Multiple patterns | Rabin-Karp or Aho-Corasick |
| All occurrences | Z-Algorithm |
| Longest palindrome | Manacher |
| Multiple queries | Suffix Array / Trie |
| Autocomplete | Trie |
| Approximate matching | Edit Distance DP |

## See Also

- [Hash Tables](../hash-tables/README.md) - String hashing, anagram detection
- [Trees](../trees/README.md) - Trie data structure
- [Dynamic Programming](../dynamic-programming/README.md) - LCS, Edit Distance
- [Searching](../searching/README.md) - Sliding window patterns
