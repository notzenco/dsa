import { describe, test, expect } from "bun:test";
import { LRUCache } from "../../src/data-structures/lru-cache";

describe("LRUCache", () => {
  describe("constructor", () => {
    test("should create with given capacity", () => {
      const cache = new LRUCache<string, number>(5);
      expect(cache.capacity).toBe(5);
      expect(cache.size).toBe(0);
    });

    test("should throw for capacity < 1", () => {
      expect(() => new LRUCache<string, number>(0)).toThrow(
        "Capacity must be at least 1"
      );
      expect(() => new LRUCache<string, number>(-1)).toThrow(
        "Capacity must be at least 1"
      );
    });
  });

  describe("basic operations", () => {
    test("should put and get values", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      expect(cache.get("a")).toBe(1);
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
    });

    test("should return undefined for missing key", () => {
      const cache = new LRUCache<string, number>(3);
      expect(cache.get("missing")).toBeUndefined();
    });

    test("should update existing key", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("a", 100);
      expect(cache.get("a")).toBe(100);
      expect(cache.size).toBe(1);
    });

    test("should check has()", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      expect(cache.has("a")).toBe(true);
      expect(cache.has("b")).toBe(false);
    });

    test("should delete entries", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.delete("a")).toBe(true);
      expect(cache.get("a")).toBeUndefined();
      expect(cache.size).toBe(1);
    });

    test("should return false for deleting non-existent key", () => {
      const cache = new LRUCache<string, number>(3);
      expect(cache.delete("nope")).toBe(false);
    });

    test("should clear all entries", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.clear();
      expect(cache.size).toBe(0);
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBeUndefined();
    });
  });

  describe("eviction policy", () => {
    test("should evict LRU item when at capacity", () => {
      const cache = new LRUCache<string, number>(2);
      cache.put("a", 1);
      cache.put("b", 2);
      // "a" is LRU
      cache.put("c", 3);
      expect(cache.get("a")).toBeUndefined(); // evicted
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
    });

    test("should evict correct item after access updates order", () => {
      const cache = new LRUCache<string, number>(2);
      cache.put("a", 1);
      cache.put("b", 2);
      // Access "a" to make it MRU
      cache.get("a");
      // Now "b" is LRU
      cache.put("c", 3);
      expect(cache.get("b")).toBeUndefined(); // evicted
      expect(cache.get("a")).toBe(1);
      expect(cache.get("c")).toBe(3);
    });

    test("updating a key should make it MRU", () => {
      const cache = new LRUCache<string, number>(2);
      cache.put("a", 1);
      cache.put("b", 2);
      // Update "a" to make it MRU
      cache.put("a", 10);
      // "b" is LRU
      cache.put("c", 3);
      expect(cache.get("b")).toBeUndefined(); // evicted
      expect(cache.get("a")).toBe(10);
      expect(cache.get("c")).toBe(3);
    });

    test("should handle capacity of 1", () => {
      const cache = new LRUCache<string, number>(1);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBe(2);
      expect(cache.size).toBe(1);
    });
  });

  describe("access order updates", () => {
    test("get() should move item to MRU position", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);

      // Access "a" making it MRU
      cache.get("a");

      // Order: a(MRU), c, b(LRU)
      cache.put("d", 4); // evicts "b"
      expect(cache.has("b")).toBe(false);
      expect(cache.has("a")).toBe(true);
      expect(cache.has("c")).toBe(true);
      expect(cache.has("d")).toBe(true);
    });

    test("sequence of operations should maintain correct order", () => {
      const cache = new LRUCache<number, string>(3);
      cache.put(1, "a");
      cache.put(2, "b");
      cache.put(3, "c");
      // Order: 3(MRU), 2, 1(LRU)

      cache.get(1); // Order: 1(MRU), 3, 2(LRU)
      cache.put(4, "d"); // Evicts 2, Order: 4(MRU), 1, 3(LRU)

      expect(cache.has(2)).toBe(false);
      expect(cache.get(3)).toBe("c"); // Order: 3(MRU), 4, 1(LRU)
      cache.put(5, "e"); // Evicts 1

      expect(cache.has(1)).toBe(false);
      expect(cache.get(3)).toBe("c");
      expect(cache.get(4)).toBe("d");
      expect(cache.get(5)).toBe("e");
    });
  });

  describe("keys and values", () => {
    test("should return keys in MRU to LRU order", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      // Order: c(MRU), b, a(LRU)
      expect(cache.keys()).toEqual(["c", "b", "a"]);
    });

    test("should return values in MRU to LRU order", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      expect(cache.values()).toEqual([3, 2, 1]);
    });

    test("should reflect order after get()", () => {
      const cache = new LRUCache<string, number>(3);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3);
      cache.get("a");
      // Order: a(MRU), c, b(LRU)
      expect(cache.keys()).toEqual(["a", "c", "b"]);
    });
  });

  describe("capacity limits", () => {
    test("should not exceed capacity", () => {
      const cache = new LRUCache<number, number>(5);
      for (let i = 0; i < 100; i++) {
        cache.put(i, i);
      }
      expect(cache.size).toBe(5);
    });

    test("only the last N items should remain", () => {
      const cache = new LRUCache<number, number>(3);
      for (let i = 0; i < 10; i++) {
        cache.put(i, i * 10);
      }
      expect(cache.size).toBe(3);
      // Only 7, 8, 9 should remain
      expect(cache.has(7)).toBe(true);
      expect(cache.has(8)).toBe(true);
      expect(cache.has(9)).toBe(true);
      expect(cache.has(6)).toBe(false);
    });
  });
});
