package algorithms

import (
	"testing"
)

func TestActivitySelection(t *testing.T) {
	activities := []Interval{
		{1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 9}, {5, 9}, {6, 10}, {8, 11}, {8, 12}, {2, 14}, {12, 16},
	}
	selected := ActivitySelection(activities)
	if len(selected) != 4 {
		t.Errorf("expected 4 activities, got %d", len(selected))
	}
}

func TestActivitySelectionEmpty(t *testing.T) {
	result := ActivitySelection(nil)
	if result != nil {
		t.Error("expected nil for empty input")
	}
}

func TestActivitySelectionNoOverlap(t *testing.T) {
	activities := []Interval{{1, 2}, {3, 4}, {5, 6}}
	selected := ActivitySelection(activities)
	if len(selected) != 3 {
		t.Errorf("expected 3, got %d", len(selected))
	}
}

func TestMergeIntervals(t *testing.T) {
	intervals := []Interval{{1, 3}, {2, 6}, {8, 10}, {15, 18}}
	merged := MergeIntervals(intervals)
	expected := []Interval{{1, 6}, {8, 10}, {15, 18}}
	if len(merged) != len(expected) {
		t.Fatalf("expected %d intervals, got %d", len(expected), len(merged))
	}
	for i, iv := range expected {
		if merged[i].Start != iv.Start || merged[i].End != iv.End {
			t.Errorf("index %d: expected %v, got %v", i, iv, merged[i])
		}
	}
}

func TestMergeIntervalsAllOverlap(t *testing.T) {
	intervals := []Interval{{1, 4}, {2, 5}, {3, 6}}
	merged := MergeIntervals(intervals)
	if len(merged) != 1 || merged[0].Start != 1 || merged[0].End != 6 {
		t.Errorf("expected [{1 6}], got %v", merged)
	}
}

func TestMergeIntervalsNoOverlap(t *testing.T) {
	intervals := []Interval{{1, 2}, {5, 6}, {9, 10}}
	merged := MergeIntervals(intervals)
	if len(merged) != 3 {
		t.Errorf("expected 3, got %d", len(merged))
	}
}

func TestMergeIntervalsSingle(t *testing.T) {
	intervals := []Interval{{1, 5}}
	merged := MergeIntervals(intervals)
	if len(merged) != 1 {
		t.Errorf("expected 1, got %d", len(merged))
	}
}

func TestIntervalIntersection(t *testing.T) {
	a := []Interval{{0, 2}, {5, 10}, {13, 23}, {24, 25}}
	b := []Interval{{1, 5}, {8, 12}, {15, 24}, {25, 26}}
	result := IntervalIntersection(a, b)
	expected := []Interval{{1, 2}, {5, 5}, {8, 10}, {15, 23}, {24, 24}, {25, 25}}
	if len(result) != len(expected) {
		t.Fatalf("expected %d intersections, got %d: %v", len(expected), len(result), result)
	}
	for i, iv := range expected {
		if result[i].Start != iv.Start || result[i].End != iv.End {
			t.Errorf("index %d: expected %v, got %v", i, iv, result[i])
		}
	}
}

func TestIntervalIntersectionNoOverlap(t *testing.T) {
	a := []Interval{{1, 2}, {5, 6}}
	b := []Interval{{3, 4}, {7, 8}}
	result := IntervalIntersection(a, b)
	if len(result) != 0 {
		t.Errorf("expected no intersections, got %v", result)
	}
}

func TestMinMeetingRooms(t *testing.T) {
	intervals := []Interval{{0, 30}, {5, 10}, {15, 20}}
	rooms := MinMeetingRooms(intervals)
	if rooms != 2 {
		t.Errorf("expected 2 rooms, got %d", rooms)
	}
}

func TestMinMeetingRoomsNoOverlap(t *testing.T) {
	intervals := []Interval{{0, 5}, {5, 10}, {10, 15}}
	rooms := MinMeetingRooms(intervals)
	if rooms != 1 {
		t.Errorf("expected 1 room, got %d", rooms)
	}
}

