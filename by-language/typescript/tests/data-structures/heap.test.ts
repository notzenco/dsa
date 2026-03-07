import { describe, test, expect } from "bun:test";
import {
  MinHeap,
  MaxHeap,
  PriorityQueue,
  heapSort,
  nLargest,
  nSmallest,
} from "../../src/data-structures/heap";

describe("MinHeap", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const heap = new MinHeap<number>();
      expect(heap.size).toBe(0);
      expect(heap.isEmpty).toBe(true);
    });

    test("should push elements and maintain min at top", () => {
      const heap = new MinHeap<number>();
      heap.push(5);
      heap.push(3);
      heap.push(7);
      heap.push(1);
      expect(heap.peek()).toBe(1);
      expect(heap.size).toBe(4);
    });

    test("should pop minimum element", () => {
      const heap = new MinHeap<number>();
      heap.push(5);
      heap.push(3);
      heap.push(7);
      heap.push(1);
      expect(heap.pop()).toBe(1);
      expect(heap.pop()).toBe(3);
      expect(heap.pop()).toBe(5);
      expect(heap.pop()).toBe(7);
      expect(heap.isEmpty).toBe(true);
    });

    test("should throw on pop from empty heap", () => {
      const heap = new MinHeap<number>();
      expect(() => heap.pop()).toThrow("Pop from empty heap");
    });

    test("should throw on peek from empty heap", () => {
      const heap = new MinHeap<number>();
      expect(() => heap.peek()).toThrow("Peek from empty heap");
    });

    test("should clear the heap", () => {
      const heap = new MinHeap<number>();
      heap.push(1);
      heap.push(2);
      heap.clear();
      expect(heap.size).toBe(0);
      expect(heap.isEmpty).toBe(true);
    });
  });

  describe("heap property", () => {
    test("should maintain heap property after many pushes", () => {
      const heap = new MinHeap<number>();
      const values = [10, 4, 15, 20, 0, 8, 2, 25, 1, 13];
      for (const v of values) {
        heap.push(v);
      }

      let prev = -Infinity;
      while (!heap.isEmpty) {
        const val = heap.pop();
        expect(val).toBeGreaterThanOrEqual(prev);
        prev = val;
      }
    });

    test("should handle duplicate values", () => {
      const heap = new MinHeap<number>();
      heap.push(5);
      heap.push(5);
      heap.push(5);
      expect(heap.pop()).toBe(5);
      expect(heap.pop()).toBe(5);
      expect(heap.pop()).toBe(5);
    });

    test("should handle single element", () => {
      const heap = new MinHeap<number>();
      heap.push(42);
      expect(heap.peek()).toBe(42);
      expect(heap.pop()).toBe(42);
      expect(heap.isEmpty).toBe(true);
    });
  });

  describe("toArray and toSortedArray", () => {
    test("toArray returns elements (not necessarily sorted)", () => {
      const heap = new MinHeap<number>();
      heap.push(3);
      heap.push(1);
      heap.push(2);
      expect(heap.toArray().sort()).toEqual([1, 2, 3]);
    });

    test("toSortedArray returns sorted elements", () => {
      const heap = new MinHeap<number>();
      heap.push(5);
      heap.push(3);
      heap.push(8);
      heap.push(1);
      expect(heap.toSortedArray()).toEqual([1, 3, 5, 8]);
      // Original heap should not be modified
      expect(heap.size).toBe(4);
    });
  });

  describe("heapify", () => {
    test("should build heap from array in O(n)", () => {
      const heap = MinHeap.heapify([5, 3, 8, 1, 2, 7, 4]);
      expect(heap.peek()).toBe(1);

      const sorted: number[] = [];
      while (!heap.isEmpty) {
        sorted.push(heap.pop());
      }
      expect(sorted).toEqual([1, 2, 3, 4, 5, 7, 8]);
    });

    test("should handle empty array", () => {
      const heap = MinHeap.heapify<number>([]);
      expect(heap.isEmpty).toBe(true);
    });

    test("should handle single element", () => {
      const heap = MinHeap.heapify([42]);
      expect(heap.peek()).toBe(42);
    });
  });

  describe("with strings", () => {
    test("should sort strings lexicographically", () => {
      const heap = new MinHeap<string>();
      heap.push("banana");
      heap.push("apple");
      heap.push("cherry");
      expect(heap.pop()).toBe("apple");
      expect(heap.pop()).toBe("banana");
      expect(heap.pop()).toBe("cherry");
    });
  });
});

