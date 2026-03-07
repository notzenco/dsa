import { describe, test, expect } from "bun:test";
import {
  Queue,
  CircularQueue,
  Deque,
  MonotonicQueue,
} from "../../src/data-structures/queue";

describe("Queue", () => {
  describe("basic operations", () => {
    test("new queue is empty", () => {
      const q = new Queue<number>();
      expect(q.isEmpty).toBe(true);
      expect(q.size).toBe(0);
    });

    test("enqueue increases size", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      expect(q.size).toBe(1);
      expect(q.isEmpty).toBe(false);
      q.enqueue(2);
      expect(q.size).toBe(2);
    });

    test("dequeue returns elements in FIFO order", () => {
      const q = new Queue<number>();
      q.enqueue(10);
      q.enqueue(20);
      q.enqueue(30);
      expect(q.dequeue()).toBe(10);
      expect(q.dequeue()).toBe(20);
      expect(q.dequeue()).toBe(30);
    });

    test("dequeue from empty queue throws", () => {
      const q = new Queue<number>();
      expect(() => q.dequeue()).toThrow("Dequeue from empty queue");
    });

    test("peek returns front without removing", () => {
      const q = new Queue<number>();
      q.enqueue(10);
      q.enqueue(20);
      expect(q.peek()).toBe(10);
      expect(q.size).toBe(2);
    });

    test("peek from empty queue throws", () => {
      const q = new Queue<number>();
      expect(() => q.peek()).toThrow("Peek from empty queue");
    });

    test("clear empties the queue", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      q.clear();
      expect(q.isEmpty).toBe(true);
      expect(q.size).toBe(0);
    });
  });

  describe("interleaved operations", () => {
    test("enqueue and dequeue interleaved", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      expect(q.dequeue()).toBe(1);
      q.enqueue(3);
      expect(q.dequeue()).toBe(2);
      expect(q.dequeue()).toBe(3);
      expect(q.isEmpty).toBe(true);
    });

    test("dequeue all then enqueue again", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.dequeue();
      q.enqueue(2);
      expect(q.peek()).toBe(2);
      expect(q.dequeue()).toBe(2);
    });
  });

  describe("toArray and toString", () => {
    test("toArray returns elements front to rear", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      expect(q.toArray()).toEqual([1, 2, 3]);
    });

    test("toArray after dequeue", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      q.dequeue();
      expect(q.toArray()).toEqual([2, 3]);
    });

    test("toArray on empty queue", () => {
      const q = new Queue<number>();
      expect(q.toArray()).toEqual([]);
    });

    test("toString shows queue contents", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      expect(q.toString()).toBe("Queue([1, 2])");
    });
  });

  describe("iterator", () => {
    test("iterates front to rear", () => {
      const q = new Queue<number>();
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      const values: number[] = [];
      for (const v of q) {
        values.push(v);
      }
      expect(values).toEqual([1, 2, 3]);
    });

    test("spread operator works", () => {
      const q = new Queue<number>();
      q.enqueue(10);
      q.enqueue(20);
      expect([...q]).toEqual([10, 20]);
    });
  });

  describe("fromArray", () => {
    test("creates queue from array", () => {
      const q = Queue.fromArray([1, 2, 3]);
      expect(q.size).toBe(3);
      expect(q.dequeue()).toBe(1);
      expect(q.dequeue()).toBe(2);
      expect(q.dequeue()).toBe(3);
    });

    test("creates empty queue from empty array", () => {
      const q = Queue.fromArray<number>([]);
      expect(q.isEmpty).toBe(true);
    });
  });

  describe("with different types", () => {
    test("string queue", () => {
      const q = new Queue<string>();
      q.enqueue("hello");
      q.enqueue("world");
      expect(q.dequeue()).toBe("hello");
      expect(q.dequeue()).toBe("world");
    });
  });

  describe("single element", () => {
    test("enqueue and dequeue single element", () => {
      const q = new Queue<number>();
      q.enqueue(42);
      expect(q.dequeue()).toBe(42);
      expect(q.isEmpty).toBe(true);
    });
  });

  describe("large number of operations", () => {
    test("many enqueue/dequeue cycles", () => {
      const q = new Queue<number>();
      for (let i = 0; i < 1000; i++) {
        q.enqueue(i);
      }
      for (let i = 0; i < 1000; i++) {
        expect(q.dequeue()).toBe(i);
      }
      expect(q.isEmpty).toBe(true);
    });
  });
});

