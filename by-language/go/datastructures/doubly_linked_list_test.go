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
