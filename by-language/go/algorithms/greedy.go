package algorithms

import "sort"

// Interval represents a time interval with start and end.
type Interval struct {
	Start, End int
}

// ActivitySelection selects the maximum number of non-overlapping activities.
// Activities are selected greedily by earliest finish time.
func ActivitySelection(activities []Interval) []Interval {
	if len(activities) == 0 {
		return nil
	}
	sorted := make([]Interval, len(activities))
	copy(sorted, activities)
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].End < sorted[j].End
	})

	selected := []Interval{sorted[0]}
	lastEnd := sorted[0].End

	for i := 1; i < len(sorted); i++ {
		if sorted[i].Start >= lastEnd {
			selected = append(selected, sorted[i])
			lastEnd = sorted[i].End
		}
	}
	return selected
}

// MergeIntervals merges overlapping intervals.
func MergeIntervals(intervals []Interval) []Interval {
	if len(intervals) <= 1 {
		return intervals
	}
	sorted := make([]Interval, len(intervals))
	copy(sorted, intervals)
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].Start < sorted[j].Start
	})

	merged := []Interval{sorted[0]}
	for i := 1; i < len(sorted); i++ {
		last := &merged[len(merged)-1]
		if sorted[i].Start <= last.End {
			if sorted[i].End > last.End {
				last.End = sorted[i].End
			}
		} else {
			merged = append(merged, sorted[i])
		}
	}
	return merged
}

// IntervalIntersection returns the intersection of two lists of sorted intervals.
func IntervalIntersection(a, b []Interval) []Interval {
	var result []Interval
	i, j := 0, 0
	for i < len(a) && j < len(b) {
		lo := a[i].Start
		if b[j].Start > lo {
			lo = b[j].Start
		}
		hi := a[i].End
		if b[j].End < hi {
			hi = b[j].End
		}
		if lo <= hi {
			result = append(result, Interval{lo, hi})
		}
		if a[i].End < b[j].End {
			i++
		} else {
			j++
		}
	}
	return result
}

// MinMeetingRooms returns the minimum number of meeting rooms needed.
func MinMeetingRooms(intervals []Interval) int {
	if len(intervals) == 0 {
		return 0
	}
	starts := make([]int, len(intervals))
	ends := make([]int, len(intervals))
	for i, iv := range intervals {
		starts[i] = iv.Start
		ends[i] = iv.End
	}
	sort.Ints(starts)
	sort.Ints(ends)

	rooms, maxRooms := 0, 0
	si, ei := 0, 0
	for si < len(starts) {
		if starts[si] < ends[ei] {
			rooms++
			si++
		} else {
			rooms--
			ei++
		}
		if rooms > maxRooms {
			maxRooms = rooms
		}
	}
	return maxRooms
}

// InsertInterval inserts a new interval into a sorted list and merges if necessary.
func InsertInterval(intervals []Interval, newInterval Interval) []Interval {
	var result []Interval
	i := 0

	// Add all intervals before the new one
	for i < len(intervals) && intervals[i].End < newInterval.Start {
		result = append(result, intervals[i])
		i++
	}

	// Merge overlapping intervals
	for i < len(intervals) && intervals[i].Start <= newInterval.End {
		if intervals[i].Start < newInterval.Start {
			newInterval.Start = intervals[i].Start
		}
		if intervals[i].End > newInterval.End {
			newInterval.End = intervals[i].End
		}
		i++
	}
	result = append(result, newInterval)

	// Add remaining
	for i < len(intervals) {
		result = append(result, intervals[i])
		i++
	}
	return result
}

// FractionalKnapsack solves the knapsack problem with fractional items.
// Returns the maximum value achievable.
func FractionalKnapsack(weights, values []float64, capacity float64) float64 {
	n := len(weights)
	if n == 0 || capacity <= 0 {
		return 0
	}

	type item struct {
		weight, value, ratio float64
	}
	items := make([]item, n)
	for i := 0; i < n; i++ {
		items[i] = item{weights[i], values[i], values[i] / weights[i]}
	}
	sort.Slice(items, func(i, j int) bool {
		return items[i].ratio > items[j].ratio
	})

	totalValue := 0.0
	remaining := capacity
	for _, it := range items {
		if remaining <= 0 {
			break
		}
		if it.weight <= remaining {
			totalValue += it.value
			remaining -= it.weight
		} else {
			totalValue += it.ratio * remaining
			remaining = 0
		}
	}
	return totalValue
}

// JobSequencing schedules jobs to maximize profit.
// Each job has a deadline and profit. Returns max profit and scheduled job indices.
func JobSequencing(deadlines []int, profits []int) (int, []int) {
	n := len(deadlines)
	if n == 0 {
		return 0, nil
	}

	type job struct {
		index    int
		deadline int
		profit   int
	}
	jobs := make([]job, n)
	maxDeadline := 0
	for i := 0; i < n; i++ {
		jobs[i] = job{i, deadlines[i], profits[i]}
		if deadlines[i] > maxDeadline {
			maxDeadline = deadlines[i]
		}
	}
	sort.Slice(jobs, func(i, j int) bool {
		return jobs[i].profit > jobs[j].profit
	})

	slots := make([]int, maxDeadline+1)
	for i := range slots {
		slots[i] = -1
	}

	totalProfit := 0
	var scheduled []int
	for _, j := range jobs {
		for t := j.deadline; t >= 1; t-- {
			if slots[t] == -1 {
				slots[t] = j.index
				totalProfit += j.profit
				scheduled = append(scheduled, j.index)
				break
			}
		}
	}
	return totalProfit, scheduled
}

