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
