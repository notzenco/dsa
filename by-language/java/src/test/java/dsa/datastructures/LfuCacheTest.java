package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class LfuCacheTest {

    @Test
    void putAndGet() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void getNonExistent() {
        var cache = new LfuCache<String, Integer>(3);
        assertEquals(Optional.empty(), cache.get("missing"));
    }

    @Test
    void putOverwrite() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("a", 10);
        assertEquals(Optional.of(10), cache.get("a"));
        assertEquals(1, cache.size());
    }

    @Test
    void evictsLeastFrequentlyUsed() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);

        // Access "a" and "b" to increase their frequency
        cache.get("a"); // freq: a=2, b=1, c=1
        cache.get("b"); // freq: a=2, b=2, c=1

        // Insert new item, should evict "c" (lowest frequency)
        cache.put("d", 4);
        assertEquals(Optional.empty(), cache.get("c"));
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(4), cache.get("d"));
    }

    @Test
    void evictsLeastRecentAmongSameFrequency() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1); // freq=1
        cache.put("b", 2); // freq=1
        cache.put("c", 3); // freq=1
        // All have same frequency; "a" was inserted first (LRU among same freq)
        cache.put("d", 4); // should evict "a"
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
        assertEquals(Optional.of(4), cache.get("d"));
    }

    @Test
    void sizeAndCapacity() {
        var cache = new LfuCache<String, Integer>(5);
        assertEquals(0, cache.size());
        assertEquals(5, cache.capacity());
        cache.put("a", 1);
        cache.put("b", 2);
        assertEquals(2, cache.size());
    }

    @Test
    void containsKey() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        assertTrue(cache.containsKey("a"));
        assertFalse(cache.containsKey("b"));
    }

    @Test
    void clear() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.clear();
        assertEquals(0, cache.size());
        assertEquals(Optional.empty(), cache.get("a"));
        assertFalse(cache.containsKey("b"));
    }

    @Test
    void invalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new LfuCache<>(0));
        assertThrows(IllegalArgumentException.class, () -> new LfuCache<>(-1));
    }

    @Test
    void capacityOne() {
        var cache = new LfuCache<String, Integer>(1);
        cache.put("a", 1);
        assertEquals(Optional.of(1), cache.get("a"));
        cache.put("b", 2); // evicts "a"
        assertEquals(Optional.empty(), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
    }

    @Test
    void frequencyIncrementsOnGet() {
        var cache = new LfuCache<String, Integer>(2);
        cache.put("a", 1); // freq=1
        cache.put("b", 2); // freq=1
        cache.get("a");    // freq: a=2, b=1
        cache.put("c", 3); // evicts "b" (freq=1)
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void frequencyIncrementsOnPutUpdate() {
        var cache = new LfuCache<String, Integer>(2);
        cache.put("a", 1); // freq=1
        cache.put("b", 2); // freq=1
        cache.put("a", 10); // updates value and increases freq: a=2, b=1
        cache.put("c", 3); // evicts "b" (freq=1)
        assertEquals(Optional.of(10), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    @Test
    void multipleEvictions() {
        var cache = new LfuCache<Integer, Integer>(2);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.get(1); // freq: 1->2, 2->1
        cache.put(3, 30); // evicts 2 (freq=1)
        assertEquals(Optional.empty(), cache.get(2));
        cache.get(1); // freq: 1->3, 3->1
        cache.put(4, 40); // evicts 3 (freq=1)
        assertEquals(Optional.empty(), cache.get(3));
        assertEquals(Optional.of(10), cache.get(1));
        assertEquals(Optional.of(40), cache.get(4));
    }

    @Test
    void complexEvictionScenario() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);

        // Make frequencies: a=3, b=2, c=1
        cache.get("a");
        cache.get("a");
        cache.get("b");

        cache.put("d", 4); // evicts "c" (freq=1)
        assertEquals(Optional.empty(), cache.get("c"));

        // Now: a=3+1(from get above)=4, b=2, d=1
        // Actually after evicting c: a freq=3, b freq=2, d freq=1
        // The get("c") returned empty, so no freq change
        cache.put("e", 5); // evicts "d" (freq=1)
        assertEquals(Optional.empty(), cache.get("d"));
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.of(2), cache.get("b"));
        assertEquals(Optional.of(5), cache.get("e"));
    }

    @Test
    void newItemHasMinFrequency() {
        var cache = new LfuCache<String, Integer>(2);
        cache.put("a", 1);
        cache.get("a"); // freq=2
        cache.get("a"); // freq=3
        cache.put("b", 2); // freq=1
        // "b" has the lowest frequency
        cache.put("c", 3); // evicts "b" (freq=1)
        assertEquals(Optional.of(1), cache.get("a"));
        assertEquals(Optional.empty(), cache.get("b"));
        assertEquals(Optional.of(3), cache.get("c"));
    }

    // ---- getFrequency ----

    @Test
    void getFrequency() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        assertEquals(1, cache.getFrequency("a"));
        cache.get("a");
        assertEquals(2, cache.getFrequency("a"));
        assertEquals(-1, cache.getFrequency("missing"));
    }

    // ---- remove ----

    @Test
    void removeKey() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        cache.put("c", 3);
        assertEquals(Optional.of(2), cache.remove("b"));
        assertEquals(2, cache.size());
        assertFalse(cache.containsKey("b"));
        assertEquals(Optional.empty(), cache.remove("missing"));
    }

    @Test
    void removeUpdatesMinFrequency() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1); // freq=1
        cache.get("a");    // freq=2
        cache.put("b", 2); // freq=1
        cache.remove("b"); // remove the min-freq entry
        // min frequency should recalculate to 2
        assertEquals(2, cache.getMinFrequency());
    }

    // ---- keys and values ----

    @Test
    void keysAndValues() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        cache.put("b", 2);
        List<String> keys = cache.keys();
        assertEquals(2, keys.size());
        assertTrue(keys.contains("a"));
        assertTrue(keys.contains("b"));
        List<Integer> values = cache.values();
        assertEquals(2, values.size());
        assertTrue(values.contains(1));
        assertTrue(values.contains(2));
    }

    // ---- getMinFrequency / getMaxFrequency ----

    @Test
    void getMinAndMaxFrequency() {
        var cache = new LfuCache<String, Integer>(3);
        assertEquals(0, cache.getMinFrequency());
        assertEquals(0, cache.getMaxFrequency());

        cache.put("a", 1); // freq=1
        cache.put("b", 2); // freq=1
        cache.get("a");    // freq: a=2, b=1
        cache.get("a");    // freq: a=3, b=1
        assertEquals(1, cache.getMinFrequency());
        assertEquals(3, cache.getMaxFrequency());
    }

    // ---- peek ----

    @Test
    void peekDoesNotIncrementFrequency() {
        var cache = new LfuCache<String, Integer>(2);
        cache.put("a", 1); // freq=1
        assertEquals(Optional.of(1), cache.peek("a"));
        assertEquals(1, cache.getFrequency("a")); // still 1
        assertEquals(Optional.empty(), cache.peek("missing"));
    }

    // ---- putIfAbsent ----

    @Test
    void putIfAbsentNewKey() {
        var cache = new LfuCache<String, Integer>(3);
        assertEquals(Optional.empty(), cache.putIfAbsent("a", 1));
        assertEquals(Optional.of(1), cache.get("a"));
    }

    @Test
    void putIfAbsentExistingKey() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        assertEquals(Optional.of(1), cache.putIfAbsent("a", 99));
        assertEquals(Optional.of(1), cache.peek("a")); // value unchanged
    }

    // ---- getOrDefault ----

    @Test
    void getOrDefaultExistingKey() {
        var cache = new LfuCache<String, Integer>(3);
        cache.put("a", 1);
        assertEquals(1, cache.getOrDefault("a", 99));
        assertEquals(2, cache.getFrequency("a")); // frequency incremented
    }

    @Test
    void getOrDefaultMissingKey() {
        var cache = new LfuCache<String, Integer>(3);
        assertEquals(99, cache.getOrDefault("missing", 99));
    }
}
