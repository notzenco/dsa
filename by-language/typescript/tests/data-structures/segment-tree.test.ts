import { describe, test, expect } from "bun:test";
import {
  SegmentTree,
  SegmentTreeLazy,
} from "../../src/data-structures/segment-tree";

// ============================================================================
// SegmentTree<T> - Basic Segment Tree
// ============================================================================
describe("SegmentTree", () => {
  describe("sum operations (default)", () => {
    test("build and query entire range", () => {
      const st = new SegmentTree([1, 3, 5, 7, 9, 11]);
      expect(st.query(0, 5)).toBe(36);
    });

    test("query subrange", () => {
      const st = new SegmentTree([1, 3, 5, 7, 9, 11]);
      expect(st.query(1, 4)).toBe(24); // 3+5+7+9
      expect(st.query(0, 2)).toBe(9); // 1+3+5
      expect(st.query(3, 5)).toBe(27); // 7+9+11
    });

    test("query single element", () => {
      const st = new SegmentTree([1, 3, 5, 7, 9, 11]);
      expect(st.query(0, 0)).toBe(1);
      expect(st.query(2, 2)).toBe(5);
      expect(st.query(5, 5)).toBe(11);
    });

    test("update and query", () => {
      const st = new SegmentTree([1, 3, 5, 7, 9, 11]);
      st.update(2, 10); // Change 5 -> 10
      expect(st.query(0, 5)).toBe(41); // 1+3+10+7+9+11
      expect(st.query(1, 4)).toBe(29); // 3+10+7+9
      expect(st.query(2, 2)).toBe(10);
    });

    test("multiple updates", () => {
      const st = new SegmentTree([1, 2, 3, 4, 5]);
      st.update(0, 10);
      st.update(4, 50);
      expect(st.query(0, 4)).toBe(69); // 10+2+3+4+50
      expect(st.query(0, 0)).toBe(10);
      expect(st.query(4, 4)).toBe(50);
    });

    test("update throws for out-of-range index", () => {
      const st = new SegmentTree([1, 2, 3]);
      expect(() => st.update(-1, 5)).toThrow();
      expect(() => st.update(3, 5)).toThrow();
    });

    test("single element array", () => {
      const st = new SegmentTree([42]);
      expect(st.query(0, 0)).toBe(42);
      expect(st.size).toBe(1);
      st.update(0, 100);
      expect(st.query(0, 0)).toBe(100);
    });

    test("empty array", () => {
      const st = new SegmentTree<number>([]);
      expect(st.size).toBe(0);
      expect(st.query(0, 0)).toBe(0); // returns identity
    });

    test("size property", () => {
      const st = new SegmentTree([1, 2, 3, 4, 5]);
      expect(st.size).toBe(5);
    });

    test("negative numbers", () => {
      const st = new SegmentTree([-1, -3, 5, -7, 9]);
      expect(st.query(0, 4)).toBe(3);
      expect(st.query(0, 1)).toBe(-4);
      expect(st.query(2, 4)).toBe(7);
    });

    test("all zeros", () => {
      const st = new SegmentTree([0, 0, 0, 0, 0]);
      expect(st.query(0, 4)).toBe(0);
      st.update(2, 5);
      expect(st.query(0, 4)).toBe(5);
    });
  });

  describe("min operations", () => {
    test("range minimum query", () => {
      const st = new SegmentTree<number>(
        [3, 1, 4, 1, 5, 9, 2, 6],
        (a, b) => Math.min(a, b),
        Infinity
      );
      expect(st.query(0, 7)).toBe(1);
      expect(st.query(0, 1)).toBe(1);
      expect(st.query(4, 7)).toBe(2);
      expect(st.query(4, 5)).toBe(5);
    });

    test("min update and query", () => {
      const st = new SegmentTree<number>(
        [5, 3, 8, 1, 7],
        (a, b) => Math.min(a, b),
        Infinity
      );
      expect(st.query(0, 4)).toBe(1);
      st.update(3, 10); // replace 1 with 10
      expect(st.query(0, 4)).toBe(3);
      expect(st.query(2, 4)).toBe(7);
    });

    test("min with single element query", () => {
      const st = new SegmentTree<number>(
        [5, 3, 8],
        (a, b) => Math.min(a, b),
        Infinity
      );
      expect(st.query(1, 1)).toBe(3);
    });
  });

  describe("max operations", () => {
    test("range maximum query", () => {
      const st = new SegmentTree<number>(
        [3, 1, 4, 1, 5, 9, 2, 6],
        (a, b) => Math.max(a, b),
        -Infinity
      );
      expect(st.query(0, 7)).toBe(9);
      expect(st.query(0, 3)).toBe(4);
      expect(st.query(4, 7)).toBe(9);
      expect(st.query(6, 7)).toBe(6);
    });

    test("max update and query", () => {
      const st = new SegmentTree<number>(
        [1, 5, 3, 8, 2],
        (a, b) => Math.max(a, b),
        -Infinity
      );
      expect(st.query(0, 4)).toBe(8);
      st.update(3, 0); // replace 8 with 0
      expect(st.query(0, 4)).toBe(5);
    });
  });

  describe("large arrays", () => {
    test("1000 elements sum", () => {
      const arr = Array.from({ length: 1000 }, (_, i) => i + 1);
      const st = new SegmentTree(arr);
      // Sum of 1 to 1000 = 500500
      expect(st.query(0, 999)).toBe(500500);
      // Sum of first 100 = 5050
      expect(st.query(0, 99)).toBe(5050);
    });

    test("many updates", () => {
      const arr = new Array(100).fill(1);
      const st = new SegmentTree(arr);
      expect(st.query(0, 99)).toBe(100);

      for (let i = 0; i < 100; i++) {
        st.update(i, i + 1);
      }
      expect(st.query(0, 99)).toBe(5050);
    });
  });
});

