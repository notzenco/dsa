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
	succ, ok = tree.Successor(4)
	if !ok || succ != 5 {
		t.Errorf("expected successor of 4 to be 5, got %d", succ)
	}
	succ, ok = tree.Successor(1)
	if !ok || succ != 3 {
		t.Errorf("expected successor of 1 to be 3, got %d", succ)
	}
	succ, ok = tree.Successor(6)
	if !ok || succ != 7 {
		t.Errorf("expected successor of 6 to be 7, got %d", succ)
	}
	if _, ok = tree.Successor(8); ok {
		t.Error("maximum should have no successor")
	}
	if _, ok = tree.Successor(99); ok {
		t.Error("missing value should have no successor")
	}

	pred, ok := tree.Predecessor(5)
	if !ok || pred != 4 {
		t.Errorf("expected predecessor 4, got %d", pred)
	}
	pred, ok = tree.Predecessor(6)
	if !ok || pred != 5 {
		t.Errorf("expected predecessor of 6 to be 5, got %d", pred)
	}
	pred, ok = tree.Predecessor(4)
	if !ok || pred != 3 {
		t.Errorf("expected predecessor of 4 to be 3, got %d", pred)
	}
	pred, ok = tree.Predecessor(7)
	if !ok || pred != 6 {
		t.Errorf("expected predecessor of 7 to be 6, got %d", pred)
	}
	if _, ok = tree.Predecessor(1); ok {
		t.Error("minimum should have no predecessor")
	}
	if _, ok = tree.Predecessor(99); ok {
		t.Error("missing value should have no predecessor")
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

	repeated := BSTFromSortedSlice([]int{1, 1, 2, 3, 3, 3})
	if repeated.Size() != 3 {
		t.Errorf("expected repeated values to be kept once, got size %d", repeated.Size())
	}
	if got := repeated.Inorder(); len(got) != 3 || got[0] != 1 || got[1] != 2 || got[2] != 3 {
		t.Errorf("expected in-order [1 2 3], got %v", got)
	}
	if !repeated.IsValidBST() {
		t.Error("tree built from repeated values should be valid")
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

	lca, ok = tree.LowestCommonAncestor(7, 9)
	if !ok || lca != 8 {
		t.Errorf("expected LCA 8, got %d", lca)
	}
	if _, ok = tree.LowestCommonAncestor(-2, -1); ok {
		t.Error("keys past the minimum should have no ancestor")
	}

	empty := NewBST[int]()
	if _, ok = empty.LowestCommonAncestor(1, 2); ok {
		t.Error("empty tree should have no ancestor")
	}
}

func TestBSTCountNodes(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4} {
		tree.Insert(v)
	}
	if tree.CountNodes() != 5 {
		t.Errorf("expected 5, got %d", tree.CountNodes())
	}
}

func TestBSTLeafCount(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	if tree.LeafCount() != 4 {
		t.Errorf("expected 4 leaves, got %d", tree.LeafCount())
	}
}

func TestBSTInternalCount(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	if tree.InternalCount() != 3 {
		t.Errorf("expected 3 internal nodes, got %d", tree.InternalCount())
	}
}

func TestBSTWidth(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	if tree.Width() != 4 {
		t.Errorf("expected width 4, got %d", tree.Width())
	}
}

func TestBSTDiameter(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 8} {
		tree.Insert(v)
	}
	d := tree.Diameter()
	if d != 4 {
		t.Errorf("expected diameter 4, got %d", d)
	}
}

func TestBSTIsBalanced(t *testing.T) {
	balanced := BSTFromSortedSlice([]int{1, 2, 3, 4, 5, 6, 7})
	if !balanced.IsBalanced() {
		t.Error("tree from sorted slice should be balanced")
	}

	skewed := NewBST[int]()
	for i := 1; i <= 10; i++ {
		skewed.Insert(i)
	}
	if skewed.IsBalanced() {
		t.Error("sequentially inserted tree should not be balanced")
	}
}

func TestBSTIsFull(t *testing.T) {
	tree := BSTFromSortedSlice([]int{1, 2, 3, 4, 5, 6, 7})
	if !tree.IsFull() {
		t.Error("perfect tree should be full")
	}

	tree2 := NewBST[int]()
	tree2.Insert(5)
	tree2.Insert(3)
	// 5 has only left child
	if tree2.IsFull() {
		t.Error("tree with one child should not be full")
	}
}

