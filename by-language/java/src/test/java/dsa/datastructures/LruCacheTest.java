package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.*;

import static org.junit.jupiter.api.Assertions.*;

class LruCacheTest {

    @Test
    void putAndGet() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void getNonExistent() {
        var cache = new LruCache<String, Integer>(3);
        assertEquals(Optional.empty(), cache.get("missing"));
    }

    @Test
    void putOverwrite() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("a", 10);
        assertEquals(Optional.of(10), cache.get("a"));
        assertEquals(1, cache.size());
    }

    @Test
    void evictionOnCapacityExceeded() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.put("d", 4); // should evict "a" (least recently used)
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
        assertEquals(Optional.of(4), cache.get("d"));
        assertEquals(3, cache.size());
    }

    @Test
    void getUpdatesRecency() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.get("a"); // "a" is now most recently used
        cache.put("d", 4); // should evict "b" (now least recently used)
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
        assertEquals(Optional.of(4), cache.get("d"));
    }

    @Test
    void putUpdatesRecency() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.put("a", 10); // update "a", now most recently used
        cache.put("d", 4); // should evict "b"
        assertEquals(Optional.of(10), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
        assertEquals(Optional.of(4), cache.get("d"));
    }

    @Test
    void sizeAndCapacity() {
        var cache = new LruCache<String, Integer>(5);
        assertEquals(0, cache.size());
        assertEquals(5, cache.capacity());
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(2, cache.size());
        assertEquals(5, cache.capacity());
    }

    @Test
    void containsKey() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        assertTrue(cache.containsKey("a"));
        assertFalse(cache.containsKey("b"));
    }

    @Test
    void clear() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.clear();
        assertEquals(0, cache.size());
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
    }

    @Test
    void capacityOne() {
        var cache = new LruCache<String, Integer>(1);
        cache.put("a", 1);
        assertEquals(Optional.of(1), cache.get("a"));
        cache.put("b", 2);
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(1, cache.size());
    }

    @Test
    void invalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new LruCache<>(0));
        assertThrows(IllegalArgumentException.class, () -> new LruCache<>(-1));
    }

    @Test
    void multipleEvictions() {
        var cache = new LruCache<Integer, Integer>(2);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(3, 30); // evicts 1
        cache.put(4, 40); // evicts 2
        assertEquals(Optional.empty(), cache.get(1));
        assertEquals(Optional.empty(), cache.get(2));
        assertEquals(Optional.of(30), cache.get(3));
        assertEquals(Optional.of(40), cache.get(4));
    }

    @Test
    void evictionOrderWithAccesses() {
        var cache = new LruCache<Integer, Integer>(3);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(3, 30);
        cache.get(1); // access 1
        cache.get(2); // access 2
        // LRU order: 3, 1, 2 (3 is least recent)
        cache.put(4, 40); // evicts 3
        assertEquals(Optional.empty(), cache.get(3));
        assertEquals(Optional.of(10), cache.get(1));
        assertEquals(Optional.of(20), cache.get(2));
        assertEquals(Optional.of(40), cache.get(4));
    }

    @Test
    void largeNumberOfEntries() {
        var cache = new LruCache<Integer, Integer>(100);
        for (int i = 0; i < 200; i++) {
            cache.put(i, i * 10);
        }
        assertEquals(100, cache.size());
        // First 100 should be evicted
        for (int i = 0; i < 100; i++) {
            assertEquals(Optional.empty(), cache.get(i));
        }
        // Last 100 should be present
        for (int i = 100; i < 200; i++) {
            assertEquals(Optional.of(i * 10), cache.get(i));
        }
    }

    // --- peek tests ---

    @Test
    void peekReturnsValueWithoutAffectingOrder() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        // Peek at "a" should NOT move it to front
        assertEquals(Optional.of(1), cache.peek("a"));
        // Now insert "d" - should evict "a" since peek didn't update recency
        cache.put("d", 4);
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
    }

    @Test
    void peekNonExistent() {
        var cache = new LruCache<String, Integer>(3);
        assertEquals(Optional.empty(), cache.peek("missing"));
    }

    // --- remove tests ---

    @Test
    void removeExistingKey() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(Optional.of(1), cache.remove("a"));
        assertEquals(1, cache.size());
        assertFalse(cache.containsKey("a"));
        assertTrue(cache.containsKey("b"));
    }

    @Test
    void removeNonExistent() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        assertEquals(Optional.empty(), cache.remove("missing"));
        assertEquals(1, cache.size());
    }

    @Test
    void removeFreesCapacity() {
        var cache = new LruCache<String, Integer>(2);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.remove("a");
        cache.put("c", 3);
        // No eviction should have occurred; "b" and "c" remain
        assertEquals(2, cache.size());
        assertTrue(cache.containsKey("b"));
        assertTrue(cache.containsKey("c"));
    }

    // --- keys/values/entries tests ---

    @Test
    void keysInMruToLruOrder() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        // MRU to LRU: c, b, a
        assertEquals(List.of("c", "b", "a"), cache.keys());
    }

    @Test
    void valuesInMruToLruOrder() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        assertEquals(List.of(3, 2, 1), cache.values());
    }

    @Test
    void entriesInMruToLruOrder() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        var entries = cache.entries();
        assertEquals(3, entries.size());
        assertEquals("c", entries.get(0).getKey());
        assertEquals(3, entries.get(0).getValue());
        assertEquals("a", entries.get(2).getKey());
        assertEquals(1, entries.get(2).getValue());
    }

    @Test
    void keysReflectsAccessOrder() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.get("a"); // Move "a" to front
        assertEquals(List.of("a", "c", "b"), cache.keys());
    }

    // --- getOrDefault tests ---

    @Test
    void getOrDefaultReturnsValueAndUpdatesRecency() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        assertEquals(1, cache.getOrDefault("a", 99));
        // "a" should now be MRU
        assertEquals(List.of("a", "c", "b"), cache.keys());
    }

    @Test
    void getOrDefaultReturnsDefaultForMissing() {
        var cache = new LruCache<String, Integer>(3);
        assertEquals(99, cache.getOrDefault("missing", 99));
    }

    // --- putAll tests ---

    @Test
    void putAllAddsEntries() {
        var cache = new LruCache<String, Integer>(5);
        Map<String, Integer> entries = new LinkedHashMap<>();
        entries.put("a", 1);
        entries.put("b", 2);
        entries.put("c", 3);
        cache.putAll(entries);
        assertEquals(3, cache.size());
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void putAllEvictsWhenOverCapacity() {
        var cache = new LruCache<String, Integer>(2);
        cache.put("x", 0);
        Map<String, Integer> entries = new LinkedHashMap<>();
        entries.put("a", 1);
        entries.put("b", 2);
        cache.putAll(entries);
        assertEquals(2, cache.size());
        // "x" should have been evicted
        assertFalse(cache.containsKey("x"));
    }

    // --- resize tests ---

    @Test
    void resizeLargerNoEviction() {
        var cache = new LruCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.resize(5);
        assertEquals(5, cache.capacity());
        assertEquals(3, cache.size());
        // All entries should still be present
        assertTrue(cache.containsKey("a"));
        assertTrue(cache.containsKey("b"));
        assertTrue(cache.containsKey("c"));
    }

    @Test
    void resizeSmallerEvicts() {
        var cache = new LruCache<String, Integer>(5);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        cache.put("d", 4);
        cache.put("e", 5);
        cache.resize(2);
        assertEquals(2, cache.capacity());
        assertEquals(2, cache.size());
        // Only MRU entries "d" and "e" should remain
        assertTrue(cache.containsKey("d"));
        assertTrue(cache.containsKey("e"));
        assertFalse(cache.containsKey("a"));
        assertFalse(cache.containsKey("b"));
        assertFalse(cache.containsKey("c"));
    }

    @Test
    void resizeInvalid() {
        var cache = new LruCache<String, Integer>(3);
        assertThrows(IllegalArgumentException.class, () -> cache.resize(0));
        assertThrows(IllegalArgumentException.class, () -> cache.resize(-1));
    }

    // --- eviction count tests ---

    @Test
    void evictionCountTracking() {
        var cache = new LruCache<String, Integer>(2);
        assertEquals(0, cache.getEvictionCount());
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(0, cache.getEvictionCount());
        cache.put("c", 3); // evicts "a"
        assertEquals(1, cache.getEvictionCount());
        cache.put("d", 4); // evicts "b"
        assertEquals(2, cache.getEvictionCount());
    }

    @Test
    void evictionCountWithResize() {
        var cache = new LruCache<String, Integer>(5);
        for (int i = 0; i < 5; i++) {
            cache.put("k" + i, i);
        }
        assertEquals(0, cache.getEvictionCount());
        cache.resize(2);
        assertEquals(3, cache.getEvictionCount());
    }

    @Test
    void evictionCountNotAffectedByClear() {
        var cache = new LruCache<String, Integer>(2);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3); // evicts "a"
        assertEquals(1, cache.getEvictionCount());
        cache.clear();
        // evictionCount persists across clear
        assertEquals(1, cache.getEvictionCount());
    }
}
