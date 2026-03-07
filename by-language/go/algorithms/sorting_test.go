package algorithms

import (
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

	arr2 := []int{-3, -1, 0, 2, 5, -2}
	CountingSort(arr2)
	verifySorted(t, "CountingSort negative", arr2)
}

func TestRadixSort(t *testing.T) {
	arr := []int{170, 45, 75, 90, 802, 24, 2, 66}
	RadixSort(arr)
	verifySorted(t, "RadixSort", arr)
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
