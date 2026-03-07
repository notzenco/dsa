/**
 * Doubly Linked List
 *
 * COMPLEXITY:
 * | Operation           | Average     | Worst       | Space       |
 * |---------------------|-------------|-------------|-------------|
 * | Access by index     | O(n)        | O(n)        | O(1)        |
 * | Search              | O(n)        | O(n)        | O(1)        |
 * | Insert at head      | O(1)        | O(1)        | O(1)        |
 * | Insert at tail      | O(1)        | O(1)        | O(1)        |
 * | Insert at index     | O(n)        | O(n)        | O(1)        |
 * | Delete at head      | O(1)        | O(1)        | O(1)        |
 * | Delete at tail      | O(1)        | O(1)        | O(1)        |
 * | Delete at index     | O(n)        | O(n)        | O(1)        |
 * | Delete given node   | O(1)        | O(1)        | O(1)        |
 */

export class DNode<T> {
  constructor(
    public data: T,
    public prev: DNode<T> | null = null,
    public next: DNode<T> | null = null,
  ) {}

  toString(): string {
    return `DNode(${this.data})`;
  }
}

export class DoublyLinkedList<T> {
  private _head: DNode<T> | null = null;
  private _tail: DNode<T> | null = null;
  private _size: number = 0;

  get head(): T | null {
    return this._head ? this._head.data : null;
  }

  get tail(): T | null {
    return this._tail ? this._tail.data : null;
  }

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  private _getNode(index: number): DNode<T> {
    if (index < 0) index = this._size + index;
    if (index < 0 || index >= this._size) throw new RangeError("Index out of bounds");

    let curr: DNode<T>;
    if (index <= this._size / 2) {
      curr = this._head!;
      for (let i = 0; i < index; i++) curr = curr.next!;
    } else {
      curr = this._tail!;
      for (let i = this._size - 1; i > index; i--) curr = curr.prev!;
    }
    return curr;
  }

  prepend(value: T): DNode<T> {
    const node = new DNode(value, null, this._head);
    if (this._head) this._head.prev = node;
    else this._tail = node;
    this._head = node;
    this._size++;
    return node;
  }

  append(value: T): DNode<T> {
    const node = new DNode(value, this._tail, null);
    if (this._tail) this._tail.next = node;
    else this._head = node;
    this._tail = node;
    this._size++;
    return node;
  }

  insertAt(index: number, value: T): void {
    if (index < 0) index = this._size + index + 1;
    if (index < 0 || index > this._size) throw new RangeError("Index out of bounds");
    if (index === 0) { this.prepend(value); return; }
    if (index === this._size) { this.append(value); return; }
    const nextNode = this._getNode(index);
    const prevNode = nextNode.prev!;
    const node = new DNode(value, prevNode, nextNode);
    prevNode.next = node;
    nextNode.prev = node;
    this._size++;
  }

  popFront(): T {
    if (!this._head) throw new Error("Pop from empty list");
    const value = this._head.data;
    this._head = this._head.next;
    if (this._head) this._head.prev = null;
    else this._tail = null;
    this._size--;
    return value;
  }

  popBack(): T {
    if (!this._tail) throw new Error("Pop from empty list");
    const value = this._tail.data;
    this._tail = this._tail.prev;
    if (this._tail) this._tail.next = null;
    else this._head = null;
    this._size--;
    return value;
  }

  removeNode(node: DNode<T>): T {
    if (node.prev) node.prev.next = node.next;
    else this._head = node.next;
    if (node.next) node.next.prev = node.prev;
    else this._tail = node.prev;
    node.prev = null;
    node.next = null;
    this._size--;
    return node.data;
  }

  removeValue(value: T): boolean {
    let curr = this._head;
    while (curr) {
      if (curr.data === value) {
        this.removeNode(curr);
        return true;
      }
      curr = curr.next;
    }
    return false;
  }

  find(value: T): number {
    let curr = this._head;
    let index = 0;
    while (curr) {
      if (curr.data === value) return index;
      curr = curr.next;
      index++;
    }
    return -1;
  }

  findNode(value: T): DNode<T> | null {
    let curr = this._head;
    while (curr) {
      if (curr.data === value) return curr;
      curr = curr.next;
    }
    return null;
  }

  get(index: number): T {
    return this._getNode(index).data;
  }

  set(index: number, value: T): void {
    this._getNode(index).data = value;
  }

  reverse(): void {
    let curr = this._head;
    while (curr) {
      const tmp = curr.next;
      curr.next = curr.prev;
      curr.prev = tmp;
      curr = tmp;
    }
    const tmp = this._head;
    this._head = this._tail;
    this._tail = tmp;
  }

  moveToFront(node: DNode<T>): void {
    if (node === this._head) return;
    // Remove from current position
    if (node.prev) node.prev.next = node.next;
    if (node.next) node.next.prev = node.prev;
    else this._tail = node.prev;
    // Insert at front
    node.prev = null;
    node.next = this._head;
    if (this._head) this._head.prev = node;
    else this._tail = node;
    this._head = node;
  }

  moveToBack(node: DNode<T>): void {
    if (node === this._tail) return;
    // Remove from current position
    if (node.next) node.next.prev = node.prev;
    if (node.prev) node.prev.next = node.next;
    else this._head = node.next;
    // Insert at back
    node.next = null;
    node.prev = this._tail;
    if (this._tail) this._tail.next = node;
    else this._head = node;
    this._tail = node;
  }

  toArray(): T[] {
    const result: T[] = [];
    let curr = this._head;
    while (curr) {
      result.push(curr.data);
      curr = curr.next;
    }
    return result;
  }

  toArrayReverse(): T[] {
    const result: T[] = [];
    let curr = this._tail;
    while (curr) {
      result.push(curr.data);
      curr = curr.prev;
    }
    return result;
  }

  clear(): void {
    this._head = null;
    this._tail = null;
    this._size = 0;
  }

  toString(): string {
    const elements = this.toArray().join(" <-> ");
    return `DoublyLinkedList([${elements}])`;
  }

  [Symbol.iterator](): Iterator<T> {
    let curr = this._head;
    return {
      next(): IteratorResult<T> {
        if (curr) {
          const value = curr.data;
          curr = curr.next;
          return { value, done: false };
        }
        return { value: undefined as unknown as T, done: true };
      }
    };
  }

  static fromArray<T>(items: T[]): DoublyLinkedList<T> {
    const list = new DoublyLinkedList<T>();
    for (const item of items) {
      list.append(item);
    }
    return list;
  }
}
