/**
 * Dynamic Programming Algorithms
 *
 * Classic DP problems with iterative and memoized approaches.
 */

/**
 * Fibonacci using iterative bottom-up DP.
 *
 * Time: O(n)
 * Space: O(1)
 */
export function fibonacci(n: number): number {
  if (n <= 1) return n;

  let prev2 = 0;
  let prev1 = 1;

  for (let i = 2; i <= n; i++) {
    const curr = prev1 + prev2;
    prev2 = prev1;
    prev1 = curr;
  }

  return prev1;
}

/**
 * Fibonacci using memoized recursion (top-down DP).
 *
 * Time: O(n)
 * Space: O(n)
 */
export function fibonacciMemo(n: number): number {
  const memo = new Map<number, number>();

  function fib(k: number): number {
    if (k <= 1) return k;
    if (memo.has(k)) return memo.get(k)!;

    const result = fib(k - 1) + fib(k - 2);
    memo.set(k, result);
    return result;
  }

  return fib(n);
}

/**
 * 0/1 Knapsack - each item can be used at most once.
 *
 * Time: O(n * capacity)
 * Space: O(n * capacity)
 *
 * @returns Maximum value achievable
 */
export function knapsack01(
  weights: number[],
  values: number[],
  capacity: number
): number {
  const n = weights.length;
  const dp: number[][] = Array.from({ length: n + 1 }, () =>
    new Array(capacity + 1).fill(0)
  );

  for (let i = 1; i <= n; i++) {
    for (let w = 0; w <= capacity; w++) {
      // Don't take item i-1
      dp[i][w] = dp[i - 1][w];

      // Take item i-1 if possible
      if (weights[i - 1] <= w) {
        dp[i][w] = Math.max(
          dp[i][w],
          dp[i - 1][w - weights[i - 1]] + values[i - 1]
        );
      }
    }
  }

  return dp[n][capacity];
}

/**
 * Unbounded Knapsack - each item can be used unlimited times.
 *
 * Time: O(n * capacity)
 * Space: O(capacity)
 *
 * @returns Maximum value achievable
 */
export function knapsackUnbounded(
  weights: number[],
  values: number[],
  capacity: number
): number {
  const dp = new Array(capacity + 1).fill(0);

  for (let w = 1; w <= capacity; w++) {
    for (let i = 0; i < weights.length; i++) {
      if (weights[i] <= w) {
        dp[w] = Math.max(dp[w], dp[w - weights[i]] + values[i]);
      }
    }
  }

  return dp[capacity];
}

/**
 * Longest Common Subsequence - returns the actual LCS string.
 *
 * Time: O(m * n)
 * Space: O(m * n)
 */
export function longestCommonSubsequence(s1: string, s2: string): string {
  const m = s1.length;
  const n = s2.length;
  const dp: number[][] = Array.from({ length: m + 1 }, () =>
    new Array(n + 1).fill(0)
  );

  for (let i = 1; i <= m; i++) {
    for (let j = 1; j <= n; j++) {
      if (s1[i - 1] === s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

  // Backtrack to find the string
  const lcs: string[] = [];
  let i = m;
  let j = n;
  while (i > 0 && j > 0) {
    if (s1[i - 1] === s2[j - 1]) {
      lcs.push(s1[i - 1]);
      i--;
      j--;
    } else if (dp[i - 1][j] > dp[i][j - 1]) {
      i--;
    } else {
      j--;
    }
  }

  return lcs.reverse().join("");
}

/**
 * Longest Increasing Subsequence - returns the actual LIS array.
 *
 * Uses binary search approach for O(n log n) time.
 *
 * Time: O(n log n)
 * Space: O(n)
 */
export function longestIncreasingSubsequence(arr: number[]): number[] {
  if (arr.length === 0) return [];

  const n = arr.length;
  const tails: number[] = [];
  const indices: number[] = [];
  const parent: number[] = new Array(n).fill(-1);

  for (let i = 0; i < n; i++) {
    const num = arr[i];

    // Binary search for the position
    let left = 0;
    let right = tails.length;
    while (left < right) {
      const mid = Math.floor((left + right) / 2);
      if (tails[mid] < num) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }

    if (left === tails.length) {
      tails.push(num);
      indices.push(i);
    } else {
      tails[left] = num;
      indices[left] = i;
    }

    if (left > 0) {
      parent[i] = indices[left - 1];
    }
  }

  // Reconstruct sequence
  const result: number[] = [];
  let idx = indices[indices.length - 1];
  while (idx !== -1) {
    result.push(arr[idx]);
    idx = parent[idx];
  }

  return result.reverse();
}

/**
 * Edit Distance (Levenshtein distance) - minimum operations to transform s1 into s2.
 *
 * Operations: insert, delete, replace.
 *
 * Time: O(m * n)
 * Space: O(m * n)
 */
export function editDistance(s1: string, s2: string): number {
  const m = s1.length;
  const n = s2.length;
  const dp: number[][] = Array.from({ length: m + 1 }, () =>
    new Array(n + 1).fill(0)
  );

  // Base cases
  for (let i = 0; i <= m; i++) dp[i][0] = i;
  for (let j = 0; j <= n; j++) dp[0][j] = j;

  for (let i = 1; i <= m; i++) {
    for (let j = 1; j <= n; j++) {
      if (s1[i - 1] === s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] =
          1 +
          Math.min(
            dp[i - 1][j], // Delete
            dp[i][j - 1], // Insert
            dp[i - 1][j - 1] // Replace
          );
      }
    }
  }

  return dp[m][n];
}

/**
 * Coin Change - minimum number of coins to make amount.
 *
 * Time: O(amount * len(coins))
 * Space: O(amount)
 *
 * @returns Minimum coins needed, or -1 if impossible
 */
export function coinChange(coins: number[], amount: number): number {
  const dp = new Array(amount + 1).fill(Infinity);
  dp[0] = 0;

  for (let a = 1; a <= amount; a++) {
    for (const coin of coins) {
      if (coin <= a) {
        dp[a] = Math.min(dp[a], dp[a - coin] + 1);
      }
    }
  }

  return dp[amount] === Infinity ? -1 : dp[amount];
}

/**
 * Climbing Stairs - number of ways to climb n stairs (1 or 2 steps at a time).
 *
 * Time: O(n)
 * Space: O(1)
 */
export function climbingStairs(n: number): number {
  if (n <= 2) return n;

  let prev2 = 1;
  let prev1 = 2;

  for (let i = 3; i <= n; i++) {
    const curr = prev1 + prev2;
    prev2 = prev1;
    prev1 = curr;
  }

  return prev1;
}

/**
 * House Robber - maximum amount that can be robbed (no adjacent houses).
 *
 * Time: O(n)
 * Space: O(1)
 */
export function houseRobber(nums: number[]): number {
  if (nums.length === 0) return 0;
  if (nums.length === 1) return nums[0];

  let prev2 = 0;
  let prev1 = nums[0];

  for (let i = 1; i < nums.length; i++) {
    const curr = Math.max(prev1, prev2 + nums[i]);
    prev2 = prev1;
    prev1 = curr;
  }

  return prev1;
}

/**
 * Maximum Subarray (Kadane's algorithm) - maximum sum of contiguous subarray.
 *
 * Time: O(n)
 * Space: O(1)
 */
export function maxSubarray(nums: number[]): number {
  if (nums.length === 0) return 0;

  let maxSum = nums[0];
  let currSum = nums[0];

  for (let i = 1; i < nums.length; i++) {
    currSum = Math.max(nums[i], currSum + nums[i]);
    maxSum = Math.max(maxSum, currSum);
  }

  return maxSum;
}
