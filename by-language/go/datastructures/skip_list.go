package datastructures

import "math/rand"

const (
	skipListMaxLevel = 32
	skipListProb     = 0.5
)

// SkipListNode is a node in a skip list.
type SkipListNode[K Ordered, V any] struct {
	Key     K
	Value   V
	Forward []*SkipListNode[K, V]
}

// SkipList is a probabilistic data structure for ordered key-value pairs.
type SkipList[K Ordered, V any] struct {
	head  *SkipListNode[K, V]
	level int
	size  int
}

func NewSkipList[K Ordered, V any]() *SkipList[K, V] {
	head := &SkipListNode[K, V]{
		Forward: make([]*SkipListNode[K, V], skipListMaxLevel),
	}
	return &SkipList[K, V]{head: head, level: 0}
}

func (sl *SkipList[K, V]) Size() int     { return sl.size }
func (sl *SkipList[K, V]) IsEmpty() bool { return sl.size == 0 }

func (sl *SkipList[K, V]) randomLevel() int {
	level := 0
	for level < skipListMaxLevel-1 && rand.Float64() < skipListProb {
		level++
	}
	return level
}

func (sl *SkipList[K, V]) Insert(key K, value V) {
	update := make([]*SkipListNode[K, V], skipListMaxLevel)
	current := sl.head

	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < key {
			current = current.Forward[i]
		}
		update[i] = current
	}

	current = current.Forward[0]
	if current != nil && current.Key == key {
		current.Value = value
		return
	}

	newLevel := sl.randomLevel()
	if newLevel > sl.level {
		for i := sl.level + 1; i <= newLevel; i++ {
			update[i] = sl.head
		}
		sl.level = newLevel
	}

	node := &SkipListNode[K, V]{
		Key:     key,
		Value:   value,
		Forward: make([]*SkipListNode[K, V], newLevel+1),
	}

	for i := 0; i <= newLevel; i++ {
		node.Forward[i] = update[i].Forward[i]
		update[i].Forward[i] = node
	}
	sl.size++
}

func (sl *SkipList[K, V]) Search(key K) (V, bool) {
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < key {
			current = current.Forward[i]
		}
	}
	current = current.Forward[0]
	if current != nil && current.Key == key {
		return current.Value, true
	}
	var zero V
	return zero, false
}

func (sl *SkipList[K, V]) Delete(key K) bool {
	update := make([]*SkipListNode[K, V], skipListMaxLevel)
	current := sl.head

	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < key {
			current = current.Forward[i]
		}
		update[i] = current
	}

	current = current.Forward[0]
	if current == nil || current.Key != key {
		return false
	}

	for i := 0; i <= sl.level; i++ {
		if update[i].Forward[i] != current {
			break
		}
		update[i].Forward[i] = current.Forward[i]
	}

	for sl.level > 0 && sl.head.Forward[sl.level] == nil {
		sl.level--
	}
	sl.size--
	return true
}

func (sl *SkipList[K, V]) Contains(key K) bool {
	_, ok := sl.Search(key)
	return ok
}

func (sl *SkipList[K, V]) Min() (K, V, bool) {
	if sl.head.Forward[0] == nil {
		var zk K
		var zv V
		return zk, zv, false
	}
	n := sl.head.Forward[0]
	return n.Key, n.Value, true
}

func (sl *SkipList[K, V]) Max() (K, V, bool) {
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil {
			current = current.Forward[i]
		}
	}
	if current == sl.head {
		var zk K
		var zv V
		return zk, zv, false
	}
	return current.Key, current.Value, true
}

func (sl *SkipList[K, V]) Floor(key K) (K, V, bool) {
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key <= key {
			current = current.Forward[i]
		}
	}
	if current == sl.head {
		var zk K
		var zv V
		return zk, zv, false
	}
	return current.Key, current.Value, true
}

func (sl *SkipList[K, V]) Ceiling(key K) (K, V, bool) {
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < key {
			current = current.Forward[i]
		}
	}
	current = current.Forward[0]
	if current == nil {
		var zk K
		var zv V
		return zk, zv, false
	}
	return current.Key, current.Value, true
}

func (sl *SkipList[K, V]) Range(low, high K) []K {
	var result []K
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < low {
			current = current.Forward[i]
		}
	}
	current = current.Forward[0]
	for current != nil && current.Key <= high {
		result = append(result, current.Key)
		current = current.Forward[0]
	}
	return result
}

func (sl *SkipList[K, V]) Keys() []K {
	var result []K
	current := sl.head.Forward[0]
	for current != nil {
		result = append(result, current.Key)
		current = current.Forward[0]
	}
	return result
}

// Values returns all values in order.
func (sl *SkipList[K, V]) Values() []V {
	var result []V
	current := sl.head.Forward[0]
	for current != nil {
		result = append(result, current.Value)
		current = current.Forward[0]
	}
	return result
}

// Clear removes all elements.
func (sl *SkipList[K, V]) Clear() {
	for i := range sl.head.Forward {
		sl.head.Forward[i] = nil
	}
	sl.level = 0
	sl.size = 0
}

// Level returns the current maximum level.
func (sl *SkipList[K, V]) Level() int {
	return sl.level
}

// RangeCount returns count of keys in [low, high].
func (sl *SkipList[K, V]) RangeCount(low, high K) int {
	count := 0
	current := sl.head
	for i := sl.level; i >= 0; i-- {
		for current.Forward[i] != nil && current.Forward[i].Key < low {
			current = current.Forward[i]
		}
	}
	current = current.Forward[0]
	for current != nil && current.Key <= high {
		count++
		current = current.Forward[0]
	}
	return count
}

// ForEach applies a function to each key-value pair in order.
func (sl *SkipList[K, V]) ForEach(fn func(K, V)) {
	current := sl.head.Forward[0]
	for current != nil {
		fn(current.Key, current.Value)
		current = current.Forward[0]
	}
}

// GetOrDefault returns the value for key, or a default if not found.
func (sl *SkipList[K, V]) GetOrDefault(key K, defaultVal V) V {
	v, ok := sl.Search(key)
	if ok {
		return v
	}
	return defaultVal
}

// Head returns the first key-value pair.
func (sl *SkipList[K, V]) Head() (K, V, bool) {
	return sl.Min()
}

// Tail returns the last key-value pair.
func (sl *SkipList[K, V]) Tail() (K, V, bool) {
	return sl.Max()
}

// Pop removes and returns the minimum element.
func (sl *SkipList[K, V]) Pop() (K, V, bool) {
	if sl.head.Forward[0] == nil {
		var zk K
		var zv V
		return zk, zv, false
	}
	first := sl.head.Forward[0]
	k, v := first.Key, first.Value
	sl.Delete(k)
	return k, v, true
}

// Rank returns the 1-based rank of a key (position in sorted order).
func (sl *SkipList[K, V]) Rank(key K) int {
	rank := 0
	current := sl.head.Forward[0]
	for current != nil {
		if current.Key > key {
			break
		}
		rank++
		if current.Key == key {
			return rank
		}
		current = current.Forward[0]
	}
	return 0 // not found
}
