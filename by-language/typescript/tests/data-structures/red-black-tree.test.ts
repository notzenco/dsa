import { describe, test, expect } from "bun:test";
import {
  RedBlackTree,
  RBNode,
  Color,
} from "../../src/data-structures/red-black-tree";

describe("Color enum", () => {
  test("RED is 0 and BLACK is 1", () => {
    expect(Color.RED).toBe(0);
    expect(Color.BLACK).toBe(1);
  });
});

describe("RBNode", () => {
  test("creates node with correct defaults", () => {
    const node = new RBNode(10);
    expect(node.key).toBe(10);
    expect(node.color).toBe(Color.RED);
    expect(node.parent).toBeNull();
    expect(node.left).toBeNull();
    expect(node.right).toBeNull();
  });

  test("creates node with specified color", () => {
    const node = new RBNode(10, Color.BLACK);
    expect(node.color).toBe(Color.BLACK);
  });
});

describe("RedBlackTree", () => {
  describe("empty tree", () => {
    test("should be empty initially", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
    });

    test("search returns null for empty tree", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.search(5)).toBeNull();
      expect(tree.contains(5)).toBe(false);
    });

    test("min and max return null for empty tree", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.min()).toBeNull();
      expect(tree.max()).toBeNull();
    });

    test("delete returns false for empty tree", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.delete(5)).toBe(false);
    });

    test("inOrder returns empty array", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.inOrder()).toEqual([]);
    });

    test("isValid returns true for empty tree", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.isValid()).toBe(true);
    });

    test("height is 0 for empty tree", () => {
      const tree = new RedBlackTree<number>();
      expect(tree.height()).toBe(0);
    });
  });

  describe("single element", () => {
    test("insert single element", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(10);

      expect(tree.size).toBe(1);
      expect(tree.isEmpty()).toBe(false);
      expect(tree.search(10)).toBe(10);
      expect(tree.contains(10)).toBe(true);
    });

    test("root is always black", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(10);
      expect(tree.isValid()).toBe(true);
    });

    test("delete single element", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(10);
      expect(tree.delete(10)).toBe(true);
      expect(tree.isEmpty()).toBe(true);
      expect(tree.isValid()).toBe(true);
    });
  });

  describe("insert and RB properties", () => {
    test("root remains black after multiple inserts", () => {
      const tree = new RedBlackTree<number>();
      [10, 20, 30, 40, 50].forEach((v) => tree.insert(v));
      expect(tree.isValid()).toBe(true);
    });

    test("ascending insertion maintains RB properties", () => {
      const tree = new RedBlackTree<number>();
      for (let i = 1; i <= 20; i++) {
        tree.insert(i);
        expect(tree.isValid()).toBe(true);
      }
      expect(tree.size).toBe(20);
      expect(tree.inOrder()).toEqual(
        Array.from({ length: 20 }, (_, i) => i + 1),
      );
    });

    test("descending insertion maintains RB properties", () => {
      const tree = new RedBlackTree<number>();
      for (let i = 20; i >= 1; i--) {
        tree.insert(i);
        expect(tree.isValid()).toBe(true);
      }
      expect(tree.size).toBe(20);
    });

    test("random insertion maintains RB properties", () => {
      const tree = new RedBlackTree<number>();
      const values = [15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9];
      for (const v of values) {
        tree.insert(v);
        expect(tree.isValid()).toBe(true);
      }
    });

    test("duplicate key updates value", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(10);
      tree.insert(10);
      expect(tree.size).toBe(1);
      expect(tree.isValid()).toBe(true);
    });

    test("case 1: uncle is red (recoloring)", () => {
      const tree = new RedBlackTree<number>();
      // Insert order that triggers uncle-is-red case
      tree.insert(10);
      tree.insert(5);
      tree.insert(15);
      tree.insert(3); // This should trigger recoloring
      expect(tree.isValid()).toBe(true);
    });

    test("case 2 and 3: uncle is black, rotations needed", () => {
      const tree = new RedBlackTree<number>();
      // This sequence triggers rotation cases
      tree.insert(10);
      tree.insert(20);
      tree.insert(30); // Triggers left rotation
      expect(tree.isValid()).toBe(true);
      expect(tree.inOrder()).toEqual([10, 20, 30]);
    });

    test("mirror case rotations", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(30);
      tree.insert(20);
      tree.insert(10); // Triggers right rotation
      expect(tree.isValid()).toBe(true);
      expect(tree.inOrder()).toEqual([10, 20, 30]);
    });
  });

  describe("search", () => {
    test("finds existing values", () => {
      const tree = new RedBlackTree<number>();
      [50, 30, 70, 20, 40].forEach((v) => tree.insert(v));

      expect(tree.search(50)).toBe(50);
      expect(tree.search(30)).toBe(30);
      expect(tree.contains(70)).toBe(true);
    });

    test("returns null for non-existing values", () => {
      const tree = new RedBlackTree<number>();
      [50, 30, 70].forEach((v) => tree.insert(v));

      expect(tree.search(10)).toBeNull();
      expect(tree.contains(100)).toBe(false);
    });
  });

  describe("delete", () => {
    test("delete leaf node maintains properties", () => {
      const tree = new RedBlackTree<number>();
      [10, 5, 15, 3, 7].forEach((v) => tree.insert(v));

      expect(tree.delete(3)).toBe(true);
      expect(tree.isValid()).toBe(true);
      expect(tree.size).toBe(4);
      expect(tree.contains(3)).toBe(false);
    });

    test("delete node with one child", () => {
      const tree = new RedBlackTree<number>();
      [10, 5, 15, 3].forEach((v) => tree.insert(v));

      expect(tree.delete(5)).toBe(true);
      expect(tree.isValid()).toBe(true);
    });

    test("delete node with two children", () => {
      const tree = new RedBlackTree<number>();
      [10, 5, 15, 3, 7, 12, 18].forEach((v) => tree.insert(v));

      expect(tree.delete(10)).toBe(true);
      expect(tree.isValid()).toBe(true);
      expect(tree.contains(10)).toBe(false);
    });

    test("delete root", () => {
      const tree = new RedBlackTree<number>();
      [10, 5, 15].forEach((v) => tree.insert(v));

      expect(tree.delete(10)).toBe(true);
      expect(tree.isValid()).toBe(true);
    });

    test("delete non-existing value", () => {
      const tree = new RedBlackTree<number>();
      tree.insert(10);
      expect(tree.delete(20)).toBe(false);
      expect(tree.size).toBe(1);
    });

    test("delete all nodes one by one", () => {
      const tree = new RedBlackTree<number>();
      const values = [10, 5, 15, 3, 7, 12, 18];
      values.forEach((v) => tree.insert(v));

      for (const v of values) {
        expect(tree.delete(v)).toBe(true);
        expect(tree.isValid()).toBe(true);
      }
      expect(tree.isEmpty()).toBe(true);
    });

    test("delete in ascending order maintains properties", () => {
      const tree = new RedBlackTree<number>();
      for (let i = 1; i <= 15; i++) {
        tree.insert(i);
      }

      for (let i = 1; i <= 15; i++) {
        expect(tree.delete(i)).toBe(true);
        expect(tree.isValid()).toBe(true);
      }
    });

    test("delete in descending order maintains properties", () => {
      const tree = new RedBlackTree<number>();
      for (let i = 1; i <= 15; i++) {
        tree.insert(i);
      }

      for (let i = 15; i >= 1; i--) {
        expect(tree.delete(i)).toBe(true);
        expect(tree.isValid()).toBe(true);
      }
    });

    test("alternating insert and delete maintains properties", () => {
      const tree = new RedBlackTree<number>();

      for (let i = 0; i < 20; i++) {
        tree.insert(i);
      }

      // Delete even numbers
      for (let i = 0; i < 20; i += 2) {
        expect(tree.delete(i)).toBe(true);
        expect(tree.isValid()).toBe(true);
      }

      // Verify odd numbers remain
      for (let i = 1; i < 20; i += 2) {
        expect(tree.contains(i)).toBe(true);
      }
    });

    test("delete fixup case 1: sibling is red", () => {
      const tree = new RedBlackTree<number>();
      // Build tree that will trigger case 1 on deletion
      [13, 8, 17, 1, 11, 15, 25, 6, 22, 27].forEach((v) => tree.insert(v));
      tree.delete(1);
      expect(tree.isValid()).toBe(true);
    });

    test("large sequence of inserts and deletes", () => {
      const tree = new RedBlackTree<number>();
      const n = 100;

      // Insert n values
      for (let i = 0; i < n; i++) {
        tree.insert(i);
      }
      expect(tree.size).toBe(n);
      expect(tree.isValid()).toBe(true);

      // Delete half
      for (let i = 0; i < n; i += 2) {
        tree.delete(i);
      }
      expect(tree.size).toBe(n / 2);
      expect(tree.isValid()).toBe(true);

      // Reinsert some
      for (let i = 0; i < 20; i += 2) {
        tree.insert(i);
      }
      expect(tree.isValid()).toBe(true);
    });
  });

  describe("min and max", () => {
    test("min and max are correct", () => {
      const tree = new RedBlackTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => tree.insert(v));

      expect(tree.min()).toBe(20);
      expect(tree.max()).toBe(80);
    });

    test("min and max after deletions", () => {
      const tree = new RedBlackTree<number>();
      [10, 20, 30, 40, 50].forEach((v) => tree.insert(v));

      tree.delete(10);
      expect(tree.min()).toBe(20);

      tree.delete(50);
      expect(tree.max()).toBe(40);
    });
  });

  describe("inOrder traversal", () => {
    test("returns sorted values", () => {
      const tree = new RedBlackTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => tree.insert(v));

      expect(tree.inOrder()).toEqual([20, 30, 40, 50, 60, 70, 80]);
    });

    test("returns correct order after deletions", () => {
      const tree = new RedBlackTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => tree.insert(v));

      tree.delete(30);
      tree.delete(70);

      expect(tree.inOrder()).toEqual([20, 40, 50, 60, 80]);
    });
  });

  describe("height and black height", () => {
    test("height is bounded by 2*log(n+1)", () => {
      const tree = new RedBlackTree<number>();
      const n = 100;
      for (let i = 0; i < n; i++) {
        tree.insert(i);
      }
      // Red-black tree height <= 2 * log2(n+1)
      const maxHeight = Math.ceil(2 * Math.log2(n + 1));
      expect(tree.height()).toBeLessThanOrEqual(maxHeight);
    });

    test("black height is consistent", () => {
      const tree = new RedBlackTree<number>();
      for (let i = 1; i <= 31; i++) {
        tree.insert(i);
      }
      expect(tree.blackHeight()).toBeGreaterThan(0);
      expect(tree.isValid()).toBe(true);
    });
  });

  describe("isValid", () => {
    test("validates all red-black properties", () => {
      const tree = new RedBlackTree<number>();
      const values = [7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13];
      for (const v of values) {
        tree.insert(v);
        expect(tree.isValid()).toBe(true);
      }
    });
  });

  describe("clear", () => {
    test("removes all nodes", () => {
      const tree = new RedBlackTree<number>();
      [10, 20, 30].forEach((v) => tree.insert(v));

      tree.clear();
      expect(tree.isEmpty()).toBe(true);
      expect(tree.size).toBe(0);
      expect(tree.isValid()).toBe(true);
    });
  });

  describe("string keys", () => {
    test("works with string keys", () => {
      const tree = new RedBlackTree<string>();
      tree.insert("banana");
      tree.insert("apple");
      tree.insert("cherry");

      expect(tree.inOrder()).toEqual(["apple", "banana", "cherry"]);
      expect(tree.contains("banana")).toBe(true);
      expect(tree.min()).toBe("apple");
      expect(tree.max()).toBe("cherry");
      expect(tree.isValid()).toBe(true);
    });
  });

  describe("stress test", () => {
    test("maintains properties with many operations", () => {
      const tree = new RedBlackTree<number>();
      const inserted: Set<number> = new Set();

      // Insert 200 unique values
      for (let i = 0; i < 200; i++) {
        const val = Math.floor(Math.random() * 1000);
        tree.insert(val);
        inserted.add(val);
      }
      expect(tree.isValid()).toBe(true);
      expect(tree.size).toBe(inserted.size);

      // Delete half
      const toDelete = [...inserted].slice(0, Math.floor(inserted.size / 2));
      for (const val of toDelete) {
        tree.delete(val);
      }
      expect(tree.isValid()).toBe(true);

      // Verify remaining
      const remaining = [...inserted].slice(Math.floor(inserted.size / 2));
      for (const val of remaining) {
        expect(tree.contains(val)).toBe(true);
      }
    });
  });
});
