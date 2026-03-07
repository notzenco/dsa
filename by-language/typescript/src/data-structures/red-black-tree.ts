/**
 * Red-Black Tree Data Structure
 *
 * A self-balancing binary search tree with the following properties:
 *   1. Every node is either RED or BLACK
 *   2. Root is always BLACK
 *   3. All leaves (NIL) are BLACK
 *   4. If a node is RED, both its children are BLACK
 *   5. Every path from root to leaves has the same number of BLACK nodes
 *
 * COMPLEXITY:
 * | Operation | Average  | Worst    |
 * |-----------|----------|----------|
 * | Search    | O(log n) | O(log n) |
 * | Insert    | O(log n) | O(log n) |
 * | Delete    | O(log n) | O(log n) |
 * | Space     | O(n)     | O(n)     |
 */

export enum Color {
  RED = 0,
  BLACK = 1,
}

export class RBNode<T> {
  key: T;
  color: Color;
  parent: RBNode<T> | null;
  left: RBNode<T> | null;
  right: RBNode<T> | null;

  constructor(
    key: T,
    color: Color = Color.RED,
    parent: RBNode<T> | null = null,
  ) {
    this.key = key;
    this.color = color;
    this.parent = parent;
    this.left = null;
    this.right = null;
  }
}

export type Comparator<T> = (a: T, b: T) => number;

function defaultCompare<T>(a: T, b: T): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

export class RedBlackTree<T> {
  private _root: RBNode<T> | null = null;
  private _size: number = 0;
  private _compare: Comparator<T>;

  constructor(compare?: Comparator<T>) {
    this._compare = compare ?? defaultCompare;
  }

  get size(): number {
    return this._size;
  }

  isEmpty(): boolean {
    return this._root === null;
  }

  // =========================================================================
  // SEARCH
  // =========================================================================

  private _findNode(key: T): RBNode<T> | null {
    let node = this._root;
    while (node !== null) {
      const cmp = this._compare(key, node.key);
      if (cmp < 0) {
        node = node.left;
      } else if (cmp > 0) {
        node = node.right;
      } else {
        return node;
      }
    }
    return null;
  }

  search(key: T): T | null {
    const node = this._findNode(key);
    return node ? node.key : null;
  }

  contains(key: T): boolean {
    return this._findNode(key) !== null;
  }

  // =========================================================================
  // ROTATIONS
  // =========================================================================

  private _rotateLeft(x: RBNode<T>): void {
    const y = x.right;
    if (y === null) return;

    x.right = y.left;
    if (y.left !== null) {
      y.left.parent = x;
    }

    y.parent = x.parent;
    if (x.parent === null) {
      this._root = y;
    } else if (x === x.parent.left) {
      x.parent.left = y;
    } else {
      x.parent.right = y;
    }

    y.left = x;
    x.parent = y;
  }

  private _rotateRight(y: RBNode<T>): void {
    const x = y.left;
    if (x === null) return;

    y.left = x.right;
    if (x.right !== null) {
      x.right.parent = y;
    }

    x.parent = y.parent;
    if (y.parent === null) {
      this._root = x;
    } else if (y === y.parent.right) {
      y.parent.right = x;
    } else {
      y.parent.left = x;
    }

    x.right = y;
    y.parent = x;
  }

  // =========================================================================
  // INSERTION
  // =========================================================================

  insert(key: T): void {
    const newNode = new RBNode<T>(key, Color.RED);

    let parent: RBNode<T> | null = null;
    let current = this._root;

    while (current !== null) {
      parent = current;
      const cmp = this._compare(key, current.key);
      if (cmp < 0) {
        current = current.left;
      } else if (cmp > 0) {
        current = current.right;
      } else {
        // Key exists, update
        current.key = key;
        return;
      }
    }

    newNode.parent = parent;

    if (parent === null) {
      this._root = newNode;
    } else if (this._compare(key, parent.key) < 0) {
      parent.left = newNode;
    } else {
      parent.right = newNode;
    }

    this._size++;
    this.insertFixup(newNode);
  }

