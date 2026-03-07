package algorithms

import "testing"

func TestFibonacci(t *testing.T) {
	expected := []int{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}

	for i, e := range expected {
		if FibonacciRecursive(i) != e {
			t.Errorf("recursive fib(%d) expected %d", i, e)
		}
		if FibonacciMemoized(i) != e {
			t.Errorf("memoized fib(%d) expected %d", i, e)
		}
		if FibonacciTabulation(i) != e {
			t.Errorf("tabulation fib(%d) expected %d", i, e)
		}
		if FibonacciOptimized(i) != e {
			t.Errorf("optimized fib(%d) expected %d", i, e)
		}
	}
}

func TestKnapsack01(t *testing.T) {
	weights := []int{2, 3, 4, 5}
	values := []int{3, 4, 5, 6}

	result := Knapsack01(weights, values, 5)
	if result != 7 {
		t.Errorf("expected 7, got %d", result)
	}

	result = Knapsack01Optimized(weights, values, 5)
	if result != 7 {
		t.Errorf("optimized expected 7, got %d", result)
	}
}

func TestKnapsack01WithItems(t *testing.T) {
	weights := []int{2, 3, 4, 5}
	values := []int{3, 4, 5, 6}

	maxVal, items := Knapsack01WithItems(weights, values, 5)
	if maxVal != 7 {
		t.Errorf("expected 7, got %d", maxVal)
	}
	if len(items) == 0 {
		t.Error("should have selected items")
	}
}

func TestKnapsackUnbounded(t *testing.T) {
	weights := []int{2, 3, 4, 5}
	values := []int{3, 4, 5, 6}

	result := KnapsackUnbounded(weights, values, 10)
	if result != 15 {
		t.Errorf("expected 15, got %d", result)
	}
}

func TestLCS(t *testing.T) {
	length := LCSLength("ABCBDAB", "BDCAB")
	if length != 4 {
		t.Errorf("expected LCS length 4, got %d", length)
	}

	s := LCSString("ABCBDAB", "BDCAB")
	if len(s) != 4 {
		t.Errorf("expected LCS string length 4, got %q", s)
	}
}

func TestLIS(t *testing.T) {
	length := LISLength([]int{10, 22, 9, 33, 21, 50, 41, 60})
	if length != 5 {
		t.Errorf("expected LIS length 5, got %d", length)
	}

	seq := LISSequence([]int{10, 22, 9, 33, 21, 50, 41, 60})
	if len(seq) != 5 {
		t.Errorf("expected LIS sequence length 5, got %v", seq)
	}
	for i := 1; i < len(seq); i++ {
		if seq[i] <= seq[i-1] {
			t.Errorf("LIS not increasing: %v", seq)
		}
	}
}

func TestEditDistance(t *testing.T) {
	result := EditDistance("kitten", "sitting")
	if result != 3 {
		t.Errorf("expected 3, got %d", result)
	}

	result = EditDistance("", "abc")
	if result != 3 {
		t.Errorf("expected 3, got %d", result)
	}

	result = EditDistance("abc", "abc")
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}

func TestCoinChange(t *testing.T) {
	result := CoinChangeMin([]int{1, 5, 10, 25}, 30)
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}

	result = CoinChangeMin([]int{2}, 3)
	if result != -1 {
		t.Errorf("expected -1, got %d", result)
	}

	ways := CoinChangeWays([]int{1, 2, 5}, 5)
	if ways != 4 {
		t.Errorf("expected 4 ways, got %d", ways)
	}
}

func TestClimbingStairs(t *testing.T) {
	if ClimbingStairs(1) != 1 {
		t.Error("expected 1")
	}
	if ClimbingStairs(2) != 2 {
		t.Error("expected 2")
	}
	if ClimbingStairs(5) != 8 {
		t.Error("expected 8")
	}
}

func TestHouseRobber(t *testing.T) {
	result := HouseRobber([]int{1, 2, 3, 1})
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}

	result = HouseRobber([]int{2, 7, 9, 3, 1})
	if result != 12 {
		t.Errorf("expected 12, got %d", result)
	}
}

func TestMaxSubarraySum(t *testing.T) {
	result := MaxSubarraySum([]int{-2, 1, -3, 4, -1, 2, 1, -5, 4})
	if result != 6 {
		t.Errorf("expected 6, got %d", result)
	}

	result = MaxSubarraySum([]int{-1, -2, -3})
	if result != -1 {
		t.Errorf("expected -1, got %d", result)
	}
}

func TestLongestPalindromicSubstring(t *testing.T) {
	result := LongestPalindromicSubstring("babad")
	if result != "bab" && result != "aba" {
		t.Errorf("expected 'bab' or 'aba', got %q", result)
	}

	result = LongestPalindromicSubstring("cbbd")
	if result != "bb" {
		t.Errorf("expected 'bb', got %q", result)
	}
}

func TestWordBreak(t *testing.T) {
	if !WordBreak("leetcode", []string{"leet", "code"}) {
		t.Error("expected true")
	}
	if WordBreak("catsandog", []string{"cats", "dog", "sand", "and", "cat"}) {
		t.Error("expected false")
	}
}

