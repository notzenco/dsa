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

func TestPersistentUnionFind(t *testing.T) {
	puf := NewPersistentUnionFind(5)

	v1 := puf.Union(0, 1)
	v2 := puf.Union(2, 3)

	// At version 0 (before any unions), nothing connected
	if puf.ConnectedAtVersion(0, 1, 0) {
		t.Error("0 and 1 should not be connected at version 0")
	}

	// At version 1, 0 and 1 are connected
	if !puf.ConnectedAtVersion(0, 1, v1) {
		t.Error("0 and 1 should be connected at version 1")
	}

	// At version 1, 2 and 3 are not yet connected
	if puf.ConnectedAtVersion(2, 3, v1) {
		t.Error("2 and 3 should not be connected at version 1")
	}

	// At version 2, 2 and 3 are connected
	if !puf.ConnectedAtVersion(2, 3, v2) {
		t.Error("2 and 3 should be connected at version 2")
	}

	// Current state
	if !puf.Connected(0, 1) {
		t.Error("0 and 1 should be connected")
	}
	if !puf.Connected(2, 3) {
		t.Error("2 and 3 should be connected")
	}
	if puf.Connected(0, 2) {
		t.Error("0 and 2 should not be connected")
	}
}

func TestPersistentUnionFindCurrentVersion(t *testing.T) {
	puf := NewPersistentUnionFind(4)
	if puf.CurrentVersion() != 0 {
		t.Errorf("expected version 0, got %d", puf.CurrentVersion())
	}

	puf.Union(0, 1)
	if puf.CurrentVersion() != 1 {
		t.Errorf("expected version 1, got %d", puf.CurrentVersion())
	}

	// Duplicate union shouldn't increase version
	puf.Union(0, 1)
	if puf.CurrentVersion() != 1 {
		t.Errorf("expected version still 1, got %d", puf.CurrentVersion())
	}
}

func TestFindRedundantConnection(t *testing.T) {
	edges := [][2]int{{1, 2}, {1, 3}, {2, 3}}
	result := FindRedundantConnection(edges)
	if result != [2]int{2, 3} {
		t.Errorf("expected [2, 3], got %v", result)
	}
}

func TestFindRedundantConnectionLarger(t *testing.T) {
	edges := [][2]int{{1, 2}, {2, 3}, {3, 4}, {1, 4}, {1, 5}}
	result := FindRedundantConnection(edges)
	if result != [2]int{1, 4} {
		t.Errorf("expected [1, 4], got %v", result)
	}
}

func TestUnionFindGetComponents(t *testing.T) {
	uf := NewUnionFind(6)
	uf.Union(0, 1)
	uf.Union(1, 2)
	uf.Union(3, 4)

	components := uf.GetComponents()
	if len(components) != 3 { // {0,1,2}, {3,4}, {5}
		t.Errorf("expected 3 components, got %d", len(components))
	}
}

func TestWeightedUnionFindComponents(t *testing.T) {
	wuf := NewWeightedUnionFind(4)
	wuf.Union(0, 1, 1.0)
	wuf.Union(2, 3, 2.0)

	if wuf.Components() != 2 {
		t.Errorf("expected 2 components, got %d", wuf.Components())
	}
}

func TestUnionFindAllConnected(t *testing.T) {
	uf := NewUnionFind(4)
	uf.Union(0, 1)
	uf.Union(1, 2)
	uf.Union(2, 3)

	if uf.Components() != 1 {
		t.Errorf("expected 1 component, got %d", uf.Components())
	}
	for i := 0; i < 4; i++ {
		for j := i + 1; j < 4; j++ {
			if !uf.Connected(i, j) {
				t.Errorf("%d and %d should be connected", i, j)
			}
		}
	}
}

func TestWeightedUnionFindGetWeightNotConnected(t *testing.T) {
	wuf := NewWeightedUnionFind(3)
	wuf.Union(0, 1, 5.0)

	_, ok := wuf.GetWeight(0, 2)
	if ok {
		t.Error("should not find weight between disconnected nodes")
	}
}

func TestNumIslandsSingleIsland(t *testing.T) {
	grid := [][]byte{
		{'1', '1', '1'},
		{'1', '1', '1'},
		{'1', '1', '1'},
	}
	result := NumIslands(grid)
	if result != 1 {
		t.Errorf("expected 1 island, got %d", result)
	}
}

func TestUnionFindSetSize(t *testing.T) {
	uf := NewUnionFind(5)
	uf.Union(0, 1)
	uf.Union(1, 2)

	size := uf.SetSize(0)
	if size != 3 {
		t.Errorf("expected set size 3, got %d", size)
	}

	size = uf.SetSize(3)
	if size != 1 {
		t.Errorf("expected set size 1, got %d", size)
	}
}