  insertFixup(z: RBNode<T>): void {
    while (z.parent !== null && z.parent.color === Color.RED) {
      if (z.parent.parent === null) break;

      if (z.parent === z.parent.parent.left) {
        const uncle = z.parent.parent.right;

        if (uncle !== null && uncle.color === Color.RED) {
          // Case 1: Uncle is red
          z.parent.color = Color.BLACK;
          uncle.color = Color.BLACK;
          z.parent.parent.color = Color.RED;
          z = z.parent.parent;
        } else {
          if (z === z.parent.right) {
            // Case 2: z is right child
            z = z.parent;
            this._rotateLeft(z);
          }
          // Case 3: z is left child
          if (z.parent !== null) {
            z.parent.color = Color.BLACK;
            if (z.parent.parent !== null) {
              z.parent.parent.color = Color.RED;
              this._rotateRight(z.parent.parent);
            }
          }
        }
      } else {
        // Mirror cases: parent is right child of grandparent
        const uncle = z.parent.parent.left;

        if (uncle !== null && uncle.color === Color.RED) {
          z.parent.color = Color.BLACK;
          uncle.color = Color.BLACK;
          z.parent.parent.color = Color.RED;
          z = z.parent.parent;
        } else {
          if (z === z.parent.left) {
            z = z.parent;
            this._rotateRight(z);
          }
          if (z.parent !== null) {
            z.parent.color = Color.BLACK;
            if (z.parent.parent !== null) {
              z.parent.parent.color = Color.RED;
              this._rotateLeft(z.parent.parent);
            }
          }
        }
      }
    }

    if (this._root !== null) {
      this._root.color = Color.BLACK;
    }
  }

  // =========================================================================
  // DELETION
  // =========================================================================

  private _transplant(
    u: RBNode<T>,
    v: RBNode<T> | null,
  ): void {
    if (u.parent === null) {
      this._root = v;
    } else if (u === u.parent.left) {
      u.parent.left = v;
    } else {
      u.parent.right = v;
    }
    if (v !== null) {
      v.parent = u.parent;
    }
  }

  private _minimum(node: RBNode<T>): RBNode<T> {
    let current = node;
    while (current.left !== null) {
      current = current.left;
    }
    return current;
  }

  delete(key: T): boolean {
    const z = this._findNode(key);
    if (z === null) return false;

    let y = z;
    let yOriginalColor = y.color;
    let x: RBNode<T> | null = null;
    let xParent: RBNode<T> | null = null;

    if (z.left === null) {
      x = z.right;
      xParent = z.parent;
      this._transplant(z, z.right);
    } else if (z.right === null) {
      x = z.left;
      xParent = z.parent;
      this._transplant(z, z.left);
    } else {
      y = this._minimum(z.right);
      yOriginalColor = y.color;
      x = y.right;

      if (y.parent === z) {
        xParent = y;
      } else {
        xParent = y.parent;
        this._transplant(y, y.right);
        y.right = z.right;
        if (y.right !== null) {
          y.right.parent = y;
        }
      }

      this._transplant(z, y);
      y.left = z.left;
      if (y.left !== null) {
        y.left.parent = y;
      }
      y.color = z.color;
    }

    this._size--;

    if (yOriginalColor === Color.BLACK) {
      this.deleteFixup(x, xParent);
    }

    return true;
  }

