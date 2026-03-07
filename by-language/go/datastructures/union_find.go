package datastructures

// UnionFind implements disjoint set with path compression and union by rank.
type UnionFind struct {
	parent []int
	rank   []int
	count  int // number of components
}

func NewUnionFind(n int) *UnionFind {
	parent := make([]int, n)
	rank := make([]int, n)
	for i := range parent {
		parent[i] = i
	}
	return &UnionFind{parent: parent, rank: rank, count: n}
}

func (uf *UnionFind) Find(x int) int {
	if uf.parent[x] != x {
		uf.parent[x] = uf.Find(uf.parent[x]) // path compression
	}
	return uf.parent[x]
}

func (uf *UnionFind) Union(x, y int) bool {
	rx, ry := uf.Find(x), uf.Find(y)
	if rx == ry {
		return false
	}

	// union by rank
	if uf.rank[rx] < uf.rank[ry] {
		uf.parent[rx] = ry
	} else if uf.rank[rx] > uf.rank[ry] {
		uf.parent[ry] = rx
	} else {
		uf.parent[ry] = rx
		uf.rank[rx]++
	}
	uf.count--
	return true
}

func (uf *UnionFind) Connected(x, y int) bool {
	return uf.Find(x) == uf.Find(y)
}

func (uf *UnionFind) Components() int {
	return uf.count
}

func (uf *UnionFind) Size() int {
	return len(uf.parent)
}

func (uf *UnionFind) ComponentSize(x int) int {
	root := uf.Find(x)
	count := 0
	for i := range uf.parent {
		if uf.Find(i) == root {
			count++
		}
	}
	return count
}

// WeightedUnionFind supports weighted edges between elements.
type WeightedUnionFind struct {
	parent []int
	rank   []int
	weight []float64
	count  int
}

func NewWeightedUnionFind(n int) *WeightedUnionFind {
	parent := make([]int, n)
	rank := make([]int, n)
	weight := make([]float64, n)
	for i := range parent {
		parent[i] = i
	}
	return &WeightedUnionFind{parent: parent, rank: rank, weight: weight, count: n}
}

func (wuf *WeightedUnionFind) Find(x int) (int, float64) {
	if wuf.parent[x] != x {
		root, w := wuf.Find(wuf.parent[x])
		wuf.parent[x] = root
		wuf.weight[x] += w
	}
	return wuf.parent[x], wuf.weight[x]
}

func (wuf *WeightedUnionFind) Union(x, y int, w float64) bool {
	rx, wx := wuf.Find(x)
	ry, wy := wuf.Find(y)
	if rx == ry {
		return false
	}

	if wuf.rank[rx] < wuf.rank[ry] {
		wuf.parent[rx] = ry
		wuf.weight[rx] = wy - wx + w
	} else if wuf.rank[rx] > wuf.rank[ry] {
		wuf.parent[ry] = rx
		wuf.weight[ry] = wx - wy - w
	} else {
		wuf.parent[ry] = rx
		wuf.weight[ry] = wx - wy - w
		wuf.rank[rx]++
	}
	wuf.count--
	return true
}

func (wuf *WeightedUnionFind) Connected(x, y int) bool {
	rx, _ := wuf.Find(x)
	ry, _ := wuf.Find(y)
	return rx == ry
}

func (wuf *WeightedUnionFind) GetWeight(x, y int) (float64, bool) {
	rx, wx := wuf.Find(x)
	ry, wy := wuf.Find(y)
	if rx != ry {
		return 0, false
	}
	return wx - wy, true
}

func (wuf *WeightedUnionFind) Components() int {
	return wuf.count
}

// PersistentUnionFind supports querying past states via version history.
type PersistentUnionFind struct {
	parent  []int
	rank    []int
	version int
	history []persistentEntry // (version, index, oldParent, oldRank)
}

type persistentEntry struct {
	version   int
	index     int
	oldParent int
	oldRank   int
}

func NewPersistentUnionFind(n int) *PersistentUnionFind {
	parent := make([]int, n)
	rank := make([]int, n)
	for i := range parent {
		parent[i] = i
	}
	return &PersistentUnionFind{parent: parent, rank: rank}
}

func (puf *PersistentUnionFind) CurrentVersion() int {
	return puf.version
}

func (puf *PersistentUnionFind) getParentAtVersion(x, version int) int {
	parent := puf.parent[x]
	for i := len(puf.history) - 1; i >= 0; i-- {
		e := puf.history[i]
		if e.version > version && e.index == x {
			parent = e.oldParent
		}
	}
	return parent
}

func (puf *PersistentUnionFind) FindAtVersion(x, version int) int {
	parent := puf.getParentAtVersion(x, version)
	if parent == x {
		return x
	}
	return puf.FindAtVersion(parent, version)
}

func (puf *PersistentUnionFind) Find(x int) int {
	return puf.FindAtVersion(x, puf.version)
}

func (puf *PersistentUnionFind) Union(x, y int) int {
	rx := puf.Find(x)
	ry := puf.Find(y)
	if rx == ry {
		return puf.version
	}
	puf.version++
	if puf.rank[rx] < puf.rank[ry] {
		rx, ry = ry, rx
	}
	puf.history = append(puf.history, persistentEntry{
		version: puf.version, index: ry, oldParent: puf.parent[ry], oldRank: puf.rank[ry],
	})
	puf.parent[ry] = rx
	if puf.rank[rx] == puf.rank[ry] {
		puf.history = append(puf.history, persistentEntry{
			version: puf.version, index: rx, oldParent: puf.parent[rx], oldRank: puf.rank[rx],
		})
		puf.rank[rx]++
	}
	return puf.version
}

func (puf *PersistentUnionFind) ConnectedAtVersion(x, y, version int) bool {
	return puf.FindAtVersion(x, version) == puf.FindAtVersion(y, version)
}

func (puf *PersistentUnionFind) Connected(x, y int) bool {
	return puf.Find(x) == puf.Find(y)
}

// FindRedundantConnection finds the edge that creates a cycle in an edge list.
func FindRedundantConnection(edges [][2]int) [2]int {
	n := len(edges)
	uf := NewUnionFind(n + 1) // 1-indexed
	for _, e := range edges {
		if !uf.Union(e[0], e[1]) {
			return e
		}
	}
	return [2]int{}
}

// GetComponents returns all connected components as slices of indices.
func (uf *UnionFind) GetComponents() [][]int {
	components := make(map[int][]int)
	for i := 0; i < len(uf.parent); i++ {
		root := uf.Find(i)
		components[root] = append(components[root], i)
	}
	result := make([][]int, 0, len(components))
	for _, comp := range components {
		result = append(result, comp)
	}
	return result
}

// SetSize returns the size of the set containing x using tracking.
func (uf *UnionFind) SetSize(x int) int {
	root := uf.Find(x)
	count := 0
	for i := range uf.parent {
		if uf.Find(i) == root {
			count++
		}
	}
	return count
}

// NumIslands counts connected components in a 2D grid.
func NumIslands(grid [][]byte) int {
	if len(grid) == 0 || len(grid[0]) == 0 {
		return 0
	}

	rows, cols := len(grid), len(grid[0])
	uf := NewUnionFind(rows * cols)
	waterCount := 0

	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			if grid[i][j] == '0' {
				waterCount++
				continue
			}
			idx := i*cols + j
			if i+1 < rows && grid[i+1][j] == '1' {
				uf.Union(idx, (i+1)*cols+j)
			}
			if j+1 < cols && grid[i][j+1] == '1' {
				uf.Union(idx, i*cols+j+1)
			}
		}
	}

	return uf.Components() - waterCount
}
