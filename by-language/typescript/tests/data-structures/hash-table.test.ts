import { describe, test, expect } from "bun:test";
import {
  HashTableChaining,
  HashTableOpenAddressing,
  HashSet,
} from "../../src/data-structures/hash-table";

describe("HashTableChaining", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const ht = new HashTableChaining<string, number>();
      expect(ht.size).toBe(0);
      expect(ht.isEmpty).toBe(true);
    });

    test("should put and get values", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.put("c", 3);
      expect(ht.get("a")).toBe(1);
      expect(ht.get("b")).toBe(2);
      expect(ht.get("c")).toBe(3);
      expect(ht.size).toBe(3);
    });

    test("should return undefined for missing keys", () => {
      const ht = new HashTableChaining<string, number>();
      expect(ht.get("missing")).toBeUndefined();
    });

    test("should update existing keys", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("key", 1);
      ht.put("key", 2);
      expect(ht.get("key")).toBe(2);
      expect(ht.size).toBe(1);
    });

    test("should delete keys", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      expect(ht.delete("a")).toBe(true);
      expect(ht.get("a")).toBeUndefined();
      expect(ht.size).toBe(1);
    });

    test("should return false when deleting non-existent key", () => {
      const ht = new HashTableChaining<string, number>();
      expect(ht.delete("nope")).toBe(false);
    });

    test("should check if key exists with has()", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      expect(ht.has("a")).toBe(true);
      expect(ht.has("b")).toBe(false);
    });

    test("should clear all entries", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.clear();
      expect(ht.size).toBe(0);
      expect(ht.isEmpty).toBe(true);
      expect(ht.get("a")).toBeUndefined();
    });
  });

  describe("keys, values, entries", () => {
    test("should return all keys", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.put("c", 3);
      const keys = ht.keys().sort();
      expect(keys).toEqual(["a", "b", "c"]);
    });

    test("should return all values", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.put("c", 3);
      const values = ht.values().sort();
      expect(values).toEqual([1, 2, 3]);
    });

    test("should return all entries", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      const entries = ht.entries().sort((a, b) => a[0].localeCompare(b[0]));
      expect(entries).toEqual([
        ["a", 1],
        ["b", 2],
      ]);
    });
  });

  describe("collision handling and resize", () => {
    test("should handle many insertions (triggers resize)", () => {
      const ht = new HashTableChaining<number, number>();
      for (let i = 0; i < 100; i++) {
        ht.put(i, i * 10);
      }
      expect(ht.size).toBe(100);
      for (let i = 0; i < 100; i++) {
        expect(ht.get(i)).toBe(i * 10);
      }
    });

    test("should resize when load factor exceeded", () => {
      const ht = new HashTableChaining<number, number>(4);
      const initialCapacity = ht.capacity;
      // Fill past load factor threshold
      for (let i = 0; i < 10; i++) {
        ht.put(i, i);
      }
      expect(ht.capacity).toBeGreaterThan(initialCapacity);
      // All data intact after resize
      for (let i = 0; i < 10; i++) {
        expect(ht.get(i)).toBe(i);
      }
    });

    test("should handle collisions with small capacity", () => {
      const ht = new HashTableChaining<number, string>(1);
      ht.put(0, "zero");
      ht.put(1, "one");
      ht.put(2, "two");
      expect(ht.get(0)).toBe("zero");
      expect(ht.get(1)).toBe("one");
      expect(ht.get(2)).toBe("two");
    });
  });

  describe("numeric and mixed key types", () => {
    test("should work with number keys", () => {
      const ht = new HashTableChaining<number, string>();
      ht.put(1, "one");
      ht.put(2, "two");
      expect(ht.get(1)).toBe("one");
      expect(ht.get(2)).toBe("two");
    });

    test("should handle empty string key", () => {
      const ht = new HashTableChaining<string, number>();
      ht.put("", 42);
      expect(ht.get("")).toBe(42);
      expect(ht.has("")).toBe(true);
    });
  });
});

