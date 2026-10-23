package datastructures

// BTreeNode is a node in a B-tree.
type BTreeNode[K Ordered, V any] struct {
	Keys     []K
	Values   []V
	Children []*BTreeNode[K, V]
	Leaf     bool
}

// BTree is a self-balancing tree data structure with variable branching factor.
type BTree[K Ordered, V any] struct {
	Root *BTreeNode[K, V]
	t    int // minimum degree
	size int
}

func NewBTree[K Ordered, V any](minDegree int) *BTree[K, V] {
	if minDegree < 2 {
		minDegree = 2
	}
	root := &BTreeNode[K, V]{Leaf: true}
	return &BTree[K, V]{Root: root, t: minDegree}
}

func (bt *BTree[K, V]) Size() int {
	return bt.size
}

func (bt *BTree[K, V]) IsEmpty() bool {
	return bt.size == 0
}

func (bt *BTree[K, V]) Search(key K) (V, bool) {
	return bt.search(bt.Root, key)
}

func (bt *BTree[K, V]) search(node *BTreeNode[K, V], key K) (V, bool) {
	i := 0
	for i < len(node.Keys) && key > node.Keys[i] {
		i++
	}
	if i < len(node.Keys) && key == node.Keys[i] {
		return node.Values[i], true
	}
	if node.Leaf {
		var zero V
		return zero, false
	}
	return bt.search(node.Children[i], key)
}

func (bt *BTree[K, V]) Insert(key K, value V) {
	root := bt.Root
	if len(root.Keys) == 2*bt.t-1 {
		newRoot := &BTreeNode[K, V]{Leaf: false}
		newRoot.Children = append(newRoot.Children, root)
		bt.Root = newRoot
		bt.splitChild(newRoot, 0)
		bt.insertNonFull(newRoot, key, value)
	} else {
		bt.insertNonFull(root, key, value)
	}
}

func (bt *BTree[K, V]) insertNonFull(node *BTreeNode[K, V], key K, value V) {
	if node.Leaf {
		pos := 0
		for pos < len(node.Keys) && key > node.Keys[pos] {
			pos++
		}
		if pos < len(node.Keys) && key == node.Keys[pos] {
			node.Values[pos] = value
			return
		}
		node.Keys = append(node.Keys, key)
		node.Values = append(node.Values, value)
		for j := len(node.Keys) - 1; j > pos; j-- {
			node.Keys[j] = node.Keys[j-1]
			node.Values[j] = node.Values[j-1]
		}
		node.Keys[pos] = key
		node.Values[pos] = value
		bt.size++
	} else {
		i := len(node.Keys) - 1
		for i >= 0 && key < node.Keys[i] {
			i--
		}
		// i is the rightmost key that is <= key, or -1 when every key is larger.
		if i >= 0 && key == node.Keys[i] {
			node.Values[i] = value
			return
		}
		i++
		if len(node.Children[i].Keys) == 2*bt.t-1 {
			bt.splitChild(node, i)
			if key > node.Keys[i] {
				i++
			} else if key == node.Keys[i] {
				node.Values[i] = value
				return
			}
		}
		bt.insertNonFull(node.Children[i], key, value)
	}
}

func (bt *BTree[K, V]) splitChild(parent *BTreeNode[K, V], i int) {
	t := bt.t
	child := parent.Children[i]
	newNode := &BTreeNode[K, V]{Leaf: child.Leaf}

	midKey := child.Keys[t-1]
	midVal := child.Values[t-1]

	newNode.Keys = make([]K, len(child.Keys[t:]))
	copy(newNode.Keys, child.Keys[t:])
	newNode.Values = make([]V, len(child.Values[t:]))
	copy(newNode.Values, child.Values[t:])

	if !child.Leaf {
		newNode.Children = make([]*BTreeNode[K, V], len(child.Children[t:]))
		copy(newNode.Children, child.Children[t:])
		child.Children = child.Children[:t]
	}

	child.Keys = child.Keys[:t-1]
	child.Values = child.Values[:t-1]

	// Insert midKey into parent at position i
	parent.Keys = append(parent.Keys, midKey)
	parent.Values = append(parent.Values, midVal)
	for j := len(parent.Keys) - 1; j > i; j-- {
		parent.Keys[j] = parent.Keys[j-1]
		parent.Values[j] = parent.Values[j-1]
	}
	parent.Keys[i] = midKey
	parent.Values[i] = midVal

	// Insert newNode into parent.Children at position i+1
	parent.Children = append(parent.Children, newNode)
	for j := len(parent.Children) - 1; j > i+1; j-- {
		parent.Children[j] = parent.Children[j-1]
	}
	parent.Children[i+1] = newNode
}

func (bt *BTree[K, V]) Delete(key K) bool {
	if bt.Root == nil || len(bt.Root.Keys) == 0 {
		return false
	}
	deleted := bt.deleteKey(bt.Root, key)
	if deleted {
		bt.size--
	}
	if len(bt.Root.Keys) == 0 && !bt.Root.Leaf {
		bt.Root = bt.Root.Children[0]
	}
	return deleted
}

