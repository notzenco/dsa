package datastructures

import (
	"slices"
	"strconv"
	"strings"
	"testing"
)

func FuzzBST(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte{0, 5, 0, 3, 0, 7, 0, 5, 1, 3, 2, 7, 1, 9})
	f.Fuzz(func(t *testing.T, data []byte) {
		if len(data) > 96 {
			data = data[:96]
		}
		tree := NewBST[int]()
		ref := make(map[int]int)
		var trace []string
		for i := 0; i+1 < len(data); i += 2 {
			op := data[i] % 3
			key := int(data[i+1])
			switch op {
			case 0:
				tree.Insert(key)
				ref[key] = 0
				trace = append(trace, "insert "+strconv.Itoa(key))
			case 1:
				deleted := tree.Delete(key)
				_, exists := ref[key]
				if deleted != exists {
					t.Fatalf("BST delete %d = %v, exists %v; data %v after %s", key, deleted, exists, data, strings.Join(trace, ","))
				}
				delete(ref, key)
				trace = append(trace, "delete "+strconv.Itoa(key))
			default:
				trace = append(trace, "contains "+strconv.Itoa(key))
			}
			if !tree.IsValidBST() {
				t.Fatalf("BST invalid; data %v after %s", data, strings.Join(trace, ","))
			}
			checkOrderedMap(t, "BST", data, trace, tree.Size(), tree.Inorder(), tree.Contains, func(k int) (int, bool) {
				if tree.Contains(k) {
					return 0, true
				}
				return 0, false
			}, key, ref)
		}
	})
}

func FuzzAVL(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte{0, 5, 3, 5, 0, 1, 1, 5, 2, 1, 0, 9})
	f.Fuzz(func(t *testing.T, data []byte) {
		if len(data) > 96 {
			data = data[:96]
		}
		tree := NewAVLTree[int, int]()
		ref := make(map[int]int)
		var trace []string
		for i := 0; i+1 < len(data); i += 2 {
			raw := data[i]
			op := raw % 3
			key := int(data[i+1])
			switch op {
			case 0:
				tree.Insert(key, int(raw))
				ref[key] = int(raw)
				trace = append(trace, "insert "+strconv.Itoa(key)+"="+strconv.Itoa(int(raw)))
			case 1:
				deleted := tree.Delete(key)
				_, exists := ref[key]
				if deleted != exists {
					t.Fatalf("AVL delete %d = %v, exists %v; data %v after %s", key, deleted, exists, data, strings.Join(trace, ","))
				}
				delete(ref, key)
				trace = append(trace, "delete "+strconv.Itoa(key))
			default:
				trace = append(trace, "contains "+strconv.Itoa(key))
			}
			if !tree.IsBalanced() {
				t.Fatalf("AVL unbalanced; data %v after %s", data, strings.Join(trace, ","))
			}
			checkOrderedMap(t, "AVL", data, trace, tree.Size(), tree.Inorder(), tree.Contains, tree.Search, key, ref)
		}
	})
}

func FuzzRedBlack(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte{0, 4, 0, 2, 0, 6, 0, 4, 1, 2, 2, 6, 1, 8})
	f.Fuzz(func(t *testing.T, data []byte) {
		if len(data) > 96 {
			data = data[:96]
		}
		tree := NewRedBlackTree[int]()
		ref := make(map[int]int)
		var trace []string
		for i := 0; i+1 < len(data); i += 2 {
			op := data[i] % 3
			key := int(data[i+1])
			switch op {
			case 0:
				tree.Insert(key)
				ref[key] = 0
				trace = append(trace, "insert "+strconv.Itoa(key))
			case 1:
				deleted := tree.Delete(key)
				_, exists := ref[key]
				if deleted != exists {
					t.Fatalf("RB delete %d = %v, exists %v; data %v after %s", key, deleted, exists, data, strings.Join(trace, ","))
				}
				delete(ref, key)
				trace = append(trace, "delete "+strconv.Itoa(key))
			default:
				trace = append(trace, "contains "+strconv.Itoa(key))
			}
			if !tree.IsValid() {
				t.Fatalf("RB invalid; data %v after %s", data, strings.Join(trace, ","))
			}
			checkOrderedMap(t, "RB", data, trace, tree.Size(), tree.Inorder(), tree.Contains, func(k int) (int, bool) {
				if tree.Contains(k) {
					return 0, true
				}
				return 0, false
			}, key, ref)
		}
	})
}

