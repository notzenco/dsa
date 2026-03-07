package datastructures

import "testing"

func TestMinHeap(t *testing.T) {
	h := NewMinHeap[int]()

	h.Push(5)
	h.Push(3)
	h.Push(7)
	h.Push(1)

	val, ok := h.Peek()
	if !ok || val != 1 {
		t.Errorf("expected peek 1, got %d", val)
	}

	val, _ = h.Pop()
	if val != 1 {
		t.Errorf("expected pop 1, got %d", val)
	}

	val, _ = h.Pop()
	if val != 3 {
		t.Errorf("expected pop 3, got %d", val)
	}

	val, _ = h.Pop()
	if val != 5 {
		t.Errorf("expected pop 5, got %d", val)
	}

	val, _ = h.Pop()
	if val != 7 {
		t.Errorf("expected pop 7, got %d", val)
	}

	_, ok = h.Pop()
	if ok {
		t.Error("pop from empty heap should fail")
	}
}

func TestMinHeapFromSlice(t *testing.T) {
	h := MinHeapFromSlice([]int{5, 3, 7, 1, 9, 2})

	sorted := h.ToSortedSlice()
	expected := []int{1, 2, 3, 5, 7, 9}
	for i, v := range expected {
		if sorted[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, sorted[i])
		}
	}
}

func TestMinHeapPushPop(t *testing.T) {
	h := MinHeapFromSlice([]int{1, 3, 5})

	val := h.PushPop(2)
	if val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	val = h.PushPop(0)
	if val != 0 {
		t.Errorf("expected 0, got %d", val)
	}
}

func TestMinHeapReplace(t *testing.T) {
	h := MinHeapFromSlice([]int{1, 3, 5})

	old, ok := h.Replace(10)
	if !ok || old != 1 {
		t.Errorf("expected old=1, got %d", old)
	}

	peek, _ := h.Peek()
	if peek != 3 {
		t.Errorf("expected peek 3, got %d", peek)
	}
}

func TestMaxHeap(t *testing.T) {
	h := NewMaxHeap[int]()

	h.Push(5)
	h.Push(3)
	h.Push(7)
	h.Push(1)

	val, _ := h.Pop()
	if val != 7 {
		t.Errorf("expected 7, got %d", val)
	}

	val, _ = h.Pop()
	if val != 5 {
		t.Errorf("expected 5, got %d", val)
	}
}

func TestMaxHeapFromSlice(t *testing.T) {
	h := MaxHeapFromSlice([]int{5, 3, 7, 1, 9, 2})

	val, _ := h.Peek()
	if val != 9 {
		t.Errorf("expected peek 9, got %d", val)
	}

	sorted := h.ToSortedSlice()
	expected := []int{9, 7, 5, 3, 2, 1}
	for i, v := range expected {
		if sorted[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, sorted[i])
		}
	}
}

func TestHeapSort(t *testing.T) {
	arr := []int{5, 3, 7, 1, 9, 2, 8, 4, 6}
	HeapSort(arr)

	for i := 1; i < len(arr); i++ {
		if arr[i] < arr[i-1] {
			t.Errorf("not sorted at index %d: %v", i, arr)
			break
		}
	}
}

func TestNLargest(t *testing.T) {
	result := NLargest([]int{5, 3, 7, 1, 9, 2}, 3)
	if len(result) != 3 {
		t.Fatalf("expected 3 results, got %d", len(result))
	}
	// Should contain 5, 7, 9 in sorted order
	expected := []int{5, 7, 9}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestNSmallest(t *testing.T) {
	result := NSmallest([]int{5, 3, 7, 1, 9, 2}, 3)
	if len(result) != 3 {
		t.Fatalf("expected 3 results, got %d", len(result))
	}
	expected := []int{1, 2, 3}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}
