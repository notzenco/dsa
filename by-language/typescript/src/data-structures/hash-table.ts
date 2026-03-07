/**
 * Hash Table (Hash Map)
 *
 * COMPLEXITY:
 * | Operation           | Average     | Worst       | Space       |
 * |---------------------|-------------|-------------|-------------|
 * | Insert              | O(1)        | O(n)        | O(n)        |
 * | Delete              | O(1)        | O(n)        | O(1)        |
 * | Search              | O(1)        | O(n)        | O(1)        |
 * | Resize              | O(n)        | O(n)        | O(n)        |
 */

interface HashEntry<K, V> {
  key: K;
  value: V;
}

function hashCode(key: unknown): number {
  if (key === null || key === undefined) return 0;
  if (typeof key === "number") {
    // Handle negative numbers and floats
    const n = key | 0;
    return n < 0 ? ~n + 1 : n;
  }
  if (typeof key === "boolean") return key ? 1 : 0;
  const str = String(key);
  let hash = 0;
  for (let i = 0; i < str.length; i++) {
    hash = ((hash << 5) - hash + str.charCodeAt(i)) | 0;
  }
  return hash < 0 ? ~hash + 1 : hash;
}

export class HashTableChaining<K, V> {
  static readonly DEFAULT_CAPACITY = 16;
  static readonly LOAD_FACTOR = 0.75;

  private _capacity: number;
  private _size: number;
  private _buckets: HashEntry<K, V>[][];

  constructor(capacity: number = HashTableChaining.DEFAULT_CAPACITY) {
    this._capacity = Math.max(1, capacity);
    this._size = 0;
    this._buckets = Array.from({ length: this._capacity }, () => []);
  }

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  get capacity(): number {
    return this._capacity;
  }

  get loadFactor(): number {
    return this._size / this._capacity;
  }

  private _hash(key: K): number {
    return hashCode(key) % this._capacity;
  }

  private _resize(newCapacity: number): void {
    const oldBuckets = this._buckets;
    this._capacity = newCapacity;
    this._buckets = Array.from({ length: this._capacity }, () => []);
    this._size = 0;

    for (const bucket of oldBuckets) {
      for (const entry of bucket) {
        this.put(entry.key, entry.value);
      }
    }
  }

  put(key: K, value: V): void {
    if (this.loadFactor >= HashTableChaining.LOAD_FACTOR) {
      this._resize(this._capacity * 2);
    }

    const index = this._hash(key);
    const bucket = this._buckets[index];

    for (const entry of bucket) {
      if (entry.key === key) {
        entry.value = value;
        return;
      }
    }

    bucket.push({ key, value });
    this._size++;
  }

  get(key: K): V | undefined {
    const index = this._hash(key);
    const bucket = this._buckets[index];

    for (const entry of bucket) {
      if (entry.key === key) {
        return entry.value;
      }
    }

    return undefined;
  }

  delete(key: K): boolean {
    const index = this._hash(key);
    const bucket = this._buckets[index];

    for (let i = 0; i < bucket.length; i++) {
      if (bucket[i].key === key) {
        bucket.splice(i, 1);
        this._size--;
        return true;
      }
    }

    return false;
  }

  has(key: K): boolean {
    const index = this._hash(key);
    const bucket = this._buckets[index];

    for (const entry of bucket) {
      if (entry.key === key) {
        return true;
      }
    }

    return false;
  }

  clear(): void {
    this._buckets = Array.from({ length: this._capacity }, () => []);
    this._size = 0;
  }

  keys(): K[] {
    const result: K[] = [];
    for (const bucket of this._buckets) {
      for (const entry of bucket) {
        result.push(entry.key);
      }
    }
    return result;
  }

  values(): V[] {
    const result: V[] = [];
    for (const bucket of this._buckets) {
      for (const entry of bucket) {
        result.push(entry.value);
      }
    }
    return result;
  }

  entries(): [K, V][] {
    const result: [K, V][] = [];
    for (const bucket of this._buckets) {
      for (const entry of bucket) {
        result.push([entry.key, entry.value]);
      }
    }
    return result;
  }
}

const EMPTY = Symbol("EMPTY");
const DELETED = Symbol("DELETED");

export class HashTableOpenAddressing<K, V> {
  static readonly DEFAULT_CAPACITY = 16;
  static readonly LOAD_FACTOR = 0.5;

  private _capacity: number;
  private _size: number;
  private _keys: (K | typeof EMPTY | typeof DELETED)[];
  private _values: (V | typeof EMPTY)[];

  constructor(capacity: number = HashTableOpenAddressing.DEFAULT_CAPACITY) {
    this._capacity = Math.max(1, capacity);
    this._size = 0;
    this._keys = new Array(this._capacity).fill(EMPTY);
    this._values = new Array(this._capacity).fill(EMPTY);
  }

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  get capacity(): number {
    return this._capacity;
  }

