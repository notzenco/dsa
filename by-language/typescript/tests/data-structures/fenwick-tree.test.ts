import { describe, test, expect } from "bun:test";
import {
  FenwickTree,
  FenwickTree2D,
  countInversions,
  RangeSumQueryMutable,
} from "../../src/data-structures/fenwick-tree";

// ============================================================================
// FenwickTree (1D)
// ============================================================================
describe("FenwickTree", () => {
  test("rejects invalid updates", () => {
    const tree = FenwickTree.fromArray([1, 2]);
    for (const index of [-2, -1, 2, 0.5]) {
      expect(() => tree.update(index, 1)).toThrow(RangeError);
    }
    expect(tree.prefixSum(1)).toBe(3);
    const tree2d = FenwickTree2D.fromMatrix([[1]]);
    expect(() => tree2d.update(-1, 0, 1)).toThrow(RangeError);
    expect(() => tree2d.update(0, -1, 1)).toThrow(RangeError);
  });
  test("fromArray and prefixSum", () => {
    const ft = FenwickTree.fromArray([3, 2, -1, 6, 5, 4, -3, 3]);
    expect(ft.prefixSum(0)).toBe(3);
    expect(ft.prefixSum(1)).toBe(5);
    expect(ft.prefixSum(4)).toBe(15); // 3+2-1+6+5
    expect(ft.prefixSum(7)).toBe(19); // sum of all
  });

  test("constructor creates zero-initialized tree", () => {
    const ft = new FenwickTree(5);
    expect(ft.prefixSum(0)).toBe(0);
    expect(ft.prefixSum(4)).toBe(0);
  });

  test("rangeSum", () => {
    const ft = FenwickTree.fromArray([3, 2, -1, 6, 5, 4, -3, 3]);
    expect(ft.rangeSum(0, 7)).toBe(19);
    expect(ft.rangeSum(2, 5)).toBe(14); // -1+6+5+4
    expect(ft.rangeSum(0, 0)).toBe(3);
    expect(ft.rangeSum(3, 3)).toBe(6);
  });

  test("pointQuery returns single element value", () => {
    const ft = FenwickTree.fromArray([3, 2, -1, 6, 5]);
    expect(ft.pointQuery(0)).toBe(3);
    expect(ft.pointQuery(1)).toBe(2);
    expect(ft.pointQuery(2)).toBe(-1);
    expect(ft.pointQuery(3)).toBe(6);
    expect(ft.pointQuery(4)).toBe(5);
  });

  test("update adds delta", () => {
    const ft = FenwickTree.fromArray([1, 2, 3, 4, 5]);
    ft.update(2, 10); // index 2: 3 -> 13
    expect(ft.pointQuery(2)).toBe(13);
    expect(ft.prefixSum(4)).toBe(25); // 1+2+13+4+5
    expect(ft.rangeSum(1, 3)).toBe(19); // 2+13+4
  });

  test("multiple updates", () => {
    const ft = FenwickTree.fromArray([0, 0, 0, 0, 0]);
    ft.update(0, 1);
    ft.update(1, 2);
    ft.update(2, 3);
    ft.update(3, 4);
    ft.update(4, 5);
    expect(ft.prefixSum(4)).toBe(15);
    expect(ft.rangeSum(2, 4)).toBe(12);
  });

  test("negative updates", () => {
    const ft = FenwickTree.fromArray([10, 20, 30]);
    ft.update(1, -5); // 20 -> 15
    expect(ft.pointQuery(1)).toBe(15);
    expect(ft.prefixSum(2)).toBe(55);
  });

  test("size property", () => {
    const ft = new FenwickTree(10);
    expect(ft.size).toBe(10);
  });

  test("single element tree", () => {
    const ft = FenwickTree.fromArray([42]);
    expect(ft.prefixSum(0)).toBe(42);
    expect(ft.pointQuery(0)).toBe(42);
    ft.update(0, 8);
    expect(ft.pointQuery(0)).toBe(50);
  });

  test("large array", () => {
    const arr = Array.from({ length: 1000 }, (_, i) => i + 1);
    const ft = FenwickTree.fromArray(arr);
    expect(ft.prefixSum(999)).toBe(500500);
    expect(ft.rangeSum(0, 99)).toBe(5050);
    expect(ft.rangeSum(100, 199)).toBe(15050);
  });

  test("all negative values", () => {
    const ft = FenwickTree.fromArray([-5, -3, -8, -1, -7]);
    expect(ft.prefixSum(4)).toBe(-24);
    expect(ft.rangeSum(1, 3)).toBe(-12);
  });

  test("alternating updates and queries", () => {
    const ft = FenwickTree.fromArray([1, 1, 1, 1, 1]);
    expect(ft.prefixSum(4)).toBe(5);
    ft.update(2, 4); // [1,1,5,1,1]
    expect(ft.rangeSum(1, 3)).toBe(7);
    ft.update(0, -1); // [0,1,5,1,1]
    expect(ft.prefixSum(4)).toBe(8);
    ft.update(4, 10); // [0,1,5,1,11]
    expect(ft.rangeSum(3, 4)).toBe(12);
  });
});

