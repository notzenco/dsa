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

// LongestSubstringWithoutRepeating returns the length of the longest substring
// without repeating characters.
func LongestSubstringWithoutRepeating(s string) int {
	charIndex := make(map[byte]int)
	maxLen := 0
	left := 0

	for right := 0; right < len(s); right++ {
		if idx, ok := charIndex[s[right]]; ok && idx >= left {
			left = idx + 1
		}
		charIndex[s[right]] = right
		length := right - left + 1
		if length > maxLen {
			maxLen = length
		}
	}
	return maxLen
}

// MinWindowSubstring returns the minimum window in s that contains all characters in t.
func MinWindowSubstring(s, t string) string {
	if len(s) == 0 || len(t) == 0 || len(s) < len(t) {
		return ""
	}

	need := make(map[byte]int)
	for i := 0; i < len(t); i++ {
		need[t[i]]++
	}

	window := make(map[byte]int)
	have, required := 0, len(need)
	left := 0
	bestLeft, bestLen := 0, len(s)+1

	for right := 0; right < len(s); right++ {
		c := s[right]
		window[c]++
		if need[c] > 0 && window[c] == need[c] {
			have++
		}

		for have == required {
			length := right - left + 1
			if length < bestLen {
				bestLen = length
				bestLeft = left
			}
			lc := s[left]
			window[lc]--
			if need[lc] > 0 && window[lc] < need[lc] {
				have--
			}
			left++
		}
	}

	if bestLen > len(s) {
		return ""
	}
	return s[bestLeft : bestLeft+bestLen]
}

// ContainerWithMostWater finds the maximum area of water that can be contained.
func ContainerWithMostWater(heights []int) int {
	maxArea := 0
	left, right := 0, len(heights)-1

	for left < right {
		h := heights[left]
		if heights[right] < h {
			h = heights[right]
		}
		area := h * (right - left)
		if area > maxArea {
			maxArea = area
		}
		if heights[left] < heights[right] {
			left++
		} else {
			right--
		}
	}
	return maxArea
}

// FindAllAnagrams returns starting indices of all anagrams of pattern in text.
func FindAllAnagrams(text, pattern string) []int {
	if len(text) < len(pattern) || len(pattern) == 0 {
		return nil
	}

	var result []int
	var pCount, wCount [26]int

	for i := 0; i < len(pattern); i++ {
		pCount[pattern[i]-'a']++
	}

	for i := 0; i < len(text); i++ {
		wCount[text[i]-'a']++
		if i >= len(pattern) {
			wCount[text[i-len(pattern)]-'a']--
		}
		if wCount == pCount {
			result = append(result, i-len(pattern)+1)
		}
	}
	return result
}

// TrappingRainWater calculates how much water can be trapped.
func TrappingRainWater(heights []int) int {
	if len(heights) < 3 {
		return 0
	}
	left, right := 0, len(heights)-1
	leftMax, rightMax := 0, 0
	water := 0

	for left < right {
		if heights[left] < heights[right] {
			if heights[left] >= leftMax {
				leftMax = heights[left]
			} else {
				water += leftMax - heights[left]
			}
			left++
		} else {
			if heights[right] >= rightMax {
				rightMax = heights[right]
			} else {
				water += rightMax - heights[right]
			}
			right--
		}
	}
	return water
}

// LongestSubarrayWithSumK finds the longest subarray with sum exactly k.
func LongestSubarrayWithSumK(arr []int, k int) int {
	prefixSum := make(map[int]int)
	prefixSum[0] = -1
	sum := 0
	maxLen := 0

	for i, v := range arr {
		sum += v
		if idx, ok := prefixSum[sum-k]; ok {
			length := i - idx
			if length > maxLen {
				maxLen = length
			}
		}
		if _, ok := prefixSum[sum]; !ok {
			prefixSum[sum] = i
		}
	}
	return maxLen
}

// SubarrayProductLessThanK counts subarrays with product < k.
func SubarrayProductLessThanK(arr []int, k int) int {
	if k <= 1 {
		return 0
	}
	count := 0
	product := 1
	left := 0

	for right := 0; right < len(arr); right++ {
		product *= arr[right]
		for product >= k && left <= right {
			product /= arr[left]
			left++
		}
		count += right - left + 1
	}
	return count
}

// RemoveDuplicatesSorted removes duplicates from a sorted array in-place.
func RemoveDuplicatesSorted[T comparable](arr []T) int {
	if len(arr) <= 1 {
		return len(arr)
	}
	writeIdx := 1
	for i := 1; i < len(arr); i++ {
		if arr[i] != arr[i-1] {
			arr[writeIdx] = arr[i]
			writeIdx++
		}
	}
	return writeIdx
}

// MoveZeroes moves all zeros to the end while maintaining order of non-zeros.
func MoveZeroes(arr []int) {
	writeIdx := 0
	for _, v := range arr {
		if v != 0 {
			arr[writeIdx] = v
			writeIdx++
		}
	}
	for i := writeIdx; i < len(arr); i++ {
		arr[i] = 0
	}
}

// FourSum finds all unique quadruplets that sum to target.
func FourSum(arr []int, target int) [][4]int {
	sorted := make([]int, len(arr))
	copy(sorted, arr)
	InsertionSort(sorted)

	var result [][4]int
	n := len(sorted)

	for i := 0; i < n-3; i++ {
		if i > 0 && sorted[i] == sorted[i-1] {
			continue
		}
		for j := i + 1; j < n-2; j++ {
			if j > i+1 && sorted[j] == sorted[j-1] {
				continue
			}
			left, right := j+1, n-1
			for left < right {
				sum := sorted[i] + sorted[j] + sorted[left] + sorted[right]
				if sum == target {
					result = append(result, [4]int{sorted[i], sorted[j], sorted[left], sorted[right]})
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
