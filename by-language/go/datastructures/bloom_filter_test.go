package datastructures

import "testing"

func TestBloomFilter(t *testing.T) {
	bf := NewBloomFilter(100, 0.01)

	bf.Add("hello")
	bf.Add("world")
	bf.Add("test")

	if !bf.Contains("hello") {
		t.Error("should contain 'hello'")
	}
	if !bf.Contains("world") {
		t.Error("should contain 'world'")
	}

	// May have false positives, but no false negatives
	if bf.Count() != 3 {
		t.Errorf("expected count 3, got %d", bf.Count())
	}
}

func TestBloomFilterFalsePositiveRate(t *testing.T) {
	bf := NewBloomFilter(1000, 0.01)

	for i := 0; i < 1000; i++ {
		bf.Add(string(rune(i)))
	}

	fpr := bf.FalsePositiveRate()
	if fpr > 0.05 { // Allow some margin
		t.Errorf("false positive rate too high: %f", fpr)
	}
}

func TestCountingBloomFilter(t *testing.T) {
	cbf := NewCountingBloomFilter(100, 0.01)

	cbf.Add("hello")
	cbf.Add("world")

	if !cbf.Contains("hello") {
		t.Error("should contain 'hello'")
	}

	cbf.Remove("hello")
	if cbf.Contains("hello") {
		t.Error("should not contain 'hello' after remove")
	}

	if !cbf.Contains("world") {
		t.Error("should still contain 'world'")
	}
}

func TestCountingBloomFilterRemoveNonExistent(t *testing.T) {
	cbf := NewCountingBloomFilter(100, 0.01)
	if cbf.Remove("nonexistent") {
		t.Error("remove nonexistent should return false")
	}
}

func TestScalableBloomFilter(t *testing.T) {
	sbf := NewScalableBloomFilter(10, 0.01)

	for i := 0; i < 100; i++ {
		sbf.Add(string(rune('A' + i)))
	}

	if sbf.Count() != 100 {
		t.Errorf("expected count 100, got %d", sbf.Count())
	}

	if sbf.NumFilters() < 2 {
		t.Errorf("expected multiple filters, got %d", sbf.NumFilters())
	}

	// Items added should be found
	for i := 0; i < 100; i++ {
		if !sbf.Contains(string(rune('A' + i))) {
			t.Errorf("should contain item %d", i)
		}
	}
}

func TestScalableBloomFilterFPRate(t *testing.T) {
	sbf := NewScalableBloomFilter(100, 0.01)
	for i := 0; i < 50; i++ {
		sbf.Add(string(rune(i)))
	}
	fpr := sbf.FalsePositiveRate()
	if fpr > 0.1 {
		t.Errorf("false positive rate too high: %f", fpr)
	}
}

func TestBloomFilterUnion(t *testing.T) {
	a := NewBloomFilterWithSize(100, 3)
	b := NewBloomFilterWithSize(100, 3)

	a.Add("hello")
	a.Add("world")
	b.Add("foo")
	b.Add("bar")

	u := BloomFilterUnion(a, b)
	if u == nil {
		t.Fatal("union should not be nil")
	}

	if !u.Contains("hello") || !u.Contains("world") || !u.Contains("foo") || !u.Contains("bar") {
		t.Error("union should contain all items from both filters")
	}
}

func TestBloomFilterUnionMismatch(t *testing.T) {
	a := NewBloomFilterWithSize(100, 3)
	b := NewBloomFilterWithSize(200, 3)

	result := BloomFilterUnion(a, b)
	if result != nil {
		t.Error("union of different-sized filters should return nil")
	}
}

func TestBloomFilterIntersection(t *testing.T) {
	a := NewBloomFilterWithSize(100, 3)
	b := NewBloomFilterWithSize(100, 3)

	a.Add("hello")
	a.Add("shared")
	b.Add("world")
	b.Add("shared")

	inter := BloomFilterIntersection(a, b)
	if inter == nil {
		t.Fatal("intersection should not be nil")
	}

	if !inter.Contains("shared") {
		t.Error("intersection should contain 'shared'")
	}
}

func TestBloomFilterIntersectionMismatch(t *testing.T) {
	a := NewBloomFilterWithSize(100, 3)
	b := NewBloomFilterWithSize(100, 4) // different numHash

	result := BloomFilterIntersection(a, b)
	if result != nil {
		t.Error("intersection of different-hash filters should return nil")
	}
}

func TestBloomFilterEstimateCount(t *testing.T) {
	bf := NewBloomFilter(1000, 0.01)
	for i := 0; i < 100; i++ {
		bf.Add(string(rune(i + 256)))
	}

	estimate := bf.EstimateCount()
	if estimate < 50 || estimate > 200 {
		t.Errorf("estimate should be roughly 100, got %f", estimate)
	}
}

func TestBloomFilterClear(t *testing.T) {
	bf := NewBloomFilter(100, 0.01)
	bf.Add("hello")
	bf.Add("world")

	bf.Clear()
	if bf.Count() != 0 {
		t.Errorf("expected count 0 after clear, got %d", bf.Count())
	}
	if bf.Contains("hello") {
		t.Error("should not contain 'hello' after clear")
	}
}

func TestBloomFilterSize(t *testing.T) {
	bf := NewBloomFilter(100, 0.01)
	if bf.Size() == 0 {
		t.Error("size should be > 0")
	}
}

func TestBloomFilterNumHashFunctions(t *testing.T) {
	bf := NewBloomFilter(100, 0.01)
	if bf.NumHashFunctions() == 0 {
		t.Error("numHash should be > 0")
	}
}

func TestCountingBloomFilterFPRate(t *testing.T) {
	cbf := NewCountingBloomFilter(1000, 0.01)
	for i := 0; i < 100; i++ {
		cbf.Add(string(rune(i + 256)))
	}
	fpr := cbf.FalsePositiveRate()
	if fpr > 0.1 {
		t.Errorf("false positive rate too high: %f", fpr)
	}
}

func TestCountingBloomFilterCount(t *testing.T) {
	cbf := NewCountingBloomFilter(100, 0.01)
	cbf.Add("a")
	cbf.Add("b")
	cbf.Add("c")

	if cbf.Count() != 3 {
		t.Errorf("expected 3, got %d", cbf.Count())
	}

	cbf.Remove("b")
	if cbf.Count() != 2 {
		t.Errorf("expected 2 after remove, got %d", cbf.Count())
	}
}

func TestBloomFilterEstimateCountEmpty(t *testing.T) {
	bf := NewBloomFilter(100, 0.01)
	estimate := bf.EstimateCount()
	if estimate != 0 {
		t.Errorf("expected 0 for empty filter, got %f", estimate)
	}
}