// ============================================================================
// FenwickTree2D
// ============================================================================
describe("FenwickTree2D", () => {
  test("fromMatrix and rangeSum for entire matrix", () => {
    const matrix = [
      [1, 2, 3],
      [4, 5, 6],
      [7, 8, 9],
    ];
    const ft = FenwickTree2D.fromMatrix(matrix);
    expect(ft.rangeSum(0, 0, 2, 2)).toBe(45);
  });

  test("constructor creates zero-initialized 2D tree", () => {
    const ft = new FenwickTree2D(3, 3);
    expect(ft.rangeSum(0, 0, 2, 2)).toBe(0);
  });

  test("prefixSum", () => {
    const matrix = [
      [1, 2],
      [3, 4],
    ];
    const ft = FenwickTree2D.fromMatrix(matrix);
    expect(ft.prefixSum(0, 0)).toBe(1);
    expect(ft.prefixSum(0, 1)).toBe(3); // 1+2
    expect(ft.prefixSum(1, 0)).toBe(4); // 1+3
    expect(ft.prefixSum(1, 1)).toBe(10); // 1+2+3+4
  });

  test("rangeSum submatrix", () => {
    const matrix = [
      [1, 2, 3],
      [4, 5, 6],
      [7, 8, 9],
    ];
    const ft = FenwickTree2D.fromMatrix(matrix);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(1);
    expect(ft.rangeSum(1, 1, 2, 2)).toBe(28); // 5+6+8+9
    expect(ft.rangeSum(0, 0, 1, 1)).toBe(12); // 1+2+4+5
    expect(ft.rangeSum(0, 2, 2, 2)).toBe(18); // 3+6+9
    expect(ft.rangeSum(2, 0, 2, 2)).toBe(24); // 7+8+9
  });

  test("update and query", () => {
    const matrix = [
      [1, 2],
      [3, 4],
    ];
    const ft = FenwickTree2D.fromMatrix(matrix);
    ft.update(0, 0, 9); // (0,0): 1 -> 10
    expect(ft.rangeSum(0, 0, 1, 1)).toBe(19); // 10+2+3+4
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(10);
    expect(ft.rangeSum(1, 1, 1, 1)).toBe(4); // unchanged
  });

  test("single cell query", () => {
    const matrix = [
      [5, 3],
      [7, 1],
    ];
    const ft = FenwickTree2D.fromMatrix(matrix);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(5);
    expect(ft.rangeSum(0, 1, 0, 1)).toBe(3);
    expect(ft.rangeSum(1, 0, 1, 0)).toBe(7);
    expect(ft.rangeSum(1, 1, 1, 1)).toBe(1);
  });

  test("rows and cols properties", () => {
    const ft = new FenwickTree2D(4, 6);
    expect(ft.rows).toBe(4);
    expect(ft.cols).toBe(6);
  });

  test("empty matrix", () => {
    const ft = FenwickTree2D.fromMatrix([]);
    expect(ft.rows).toBe(0);
    expect(ft.cols).toBe(0);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(0);
  });

  test("single element matrix", () => {
    const ft = FenwickTree2D.fromMatrix([[42]]);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(42);
    ft.update(0, 0, 8);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(50);
  });

  test("row vector", () => {
    const ft = FenwickTree2D.fromMatrix([[1, 2, 3, 4, 5]]);
    expect(ft.rangeSum(0, 0, 0, 4)).toBe(15);
    expect(ft.rangeSum(0, 1, 0, 3)).toBe(9);
  });

  test("column vector", () => {
    const ft = FenwickTree2D.fromMatrix([[1], [2], [3], [4], [5]]);
    expect(ft.rangeSum(0, 0, 4, 0)).toBe(15);
    expect(ft.rangeSum(1, 0, 3, 0)).toBe(9);
  });

  test("multiple updates", () => {
    const ft = new FenwickTree2D(3, 3);
    ft.update(0, 0, 1);
    ft.update(1, 1, 5);
    ft.update(2, 2, 9);
    expect(ft.rangeSum(0, 0, 2, 2)).toBe(15);
    expect(ft.rangeSum(0, 0, 0, 0)).toBe(1);
    expect(ft.rangeSum(1, 1, 1, 1)).toBe(5);
    expect(ft.rangeSum(2, 2, 2, 2)).toBe(9);
  });
});

