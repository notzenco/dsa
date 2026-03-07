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

// Peek returns the value without marking as recently used.
func (c *LRUCache[K, V]) Peek(key K) (V, bool) {
	node, ok := c.cache[key]
	if !ok {
		var zero V
		return zero, false
	}
	return node.Val, true
}

// Resize changes the cache capacity, evicting LRU items if necessary.
func (c *LRUCache[K, V]) Resize(newCapacity int) {
	if newCapacity < 1 {
		return
	}
	for len(c.cache) > newCapacity {
		c.evict()
	}
	c.capacity = newCapacity
}

// GetLRUKey returns the least recently used key.
func (c *LRUCache[K, V]) GetLRUKey() (K, bool) {
	if len(c.cache) == 0 {
		var zero K
		return zero, false
	}
	return c.tail.Prev.Key, true
}

// GetMRUKey returns the most recently used key.
func (c *LRUCache[K, V]) GetMRUKey() (K, bool) {
	if len(c.cache) == 0 {
		var zero K
		return zero, false
	}
	return c.head.Next.Key, true
}

// Values returns all values from most to least recently used.
func (c *LRUCache[K, V]) Values() []V {
	vals := make([]V, 0, len(c.cache))
	node := c.head.Next
	for node != c.tail {
		vals = append(vals, node.Val)
		node = node.Next
	}
	return vals
}

// Items returns all key-value pairs from most to least recently used.
func (c *LRUCache[K, V]) Items() []struct {
	Key K
	Val V
} {
	items := make([]struct {
		Key K
		Val V
	}, 0, len(c.cache))
	node := c.head.Next
	for node != c.tail {
		items = append(items, struct {
			Key K
			Val V
		}{node.Key, node.Val})
		node = node.Next
	}
	return items
}

// ForEach iterates over all items from MRU to LRU.
func (c *LRUCache[K, V]) ForEach(fn func(K, V)) {
	node := c.head.Next
	for node != c.tail {
		fn(node.Key, node.Val)
		node = node.Next
	}
}

// GetOrPut returns existing value or inserts default.
func (c *LRUCache[K, V]) GetOrPut(key K, defaultVal V) V {
	if val, ok := c.Get(key); ok {
		return val
	}
	c.Put(key, defaultVal)
	return defaultVal
}

// PutIfAbsent inserts only if key doesn't exist, returns true if inserted.
func (c *LRUCache[K, V]) PutIfAbsent(key K, value V) bool {
	if c.Contains(key) {
		return false
	}
	c.Put(key, value)
	return true
}

// OldestItems returns up to n items from least to most recently used.
func (c *LRUCache[K, V]) OldestItems(n int) []struct {
	Key K
	Val V
} {
	items := make([]struct {
		Key K
		Val V
	}, 0, n)
	node := c.tail.Prev
	for node != c.head && len(items) < n {
		items = append(items, struct {
			Key K
			Val V
		}{node.Key, node.Val})
		node = node.Prev
	}
	return items
}
