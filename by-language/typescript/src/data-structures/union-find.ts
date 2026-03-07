/**
 * Union-Find (Disjoint Set Union) Implementation.
 *
 * A data structure that tracks elements partitioned into disjoint (non-overlapping) sets.
 * Supports near-constant time operations for union and find.
 *
 * COMPLEXITY (with path compression + union by rank):
 * | Operation | Time          |
 * |-----------|---------------|
 * | Find      | O(alpha(n))   | (alpha = inverse Ackermann, effectively O(1))
 * | Union     | O(alpha(n))   |
 * | Connected | O(alpha(n))   |
 * | Make Set  | O(1)          |
 *
 * Space: O(n)
 */

/**
 * Generic dictionary-based Union-Find with path compression and union by rank.
 */
export class UnionFind<T> {
  private _parent: Map<T, T> = new Map();
  private _rank: Map<T, number> = new Map();
  private _size: Map<T, number> = new Map();
  private _numComponents = 0;

  /**
   * Create a new set containing only element x.
   * Returns true if new set was created, false if x already exists.
   */
  makeSet(x: T): boolean {
    if (this._parent.has(x)) {
      return false;
    }
    this._parent.set(x, x);
    this._rank.set(x, 0);
    this._size.set(x, 1);
    this._numComponents++;
    return true;
  }

  /**
   * Find the representative (root) of the set containing x.
   * Uses path compression for efficiency.
   * Throws if x is not in any set.
   */
  find(x: T): T {
    if (!this._parent.has(x)) {
      throw new Error(`Element ${x} not found in Union-Find`);
    }

    let root = x;
    while (this._parent.get(root) !== root) {
      root = this._parent.get(root)!;
    }

    // Path compression
    let current = x;
    while (this._parent.get(current) !== root) {
      const next = this._parent.get(current)!;
      this._parent.set(current, root);
      current = next;
    }

    return root;
  }

  /**
   * Merge the sets containing x and y using union by rank.
   * Returns true if sets were merged, false if already in same set.
   */
  union(x: T, y: T): boolean {
    let rootX = this.find(x);
    let rootY = this.find(y);

    if (rootX === rootY) {
      return false;
    }

    // Union by rank: attach smaller tree under larger
    if (this._rank.get(rootX)! < this._rank.get(rootY)!) {
      [rootX, rootY] = [rootY, rootX];
    }

    this._parent.set(rootY, rootX);
    this._size.set(rootX, this._size.get(rootX)! + this._size.get(rootY)!);

    if (this._rank.get(rootX) === this._rank.get(rootY)) {
      this._rank.set(rootX, this._rank.get(rootX)! + 1);
    }

    this._numComponents--;
    return true;
  }

  /** Check if x and y are in the same set. */
  connected(x: T, y: T): boolean {
    return this.find(x) === this.find(y);
  }

  /** Get the size of the set containing x. */
  componentSize(x: T): number {
    const root = this.find(x);
    return this._size.get(root)!;
  }

  /** Get all connected components as arrays of sets. */
  getComponents(): Set<T>[] {
    const components = new Map<T, Set<T>>();

    for (const x of this._parent.keys()) {
      const root = this.find(x);
      if (!components.has(root)) {
        components.set(root, new Set());
      }
      components.get(root)!.add(x);
    }

    return Array.from(components.values());
  }

  /** Return the number of disjoint sets. */
  get numComponents(): number {
    return this._numComponents;
  }

  /** Return total number of elements. */
  get size(): number {
    return this._parent.size;
  }

  /** Check if element exists in the structure. */
  has(x: T): boolean {
    return this._parent.has(x);
  }

  toString(): string {
    return `UnionFind(elements=${this.size}, components=${this._numComponents})`;
  }
}

/**
 * Array-based Union-Find for integer elements [0, n).
 * More memory efficient and cache-friendly than dictionary-based version.
 */
export class UnionFindArray {
  private _n: number;
  private _parent: number[];
  private _rank: number[];
  private _sz: number[];
  private _numComponents: number;

  constructor(n: number) {
    this._n = n;
    this._parent = Array.from({ length: n }, (_, i) => i);
    this._rank = new Array(n).fill(0);
    this._sz = new Array(n).fill(1);
    this._numComponents = n;
  }

  /** Find root of x with path compression. */
  find(x: number): number {
    if (x < 0 || x >= this._n) {
      throw new RangeError(`Index ${x} out of range [0, ${this._n})`);
    }

    let root = x;
    while (this._parent[root] !== root) {
      root = this._parent[root];
    }

    // Path compression
    while (this._parent[x] !== root) {
      const next = this._parent[x];
      this._parent[x] = root;
      x = next;
    }

    return root;
  }

