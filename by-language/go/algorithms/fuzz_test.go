package algorithms

import (
	"encoding/binary"
	"math"
	"slices"
	"sort"
	"strings"
	"testing"
)

func encodeInts(vals ...int64) []byte {
	buf := make([]byte, 8*len(vals))
	for i, v := range vals {
		binary.LittleEndian.PutUint64(buf[i*8:], uint64(v))
	}
	return buf
}

func decodeInts(data []byte) []int {
	n := len(data) / 8
	if n > 32 {
		n = 32
	}
	out := make([]int, n)
	for i := 0; i < n; i++ {
		out[i] = int(int64(binary.LittleEndian.Uint64(data[i*8 : i*8+8])))
	}
	return out
}

func FuzzIntSorts(f *testing.F) {
	f.Add([]byte{})
	f.Add(encodeInts(1))
	f.Add(encodeInts(5, 3, 8, 1, 9, 2, 7, 4, 6))
	f.Add(encodeInts(3, 3, 3, 1, 1, 2, 2))
	f.Add(encodeInts(-4, -1, 0, 2, 2, -3))
	f.Add(encodeInts(math.MaxInt64, -1, math.MinInt64, 0, 1))
	f.Add(encodeInts(0, 1<<40, 1<<40))

	f.Fuzz(func(t *testing.T, data []byte) {
		base := decodeInts(data)
		want := append([]int(nil), base...)
		slices.Sort(want)

		check := func(name string, got []int) {
			t.Helper()
			if !slices.Equal(got, want) {
				t.Fatalf("%s(%v) = %v, want %v", name, base, got, want)
			}
		}
		sortCopy := func(name string, fn func([]int)) {
			t.Helper()
			arr := append([]int(nil), base...)
			fn(arr)
			check(name, arr)
		}

		sortCopy("BubbleSort", func(a []int) { BubbleSort(a) })
		sortCopy("SelectionSort", func(a []int) { SelectionSort(a) })
		sortCopy("InsertionSort", func(a []int) { InsertionSort(a) })
		sortCopy("QuickSort", func(a []int) { QuickSort(a) })
		sortCopy("HeapSortAlgo", func(a []int) { HeapSortAlgo(a) })
		sortCopy("ShellSort", func(a []int) { ShellSort(a) })
		sortCopy("CountingSort", func(a []int) { CountingSort(a) })
		sortCopy("RadixSort", func(a []int) { RadixSort(a) })
		sortCopy("TimSort", func(a []int) { TimSort(a) })
		sortCopy("CombSort", func(a []int) { CombSort(a) })
		sortCopy("GnomeSort", func(a []int) { GnomeSort(a) })
		sortCopy("PancakeSort", func(a []int) { PancakeSort(a) })
		sortCopy("CycleSort", func(a []int) { CycleSort(a) })
		sortCopy("MergeSortIterative", func(a []int) { MergeSortIterative(a) })
		check("MergeSort", MergeSort(append([]int(nil), base...)))
	})
}

func FuzzSortedSearch(f *testing.F) {
	f.Add([]byte{})
	f.Add(encodeInts(1, 3, 3, 5, 8, 3))
	f.Add(encodeInts(0, 4, 9))
	f.Add(encodeInts(-1<<62, 0, 1<<62, 0))
	f.Add(encodeInts(math.MinInt64, -1, 0, 1, math.MaxInt64, 42))

	f.Fuzz(func(t *testing.T, data []byte) {
		vals := decodeInts(data)
		target := 0
		var arr []int
		if len(vals) > 0 {
			target = vals[len(vals)-1]
			arr = append([]int(nil), vals[:len(vals)-1]...)
		}
		slices.Sort(arr)

		checkSortedSearch(t, arr, target)
		seen := make(map[int]struct{}, len(arr))
		for _, v := range arr {
			if _, ok := seen[v]; ok {
				continue
			}
			seen[v] = struct{}{}
			checkSortedSearch(t, arr, v)
		}
		if len(arr) > 0 && arr[0] > math.MinInt {
			checkSortedSearch(t, arr, arr[0]-1)
		}
		if len(arr) > 0 && arr[len(arr)-1] < math.MaxInt {
			checkSortedSearch(t, arr, arr[len(arr)-1]+1)
		}
	})
}

