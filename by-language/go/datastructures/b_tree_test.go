package datastructures

import (
	"sort"
	"testing"
)

func TestBTree(t *testing.T) {
	bt := NewBTree[int, string](2)

	bt.Insert(10, "ten")
	bt.Insert(20, "twenty")
	bt.Insert(5, "five")
	bt.Insert(6, "six")
	bt.Insert(12, "twelve")
	bt.Insert(30, "thirty")
	bt.Insert(7, "seven")
	bt.Insert(17, "seventeen")

	if bt.Size() != 8 {
		t.Errorf("expected size 8, got %d", bt.Size())
	}

	val, ok := bt.Search(12)
	if !ok || val != "twelve" {
		t.Errorf("expected 'twelve', got '%s'", val)
	}

	_, ok = bt.Search(99)
	if ok {
		t.Error("should not find 99")
	}
}

func TestBTreeInorder(t *testing.T) {
	bt := NewBTree[int, int](2)
	values := []int{10, 20, 5, 6, 12, 30, 7, 17}
	for _, v := range values {
		bt.Insert(v, v)
	}

	inorder := bt.Inorder()
	sort.Ints(values)
	for i, v := range values {
		if inorder[i] != v {
			t.Errorf("inorder[%d]: expected %d, got %d", i, v, inorder[i])
		}
	}
}

func TestBTreeDelete(t *testing.T) {
	bt := NewBTree[int, int](2)
	values := []int{1, 3, 7, 10, 11, 13, 14, 15, 18, 16, 19, 24, 25, 26, 21, 4, 5, 20, 22, 2, 17, 12, 6}
	for _, v := range values {
		bt.Insert(v, v)
	}

	// Delete various keys
	for _, key := range []int{6, 13, 7, 4, 2, 16} {
		if !bt.Delete(key) {
			t.Errorf("delete %d should succeed", key)
		}
		if bt.Contains(key) {
			t.Errorf("should not contain %d after delete", key)
		}
	}

	// Verify remaining keys are still sorted
	inorder := bt.Inorder()
	for i := 1; i < len(inorder); i++ {
		if inorder[i] <= inorder[i-1] {
			t.Errorf("inorder not sorted at index %d: %v", i, inorder)
			break
		}
	}
}

func TestBTreeDeleteNonExistent(t *testing.T) {
	bt := NewBTree[int, int](2)
	bt.Insert(1, 1)
	bt.Insert(2, 2)

	if bt.Delete(99) {
		t.Error("delete non-existent should return false")
	}
}

func TestBTreeUpdate(t *testing.T) {
	bt := NewBTree[int, string](2)
	bt.Insert(1, "one")
	bt.Insert(1, "ONE")

	val, _ := bt.Search(1)
	if val != "ONE" {
		t.Errorf("expected 'ONE' after update, got '%s'", val)
	}
	if bt.Size() != 1 {
		t.Errorf("expected size 1, got %d", bt.Size())
	}
}

func TestBTreeLargeScale(t *testing.T) {
	bt := NewBTree[int, int](3)

	n := 200
	for i := 0; i < n; i++ {
		bt.Insert(i, i*10)
	}

	if bt.Size() != n {
		t.Errorf("expected size %d, got %d", n, bt.Size())
	}

	for i := 0; i < n; i++ {
		val, ok := bt.Search(i)
		if !ok || val != i*10 {
			t.Errorf("search %d: expected %d, got %d (ok=%v)", i, i*10, val, ok)
		}
	}

	// Delete first half
	for i := 0; i < n/2; i++ {
		bt.Delete(i)
	}

	if bt.Size() != n/2 {
		t.Errorf("expected size %d, got %d", n/2, bt.Size())
	}

	// Verify remaining
	inorder := bt.Inorder()
	if len(inorder) != n/2 {
		t.Errorf("expected %d elements in inorder, got %d", n/2, len(inorder))
	}
	for i := 1; i < len(inorder); i++ {
		if inorder[i] <= inorder[i-1] {
			t.Error("inorder not sorted")
			break
		}
	}
}
