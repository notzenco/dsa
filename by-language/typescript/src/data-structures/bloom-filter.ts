/**
 * Bloom Filter Data Structures
 *
 * Space-efficient probabilistic data structures for set membership testing.
 *
 * COMPLEXITY:
 * | Operation     | Time   | Space  |
 * |---------------|--------|--------|
 * | Insert        | O(k)   | O(m)   |
 * | Query         | O(k)   | O(1)   |
 *
 * k = number of hash functions, m = size of bit array
 *
 * FALSE POSITIVE RATE: p ~ (1 - e^(-kn/m))^k
 */

function djb2(str: string, seed: number): number {
  let hash = 5381 + seed;
  for (let i = 0; i < str.length; i++) {
    hash = ((hash << 5) + hash + str.charCodeAt(i)) | 0;
  }
  return hash < 0 ? ~hash : hash;
}

function fnv1a(str: string, seed: number): number {
  let hash = (2166136261 + seed) | 0;
  for (let i = 0; i < str.length; i++) {
    hash ^= str.charCodeAt(i);
    hash = (hash * 16777619) | 0;
  }
  return hash < 0 ? ~hash : hash;
}

function computeHash(item: unknown, seed: number, size: number): number {
  const str = String(item);
  const h1 = djb2(str, 0);
  const h2 = fnv1a(str, 0);
  const combined = (h1 + seed * h2) | 0;
  return (combined < 0 ? ~combined : combined) % size;
}

function optimalSize(expectedItems: number, falsePositiveRate: number): number {
  const p = falsePositiveRate <= 0 ? 0.0001 : falsePositiveRate;
  const m = -(expectedItems * Math.log(p)) / (Math.log(2) ** 2);
  return Math.max(Math.ceil(m), 64);
}

function optimalHashes(m: number, n: number): number {
  const count = n <= 0 ? 1 : n;
  const k = (m / count) * Math.log(2);
  return Math.max(Math.ceil(k), 1);
}

export class BloomFilter {
  private _bitArray: boolean[];
  private _size: number;
  private _numHashes: number;
  private _count: number;

  constructor(expectedItems: number = 1000, falsePositiveRate: number = 0.01) {
    this._size = optimalSize(expectedItems, falsePositiveRate);
    this._numHashes = optimalHashes(this._size, expectedItems);
    this._bitArray = new Array(this._size).fill(false);
    this._count = 0;
  }

  get bitArraySize(): number {
    return this._size;
  }

  get numHashes(): number {
    return this._numHashes;
  }

  get count(): number {
    return this._count;
  }

  add(item: unknown): void {
    for (let i = 0; i < this._numHashes; i++) {
      const index = computeHash(item, i, this._size);
      this._bitArray[index] = true;
    }
    this._count++;
  }

  contains(item: unknown): boolean {
    for (let i = 0; i < this._numHashes; i++) {
      const index = computeHash(item, i, this._size);
      if (!this._bitArray[index]) {
        return false;
      }
    }
    return true;
  }

  estimatedFalsePositiveRate(): number {
    if (this._count === 0) return 0.0;
    const exponent = (-this._numHashes * this._count) / this._size;
    return Math.pow(1 - Math.exp(exponent), this._numHashes);
  }
}

export class CountingBloomFilter {
  private _counters: number[];
  private _size: number;
  private _numHashes: number;
  private _maxCount: number;
  private _count: number;

  constructor(
    expectedItems: number = 1000,
    falsePositiveRate: number = 0.01,
    counterBits: number = 4
  ) {
    this._size = optimalSize(expectedItems, falsePositiveRate);
    this._numHashes = optimalHashes(this._size, expectedItems);
    this._maxCount = (1 << counterBits) - 1;
    this._counters = new Array(this._size).fill(0);
    this._count = 0;
  }

  get count(): number {
    return this._count;
  }

  add(item: unknown): boolean {
    const indices: number[] = [];
    for (let i = 0; i < this._numHashes; i++) {
      indices.push(computeHash(item, i, this._size));
    }

    // Check for overflow
    if (indices.some((idx) => this._counters[idx] >= this._maxCount)) {
      return false;
    }

    for (const idx of indices) {
      this._counters[idx]++;
    }
    this._count++;
    return true;
  }

  delete(item: unknown): boolean {
    if (!this.contains(item)) {
      return false;
    }

    for (let i = 0; i < this._numHashes; i++) {
      const idx = computeHash(item, i, this._size);
      if (this._counters[idx] > 0) {
        this._counters[idx]--;
      }
    }
    this._count--;
    return true;
  }

  contains(item: unknown): boolean {
    for (let i = 0; i < this._numHashes; i++) {
      const idx = computeHash(item, i, this._size);
      if (this._counters[idx] <= 0) {
        return false;
      }
    }
    return true;
  }
}

export class ScalableBloomFilter {
  private _initialCapacity: number;
  private _fpRate: number;
  private _growthRate: number;
  private _filters: BloomFilter[];
  private _count: number;
  private _fpRatio: number;

  constructor(
    initialCapacity: number = 1000,
    falsePositiveRate: number = 0.01,
    growthRate: number = 2
  ) {
    this._initialCapacity = initialCapacity;
    this._fpRate = falsePositiveRate;
    this._growthRate = growthRate;
    this._filters = [];
    this._count = 0;
    this._fpRatio = 0.9;

    this._addFilter();
  }

  get count(): number {
    return this._count;
  }

  get numFilters(): number {
    return this._filters.length;
  }

  private _addFilter(): void {
    const capacity =
      this._initialCapacity *
      Math.pow(this._growthRate, this._filters.length);
    const fp =
      this._fpRate * Math.pow(this._fpRatio, this._filters.length);
    this._filters.push(new BloomFilter(capacity, fp));
  }

  add(item: unknown): void {
    if (this.contains(item)) {
      return;
    }

    const currentFilter = this._filters[this._filters.length - 1];
    const currentCapacity =
      this._initialCapacity *
      Math.pow(this._growthRate, this._filters.length - 1);

    if (currentFilter.count >= currentCapacity) {
      this._addFilter();
    }

    this._filters[this._filters.length - 1].add(item);
    this._count++;
  }

  contains(item: unknown): boolean {
    return this._filters.some((f) => f.contains(item));
  }
}
