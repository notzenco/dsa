import { describe, test, expect } from "bun:test";
import {
  bubbleSort,
  selectionSort,
  insertionSort,
  mergeSort,
  quickSort,
  heapSort,
  shellSort,
  countingSort,
  radixSort,
  bucketSort,
} from "../../src/algorithms/sorting";

// =============================================================================
// Shared test arrays
// =============================================================================

const empty: number[] = [];
const single = [42];
const alreadySorted = [1, 2, 3, 4, 5];
const reverseSorted = [5, 4, 3, 2, 1];
const withDuplicates = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5];
const withNegatives = [-3, 5, -1, 0, 4, -2, 3];
const allSame = [7, 7, 7, 7, 7];
const twoElements = [2, 1];
const largeArray = Array.from({ length: 500 }, (_, i) => 500 - i);

// =============================================================================
// Generic comparison sort tests
// =============================================================================

function testComparisonSort(
  name: string,
  sortFn: <T>(arr: T[], cmp?: (a: T, b: T) => number) => T[]
) {
  describe(name, () => {
    test("empty array", () => {
      expect(sortFn([])).toEqual([]);
    });

    test("single element", () => {
      expect(sortFn([42])).toEqual([42]);
    });

    test("already sorted", () => {
      expect(sortFn(alreadySorted)).toEqual([1, 2, 3, 4, 5]);
    });

    test("reverse sorted", () => {
      expect(sortFn(reverseSorted)).toEqual([1, 2, 3, 4, 5]);
    });

    test("with duplicates", () => {
      expect(sortFn(withDuplicates)).toEqual([1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9]);
    });

    test("with negative numbers", () => {
      expect(sortFn(withNegatives)).toEqual([-3, -2, -1, 0, 3, 4, 5]);
    });

    test("all same elements", () => {
      expect(sortFn(allSame)).toEqual([7, 7, 7, 7, 7]);
    });

    test("two elements", () => {
      expect(sortFn(twoElements)).toEqual([1, 2]);
    });

    test("large array", () => {
      const result = sortFn(largeArray);
      const expected = Array.from({ length: 500 }, (_, i) => i + 1);
      expect(result).toEqual(expected);
    });

    test("does not mutate input", () => {
      const input = [3, 1, 2];
      const copy = [...input];
      sortFn(input);
      expect(input).toEqual(copy);
    });

    test("strings", () => {
      expect(sortFn(["banana", "apple", "cherry"])).toEqual([
        "apple",
        "banana",
        "cherry",
      ]);
    });

    test("custom comparator - descending", () => {
      const result = sortFn([3, 1, 4, 1, 5], (a, b) => {
        if (a > b) return -1;
        if (a < b) return 1;
        return 0;
      });
      expect(result).toEqual([5, 4, 3, 1, 1]);
    });

    test("custom comparator - sort by absolute value", () => {
      const result = sortFn([-5, 3, -1, 4, -2], (a: number, b: number) => {
        return Math.abs(a) - Math.abs(b);
      });
      expect(result).toEqual([-1, -2, 3, 4, -5]);
    });
  });
}

// =============================================================================
// Stability tests
// =============================================================================

interface StableItem {
  key: number;
  order: number;
}

function testStability(
  name: string,
  sortFn: <T>(arr: T[], cmp?: (a: T, b: T) => number) => T[]
) {
  describe(`${name} - stability`, () => {
    test("preserves relative order of equal elements", () => {
      const items: StableItem[] = [
        { key: 3, order: 0 },
        { key: 1, order: 1 },
        { key: 2, order: 2 },
        { key: 1, order: 3 },
        { key: 3, order: 4 },
        { key: 2, order: 5 },
      ];

      const sorted = sortFn(items, (a: StableItem, b: StableItem) => a.key - b.key);

      // Elements with the same key should maintain their original relative order
      const onesOrders = sorted.filter((x) => x.key === 1).map((x) => x.order);
      expect(onesOrders).toEqual([1, 3]);

      const twosOrders = sorted.filter((x) => x.key === 2).map((x) => x.order);
      expect(twosOrders).toEqual([2, 5]);

      const threesOrders = sorted.filter((x) => x.key === 3).map((x) => x.order);
      expect(threesOrders).toEqual([0, 4]);
    });
  });
}

