/**
 * Segment Tree Implementation.
 *
 * A data structure for efficient range queries and point/range updates.
 *
 * COMPLEXITY:
 * | Operation         | Time          |
 * |-------------------|---------------|
 * | Build             | O(n)          |
 * | Point Update      | O(log n)      |
 * | Range Query       | O(log n)      |
 * | Range Update*     | O(log n)      |
 * | Space             | O(n)          |
 *
 * * With lazy propagation
 */

/**
 * Segment Tree for range queries and point updates.
 * Supports any associative operation (sum, min, max, gcd, etc.)
 */
export class SegmentTree<T> {
  private _n: number;
  private _tree: T[];
  private _op: (a: T, b: T) => T;
  private _identity: T;

  /**
   * Build a segment tree over the given array.
   * @param arr - Input array
   * @param operation - Associative binary operation (default: sum for numbers)
   * @param identity - Identity element for the operation (default: 0)
   */
  constructor(
    arr: T[],
    operation: (a: T, b: T) => T = (a: T, b: T) =>
      ((a as number) + (b as number)) as unknown as T,
    identity: T = 0 as unknown as T
  ) {
    this._n = arr.length;
    this._op = operation;
    this._identity = identity;
    this._tree = this._n > 0 ? new Array<T>(4 * this._n).fill(identity) : [];

    if (this._n > 0) {
      this._build(arr, 0, 0, this._n - 1);
    }
  }

  private _build(arr: T[], node: number, start: number, end: number): void {
    if (start === end) {
      this._tree[node] = arr[start];
    } else {
      const mid = (start + end) >> 1;
      const left = 2 * node + 1;
      const right = 2 * node + 2;

      this._build(arr, left, start, mid);
      this._build(arr, right, mid + 1, end);

      this._tree[node] = this._op(this._tree[left], this._tree[right]);
    }
  }

  /**
   * Update the value at a specific index.
   * O(log n)
   */
  update(index: number, value: T): void {
    if (index < 0 || index >= this._n) {
      throw new RangeError(`Index ${index} out of range [0, ${this._n})`);
    }
    this._update(0, 0, this._n - 1, index, value);
  }

  private _update(
    node: number,
    start: number,
    end: number,
    index: number,
    value: T
  ): void {
    if (start === end) {
      this._tree[node] = value;
    } else {
      const mid = (start + end) >> 1;
      const left = 2 * node + 1;
      const right = 2 * node + 2;

      if (index <= mid) {
        this._update(left, start, mid, index, value);
      } else {
        this._update(right, mid + 1, end, index, value);
      }

      this._tree[node] = this._op(this._tree[left], this._tree[right]);
    }
  }

  /**
   * Query the operation result over range [lo, hi] (inclusive).
   * O(log n)
   */
  query(lo: number, hi: number): T {
    if (this._n === 0) return this._identity;
    return this._query(0, 0, this._n - 1, lo, hi);
  }

  private _query(
    node: number,
    start: number,
    end: number,
    lo: number,
    hi: number
  ): T {
    // No overlap
    if (hi < start || lo > end) {
      return this._identity;
    }

    // Complete overlap
    if (lo <= start && end <= hi) {
      return this._tree[node];
    }

    // Partial overlap
    const mid = (start + end) >> 1;
    const leftResult = this._query(2 * node + 1, start, mid, lo, hi);
    const rightResult = this._query(2 * node + 2, mid + 1, end, lo, hi);

    return this._op(leftResult, rightResult);
  }

  /** Return the number of elements in the underlying array. */
  get size(): number {
    return this._n;
  }
}

/**
 * Segment Tree with Lazy Propagation for efficient range updates.
 * Supports range add updates and range sum queries.
 */
export class SegmentTreeLazy {
  private _n: number;
  private _tree: number[];
  private _lazy: number[];

  constructor(arr: number[]) {
    this._n = arr.length;
    this._tree = this._n > 0 ? new Array(4 * this._n).fill(0) : [];
    this._lazy = this._n > 0 ? new Array(4 * this._n).fill(0) : [];

    if (this._n > 0) {
      this._build(arr, 0, 0, this._n - 1);
    }
  }

  private _build(
    arr: number[],
    node: number,
    start: number,
    end: number
  ): void {
    if (start === end) {
      this._tree[node] = arr[start];
    } else {
      const mid = (start + end) >> 1;
      this._build(arr, 2 * node + 1, start, mid);
      this._build(arr, 2 * node + 2, mid + 1, end);
      this._tree[node] =
        this._tree[2 * node + 1] + this._tree[2 * node + 2];
    }
  }

  private _pushDown(node: number, start: number, end: number): void {
    if (this._lazy[node] !== 0) {
      const mid = (start + end) >> 1;
      const leftChild = 2 * node + 1;
      const rightChild = 2 * node + 2;

      // Update children
      this._tree[leftChild] += this._lazy[node] * (mid - start + 1);
      this._tree[rightChild] += this._lazy[node] * (end - mid);

      // Propagate lazy to children
      this._lazy[leftChild] += this._lazy[node];
      this._lazy[rightChild] += this._lazy[node];

      this._lazy[node] = 0;
    }
  }

  /**
   * Add value to all elements in range [lo, hi] (inclusive).
   * O(log n)
   */
  rangeUpdate(lo: number, hi: number, value: number): void {
    if (this._n === 0) return;
    this._rangeUpdate(0, 0, this._n - 1, lo, hi, value);
  }

  private _rangeUpdate(
    node: number,
    start: number,
    end: number,
    lo: number,
    hi: number,
    value: number
  ): void {
    // No overlap
    if (hi < start || lo > end) {
      return;
    }

    // Complete overlap
    if (lo <= start && end <= hi) {
      this._tree[node] += value * (end - start + 1);
      this._lazy[node] += value;
      return;
    }

    // Partial overlap - push down lazy first
    this._pushDown(node, start, end);

    const mid = (start + end) >> 1;
    this._rangeUpdate(2 * node + 1, start, mid, lo, hi, value);
    this._rangeUpdate(2 * node + 2, mid + 1, end, lo, hi, value);

    this._tree[node] =
      this._tree[2 * node + 1] + this._tree[2 * node + 2];
  }

  /**
   * Update a single point by adding a value.
   * O(log n)
   */
  pointUpdate(index: number, value: number): void {
    this.rangeUpdate(index, index, value);
  }

  /**
   * Query the sum of range [lo, hi] (inclusive).
   * O(log n)
   */
  query(lo: number, hi: number): number {
    if (this._n === 0) return 0;
    return this._query(0, 0, this._n - 1, lo, hi);
  }

  private _query(
    node: number,
    start: number,
    end: number,
    lo: number,
    hi: number
  ): number {
    if (hi < start || lo > end) {
      return 0;
    }

    if (lo <= start && end <= hi) {
      return this._tree[node];
    }

    this._pushDown(node, start, end);

    const mid = (start + end) >> 1;
    return (
      this._query(2 * node + 1, start, mid, lo, hi) +
      this._query(2 * node + 2, mid + 1, end, lo, hi)
    );
  }

  /** Return the number of elements. */
  get size(): number {
    return this._n;
  }
}
