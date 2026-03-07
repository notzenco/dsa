package datastructures

import "testing"

func TestSinglyLinkedList(t *testing.T) {
	l := NewSinglyLinkedList[int]()
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
	expected := []int{1, 2, 3}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestSLLPrepend(t *testing.T) {
	l := NewSinglyLinkedList[int]()
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

func TestSLLInsert(t *testing.T) {
	l := SLLFromSlice([]int{1, 3, 5})

	l.Insert(1, 2)
	l.Insert(3, 4)

	slice := l.ToSlice()
	for i, v := range []int{1, 2, 3, 4, 5} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	if l.Insert(-1, 0) {
		t.Error("insert at -1 should fail")
	}
	if l.Insert(100, 0) {
		t.Error("insert at 100 should fail")
	}
}

func TestSLLPop(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})

	val, ok := l.PopFirst()
	if !ok || val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	val, ok = l.PopLast()
	if !ok || val != 5 {
		t.Errorf("expected 5, got %d", val)
	}

	val, ok = l.Pop(1)
	if !ok || val != 3 {
		t.Errorf("expected 3, got %d", val)
	}

	slice := l.ToSlice()
	for i, v := range []int{2, 4} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestSLLRemove(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 2, 4})
	if !l.Remove(2) {
		t.Error("remove should succeed")
	}
	slice := l.ToSlice()
	for i, v := range []int{1, 3, 2, 4} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	if l.Remove(99) {
		t.Error("remove nonexistent should fail")
	}
}

func TestSLLFind(t *testing.T) {
	l := SLLFromSlice([]int{10, 20, 30})
	if l.Find(20) != 1 {
		t.Error("expected index 1")
	}
	if l.Find(99) != -1 {
		t.Error("expected -1 for not found")
	}
}

func TestSLLReverse(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4})
	l.Reverse()

	slice := l.ToSlice()
	for i, v := range []int{4, 3, 2, 1} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	if l.Head().Value != 4 {
		t.Errorf("expected head 4, got %d", l.Head().Value)
	}
	if l.Tail().Value != 1 {
		t.Errorf("expected tail 1, got %d", l.Tail().Value)
	}
}

func TestSLLGetMiddle(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})
	mid, ok := l.GetMiddle()
	if !ok || mid != 3 {
		t.Errorf("expected middle 3, got %d", mid)
	}

	l2 := SLLFromSlice([]int{1, 2, 3, 4})
	mid, ok = l2.GetMiddle()
	if !ok || mid != 2 {
		t.Errorf("expected middle 2, got %d", mid)
	}
}

func TestSLLHasCycle(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3})
	if l.HasCycle() {
		t.Error("list should not have cycle")
	}
}

func TestSLLGetNthFromEnd(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})

	val, ok := l.GetNthFromEnd(1)
	if !ok || val != 5 {
		t.Errorf("expected 5, got %d", val)
	}

	val, ok = l.GetNthFromEnd(3)
	if !ok || val != 3 {
		t.Errorf("expected 3, got %d", val)
	}

	_, ok = l.GetNthFromEnd(0)
	if ok {
		t.Error("n=0 should fail")
	}

	_, ok = l.GetNthFromEnd(10)
	if ok {
		t.Error("n>len should fail")
	}
}

func TestSLLRemoveDuplicates(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 2, 4, 1, 5})
	l.RemoveDuplicates()

	slice := l.ToSlice()
	for i, v := range []int{1, 2, 3, 4, 5} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}