describe("MaxHeap", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const heap = new MaxHeap<number>();
      expect(heap.size).toBe(0);
      expect(heap.isEmpty).toBe(true);
    });

    test("should push elements and maintain max at top", () => {
      const heap = new MaxHeap<number>();
      heap.push(5);
      heap.push(3);
      heap.push(7);
      heap.push(1);
      expect(heap.peek()).toBe(7);
    });

    test("should pop maximum element", () => {
      const heap = new MaxHeap<number>();
      heap.push(5);
      heap.push(3);
      heap.push(7);
      heap.push(1);
      expect(heap.pop()).toBe(7);
      expect(heap.pop()).toBe(5);
      expect(heap.pop()).toBe(3);
      expect(heap.pop()).toBe(1);
    });

    test("should throw on pop from empty heap", () => {
      const heap = new MaxHeap<number>();
      expect(() => heap.pop()).toThrow("Pop from empty heap");
    });

    test("should throw on peek from empty heap", () => {
      const heap = new MaxHeap<number>();
      expect(() => heap.peek()).toThrow("Peek from empty heap");
    });
  });

  describe("heap property", () => {
    test("should maintain max-heap property", () => {
      const heap = new MaxHeap<number>();
      const values = [10, 4, 15, 20, 0, 8, 2, 25, 1, 13];
      for (const v of values) {
        heap.push(v);
      }

      let prev = Infinity;
      while (!heap.isEmpty) {
        const val = heap.pop();
        expect(val).toBeLessThanOrEqual(prev);
        prev = val;
      }
    });

    test("should handle duplicate values", () => {
      const heap = new MaxHeap<number>();
      heap.push(3);
      heap.push(3);
      heap.push(3);
      expect(heap.pop()).toBe(3);
      expect(heap.pop()).toBe(3);
      expect(heap.pop()).toBe(3);
    });
  });

  describe("heapify", () => {
    test("should build max heap from array", () => {
      const heap = MaxHeap.heapify([5, 3, 8, 1, 2, 7, 4]);
      expect(heap.peek()).toBe(8);

      const sorted: number[] = [];
      while (!heap.isEmpty) {
        sorted.push(heap.pop());
      }
      expect(sorted).toEqual([8, 7, 5, 4, 3, 2, 1]);
    });
  });

  describe("toSortedArray", () => {
    test("should return descending sorted array", () => {
      const heap = new MaxHeap<number>();
      heap.push(1);
      heap.push(5);
      heap.push(3);
      expect(heap.toSortedArray()).toEqual([5, 3, 1]);
      expect(heap.size).toBe(3); // original not modified
    });
  });
});