// ============================================================================
// countInversions
// ============================================================================
describe("countInversions", () => {
  test("empty array has 0 inversions", () => {
    expect(countInversions([])).toBe(0);
  });

  test("single element has 0 inversions", () => {
    expect(countInversions([1])).toBe(0);
  });

  test("sorted array has 0 inversions", () => {
    expect(countInversions([1, 2, 3, 4, 5])).toBe(0);
  });

  test("reverse sorted has maximum inversions", () => {
    // [5,4,3,2,1] -> n*(n-1)/2 = 10 inversions
    expect(countInversions([5, 4, 3, 2, 1])).toBe(10);
  });

  test("single inversion", () => {
    expect(countInversions([2, 1])).toBe(1);
  });

  test("mixed inversions", () => {
    expect(countInversions([2, 4, 1, 3, 5])).toBe(3);
    // Inversions: (2,1), (4,1), (4,3)
  });

  test("duplicate elements", () => {
    expect(countInversions([1, 1, 1])).toBe(0);
    expect(countInversions([2, 1, 1])).toBe(2);
  });

  test("large array", () => {
    const arr = Array.from({ length: 100 }, (_, i) => 100 - i);
    expect(countInversions(arr)).toBe(4950); // 100*99/2
  });

  test("alternating elements", () => {
    expect(countInversions([3, 1, 2])).toBe(2);
    // Inversions: (3,1), (3,2)
  });

  test("negative numbers", () => {
    // -1 > -3 (yes), -1 > -2 (yes), -3 > -2 (no) => 2 inversions
    expect(countInversions([-1, -3, -2])).toBe(2);
  });
});

// ============================================================================
// RangeSumQueryMutable
// ============================================================================
describe("RangeSumQueryMutable", () => {
  test("basic query", () => {
    const rsq = new RangeSumQueryMutable([1, 3, 5]);
    expect(rsq.sumRange(0, 2)).toBe(9);
    expect(rsq.sumRange(0, 1)).toBe(4);
    expect(rsq.sumRange(1, 2)).toBe(8);
  });

  test("update and query", () => {
    const rsq = new RangeSumQueryMutable([1, 3, 5]);
    rsq.update(1, 2); // [1, 2, 5]
    expect(rsq.sumRange(0, 2)).toBe(8);
  });

  test("multiple updates", () => {
    const rsq = new RangeSumQueryMutable([1, 2, 3, 4, 5]);
    rsq.update(0, 10); // [10, 2, 3, 4, 5]
    rsq.update(4, 50); // [10, 2, 3, 4, 50]
    expect(rsq.sumRange(0, 4)).toBe(69);
    expect(rsq.sumRange(0, 0)).toBe(10);
    expect(rsq.sumRange(4, 4)).toBe(50);
  });

  test("update same index multiple times", () => {
    const rsq = new RangeSumQueryMutable([5, 5, 5]);
    rsq.update(1, 10); // [5, 10, 5]
    expect(rsq.sumRange(0, 2)).toBe(20);
    rsq.update(1, 0); // [5, 0, 5]
    expect(rsq.sumRange(0, 2)).toBe(10);
    rsq.update(1, -5); // [5, -5, 5]
    expect(rsq.sumRange(0, 2)).toBe(5);
  });

  test("LeetCode example", () => {
    const rsq = new RangeSumQueryMutable([1, 3, 5]);
    expect(rsq.sumRange(0, 2)).toBe(9);
    rsq.update(1, 2);
    expect(rsq.sumRange(0, 2)).toBe(8);
  });
});
