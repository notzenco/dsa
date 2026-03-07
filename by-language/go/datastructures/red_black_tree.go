package datastructures

const (
	rbRed   = false
	rbBlack = true
)

// RBNode is a node in a Red-Black tree.
type RBNode[K Ordered] struct {
	Key    K
	Left   *RBNode[K]
	Right  *RBNode[K]
	Parent *RBNode[K]
	Color  bool // false=red, true=black
}

// RedBlackTree is a self-balancing binary search tree.
type RedBlackTree[K Ordered] struct {
	Root *RBNode[K]
	nil_ *RBNode[K] // sentinel nil node
	size int
}

func NewRedBlackTree[K Ordered]() *RedBlackTree[K] {
	nil_ := &RBNode[K]{Color: rbBlack}
	return &RedBlackTree[K]{
		Root: nil_,
		nil_: nil_,
	}
}

func (t *RedBlackTree[K]) Size() int {
	return t.size
}

func (t *RedBlackTree[K]) IsEmpty() bool {
	return t.size == 0
}

func (t *RedBlackTree[K]) rotateLeft(x *RBNode[K]) {
	y := x.Right
	x.Right = y.Left
	if y.Left != t.nil_ {
		y.Left.Parent = x
	}
	y.Parent = x.Parent
	if x.Parent == t.nil_ {
		t.Root = y
	} else if x == x.Parent.Left {
		x.Parent.Left = y
	} else {
		x.Parent.Right = y
	}
	y.Left = x
	x.Parent = y
}

func (t *RedBlackTree[K]) rotateRight(y *RBNode[K]) {
	x := y.Left
	y.Left = x.Right
	if x.Right != t.nil_ {
		x.Right.Parent = y
	}
	x.Parent = y.Parent
	if y.Parent == t.nil_ {
		t.Root = x
	} else if y == y.Parent.Left {
		y.Parent.Left = x
	} else {
		y.Parent.Right = x
	}
	x.Right = y
	y.Parent = x
}

func (t *RedBlackTree[K]) Insert(key K) {
	node := &RBNode[K]{Key: key, Left: t.nil_, Right: t.nil_, Color: rbRed}

	parent := t.nil_
	current := t.Root
	for current != t.nil_ {
		parent = current
		if key < current.Key {
			current = current.Left
		} else if key > current.Key {
			current = current.Right
		} else {
			return // duplicate
		}
	}

	node.Parent = parent
	if parent == t.nil_ {
		t.Root = node
	} else if key < parent.Key {
		parent.Left = node
	} else {
		parent.Right = node
	}

	t.size++
	t.insertFixup(node)
}

func (t *RedBlackTree[K]) insertFixup(z *RBNode[K]) {
	for z.Parent.Color == rbRed {
		if z.Parent == z.Parent.Parent.Left {
			y := z.Parent.Parent.Right
			if y.Color == rbRed {
				z.Parent.Color = rbBlack
				y.Color = rbBlack
				z.Parent.Parent.Color = rbRed
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Right {
					z = z.Parent
					t.rotateLeft(z)
				}
				z.Parent.Color = rbBlack
				z.Parent.Parent.Color = rbRed
				t.rotateRight(z.Parent.Parent)
			}
		} else {
			y := z.Parent.Parent.Left
			if y.Color == rbRed {
				z.Parent.Color = rbBlack
				y.Color = rbBlack
				z.Parent.Parent.Color = rbRed
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Left {
					z = z.Parent
					t.rotateRight(z)
				}
				z.Parent.Color = rbBlack
				z.Parent.Parent.Color = rbRed
				t.rotateLeft(z.Parent.Parent)
			}
		}
	}
	t.Root.Color = rbBlack
}

func (t *RedBlackTree[K]) transplant(u, v *RBNode[K]) {
	if u.Parent == t.nil_ {
		t.Root = v
	} else if u == u.Parent.Left {
		u.Parent.Left = v
	} else {
		u.Parent.Right = v
	}
	v.Parent = u.Parent
}

