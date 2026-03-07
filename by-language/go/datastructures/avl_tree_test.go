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
