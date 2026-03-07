package algorithms

import (
	"math/rand"
)

// Ordered constraint for comparable types.
type Ordered interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64 |
		~uint | ~uint8 | ~uint16 | ~uint32 | ~uint64 | ~uintptr |
		~float32 | ~float64 | ~string
}

// BubbleSort sorts a slice using bubble sort. O(n²)
func BubbleSort[T Ordered](arr []T) {
	n := len(arr)
	for i := 0; i < n; i++ {
		swapped := false
		for j := 0; j < n-i-1; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j]
				swapped = true
			}
		}
		if !swapped {
			break
		}
	}
}

// SelectionSort sorts a slice using selection sort. O(n²)
func SelectionSort[T Ordered](arr []T) {
	n := len(arr)
	for i := 0; i < n-1; i++ {
		minIdx := i
		for j := i + 1; j < n; j++ {
			if arr[j] < arr[minIdx] {
				minIdx = j
			}
		}
		arr[i], arr[minIdx] = arr[minIdx], arr[i]
	}
}

// InsertionSort sorts a slice using insertion sort. O(n²)
func InsertionSort[T Ordered](arr []T) {
	for i := 1; i < len(arr); i++ {
		key := arr[i]
		j := i - 1
		for j >= 0 && arr[j] > key {
			arr[j+1] = arr[j]
			j--
		}
		arr[j+1] = key
	}
}

// MergeSort sorts a slice using merge sort. O(n log n)
func MergeSort[T Ordered](arr []T) []T {
	if len(arr) <= 1 {
		return arr
	}
	mid := len(arr) / 2
	left := MergeSort(append([]T{}, arr[:mid]...))
	right := MergeSort(append([]T{}, arr[mid:]...))
	return merge(left, right)
}

func merge[T Ordered](left, right []T) []T {
	result := make([]T, 0, len(left)+len(right))
	i, j := 0, 0
	for i < len(left) && j < len(right) {
		if left[i] <= right[j] {
			result = append(result, left[i])
			i++
		} else {
			result = append(result, right[j])
			j++
		}
	}
	result = append(result, left[i:]...)
	result = append(result, right[j:]...)
	return result
}

// QuickSort sorts a slice using randomized quicksort. O(n log n) average
func QuickSort[T Ordered](arr []T) {
	quickSortHelper(arr, 0, len(arr)-1)
}

func quickSortHelper[T Ordered](arr []T, low, high int) {
	if low < high {
		pivot := partition(arr, low, high)
		quickSortHelper(arr, low, pivot-1)
		quickSortHelper(arr, pivot+1, high)
	}
}

func partition[T Ordered](arr []T, low, high int) int {
	// Randomized pivot
	randIdx := low + rand.Intn(high-low+1)
	arr[randIdx], arr[high] = arr[high], arr[randIdx]

	pivot := arr[high]
	i := low - 1
	for j := low; j < high; j++ {
		if arr[j] <= pivot {
			i++
			arr[i], arr[j] = arr[j], arr[i]
		}
	}
	arr[i+1], arr[high] = arr[high], arr[i+1]
	return i + 1
}

// HeapSortAlgo sorts a slice using heap sort. O(n log n)
func HeapSortAlgo[T Ordered](arr []T) {
	n := len(arr)
	for i := n/2 - 1; i >= 0; i-- {
		siftDown(arr, n, i)
	}
	for i := n - 1; i > 0; i-- {
		arr[0], arr[i] = arr[i], arr[0]
		siftDown(arr, i, 0)
	}
}

func siftDown[T Ordered](arr []T, n, i int) {
	for {
		largest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && arr[left] > arr[largest] {
			largest = left
		}
		if right < n && arr[right] > arr[largest] {
			largest = right
		}
		if largest == i {
			break
		}
		arr[i], arr[largest] = arr[largest], arr[i]
		i = largest
	}
}

// ShellSort sorts using shell sort. O(n^1.5) with Knuth gaps
func ShellSort[T Ordered](arr []T) {
	n := len(arr)
	gap := 1
	for gap < n/3 {
		gap = gap*3 + 1
	}
	for gap > 0 {
		for i := gap; i < n; i++ {
			key := arr[i]
			j := i
			for j >= gap && arr[j-gap] > key {
				arr[j] = arr[j-gap]
				j -= gap
			}
			arr[j] = key
		}
		gap /= 3
	}
}

// CountingSort sorts integers in O(n + k) where k is the range.
func CountingSort(arr []int) {
	if len(arr) <= 1 {
		return
	}
	min, max := arr[0], arr[0]
	for _, v := range arr {
		if v < min {
			min = v
		}
		if v > max {
			max = v
		}
	}
	offset := min
	count := make([]int, max-min+1)
	for _, v := range arr {
		count[v-offset]++
	}
	idx := 0
	for i, c := range count {
		for j := 0; j < c; j++ {
			arr[idx] = i + offset
			idx++
		}
	}
}

