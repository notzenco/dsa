/**
 * Sorting Algorithms
 *
 * Comparison-Based Sorts:
 * | Algorithm       | Best       | Average    | Worst      | Stable |
 * |-----------------|------------|------------|------------|--------|
 * | Bubble Sort     | O(n)       | O(n^2)     | O(n^2)     | Yes    |
 * | Selection Sort  | O(n^2)     | O(n^2)     | O(n^2)     | No     |
 * | Insertion Sort  | O(n)       | O(n^2)     | O(n^2)     | Yes    |
 * | Merge Sort      | O(n log n) | O(n log n) | O(n log n) | Yes    |
 * | Quick Sort      | O(n log n) | O(n log n) | O(n^2)     | No     |
 * | Heap Sort       | O(n log n) | O(n log n) | O(n log n) | No     |
 * | Shell Sort      | O(n log n) | O(n^1.3)   | O(n^2)     | No     |
 *
 * Non-Comparison Sorts:
 * | Algorithm       | Time       | Space  | Constraint | Stable |
 * |-----------------|------------|--------|------------|--------|
 * | Counting Sort   | O(n + k)   | O(k)   | Integers   | Yes    |
 * | Radix Sort      | O(d(n+k))  | O(n+k) | Integers   | Yes    |
 * | Bucket Sort     | O(n + k)   | O(n)   | Uniform    | Yes    |
 */

type CompareFn<T> = (a: T, b: T) => number;

function defaultCompare<T>(a: T, b: T): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

/**
 * Bubble Sort - repeatedly swap adjacent elements if in wrong order.
 *
 * Time: O(n^2) average/worst, O(n) best (already sorted)
 * Space: O(1)
 * Stable: Yes
 */
export function bubbleSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  const n = result.length;

  for (let i = 0; i < n; i++) {
    let swapped = false;

    for (let j = 0; j < n - i - 1; j++) {
      if (compareFn(result[j], result[j + 1]) > 0) {
        [result[j], result[j + 1]] = [result[j + 1], result[j]];
        swapped = true;
      }
    }

    if (!swapped) break;
  }

  return result;
}

/**
 * Selection Sort - find minimum and place at beginning.
 *
 * Time: O(n^2) all cases
 * Space: O(1)
 * Stable: No
 */
export function selectionSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  const n = result.length;

  for (let i = 0; i < n; i++) {
    let minIdx = i;

    for (let j = i + 1; j < n; j++) {
      if (compareFn(result[j], result[minIdx]) < 0) {
        minIdx = j;
      }
    }

    [result[i], result[minIdx]] = [result[minIdx], result[i]];
  }

  return result;
}

/**
 * Insertion Sort - build sorted portion one element at a time.
 *
 * Time: O(n^2) average/worst, O(n) best (nearly sorted)
 * Space: O(1)
 * Stable: Yes
 */
export function insertionSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  const n = result.length;

  for (let i = 1; i < n; i++) {
    const key = result[i];
    let j = i - 1;

    while (j >= 0 && compareFn(result[j], key) > 0) {
      result[j + 1] = result[j];
      j--;
    }

    result[j + 1] = key;
  }

  return result;
}

function merge<T>(left: T[], right: T[], compareFn: CompareFn<T>): T[] {
  const result: T[] = [];
  let i = 0;
  let j = 0;

  while (i < left.length && j < right.length) {
    if (compareFn(left[i], right[j]) <= 0) {
      result.push(left[i]);
      i++;
    } else {
      result.push(right[j]);
      j++;
    }
  }

  while (i < left.length) {
    result.push(left[i]);
    i++;
  }

  while (j < right.length) {
    result.push(right[j]);
    j++;
  }

  return result;
}

/**
 * Merge Sort - divide and conquer with merging.
 *
 * Time: O(n log n) all cases
 * Space: O(n)
 * Stable: Yes
 */
export function mergeSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  if (arr.length <= 1) {
    return [...arr];
  }

  const mid = Math.floor(arr.length / 2);
  const left = mergeSort(arr.slice(0, mid), compareFn);
  const right = mergeSort(arr.slice(mid), compareFn);

  return merge(left, right, compareFn);
}

function partition<T>(
  arr: T[],
  low: number,
  high: number,
  compareFn: CompareFn<T>
): number {
  // Randomized pivot to avoid worst case
  const pivotIdx = low + Math.floor(Math.random() * (high - low + 1));
  [arr[pivotIdx], arr[high]] = [arr[high], arr[pivotIdx]];

  const pivot = arr[high];
  let i = low - 1;

  for (let j = low; j < high; j++) {
    if (compareFn(arr[j], pivot) <= 0) {
      i++;
      [arr[i], arr[j]] = [arr[j], arr[i]];
    }
  }

  [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
  return i + 1;
}

function quickSortHelper<T>(
  arr: T[],
  low: number,
  high: number,
  compareFn: CompareFn<T>
): void {
  if (low < high) {
    const pivotIdx = partition(arr, low, high, compareFn);
    quickSortHelper(arr, low, pivotIdx - 1, compareFn);
    quickSortHelper(arr, pivotIdx + 1, high, compareFn);
  }
}

/**
 * Quick Sort - divide and conquer with partitioning.
 *
 * Time: O(n log n) average, O(n^2) worst (sorted input with bad pivot)
 * Space: O(log n) average (recursion stack)
 * Stable: No
 */
export function quickSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  quickSortHelper(result, 0, result.length - 1, compareFn);
  return result;
}

