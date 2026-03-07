package datastructures

import "testing"

func TestBST(t *testing.T) {
	tree := NewBST[int]()
	if !tree.IsEmpty() {
		t.Error("new tree should be empty")
	}

	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(7)
	tree.Insert(1)
	tree.Insert(4)
	tree.Insert(6)
	tree.Insert(8)

	if tree.Size() != 7 {
		t.Errorf("expected size 7, got %d", tree.Size())
	}

	if !tree.Contains(5) {
		t.Error("should contain 5")
	}
	if tree.Contains(99) {
		t.Error("should not contain 99")
	}
}

func TestBSTTraversals(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}

	inorder := tree.Inorder()
	expected := []int{1, 3, 4, 5, 6, 7, 8}
	for i, v := range expected {
		if inorder[i] != v {
			t.Errorf("inorder[%d]: expected %d, got %d", i, v, inorder[i])
		}
	}

	preorder := tree.Preorder()
	expectedPre := []int{5, 3, 1, 4, 7, 6, 8}
	for i, v := range expectedPre {
		if preorder[i] != v {
			t.Errorf("preorder[%d]: expected %d, got %d", i, v, preorder[i])
		}
	}

	postorder := tree.Postorder()
	expectedPost := []int{1, 4, 3, 6, 8, 7, 5}
	for i, v := range expectedPost {
		if postorder[i] != v {
			t.Errorf("postorder[%d]: expected %d, got %d", i, v, postorder[i])
		}
	}

	levelOrder := tree.LevelOrder()
	expectedLevel := []int{5, 3, 7, 1, 4, 6, 8}
	for i, v := range expectedLevel {
		if levelOrder[i] != v {
			t.Errorf("levelorder[%d]: expected %d, got %d", i, v, levelOrder[i])
		}
	}
}

func TestBSTDelete(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}

	// Delete leaf
	tree.Delete(1)
	if tree.Contains(1) {
		t.Error("should not contain 1 after delete")
	}

	// Delete node with one child
	tree.Delete(3)
	if tree.Contains(3) {
		t.Error("should not contain 3 after delete")
	}

	// Delete node with two children
	tree.Delete(7)
	if tree.Contains(7) {
		t.Error("should not contain 7 after delete")
	}

	if !tree.IsValidBST() {
		t.Error("tree should still be valid BST")
	}
}

func TestBSTMinMax(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 9} {
		tree.Insert(v)
	}

	min, ok := tree.Min()
	if !ok || min != 1 {
		t.Errorf("expected min 1, got %d", min)
	}

	max, ok := tree.Max()
	if !ok || max != 9 {
		t.Errorf("expected max 9, got %d", max)
	}
}

func TestBSTSuccessorPredecessor(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}

	succ, ok := tree.Successor(5)
	if !ok || succ != 6 {
		t.Errorf("expected successor 6, got %d", succ)
	}

	pred, ok := tree.Predecessor(5)
	if !ok || pred != 4 {
		t.Errorf("expected predecessor 4, got %d", pred)
	}
}

func TestBSTKthSmallest(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}

	val, ok := tree.KthSmallest(1)
	if !ok || val != 1 {
		t.Errorf("expected 1st smallest = 1, got %d", val)
	}

	val, ok = tree.KthSmallest(4)
	if !ok || val != 5 {
		t.Errorf("expected 4th smallest = 5, got %d", val)
	}
}

func TestBSTFloorCeiling(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{10, 5, 15, 3, 7, 12, 18} {
		tree.Insert(v)
	}

	floor, ok := tree.Floor(6)
	if !ok || floor != 5 {
		t.Errorf("expected floor 5, got %d", floor)
	}

	ceiling, ok := tree.Ceiling(6)
	if !ok || ceiling != 7 {
		t.Errorf("expected ceiling 7, got %d", ceiling)
	}

	floor, ok = tree.Floor(10)
	if !ok || floor != 10 {
		t.Errorf("expected floor 10, got %d", floor)
	}
}

func TestBSTRangeQuery(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}

	result := tree.RangeQuery(3, 7)
	expected := []int{3, 4, 5, 6, 7}
	if len(result) != len(expected) {
		t.Fatalf("expected %d results, got %d", len(expected), len(result))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestBSTFromSorted(t *testing.T) {
	tree := BSTFromSortedSlice([]int{1, 2, 3, 4, 5, 6, 7})

	if tree.Size() != 7 {
		t.Errorf("expected size 7, got %d", tree.Size())
	}
	if !tree.IsValidBST() {
		t.Error("should be valid BST")
	}
	if tree.Height() > 3 {
		t.Error("tree from sorted should be balanced")
	}
}

func TestBSTLowestCommonAncestor(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{6, 2, 8, 0, 4, 7, 9, 3, 5} {
		tree.Insert(v)
	}

	lca, ok := tree.LowestCommonAncestor(2, 8)
	if !ok || lca != 6 {
		t.Errorf("expected LCA 6, got %d", lca)
	}

	lca, ok = tree.LowestCommonAncestor(2, 4)
	if !ok || lca != 2 {
		t.Errorf("expected LCA 2, got %d", lca)
	}
}
