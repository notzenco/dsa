package datastructures

import "testing"

func TestAVLTree(t *testing.T) {
	tree := NewAVLTree[int, string]()

	tree.Insert(10, "ten")
	tree.Insert(20, "twenty")
	tree.Insert(30, "thirty")
	tree.Insert(40, "forty")
	tree.Insert(50, "fifty")
	tree.Insert(25, "twenty-five")

	if tree.Size() != 6 {
		t.Errorf("expected size 6, got %d", tree.Size())
	}

	if !tree.IsBalanced() {
		t.Error("AVL tree should be balanced")
	}

	val, ok := tree.Search(30)
	if !ok || val != "thirty" {
		t.Errorf("expected 'thirty', got '%s'", val)
	}

	_, ok = tree.Search(99)
	if ok {
		t.Error("should not find 99")
	}
}

func TestAVLTreeInsertUpdate(t *testing.T) {
	tree := NewAVLTree[int, string]()
	tree.Insert(1, "one")
	tree.Insert(1, "ONE")

	val, _ := tree.Search(1)
	if val != "ONE" {
		t.Errorf("expected 'ONE' after update, got '%s'", val)
	}
	if tree.Size() != 1 {
		t.Errorf("size should be 1 after update, got %d", tree.Size())
	}
}

func TestAVLTreeDelete(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{10, 20, 30, 40, 50, 25} {
		tree.Insert(v, v)
	}

	tree.Delete(40)
	if tree.Size() != 5 {
		t.Errorf("expected size 5, got %d", tree.Size())
	}

	_, ok := tree.Search(40)
	if ok {
		t.Error("should not find 40 after delete")
	}

	if !tree.IsBalanced() {
		t.Error("tree should be balanced after delete")
	}

	if !tree.Delete(10) {
		t.Error("delete should return true for existing key")
	}
	if tree.Delete(99) {
		t.Error("delete should return false for non-existing key")
	}
}

func TestAVLTreeTraversals(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v, v)
	}

	inorder := tree.Inorder()
	expected := []int{1, 3, 4, 5, 6, 7, 8}
	if len(inorder) != len(expected) {
		t.Fatalf("expected %d elements, got %d", len(expected), len(inorder))
	}
	for i, v := range expected {
		if inorder[i] != v {
			t.Errorf("inorder[%d]: expected %d, got %d", i, v, inorder[i])
		}
	}
}

func TestAVLTreeBalance(t *testing.T) {
	tree := NewAVLTree[int, int]()

	// Insert sequential values - should still be balanced
	for i := 1; i <= 100; i++ {
		tree.Insert(i, i)
	}

	if !tree.IsBalanced() {
		t.Error("tree should be balanced even with sequential inserts")
	}

	// Height should be O(log n), so ~7 for 100 elements
	h := tree.GetHeight()
	if h > 10 {
		t.Errorf("height should be reasonable for 100 elements, got %d", h)
	}
}

func TestAVLTreeDeleteAll(t *testing.T) {
	tree := NewAVLTree[int, int]()
	values := []int{5, 3, 7, 1, 4, 6, 8, 2, 9, 10}
	for _, v := range values {
		tree.Insert(v, v)
	}

	for _, v := range values {
		tree.Delete(v)
		if !tree.IsBalanced() {
			t.Errorf("tree should be balanced after deleting %d", v)
		}
	}

	if tree.Size() != 0 {
		t.Errorf("expected size 0, got %d", tree.Size())
	}
}

func TestAVLTreeMinMax(t *testing.T) {
	tree := NewAVLTree[int, string]()
	tree.Insert(5, "five")
	tree.Insert(1, "one")
	tree.Insert(9, "nine")

	k, v, ok := tree.Min()
	if !ok || k != 1 || v != "one" {
		t.Errorf("expected min (1, 'one'), got (%d, %q)", k, v)
	}

	k, v, ok = tree.Max()
	if !ok || k != 9 || v != "nine" {
		t.Errorf("expected max (9, 'nine'), got (%d, %q)", k, v)
	}
}