// RadixSort sorts non-negative integers in O(d*(n+10)).
func RadixSort(arr []int) {
	if len(arr) <= 1 {
		return
	}
	max := arr[0]
	for _, v := range arr {
		if v > max {
			max = v
		}
	}

	for exp := 1; max/exp > 0; exp *= 10 {
		countingSortByDigit(arr, exp)
	}
}

func countingSortByDigit(arr []int, exp int) {
	n := len(arr)
	output := make([]int, n)
	count := make([]int, 10)

	for _, v := range arr {
		count[(v/exp)%10]++
	}
	for i := 1; i < 10; i++ {
		count[i] += count[i-1]
	}
	for i := n - 1; i >= 0; i-- {
		digit := (arr[i] / exp) % 10
		count[digit]--
		output[count[digit]] = arr[i]
	}
	copy(arr, output)
}

// BucketSort sorts floats in [0, 1) range. O(n) average.
func BucketSort(arr []float64) {
	n := len(arr)
	if n <= 1 {
		return
	}

	buckets := make([][]float64, n)
	for i := range buckets {
		buckets[i] = make([]float64, 0)
	}

	for _, v := range arr {
		idx := int(v * float64(n))
		if idx >= n {
			idx = n - 1
		}
		buckets[idx] = append(buckets[idx], v)
	}

	// Sort each bucket with insertion sort
	for _, bucket := range buckets {
		for i := 1; i < len(bucket); i++ {
			key := bucket[i]
			j := i - 1
			for j >= 0 && bucket[j] > key {
				bucket[j+1] = bucket[j]
				j--
			}
			bucket[j+1] = key
		}
	}

	idx := 0
	for _, bucket := range buckets {
		for _, v := range bucket {
			arr[idx] = v
			idx++
		}
	}
}

// QuickSelect finds the kth smallest element. O(n) average.
func QuickSelect[T Ordered](arr []T, k int) T {
	return quickSelectHelper(arr, 0, len(arr)-1, k-1)
}

func quickSelectHelper[T Ordered](arr []T, low, high, k int) T {
	if low == high {
		return arr[low]
	}
	pivot := partition(arr, low, high)
	if k == pivot {
		return arr[pivot]
	} else if k < pivot {
		return quickSelectHelper(arr, low, pivot-1, k)
	}
	return quickSelectHelper(arr, pivot+1, high, k)
}

// TimSort sorts a slice using Tim sort (hybrid merge + insertion sort).
func TimSort[T Ordered](arr []T) {
	n := len(arr)
	if n <= 1 {
		return
	}

	const minRun = 32

	// Insertion sort for small runs
	for start := 0; start < n; start += minRun {
		end := start + minRun - 1
		if end >= n {
			end = n - 1
		}
		timInsertionSort(arr, start, end)
	}

	// Merge runs
	for size := minRun; size < n; size *= 2 {
		for left := 0; left < n; left += 2 * size {
			mid := left + size - 1
			right := left + 2*size - 1
			if mid >= n-1 {
				continue
			}
			if right >= n {
				right = n - 1
			}
			timMerge(arr, left, mid, right)
		}
	}
}

func timInsertionSort[T Ordered](arr []T, left, right int) {
	for i := left + 1; i <= right; i++ {
		key := arr[i]
		j := i - 1
		for j >= left && arr[j] > key {
			arr[j+1] = arr[j]
			j--
		}
		arr[j+1] = key
	}
}

func timMerge[T Ordered](arr []T, left, mid, right int) {
	leftArr := make([]T, mid-left+1)
	rightArr := make([]T, right-mid)
	copy(leftArr, arr[left:mid+1])
	copy(rightArr, arr[mid+1:right+1])

	i, j, k := 0, 0, left
	for i < len(leftArr) && j < len(rightArr) {
		if leftArr[i] <= rightArr[j] {
			arr[k] = leftArr[i]
			i++
		} else {
			arr[k] = rightArr[j]
			j++
		}
		k++
	}
	for i < len(leftArr) {
		arr[k] = leftArr[i]
		i++
		k++
	}
	for j < len(rightArr) {
		arr[k] = rightArr[j]
		j++
		k++
	}
}

// CombSort sorts using comb sort with shrink factor 1.3.
func CombSort[T Ordered](arr []T) {
	n := len(arr)
	gap := n
	shrink := 1.3
	sorted := false

	for !sorted {
		gap = int(float64(gap) / shrink)
		if gap <= 1 {
			gap = 1
			sorted = true
		}
		for i := 0; i+gap < n; i++ {
			if arr[i] > arr[i+gap] {
				arr[i], arr[i+gap] = arr[i+gap], arr[i]
				sorted = false
			}
		}
	}
}

