package datastructures

import "testing"

func TestHashTableChaining(t *testing.T) {
	ht := NewHashTableChaining[string, int]()

	ht.Put("one", 1)
	ht.Put("two", 2)
	ht.Put("three", 3)

	if ht.Size() != 3 {
		t.Errorf("expected size 3, got %d", ht.Size())
	}

	val, ok := ht.Get("two")
	if !ok || val != 2 {
		t.Errorf("expected 2, got %d", val)
	}

	_, ok = ht.Get("four")
	if ok {
		t.Error("should not find 'four'")
	}

	ht.Put("two", 22)
	val, _ = ht.Get("two")
	if val != 22 {
		t.Errorf("expected 22, got %d", val)
	}
}

func TestHashTableChainingDelete(t *testing.T) {
	ht := NewHashTableChaining[string, int]()
	ht.Put("a", 1)
	ht.Put("b", 2)

	if !ht.Delete("a") {
		t.Error("delete should succeed")
	}
	if ht.Contains("a") {
		t.Error("should not contain 'a' after delete")
	}
	if !ht.Contains("b") {
		t.Error("should still contain 'b'")
	}
	if ht.Delete("nonexistent") {
		t.Error("delete nonexistent should return false")
	}
}

func TestHashTableChainingResize(t *testing.T) {
	ht := NewHashTableChaining[int, int]()
	for i := 0; i < 100; i++ {
		ht.Put(i, i*10)
	}

	if ht.Size() != 100 {
		t.Errorf("expected size 100, got %d", ht.Size())
	}

	for i := 0; i < 100; i++ {
		val, ok := ht.Get(i)
		if !ok || val != i*10 {
			t.Errorf("key %d: expected %d, got %d", i, i*10, val)
		}
	}
}

func TestHashTableChainingKeysValues(t *testing.T) {
	ht := NewHashTableChaining[string, int]()
	ht.Put("a", 1)
	ht.Put("b", 2)

	keys := ht.Keys()
	if len(keys) != 2 {
		t.Errorf("expected 2 keys, got %d", len(keys))
	}

	values := ht.Values()
	if len(values) != 2 {
		t.Errorf("expected 2 values, got %d", len(values))
	}
}

func TestHashTableOpenAddressing(t *testing.T) {
	ht := NewHashTableOpenAddressing[string, int]()

	ht.Put("one", 1)
	ht.Put("two", 2)
	ht.Put("three", 3)

	if ht.Size() != 3 {
		t.Errorf("expected size 3, got %d", ht.Size())
	}

	val, ok := ht.Get("two")
	if !ok || val != 2 {
		t.Errorf("expected 2, got %d", val)
	}

	ht.Put("two", 22)
	val, _ = ht.Get("two")
	if val != 22 {
		t.Errorf("expected 22, got %d", val)
	}
}

func TestHashTableOpenAddressingDelete(t *testing.T) {
	ht := NewHashTableOpenAddressing[string, int]()
	ht.Put("a", 1)
	ht.Put("b", 2)
	ht.Put("c", 3)

	ht.Delete("b")
	if ht.Contains("b") {
		t.Error("should not contain 'b' after delete")
	}
	if !ht.Contains("a") || !ht.Contains("c") {
		t.Error("should still contain 'a' and 'c'")
	}
}

func TestHashTableOpenAddressingResize(t *testing.T) {
	ht := NewHashTableOpenAddressing[int, int]()
	for i := 0; i < 50; i++ {
		ht.Put(i, i*10)
	}

	for i := 0; i < 50; i++ {
		val, ok := ht.Get(i)
		if !ok || val != i*10 {
			t.Errorf("key %d: expected %d, got %d", i, i*10, val)
		}
	}
}

func TestHashSet(t *testing.T) {
	s := NewHashSet[int]()

	s.Add(1)
	s.Add(2)
	s.Add(3)
	s.Add(2) // duplicate

	if s.Size() != 3 {
		t.Errorf("expected size 3, got %d", s.Size())
	}

	if !s.Contains(2) {
		t.Error("should contain 2")
	}
	if s.Contains(99) {
		t.Error("should not contain 99")
	}

	s.Remove(2)
	if s.Contains(2) {
		t.Error("should not contain 2 after remove")
	}
}
