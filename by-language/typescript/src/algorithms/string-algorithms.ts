/**
 * String Algorithms
 *
 * Pattern matching and string manipulation algorithms.
 *
 * | Algorithm    | Time       | Space  | Best For          |
 * |--------------|------------|--------|-------------------|
 * | Naive        | O(n*m)     | O(1)   | Small inputs      |
 * | KMP          | O(n+m)     | O(m)   | Single pattern    |
 * | Rabin-Karp   | O(n+m) avg | O(1)   | Multiple patterns |
 * | Z-Algorithm  | O(n+m)     | O(n+m) | All occurrences   |
 * | Manacher     | O(n)       | O(n)   | Palindromes       |
 * | Suffix Array | O(n log n) | O(n)   | Multiple queries  |
 */

/**
 * Find all occurrences of pattern in text using naive approach.
 *
 * Time: O(n*m) where n=len(text), m=len(pattern)
 * Space: O(1) excluding output
 *
 * @returns List of starting indices where pattern occurs
 */
export function naiveSearch(text: string, pattern: string): number[] {
  if (!pattern || !text) return [];

  const n = text.length;
  const m = pattern.length;
  if (m > n) return [];

  const result: number[] = [];
  for (let i = 0; i <= n - m; i++) {
    let match = true;
    for (let j = 0; j < m; j++) {
      if (text[i + j] !== pattern[j]) {
        match = false;
        break;
      }
    }
    if (match) result.push(i);
  }

  return result;
}

/**
 * Compute the LPS (Longest Proper Prefix which is also Suffix) array.
 * Also known as the failure function or partial match table.
 *
 * Time: O(m)
 * Space: O(m)
 */
function computeLPS(pattern: string): number[] {
  const m = pattern.length;
  const lps = new Array<number>(m).fill(0);

  let length = 0;
  let i = 1;

  while (i < m) {
    if (pattern[i] === pattern[length]) {
      length++;
      lps[i] = length;
      i++;
    } else {
      if (length !== 0) {
        length = lps[length - 1];
      } else {
        lps[i] = 0;
        i++;
      }
    }
  }

  return lps;
}

/**
 * Find all occurrences of pattern in text using KMP algorithm.
 *
 * The key insight is that when a mismatch occurs, we don't need to
 * re-examine characters we know will match based on the LPS array.
 *
 * Time: O(n + m)
 * Space: O(m)
 *
 * @returns List of starting indices where pattern occurs
 */
export function kmpSearch(text: string, pattern: string): number[] {
  if (!pattern || !text) return [];

  const n = text.length;
  const m = pattern.length;
  if (m > n) return [];

  const lps = computeLPS(pattern);
  const result: number[] = [];

  let i = 0; // Index for text
  let j = 0; // Index for pattern

  while (i < n) {
    if (text[i] === pattern[j]) {
      i++;
      j++;

      if (j === m) {
        result.push(i - m);
        j = lps[j - 1];
      }
    } else {
      if (j !== 0) {
        j = lps[j - 1];
      } else {
        i++;
      }
    }
  }

  return result;
}

/**
 * Find all occurrences of pattern in text using Rabin-Karp algorithm.
 *
 * Uses a rolling hash to compute hash values in O(1) after initial computation.
 *
 * Time: O(n + m) average, O(n*m) worst case (many hash collisions)
 * Space: O(1) excluding output
 *
 * @returns List of starting indices where pattern occurs
 */
export function rabinKarp(text: string, pattern: string): number[] {
  if (!pattern || !text) return [];

  const n = text.length;
  const m = pattern.length;
  if (m > n) return [];

  const result: number[] = [];
  const d = 256; // Number of characters in alphabet
  const prime = 101;

  // Calculate d^(m-1) % prime
  let h = 1;
  for (let i = 0; i < m - 1; i++) {
    h = (h * d) % prime;
  }

  // Calculate hash values for pattern and first window of text
  let patternHash = 0;
  let textHash = 0;

  for (let i = 0; i < m; i++) {
    patternHash = (d * patternHash + pattern.charCodeAt(i)) % prime;
    textHash = (d * textHash + text.charCodeAt(i)) % prime;
  }

  // Slide the pattern over text
  for (let i = 0; i <= n - m; i++) {
    // Check if hash values match
    if (patternHash === textHash) {
      // Verify character by character (to handle hash collisions)
      if (text.substring(i, i + m) === pattern) {
        result.push(i);
      }
    }

    // Calculate hash for next window
    if (i < n - m) {
      textHash =
        (d * (textHash - text.charCodeAt(i) * h) +
          text.charCodeAt(i + m)) %
        prime;

      // Handle negative hash
      if (textHash < 0) {
        textHash += prime;
      }
    }
  }

  return result;
}

