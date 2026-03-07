package algorithms

// FibonacciRecursive computes fibonacci recursively (exponential time).
func FibonacciRecursive(n int) int {
	if n <= 1 {
		return n
	}
	return FibonacciRecursive(n-1) + FibonacciRecursive(n-2)
}

// FibonacciMemoized computes fibonacci with memoization. O(n)
func FibonacciMemoized(n int) int {
	memo := make(map[int]int)
	var fib func(int) int
	fib = func(n int) int {
		if n <= 1 {
			return n
		}
		if v, ok := memo[n]; ok {
			return v
		}
		memo[n] = fib(n-1) + fib(n-2)
		return memo[n]
	}
	return fib(n)
}

// FibonacciTabulation computes fibonacci with tabulation. O(n)
func FibonacciTabulation(n int) int {
	if n <= 1 {
		return n
	}
	dp := make([]int, n+1)
	dp[1] = 1
	for i := 2; i <= n; i++ {
		dp[i] = dp[i-1] + dp[i-2]
	}
	return dp[n]
}

// FibonacciOptimized computes fibonacci with O(1) space.
func FibonacciOptimized(n int) int {
	if n <= 1 {
		return n
	}
	a, b := 0, 1
	for i := 2; i <= n; i++ {
		a, b = b, a+b
	}
	return b
}

// Knapsack01 solves the 0/1 knapsack problem. O(n*W)
func Knapsack01(weights, values []int, capacity int) int {
	n := len(weights)
	dp := make([][]int, n+1)
	for i := range dp {
		dp[i] = make([]int, capacity+1)
	}
	for i := 1; i <= n; i++ {
		for w := 0; w <= capacity; w++ {
			dp[i][w] = dp[i-1][w]
			if weights[i-1] <= w {
				val := dp[i-1][w-weights[i-1]] + values[i-1]
				if val > dp[i][w] {
					dp[i][w] = val
				}
			}
		}
	}
	return dp[n][capacity]
}

// Knapsack01Optimized uses O(W) space.
func Knapsack01Optimized(weights, values []int, capacity int) int {
	dp := make([]int, capacity+1)
	for i := 0; i < len(weights); i++ {
		for w := capacity; w >= weights[i]; w-- {
			val := dp[w-weights[i]] + values[i]
			if val > dp[w] {
				dp[w] = val
			}
		}
	}
	return dp[capacity]
}

// Knapsack01WithItems returns both max value and selected item indices.
func Knapsack01WithItems(weights, values []int, capacity int) (int, []int) {
	n := len(weights)
	dp := make([][]int, n+1)
	for i := range dp {
		dp[i] = make([]int, capacity+1)
	}
	for i := 1; i <= n; i++ {
		for w := 0; w <= capacity; w++ {
			dp[i][w] = dp[i-1][w]
			if weights[i-1] <= w {
				val := dp[i-1][w-weights[i-1]] + values[i-1]
				if val > dp[i][w] {
					dp[i][w] = val
				}
			}
		}
	}

	// Backtrack to find items
	var items []int
	w := capacity
	for i := n; i > 0; i-- {
		if dp[i][w] != dp[i-1][w] {
			items = append([]int{i - 1}, items...)
			w -= weights[i-1]
		}
	}
	return dp[n][capacity], items
}

// KnapsackUnbounded solves the unbounded knapsack problem.
func KnapsackUnbounded(weights, values []int, capacity int) int {
	dp := make([]int, capacity+1)
	for w := 1; w <= capacity; w++ {
		for i := 0; i < len(weights); i++ {
			if weights[i] <= w {
				val := dp[w-weights[i]] + values[i]
				if val > dp[w] {
					dp[w] = val
				}
			}
		}
	}
	return dp[capacity]
}