describe("CircularQueue", () => {
  describe("constructor", () => {
    test("creates queue with given capacity", () => {
      const q = new CircularQueue<number>(5);
      expect(q.capacity).toBe(5);
      expect(q.isEmpty).toBe(true);
      expect(q.size).toBe(0);
    });

    test("throws for capacity less than 1", () => {
      expect(() => new CircularQueue<number>(0)).toThrow(
        "Capacity must be at least 1"
      );
      expect(() => new CircularQueue<number>(-1)).toThrow(
        "Capacity must be at least 1"
      );
    });
  });

  describe("enqueue and dequeue", () => {
    test("enqueue adds elements", () => {
      const q = new CircularQueue<number>(3);
      expect(q.enqueue(1)).toBe(true);
      expect(q.enqueue(2)).toBe(true);
      expect(q.enqueue(3)).toBe(true);
      expect(q.size).toBe(3);
    });

    test("enqueue returns false when full", () => {
      const q = new CircularQueue<number>(2);
      q.enqueue(1);
      q.enqueue(2);
      expect(q.enqueue(3)).toBe(false);
      expect(q.size).toBe(2);
    });

    test("dequeue returns elements in FIFO order", () => {
      const q = new CircularQueue<number>(3);
      q.enqueue(10);
      q.enqueue(20);
      q.enqueue(30);
      expect(q.dequeue()).toBe(10);
      expect(q.dequeue()).toBe(20);
      expect(q.dequeue()).toBe(30);
    });

    test("dequeue returns undefined when empty", () => {
      const q = new CircularQueue<number>(3);
      expect(q.dequeue()).toBeUndefined();
    });

    test("circular wrap-around behavior", () => {
      const q = new CircularQueue<number>(3);
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      q.dequeue(); // removes 1
      q.dequeue(); // removes 2
      expect(q.enqueue(4)).toBe(true); // wraps around
      expect(q.enqueue(5)).toBe(true); // wraps around
      expect(q.dequeue()).toBe(3);
      expect(q.dequeue()).toBe(4);
      expect(q.dequeue()).toBe(5);
    });
  });

  describe("peek and rear", () => {
    test("peek returns front element", () => {
      const q = new CircularQueue<number>(3);
      q.enqueue(10);
      q.enqueue(20);
      expect(q.peek()).toBe(10);
    });

    test("peek returns undefined when empty", () => {
      const q = new CircularQueue<number>(3);
      expect(q.peek()).toBeUndefined();
    });

    test("rear returns last element", () => {
      const q = new CircularQueue<number>(3);
      q.enqueue(10);
      q.enqueue(20);
      expect(q.rear()).toBe(20);
    });

    test("rear returns undefined when empty", () => {
      const q = new CircularQueue<number>(3);
      expect(q.rear()).toBeUndefined();
    });
  });

  describe("isFull", () => {
    test("isFull when at capacity", () => {
      const q = new CircularQueue<number>(2);
      expect(q.isFull).toBe(false);
      q.enqueue(1);
      expect(q.isFull).toBe(false);
      q.enqueue(2);
      expect(q.isFull).toBe(true);
    });

    test("not full after dequeue", () => {
      const q = new CircularQueue<number>(2);
      q.enqueue(1);
      q.enqueue(2);
      q.dequeue();
      expect(q.isFull).toBe(false);
    });
  });

  describe("capacity 1", () => {
    test("works with capacity 1", () => {
      const q = new CircularQueue<number>(1);
      expect(q.enqueue(42)).toBe(true);
      expect(q.isFull).toBe(true);
      expect(q.enqueue(99)).toBe(false);
      expect(q.dequeue()).toBe(42);
      expect(q.isEmpty).toBe(true);
      expect(q.enqueue(99)).toBe(true);
      expect(q.dequeue()).toBe(99);
    });
  });

  describe("toArray and toString", () => {
    test("toArray returns elements in order", () => {
      const q = new CircularQueue<number>(4);
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      expect(q.toArray()).toEqual([1, 2, 3]);
    });

    test("toArray after wrap-around", () => {
      const q = new CircularQueue<number>(3);
      q.enqueue(1);
      q.enqueue(2);
      q.enqueue(3);
      q.dequeue();
      q.enqueue(4);
      expect(q.toArray()).toEqual([2, 3, 4]);
    });

    test("toArray on empty", () => {
      const q = new CircularQueue<number>(3);
      expect(q.toArray()).toEqual([]);
    });

    test("toString", () => {
      const q = new CircularQueue<number>(5);
      q.enqueue(1);
      q.enqueue(2);
      expect(q.toString()).toBe("CircularQueue([1, 2], capacity=5)");
    });
  });
});

