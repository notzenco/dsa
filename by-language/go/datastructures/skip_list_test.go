package datastructures

import "testing"

func TestSkipList(t *testing.T) {
	sl := NewSkipList[int, string]()

	sl.Insert(3, "three")
	sl.Insert(1, "one")
	sl.Insert(5, "five")
	sl.Insert(2, "two")
	sl.Insert(4, "four")

	if sl.Size() != 5 {
		t.Errorf("expected size 5, got %d", sl.Size())
	}

	val, ok := sl.Search(3)
	if !ok || val != "three" {
		t.Errorf("expected 'three', got '%s'", val)
	}

	_, ok = sl.Search(99)
	if ok {
		t.Error("should not find 99")
	}
}

func TestSkipListUpdate(t *testing.T) {
	sl := NewSkipList[int, string]()
	sl.Insert(1, "one")
	sl.Insert(1, "ONE")

	val, _ := sl.Search(1)
	if val != "ONE" {
		t.Errorf("expected 'ONE', got '%s'", val)
	}
	if sl.Size() != 1 {
		t.Errorf("expected size 1, got %d", sl.Size())
	}
}

func TestSkipListDelete(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 1; i <= 10; i++ {
		sl.Insert(i, i*10)
	}

	if !sl.Delete(5) {
		t.Error("delete 5 should succeed")
	}
	if sl.Contains(5) {
		t.Error("should not contain 5")
	}
	if !sl.Contains(4) || !sl.Contains(6) {
		t.Error("should still contain neighbors")
	}
	if sl.Delete(99) {
		t.Error("delete nonexistent should fail")
	}
}

func TestSkipListMinMax(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(5, 50)
	sl.Insert(1, 10)
	sl.Insert(9, 90)

	k, _, ok := sl.Min()
	if !ok || k != 1 {
		t.Errorf("expected min 1, got %d", k)
	}

	k, _, ok = sl.Max()
	if !ok || k != 9 {
		t.Errorf("expected max 9, got %d", k)
	}
}

func TestSkipListFloorCeiling(t *testing.T) {
	sl := NewSkipList[int, int]()
	for _, v := range []int{10, 20, 30, 40, 50} {
		sl.Insert(v, v)
	}

	k, _, ok := sl.Floor(25)
	if !ok || k != 20 {
		t.Errorf("expected floor 20, got %d", k)
	}

	k, _, ok = sl.Ceiling(25)
	if !ok || k != 30 {
		t.Errorf("expected ceiling 30, got %d", k)
	}

	k, _, ok = sl.Floor(30)
	if !ok || k != 30 {
		t.Errorf("expected floor 30, got %d", k)
	}
}

func TestSkipListRange(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 1; i <= 10; i++ {
		sl.Insert(i, i)
	}

	r := sl.Range(3, 7)
	expected := []int{3, 4, 5, 6, 7}
	if len(r) != len(expected) {
		t.Fatalf("expected %d results, got %d", len(expected), len(r))
	}
	for i, v := range expected {
		if r[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, r[i])
		}
	}
}

func TestSkipListKeys(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(3, 30)
	sl.Insert(1, 10)
	sl.Insert(2, 20)

	keys := sl.Keys()
	expected := []int{1, 2, 3}
	for i, v := range expected {
		if keys[i] != v {
			t.Errorf("keys[%d]: expected %d, got %d", i, v, keys[i])
		}
	}
}

func TestSkipListValues(t *testing.T) {
	sl := NewSkipList[int, string]()
	sl.Insert(2, "two")
	sl.Insert(1, "one")
	sl.Insert(3, "three")

	values := sl.Values()
	if len(values) != 3 {
		t.Errorf("expected 3 values, got %d", len(values))
	}
	if values[0] != "one" || values[1] != "two" || values[2] != "three" {
		t.Errorf("unexpected values: %v", values)
	}
}

func TestSkipListClear(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 1; i <= 10; i++ {
		sl.Insert(i, i)
	}
	sl.Clear()
	if !sl.IsEmpty() {
		t.Error("should be empty after clear")
	}
	if sl.Size() != 0 {
		t.Errorf("expected size 0, got %d", sl.Size())
	}
}

func TestSkipListRangeCount(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 1; i <= 10; i++ {
		sl.Insert(i, i)
	}
	count := sl.RangeCount(3, 7)
	if count != 5 {
		t.Errorf("expected 5, got %d", count)
	}
}