// LCSLength computes the length of the longest common subsequence.
func LCSLength(s1, s2 string) int {
	m, n := len(s1), len(s2)
	dp := make([][]int, m+1)
	for i := range dp {
		dp[i] = make([]int, n+1)
	}
	for i := 1; i <= m; i++ {
		for j := 1; j <= n; j++ {
			if s1[i-1] == s2[j-1] {
				dp[i][j] = dp[i-1][j-1] + 1
			} else {
				dp[i][j] = dp[i-1][j]
				if dp[i][j-1] > dp[i][j] {
					dp[i][j] = dp[i][j-1]
				}
			}
		}
	}
	return dp[m][n]
}

// LCSString returns the actual LCS string.
func LCSString(s1, s2 string) string {
	m, n := len(s1), len(s2)
	dp := make([][]int, m+1)
	for i := range dp {
		dp[i] = make([]int, n+1)
	}
	for i := 1; i <= m; i++ {
		for j := 1; j <= n; j++ {
			if s1[i-1] == s2[j-1] {
				dp[i][j] = dp[i-1][j-1] + 1
			} else {
				dp[i][j] = dp[i-1][j]
				if dp[i][j-1] > dp[i][j] {
					dp[i][j] = dp[i][j-1]
				}
			}
		}
	}

	// Backtrack
	result := make([]byte, 0, dp[m][n])
	i, j := m, n
	for i > 0 && j > 0 {
		if s1[i-1] == s2[j-1] {
			result = append([]byte{s1[i-1]}, result...)
			i--
			j--
		} else if dp[i-1][j] > dp[i][j-1] {
			i--
		} else {
			j--
		}
	}
	return string(result)
}

// LISLength computes the length of longest increasing subsequence. O(n log n)
func LISLength(arr []int) int {
	if len(arr) == 0 {
		return 0
	}
	tails := []int{}
	for _, v := range arr {
		// Binary search for position
		lo, hi := 0, len(tails)
		for lo < hi {
			mid := (lo + hi) / 2
			if tails[mid] < v {
				lo = mid + 1
			} else {
				hi = mid
			}
		}
		if lo == len(tails) {
			tails = append(tails, v)
		} else {
			tails[lo] = v
		}
	}
	return len(tails)
}

// LISSequence returns the actual LIS.
func LISSequence(arr []int) []int {
	n := len(arr)
	if n == 0 {
		return nil
	}

	dp := make([]int, n)
	parent := make([]int, n)
	for i := range dp {
		dp[i] = 1
		parent[i] = -1
	}

	maxLen := 1
	maxIdx := 0

	for i := 1; i < n; i++ {
		for j := 0; j < i; j++ {
			if arr[j] < arr[i] && dp[j]+1 > dp[i] {
				dp[i] = dp[j] + 1
				parent[i] = j
			}
		}
		if dp[i] > maxLen {
			maxLen = dp[i]
			maxIdx = i
		}
	}

	result := make([]int, maxLen)
	idx := maxIdx
	for i := maxLen - 1; i >= 0; i-- {
		result[i] = arr[idx]
		idx = parent[idx]
	}
	return result
}

// EditDistance computes the minimum edit distance between two strings.
func EditDistance(s1, s2 string) int {
	m, n := len(s1), len(s2)
	dp := make([][]int, m+1)
	for i := range dp {
		dp[i] = make([]int, n+1)
		dp[i][0] = i
	}
	for j := 0; j <= n; j++ {
		dp[0][j] = j
	}

	for i := 1; i <= m; i++ {
		for j := 1; j <= n; j++ {
			if s1[i-1] == s2[j-1] {
				dp[i][j] = dp[i-1][j-1]
			} else {
				dp[i][j] = 1 + minOf3(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])
			}
		}
	}
	return dp[m][n]
}

func minOf3(a, b, c int) int {
	m := a
	if b < m {
		m = b
	}
	if c < m {
		m = c
	}
	return m
}

// CoinChangeMin returns minimum coins needed for amount, or -1.
func CoinChangeMin(coins []int, amount int) int {
	dp := make([]int, amount+1)
	for i := range dp {
		dp[i] = amount + 1
	}
	dp[0] = 0

	for i := 1; i <= amount; i++ {
		for _, coin := range coins {
			if coin <= i && dp[i-coin]+1 < dp[i] {
				dp[i] = dp[i-coin] + 1
			}
		}
	}
	if dp[amount] > amount {
		return -1
	}
	return dp[amount]
}

