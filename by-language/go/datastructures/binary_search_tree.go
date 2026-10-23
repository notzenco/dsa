package datastructures

// BSTNode is a node in a binary search tree.
type BSTNode[T Ordered] struct {
	Value  T
	Left   *BSTNode[T]
	Right  *BSTNode[T]
	Parent *BSTNode[T]
}

// BST is a binary search tree.
type BST[T Ordered] struct {
	Root *BSTNode[T]
	size int
}

func NewBST[T Ordered]() *BST[T] {
	return &BST[T]{}
}

func (t *BST[T]) Size() int {
	return t.size
}

func (t *BST[T]) IsEmpty() bool {
	return t.size == 0
}

func (t *BST[T]) Insert(value T) *BSTNode[T] {
	node := &BSTNode[T]{Value: value}
	if t.Root == nil {
		t.Root = node
		t.size++
		return node
	}

	current := t.Root
	for {
		if value < current.Value {
			if current.Left == nil {
				current.Left = node
				node.Parent = current
				t.size++
				return node
			}
			current = current.Left
		} else if value > current.Value {
			if current.Right == nil {
				current.Right = node
				node.Parent = current
				t.size++
				return node
			}
			current = current.Right
		} else {
			return current // duplicate
		}
	}
}

func (t *BST[T]) Search(value T) *BSTNode[T] {
	current := t.Root
	for current != nil {
		if value < current.Value {
			current = current.Left
		} else if value > current.Value {
			current = current.Right
		} else {
			return current
		}
	}
	return nil
}

func (t *BST[T]) Contains(value T) bool {
	return t.Search(value) != nil
}

func (t *BST[T]) Delete(value T) bool {
	node := t.Search(value)
	if node == nil {
		return false
	}
	t.deleteNode(node)
	t.size--
	return true
}

func (t *BST[T]) deleteNode(node *BSTNode[T]) {
	if node.Left == nil && node.Right == nil {
		t.replaceNode(node, nil)
	} else if node.Left == nil {
		t.replaceNode(node, node.Right)
	} else if node.Right == nil {
		t.replaceNode(node, node.Left)
	} else {
		successor := t.minNode(node.Right)
		node.Value = successor.Value
		t.deleteNode(successor)
	}
}

func (t *BST[T]) replaceNode(old, new *BSTNode[T]) {
	if old.Parent == nil {
		t.Root = new
	} else if old == old.Parent.Left {
		old.Parent.Left = new
	} else {
		old.Parent.Right = new
	}
	if new != nil {
		new.Parent = old.Parent
	}
}

func (t *BST[T]) Min() (T, bool) {
	if t.Root == nil {
		var zero T
		return zero, false
	}
	return t.minNode(t.Root).Value, true
}

func (t *BST[T]) Max() (T, bool) {
	if t.Root == nil {
		var zero T
		return zero, false
	}
	return t.maxNode(t.Root).Value, true
}

func (t *BST[T]) minNode(node *BSTNode[T]) *BSTNode[T] {
	for node.Left != nil {
		node = node.Left
	}
	return node
}

func (t *BST[T]) maxNode(node *BSTNode[T]) *BSTNode[T] {
	for node.Right != nil {
		node = node.Right
	}
	return node
}

func (t *BST[T]) Successor(value T) (T, bool) {
	node := t.Search(value)
	if node == nil {
		var zero T
		return zero, false
	}
	if node.Right != nil {
		return t.minNode(node.Right).Value, true
	}
	parent := node.Parent
	for parent != nil && node == parent.Right {
		node = parent
		parent = parent.Parent
	}
	if parent == nil {
		var zero T
		return zero, false
	}
	return parent.Value, true
}

func (t *BST[T]) Predecessor(value T) (T, bool) {
	node := t.Search(value)
	if node == nil {
		var zero T
		return zero, false
	}
	if node.Left != nil {
		return t.maxNode(node.Left).Value, true
	}
	parent := node.Parent
	for parent != nil && node == parent.Left {
		node = parent
		parent = parent.Parent
	}
	if parent == nil {
		var zero T
		return zero, false
	}
	return parent.Value, true
}

