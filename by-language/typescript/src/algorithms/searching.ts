/**
 * Searching Algorithms
 *
 * | Algorithm            | Time         | Space |
 * |----------------------|--------------|-------|
 * | Linear Search        | O(n)         | O(1)  |
 * | Binary Search        | O(log n)     | O(1)  |
 * | Jump Search          | O(sqrt(n))   | O(1)  |
 * | Interpolation Search | O(log log n) | O(1)  | (average, uniform data)
 * | Exponential Search   | O(log n)     | O(1)  |
 * | Ternary Search       | O(log n)     | O(1)  |
 */

type CompareFn<T> = (a: T, b: T) => number;

function defaultCompare<T>(a: T, b: T): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

/**
 * Linear search - iterate through all elements.
 *
 * Time: O(n)
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function linearSearch<T>(arr: T[], target: T): number {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] === target) return i;
  }
  return -1;
}

/**
 * Classic binary search on a sorted array.
 *
 * Time: O(log n)
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function binarySearch<T>(
  arr: T[],
  target: T,
  compareFn: CompareFn<T> = defaultCompare
): number {
  let left = 0;
  let right = arr.length - 1;

  while (left <= right) {
    const mid = left + Math.floor((right - left) / 2);
    const cmp = compareFn(arr[mid], target);

    if (cmp === 0) return mid;
    if (cmp < 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

/**
 * Find the leftmost position where target can be inserted (first index >= target).
 *
 * Also known as bisect_left. Returns index of first element >= target,
 * or arr.length if all elements are < target.
 *
 * Time: O(log n)
 */
export function lowerBound<T>(
  arr: T[],
  target: T,
  compareFn: CompareFn<T> = defaultCompare
): number {
  let left = 0;
  let right = arr.length;

  while (left < right) {
    const mid = left + Math.floor((right - left) / 2);
    if (compareFn(arr[mid], target) < 0) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  return left;
}

/**
 * Find the rightmost position where target can be inserted (first index > target).
 *
 * Also known as bisect_right. Returns index of first element > target,
 * or arr.length if all elements are <= target.
 *
 * Time: O(log n)
 */
export function upperBound<T>(
  arr: T[],
  target: T,
  compareFn: CompareFn<T> = defaultCompare
): number {
  let left = 0;
  let right = arr.length;

  while (left < right) {
    const mid = left + Math.floor((right - left) / 2);
    if (compareFn(arr[mid], target) <= 0) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  return left;
}

/**
 * Jump search - skip ahead by sqrt(n) steps, then linear search.
 *
 * Time: O(sqrt(n))
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function jumpSearch(arr: number[], target: number): number {
  const n = arr.length;
  if (n === 0) return -1;

  const step = Math.floor(Math.sqrt(n));
  let prev = 0;
  let curr = step;

  // Jump ahead
  while (arr[Math.min(curr, n) - 1] < target) {
    prev = curr;
    curr += step;
    if (prev >= n) return -1;
  }

  // Linear search in block
  for (let i = prev; i < Math.min(curr, n); i++) {
    if (arr[i] === target) return i;
  }

  return -1;
}

/**
 * Interpolation search - estimate position based on value distribution.
 *
 * Best for uniformly distributed data. For non-uniform data, can degrade to O(n).
 *
 * Time: O(log log n) average for uniform data, O(n) worst case
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function interpolationSearch(arr: number[], target: number): number {
  let low = 0;
  let high = arr.length - 1;

  while (low <= high && arr[low] <= target && target <= arr[high]) {
    if (low === high) {
      return arr[low] === target ? low : -1;
    }
    if (arr[low] === arr[high]) return low;

    // Estimate position using linear interpolation
    const pos =
      low +
      Math.floor(
        ((target - arr[low]) * (high - low)) / (arr[high] - arr[low])
      );

    if (arr[pos] === target) return pos;
    if (arr[pos] < target) {
      low = pos + 1;
    } else {
      high = pos - 1;
    }
  }

  return -1;
}

/**
 * Exponential search - find range then binary search.
 *
 * Useful when target is near the beginning of a large array.
 *
 * Time: O(log n)
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function exponentialSearch(arr: number[], target: number): number {
  const n = arr.length;
  if (n === 0) return -1;

  if (arr[0] === target) return 0;

  // Find range for binary search
  let i = 1;
  while (i < n && arr[i] <= target) {
    i *= 2;
  }

  // Binary search in range [i/2, min(i, n-1)]
  let left = Math.floor(i / 2);
  let right = Math.min(i, n - 1);

  while (left <= right) {
    const mid = left + Math.floor((right - left) / 2);
    if (arr[mid] === target) return mid;
    if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

/**
 * Ternary search - divide into three parts.
 *
 * Similar to binary search but divides into three parts.
 *
 * Time: O(log n)
 * Space: O(1)
 *
 * @returns Index of target if found, -1 otherwise
 */
export function ternarySearch(arr: number[], target: number): number {
  let left = 0;
  let right = arr.length - 1;

  while (left <= right) {
    const mid1 = left + Math.floor((right - left) / 3);
    const mid2 = right - Math.floor((right - left) / 3);

    if (arr[mid1] === target) return mid1;
    if (arr[mid2] === target) return mid2;

    if (target < arr[mid1]) {
      right = mid1 - 1;
    } else if (target > arr[mid2]) {
      left = mid2 + 1;
    } else {
      left = mid1 + 1;
      right = mid2 - 1;
    }
  }

  return -1;
}