describe("PriorityQueue", () => {
  describe("basic operations", () => {
    test("should start empty", () => {
      const pq = new PriorityQueue<string>();
      expect(pq.size).toBe(0);
      expect(pq.isEmpty).toBe(true);
    });

    test("should enqueue and dequeue by priority", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("low", 3);
      pq.enqueue("high", 1);
      pq.enqueue("medium", 2);
      expect(pq.dequeue()).toBe("high");
      expect(pq.dequeue()).toBe("medium");
      expect(pq.dequeue()).toBe("low");
    });

    test("should peek without removing", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("a", 2);
      pq.enqueue("b", 1);
      expect(pq.peek()).toBe("b");
      expect(pq.size).toBe(2);
    });

    test("should throw on dequeue from empty", () => {
      const pq = new PriorityQueue<string>();
      expect(() => pq.dequeue()).toThrow("Dequeue from empty queue");
    });

    test("should throw on peek from empty", () => {
      const pq = new PriorityQueue<string>();
      expect(() => pq.peek()).toThrow("Peek from empty queue");
    });

    test("should clear the queue", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("a", 1);
      pq.enqueue("b", 2);
      pq.clear();
      expect(pq.size).toBe(0);
      expect(pq.isEmpty).toBe(true);
    });
  });

  describe("stability (FIFO for same priority)", () => {
    test("should maintain insertion order for equal priorities", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("first", 1);
      pq.enqueue("second", 1);
      pq.enqueue("third", 1);
      expect(pq.dequeue()).toBe("first");
      expect(pq.dequeue()).toBe("second");
      expect(pq.dequeue()).toBe("third");
    });

    test("stability across different priority groups", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("a1", 1);
      pq.enqueue("b1", 2);
      pq.enqueue("a2", 1);
      pq.enqueue("b2", 2);
      expect(pq.dequeue()).toBe("a1");
      expect(pq.dequeue()).toBe("a2");
      expect(pq.dequeue()).toBe("b1");
      expect(pq.dequeue()).toBe("b2");
    });
  });

  describe("mixed operations", () => {
    test("interleaved enqueue and dequeue", () => {
      const pq = new PriorityQueue<number>();
      pq.enqueue(10, 10);
      pq.enqueue(5, 5);
      expect(pq.dequeue()).toBe(5);
      pq.enqueue(1, 1);
      pq.enqueue(8, 8);
      expect(pq.dequeue()).toBe(1);
      expect(pq.dequeue()).toBe(8);
      expect(pq.dequeue()).toBe(10);
    });

    test("should handle negative priorities", () => {
      const pq = new PriorityQueue<string>();
      pq.enqueue("a", -5);
      pq.enqueue("b", -10);
      pq.enqueue("c", 0);
      expect(pq.dequeue()).toBe("b");
      expect(pq.dequeue()).toBe("a");
      expect(pq.dequeue()).toBe("c");
    });
  });
});

describe("heapSort", () => {
  test("should sort array in ascending order", () => {
    expect(heapSort([5, 3, 8, 1, 2, 7, 4])).toEqual([1, 2, 3, 4, 5, 7, 8]);
  });

  test("should handle empty array", () => {
    expect(heapSort([])).toEqual([]);
  });

  test("should handle single element", () => {
    expect(heapSort([42])).toEqual([42]);
  });

  test("should handle already sorted array", () => {
    expect(heapSort([1, 2, 3, 4, 5])).toEqual([1, 2, 3, 4, 5]);
  });

  test("should handle reverse sorted array", () => {
    expect(heapSort([5, 4, 3, 2, 1])).toEqual([1, 2, 3, 4, 5]);
  });

  test("should handle duplicates", () => {
    expect(heapSort([3, 1, 2, 1, 3])).toEqual([1, 1, 2, 3, 3]);
  });

  test("should not modify original array", () => {
    const original = [5, 3, 1, 4, 2];
    heapSort(original);
    expect(original).toEqual([5, 3, 1, 4, 2]);
  });
});

describe("nLargest", () => {
  test("should find n largest elements", () => {
    expect(nLargest([3, 1, 5, 2, 4], 3)).toEqual([5, 4, 3]);
  });

  test("should return empty for n=0", () => {
    expect(nLargest([1, 2, 3], 0)).toEqual([]);
  });

  test("should return all elements (sorted desc) if n >= length", () => {
    expect(nLargest([3, 1, 2], 5)).toEqual([3, 2, 1]);
  });

  test("should handle n=1", () => {
    expect(nLargest([10, 20, 5, 15], 1)).toEqual([20]);
  });

  test("should handle duplicates", () => {
    expect(nLargest([5, 5, 3, 3, 1], 3)).toEqual([5, 5, 3]);
  });
});

describe("nSmallest", () => {
  test("should find n smallest elements", () => {
    expect(nSmallest([3, 1, 5, 2, 4], 3)).toEqual([1, 2, 3]);
  });

  test("should return empty for n=0", () => {
    expect(nSmallest([1, 2, 3], 0)).toEqual([]);
  });

  test("should return all elements (sorted asc) if n >= length", () => {
    expect(nSmallest([3, 1, 2], 5)).toEqual([1, 2, 3]);
  });

  test("should handle n=1", () => {
    expect(nSmallest([10, 20, 5, 15], 1)).toEqual([5]);
  });

  test("should handle duplicates", () => {
    expect(nSmallest([5, 5, 3, 3, 1], 3)).toEqual([1, 3, 3]);
  });
});