func (t *RedBlackTree[K]) Delete(key K) bool {
	z := t.searchNode(key)
	if z == t.nil_ {
		return false
	}

	y := z
	yOrigColor := y.Color
	var x *RBNode[K]

	if z.Left == t.nil_ {
		x = z.Right
		t.transplant(z, z.Right)
	} else if z.Right == t.nil_ {
		x = z.Left
		t.transplant(z, z.Left)
	} else {
		y = t.minimum(z.Right)
		yOrigColor = y.Color
		x = y.Right
		if y.Parent == z {
			x.Parent = y
		} else {
			t.transplant(y, y.Right)
			y.Right = z.Right
			y.Right.Parent = y
		}
		t.transplant(z, y)
		y.Left = z.Left
		y.Left.Parent = y
		y.Color = z.Color
	}

	t.size--
	if yOrigColor == rbBlack {
		t.deleteFixup(x)
	}
	return true
}

func (t *RedBlackTree[K]) deleteFixup(x *RBNode[K]) {
	for x != t.Root && x.Color == rbBlack {
		if x == x.Parent.Left {
			w := x.Parent.Right
			if w.Color == rbRed {
				w.Color = rbBlack
				x.Parent.Color = rbRed
				t.rotateLeft(x.Parent)
				w = x.Parent.Right
			}
			if w.Left.Color == rbBlack && w.Right.Color == rbBlack {
				w.Color = rbRed
				x = x.Parent
			} else {
				if w.Right.Color == rbBlack {
					w.Left.Color = rbBlack
					w.Color = rbRed
					t.rotateRight(w)
					w = x.Parent.Right
				}
				w.Color = x.Parent.Color
				x.Parent.Color = rbBlack
				w.Right.Color = rbBlack
				t.rotateLeft(x.Parent)
				x = t.Root
			}
		} else {
			w := x.Parent.Left
			if w.Color == rbRed {
				w.Color = rbBlack
				x.Parent.Color = rbRed
				t.rotateRight(x.Parent)
				w = x.Parent.Left
			}
			if w.Right.Color == rbBlack && w.Left.Color == rbBlack {
				w.Color = rbRed
				x = x.Parent
			} else {
				if w.Left.Color == rbBlack {
					w.Right.Color = rbBlack
					w.Color = rbRed
					t.rotateLeft(w)
					w = x.Parent.Left
				}
				w.Color = x.Parent.Color
				x.Parent.Color = rbBlack
				w.Left.Color = rbBlack
				t.rotateRight(x.Parent)
				x = t.Root
			}
		}
	}
	x.Color = rbBlack
}

func (t *RedBlackTree[K]) Search(key K) bool {
	return t.searchNode(key) != t.nil_
}

func (t *RedBlackTree[K]) searchNode(key K) *RBNode[K] {
	current := t.Root
	for current != t.nil_ {
		if key < current.Key {
			current = current.Left
		} else if key > current.Key {
			current = current.Right
		} else {
			return current
		}
	}
	return t.nil_
}

func (t *RedBlackTree[K]) minimum(node *RBNode[K]) *RBNode[K] {
	for node.Left != t.nil_ {
		node = node.Left
	}
	return node
}

func (t *RedBlackTree[K]) Inorder() []K {
	var result []K
	t.inorder(t.Root, &result)
	return result
}

func (t *RedBlackTree[K]) inorder(node *RBNode[K], result *[]K) {
	if node == t.nil_ {
		return
	}
	t.inorder(node.Left, result)
	*result = append(*result, node.Key)
	t.inorder(node.Right, result)
}

func (t *RedBlackTree[K]) IsValid() bool {
	if t.Root.Color != rbBlack {
		return false
	}
	_, valid := t.validateRB(t.Root)
	return valid
}

func (t *RedBlackTree[K]) validateRB(node *RBNode[K]) (int, bool) {
	if node == t.nil_ {
		return 1, true
	}

	// Red node cannot have red children
	if node.Color == rbRed {
		if node.Left.Color == rbRed || node.Right.Color == rbRed {
			return 0, false
		}
	}

	leftBlack, leftValid := t.validateRB(node.Left)
	rightBlack, rightValid := t.validateRB(node.Right)

	if !leftValid || !rightValid {
		return 0, false
	}

	if leftBlack != rightBlack {
		return 0, false
	}

	blackHeight := leftBlack
	if node.Color == rbBlack {
		blackHeight++
	}
	return blackHeight, true
}
