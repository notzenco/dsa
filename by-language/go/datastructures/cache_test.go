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