func checkSortedSearch(t *testing.T, arr []int, target int) {
	t.Helper()
	wantLower := sort.SearchInts(arr, target)
	if got := LowerBound(arr, target); got != wantLower {
		t.Fatalf("LowerBound(%v, %d) = %d, want %d", arr, target, got, wantLower)
	}
	wantUpper := sort.Search(len(arr), func(i int) bool { return arr[i] > target })
	if got := UpperBound(arr, target); got != wantUpper {
		t.Fatalf("UpperBound(%v, %d) = %d, want %d", arr, target, got, wantUpper)
	}

	found := wantLower < len(arr) && arr[wantLower] == target
	gotBin := BinarySearch(arr, target)
	if found {
		if gotBin < 0 || gotBin >= len(arr) || arr[gotBin] != target {
			t.Fatalf("BinarySearch(%v, %d) = %d", arr, target, gotBin)
		}
	} else if gotBin != -1 {
		t.Fatalf("BinarySearch(%v, %d) = %d, want -1", arr, target, gotBin)
	}

	wantFirst, wantLast := -1, -1
	if found {
		wantFirst = wantLower
		wantLast = wantUpper - 1
	}
	if got := FindFirstOccurrence(arr, target); got != wantFirst {
		t.Fatalf("FindFirstOccurrence(%v, %d) = %d, want %d", arr, target, got, wantFirst)
	}
	if got := FindLastOccurrence(arr, target); got != wantLast {
		t.Fatalf("FindLastOccurrence(%v, %d) = %d, want %d", arr, target, got, wantLast)
	}

	checkIndex := func(name string, got int) {
		t.Helper()
		if found {
			if got < 0 || got >= len(arr) || arr[got] != target {
				t.Fatalf("%s(%v, %d) = %d", name, arr, target, got)
			}
			return
		}
		if got != -1 {
			t.Fatalf("%s(%v, %d) = %d, want -1", name, arr, target, got)
		}
	}
	checkIndex("JumpSearch", JumpSearch(arr, target))
	checkIndex("ExponentialSearch", ExponentialSearch(arr, target))
	checkIndex("InterpolationSearch", InterpolationSearch(arr, target))
}

func FuzzStringSearch(f *testing.F) {
	f.Add("AABAACAADAABAABA", "AABA")
	f.Add("aaaaaa", "aa")
	f.Add("hello", "xyz")
	f.Add("abc", "")
	f.Add("", "")
	f.Add("", "a")
	f.Add("$$", "$")
	f.Add("a$a$a", "a$")
	f.Add("abababa", "aba")

	f.Fuzz(func(t *testing.T, text, pattern string) {
		if len(text) > 48 {
			text = text[:48]
		}
		if len(pattern) > 12 {
			pattern = pattern[:12]
		}
		want := bruteForceSearch(text, pattern)
		algs := []struct {
			name string
			fn   func(string, string) []int
		}{
			{"KMPSearch", KMPSearch},
			{"RabinKarpSearch", RabinKarpSearch},
			{"ZAlgorithmSearch", ZAlgorithmSearch},
		}
		for _, alg := range algs {
			got := alg.fn(text, pattern)
			if !slices.Equal(got, want) {
				t.Fatalf("%s(%q, %q) = %v, want %v", alg.name, text, pattern, got, want)
			}
		}
	})
}

// bruteForceSearch reports every overlapping match. An empty pattern matches
// nothing, which is the contract already pinned by KMP and Boyer-Moore.
func bruteForceSearch(text, pattern string) []int {
	if pattern == "" {
		return nil
	}
	var idx []int
	start := 0
	for start+len(pattern) <= len(text) {
		rel := strings.Index(text[start:], pattern)
		if rel < 0 {
			break
		}
		idx = append(idx, start+rel)
		start += rel + 1
	}
	return idx
}
