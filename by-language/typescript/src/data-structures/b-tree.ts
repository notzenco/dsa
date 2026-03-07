/**
 * B-Tree Data Structure
 *
 * A self-balancing tree optimized for disk access, where each node can
 * contain multiple keys and children.
 *
 * B-Tree of minimum degree t:
 *   - Every node has at most 2t - 1 keys
 *   - Every node has at most 2t children
 *   - Every non-root node has at least t - 1 keys
 *   - Root has at least 1 key (if non-empty)
 *   - All leaves are at the same level
 *
 * COMPLEXITY:
 * | Operation | Time             | Disk Accesses |
 * |-----------|------------------|---------------|
 * | Search    | O(t * log_t(n))  | O(log_t(n))   |
 * | Insert    | O(t * log_t(n))  | O(log_t(n))   |
 * | Delete    | O(t * log_t(n))  | O(log_t(n))   |
 * | Space     | O(n)             | -             |
 */

export class BTreeNode {
  keys: number[];
  children: BTreeNode[];
  leaf: boolean;

  constructor(leaf: boolean = true) {
    this.keys = [];
    this.children = [];
    this.leaf = leaf;
  }
}

export class BTree {
  private _root: BTreeNode;
  private _t: number;
  private _size: number = 0;

  constructor(t: number = 2) {
    if (t < 2) {
      throw new Error("Minimum degree t must be at least 2");
    }
    this._t = t;
    this._root = new BTreeNode(true);
  }

  get minDegree(): number {
    return this._t;
  }

  get size(): number {
    return this._size;
  }

  isEmpty(): boolean {
    return this._size === 0;
  }

  // =========================================================================
  // SEARCH
  // =========================================================================

  search(key: number): boolean {
    return this._search(this._root, key);
  }

  private _search(node: BTreeNode, key: number): boolean {
    let i = 0;
    while (i < node.keys.length && key > node.keys[i]) {
      i++;
    }

    if (i < node.keys.length && key === node.keys[i]) {
      return true;
    }

    if (node.leaf) return false;

    return this._search(node.children[i], key);
  }

  // =========================================================================
  // INSERTION
  // =========================================================================

  insert(key: number): void {
    const root = this._root;

    if (root.keys.length === 2 * this._t - 1) {
      const newRoot = new BTreeNode(false);
      newRoot.children.push(this._root);
      this.splitChild(newRoot, 0);
      this._root = newRoot;
    }

    this._insertNonFull(this._root, key);
    this._size++;
  }

  splitChild(parent: BTreeNode, i: number): void {
    const t = this._t;
    const fullChild = parent.children[i];

    const originalKeys = [...fullChild.keys];
    const originalChildren = [...fullChild.children];

    const medianKey = originalKeys[t - 1];

    const newNode = new BTreeNode(fullChild.leaf);

    // Left child keeps first t-1 keys
    fullChild.keys = originalKeys.slice(0, t - 1);

    // New node gets keys after median
    newNode.keys = originalKeys.slice(t);

    // Move children if not leaf
    if (!fullChild.leaf) {
      fullChild.children = originalChildren.slice(0, t);
      newNode.children = originalChildren.slice(t);
    }

    // Insert median into parent
    parent.keys.splice(i, 0, medianKey);
    parent.children.splice(i + 1, 0, newNode);
  }

  private _insertNonFull(node: BTreeNode, key: number): void {
    let i = node.keys.length - 1;

    if (node.leaf) {
      // Find position and insert
      while (i >= 0 && key < node.keys[i]) {
        i--;
      }

      // Check for duplicate
      if (i >= 0 && node.keys[i] === key) {
        this._size--; // Compensate
        return;
      }

      node.keys.splice(i + 1, 0, key);
    } else {
      // Find child to recurse into
      while (i >= 0 && key < node.keys[i]) {
        i--;
      }

      // Check for duplicate in this node
      if (i >= 0 && node.keys[i] === key) {
        this._size--;
        return;
      }

      i++;

      // Split child if full
      if (node.children[i].keys.length === 2 * this._t - 1) {
        this.splitChild(node, i);
        if (key > node.keys[i]) {
          i++;
        } else if (key === node.keys[i]) {
          this._size--;
          return;
        }
      }

      this._insertNonFull(node.children[i], key);
    }
  }

  // =========================================================================
  // DELETION
  // =========================================================================

  delete(key: number): boolean {
    if (!this.search(key)) return false;

    this._delete(this._root, key);
    this._size--;

    // If root is empty but has children, make first child new root
    if (this._root.keys.length === 0 && !this._root.leaf) {
      this._root = this._root.children[0];
    }

    return true;
  }