function heapify<T>(
  arr: T[],
  n: number,
  i: number,
  compareFn: CompareFn<T>
): void {
  let largest = i;
  const left = 2 * i + 1;
  const right = 2 * i + 2;

  if (left < n && compareFn(arr[left], arr[largest]) > 0) {
    largest = left;
  }

  if (right < n && compareFn(arr[right], arr[largest]) > 0) {
    largest = right;
  }

  if (largest !== i) {
    [arr[i], arr[largest]] = [arr[largest], arr[i]];
    heapify(arr, n, largest, compareFn);
  }
}

/**
 * Heap Sort - build max-heap and extract elements.
 *
 * Time: O(n log n) all cases
 * Space: O(1)
 * Stable: No
 */
export function heapSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  const n = result.length;

  // Build max-heap
  for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
    heapify(result, n, i, compareFn);
  }

  // Extract elements
  for (let i = n - 1; i > 0; i--) {
    [result[0], result[i]] = [result[i], result[0]];
    heapify(result, i, 0, compareFn);
  }

  return result;
}

/**
 * Shell Sort - generalization of insertion sort with gap sequence.
 *
 * Time: O(n^2) worst, O(n log n) to O(n^1.3) average
 * Space: O(1)
 * Stable: No
 *
 * Uses Knuth's gap sequence: 1, 4, 13, 40, 121, ...
 */
export function shellSort<T>(
  arr: T[],
  compareFn: CompareFn<T> = defaultCompare
): T[] {
  const result = [...arr];
  const n = result.length;

  // Generate Knuth's gap sequence
  let gap = 1;
  while (gap < Math.floor(n / 3)) {
    gap = 3 * gap + 1;
  }

  while (gap > 0) {
    for (let i = gap; i < n; i++) {
      const temp = result[i];
      let j = i;

      while (j >= gap && compareFn(result[j - gap], temp) > 0) {
        result[j] = result[j - gap];
        j -= gap;
      }

      result[j] = temp;
    }

    gap = Math.floor(gap / 3);
  }

  return result;
}

// =============================================================================
// NON-COMPARISON SORTING ALGORITHMS
// =============================================================================

/**
 * Counting Sort - count occurrences, then reconstruct.
 *
 * Time: O(n + k) where k is range of values
 * Space: O(k)
 * Stable: Yes
 *
 * Only works for integers.
 */
export function countingSort(arr: number[]): number[] {
  if (arr.length === 0) return [];

  const minVal = Math.min(...arr);
  const maxVal = Math.max(...arr);
  const rangeSize = maxVal - minVal + 1;

  // Count occurrences
  const count = new Array<number>(rangeSize).fill(0);
  for (const num of arr) {
    count[num - minVal]++;
  }

  // Compute cumulative counts
  for (let i = 1; i < rangeSize; i++) {
    count[i] += count[i - 1];
  }

  // Build output array (iterate backwards for stability)
  const result = new Array<number>(arr.length);
  for (let k = arr.length - 1; k >= 0; k--) {
    const num = arr[k];
    const idx = count[num - minVal] - 1;
    result[idx] = num;
    count[num - minVal]--;
  }

  return result;
}

function countingSortByDigit(arr: number[], exp: number): number[] {
  const n = arr.length;
  const result = new Array<number>(n);
  const count = new Array<number>(10).fill(0);

  for (const num of arr) {
    const digit = Math.floor(num / exp) % 10;
    count[digit]++;
  }

  for (let i = 1; i < 10; i++) {
    count[i] += count[i - 1];
  }

  for (let k = n - 1; k >= 0; k--) {
    const num = arr[k];
    const digit = Math.floor(num / exp) % 10;
    const idx = count[digit] - 1;
    result[idx] = num;
    count[digit]--;
  }

  return result;
}

/**
 * Radix Sort - sort digit by digit using counting sort.
 *
 * Time: O(d * (n + k)) where d is digits, k is base
 * Space: O(n + k)
 * Stable: Yes
 *
 * Works for non-negative integers.
 */
export function radixSort(arr: number[]): number[] {
  if (arr.length === 0) return [];

  const maxVal = Math.max(...arr);
  let result = [...arr];
  let exp = 1;

  while (Math.floor(maxVal / exp) > 0) {
    result = countingSortByDigit(result, exp);
    exp *= 10;
  }

  return result;
}

/**
 * Bucket Sort - distribute into buckets, sort each, concatenate.
 *
 * Time: O(n + k) average, O(n^2) worst
 * Space: O(n)
 * Stable: Yes (if using stable sort for buckets)
 */
export function bucketSort(arr: number[], numBuckets: number = 10): number[] {
  if (arr.length === 0) return [];

  const minVal = Math.min(...arr);
  const maxVal = Math.max(...arr);
  const rangeVal = maxVal - minVal;

  if (rangeVal === 0) return [...arr];

  // Create buckets
  const buckets: number[][] = Array.from({ length: numBuckets }, () => []);

  // Distribute elements into buckets
  for (const num of arr) {
    const idx = Math.min(
      Math.floor(((num - minVal) / rangeVal) * (numBuckets - 1)),
      numBuckets - 1
    );
    buckets[idx].push(num);
  }

  // Sort each bucket and concatenate
  const result: number[] = [];
  for (const bucket of buckets) {
    bucket.sort((a, b) => a - b);
    result.push(...bucket);
  }

  return result;
}