describe("Deque", () => {
  describe("basic operations", () => {
    test("new deque is empty", () => {
      const d = new Deque<number>();
      expect(d.isEmpty).toBe(true);
      expect(d.size).toBe(0);
    });

    test("addFront adds to front", () => {
      const d = new Deque<number>();
      d.addFront(1);
      d.addFront(2);
      d.addFront(3);
      expect(d.toArray()).toEqual([3, 2, 1]);
    });

    test("addRear adds to rear", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.toArray()).toEqual([1, 2, 3]);
    });

    test("removeFront removes from front", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.removeFront()).toBe(1);
      expect(d.removeFront()).toBe(2);
      expect(d.removeFront()).toBe(3);
    });

    test("removeRear removes from rear", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.removeRear()).toBe(3);
      expect(d.removeRear()).toBe(2);
      expect(d.removeRear()).toBe(1);
    });

    test("removeFront from empty throws", () => {
      const d = new Deque<number>();
      expect(() => d.removeFront()).toThrow("Remove from empty deque");
    });

    test("removeRear from empty throws", () => {
      const d = new Deque<number>();
      expect(() => d.removeRear()).toThrow("Remove from empty deque");
    });
  });

  describe("peek operations", () => {
    test("peekFront returns front element", () => {
      const d = new Deque<number>();
      d.addRear(10);
      d.addRear(20);
      expect(d.peekFront()).toBe(10);
      expect(d.size).toBe(2);
    });

    test("peekRear returns rear element", () => {
      const d = new Deque<number>();
      d.addRear(10);
      d.addRear(20);
      expect(d.peekRear()).toBe(20);
      expect(d.size).toBe(2);
    });

    test("peekFront from empty throws", () => {
      const d = new Deque<number>();
      expect(() => d.peekFront()).toThrow("Peek from empty deque");
    });

    test("peekRear from empty throws", () => {
      const d = new Deque<number>();
      expect(() => d.peekRear()).toThrow("Peek from empty deque");
    });
  });

  describe("mixed operations", () => {
    test("addFront and addRear interleaved", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addFront(2);
      d.addRear(3);
      d.addFront(4);
      expect(d.toArray()).toEqual([4, 2, 1, 3]);
    });

    test("removeFront and removeRear interleaved", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      d.addRear(4);
      expect(d.removeFront()).toBe(1);
      expect(d.removeRear()).toBe(4);
      expect(d.removeFront()).toBe(2);
      expect(d.removeRear()).toBe(3);
      expect(d.isEmpty).toBe(true);
    });

    test("use as stack (addRear/removeRear)", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.removeRear()).toBe(3);
      expect(d.removeRear()).toBe(2);
      expect(d.removeRear()).toBe(1);
    });

    test("use as queue (addRear/removeFront)", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.removeFront()).toBe(1);
      expect(d.removeFront()).toBe(2);
      expect(d.removeFront()).toBe(3);
    });
  });

  describe("clear", () => {
    test("clear empties the deque", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.clear();
      expect(d.isEmpty).toBe(true);
      expect(d.size).toBe(0);
    });
  });

  describe("toArray and toString", () => {
    test("toArray returns elements front to rear", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect(d.toArray()).toEqual([1, 2, 3]);
    });

    test("toArray on empty deque", () => {
      const d = new Deque<number>();
      expect(d.toArray()).toEqual([]);
    });

    test("toString", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      expect(d.toString()).toBe("Deque([1, 2])");
    });
  });

  describe("iterator", () => {
    test("iterates front to rear", () => {
      const d = new Deque<number>();
      d.addRear(1);
      d.addRear(2);
      d.addRear(3);
      expect([...d]).toEqual([1, 2, 3]);
    });
  });

  describe("single element", () => {
    test("add and remove single element from front", () => {
      const d = new Deque<number>();
      d.addFront(42);
      expect(d.peekFront()).toBe(42);
      expect(d.peekRear()).toBe(42);
      expect(d.removeFront()).toBe(42);
      expect(d.isEmpty).toBe(true);
    });

    test("add front and remove from rear", () => {
      const d = new Deque<number>();
      d.addFront(42);
      expect(d.removeRear()).toBe(42);
      expect(d.isEmpty).toBe(true);
    });
  });
});