// CoinChangeWays returns number of ways to make amount.
func CoinChangeWays(coins []int, amount int) int {
	dp := make([]int, amount+1)
	dp[0] = 1
	for _, coin := range coins {
		for i := coin; i <= amount; i++ {
			dp[i] += dp[i-coin]
		}
	}
	return dp[amount]
}

// ClimbingStairs returns number of ways to climb n stairs.
func ClimbingStairs(n int) int {
	if n <= 2 {
		return n
	}
	a, b := 1, 2
	for i := 3; i <= n; i++ {
		a, b = b, a+b
	}
	return b
}

// HouseRobber returns max sum of non-adjacent elements.
func HouseRobber(nums []int) int {
	if len(nums) == 0 {
		return 0
	}
	if len(nums) == 1 {
		return nums[0]
	}

	prev2, prev1 := 0, 0
	for _, num := range nums {
		curr := prev1
		if prev2+num > curr {
			curr = prev2 + num
		}
		prev2 = prev1
		prev1 = curr
	}
	return prev1
}

// MaxSubarraySum returns the maximum subarray sum (Kadane's algorithm).
func MaxSubarraySum(arr []int) int {
	if len(arr) == 0 {
		return 0
	}
	maxSum := arr[0]
	currentSum := arr[0]
	for i := 1; i < len(arr); i++ {
		if currentSum+arr[i] > arr[i] {
			currentSum = currentSum + arr[i]
		} else {
			currentSum = arr[i]
		}
		if currentSum > maxSum {
			maxSum = currentSum
		}
	}
	return maxSum
}

// LongestPalindromicSubstring returns the longest palindromic substring.
func LongestPalindromicSubstring(s string) string {
	if len(s) <= 1 {
		return s
	}
	start, maxLen := 0, 1

	expand := func(left, right int) {
		for left >= 0 && right < len(s) && s[left] == s[right] {
			if right-left+1 > maxLen {
				start = left
				maxLen = right - left + 1
			}
			left--
			right++
		}
	}

	for i := 0; i < len(s); i++ {
		expand(i, i)     // odd length
		expand(i, i+1)   // even length
	}
	return s[start : start+maxLen]
}

// WordBreak checks if s can be segmented into dictionary words.
func WordBreak(s string, wordDict []string) bool {
	wordSet := make(map[string]bool)
	for _, w := range wordDict {
		wordSet[w] = true
	}

	dp := make([]bool, len(s)+1)
	dp[0] = true
	for i := 1; i <= len(s); i++ {
		for j := 0; j < i; j++ {
			if dp[j] && wordSet[s[j:i]] {
				dp[i] = true
				break
			}
		}
	}
	return dp[len(s)]
}

// UniquePaths counts paths in an m x n grid from top-left to bottom-right.
func UniquePaths(m, n int) int {
	dp := make([]int, n)
	for i := range dp {
		dp[i] = 1
	}
	for i := 1; i < m; i++ {
		for j := 1; j < n; j++ {
			dp[j] += dp[j-1]
		}
	}
	return dp[n-1]
}

// MinPathSum finds the minimum path sum in a grid.
func MinPathSum(grid [][]int) int {
	if len(grid) == 0 || len(grid[0]) == 0 {
		return 0
	}
	m, n := len(grid), len(grid[0])
	dp := make([]int, n)
	dp[0] = grid[0][0]
	for j := 1; j < n; j++ {
		dp[j] = dp[j-1] + grid[0][j]
	}
	for i := 1; i < m; i++ {
		dp[0] += grid[i][0]
		for j := 1; j < n; j++ {
			if dp[j] < dp[j-1] {
				dp[j] = dp[j] + grid[i][j]
			} else {
				dp[j] = dp[j-1] + grid[i][j]
			}
		}
	}
	return dp[n-1]
}
