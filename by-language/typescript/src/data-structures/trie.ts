/**
 * Trie (Prefix Tree) Implementation
 *
 * A tree-like data structure for storing strings that allows
 * efficient prefix-based operations.
 *
 * COMPLEXITY:
 * | Operation           | Time   | Space |
 * |---------------------|--------|-------|
 * | Insert              | O(m)   | O(m)  |
 * | Search              | O(m)   | O(1)  |
 * | Starts With         | O(m)   | O(1)  |
 * | Delete              | O(m)   | O(1)  |
 * | Get All With Prefix | O(m+k) | O(k)  |
 *
 * Where m = length of word/prefix, k = total characters in matching words
 */

export class TrieNode {
  children: Map<string, TrieNode>;
  isEnd: boolean;
  value: unknown;
  count: number;

  constructor() {
    this.children = new Map();
    this.isEnd = false;
    this.value = null;
    this.count = 0;
  }
}

export class Trie {
  private _root: TrieNode;
  private _wordCount: number = 0;

  constructor() {
    this._root = new TrieNode();
  }

  insert(word: string): void {
    if (this.search(word)) return;
    let node = this._root;
    for (const char of word) {
      node.count++;
      if (!node.children.has(char)) {
        node.children.set(char, new TrieNode());
      }
      node = node.children.get(char)!;
    }

    if (!node.isEnd) {
      this._wordCount++;
    }
    node.isEnd = true;
    node.count++;
  }

  search(word: string): boolean {
    const node = this._findNode(word);
    return node !== null && node.isEnd;
  }

  startsWith(prefix: string): boolean {
    return this._findNode(prefix) !== null;
  }

  delete(word: string): boolean {
    if (!this.search(word)) return false;

    let node = this._root;
    const path: [TrieNode, string][] = [];

    for (const char of word) {
      path.push([node, char]);
      node.count--;
      node = node.children.get(char)!;
    }

    node.count--;
    node.isEnd = false;
    this._wordCount--;

    // Clean up unused nodes
    for (let i = path.length - 1; i >= 0; i--) {
      const [parent, char] = path[i];
      const child = parent.children.get(char)!;
      if (child.count === 0) {
        parent.children.delete(char);
      } else {
        break;
      }
    }

    return true;
  }

  getAllWords(): string[] {
    const words: string[] = [];
    this._collectWords(this._root, "", words);
    return words;
  }

  autocomplete(prefix: string, limit: number = 10): string[] {
    const node = this._findNode(prefix);
    if (node === null) return [];

    const words: string[] = [];
    // BFS for shortest words first
    const queue: [TrieNode, string][] = [[node, prefix]];
    let front = 0;

    while (front < queue.length && words.length < limit) {
      const [currNode, currPrefix] = queue[front++];

      if (currNode.isEnd) {
        words.push(currPrefix);
      }

      const sortedKeys = [...currNode.children.keys()].sort();
      for (const char of sortedKeys) {
        if (words.length >= limit) break;
        queue.push([currNode.children.get(char)!, currPrefix + char]);
      }
    }

    return words;
  }

  countWordsWithPrefix(prefix: string): number {
    const node = this._findNode(prefix);
    return node ? node.count : 0;
  }

  get size(): number {
    return this._wordCount;
  }

  isEmpty(): boolean {
    return this._wordCount === 0;
  }

  private _findNode(prefix: string): TrieNode | null {
    let node = this._root;
    for (const char of prefix) {
      if (!node.children.has(char)) return null;
      node = node.children.get(char)!;
    }
    return node;
  }

  private _collectWords(
    node: TrieNode,
    prefix: string,
    words: string[],
  ): void {
    if (node.isEnd) {
      words.push(prefix);
    }
    for (const [char, child] of node.children) {
      this._collectWords(child, prefix + char, words);
    }
  }
}

export class TrieMap<V> {
  private _root: TrieNode;
  private _size: number = 0;

  constructor() {
    this._root = new TrieNode();
  }

  insert(word: string, value: V): void {
    let node = this._root;
    for (const char of word) {
      if (!node.children.has(char)) {
        node.children.set(char, new TrieNode());
      }
      node = node.children.get(char)!;
    }

    if (!node.isEnd) {
      this._size++;
    }
    node.isEnd = true;
    node.value = value;
  }

  search(word: string): boolean {
    const node = this._findNode(word);
    return node !== null && node.isEnd;
  }

  get(word: string): V | null {
    const node = this._findNode(word);
    if (node === null || !node.isEnd) return null;
    return node.value as V;
  }

  delete(word: string): boolean {
    if (!this.search(word)) return false;

    let node = this._root;
    const path: [TrieNode, string][] = [];

    for (const char of word) {
      path.push([node, char]);
      node = node.children.get(char)!;
    }

    node.isEnd = false;
    node.value = null;
    this._size--;

    // Clean up unused nodes
    for (let i = path.length - 1; i >= 0; i--) {
      const [parent, char] = path[i];
      const child = parent.children.get(char)!;
      if (!child.isEnd && child.children.size === 0) {
        parent.children.delete(char);
      } else {
        break;
      }
    }

    return true;
  }

  getAllEntries(): [string, V][] {
    const entries: [string, V][] = [];
    this._collectEntries(this._root, "", entries);
    return entries;
  }

  get size(): number {
    return this._size;
  }

  isEmpty(): boolean {
    return this._size === 0;
  }

  private _findNode(prefix: string): TrieNode | null {
    let node = this._root;
    for (const char of prefix) {
      if (!node.children.has(char)) return null;
      node = node.children.get(char)!;
    }
    return node;
  }

  private _collectEntries(
    node: TrieNode,
    prefix: string,
    entries: [string, V][],
  ): void {
    if (node.isEnd) {
      entries.push([prefix, node.value as V]);
    }
    for (const [char, child] of node.children) {
      this._collectEntries(child, prefix + char, entries);
    }
  }
}

export class WordDictionary {
  private _root: TrieNode;

  constructor() {
    this._root = new TrieNode();
  }

  insert(word: string): void {
    let node = this._root;
    for (const char of word) {
      if (!node.children.has(char)) {
        node.children.set(char, new TrieNode());
      }
      node = node.children.get(char)!;
    }
    node.isEnd = true;
  }

  search(word: string): boolean {
    return this._searchHelper(Array.from(word), 0, this._root);
  }

  private _searchHelper(
    word: string[],
    index: number,
    node: TrieNode,
  ): boolean {
    if (index === word.length) {
      return node.isEnd;
    }

    const char = word[index];

    if (char === ".") {
      // Try all children
      for (const child of node.children.values()) {
        if (this._searchHelper(word, index + 1, child)) {
          return true;
        }
      }
      return false;
    } else {
      if (!node.children.has(char)) return false;
      return this._searchHelper(word, index + 1, node.children.get(char)!);
    }
  }
}
