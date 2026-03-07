/**
 * Monotonic Queue Implementation.
 *
 * A data structure that maintains elements in monotonic (non-increasing or
 * non-decreasing) order using a deque, supporting efficient max/min queries.
 *
 * COMPLEXITY:
 * | Operation         | Time (amortized) |
 * |-------------------|------------------|
 * | push              | O(1)             |
 * | popFront          | O(1)             |
 * | popMax / popMin   | O(1)             |
 * | max / min         | O(1)             |
 * | size              | O(1)             |
 *
 * Space: O(n)
 */

/**
 * A double-ended queue (deque) used internally by MonotonicQueue.
 */
class Deque<T> {
  private _data: T[] = [];
  private _head = 0;

  get size(): number {
    return this._data.length - this._head;
  }

  get isEmpty(): boolean {
    return this.size === 0;
  }

  pushBack(value: T): void {
    this._data.push(value);
  }

  popFront(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty deque");
    }
    const value = this._data[this._head];
    this._head++;
    // Compact if too much wasted space
    if (this._head > this._data.length / 2 && this._head > 64) {
      this._data = this._data.slice(this._head);
      this._head = 0;
    }
    return value;
  }

  popBack(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty deque");
    }
    return this._data.pop()!;
  }

  peekFront(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty deque");
    }
    return this._data[this._head];
  }

  peekBack(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty deque");
    }
    return this._data[this._data.length - 1];
  }
}

/**
 * Monotonic Queue that supports O(1) max and min queries.
 *
 * Maintains two internal deques:
 * - A max-deque in non-increasing order for O(1) max queries
 * - A min-deque in non-decreasing order for O(1) min queries
 *
 * Also maintains the actual elements in insertion order for popFront.
 *
 * The comparator determines ordering. Default is numeric comparison.
 */
export class MonotonicQueue<T> {
  private _queue: Deque<T> = new Deque();
  private _maxDeque: Deque<T> = new Deque(); // non-increasing
  private _minDeque: Deque<T> = new Deque(); // non-decreasing
  private _compare: (a: T, b: T) => number;
  private _size = 0;

  /**
   * @param compare - Comparator function. Returns negative if a < b, 0 if equal, positive if a > b.
   *                  Default works for numbers.
   */
  constructor(compare?: (a: T, b: T) => number) {
    this._compare =
      compare ?? ((a: T, b: T) => (a as number) - (b as number));
  }

  /**
   * Push a value onto the back of the queue.
   * Maintains monotonic invariants on max and min deques.
   * Amortized O(1).
   */
  push(value: T): void {
    this._queue.pushBack(value);

    // Maintain max-deque (non-increasing): remove smaller elements from back
    while (
      !this._maxDeque.isEmpty &&
      this._compare(this._maxDeque.peekBack(), value) < 0
    ) {
      this._maxDeque.popBack();
    }
    this._maxDeque.pushBack(value);

    // Maintain min-deque (non-decreasing): remove larger elements from back
    while (
      !this._minDeque.isEmpty &&
      this._compare(this._minDeque.peekBack(), value) > 0
    ) {
      this._minDeque.popBack();
    }
    this._minDeque.pushBack(value);

    this._size++;
  }

  /**
   * Remove and return the front element of the queue.
   * Also removes from max/min deques if it was the front there.
   * O(1).
   */
  popFront(): T {
    if (this._size === 0) {
      throw new Error("Pop from empty queue");
    }

    const value = this._queue.popFront();

    // If the front of max-deque matches, remove it
    if (
      !this._maxDeque.isEmpty &&
      this._compare(this._maxDeque.peekFront(), value) === 0
    ) {
      this._maxDeque.popFront();
    }

    // If the front of min-deque matches, remove it
    if (
      !this._minDeque.isEmpty &&
      this._compare(this._minDeque.peekFront(), value) === 0
    ) {
      this._minDeque.popFront();
    }

    this._size--;
    return value;
  }

  /**
   * Return the current maximum element (front of max-deque).
   * O(1).
   */
  popMax(): T {
    if (this._size === 0) {
      throw new Error("PopMax from empty queue");
    }
    return this._maxDeque.peekFront();
  }

  /**
   * Return the current minimum element (front of min-deque).
   * O(1).
   */
  popMin(): T {
    if (this._size === 0) {
      throw new Error("PopMin from empty queue");
    }
    return this._minDeque.peekFront();
  }

  /**
   * Get the current maximum without removing it.
   * O(1).
   */
  get max(): T {
    if (this._size === 0) {
      throw new Error("Max from empty queue");
    }
    return this._maxDeque.peekFront();
  }

  /**
   * Get the current minimum without removing it.
   * O(1).
   */
  get min(): T {
    if (this._size === 0) {
      throw new Error("Min from empty queue");
    }
    return this._minDeque.peekFront();
  }

  /** Return the number of elements in the queue. */
  get size(): number {
    return this._size;
  }

  /** Check if the queue is empty. */
  get isEmpty(): boolean {
    return this._size === 0;
  }
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Sliding window maximum.
 * Returns an array of maximum values for each window of size k.
 *
 * LeetCode #239
 *
 * @param nums - Input array
 * @param k - Window size
 * @returns Array of maximums for each window position
 *
 * Time: O(n), Space: O(k)
 */
export function slidingWindowMaximum(nums: number[], k: number): number[] {
  if (nums.length === 0 || k <= 0) {
    return [];
  }

  if (k === 1) {
    return [...nums];
  }

  const result: number[] = [];
  // Deque stores indices, maintains non-increasing order of values
  const deque: number[] = [];
  let head = 0;

  for (let i = 0; i < nums.length; i++) {
    // Remove elements outside the window from front
    while (head < deque.length && deque[head] <= i - k) {
      head++;
    }

    // Remove elements smaller than current from back
    while (head < deque.length && nums[deque[deque.length - 1]] <= nums[i]) {
      deque.pop();
    }

    deque.push(i);

    // Window is fully formed when i >= k - 1
    if (i >= k - 1) {
      result.push(nums[deque[head]]);
    }
  }

  return result;
}
