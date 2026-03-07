package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.Optional;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;

class TtlCacheTest {

    /**
     * Helper to create a TtlCache with a controllable clock.
     */
    private static AtomicLong createClock() {
        return new AtomicLong(0);
    }

    @Test
    void putAndGet() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
    }

    @Test
    void getNonExistent() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        assertEquals(Optional.empty(), cache.get("missing"));
    }

    @Test
    void putOverwrite() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        cache.put("a", 10);
        assertEquals(Optional.of(10), cache.get("a"));
    }

    @Test
    void expirationWithDefaultTtl() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        assertEquals(Optional.of(1), cache.get("a"));

        // Advance time past TTL
        clock.set(1000);
        assertEquals(Optional.empty(), cache.get("a"));
    }

    @Test
    void expirationWithCustomTtl() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1, 500); // custom TTL of 500ms
        cache.put("b", 2);      // default TTL of 1000ms

        clock.set(500);
        assertEquals(Optional.empty(), cache.get("a")); // expired
        assertEquals(Optional.of(2), cache.get("b"));   // still valid

        clock.set(1000);
        assertEquals(Optional.empty(), cache.get("b")); // now expired
    }

    @Test
    void notExpiredJustBeforeTtl() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        clock.set(999);
        assertEquals(Optional.of(1), cache.get("a"));
    }

    @Test
    void cleanup() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);

        clock.set(1000);
        cache.cleanup();
        assertEquals(0, cache.size());
    }

    @Test
    void cleanupPartial() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        clock.set(0);
        cache.put("a", 1, 500);
        cache.put("b", 2, 1500);

        clock.set(500);
        cache.cleanup();
        assertEquals(1, cache.size());
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
    }

    @Test
    void sizeReflectsExpiration() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(2, cache.size());

        clock.set(1000);
        assertEquals(0, cache.size()); // size() calls cleanup
    }

    @Test
    void containsKey() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        assertTrue(cache.containsKey("a"));
        assertFalse(cache.containsKey("b"));

        clock.set(1000);
        assertFalse(cache.containsKey("a")); // expired
    }

    @Test
    void clear() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.clear();
        assertEquals(0, cache.size());
        assertEquals(Optional.empty(), cache.get("a"));
    }

    @Test
    void capacityEviction() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(2, 1000, clock::get);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3); // should evict oldest entry ("a")
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
        assertEquals(2, cache.size());
    }

    @Test
    void capacityEvictionPrefersExpired() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(2, 1000, clock::get);
        cache.put("a", 1, 500);
        cache.put("b", 2, 1500);

        clock.set(600);
        // "a" is expired, cleanup happens before eviction
        cache.put("c", 3);
        assertEquals(2, cache.size());
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void invalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new TtlCache<>(0, 1000));
        assertThrows(IllegalArgumentException.class, () -> new TtlCache<>(-1, 1000));
    }

    @Test
    void invalidTtl() {
        assertThrows(IllegalArgumentException.class, () -> new TtlCache<>(10, 0));
        assertThrows(IllegalArgumentException.class, () -> new TtlCache<>(10, -100));
    }

    @Test
    void putRefreshesTtl() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(10, 1000, clock::get);
        cache.put("a", 1);

        clock.set(800);
        cache.put("a", 2); // refresh TTL from time 800

        clock.set(1000);
        // Old TTL would have expired, but refreshed TTL is 800+1000=1800
        assertEquals(Optional.of(2), cache.get("a"));

        clock.set(1800);
        assertEquals(Optional.empty(), cache.get("a"));
    }

    @Test
    void manyEntriesWithExpiration() {
        var clock = createClock();
        var cache = new TtlCache<Integer, Integer>(100, 1000, clock::get);
        for (int i = 0; i < 50; i++) {
            cache.put(i, i * 10);
        }
        assertEquals(50, cache.size());

        clock.set(500);
        // Add more with different TTL
        for (int i = 50; i < 100; i++) {
            cache.put(i, i * 10, 2000);
        }

        clock.set(1000);
        cache.cleanup();
        // First 50 should be expired, last 50 should still be valid
        for (int i = 0; i < 50; i++) {
            assertEquals(Optional.empty(), cache.get(i));
        }
        for (int i = 50; i < 100; i++) {
            assertEquals(Optional.of(i * 10), cache.get(i));
        }
    }

    @Test
    void capacityOneWithExpiration() {
        var clock = createClock();
        var cache = new TtlCache<String, Integer>(1, 1000, clock::get);
        cache.put("a", 1);
        assertEquals(Optional.of(1), cache.get("a"));

        cache.put("b", 2); // evicts "a"
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));

        clock.set(1000);
        assertEquals(Optional.empty(), cache.get("b")); // expired
    }
}
