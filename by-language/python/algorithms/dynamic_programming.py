"""
Dynamic Programming Algorithms Implementation.

Classic DP problems with multiple solution approaches showing
the progression from recursion to memoization to tabulation.

================================================================================
                     DYNAMIC PROGRAMMING VISUAL EXAMPLES
================================================================================

    FIBONACCI - Overlapping Subproblems:

    fib(5)
    ├── fib(4)
    │   ├── fib(3)
    │   │   ├── fib(2) ─┐
    │   │   └── fib(1)  │ These subproblems
    │   └── fib(2) ─────┘ are computed multiple times!
    └── fib(3)
        ├── fib(2)
        └── fib(1)

    KNAPSACK - 2D DP Table:

    Items: [(weight=1, value=6), (weight=2, value=10), (weight=3, value=12)]
    Capacity: 5

    dp[i][w] = max value using first i items with capacity w

         w:  0   1   2   3   4   5
    i=0:    [0] [0] [0] [0] [0] [0]
    i=1:    [0] [6] [6] [6] [6] [6]
    i=2:    [0] [6] [10][16][16][16]
    i=3:    [0] [6] [10][16][18][22]

================================================================================

LEETCODE PROBLEMS:
- #70 Climbing Stairs (Fibonacci)
- #322 Coin Change
- #518 Coin Change 2
- #1143 Longest Common Subsequence
- #300 Longest Increasing Subsequence
- #72 Edit Distance
- #0/1 Knapsack variations
- #198 House Robber
- #53 Maximum Subarray (Kadane's)
"""

from typing import List, Tuple, Optional
from functools import lru_cache


# =============================================================================
# Fibonacci Variants
# =============================================================================

def fibonacci_recursive(n: int) -> int:
    """
    Fibonacci using naive recursion.

    Time: O(2^n) - exponential
    Space: O(n) - recursion stack
    """
    if n <= 1:
        return n
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2)


def fibonacci_memoized(n: int) -> int:
    """
    Fibonacci with memoization (top-down DP).

    Time: O(n)
    Space: O(n)
    """
    @lru_cache(maxsize=None)
    def fib(k: int) -> int:
        if k <= 1:
            return k
        return fib(k - 1) + fib(k - 2)

    return fib(n)


def fibonacci_tabulation(n: int) -> int:
    """
    Fibonacci with tabulation (bottom-up DP).

    Time: O(n)
    Space: O(n)
    """
    if n <= 1:
        return n

    dp = [0] * (n + 1)
    dp[1] = 1

    for i in range(2, n + 1):
        dp[i] = dp[i - 1] + dp[i - 2]

    return dp[n]


def fibonacci_optimized(n: int) -> int:
    """
    Fibonacci with space optimization.

    Time: O(n)
    Space: O(1)
    """
    if n <= 1:
        return n

    prev2, prev1 = 0, 1
    for _ in range(2, n + 1):
        curr = prev1 + prev2
        prev2 = prev1
        prev1 = curr

    return prev1


def fibonacci_matrix(n: int) -> int:
    """
    Fibonacci using matrix exponentiation.

    Time: O(log n)
    Space: O(1)
    """
    if n <= 1:
        return n

    def matrix_mult(A: List[List[int]], B: List[List[int]]) -> List[List[int]]:
        return [
            [A[0][0] * B[0][0] + A[0][1] * B[1][0],
             A[0][0] * B[0][1] + A[0][1] * B[1][1]],
            [A[1][0] * B[0][0] + A[1][1] * B[1][0],
             A[1][0] * B[0][1] + A[1][1] * B[1][1]]
        ]

    def matrix_pow(M: List[List[int]], p: int) -> List[List[int]]:
        result = [[1, 0], [0, 1]]  # Identity

        while p > 0:
            if p & 1:
                result = matrix_mult(result, M)
            M = matrix_mult(M, M)
            p >>= 1

        return result

    M = [[1, 1], [1, 0]]
    result = matrix_pow(M, n)
    return result[0][1]


