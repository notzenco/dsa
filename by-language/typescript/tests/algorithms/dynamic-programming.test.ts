import { describe, test, expect } from "bun:test";
import {
  fibonacci,
  fibonacciMemo,
  knapsack01,
  knapsackUnbounded,
  longestCommonSubsequence,
  longestIncreasingSubsequence,
  editDistance,
  coinChange,
  climbingStairs,
  houseRobber,
  maxSubarray,
} from "../../src/algorithms/dynamic-programming";

// =============================================================================
// fibonacci
// =============================================================================

describe("fibonacci", () => {
  test("fib(0) = 0", () => {
    expect(fibonacci(0)).toBe(0);
  });

  test("fib(1) = 1", () => {
    expect(fibonacci(1)).toBe(1);
  });

  test("fib(2) = 1", () => {
    expect(fibonacci(2)).toBe(1);
  });

  test("fib(5) = 5", () => {
    expect(fibonacci(5)).toBe(5);
  });

  test("fib(10) = 55", () => {
    expect(fibonacci(10)).toBe(55);
  });

  test("fib(20) = 6765", () => {
    expect(fibonacci(20)).toBe(6765);
  });

  test("fib(30) = 832040", () => {
    expect(fibonacci(30)).toBe(832040);
  });
});

// =============================================================================
// fibonacciMemo
// =============================================================================

describe("fibonacciMemo", () => {
  test("fib(0) = 0", () => {
    expect(fibonacciMemo(0)).toBe(0);
  });

  test("fib(1) = 1", () => {
    expect(fibonacciMemo(1)).toBe(1);
  });

  test("fib(2) = 1", () => {
    expect(fibonacciMemo(2)).toBe(1);
  });

  test("fib(5) = 5", () => {
    expect(fibonacciMemo(5)).toBe(5);
  });

  test("fib(10) = 55", () => {
    expect(fibonacciMemo(10)).toBe(55);
  });

  test("fib(20) = 6765", () => {
    expect(fibonacciMemo(20)).toBe(6765);
  });

  test("agrees with iterative version", () => {
    for (let i = 0; i <= 25; i++) {
      expect(fibonacciMemo(i)).toBe(fibonacci(i));
    }
  });
});

// =============================================================================
// knapsack01
// =============================================================================

describe("knapsack01", () => {
  test("empty items", () => {
    expect(knapsack01([], [], 10)).toBe(0);
  });

  test("zero capacity", () => {
    expect(knapsack01([1, 2, 3], [10, 20, 30], 0)).toBe(0);
  });

  test("single item fits", () => {
    expect(knapsack01([5], [10], 5)).toBe(10);
  });

  test("single item does not fit", () => {
    expect(knapsack01([5], [10], 3)).toBe(0);
  });

  test("classic example", () => {
    // Items: (weight=1, value=6), (weight=2, value=10), (weight=3, value=12)
    // Capacity: 5
    // Best: take items 0 and 2 for value 18, or items 0 and 1 for value 16
    // Actually: items with w=1,v=6 + w=2,v=10 + w=2 remaining (can't fit w=3)
    // Take all? 1+2+3=6 > 5. Take 1+2=3 <= 5, value=16. Take 2+3=5 <= 5, value=22.
    expect(knapsack01([1, 2, 3], [6, 10, 12], 5)).toBe(22);
  });

  test("all items fit", () => {
    expect(knapsack01([1, 2, 3], [10, 20, 30], 10)).toBe(60);
  });

  test("no items fit", () => {
    expect(knapsack01([10, 20, 30], [10, 20, 30], 5)).toBe(0);
  });

  test("larger example", () => {
    const weights = [2, 3, 4, 5];
    const values = [3, 4, 5, 6];
    expect(knapsack01(weights, values, 5)).toBe(7); // items 0,1 => w=5, v=7
  });
});

// =============================================================================
// knapsackUnbounded
// =============================================================================

describe("knapsackUnbounded", () => {
  test("empty items", () => {
    expect(knapsackUnbounded([], [], 10)).toBe(0);
  });

  test("zero capacity", () => {
    expect(knapsackUnbounded([1, 2, 3], [10, 20, 30], 0)).toBe(0);
  });

  test("single item repeated", () => {
    // weight=2, value=5, capacity=6 => take 3 times for 15
    expect(knapsackUnbounded([2], [5], 6)).toBe(15);
  });

  test("classic example", () => {
    // weight=1 value=1, weight=3 value=4, weight=4 value=5
    // capacity=7
    // Best: take 2x(w=3,v=4) + 1x(w=1,v=1) = w=7, v=9
    expect(knapsackUnbounded([1, 3, 4], [1, 4, 5], 7)).toBe(9);
  });

  test("better to repeat small item", () => {
    // weight=1 value=10, weight=5 value=30, capacity=5
    // 5 of item 0 => 50 vs 1 of item 1 => 30
    expect(knapsackUnbounded([1, 5], [10, 30], 5)).toBe(50);
  });
});

