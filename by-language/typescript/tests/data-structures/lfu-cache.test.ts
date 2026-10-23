import { describe, test, expect } from "bun:test";
import { LFUCache } from "../../src/data-structures/lfu-cache";

describe("LFUCache", () => {
  test("does not retain empty frequency buckets", () => {
    const cache = new LFUCache<string, number>(1);
    cache.put("a", 1);
    for (let i = 0; i < 1000; i++) cache.get("a");
    const buckets = Reflect.get(cache, "_freqMap") as Map<number, unknown>;
    expect(buckets.size).toBe(1);
  });
  describe("constructor", () => {
    test("should create with given capacity", () => {
      const cache = new LFUCache<string, number>(5);
      expect(cache.capacity).toBe(5);
      expect(cache.size).toBe(0);
    });

    test("should throw for capacity < 1", () => {
      expect(() => new LFUCache<string, number>(0)).toThrow(
        "Capacity must be at least 1"
      );
    });
  });

  describe("basic operations", () => {
    test("should put and get values", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      expect(cache.get("a")).toBe(1);
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
    });

    test("should return undefined for missing key", () => {
      const cache = new LFUCache<string, number>(3);
      expect(cache.get("missing")).toBeUndefined();
    });

    test("should update existing key", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("a", 100);
      expect(cache.get("a")).toBe(100);
      expect(cache.size).toBe(1);
    });

    test("should check has()", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      expect(cache.has("a")).toBe(true);
      expect(cache.has("b")).toBe(false);
    });

    test("should delete entries", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.delete("a")).toBe(true);
      expect(cache.get("a")).toBeUndefined();
      expect(cache.size).toBe(1);
    });

    test("should return false for deleting non-existent key", () => {
      const cache = new LFUCache<string, number>(3);
      expect(cache.delete("nope")).toBe(false);
    });

    test("should clear all entries", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.clear();
      expect(cache.size).toBe(0);
      expect(cache.get("a")).toBeUndefined();
    });
  });

  describe("frequency tracking", () => {
    test("get() should increment frequency", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1); // freq=1
      cache.get("a"); // freq=2
      cache.get("a"); // freq=3
      expect(cache.getFrequency("a")).toBe(3);
    });

    test("put() on existing key should increment frequency", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1); // freq=1
      cache.put("a", 2); // freq=2 (update)
      expect(cache.getFrequency("a")).toBe(2);
    });

    test("new keys start with frequency 1", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      expect(cache.getFrequency("a")).toBe(1);
    });

    test("getFrequency returns -1 for missing key", () => {
      const cache = new LFUCache<string, number>(3);
      expect(cache.getFrequency("x")).toBe(-1);
    });
  });

  describe("eviction of least frequent", () => {
    test("should evict least frequently used item", () => {
      const cache = new LFUCache<string, number>(2);
      cache.put("a", 1); // freq=1
      cache.put("b", 2); // freq=1
      cache.get("a"); // freq(a)=2

      // Now adding "c" should evict "b" (lower frequency)
      cache.put("c", 3);
      expect(cache.get("b")).toBeUndefined(); // evicted
      expect(cache.get("a")).toBe(1);
      expect(cache.get("c")).toBe(3);
    });

    test("should evict item with frequency 1 over higher freq", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      // Access a and b to increase frequency
      cache.get("a"); // freq(a)=2
      cache.get("b"); // freq(b)=2
      cache.get("a"); // freq(a)=3

      // "c" has freq=1, should be evicted
      cache.put("d", 4);
      expect(cache.get("c")).toBeUndefined(); // evicted
      expect(cache.get("a")).toBe(1);
      expect(cache.get("b")).toBe(2);
      expect(cache.get("d")).toBe(4);
    });
  });

  describe("tie-breaking by LRU", () => {
    test("when frequencies tie, evict the LRU among them", () => {
      const cache = new LFUCache<string, number>(2);
      cache.put("a", 1); // freq=1
      cache.put("b", 2); // freq=1
      // Both have freq=1, "a" was used least recently
      cache.put("c", 3);
      expect(cache.get("a")).toBeUndefined(); // evicted (LRU tie-break)
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
    });

    test("LRU tie-breaking with multiple same-frequency items", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1); // freq=1, order: a
      cache.put("b", 2); // freq=1, order: b, a
      cache.put("c", 3); // freq=1, order: c, b, a
      // All freq=1, "a" is LRU
      cache.put("d", 4);
      expect(cache.get("a")).toBeUndefined(); // evicted
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
      expect(cache.get("d")).toBe(4);
    });

    test("LRU order updates when accessing same-frequency items", () => {
      const cache = new LFUCache<string, number>(3);
      cache.put("a", 1); // freq=1
      cache.put("b", 2); // freq=1
      cache.put("c", 3); // freq=1

      // Access all once -> all freq=2
      cache.get("a"); // freq(a)=2
      cache.get("b"); // freq(b)=2
      cache.get("c"); // freq(c)=2

      // All freq=2, "a" is LRU in freq-2 bucket
      cache.put("d", 4); // freq(d)=1, evicts "a" (min_freq becomes 1, then evicts from freq=1... wait)

      // Actually, inserting d sets min_freq=1 so d has freq=1
      // The eviction happens first: min_freq is 2 (all items have freq=2), so "a" is evicted (LRU in freq-2)
      expect(cache.get("a")).toBeUndefined();
    });
  });

  describe("capacity of 1", () => {
    test("should handle capacity 1 correctly", () => {
      const cache = new LFUCache<string, number>(1);
      cache.put("a", 1);
      expect(cache.get("a")).toBe(1);
      cache.put("b", 2); // evicts "a"
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBe(2);
      expect(cache.size).toBe(1);
    });
  });

  describe("LeetCode #460 example", () => {
    test("should match LeetCode expected behavior", () => {
      const cache = new LFUCache<number, number>(2);
      cache.put(1, 1); // cache={1:1}
      cache.put(2, 2); // cache={1:1, 2:2}
      expect(cache.get(1)).toBe(1); // freq(1)=2, cache={1:1, 2:2}
      cache.put(3, 3); // evicts key 2, cache={1:1, 3:3}
      expect(cache.get(2)).toBeUndefined();
      expect(cache.get(3)).toBe(3); // freq(3)=2
      cache.put(4, 4); // evicts key 1, cache={3:3, 4:4}
      expect(cache.get(1)).toBeUndefined();
      expect(cache.get(3)).toBe(3); // freq(3)=3
      expect(cache.get(4)).toBe(4); // freq(4)=2
    });
  });

  describe("complex scenarios", () => {
    test("should handle interleaved operations", () => {
      const cache = new LFUCache<number, number>(3);
      cache.put(1, 10);
      cache.put(2, 20);
      cache.put(3, 30);
      cache.get(1); // freq(1)=2
      cache.get(2); // freq(2)=2
      cache.get(3); // freq(3)=2

      cache.get(1); // freq(1)=3
      cache.put(4, 40); // evicts LRU among freq=2 -> evicts 2

      expect(cache.has(1)).toBe(true);
      expect(cache.has(2)).toBe(false);
      expect(cache.has(3)).toBe(true);
      expect(cache.has(4)).toBe(true);
    });

    test("should not exceed capacity", () => {
      const cache = new LFUCache<number, number>(3);
      for (let i = 0; i < 100; i++) {
        cache.put(i, i);
      }
      expect(cache.size).toBe(3);
    });
  });
});