func TestBSTIsComplete(t *testing.T) {
	tree := BSTFromSortedSlice([]int{1, 2, 3, 4, 5, 6, 7})
	if !tree.IsComplete() {
		t.Error("perfect tree should be complete")
	}
}

func TestBSTPathSum(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4} {
		tree.Insert(v)
	}
	if !BSTPathSum(tree.Root, 9) {
		t.Error("expected path sum 9 (5->3->1=9)")
	}
	if BSTPathSum(tree.Root, 100) {
		t.Error("should not find path sum 100")
	}
}

func TestBSTKthLargest(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	val, ok := tree.KthLargest(1)
	if !ok || val != 8 {
		t.Errorf("expected 8, got %d", val)
	}
	val, ok = tree.KthLargest(3)
	if !ok || val != 6 {
		t.Errorf("expected 6, got %d", val)
	}
}

func TestBSTRangeCount(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	count := tree.RangeCount(3, 7)
	if count != 5 {
		t.Errorf("expected 5 in range [3,7], got %d", count)
	}
}

func TestBSTMorrisInorder(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7, 1, 4, 6, 8} {
		tree.Insert(v)
	}
	result := tree.MorrisInorder()
	expected := []int{1, 3, 4, 5, 6, 7, 8}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}

func TestBSTSerialize(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7} {
		tree.Insert(v)
	}
	s := tree.Serialize()
	if len(s) != 3 {
		t.Errorf("expected 3 elements, got %d", len(s))
	}
}

func TestBSTClear(t *testing.T) {
	tree := NewBST[int]()
	tree.Insert(1)
	tree.Insert(2)
	tree.Clear()
	if !tree.IsEmpty() {
		t.Error("should be empty after clear")
	}
}

func TestBSTFromPreorder(t *testing.T) {
	tree := BSTFromPreorder([]int{5, 3, 1, 4, 7, 6, 8})
	if !tree.IsValidBST() {
		t.Error("should be valid BST")
	}
	if tree.Size() != 7 {
		t.Errorf("expected 7, got %d", tree.Size())
	}
}

func TestBSTHeight(t *testing.T) {
	tree := NewBST[int]()
	tree.Insert(5)
	if tree.Height() != 0 {
		t.Errorf("single node height should be 0, got %d", tree.Height())
	}

	tree.Insert(3)
	tree.Insert(7)
	if tree.Height() != 1 {
		t.Errorf("expected height 1, got %d", tree.Height())
	}
}

func TestBSTDeleteNonExistent(t *testing.T) {
	tree := NewBST[int]()
	tree.Insert(5)
	if tree.Delete(99) {
		t.Error("should not delete non-existent value")
	}
}

func TestBSTMinMaxEmpty(t *testing.T) {
	tree := NewBST[int]()
	_, ok := tree.Min()
	if ok {
		t.Error("min on empty tree should fail")
	}
	_, ok = tree.Max()
	if ok {
		t.Error("max on empty tree should fail")
	}
}

func TestBSTDuplicate(t *testing.T) {
	tree := NewBST[int]()
	first := tree.Insert(5)
	tree.Insert(3)
	tree.Insert(7)
	again := tree.Insert(5)
	if again != first {
		t.Fatal("duplicate insert should return the existing node")
	}
	if tree.Size() != 3 {
		t.Fatalf("size = %d, want 3", tree.Size())
	}
	if !tree.IsValidBST() {
		t.Fatal("duplicate insert should leave a valid BST")
	}
	if tree.Root != first || tree.Root.Left.Value != 3 || tree.Root.Right.Value != 7 {
		t.Fatalf("duplicate insert changed the tree shape: %+v", tree.Root)
	}
	got := tree.Inorder()
	want := []int{3, 5, 7}
	if len(got) != len(want) {
		t.Fatalf("inorder = %v, want %v", got, want)
	}
	for i := range want {
		if got[i] != want[i] {
			t.Fatalf("inorder = %v, want %v", got, want)
		}
	}
}

func TestBSTInvert(t *testing.T) {
	tree := NewBST[int]()
	for _, v := range []int{5, 3, 7} {
		tree.Insert(v)
	}
	tree.Invert()
	// After inversion, left should be 7, right should be 3
	if tree.Root.Left.Value != 7 || tree.Root.Right.Value != 3 {
		t.Error("inversion failed")
	}
}