func TestMinMeetingRoomsEmpty(t *testing.T) {
	rooms := MinMeetingRooms(nil)
	if rooms != 0 {
		t.Errorf("expected 0, got %d", rooms)
	}
}

func TestInsertInterval(t *testing.T) {
	intervals := []Interval{{1, 3}, {6, 9}}
	result := InsertInterval(intervals, Interval{2, 5})
	if len(result) != 2 || result[0].Start != 1 || result[0].End != 5 {
		t.Errorf("expected [{1 5} {6 9}], got %v", result)
	}
}

func TestInsertIntervalMergeAll(t *testing.T) {
	intervals := []Interval{{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}}
	result := InsertInterval(intervals, Interval{4, 8})
	expected := []Interval{{1, 2}, {3, 10}, {12, 16}}
	if len(result) != len(expected) {
		t.Fatalf("expected %d, got %d: %v", len(expected), len(result), result)
	}
}

func TestInsertIntervalNoMerge(t *testing.T) {
	intervals := []Interval{{1, 2}, {5, 6}}
	result := InsertInterval(intervals, Interval{3, 4})
	if len(result) != 3 {
		t.Errorf("expected 3, got %d", len(result))
	}
}

func TestFractionalKnapsack(t *testing.T) {
	weights := []float64{10, 20, 30}
	values := []float64{60, 100, 120}
	result := FractionalKnapsack(weights, values, 50)
	if result < 239.9 || result > 240.1 {
		t.Errorf("expected 240, got %f", result)
	}
}

func TestFractionalKnapsackAllFit(t *testing.T) {
	weights := []float64{10, 20}
	values := []float64{60, 100}
	result := FractionalKnapsack(weights, values, 100)
	if result != 160 {
		t.Errorf("expected 160, got %f", result)
	}
}

func TestFractionalKnapsackEmpty(t *testing.T) {
	result := FractionalKnapsack(nil, nil, 50)
	if result != 0 {
		t.Errorf("expected 0, got %f", result)
	}
}

func TestJobSequencing(t *testing.T) {
	deadlines := []int{4, 1, 1, 1}
	profits := []int{20, 10, 40, 30}
	profit, scheduled := JobSequencing(deadlines, profits)
	if profit != 60 {
		t.Errorf("expected profit 60, got %d", profit)
	}
	if len(scheduled) != 2 {
		t.Errorf("expected 2 jobs scheduled, got %d", len(scheduled))
	}
}

func TestJobSequencingEmpty(t *testing.T) {
	profit, scheduled := JobSequencing(nil, nil)
	if profit != 0 || scheduled != nil {
		t.Error("expected 0 profit and nil for empty")
	}
}

func TestJobSequencingSingle(t *testing.T) {
	profit, scheduled := JobSequencing([]int{1}, []int{50})
	if profit != 50 || len(scheduled) != 1 {
		t.Errorf("expected 50 profit with 1 job, got %d with %d jobs", profit, len(scheduled))
	}
}

func TestHuffmanEncoding(t *testing.T) {
	freq := map[byte]int{'a': 5, 'b': 9, 'c': 12, 'd': 13, 'e': 16, 'f': 45}
	codes := HuffmanEncoding(freq)
	if len(codes) != 6 {
		t.Errorf("expected 6 codes, got %d", len(codes))
	}
	// Verify prefix-free property: no code is prefix of another
	for c1, code1 := range codes {
		for c2, code2 := range codes {
			if c1 != c2 {
				if len(code1) <= len(code2) && code2[:len(code1)] == code1 {
					t.Errorf("code for %c (%s) is prefix of code for %c (%s)", c1, code1, c2, code2)
				}
			}
		}
	}
}

func TestHuffmanEncodingSingle(t *testing.T) {
	freq := map[byte]int{'a': 5}
	codes := HuffmanEncoding(freq)
	if codes['a'] != "0" {
		t.Errorf("expected '0' for single char, got %q", codes['a'])
	}
}

func TestHuffmanEncodingEmpty(t *testing.T) {
	codes := HuffmanEncoding(nil)
	if codes != nil {
		t.Error("expected nil for empty")
	}
}