func FuzzBTree(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte{0, 10, 0, 20, 0, 5, 3, 10, 1, 20, 2, 5, 0, 15, 1, 10})
	f.Fuzz(func(t *testing.T, data []byte) {
		if len(data) > 96 {
			data = data[:96]
		}
		for _, degree := range []int{2, 3, 5} {
			tree := NewBTree[int, int](degree)
			ref := make(map[int]int)
			var trace []string
			for i := 0; i+1 < len(data); i += 2 {
				raw := data[i]
				op := raw % 3
				key := int(data[i+1])
				switch op {
				case 0:
					tree.Insert(key, int(raw))
					ref[key] = int(raw)
					trace = append(trace, "insert "+strconv.Itoa(key)+"="+strconv.Itoa(int(raw)))
				case 1:
					deleted := tree.Delete(key)
					_, exists := ref[key]
					if deleted != exists {
						t.Fatalf("BTree t=%d delete %d = %v, exists %v; data %v after %s", degree, key, deleted, exists, data, strings.Join(trace, ","))
					}
					delete(ref, key)
					trace = append(trace, "delete "+strconv.Itoa(key))
				default:
					trace = append(trace, "contains "+strconv.Itoa(key))
				}
				checkOrderedMap(t, "BTree t="+strconv.Itoa(degree), data, trace, tree.Size(), tree.Inorder(), tree.Contains, tree.Search, key, ref)
			}
		}
	})
}

func FuzzSkipList(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte{0, 4, 3, 4, 0, 1, 0, 8, 1, 1, 2, 8, 1, 4})
	f.Fuzz(func(t *testing.T, data []byte) {
		if len(data) > 96 {
			data = data[:96]
		}
		list := NewSkipList[int, int]()
		ref := make(map[int]int)
		var trace []string
		for i := 0; i+1 < len(data); i += 2 {
			raw := data[i]
			op := raw % 3
			key := int(data[i+1])
			switch op {
			case 0:
				list.Insert(key, int(raw))
				ref[key] = int(raw)
				trace = append(trace, "insert "+strconv.Itoa(key)+"="+strconv.Itoa(int(raw)))
			case 1:
				deleted := list.Delete(key)
				_, exists := ref[key]
				if deleted != exists {
					t.Fatalf("SkipList delete %d = %v, exists %v; data %v after %s", key, deleted, exists, data, strings.Join(trace, ","))
				}
				delete(ref, key)
				trace = append(trace, "delete "+strconv.Itoa(key))
			default:
				trace = append(trace, "contains "+strconv.Itoa(key))
			}
			checkOrderedMap(t, "SkipList", data, trace, list.Size(), list.Keys(), list.Contains, list.Search, key, ref)
		}
	})
}

func checkOrderedMap(t *testing.T, name string, data []byte, trace []string, size int, inorder []int, contains func(int) bool, search func(int) (int, bool), probe int, ref map[int]int) {
	t.Helper()
	done := strings.Join(trace, ",")
	if size != len(ref) {
		t.Fatalf("%s size=%d want %d; data %v after %s", name, size, len(ref), data, done)
	}
	want := make([]int, 0, len(ref))
	for k, v := range ref {
		want = append(want, k)
		got, ok := search(k)
		if !ok || got != v {
			t.Fatalf("%s search(%d)=(%d,%v) want %d; data %v after %s", name, k, got, ok, v, data, done)
		}
		if !contains(k) {
			t.Fatalf("%s contains(%d)=false; data %v after %s", name, k, data, done)
		}
	}
	slices.Sort(want)
	if !slices.Equal(inorder, want) {
		t.Fatalf("%s inorder=%v want %v; data %v after %s", name, inorder, want, data, done)
	}
	if _, exists := ref[probe]; contains(probe) != exists {
		t.Fatalf("%s contains(%d)=%v want %v; data %v after %s", name, probe, contains(probe), exists, data, done)
	}
}
