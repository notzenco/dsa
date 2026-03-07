/**
 * AVL Tree - Self-balancing Binary Search Tree
 *
 * Maintains balance by ensuring the height difference between
 * left and right subtrees is at most 1 for every node.
 *
 * COMPLEXITY:
 * | Operation | Time     | Space |
 * |-----------|----------|-------|
 * | Search    | O(log n) | O(1)  |
 * | Insert    | O(log n) | O(1)  |
 * | Delete    | O(log n) | O(1)  |
 * | Min/Max   | O(log n) | O(1)  |
 */

export class AVLNode<K, V> {
  key: K;
  value: V;
  height: number;
  left: AVLNode<K, V> | null;
  right: AVLNode<K, V> | null;

  constructor(key: K, value?: V) {
    this.key = key;
    this.value = (value !== undefined ? value : key) as V;
    this.height = 1;
    this.left = null;
    this.right = null;
  }
}

export type Comparator<K> = (a: K, b: K) => number;

function defaultCompare<K>(a: K, b: K): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

export class AVLTree<K, V = K> {
  private _root: AVLNode<K, V> | null = null;
  private _size: number = 0;
  private _compare: Comparator<K>;

  constructor(compare?: Comparator<K>) {
    this._compare = compare ?? defaultCompare;
  }

  private _height(node: AVLNode<K, V> | null): number {
    return node ? node.height : 0;
  }

  getBalance(node: AVLNode<K, V> | null): number {
    if (!node) return 0;
    return this._height(node.left) - this._height(node.right);
  }

  private _updateHeight(node: AVLNode<K, V>): void {
    node.height =
      1 + Math.max(this._height(node.left), this._height(node.right));
  }

  rotateRight(z: AVLNode<K, V>): AVLNode<K, V> {
    const y = z.left!;
    const T3 = y.right;

    y.right = z;
    z.left = T3;

    this._updateHeight(z);
    this._updateHeight(y);

    return y;
  }

  rotateLeft(z: AVLNode<K, V>): AVLNode<K, V> {
    const y = z.right!;
    const T2 = y.left;

    y.left = z;
    z.right = T2;

    this._updateHeight(z);
    this._updateHeight(y);

    return y;
  }

  rebalance(node: AVLNode<K, V>): AVLNode<K, V> {
    this._updateHeight(node);
    const balance = this.getBalance(node);

    // Left heavy
    if (balance > 1) {
      // Left-Right case
      if (this.getBalance(node.left!) < 0) {
        node.left = this.rotateLeft(node.left!);
      }
      // Left-Left case
      return this.rotateRight(node);
    }

    // Right heavy
    if (balance < -1) {
      // Right-Left case
      if (this.getBalance(node.right!) > 0) {
        node.right = this.rotateRight(node.right!);
      }
      // Right-Right case
      return this.rotateLeft(node);
    }

    return node;
  }

  insert(key: K, value?: V): void {
    this._root = this._insert(this._root, key, value);
    this._size++;
  }

  private _insert(
    node: AVLNode<K, V> | null,
    key: K,
    value?: V,
  ): AVLNode<K, V> {
    if (!node) {
      return new AVLNode<K, V>(key, value as V);
    }

    const cmp = this._compare(key, node.key);
    if (cmp < 0) {
      node.left = this._insert(node.left, key, value);
    } else if (cmp > 0) {
      node.right = this._insert(node.right, key, value);
    } else {
      // Key exists, update value
      node.value = (value !== undefined ? value : key) as V;
      this._size--; // Compensate for increment in insert()
      return node;
    }

    return this.rebalance(node);
  }

  delete(key: K): boolean {
    if (!this.search(key)) return false;
    this._root = this._delete(this._root, key);
    this._size--;
    return true;
  }