  /** Merge sets containing x and y. Returns true if merged. */
  union(x: number, y: number): boolean {
    let rootX = this.find(x);
    let rootY = this.find(y);

    if (rootX === rootY) {
      return false;
    }

    // Union by rank
    if (this._rank[rootX] < this._rank[rootY]) {
      [rootX, rootY] = [rootY, rootX];
    }

    this._parent[rootY] = rootX;
    this._sz[rootX] += this._sz[rootY];

    if (this._rank[rootX] === this._rank[rootY]) {
      this._rank[rootX]++;
    }

    this._numComponents--;
    return true;
  }

  /** Check if x and y are connected. */
  connected(x: number, y: number): boolean {
    return this.find(x) === this.find(y);
  }

  /** Get size of the component containing x. */
  componentSize(x: number): number {
    return this._sz[this.find(x)];
  }

  /** Return number of components. */
  get numComponents(): number {
    return this._numComponents;
  }

  /** Get all components as arrays of integers. */
  getComponents(): number[][] {
    const components = new Map<number, number[]>();
    for (let i = 0; i < this._n; i++) {
      const root = this.find(i);
      if (!components.has(root)) {
        components.set(root, []);
      }
      components.get(root)!.push(i);
    }
    return Array.from(components.values());
  }

  /** Return total number of elements. */
  get size(): number {
    return this._n;
  }

  toString(): string {
    return `UnionFindArray(n=${this._n}, components=${this._numComponents})`;
  }
}

/**
 * Union-Find with weighted edges (potential/distance tracking).
 * Tracks relative weights/distances between elements within the same set.
 * weight represents: value[x] - value[y] = weight when calling union(x, y, weight).
 */
export class WeightedUnionFind<T = number> {
  private _parent: Map<T, T> = new Map();
  private _rank: Map<T, number> = new Map();
  private _weight: Map<T, number> = new Map();
  private _numComponents = 0;

  /** Create a new set containing x with weight 0. */
  makeSet(x: T): boolean {
    if (this._parent.has(x)) {
      return false;
    }
    this._parent.set(x, x);
    this._rank.set(x, 0);
    this._weight.set(x, 0);
    this._numComponents++;
    return true;
  }

  /**
   * Find root of x and total weight from x to root.
   * Returns [root, weightToRoot].
   */
  find(x: T): [T, number] {
    if (!this._parent.has(x)) {
      throw new Error(`Element ${x} not found`);
    }

    if (this._parent.get(x) === x) {
      return [x, 0];
    }

    // Recursive find with path compression and weight accumulation
    const [root, parentWeight] = this.find(this._parent.get(x)!);
    this._parent.set(x, root);
    this._weight.set(x, this._weight.get(x)! + parentWeight);

    return [root, this._weight.get(x)!];
  }

  /**
   * Union sets containing x and y with relative weight.
   * weight represents: value[x] - value[y] = weight.
   * Returns true if union was performed.
   */
  union(x: T, y: T, weight: number = 0): boolean {
    const [rootX, weightX] = this.find(x);
    const [rootY, weightY] = this.find(y);

    if (rootX === rootY) {
      return false;
    }

    if (this._rank.get(rootX)! < this._rank.get(rootY)!) {
      this._parent.set(rootX, rootY);
      // weightX + newWeight = weight + weightY
      this._weight.set(rootX, weight + weightY - weightX);
    } else {
      this._parent.set(rootY, rootX);
      // weightY + newWeight = -weight + weightX
      this._weight.set(rootY, -weight + weightX - weightY);
      if (this._rank.get(rootX) === this._rank.get(rootY)) {
        this._rank.set(rootX, this._rank.get(rootX)! + 1);
      }
    }

    this._numComponents--;
    return true;
  }

  /** Check if x and y are connected. */
  connected(x: T, y: T): boolean {
    if (!this._parent.has(x) || !this._parent.has(y)) {
      return false;
    }
    const [rootX] = this.find(x);
    const [rootY] = this.find(y);
    return rootX === rootY;
  }

  /**
   * Get the weight from x to root.
   * Returns the weight or undefined if x is not in the structure.
   */
  getWeight(x: T): number | undefined {
    if (!this._parent.has(x)) {
      return undefined;
    }
    const [, w] = this.find(x);
    return w;
  }

  /**
   * Get the difference value[x] - value[y].
   * Returns the difference if x and y are connected, undefined otherwise.
   */
  diff(x: T, y: T): number | undefined {
    if (!this.connected(x, y)) {
      return undefined;
    }
    const [, weightX] = this.find(x);
    const [, weightY] = this.find(y);
    return weightX - weightY;
  }

  /** Return number of components. */
  get numComponents(): number {
    return this._numComponents;
  }

