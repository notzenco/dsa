# Dynamic Programming

Dynamic Programming (DP) solves complex problems by breaking them into overlapping subproblems, storing solutions to avoid redundant computation.

## Implementations

| Algorithm | Python | C |
|-----------|--------|---|
| DP Algorithms | [dynamic_programming.py](../../by-language/python/algorithms/dynamic_programming.py) | [dynamic_programming.c](../../by-language/c/algorithms/dynamic_programming.c) |

## Complexity Summary

| Problem | Time | Space | Optimized Space |
|---------|------|-------|-----------------|
| Fibonacci | O(n) | O(n) | O(1) |
| 0/1 Knapsack | O(nW) | O(nW) | O(W) |
| Unbounded Knapsack | O(nW) | O(W) | O(W) |
| LCS | O(mn) | O(mn) | O(min(m,n)) |
| LIS | O(n²) or O(n log n) | O(n) | O(n) |
| Edit Distance | O(mn) | O(mn) | O(min(m,n)) |
| Coin Change | O(nS) | O(S) | O(S) |
| Matrix Chain | O(n³) | O(n²) | O(n²) |

## DP Patterns

### 1. Linear DP
State depends on previous elements in sequence.

#### Fibonacci
```python
# Bottom-up with O(1) space
def fibonacci(n):
    if n <= 1:
        return n
    prev, curr = 0, 1
    for _ in range(2, n + 1):
        prev, curr = curr, prev + curr
    return curr
```

#### Climbing Stairs
```python
def climb_stairs(n):
    if n <= 2:
        return n
    prev, curr = 1, 2
    for _ in range(3, n + 1):
        prev, curr = curr, prev + curr
    return curr
```

### 2. Knapsack Pattern
Choose items with constraints on weight/capacity.

#### 0/1 Knapsack
```python
def knapsack_01(weights, values, capacity):
    n = len(weights)
    dp = [0] * (capacity + 1)

    for i in range(n):
        for w in range(capacity, weights[i] - 1, -1):
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i])

    return dp[capacity]
```

#### Unbounded Knapsack
```python
def knapsack_unbounded(weights, values, capacity):
    dp = [0] * (capacity + 1)

    for w in range(1, capacity + 1):
        for i in range(len(weights)):
            if weights[i] <= w:
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i])

    return dp[capacity]
```

#### Coin Change (Minimum Coins)
```python
def coin_change(coins, amount):
    dp = [float('inf')] * (amount + 1)
    dp[0] = 0

    for coin in coins:
        for x in range(coin, amount + 1):
            dp[x] = min(dp[x], dp[x - coin] + 1)

    return dp[amount] if dp[amount] != float('inf') else -1
```

#### Coin Change (Number of Ways)
```python
def coin_change_ways(coins, amount):
    dp = [0] * (amount + 1)
    dp[0] = 1

    for coin in coins:
        for x in range(coin, amount + 1):
            dp[x] += dp[x - coin]

    return dp[amount]
```

### 3. String DP
Operations on one or two strings.

#### Longest Common Subsequence (LCS)
```python
def lcs(s1, s2):
    m, n = len(s1), len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i-1] == s2[j-1]:
                dp[i][j] = dp[i-1][j-1] + 1
            else:
                dp[i][j] = max(dp[i-1][j], dp[i][j-1])

    return dp[m][n]
```

#### Edit Distance (Levenshtein)
```python
def edit_distance(s1, s2):
    m, n = len(s1), len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(m + 1):
        dp[i][0] = i
    for j in range(n + 1):
        dp[0][j] = j

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i-1] == s2[j-1]:
                dp[i][j] = dp[i-1][j-1]
            else:
                dp[i][j] = 1 + min(dp[i-1][j],      # Delete
                                   dp[i][j-1],      # Insert
                                   dp[i-1][j-1])    # Replace

    return dp[m][n]
```

### 4. Longest Increasing Subsequence (LIS)
```python
# O(n²) solution
def lis_n2(nums):
    n = len(nums)
    dp = [1] * n

    for i in range(1, n):
        for j in range(i):
            if nums[j] < nums[i]:
                dp[i] = max(dp[i], dp[j] + 1)

    return max(dp)

# O(n log n) solution with binary search
import bisect

def lis_nlogn(nums):
    tails = []

    for num in nums:
        pos = bisect.bisect_left(tails, num)
        if pos == len(tails):
            tails.append(num)
        else:
            tails[pos] = num

    return len(tails)
```

### 5. Grid DP
Problems on 2D grids.

#### Unique Paths
```python
def unique_paths(m, n):
    dp = [1] * n

    for _ in range(1, m):
        for j in range(1, n):
            dp[j] += dp[j-1]

    return dp[n-1]
```

#### Minimum Path Sum
```python
def min_path_sum(grid):
    m, n = len(grid), len(grid[0])
    dp = [float('inf')] * n
    dp[0] = 0

    for i in range(m):
        dp[0] += grid[i][0]
        for j in range(1, n):
            dp[j] = min(dp[j], dp[j-1]) + grid[i][j]

    return dp[n-1]
```

### 6. Interval DP
Problems on ranges/intervals.

#### Matrix Chain Multiplication
```python
def matrix_chain(dims):
    n = len(dims) - 1
    dp = [[0] * n for _ in range(n)]

    for length in range(2, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            dp[i][j] = float('inf')
            for k in range(i, j):
                cost = dp[i][k] + dp[k+1][j] + dims[i]*dims[k+1]*dims[j+1]
                dp[i][j] = min(dp[i][j], cost)

    return dp[0][n-1]
```