describe("HashTableOpenAddressing", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      expect(ht.size).toBe(0);
      expect(ht.isEmpty).toBe(true);
    });

    test("should put and get values", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      expect(ht.get("a")).toBe(1);
      expect(ht.get("b")).toBe(2);
      expect(ht.size).toBe(2);
    });

    test("should return undefined for missing keys", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      expect(ht.get("x")).toBeUndefined();
    });

    test("should update existing keys", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("key", 1);
      ht.put("key", 99);
      expect(ht.get("key")).toBe(99);
      expect(ht.size).toBe(1);
    });

    test("should check has()", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("x", 1);
      expect(ht.has("x")).toBe(true);
      expect(ht.has("y")).toBe(false);
    });
  });

  describe("tombstone support for deletion", () => {
    test("should delete keys and support tombstones", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.put("c", 3);

      expect(ht.delete("b")).toBe(true);
      expect(ht.get("b")).toBeUndefined();
      expect(ht.has("b")).toBe(false);
      expect(ht.size).toBe(2);

      // Other keys still accessible
      expect(ht.get("a")).toBe(1);
      expect(ht.get("c")).toBe(3);
    });

    test("should reuse tombstone slots on insertion", () => {
      const ht = new HashTableOpenAddressing<number, number>(4);
      ht.put(1, 10);
      ht.put(2, 20);
      ht.delete(1);
      ht.put(3, 30);
      expect(ht.get(3)).toBe(30);
      expect(ht.has(1)).toBe(false);
    });

    test("should return false when deleting non-existent key", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      expect(ht.delete("nope")).toBe(false);
    });

    test("should handle delete-then-reinsert", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("a", 1);
      ht.delete("a");
      ht.put("a", 2);
      expect(ht.get("a")).toBe(2);
      expect(ht.size).toBe(1);
    });
  });

  describe("resize", () => {
    test("should resize when load factor exceeded", () => {
      const ht = new HashTableOpenAddressing<number, number>(4);
      const initialCapacity = ht.capacity;
      for (let i = 0; i < 10; i++) {
        ht.put(i, i * 100);
      }
      expect(ht.capacity).toBeGreaterThan(initialCapacity);
      for (let i = 0; i < 10; i++) {
        expect(ht.get(i)).toBe(i * 100);
      }
    });

    test("should handle many insertions", () => {
      const ht = new HashTableOpenAddressing<number, number>();
      for (let i = 0; i < 200; i++) {
        ht.put(i, i);
      }
      expect(ht.size).toBe(200);
      for (let i = 0; i < 200; i++) {
        expect(ht.get(i)).toBe(i);
      }
    });
  });

  describe("keys and values", () => {
    test("should return keys and values", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("x", 10);
      ht.put("y", 20);
      ht.put("z", 30);
      expect(ht.keys().sort()).toEqual(["x", "y", "z"]);
      expect(ht.values().sort()).toEqual([10, 20, 30]);
    });

    test("should not include deleted keys", () => {
      const ht = new HashTableOpenAddressing<string, number>();
      ht.put("a", 1);
      ht.put("b", 2);
      ht.delete("a");
      expect(ht.keys()).toEqual(["b"]);
      expect(ht.values()).toEqual([2]);
    });
  });
});

describe("HashSet", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const s = new HashSet<number>();
      expect(s.size).toBe(0);
      expect(s.isEmpty).toBe(true);
    });

    test("should add elements", () => {
      const s = new HashSet<number>();
      s.add(1);
      s.add(2);
      s.add(3);
      expect(s.size).toBe(3);
      expect(s.has(1)).toBe(true);
      expect(s.has(2)).toBe(true);
      expect(s.has(3)).toBe(true);
    });

    test("should not add duplicates", () => {
      const s = new HashSet<number>();
      s.add(1);
      s.add(1);
      s.add(1);
      expect(s.size).toBe(1);
    });

    test("should delete elements", () => {
      const s = new HashSet<number>();
      s.add(1);
      s.add(2);
      expect(s.delete(1)).toBe(true);
      expect(s.has(1)).toBe(false);
      expect(s.size).toBe(1);
    });

    test("should return false when deleting non-existent", () => {
      const s = new HashSet<number>();
      expect(s.delete(99)).toBe(false);
    });

    test("should clear", () => {
      const s = new HashSet<number>();
      s.add(1);
      s.add(2);
      s.clear();
      expect(s.size).toBe(0);
      expect(s.isEmpty).toBe(true);
    });

    test("should convert to array", () => {
      const s = new HashSet<number>();
      s.add(3);
      s.add(1);
      s.add(2);
      expect(s.toArray().sort()).toEqual([1, 2, 3]);
    });

    test("should create from array", () => {
      const s = HashSet.fromArray([1, 2, 3, 2, 1]);
      expect(s.size).toBe(3);
      expect(s.has(1)).toBe(true);
      expect(s.has(2)).toBe(true);
      expect(s.has(3)).toBe(true);
    });
  });

  describe("set operations", () => {
    test("union", () => {
      const a = HashSet.fromArray([1, 2, 3]);
      const b = HashSet.fromArray([3, 4, 5]);
      const result = a.union(b);
      expect(result.toArray().sort()).toEqual([1, 2, 3, 4, 5]);
    });

    test("union with empty set", () => {
      const a = HashSet.fromArray([1, 2]);
      const b = new HashSet<number>();
      const result = a.union(b);
      expect(result.toArray().sort()).toEqual([1, 2]);
    });

    test("intersection", () => {
      const a = HashSet.fromArray([1, 2, 3, 4]);
      const b = HashSet.fromArray([3, 4, 5, 6]);
      const result = a.intersection(b);
      expect(result.toArray().sort()).toEqual([3, 4]);
    });

    test("intersection with disjoint sets", () => {
      const a = HashSet.fromArray([1, 2]);
      const b = HashSet.fromArray([3, 4]);
      const result = a.intersection(b);
      expect(result.size).toBe(0);
    });

    test("difference", () => {
      const a = HashSet.fromArray([1, 2, 3, 4]);
      const b = HashSet.fromArray([3, 4, 5]);
      const result = a.difference(b);
      expect(result.toArray().sort()).toEqual([1, 2]);
    });

    test("difference with no overlap", () => {
      const a = HashSet.fromArray([1, 2]);
      const b = HashSet.fromArray([3, 4]);
      const result = a.difference(b);
      expect(result.toArray().sort()).toEqual([1, 2]);
    });

    test("difference with complete overlap", () => {
      const a = HashSet.fromArray([1, 2, 3]);
      const b = HashSet.fromArray([1, 2, 3]);
      const result = a.difference(b);
      expect(result.size).toBe(0);
    });
  });

  describe("string elements", () => {
    test("should work with strings", () => {
      const s = new HashSet<string>();
      s.add("hello");
      s.add("world");
      expect(s.has("hello")).toBe(true);
      expect(s.has("missing")).toBe(false);
    });
  });
});