# =============================================================================
# Knapsack Problems
# =============================================================================

def knapsack_01(weights: List[int], values: List[int], capacity: int) -> int:
    """
    0/1 Knapsack - each item can be used at most once.

    Args:
        weights: Weight of each item
        values: Value of each item
        capacity: Maximum weight capacity

    Returns:
        Maximum value achievable

    Time: O(n * capacity)
    Space: O(n * capacity)
    """
    n = len(weights)
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        for w in range(capacity + 1):
            # Don't take item i-1
            dp[i][w] = dp[i - 1][w]

            # Take item i-1 if possible
            if weights[i - 1] <= w:
                dp[i][w] = max(dp[i][w],
                              dp[i - 1][w - weights[i - 1]] + values[i - 1])

    return dp[n][capacity]


def knapsack_01_optimized(weights: List[int], values: List[int], capacity: int) -> int:
    """
    0/1 Knapsack with space optimization.

    Time: O(n * capacity)
    Space: O(capacity)
    """
    dp = [0] * (capacity + 1)

    for i in range(len(weights)):
        # Traverse backwards to avoid using same item twice
        for w in range(capacity, weights[i] - 1, -1):
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i])

    return dp[capacity]


def knapsack_01_items(weights: List[int], values: List[int],
                      capacity: int) -> Tuple[int, List[int]]:
    """
    0/1 Knapsack with item reconstruction.

    Returns:
        Tuple of (max_value, list of item indices taken)
    """
    n = len(weights)
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        for w in range(capacity + 1):
            dp[i][w] = dp[i - 1][w]
            if weights[i - 1] <= w:
                dp[i][w] = max(dp[i][w],
                              dp[i - 1][w - weights[i - 1]] + values[i - 1])

    # Backtrack to find items
    items = []
    w = capacity
    for i in range(n, 0, -1):
        if dp[i][w] != dp[i - 1][w]:
            items.append(i - 1)
            w -= weights[i - 1]

    return dp[n][capacity], items[::-1]


def knapsack_unbounded(weights: List[int], values: List[int], capacity: int) -> int:
    """
    Unbounded Knapsack - each item can be used unlimited times.

    Time: O(n * capacity)
    Space: O(capacity)
    """
    dp = [0] * (capacity + 1)

    for w in range(1, capacity + 1):
        for i in range(len(weights)):
            if weights[i] <= w:
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i])

    return dp[capacity]


# =============================================================================
# Longest Common Subsequence (LCS)
# =============================================================================

def lcs_length(s1: str, s2: str) -> int:
    """
    Find length of longest common subsequence.

    LeetCode #1143

    Time: O(m * n)
    Space: O(m * n)
    """
    m, n = len(s1), len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    return dp[m][n]


def lcs_string(s1: str, s2: str) -> str:
    """
    Find the actual longest common subsequence string.

    Time: O(m * n)
    Space: O(m * n)
    """
    m, n = len(s1), len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    # Backtrack to find the string
    lcs = []
    i, j = m, n
    while i > 0 and j > 0:
        if s1[i - 1] == s2[j - 1]:
            lcs.append(s1[i - 1])
            i -= 1
            j -= 1
        elif dp[i - 1][j] > dp[i][j - 1]:
            i -= 1
        else:
            j -= 1

    return ''.join(reversed(lcs))


def lcs_optimized(s1: str, s2: str) -> int:
    """
    LCS with space optimization.

    Time: O(m * n)
    Space: O(min(m, n))
    """
    if len(s1) < len(s2):
        s1, s2 = s2, s1

    m, n = len(s1), len(s2)
    prev = [0] * (n + 1)
    curr = [0] * (n + 1)

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                curr[j] = prev[j - 1] + 1
            else:
                curr[j] = max(prev[j], curr[j - 1])
        prev, curr = curr, prev

    return prev[n]


# =============================================================================
# Longest Increasing Subsequence (LIS)
# =============================================================================