  /** Return number of elements. */
  get size(): number {
    return this._parent.size;
  }

  /** Check if element exists. */
  has(x: T): boolean {
    return this._parent.has(x);
  }
}

/**
 * Partially persistent Union-Find that supports querying past states.
 * Maintains version history allowing queries at any past version.
 * Uses snapshot/restore model with named versions.
 */
export class PersistentUnionFind {
  private _n: number;
  private _parent: number[];
  private _rank: number[];
  private _version = 0;
  private _history: Array<{
    version: number;
    index: number;
    oldParent: number;
    oldRank: number;
  }> = [];
  private _snapshots: Map<number, number> = new Map(); // snapshot label -> version

  constructor(n: number) {
    this._n = n;
    this._parent = Array.from({ length: n }, (_, i) => i);
    this._rank = new Array(n).fill(0);
  }

  /** Find root of x at a specific version (default: current). */
  find(x: number, version?: number): number {
    const v = version ?? this._version;
    const parent = this._getParentAtVersion(x, v);

    if (parent === x) {
      return x;
    }

    return this.find(parent, v);
  }

  private _getParentAtVersion(x: number, version: number): number {
    let parent = this._parent[x];

    // Check history for earlier values (most recent first)
    for (let i = this._history.length - 1; i >= 0; i--) {
      const h = this._history[i];
      if (h.version > version && h.index === x) {
        parent = h.oldParent;
      }
    }

    return parent;
  }

  /**
   * Union sets and return new version number.
   * Returns the current version if already connected.
   */
  union(x: number, y: number): number {
    let rootX = this.find(x);
    let rootY = this.find(y);

    if (rootX === rootY) {
      return this._version;
    }

    this._version++;

    // Union by rank
    if (this._rank[rootX] < this._rank[rootY]) {
      [rootX, rootY] = [rootY, rootX];
    }

    // Record history before modification
    this._history.push({
      version: this._version,
      index: rootY,
      oldParent: this._parent[rootY],
      oldRank: this._rank[rootY],
    });
    this._parent[rootY] = rootX;

    if (this._rank[rootX] === this._rank[rootY]) {
      this._history.push({
        version: this._version,
        index: rootX,
        oldParent: this._parent[rootX],
        oldRank: this._rank[rootX],
      });
      this._rank[rootX]++;
    }

    return this._version;
  }

  /** Check if x and y are connected at specific version. */
  connected(x: number, y: number, version?: number): boolean {
    return this.find(x, version) === this.find(y, version);
  }

  /** Take a snapshot and return the current version number. */
  snapshot(): number {
    const label = this._version;
    this._snapshots.set(label, this._version);
    return label;
  }

  /**
   * Restore to a previously saved version.
   * Note: this only affects queries via version parameter,
   * since the persistent UF never truly mutates past state.
   */
  restore(version: number): void {
    // The version parameter on find/connected already handles this.
    // We just validate the version exists.
    if (version < 0 || version > this._version) {
      throw new RangeError(
        `Version ${version} out of range [0, ${this._version}]`
      );
    }
  }

  /** Return current version number. */
  get currentVersion(): number {
    return this._version;
  }

  /** Return number of elements. */
  get size(): number {
    return this._n;
  }
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Count number of islands (connected 1s) in a grid.
 * LeetCode #200
 */
export function numIslands(grid: number[][]): number {
  if (grid.length === 0 || grid[0].length === 0) {
    return 0;
  }

  const rows = grid.length;
  const cols = grid[0].length;
  const uf = new UnionFindArray(rows * cols);

  let landCount = 0;

  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {
      if (grid[i][j] === 1) {
        landCount++;
        const idx = i * cols + j;

        // Check right neighbor
        if (j + 1 < cols && grid[i][j + 1] === 1) {
          uf.union(idx, idx + 1);
        }

        // Check down neighbor
        if (i + 1 < rows && grid[i + 1][j] === 1) {
          uf.union(idx, idx + cols);
        }
      }
    }
  }

  if (landCount === 0) {
    return 0;
  }

  const roots = new Set<number>();
  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {
      if (grid[i][j] === 1) {
        roots.add(uf.find(i * cols + j));
      }
    }
  }

  return roots.size;
}

/**
 * Find the edge that makes a tree cyclic.
 * LeetCode #684
 * Edges are 1-indexed.
 */
export function findRedundantConnection(
  edges: [number, number][]
): [number, number] {
  const n = edges.length;
  const uf = new UnionFindArray(n + 1); // 1-indexed

  for (const [u, v] of edges) {
    if (!uf.union(u, v)) {
      return [u, v];
    }
  }

  return [0, 0]; // Should not reach here if input is valid
}
