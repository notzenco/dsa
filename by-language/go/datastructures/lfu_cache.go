package datastructures

// lfuNode is a node in the LFU cache.
type lfuNode[K comparable, V any] struct {
	Key  K
	Val  V
	Freq int
	Prev *lfuNode[K, V]
	Next *lfuNode[K, V]
}

// freqList is a doubly linked list for nodes of the same frequency.
type freqList[K comparable, V any] struct {
	head *lfuNode[K, V]
	tail *lfuNode[K, V]
	size int
}

func newFreqList[K comparable, V any]() *freqList[K, V] {
	head := &lfuNode[K, V]{}
	tail := &lfuNode[K, V]{}
	head.Next = tail
	tail.Prev = head
	return &freqList[K, V]{head: head, tail: tail}
}

func (fl *freqList[K, V]) addToFront(node *lfuNode[K, V]) {
	node.Prev = fl.head
	node.Next = fl.head.Next
	fl.head.Next.Prev = node
	fl.head.Next = node
	fl.size++
}

func (fl *freqList[K, V]) removeNode(node *lfuNode[K, V]) {
	node.Prev.Next = node.Next
	node.Next.Prev = node.Prev
	fl.size--
}

func (fl *freqList[K, V]) removeLast() *lfuNode[K, V] {
	if fl.size == 0 {
		return nil
	}
	node := fl.tail.Prev
	fl.removeNode(node)
	return node
}

func (fl *freqList[K, V]) isEmpty() bool {
	return fl.size == 0
}

// LFUCache is a Least Frequently Used cache.
type LFUCache[K comparable, V any] struct {
	capacity int
	keyMap   map[K]*lfuNode[K, V]
	freqMap  map[int]*freqList[K, V]
	minFreq  int
}

func NewLFUCache[K comparable, V any](capacity int) *LFUCache[K, V] {
	return &LFUCache[K, V]{
		capacity: capacity,
		keyMap:   make(map[K]*lfuNode[K, V]),
		freqMap:  make(map[int]*freqList[K, V]),
	}
}

func (c *LFUCache[K, V]) Get(key K) (V, bool) {
	node, ok := c.keyMap[key]
	if !ok {
		var zero V
		return zero, false
	}
	c.incrementFreq(node)
	return node.Val, true
}

func (c *LFUCache[K, V]) Put(key K, value V) {
	if c.capacity <= 0 {
		return
	}

	if node, ok := c.keyMap[key]; ok {
		node.Val = value
		c.incrementFreq(node)
		return
	}

	if len(c.keyMap) >= c.capacity {
		c.evict()
	}

	node := &lfuNode[K, V]{Key: key, Val: value, Freq: 1}
	c.keyMap[key] = node

	if _, ok := c.freqMap[1]; !ok {
		c.freqMap[1] = newFreqList[K, V]()
	}
	c.freqMap[1].addToFront(node)
	c.minFreq = 1
}

func (c *LFUCache[K, V]) Delete(key K) bool {
	node, ok := c.keyMap[key]
	if !ok {
		return false
	}
	fl := c.freqMap[node.Freq]
	fl.removeNode(node)
	if fl.isEmpty() {
		delete(c.freqMap, node.Freq)
	}
	delete(c.keyMap, key)
	return true
}

func (c *LFUCache[K, V]) Contains(key K) bool {
	_, ok := c.keyMap[key]
	return ok
}

func (c *LFUCache[K, V]) Size() int {
	return len(c.keyMap)
}

func (c *LFUCache[K, V]) Capacity() int {
	return c.capacity
}

func (c *LFUCache[K, V]) incrementFreq(node *lfuNode[K, V]) {
	fl := c.freqMap[node.Freq]
	fl.removeNode(node)
	if fl.isEmpty() {
		delete(c.freqMap, node.Freq)
		if c.minFreq == node.Freq {
			c.minFreq++
		}
	}

	node.Freq++
	if _, ok := c.freqMap[node.Freq]; !ok {
		c.freqMap[node.Freq] = newFreqList[K, V]()
	}
	c.freqMap[node.Freq].addToFront(node)
}

func (c *LFUCache[K, V]) evict() {
	fl := c.freqMap[c.minFreq]
	if fl == nil {
		return
	}
	node := fl.removeLast()
	if node == nil {
		return
	}
	if fl.isEmpty() {
		delete(c.freqMap, c.minFreq)
	}
	delete(c.keyMap, node.Key)
}
