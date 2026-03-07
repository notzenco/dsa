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
