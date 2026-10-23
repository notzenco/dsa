/**
 * Binary Search Tree (BST)
 *
 * BST Property: For each node:
 *   - All values in left subtree < node value
 *   - All values in right subtree > node value
 *
 * Keys are unique. Inserting a value already present returns that node
 * and leaves the size and shape unchanged.
 *
 * COMPLEXITY:
 * | Operation          | Average   | Worst  | Space |
 * |--------------------|-----------|--------|-------|
 * | Search             | O(log n)  | O(n)   | O(1)  |
 * | Insert             | O(log n)  | O(n)   | O(1)  |
 * | Delete             | O(log n)  | O(n)   | O(1)  |
 * | Min/Max            | O(log n)  | O(n)   | O(1)  |
 * | In-order traversal | O(n)      | O(n)   | O(h)  |
 */

export class TreeNode<T> {
  value: T;
  left: TreeNode<T> | null;
  right: TreeNode<T> | null;
  parent: TreeNode<T> | null;

  constructor(
    value: T,
    left: TreeNode<T> | null = null,
    right: TreeNode<T> | null = null,
    parent: TreeNode<T> | null = null,
  ) {
    this.value = value;
    this.left = left;
    this.right = right;
    this.parent = parent;
  }

  get isLeaf(): boolean {
    return this.left === null && this.right === null;
  }

  get hasLeftChild(): boolean {
    return this.left !== null;
  }

  get hasRightChild(): boolean {
    return this.right !== null;
  }

  get isLeftChild(): boolean {
    return this.parent !== null && this.parent.left === this;
  }

  get isRightChild(): boolean {
    return this.parent !== null && this.parent.right === this;
  }
}

export type Comparator<T> = (a: T, b: T) => number;

function defaultCompare<T>(a: T, b: T): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

export class BinarySearchTree<T> {
  private _root: TreeNode<T> | null = null;
  private _size: number = 0;
  private _compare: Comparator<T>;

  constructor(compare?: Comparator<T>) {
    this._compare = compare ?? defaultCompare;
  }

  get size(): number {
    return this._size;
  }

  get root(): T | null {
    return this._root ? this._root.value : null;
  }

  isEmpty(): boolean {
    return this._root === null;
  }

  get height(): number {
    return this._heightOf(this._root);
  }

  private _heightOf(node: TreeNode<T> | null): number {
    if (node === null) return -1;
    return 1 + Math.max(this._heightOf(node.left), this._heightOf(node.right));
  }

  insert(value: T): TreeNode<T> {
    if (this._root === null) {
      const node = new TreeNode(value);
      this._root = node;
      this._size++;
      return node;
    }

    let current: TreeNode<T> = this._root;
    while (true) {
      const cmp = this._compare(value, current.value);
      if (cmp === 0) {
        return current;
      }
      if (cmp < 0) {
        if (current.left === null) {
          const node = new TreeNode(value);
          current.left = node;
          node.parent = current;
          this._size++;
          return node;
        }
        current = current.left;
      } else {
        if (current.right === null) {
          const node = new TreeNode(value);
          current.right = node;
          node.parent = current;
          this._size++;
          return node;
        }
        current = current.right;
      }
    }
  }

  search(value: T): TreeNode<T> | null {
    let current = this._root;
    while (current !== null) {
      const cmp = this._compare(value, current.value);
      if (cmp === 0) return current;
      if (cmp < 0) {
        current = current.left;
      } else {
        current = current.right;
      }
    }
    return null;
  }

  contains(value: T): boolean {
    return this.search(value) !== null;
  }

  delete(value: T): boolean {
    const node = this.search(value);
    if (node === null) return false;
    this._deleteNode(node);
    this._size--;
    return true;
  }

  private _deleteNode(node: TreeNode<T>): void {
    if (node.isLeaf) {
      this._replaceNode(node, null);
    } else if (node.left === null) {
      this._replaceNode(node, node.right);
    } else if (node.right === null) {
      this._replaceNode(node, node.left);
    } else {
      const successor = this._minimumNode(node.right);
      if (successor.parent !== node) {
        this._replaceNode(successor, successor.right);
        successor.right = node.right;
        successor.right.parent = successor;
      }
      this._replaceNode(node, successor);
      successor.left = node.left;
      successor.left.parent = successor;
    }
  }

  private _replaceNode(
    node: TreeNode<T>,
    replacement: TreeNode<T> | null,
  ): void {
    if (node.parent === null) {
      this._root = replacement;
    } else if (node.isLeftChild) {
      node.parent.left = replacement;
    } else {
      node.parent.right = replacement;
    }
    if (replacement !== null) {
      replacement.parent = node.parent;
    }
  }

  min(): T | null {
    if (this._root === null) return null;
    return this._minimumNode(this._root).value;
  }

  max(): T | null {
    if (this._root === null) return null;
    return this._maximumNode(this._root).value;
  }

  private _minimumNode(node: TreeNode<T>): TreeNode<T> {
    let current = node;
    while (current.left !== null) {
      current = current.left;
    }
    return current;
  }

  private _maximumNode(node: TreeNode<T>): TreeNode<T> {
    let current = node;
    while (current.right !== null) {
      current = current.right;
    }
    return current;
  }

  successor(value: T): T | null {
    const node = this.search(value);
    if (node === null) return null;

    if (node.right !== null) {
      return this._minimumNode(node.right).value;
    }

    let current: TreeNode<T> = node;
    let parent = node.parent;
    while (parent !== null && current.isRightChild) {
      current = parent;
      parent = parent.parent;
    }
    return parent ? parent.value : null;
  }