  get loadFactor(): number {
    return this._size / this._capacity;
  }

  private _hash(key: K): number {
    return hashCode(key) % this._capacity;
  }

  private _probe(index: number, i: number): number {
    return (index + i) % this._capacity;
  }

  private _findSlot(key: K): number | null {
    const index = this._hash(key);
    let firstDeleted: number | null = null;

    for (let i = 0; i < this._capacity; i++) {
      const probeIndex = this._probe(index, i);
      const currentKey = this._keys[probeIndex];

      if (currentKey === EMPTY) {
        return firstDeleted !== null ? firstDeleted : probeIndex;
      }

      if (currentKey === DELETED) {
        if (firstDeleted === null) {
          firstDeleted = probeIndex;
        }
      } else if (currentKey === key) {
        return probeIndex;
      }
    }

    return firstDeleted;
  }

  private _resize(newCapacity: number): void {
    const oldKeys = this._keys;
    const oldValues = this._values;

    this._capacity = newCapacity;
    this._keys = new Array(this._capacity).fill(EMPTY);
    this._values = new Array(this._capacity).fill(EMPTY);
    this._size = 0;

    for (let i = 0; i < oldKeys.length; i++) {
      const key = oldKeys[i];
      if (key !== EMPTY && key !== DELETED) {
        this.put(key as K, oldValues[i] as V);
      }
    }
  }

  put(key: K, value: V): void {
    if (this.loadFactor >= HashTableOpenAddressing.LOAD_FACTOR) {
      this._resize(this._capacity * 2);
    }

    let index = this._findSlot(key);

    if (index === null) {
      this._resize(this._capacity * 2);
      index = this._findSlot(key);
    }

    if (this._keys[index!] === EMPTY || this._keys[index!] === DELETED) {
      this._size++;
    }

    this._keys[index!] = key;
    this._values[index!] = value;
  }

  get(key: K): V | undefined {
    const index = this._findSlot(key);

    if (index === null) return undefined;

    const currentKey = this._keys[index];
    if (currentKey === EMPTY || currentKey === DELETED) return undefined;
    if (currentKey === key) return this._values[index] as V;

    return undefined;
  }

  delete(key: K): boolean {
    const index = this._hash(key);

    for (let i = 0; i < this._capacity; i++) {
      const probeIndex = this._probe(index, i);
      const currentKey = this._keys[probeIndex];

      if (currentKey === EMPTY) return false;

      if (currentKey === key) {
        this._keys[probeIndex] = DELETED;
        this._values[probeIndex] = EMPTY;
        this._size--;
        return true;
      }
    }

    return false;
  }

  has(key: K): boolean {
    const index = this._findSlot(key);
    if (index === null) return false;
    const currentKey = this._keys[index];
    return currentKey !== EMPTY && currentKey !== DELETED && currentKey === key;
  }

  keys(): K[] {
    const result: K[] = [];
    for (const key of this._keys) {
      if (key !== EMPTY && key !== DELETED) {
        result.push(key as K);
      }
    }
    return result;
  }

  values(): V[] {
    const result: V[] = [];
    for (let i = 0; i < this._keys.length; i++) {
      if (this._keys[i] !== EMPTY && this._keys[i] !== DELETED) {
        result.push(this._values[i] as V);
      }
    }
    return result;
  }
}

export class HashSet<T> {
  private _table: HashTableChaining<T, true>;

  constructor() {
    this._table = new HashTableChaining<T, true>();
  }

  add(value: T): void {
    this._table.put(value, true);
  }

  delete(value: T): boolean {
    return this._table.delete(value);
  }

  has(value: T): boolean {
    return this._table.has(value);
  }

  get size(): number {
    return this._table.size;
  }

  get isEmpty(): boolean {
    return this._table.isEmpty;
  }

  clear(): void {
    this._table.clear();
  }

  toArray(): T[] {
    return this._table.keys();
  }

  union(other: HashSet<T>): HashSet<T> {
    const result = new HashSet<T>();
    for (const item of this.toArray()) {
      result.add(item);
    }
    for (const item of other.toArray()) {
      result.add(item);
    }
    return result;
  }

  intersection(other: HashSet<T>): HashSet<T> {
    const result = new HashSet<T>();
    for (const item of this.toArray()) {
      if (other.has(item)) {
        result.add(item);
      }
    }
    return result;
  }

  difference(other: HashSet<T>): HashSet<T> {
    const result = new HashSet<T>();
    for (const item of this.toArray()) {
      if (!other.has(item)) {
        result.add(item);
      }
    }
    return result;
  }

  static fromArray<T>(items: T[]): HashSet<T> {
    const set = new HashSet<T>();
    for (const item of items) {
      set.add(item);
    }
    return set;
  }
}
