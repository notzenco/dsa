import { describe, test, expect } from "bun:test";
import {
  AVLTree,
  AVLTreeMap,
  AVLNode,
} from "../../src/data-structures/avl-tree";

describe("AVLTree", () => {
  describe("empty tree", () => {
    test("should be empty initially", () => {
      const tree = new AVLTree<number>();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
    });

    test("height of empty tree is 0", () => {
      const tree = new AVLTree<number>();
      expect(tree.height()).toBe(0);
    });

    test("min and max return null for empty tree", () => {
      const tree = new AVLTree<number>();
      expect(tree.min()).toBeNull();
      expect(tree.max()).toBeNull();
    });

    test("search returns false for empty tree", () => {
      const tree = new AVLTree<number>();
      expect(tree.search(5)).toBe(false);
    });

    test("get returns null for empty tree", () => {
      const tree = new AVLTree<number>();
      expect(tree.get(5)).toBeNull();
    });

    test("delete returns false for empty tree", () => {
      const tree = new AVLTree<number>();
      expect(tree.delete(5)).toBe(false);
    });

    test("inOrder returns empty array", () => {
      const tree = new AVLTree<number>();
      expect(tree.inOrder()).toEqual([]);
    });

    test("isBalanced returns true for empty tree", () => {
      const tree = new AVLTree<number>();
      expect(tree.isBalanced()).toBe(true);
    });
  });

  describe("single element", () => {
    test("insert single element", () => {
      const tree = new AVLTree<number>();
      tree.insert(10);
      expect(tree.size).toBe(1);
      expect(tree.isEmpty()).toBe(false);
      expect(tree.search(10)).toBe(true);
    });

    test("height of single node is 1", () => {
      const tree = new AVLTree<number>();
      tree.insert(10);
      expect(tree.height()).toBe(1);
    });

    test("delete single element", () => {
      const tree = new AVLTree<number>();
      tree.insert(10);
      expect(tree.delete(10)).toBe(true);
      expect(tree.isEmpty()).toBe(true);
    });
  });

  describe("LL rotation (right rotation)", () => {
    test("triggers right rotation on left-left insertion", () => {
      const tree = new AVLTree<number>();
      // Insert 3, 2, 1 - causes LL imbalance at 3
      tree.insert(3);
      tree.insert(2);
      tree.insert(1);

      expect(tree.isBalanced()).toBe(true);
      expect(tree.inOrder()).toEqual([1, 2, 3]);
      // After rotation, root should be 2
      expect(tree.height()).toBe(2);
    });
  });

  describe("RR rotation (left rotation)", () => {
    test("triggers left rotation on right-right insertion", () => {
      const tree = new AVLTree<number>();
      // Insert 1, 2, 3 - causes RR imbalance at 1
      tree.insert(1);
      tree.insert(2);
      tree.insert(3);

      expect(tree.isBalanced()).toBe(true);
      expect(tree.inOrder()).toEqual([1, 2, 3]);
      expect(tree.height()).toBe(2);
    });
  });

  describe("LR rotation (left-right)", () => {
    test("triggers left-right rotation", () => {
      const tree = new AVLTree<number>();
      // Insert 3, 1, 2 - causes LR imbalance
      tree.insert(3);
      tree.insert(1);
      tree.insert(2);

      expect(tree.isBalanced()).toBe(true);
      expect(tree.inOrder()).toEqual([1, 2, 3]);
      expect(tree.height()).toBe(2);
    });
  });

  describe("RL rotation (right-left)", () => {
    test("triggers right-left rotation", () => {
      const tree = new AVLTree<number>();
      // Insert 1, 3, 2 - causes RL imbalance
      tree.insert(1);
      tree.insert(3);
      tree.insert(2);

      expect(tree.isBalanced()).toBe(true);
      expect(tree.inOrder()).toEqual([1, 2, 3]);
      expect(tree.height()).toBe(2);
    });
  });

  describe("insert", () => {
    test("multiple insertions maintain balance", () => {
      const tree = new AVLTree<number>();
      const values = [10, 20, 30, 40, 50, 25, 5, 15, 35, 45];

      for (const v of values) {
        tree.insert(v);
        expect(tree.isBalanced()).toBe(true);
      }

      expect(tree.size).toBe(10);
      expect(tree.inOrder()).toEqual([5, 10, 15, 20, 25, 30, 35, 40, 45, 50]);
    });

    test("ascending insertion maintains balance", () => {
      const tree = new AVLTree<number>();
      for (let i = 1; i <= 15; i++) {
        tree.insert(i);
        expect(tree.isBalanced()).toBe(true);
      }
      expect(tree.size).toBe(15);
    });

    test("descending insertion maintains balance", () => {
      const tree = new AVLTree<number>();
      for (let i = 15; i >= 1; i--) {
        tree.insert(i);
        expect(tree.isBalanced()).toBe(true);
      }
      expect(tree.size).toBe(15);
    });

    test("duplicate key updates value without increasing size", () => {
      const tree = new AVLTree<number, string>();
      tree.insert(5, "hello");
      tree.insert(5, "world");

      expect(tree.size).toBe(1);
      expect(tree.get(5)).toBe("world");
    });

    test("insert with key-value pairs", () => {
      const tree = new AVLTree<string, number>();
      tree.insert("a", 1);
      tree.insert("b", 2);
      tree.insert("c", 3);

      expect(tree.get("a")).toBe(1);
      expect(tree.get("b")).toBe(2);
      expect(tree.get("c")).toBe(3);
    });
  });

  describe("delete", () => {
    test("delete leaf node maintains balance", () => {
      const tree = new AVLTree<number>();
      [5, 3, 7, 2, 4, 6, 8].forEach((v) => tree.insert(v));

      expect(tree.delete(2)).toBe(true);
      expect(tree.isBalanced()).toBe(true);
      expect(tree.size).toBe(6);
    });

    test("delete node with one child maintains balance", () => {
      const tree = new AVLTree<number>();
      [5, 3, 7, 2].forEach((v) => tree.insert(v));

      expect(tree.delete(3)).toBe(true);
      expect(tree.isBalanced()).toBe(true);
    });

    test("delete node with two children maintains balance", () => {
      const tree = new AVLTree<number>();
      [5, 3, 7, 2, 4, 6, 8].forEach((v) => tree.insert(v));

      expect(tree.delete(5)).toBe(true);
      expect(tree.isBalanced()).toBe(true);
      expect(tree.search(5)).toBe(false);
    });

    test("delete root node", () => {
      const tree = new AVLTree<number>();
      [5, 3, 7].forEach((v) => tree.insert(v));

      expect(tree.delete(5)).toBe(true);
      expect(tree.isBalanced()).toBe(true);
      expect(tree.size).toBe(2);
    });

    test("delete non-existing key returns false", () => {
      const tree = new AVLTree<number>();
      tree.insert(10);
      expect(tree.delete(20)).toBe(false);
    });

    test("multiple deletions maintain balance", () => {
      const tree = new AVLTree<number>();
      const values = [10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8, 11, 13, 16, 20];
      values.forEach((v) => tree.insert(v));

      // Delete in various order
      for (const v of [10, 5, 15, 3, 7, 12, 18]) {
        expect(tree.delete(v)).toBe(true);
        expect(tree.isBalanced()).toBe(true);
      }
    });

    test("delete triggering rebalance", () => {
      const tree = new AVLTree<number>();
      [5, 3, 8, 7, 10].forEach((v) => tree.insert(v));

      // Deleting 3 causes imbalance that requires rotation
      tree.delete(3);
      expect(tree.isBalanced()).toBe(true);
    });
  });

  describe("search and get", () => {
    test("search finds existing keys", () => {
      const tree = new AVLTree<number>();
      [10, 5, 15, 3, 7].forEach((v) => tree.insert(v));

      expect(tree.search(10)).toBe(true);
      expect(tree.search(5)).toBe(true);
      expect(tree.search(15)).toBe(true);
    });

    test("search returns false for missing keys", () => {
      const tree = new AVLTree<number>();
      tree.insert(10);
      expect(tree.search(20)).toBe(false);
    });

    test("get returns value", () => {
      const tree = new AVLTree<number, string>();
      tree.insert(1, "one");
      tree.insert(2, "two");

      expect(tree.get(1)).toBe("one");
      expect(tree.get(2)).toBe("two");
      expect(tree.get(3)).toBeNull();
    });
  });

  describe("min and max", () => {
    test("min and max are correct", () => {
      const tree = new AVLTree<number>();
      [50, 30, 70, 10, 90].forEach((v) => tree.insert(v));

      expect(tree.min()).toBe(10);
      expect(tree.max()).toBe(90);
    });

    test("min and max after deletions", () => {
      const tree = new AVLTree<number>();
      [10, 20, 30, 40, 50].forEach((v) => tree.insert(v));

      tree.delete(10);
      expect(tree.min()).toBe(20);

      tree.delete(50);
      expect(tree.max()).toBe(40);
    });
  });

  describe("getBalance", () => {
    test("balance factor is within [-1, 1] after operations", () => {
      const tree = new AVLTree<number>();
      for (let i = 1; i <= 20; i++) {
        tree.insert(i);
      }
      expect(tree.isBalanced()).toBe(true);
    });
  });

  describe("clear", () => {
    test("clears all elements", () => {
      const tree = new AVLTree<number>();
      [1, 2, 3, 4, 5].forEach((v) => tree.insert(v));

      tree.clear();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
      expect(tree.height()).toBe(0);
    });
  });

  describe("height bound", () => {
    test("height is O(log n)", () => {
      const tree = new AVLTree<number>();
      const n = 1000;
      for (let i = 0; i < n; i++) {
        tree.insert(i);
      }
      // AVL height <= 1.44 * log2(n+2)
      const maxHeight = Math.ceil(1.44 * Math.log2(n + 2));
      expect(tree.height()).toBeLessThanOrEqual(maxHeight);
    });
  });
});

