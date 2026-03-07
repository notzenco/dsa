package datastructures

import "testing"

func TestDynamicArray(t *testing.T) {
	a := NewDynamicArray[int]()

	if !a.IsEmpty() {
		t.Error("new array should be empty")
	}
	if a.Capacity() != defaultCapacity {
		t.Errorf("expected capacity %d, got %d", defaultCapacity, a.Capacity())
	}

	for i := 0; i < 10; i++ {
		a.Append(i)
	}

	if a.Size() != 10 {
		t.Errorf("expected size 10, got %d", a.Size())
	}
	if a.Capacity() <= 10 {
		t.Error("capacity should have grown")
	}

	val, ok := a.Get(5)
	if !ok || val != 5 {
		t.Errorf("expected 5, got %d", val)
	}
}

func TestDynamicArrayInsert(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 3, 5})
	a.Insert(1, 2)
	a.Insert(3, 4)

	slice := a.ToSlice()
	for i, v := range []int{1, 2, 3, 4, 5} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDynamicArrayPop(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 2, 3})

	val, ok := a.Pop()
	if !ok || val != 3 {
		t.Errorf("expected 3, got %d", val)
	}
	if a.Size() != 2 {
		t.Errorf("expected size 2, got %d", a.Size())
	}
}

func TestDynamicArrayRemove(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 2, 3, 4, 5})

	if !a.Remove(3) {
		t.Error("remove should succeed")
	}

	slice := a.ToSlice()
	for i, v := range []int{1, 2, 4, 5} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}

	if a.Remove(99) {
		t.Error("remove nonexistent should fail")
	}
}

func TestDynamicArrayFind(t *testing.T) {
	a := DynamicArrayFromSlice([]int{10, 20, 30})

	if a.Find(20) != 1 {
		t.Error("expected index 1")
	}
	if a.Find(99) != -1 {
		t.Error("expected -1")
	}
	if !a.Contains(30) {
		t.Error("should contain 30")
	}
}

func TestDynamicArrayReverse(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 2, 3, 4})
	a.Reverse()

	slice := a.ToSlice()
	for i, v := range []int{4, 3, 2, 1} {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestDynamicArrayShrink(t *testing.T) {
	a := NewDynamicArray[int]()
	for i := 0; i < 100; i++ {
		a.Append(i)
	}
	capBefore := a.Capacity()

	for i := 0; i < 90; i++ {
		a.Pop()
	}

	if a.Capacity() >= capBefore {
		t.Error("capacity should have shrunk")
	}
}

func TestDynamicArrayClear(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 2, 3})
	a.Clear()
	if !a.IsEmpty() {
		t.Error("cleared array should be empty")
	}
}

func TestDynamicArraySet(t *testing.T) {
	a := DynamicArrayFromSlice([]int{1, 2, 3})
	a.Set(1, 20)
	val, _ := a.Get(1)
	if val != 20 {
		t.Errorf("expected 20, got %d", val)
	}
	if a.Set(-1, 0) {
		t.Error("negative index should fail")
	}
}
