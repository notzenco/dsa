package datastructures

// FenwickTree (Binary Indexed Tree) for prefix sum queries.
type FenwickTree struct {
	tree []int
	n    int
}

func NewFenwickTree(arr []int) *FenwickTree {
	n := len(arr)
	ft := &FenwickTree{
		tree: make([]int, n+1),
		n:    n,
	}
	for i, val := range arr {
		ft.add(i+1, val)
	}
	return ft
}

func (ft *FenwickTree) add(i, delta int) {
	for i <= ft.n {
		ft.tree[i] += delta
		i += i & (-i)
	}
}

func (ft *FenwickTree) Update(idx, delta int) {
	ft.add(idx+1, delta)
}

func (ft *FenwickTree) Set(idx, value int) {
	current := ft.RangeSum(idx, idx)
	ft.Update(idx, value-current)
}

func (ft *FenwickTree) PrefixSum(idx int) int {
	i := idx + 1
	total := 0
	for i > 0 {
		total += ft.tree[i]
		i -= i & (-i)
	}
	return total
}

func (ft *FenwickTree) RangeSum(left, right int) int {
	rightSum := ft.PrefixSum(right)
	if left > 0 {
		return rightSum - ft.PrefixSum(left-1)
	}
	return rightSum
}

func (ft *FenwickTree) Size() int {
	return ft.n
}

// FenwickTree2D for 2D range sum queries.
type FenwickTree2D struct {
	tree [][]int
	rows int
	cols int
}

func NewFenwickTree2D(matrix [][]int) *FenwickTree2D {
	if len(matrix) == 0 || len(matrix[0]) == 0 {
		return &FenwickTree2D{}
	}

	rows, cols := len(matrix), len(matrix[0])
	ft := &FenwickTree2D{
		tree: make([][]int, rows+1),
		rows: rows,
		cols: cols,
	}
	for i := range ft.tree {
		ft.tree[i] = make([]int, cols+1)
	}

	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			ft.add(i+1, j+1, matrix[i][j])
		}
	}
	return ft
}

func (ft *FenwickTree2D) add(row, col, delta int) {
	for i := row; i <= ft.rows; i += i & (-i) {
		for j := col; j <= ft.cols; j += j & (-j) {
			ft.tree[i][j] += delta
		}
	}
}

func (ft *FenwickTree2D) Update(row, col, delta int) {
	ft.add(row+1, col+1, delta)
}

func (ft *FenwickTree2D) prefixSum(row, col int) int {
	total := 0
	for i := row; i > 0; i -= i & (-i) {
		for j := col; j > 0; j -= j & (-j) {
			total += ft.tree[i][j]
		}
	}
	return total
}

func (ft *FenwickTree2D) Query(r1, c1, r2, c2 int) int {
	if ft.rows == 0 || ft.cols == 0 {
		return 0
	}
	r1++
	c1++
	r2++
	c2++
	return ft.prefixSum(r2, c2) - ft.prefixSum(r1-1, c2) -
		ft.prefixSum(r2, c1-1) + ft.prefixSum(r1-1, c1-1)
}

// CountInversions counts inversions in an array using Fenwick tree.
func CountInversions(arr []int) int {
	if len(arr) == 0 {
		return 0
	}

	// Coordinate compression
	sorted := make([]int, len(arr))
	copy(sorted, arr)
	HeapSort(sorted)

	// Remove duplicates and create rank map
	rank := make(map[int]int)
	r := 1
	prev := sorted[0]
	rank[prev] = r
	for _, v := range sorted[1:] {
		if v != prev {
			r++
			rank[v] = r
			prev = v
		}
	}

	n := r
	ft := NewFenwickTree(make([]int, n))
	inversions := 0

	for i := len(arr) - 1; i >= 0; i-- {
		rv := rank[arr[i]]
		if rv > 1 {
			inversions += ft.PrefixSum(rv - 2)
		}
		ft.Update(rv-1, 1)
	}

	return inversions
}
