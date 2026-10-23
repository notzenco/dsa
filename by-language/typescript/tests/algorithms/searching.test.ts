import { describe, test, expect } from "bun:test";
import {
  linearSearch,
  binarySearch,
  lowerBound,
  upperBound,
  jumpSearch,
  interpolationSearch,
  exponentialSearch,
  ternarySearch,
} from "../../src/algorithms/searching";

// =============================================================================
// linearSearch
// =============================================================================

describe("linearSearch", () => {
  test("empty array", () => {
    expect(linearSearch([], 5)).toBe(-1);
  });

  test("single element - found", () => {
    expect(linearSearch([5], 5)).toBe(0);
  });

  test("single element - not found", () => {
    expect(linearSearch([5], 3)).toBe(-1);
  });

  test("first element", () => {
    expect(linearSearch([1, 2, 3, 4, 5], 1)).toBe(0);
  });

  test("last element", () => {
    expect(linearSearch([1, 2, 3, 4, 5], 5)).toBe(4);
  });

  test("middle element", () => {
    expect(linearSearch([1, 2, 3, 4, 5], 3)).toBe(2);
  });

  test("not found", () => {
    expect(linearSearch([1, 2, 3, 4, 5], 6)).toBe(-1);
  });

  test("returns first occurrence with duplicates", () => {
    expect(linearSearch([1, 2, 3, 2, 1], 2)).toBe(1);
  });

  test("strings", () => {
    expect(linearSearch(["apple", "banana", "cherry"], "banana")).toBe(1);
  });

  test("strings - not found", () => {
    expect(linearSearch(["apple", "banana", "cherry"], "grape")).toBe(-1);
  });
});

// =============================================================================
// binarySearch
// =============================================================================

describe("binarySearch", () => {
  test("empty array", () => {
    expect(binarySearch([], 5)).toBe(-1);
  });

  test("single element - found", () => {
    expect(binarySearch([5], 5)).toBe(0);
  });

  test("single element - not found", () => {
    expect(binarySearch([5], 3)).toBe(-1);
  });

  test("first element", () => {
    expect(binarySearch([1, 2, 3, 4, 5], 1)).toBe(0);
  });

  test("last element", () => {
    expect(binarySearch([1, 2, 3, 4, 5], 5)).toBe(4);
  });

  test("middle element", () => {
    expect(binarySearch([1, 2, 3, 4, 5], 3)).toBe(2);
  });

  test("not found - too small", () => {
    expect(binarySearch([1, 2, 3, 4, 5], 0)).toBe(-1);
  });

  test("not found - too large", () => {
    expect(binarySearch([1, 2, 3, 4, 5], 6)).toBe(-1);
  });

  test("not found - gap", () => {
    expect(binarySearch([1, 3, 5, 7, 9], 4)).toBe(-1);
  });

  test("two elements - find first", () => {
    expect(binarySearch([1, 2], 1)).toBe(0);
  });

  test("two elements - find second", () => {
    expect(binarySearch([1, 2], 2)).toBe(1);
  });

  test("even number of elements", () => {
    expect(binarySearch([1, 2, 3, 4, 5, 6], 4)).toBe(3);
  });

  test("strings with comparator", () => {
    const arr = ["apple", "banana", "cherry", "grape"];
    const cmp = (a: string, b: string) => a.localeCompare(b);
    expect(binarySearch(arr, "cherry", cmp)).toBe(2);
  });

  test("custom comparator - descending", () => {
    const arr = [5, 4, 3, 2, 1];
    const cmp = (a: number, b: number) => b - a;
    expect(binarySearch(arr, 3, cmp)).toBe(2);
  });

  test("large array", () => {
    const arr = Array.from({ length: 10000 }, (_, i) => i);
    expect(binarySearch(arr, 7777)).toBe(7777);
    expect(binarySearch(arr, 10000)).toBe(-1);
  });
});

// =============================================================================
// lowerBound
// =============================================================================