// =============================================================================
// longestCommonSubsequence
// =============================================================================

describe("longestCommonSubsequence", () => {
  test("both empty", () => {
    expect(longestCommonSubsequence("", "")).toBe("");
  });

  test("one empty", () => {
    expect(longestCommonSubsequence("abc", "")).toBe("");
    expect(longestCommonSubsequence("", "abc")).toBe("");
  });

  test("identical strings", () => {
    expect(longestCommonSubsequence("abc", "abc")).toBe("abc");
  });

  test("no common subsequence", () => {
    expect(longestCommonSubsequence("abc", "def")).toBe("");
  });

  test("classic example", () => {
    const result = longestCommonSubsequence("abcde", "ace");
    expect(result).toBe("ace");
  });

  test("another example", () => {
    const result = longestCommonSubsequence("ABCBDAB", "BDCAB");
    expect(result.length).toBe(4);
    // Multiple valid LCS exist: "BCAB", "BDAB", etc.
  });

  test("single character match", () => {
    expect(longestCommonSubsequence("a", "a")).toBe("a");
  });

  test("single character no match", () => {
    expect(longestCommonSubsequence("a", "b")).toBe("");
  });

  test("subsequence at edges", () => {
    const result = longestCommonSubsequence("axb", "ayb");
    expect(result).toBe("ab");
  });
});

// =============================================================================
// longestIncreasingSubsequence
// =============================================================================

describe("longestIncreasingSubsequence", () => {
  test("empty array", () => {
    expect(longestIncreasingSubsequence([])).toEqual([]);
  });

  test("single element", () => {
    expect(longestIncreasingSubsequence([5])).toEqual([5]);
  });

  test("already sorted", () => {
    const result = longestIncreasingSubsequence([1, 2, 3, 4, 5]);
    expect(result).toEqual([1, 2, 3, 4, 5]);
  });

  test("reverse sorted", () => {
    const result = longestIncreasingSubsequence([5, 4, 3, 2, 1]);
    expect(result.length).toBe(1);
  });

  test("classic example", () => {
    const result = longestIncreasingSubsequence([10, 9, 2, 5, 3, 7, 101, 18]);
    expect(result.length).toBe(4);
    // Valid LIS: [2, 3, 7, 18] or [2, 3, 7, 101] or [2, 5, 7, 18] etc.
    // Verify it is actually increasing
    for (let i = 1; i < result.length; i++) {
      expect(result[i]).toBeGreaterThan(result[i - 1]);
    }
  });

  test("all same elements", () => {
    const result = longestIncreasingSubsequence([3, 3, 3, 3]);
    expect(result.length).toBe(1);
  });

  test("two elements increasing", () => {
    expect(longestIncreasingSubsequence([1, 2])).toEqual([1, 2]);
  });

  test("two elements decreasing", () => {
    const result = longestIncreasingSubsequence([2, 1]);
    expect(result.length).toBe(1);
  });

  test("longer example", () => {
    const result = longestIncreasingSubsequence([0, 1, 0, 3, 2, 3]);
    expect(result.length).toBe(4);
    for (let i = 1; i < result.length; i++) {
      expect(result[i]).toBeGreaterThan(result[i - 1]);
    }
  });
});

// =============================================================================
// editDistance
// =============================================================================

describe("editDistance", () => {
  test("both empty", () => {
    expect(editDistance("", "")).toBe(0);
  });

  test("one empty", () => {
    expect(editDistance("abc", "")).toBe(3);
    expect(editDistance("", "abc")).toBe(3);
  });

  test("identical strings", () => {
    expect(editDistance("abc", "abc")).toBe(0);
  });

  test("single replacement", () => {
    expect(editDistance("abc", "adc")).toBe(1);
  });

  test("single insertion", () => {
    expect(editDistance("ac", "abc")).toBe(1);
  });

  test("single deletion", () => {
    expect(editDistance("abc", "ac")).toBe(1);
  });

  test("classic example - horse to ros", () => {
    expect(editDistance("horse", "ros")).toBe(3);
  });

  test("classic example - intention to execution", () => {
    expect(editDistance("intention", "execution")).toBe(5);
  });

  test("completely different", () => {
    expect(editDistance("abc", "def")).toBe(3);
  });

  test("single character", () => {
    expect(editDistance("a", "b")).toBe(1);
    expect(editDistance("a", "a")).toBe(0);
  });
});

// =============================================================================
// coinChange
// =============================================================================