  private _delete(node: BTreeNode, key: number): void {
    const t = this._t;
    let i = 0;

    while (i < node.keys.length && key > node.keys[i]) {
      i++;
    }

    if (node.leaf) {
      // Case 1: Key is in leaf node
      if (i < node.keys.length && node.keys[i] === key) {
        node.keys.splice(i, 1);
      }
      return;
    }

    if (i < node.keys.length && node.keys[i] === key) {
      // Case 2: Key is in internal node
      this._deleteInternal(node, i);
    } else {
      // Case 3: Key is in subtree
      this._deleteSubtree(node, i, key);
    }
  }

  private _deleteInternal(node: BTreeNode, i: number): void {
    const t = this._t;
    const key = node.keys[i];

    if (node.children[i].keys.length >= t) {
      // Case 2a: Left child has enough keys
      const pred = this._getPredecessor(node, i);
      node.keys[i] = pred;
      this._delete(node.children[i], pred);
    } else if (node.children[i + 1].keys.length >= t) {
      // Case 2b: Right child has enough keys
      const succ = this._getSuccessor(node, i);
      node.keys[i] = succ;
      this._delete(node.children[i + 1], succ);
    } else {
      // Case 2c: Both children have t-1 keys, merge
      this._merge(node, i);
      this._delete(node.children[i], key);
    }
  }

  private _deleteSubtree(
    node: BTreeNode,
    i: number,
    key: number,
  ): void {
    const t = this._t;

    if (node.children[i].keys.length < t) {
      if (i > 0 && node.children[i - 1].keys.length >= t) {
        this._borrowFromLeft(node, i);
      } else if (
        i < node.children.length - 1 &&
        node.children[i + 1].keys.length >= t
      ) {
        this._borrowFromRight(node, i);
      } else {
        if (i < node.children.length - 1) {
          this._merge(node, i);
        } else {
          this._merge(node, i - 1);
          i--;
        }
      }
    }

    this._delete(node.children[i], key);
  }

  private _getPredecessor(node: BTreeNode, i: number): number {
    let current = node.children[i];
    while (!current.leaf) {
      current = current.children[current.children.length - 1];
    }
    return current.keys[current.keys.length - 1];
  }

  private _getSuccessor(node: BTreeNode, i: number): number {
    let current = node.children[i + 1];
    while (!current.leaf) {
      current = current.children[0];
    }
    return current.keys[0];
  }

  private _borrowFromLeft(node: BTreeNode, i: number): void {
    const child = node.children[i];
    const leftSibling = node.children[i - 1];

    // Move key from parent to child
    child.keys.unshift(node.keys[i - 1]);

    // Move key from sibling to parent
    node.keys[i - 1] = leftSibling.keys.pop()!;

    // Move child pointer if not leaf
    if (!child.leaf) {
      child.children.unshift(leftSibling.children.pop()!);
    }
  }

  private _borrowFromRight(node: BTreeNode, i: number): void {
    const child = node.children[i];
    const rightSibling = node.children[i + 1];

    // Move key from parent to child
    child.keys.push(node.keys[i]);

    // Move key from sibling to parent
    node.keys[i] = rightSibling.keys.shift()!;

    // Move child pointer if not leaf
    if (!child.leaf) {
      child.children.push(rightSibling.children.shift()!);
    }
  }

  private _merge(node: BTreeNode, i: number): void {
    const left = node.children[i];
    const right = node.children[i + 1];

    // Move separator key from parent to left child
    left.keys.push(node.keys[i]);

    // Move all keys from right to left
    left.keys.push(...right.keys);

    // Move children if not leaf
    if (!left.leaf) {
      left.children.push(...right.children);
    }

    // Remove separator and right child from parent
    node.keys.splice(i, 1);
    node.children.splice(i + 1, 1);
  }

  // =========================================================================
  // TRAVERSAL & UTILITY
  // =========================================================================

  traverse(): number[] {
    const result: number[] = [];
    this._inOrder(this._root, result);
    return result;
  }

  private _inOrder(node: BTreeNode, result: number[]): void {
    for (let i = 0; i < node.keys.length; i++) {
      if (!node.leaf) {
        this._inOrder(node.children[i], result);
      }
      result.push(node.keys[i]);
    }

    if (!node.leaf && node.children.length > 0) {
      this._inOrder(node.children[node.children.length - 1], result);
    }
  }

  height(): number {
    let h = 0;
    let node = this._root;
    while (!node.leaf) {
      h++;
      node = node.children[0];
    }
    return h;
  }

  min(): number | null {
    if (this._root.keys.length === 0) return null;

    let node = this._root;
    while (!node.leaf) {
      node = node.children[0];
    }
    return node.keys[0];
  }

  max(): number | null {
    if (this._root.keys.length === 0) return null;

    let node = this._root;
    while (!node.leaf) {
      node = node.children[node.children.length - 1];
    }
    return node.keys[node.keys.length - 1];
  }

  clear(): void {
    this._root = new BTreeNode(true);
    this._size = 0;
  }
}