  deleteFixup(
    x: RBNode<T> | null,
    xParent: RBNode<T> | null,
  ): void {
    while (x !== this._root && (x === null || x.color === Color.BLACK)) {
      if (xParent === null) break;

      if (x === xParent.left) {
        let w = xParent.right;

        if (w !== null && w.color === Color.RED) {
          // Case 1: Sibling is red
          w.color = Color.BLACK;
          xParent.color = Color.RED;
          this._rotateLeft(xParent);
          w = xParent.right;
        }

        if (w === null) {
          x = xParent;
          xParent = x.parent;
          continue;
        }

        const leftBlack = w.left === null || w.left.color === Color.BLACK;
        const rightBlack = w.right === null || w.right.color === Color.BLACK;

        if (leftBlack && rightBlack) {
          // Case 2: Both sibling's children are black
          w.color = Color.RED;
          x = xParent;
          xParent = x.parent;
        } else {
          if (rightBlack) {
            // Case 3: Sibling's right child is black
            if (w.left !== null) {
              w.left.color = Color.BLACK;
            }
            w.color = Color.RED;
            this._rotateRight(w);
            w = xParent.right;
          }

          // Case 4: Sibling's right child is red
          if (w !== null) {
            w.color = xParent.color;
            if (w.right !== null) {
              w.right.color = Color.BLACK;
            }
          }
          xParent.color = Color.BLACK;
          this._rotateLeft(xParent);
          x = this._root;
          break;
        }
      } else {
        // Mirror cases
        let w = xParent.left;

        if (w !== null && w.color === Color.RED) {
          w.color = Color.BLACK;
          xParent.color = Color.RED;
          this._rotateRight(xParent);
          w = xParent.left;
        }

        if (w === null) {
          x = xParent;
          xParent = x.parent;
          continue;
        }

        const leftBlack = w.left === null || w.left.color === Color.BLACK;
        const rightBlack = w.right === null || w.right.color === Color.BLACK;

        if (leftBlack && rightBlack) {
          w.color = Color.RED;
          x = xParent;
          xParent = x.parent;
        } else {
          if (leftBlack) {
            if (w.right !== null) {
              w.right.color = Color.BLACK;
            }
            w.color = Color.RED;
            this._rotateLeft(w);
            w = xParent.left;
          }

          if (w !== null) {
            w.color = xParent.color;
            if (w.left !== null) {
              w.left.color = Color.BLACK;
            }
          }
          xParent.color = Color.BLACK;
          this._rotateRight(xParent);
          x = this._root;
          break;
        }
      }
    }

    if (x !== null) {
      x.color = Color.BLACK;
    }
  }

  // =========================================================================
  // TRAVERSAL & UTILITY
  // =========================================================================

  inOrder(): T[] {
    const result: T[] = [];
    const traverse = (node: RBNode<T> | null): void => {
      if (node !== null) {
        traverse(node.left);
        result.push(node.key);
        traverse(node.right);
      }
    };
    traverse(this._root);
    return result;
  }

  min(): T | null {
    if (this._root === null) return null;
    return this._minimum(this._root).key;
  }

  max(): T | null {
    if (this._root === null) return null;
    let node = this._root;
    while (node.right !== null) {
      node = node.right;
    }
    return node.key;
  }

  height(): number {
    const calcHeight = (node: RBNode<T> | null): number => {
      if (node === null) return 0;
      return 1 + Math.max(calcHeight(node.left), calcHeight(node.right));
    };
    return calcHeight(this._root);
  }

  blackHeight(): number {
    let count = 0;
    let node = this._root;
    while (node !== null) {
      if (node.color === Color.BLACK) count++;
      node = node.left;
    }
    return count;
  }

  isValid(): boolean {
    if (this._root === null) return true;

    // Property 2: Root is black
    if (this._root.color !== Color.BLACK) return false;

    const pathBlack: number[] = [];

    const check = (
      node: RBNode<T> | null,
      blackCount: number,
    ): boolean => {
      if (node === null) {
        // Property 5: All paths have same black count
        if (pathBlack.length === 0) {
          pathBlack.push(blackCount);
        }
        return blackCount === pathBlack[0];
      }

      // Property 4: Red node has black children
      if (node.color === Color.RED) {
        if (node.left !== null && node.left.color === Color.RED) return false;
        if (node.right !== null && node.right.color === Color.RED) return false;
      }

      if (node.color === Color.BLACK) {
        blackCount++;
      }

      return (
        check(node.left, blackCount) && check(node.right, blackCount)
      );
    };

    return check(this._root, 0);
  }

  clear(): void {
    this._root = null;
    this._size = 0;
  }
}
