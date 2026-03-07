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

func TestHashSetUnion(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	a.Add(3)
	b.Add(3)
	b.Add(4)
	b.Add(5)

	u := a.Union(b)
	if u.Size() != 5 {
		t.Errorf("expected union size 5, got %d", u.Size())
	}
	for i := 1; i <= 5; i++ {
		if !u.Contains(i) {
			t.Errorf("union should contain %d", i)
		}
	}
}

func TestHashSetIntersection(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	a.Add(3)
	b.Add(2)
	b.Add(3)
	b.Add(4)

	inter := a.Intersection(b)
	if inter.Size() != 2 {
		t.Errorf("expected intersection size 2, got %d", inter.Size())
	}
	if !inter.Contains(2) || !inter.Contains(3) {
		t.Error("intersection should contain 2 and 3")
	}
}

func TestHashSetDifference(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	a.Add(3)
	b.Add(2)
	b.Add(3)
	b.Add(4)

	diff := a.Difference(b)
	if diff.Size() != 1 {
		t.Errorf("expected difference size 1, got %d", diff.Size())
	}
	if !diff.Contains(1) {
		t.Error("difference should contain 1")
	}
}

func TestHashSetSymmetricDifference(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	a.Add(3)
	b.Add(2)
	b.Add(3)
	b.Add(4)

	sd := a.SymmetricDifference(b)
	if sd.Size() != 2 {
		t.Errorf("expected symmetric difference size 2, got %d", sd.Size())
	}
	if !sd.Contains(1) || !sd.Contains(4) {
		t.Error("symmetric difference should contain 1 and 4")
	}
}

func TestHashSetSubsetSuperset(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	b.Add(1)
	b.Add(2)
	b.Add(3)

	if !a.IsSubset(b) {
		t.Error("a should be a subset of b")
	}
	if a.IsSuperset(b) {
		t.Error("a should not be a superset of b")
	}
	if !b.IsSuperset(a) {
		t.Error("b should be a superset of a")
	}
}

func TestHashSetEquals(t *testing.T) {
	a := NewHashSet[int]()
	b := NewHashSet[int]()
	a.Add(1)
	a.Add(2)
	b.Add(2)
	b.Add(1)

	if !a.Equals(b) {
		t.Error("sets should be equal")
	}

	b.Add(3)
	if a.Equals(b) {
		t.Error("sets should not be equal")
	}
}

func TestHashSetItems(t *testing.T) {
	s := NewHashSet[string]()
	s.Add("a")
	s.Add("b")
	s.Add("c")

	items := s.Items()
	if len(items) != 3 {
		t.Errorf("expected 3 items, got %d", len(items))
	}
}

func TestHashSetClear(t *testing.T) {
	s := NewHashSet[int]()
	s.Add(1)
	s.Add(2)
	s.Clear()

	if !s.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestHashTableChainingLoadFactor(t *testing.T) {
	ht := NewHashTableChaining[int, int]()
	ht.Put(1, 10)
	ht.Put(2, 20)

	lf := ht.LoadFactor()
	if lf <= 0 {
		t.Errorf("load factor should be > 0, got %f", lf)
	}
}

func TestHashTableChainingEntries(t *testing.T) {
	ht := NewHashTableChaining[string, int]()
	ht.Put("a", 1)
	ht.Put("b", 2)

	entries := ht.Entries()
	if len(entries) != 2 {
		t.Errorf("expected 2 entries, got %d", len(entries))
	}
}

func TestHashTableChainingGetOrDefault(t *testing.T) {
	ht := NewHashTableChaining[string, int]()
	ht.Put("a", 1)

	val := ht.GetOrDefault("a", 99)
	if val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	val = ht.GetOrDefault("b", 99)
	if val != 99 {
		t.Errorf("expected default 99, got %d", val)
	}
}

func TestHashTableChainingPutIfAbsent(t *testing.T) {
	ht := NewHashTableChaining[string, int]()
	ht.Put("a", 1)

	if ht.PutIfAbsent("a", 99) {
		t.Error("should return false for existing key")
	}
	val, _ := ht.Get("a")
	if val != 1 {
		t.Errorf("value should remain 1, got %d", val)
	}

	if !ht.PutIfAbsent("b", 2) {
		t.Error("should return true for new key")
	}
}

func TestHashTableChainingBucketCount(t *testing.T) {
	ht := NewHashTableChaining[int, int]()
	ht.Put(1, 10)
	ht.Put(2, 20)

	bc := ht.BucketCount()
	if bc < 1 {
		t.Errorf("bucket count should be >= 1, got %d", bc)
	}
}

func TestHashTableChainingMaxBucketSize(t *testing.T) {
	ht := NewHashTableChaining[int, int]()
	ht.Put(1, 10)

	mbs := ht.MaxBucketSize()
	if mbs < 1 {
		t.Errorf("max bucket size should be >= 1, got %d", mbs)
	}
}

func TestHashTableChainingClear(t *testing.T) {
	ht := NewHashTableChaining[int, int]()
	ht.Put(1, 10)
	ht.Put(2, 20)
	ht.Clear()

	if !ht.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestHashTableOpenAddressingLoadFactor(t *testing.T) {
	ht := NewHashTableOpenAddressing[int, int]()
	ht.Put(1, 10)
	ht.Put(2, 20)

	lf := ht.LoadFactor()
	if lf <= 0 {
		t.Errorf("load factor should be > 0, got %f", lf)
	}
}

func TestHashTableOpenAddressingKeysValues(t *testing.T) {
	ht := NewHashTableOpenAddressing[string, int]()
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

func TestHashTableOpenAddressingClear(t *testing.T) {
	ht := NewHashTableOpenAddressing[int, int]()
	ht.Put(1, 10)
	ht.Put(2, 20)
	ht.Clear()

	if !ht.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestHashTableOpenAddressingDeleteAndReinsert(t *testing.T) {
	ht := NewHashTableOpenAddressing[string, int]()
	ht.Put("a", 1)
	ht.Put("b", 2)
	ht.Delete("a")
	ht.Put("a", 10)

	val, ok := ht.Get("a")
	if !ok || val != 10 {
		t.Errorf("expected 10 after reinsert, got %d", val)
	}
}