/**
 * Compute the Z-array for string s.
 *
 * Z[i] = length of longest substring starting at i that is also a prefix of s.
 *
 * Time: O(n)
 * Space: O(n)
 */
function computeZArray(s: string): number[] {
  const n = s.length;
  if (n === 0) return [];

  const z = new Array<number>(n).fill(0);
  z[0] = n;

  let l = 0;
  let r = 0;

  for (let i = 1; i < n; i++) {
    if (i < r) {
      z[i] = Math.min(z[i - l], r - i);
    }

    while (i + z[i] < n && s[z[i]] === s[i + z[i]]) {
      z[i]++;
    }

    if (i + z[i] > r) {
      l = i;
      r = i + z[i];
    }
  }

  return z;
}

/**
 * Find all occurrences of pattern in text using Z-algorithm.
 *
 * Concatenates pattern + '$' + text and computes Z-array.
 * Positions where Z[i] == len(pattern) are matches.
 *
 * Time: O(n + m)
 * Space: O(n + m)
 *
 * @returns List of starting indices where pattern occurs
 */
export function zAlgorithm(text: string, pattern: string): number[] {
  if (!pattern || !text) return [];

  const m = pattern.length;
  if (m > text.length) return [];

  const concat = pattern + "$" + text;
  const z = computeZArray(concat);

  const result: number[] = [];
  for (let i = m + 1; i < concat.length; i++) {
    if (z[i] >= m) {
      result.push(i - m - 1);
    }
  }

  return result;
}

/**
 * Find the longest palindromic substring using Manacher's algorithm.
 *
 * Time: O(n)
 * Space: O(n)
 *
 * @returns Longest palindromic substring
 */
export function manacher(s: string): string {
  if (!s) return "";

  // Symbols cannot collide with any input character.
  const startSentinel = Symbol();
  const separator = Symbol();
  const endSentinel = Symbol();
  const t: (string | symbol)[] = [startSentinel, separator];
  for (const char of s.split("")) {
    t.push(char, separator);
  }
  t.push(endSentinel);
  const n = t.length;
  const p = new Array<number>(n).fill(0);

  let center = 0;
  let right = 0;

  for (let i = 1; i < n - 1; i++) {
    if (i < right) {
      const mirror = 2 * center - i;
      p[i] = Math.min(right - i, p[mirror]);
    }

    // Try to expand
    while (t[i + p[i] + 1] === t[i - p[i] - 1]) {
      p[i]++;
    }

    // Update center and right
    if (i + p[i] > right) {
      center = i;
      right = i + p[i];
    }
  }

  // Find the maximum radius and its center
  let maxRadius = 0;
  let maxCenter = 0;
  for (let i = 1; i < n - 1; i++) {
    if (p[i] > maxRadius) {
      maxRadius = p[i];
      maxCenter = i;
    }
  }

  // Convert back to original indices
  const start = Math.floor((maxCenter - maxRadius) / 2);
  const length = maxRadius;

  return s.substring(start, start + length);
}

/**
 * Build suffix array for string s.
 *
 * Suffix array contains starting indices of all suffixes in sorted order.
 *
 * Time: O(n log n) using sorting (with O(n) string comparison at worst)
 * Space: O(n)
 *
 * @returns Suffix array
 */
export function buildSuffixArray(s: string): number[] {
  const n = s.length;
  if (n === 0) return [];

  const suffixes = Array.from({ length: n }, (_, i) => i);
  suffixes.sort((a, b) => {
    const sa = s.substring(a);
    const sb = s.substring(b);
    if (sa < sb) return -1;
    if (sa > sb) return 1;
    return 0;
  });

  return suffixes;
}