  private _delete(
    node: AVLNode<K, V> | null,
    key: K,
  ): AVLNode<K, V> | null {
    if (!node) return null;

    const cmp = this._compare(key, node.key);
    if (cmp < 0) {
      node.left = this._delete(node.left, key);
    } else if (cmp > 0) {
      node.right = this._delete(node.right, key);
    } else {
      if (!node.left) return node.right;
      if (!node.right) return node.left;

      const successor = this._minNode(node.right);
      node.key = successor.key;
      node.value = successor.value;
      node.right = this._delete(node.right, successor.key);
    }

    return this.rebalance(node);
  }

  private _minNode(node: AVLNode<K, V>): AVLNode<K, V> {
    let current = node;
    while (current.left) {
      current = current.left;
    }
    return current;
  }

  search(key: K): boolean {
    return this._search(this._root, key) !== null;
  }

  private _search(
    node: AVLNode<K, V> | null,
    key: K,
  ): AVLNode<K, V> | null {
    if (!node) return null;
    const cmp = this._compare(key, node.key);
    if (cmp === 0) return node;
    if (cmp < 0) return this._search(node.left, key);
    return this._search(node.right, key);
  }

  get(key: K): V | null {
    const node = this._search(this._root, key);
    return node ? node.value : null;
  }

  min(): K | null {
    if (!this._root) return null;
    return this._minNode(this._root).key;
  }

  max(): K | null {
    if (!this._root) return null;
    let node = this._root;
    while (node.right) {
      node = node.right;
    }
    return node.key;
  }

  inOrder(): K[] {
    const result: K[] = [];
    this._inOrder(this._root, result);
    return result;
  }

  private _inOrder(node: AVLNode<K, V> | null, result: K[]): void {
    if (node) {
      this._inOrder(node.left, result);
      result.push(node.key);
      this._inOrder(node.right, result);
    }
  }

  height(): number {
    return this._height(this._root);
  }

  get size(): number {
    return this._size;
  }

  isEmpty(): boolean {
    return this._size === 0;
  }

  isBalanced(): boolean {
    return this._isBalanced(this._root)[0];
  }

  private _isBalanced(
    node: AVLNode<K, V> | null,
  ): [boolean, number] {
    if (!node) return [true, 0];

    const [leftBalanced, leftHeight] = this._isBalanced(node.left);
    const [rightBalanced, rightHeight] = this._isBalanced(node.right);

    const balanced =
      leftBalanced &&
      rightBalanced &&
      Math.abs(leftHeight - rightHeight) <= 1;

    return [balanced, 1 + Math.max(leftHeight, rightHeight)];
  }

  clear(): void {
    this._root = null;
    this._size = 0;
  }
}

export class AVLTreeMap<K, V> {
  private _tree: AVLTree<K, V>;

  constructor(compare?: Comparator<K>) {
    this._tree = new AVLTree<K, V>(compare);
  }

  set(key: K, value: V): void {
    this._tree.insert(key, value);
  }

  get(key: K): V | null {
    return this._tree.get(key);
  }

  delete(key: K): boolean {
    return this._tree.delete(key);
  }

  has(key: K): boolean {
    return this._tree.search(key);
  }

  keys(): K[] {
    return this._tree.inOrder();
  }

  values(): V[] {
    const result: V[] = [];
    this._collectValues((this._tree as any)._root, result);
    return result;
  }

  private _collectValues(
    node: AVLNode<K, V> | null,
    result: V[],
  ): void {
    if (node) {
      this._collectValues(node.left, result);
      result.push(node.value);
      this._collectValues(node.right, result);
    }
  }

  entries(): [K, V][] {
    const result: [K, V][] = [];
    this._collectEntries((this._tree as any)._root, result);
    return result;
  }

  private _collectEntries(
    node: AVLNode<K, V> | null,
    result: [K, V][],
  ): void {
    if (node) {
      this._collectEntries(node.left, result);
      result.push([node.key, node.value]);
      this._collectEntries(node.right, result);
    }
  }

  get size(): number {
    return this._tree.size;
  }

  forEach(callback: (value: V, key: K) => void): void {
    const entries = this.entries();
    for (const [key, value] of entries) {
      callback(value, key);
    }
  }
}
