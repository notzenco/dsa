package algorithms

import (
	"math"
	"testing"
)

func TestLinearSearch(t *testing.T) {
	arr := []int{5, 3, 8, 1, 9}
	if LinearSearch(arr, 8) != 2 {
		t.Error("expected index 2")
	}
	if LinearSearch(arr, 99) != -1 {
		t.Error("expected -1")
	}
}

func TestBinarySearch(t *testing.T) {
	arr := []int{1, 3, 5, 7, 9, 11}
	if BinarySearch(arr, 7) != 3 {
		t.Error("expected index 3")
	}
	if BinarySearch(arr, 4) != -1 {
		t.Error("expected -1")
	}
}

func TestLowerUpperBound(t *testing.T) {
	arr := []int{1, 2, 2, 2, 3, 4}

	lb := LowerBound(arr, 2)
	if lb != 1 {
		t.Errorf("lower bound: expected 1, got %d", lb)
	}

	ub := UpperBound(arr, 2)
	if ub != 4 {
		t.Errorf("upper bound: expected 4, got %d", ub)
	}
}

func TestFindFirstLastOccurrence(t *testing.T) {
	arr := []int{1, 2, 2, 2, 3, 4}

	first := FindFirstOccurrence(arr, 2)
	if first != 1 {
		t.Errorf("expected first=1, got %d", first)
	}

	last := FindLastOccurrence(arr, 2)
	if last != 3 {
		t.Errorf("expected last=3, got %d", last)
	}

	if FindFirstOccurrence(arr, 99) != -1 {
		t.Error("expected -1 for missing")
	}
}

func TestJumpSearch(t *testing.T) {
	arr := []int{1, 3, 5, 7, 9, 11, 13, 15}
	if JumpSearch(arr, 7) != 3 {
		t.Error("expected index 3")
	}
	if JumpSearch(arr, 4) != -1 {
		t.Error("expected -1")
	}
}

func TestInterpolationSearch(t *testing.T) {
	arr := []int{10, 20, 30, 40, 50, 60, 70}
	if InterpolationSearch(arr, 40) != 3 {
		t.Error("expected index 3")
	}
	if InterpolationSearch(arr, 35) != -1 {
		t.Error("expected -1")
	}
}

func TestInterpolationSearchWideValues(t *testing.T) {
	arr := []int{-1 << 62, 0, 1 << 62}
	if got := InterpolationSearch(arr, 0); got != 1 {
		t.Fatalf("InterpolationSearch wide = %d, want 1", got)
	}
	if got := InterpolationSearch(arr, 1); got != -1 {
		t.Fatalf("InterpolationSearch missing wide = %d, want -1", got)
	}
	if got := InterpolationSearch([]int{math.MinInt, math.MaxInt}, math.MaxInt); got != 1 {
		t.Fatalf("InterpolationSearch extremes = %d, want 1", got)
	}
}

func TestExponentialSearch(t *testing.T) {
	arr := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	if ExponentialSearch(arr, 6) != 5 {
		t.Error("expected index 5")
	}
	if ExponentialSearch(arr, 11) != -1 {
		t.Error("expected -1")
	}
}

func TestTernarySearch(t *testing.T) {
	f := func(x float64) float64 { return -(x-3)*(x-3) + 9 }
	result := TernarySearch(f, 0, 6, 100)
	if result < 2.99 || result > 3.01 {
		t.Errorf("expected ~3.0, got %f", result)
	}
}

func TestSearchRotatedArray(t *testing.T) {
	arr := []int{4, 5, 6, 7, 0, 1, 2}
	if SearchRotatedArray(arr, 0) != 4 {
		t.Error("expected index 4")
	}
	if SearchRotatedArray(arr, 3) != -1 {
		t.Error("expected -1")
	}
}

func TestFindRotationPoint(t *testing.T) {
	arr := []int{4, 5, 6, 7, 0, 1, 2}
	if FindRotationPoint(arr) != 4 {
		t.Error("expected index 4")
	}

	arr2 := []int{1, 2, 3, 4, 5}
	if FindRotationPoint(arr2) != 0 {
		t.Error("expected 0 for non-rotated")
	}
}

func TestFindPeakElement(t *testing.T) {
	arr := []int{1, 3, 5, 4, 2}
	peak := FindPeakElement(arr)
	if arr[peak] != 5 {
		t.Errorf("expected peak value 5, got %d", arr[peak])
	}
}

func TestSqrtInt(t *testing.T) {
	if SqrtInt(16) != 4 {
		t.Error("sqrt(16) should be 4")
	}
	if SqrtInt(15) != 3 {
		t.Error("sqrt(15) should be 3")
	}
	if SqrtInt(0) != 0 {
		t.Error("sqrt(0) should be 0")
	}
	if SqrtInt(1) != 1 {
		t.Error("sqrt(1) should be 1")
	}
}

func TestFirstBadVersion(t *testing.T) {
	isBad := func(v int) bool { return v >= 5 }
	result := FirstBadVersion(10, isBad)
	if result != 5 {
		t.Errorf("expected 5, got %d", result)
	}
}

