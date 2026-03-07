package datastructures

import (
	"testing"
	"time"
)

// --- LRU Cache Tests ---

func TestLRUCache(t *testing.T) {
	cache := NewLRUCache[int, string](3)

	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	val, ok := cache.Get(1)
	if !ok || val != "one" {
		t.Errorf("expected 'one', got '%s'", val)
	}

	cache.Put(4, "four") // evicts 2 (least recently used)

	_, ok = cache.Get(2)
	if ok {
		t.Error("should not find 2 after eviction")
	}

	if !cache.Contains(1) || !cache.Contains(3) || !cache.Contains(4) {
		t.Error("should contain 1, 3, 4")
	}
}

func TestLRUCacheUpdate(t *testing.T) {
	cache := NewLRUCache[string, int](2)
	cache.Put("a", 1)
	cache.Put("b", 2)
	cache.Put("a", 10)

	val, _ := cache.Get("a")
	if val != 10 {
		t.Errorf("expected 10, got %d", val)
	}

	cache.Put("c", 3) // should evict "b", not "a"
	if cache.Contains("b") {
		t.Error("'b' should be evicted")
	}
	if !cache.Contains("a") {
		t.Error("'a' should still exist")
	}
}

func TestLRUCacheDelete(t *testing.T) {
	cache := NewLRUCache[int, int](3)
	cache.Put(1, 10)
	cache.Put(2, 20)

	if !cache.Delete(1) {
		t.Error("delete should succeed")
	}
	if cache.Contains(1) {
		t.Error("should not contain 1 after delete")
	}
	if cache.Size() != 1 {
		t.Errorf("expected size 1, got %d", cache.Size())
	}
}

func TestLRUCacheKeys(t *testing.T) {
	cache := NewLRUCache[int, int](3)
	cache.Put(1, 10)
	cache.Put(2, 20)
	cache.Put(3, 30)
	cache.Get(1) // move 1 to front

	keys := cache.Keys()
	if len(keys) != 3 {
		t.Errorf("expected 3 keys, got %d", len(keys))
	}
	// Most recent first
	if keys[0] != 1 {
		t.Errorf("expected first key 1, got %d", keys[0])
	}
}

// --- LFU Cache Tests ---

func TestLFUCache(t *testing.T) {
	cache := NewLFUCache[int, string](3)

	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	cache.Get(1) // freq(1)=2
	cache.Get(1) // freq(1)=3
	cache.Get(2) // freq(2)=2

	cache.Put(4, "four") // evicts 3 (least frequent)

	if cache.Contains(3) {
		t.Error("3 should be evicted (lowest freq)")
	}
	if !cache.Contains(1) || !cache.Contains(2) || !cache.Contains(4) {
		t.Error("1, 2, 4 should exist")
	}
}

func TestLFUCacheTieBreaking(t *testing.T) {
	cache := NewLFUCache[int, int](2)

	cache.Put(1, 10)
	cache.Put(2, 20)
	// Both have freq=1, LRU should break tie

	cache.Put(3, 30) // evicts 1 (oldest with freq=1)

	if cache.Contains(1) {
		t.Error("1 should be evicted")
	}
	if !cache.Contains(2) || !cache.Contains(3) {
		t.Error("2 and 3 should exist")
	}
}

func TestLFUCacheUpdate(t *testing.T) {
	cache := NewLFUCache[int, string](2)
	cache.Put(1, "one")
	cache.Put(1, "ONE")

	val, _ := cache.Get(1)
	if val != "ONE" {
		t.Errorf("expected 'ONE', got '%s'", val)
	}
	if cache.Size() != 1 {
		t.Errorf("expected size 1, got %d", cache.Size())
	}
}

func TestLFUCacheDelete(t *testing.T) {
	cache := NewLFUCache[int, int](3)
	cache.Put(1, 10)
	cache.Put(2, 20)

	if !cache.Delete(1) {
		t.Error("delete should succeed")
	}
	if cache.Contains(1) {
		t.Error("should not contain 1")
	}
}

