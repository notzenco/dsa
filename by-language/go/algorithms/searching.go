package algorithms

import "math"

// LinearSearch returns the index of target or -1.
func LinearSearch[T comparable](arr []T, target T) int {
	for i, v := range arr {
		if v == target {
			return i
		}
	}
	return -1
}

// BinarySearch returns the index of target in a sorted slice, or -1.
func BinarySearch[T Ordered](arr []T, target T) int {
	low, high := 0, len(arr)-1
	for low <= high {
		mid := low + (high-low)/2
		if arr[mid] == target {
			return mid
		} else if arr[mid] < target {
			low = mid + 1
		} else {
			high = mid - 1
		}
	}
	return -1
}

// LowerBound returns the first index where arr[i] >= target.
func LowerBound[T Ordered](arr []T, target T) int {
	low, high := 0, len(arr)
	for low < high {
		mid := low + (high-low)/2
		if arr[mid] < target {
			low = mid + 1
		} else {
			high = mid
		}
	}
	return low
}

// UpperBound returns the first index where arr[i] > target.
func UpperBound[T Ordered](arr []T, target T) int {
	low, high := 0, len(arr)
	for low < high {
		mid := low + (high-low)/2
		if arr[mid] <= target {
			low = mid + 1
		} else {
			high = mid
		}
	}
	return low
}

// FindFirstOccurrence returns the first index of target in a sorted slice.
func FindFirstOccurrence[T Ordered](arr []T, target T) int {
	idx := LowerBound(arr, target)
	if idx < len(arr) && arr[idx] == target {
		return idx
	}
	return -1
}

// FindLastOccurrence returns the last index of target in a sorted slice.
func FindLastOccurrence[T Ordered](arr []T, target T) int {
	idx := UpperBound(arr, target) - 1
	if idx >= 0 && arr[idx] == target {
		return idx
	}
	return -1
}

