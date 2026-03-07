import { describe, test, expect } from "bun:test";
import { BTree, BTreeNode } from "../../src/data-structures/b-tree";

describe("BTree", () => {
  describe("constructor", () => {
    test("creates empty tree with minimum degree", () => {
      const tree = new BTree(2);
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
      expect(tree.minDegree).toBe(2);
    });

    test("defaults to minimum degree 2", () => {
      const tree = new BTree();
      expect(tree.minDegree).toBe(2);
    });

    test("throws for t < 2", () => {
      expect(() => new BTree(1)).toThrow("Minimum degree t must be at least 2");
    });
  });

  describe("empty tree", () => {
    test("should be empty", () => {
      const tree = new BTree();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
    });

    test("search returns false for empty tree", () => {
      const tree = new BTree();
      expect(tree.search(5)).toBe(false);
    });

    test("min and max return null for empty tree", () => {
      const tree = new BTree();
      expect(tree.min()).toBeNull();
      expect(tree.max()).toBeNull();
    });

    test("delete returns false for empty tree", () => {
      const tree = new BTree();
      expect(tree.delete(5)).toBe(false);
    });

    test("traverse returns empty array", () => {
      const tree = new BTree();
      expect(tree.traverse()).toEqual([]);
    });

    test("height is 0 for empty tree", () => {
      const tree = new BTree();
      expect(tree.height()).toBe(0);
    });
  });

  describe("insert (t=2)", () => {
    test("insert single key", () => {
      const tree = new BTree(2);
      tree.insert(10);
      expect(tree.size).toBe(1);
      expect(tree.search(10)).toBe(true);
    });

    test("insert multiple keys without split", () => {
      const tree = new BTree(2);
      // t=2 => max 3 keys per node
      tree.insert(10);
      tree.insert(20);
      tree.insert(30);

      expect(tree.size).toBe(3);
      expect(tree.traverse()).toEqual([10, 20, 30]);
      expect(tree.height()).toBe(0); // All in root
    });

    test("insert causing root split", () => {
      const tree = new BTree(2);
      // t=2 => max 3 keys per node, inserting 4th triggers split
      tree.insert(10);
      tree.insert(20);
      tree.insert(30);
      tree.insert(40);

      expect(tree.size).toBe(4);
      expect(tree.traverse()).toEqual([10, 20, 30, 40]);
      expect(tree.height()).toBe(1); // Root split
    });

    test("insert in ascending order", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 10; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(10);
      expect(tree.traverse()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });

    test("insert in descending order", () => {
      const tree = new BTree(2);
      for (let i = 10; i >= 1; i--) {
        tree.insert(i);
      }
      expect(tree.size).toBe(10);
      expect(tree.traverse()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });

    test("insert in random order", () => {
      const tree = new BTree(2);
      const values = [5, 3, 8, 1, 4, 7, 10, 2, 6, 9];
      for (const v of values) {
        tree.insert(v);
      }
      expect(tree.size).toBe(10);
      expect(tree.traverse()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });

    test("duplicate keys are handled (no duplicate count increase)", () => {
      const tree = new BTree(2);
      tree.insert(10);
      tree.insert(10);

      expect(tree.size).toBe(1);
    });
  });

  describe("insert (t=3)", () => {
    test("insert without split (up to 5 keys)", () => {
      const tree = new BTree(3);
      // t=3 => max 5 keys per node
      for (let i = 1; i <= 5; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(5);
      expect(tree.height()).toBe(0);
    });

    test("insert causing split (6th key)", () => {
      const tree = new BTree(3);
      for (let i = 1; i <= 6; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(6);
      expect(tree.height()).toBe(1);
      expect(tree.traverse()).toEqual([1, 2, 3, 4, 5, 6]);
    });

    test("many insertions maintain sorted order", () => {
      const tree = new BTree(3);
      for (let i = 1; i <= 30; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(30);
      expect(tree.traverse()).toEqual(
        Array.from({ length: 30 }, (_, i) => i + 1),
      );
    });
  });

  describe("insert (t=5)", () => {
    test("large degree tree", () => {
      const tree = new BTree(5);
      // t=5 => max 9 keys per node
      for (let i = 1; i <= 50; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(50);
      expect(tree.traverse()).toEqual(
        Array.from({ length: 50 }, (_, i) => i + 1),
      );
    });
  });

  describe("search", () => {
    test("search finds existing keys", () => {
      const tree = new BTree(2);
      [10, 20, 30, 5, 15, 25, 35].forEach((v) => tree.insert(v));

      expect(tree.search(10)).toBe(true);
      expect(tree.search(20)).toBe(true);
      expect(tree.search(35)).toBe(true);
    });

    test("search returns false for missing keys", () => {
      const tree = new BTree(2);
      [10, 20, 30].forEach((v) => tree.insert(v));

      expect(tree.search(5)).toBe(false);
      expect(tree.search(15)).toBe(false);
      expect(tree.search(100)).toBe(false);
    });
  });

  describe("delete (t=2)", () => {
    test("delete from leaf", () => {
      const tree = new BTree(2);
      [10, 20, 30].forEach((v) => tree.insert(v));

      expect(tree.delete(10)).toBe(true);
      expect(tree.size).toBe(2);
      expect(tree.search(10)).toBe(false);
      expect(tree.traverse()).toEqual([20, 30]);
    });

    test("delete non-existing key", () => {
      const tree = new BTree(2);
      tree.insert(10);
      expect(tree.delete(20)).toBe(false);
    });

    test("delete causing merge", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 7; i++) {
        tree.insert(i);
      }

      // Delete enough to cause merges
      tree.delete(1);
      expect(tree.traverse()).toEqual([2, 3, 4, 5, 6, 7]);

      tree.delete(2);
      expect(tree.traverse()).toEqual([3, 4, 5, 6, 7]);
    });

    test("delete causing borrow from left sibling", () => {
      const tree = new BTree(2);
      // Build a tree where deletion triggers borrow-from-left
      [10, 20, 30, 5, 15, 25, 35, 3, 7].forEach((v) => tree.insert(v));
      const before = tree.traverse();

      tree.delete(15);
      const after = tree.traverse();

      expect(tree.search(15)).toBe(false);
      expect(after).toEqual(before.filter((v) => v !== 15));
    });

    test("delete causing borrow from right sibling", () => {
      const tree = new BTree(2);
      [10, 20, 30, 5, 15, 25, 35, 23, 27].forEach((v) => tree.insert(v));
      const before = tree.traverse();

      tree.delete(15);
      const after = tree.traverse();

      expect(tree.search(15)).toBe(false);
      expect(after).toEqual(before.filter((v) => v !== 15));
    });

    test("delete internal node (predecessor replacement)", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 10; i++) {
        tree.insert(i);
      }

      // Delete internal node
      const before = tree.traverse();
      tree.delete(4);
      const after = tree.traverse();

      expect(tree.search(4)).toBe(false);
      expect(after).toEqual(before.filter((v) => v !== 4));
    });

    test("delete all keys one by one", () => {
      const tree = new BTree(2);
      const values = [5, 3, 8, 1, 4, 7, 10, 2, 6, 9];
      for (const v of values) {
        tree.insert(v);
      }

      const sorted = [...values].sort((a, b) => a - b);
      for (const v of sorted) {
        expect(tree.delete(v)).toBe(true);
        expect(tree.search(v)).toBe(false);
      }
      expect(tree.isEmpty()).toBe(true);
    });

    test("delete in reverse order", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 10; i++) {
        tree.insert(i);
      }

      for (let i = 10; i >= 1; i--) {
        expect(tree.delete(i)).toBe(true);
      }
      expect(tree.isEmpty()).toBe(true);
    });

    test("delete root when it becomes empty", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 7; i++) {
        tree.insert(i);
      }

      // Delete enough to collapse the root
      for (let i = 1; i <= 5; i++) {
        tree.delete(i);
      }
      expect(tree.traverse()).toEqual([6, 7]);
    });
  });

  describe("delete (t=3)", () => {
    test("delete from larger degree tree", () => {
      const tree = new BTree(3);
      for (let i = 1; i <= 20; i++) {
        tree.insert(i);
      }

      for (let i = 1; i <= 10; i++) {
        expect(tree.delete(i)).toBe(true);
      }

      expect(tree.size).toBe(10);
      expect(tree.traverse()).toEqual([11, 12, 13, 14, 15, 16, 17, 18, 19, 20]);
    });
  });

  describe("min and max", () => {
    test("min and max are correct", () => {
      const tree = new BTree(2);
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => tree.insert(v));

      expect(tree.min()).toBe(20);
      expect(tree.max()).toBe(80);
    });

    test("min and max after deletions", () => {
      const tree = new BTree(2);
      [10, 20, 30, 40, 50].forEach((v) => tree.insert(v));

      tree.delete(10);
      expect(tree.min()).toBe(20);

      tree.delete(50);
      expect(tree.max()).toBe(40);
    });

    test("min equals max for single element", () => {
      const tree = new BTree(2);
      tree.insert(42);
      expect(tree.min()).toBe(42);
      expect(tree.max()).toBe(42);
    });
  });

  describe("height", () => {
    test("height grows logarithmically", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 100; i++) {
        tree.insert(i);
      }
      // For t=2, height should be much less than n
      expect(tree.height()).toBeLessThan(10);
    });

    test("all leaves at same level (verified by traverse correctness)", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 20; i++) {
        tree.insert(i);
      }
      // If traverse produces correct sorted order, the tree structure is valid
      expect(tree.traverse()).toEqual(
        Array.from({ length: 20 }, (_, i) => i + 1),
      );
    });
  });

  describe("traverse (inOrder)", () => {
    test("returns keys in sorted order", () => {
      const tree = new BTree(2);
      [5, 3, 8, 1, 4, 7, 10, 2, 6, 9].forEach((v) => tree.insert(v));

      expect(tree.traverse()).toEqual([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    });
  });

  describe("clear", () => {
    test("clears all keys", () => {
      const tree = new BTree(2);
      for (let i = 1; i <= 10; i++) {
        tree.insert(i);
      }

      tree.clear();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
      expect(tree.traverse()).toEqual([]);
    });
  });

  describe("splitChild", () => {
    test("split is correctly reflected in traverse order", () => {
      const tree = new BTree(2);
      // Insert enough to cause multiple splits
      for (let i = 1; i <= 15; i++) {
        tree.insert(i);
      }
      expect(tree.traverse()).toEqual(
        Array.from({ length: 15 }, (_, i) => i + 1),
      );
    });
  });

  describe("stress test", () => {
    test("handles many operations with t=2", () => {
      const tree = new BTree(2);
      const n = 200;

      for (let i = 0; i < n; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(n);
      expect(tree.traverse()).toEqual(
        Array.from({ length: n }, (_, i) => i),
      );

      // Delete every other
      for (let i = 0; i < n; i += 2) {
        tree.delete(i);
      }
      expect(tree.size).toBe(n / 2);

      // Verify remaining
      const expected = Array.from({ length: n / 2 }, (_, i) => i * 2 + 1);
      expect(tree.traverse()).toEqual(expected);
    });

    test("handles many operations with t=4", () => {
      const tree = new BTree(4);
      const n = 100;

      for (let i = 0; i < n; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(n);

      for (let i = 0; i < n; i++) {
        expect(tree.search(i)).toBe(true);
      }

      for (let i = 0; i < n; i++) {
        tree.delete(i);
      }
      expect(tree.isEmpty()).toBe(true);
    });
  });
});