describe("MonotonicQueue", () => {
  describe("basic operations", () => {
    test("new monotonic queue is empty", () => {
      const mq = new MonotonicQueue<number>();
      expect(mq.isEmpty).toBe(true);
      expect(mq.size).toBe(0);
    });

    test("push maintains decreasing order", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 5);
      mq.push(1, 3);
      mq.push(2, 4);
      // After push(2, 4): 3 is removed because 4 > 3, so we have [5, 4]
      expect(mq.max()).toBe(5);
    });

    test("push larger element removes smaller ones", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 1);
      mq.push(1, 2);
      mq.push(2, 3);
      // Each push removes the previous, leaving only [3]
      expect(mq.max()).toBe(3);
      expect(mq.size).toBe(1);
    });

    test("max returns the maximum (front) element", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 10);
      mq.push(1, 5);
      mq.push(2, 7);
      // 5 is removed by 7, queue is [10, 7]
      expect(mq.max()).toBe(10);
    });

    test("max returns undefined when empty", () => {
      const mq = new MonotonicQueue<number>();
      expect(mq.max()).toBeUndefined();
    });
  });

  describe("popFront", () => {
    test("removes front element by index", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 10);
      mq.push(1, 5);
      mq.popFront(0);
      expect(mq.max()).toBe(5);
    });

    test("does not remove if index does not match front", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 10);
      mq.push(1, 5);
      mq.popFront(1); // index 1 is not at front
      expect(mq.max()).toBe(10);
      expect(mq.size).toBe(2);
    });
  });

  describe("popMax", () => {
    test("removes and returns the maximum", () => {
      const mq = new MonotonicQueue<number>();
      mq.push(0, 10);
      mq.push(1, 5);
      mq.push(2, 3);
      expect(mq.popMax()).toBe(10);
      expect(mq.max()).toBe(5);
    });

    test("popMax returns undefined when empty", () => {
      const mq = new MonotonicQueue<number>();
      expect(mq.popMax()).toBeUndefined();
    });
  });

  describe("sliding window maximum pattern", () => {
    test("computes sliding window max", () => {
      const nums = [1, 3, -1, -3, 5, 3, 6, 7];
      const k = 3;
      const result: number[] = [];
      const mq = new MonotonicQueue<number>();

      for (let i = 0; i < nums.length; i++) {
        mq.push(i, nums[i]);
        if (i >= k) {
          mq.popFront(i - k);
        }
        if (i >= k - 1) {
          result.push(mq.max()!);
        }
      }

      expect(result).toEqual([3, 3, 5, 5, 6, 7]);
    });

    test("window of size 1", () => {
      const nums = [1, 3, 2];
      const k = 1;
      const result: number[] = [];
      const mq = new MonotonicQueue<number>();

      for (let i = 0; i < nums.length; i++) {
        mq.push(i, nums[i]);
        if (i >= k) {
          mq.popFront(i - k);
        }
        if (i >= k - 1) {
          result.push(mq.max()!);
        }
      }

      expect(result).toEqual([1, 3, 2]);
    });
  });
});