// =============================================================================
// Run comparison sort tests
// =============================================================================

testComparisonSort("bubbleSort", bubbleSort);
testStability("bubbleSort", bubbleSort);

testComparisonSort("selectionSort", selectionSort);

testComparisonSort("insertionSort", insertionSort);
testStability("insertionSort", insertionSort);

testComparisonSort("mergeSort", mergeSort);
testStability("mergeSort", mergeSort);

testComparisonSort("quickSort", quickSort);

testComparisonSort("heapSort", heapSort);

testComparisonSort("shellSort", shellSort);

// =============================================================================
// Non-comparison sort tests
// =============================================================================

describe("countingSort", () => {
  test("empty array", () => {
    expect(countingSort([])).toEqual([]);
  });

  test("single element", () => {
    expect(countingSort([42])).toEqual([42]);
  });

  test("already sorted", () => {
    expect(countingSort([1, 2, 3, 4, 5])).toEqual([1, 2, 3, 4, 5]);
  });

  test("reverse sorted", () => {
    expect(countingSort([5, 4, 3, 2, 1])).toEqual([1, 2, 3, 4, 5]);
  });

  test("with duplicates", () => {
    expect(countingSort([3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5])).toEqual([
      1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9,
    ]);
  });

  test("with negative numbers", () => {
    expect(countingSort([-3, 5, -1, 0, 4, -2, 3])).toEqual([
      -3, -2, -1, 0, 3, 4, 5,
    ]);
  });

  test("all same elements", () => {
    expect(countingSort([7, 7, 7, 7, 7])).toEqual([7, 7, 7, 7, 7]);
  });

  test("does not mutate input", () => {
    const input = [3, 1, 2];
    const copy = [...input];
    countingSort(input);
    expect(input).toEqual(copy);
  });

  test("large array", () => {
    const input = Array.from({ length: 500 }, (_, i) => 500 - i);
    const expected = Array.from({ length: 500 }, (_, i) => i + 1);
    expect(countingSort(input)).toEqual(expected);
  });

  test("falls back to a comparison sort when the range exceeds 16n", () => {
    const wide = [0, 1_000_000, -5, 1_000_000];
    expect(countingSort(wide)).toEqual([-5, 0, 1_000_000, 1_000_000]);
    const copy = [...wide];
    countingSort(wide);
    expect(wide).toEqual(copy);

    // range 33 > 16 * 2, so this takes the fallback path
    expect(countingSort([0, 32])).toEqual([0, 32]);
    // range 32 === 16 * 2, so this still uses the count array
    expect(countingSort([0, 31])).toEqual([0, 31]);
  });

  test("safe integer extremes", () => {
    const input = [
      Number.MAX_SAFE_INTEGER,
      Number.MIN_SAFE_INTEGER,
      0,
      -1,
      1,
    ];
    expect(countingSort(input)).toEqual([...input].sort((a, b) => a - b));
  });

  test("rejects values that are not safe integers", () => {
    const bad = [
      NaN,
      Infinity,
      -Infinity,
      1.5,
      -0.1,
      Number.MAX_SAFE_INTEGER + 1,
      Number.MIN_SAFE_INTEGER - 1,
    ];
    for (const value of bad) {
      expect(() => countingSort([value])).toThrow(RangeError);
      expect(() => countingSort([0, value, -3])).toThrow(RangeError);
    }
  });
});