// --- TTL Cache Tests ---

func TestTTLCache(t *testing.T) {
	cache := NewTTLCache[string, int](3, 100*time.Millisecond)

	cache.Put("a", 1)
	cache.Put("b", 2)

	val, ok := cache.Get("a")
	if !ok || val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	time.Sleep(150 * time.Millisecond)

	_, ok = cache.Get("a")
	if ok {
		t.Error("should expire after TTL")
	}
}

func TestTTLCacheEviction(t *testing.T) {
	cache := NewTTLCache[int, int](2, time.Second)

	cache.Put(1, 10)
	cache.Put(2, 20)
	cache.Get(1) // make 1 recently used

	cache.Put(3, 30) // evicts 2 (LRU)

	if cache.Contains(2) {
		t.Error("2 should be evicted")
	}
	if !cache.Contains(1) || !cache.Contains(3) {
		t.Error("1 and 3 should exist")
	}
}

func TestTTLCacheDelete(t *testing.T) {
	cache := NewTTLCache[string, int](3, time.Second)
	cache.Put("x", 10)

	if !cache.Delete("x") {
		t.Error("delete should succeed")
	}
	if cache.Contains("x") {
		t.Error("should not contain 'x'")
	}
}

func TestTTLCacheClear(t *testing.T) {
	cache := NewTTLCache[int, int](3, time.Second)
	cache.Put(1, 10)
	cache.Put(2, 20)
	cache.Clear()

	if cache.Size() != 0 {
		t.Errorf("expected size 0, got %d", cache.Size())
	}
}

// --- LRU Cache Extended Tests ---

func TestLRUCachePeek(t *testing.T) {
	cache := NewLRUCache[int, string](2)
	cache.Put(1, "one")
	cache.Put(2, "two")

	val, ok := cache.Peek(1)
	if !ok || val != "one" {
		t.Errorf("expected 'one', got '%s'", val)
	}

	// Peek shouldn't move 1 to front, so adding 3 should evict 1
	cache.Put(3, "three")
	_, ok = cache.Peek(1)
	if ok {
		t.Error("1 should be evicted since Peek doesn't update order")
	}
}

func TestLRUCacheResize(t *testing.T) {
	cache := NewLRUCache[int, string](5)
	for i := 1; i <= 5; i++ {
		cache.Put(i, "val")
	}

	cache.Resize(3)
	if cache.Size() != 3 {
		t.Errorf("expected size 3 after resize, got %d", cache.Size())
	}
	if cache.Capacity() != 3 {
		t.Errorf("expected capacity 3, got %d", cache.Capacity())
	}
	// LRU items (1, 2) should be evicted
	if cache.Contains(1) || cache.Contains(2) {
		t.Error("oldest items should be evicted on resize")
	}
}

func TestLRUCacheGetLRUAndMRUKey(t *testing.T) {
	cache := NewLRUCache[int, string](3)
	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	mru, ok := cache.GetMRUKey()
	if !ok || mru != 3 {
		t.Errorf("expected MRU key 3, got %d", mru)
	}

	lru, ok := cache.GetLRUKey()
	if !ok || lru != 1 {
		t.Errorf("expected LRU key 1, got %d", lru)
	}

	cache.Get(1) // Move 1 to front
	mru, _ = cache.GetMRUKey()
	if mru != 1 {
		t.Errorf("expected MRU key 1 after Get, got %d", mru)
	}
}

func TestLRUCacheValues(t *testing.T) {
	cache := NewLRUCache[int, string](3)
	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	values := cache.Values()
	if len(values) != 3 {
		t.Errorf("expected 3 values, got %d", len(values))
	}
}

func TestLRUCacheItems(t *testing.T) {
	cache := NewLRUCache[int, string](3)
	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	items := cache.Items()
	if len(items) != 3 {
		t.Errorf("expected 3 items, got %d", len(items))
	}
}