func (t *BST[T]) Inorder() []T {
	var result []T
	var inorder func(*BSTNode[T])
	inorder = func(node *BSTNode[T]) {
		if node == nil {
			return
		}
		inorder(node.Left)
		result = append(result, node.Value)
		inorder(node.Right)
	}
	inorder(t.Root)
	return result
}

func (t *BST[T]) Preorder() []T {
	var result []T
	var preorder func(*BSTNode[T])
	preorder = func(node *BSTNode[T]) {
		if node == nil {
			return
		}
		result = append(result, node.Value)
		preorder(node.Left)
		preorder(node.Right)
	}
	preorder(t.Root)
	return result
}

func (t *BST[T]) Postorder() []T {
	var result []T
	var postorder func(*BSTNode[T])
	postorder = func(node *BSTNode[T]) {
		if node == nil {
			return
		}
		postorder(node.Left)
		postorder(node.Right)
		result = append(result, node.Value)
	}
	postorder(t.Root)
	return result
}

func (t *BST[T]) LevelOrder() []T {
	if t.Root == nil {
		return nil
	}
	var result []T
	queue := []*BSTNode[T]{t.Root}
	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]
		result = append(result, node.Value)
		if node.Left != nil {
			queue = append(queue, node.Left)
		}
		if node.Right != nil {
			queue = append(queue, node.Right)
		}
	}
	return result
}

func (t *BST[T]) IsValidBST() bool {
	return t.isValidBST(t.Root, nil, nil)
}

func (t *BST[T]) isValidBST(node *BSTNode[T], min, max *T) bool {
	if node == nil {
		return true
	}
	if min != nil && node.Value <= *min {
		return false
	}
	if max != nil && node.Value >= *max {
		return false
	}
	return t.isValidBST(node.Left, min, &node.Value) &&
		t.isValidBST(node.Right, &node.Value, max)
}

func (t *BST[T]) KthSmallest(k int) (T, bool) {
	count := 0
	var result T
	found := false

	var inorder func(*BSTNode[T])
	inorder = func(node *BSTNode[T]) {
		if node == nil || found {
			return
		}
		inorder(node.Left)
		count++
		if count == k {
			result = node.Value
			found = true
			return
		}
		inorder(node.Right)
	}
	inorder(t.Root)
	return result, found
}

func (t *BST[T]) Floor(value T) (T, bool) {
	var result T
	found := false
	current := t.Root

	for current != nil {
		if current.Value == value {
			return current.Value, true
		} else if current.Value < value {
			result = current.Value
			found = true
			current = current.Right
		} else {
			current = current.Left
		}
	}
	return result, found
}

func (t *BST[T]) Ceiling(value T) (T, bool) {
	var result T
	found := false
	current := t.Root

	for current != nil {
		if current.Value == value {
			return current.Value, true
		} else if current.Value > value {
			result = current.Value
			found = true
			current = current.Left
		} else {
			current = current.Right
		}
	}
	return result, found
}

func (t *BST[T]) RangeQuery(low, high T) []T {
	var result []T
	var query func(*BSTNode[T])
	query = func(node *BSTNode[T]) {
		if node == nil {
			return
		}
		if node.Value > low {
			query(node.Left)
		}
		if node.Value >= low && node.Value <= high {
			result = append(result, node.Value)
		}
		if node.Value < high {
			query(node.Right)
		}
	}
	query(t.Root)
	return result
}

func (t *BST[T]) Height() int {
	return t.height(t.Root)
}

func (t *BST[T]) height(node *BSTNode[T]) int {
	if node == nil {
		return -1
	}
	lh := t.height(node.Left)
	rh := t.height(node.Right)
	if lh > rh {
		return lh + 1
	}
	return rh + 1
}

