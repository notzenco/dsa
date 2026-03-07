package datastructures

// AVLNode is a node in an AVL tree.
type AVLNode[K Ordered, V any] struct {
	Key    K
	Value  V
	Left   *AVLNode[K, V]
	Right  *AVLNode[K, V]
	Height int
}

// AVLTree is a self-balancing binary search tree.
type AVLTree[K Ordered, V any] struct {
	Root *AVLNode[K, V]
	size int
}

func NewAVLTree[K Ordered, V any]() *AVLTree[K, V] {
	return &AVLTree[K, V]{}
}

func (t *AVLTree[K, V]) Size() int {
	return t.size
}

func (t *AVLTree[K, V]) IsEmpty() bool {
	return t.size == 0
}

func (t *AVLTree[K, V]) height(node *AVLNode[K, V]) int {
	if node == nil {
		return -1
	}
	return node.Height
}

func (t *AVLTree[K, V]) updateHeight(node *AVLNode[K, V]) {
	lh := t.height(node.Left)
	rh := t.height(node.Right)
	if lh > rh {
		node.Height = lh + 1
	} else {
		node.Height = rh + 1
	}
}

func (t *AVLTree[K, V]) balanceFactor(node *AVLNode[K, V]) int {
	if node == nil {
		return 0
	}
	return t.height(node.Left) - t.height(node.Right)
}

func (t *AVLTree[K, V]) rotateRight(y *AVLNode[K, V]) *AVLNode[K, V] {
	x := y.Left
	b := x.Right
	x.Right = y
	y.Left = b
	t.updateHeight(y)
	t.updateHeight(x)
	return x
}

func (t *AVLTree[K, V]) rotateLeft(x *AVLNode[K, V]) *AVLNode[K, V] {
	y := x.Right
	b := y.Left
	y.Left = x
	x.Right = b
	t.updateHeight(x)
	t.updateHeight(y)
	return y
}

func (t *AVLTree[K, V]) rebalance(node *AVLNode[K, V]) *AVLNode[K, V] {
	t.updateHeight(node)
	bf := t.balanceFactor(node)

	if bf > 1 {
		if t.balanceFactor(node.Left) < 0 {
			node.Left = t.rotateLeft(node.Left)
		}
		return t.rotateRight(node)
	}
	if bf < -1 {
		if t.balanceFactor(node.Right) > 0 {
			node.Right = t.rotateRight(node.Right)
		}
		return t.rotateLeft(node)
	}
	return node
}

func (t *AVLTree[K, V]) Insert(key K, value V) {
	var inserted bool
	t.Root, inserted = t.insert(t.Root, key, value)
	if inserted {
		t.size++
	}
}

func (t *AVLTree[K, V]) insert(node *AVLNode[K, V], key K, value V) (*AVLNode[K, V], bool) {
	if node == nil {
		return &AVLNode[K, V]{Key: key, Value: value, Height: 0}, true
	}

	var inserted bool
	if key < node.Key {
		node.Left, inserted = t.insert(node.Left, key, value)
	} else if key > node.Key {
		node.Right, inserted = t.insert(node.Right, key, value)
	} else {
		node.Value = value // update existing
		return node, false
	}

	return t.rebalance(node), inserted
}

func (t *AVLTree[K, V]) Delete(key K) bool {
	var deleted bool
	t.Root, deleted = t.delete(t.Root, key)
	if deleted {
		t.size--
	}
	return deleted
}

func (t *AVLTree[K, V]) delete(node *AVLNode[K, V], key K) (*AVLNode[K, V], bool) {
	if node == nil {
		return nil, false
	}

	var deleted bool
	if key < node.Key {
		node.Left, deleted = t.delete(node.Left, key)
	} else if key > node.Key {
		node.Right, deleted = t.delete(node.Right, key)
	} else {
		deleted = true
		if node.Left == nil {
			return node.Right, true
		}
		if node.Right == nil {
			return node.Left, true
		}
		// Find inorder successor
		successor := node.Right
		for successor.Left != nil {
			successor = successor.Left
		}
		node.Key = successor.Key
		node.Value = successor.Value
		node.Right, _ = t.delete(node.Right, successor.Key)
	}

	if !deleted {
		return node, false
	}
	return t.rebalance(node), true
}

func (t *AVLTree[K, V]) Search(key K) (V, bool) {
	node := t.searchNode(t.Root, key)
	if node == nil {
		var zero V
		return zero, false
	}
	return node.Value, true
}

func (t *AVLTree[K, V]) searchNode(node *AVLNode[K, V], key K) *AVLNode[K, V] {
	if node == nil {
		return nil
	}
	if key < node.Key {
		return t.searchNode(node.Left, key)
	}
	if key > node.Key {
		return t.searchNode(node.Right, key)
	}
	return node
}

func (t *AVLTree[K, V]) Inorder() []K {
	var result []K
	var inorder func(*AVLNode[K, V])
	inorder = func(node *AVLNode[K, V]) {
		if node == nil {
			return
		}
		inorder(node.Left)
		result = append(result, node.Key)
		inorder(node.Right)
	}
	inorder(t.Root)
	return result
}

func (t *AVLTree[K, V]) Preorder() []K {
	var result []K
	var preorder func(*AVLNode[K, V])
	preorder = func(node *AVLNode[K, V]) {
		if node == nil {
			return
		}
		result = append(result, node.Key)
		preorder(node.Left)
		preorder(node.Right)
	}
	preorder(t.Root)
	return result
}

func (t *AVLTree[K, V]) LevelOrder() []K {
	if t.Root == nil {
		return nil
	}
	var result []K
	queue := []*AVLNode[K, V]{t.Root}
	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]
		result = append(result, node.Key)
		if node.Left != nil {
			queue = append(queue, node.Left)
		}
		if node.Right != nil {
			queue = append(queue, node.Right)
		}
	}
	return result
}

func (t *AVLTree[K, V]) IsBalanced() bool {
	return t.isBalanced(t.Root)
}

func (t *AVLTree[K, V]) isBalanced(node *AVLNode[K, V]) bool {
	if node == nil {
		return true
	}
	bf := t.balanceFactor(node)
	if bf < -1 || bf > 1 {
		return false
	}
	return t.isBalanced(node.Left) && t.isBalanced(node.Right)
}

func (t *AVLTree[K, V]) GetHeight() int {
	return t.height(t.Root)
}
