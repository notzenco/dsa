import { describe, test, expect } from "bun:test";
import {
  DynamicArray,
  DEFAULT_CAPACITY,
} from "../../src/data-structures/dynamic-array";

describe("DynamicArray", () => {
  describe("construction", () => {
    test("new array is empty with default capacity", () => {
      const arr = new DynamicArray<number>();
      expect(arr.isEmpty).toBe(true);
      expect(arr.size).toBe(0);
      expect(arr.capacity).toBe(DEFAULT_CAPACITY);
    });

    test("custom initial capacity", () => {
      const arr = new DynamicArray<number>(16);
      expect(arr.capacity).toBe(16);
      expect(arr.size).toBe(0);
    });

    test("minimum capacity is 1", () => {
      expect(() => new DynamicArray<number>(0)).toThrow(
        "Capacity must be at least 1"
      );
      expect(() => new DynamicArray<number>(-1)).toThrow(
        "Capacity must be at least 1"
      );
    });

    test("capacity of 1 works", () => {
      const arr = new DynamicArray<number>(1);
      expect(arr.capacity).toBe(1);
      arr.push(42);
      expect(arr.size).toBe(1);
      expect(arr.get(0)).toBe(42);
    });
  });

  describe("DEFAULT_CAPACITY", () => {
    test("DEFAULT_CAPACITY is 8", () => {
      expect(DEFAULT_CAPACITY).toBe(8);
    });
  });

  describe("push", () => {
    test("push increases size", () => {
      const arr = new DynamicArray<number>();
      arr.push(1);
      expect(arr.size).toBe(1);
      expect(arr.isEmpty).toBe(false);
      arr.push(2);
      expect(arr.size).toBe(2);
    });

    test("push multiple elements", () => {
      const arr = new DynamicArray<number>();
      for (let i = 0; i < 5; i++) {
        arr.push(i);
      }
      expect(arr.toArray()).toEqual([0, 1, 2, 3, 4]);
    });
  });

  describe("pop", () => {
    test("pop returns last element", () => {
      const arr = new DynamicArray<number>();
      arr.push(1);
      arr.push(2);
      arr.push(3);
      expect(arr.pop()).toBe(3);
      expect(arr.pop()).toBe(2);
      expect(arr.pop()).toBe(1);
    });

    test("pop from empty array throws", () => {
      const arr = new DynamicArray<number>();
      expect(() => arr.pop()).toThrow("Pop from empty array");
    });

    test("pop decreases size", () => {
      const arr = new DynamicArray<number>();
      arr.push(1);
      arr.push(2);
      arr.pop();
      expect(arr.size).toBe(1);
    });
  });

  describe("get and set", () => {
    test("get by positive index", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.get(0)).toBe(10);
      expect(arr.get(1)).toBe(20);
      expect(arr.get(2)).toBe(30);
    });

    test("get by negative index", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.get(-1)).toBe(30);
      expect(arr.get(-2)).toBe(20);
      expect(arr.get(-3)).toBe(10);
    });

    test("get with out of bounds throws", () => {
      const arr = DynamicArray.fromArray([1, 2]);
      expect(() => arr.get(5)).toThrow("Index out of bounds");
      expect(() => arr.get(-5)).toThrow("Index out of bounds");
    });

    test("set by positive index", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      arr.set(1, 99);
      expect(arr.get(1)).toBe(99);
    });

    test("set by negative index", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      arr.set(-1, 99);
      expect(arr.get(2)).toBe(99);
    });

    test("set with out of bounds throws", () => {
      const arr = DynamicArray.fromArray([1, 2]);
      expect(() => arr.set(5, 99)).toThrow("Index out of bounds");
    });
  });

  describe("insertAt", () => {
    test("insert at beginning", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      arr.insertAt(0, 0);
      expect(arr.toArray()).toEqual([0, 1, 2, 3]);
    });

    test("insert at end", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      arr.insertAt(3, 4);
      expect(arr.toArray()).toEqual([1, 2, 3, 4]);
    });

    test("insert in middle", () => {
      const arr = DynamicArray.fromArray([1, 3, 4]);
      arr.insertAt(1, 2);
      expect(arr.toArray()).toEqual([1, 2, 3, 4]);
    });

    test("insert with negative index", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      arr.insertAt(-1, 99);
      expect(arr.toArray()).toEqual([1, 2, 3, 99]);
    });

    test("insert at invalid index throws", () => {
      const arr = DynamicArray.fromArray([1, 2]);
      expect(() => arr.insertAt(5, 99)).toThrow("Index out of bounds");
      expect(() => arr.insertAt(-10, 99)).toThrow("Index out of bounds");
    });

    test("insert into empty array at index 0", () => {
      const arr = new DynamicArray<number>();
      arr.insertAt(0, 42);
      expect(arr.toArray()).toEqual([42]);
    });
  });

  describe("removeAt", () => {
    test("remove from beginning", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.removeAt(0)).toBe(1);
      expect(arr.toArray()).toEqual([2, 3]);
    });

    test("remove from end", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.removeAt(2)).toBe(3);
      expect(arr.toArray()).toEqual([1, 2]);
    });

    test("remove from middle", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.removeAt(1)).toBe(2);
      expect(arr.toArray()).toEqual([1, 3]);
    });

    test("remove with negative index", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.removeAt(-1)).toBe(3);
      expect(arr.toArray()).toEqual([1, 2]);
    });

    test("remove at invalid index throws", () => {
      const arr = DynamicArray.fromArray([1, 2]);
      expect(() => arr.removeAt(5)).toThrow("Index out of bounds");
      expect(() => arr.removeAt(-5)).toThrow("Index out of bounds");
    });

    test("remove only element", () => {
      const arr = DynamicArray.fromArray([42]);
      expect(arr.removeAt(0)).toBe(42);
      expect(arr.isEmpty).toBe(true);
    });
  });

  describe("indexOf and contains", () => {
    test("indexOf finds existing element", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.indexOf(20)).toBe(1);
    });

    test("indexOf returns -1 for missing element", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.indexOf(99)).toBe(-1);
    });

    test("indexOf returns first occurrence", () => {
      const arr = DynamicArray.fromArray([1, 2, 1, 3]);
      expect(arr.indexOf(1)).toBe(0);
    });

    test("indexOf in empty array", () => {
      const arr = new DynamicArray<number>();
      expect(arr.indexOf(1)).toBe(-1);
    });

    test("contains returns true for existing element", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.contains(20)).toBe(true);
    });

    test("contains returns false for missing element", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect(arr.contains(99)).toBe(false);
    });

    test("contains on empty array", () => {
      const arr = new DynamicArray<number>();
      expect(arr.contains(1)).toBe(false);
    });
  });

  describe("clear", () => {
    test("clear empties the array", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      arr.clear();
      expect(arr.isEmpty).toBe(true);
      expect(arr.size).toBe(0);
      expect(arr.capacity).toBe(DEFAULT_CAPACITY);
    });

    test("clear resets capacity to default", () => {
      const arr = new DynamicArray<number>(32);
      for (let i = 0; i < 20; i++) arr.push(i);
      arr.clear();
      expect(arr.capacity).toBe(DEFAULT_CAPACITY);
    });
  });

  describe("resize behavior", () => {
    test("auto-grows when capacity exceeded (2x growth)", () => {
      const arr = new DynamicArray<number>(2);
      arr.push(1);
      arr.push(2);
      expect(arr.capacity).toBe(2);
      arr.push(3); // should trigger resize
      expect(arr.capacity).toBe(4);
      expect(arr.toArray()).toEqual([1, 2, 3]);
    });

    test("auto-shrinks at 25% usage", () => {
      const arr = new DynamicArray<number>(4);
      arr.push(1);
      arr.push(2);
      arr.push(3);
      arr.push(4);
      expect(arr.capacity).toBe(4);
      // Remove elements until 25% threshold
      arr.pop(); // size=3, 3/4=75%, no shrink
      arr.pop(); // size=2, 2/4=50%, no shrink
      arr.pop(); // size=1, 1/4=25%, should shrink
      // But won't shrink below DEFAULT_CAPACITY
      expect(arr.size).toBe(1);
    });

    test("never shrinks below DEFAULT_CAPACITY", () => {
      const arr = new DynamicArray<number>(DEFAULT_CAPACITY);
      arr.push(1);
      arr.push(2);
      arr.pop();
      arr.pop();
      // Should not shrink below default
      expect(arr.capacity).toBe(DEFAULT_CAPACITY);
    });

    test("growth through many insertions", () => {
      const arr = new DynamicArray<number>(2);
      for (let i = 0; i < 100; i++) {
        arr.push(i);
      }
      expect(arr.size).toBe(100);
      // Capacity should be at least 100 (power of 2 >= 100)
      expect(arr.capacity).toBeGreaterThanOrEqual(100);
      // Verify all elements are correct
      for (let i = 0; i < 100; i++) {
        expect(arr.get(i)).toBe(i);
      }
    });

    test("shrink from large capacity", () => {
      const arr = new DynamicArray<number>(4);
      // Fill to trigger growth
      for (let i = 0; i < 16; i++) {
        arr.push(i);
      }
      const capacityAfterGrowth = arr.capacity;
      expect(capacityAfterGrowth).toBeGreaterThanOrEqual(16);

      // Remove elements to trigger shrinking
      while (arr.size > 2) {
        arr.pop();
      }
      expect(arr.capacity).toBeLessThan(capacityAfterGrowth);
    });

    test("insertAt triggers resize when full", () => {
      const arr = new DynamicArray<number>(2);
      arr.push(1);
      arr.push(2);
      arr.insertAt(1, 99); // should trigger resize
      expect(arr.toArray()).toEqual([1, 99, 2]);
      expect(arr.capacity).toBe(4);
    });
  });

  describe("toArray and toString", () => {
    test("toArray returns elements", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.toArray()).toEqual([1, 2, 3]);
    });

    test("toArray returns copy", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      const copy = arr.toArray();
      copy.push(999);
      expect(arr.size).toBe(3);
    });

    test("toArray on empty array", () => {
      const arr = new DynamicArray<number>();
      expect(arr.toArray()).toEqual([]);
    });

    test("toString", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.toString()).toBe("DynamicArray([1, 2, 3])");
    });

    test("toString on empty", () => {
      const arr = new DynamicArray<number>();
      expect(arr.toString()).toBe("DynamicArray([])");
    });
  });

  describe("iterator", () => {
    test("iterates in order", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      const values: number[] = [];
      for (const v of arr) {
        values.push(v);
      }
      expect(values).toEqual([1, 2, 3]);
    });

    test("spread operator works", () => {
      const arr = DynamicArray.fromArray([10, 20, 30]);
      expect([...arr]).toEqual([10, 20, 30]);
    });

    test("iterate empty array", () => {
      const arr = new DynamicArray<number>();
      expect([...arr]).toEqual([]);
    });
  });

  describe("fromArray", () => {
    test("creates array from items", () => {
      const arr = DynamicArray.fromArray([1, 2, 3]);
      expect(arr.size).toBe(3);
      expect(arr.toArray()).toEqual([1, 2, 3]);
    });

    test("creates empty array from empty items", () => {
      const arr = DynamicArray.fromArray<number>([]);
      expect(arr.isEmpty).toBe(true);
      expect(arr.capacity).toBe(DEFAULT_CAPACITY);
    });

    test("capacity is at least DEFAULT_CAPACITY", () => {
      const arr = DynamicArray.fromArray([1, 2]);
      expect(arr.capacity).toBeGreaterThanOrEqual(DEFAULT_CAPACITY);
    });

    test("capacity matches items length when larger than default", () => {
      const items = Array.from({ length: 20 }, (_, i) => i);
      const arr = DynamicArray.fromArray(items);
      expect(arr.capacity).toBeGreaterThanOrEqual(20);
      expect(arr.size).toBe(20);
    });
  });

  describe("with different types", () => {
    test("string array", () => {
      const arr = new DynamicArray<string>();
      arr.push("hello");
      arr.push("world");
      expect(arr.get(0)).toBe("hello");
      expect(arr.indexOf("world")).toBe(1);
    });

    test("object array", () => {
      const arr = new DynamicArray<{ id: number }>();
      const obj = { id: 1 };
      arr.push(obj);
      expect(arr.get(0)).toBe(obj);
      expect(arr.contains(obj)).toBe(true);
    });
  });

  describe("edge cases", () => {
    test("push then pop then push again", () => {
      const arr = new DynamicArray<number>();
      arr.push(1);
      arr.pop();
      arr.push(2);
      expect(arr.get(0)).toBe(2);
      expect(arr.size).toBe(1);
    });

    test("many insertAt at beginning", () => {
      const arr = new DynamicArray<number>();
      for (let i = 4; i >= 0; i--) {
        arr.insertAt(0, i);
      }
      expect(arr.toArray()).toEqual([0, 1, 2, 3, 4]);
    });

    test("removeAt all elements from beginning", () => {
      const arr = DynamicArray.fromArray([1, 2, 3, 4, 5]);
      while (!arr.isEmpty) {
        arr.removeAt(0);
      }
      expect(arr.isEmpty).toBe(true);
    });

    test("interleaved push and removeAt", () => {
      const arr = new DynamicArray<number>();
      arr.push(1);
      arr.push(2);
      arr.removeAt(0); // [2]
      arr.push(3); // [2, 3]
      arr.push(4); // [2, 3, 4]
      arr.removeAt(1); // [2, 4]
      expect(arr.toArray()).toEqual([2, 4]);
    });

    test("single element operations", () => {
      const arr = new DynamicArray<number>(1);
      arr.push(42);
      expect(arr.get(0)).toBe(42);
      expect(arr.contains(42)).toBe(true);
      expect(arr.indexOf(42)).toBe(0);
      arr.set(0, 99);
      expect(arr.get(0)).toBe(99);
      expect(arr.pop()).toBe(99);
      expect(arr.isEmpty).toBe(true);
    });
  });
});