describe("radixSort", () => {
  test("empty array", () => {
    expect(radixSort([])).toEqual([]);
  });

  test("single element", () => {
    expect(radixSort([42])).toEqual([42]);
  });

  test("already sorted", () => {
    expect(radixSort([1, 2, 3, 4, 5])).toEqual([1, 2, 3, 4, 5]);
  });

  test("reverse sorted", () => {
    expect(radixSort([5, 4, 3, 2, 1])).toEqual([1, 2, 3, 4, 5]);
  });

  test("with duplicates", () => {
    expect(radixSort([3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5])).toEqual([
      1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9,
    ]);
  });

  test("all same elements", () => {
    expect(radixSort([7, 7, 7, 7, 7])).toEqual([7, 7, 7, 7, 7]);
  });

  test("multi-digit numbers", () => {
    expect(radixSort([170, 45, 75, 90, 802, 24, 2, 66])).toEqual([
      2, 24, 45, 66, 75, 90, 170, 802,
    ]);
  });

  test("does not mutate input", () => {
    const input = [3, 1, 2];
    const copy = [...input];
    radixSort(input);
    expect(input).toEqual(copy);
  });

  test("zeros", () => {
    expect(radixSort([0, 0, 0])).toEqual([0, 0, 0]);
  });

  test("large array", () => {
    const input = Array.from({ length: 500 }, (_, i) => 500 - i);
    const expected = Array.from({ length: 500 }, (_, i) => i + 1);
    expect(radixSort(input)).toEqual(expected);
  });

  test("with negative numbers", () => {
    expect(radixSort([-3, 5, -1, 0, 4, -2, 3])).toEqual([
      -3, -2, -1, 0, 3, 4, 5,
    ]);
  });

  test("all negative numbers", () => {
    expect(radixSort([-10, -1, -20, -3, -1])).toEqual([-20, -10, -3, -1, -1]);
  });

  test("safe integer extremes", () => {
    const input = [
      Number.MAX_SAFE_INTEGER,
      Number.MIN_SAFE_INTEGER,
      0,
      -1,
      1,
      42,
      -42,
      Number.MIN_SAFE_INTEGER + 2,
    ];
    expect(radixSort(input)).toEqual([...input].sort((a, b) => a - b));
    const copy = [...input];
    radixSort(input);
    expect(input).toEqual(copy);
  });

  test("rejects values that are not safe integers", () => {
    const bad = [
      NaN,
      Infinity,
      -Infinity,
      1.5,
      -0.1,
      Number.MAX_SAFE_INTEGER + 1,
      Number.MIN_SAFE_INTEGER - 1,
    ];
    for (const value of bad) {
      expect(() => radixSort([value])).toThrow(RangeError);
      expect(() => radixSort([-2, value, 4])).toThrow(RangeError);
    }
  });
});

describe("bucketSort", () => {
  test("empty array", () => {
    expect(bucketSort([])).toEqual([]);
  });

  test("single element", () => {
    expect(bucketSort([42])).toEqual([42]);
  });

  test("already sorted", () => {
    expect(bucketSort([1, 2, 3, 4, 5])).toEqual([1, 2, 3, 4, 5]);
  });

  test("reverse sorted", () => {
    expect(bucketSort([5, 4, 3, 2, 1])).toEqual([1, 2, 3, 4, 5]);
  });

  test("with duplicates", () => {
    expect(bucketSort([3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5])).toEqual([
      1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9,
    ]);
  });

  test("all same elements", () => {
    expect(bucketSort([7, 7, 7, 7, 7])).toEqual([7, 7, 7, 7, 7]);
  });

  test("floating point numbers", () => {
    const result = bucketSort([0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68]);
    const expected = [0.12, 0.17, 0.21, 0.23, 0.26, 0.39, 0.68, 0.72, 0.78, 0.94];
    expect(result).toEqual(expected);
  });

  test("custom number of buckets", () => {
    expect(bucketSort([5, 3, 1, 4, 2], 3)).toEqual([1, 2, 3, 4, 5]);
  });

  test("does not mutate input", () => {
    const input = [3, 1, 2];
    const copy = [...input];
    bucketSort(input);
    expect(input).toEqual(copy);
  });

  test("with negative numbers", () => {
    expect(bucketSort([-3, 5, -1, 0, 4, -2, 3])).toEqual([
      -3, -2, -1, 0, 3, 4, 5,
    ]);
  });

  test("large array", () => {
    const input = Array.from({ length: 500 }, (_, i) => 500 - i);
    const expected = Array.from({ length: 500 }, (_, i) => i + 1);
    expect(bucketSort(input)).toEqual(expected);
  });
});