// ============================================================================
// SegmentTreeLazy - Lazy Propagation
// ============================================================================
describe("SegmentTreeLazy", () => {
  test("build and query entire range", () => {
    const st = new SegmentTreeLazy([1, 3, 5, 7, 9, 11]);
    expect(st.query(0, 5)).toBe(36);
  });

  test("query subrange", () => {
    const st = new SegmentTreeLazy([1, 3, 5, 7, 9, 11]);
    expect(st.query(1, 4)).toBe(24);
    expect(st.query(0, 2)).toBe(9);
  });

  test("range update and query", () => {
    const st = new SegmentTreeLazy([1, 3, 5, 7, 9, 11]);
    st.rangeUpdate(1, 4, 10); // Add 10 to indices 1-4
    // After: [1, 13, 15, 17, 19, 11]
    expect(st.query(0, 5)).toBe(76); // 36 + 40
    expect(st.query(1, 4)).toBe(64); // 24 + 40
    expect(st.query(0, 0)).toBe(1); // unchanged
    expect(st.query(5, 5)).toBe(11); // unchanged
  });

  test("multiple range updates", () => {
    const st = new SegmentTreeLazy([0, 0, 0, 0, 0]);
    st.rangeUpdate(0, 4, 1); // [1,1,1,1,1]
    expect(st.query(0, 4)).toBe(5);

    st.rangeUpdate(1, 3, 2); // [1,3,3,3,1]
    expect(st.query(0, 4)).toBe(11);
    expect(st.query(1, 3)).toBe(9);
  });

  test("overlapping range updates", () => {
    const st = new SegmentTreeLazy([0, 0, 0, 0, 0, 0]);
    st.rangeUpdate(0, 3, 5); // [5,5,5,5,0,0]
    st.rangeUpdate(2, 5, 3); // [5,5,8,8,3,3]
    expect(st.query(0, 5)).toBe(32);
    expect(st.query(0, 1)).toBe(10);
    expect(st.query(2, 3)).toBe(16);
    expect(st.query(4, 5)).toBe(6);
  });

  test("point update", () => {
    const st = new SegmentTreeLazy([1, 2, 3, 4, 5]);
    st.pointUpdate(2, 10); // [1,2,13,4,5]
    expect(st.query(0, 4)).toBe(25);
    expect(st.query(2, 2)).toBe(13);
  });

  test("range update entire array", () => {
    const st = new SegmentTreeLazy([1, 2, 3, 4, 5]);
    st.rangeUpdate(0, 4, 100);
    expect(st.query(0, 4)).toBe(515); // 15 + 500
  });

  test("negative range updates", () => {
    const st = new SegmentTreeLazy([10, 20, 30, 40, 50]);
    st.rangeUpdate(0, 4, -5);
    expect(st.query(0, 4)).toBe(125); // 150 - 25
    expect(st.query(0, 0)).toBe(5);
    expect(st.query(4, 4)).toBe(45);
  });

  test("single element", () => {
    const st = new SegmentTreeLazy([42]);
    expect(st.query(0, 0)).toBe(42);
    st.rangeUpdate(0, 0, 8);
    expect(st.query(0, 0)).toBe(50);
  });

  test("empty array", () => {
    const st = new SegmentTreeLazy([]);
    expect(st.size).toBe(0);
    expect(st.query(0, 0)).toBe(0);
  });

  test("size property", () => {
    const st = new SegmentTreeLazy([1, 2, 3, 4, 5]);
    expect(st.size).toBe(5);
  });

  test("interleaved updates and queries", () => {
    const st = new SegmentTreeLazy([1, 1, 1, 1, 1]);
    expect(st.query(0, 4)).toBe(5);

    st.rangeUpdate(0, 2, 3); // [4,4,4,1,1]
    expect(st.query(0, 4)).toBe(14);

    st.rangeUpdate(2, 4, 2); // [4,4,6,3,3]
    expect(st.query(0, 4)).toBe(20);

    st.pointUpdate(0, -4); // [0,4,6,3,3]
    expect(st.query(0, 4)).toBe(16);
    expect(st.query(0, 0)).toBe(0);
  });

  test("large array with range updates", () => {
    const n = 1000;
    const arr = new Array(n).fill(0);
    const st = new SegmentTreeLazy(arr);

    // Add 1 to entire array
    st.rangeUpdate(0, n - 1, 1);
    expect(st.query(0, n - 1)).toBe(n);

    // Add 2 to first half
    st.rangeUpdate(0, 499, 2);
    expect(st.query(0, 499)).toBe(1500); // 500 * 3
    expect(st.query(500, 999)).toBe(500); // 500 * 1
  });

  test("single element queries after range update", () => {
    const st = new SegmentTreeLazy([1, 2, 3, 4, 5]);
    st.rangeUpdate(0, 4, 10);
    expect(st.query(0, 0)).toBe(11);
    expect(st.query(1, 1)).toBe(12);
    expect(st.query(2, 2)).toBe(13);
    expect(st.query(3, 3)).toBe(14);
    expect(st.query(4, 4)).toBe(15);
  });
});