// HuffmanNode represents a node in a Huffman tree.
type HuffmanNode struct {
	Char  byte
	Freq  int
	Left  *HuffmanNode
	Right *HuffmanNode
}

// HuffmanEncoding builds a Huffman encoding map from character frequencies.
func HuffmanEncoding(freq map[byte]int) map[byte]string {
	if len(freq) == 0 {
		return nil
	}
	if len(freq) == 1 {
		result := make(map[byte]string)
		for ch := range freq {
			result[ch] = "0"
		}
		return result
	}

	// Build priority queue (min-heap by frequency)
	nodes := make([]*HuffmanNode, 0, len(freq))
	for ch, f := range freq {
		nodes = append(nodes, &HuffmanNode{Char: ch, Freq: f})
	}
	sort.Slice(nodes, func(i, j int) bool {
		return nodes[i].Freq < nodes[j].Freq
	})

	for len(nodes) > 1 {
		left := nodes[0]
		right := nodes[1]
		nodes = nodes[2:]

		parent := &HuffmanNode{
			Freq:  left.Freq + right.Freq,
			Left:  left,
			Right: right,
		}

		// Insert in sorted position
		pos := sort.Search(len(nodes), func(i int) bool {
			return nodes[i].Freq >= parent.Freq
		})
		nodes = append(nodes, nil)
		copy(nodes[pos+1:], nodes[pos:])
		nodes[pos] = parent
	}

	// Build encoding map
	result := make(map[byte]string)
	var buildCodes func(*HuffmanNode, string)
	buildCodes = func(node *HuffmanNode, code string) {
		if node.Left == nil && node.Right == nil {
			result[node.Char] = code
			return
		}
		if node.Left != nil {
			buildCodes(node.Left, code+"0")
		}
		if node.Right != nil {
			buildCodes(node.Right, code+"1")
		}
	}
	buildCodes(nodes[0], "")
	return result
}

// JumpGame checks if you can reach the last index from index 0.
// Each element represents the max jump length at that position.
func JumpGame(nums []int) bool {
	maxReach := 0
	for i := 0; i < len(nums); i++ {
		if i > maxReach {
			return false
		}
		if i+nums[i] > maxReach {
			maxReach = i + nums[i]
		}
	}
	return true
}

// JumpGameMinJumps returns the minimum number of jumps to reach the end.
// Returns -1 if impossible.
func JumpGameMinJumps(nums []int) int {
	if len(nums) <= 1 {
		return 0
	}
	jumps, currentEnd, farthest := 0, 0, 0
	for i := 0; i < len(nums)-1; i++ {
		if i+nums[i] > farthest {
			farthest = i + nums[i]
		}
		if i == currentEnd {
			jumps++
			currentEnd = farthest
			if currentEnd >= len(nums)-1 {
				break
			}
		}
	}
	if currentEnd < len(nums)-1 {
		return -1
	}
	return jumps
}

// GasStation finds the starting gas station index for a circular tour.
// Returns -1 if no valid start exists.
func GasStation(gas, cost []int) int {
	totalSurplus, currentSurplus, start := 0, 0, 0
	for i := 0; i < len(gas); i++ {
		diff := gas[i] - cost[i]
		totalSurplus += diff
		currentSurplus += diff
		if currentSurplus < 0 {
			start = i + 1
			currentSurplus = 0
		}
	}
	if totalSurplus < 0 {
		return -1
	}
	return start
}

// CanCompleteCircuit is an alias for GasStation.
func CanCompleteCircuit(gas, cost []int) int {
	return GasStation(gas, cost)
}

// MinPlatforms returns the minimum number of platforms needed at a train station.
func MinPlatforms(arrivals, departures []int) int {
	arr := make([]int, len(arrivals))
	dep := make([]int, len(departures))
	copy(arr, arrivals)
	copy(dep, departures)
	sort.Ints(arr)
	sort.Ints(dep)

	platforms, maxPlatforms := 0, 0
	i, j := 0, 0
	for i < len(arr) {
		if arr[i] <= dep[j] {
			platforms++
			i++
		} else {
			platforms--
			j++
		}
		if platforms > maxPlatforms {
			maxPlatforms = platforms
		}
	}
	return maxPlatforms
}

// TaskScheduler returns minimum intervals needed to complete all tasks
// with a cooldown period between same tasks.
func TaskScheduler(tasks []byte, cooldown int) int {
	freq := make(map[byte]int)
	maxFreq := 0
	for _, t := range tasks {
		freq[t]++
		if freq[t] > maxFreq {
			maxFreq = freq[t]
		}
	}

	maxCount := 0
	for _, f := range freq {
		if f == maxFreq {
			maxCount++
		}
	}

	intervals := (maxFreq-1)*(cooldown+1) + maxCount
	if len(tasks) > intervals {
		return len(tasks)
	}
	return intervals
}

// NonOverlappingIntervals returns the minimum number of intervals to remove
// to make all intervals non-overlapping.
func NonOverlappingIntervals(intervals []Interval) int {
	if len(intervals) <= 1 {
		return 0
	}
	sorted := make([]Interval, len(intervals))
	copy(sorted, intervals)
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].End < sorted[j].End
	})

	count := 0
	lastEnd := sorted[0].End
	for i := 1; i < len(sorted); i++ {
		if sorted[i].Start < lastEnd {
			count++
		} else {
			lastEnd = sorted[i].End
		}
	}
	return count
}
