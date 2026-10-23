import { describe, test, expect } from "bun:test";
import {
  BinarySearchTree,
  TreeNode,
} from "../../src/data-structures/binary-search-tree";

describe("TreeNode", () => {
  test("should create a node with value", () => {
    const node = new TreeNode(10);
    expect(node.value).toBe(10);
    expect(node.left).toBeNull();
    expect(node.right).toBeNull();
    expect(node.parent).toBeNull();
  });

  test("isLeaf returns true for leaf node", () => {
    const node = new TreeNode(5);
    expect(node.isLeaf).toBe(true);
  });

  test("isLeaf returns false when node has children", () => {
    const node = new TreeNode(5);
    node.left = new TreeNode(3);
    expect(node.isLeaf).toBe(false);
  });

  test("isLeftChild and isRightChild", () => {
    const parent = new TreeNode(10);
    const leftChild = new TreeNode(5);
    const rightChild = new TreeNode(15);
    parent.left = leftChild;
    leftChild.parent = parent;
    parent.right = rightChild;
    rightChild.parent = parent;

    expect(leftChild.isLeftChild).toBe(true);
    expect(leftChild.isRightChild).toBe(false);
    expect(rightChild.isLeftChild).toBe(false);
    expect(rightChild.isRightChild).toBe(true);
    expect(parent.isLeftChild).toBe(false);
    expect(parent.isRightChild).toBe(false);
  });

  test("hasLeftChild and hasRightChild", () => {
    const node = new TreeNode(5);
    expect(node.hasLeftChild).toBe(false);
    expect(node.hasRightChild).toBe(false);

    node.left = new TreeNode(3);
    expect(node.hasLeftChild).toBe(true);
    expect(node.hasRightChild).toBe(false);

    node.right = new TreeNode(7);
    expect(node.hasLeftChild).toBe(true);
    expect(node.hasRightChild).toBe(true);
  });
});

