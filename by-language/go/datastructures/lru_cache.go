package datastructures

// lruNode is a node in the LRU cache's doubly linked list.
type lruNode[K comparable, V any] struct {
	Key  K
	Val  V
	Prev *lruNode[K, V]
	Next *lruNode[K, V]
}

// LRUCache is a Least Recently Used cache.
type LRUCache[K comparable, V any] struct {
	capacity int
	cache    map[K]*lruNode[K, V]
	head     *lruNode[K, V] // dummy head
	tail     *lruNode[K, V] // dummy tail
}

func NewLRUCache[K comparable, V any](capacity int) *LRUCache[K, V] {
	head := &lruNode[K, V]{}
	tail := &lruNode[K, V]{}
	head.Next = tail
	tail.Prev = head
	return &LRUCache[K, V]{
		capacity: capacity,
		cache:    make(map[K]*lruNode[K, V]),
		head:     head,
		tail:     tail,
	}
}

func (c *LRUCache[K, V]) Get(key K) (V, bool) {
	node, ok := c.cache[key]
	if !ok {
		var zero V
		return zero, false
	}
	c.moveToFront(node)
	return node.Val, true
}

func (c *LRUCache[K, V]) Put(key K, value V) {
	if node, ok := c.cache[key]; ok {
		node.Val = value
		c.moveToFront(node)
		return
	}

	if len(c.cache) >= c.capacity {
		c.evict()
	}

	node := &lruNode[K, V]{Key: key, Val: value}
	c.cache[key] = node
	c.addToFront(node)
}

func (c *LRUCache[K, V]) Delete(key K) bool {
	node, ok := c.cache[key]
	if !ok {
		return false
	}
	c.removeNode(node)
	delete(c.cache, key)
	return true
}

func (c *LRUCache[K, V]) Contains(key K) bool {
	_, ok := c.cache[key]
	return ok
}

func (c *LRUCache[K, V]) Size() int {
	return len(c.cache)
}

func (c *LRUCache[K, V]) Capacity() int {
	return c.capacity
}

func (c *LRUCache[K, V]) Clear() {
	c.cache = make(map[K]*lruNode[K, V])
	c.head.Next = c.tail
	c.tail.Prev = c.head
}

func (c *LRUCache[K, V]) Keys() []K {
	keys := make([]K, 0, len(c.cache))
	node := c.head.Next
	for node != c.tail {
		keys = append(keys, node.Key)
		node = node.Next
	}
	return keys
}

func (c *LRUCache[K, V]) addToFront(node *lruNode[K, V]) {
	node.Prev = c.head
	node.Next = c.head.Next
	c.head.Next.Prev = node
	c.head.Next = node
}

func (c *LRUCache[K, V]) removeNode(node *lruNode[K, V]) {
	node.Prev.Next = node.Next
	node.Next.Prev = node.Prev
}

func (c *LRUCache[K, V]) moveToFront(node *lruNode[K, V]) {
	c.removeNode(node)
	c.addToFront(node)
}

func (c *LRUCache[K, V]) evict() {
	lru := c.tail.Prev
	if lru == c.head {
		return
	}
	c.removeNode(lru)
	delete(c.cache, lru.Key)
}