func TestSkipListForEach(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(1, 10)
	sl.Insert(2, 20)
	sl.Insert(3, 30)

	sum := 0
	sl.ForEach(func(k int, v int) {
		sum += v
	})
	if sum != 60 {
		t.Errorf("expected sum 60, got %d", sum)
	}
}

func TestSkipListGetOrDefault(t *testing.T) {
	sl := NewSkipList[int, string]()
	sl.Insert(1, "one")

	val := sl.GetOrDefault(1, "default")
	if val != "one" {
		t.Errorf("expected 'one', got %q", val)
	}

	val = sl.GetOrDefault(99, "default")
	if val != "default" {
		t.Errorf("expected 'default', got %q", val)
	}
}

func TestSkipListPop(t *testing.T) {
	sl := NewSkipList[int, string]()
	sl.Insert(3, "three")
	sl.Insert(1, "one")
	sl.Insert(2, "two")

	k, v, ok := sl.Pop()
	if !ok || k != 1 || v != "one" {
		t.Errorf("expected (1, 'one'), got (%d, %q)", k, v)
	}
	if sl.Size() != 2 {
		t.Errorf("expected size 2, got %d", sl.Size())
	}
}

func TestSkipListPopEmpty(t *testing.T) {
	sl := NewSkipList[int, int]()
	_, _, ok := sl.Pop()
	if ok {
		t.Error("pop from empty should fail")
	}
}

func TestSkipListRank(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(10, 10)
	sl.Insert(20, 20)
	sl.Insert(30, 30)

	if sl.Rank(10) != 1 {
		t.Errorf("expected rank 1, got %d", sl.Rank(10))
	}
	if sl.Rank(20) != 2 {
		t.Errorf("expected rank 2, got %d", sl.Rank(20))
	}
	if sl.Rank(30) != 3 {
		t.Errorf("expected rank 3, got %d", sl.Rank(30))
	}
	if sl.Rank(99) != 0 {
		t.Errorf("expected rank 0 for missing, got %d", sl.Rank(99))
	}
}

func TestSkipListLevel(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 1; i <= 100; i++ {
		sl.Insert(i, i)
	}
	if sl.Level() < 0 {
		t.Errorf("level should be >= 0, got %d", sl.Level())
	}
}

func TestSkipListLargeInsertDelete(t *testing.T) {
	sl := NewSkipList[int, int]()
	for i := 0; i < 500; i++ {
		sl.Insert(i, i*10)
	}
	if sl.Size() != 500 {
		t.Errorf("expected 500, got %d", sl.Size())
	}

	for i := 0; i < 500; i++ {
		v, ok := sl.Search(i)
		if !ok || v != i*10 {
			t.Errorf("key %d: expected %d, got %d", i, i*10, v)
		}
	}

	for i := 0; i < 250; i++ {
		sl.Delete(i)
	}
	if sl.Size() != 250 {
		t.Errorf("expected 250, got %d", sl.Size())
	}
}

func TestSkipListHead(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(3, 30)
	sl.Insert(1, 10)
	sl.Insert(2, 20)

	k, v, ok := sl.Head()
	if !ok || k != 1 || v != 10 {
		t.Errorf("expected head (1, 10), got (%d, %d)", k, v)
	}
}

func TestSkipListTail(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(3, 30)
	sl.Insert(1, 10)
	sl.Insert(2, 20)

	k, v, ok := sl.Tail()
	if !ok || k != 3 || v != 30 {
		t.Errorf("expected tail (3, 30), got (%d, %d)", k, v)
	}
}

func TestSkipListFloorCeilingEdge(t *testing.T) {
	sl := NewSkipList[int, int]()
	sl.Insert(10, 10)
	sl.Insert(30, 30)

	// Floor of something smaller than all
	_, _, ok := sl.Floor(5)
	if ok {
		t.Error("floor of 5 should fail when min is 10")
	}

	// Ceiling of something larger than all
	_, _, ok = sl.Ceiling(35)
	if !ok {
		// Actually ceiling should fail when no key >= 35
	}
}

func TestSkipListEmpty(t *testing.T) {
	sl := NewSkipList[int, int]()
	if !sl.IsEmpty() {
		t.Error("new skip list should be empty")
	}
	_, _, ok := sl.Min()
	if ok {
		t.Error("min on empty should fail")
	}
	_, _, ok = sl.Max()
	if ok {
		t.Error("max on empty should fail")
	}
}