describe("BinarySearchTree", () => {
  describe("empty tree", () => {
    test("should be empty initially", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.isEmpty()).toBe(true);
      expect(bst.size).toBe(0);
      expect(bst.root).toBeNull();
    });

    test("height of empty tree is -1", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.height).toBe(-1);
    });

    test("min and max return null for empty tree", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.min()).toBeNull();
      expect(bst.max()).toBeNull();
    });

    test("search returns null for empty tree", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.search(5)).toBeNull();
      expect(bst.contains(5)).toBe(false);
    });

    test("delete returns false for empty tree", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.delete(5)).toBe(false);
    });

    test("traversals return empty arrays", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.inOrder()).toEqual([]);
      expect(bst.preOrder()).toEqual([]);
      expect(bst.postOrder()).toEqual([]);
      expect(bst.levelOrder()).toEqual([]);
    });

    test("isValid returns true for empty tree", () => {
      const bst = new BinarySearchTree<number>();
      expect(bst.isValid()).toBe(true);
    });
  });

  describe("single node", () => {
    test("insert single value", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.size).toBe(1);
      expect(bst.root).toBe(10);
      expect(bst.isEmpty()).toBe(false);
    });

    test("height of single node is 0", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.height).toBe(0);
    });

    test("min and max are the same for single node", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.min()).toBe(10);
      expect(bst.max()).toBe(10);
    });

    test("delete single node makes tree empty", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.delete(10)).toBe(true);
      expect(bst.isEmpty()).toBe(true);
      expect(bst.size).toBe(0);
    });
  });

  describe("insert", () => {
    test("inserts multiple values correctly", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(50);
      bst.insert(30);
      bst.insert(70);
      bst.insert(20);
      bst.insert(40);
      bst.insert(60);
      bst.insert(80);

      expect(bst.size).toBe(7);
      expect(bst.inOrder()).toEqual([20, 30, 40, 50, 60, 70, 80]);
    });

    test("returns the newly created node", () => {
      const bst = new BinarySearchTree<number>();
      const node = bst.insert(10);
      expect(node.value).toBe(10);
    });

    test("handles ascending insertion (creates right-skewed tree)", () => {
      const bst = new BinarySearchTree<number>();
      for (let i = 1; i <= 5; i++) {
        bst.insert(i);
      }
      expect(bst.inOrder()).toEqual([1, 2, 3, 4, 5]);
      expect(bst.height).toBe(4); // Skewed
    });

    test("handles descending insertion (creates left-skewed tree)", () => {
      const bst = new BinarySearchTree<number>();
      for (let i = 5; i >= 1; i--) {
        bst.insert(i);
      }
      expect(bst.inOrder()).toEqual([1, 2, 3, 4, 5]);
      expect(bst.height).toBe(4); // Skewed
    });

    test("duplicate key returns the existing node", () => {
      const bst = new BinarySearchTree<number>();
      const first = bst.insert(10);
      bst.insert(5);
      bst.insert(15);
      const again = bst.insert(10);
      const third = bst.insert(10);

      expect(again).toBe(first);
      expect(third).toBe(first);
      expect(bst.size).toBe(3);
      expect(bst.isValid()).toBe(true);
      expect(bst.inOrder()).toEqual([5, 10, 15]);
      expect(bst.preOrder()).toEqual([10, 5, 15]);
    });
  });

  describe("search", () => {
    test("finds existing values", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40].forEach((v) => bst.insert(v));

      expect(bst.search(50)).not.toBeNull();
      expect(bst.search(30)).not.toBeNull();
      expect(bst.search(20)).not.toBeNull();
      expect(bst.contains(70)).toBe(true);
      expect(bst.contains(40)).toBe(true);
    });

    test("returns null for non-existing values", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70].forEach((v) => bst.insert(v));

      expect(bst.search(10)).toBeNull();
      expect(bst.search(100)).toBeNull();
      expect(bst.contains(42)).toBe(false);
    });
  });

  describe("delete", () => {
    function buildTree(): BinarySearchTree<number> {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));
      return bst;
    }

    test("delete leaf node", () => {
      const bst = buildTree();
      expect(bst.delete(20)).toBe(true);
      expect(bst.size).toBe(6);
      expect(bst.contains(20)).toBe(false);
      expect(bst.inOrder()).toEqual([30, 40, 50, 60, 70, 80]);
    });

    test("delete node with one child (left)", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(50);
      bst.insert(30);
      bst.insert(20);

      expect(bst.delete(30)).toBe(true);
      expect(bst.size).toBe(2);
      expect(bst.inOrder()).toEqual([20, 50]);
    });

    test("delete node with one child (right)", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(50);
      bst.insert(30);
      bst.insert(40);

      expect(bst.delete(30)).toBe(true);
      expect(bst.size).toBe(2);
      expect(bst.inOrder()).toEqual([40, 50]);
    });

    test("delete node with two children", () => {
      const bst = buildTree();
      expect(bst.delete(30)).toBe(true);
      expect(bst.size).toBe(6);
      expect(bst.inOrder()).toEqual([20, 40, 50, 60, 70, 80]);
      expect(bst.isValid()).toBe(true);
    });

    test("delete root node", () => {
      const bst = buildTree();
      expect(bst.delete(50)).toBe(true);
      expect(bst.size).toBe(6);
      expect(bst.inOrder()).toEqual([20, 30, 40, 60, 70, 80]);
      expect(bst.isValid()).toBe(true);
    });

    test("delete non-existing value", () => {
      const bst = buildTree();
      expect(bst.delete(100)).toBe(false);
      expect(bst.size).toBe(7);
    });

    test("delete all nodes one by one", () => {
      const bst = buildTree();
      const values = [50, 30, 70, 20, 40, 60, 80];
      for (const v of values) {
        expect(bst.delete(v)).toBe(true);
        expect(bst.isValid()).toBe(true);
      }
      expect(bst.isEmpty()).toBe(true);
    });
  });

  describe("min and max", () => {
    test("finds minimum and maximum", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.min()).toBe(20);
      expect(bst.max()).toBe(80);
    });

    test("after deleting min/max, new min/max is correct", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      bst.delete(20);
      expect(bst.min()).toBe(30);

      bst.delete(80);
      expect(bst.max()).toBe(70);
    });
  });

  describe("successor and predecessor", () => {
    test("successor returns next larger value", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.successor(20)).toBe(30);
      expect(bst.successor(30)).toBe(40);
      expect(bst.successor(40)).toBe(50);
      expect(bst.successor(50)).toBe(60);
      expect(bst.successor(60)).toBe(70);
      expect(bst.successor(70)).toBe(80);
      expect(bst.successor(80)).toBeNull(); // No successor for max
    });

    test("predecessor returns next smaller value", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.predecessor(80)).toBe(70);
      expect(bst.predecessor(70)).toBe(60);
      expect(bst.predecessor(60)).toBe(50);
      expect(bst.predecessor(50)).toBe(40);
      expect(bst.predecessor(40)).toBe(30);
      expect(bst.predecessor(30)).toBe(20);
      expect(bst.predecessor(20)).toBeNull(); // No predecessor for min
    });

    test("successor and predecessor return null for non-existing", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.successor(99)).toBeNull();
      expect(bst.predecessor(99)).toBeNull();
    });
  });

  describe("traversals", () => {
    test("inOrder returns sorted values", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.inOrder()).toEqual([20, 30, 40, 50, 60, 70, 80]);
    });

    test("preOrder returns root-left-right", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.preOrder()).toEqual([50, 30, 20, 40, 70, 60, 80]);
    });

    test("postOrder returns left-right-root", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.postOrder()).toEqual([20, 40, 30, 60, 80, 70, 50]);
    });

    test("levelOrder returns BFS order", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.levelOrder()).toEqual([50, 30, 70, 20, 40, 60, 80]);
    });
  });

  describe("isValid", () => {
    test("valid BST returns true", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));
      expect(bst.isValid()).toBe(true);
    });

    test("remains valid after insertions and deletions", () => {
      const bst = new BinarySearchTree<number>();
      const values = [50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93, 3, 9, 15, 21, 28];
      for (const v of values) {
        bst.insert(v);
      }
      expect(bst.isValid()).toBe(true);

      for (const val of bst.inOrder().slice(0, 5)) {
        bst.delete(val);
        expect(bst.isValid()).toBe(true);
      }
    });
  });

  describe("floor and ceiling", () => {
    test("floor returns largest value <= target", () => {
      const bst = new BinarySearchTree<number>();
      [20, 30, 40, 50, 60, 70, 80].forEach((v) => bst.insert(v));

      expect(bst.floor(50)).toBe(50);
      expect(bst.floor(55)).toBe(50);
      expect(bst.floor(25)).toBe(20);
      expect(bst.floor(10)).toBeNull(); // No floor
      expect(bst.floor(80)).toBe(80);
      expect(bst.floor(100)).toBe(80);
    });

    test("ceiling returns smallest value >= target", () => {
      const bst = new BinarySearchTree<number>();
      [20, 30, 40, 50, 60, 70, 80].forEach((v) => bst.insert(v));

      expect(bst.ceiling(50)).toBe(50);
      expect(bst.ceiling(55)).toBe(60);
      expect(bst.ceiling(25)).toBe(30);
      expect(bst.ceiling(90)).toBeNull(); // No ceiling
      expect(bst.ceiling(20)).toBe(20);
      expect(bst.ceiling(5)).toBe(20);
    });
  });

  describe("rangeQuery", () => {
    test("returns values in range [low, high]", () => {
      const bst = new BinarySearchTree<number>();
      [20, 30, 40, 50, 60, 70, 80].forEach((v) => bst.insert(v));

      expect(bst.rangeQuery(30, 60)).toEqual([30, 40, 50, 60]);
      expect(bst.rangeQuery(25, 55)).toEqual([30, 40, 50]);
      expect(bst.rangeQuery(100, 200)).toEqual([]);
      expect(bst.rangeQuery(20, 80)).toEqual([20, 30, 40, 50, 60, 70, 80]);
    });

    test("single element range", () => {
      const bst = new BinarySearchTree<number>();
      [10, 20, 30].forEach((v) => bst.insert(v));
      expect(bst.rangeQuery(20, 20)).toEqual([20]);
    });
  });

  describe("kthSmallest", () => {
    test("returns kth smallest element", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.kthSmallest(1)).toBe(20);
      expect(bst.kthSmallest(2)).toBe(30);
      expect(bst.kthSmallest(3)).toBe(40);
      expect(bst.kthSmallest(4)).toBe(50);
      expect(bst.kthSmallest(7)).toBe(80);
    });

    test("returns null for invalid k", () => {
      const bst = new BinarySearchTree<number>();
      bst.insert(10);
      expect(bst.kthSmallest(0)).toBeNull();
      expect(bst.kthSmallest(2)).toBeNull();
      expect(bst.kthSmallest(-1)).toBeNull();
    });
  });

  describe("fromSortedArray", () => {
    test("creates balanced BST from sorted array", () => {
      const bst = BinarySearchTree.fromSortedArray([10, 20, 30, 40, 50, 60, 70]);

      expect(bst.size).toBe(7);
      expect(bst.isValid()).toBe(true);
      expect(bst.inOrder()).toEqual([10, 20, 30, 40, 50, 60, 70]);
      // Balanced tree of 7 nodes should have height 2
      expect(bst.height).toBe(2);
    });

    test("creates balanced BST from empty array", () => {
      const bst = BinarySearchTree.fromSortedArray<number>([]);
      expect(bst.isEmpty()).toBe(true);
      expect(bst.size).toBe(0);
    });

    test("creates balanced BST from single element", () => {
      const bst = BinarySearchTree.fromSortedArray([42]);
      expect(bst.size).toBe(1);
      expect(bst.root).toBe(42);
    });

    test("keeps repeated values once", () => {
      const bst = BinarySearchTree.fromSortedArray([1, 1, 2, 3, 3, 3]);
      expect(bst.size).toBe(3);
      expect(bst.inOrder()).toEqual([1, 2, 3]);
      expect(bst.isValid()).toBe(true);
    });
  });

  describe("lowestCommonAncestor", () => {
    test("finds LCA of two values", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40, 60, 80].forEach((v) => bst.insert(v));

      expect(bst.lowestCommonAncestor(20, 40)).toBe(30);
      expect(bst.lowestCommonAncestor(20, 70)).toBe(50);
      expect(bst.lowestCommonAncestor(60, 80)).toBe(70);
      expect(bst.lowestCommonAncestor(20, 30)).toBe(30);
    });

    test("returns null when value does not exist", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70].forEach((v) => bst.insert(v));

      expect(bst.lowestCommonAncestor(30, 100)).toBeNull();
      expect(bst.lowestCommonAncestor(100, 200)).toBeNull();
    });
  });

  describe("toArray", () => {
    test("returns sorted array of values", () => {
      const bst = new BinarySearchTree<number>();
      [50, 30, 70, 20, 40].forEach((v) => bst.insert(v));

      expect(bst.toArray()).toEqual([20, 30, 40, 50, 70]);
    });
  });

  describe("clear", () => {
    test("clears all nodes", () => {
      const bst = new BinarySearchTree<number>();
      [10, 20, 30].forEach((v) => bst.insert(v));

      bst.clear();
      expect(bst.isEmpty()).toBe(true);
      expect(bst.size).toBe(0);
      expect(bst.root).toBeNull();
    });
  });

  describe("string type", () => {
    test("works with strings", () => {
      const bst = new BinarySearchTree<string>();
      bst.insert("banana");
      bst.insert("apple");
      bst.insert("cherry");

      expect(bst.inOrder()).toEqual(["apple", "banana", "cherry"]);
      expect(bst.contains("banana")).toBe(true);
      expect(bst.min()).toBe("apple");
      expect(bst.max()).toBe("cherry");
    });
  });

  describe("custom comparator", () => {
    test("works with custom comparator (reverse order)", () => {
      const bst = new BinarySearchTree<number>((a, b) => b - a);
      [10, 20, 30, 40, 50].forEach((v) => bst.insert(v));

      // Reverse order: in-order should be descending
      expect(bst.inOrder()).toEqual([50, 40, 30, 20, 10]);
      expect(bst.min()).toBe(50); // In reverse comparator, "min" is largest number
      expect(bst.max()).toBe(10);
    });
  });
});
