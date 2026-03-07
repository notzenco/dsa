/**
 * LRU Cache (Least Recently Used Cache)
 *
 * Combines a hash map with a doubly linked list for O(1) operations.
 *
 * COMPLEXITY:
 * | Operation | Time | Space |
 * |-----------|------|-------|
 * | get       | O(1) | O(1)  |
 * | put       | O(1) | O(1)  |
 * | delete    | O(1) | O(1)  |
 *
 * Space: O(capacity)
 */

class LRUNode<K, V> {
  key: K;
  value: V;
  prev: LRUNode<K, V> | null = null;
  next: LRUNode<K, V> | null = null;

  constructor(key: K, value: V) {
    this.key = key;
    this.value = value;
  }
}

export class LRUCache<K, V> {
  private _capacity: number;
  private _cache: Map<K, LRUNode<K, V>>;
  private _head: LRUNode<K, V>;
  private _tail: LRUNode<K, V>;

  constructor(capacity: number) {
    if (capacity < 1) {
      throw new Error("Capacity must be at least 1");
    }
    this._capacity = capacity;
    this._cache = new Map();

    // Dummy head and tail to simplify edge cases
    this._head = new LRUNode<K, V>(null as unknown as K, null as unknown as V);
    this._tail = new LRUNode<K, V>(null as unknown as K, null as unknown as V);
    this._head.next = this._tail;
    this._tail.prev = this._head;
  }

  get size(): number {
    return this._cache.size;
  }

  get capacity(): number {
    return this._capacity;
  }

  get(key: K): V | undefined {
    if (!this._cache.has(key)) {
      return undefined;
    }

    const node = this._cache.get(key)!;
    this._moveToFront(node);
    return node.value;
  }

  put(key: K, value: V): void {
    if (this._cache.has(key)) {
      const node = this._cache.get(key)!;
      node.value = value;
      this._moveToFront(node);
    } else {
      if (this._cache.size >= this._capacity) {
        this._evictLRU();
      }

      const node = new LRUNode(key, value);
      this._cache.set(key, node);
      this._addToFront(node);
    }
  }

  delete(key: K): boolean {
    if (!this._cache.has(key)) {
      return false;
    }

    const node = this._cache.get(key)!;
    this._removeNode(node);
    this._cache.delete(key);
    return true;
  }

  has(key: K): boolean {
    return this._cache.has(key);
  }

  clear(): void {
    this._cache.clear();
    this._head.next = this._tail;
    this._tail.prev = this._head;
  }

  keys(): K[] {
    const result: K[] = [];
    let node = this._head.next;
    while (node !== this._tail) {
      result.push(node!.key);
      node = node!.next;
    }
    return result;
  }

  values(): V[] {
    const result: V[] = [];
    let node = this._head.next;
    while (node !== this._tail) {
      result.push(node!.value);
      node = node!.next;
    }
    return result;
  }

  private _addToFront(node: LRUNode<K, V>): void {
    node.prev = this._head;
    node.next = this._head.next;
    this._head.next!.prev = node;
    this._head.next = node;
  }

  private _removeNode(node: LRUNode<K, V>): void {
    node.prev!.next = node.next;
    node.next!.prev = node.prev;
  }

  private _moveToFront(node: LRUNode<K, V>): void {
    this._removeNode(node);
    this._addToFront(node);
  }

  private _evictLRU(): void {
    const lruNode = this._tail.prev!;
    this._removeNode(lruNode);
    this._cache.delete(lruNode.key);
  }
}
