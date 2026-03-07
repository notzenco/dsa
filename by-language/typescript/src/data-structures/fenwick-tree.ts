/**
 * Fenwick Tree (Binary Indexed Tree) Implementation.
 *
 * A data structure for efficient prefix sum queries and point updates.
 * More space-efficient than segment trees but less flexible.
 *
 * COMPLEXITY:
 * | Operation         | Time          |
 * |-------------------|---------------|
 * | Build             | O(n log n)    |
 * | Point Update      | O(log n)      |
 * | Prefix Sum        | O(log n)      |
 * | Range Sum         | O(log n)      |
 * | Space             | O(n)          |
 *
 * Key insight: i & (-i) gives the lowest set bit of i (1-indexed internal).
 * 0-indexed external API, 1-indexed internal storage.
 */

/**
 * 1D Fenwick Tree (Binary Indexed Tree) for prefix sums.
 * 0-indexed API, 1-indexed internal.
 */
export class FenwickTree {
  private _n: number;
  private _tree: number[];

  /**
   * Create a Fenwick tree from an initial size.
   * All values start at 0. Use update() to set initial values.
   * @param size - Number of elements
   */
  constructor(size: number) {
    this._n = size;
    this._tree = new Array(size + 1).fill(0);
  }

  /**
   * Create a Fenwick tree from an array of initial values.
   */
  static fromArray(arr: number[]): FenwickTree {
    const ft = new FenwickTree(arr.length);
    for (let i = 0; i < arr.length; i++) {
      ft._add(i + 1, arr[i]);
    }
    return ft;
  }

  /** Internal 1-indexed add. */
  private _add(i: number, delta: number): void {
    while (i <= this._n) {
      this._tree[i] += delta;
      i += i & -i; // Add lowest set bit
    }
  }

  /**
   * Add delta to element at index (0-indexed).
   * O(log n)
   */
  update(index: number, delta: number): void {
    this._add(index + 1, delta);
  }

  /**
   * Get sum of elements from 0 to index (inclusive, 0-indexed).
   * O(log n)
   */
  prefixSum(index: number): number {
    let i = index + 1; // Convert to 1-indexed
    let total = 0;
    while (i > 0) {
      total += this._tree[i];
      i -= i & -i; // Remove lowest set bit
    }
    return total;
  }

  /**
   * Get sum of elements in range [left, right] (inclusive, 0-indexed).
   * O(log n)
   */
  rangeSum(left: number, right: number): number {
    const rightSum = this.prefixSum(right);
    const leftSum = left > 0 ? this.prefixSum(left - 1) : 0;
    return rightSum - leftSum;
  }

  /**
   * Get the value at a single index (0-indexed).
   * This computes rangeSum(index, index).
   * O(log n)
   */
  pointQuery(index: number): number {
    return this.rangeSum(index, index);
  }

  /** Return number of elements. */
  get size(): number {
    return this._n;
  }
}

/**
 * 2D Fenwick Tree for 2D range sum queries.
 * 0-indexed API, 1-indexed internal.
 */
export class FenwickTree2D {
  private _rows: number;
  private _cols: number;
  private _tree: number[][];

  /**
   * Create a 2D Fenwick tree of given dimensions.
   * All values start at 0.
   */
  constructor(rows: number, cols: number) {
    this._rows = rows;
    this._cols = cols;
    this._tree = Array.from({ length: rows + 1 }, () =>
      new Array(cols + 1).fill(0)
    );
  }

  /**
   * Create a 2D Fenwick tree from a matrix.
   */
  static fromMatrix(matrix: number[][]): FenwickTree2D {
    if (matrix.length === 0 || matrix[0].length === 0) {
      return new FenwickTree2D(0, 0);
    }
    const rows = matrix.length;
    const cols = matrix[0].length;
    const ft = new FenwickTree2D(rows, cols);
    for (let i = 0; i < rows; i++) {
      for (let j = 0; j < cols; j++) {
        ft._add(i + 1, j + 1, matrix[i][j]);
      }
    }
    return ft;
  }

  /** Internal 1-indexed add. */
  private _add(row: number, col: number, delta: number): void {
    let i = row;
    while (i <= this._rows) {
      let j = col;
      while (j <= this._cols) {
        this._tree[i][j] += delta;
        j += j & -j;
      }
      i += i & -i;
    }
  }

  /**
   * Add delta to position (row, col) - 0-indexed.
   * O(log(rows) * log(cols))
   */
  update(row: number, col: number, delta: number): void {
    this._add(row + 1, col + 1, delta);
  }

  /** Internal 1-indexed prefix sum. */
  private _prefixSumInternal(row: number, col: number): number {
    let total = 0;
    let i = row;
    while (i > 0) {
      let j = col;
      while (j > 0) {
        total += this._tree[i][j];
        j -= j & -j;
      }
      i -= i & -i;
    }
    return total;
  }

  /**
   * Get prefix sum from (0,0) to (row, col) inclusive - 0-indexed.
   * O(log(rows) * log(cols))
   */
  prefixSum(row: number, col: number): number {
    return this._prefixSumInternal(row + 1, col + 1);
  }

  /**
   * Get sum of submatrix from (r1,c1) to (r2,c2) inclusive - 0-indexed.
   * O(log(rows) * log(cols))
   */
  rangeSum(r1: number, c1: number, r2: number, c2: number): number {
    if (this._rows === 0 || this._cols === 0) {
      return 0;
    }

    const ir1 = r1 + 1;
    const ic1 = c1 + 1;
    const ir2 = r2 + 1;
    const ic2 = c2 + 1;

    return (
      this._prefixSumInternal(ir2, ic2) -
      this._prefixSumInternal(ir1 - 1, ic2) -
      this._prefixSumInternal(ir2, ic1 - 1) +
      this._prefixSumInternal(ir1 - 1, ic1 - 1)
    );
  }

  /** Return number of rows. */
  get rows(): number {
    return this._rows;
  }

  /** Return number of cols. */
  get cols(): number {
    return this._cols;
  }
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Count inversions in an array using a Fenwick tree.
 * An inversion is a pair (i, j) where i < j and arr[i] > arr[j].
 * O(n log n)
 */
export function countInversions(arr: number[]): number {
  if (arr.length === 0) {
    return 0;
  }

  // Coordinate compression
  const sortedUnique = [...new Set(arr)].sort((a, b) => a - b);
  const rank = new Map<number, number>();
  sortedUnique.forEach((v, i) => rank.set(v, i + 1));

  const n = sortedUnique.length;
  const ft = new FenwickTree(n);
  let inversions = 0;

  for (let i = arr.length - 1; i >= 0; i--) {
    const r = rank.get(arr[i])!;
    if (r > 1) {
      inversions += ft.prefixSum(r - 2); // 0-indexed: prefix sum up to rank r-2
    }
    ft.update(r - 1, 1); // 0-indexed: add 1 at position r-1
  }

  return inversions;
}

/**
 * Mutable range sum query class backed by a Fenwick tree.
 * LeetCode #307
 */
export class RangeSumQueryMutable {
  private _ft: FenwickTree;
  private _original: number[];

  constructor(arr: number[]) {
    this._original = [...arr];
    this._ft = FenwickTree.fromArray(arr);
  }

  /** Set the value at index to val. */
  update(index: number, val: number): void {
    const delta = val - this._original[index];
    this._original[index] = val;
    this._ft.update(index, delta);
  }

  /** Query the sum of range [left, right] inclusive. */
  sumRange(left: number, right: number): number {
    return this._ft.rangeSum(left, right);
  }
}