func TestAVLTreeMinMaxEmpty(t *testing.T) {
	tree := NewAVLTree[int, int]()
	_, _, ok := tree.Min()
	if ok {
		t.Error("min on empty should fail")
	}
	_, _, ok = tree.Max()
	if ok {
		t.Error("max on empty should fail")
	}
}

func TestAVLTreeContains(t *testing.T) {
	tree := NewAVLTree[int, int]()
	tree.Insert(5, 50)
	tree.Insert(3, 30)

	if !tree.Contains(5) {
		t.Error("should contain 5")
	}
	if tree.Contains(99) {
		t.Error("should not contain 99")
	}
}

func TestAVLTreePostorder(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v, v)
	}
	post := tree.Postorder()
	if len(post) != 7 {
		t.Errorf("expected 7 elements, got %d", len(post))
	}
}

func TestAVLTreeRangeQuery(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v, v)
	}
	result := tree.RangeQuery(3, 6)
	expected := []int{3, 4, 5, 6}
	if len(result) != len(expected) {
		t.Fatalf("expected %d results, got %d", len(expected), len(result))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestAVLTreeFloorCeiling(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{10, 20, 30, 40, 50} {
		tree.Insert(v, v)
	}

	k, _, ok := tree.Floor(25)
	if !ok || k != 20 {
		t.Errorf("expected floor 20, got %d", k)
	}

	k, _, ok = tree.Ceiling(25)
	if !ok || k != 30 {
		t.Errorf("expected ceiling 30, got %d", k)
	}
}

func TestAVLTreeKthSmallest(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v, v)
	}
	k, ok := tree.KthSmallest(3)
	if !ok || k != 4 {
		t.Errorf("expected 3rd smallest = 4, got %d", k)
	}
}

func TestAVLTreeClear(t *testing.T) {
	tree := NewAVLTree[int, int]()
	tree.Insert(1, 10)
	tree.Insert(2, 20)
	tree.Clear()
	if !tree.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestAVLTreeValues(t *testing.T) {
	tree := NewAVLTree[int, string]()
	tree.Insert(2, "two")
	tree.Insert(1, "one")
	tree.Insert(3, "three")

	values := tree.Values()
	if len(values) != 3 {
		t.Errorf("expected 3 values, got %d", len(values))
	}
	if values[0] != "one" || values[1] != "two" || values[2] != "three" {
		t.Errorf("values not in order: %v", values)
	}
}

func TestAVLTreeCountNodes(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for i := 1; i <= 10; i++ {
		tree.Insert(i, i)
	}
	if tree.CountNodes() != 10 {
		t.Errorf("expected 10, got %d", tree.CountNodes())
	}
}

func TestAVLTreePreorder(t *testing.T) {
	tree := NewAVLTree[int, int]()
	tree.Insert(2, 2)
	tree.Insert(1, 1)
	tree.Insert(3, 3)

	pre := tree.Preorder()
	if len(pre) != 3 {
		t.Errorf("expected 3 elements, got %d", len(pre))
	}
	if pre[0] != 2 {
		t.Errorf("expected root 2, got %d", pre[0])
	}
}

func TestAVLTreeLevelOrder(t *testing.T) {
	tree := NewAVLTree[int, int]()
	tree.Insert(2, 2)
	tree.Insert(1, 1)
	tree.Insert(3, 3)

	lo := tree.LevelOrder()
	if len(lo) != 3 {
		t.Errorf("expected 3 elements, got %d", len(lo))
	}
	if lo[0] != 2 {
		t.Errorf("expected root 2 first, got %d", lo[0])
	}
}

func TestAVLTreeReverseSequentialInsert(t *testing.T) {
	tree := NewAVLTree[int, int]()
	for i := 100; i >= 1; i-- {
		tree.Insert(i, i)
	}
	if !tree.IsBalanced() {
		t.Error("should be balanced after reverse sequential inserts")
	}
	h := tree.GetHeight()
	if h > 10 {
		t.Errorf("height should be reasonable, got %d", h)
	}
}