describe("lowerBound", () => {
  test("empty array", () => {
    expect(lowerBound([], 5)).toBe(0);
  });

  test("all elements less than target", () => {
    expect(lowerBound([1, 2, 3], 5)).toBe(3);
  });

  test("all elements greater than target", () => {
    expect(lowerBound([5, 6, 7], 3)).toBe(0);
  });

  test("target exists - single occurrence", () => {
    expect(lowerBound([1, 3, 5, 7, 9], 5)).toBe(2);
  });

  test("target exists - multiple occurrences returns first", () => {
    expect(lowerBound([1, 2, 2, 2, 3], 2)).toBe(1);
  });

  test("target does not exist - returns insertion point", () => {
    expect(lowerBound([1, 3, 5, 7, 9], 4)).toBe(2);
  });

  test("target is smallest", () => {
    expect(lowerBound([1, 2, 3], 1)).toBe(0);
  });

  test("target is largest", () => {
    expect(lowerBound([1, 2, 3], 3)).toBe(2);
  });

  test("single element - equal", () => {
    expect(lowerBound([5], 5)).toBe(0);
  });

  test("single element - less", () => {
    expect(lowerBound([5], 3)).toBe(0);
  });

  test("single element - greater", () => {
    expect(lowerBound([5], 7)).toBe(1);
  });

  test("all same elements", () => {
    expect(lowerBound([3, 3, 3, 3], 3)).toBe(0);
  });
});

// =============================================================================
// upperBound
// =============================================================================

describe("upperBound", () => {
  test("empty array", () => {
    expect(upperBound([], 5)).toBe(0);
  });

  test("all elements less than target", () => {
    expect(upperBound([1, 2, 3], 5)).toBe(3);
  });

  test("all elements greater than target", () => {
    expect(upperBound([5, 6, 7], 3)).toBe(0);
  });

  test("target exists - single occurrence", () => {
    expect(upperBound([1, 3, 5, 7, 9], 5)).toBe(3);
  });

  test("target exists - multiple occurrences returns past last", () => {
    expect(upperBound([1, 2, 2, 2, 3], 2)).toBe(4);
  });

  test("target does not exist", () => {
    expect(upperBound([1, 3, 5, 7, 9], 4)).toBe(2);
  });

  test("target is smallest", () => {
    expect(upperBound([1, 2, 3], 1)).toBe(1);
  });

  test("target is largest", () => {
    expect(upperBound([1, 2, 3], 3)).toBe(3);
  });

  test("single element - equal", () => {
    expect(upperBound([5], 5)).toBe(1);
  });

  test("all same elements", () => {
    expect(upperBound([3, 3, 3, 3], 3)).toBe(4);
  });

  test("lowerBound and upperBound together count occurrences", () => {
    const arr = [1, 2, 2, 2, 3, 4, 4, 5];
    expect(upperBound(arr, 2) - lowerBound(arr, 2)).toBe(3);
    expect(upperBound(arr, 4) - lowerBound(arr, 4)).toBe(2);
    expect(upperBound(arr, 6) - lowerBound(arr, 6)).toBe(0);
  });
});

// =============================================================================
// Shared test helper for number-based search functions
// =============================================================================

function testNumberSearch(
  name: string,
  searchFn: (arr: number[], target: number) => number
) {
  describe(name, () => {
    test("empty array", () => {
      expect(searchFn([], 5)).toBe(-1);
    });

    test("single element - found", () => {
      expect(searchFn([5], 5)).toBe(0);
    });

    test("single element - not found", () => {
      expect(searchFn([5], 3)).toBe(-1);
    });

    test("first element", () => {
      expect(searchFn([1, 2, 3, 4, 5], 1)).toBe(0);
    });

    test("last element", () => {
      expect(searchFn([1, 2, 3, 4, 5], 5)).toBe(4);
    });

    test("middle element", () => {
      expect(searchFn([1, 2, 3, 4, 5], 3)).toBe(2);
    });

    test("not found - too small", () => {
      expect(searchFn([1, 2, 3, 4, 5], 0)).toBe(-1);
    });

    test("not found - too large", () => {
      expect(searchFn([1, 2, 3, 4, 5], 6)).toBe(-1);
    });

    test("not found - gap", () => {
      expect(searchFn([1, 3, 5, 7, 9], 4)).toBe(-1);
    });

    test("two elements - find first", () => {
      expect(searchFn([1, 2], 1)).toBe(0);
    });

    test("two elements - find second", () => {
      expect(searchFn([1, 2], 2)).toBe(1);
    });

    test("large sorted array", () => {
      const arr = Array.from({ length: 10000 }, (_, i) => i * 2);
      expect(searchFn(arr, 5000)).toBe(2500);
      expect(searchFn(arr, 5001)).toBe(-1);
    });
  });
}

testNumberSearch("jumpSearch", jumpSearch);
testNumberSearch("interpolationSearch", interpolationSearch);
test("interpolationSearch handles equal endpoints", () => {
  expect(interpolationSearch([1, 1], 1)).toBe(0);
});
testNumberSearch("exponentialSearch", exponentialSearch);
testNumberSearch("ternarySearch", ternarySearch);
