package datastructures

import "time"

// ttlEntry holds a cached value with its expiry time.
type ttlEntry[V any] struct {
	Value     V
	ExpiresAt time.Time
}

// TTLCache is a cache with time-to-live expiration and LRU eviction.
type TTLCache[K comparable, V any] struct {
	capacity int
	ttl      time.Duration
	cache    map[K]*ttlCacheNode[K, V]
	head     *ttlCacheNode[K, V]
	tail     *ttlCacheNode[K, V]
}

type ttlCacheNode[K comparable, V any] struct {
	Key       K
	Value     V
	ExpiresAt time.Time
	Prev      *ttlCacheNode[K, V]
	Next      *ttlCacheNode[K, V]
}

func NewTTLCache[K comparable, V any](capacity int, ttl time.Duration) *TTLCache[K, V] {
	head := &ttlCacheNode[K, V]{}
	tail := &ttlCacheNode[K, V]{}
	head.Next = tail
	tail.Prev = head
	return &TTLCache[K, V]{
		capacity: capacity,
		ttl:      ttl,
		cache:    make(map[K]*ttlCacheNode[K, V]),
		head:     head,
		tail:     tail,
	}
}

func (c *TTLCache[K, V]) Get(key K) (V, bool) {
	c.cleanup()
	node, ok := c.cache[key]
	if !ok {
		var zero V
		return zero, false
	}
	if time.Now().After(node.ExpiresAt) {
		c.removeNode(node)
		delete(c.cache, key)
		var zero V
		return zero, false
	}
	c.moveToFront(node)
	return node.Value, true
}

func (c *TTLCache[K, V]) Put(key K, value V) {
	c.cleanup()

	if node, ok := c.cache[key]; ok {
		node.Value = value
		node.ExpiresAt = time.Now().Add(c.ttl)
		c.moveToFront(node)
		return
	}

	if len(c.cache) >= c.capacity {
		c.evictLRU()
	}

	node := &ttlCacheNode[K, V]{
		Key:       key,
		Value:     value,
		ExpiresAt: time.Now().Add(c.ttl),
	}
	c.cache[key] = node
	c.addToFront(node)
}

func (c *TTLCache[K, V]) Delete(key K) bool {
	node, ok := c.cache[key]
	if !ok {
		return false
	}
	c.removeNode(node)
	delete(c.cache, key)
	return true
}

func (c *TTLCache[K, V]) Contains(key K) bool {
	_, ok := c.Get(key)
	return ok
}

func (c *TTLCache[K, V]) Size() int {
	c.cleanup()
	return len(c.cache)
}

func (c *TTLCache[K, V]) Clear() {
	c.cache = make(map[K]*ttlCacheNode[K, V])
	c.head.Next = c.tail
	c.tail.Prev = c.head
}

func (c *TTLCache[K, V]) addToFront(node *ttlCacheNode[K, V]) {
	node.Prev = c.head
	node.Next = c.head.Next
	c.head.Next.Prev = node
	c.head.Next = node
}

func (c *TTLCache[K, V]) removeNode(node *ttlCacheNode[K, V]) {
	node.Prev.Next = node.Next
	node.Next.Prev = node.Prev
}

func (c *TTLCache[K, V]) moveToFront(node *ttlCacheNode[K, V]) {
	c.removeNode(node)
	c.addToFront(node)
}

func (c *TTLCache[K, V]) evictLRU() {
	lru := c.tail.Prev
	if lru == c.head {
		return
	}
	c.removeNode(lru)
	delete(c.cache, lru.Key)
}

func (c *TTLCache[K, V]) cleanup() {
	now := time.Now()
	node := c.tail.Prev
	for node != c.head {
		prev := node.Prev
		if now.After(node.ExpiresAt) {
			c.removeNode(node)
			delete(c.cache, node.Key)
		}
		node = prev
	}
}
