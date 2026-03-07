/**
 * Queue (FIFO - First In, First Out)
 *
 * COMPLEXITY:
 * | Operation           | Time        | Space       |
 * |---------------------|-------------|-------------|
 * | enqueue             | O(1)*       | O(1)        |
 * | dequeue             | O(1) amort  | O(1)        |
 * | peek/front          | O(1)        | O(1)        |
 * | isEmpty             | O(1)        | O(1)        |
 * | size                | O(1)        | O(1)        |
 * | Deque add/remove    | O(1)        | O(1)        |
 *
 * *amortized
 */

/**
 * A queue implementation using two internal arrays for amortized O(1) dequeue.
 */
export class Queue<T> {
  private _inbox: T[] = [];
  private _outbox: T[] = [];

  get size(): number {
    return this._inbox.length + this._outbox.length;
  }

  get isEmpty(): boolean {
    return this._inbox.length === 0 && this._outbox.length === 0;
  }

  enqueue(value: T): void {
    this._inbox.push(value);
  }

  dequeue(): T {
    if (this.isEmpty) {
      throw new Error("Dequeue from empty queue");
    }
    if (this._outbox.length === 0) {
      while (this._inbox.length > 0) {
        this._outbox.push(this._inbox.pop()!);
      }
    }
    return this._outbox.pop()!;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty queue");
    }
    if (this._outbox.length === 0) {
      while (this._inbox.length > 0) {
        this._outbox.push(this._inbox.pop()!);
      }
    }
    return this._outbox[this._outbox.length - 1];
  }

  clear(): void {
    this._inbox = [];
    this._outbox = [];
  }

  toArray(): T[] {
    const out = [...this._outbox].reverse();
    return out.concat(this._inbox);
  }

  toString(): string {
    return `Queue([${this.toArray().join(", ")}])`;
  }

  [Symbol.iterator](): Iterator<T> {
    const arr = this.toArray();
    let index = 0;
    return {
      next(): IteratorResult<T> {
        if (index < arr.length) {
          return { value: arr[index++], done: false };
        }
        return { value: undefined as unknown as T, done: true };
      },
    };
  }

  static fromArray<T>(items: T[]): Queue<T> {
    const queue = new Queue<T>();
    for (const item of items) {
      queue.enqueue(item);
    }
    return queue;
  }
}

/**
 * A fixed-size circular queue implementation using an array.
 */
export class CircularQueue<T> {
  private _data: (T | undefined)[];
  private _front: number = 0;
  private _rear: number = -1;
  private _size: number = 0;
  private _capacity: number;

  constructor(capacity: number) {
    if (capacity < 1) {
      throw new Error("Capacity must be at least 1");
    }
    this._capacity = capacity;
    this._data = new Array<T | undefined>(capacity).fill(undefined);
  }

  get size(): number {
    return this._size;
  }

  get capacity(): number {
    return this._capacity;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  get isFull(): boolean {
    return this._size === this._capacity;
  }

  enqueue(value: T): boolean {
    if (this.isFull) {
      return false;
    }
    this._rear = (this._rear + 1) % this._capacity;
    this._data[this._rear] = value;
    this._size++;
    return true;
  }

  dequeue(): T | undefined {
    if (this.isEmpty) {
      return undefined;
    }
    const value = this._data[this._front];
    this._data[this._front] = undefined;
    this._front = (this._front + 1) % this._capacity;
    this._size--;
    return value;
  }

  peek(): T | undefined {
    if (this.isEmpty) {
      return undefined;
    }
    return this._data[this._front];
  }

  rear(): T | undefined {
    if (this.isEmpty) {
      return undefined;
    }
    return this._data[this._rear];
  }

  toArray(): T[] {
    const result: T[] = [];
    if (this._size > 0) {
      let idx = this._front;
      for (let i = 0; i < this._size; i++) {
        result.push(this._data[idx] as T);
        idx = (idx + 1) % this._capacity;
      }
    }
    return result;
  }

  toString(): string {
    return `CircularQueue([${this.toArray().join(", ")}], capacity=${this._capacity})`;
  }
}

/**
 * A double-ended queue (deque) supporting O(1) operations at both ends.
 *
 * Implemented with a doubly linked list internally for true O(1) at both ends.
 */

interface DequeNode<T> {
  value: T;
  prev: DequeNode<T> | null;
  next: DequeNode<T> | null;
}

export class Deque<T> {
  private _head: DequeNode<T> | null = null;
  private _tail: DequeNode<T> | null = null;
  private _size: number = 0;

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  addFront(value: T): void {
    const node: DequeNode<T> = { value, prev: null, next: this._head };
    if (this._head !== null) {
      this._head.prev = node;
    }
    this._head = node;
    if (this._tail === null) {
      this._tail = node;
    }
    this._size++;
  }

  addRear(value: T): void {
    const node: DequeNode<T> = { value, prev: this._tail, next: null };
    if (this._tail !== null) {
      this._tail.next = node;
    }
    this._tail = node;
    if (this._head === null) {
      this._head = node;
    }
    this._size++;
  }

  removeFront(): T {
    if (this._head === null) {
      throw new Error("Remove from empty deque");
    }
    const value = this._head.value;
    this._head = this._head.next;
    if (this._head !== null) {
      this._head.prev = null;
    } else {
      this._tail = null;
    }
    this._size--;
    return value;
  }

  removeRear(): T {
    if (this._tail === null) {
      throw new Error("Remove from empty deque");
    }
    const value = this._tail.value;
    this._tail = this._tail.prev;
    if (this._tail !== null) {
      this._tail.next = null;
    } else {
      this._head = null;
    }
    this._size--;
    return value;
  }

  peekFront(): T {
    if (this._head === null) {
      throw new Error("Peek from empty deque");
    }
    return this._head.value;
  }

  peekRear(): T {
    if (this._tail === null) {
      throw new Error("Peek from empty deque");
    }
    return this._tail.value;
  }

  clear(): void {
    this._head = null;
    this._tail = null;
    this._size = 0;
  }

  toArray(): T[] {
    const result: T[] = [];
    let current = this._head;
    while (current !== null) {
      result.push(current.value);
      current = current.next;
    }
    return result;
  }

  toString(): string {
    return `Deque([${this.toArray().join(", ")}])`;
  }

  [Symbol.iterator](): Iterator<T> {
    let current = this._head;
    return {
      next(): IteratorResult<T> {
        if (current !== null) {
          const value = current.value;
          current = current.next;
          return { value, done: false };
        }
        return { value: undefined as unknown as T, done: true };
      },
    };
  }
}

/**
 * A monotonic deque that maintains elements in decreasing order.
 * Useful for sliding window maximum problems.
 */
export class MonotonicQueue<T> {
  private _data: Array<{ index: number; value: T }> = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  /**
   * Add element maintaining decreasing order.
   * Removes all elements smaller than the new value from the back.
   */
  push(index: number, value: T): void {
    while (
      this._data.length > 0 &&
      this._data[this._data.length - 1].value < value
    ) {
      this._data.pop();
    }
    this._data.push({ index, value });
  }

  /**
   * Remove element at index if it is at the front.
   */
  popFront(index: number): void {
    if (this._data.length > 0 && this._data[0].index === index) {
      this._data.shift();
    }
  }

  /**
   * Remove and return the maximum element (the front).
   */
  popMax(): T | undefined {
    if (this.isEmpty) {
      return undefined;
    }
    return this._data.shift()!.value;
  }

  /**
   * Get the maximum element (the front).
   */
  max(): T | undefined {
    if (this.isEmpty) {
      return undefined;
    }
    return this._data[0].value;
  }
}