// BSTFromSortedSlice builds a balanced BST from a sorted slice. Repeated
// values are kept once.
func BSTFromSortedSlice[T Ordered](arr []T) *BST[T] {
	distinct := make([]T, 0, len(arr))
	for i, v := range arr {
		if i == 0 || v != arr[i-1] {
			distinct = append(distinct, v)
		}
	}
	tree := NewBST[T]()
	tree.Root = buildBST(distinct, 0, len(distinct)-1, nil)
	tree.size = len(distinct)
	return tree
}

func buildBST[T Ordered](arr []T, start, end int, parent *BSTNode[T]) *BSTNode[T] {
	if start > end {
		return nil
	}
	mid := (start + end) / 2
	node := &BSTNode[T]{Value: arr[mid], Parent: parent}
	node.Left = buildBST(arr, start, mid-1, node)
	node.Right = buildBST(arr, mid+1, end, node)
	return node
}

func (t *BST[T]) LowestCommonAncestor(a, b T) (T, bool) {
	node := t.lca(t.Root, a, b)
	if node == nil {
		var zero T
		return zero, false
	}
	return node.Value, true
}

func (t *BST[T]) lca(node *BSTNode[T], a, b T) *BSTNode[T] {
	if node == nil {
		return nil
	}
	if a < node.Value && b < node.Value {
		return t.lca(node.Left, a, b)
	}
	if a > node.Value && b > node.Value {
		return t.lca(node.Right, a, b)
	}
	return node
}

// CountNodes returns the total number of nodes.
func (t *BST[T]) CountNodes() int {
	return t.countNodes(t.Root)
}

func (t *BST[T]) countNodes(node *BSTNode[T]) int {
	if node == nil {
		return 0
	}
	return 1 + t.countNodes(node.Left) + t.countNodes(node.Right)
}

// LeafCount returns the number of leaf nodes.
func (t *BST[T]) LeafCount() int {
	return t.leafCount(t.Root)
}

func (t *BST[T]) leafCount(node *BSTNode[T]) int {
	if node == nil {
		return 0
	}
	if node.Left == nil && node.Right == nil {
		return 1
	}
	return t.leafCount(node.Left) + t.leafCount(node.Right)
}

// InternalCount returns the number of internal (non-leaf) nodes.
func (t *BST[T]) InternalCount() int {
	return t.Size() - t.LeafCount()
}

// Width returns the maximum width (number of nodes at any level).
func (t *BST[T]) Width() int {
	if t.Root == nil {
		return 0
	}
	maxWidth := 0
	queue := []*BSTNode[T]{t.Root}
	for len(queue) > 0 {
		w := len(queue)
		if w > maxWidth {
			maxWidth = w
		}
		for i := 0; i < w; i++ {
			node := queue[0]
			queue = queue[1:]
			if node.Left != nil {
				queue = append(queue, node.Left)
			}
			if node.Right != nil {
				queue = append(queue, node.Right)
			}
		}
	}
	return maxWidth
}

// Diameter returns the longest path between any two nodes.
func (t *BST[T]) Diameter() int {
	d := 0
	t.diameter(t.Root, &d)
	return d
}

func (t *BST[T]) diameter(node *BSTNode[T], maxDiam *int) int {
	if node == nil {
		return -1
	}
	lh := t.diameter(node.Left, maxDiam)
	rh := t.diameter(node.Right, maxDiam)
	if lh+rh+2 > *maxDiam {
		*maxDiam = lh + rh + 2
	}
	if lh > rh {
		return lh + 1
	}
	return rh + 1
}

// IsBalanced checks if the BST is height-balanced.
func (t *BST[T]) IsBalanced() bool {
	return t.checkBalance(t.Root) != -2
}

func (t *BST[T]) checkBalance(node *BSTNode[T]) int {
	if node == nil {
		return -1
	}
	lh := t.checkBalance(node.Left)
	if lh == -2 {
		return -2
	}
	rh := t.checkBalance(node.Right)
	if rh == -2 {
		return -2
	}
	diff := lh - rh
	if diff < -1 || diff > 1 {
		return -2
	}
	if lh > rh {
		return lh + 1
	}
	return rh + 1
}