// GnomeSort sorts using gnome sort (similar to insertion sort).
func GnomeSort[T Ordered](arr []T) {
	n := len(arr)
	pos := 0
	for pos < n {
		if pos == 0 || arr[pos] >= arr[pos-1] {
			pos++
		} else {
			arr[pos], arr[pos-1] = arr[pos-1], arr[pos]
			pos--
		}
	}
}

// PancakeSort sorts using pancake flipping.
func PancakeSort[T Ordered](arr []T) {
	n := len(arr)
	for size := n; size > 1; size-- {
		// Find max in arr[0..size-1]
		maxIdx := 0
		for i := 1; i < size; i++ {
			if arr[i] > arr[maxIdx] {
				maxIdx = i
			}
		}
		if maxIdx != size-1 {
			// Flip to bring max to front
			pancakeFlip(arr, maxIdx+1)
			// Flip to put max at its position
			pancakeFlip(arr, size)
		}
	}
}

func pancakeFlip[T Ordered](arr []T, k int) {
	for i, j := 0, k-1; i < j; i, j = i+1, j-1 {
		arr[i], arr[j] = arr[j], arr[i]
	}
}

// CycleSort sorts with minimum number of writes. O(n²) time, O(1) space.
func CycleSort[T Ordered](arr []T) int {
	writes := 0
	n := len(arr)

	for cycleStart := 0; cycleStart < n-1; cycleStart++ {
		item := arr[cycleStart]
		pos := cycleStart
		for i := cycleStart + 1; i < n; i++ {
			if arr[i] < item {
				pos++
			}
		}
		if pos == cycleStart {
			continue
		}
		for item == arr[pos] {
			pos++
		}
		arr[pos], item = item, arr[pos]
		writes++

		for pos != cycleStart {
			pos = cycleStart
			for i := cycleStart + 1; i < n; i++ {
				if arr[i] < item {
					pos++
				}
			}
			for item == arr[pos] {
				pos++
			}
			arr[pos], item = item, arr[pos]
			writes++
		}
	}
	return writes
}

// MergeSortIterative sorts using iterative (bottom-up) merge sort.
func MergeSortIterative[T Ordered](arr []T) {
	n := len(arr)
	if n <= 1 {
		return
	}

	for width := 1; width < n; width *= 2 {
		for i := 0; i < n; i += 2 * width {
			mid := i + width - 1
			right := i + 2*width - 1
			if mid >= n-1 {
				continue
			}
			if right >= n {
				right = n - 1
			}
			timMerge(arr, i, mid, right)
		}
	}
}

// DutchNationalFlag sorts array of 0s, 1s, and 2s in-place. O(n)
func DutchNationalFlag(arr []int) {
	low, mid, high := 0, 0, len(arr)-1
	for mid <= high {
		switch arr[mid] {
		case 0:
			arr[low], arr[mid] = arr[mid], arr[low]
			low++
			mid++
		case 1:
			mid++
		case 2:
			arr[mid], arr[high] = arr[high], arr[mid]
			high--
		}
	}
}

// IsStableSorted checks if a slice is sorted considering stability.
func IsStableSorted[T any](arr []T, less func(T, T) bool) bool {
	for i := 1; i < len(arr); i++ {
		if less(arr[i], arr[i-1]) {
			return false
		}
	}
	return true
}

// IsSorted checks if a slice is sorted.
func IsSorted[T Ordered](arr []T) bool {
	for i := 1; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			return false
		}
	}
	return true
}

// SortWithComparator sorts using a custom comparator.
func SortWithComparator[T any](arr []T, less func(T, T) bool) {
	sortWithCompHelper(arr, less, 0, len(arr)-1)
}

func sortWithCompHelper[T any](arr []T, less func(T, T) bool, low, high int) {
	if low < high {
		pivot := partitionComp(arr, less, low, high)
		sortWithCompHelper(arr, less, low, pivot-1)
		sortWithCompHelper(arr, less, pivot+1, high)
	}
}

func partitionComp[T any](arr []T, less func(T, T) bool, low, high int) int {
	pivot := arr[high]
	i := low - 1
	for j := low; j < high; j++ {
		if less(arr[j], pivot) || (!less(pivot, arr[j]) && !less(arr[j], pivot)) {
			i++
			arr[i], arr[j] = arr[j], arr[i]
		}
	}
	arr[i+1], arr[high] = arr[high], arr[i+1]
	return i + 1
}
