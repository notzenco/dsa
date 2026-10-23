package datastructures

import "testing"

func TestFenwickInvalidUpdates(t *testing.T) {
	for _, index := range []int{-2, -1, 2} {
		func() {
			defer func() {
				if recover() == nil {
					t.Errorf("Update(%d) did not reject invalid index", index)
				}
			}()
			NewFenwickTree([]int{1, 2}).Update(index, 1)
		}()
	}
	func() {
		defer func() {
			if recover() == nil {
				t.Error("2D Update did not reject negative index")
			}
		}()
		NewFenwickTree2D([][]int{{1}}).Update(-1, 0, 1)
	}()
}

// --- Segment Tree Tests ---

func TestSegmentTree(t *testing.T) {
	arr := []int{1, 3, 5, 7, 9, 11}
	st := NewSumSegmentTree(arr)

	result := st.Query(1, 4) // 3+5+7+9 = 24
	if result != 24 {
		t.Errorf("expected 24, got %d", result)
	}

	st.Update(2, 10)        // set index 2 to 10
	result = st.Query(1, 4) // 3+10+7+9 = 29
	if result != 29 {
		t.Errorf("expected 29, got %d", result)
	}
}

func TestSegmentTreeMin(t *testing.T) {
	arr := []int{5, 2, 8, 1, 4, 7}
	st := NewMinSegmentTree(arr)

	result := st.Query(0, 5)
	if result != 1 {
		t.Errorf("expected min 1, got %d", result)
	}

	result = st.Query(0, 2)
	if result != 2 {
		t.Errorf("expected min 2, got %d", result)
	}

	st.Update(3, 10) // change 1 to 10
	result = st.Query(0, 5)
	if result != 2 {
		t.Errorf("expected min 2, got %d", result)
	}
}

func TestSegmentTreeMax(t *testing.T) {
	arr := []int{5, 2, 8, 1, 4, 7}
	st := NewMaxSegmentTree(arr)

	result := st.Query(0, 5)
	if result != 8 {
		t.Errorf("expected max 8, got %d", result)
	}
}

func TestSegmentTreeLazy(t *testing.T) {
	arr := []int{1, 3, 5, 7, 9, 11}
	st := NewSegmentTreeLazy(arr)

	result := st.Query(1, 4) // 3+5+7+9 = 24
	if result != 24 {
		t.Errorf("expected 24, got %d", result)
	}

	st.RangeUpdate(1, 4, 10) // add 10 to indices 1-4
	result = st.Query(1, 4)  // (3+10)+(5+10)+(7+10)+(9+10) = 64
	if result != 64 {
		t.Errorf("expected 64, got %d", result)
	}

	// Original indices outside range should be unchanged
	result = st.Query(0, 0)
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}

	result = st.Query(5, 5)
	if result != 11 {
		t.Errorf("expected 11, got %d", result)
	}
}

// --- Fenwick Tree Tests ---

func TestFenwickTree(t *testing.T) {
	arr := []int{3, 2, -1, 6, 5, 4, -3, 3}
	ft := NewFenwickTree(arr)

	result := ft.PrefixSum(4) // 3+2-1+6+5 = 15
	if result != 15 {
		t.Errorf("expected 15, got %d", result)
	}

	result = ft.RangeSum(2, 5) // -1+6+5+4 = 14
	if result != 14 {
		t.Errorf("expected 14, got %d", result)
	}

	ft.Update(3, 4)            // add 4 to index 3
	result = ft.RangeSum(2, 5) // -1+10+5+4 = 18
	if result != 18 {
		t.Errorf("expected 18, got %d", result)
	}
}

func TestFenwickTreeSet(t *testing.T) {
	arr := []int{1, 2, 3, 4, 5}
	ft := NewFenwickTree(arr)

	ft.Set(2, 10) // set index 2 to 10
	result := ft.RangeSum(2, 2)
	if result != 10 {
		t.Errorf("expected 10, got %d", result)
	}

	result = ft.PrefixSum(4) // 1+2+10+4+5 = 22
	if result != 22 {
		t.Errorf("expected 22, got %d", result)
	}
}

func TestFenwickTree2D(t *testing.T) {
	matrix := [][]int{
		{1, 2},
		{3, 4},
	}
	ft := NewFenwickTree2D(matrix)

	result := ft.Query(0, 0, 1, 1) // 1+2+3+4 = 10
	if result != 10 {
		t.Errorf("expected 10, got %d", result)
	}

	result = ft.Query(0, 0, 0, 0)
	if result != 1 {
		t.Errorf("expected 1, got %d", result)
	}

	result = ft.Query(1, 1, 1, 1)
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}
}

func TestCountInversions(t *testing.T) {
	result := CountInversions([]int{2, 4, 1, 3, 5})
	if result != 3 {
		t.Errorf("expected 3 inversions, got %d", result)
	}

	result = CountInversions([]int{5, 4, 3, 2, 1})
	if result != 10 {
		t.Errorf("expected 10 inversions, got %d", result)
	}

	result = CountInversions([]int{1, 2, 3, 4, 5})
	if result != 0 {
		t.Errorf("expected 0 inversions, got %d", result)
	}
}