func TestTwoSumSorted(t *testing.T) {
	arr := []int{2, 7, 11, 15}
	i, j, ok := TwoSumSorted(arr, 9)
	if !ok || arr[i]+arr[j] != 9 {
		t.Errorf("expected sum 9, got indices %d,%d", i, j)
	}

	_, _, ok = TwoSumSorted(arr, 100)
	if ok {
		t.Error("should not find pair")
	}
}

func TestThreeSum(t *testing.T) {
	arr := []int{-1, 0, 1, 2, -1, -4}
	result := ThreeSum(arr, 0)

	if len(result) != 2 {
		t.Errorf("expected 2 triplets, got %d", len(result))
	}
}

func TestMaxSlidingWindow(t *testing.T) {
	arr := []int{1, 3, -1, -3, 5, 3, 6, 7}
	result := MaxSlidingWindow(arr, 3)
	expected := []int{3, 3, 5, 5, 6, 7}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestMinSubarraySum(t *testing.T) {
	arr := []int{2, 3, 1, 2, 4, 3}
	result := MinSubarraySum(arr, 7)
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}

	result = MinSubarraySum([]int{1, 1, 1}, 100)
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}

func TestLongestSubstringWithoutRepeating(t *testing.T) {
	if LongestSubstringWithoutRepeating("abcabcbb") != 3 {
		t.Errorf("expected 3")
	}
	if LongestSubstringWithoutRepeating("bbbbb") != 1 {
		t.Errorf("expected 1")
	}
	if LongestSubstringWithoutRepeating("pwwkew") != 3 {
		t.Errorf("expected 3")
	}
	if LongestSubstringWithoutRepeating("") != 0 {
		t.Errorf("expected 0")
	}
}

func TestMinWindowSubstring(t *testing.T) {
	result := MinWindowSubstring("ADOBECODEBANC", "ABC")
	if result != "BANC" {
		t.Errorf("expected 'BANC', got %q", result)
	}

	result = MinWindowSubstring("a", "a")
	if result != "a" {
		t.Errorf("expected 'a', got %q", result)
	}

	result = MinWindowSubstring("a", "aa")
	if result != "" {
		t.Errorf("expected empty, got %q", result)
	}
}

func TestContainerWithMostWater(t *testing.T) {
	result := ContainerWithMostWater([]int{1, 8, 6, 2, 5, 4, 8, 3, 7})
	if result != 49 {
		t.Errorf("expected 49, got %d", result)
	}

	result = ContainerWithMostWater([]int{1, 1})
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}
}

func TestFindAllAnagrams(t *testing.T) {
	result := FindAllAnagrams("cbaebabacd", "abc")
	if len(result) != 2 || result[0] != 0 || result[1] != 6 {
		t.Errorf("expected [0,6], got %v", result)
	}

	result = FindAllAnagrams("abab", "ab")
	if len(result) != 3 {
		t.Errorf("expected 3 results, got %d", len(result))
	}
}

func TestTrappingRainWater(t *testing.T) {
	result := TrappingRainWater([]int{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1})
	if result != 6 {
		t.Errorf("expected 6, got %d", result)
	}

	result = TrappingRainWater([]int{4, 2, 0, 3, 2, 5})
	if result != 9 {
		t.Errorf("expected 9, got %d", result)
	}
}

func TestLongestSubarrayWithSumK(t *testing.T) {
	result := LongestSubarrayWithSumK([]int{1, -1, 5, -2, 3}, 3)
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}

	result = LongestSubarrayWithSumK([]int{-2, -1, 2, 1}, 1)
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}
}

func TestSubarrayProductLessThanK(t *testing.T) {
	result := SubarrayProductLessThanK([]int{10, 5, 2, 6}, 100)
	if result != 8 {
		t.Errorf("expected 8, got %d", result)
	}

	result = SubarrayProductLessThanK([]int{1, 2, 3}, 0)
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}

func TestRemoveDuplicatesSorted(t *testing.T) {
	arr := []int{1, 1, 2, 2, 3}
	n := RemoveDuplicatesSorted(arr)
	if n != 3 {
		t.Errorf("expected 3 unique, got %d", n)
	}
	if arr[0] != 1 || arr[1] != 2 || arr[2] != 3 {
		t.Errorf("unexpected values: %v", arr[:n])
	}
}

func TestMoveZeroes(t *testing.T) {
	arr := []int{0, 1, 0, 3, 12}
	MoveZeroes(arr)
	expected := []int{1, 3, 12, 0, 0}
	for i, v := range expected {
		if arr[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, arr[i])
		}
	}
}

func TestFourSum(t *testing.T) {
	result := FourSum([]int{1, 0, -1, 0, -2, 2}, 0)
	if len(result) != 3 {
		t.Errorf("expected 3 quadruplets, got %d", len(result))
	}

	result = FourSum([]int{2, 2, 2, 2, 2}, 8)
	if len(result) != 1 {
		t.Errorf("expected 1 quadruplet, got %d", len(result))
	}
}
