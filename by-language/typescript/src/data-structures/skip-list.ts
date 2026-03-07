/**
 * Skip List Implementation
 *
 * A probabilistic data structure that provides O(log n) search, insert, and delete.
 *
 * COMPLEXITY:
 * | Operation         | Expected  | Worst   |
 * |-------------------|-----------|---------|
 * | Search            | O(log n)  | O(n)    |
 * | Insert            | O(log n)  | O(n)    |
 * | Delete            | O(log n)  | O(n)    |
 * | Space             | O(n)      | O(n lg n)|
 */

const MAX_LEVEL = 32;

class SkipNode<T> {
  value: T;
  forward: (SkipNode<T> | null)[];

  constructor(value: T, level: number) {
    this.value = value;
    this.forward = new Array(level).fill(null);
  }
}

export class SkipList<T> {
  static readonly MAX_LEVEL = MAX_LEVEL;

  private _probability: number;
  private _level: number;
  private _size: number;
  private _head: SkipNode<T>;

  constructor(probability: number = 0.5) {
    if (probability <= 0 || probability >= 1) {
      probability = 0.5;
    }
    this._probability = probability;
    this._level = 1;
    this._size = 0;
    // Head node is a sentinel with max level
    this._head = new SkipNode<T>(undefined as unknown as T, MAX_LEVEL);
  }

  get size(): number {
    return this._size;
  }

  get isEmpty(): boolean {
    return this._size === 0;
  }

  private _randomLevel(): number {
    let level = 1;
    while (Math.random() < this._probability && level < MAX_LEVEL) {
      level++;
    }
    return level;
  }

  insert(value: T): void {
    const update: SkipNode<T>[] = new Array(MAX_LEVEL).fill(this._head);
    let current = this._head;

    for (let i = this._level - 1; i >= 0; i--) {
      while (current.forward[i] !== null && current.forward[i]!.value < value) {
        current = current.forward[i]!;
      }
      update[i] = current;
    }

    current = current.forward[0]!;

    // If key already exists, update value (replace)
    if (current !== null && current.value === value) {
      return;
    }

    const newLevel = this._randomLevel();

    if (newLevel > this._level) {
      for (let i = this._level; i < newLevel; i++) {
        update[i] = this._head;
      }
      this._level = newLevel;
    }

    const newNode = new SkipNode<T>(value, newLevel);

    for (let i = 0; i < newLevel; i++) {
      newNode.forward[i] = update[i].forward[i];
      update[i].forward[i] = newNode;
    }

    this._size++;
  }

  delete(value: T): boolean {
    const update: SkipNode<T>[] = new Array(MAX_LEVEL).fill(this._head);
    let current = this._head;

    for (let i = this._level - 1; i >= 0; i--) {
      while (current.forward[i] !== null && current.forward[i]!.value < value) {
        current = current.forward[i]!;
      }
      update[i] = current;
    }

    current = current.forward[0]!;

    if (current === null || current.value !== value) {
      return false;
    }

    for (let i = 0; i < this._level; i++) {
      if (update[i].forward[i] !== current) {
        break;
      }
      update[i].forward[i] = current.forward[i];
    }

    while (this._level > 1 && this._head.forward[this._level - 1] === null) {
      this._level--;
    }

    this._size--;
    return true;
  }

  search(value: T): boolean {
    let current = this._head;

    for (let i = this._level - 1; i >= 0; i--) {
      while (current.forward[i] !== null && current.forward[i]!.value < value) {
        current = current.forward[i]!;
      }
    }

    current = current.forward[0]!;
    return current !== null && current.value === value;
  }

  range(lo: T, hi: T): T[] {
    if (lo > hi) return [];

    const result: T[] = [];
    let current = this._head;

    for (let i = this._level - 1; i >= 0; i--) {
      while (current.forward[i] !== null && current.forward[i]!.value < lo) {
        current = current.forward[i]!;
      }
    }

    current = current.forward[0]!;

    while (current !== null && current.value <= hi) {
      result.push(current.value);
      current = current.forward[0]!;
    }

    return result;
  }

  floor(value: T): T | null {
    let current = this._head;
    let floorNode: SkipNode<T> | null = null;

    for (let i = this._level - 1; i >= 0; i--) {
      while (
        current.forward[i] !== null &&
        current.forward[i]!.value <= value
      ) {
        current = current.forward[i]!;
      }
      if (current !== this._head && current.value <= value) {
        floorNode = current;
      }
    }

    return floorNode !== null ? floorNode.value : null;
  }

  ceiling(value: T): T | null {
    let current = this._head;

    for (let i = this._level - 1; i >= 0; i--) {
      while (current.forward[i] !== null && current.forward[i]!.value < value) {
        current = current.forward[i]!;
      }
    }

    current = current.forward[0]!;

    if (current !== null) {
      return current.value;
    }

    return null;
  }

  toArray(): T[] {
    const result: T[] = [];
    let current = this._head.forward[0];
    while (current !== null) {
      result.push(current.value);
      current = current.forward[0];
    }
    return result;
  }

  clear(): void {
    for (let i = 0; i < MAX_LEVEL; i++) {
      this._head.forward[i] = null;
    }
    this._level = 1;
    this._size = 0;
  }
}
