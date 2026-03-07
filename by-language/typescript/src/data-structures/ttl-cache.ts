/**
 * TTL Cache (Time-To-Live Cache)
 *
 * A cache that combines time-based expiry with LRU eviction policy.
 * Items automatically expire after a specified duration.
 *
 * COMPLEXITY:
 * | Operation | Time   | Space  |
 * |-----------|--------|--------|
 * | get       | O(1)   | O(1)   |
 * | put       | O(1)*  | O(1)   |
 * | has       | O(1)   | O(1)   |
 * | cleanup   | O(n)   | O(1)   |
 *
 * * O(n) if cleanup triggers during put
 * Space: O(capacity)
 */

interface TTLEntry<V> {
  value: V;
  expireAt: number;
}

class LRUOrderNode<K> {
  key: K;
  prev: LRUOrderNode<K> | null = null;
  next: LRUOrderNode<K> | null = null;

  constructor(key: K) {
    this.key = key;
  }
}

export class TTLCache<K, V> {
  private _capacity: number;
  private _defaultTTLms: number;
  private _cache: Map<K, TTLEntry<V>>;
  private _orderMap: Map<K, LRUOrderNode<K>>;
  private _head: LRUOrderNode<K>;
  private _tail: LRUOrderNode<K>;
  private _now: () => number;

  constructor(capacity: number, defaultTTLms: number) {
    if (capacity < 1) {
      throw new Error("Capacity must be at least 1");
    }
    if (defaultTTLms <= 0) {
      throw new Error("TTL must be positive");
    }

    this._capacity = capacity;
    this._defaultTTLms = defaultTTLms;
    this._cache = new Map();
    this._orderMap = new Map();
    this._now = () => Date.now();

    // Dummy head and tail for the LRU linked list
    this._head = new LRUOrderNode<K>(null as unknown as K);
    this._tail = new LRUOrderNode<K>(null as unknown as K);
    this._head.next = this._tail;
    this._tail.prev = this._head;
  }

  get size(): number {
    this._cleanup();
    return this._cache.size;
  }

  get capacity(): number {
    return this._capacity;
  }

  get(key: K): V | undefined {
    if (!this._cache.has(key)) {
      return undefined;
    }

    const entry = this._cache.get(key)!;

    if (this._now() > entry.expireAt) {
      this._remove(key);
      return undefined;
    }

    // Move to front (most recently used)
    this._moveToFront(key);
    return entry.value;
  }

  put(key: K, value: V, ttl?: number): void {
    this._cleanup();

    const ttlMs = ttl !== undefined ? ttl : this._defaultTTLms;
    const expireAt = this._now() + ttlMs;

    if (this._cache.has(key)) {
      this._cache.set(key, { value, expireAt });
      this._moveToFront(key);
    } else {
      if (this._cache.size >= this._capacity) {
        this._evictLRU();
      }

      this._cache.set(key, { value, expireAt });
      const node = new LRUOrderNode(key);
      this._orderMap.set(key, node);
      this._addToFront(node);
    }
  }

  delete(key: K): boolean {
    return this._remove(key);
  }

  has(key: K): boolean {
    if (!this._cache.has(key)) {
      return false;
    }

    const entry = this._cache.get(key)!;
    if (this._now() > entry.expireAt) {
      this._remove(key);
      return false;
    }

    return true;
  }

  clear(): void {
    this._cache.clear();
    this._orderMap.clear();
    this._head.next = this._tail;
    this._tail.prev = this._head;
  }

  /** Visible for testing: override the clock source. */
  _setNow(fn: () => number): void {
    this._now = fn;
  }

  private _remove(key: K): boolean {
    if (!this._cache.has(key)) return false;

    this._cache.delete(key);

    const node = this._orderMap.get(key);
    if (node) {
      this._removeNode(node);
      this._orderMap.delete(key);
    }

    return true;
  }

  private _cleanup(): void {
    const now = this._now();
    const expired: K[] = [];

    for (const [key, entry] of this._cache) {
      if (now > entry.expireAt) {
        expired.push(key);
      }
    }

    for (const key of expired) {
      this._remove(key);
    }
  }

  private _addToFront(node: LRUOrderNode<K>): void {
    node.prev = this._head;
    node.next = this._head.next;
    this._head.next!.prev = node;
    this._head.next = node;
  }

  private _removeNode(node: LRUOrderNode<K>): void {
    node.prev!.next = node.next;
    node.next!.prev = node.prev;
  }

  private _moveToFront(key: K): void {
    const node = this._orderMap.get(key);
    if (node) {
      this._removeNode(node);
      this._addToFront(node);
    }
  }

  private _evictLRU(): void {
    const lruNode = this._tail.prev!;
    if (lruNode === this._head) return;

    this._cache.delete(lruNode.key);
    this._orderMap.delete(lruNode.key);
    this._removeNode(lruNode);
  }
}
