import { describe, test, expect } from "bun:test";
import {
  Node,
  SinglyLinkedList,
} from "../../src/data-structures/singly-linked-list";

describe("Node", () => {
  test("creates node with data", () => {
    const node = new Node(42);
    expect(node.data).toBe(42);
    expect(node.next).toBeNull();
  });

  test("creates node with next pointer", () => {
    const next = new Node(2);
    const node = new Node(1, next);
    expect(node.data).toBe(1);
    expect(node.next).toBe(next);
  });

  test("toString shows data", () => {
    const node = new Node(42);
    expect(node.toString()).toBe("Node(42)");
  });
});

describe("SinglyLinkedList", () => {
  describe("construction and basic properties", () => {
    test("new list is empty", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.isEmpty).toBe(true);
      expect(list.size).toBe(0);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });
  });

  describe("prepend", () => {
    test("prepend to empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.prepend(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(1);
      expect(list.size).toBe(1);
    });

    test("prepend to non-empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.prepend(2);
      list.prepend(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(2);
      expect(list.toArray()).toEqual([1, 2]);
    });

    test("multiple prepends", () => {
      const list = new SinglyLinkedList<number>();
      list.prepend(3);
      list.prepend(2);
      list.prepend(1);
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.size).toBe(3);
    });
  });

  describe("append", () => {
    test("append to empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.append(1);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(1);
      expect(list.size).toBe(1);
    });

    test("append to non-empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.append(1);
      list.append(2);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(2);
      expect(list.toArray()).toEqual([1, 2]);
    });

    test("multiple appends", () => {
      const list = new SinglyLinkedList<number>();
      list.append(1);
      list.append(2);
      list.append(3);
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.size).toBe(3);
    });
  });

  describe("insertAt", () => {
    test("insert at beginning", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(0, 0);
      expect(list.toArray()).toEqual([0, 1, 2, 3]);
      expect(list.head).toBe(0);
    });

    test("insert at end", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(3, 4);
      expect(list.toArray()).toEqual([1, 2, 3, 4]);
      expect(list.tail).toBe(4);
    });

    test("insert in middle", () => {
      const list = SinglyLinkedList.fromArray([1, 3, 4]);
      list.insertAt(1, 2);
      expect(list.toArray()).toEqual([1, 2, 3, 4]);
    });

    test("insert with negative index", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      list.insertAt(-1, 99);
      expect(list.toArray()).toEqual([1, 2, 3, 99]);
    });

    test("insert at invalid index throws", () => {
      const list = SinglyLinkedList.fromArray([1, 2]);
      expect(() => list.insertAt(5, 99)).toThrow("Index out of bounds");
      expect(() => list.insertAt(-10, 99)).toThrow("Index out of bounds");
    });

    test("insert into empty list at index 0", () => {
      const list = new SinglyLinkedList<number>();
      list.insertAt(0, 42);
      expect(list.toArray()).toEqual([42]);
    });
  });

  describe("popFront", () => {
    test("pop from single-element list", () => {
      const list = SinglyLinkedList.fromArray([42]);
      expect(list.popFront()).toBe(42);
      expect(list.isEmpty).toBe(true);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });

    test("pop from multi-element list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.popFront()).toBe(1);
      expect(list.head).toBe(2);
      expect(list.size).toBe(2);
    });

    test("pop from empty list throws", () => {
      const list = new SinglyLinkedList<number>();
      expect(() => list.popFront()).toThrow("Pop from empty list");
    });
  });

  describe("popBack", () => {
    test("pop from single-element list", () => {
      const list = SinglyLinkedList.fromArray([42]);
      expect(list.popBack()).toBe(42);
      expect(list.isEmpty).toBe(true);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });

    test("pop from multi-element list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.popBack()).toBe(3);
      expect(list.tail).toBe(2);
      expect(list.size).toBe(2);
    });

    test("pop all from back", () => {
      const list = SinglyLinkedList.fromArray([1, 2]);
      expect(list.popBack()).toBe(2);
      expect(list.popBack()).toBe(1);
      expect(list.isEmpty).toBe(true);
    });

    test("pop from empty list throws", () => {
      const list = new SinglyLinkedList<number>();
      expect(() => list.popBack()).toThrow("Pop from empty list");
    });
  });

  describe("removeValue", () => {
    test("remove existing value from head", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(1)).toBe(true);
      expect(list.toArray()).toEqual([2, 3]);
      expect(list.head).toBe(2);
    });

    test("remove existing value from tail", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(3)).toBe(true);
      expect(list.toArray()).toEqual([1, 2]);
      expect(list.tail).toBe(2);
    });

    test("remove existing value from middle", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(2)).toBe(true);
      expect(list.toArray()).toEqual([1, 3]);
    });

    test("remove non-existing value returns false", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.removeValue(99)).toBe(false);
      expect(list.size).toBe(3);
    });

    test("remove from empty list returns false", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.removeValue(1)).toBe(false);
    });

    test("remove only element", () => {
      const list = SinglyLinkedList.fromArray([42]);
      expect(list.removeValue(42)).toBe(true);
      expect(list.isEmpty).toBe(true);
    });
  });

  describe("find", () => {
    test("find existing element", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(20)).toBe(1);
    });

    test("find first element", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(10)).toBe(0);
    });

    test("find last element", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(30)).toBe(2);
    });

    test("find non-existing element returns -1", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.find(99)).toBe(-1);
    });

    test("find in empty list returns -1", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.find(1)).toBe(-1);
    });

    test("find returns first occurrence", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 1, 3]);
      expect(list.find(1)).toBe(0);
    });
  });

  describe("get", () => {
    test("get by positive index", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.get(0)).toBe(10);
      expect(list.get(1)).toBe(20);
      expect(list.get(2)).toBe(30);
    });

    test("get by negative index", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect(list.get(-1)).toBe(30);
      expect(list.get(-2)).toBe(20);
      expect(list.get(-3)).toBe(10);
    });

    test("get with out of bounds index throws", () => {
      const list = SinglyLinkedList.fromArray([1, 2]);
      expect(() => list.get(5)).toThrow("Index out of bounds");
      expect(() => list.get(-5)).toThrow("Index out of bounds");
    });
  });

  describe("reverse", () => {
    test("reverse multi-element list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4]);
      list.reverse();
      expect(list.toArray()).toEqual([4, 3, 2, 1]);
      expect(list.head).toBe(4);
      expect(list.tail).toBe(1);
    });

    test("reverse single-element list", () => {
      const list = SinglyLinkedList.fromArray([42]);
      list.reverse();
      expect(list.toArray()).toEqual([42]);
    });

    test("reverse empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.reverse(); // should not throw
      expect(list.isEmpty).toBe(true);
    });

    test("reverse two-element list", () => {
      const list = SinglyLinkedList.fromArray([1, 2]);
      list.reverse();
      expect(list.toArray()).toEqual([2, 1]);
      expect(list.head).toBe(2);
      expect(list.tail).toBe(1);
    });
  });

  describe("getMiddle", () => {
    test("odd length list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.getMiddle()).toBe(3);
    });

    test("even length list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4]);
      expect(list.getMiddle()).toBe(2);
    });

    test("single element", () => {
      const list = SinglyLinkedList.fromArray([42]);
      expect(list.getMiddle()).toBe(42);
    });

    test("two elements", () => {
      const list = SinglyLinkedList.fromArray([1, 2]);
      expect(list.getMiddle()).toBe(1);
    });

    test("empty list", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.getMiddle()).toBeNull();
    });
  });

  describe("hasCycle", () => {
    test("no cycle in normal list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.hasCycle()).toBe(false);
    });

    test("no cycle in empty list", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.hasCycle()).toBe(false);
    });

    test("no cycle in single element", () => {
      const list = SinglyLinkedList.fromArray([1]);
      expect(list.hasCycle()).toBe(false);
    });

    test("detects cycle", () => {
      const list = new SinglyLinkedList<number>();
      list.append(1);
      list.append(2);
      list.append(3);
      // Create a cycle: tail points back to head
      const tailNode = list._getTailNode()!;
      const headNode = list._getHeadNode()!;
      tailNode.next = headNode;
      expect(list.hasCycle()).toBe(true);
      // Clean up cycle to prevent infinite loops in other tests
      tailNode.next = null;
    });
  });

  describe("getNthFromEnd", () => {
    test("get last element (n=1)", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.getNthFromEnd(1)).toBe(5);
    });

    test("get first element (n=size)", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.getNthFromEnd(5)).toBe(1);
    });

    test("get middle element", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4, 5]);
      expect(list.getNthFromEnd(3)).toBe(3);
    });

    test("n=0 returns null", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.getNthFromEnd(0)).toBeNull();
    });

    test("n greater than size returns null", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.getNthFromEnd(10)).toBeNull();
    });

    test("negative n returns null", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.getNthFromEnd(-1)).toBeNull();
    });
  });

  describe("removeDuplicates", () => {
    test("removes duplicates preserving first occurrence", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 2, 1, 4]);
      list.removeDuplicates();
      expect(list.toArray()).toEqual([1, 2, 3, 4]);
    });

    test("no duplicates unchanged", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      list.removeDuplicates();
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("all same elements", () => {
      const list = SinglyLinkedList.fromArray([5, 5, 5, 5]);
      list.removeDuplicates();
      expect(list.toArray()).toEqual([5]);
      expect(list.size).toBe(1);
    });

    test("empty list", () => {
      const list = new SinglyLinkedList<number>();
      list.removeDuplicates();
      expect(list.isEmpty).toBe(true);
    });

    test("single element", () => {
      const list = SinglyLinkedList.fromArray([42]);
      list.removeDuplicates();
      expect(list.toArray()).toEqual([42]);
    });

    test("duplicates at tail are removed", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 3]);
      list.removeDuplicates();
      expect(list.toArray()).toEqual([1, 2, 3]);
      expect(list.tail).toBe(3);
    });
  });

  describe("toArray", () => {
    test("returns array of elements", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("empty list returns empty array", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.toArray()).toEqual([]);
    });
  });

  describe("clear", () => {
    test("clears all elements", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      list.clear();
      expect(list.isEmpty).toBe(true);
      expect(list.size).toBe(0);
      expect(list.head).toBeNull();
      expect(list.tail).toBeNull();
    });
  });

  describe("map", () => {
    test("maps values to new list", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      const doubled = list.map((x) => x * 2);
      expect(doubled.toArray()).toEqual([2, 4, 6]);
      // original unchanged
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("map empty list", () => {
      const list = new SinglyLinkedList<number>();
      const result = list.map((x) => x * 2);
      expect(result.isEmpty).toBe(true);
    });

    test("map to different type", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      const strings = list.map((x) => x.toString());
      expect(strings.toArray()).toEqual(["1", "2", "3"]);
    });
  });

  describe("filter", () => {
    test("filters elements", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3, 4, 5]);
      const evens = list.filter((x) => x % 2 === 0);
      expect(evens.toArray()).toEqual([2, 4]);
      // original unchanged
      expect(list.size).toBe(5);
    });

    test("filter with no matches", () => {
      const list = SinglyLinkedList.fromArray([1, 3, 5]);
      const evens = list.filter((x) => x % 2 === 0);
      expect(evens.isEmpty).toBe(true);
    });

    test("filter with all matching", () => {
      const list = SinglyLinkedList.fromArray([2, 4, 6]);
      const evens = list.filter((x) => x % 2 === 0);
      expect(evens.toArray()).toEqual([2, 4, 6]);
    });

    test("filter empty list", () => {
      const list = new SinglyLinkedList<number>();
      const result = list.filter(() => true);
      expect(result.isEmpty).toBe(true);
    });
  });

  describe("iterator", () => {
    test("iterates in order", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      const values: number[] = [];
      for (const v of list) {
        values.push(v);
      }
      expect(values).toEqual([1, 2, 3]);
    });

    test("spread operator works", () => {
      const list = SinglyLinkedList.fromArray([10, 20, 30]);
      expect([...list]).toEqual([10, 20, 30]);
    });

    test("iterate empty list", () => {
      const list = new SinglyLinkedList<number>();
      expect([...list]).toEqual([]);
    });
  });

  describe("fromArray", () => {
    test("creates list from array", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.size).toBe(3);
      expect(list.head).toBe(1);
      expect(list.tail).toBe(3);
      expect(list.toArray()).toEqual([1, 2, 3]);
    });

    test("creates empty list from empty array", () => {
      const list = SinglyLinkedList.fromArray<number>([]);
      expect(list.isEmpty).toBe(true);
    });
  });

  describe("toString", () => {
    test("shows linked list representation", () => {
      const list = SinglyLinkedList.fromArray([1, 2, 3]);
      expect(list.toString()).toBe("SinglyLinkedList([1 -> 2 -> 3])");
    });

    test("empty list", () => {
      const list = new SinglyLinkedList<number>();
      expect(list.toString()).toBe("SinglyLinkedList([])");
    });
  });

  describe("with string type", () => {
    test("works with strings", () => {
      const list = SinglyLinkedList.fromArray(["a", "b", "c"]);
      expect(list.find("b")).toBe(1);
      list.prepend("z");
      expect(list.toArray()).toEqual(["z", "a", "b", "c"]);
    });
  });
});
