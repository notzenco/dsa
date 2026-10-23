import { describe, expect, test } from "bun:test";
import {
  binarySearch,
  exponentialSearch,
  interpolationSearch,
  jumpSearch,
  lowerBound,
  ternarySearch,
  upperBound,
} from "../../src/algorithms/searching";
import {
  bubbleSort,
  bucketSort,
  countingSort,
  heapSort,
  insertionSort,
  mergeSort,
  quickSort,
  radixSort,
  selectionSort,
  shellSort,
} from "../../src/algorithms/sorting";
import {
  buildSuffixArray,
  kmpSearch,
  naiveSearch,
  rabinKarp,
  zAlgorithm,
} from "../../src/algorithms/string-algorithms";
import { AVLTree } from "../../src/data-structures/avl-tree";
import { BTree } from "../../src/data-structures/b-tree";
import { BinarySearchTree } from "../../src/data-structures/binary-search-tree";
import { FenwickTree } from "../../src/data-structures/fenwick-tree";
import {
  HashTableChaining,
  HashTableOpenAddressing,
} from "../../src/data-structures/hash-table";
import { MaxHeap, MinHeap } from "../../src/data-structures/heap";
import { LFUCache } from "../../src/data-structures/lfu-cache";
import { LRUCache } from "../../src/data-structures/lru-cache";
import { RedBlackTree } from "../../src/data-structures/red-black-tree";
import {
  SegmentTree,
  SegmentTreeLazy,
} from "../../src/data-structures/segment-tree";
import { SkipList } from "../../src/data-structures/skip-list";
import { Trie } from "../../src/data-structures/trie";
import { UnionFind } from "../../src/data-structures/union-find";
import { mulberry32, randInt } from "./prng";

const SEED = 0x5a17d5a;

type Rng = () => number;

function expectEqual(actual: unknown, expected: unknown, detail: string): void {
  const left = JSON.stringify(actual);
  const right = JSON.stringify(expected);
  if (left !== right) {
    throw new Error(`${detail}\n  actual: ${left}\n  expected: ${right}`);
  }
}

function numericSorted(values: number[]): number[] {
  return [...values].sort((a, b) => a - b);
}

function sumRange(data: number[], lo: number, hi: number): number {
  let total = 0;
  for (let i = lo; i <= hi; i++) total += data[i];
  return total;
}

function bruteSearch(text: string, pattern: string): number[] {
  if (pattern.length === 0 || text.length === 0 || pattern.length > text.length) {
    return [];
  }
  const hits: number[] = [];
  for (let i = 0; i <= text.length - pattern.length; i++) {
    let match = true;
    for (let j = 0; j < pattern.length; j++) {
      if (text[i + j] !== pattern[j]) {
        match = false;
        break;
      }
    }
    if (match) hits.push(i);
  }
  return hits;
}

function naiveSuffixArray(text: string): number[] {
  const order = Array.from({ length: text.length }, (_, i) => i);
  order.sort((a, b) => {
    const left = text.slice(a);
    const right = text.slice(b);
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
  });
  return order;
}

function linearIndex(arr: number[], target: number): number {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] === target) return i;
  }
  return -1;
}

function linearLower(arr: number[], target: number): number {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] >= target) return i;
  }
  return arr.length;
}

function linearUpper(arr: number[], target: number): number {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] > target) return i;
  }
  return arr.length;
}

function expectHit(
  arr: number[],
  target: number,
  index: number,
  detail: string,
): void {
  const found = linearIndex(arr, target);
  if (found === -1) {
    expectEqual(index, -1, detail);
    return;
  }
  if (index < 0 || index >= arr.length || arr[index] !== target) {
    throw new Error(
      `${detail}\n  index ${index} is not a match for ${target} in ${JSON.stringify(arr)}`,
    );
  }
}

function randomArray(next: Rng, length: number, lo: number, hi: number): number[] {
  const values: number[] = [];
  for (let i = 0; i < length; i++) values.push(randInt(next, lo, hi));
  return values;
}

