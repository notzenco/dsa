package algorithms

import (
	"math"
	"testing"
)

func verifySorted(t *testing.T, name string, arr []int) {
	t.Helper()
	for i := 1; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			t.Errorf("%s: not sorted at index %d: %v", name, i, arr)
			return
		}
	}
}

var testCases = [][]int{
	{},
	{1},
	{5, 3, 8, 1, 9, 2, 7, 4, 6},
	{1, 2, 3, 4, 5},
	{5, 4, 3, 2, 1},
	{3, 3, 3, 1, 1, 2, 2},
}

func copySlice(arr []int) []int {
	c := make([]int, len(arr))
	copy(c, arr)
	return c
}

func TestBubbleSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		BubbleSort(arr)
		verifySorted(t, "BubbleSort", arr)
	}
}

func TestSelectionSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		SelectionSort(arr)
		verifySorted(t, "SelectionSort", arr)
	}
}

func TestInsertionSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		InsertionSort(arr)
		verifySorted(t, "InsertionSort", arr)
	}
}

func TestMergeSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		result := MergeSort(arr)
		verifySorted(t, "MergeSort", result)
		if len(result) != len(arr) {
			t.Errorf("MergeSort changed length")
		}
	}
}

func TestQuickSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		QuickSort(arr)
		verifySorted(t, "QuickSort", arr)
	}
}

func TestHeapSortAlgo(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		HeapSortAlgo(arr)
		verifySorted(t, "HeapSortAlgo", arr)
	}
}

func TestShellSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		ShellSort(arr)
		verifySorted(t, "ShellSort", arr)
	}
}

func TestCountingSort(t *testing.T) {
	arr := []int{4, 2, 2, 8, 3, 3, 1}
	CountingSort(arr)
	verifySorted(t, "CountingSort", arr)
	want := []int{1, 2, 2, 3, 3, 4, 8}
	for i := range want {
		if arr[i] != want[i] {
			t.Fatalf("CountingSort = %v, want %v", arr, want)
		}
	}

	arr2 := []int{-3, -1, 0, 2, 5, -2}
	CountingSort(arr2)
	verifySorted(t, "CountingSort negative", arr2)

	// Narrow ranges around the int extremes still use counting sort.
	nearMin := []int{math.MinInt + 2, math.MinInt, math.MinInt + 2, math.MinInt + 1}
	CountingSort(nearMin)
	wantNearMin := []int{math.MinInt, math.MinInt + 1, math.MinInt + 2, math.MinInt + 2}
	for i := range wantNearMin {
		if nearMin[i] != wantNearMin[i] {
			t.Fatalf("CountingSort near MinInt = %v, want %v", nearMin, wantNearMin)
		}
	}
	nearMax := []int{math.MaxInt, math.MaxInt - 2, math.MaxInt - 1}
	CountingSort(nearMax)
	wantNearMax := []int{math.MaxInt - 2, math.MaxInt - 1, math.MaxInt}
	for i := range wantNearMax {
		if nearMax[i] != wantNearMax[i] {
			t.Fatalf("CountingSort near MaxInt = %v, want %v", nearMax, wantNearMax)
		}
	}

	// The full int domain and a sparse wide range must not overflow or allocate.
	extremes := []int{math.MaxInt, -1, math.MinInt, 0, 1}
	CountingSort(extremes)
	wantExtremes := []int{math.MinInt, -1, 0, 1, math.MaxInt}
	for i := range wantExtremes {
		if extremes[i] != wantExtremes[i] {
			t.Fatalf("CountingSort extremes = %v, want %v", extremes, wantExtremes)
		}
	}

	sparse := []int{1 << 40, 0, 1 << 40}
	CountingSort(sparse)
	wantSparse := []int{0, 1 << 40, 1 << 40}
	for i := range wantSparse {
		if sparse[i] != wantSparse[i] {
			t.Fatalf("CountingSort sparse = %v, want %v", sparse, wantSparse)
		}
	}

	single := []int{math.MinInt}
	CountingSort(single)
	if single[0] != math.MinInt {
		t.Fatalf("CountingSort single = %d", single[0])
	}
	var empty []int
	CountingSort(empty)
}