def lis_length(arr: List[int]) -> int:
    """
    Find length of longest increasing subsequence.

    LeetCode #300

    Time: O(n^2)
    Space: O(n)
    """
    if not arr:
        return 0

    n = len(arr)
    dp = [1] * n  # dp[i] = LIS ending at i

    for i in range(1, n):
        for j in range(i):
            if arr[j] < arr[i]:
                dp[i] = max(dp[i], dp[j] + 1)

    return max(dp)


def lis_binary_search(arr: List[int]) -> int:
    """
    LIS using binary search.

    Time: O(n log n)
    Space: O(n)
    """
    if not arr:
        return 0

    # tails[i] = smallest ending element of LIS with length i+1
    tails = []

    for num in arr:
        # Binary search for position
        left, right = 0, len(tails)
        while left < right:
            mid = (left + right) // 2
            if tails[mid] < num:
                left = mid + 1
            else:
                right = mid

        if left == len(tails):
            tails.append(num)
        else:
            tails[left] = num

    return len(tails)


def lis_sequence(arr: List[int]) -> List[int]:
    """
    Find the actual longest increasing subsequence.

    Time: O(n log n)
    Space: O(n)
    """
    if not arr:
        return []

    n = len(arr)
    tails = []  # Smallest tail for each length
    indices = []  # Index in arr for each tail
    parent = [-1] * n  # Parent pointers for reconstruction

    for i, num in enumerate(arr):
        left, right = 0, len(tails)
        while left < right:
            mid = (left + right) // 2
            if tails[mid] < num:
                left = mid + 1
            else:
                right = mid

        if left == len(tails):
            tails.append(num)
            indices.append(i)
        else:
            tails[left] = num
            indices[left] = i

        if left > 0:
            parent[i] = indices[left - 1]

    # Reconstruct sequence
    result = []
    idx = indices[-1]
    while idx != -1:
        result.append(arr[idx])
        idx = parent[idx]

    return result[::-1]


# =============================================================================
# Edit Distance
# =============================================================================

def edit_distance(s1: str, s2: str) -> int:
    """
    Minimum edit distance (Levenshtein distance).

    LeetCode #72

    Time: O(m * n)
    Space: O(m * n)
    """
    m, n = len(s1), len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    # Base cases
    for i in range(m + 1):
        dp[i][0] = i  # Delete all characters from s1
    for j in range(n + 1):
        dp[0][j] = j  # Insert all characters to s1

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]  # No operation needed
            else:
                dp[i][j] = 1 + min(
                    dp[i - 1][j],      # Delete
                    dp[i][j - 1],      # Insert
                    dp[i - 1][j - 1]   # Replace
                )

    return dp[m][n]


def edit_distance_optimized(s1: str, s2: str) -> int:
    """
    Edit distance with space optimization.

    Time: O(m * n)
    Space: O(min(m, n))
    """
    if len(s1) < len(s2):
        s1, s2 = s2, s1

    m, n = len(s1), len(s2)
    prev = list(range(n + 1))
    curr = [0] * (n + 1)

    for i in range(1, m + 1):
        curr[0] = i
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                curr[j] = prev[j - 1]
            else:
                curr[j] = 1 + min(prev[j], curr[j - 1], prev[j - 1])
        prev, curr = curr, prev

    return prev[n]


# =============================================================================
# Coin Change
# =============================================================================

def coin_change_min(coins: List[int], amount: int) -> int:
    """
    Minimum number of coins to make amount.

    LeetCode #322

    Args:
        coins: Available coin denominations
        amount: Target amount

    Returns:
        Minimum coins needed, or -1 if impossible

    Time: O(amount * len(coins))
    Space: O(amount)
    """
    dp = [float('inf')] * (amount + 1)
    dp[0] = 0

    for a in range(1, amount + 1):
        for coin in coins:
            if coin <= a:
                dp[a] = min(dp[a], dp[a - coin] + 1)

    return dp[amount] if dp[amount] != float('inf') else -1