describe("coinChange", () => {
  test("amount 0", () => {
    expect(coinChange([1, 2, 5], 0)).toBe(0);
  });

  test("single coin exact", () => {
    expect(coinChange([5], 5)).toBe(1);
  });

  test("impossible", () => {
    expect(coinChange([2], 3)).toBe(-1);
  });

  test("classic example - 11 with [1,2,5]", () => {
    expect(coinChange([1, 2, 5], 11)).toBe(3); // 5+5+1
  });

  test("classic example - 6 with [1,3,4]", () => {
    expect(coinChange([1, 3, 4], 6)).toBe(2); // 3+3
  });

  test("single coin denomination", () => {
    expect(coinChange([1], 5)).toBe(5);
  });

  test("large coin, small amount", () => {
    expect(coinChange([10], 5)).toBe(-1);
  });

  test("empty coins", () => {
    expect(coinChange([], 5)).toBe(-1);
  });

  test("amount 1 with coin 1", () => {
    expect(coinChange([1], 1)).toBe(1);
  });

  test("greedy fails but DP succeeds", () => {
    // Greedy would pick 6, then can't make 3 from [1,5,6]? Actually 6+1+1+1=9 but 5+1+1+1+1=9
    // Better: coins [1,3,4], amount 6: greedy picks 4+1+1=3 coins, but 3+3=2 coins
    expect(coinChange([1, 3, 4], 6)).toBe(2);
  });
});

// =============================================================================
// climbingStairs
// =============================================================================

describe("climbingStairs", () => {
  test("1 stair", () => {
    expect(climbingStairs(1)).toBe(1);
  });

  test("2 stairs", () => {
    expect(climbingStairs(2)).toBe(2);
  });

  test("3 stairs", () => {
    expect(climbingStairs(3)).toBe(3);
  });

  test("4 stairs", () => {
    expect(climbingStairs(4)).toBe(5);
  });

  test("5 stairs", () => {
    expect(climbingStairs(5)).toBe(8);
  });

  test("10 stairs", () => {
    expect(climbingStairs(10)).toBe(89);
  });

  test("follows fibonacci pattern", () => {
    // climbingStairs(n) = fib(n+1) for n >= 1
    for (let n = 1; n <= 15; n++) {
      expect(climbingStairs(n)).toBe(fibonacci(n + 1));
    }
  });
});

// =============================================================================
// houseRobber
// =============================================================================

describe("houseRobber", () => {
  test("empty array", () => {
    expect(houseRobber([])).toBe(0);
  });

  test("single house", () => {
    expect(houseRobber([5])).toBe(5);
  });

  test("two houses - pick larger", () => {
    expect(houseRobber([1, 2])).toBe(2);
  });

  test("classic example [1,2,3,1]", () => {
    expect(houseRobber([1, 2, 3, 1])).toBe(4); // 1+3
  });

  test("classic example [2,7,9,3,1]", () => {
    expect(houseRobber([2, 7, 9, 3, 1])).toBe(12); // 2+9+1
  });

  test("all same values", () => {
    expect(houseRobber([5, 5, 5, 5, 5])).toBe(15); // skip every other
  });

  test("alternating high and low", () => {
    expect(houseRobber([1, 100, 1, 100, 1])).toBe(200);
  });

  test("single large value in middle", () => {
    expect(houseRobber([1, 1, 100, 1, 1])).toBe(102); // 1+100+1
  });

  test("all zeros", () => {
    expect(houseRobber([0, 0, 0])).toBe(0);
  });
});

// =============================================================================
// maxSubarray
// =============================================================================

describe("maxSubarray", () => {
  test("empty array", () => {
    expect(maxSubarray([])).toBe(0);
  });

  test("single positive element", () => {
    expect(maxSubarray([5])).toBe(5);
  });

  test("single negative element", () => {
    expect(maxSubarray([-3])).toBe(-3);
  });

  test("all positive", () => {
    expect(maxSubarray([1, 2, 3, 4])).toBe(10);
  });

  test("all negative", () => {
    expect(maxSubarray([-1, -2, -3, -4])).toBe(-1);
  });

  test("classic example [-2,1,-3,4,-1,2,1,-5,4]", () => {
    expect(maxSubarray([-2, 1, -3, 4, -1, 2, 1, -5, 4])).toBe(6); // [4,-1,2,1]
  });

  test("positive and negative mix", () => {
    expect(maxSubarray([2, -1, 3, -2, 4])).toBe(6); // [2,-1,3,-2,4]
  });

  test("max at end", () => {
    expect(maxSubarray([-5, -2, 10])).toBe(10);
  });

  test("max at start", () => {
    expect(maxSubarray([10, -2, -5])).toBe(10);
  });

  test("all zeros", () => {
    expect(maxSubarray([0, 0, 0])).toBe(0);
  });
});
