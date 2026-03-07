package datastructures

// SegmentTree supports range queries and point updates.
type SegmentTree struct {
	tree     []int
	n        int
	op       func(int, int) int
	identity int
}

func NewSegmentTree(arr []int, op func(int, int) int, identity int) *SegmentTree {
	n := len(arr)
	st := &SegmentTree{
		tree:     make([]int, 4*n),
		n:        n,
		op:       op,
		identity: identity,
	}
	if n > 0 {
		st.build(arr, 0, 0, n-1)
	}
	return st
}

func NewSumSegmentTree(arr []int) *SegmentTree {
	return NewSegmentTree(arr, func(a, b int) int { return a + b }, 0)
}

func NewMinSegmentTree(arr []int) *SegmentTree {
	return NewSegmentTree(arr, func(a, b int) int {
		if a < b {
			return a
		}
		return b
	}, int(^uint(0)>>1)) // MaxInt
}

func NewMaxSegmentTree(arr []int) *SegmentTree {
	return NewSegmentTree(arr, func(a, b int) int {
		if a > b {
			return a
		}
		return b
	}, -int(^uint(0)>>1)-1) // MinInt
}

func (st *SegmentTree) build(arr []int, node, start, end int) {
	if start == end {
		st.tree[node] = arr[start]
		return
	}
	mid := (start + end) / 2
	st.build(arr, 2*node+1, start, mid)
	st.build(arr, 2*node+2, mid+1, end)
	st.tree[node] = st.op(st.tree[2*node+1], st.tree[2*node+2])
}

func (st *SegmentTree) Update(idx, value int) {
	st.update(0, 0, st.n-1, idx, value)
}

func (st *SegmentTree) update(node, start, end, idx, value int) {
	if start == end {
		st.tree[node] = value
		return
	}
	mid := (start + end) / 2
	if idx <= mid {
		st.update(2*node+1, start, mid, idx, value)
	} else {
		st.update(2*node+2, mid+1, end, idx, value)
	}
	st.tree[node] = st.op(st.tree[2*node+1], st.tree[2*node+2])
}

func (st *SegmentTree) Query(left, right int) int {
	return st.query(0, 0, st.n-1, left, right)
}

func (st *SegmentTree) query(node, start, end, left, right int) int {
	if right < start || left > end {
		return st.identity
	}
	if left <= start && end <= right {
		return st.tree[node]
	}
	mid := (start + end) / 2
	leftResult := st.query(2*node+1, start, mid, left, right)
	rightResult := st.query(2*node+2, mid+1, end, left, right)
	return st.op(leftResult, rightResult)
}

// SegmentTreeLazy supports range updates with lazy propagation.
type SegmentTreeLazy struct {
	tree []int
	lazy []int
	n    int
}

func NewSegmentTreeLazy(arr []int) *SegmentTreeLazy {
	n := len(arr)
	st := &SegmentTreeLazy{
		tree: make([]int, 4*n),
		lazy: make([]int, 4*n),
		n:    n,
	}
	if n > 0 {
		st.build(arr, 0, 0, n-1)
	}
	return st
}

func (st *SegmentTreeLazy) build(arr []int, node, start, end int) {
	if start == end {
		st.tree[node] = arr[start]
		return
	}
	mid := (start + end) / 2
	st.build(arr, 2*node+1, start, mid)
	st.build(arr, 2*node+2, mid+1, end)
	st.tree[node] = st.tree[2*node+1] + st.tree[2*node+2]
}

func (st *SegmentTreeLazy) pushDown(node, start, end int) {
	if st.lazy[node] != 0 {
		mid := (start + end) / 2
		left := 2*node + 1
		right := 2*node + 2

		st.tree[left] += st.lazy[node] * (mid - start + 1)
		st.tree[right] += st.lazy[node] * (end - mid)
		st.lazy[left] += st.lazy[node]
		st.lazy[right] += st.lazy[node]
		st.lazy[node] = 0
	}
}

func (st *SegmentTreeLazy) RangeUpdate(left, right, value int) {
	st.rangeUpdate(0, 0, st.n-1, left, right, value)
}

func (st *SegmentTreeLazy) rangeUpdate(node, start, end, left, right, value int) {
	if right < start || left > end {
		return
	}
	if left <= start && end <= right {
		st.tree[node] += value * (end - start + 1)
		st.lazy[node] += value
		return
	}
	st.pushDown(node, start, end)
	mid := (start + end) / 2
	st.rangeUpdate(2*node+1, start, mid, left, right, value)
	st.rangeUpdate(2*node+2, mid+1, end, left, right, value)
	st.tree[node] = st.tree[2*node+1] + st.tree[2*node+2]
}

func (st *SegmentTreeLazy) Query(left, right int) int {
	return st.query(0, 0, st.n-1, left, right)
}

func (st *SegmentTreeLazy) query(node, start, end, left, right int) int {
	if right < start || left > end {
		return 0
	}
	if left <= start && end <= right {
		return st.tree[node]
	}
	st.pushDown(node, start, end)
	mid := (start + end) / 2
	return st.query(2*node+1, start, mid, left, right) +
		st.query(2*node+2, mid+1, end, left, right)
}