def coin_change_ways(coins: List[int], amount: int) -> int:
    """
    Number of ways to make amount.

    LeetCode #518

    Time: O(amount * len(coins))
    Space: O(amount)
    """
    dp = [0] * (amount + 1)
    dp[0] = 1

    for coin in coins:
        for a in range(coin, amount + 1):
            dp[a] += dp[a - coin]

    return dp[amount]


# =============================================================================
# Other Classic DP Problems
# =============================================================================

def climbing_stairs(n: int) -> int:
    """
    Number of ways to climb n stairs (1 or 2 steps at a time).

    LeetCode #70

    Time: O(n)
    Space: O(1)
    """
    if n <= 2:
        return n

    prev2, prev1 = 1, 2
    for _ in range(3, n + 1):
        curr = prev1 + prev2
        prev2, prev1 = prev1, curr

    return prev1


def house_robber(nums: List[int]) -> int:
    """
    Maximum amount that can be robbed (no adjacent houses).

    LeetCode #198

    Time: O(n)
    Space: O(1)
    """
    if not nums:
        return 0
    if len(nums) == 1:
        return nums[0]

    prev2, prev1 = 0, nums[0]

    for i in range(1, len(nums)):
        curr = max(prev1, prev2 + nums[i])
        prev2, prev1 = prev1, curr

    return prev1


def max_subarray_sum(nums: List[int]) -> int:
    """
    Maximum sum of contiguous subarray (Kadane's algorithm).

    LeetCode #53

    Time: O(n)
    Space: O(1)
    """
    if not nums:
        return 0

    max_sum = curr_sum = nums[0]

    for num in nums[1:]:
        curr_sum = max(num, curr_sum + num)
        max_sum = max(max_sum, curr_sum)

    return max_sum


def longest_palindromic_substring(s: str) -> str:
    """
    Find longest palindromic substring.

    LeetCode #5

    Time: O(n^2)
    Space: O(1)
    """
    if not s:
        return ""

    start, max_len = 0, 1

    def expand_around_center(left: int, right: int) -> Tuple[int, int]:
        while left >= 0 and right < len(s) and s[left] == s[right]:
            left -= 1
            right += 1
        return left + 1, right - left - 1

    for i in range(len(s)):
        # Odd length palindrome
        l1, len1 = expand_around_center(i, i)
        if len1 > max_len:
            start, max_len = l1, len1

        # Even length palindrome
        l2, len2 = expand_around_center(i, i + 1)
        if len2 > max_len:
            start, max_len = l2, len2

    return s[start:start + max_len]


def word_break(s: str, word_dict: List[str]) -> bool:
    """
    Check if string can be segmented into dictionary words.

    LeetCode #139

    Time: O(n^2)
    Space: O(n)
    """
    word_set = set(word_dict)
    n = len(s)
    dp = [False] * (n + 1)
    dp[0] = True

    for i in range(1, n + 1):
        for j in range(i):
            if dp[j] and s[j:i] in word_set:
                dp[i] = True
                break

    return dp[n]


def unique_paths(m: int, n: int) -> int:
    """
    Number of unique paths in m x n grid from top-left to bottom-right.

    LeetCode #62

    Time: O(m * n)
    Space: O(n)
    """
    dp = [1] * n

    for _ in range(1, m):
        for j in range(1, n):
            dp[j] += dp[j - 1]

    return dp[n - 1]


def min_path_sum(grid: List[List[int]]) -> int:
    """
    Minimum path sum from top-left to bottom-right.

    LeetCode #64

    Time: O(m * n)
    Space: O(n)
    """
    if not grid or not grid[0]:
        return 0

    m, n = len(grid), len(grid[0])
    dp = [float('inf')] * n
    dp[0] = 0

    for i in range(m):
        dp[0] += grid[i][0]
        for j in range(1, n):
            dp[j] = min(dp[j], dp[j - 1]) + grid[i][j]

    return dp[n - 1]