#### Palindrome Partitioning (Minimum Cuts)
```python
def min_cut(s):
    n = len(s)
    # is_palindrome[i][j] = True if s[i:j+1] is palindrome
    is_pal = [[False] * n for _ in range(n)]

    for i in range(n):
        is_pal[i][i] = True
    for i in range(n - 1):
        is_pal[i][i + 1] = s[i] == s[i + 1]
    for length in range(3, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            is_pal[i][j] = s[i] == s[j] and is_pal[i + 1][j - 1]

    # dp[i] = minimum cuts for s[0:i+1]
    dp = list(range(n))

    for i in range(1, n):
        if is_pal[0][i]:
            dp[i] = 0
        else:
            for j in range(i):
                if is_pal[j + 1][i]:
                    dp[i] = min(dp[i], dp[j] + 1)

    return dp[n - 1]
```

### 7. Tree DP
DP on tree structures.

#### House Robber III
```python
def rob(root):
    def dfs(node):
        if not node:
            return (0, 0)  # (rob_this, skip_this)

        left = dfs(node.left)
        right = dfs(node.right)

        rob_this = node.val + left[1] + right[1]
        skip_this = max(left) + max(right)

        return (rob_this, skip_this)

    return max(dfs(root))
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 5 | Longest Palindromic Substring | Medium | String DP |
| 10 | Regular Expression Matching | Hard | String DP |
| 22 | Generate Parentheses | Medium | Backtrack/DP |
| 32 | Longest Valid Parentheses | Hard | Linear DP |
| 44 | Wildcard Matching | Hard | String DP |
| 53 | Maximum Subarray | Medium | Kadane's |
| 62 | Unique Paths | Medium | Grid DP |
| 63 | Unique Paths II | Medium | Grid DP |
| 64 | Minimum Path Sum | Medium | Grid DP |
| 70 | Climbing Stairs | Easy | Linear DP |
| 72 | Edit Distance | Medium | String DP |
| 91 | Decode Ways | Medium | Linear DP |
| 97 | Interleaving String | Medium | String DP |
| 115 | Distinct Subsequences | Hard | String DP |
| 120 | Triangle | Medium | Grid DP |
| 121 | Best Time to Buy and Sell Stock | Easy | State machine |
| 122 | Best Time II | Medium | State machine |
| 123 | Best Time III | Hard | State machine |
| 139 | Word Break | Medium | Linear DP |
| 140 | Word Break II | Hard | DP + backtrack |
| 152 | Maximum Product Subarray | Medium | Linear DP |
| 188 | Best Time IV | Hard | State machine |
| 198 | House Robber | Medium | Linear DP |
| 213 | House Robber II | Medium | Circular DP |
| 221 | Maximal Square | Medium | Grid DP |
| 256 | Paint House | Medium | Linear DP |
| 264 | Ugly Number II | Medium | Linear DP |
| 279 | Perfect Squares | Medium | Knapsack |
| 300 | Longest Increasing Subsequence | Medium | LIS |
| 309 | Best Time with Cooldown | Medium | State machine |
| 312 | Burst Balloons | Hard | Interval DP |
| 322 | Coin Change | Medium | Knapsack |
| 337 | House Robber III | Medium | Tree DP |
| 354 | Russian Doll Envelopes | Hard | LIS |
| 368 | Largest Divisible Subset | Medium | LIS variant |
| 377 | Combination Sum IV | Medium | Knapsack |
| 416 | Partition Equal Subset Sum | Medium | Knapsack |
| 446 | Arithmetic Slices II | Hard | LIS variant |
| 464 | Can I Win | Medium | Bitmask DP |
| 472 | Concatenated Words | Hard | Word Break |
| 474 | Ones and Zeroes | Medium | 2D Knapsack |
| 494 | Target Sum | Medium | Knapsack |
| 509 | Fibonacci Number | Easy | Linear DP |
| 516 | Longest Palindromic Subsequence | Medium | String DP |
| 518 | Coin Change 2 | Medium | Knapsack |
| 583 | Delete Operation for Two Strings | Medium | LCS |
| 639 | Decode Ways II | Hard | Linear DP |
| 647 | Palindromic Substrings | Medium | String DP |
| 673 | Number of Longest Increasing Subsequence | Medium | LIS |
| 712 | Minimum ASCII Delete Sum | Medium | String DP |
| 714 | Best Time with Transaction Fee | Medium | State machine |
| 746 | Min Cost Climbing Stairs | Easy | Linear DP |
| 787 | Cheapest Flights K Stops | Medium | Graph DP |
| 1143 | Longest Common Subsequence | Medium | String DP |

## DP Problem-Solving Framework

1. **Define the state**: What does dp[i] or dp[i][j] represent?
2. **Define the base case**: What are the trivial cases?
3. **Define the transition**: How does dp[i] depend on previous states?
4. **Determine the order**: Fill table in correct order
5. **Optimize space**: Can we reduce from O(n²) to O(n)?

## See Also

- [Graphs](../graphs/README.md) - Shortest path algorithms use DP
- [Strings](../strings/README.md) - String matching with DP
- [Trees](../trees/README.md) - Tree DP problems
- [Arrays](../arrays/README.md) - Many DP problems use arrays
