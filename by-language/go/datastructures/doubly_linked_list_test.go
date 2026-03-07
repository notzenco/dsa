package datastructures

import "testing"

func TestDoublyLinkedList(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	if !l.IsEmpty() {
		t.Error("new list should be empty")
	}

	l.Append(1)
	l.Append(2)
	l.Append(3)

	if l.Len() != 3 {
		t.Errorf("expected len 3, got %d", l.Len())
	}

	slice := l.ToSlice()
	for i, v := range []int{1, 2, 3} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	rev := l.ToSliceReverse()
	for i, v := range []int{3, 2, 1} {
		if rev[i] != v {
			t.Errorf("reverse index %d: expected %d, got %d", i, v, rev[i])
		}
	}
}

func TestDLLPrepend(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	l.Prepend(3)
	l.Prepend(2)
	l.Prepend(1)

	slice := l.ToSlice()
	for i, v := range []int{1, 2, 3} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLInsert(t *testing.T) {
	l := DLLFromSlice([]int{1, 3, 5})
	l.Insert(1, 2)
	l.Insert(3, 4)

	slice := l.ToSlice()
	for i, v := range []int{1, 2, 3, 4, 5} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLInsertAfterBefore(t *testing.T) {
	l := DLLFromSlice([]int{1, 3})

	node := l.Find(1)
	l.InsertAfter(node, 2)

	node = l.Find(3)
	l.InsertBefore(node, 2) // duplicate 2 before 3

	slice := l.ToSlice()
	if l.Len() != 4 {
		t.Errorf("expected len 4, got %d", l.Len())
	}
	if slice[0] != 1 || slice[1] != 2 || slice[2] != 2 || slice[3] != 3 {
		t.Errorf("unexpected slice: %v", slice)
	}
}

func TestDLLPopFirstLast(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3, 4, 5})

	val, ok := l.PopFirst()
	if !ok || val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	val, ok = l.PopLast()
	if !ok || val != 5 {
		t.Errorf("expected 5, got %d", val)
	}

	slice := l.ToSlice()
	for i, v := range []int{2, 3, 4} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLRemoveNode(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	node := l.Find(2)
	l.RemoveNode(node)

	slice := l.ToSlice()
	for i, v := range []int{1, 3} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLMoveToFront(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	node := l.Find(3)
	l.MoveToFront(node)

	slice := l.ToSlice()
	for i, v := range []int{3, 1, 2} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLMoveToBack(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	node := l.Find(1)
	l.MoveToBack(node)

	slice := l.ToSlice()
	for i, v := range []int{2, 3, 1} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLGet(t *testing.T) {
	l := DLLFromSlice([]int{10, 20, 30, 40, 50})

	val, ok := l.Get(0)
	if !ok || val != 10 {
		t.Errorf("expected 10, got %d", val)
	}

	val, ok = l.Get(4)
	if !ok || val != 50 {
		t.Errorf("expected 50, got %d", val)
	}

	val, ok = l.Get(2)
	if !ok || val != 30 {
		t.Errorf("expected 30, got %d", val)
	}

	_, ok = l.Get(-1)
	if ok {
		t.Error("negative index should fail")
	}

	_, ok = l.Get(5)
	if ok {
		t.Error("out of bounds should fail")
	}
}

func TestDLLReverse(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3, 4, 5})
	l.Reverse()

	slice := l.ToSlice()
	expected := []int{5, 4, 3, 2, 1}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	if l.Head().Value != 5 {
		t.Errorf("expected head 5, got %d", l.Head().Value)
	}
	if l.Tail().Value != 1 {
		t.Errorf("expected tail 1, got %d", l.Tail().Value)
	}

	// Verify reverse traversal still works
	rev := l.ToSliceReverse()
	for i, v := range []int{1, 2, 3, 4, 5} {
		if rev[i] != v {
			t.Errorf("reverse index %d: expected %d, got %d", i, v, rev[i])
		}
	}
}

func TestDLLForEach(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	sum := 0
	l.ForEach(func(v int) {
		sum += v
	})
	if sum != 6 {
		t.Errorf("expected sum 6, got %d", sum)
	}
}

func TestDLLForEachReverse(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	var result []int
	l.ForEachReverse(func(v int) {
		result = append(result, v)
	})
	expected := []int{3, 2, 1}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestDLLContains(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	if !l.Contains(2) {
		t.Error("should contain 2")
	}
	if l.Contains(99) {
		t.Error("should not contain 99")
	}
}

func TestDLLRemoveAll(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3, 2, 4, 2})
	count := l.RemoveAll(2)
	if count != 3 {
		t.Errorf("expected 3 removed, got %d", count)
	}
	if l.Len() != 3 {
		t.Errorf("expected len 3, got %d", l.Len())
	}
	if l.Contains(2) {
		t.Error("should not contain 2")
	}
}

func TestDLLIsPalindrome(t *testing.T) {
	l1 := DLLFromSlice([]int{1, 2, 3, 2, 1})
	if !l1.IsPalindrome() {
		t.Error("should be palindrome")
	}

	l2 := DLLFromSlice([]int{1, 2, 3, 4})
	if l2.IsPalindrome() {
		t.Error("should not be palindrome")
	}

	l3 := DLLFromSlice([]int{1, 1})
	if !l3.IsPalindrome() {
		t.Error("should be palindrome")
	}
}

func TestDLLRotate(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3, 4, 5})
	l.Rotate(2)

	slice := l.ToSlice()
	expected := []int{4, 5, 1, 2, 3}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	// Verify prev pointers are intact
	rev := l.ToSliceReverse()
	for i, v := range []int{3, 2, 1, 5, 4} {
		if rev[i] != v {
			t.Errorf("reverse index %d: expected %d, got %d", i, v, rev[i])
		}
	}
}