func TestUniquePaths(t *testing.T) {
	if UniquePaths(3, 7) != 28 {
		t.Errorf("expected 28, got %d", UniquePaths(3, 7))
	}
	if UniquePaths(1, 1) != 1 {
		t.Errorf("expected 1, got %d", UniquePaths(1, 1))
	}
}

func TestMinPathSum(t *testing.T) {
	grid := [][]int{
		{1, 3, 1},
		{1, 5, 1},
		{4, 2, 1},
	}
	result := MinPathSum(grid)
	if result != 7 {
		t.Errorf("expected 7, got %d", result)
	}
}

func TestMatrixChainMultiplication(t *testing.T) {
	// dims = [10, 30, 5, 60] means matrices 10x30, 30x5, 5x60
	result := MatrixChainMultiplication([]int{10, 30, 5, 60})
	if result != 4500 {
		t.Errorf("expected 4500, got %d", result)
	}

	result = MatrixChainMultiplication([]int{40, 20, 30, 10, 30})
	if result != 26000 {
		t.Errorf("expected 26000, got %d", result)
	}
}

func TestSubsetSum(t *testing.T) {
	if !SubsetSum([]int{3, 34, 4, 12, 5, 2}, 9) {
		t.Error("expected true for target 9")
	}
	if SubsetSum([]int{3, 34, 4, 12, 5, 2}, 30) {
		t.Error("expected false for target 30")
	}
	if !SubsetSum([]int{1, 2, 3}, 0) {
		t.Error("expected true for target 0")
	}
}

func TestPartitionEqualSubset(t *testing.T) {
	if !PartitionEqualSubset([]int{1, 5, 11, 5}) {
		t.Error("expected true")
	}
	if PartitionEqualSubset([]int{1, 2, 3, 5}) {
		t.Error("expected false")
	}
}

func TestFibonacciMatrixExponentiation(t *testing.T) {
	expected := []int{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}
	for i, e := range expected {
		result := FibonacciMatrixExponentiation(i)
		if result != e {
			t.Errorf("fib(%d): expected %d, got %d", i, e, result)
		}
	}
}

func TestRodCutting(t *testing.T) {
	prices := []int{1, 5, 8, 9, 10, 17, 17, 20}
	result := RodCutting(prices, 8)
	if result != 22 {
		t.Errorf("expected 22, got %d", result)
	}

	result = RodCutting(prices, 4)
	if result != 10 {
		t.Errorf("expected 10, got %d", result)
	}
}

func TestPalindromePartitionMin(t *testing.T) {
	result := PalindromePartitionMin("aab")
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}

	result = PalindromePartitionMin("a")
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}

	result = PalindromePartitionMin("ab")
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}
}

func TestMaximalSquare(t *testing.T) {
	matrix := [][]int{
		{1, 0, 1, 0, 0},
		{1, 0, 1, 1, 1},
		{1, 1, 1, 1, 1},
		{1, 0, 0, 1, 0},
	}
	result := MaximalSquare(matrix)
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}
}

func TestMaximalSquareAllZeros(t *testing.T) {
	matrix := [][]int{
		{0, 0},
		{0, 0},
	}
	result := MaximalSquare(matrix)
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}

func TestDecodeWays(t *testing.T) {
	result := DecodeWays("12")
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}

	result = DecodeWays("226")
	if result != 3 {
		t.Errorf("expected 3, got %d", result)
	}

	result = DecodeWays("06")
	if result != 0 {
		t.Errorf("expected 0 for leading zero, got %d", result)
	}

	result = DecodeWays("0")
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}

func TestCoinChangeCombinations(t *testing.T) {
	result := CoinChangeCombinations([]int{1, 2, 5}, 5)
	if result == nil {
		t.Fatal("expected non-nil result")
	}
	// Should return coins that sum to 5
	sum := 0
	for _, c := range result {
		sum += c
	}
	if sum != 5 {
		t.Errorf("expected coins summing to 5, got sum %d from %v", sum, result)
	}
}

func TestCoinChangeCombinationsImpossible(t *testing.T) {
	result := CoinChangeCombinations([]int{2}, 3)
	if result != nil {
		t.Errorf("expected nil for impossible, got %v", result)
	}
}

func TestStockBuySellOnce(t *testing.T) {
	result := StockBuySellOnce([]int{7, 1, 5, 3, 6, 4})
	if result != 5 {
		t.Errorf("expected 5, got %d", result)
	}

	result = StockBuySellOnce([]int{7, 6, 4, 3, 1})
	if result != 0 {
		t.Errorf("expected 0 (no profit), got %d", result)
	}
}

func TestStockBuySellMultiple(t *testing.T) {
	result := StockBuySellMultiple([]int{7, 1, 5, 3, 6, 4})
	if result != 7 {
		t.Errorf("expected 7, got %d", result)
	}

	result = StockBuySellMultiple([]int{1, 2, 3, 4, 5})
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}
}