describe("AVLTreeMap", () => {
  describe("basic operations", () => {
    test("set and get", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("a", 1);
      map.set("b", 2);
      map.set("c", 3);

      expect(map.get("a")).toBe(1);
      expect(map.get("b")).toBe(2);
      expect(map.get("c")).toBe(3);
    });

    test("get returns null for missing key", () => {
      const map = new AVLTreeMap<string, number>();
      expect(map.get("missing")).toBeNull();
    });

    test("set overwrites existing value", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("key", 1);
      map.set("key", 2);

      expect(map.get("key")).toBe(2);
      expect(map.size).toBe(1);
    });

    test("has checks existence", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("exists", 42);

      expect(map.has("exists")).toBe(true);
      expect(map.has("missing")).toBe(false);
    });

    test("delete removes key", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("a", 1);
      map.set("b", 2);

      expect(map.delete("a")).toBe(true);
      expect(map.has("a")).toBe(false);
      expect(map.size).toBe(1);
    });

    test("delete returns false for missing key", () => {
      const map = new AVLTreeMap<string, number>();
      expect(map.delete("missing")).toBe(false);
    });
  });

  describe("keys, values, entries", () => {
    test("keys returns sorted keys", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("c", 3);
      map.set("a", 1);
      map.set("b", 2);

      expect(map.keys()).toEqual(["a", "b", "c"]);
    });

    test("values returns values in key order", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("c", 3);
      map.set("a", 1);
      map.set("b", 2);

      expect(map.values()).toEqual([1, 2, 3]);
    });

    test("entries returns sorted key-value pairs", () => {
      const map = new AVLTreeMap<string, number>();
      map.set("c", 3);
      map.set("a", 1);
      map.set("b", 2);

      expect(map.entries()).toEqual([
        ["a", 1],
        ["b", 2],
        ["c", 3],
      ]);
    });

    test("empty map returns empty collections", () => {
      const map = new AVLTreeMap<string, number>();
      expect(map.keys()).toEqual([]);
      expect(map.values()).toEqual([]);
      expect(map.entries()).toEqual([]);
    });
  });

  describe("size", () => {
    test("size tracks correctly", () => {
      const map = new AVLTreeMap<number, string>();
      expect(map.size).toBe(0);

      map.set(1, "one");
      expect(map.size).toBe(1);

      map.set(2, "two");
      expect(map.size).toBe(2);

      map.set(1, "ONE"); // Update, not new
      expect(map.size).toBe(2);

      map.delete(1);
      expect(map.size).toBe(1);
    });
  });

  describe("forEach", () => {
    test("iterates over all entries in sorted order", () => {
      const map = new AVLTreeMap<number, string>();
      map.set(3, "three");
      map.set(1, "one");
      map.set(2, "two");

      const collected: [string, number][] = [];
      map.forEach((value, key) => {
        collected.push([value, key]);
      });

      expect(collected).toEqual([
        ["one", 1],
        ["two", 2],
        ["three", 3],
      ]);
    });
  });

  describe("numeric keys", () => {
    test("works with numeric keys", () => {
      const map = new AVLTreeMap<number, string>();
      map.set(100, "hundred");
      map.set(50, "fifty");
      map.set(200, "two hundred");

      expect(map.keys()).toEqual([50, 100, 200]);
      expect(map.get(100)).toBe("hundred");
    });
  });
});
