package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.Optional;

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
}
