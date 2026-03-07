import { describe, test, expect } from "bun:test";
import { SkipList } from "../../src/data-structures/skip-list";

describe("SkipList", () => {
  describe("construction and properties", () => {
    test("should start empty", () => {
      const sl = new SkipList<number>();
      expect(sl.size).toBe(0);
      expect(sl.isEmpty).toBe(true);
    });

    test("should accept a custom probability", () => {
      const sl = new SkipList<number>(0.25);
      sl.insert(1);
      expect(sl.size).toBe(1);
    });

    test("should clamp invalid probability to 0.5", () => {
      const sl1 = new SkipList<number>(0);
      const sl2 = new SkipList<number>(1);
      const sl3 = new SkipList<number>(-0.5);
      // All should work with the default probability of 0.5
      sl1.insert(1);
      sl2.insert(1);
      sl3.insert(1);
      expect(sl1.size).toBe(1);
      expect(sl2.size).toBe(1);
      expect(sl3.size).toBe(1);
    });

    test("should expose MAX_LEVEL static property", () => {
      expect(SkipList.MAX_LEVEL).toBe(32);
    });
  });

  describe("insert", () => {
    test("should insert elements and increase size", () => {
      const sl = new SkipList<number>();
      sl.insert(3);
      sl.insert(1);
      sl.insert(2);
      expect(sl.size).toBe(3);
      expect(sl.isEmpty).toBe(false);
    });

    test("should not duplicate on insert of same value", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.insert(5);
      expect(sl.size).toBe(1);
    });

    test("should insert negative numbers", () => {
      const sl = new SkipList<number>();
      sl.insert(-5);
      sl.insert(-1);
      sl.insert(-10);
      expect(sl.toArray()).toEqual([-10, -5, -1]);
    });
  });

  describe("search", () => {
    test("should find existing elements", () => {
      const sl = new SkipList<number>();
      sl.insert(10);
      sl.insert(20);
      sl.insert(30);
      expect(sl.search(10)).toBe(true);
      expect(sl.search(20)).toBe(true);
      expect(sl.search(30)).toBe(true);
    });

    test("should not find non-existent elements", () => {
      const sl = new SkipList<number>();
      sl.insert(10);
      sl.insert(20);
      sl.insert(30);
      expect(sl.search(15)).toBe(false);
      expect(sl.search(0)).toBe(false);
      expect(sl.search(35)).toBe(false);
    });

    test("should return false on empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.search(1)).toBe(false);
    });
  });

  describe("delete", () => {
    test("should delete existing elements", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      sl.insert(3);
      expect(sl.delete(2)).toBe(true);
      expect(sl.search(2)).toBe(false);
      expect(sl.size).toBe(2);
      expect(sl.search(1)).toBe(true);
      expect(sl.search(3)).toBe(true);
    });

    test("should return false when deleting non-existent element", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      expect(sl.delete(99)).toBe(false);
      expect(sl.size).toBe(1);
    });

    test("should return false when deleting from empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.delete(1)).toBe(false);
    });

    test("should delete the only element", () => {
      const sl = new SkipList<number>();
      sl.insert(42);
      expect(sl.delete(42)).toBe(true);
      expect(sl.isEmpty).toBe(true);
      expect(sl.search(42)).toBe(false);
    });

    test("should delete first element", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      sl.insert(3);
      expect(sl.delete(1)).toBe(true);
      expect(sl.toArray()).toEqual([2, 3]);
    });

    test("should delete last element", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      sl.insert(3);
      expect(sl.delete(3)).toBe(true);
      expect(sl.toArray()).toEqual([1, 2]);
    });

    test("should allow re-insertion after deletion", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.delete(5);
      sl.insert(5);
      expect(sl.search(5)).toBe(true);
      expect(sl.size).toBe(1);
    });
  });

  describe("toArray and ordering", () => {
    test("should return sorted elements", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.insert(3);
      sl.insert(8);
      sl.insert(1);
      sl.insert(4);
      expect(sl.toArray()).toEqual([1, 3, 4, 5, 8]);
    });

    test("should handle elements inserted in reverse order", () => {
      const sl = new SkipList<number>();
      for (let i = 10; i >= 1; i--) {
        sl.insert(i);
      }
      expect(sl.toArray()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });

    test("should handle elements inserted in sorted order", () => {
      const sl = new SkipList<number>();
      for (let i = 1; i <= 10; i++) {
        sl.insert(i);
      }
      expect(sl.toArray()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });

    test("should return empty array for empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.toArray()).toEqual([]);
    });
  });

  describe("range queries", () => {
    test("should return elements in range [lo, hi]", () => {
      const sl = new SkipList<number>();
      for (let i = 1; i <= 10; i++) {
        sl.insert(i);
      }
      expect(sl.range(3, 7)).toEqual([3, 4, 5, 6, 7]);
    });

    test("should return empty for out-of-range queries", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      sl.insert(3);
      expect(sl.range(10, 20)).toEqual([]);
    });

    test("should return empty when lo > hi", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      expect(sl.range(5, 3)).toEqual([]);
    });

    test("should return single element range", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.insert(10);
      sl.insert(15);
      expect(sl.range(10, 10)).toEqual([10]);
    });

    test("should handle range spanning all elements", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(5);
      sl.insert(10);
      expect(sl.range(1, 10)).toEqual([1, 5, 10]);
    });

    test("should handle range that partially overlaps", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      sl.insert(4);
      sl.insert(6);
      sl.insert(8);
      // range includes values not in the list as bounds
      expect(sl.range(3, 7)).toEqual([4, 6]);
    });

    test("should return empty range on empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.range(1, 10)).toEqual([]);
    });
  });

  describe("floor", () => {
    test("should return the value itself if present", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      sl.insert(5);
      sl.insert(8);
      expect(sl.floor(5)).toBe(5);
    });

    test("should return largest value <= given", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      sl.insert(5);
      sl.insert(8);
      sl.insert(12);
      expect(sl.floor(6)).toBe(5);
      expect(sl.floor(11)).toBe(8);
      expect(sl.floor(15)).toBe(12);
    });

    test("should return null if no value <= given", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.insert(10);
      expect(sl.floor(3)).toBeNull();
    });

    test("should return null on empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.floor(5)).toBeNull();
    });

    test("should return floor at boundary", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      expect(sl.floor(2)).toBe(2);
    });
  });

  describe("ceiling", () => {
    test("should return the value itself if present", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      sl.insert(5);
      sl.insert(8);
      expect(sl.ceiling(5)).toBe(5);
    });

    test("should return smallest value >= given", () => {
      const sl = new SkipList<number>();
      sl.insert(2);
      sl.insert(5);
      sl.insert(8);
      sl.insert(12);
      expect(sl.ceiling(6)).toBe(8);
      expect(sl.ceiling(1)).toBe(2);
      expect(sl.ceiling(9)).toBe(12);
    });

    test("should return null if no value >= given", () => {
      const sl = new SkipList<number>();
      sl.insert(5);
      sl.insert(10);
      expect(sl.ceiling(15)).toBeNull();
    });

    test("should return null on empty list", () => {
      const sl = new SkipList<number>();
      expect(sl.ceiling(5)).toBeNull();
    });

    test("should return ceiling at boundary", () => {
      const sl = new SkipList<number>();
      sl.insert(8);
      expect(sl.ceiling(8)).toBe(8);
    });
  });

  describe("clear", () => {
    test("should reset the list to empty", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.insert(2);
      sl.insert(3);
      sl.clear();
      expect(sl.size).toBe(0);
      expect(sl.isEmpty).toBe(true);
      expect(sl.search(1)).toBe(false);
      expect(sl.toArray()).toEqual([]);
    });

    test("should allow insertions after clear", () => {
      const sl = new SkipList<number>();
      sl.insert(1);
      sl.clear();
      sl.insert(2);
      expect(sl.size).toBe(1);
      expect(sl.search(2)).toBe(true);
    });
  });

  describe("with strings", () => {
    test("should work with string elements in sorted order", () => {
      const sl = new SkipList<string>();
      sl.insert("banana");
      sl.insert("apple");
      sl.insert("cherry");
      expect(sl.toArray()).toEqual(["apple", "banana", "cherry"]);
      expect(sl.search("banana")).toBe(true);
      expect(sl.search("date")).toBe(false);
    });

    test("floor and ceiling should work with strings", () => {
      const sl = new SkipList<string>();
      sl.insert("apple");
      sl.insert("cherry");
      sl.insert("grape");
      expect(sl.floor("banana")).toBe("apple");
      expect(sl.ceiling("banana")).toBe("cherry");
    });
  });

  describe("stress test", () => {
    test("should handle many elements correctly", () => {
      const sl = new SkipList<number>();
      const n = 1000;

      for (let i = 0; i < n; i++) {
        sl.insert(i);
      }
      expect(sl.size).toBe(n);

      for (let i = 0; i < n; i++) {
        expect(sl.search(i)).toBe(true);
      }

      // Delete even numbers
      for (let i = 0; i < n; i += 2) {
        expect(sl.delete(i)).toBe(true);
      }
      expect(sl.size).toBe(n / 2);

      // Verify only odd numbers remain
      for (let i = 0; i < n; i++) {
        if (i % 2 === 0) {
          expect(sl.search(i)).toBe(false);
        } else {
          expect(sl.search(i)).toBe(true);
        }
      }
    });

    test("random insert/delete/search mix", () => {
      const sl = new SkipList<number>();
      const reference = new Set<number>();

      for (let i = 0; i < 500; i++) {
        const val = Math.floor(Math.random() * 200);
        if (Math.random() < 0.6) {
          sl.insert(val);
          reference.add(val);
        } else {
          const deleted = sl.delete(val);
          const wasInRef = reference.has(val);
          expect(deleted).toBe(wasInRef);
          reference.delete(val);
        }
      }

      expect(sl.size).toBe(reference.size);
      for (const val of reference) {
        expect(sl.search(val)).toBe(true);
      }
    });
  });

  describe("edge cases", () => {
    test("should handle single element operations", () => {
      const sl = new SkipList<number>();
      sl.insert(42);
      expect(sl.search(42)).toBe(true);
      expect(sl.toArray()).toEqual([42]);
      expect(sl.floor(42)).toBe(42);
      expect(sl.ceiling(42)).toBe(42);
      expect(sl.range(42, 42)).toEqual([42]);
      sl.delete(42);
      expect(sl.isEmpty).toBe(true);
    });
  });
});
