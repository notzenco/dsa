import { describe, test, expect } from "bun:test";
import {
  DNode,
  DoublyLinkedList,
} from "../../src/data-structures/doubly-linked-list";

describe("DNode", () => {
  test("creates node with data", () => {
    const node = new DNode(42);
    expect(node.data).toBe(42);
    expect(node.prev).toBeNull();
    expect(node.next).toBeNull();
  });

  test("creates node with prev and next", () => {
    const prev = new DNode(1);
    const next = new DNode(3);
    const node = new DNode(2, prev, next);
    expect(node.data).toBe(2);
    expect(node.prev).toBe(prev);
    expect(node.next).toBe(next);
  });

  test("toString shows data", () => {
    const node = new DNode(42);
    expect(node.toString()).toBe("DNode(42)");
  });
});

describe("DoublyLinkedList", () => {
  describe("construction and basic properties", () => {
    test("new list is empty", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.isEmpty).toBe(true);
      expect(list.size).toBe(0);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });
  });

  describe("prepend", () => {
    test("prepend to empty list", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.prepend(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(1);
      expect(list.size).toBe(1);
      expect(node.data).toBe(1);
    });

    test("prepend to non-empty list", () => {
      const list = new DoublyLinkedList<number>();
      list.prepend(2);
      list.prepend(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(2);
      expect(list.toArray()).toEqual([1, 2]);
    });

    test("multiple prepends", () => {
      const list = new DoublyLinkedList<number>();
      list.prepend(3);
      list.prepend(2);
      list.prepend(1);
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.size).toBe(3);
    });
  });

  describe("append", () => {
    test("append to empty list", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(1);
      expect(list.size).toBe(1);
      expect(node.data).toBe(1);
    });

    test("append to non-empty list", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(2);
      expect(list.toArray()).toEqual([1, 2]);
    });

    test("multiple appends", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      list.append(3);
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.size).toBe(3);
    });
  });

  describe("insertAt", () => {
    test("insert at beginning", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(0, 0);
      expect(list.toArray()).toEqual([0, 1, 2, 3]);
      expect(list.head).toBe(0);
    });

    test("insert at end", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(3, 4);
      expect(list.toArray()).toEqual([1, 2, 3, 4]);
      expect(list.tail).toBe(4);
    });

    test("insert in middle", () => {
      const list = DoublyLinkedList.fromArray([1, 3, 4]);
      list.insertAt(1, 2);
      expect(list.toArray()).toEqual([1, 2, 3, 4]);
    });

    test("insert with negative index", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(-1, 99);
      expect(list.toArray()).toEqual([1, 2, 3, 99]);
    });

    test("insert at invalid index throws", () => {
      const list = DoublyLinkedList.fromArray([1, 2]);
      expect(() => list.insertAt(5, 99)).toThrow("Index out of bounds");
      expect(() => list.insertAt(-10, 99)).toThrow("Index out of bounds");
    });

    test("insert into empty list at index 0", () => {
      const list = new DoublyLinkedList<number>();
      list.insertAt(0, 42);
      expect(list.toArray()).toEqual([42]);
      expect(list.head).toBe(42);
      expect(list.tail).toBe(42);
    });

    test("insert preserves bidirectional links", () => {
      const list = DoublyLinkedList.fromArray([1, 3]);
      list.insertAt(1, 2);
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.toArrayReverse()).toEqual([3, 2, 1]);
    });
  });

  describe("popFront", () => {
    test("pop from single-element list", () => {
      const list = DoublyLinkedList.fromArray([42]);
      expect(list.popFront()).toBe(42);
      expect(list.isEmpty).toBe(true);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });

    test("pop from multi-element list", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.popFront()).toBe(1);
      expect(list.head).toBe(2);
      expect(list.size).toBe(2);
    });

    test("pop all from front", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.popFront()).toBe(1);
      expect(list.popFront()).toBe(2);
      expect(list.popFront()).toBe(3);
      expect(list.isEmpty).toBe(true);
    });

    test("pop from empty list throws", () => {
      const list = new DoublyLinkedList<number>();
      expect(() => list.popFront()).toThrow("Pop from empty list");
    });
  });

  describe("popBack", () => {
    test("pop from single-element list", () => {
      const list = DoublyLinkedList.fromArray([42]);
      expect(list.popBack()).toBe(42);
      expect(list.isEmpty).toBe(true);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });

    test("pop from multi-element list", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.popBack()).toBe(3);
      expect(list.tail).toBe(2);
      expect(list.size).toBe(2);
    });

    test("pop all from back", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.popBack()).toBe(3);
      expect(list.popBack()).toBe(2);
      expect(list.popBack()).toBe(1);
      expect(list.isEmpty).toBe(true);
    });

    test("pop from empty list throws", () => {
      const list = new DoublyLinkedList<number>();
      expect(() => list.popBack()).toThrow("Pop from empty list");
    });
  });

  describe("removeValue", () => {
    test("remove existing value from head", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(1)).toBe(true);
      expect(list.toArray()).toEqual([2, 3]);
      expect(list.head).toBe(2);
    });

    test("remove existing value from tail", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(3)).toBe(true);
      expect(list.toArray()).toEqual([1, 2]);
      expect(list.tail).toBe(2);
    });

    test("remove existing value from middle", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(2)).toBe(true);
      expect(list.toArray()).toEqual([1, 3]);
    });

    test("remove non-existing value returns false", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(99)).toBe(false);
      expect(list.size).toBe(3);
    });

    test("remove from empty list returns false", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.removeValue(1)).toBe(false);
    });

    test("remove only element", () => {
      const list = DoublyLinkedList.fromArray([42]);
      expect(list.removeValue(42)).toBe(true);
      expect(list.isEmpty).toBe(true);
    });

    test("remove preserves reverse traversal", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4, 5]);
      list.removeValue(3);
      expect(list.toArray()).toEqual([1, 2, 4, 5]);
      expect(list.toArrayReverse()).toEqual([5, 4, 2, 1]);
    });
  });

  describe("removeNode", () => {
    test("remove head node", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(1);
      list.append(2);
      list.append(3);
      expect(list.removeNode(node)).toBe(1);
      expect(list.toArray()).toEqual([2, 3]);
      expect(list.head).toBe(2);
    });

    test("remove tail node", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      const node = list.append(3);
      expect(list.removeNode(node)).toBe(3);
      expect(list.toArray()).toEqual([1, 2]);
      expect(list.tail).toBe(2);
    });

    test("remove middle node", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      const node = list.append(2);
      list.append(3);
      expect(list.removeNode(node)).toBe(2);
      expect(list.toArray()).toEqual([1, 3]);
    });

    test("remove only node", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(42);
      expect(list.removeNode(node)).toBe(42);
      expect(list.isEmpty).toBe(true);
    });
  });

  describe("find", () => {
    test("find existing element returns index", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(20)).toBe(1);
    });

    test("find first element", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(10)).toBe(0);
    });

    test("find last element", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(30)).toBe(2);
    });

    test("find non-existing element returns -1", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(99)).toBe(-1);
    });

    test("find in empty list returns -1", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.find(1)).toBe(-1);
    });

    test("find returns first occurrence", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 1, 3]);
      expect(list.find(1)).toBe(0);
    });
  });

  describe("findNode", () => {
    test("finds existing node", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      const node = list.findNode(20);
      expect(node).not.toBeNull();
      expect(node!.data).toBe(20);
    });

    test("returns null for non-existing value", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.findNode(99)).toBeNull();
    });

    test("returns null for empty list", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.findNode(1)).toBeNull();
    });
  });

  describe("get", () => {
    test("get by positive index", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.get(0)).toBe(10);
      expect(list.get(1)).toBe(20);
      expect(list.get(2)).toBe(30);
    });

    test("get by negative index", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect(list.get(-1)).toBe(30);
      expect(list.get(-2)).toBe(20);
      expect(list.get(-3)).toBe(10);
    });

    test("get with out of bounds index throws", () => {
      const list = DoublyLinkedList.fromArray([1, 2]);
      expect(() => list.get(5)).toThrow("Index out of bounds");
      expect(() => list.get(-5)).toThrow("Index out of bounds");
    });

    test("get traverses from closer end", () => {
      // With 5 elements, index 4 should traverse from tail
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.get(4)).toBe(5);
      expect(list.get(0)).toBe(1);
    });
  });

  describe("set", () => {
    test("set by positive index", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      list.set(1, 99);
      expect(list.toArray()).toEqual([10, 99, 30]);
    });

    test("set by negative index", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      list.set(-1, 99);
      expect(list.toArray()).toEqual([10, 20, 99]);
    });

    test("set with out of bounds index throws", () => {
      const list = DoublyLinkedList.fromArray([1, 2]);
      expect(() => list.set(5, 99)).toThrow("Index out of bounds");
    });
  });

  describe("reverse", () => {
    test("reverse multi-element list", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4]);
      list.reverse();
      expect(list.toArray()).toEqual([4, 3, 2, 1]);
      expect(list.head).toBe(4);
      expect(list.tail).toBe(1);
    });

    test("reverse preserves bidirectional links", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      list.reverse();
      expect(list.toArray()).toEqual([3, 2, 1]);
      expect(list.toArrayReverse()).toEqual([1, 2, 3]);
    });

    test("reverse single-element list", () => {
      const list = DoublyLinkedList.fromArray([42]);
      list.reverse();
      expect(list.toArray()).toEqual([42]);
    });

    test("reverse empty list", () => {
      const list = new DoublyLinkedList<number>();
      list.reverse();
      expect(list.isEmpty).toBe(true);
    });

    test("reverse two-element list", () => {
      const list = DoublyLinkedList.fromArray([1, 2]);
      list.reverse();
      expect(list.toArray()).toEqual([2, 1]);
      expect(list.head).toBe(2);
      expect(list.tail).toBe(1);
    });

    test("double reverse restores original", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4, 5]);
      list.reverse();
      list.reverse();
      expect(list.toArray()).toEqual([1, 2, 3, 4, 5]);
    });
  });

  describe("moveToFront", () => {
    test("move tail to front", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      const node = list.append(3);
      list.moveToFront(node);
      expect(list.toArray()).toEqual([3, 1, 2]);
      expect(list.head).toBe(3);
      expect(list.tail).toBe(2);
    });

    test("move middle to front", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      const node = list.append(2);
      list.append(3);
      list.moveToFront(node);
      expect(list.toArray()).toEqual([2, 1, 3]);
    });

    test("move head to front is no-op", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(1);
      list.append(2);
      list.append(3);
      list.moveToFront(node);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("move preserves bidirectional links", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      const node = list.append(3);
      list.moveToFront(node);
      expect(list.toArray()).toEqual([3, 1, 2]);
      expect(list.toArrayReverse()).toEqual([2, 1, 3]);
    });
  });

  describe("moveToBack", () => {
    test("move head to back", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(1);
      list.append(2);
      list.append(3);
      list.moveToBack(node);
      expect(list.toArray()).toEqual([2, 3, 1]);
      expect(list.head).toBe(2);
      expect(list.tail).toBe(1);
    });

    test("move middle to back", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      const node = list.append(2);
      list.append(3);
      list.moveToBack(node);
      expect(list.toArray()).toEqual([1, 3, 2]);
    });

    test("move tail to back is no-op", () => {
      const list = new DoublyLinkedList<number>();
      list.append(1);
      list.append(2);
      const node = list.append(3);
      list.moveToBack(node);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("move preserves bidirectional links", () => {
      const list = new DoublyLinkedList<number>();
      const node = list.append(1);
      list.append(2);
      list.append(3);
      list.moveToBack(node);
      expect(list.toArray()).toEqual([2, 3, 1]);
      expect(list.toArrayReverse()).toEqual([1, 3, 2]);
    });
  });

  describe("toArray", () => {
    test("returns array of elements in order", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("empty list returns empty array", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.toArray()).toEqual([]);
    });
  });

  describe("toArrayReverse", () => {
    test("returns array in reverse order", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.toArrayReverse()).toEqual([3, 2, 1]);
    });

    test("empty list returns empty array", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.toArrayReverse()).toEqual([]);
    });

    test("single element", () => {
      const list = DoublyLinkedList.fromArray([42]);
      expect(list.toArrayReverse()).toEqual([42]);
    });
  });

  describe("clear", () => {
    test("clears all elements", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      list.clear();
      expect(list.isEmpty).toBe(true);
      expect(list.size).toBe(0);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });
  });

  describe("toString", () => {
    test("shows doubly linked list representation", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.toString()).toBe("DoublyLinkedList([1 <-> 2 <-> 3])");
    });

    test("empty list", () => {
      const list = new DoublyLinkedList<number>();
      expect(list.toString()).toBe("DoublyLinkedList([])");
    });

    test("single element", () => {
      const list = DoublyLinkedList.fromArray([42]);
      expect(list.toString()).toBe("DoublyLinkedList([42])");
    });
  });

  describe("iterator", () => {
    test("iterates in order", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      const values: number[] = [];
      for (const v of list) {
        values.push(v);
      }
      expect(values).toEqual([1, 2, 3]);
    });

    test("spread operator works", () => {
      const list = DoublyLinkedList.fromArray([10, 20, 30]);
      expect([...list]).toEqual([10, 20, 30]);
    });

    test("iterate empty list", () => {
      const list = new DoublyLinkedList<number>();
      expect([...list]).toEqual([]);
    });
  });

  describe("fromArray", () => {
    test("creates list from array", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3]);
      expect(list.size).toBe(3);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(3);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("creates empty list from empty array", () => {
      const list = DoublyLinkedList.fromArray<number>([]);
      expect(list.isEmpty).toBe(true);
    });

    test("bidirectional traversal works after fromArray", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.toArray()).toEqual([1, 2, 3, 4, 5]);
      expect(list.toArrayReverse()).toEqual([5, 4, 3, 2, 1]);
    });
  });

  describe("with different types", () => {
    test("string list", () => {
      const list = DoublyLinkedList.fromArray(["a", "b", "c"]);
      expect(list.find("b")).toBe(1);
      list.prepend("z");
      expect(list.toArray()).toEqual(["z", "a", "b", "c"]);
    });

    test("object list", () => {
      const list = new DoublyLinkedList<{ id: number }>();
      list.append({ id: 1 });
      list.append({ id: 2 });
      expect(list.get(0)).toEqual({ id: 1 });
    });
  });

  describe("edge cases", () => {
    test("alternating popFront and popBack", () => {
      const list = DoublyLinkedList.fromArray([1, 2, 3, 4]);
      expect(list.popFront()).toBe(1);
      expect(list.popBack()).toBe(4);
      expect(list.popFront()).toBe(2);
      expect(list.popBack()).toBe(3);
      expect(list.isEmpty).toBe(true);
    });

    test("prepend and append interleaved", () => {
      const list = new DoublyLinkedList<number>();
      list.append(2);
      list.prepend(1);
      list.append(3);
      list.prepend(0);
      expect(list.toArray()).toEqual([0, 1, 2, 3]);
      expect(list.toArrayReverse()).toEqual([3, 2, 1, 0]);
    });

    test("single element operations", () => {
      const list = new DoublyLinkedList<number>();
      list.append(42);
      expect(list.head).toBe(42);
      expect(list.tail).toBe(42);
      expect(list.get(0)).toBe(42);
      expect(list.get(-1)).toBe(42);
      list.set(0, 99);
      expect(list.get(0)).toBe(99);
      list.reverse();
      expect(list.get(0)).toBe(99);
    });
  });
});
