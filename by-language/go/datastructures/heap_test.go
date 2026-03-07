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

func TestPriorityQueue(t *testing.T) {
	pq := NewPriorityQueue[string]()
	pq.Enqueue("low", 3)
	pq.Enqueue("high", 1)
	pq.Enqueue("medium", 2)

	val, pri, ok := pq.Dequeue()
	if !ok || val != "high" || pri != 1 {
		t.Errorf("expected 'high' with priority 1, got '%s' %d", val, pri)
	}

	val, pri, ok = pq.Dequeue()
	if !ok || val != "medium" || pri != 2 {
		t.Errorf("expected 'medium' with priority 2, got '%s' %d", val, pri)
	}
}

func TestPriorityQueuePeek(t *testing.T) {
	pq := NewPriorityQueue[int]()
	pq.Enqueue(42, 5)
	pq.Enqueue(99, 1)

	val, pri, ok := pq.Peek()
	if !ok || val != 99 || pri != 1 {
		t.Errorf("expected 99 with priority 1, got %d %d", val, pri)
	}
	if pq.Size() != 2 {
		t.Error("peek should not remove element")
	}
}

func TestPriorityQueueStableOrder(t *testing.T) {
	pq := NewPriorityQueue[string]()
	pq.Enqueue("first", 1)
	pq.Enqueue("second", 1)
	pq.Enqueue("third", 1)

	val, _, _ := pq.Dequeue()
	if val != "first" {
		t.Errorf("expected 'first' (stable ordering), got '%s'", val)
	}
}

func TestPriorityQueueEmpty(t *testing.T) {
	pq := NewPriorityQueue[int]()
	_, _, ok := pq.Dequeue()
	if ok {
		t.Error("dequeue from empty should return false")
	}
	_, _, ok = pq.Peek()
	if ok {
		t.Error("peek from empty should return false")
	}
}

func TestPriorityQueueClear(t *testing.T) {
	pq := NewPriorityQueue[int]()
	pq.Enqueue(1, 1)
	pq.Enqueue(2, 2)
	pq.Clear()

	if !pq.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestMergeHeaps(t *testing.T) {
	h1 := MinHeapFromSlice([]int{1, 3, 5})
	h2 := MinHeapFromSlice([]int{2, 4, 6})

	merged := MergeHeaps(h1, h2)
	if merged.Size() != 6 {
		t.Errorf("expected size 6, got %d", merged.Size())
	}

	sorted := merged.ToSortedSlice()
	expected := []int{1, 2, 3, 4, 5, 6}
	for i, v := range expected {
		if sorted[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, sorted[i])
		}
	}
}

func TestKWayMerge(t *testing.T) {
	lists := [][]int{
		{1, 4, 7},
		{2, 5, 8},
		{3, 6, 9},
	}
	result := KWayMerge(lists)
	expected := []int{1, 2, 3, 4, 5, 6, 7, 8, 9}
	if len(result) != len(expected) {
		t.Fatalf("expected %d elements, got %d", len(expected), len(result))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestKWayMergeEmpty(t *testing.T) {
	result := KWayMerge[int](nil)
	if len(result) != 0 {
		t.Errorf("expected empty result, got %d elements", len(result))
	}

	result = KWayMerge([][]int{{}, {}})
	if len(result) != 0 {
		t.Errorf("expected empty result, got %d elements", len(result))
	}
}

func TestKWayMergeSingleList(t *testing.T) {
	lists := [][]int{{1, 2, 3}}
	result := KWayMerge(lists)
	if len(result) != 3 || result[0] != 1 || result[1] != 2 || result[2] != 3 {
		t.Errorf("expected [1,2,3], got %v", result)
	}
}

func TestMedianFinder(t *testing.T) {
	mf := NewMedianFinder()

	mf.AddNum(1)
	if mf.FindMedian() != 1.0 {
		t.Errorf("expected 1.0, got %f", mf.FindMedian())
	}

	mf.AddNum(2)
	if mf.FindMedian() != 1.5 {
		t.Errorf("expected 1.5, got %f", mf.FindMedian())
	}

	mf.AddNum(3)
	if mf.FindMedian() != 2.0 {
		t.Errorf("expected 2.0, got %f", mf.FindMedian())
	}
}

func TestMedianFinderLargeSequence(t *testing.T) {
	mf := NewMedianFinder()
	for i := 1; i <= 10; i++ {
		mf.AddNum(float64(i))
	}
	if mf.FindMedian() != 5.5 {
		t.Errorf("expected 5.5, got %f", mf.FindMedian())
	}
	if mf.Size() != 10 {
		t.Errorf("expected size 10, got %d", mf.Size())
	}
}

func TestMinHeapEmpty(t *testing.T) {
	h := NewMinHeap[int]()
	if !h.IsEmpty() {
		t.Error("new heap should be empty")
	}
	_, ok := h.Peek()
	if ok {
		t.Error("peek on empty should return false")
	}
}

func TestMaxHeapEmpty(t *testing.T) {
	h := NewMaxHeap[int]()
	if !h.IsEmpty() {
		t.Error("new heap should be empty")
	}
	_, ok := h.Pop()
	if ok {
		t.Error("pop on empty should return false")
	}
}

func TestHeapSortStrings(t *testing.T) {
	arr := []string{"banana", "apple", "cherry"}
	HeapSort(arr)
	if arr[0] != "apple" || arr[1] != "banana" || arr[2] != "cherry" {
		t.Errorf("not sorted: %v", arr)
	}
}
