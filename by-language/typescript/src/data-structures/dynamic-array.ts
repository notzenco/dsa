/**
 * Dynamic Array (Resizable Array / ArrayList)
 *
 * COMPLEXITY:
 * | Operation           | Average     | Worst       | Space       |
 * |---------------------|-------------|-------------|-------------|
 * | Access by index     | O(1)        | O(1)        | O(1)        |
 * | Search              | O(n)        | O(n)        | O(1)        |
 * | Insert at end       | O(1)*       | O(n)        | O(1)        |
 * | Insert at index     | O(n)        | O(n)        | O(1)        |
 * | Delete at end       | O(1)        | O(1)        | O(1)        |
 * | Delete at index     | O(n)        | O(n)        | O(1)        |
 *
 * *Amortized O(1) due to occasional resizing
 */

export const DEFAULT_CAPACITY = 8;
const GROWTH_FACTOR = 2;
const SHRINK_THRESHOLD = 0.25;

export class DynamicArray<T> {
  private _data: (T | undefined)[];
  private _size: number = 0;
  private _capacity: number;

  constructor(capacity: number = DEFAULT_CAPACITY) {
    if (capacity < 1) {
      throw new Error("Capacity must be at least 1");
    }
    this._capacity = capacity;
    this._data = new Array(this._capacity);
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

  push(value: T): void {
    if (this._size === this._capacity) {
      this._resize(this._capacity * GROWTH_FACTOR);
    }
    this._data[this._size++] = value;
  }

  pop(): T {
    if (this._size === 0) {
      throw new Error("Pop from empty array");
    }
    const value = this._data[--this._size] as T;
    this._data[this._size] = undefined;
    if (this._size > 0 && this._size <= this._capacity * SHRINK_THRESHOLD) {
      this._resize(Math.max(Math.floor(this._capacity / GROWTH_FACTOR), DEFAULT_CAPACITY));
    }
    return value;
  }

  get(index: number): T {
    if (index < 0) index = this._size + index;
    if (index < 0 || index >= this._size) {
      throw new RangeError("Index out of bounds");
    }
    return this._data[index] as T;
  }

  set(index: number, value: T): void {
    if (index < 0) index = this._size + index;
    if (index < 0 || index >= this._size) {
      throw new RangeError("Index out of bounds");
    }
    this._data[index] = value;
  }

  insertAt(index: number, value: T): void {
    if (index < 0) index = this._size + index + 1;
    if (index < 0 || index > this._size) {
      throw new RangeError("Index out of bounds");
    }
    if (this._size === this._capacity) {
      this._resize(this._capacity * GROWTH_FACTOR);
    }
    for (let i = this._size; i > index; i--) {
      this._data[i] = this._data[i - 1];
    }
    this._data[index] = value;
    this._size++;
  }

  removeAt(index: number): T {
    if (index < 0) index = this._size + index;
    if (index < 0 || index >= this._size) {
      throw new RangeError("Index out of bounds");
    }
    const value = this._data[index] as T;
    for (let i = index; i < this._size - 1; i++) {
      this._data[i] = this._data[i + 1];
    }
    this._data[--this._size] = undefined;
    if (this._size > 0 && this._size <= this._capacity * SHRINK_THRESHOLD) {
      this._resize(Math.max(Math.floor(this._capacity / GROWTH_FACTOR), DEFAULT_CAPACITY));
    }
    return value;
  }

  indexOf(value: T): number {
    for (let i = 0; i < this._size; i++) {
      if (this._data[i] === value) return i;
    }
    return -1;
  }

  contains(value: T): boolean {
    return this.indexOf(value) !== -1;
  }

  clear(): void {
    this._data = new Array(DEFAULT_CAPACITY);
    this._capacity = DEFAULT_CAPACITY;
    this._size = 0;
  }

  toArray(): T[] {
    return this._data.slice(0, this._size) as T[];
  }

  toString(): string {
    return `DynamicArray([${this.toArray().join(", ")}])`;
  }

  private _resize(newCapacity: number): void {
    const newData = new Array(newCapacity);
    for (let i = 0; i < this._size; i++) {
      newData[i] = this._data[i];
    }
    this._data = newData;
    this._capacity = newCapacity;
  }

  [Symbol.iterator](): Iterator<T> {
    let index = 0;
    const data = this._data;
    const size = this._size;
    return {
      next(): IteratorResult<T> {
        if (index < size) {
          return { value: data[index++] as T, done: false };
        }
        return { value: undefined as unknown as T, done: true };
      }
    };
  }

  static fromArray<T>(items: T[]): DynamicArray<T> {
    const capacity = Math.max(DEFAULT_CAPACITY, items.length);
    const arr = new DynamicArray<T>(capacity);
    for (const item of items) {
      arr.push(item);
    }
    return arr;
  }
}
