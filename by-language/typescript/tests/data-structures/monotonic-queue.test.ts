import { describe, test, expect } from "bun:test";
import {
  MonotonicQueue,
  slidingWindowMaximum,
} from "../../src/data-structures/monotonic-queue";

// ============================================================================
// MonotonicQueue<T>
// ============================================================================
describe("MonotonicQueue", () => {
  describe("basic operations", () => {
    test("new queue is empty", () => {
      const mq = new MonotonicQueue<number>();
      expect(mq.size).toBe(0);
      expect(mq.isEmpty).toBe(true);
    });

    test("push increases size", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      expect(mq.size).toBe(1);
      expect(mq.isEmpty).toBe(false);
      mq.push(3);
      expect(mq.size).toBe(2);
    });

    test("popFront returns elements in FIFO order", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(1);
      mq.push(2);
      mq.push(3);
      expect(mq.popFront()).toBe(1);
      expect(mq.popFront()).toBe(2);
      expect(mq.popFront()).toBe(3);
      expect(mq.isEmpty).toBe(true);
    });

    test("popFront throws on empty queue", () => {
      const mq = new MonotonicQueue<number>();
      expect(() => mq.popFront()).toThrow("empty");
    });
  });

  describe("max queries", () => {
    test("max returns the maximum element", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(3);
      expect(mq.max).toBe(3);
      mq.push(1);
      expect(mq.max).toBe(3);
      mq.push(5);
      expect(mq.max).toBe(5);
    });

    test("max updates after popFront removes the max", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      mq.push(3);
      mq.push(4);
      expect(mq.max).toBe(5);
      mq.popFront(); // Remove 5
      expect(mq.max).toBe(4);
    });

    test("max throws on empty queue", () => {
      const mq = new MonotonicQueue<number>();
      expect(() => mq.max).toThrow("empty");
    });

    test("popMax returns current max", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(3);
      mq.push(7);
      mq.push(2);
      expect(mq.popMax()).toBe(7);
    });

    test("popMax throws on empty queue", () => {
      const mq = new MonotonicQueue<number>();
      expect(() => mq.popMax()).toThrow("empty");
    });
  });

  describe("min queries", () => {
    test("min returns the minimum element", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(3);
      expect(mq.min).toBe(3);
      mq.push(5);
      expect(mq.min).toBe(3);
      mq.push(1);
      expect(mq.min).toBe(1);
    });

    test("min updates after popFront removes the min", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(1);
      mq.push(5);
      mq.push(3);
      expect(mq.min).toBe(1);
      mq.popFront(); // Remove 1
      expect(mq.min).toBe(3);
    });

    test("min throws on empty queue", () => {
      const mq = new MonotonicQueue<number>();
      expect(() => mq.min).toThrow("empty");
    });

    test("popMin returns current min", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      mq.push(2);
      mq.push(8);
      expect(mq.popMin()).toBe(2);
    });

    test("popMin throws on empty queue", () => {
      const mq = new MonotonicQueue<number>();
      expect(() => mq.popMin()).toThrow("empty");
    });
  });

  describe("monotonic property", () => {
    test("max deque maintains non-increasing order", () => {
      const mq = new MonotonicQueue<number>();
      // Push sequence: 3, 1, 4, 1, 5, 9
      mq.push(3);
      expect(mq.max).toBe(3);
      mq.push(1);
      expect(mq.max).toBe(3);
      mq.push(4);
      expect(mq.max).toBe(4); // 4 > 3, so 3 and 1 are evicted from max-deque
      mq.push(1);
      expect(mq.max).toBe(4);
      mq.push(5);
      expect(mq.max).toBe(5);
      mq.push(9);
      expect(mq.max).toBe(9);
    });

    test("min deque maintains non-decreasing order", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      expect(mq.min).toBe(5);
      mq.push(8);
      expect(mq.min).toBe(5);
      mq.push(3);
      expect(mq.min).toBe(3); // 3 < 5, so 5 and 8 are evicted from min-deque
      mq.push(7);
      expect(mq.min).toBe(3);
      mq.push(1);
      expect(mq.min).toBe(1);
    });

    test("interleaved push and popFront with max tracking", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(2);
      mq.push(5);
      mq.push(3);
      expect(mq.max).toBe(5);

      mq.popFront(); // Remove 2
      expect(mq.max).toBe(5);

      mq.popFront(); // Remove 5
      expect(mq.max).toBe(3);

      mq.push(7);
      expect(mq.max).toBe(7);
    });

    test("interleaved push and popFront with min tracking", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      mq.push(2);
      mq.push(4);
      expect(mq.min).toBe(2);

      mq.popFront(); // Remove 5
      expect(mq.min).toBe(2);

      mq.popFront(); // Remove 2
      expect(mq.min).toBe(4);

      mq.push(1);
      expect(mq.min).toBe(1);
    });
  });

  describe("edge cases", () => {
    test("single element", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(42);
      expect(mq.max).toBe(42);
      expect(mq.min).toBe(42);
      expect(mq.size).toBe(1);
      expect(mq.popFront()).toBe(42);
      expect(mq.isEmpty).toBe(true);
    });

    test("all equal elements", () => {
      const mq = new MonotonicQueue<number>();
      for (let i = 0; i < 5; i++) mq.push(3);
      expect(mq.max).toBe(3);
      expect(mq.min).toBe(3);
      expect(mq.size).toBe(5);
      for (let i = 0; i < 5; i++) {
        expect(mq.popFront()).toBe(3);
      }
    });

    test("increasing sequence", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(1);
      mq.push(2);
      mq.push(3);
      mq.push(4);
      mq.push(5);
      expect(mq.max).toBe(5);
      expect(mq.min).toBe(1);
    });

    test("decreasing sequence", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(5);
      mq.push(4);
      mq.push(3);
      mq.push(2);
      mq.push(1);
      expect(mq.max).toBe(5);
      expect(mq.min).toBe(1);
    });

    test("negative numbers", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(-3);
      mq.push(-1);
      mq.push(-5);
      expect(mq.max).toBe(-1);
      expect(mq.min).toBe(-5);
    });
  });

  describe("custom comparator", () => {
    test("string length comparator", () => {
      const mq = new MonotonicQueue<string>(
        (a, b) => a.length - b.length
      );
      mq.push("ab");
      mq.push("abcde");
      mq.push("abc");

      expect(mq.max).toBe("abcde"); // longest
      expect(mq.min).toBe("ab"); // shortest
    });
  });

  describe("simulating sliding window with MonotonicQueue", () => {
    test("sliding window max using MonotonicQueue", () => {
      const nums = [1, 3, -1, -3, 5, 3, 6, 7];
      const k = 3;
      const result: number[] = [];
      const mq = new MonotonicQueue<number>();

      for (let i = 0; i < nums.length; i++) {
        mq.push(nums[i]);
        if (i >= k) {
          mq.popFront(); // Remove element leaving the window
        }
        if (i >= k - 1) {
          result.push(mq.max);
        }
      }

      expect(result).toEqual([3, 3, 5, 5, 6, 7]);
    });
  });
});

