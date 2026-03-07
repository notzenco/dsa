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

func TestSLLIsPalindrome(t *testing.T) {
	l1 := SLLFromSlice([]int{1, 2, 3, 2, 1})
	if !l1.IsPalindrome() {
		t.Error("should be palindrome")
	}

	l2 := SLLFromSlice([]int{1, 2, 3, 4})
	if l2.IsPalindrome() {
		t.Error("should not be palindrome")
	}

	l3 := SLLFromSlice([]int{1})
	if !l3.IsPalindrome() {
		t.Error("single element should be palindrome")
	}

	l4 := NewSinglyLinkedList[int]()
	if !l4.IsPalindrome() {
		t.Error("empty list should be palindrome")
	}
}

func TestSLLReverseBetween(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})
	l.ReverseBetween(1, 3)

	slice := l.ToSlice()
	expected := []int{1, 4, 3, 2, 5}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestSLLReverseBetweenInvalid(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3})
	l.ReverseBetween(2, 1) // left >= right, no-op
	slice := l.ToSlice()
	if slice[0] != 1 || slice[1] != 2 || slice[2] != 3 {
		t.Errorf("should be unchanged: %v", slice)
	}
}

func TestSLLGetCycleStart(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})
	if l.GetCycleStart() != nil {
		t.Error("should return nil for non-cyclic list")
	}

	// Create a cycle: 5 -> 3
	node3 := l.Head().Next.Next
	l.Tail().Next = node3
	start := l.GetCycleStart()
	if start == nil || start.Value != 3 {
		t.Errorf("expected cycle start at value 3")
	}
	// Remove cycle to avoid infinite loops in other tests
	l.Tail().Next = nil
}

func TestSortSinglyLinkedList(t *testing.T) {
	l := SLLFromSlice([]int{4, 2, 1, 3, 5})
	sorted := SortSinglyLinkedList(l.Head())

	var result []int
	for n := sorted; n != nil; n = n.Next {
		result = append(result, n.Value)
	}
	expected := []int{1, 2, 3, 4, 5}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestSortSinglyLinkedListEmpty(t *testing.T) {
	result := SortSinglyLinkedList(nil)
	if result != nil {
		t.Error("sorting nil should return nil")
	}
}

func TestMergeSortedIntLists(t *testing.T) {
	l1 := SLLFromSlice([]int{1, 3, 5})
	l2 := SLLFromSlice([]int{2, 4, 6})

	merged := MergeSortedIntLists(l1, l2)
	slice := merged.ToSlice()
	expected := []int{1, 2, 3, 4, 5, 6}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestMergeSortedIntListsEmpty(t *testing.T) {
	l1 := NewSinglyLinkedList[int]()
	l2 := SLLFromSlice([]int{1, 2, 3})

	merged := MergeSortedIntLists(l1, l2)
	if merged.Len() != 3 {
		t.Errorf("expected 3, got %d", merged.Len())
	}
}

func TestSLLRemoveAllOccurrences(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 2, 4, 2})
	count := l.RemoveAllOccurrences(2)
	if count != 3 {
		t.Errorf("expected 3 removed, got %d", count)
	}
	slice := l.ToSlice()
	for _, v := range slice {
		if v == 2 {
			t.Error("should not contain 2 after remove all")
		}
	}
	if l.Len() != 3 {
		t.Errorf("expected len 3, got %d", l.Len())
	}
}

func TestSLLRemoveAllOccurrencesHead(t *testing.T) {
	l := SLLFromSlice([]int{1, 1, 1, 2})
	count := l.RemoveAllOccurrences(1)
	if count != 3 {
		t.Errorf("expected 3, got %d", count)
	}
	if l.Head().Value != 2 {
		t.Errorf("expected head 2, got %d", l.Head().Value)
	}
}

func TestSLLRotate(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3, 4, 5})
	l.Rotate(2)

	slice := l.ToSlice()
	expected := []int{4, 5, 1, 2, 3}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestSLLRotateFullCircle(t *testing.T) {
	l := SLLFromSlice([]int{1, 2, 3})
	l.Rotate(3) // full rotation, should be unchanged
	slice := l.ToSlice()
	if slice[0] != 1 || slice[1] != 2 || slice[2] != 3 {
		t.Errorf("expected unchanged after full rotation: %v", slice)
	}
}

func TestPartitionIntsSLL(t *testing.T) {
	l := SLLFromSlice([]int{3, 5, 8, 5, 10, 2, 1})
	PartitionIntsSLL(l, 5)

	slice := l.ToSlice()
	// All values < 5 should come first
	pivotIdx := -1
	for i, v := range slice {
		if v >= 5 && pivotIdx == -1 {
			pivotIdx = i
		}
		if pivotIdx >= 0 && v < 5 {
			t.Errorf("value %d at index %d should be before pivot boundary", v, i)
		}
	}
}

func TestSLLGet(t *testing.T) {
	l := SLLFromSlice([]int{10, 20, 30})
	val, ok := l.Get(1)
	if !ok || val != 20 {
		t.Errorf("expected 20, got %d", val)
	}

	_, ok = l.Get(5)
	if ok {
		t.Error("out of bounds should fail")
	}
}

func TestSLLPopEmpty(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	_, ok := l.PopFirst()
	if ok {
		t.Error("pop from empty should fail")
	}
	_, ok = l.PopLast()
	if ok {
		t.Error("pop from empty should fail")
	}
}
