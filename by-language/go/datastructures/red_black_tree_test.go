package datastructures

import "testing"

func TestRedBlackTree(t *testing.T) {
	tree := NewRedBlackTree[int]()

	for _, v := range []int{10, 20, 30, 15, 25, 5, 1} {
		tree.Insert(v)
	}

	if tree.Size() != 7 {
		t.Errorf("expected size 7, got %d", tree.Size())
	}

	if !tree.IsValid() {
		t.Error("tree should be valid")
	}

	if !tree.Search(15) {
		t.Error("should find 15")
	}
	if tree.Search(99) {
		t.Error("should not find 99")
	}
}

func TestRBTreeInorder(t *testing.T) {
	tree := NewRedBlackTree[int]()
	values := []int{5, 3, 7, 1, 4, 6, 8}
	for _, v := range values {
		tree.Insert(v)
	}

	inorder := tree.Inorder()
	expected := []int{1, 3, 4, 5, 6, 7, 8}
	for i, v := range expected {
		if inorder[i] != v {
			t.Errorf("inorder[%d]: expected %d, got %d", i, v, inorder[i])
		}
	}
}

func TestRBTreeDelete(t *testing.T) {
	tree := NewRedBlackTree[int]()
	values := []int{10, 20, 30, 15, 25, 5, 1}
	for _, v := range values {
		tree.Insert(v)
	}

	tree.Delete(20)
	if tree.Search(20) {
		t.Error("should not find 20 after delete")
	}
	if !tree.IsValid() {
		t.Error("tree should be valid after delete")
	}

	tree.Delete(10)
	tree.Delete(5)
	if !tree.IsValid() {
		t.Error("tree should be valid after multiple deletes")
	}

	if tree.Size() != 4 {
		t.Errorf("expected size 4, got %d", tree.Size())
	}
}

func TestRBTreeSequentialInsert(t *testing.T) {
	tree := NewRedBlackTree[int]()

	for i := 1; i <= 100; i++ {
		tree.Insert(i)
	}

	if !tree.IsValid() {
		t.Error("tree should be valid after sequential inserts")
	}

	if tree.Size() != 100 {
		t.Errorf("expected size 100, got %d", tree.Size())
	}

	inorder := tree.Inorder()
	for i, v := range inorder {
		if v != i+1 {
			t.Errorf("inorder[%d]: expected %d, got %d", i, i+1, v)
			break
		}
	}
}

func TestRBTreeDeleteAll(t *testing.T) {
	tree := NewRedBlackTree[int]()
	values := []int{5, 3, 7, 1, 4, 6, 8, 2, 9, 10}
	for _, v := range values {
		tree.Insert(v)
	}

	for _, v := range values {
		tree.Delete(v)
		if !tree.IsValid() {
			t.Errorf("tree should be valid after deleting %d", v)
		}
	}

	if tree.Size() != 0 {
		t.Errorf("expected size 0, got %d", tree.Size())
	}
}

func TestRBTreeDuplicate(t *testing.T) {
	tree := NewRedBlackTree[int]()
	tree.Insert(5)
	tree.Insert(5)
	if tree.Size() != 1 {
		t.Errorf("expected size 1 for duplicate, got %d", tree.Size())
	}
}

func TestRBTreeMinMax(t *testing.T) {
	tree := NewRedBlackTree[int]()
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

func TestRBTreeMinMaxEmpty(t *testing.T) {
	tree := NewRedBlackTree[int]()
	_, ok := tree.Min()
	if ok {
		t.Error("min on empty should fail")
	}
	_, ok = tree.Max()
	if ok {
		t.Error("max on empty should fail")
	}
}

func TestRBTreeContains(t *testing.T) {
	tree := NewRedBlackTree[int]()
	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(7)

	if !tree.Contains(5) {
		t.Error("should contain 5")
	}
	if tree.Contains(99) {
		t.Error("should not contain 99")
	}
}

func TestRBTreeDeleteNonExistent(t *testing.T) {
	tree := NewRedBlackTree[int]()
	tree.Insert(5)
	if tree.Delete(99) {
		t.Error("delete non-existent should fail")
	}
}

func TestRBTreeStressInsertDelete(t *testing.T) {
	tree := NewRedBlackTree[int]()

	// Insert 200 values
	for i := 0; i < 200; i++ {
		tree.Insert(i)
	}
	if !tree.IsValid() {
		t.Error("should be valid after 200 inserts")
	}
	if tree.Size() != 200 {
		t.Errorf("expected 200, got %d", tree.Size())
	}

	// Delete even values
	for i := 0; i < 200; i += 2 {
		tree.Delete(i)
	}
	if !tree.IsValid() {
		t.Error("should be valid after deleting even values")
	}
	if tree.Size() != 100 {
		t.Errorf("expected 100, got %d", tree.Size())
	}
}

func TestRBTreeReverseInsert(t *testing.T) {
	tree := NewRedBlackTree[int]()
	for i := 50; i > 0; i-- {
		tree.Insert(i)
	}
	if !tree.IsValid() {
		t.Error("should be valid after reverse inserts")
	}
	inorder := tree.Inorder()
	for i := 0; i < 50; i++ {
		if inorder[i] != i+1 {
			t.Errorf("inorder[%d]: expected %d, got %d", i, i+1, inorder[i])
			break
		}
	}
}

func TestRBTreeSingleElement(t *testing.T) {
	tree := NewRedBlackTree[int]()
	tree.Insert(42)
	if !tree.IsValid() {
		t.Error("single element tree should be valid")
	}
	tree.Delete(42)
	if tree.Size() != 0 {
		t.Error("should be empty")
	}
}

func TestRBTreeEmpty(t *testing.T) {
	tree := NewRedBlackTree[int]()
	if !tree.IsValid() {
		t.Error("empty tree should be valid")
	}
	if tree.Size() != 0 {
		t.Error("empty tree should have size 0")
	}
	if !tree.IsEmpty() {
		t.Error("new tree should be empty")
	}
}
