/**
 * LFU Cache (Least Frequently Used Cache)
 *
 * Evicts the least frequently used item. Ties broken by LRU.
 *
 * Structure: Hash Map + Frequency Map (each frequency -> Doubly Linked List)
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

class LFUNode<K, V> {
  key: K;
  value: V;
  freq: number;
  prev: LFUNode<K, V> | null = null;
  next: LFUNode<K, V> | null = null;

  constructor(key: K, value: V, freq: number = 1) {
    this.key = key;
    this.value = value;
    this.freq = freq;
  }
}

class FrequencyList<K, V> {
  head: LFUNode<K, V>;
  tail: LFUNode<K, V>;
  size: number = 0;

  constructor() {
    this.head = new LFUNode<K, V>(
      null as unknown as K,
      null as unknown as V,
      0
    );
    this.tail = new LFUNode<K, V>(
      null as unknown as K,
      null as unknown as V,
      0
    );
    this.head.next = this.tail;
    this.tail.prev = this.head;
  }

  addFront(node: LFUNode<K, V>): void {
    node.prev = this.head;
    node.next = this.head.next;
    this.head.next!.prev = node;
    this.head.next = node;
    this.size++;
  }

  remove(node: LFUNode<K, V>): void {
    if (node.prev) node.prev.next = node.next;
    if (node.next) node.next.prev = node.prev;
    node.prev = null;
    node.next = null;
    this.size--;
  }

  removeTail(): LFUNode<K, V> | null {
    if (this.size === 0) return null;
    const node = this.tail.prev!;
    this.remove(node);
    return node;
  }

  isEmpty(): boolean {
    return this.size === 0;
  }
}

export class LFUCache<K, V> {
  private _capacity: number;
  private _size: number = 0;
  private _minFreq: number = 0;
  private _keyMap: Map<K, LFUNode<K, V>> = new Map();
  private _freqMap: Map<number, FrequencyList<K, V>> = new Map();

  constructor(capacity: number) {
    if (capacity < 1) {
      throw new Error("Capacity must be at least 1");
    }
    this._capacity = capacity;
  }

  get size(): number {
    return this._size;
  }

  get capacity(): number {
    return this._capacity;
  }

  get(key: K): V | undefined {
    if (!this._keyMap.has(key)) {
      return undefined;
    }

    const node = this._keyMap.get(key)!;
    this._updateFrequency(node);
    return node.value;
  }

  put(key: K, value: V): void {
    if (this._keyMap.has(key)) {
      const node = this._keyMap.get(key)!;
      node.value = value;
      this._updateFrequency(node);
      return;
    }

    if (this._size >= this._capacity) {
      this._evict();
    }

    const node = new LFUNode(key, value, 1);
    this._keyMap.set(key, node);

    if (!this._freqMap.has(1)) {
      this._freqMap.set(1, new FrequencyList<K, V>());
    }
    this._freqMap.get(1)!.addFront(node);
    this._minFreq = 1;
    this._size++;
  }

  delete(key: K): boolean {
    if (!this._keyMap.has(key)) {
      return false;
    }

    const node = this._keyMap.get(key)!;
    const freqList = this._freqMap.get(node.freq);
    if (freqList) {
      freqList.remove(node);
    }

    this._keyMap.delete(key);
    this._size--;
    return true;
  }

  has(key: K): boolean {
    return this._keyMap.has(key);
  }

  clear(): void {
    this._keyMap.clear();
    this._freqMap.clear();
    this._size = 0;
    this._minFreq = 0;
  }

  getFrequency(key: K): number {
    if (!this._keyMap.has(key)) return -1;
    return this._keyMap.get(key)!.freq;
  }

  private _updateFrequency(node: LFUNode<K, V>): void {
    const oldFreq = node.freq;
    const newFreq = oldFreq + 1;

    const oldList = this._freqMap.get(oldFreq);
    if (oldList) {
      oldList.remove(node);

      if (oldList.isEmpty() && this._minFreq === oldFreq) {
        this._minFreq = newFreq;
      }
    }

    node.freq = newFreq;

    if (!this._freqMap.has(newFreq)) {
      this._freqMap.set(newFreq, new FrequencyList<K, V>());
    }
    this._freqMap.get(newFreq)!.addFront(node);
  }

  private _evict(): void {
    if (this._size === 0) return;

    const minList = this._freqMap.get(this._minFreq);
    if (!minList) return;

    const node = minList.removeTail();
    if (!node) return;

    this._keyMap.delete(node.key);
    this._size--;
  }
}
