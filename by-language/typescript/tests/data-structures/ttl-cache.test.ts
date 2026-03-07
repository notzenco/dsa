import { describe, test, expect } from "bun:test";
import { TTLCache } from "../../src/data-structures/ttl-cache";

/**
 * Helper to create a TTLCache with a controllable clock.
 * Returns { cache, advance(ms) } where advance moves the clock forward.
 */
function createMockCache<K, V>(
  capacity: number,
  defaultTTLms: number
): { cache: TTLCache<K, V>; advance: (ms: number) => void; setTime: (ms: number) => void } {
  let now = 1000;
  const cache = new TTLCache<K, V>(capacity, defaultTTLms);
  cache._setNow(() => now);

  return {
    cache,
    advance: (ms: number) => {
      now += ms;
    },
    setTime: (ms: number) => {
      now = ms;
    },
  };
}

describe("TTLCache", () => {
  describe("constructor", () => {
    test("should create with valid parameters", () => {
      const cache = new TTLCache<string, number>(10, 1000);
      expect(cache.capacity).toBe(10);
    });

    test("should throw for capacity < 1", () => {
      expect(() => new TTLCache<string, number>(0, 1000)).toThrow(
        "Capacity must be at least 1"
      );
    });

    test("should throw for non-positive TTL", () => {
      expect(() => new TTLCache<string, number>(10, 0)).toThrow(
        "TTL must be positive"
      );
      expect(() => new TTLCache<string, number>(10, -1)).toThrow(
        "TTL must be positive"
      );
    });
  });

  describe("basic operations", () => {
    test("should put and get values", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.get("a")).toBe(1);
      expect(cache.get("b")).toBe(2);
    });

    test("should return undefined for missing key", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      expect(cache.get("missing")).toBeUndefined();
    });

    test("should update existing key", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      cache.put("a", 100);
      expect(cache.get("a")).toBe(100);
    });

    test("should check has()", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      expect(cache.has("a")).toBe(true);
      expect(cache.has("b")).toBe(false);
    });

    test("should delete entries", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      expect(cache.delete("a")).toBe(true);
      expect(cache.get("a")).toBeUndefined();
    });

    test("should return false for deleting non-existent key", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      expect(cache.delete("nope")).toBe(false);
    });

    test("should clear all entries", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.clear();
      expect(cache.size).toBe(0);
      expect(cache.get("a")).toBeUndefined();
    });
  });

  describe("TTL expiry", () => {
    test("should return value before expiry", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      advance(4999);
      expect(cache.get("a")).toBe(1);
    });

    test("should return undefined after expiry", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      advance(5001);
      expect(cache.get("a")).toBeUndefined();
    });

    test("has() should return false after expiry", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      advance(5001);
      expect(cache.has("a")).toBe(false);
    });

    test("size should decrease after items expire", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.size).toBe(2);
      advance(5001);
      expect(cache.size).toBe(0);
    });

    test("different items can expire at different times", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      advance(3000);
      cache.put("b", 2);
      advance(2001); // total: 5001ms since "a" was put, 2001ms since "b" was put
      expect(cache.get("a")).toBeUndefined(); // expired
      expect(cache.get("b")).toBe(2); // not expired
    });

    test("expired items should be cleaned up on put()", () => {
      const { cache, advance } = createMockCache<string, number>(10, 1000);
      cache.put("a", 1);
      cache.put("b", 2);
      advance(1001);
      // Both expired, but size isn't updated until next operation
      cache.put("c", 3); // triggers cleanup
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBeUndefined();
      expect(cache.get("c")).toBe(3);
    });
  });

  describe("custom per-key TTL", () => {
    test("should use custom TTL for specific items", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("short", 1, 1000); // 1 second TTL
      cache.put("long", 2, 10000); // 10 second TTL

      advance(1001);
      expect(cache.get("short")).toBeUndefined(); // expired
      expect(cache.get("long")).toBe(2); // still alive

      advance(9000); // total: 10001ms
      expect(cache.get("long")).toBeUndefined(); // now expired
    });

    test("should use default TTL when custom TTL not provided", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("default", 1); // uses default 5000ms TTL
      cache.put("custom", 2, 2000); // 2000ms TTL

      advance(2001);
      expect(cache.get("custom")).toBeUndefined(); // expired
      expect(cache.get("default")).toBe(1); // still alive

      advance(3000); // total: 5001ms
      expect(cache.get("default")).toBeUndefined(); // expired
    });
  });

  describe("LRU eviction", () => {
    test("should evict LRU item when at capacity", () => {
      const { cache } = createMockCache<string, number>(2, 60000);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("c", 3); // evicts "a" (LRU)
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBe(2);
      expect(cache.get("c")).toBe(3);
    });

    test("should update LRU order on get()", () => {
      const { cache } = createMockCache<string, number>(2, 60000);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.get("a"); // makes "a" MRU
      cache.put("c", 3); // evicts "b" (now LRU)
      expect(cache.get("b")).toBeUndefined();
      expect(cache.get("a")).toBe(1);
      expect(cache.get("c")).toBe(3);
    });

    test("should update LRU order on put() of existing key", () => {
      const { cache } = createMockCache<string, number>(2, 60000);
      cache.put("a", 1);
      cache.put("b", 2);
      cache.put("a", 10); // updates "a", makes it MRU
      cache.put("c", 3); // evicts "b"
      expect(cache.get("b")).toBeUndefined();
      expect(cache.get("a")).toBe(10);
      expect(cache.get("c")).toBe(3);
    });

    test("expired items should free capacity before LRU eviction", () => {
      const { cache, advance } = createMockCache<string, number>(2, 1000);
      cache.put("a", 1);
      cache.put("b", 2);
      advance(1001); // both expired

      cache.put("c", 3); // cleanup runs, a and b removed, c fits
      cache.put("d", 4); // d fits too
      expect(cache.get("c")).toBe(3);
      expect(cache.get("d")).toBe(4);
      expect(cache.size).toBe(2);
    });
  });

  describe("capacity limits", () => {
    test("should not exceed capacity", () => {
      const { cache } = createMockCache<number, number>(3, 60000);
      for (let i = 0; i < 100; i++) {
        cache.put(i, i);
      }
      expect(cache.size).toBe(3);
    });

    test("capacity of 1", () => {
      const { cache } = createMockCache<string, number>(1, 60000);
      cache.put("a", 1);
      cache.put("b", 2);
      expect(cache.get("a")).toBeUndefined();
      expect(cache.get("b")).toBe(2);
      expect(cache.size).toBe(1);
    });
  });

  describe("edge cases", () => {
    test("should handle put then immediate get", () => {
      const { cache } = createMockCache<string, number>(10, 5000);
      cache.put("key", 42);
      expect(cache.get("key")).toBe(42);
    });

    test("should handle expiry exactly at TTL boundary", () => {
      const { cache, advance } = createMockCache<string, number>(10, 5000);
      cache.put("a", 1);
      advance(5000); // exactly at TTL boundary
      // At exactly TTL, the item should still be valid (expires when time > expireAt)
      expect(cache.get("a")).toBe(1);
    });

    test("should handle rapid put/delete cycles", () => {
      const { cache } = createMockCache<string, number>(5, 5000);
      for (let i = 0; i < 20; i++) {
        cache.put(`key_${i}`, i);
        if (i > 0) {
          cache.delete(`key_${i - 1}`);
        }
      }
      expect(cache.has("key_19")).toBe(true);
    });
  });
});
