package datastructures

import "testing"

func TestUnionFind(t *testing.T) {
	uf := NewUnionFind(10)

	if uf.Components() != 10 {
		t.Errorf("expected 10 components, got %d", uf.Components())
	}

	uf.Union(0, 1)
	uf.Union(2, 3)
	uf.Union(0, 3)

	if !uf.Connected(0, 3) {
		t.Error("0 and 3 should be connected")
	}
	if !uf.Connected(1, 2) {
		t.Error("1 and 2 should be connected")
	}
	if uf.Connected(0, 4) {
		t.Error("0 and 4 should not be connected")
	}

	if uf.Components() != 7 {
		t.Errorf("expected 7 components, got %d", uf.Components())
	}
}

func TestUnionFindComponentSize(t *testing.T) {
	uf := NewUnionFind(5)
	uf.Union(0, 1)
	uf.Union(0, 2)

	size := uf.ComponentSize(0)
	if size != 3 {
		t.Errorf("expected component size 3, got %d", size)
	}

	size = uf.ComponentSize(3)
	if size != 1 {
		t.Errorf("expected component size 1, got %d", size)
	}
}

func TestUnionFindDuplicate(t *testing.T) {
	uf := NewUnionFind(3)

	if !uf.Union(0, 1) {
		t.Error("first union should return true")
	}
	if uf.Union(0, 1) {
		t.Error("duplicate union should return false")
	}
}

func TestWeightedUnionFind(t *testing.T) {
	wuf := NewWeightedUnionFind(5)

	wuf.Union(0, 1, 2.0) // 0 -> 1 with weight 2.0
	wuf.Union(1, 2, 3.0) // 1 -> 2 with weight 3.0

	if !wuf.Connected(0, 2) {
		t.Error("0 and 2 should be connected")
	}

	w, ok := wuf.GetWeight(0, 2)
	if !ok {
		t.Error("should find weight between 0 and 2")
	}
	if w < 4.9 || w > 5.1 { // should be 5.0
		t.Errorf("expected weight ~5.0, got %f", w)
	}
}

func TestNumIslands(t *testing.T) {
	grid := [][]byte{
		{'1', '1', '0', '0', '0'},
		{'1', '1', '0', '0', '0'},
		{'0', '0', '1', '0', '0'},
		{'0', '0', '0', '1', '1'},
	}

	result := NumIslands(grid)
	if result != 3 {
		t.Errorf("expected 3 islands, got %d", result)
	}
}

func TestNumIslandsEmpty(t *testing.T) {
	result := NumIslands(nil)
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}

	grid := [][]byte{
		{'0', '0'},
		{'0', '0'},
	}
	result = NumIslands(grid)
	if result != 0 {
		t.Errorf("expected 0, got %d", result)
	}
}