func TestHuffmanEncodingTwo(t *testing.T) {
	freq := map[byte]int{'a': 1, 'b': 1}
	codes := HuffmanEncoding(freq)
	if len(codes) != 2 {
		t.Errorf("expected 2 codes, got %d", len(codes))
	}
	// One should be "0" and the other "1"
	if len(codes['a']) != 1 || len(codes['b']) != 1 {
		t.Error("two chars should have single-bit codes")
	}
}

func TestJumpGame(t *testing.T) {
	if !JumpGame([]int{2, 3, 1, 1, 4}) {
		t.Error("should be able to reach end")
	}
	if JumpGame([]int{3, 2, 1, 0, 4}) {
		t.Error("should not be able to reach end")
	}
}

func TestJumpGameSingle(t *testing.T) {
	if !JumpGame([]int{0}) {
		t.Error("single element should be reachable")
	}
}

func TestJumpGameMinJumps(t *testing.T) {
	result := JumpGameMinJumps([]int{2, 3, 1, 1, 4})
	if result != 2 {
		t.Errorf("expected 2 jumps, got %d", result)
	}
}

func TestJumpGameMinJumpsSingle(t *testing.T) {
	result := JumpGameMinJumps([]int{0})
	if result != 0 {
		t.Errorf("expected 0 jumps, got %d", result)
	}
}

func TestGasStation(t *testing.T) {
	gas := []int{1, 2, 3, 4, 5}
	cost := []int{3, 4, 5, 1, 2}
	result := GasStation(gas, cost)
	if result != 3 {
		t.Errorf("expected start at 3, got %d", result)
	}
}

func TestGasStationImpossible(t *testing.T) {
	gas := []int{2, 3, 4}
	cost := []int{3, 4, 3}
	result := GasStation(gas, cost)
	if result != -1 {
		t.Errorf("expected -1, got %d", result)
	}
}

func TestMinPlatforms(t *testing.T) {
	arrivals := []int{900, 940, 950, 1100, 1500, 1800}
	departures := []int{910, 1200, 1120, 1130, 1900, 2000}
	result := MinPlatforms(arrivals, departures)
	if result != 3 {
		t.Errorf("expected 3 platforms, got %d", result)
	}
}

func TestMinPlatformsSingle(t *testing.T) {
	result := MinPlatforms([]int{900}, []int{1000})
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}
}

func TestTaskScheduler(t *testing.T) {
	tasks := []byte{'A', 'A', 'A', 'B', 'B', 'B'}
	result := TaskScheduler(tasks, 2)
	if result != 8 {
		t.Errorf("expected 8, got %d", result)
	}
}

func TestTaskSchedulerNoCooldown(t *testing.T) {
	tasks := []byte{'A', 'A', 'A', 'B', 'B', 'B'}
	result := TaskScheduler(tasks, 0)
	if result != 6 {
		t.Errorf("expected 6, got %d", result)
	}
}

func TestTaskSchedulerAllDifferent(t *testing.T) {
	tasks := []byte{'A', 'B', 'C', 'D'}
	result := TaskScheduler(tasks, 3)
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}
}

func TestNonOverlappingIntervals(t *testing.T) {
	intervals := []Interval{{1, 2}, {2, 3}, {3, 4}, {1, 3}}
	result := NonOverlappingIntervals(intervals)
	if result != 1 {
		t.Errorf("expected 1 removal, got %d", result)
	}
}

func TestNonOverlappingIntervalsNone(t *testing.T) {
	intervals := []Interval{{1, 2}, {3, 4}}
	result := NonOverlappingIntervals(intervals)
	if result != 0 {
		t.Errorf("expected 0 removals, got %d", result)
	}
}

func TestNonOverlappingIntervalsAll(t *testing.T) {
	intervals := []Interval{{1, 10}, {2, 10}, {3, 10}}
	result := NonOverlappingIntervals(intervals)
	if result != 2 {
		t.Errorf("expected 2 removals, got %d", result)
	}
}

func TestCanCompleteCircuit(t *testing.T) {
	result := CanCompleteCircuit([]int{1, 2, 3, 4, 5}, []int{3, 4, 5, 1, 2})
	if result != 3 {
		t.Errorf("expected 3, got %d", result)
	}
}