// JumpSearch searches a sorted slice using jump search. O(√n)
func JumpSearch[T Ordered](arr []T, target T) int {
	n := len(arr)
	if n == 0 {
		return -1
	}
	step := int(math.Sqrt(float64(n)))
	prev := 0

	for prev < n && arr[min(prev+step, n)-1] < target {
		prev += step
	}

	for i := prev; i < min(prev+step, n); i++ {
		if arr[i] == target {
			return i
		}
	}
	return -1
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// InterpolationSearch for uniformly distributed sorted slices.
func InterpolationSearch(arr []int, target int) int {
	low, high := 0, len(arr)-1

	for low <= high && target >= arr[low] && target <= arr[high] {
		if low == high {
			if arr[low] == target {
				return low
			}
			return -1
		}
		pos := low + int(float64(high-low)*float64(target-arr[low])/float64(arr[high]-arr[low]))
		if pos < low || pos > high {
			break
		}
		if arr[pos] == target {
			return pos
		} else if arr[pos] < target {
			low = pos + 1
		} else {
			high = pos - 1
		}
	}
	return -1
}

// ExponentialSearch for unbounded or sorted slices.
func ExponentialSearch[T Ordered](arr []T, target T) int {
	n := len(arr)
	if n == 0 {
		return -1
	}
	if arr[0] == target {
		return 0
	}

	bound := 1
	for bound < n && arr[bound] <= target {
		bound *= 2
	}

	low := bound / 2
	high := min(bound, n-1)
	for low <= high {
		mid := low + (high-low)/2
		if arr[mid] == target {
			return mid
		} else if arr[mid] < target {
			low = mid + 1
		} else {
			high = mid - 1
		}
	}
	return -1
}

// TernarySearch finds the maximum of a unimodal function.
func TernarySearch(f func(float64) float64, left, right float64, iterations int) float64 {
	for i := 0; i < iterations; i++ {
		m1 := left + (right-left)/3
		m2 := right - (right-left)/3
		if f(m1) < f(m2) {
			left = m1
		} else {
			right = m2
		}
	}
	return (left + right) / 2
}

// SearchRotatedArray searches in a rotated sorted array.
func SearchRotatedArray[T Ordered](arr []T, target T) int {
	low, high := 0, len(arr)-1
	for low <= high {
		mid := low + (high-low)/2
		if arr[mid] == target {
			return mid
		}
		if arr[low] <= arr[mid] {
			if target >= arr[low] && target < arr[mid] {
				high = mid - 1
			} else {
				low = mid + 1
			}
		} else {
			if target > arr[mid] && target <= arr[high] {
				low = mid + 1
			} else {
				high = mid - 1
			}
		}
	}
	return -1
}

// FindRotationPoint finds the index of the minimum in a rotated sorted array.
func FindRotationPoint[T Ordered](arr []T) int {
	if len(arr) == 0 {
		return -1
	}
	low, high := 0, len(arr)-1
	for low < high {
		mid := low + (high-low)/2
		if arr[mid] > arr[high] {
			low = mid + 1
		} else {
			high = mid
		}
	}
	return low
}

// FindPeakElement finds a local maximum.
func FindPeakElement[T Ordered](arr []T) int {
	if len(arr) == 0 {
		return -1
	}
	low, high := 0, len(arr)-1
	for low < high {
		mid := low + (high-low)/2
		if arr[mid] < arr[mid+1] {
			low = mid + 1
		} else {
			high = mid
		}
	}
	return low
}

// SqrtInt returns the integer square root.
func SqrtInt(n int) int {
	if n < 0 {
		return -1
	}
	if n < 2 {
		return n
	}
	low, high := 1, n/2
	result := 1
	for low <= high {
		mid := low + (high-low)/2
		if mid <= n/mid {
			result = mid
			low = mid + 1
		} else {
			high = mid - 1
		}
	}
	return result
}

// FirstBadVersion finds the first bad version using a check function.
func FirstBadVersion(n int, isBad func(int) bool) int {
	low, high := 1, n
	for low < high {
		mid := low + (high-low)/2
		if isBad(mid) {
			high = mid
		} else {
			low = mid + 1
		}
	}
	return low
}

// TwoSumSorted finds two numbers in a sorted array that sum to target.
func TwoSumSorted(arr []int, target int) (int, int, bool) {
	left, right := 0, len(arr)-1
	for left < right {
		sum := arr[left] + arr[right]
		if sum == target {
			return left, right, true
		} else if sum < target {
			left++
		} else {
			right--
		}
	}
	return 0, 0, false
}

// ThreeSum finds all unique triplets that sum to target.
func ThreeSum(arr []int, target int) [][3]int {
	sorted := make([]int, len(arr))
	copy(sorted, arr)
	InsertionSort(sorted)

	var result [][3]int
	n := len(sorted)

	for i := 0; i < n-2; i++ {
		if i > 0 && sorted[i] == sorted[i-1] {
			continue
		}
		left, right := i+1, n-1
		for left < right {
			sum := sorted[i] + sorted[left] + sorted[right]
			if sum == target {
				result = append(result, [3]int{sorted[i], sorted[left], sorted[right]})
				for left < right && sorted[left] == sorted[left+1] {
					left++
				}
				for left < right && sorted[right] == sorted[right-1] {
					right--
				}
				left++
				right--
			} else if sum < target {
				left++
			} else {
				right--
			}
		}
	}
	return result
}

// MaxSlidingWindow returns the max of each window of size k.
func MaxSlidingWindow(arr []int, k int) []int {
	if len(arr) == 0 || k <= 0 {
		return nil
	}
	if k > len(arr) {
		k = len(arr)
	}

	result := make([]int, 0, len(arr)-k+1)
	var deque []int // indices

	for i := 0; i < len(arr); i++ {
		for len(deque) > 0 && deque[0] <= i-k {
			deque = deque[1:]
		}
		for len(deque) > 0 && arr[deque[len(deque)-1]] <= arr[i] {
			deque = deque[:len(deque)-1]
		}
		deque = append(deque, i)
		if i >= k-1 {
			result = append(result, arr[deque[0]])
		}
	}
	return result
}

// MinSubarraySum finds the minimum length subarray with sum >= target.
func MinSubarraySum(arr []int, target int) int {
	n := len(arr)
	minLen := n + 1
	sum := 0
	left := 0

	for right := 0; right < n; right++ {
		sum += arr[right]
		for sum >= target {
			length := right - left + 1
			if length < minLen {
				minLen = length
			}
			sum -= arr[left]
			left++
		}
	}
	if minLen > n {
		return 0
	}
	return minLen
}