// IsFull checks if every node has 0 or 2 children.
func (t *BST[T]) IsFull() bool {
	return t.isFull(t.Root)
}

func (t *BST[T]) isFull(node *BSTNode[T]) bool {
	if node == nil {
		return true
	}
	if (node.Left == nil) != (node.Right == nil) {
		return false
	}
	return t.isFull(node.Left) && t.isFull(node.Right)
}

// IsComplete checks if the BST is a complete binary tree.
func (t *BST[T]) IsComplete() bool {
	if t.Root == nil {
		return true
	}
	queue := []*BSTNode[T]{t.Root}
	seenNil := false
	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]
		if node == nil {
			seenNil = true
			continue
		}
		if seenNil {
			return false
		}
		queue = append(queue, node.Left, node.Right)
	}
	return true
}

// PathSum checks if there's a root-to-leaf path that sums to target (int trees only).
func BSTPathSum(root *BSTNode[int], target int) bool {
	if root == nil {
		return false
	}
	if root.Left == nil && root.Right == nil {
		return root.Value == target
	}
	return BSTPathSum(root.Left, target-root.Value) ||
		BSTPathSum(root.Right, target-root.Value)
}

// InvertTree creates a mirror of the BST (note: result is not a valid BST).
func (t *BST[T]) Invert() {
	t.invert(t.Root)
}

func (t *BST[T]) invert(node *BSTNode[T]) {
	if node == nil {
		return
	}
	node.Left, node.Right = node.Right, node.Left
	t.invert(node.Left)
	t.invert(node.Right)
}

// KthLargest returns the kth largest element.
func (t *BST[T]) KthLargest(k int) (T, bool) {
	count := 0
	var result T
	found := false

	var reverseInorder func(*BSTNode[T])
	reverseInorder = func(node *BSTNode[T]) {
		if node == nil || found {
			return
		}
		reverseInorder(node.Right)
		count++
		if count == k {
			result = node.Value
			found = true
			return
		}
		reverseInorder(node.Left)
	}
	reverseInorder(t.Root)
	return result, found
}

// Serialize returns a level-order representation with sentinel values.
func (t *BST[T]) Serialize() []interface{} {
	if t.Root == nil {
		return nil
	}
	var result []interface{}
	queue := []*BSTNode[T]{t.Root}
	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]
		if node == nil {
			result = append(result, nil)
		} else {
			result = append(result, node.Value)
			queue = append(queue, node.Left, node.Right)
		}
	}
	// Trim trailing nils
	for len(result) > 0 && result[len(result)-1] == nil {
		result = result[:len(result)-1]
	}
	return result
}

// Clear removes all nodes.
func (t *BST[T]) Clear() {
	t.Root = nil
	t.size = 0
}

// RangeCount returns the count of elements in [low, high].
func (t *BST[T]) RangeCount(low, high T) int {
	count := 0
	var query func(*BSTNode[T])
	query = func(node *BSTNode[T]) {
		if node == nil {
			return
		}
		if node.Value > low {
			query(node.Left)
		}
		if node.Value >= low && node.Value <= high {
			count++
		}
		if node.Value < high {
			query(node.Right)
		}
	}
	query(t.Root)
	return count
}

// Morris inorder traversal (O(1) space).
func (t *BST[T]) MorrisInorder() []T {
	var result []T
	current := t.Root
	for current != nil {
		if current.Left == nil {
			result = append(result, current.Value)
			current = current.Right
		} else {
			pred := current.Left
			for pred.Right != nil && pred.Right != current {
				pred = pred.Right
			}
			if pred.Right == nil {
				pred.Right = current
				current = current.Left
			} else {
				pred.Right = nil
				result = append(result, current.Value)
				current = current.Right
			}
		}
	}
	return result
}

// BSTFromPreorder reconstructs a BST from its preorder traversal.
func BSTFromPreorder[T Ordered](preorder []T) *BST[T] {
	tree := NewBST[T]()
	for _, v := range preorder {
		tree.Insert(v)
	}
	return tree
}
