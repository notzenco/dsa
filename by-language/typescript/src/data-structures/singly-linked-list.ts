/**
 * Singly Linked List
 *
 * COMPLEXITY:
 * | Operation           | Average     | Worst       | Space       |
 * |---------------------|-------------|-------------|-------------|
 * | Access by index     | O(n)        | O(n)        | O(1)        |
 * | Search              | O(n)        | O(n)        | O(1)        |
 * | Insert at head      | O(1)        | O(1)        | O(1)        |
 * | Insert at tail      | O(1)*       | O(1)*       | O(1)        |
 * | Insert at index     | O(n)        | O(n)        | O(1)        |
 * | Delete at head      | O(1)        | O(1)        | O(1)        |
 * | Delete at tail      | O(n)        | O(n)        | O(1)        |
 * | Delete at index     | O(n)        | O(n)        | O(1)        |
 *
 * *O(1) if tail pointer is maintained (as in this implementation)
 */

export class Node<T> {
  data: T;
  next: Node<T> | null;

  constructor(data: T, next: Node<T> | null = null) {
    this.data = data;
    this.next = next;
  }

  toString(): string {
    return `Node(${this.data})`;
  }
}

export class SinglyLinkedList<T> {
  private _head: Node<T> | null = null;
  private _tail: Node<T> | null = null;
  private _size: number = 0;

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  get head(): T | null {
    return this._head !== null ? this._head.data : null;
  }

  get tail(): T | null {
    return this._tail !== null ? this._tail.data : null;
  }

  private _getNode(index: number): Node<T> {
    if (index < 0) {
      index = this._size + index;
    }
    if (index < 0 || index >= this._size) {
      throw new RangeError("Index out of bounds");
    }
    let current = this._head!;
    for (let i = 0; i < index; i++) {
      current = current.next!;
    }
    return current;
  }

  prepend(value: T): void {
    const newNode = new Node(value, this._head);
    this._head = newNode;
    if (this._tail === null) {
      this._tail = newNode;
    }
    this._size++;
  }

  append(value: T): void {
    const newNode = new Node(value);
    if (this._tail === null) {
      this._head = newNode;
      this._tail = newNode;
    } else {
      this._tail.next = newNode;
      this._tail = newNode;
    }
    this._size++;
  }

  insertAt(index: number, value: T): void {
    if (index < 0) {
      index = this._size + index + 1;
    }
    if (index < 0 || index > this._size) {
      throw new RangeError("Index out of bounds");
    }
    if (index === 0) {
      this.prepend(value);
    } else if (index === this._size) {
      this.append(value);
    } else {
      const prev = this._getNode(index - 1);
      const newNode = new Node(value, prev.next);
      prev.next = newNode;
      this._size++;
    }
  }

  popFront(): T {
    if (this._head === null) {
      throw new Error("Pop from empty list");
    }
    const value = this._head.data;
    this._head = this._head.next;
    if (this._head === null) {
      this._tail = null;
    }
    this._size--;
    return value;
  }

  popBack(): T {
    if (this._tail === null) {
      throw new Error("Pop from empty list");
    }
    if (this._head === this._tail) {
      const value = this._head!.data;
      this._head = null;
      this._tail = null;
      this._size--;
      return value;
    }
    let current = this._head!;
    while (current.next !== this._tail) {
      current = current.next!;
    }
    const value = this._tail.data;
    current.next = null;
    this._tail = current;
    this._size--;
    return value;
  }

  removeValue(value: T): boolean {
    if (this._head === null) {
      return false;
    }
    if (this._head.data === value) {
      this.popFront();
      return true;
    }
    let current = this._head;
    while (current.next !== null) {
      if (current.next.data === value) {
        if (current.next === this._tail) {
          this._tail = current;
        }
        current.next = current.next.next;
        this._size--;
        return true;
      }
      current = current.next;
    }
    return false;
  }

  find(value: T): number {
    let current = this._head;
    let index = 0;
    while (current !== null) {
      if (current.data === value) {
        return index;
      }
      current = current.next;
      index++;
    }
    return -1;
  }

  get(index: number): T {
    return this._getNode(index).data;
  }

  reverse(): void {
    if (this._size <= 1) {
      return;
    }
    this._tail = this._head;
    let prev: Node<T> | null = null;
    let current = this._head;
    while (current !== null) {
      const nextNode = current.next;
      current.next = prev;
      prev = current;
      current = nextNode;
    }
    this._head = prev;
  }

  getMiddle(): T | null {
    if (this._head === null) {
      return null;
    }
    let slow: Node<T> = this._head;
    let fast: Node<T> = this._head;
    while (fast.next !== null && fast.next.next !== null) {
      slow = slow.next!;
      fast = fast.next.next;
    }
    return slow.data;
  }

  hasCycle(): boolean {
    if (this._head === null) {
      return false;
    }
    let slow: Node<T> | null = this._head;
    let fast: Node<T> | null = this._head;
    while (fast !== null && fast.next !== null) {
      slow = slow!.next;
      fast = fast.next.next;
      if (slow === fast) {
        return true;
      }
    }
    return false;
  }

  getNthFromEnd(n: number): T | null {
    if (n <= 0 || n > this._size) {
      return null;
    }
    let slow = this._head;
    let fast = this._head;
    for (let i = 0; i < n; i++) {
      if (fast === null) {
        return null;
      }
      fast = fast.next;
    }
    while (fast !== null) {
      slow = slow!.next;
      fast = fast.next;
    }
    return slow !== null ? slow.data : null;
  }

  removeDuplicates(): void {
    if (this._head === null) {
      return;
    }
    const seen = new Set<T>();
    let current: Node<T> | null = this._head;
    seen.add(current.data);
    while (current.next !== null) {
      if (seen.has(current.next.data)) {
        if (current.next === this._tail) {
          this._tail = current;
        }
        current.next = current.next.next;
        this._size--;
      } else {
        seen.add(current.next.data);
        current = current.next;
      }
    }
  }

  toArray(): T[] {
    const result: T[] = [];
    let current = this._head;
    while (current !== null) {
      result.push(current.data);
      current = current.next;
    }
    return result;
  }

  clear(): void {
    this._head = null;
    this._tail = null;
    this._size = 0;
  }

  map<U>(fn: (value: T) => U): SinglyLinkedList<U> {
    const result = new SinglyLinkedList<U>();
    let current = this._head;
    while (current !== null) {
      result.append(fn(current.data));
      current = current.next;
    }
    return result;
  }

  filter(predicate: (value: T) => boolean): SinglyLinkedList<T> {
    const result = new SinglyLinkedList<T>();
    let current = this._head;
    while (current !== null) {
      if (predicate(current.data)) {
        result.append(current.data);
      }
      current = current.next;
    }
    return result;
  }

  toString(): string {
    const elements = this.toArray().join(" -> ");
    return `SinglyLinkedList([${elements}])`;
  }

  [Symbol.iterator](): Iterator<T> {
    let current = this._head;
    return {
      next(): IteratorResult<T> {
        if (current !== null) {
          const value = current.data;
          current = current.next;
          return { value, done: false };
        }
        return { value: undefined as unknown as T, done: true };
      },
    };
  }

  static fromArray<T>(items: T[]): SinglyLinkedList<T> {
    const list = new SinglyLinkedList<T>();
    for (const item of items) {
      list.append(item);
    }
    return list;
  }

  /**
   * Expose the internal head node for cycle testing.
   * Not intended for normal usage.
   */
  _getHeadNode(): Node<T> | null {
    return this._head;
  }

  _getTailNode(): Node<T> | null {
    return this._tail;
  }
}