  predecessor(value: T): T | null {
    const node = this.search(value);
    if (node === null) return null;

    if (node.left !== null) {
      return this._maximumNode(node.left).value;
    }

    let current: TreeNode<T> = node;
    let parent = node.parent;
    while (parent !== null && current.isLeftChild) {
      current = parent;
      parent = parent.parent;
    }
    return parent ? parent.value : null;
  }

  inOrder(): T[] {
    const result: T[] = [];
    const stack: TreeNode<T>[] = [];
    let current = this._root;

    while (stack.length > 0 || current !== null) {
      while (current !== null) {
        stack.push(current);
        current = current.left;
      }
      current = stack.pop()!;
      result.push(current.value);
      current = current.right;
    }
    return result;
  }

  preOrder(): T[] {
    const result: T[] = [];
    if (this._root === null) return result;

    const stack: TreeNode<T>[] = [this._root];
    while (stack.length > 0) {
      const node = stack.pop()!;
      result.push(node.value);
      if (node.right) stack.push(node.right);
      if (node.left) stack.push(node.left);
    }
    return result;
  }

  postOrder(): T[] {
    if (this._root === null) return [];

    const stack1: TreeNode<T>[] = [this._root];
    const stack2: TreeNode<T>[] = [];

    while (stack1.length > 0) {
      const node = stack1.pop()!;
      stack2.push(node);
      if (node.left) stack1.push(node.left);
      if (node.right) stack1.push(node.right);
    }

    const result: T[] = [];
    while (stack2.length > 0) {
      result.push(stack2.pop()!.value);
    }
    return result;
  }

  levelOrder(): T[] {
    if (this._root === null) return [];

    const result: T[] = [];
    const queue: TreeNode<T>[] = [this._root];
    let front = 0;

    while (front < queue.length) {
      const node = queue[front++];
      result.push(node.value);
      if (node.left) queue.push(node.left);
      if (node.right) queue.push(node.right);
    }

    return result;
  }

  isValid(): boolean {
    const validate = (
      node: TreeNode<T> | null,
      minVal: T | null,
      maxVal: T | null,
    ): boolean => {
      if (node === null) return true;

      if (minVal !== null && this._compare(node.value, minVal) <= 0)
        return false;
      if (maxVal !== null && this._compare(node.value, maxVal) >= 0)
        return false;

      return (
        validate(node.left, minVal, node.value) &&
        validate(node.right, node.value, maxVal)
      );
    };

    return validate(this._root, null, null);
  }

  floor(value: T): T | null {
    let result: T | null = null;
    let current = this._root;

    while (current !== null) {
      const cmp = this._compare(current.value, value);
      if (cmp === 0) return value;
      if (cmp < 0) {
        result = current.value;
        current = current.right;
      } else {
        current = current.left;
      }
    }
    return result;
  }

  ceiling(value: T): T | null {
    let result: T | null = null;
    let current = this._root;

    while (current !== null) {
      const cmp = this._compare(current.value, value);
      if (cmp === 0) return value;
      if (cmp > 0) {
        result = current.value;
        current = current.left;
      } else {
        current = current.right;
      }
    }
    return result;
  }

  rangeQuery(low: T, high: T): T[] {
    const result: T[] = [];

    const traverse = (node: TreeNode<T> | null): void => {
      if (node === null) return;
      if (this._compare(low, node.value) <= 0) traverse(node.left);
      if (
        this._compare(low, node.value) <= 0 &&
        this._compare(node.value, high) <= 0
      ) {
        result.push(node.value);
      }
      if (this._compare(node.value, high) <= 0) traverse(node.right);
    };

    traverse(this._root);
    return result;
  }

  kthSmallest(k: number): T | null {
    if (k < 1 || k > this._size) return null;

    const stack: TreeNode<T>[] = [];
    let current = this._root;
    let count = 0;

    while (stack.length > 0 || current !== null) {
      while (current !== null) {
        stack.push(current);
        current = current.left;
      }
      current = stack.pop()!;
      count++;
      if (count === k) return current.value;
      current = current.right;
    }

    return null;
  }

  /**
   * Build a balanced BST from a sorted array. Repeated values are kept once.
   */
  static fromSortedArray<T>(sortedValues: T[], compare?: Comparator<T>): BinarySearchTree<T> {
    const bst = new BinarySearchTree<T>(compare);
    const values = sortedValues.filter(
      (value, i) => i === 0 || bst._compare(value, sortedValues[i - 1]) !== 0,
    );

    const build = (
      left: number,
      right: number,
      parent: TreeNode<T> | null,
    ): TreeNode<T> | null => {
      if (left > right) return null;

      const mid = Math.floor((left + right) / 2);
      const node = new TreeNode(values[mid]);
      node.parent = parent;
      bst._size++;

      node.left = build(left, mid - 1, node);
      node.right = build(mid + 1, right, node);

      return node;
    };

    bst._root = build(0, values.length - 1, null);
    return bst;
  }

  lowestCommonAncestor(val1: T, val2: T): T | null {
    if (this.search(val1) === null || this.search(val2) === null) return null;

    let current = this._root;
    while (current !== null) {
      const cmp1 = this._compare(val1, current.value);
      const cmp2 = this._compare(val2, current.value);

      if (cmp1 < 0 && cmp2 < 0) {
        current = current.left;
      } else if (cmp1 > 0 && cmp2 > 0) {
        current = current.right;
      } else {
        return current.value;
      }
    }
    return null;
  }

  toArray(): T[] {
    return this.inOrder();
  }

  clear(): void {
    this._root = null;
    this._size = 0;
  }
}