func TestDLLSwap(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3, 4, 5})
	if !l.Swap(1, 3) {
		t.Error("swap should succeed")
	}
	slice := l.ToSlice()
	if slice[1] != 4 || slice[3] != 2 {
		t.Errorf("expected swapped values at 1,3: %v", slice)
	}

	if l.Swap(0, 0) {
		t.Error("swap same index should fail")
	}
	if l.Swap(-1, 2) {
		t.Error("swap invalid index should fail")
	}
}

func TestDLLInsertSorted(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	less := func(a, b int) bool { return a < b }

	l.InsertSorted(3, less)
	l.InsertSorted(1, less)
	l.InsertSorted(5, less)
	l.InsertSorted(2, less)
	l.InsertSorted(4, less)

	slice := l.ToSlice()
	expected := []int{1, 2, 3, 4, 5}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestMergeSortedDLL(t *testing.T) {
	less := func(a, b int) bool { return a < b }
	l1 := DLLFromSlice([]int{1, 3, 5})
	l2 := DLLFromSlice([]int{2, 4, 6})

	merged := MergeSortedDLL(l1, l2, less)
	slice := merged.ToSlice()
	expected := []int{1, 2, 3, 4, 5, 6}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDLLCopy(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	cp := l.Copy()

	if cp.Len() != l.Len() {
		t.Errorf("expected same length")
	}

	// Modify original, copy should be unchanged
	l.PopFirst()
	if cp.Len() != 3 {
		t.Errorf("copy should be independent, len = %d", cp.Len())
	}
}

func TestDLLClear(t *testing.T) {
	l := DLLFromSlice([]int{1, 2, 3})
	l.Clear()

	if !l.IsEmpty() {
		t.Error("should be empty after clear")
	}
	if l.Head() != nil {
		t.Error("head should be nil")
	}
	if l.Tail() != nil {
		t.Error("tail should be nil")
	}
}

func TestDLLPopEmpty(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	_, ok := l.PopFirst()
	if ok {
		t.Error("pop from empty should fail")
	}
	_, ok = l.PopLast()
	if ok {
		t.Error("pop from empty should fail")
	}
}