// ============================================================================
// slidingWindowMaximum utility
// ============================================================================
describe("slidingWindowMaximum", () => {
  test("basic example", () => {
    expect(slidingWindowMaximum([1, 3, -1, -3, 5, 3, 6, 7], 3)).toEqual([
      3, 3, 5, 5, 6, 7,
    ]);
  });

  test("window size 1 returns the array itself", () => {
    expect(slidingWindowMaximum([1, 2, 3, 4, 5], 1)).toEqual([
      1, 2, 3, 4, 5,
    ]);
  });

  test("window size equals array length", () => {
    expect(slidingWindowMaximum([1, 3, 2, 5, 4], 5)).toEqual([5]);
  });

  test("empty array returns empty", () => {
    expect(slidingWindowMaximum([], 3)).toEqual([]);
  });

  test("k <= 0 returns empty", () => {
    expect(slidingWindowMaximum([1, 2, 3], 0)).toEqual([]);
    expect(slidingWindowMaximum([1, 2, 3], -1)).toEqual([]);
  });

  test("single element array", () => {
    expect(slidingWindowMaximum([42], 1)).toEqual([42]);
  });

  test("all equal elements", () => {
    expect(slidingWindowMaximum([3, 3, 3, 3, 3], 3)).toEqual([3, 3, 3]);
  });

  test("decreasing array", () => {
    expect(slidingWindowMaximum([5, 4, 3, 2, 1], 3)).toEqual([5, 4, 3]);
  });

  test("increasing array", () => {
    expect(slidingWindowMaximum([1, 2, 3, 4, 5], 3)).toEqual([3, 4, 5]);
  });

  test("window size 2", () => {
    expect(slidingWindowMaximum([1, 3, 2, 5, 4], 2)).toEqual([3, 3, 5, 5]);
  });

  test("negative numbers", () => {
    expect(slidingWindowMaximum([-3, -1, -5, -2, -4], 3)).toEqual([
      -1, -1, -2,
    ]);
  });

  test("large window with peaks", () => {
    expect(slidingWindowMaximum([1, 10, 1, 1, 1, 10, 1], 4)).toEqual([
      10, 10, 10, 10,
    ]);
  });
});