func (bt *BTree[K, V]) deleteKey(node *BTreeNode[K, V], key K) bool {
	i := 0
	for i < len(node.Keys) && key > node.Keys[i] {
		i++
	}

	if i < len(node.Keys) && key == node.Keys[i] {
		if node.Leaf {
			node.Keys = append(node.Keys[:i], node.Keys[i+1:]...)
			node.Values = append(node.Values[:i], node.Values[i+1:]...)
			return true
		}
		return bt.deleteFromInternal(node, i, key)
	}

	if node.Leaf {
		return false
	}

	childHasMinKeys := len(node.Children[i].Keys) < bt.t
	if childHasMinKeys {
		bt.fill(node, i)
	}

	// After fill, find correct child index again
	// The key might now be in a different position
	j := 0
	for j < len(node.Keys) && key > node.Keys[j] {
		j++
	}
	if j < len(node.Keys) && key == node.Keys[j] {
		if node.Leaf {
			node.Keys = append(node.Keys[:j], node.Keys[j+1:]...)
			node.Values = append(node.Values[:j], node.Values[j+1:]...)
			return true
		}
		return bt.deleteFromInternal(node, j, key)
	}
	if j >= len(node.Children) {
		j = len(node.Children) - 1
	}
	return bt.deleteKey(node.Children[j], key)
}

func (bt *BTree[K, V]) deleteFromInternal(node *BTreeNode[K, V], i int, key K) bool {
	if len(node.Children[i].Keys) >= bt.t {
		pred := node.Children[i]
		for !pred.Leaf {
			pred = pred.Children[len(pred.Children)-1]
		}
		predKey := pred.Keys[len(pred.Keys)-1]
		predVal := pred.Values[len(pred.Values)-1]
		node.Keys[i] = predKey
		node.Values[i] = predVal
		return bt.deleteKey(node.Children[i], predKey)
	}
	if i+1 < len(node.Children) && len(node.Children[i+1].Keys) >= bt.t {
		succ := node.Children[i+1]
		for !succ.Leaf {
			succ = succ.Children[0]
		}
		succKey := succ.Keys[0]
		succVal := succ.Values[0]
		node.Keys[i] = succKey
		node.Values[i] = succVal
		return bt.deleteKey(node.Children[i+1], succKey)
	}
	// Merge children[i] and children[i+1]
	bt.merge(node, i)
	return bt.deleteKey(node.Children[i], key)
}

func (bt *BTree[K, V]) fill(node *BTreeNode[K, V], i int) {
	if i > 0 && len(node.Children[i-1].Keys) >= bt.t {
		bt.borrowFromLeft(node, i)
	} else if i < len(node.Children)-1 && len(node.Children[i+1].Keys) >= bt.t {
		bt.borrowFromRight(node, i)
	} else {
		if i < len(node.Children)-1 {
			bt.merge(node, i)
		} else {
			bt.merge(node, i-1)
		}
	}
}

func (bt *BTree[K, V]) borrowFromLeft(node *BTreeNode[K, V], i int) {
	child := node.Children[i]
	sibling := node.Children[i-1]

	child.Keys = append([]K{node.Keys[i-1]}, child.Keys...)
	child.Values = append([]V{node.Values[i-1]}, child.Values...)

	if !child.Leaf {
		child.Children = append([]*BTreeNode[K, V]{sibling.Children[len(sibling.Children)-1]}, child.Children...)
		sibling.Children = sibling.Children[:len(sibling.Children)-1]
	}

	node.Keys[i-1] = sibling.Keys[len(sibling.Keys)-1]
	node.Values[i-1] = sibling.Values[len(sibling.Values)-1]
	sibling.Keys = sibling.Keys[:len(sibling.Keys)-1]
	sibling.Values = sibling.Values[:len(sibling.Values)-1]
}

func (bt *BTree[K, V]) borrowFromRight(node *BTreeNode[K, V], i int) {
	child := node.Children[i]
	sibling := node.Children[i+1]

	child.Keys = append(child.Keys, node.Keys[i])
	child.Values = append(child.Values, node.Values[i])

	if !child.Leaf {
		child.Children = append(child.Children, sibling.Children[0])
		sibling.Children = sibling.Children[1:]
	}

	node.Keys[i] = sibling.Keys[0]
	node.Values[i] = sibling.Values[0]
	sibling.Keys = sibling.Keys[1:]
	sibling.Values = sibling.Values[1:]
}

func (bt *BTree[K, V]) merge(node *BTreeNode[K, V], i int) {
	left := node.Children[i]
	right := node.Children[i+1]

	left.Keys = append(left.Keys, node.Keys[i])
	left.Values = append(left.Values, node.Values[i])

	left.Keys = append(left.Keys, right.Keys...)
	left.Values = append(left.Values, right.Values...)
	if !left.Leaf {
		left.Children = append(left.Children, right.Children...)
	}

	node.Keys = append(node.Keys[:i], node.Keys[i+1:]...)
	node.Values = append(node.Values[:i], node.Values[i+1:]...)
	node.Children = append(node.Children[:i+1], node.Children[i+2:]...)
}

func (bt *BTree[K, V]) Inorder() []K {
	var result []K
	bt.inorder(bt.Root, &result)
	return result
}

func (bt *BTree[K, V]) inorder(node *BTreeNode[K, V], result *[]K) {
	if node == nil {
		return
	}
	for i := 0; i < len(node.Keys); i++ {
		if !node.Leaf {
			bt.inorder(node.Children[i], result)
		}
		*result = append(*result, node.Keys[i])
	}
	if !node.Leaf && len(node.Children) > len(node.Keys) {
		bt.inorder(node.Children[len(node.Children)-1], result)
	}
}

func (bt *BTree[K, V]) Contains(key K) bool {
	_, found := bt.Search(key)
	return found
}