function randomSafe(next: Rng): number {
  const sign = next() < 0.5 ? -1 : 1;
  const hi = Math.floor(next() * 0x200000);
  const lo = Math.floor(next() * 0x100000000);
  return sign * (hi * 0x100000000 + lo);
}

function randomWord(next: Rng, alphabet: string, maxLen: number): string {
  const length = randInt(next, 0, maxLen);
  let word = "";
  for (let i = 0; i < length; i++) {
    word += alphabet[randInt(next, 0, alphabet.length - 1)];
  }
  return word;
}

describe("randomized models", () => {
  test("sorts match numeric sort", () => {
    const next = mulberry32(SEED);
    const sorts = [
      bubbleSort,
      selectionSort,
      insertionSort,
      mergeSort,
      quickSort,
      heapSort,
      shellSort,
      countingSort,
      radixSort,
      bucketSort,
    ];

    for (let i = 0; i < 24; i++) {
      const input = randomArray(next, randInt(next, 0, 36), -40, 40);
      const expected = numericSorted(input);
      for (const sort of sorts) {
        expectEqual(sort(input), expected, `seed=${SEED} iteration=${i} sort input=${JSON.stringify(input)}`);
        const copy = [...input];
        sort(input);
        expectEqual(input, copy, `seed=${SEED} iteration=${i} mutated input`);
      }
    }

    for (let i = 0; i < 8; i++) {
      const input = [randomSafe(next), randomSafe(next), randomSafe(next), randomSafe(next)];
      if (next() < 0.5) input.push(0, -1, 1);
      const expected = numericSorted(input);
      expectEqual(
        countingSort(input),
        expected,
        `seed=${SEED} iteration=${i} counting wide ${JSON.stringify(input)}`,
      );
      expectEqual(
        radixSort(input),
        expected,
        `seed=${SEED} iteration=${i} radix wide ${JSON.stringify(input)}`,
      );
    }
  });

  test("searches match a linear scan", () => {
    const next = mulberry32(SEED + 1);
    for (let i = 0; i < 20; i++) {
      const arr = numericSorted(randomArray(next, randInt(next, 0, 40), -25, 25));
      for (let t = 0; t < 6; t++) {
        const target = next() < 0.7 && arr.length > 0
          ? arr[randInt(next, 0, arr.length - 1)]
          : randInt(next, -30, 30);
        const detail = `seed=${SEED} iteration=${i} target=${target} arr=${JSON.stringify(arr)}`;
        expectEqual(lowerBound(arr, target), linearLower(arr, target), `lowerBound ${detail}`);
        expectEqual(upperBound(arr, target), linearUpper(arr, target), `upperBound ${detail}`);
        expectEqual(jumpSearch(arr, target), linearIndex(arr, target), `jumpSearch ${detail}`);
        expectHit(arr, target, binarySearch(arr, target), `binarySearch ${detail}`);
        expectHit(arr, target, interpolationSearch(arr, target), `interpolationSearch ${detail}`);
        expectHit(arr, target, exponentialSearch(arr, target), `exponentialSearch ${detail}`);
        expectHit(arr, target, ternarySearch(arr, target), `ternarySearch ${detail}`);
      }
    }
  });

  test("string search and suffix array match brute force", () => {
    const next = mulberry32(SEED + 2);
    const alphabet = "abca";
    for (let i = 0; i < 20; i++) {
      const text = randomWord(next, alphabet, 28);
      const pattern = randomWord(next, "abc", 6);
      const expected = bruteSearch(text, pattern);
      const detail = `seed=${SEED} iteration=${i} text=${JSON.stringify(text)} pattern=${JSON.stringify(pattern)}`;
      expectEqual(naiveSearch(text, pattern), expected, `naive ${detail}`);
      expectEqual(kmpSearch(text, pattern), expected, `kmp ${detail}`);
      expectEqual(rabinKarp(text, pattern), expected, `rabin ${detail}`);
      expectEqual(zAlgorithm(text, pattern), expected, `z ${detail}`);
    }

    for (let i = 0; i < 12; i++) {
      const text = randomWord(next, alphabet, 18);
      expectEqual(
        buildSuffixArray(text),
        naiveSuffixArray(text),
        `seed=${SEED} iteration=${i} suffix ${JSON.stringify(text)}`,
      );
    }
  });

  test("ordered trees match a sorted key set", () => {
    const next = mulberry32(SEED + 3);
    for (let i = 0; i < 16; i++) {
      const bst = new BinarySearchTree<number>();
      const avl = new AVLTree<number>();
      const rb = new RedBlackTree<number>();
      const btree = new BTree(randInt(next, 2, 4));
      const skip = new SkipList<number>();
      const keys = new Set<number>();

      const skipRandom = mulberry32(SEED + 300 + i);
      const originalRandom = Math.random;
      Math.random = skipRandom;
      const ops = randInt(next, 20, 40);
      try {
      for (let step = 0; step < ops; step++) {
        const key = randInt(next, 0, 24);
        const detail = `seed=${SEED} iteration=${i} step=${step} key=${key}`;
        if (next() < 0.65) {
          const before = bst.size;
          const node = bst.insert(key);
          avl.insert(key);
          rb.insert(key);
          btree.insert(key);
          skip.insert(key);
          if (keys.has(key)) {
            if (bst.search(key) !== node) {
              throw new Error(`bst duplicate did not return the existing node ${detail}`);
            }
            expectEqual(bst.size, before, `bst duplicate size ${detail}`);
          }
          keys.add(key);
          expect(node.value).toBe(key);
        } else {
          const removed = keys.delete(key);
          expectEqual(bst.delete(key), removed, `bst delete ${detail}`);
          expectEqual(avl.delete(key), removed, `avl delete ${detail}`);
          expectEqual(rb.delete(key), removed, `rb delete ${detail}`);
          expectEqual(btree.delete(key), removed, `btree delete ${detail}`);
          expectEqual(skip.delete(key), removed, `skip delete ${detail}`);
        }

        const sorted = [...keys].sort((a, b) => a - b);
        expectEqual(bst.inOrder(), sorted, `bst inorder ${detail}`);
        expectEqual(bst.size, sorted.length, `bst size ${detail}`);
        expectEqual(bst.isValid(), true, `bst valid ${detail}`);
        expectEqual(avl.inOrder(), sorted, `avl inorder ${detail}`);
        expectEqual(avl.size, sorted.length, `avl size ${detail}`);
        expectEqual(avl.isBalanced(), true, `avl balanced ${detail}`);
        expectEqual(rb.inOrder(), sorted, `rb inorder ${detail}`);
        expectEqual(rb.size, sorted.length, `rb size ${detail}`);
        expectEqual(rb.isValid(), true, `rb valid ${detail}`);
        expectEqual(btree.traverse(), sorted, `btree ${detail}`);
        expectEqual(btree.size, sorted.length, `btree size ${detail}`);
        expectEqual(skip.toArray(), sorted, `skip ${detail}`);
        expectEqual(skip.size, sorted.length, `skip size ${detail}`);
        expectEqual(bst.contains(key), keys.has(key), `bst contains ${detail}`);
        expectEqual(avl.search(key), keys.has(key), `avl search ${detail}`);
        expectEqual(rb.contains(key), keys.has(key), `rb contains ${detail}`);
        expectEqual(btree.search(key), keys.has(key), `btree search ${detail}`);
        expectEqual(skip.search(key), keys.has(key), `skip search ${detail}`);

        const min = sorted.length === 0 ? null : sorted[0];
        const max = sorted.length === 0 ? null : sorted[sorted.length - 1];
        expectEqual(bst.min(), min, `bst min ${detail}`);
        expectEqual(bst.max(), max, `bst max ${detail}`);
        expectEqual(btree.min(), min, `btree min ${detail}`);
        expectEqual(btree.max(), max, `btree max ${detail}`);

        const probe = randInt(next, -1, 26);
        let floor: number | null = null;
        let ceiling: number | null = null;
        for (const value of sorted) {
          if (value <= probe) floor = value;
          if (ceiling === null && value >= probe) ceiling = value;
        }
        expectEqual(bst.floor(probe), floor, `bst floor ${detail}`);
        expectEqual(bst.ceiling(probe), ceiling, `bst ceiling ${detail}`);
        expectEqual(skip.floor(probe), floor, `skip floor ${detail}`);
        expectEqual(skip.ceiling(probe), ceiling, `skip ceiling ${detail}`);
        const lo = randInt(next, -1, 12);
        const hi = randInt(next, lo, 26);
        expectEqual(
          bst.rangeQuery(lo, hi),
          sorted.filter((value) => value >= lo && value <= hi),
          `bst range ${detail}`,
        );
        expectEqual(skip.range(lo, hi), sorted.filter((value) => value >= lo && value <= hi), `skip range ${detail}`);
        const k = randInt(next, 0, sorted.length + 1);
        const kth = k < 1 || k > sorted.length ? null : sorted[k - 1];
        expectEqual(bst.kthSmallest(k), kth, `bst kth ${detail}`);
      }
      } finally {
        Math.random = originalRandom;
      }
    }
  });

  test("hash tables match Map", () => {
    const next = mulberry32(SEED + 4);
    for (let i = 0; i < 12; i++) {
      const chain = new HashTableChaining<number, number>(2);
      const open = new HashTableOpenAddressing<number, number>(2);
      const model = new Map<number, number>();
      for (let step = 0; step < 40; step++) {
        const key = randInt(next, -15, 20);
        const detail = `seed=${SEED} iteration=${i} step=${step} key=${key} map=${JSON.stringify([...model])}`;
        const roll = next();
        if (roll < 0.6) {
          const value = randInt(next, -50, 50);
          chain.put(key, value);
          open.put(key, value);
          model.set(key, value);
        } else if (roll < 0.85) {
          chain.delete(key);
          open.delete(key);
          model.delete(key);
        }
        expectEqual(chain.get(key), model.get(key), `chain get ${detail}`);
        expectEqual(open.get(key), model.get(key), `open get ${detail}`);
        expectEqual(chain.has(key), model.has(key), `chain has ${detail}`);
        expectEqual(open.has(key), model.has(key), `open has ${detail}`);
        expectEqual(chain.size, model.size, `chain size ${detail}`);
        expectEqual(open.size, model.size, `open size ${detail}`);
        for (const [storedKey, storedValue] of model) {
          expectEqual(chain.get(storedKey), storedValue, `chain scan ${detail}`);
          expectEqual(open.get(storedKey), storedValue, `open scan ${detail}`);
        }
      }
    }
  });

  test("heaps match sort", () => {
    const next = mulberry32(SEED + 5);
    for (let i = 0; i < 12; i++) {
      const values = randomArray(next, randInt(next, 0, 30), -40, 40);
      const min = new MinHeap<number>();
      const max = new MaxHeap<number>();
      for (const value of values) {
        min.push(value);
        max.push(value);
      }
      const ascending = numericSorted(values);
      const descending = [...ascending].reverse();
      const detail = `seed=${SEED} iteration=${i} values=${JSON.stringify(values)}`;
      expectEqual(min.toSortedArray(), ascending, `min sorted ${detail}`);
      expectEqual(max.toSortedArray(), descending, `max sorted ${detail}`);
      expectEqual([...min.toArray()].sort((a, b) => a - b), ascending, `min bag ${detail}`);
      expectEqual([...max.toArray()].sort((a, b) => a - b), ascending, `max bag ${detail}`);
      const minPop: number[] = [];
      const maxPop: number[] = [];
      while (!min.isEmpty) minPop.push(min.pop());
      while (!max.isEmpty) maxPop.push(max.pop());
      expectEqual(minPop, ascending, `min pop ${detail}`);
      expectEqual(maxPop, descending, `max pop ${detail}`);
    }
  });

  test("LRU and LFU match reference caches", () => {
    const next = mulberry32(SEED + 6);
    for (let i = 0; i < 10; i++) {
      const capacity = randInt(next, 1, 4);
      const lru = new LRUCache<number, number>(capacity);
      const lfu = new LFUCache<number, number>(capacity);
      const lruOrder: number[] = [];
      const lruValues = new Map<number, number>();
      const lfuEntries = new Map<number, { value: number; freq: number; stamp: number }>();
      let stamp = 0;

      const lruKeys = (): number[] => [...lruOrder].reverse();
      const evictLfu = (): void => {
        let victim: number | null = null;
        for (const [key, entry] of lfuEntries) {
          if (victim === null) {
            victim = key;
            continue;
          }
          const best = lfuEntries.get(victim)!;
          if (entry.freq < best.freq || (entry.freq === best.freq && entry.stamp < best.stamp)) {
            victim = key;
          }
        }
        if (victim !== null) lfuEntries.delete(victim);
      };

      for (let step = 0; step < 36; step++) {
        const key = randInt(next, 0, 8);
        const detail = `seed=${SEED} iteration=${i} step=${step} cap=${capacity} key=${key}`;
        const roll = next();
        if (roll < 0.55) {
          const value = randInt(next, 0, 20);
          lru.put(key, value);
          const existing = lruOrder.indexOf(key);
          if (existing >= 0) {
            lruOrder.splice(existing, 1);
          } else if (lruOrder.length >= capacity) {
            const evicted = lruOrder.shift();
            if (evicted !== undefined) lruValues.delete(evicted);
          }
          lruOrder.push(key);
          lruValues.set(key, value);

          const hit = lfuEntries.get(key);
          if (hit) {
            hit.value = value;
            hit.freq++;
            hit.stamp = ++stamp;
          } else {
            if (lfuEntries.size >= capacity) evictLfu();
            lfuEntries.set(key, { value, freq: 1, stamp: ++stamp });
          }
          lfu.put(key, value);
        } else if (roll < 0.85) {
          const fromLru = lruValues.has(key) ? lruValues.get(key) : undefined;
          if (fromLru !== undefined) {
            const existing = lruOrder.indexOf(key);
            lruOrder.splice(existing, 1);
            lruOrder.push(key);
          }
          expectEqual(lru.get(key), fromLru, `lru get ${detail}`);

          const hit = lfuEntries.get(key);
          if (hit) {
            hit.freq++;
            hit.stamp = ++stamp;
          }
          expectEqual(lfu.get(key), hit ? hit.value : undefined, `lfu get ${detail}`);
        } else {
          const had = lruValues.delete(key);
          const existing = lruOrder.indexOf(key);
          if (existing >= 0) lruOrder.splice(existing, 1);
          expectEqual(lru.delete(key), had, `lru delete ${detail}`);
          expectEqual(lfu.delete(key), lfuEntries.delete(key), `lfu delete ${detail}`);
        }
        expectEqual(lru.keys(), lruKeys(), `lru keys ${detail}`);
        expectEqual(lru.size, lruValues.size, `lru size ${detail}`);
        expectEqual(lfu.size, lfuEntries.size, `lfu size ${detail}`);
        expectEqual(lfu.getFrequency(key), lfuEntries.get(key)?.freq ?? -1, `lfu freq ${detail}`);
      }
    }
  });

  test("segment trees and fenwick match naive sums", () => {
    const next = mulberry32(SEED + 7);
    for (let i = 0; i < 10; i++) {
      const initial = randomArray(next, randInt(next, 1, 18), -10, 10);
      const point = [...initial];
      const tree = new SegmentTree<number>(initial);
      const fenwickData = [...initial];
      const fenwick = FenwickTree.fromArray(initial);
      const lazyData = [...initial];
      const lazy = new SegmentTreeLazy(initial);
      const detailBase = `seed=${SEED} iteration=${i}`;

      for (let step = 0; step < 24; step++) {
        const lo = randInt(next, 0, point.length - 1);
        const hi = randInt(next, lo, point.length - 1);
        if (next() < 0.5) {
          const index = randInt(next, 0, point.length - 1);
          const value = randInt(next, -10, 10);
          point[index] = value;
          tree.update(index, value);
          fenwickData[index] += value;
          fenwick.update(index, value);
          lazy.pointUpdate(index, value);
          lazyData[index] += value;
        } else {
          const delta = randInt(next, -5, 5);
          lazy.rangeUpdate(lo, hi, delta);
          for (let j = lo; j <= hi; j++) lazyData[j] += delta;
        }
        const detail = `${detailBase} step=${step} lo=${lo} hi=${hi}`;
        expectEqual(tree.query(lo, hi), sumRange(point, lo, hi), `segment ${detail}`);
        expectEqual(fenwick.rangeSum(lo, hi), sumRange(fenwickData, lo, hi), `fenwick ${detail}`);
        expectEqual(fenwick.prefixSum(hi), sumRange(fenwickData, 0, hi), `fenwick prefix ${detail}`);
        expectEqual(fenwick.pointQuery(lo), fenwickData[lo], `fenwick point ${detail}`);
        expectEqual(lazy.query(lo, hi), sumRange(lazyData, lo, hi), `lazy ${detail}`);
      }
    }
  });

  test("union-find matches a parent-pointer model", () => {
    const next = mulberry32(SEED + 8);
    for (let i = 0; i < 10; i++) {
      const n = randInt(next, 2, 18);
      const uf = new UnionFind<number>();
      const parent = new Map<number, number>();
      const find = (x: number): number => {
        let root = x;
        while (parent.get(root) !== root) root = parent.get(root)!;
        return root;
      };
      const componentSize = (x: number): number => {
        const root = find(x);
        let count = 0;
        for (const value of parent.keys()) {
          if (find(value) === root) count++;
        }
        return count;
      };

      for (let x = 0; x < n; x++) {
        expectEqual(uf.makeSet(x), !parent.has(x), `seed=${SEED} iteration=${i} make ${x}`);
        parent.set(x, x);
        expectEqual(uf.makeSet(x), false, `seed=${SEED} iteration=${i} remake ${x}`);
      }

      for (let step = 0; step < 24; step++) {
        const a = randInt(next, 0, n - 1);
        const b = randInt(next, 0, n - 1);
        const detail = `seed=${SEED} iteration=${i} step=${step} a=${a} b=${b}`;
        const merged = find(a) !== find(b);
        if (merged) parent.set(find(b), find(a));
        expectEqual(uf.union(a, b), merged, `union ${detail}`);
        expectEqual(uf.connected(a, b), find(a) === find(b), `connected ${detail}`);
        expectEqual(uf.componentSize(a), componentSize(a), `size ${detail}`);
        expectEqual(uf.numComponents, new Set([...parent.keys()].map(find)).size, `components ${detail}`);
      }
    }
  });

  test("trie matches a set of words", () => {
    const next = mulberry32(SEED + 9);
    for (let i = 0; i < 10; i++) {
      const trie = new Trie();
      const words = new Set<string>();
      for (let step = 0; step < 28; step++) {
        const word = randomWord(next, "abc", 4);
        const detail = `seed=${SEED} iteration=${i} step=${step} word=${JSON.stringify(word)}`;
        const roll = next();
        if (roll < 0.7) {
          trie.insert(word);
          words.add(word);
        } else {
          const removed = words.delete(word);
          expectEqual(trie.delete(word), removed, `delete ${detail}`);
        }
        const stored = [...words].sort();
        expectEqual([...trie.getAllWords()].sort(), stored, `words ${detail}`);
        expectEqual(trie.size, words.size, `size ${detail}`);
        expectEqual(trie.search(word), words.has(word), `search ${detail}`);
        const prefix = word.slice(0, randInt(next, 0, word.length));
        const prefixed = [...words].filter((value) => value.startsWith(prefix));
        expectEqual(
          trie.startsWith(prefix),
          prefix.length === 0 || prefixed.length > 0,
          `startsWith ${detail} prefix=${JSON.stringify(prefix)}`,
        );
        expectEqual(trie.countWordsWithPrefix(prefix), prefixed.length, `count ${detail}`);
        const limit = randInt(next, 1, 4);
        const completed = [...prefixed].sort((a, b) => {
          if (a.length !== b.length) return a.length - b.length;
          if (a < b) return -1;
          if (a > b) return 1;
          return 0;
        });
        expectEqual(
          trie.autocomplete(prefix, limit),
          completed.slice(0, limit),
          `autocomplete ${detail} prefix=${JSON.stringify(prefix)} limit=${limit}`,
        );
      }
    }
  });
});