func TestLRUCacheForEach(t *testing.T) {
	cache := NewLRUCache[int, int](3)
	cache.Put(1, 10)
	cache.Put(2, 20)
	cache.Put(3, 30)

	sum := 0
	cache.ForEach(func(k int, v int) {
		sum += v
	})
	if sum != 60 {
		t.Errorf("expected sum 60, got %d", sum)
	}
}

func TestLRUCacheGetOrPut(t *testing.T) {
	cache := NewLRUCache[string, int](3)
	cache.Put("a", 1)

	val := cache.GetOrPut("a", 99)
	if val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	val = cache.GetOrPut("b", 99)
	if val != 99 {
		t.Errorf("expected 99, got %d", val)
	}
}

func TestLRUCachePutIfAbsent(t *testing.T) {
	cache := NewLRUCache[int, int](3)
	cache.Put(1, 10)

	if cache.PutIfAbsent(1, 99) {
		t.Error("PutIfAbsent should return false for existing key")
	}
	if !cache.PutIfAbsent(2, 20) {
		t.Error("PutIfAbsent should return true for new key")
	}
}

func TestLRUCacheOldestItems(t *testing.T) {
	cache := NewLRUCache[int, string](5)
	cache.Put(1, "one")
	cache.Put(2, "two")
	cache.Put(3, "three")

	oldest := cache.OldestItems(2)
	if len(oldest) != 2 {
		t.Errorf("expected 2 oldest items, got %d", len(oldest))
	}
	if oldest[0].Key != 1 {
		t.Errorf("expected oldest key 1, got %d", oldest[0].Key)
	}
}

func TestLRUCacheEmpty(t *testing.T) {
	cache := NewLRUCache[int, int](3)

	_, ok := cache.Get(1)
	if ok {
		t.Error("should not find in empty cache")
	}

	_, ok = cache.Peek(1)
	if ok {
		t.Error("should not peek in empty cache")
	}

	_, ok = cache.GetLRUKey()
	if ok {
		t.Error("should not get LRU key from empty cache")
	}
}

// --- LFU Cache Extended Tests ---

func TestLFUCacheCapacityZero(t *testing.T) {
	cache := NewLFUCache[int, int](0)
	cache.Put(1, 10)
	if cache.Size() != 0 {
		t.Error("cache with 0 capacity should not store items")
	}
}

func TestLFUCacheFrequencyTracking(t *testing.T) {
	cache := NewLFUCache[int, int](3)
	cache.Put(1, 10)
	cache.Put(2, 20)
	cache.Put(3, 30)

	// Access 1 three times, 2 two times, 3 once
	cache.Get(1)
	cache.Get(1)
	cache.Get(2)

	// Adding new item should evict 3 (lowest frequency)
	cache.Put(4, 40)
	if cache.Contains(3) {
		t.Error("3 should be evicted (lowest frequency)")
	}

	// Adding another should evict 4 (lowest frequency, freq=1)
	cache.Put(5, 50)
	if cache.Contains(4) {
		t.Error("4 should be evicted (lowest frequency)")
	}
	if !cache.Contains(1) {
		t.Error("1 should remain (highest frequency)")
	}
	if !cache.Contains(2) {
		t.Error("2 should remain (freq=2)")
	}
}

func TestLFUCacheContainsAndSize(t *testing.T) {
	cache := NewLFUCache[string, int](3)
	cache.Put("a", 1)
	cache.Put("b", 2)

	if !cache.Contains("a") || !cache.Contains("b") {
		t.Error("should contain both keys")
	}
	if cache.Contains("c") {
		t.Error("should not contain 'c'")
	}
	if cache.Size() != 2 {
		t.Errorf("expected size 2, got %d", cache.Size())
	}
	if cache.Capacity() != 3 {
		t.Errorf("expected capacity 3, got %d", cache.Capacity())
	}
}