func TestRadixSort(t *testing.T) {
	arr := []int{170, 45, 75, 90, 802, 24, 2, 66}
	RadixSort(arr)
	verifySorted(t, "RadixSort", arr)

	negative := []int{3, -12, 7, -5, 1, 0, -12}
	RadixSort(negative)
	verifySorted(t, "RadixSort negative", negative)

	extremes := []int{math.MaxInt, -1, math.MinInt, 0, 1}
	RadixSort(extremes)
	want := []int{math.MinInt, -1, 0, 1, math.MaxInt}
	for i := range want {
		if extremes[i] != want[i] {
			t.Fatalf("RadixSort extremes = %v, want %v", extremes, want)
		}
	}
}

func TestBucketSort(t *testing.T) {
	arr := []float64{0.42, 0.32, 0.33, 0.52, 0.37, 0.47, 0.51}
	BucketSort(arr)
	for i := 1; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			t.Errorf("BucketSort: not sorted at index %d: %v", i, arr)
		}
	}
}

func TestQuickSelect(t *testing.T) {
	arr := []int{3, 1, 4, 1, 5, 9, 2, 6}
	// 1st smallest = 1
	result := QuickSelect(copySlice(arr), 1)
	if result != 1 {
		t.Errorf("expected 1st smallest = 1, got %d", result)
	}

	// 3rd smallest = 2
	result = QuickSelect(copySlice(arr), 3)
	if result != 2 {
		t.Errorf("expected 3rd smallest = 2, got %d", result)
	}
}

func TestIsSorted(t *testing.T) {
	if !IsSorted([]int{1, 2, 3, 4, 5}) {
		t.Error("should be sorted")
	}
	if IsSorted([]int{1, 3, 2}) {
		t.Error("should not be sorted")
	}
	if !IsSorted([]int{}) {
		t.Error("empty should be sorted")
	}
}

func TestSortWithComparator(t *testing.T) {
	arr := []int{3, 1, 4, 1, 5}
	SortWithComparator(arr, func(a, b int) bool { return a > b })
	for i := 1; i < len(arr); i++ {
		if arr[i] > arr[i-1] {
			t.Errorf("descending sort failed at index %d: %v", i, arr)
		}
	}
}

func TestTimSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		TimSort(arr)
		verifySorted(t, "TimSort", arr)
	}
}

func TestTimSortLarge(t *testing.T) {
	arr := make([]int, 200)
	for i := range arr {
		arr[i] = 200 - i
	}
	TimSort(arr)
	verifySorted(t, "TimSort large", arr)
}

func TestCombSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		CombSort(arr)
		verifySorted(t, "CombSort", arr)
	}
}

func TestGnomeSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		GnomeSort(arr)
		verifySorted(t, "GnomeSort", arr)
	}
}

func TestPancakeSort(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		PancakeSort(arr)
		verifySorted(t, "PancakeSort", arr)
	}
}

func TestCycleSort(t *testing.T) {
	arr := []int{5, 3, 8, 1, 9, 2, 7, 4, 6}
	writes := CycleSort(arr)
	verifySorted(t, "CycleSort", arr)
	if writes < 0 {
		t.Errorf("writes should be >= 0, got %d", writes)
	}
}

func TestCycleSortAlreadySorted(t *testing.T) {
	arr := []int{1, 2, 3, 4, 5}
	writes := CycleSort(arr)
	verifySorted(t, "CycleSort already sorted", arr)
	if writes != 0 {
		t.Errorf("expected 0 writes for sorted array, got %d", writes)
	}
}

func TestMergeSortIterative(t *testing.T) {
	for _, tc := range testCases {
		arr := copySlice(tc)
		MergeSortIterative(arr)
		verifySorted(t, "MergeSortIterative", arr)
	}
}

func TestDutchNationalFlag(t *testing.T) {
	arr := []int{2, 0, 2, 1, 1, 0}
	DutchNationalFlag(arr)
	expected := []int{0, 0, 1, 1, 2, 2}
	for i, v := range expected {
		if arr[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, arr[i])
		}
	}
}

func TestDutchNationalFlagAllSame(t *testing.T) {
	arr := []int{1, 1, 1}
	DutchNationalFlag(arr)
	for _, v := range arr {
		if v != 1 {
			t.Errorf("expected all 1s, got %v", arr)
			break
		}
	}
}

func TestIsStableSorted(t *testing.T) {
	arr := []int{1, 2, 2, 3, 3, 4}
	if !IsStableSorted(arr, func(a, b int) bool { return a < b }) {
		t.Error("should be stable sorted")
	}

	arr2 := []int{3, 1, 2}
	if IsStableSorted(arr2, func(a, b int) bool { return a < b }) {
		t.Error("should not be stable sorted")
	}
}
