import { describe, test, expect } from "bun:test";
import {
  naiveSearch,
  kmpSearch,
  rabinKarp,
  zAlgorithm,
  manacher,
  buildSuffixArray,
} from "../../src/algorithms/string-algorithms";

// =============================================================================
// Shared test helper for pattern matching functions
// =============================================================================

function testPatternSearch(
  name: string,
  searchFn: (text: string, pattern: string) => number[]
) {
  describe(name, () => {
    test("empty text", () => {
      expect(searchFn("", "abc")).toEqual([]);
    });

    test("empty pattern", () => {
      expect(searchFn("abc", "")).toEqual([]);
    });

    test("both empty", () => {
      expect(searchFn("", "")).toEqual([]);
    });

    test("pattern longer than text", () => {
      expect(searchFn("abc", "abcdef")).toEqual([]);
    });

    test("no match", () => {
      expect(searchFn("abcdef", "xyz")).toEqual([]);
    });

    test("single match at start", () => {
      expect(searchFn("abcdef", "abc")).toEqual([0]);
    });

    test("single match at end", () => {
      expect(searchFn("abcdef", "def")).toEqual([3]);
    });

    test("single match in middle", () => {
      expect(searchFn("abcdef", "cde")).toEqual([2]);
    });

    test("multiple matches", () => {
      expect(searchFn("abcabcabc", "abc")).toEqual([0, 3, 6]);
    });

    test("overlapping matches", () => {
      expect(searchFn("aaaa", "aa")).toEqual([0, 1, 2]);
    });

    test("overlapping matches - aba pattern", () => {
      expect(searchFn("abababab", "aba")).toEqual([0, 2, 4]);
    });

    test("single character pattern", () => {
      expect(searchFn("abcabc", "a")).toEqual([0, 3]);
    });

    test("single character text and pattern - match", () => {
      expect(searchFn("a", "a")).toEqual([0]);
    });

    test("single character text and pattern - no match", () => {
      expect(searchFn("a", "b")).toEqual([]);
    });

    test("pattern equals text", () => {
      expect(searchFn("abc", "abc")).toEqual([0]);
    });

    test("repeated single character", () => {
      expect(searchFn("aaaa", "a")).toEqual([0, 1, 2, 3]);
    });

    test("complex KMP pattern", () => {
      expect(searchFn("ABABDABACDABABCABAB", "ABABCABAB")).toEqual([10]);
    });

    test("long text with many matches", () => {
      const text = "abababababababababab";
      const pattern = "ab";
      const expected = Array.from({ length: 10 }, (_, i) => i * 2);
      expect(searchFn(text, pattern)).toEqual(expected);
    });

    test("no false positives with similar patterns", () => {
      expect(searchFn("aabaabaab", "aab")).toEqual([0, 3, 6]);
    });
  });
}

// Run pattern search tests for all algorithms
testPatternSearch("naiveSearch", naiveSearch);
testPatternSearch("kmpSearch", kmpSearch);
testPatternSearch("rabinKarp", rabinKarp);
testPatternSearch("zAlgorithm", zAlgorithm);

// =============================================================================
// manacher
// =============================================================================

describe("manacher", () => {
  test("empty string", () => {
    expect(manacher("")).toBe("");
  });

  test("single character", () => {
    expect(manacher("a")).toBe("a");
  });

  test("two same characters", () => {
    expect(manacher("aa")).toBe("aa");
  });

  test("two different characters", () => {
    const result = manacher("ab");
    expect(result.length).toBe(1);
    // Either "a" or "b" is valid
  });

  test("odd palindrome", () => {
    expect(manacher("babad")).toHaveLength(3);
    // "bab" or "aba" are both valid
    const result = manacher("babad");
    expect(result === "bab" || result === "aba").toBe(true);
  });

  test("even palindrome", () => {
    expect(manacher("cbbd")).toBe("bb");
  });

  test("entire string is palindrome", () => {
    expect(manacher("racecar")).toBe("racecar");
  });

  test("palindrome at start", () => {
    const result = manacher("abaxyz");
    expect(result).toBe("aba");
  });

  test("palindrome at end", () => {
    const result = manacher("xyzaba");
    expect(result).toBe("aba");
  });

  test("longest palindrome in middle", () => {
    const result = manacher("xabacabay");
    expect(result).toBe("abacaba");
  });

  test("all same characters", () => {
    expect(manacher("aaaa")).toBe("aaaa");
  });

  test("result is a palindrome", () => {
    const inputs = ["abcba", "forgeeksskeegfor", "abacdfgdcaba"];
    for (const input of inputs) {
      const result = manacher(input);
      const reversed = result.split("").reverse().join("");
      expect(result).toBe(reversed);
    }
  });

  test("no longer palindrome exists", () => {
    const result = manacher("forgeeksskeegfor");
    expect(result).toBe("geeksskeeg");
  });
});

// =============================================================================
// buildSuffixArray
// =============================================================================

describe("buildSuffixArray", () => {
  test("empty string", () => {
    expect(buildSuffixArray("")).toEqual([]);
  });

  test("single character", () => {
    expect(buildSuffixArray("a")).toEqual([0]);
  });

  test("two characters - sorted", () => {
    expect(buildSuffixArray("ab")).toEqual([0, 1]);
  });

  test("two characters - reverse", () => {
    expect(buildSuffixArray("ba")).toEqual([1, 0]);
  });

  test("banana", () => {
    // Suffixes of "banana":
    // 0: banana
    // 1: anana
    // 2: nana
    // 3: ana
    // 4: na
    // 5: a
    // Sorted: a(5), ana(3), anana(1), banana(0), na(4), nana(2)
    expect(buildSuffixArray("banana")).toEqual([5, 3, 1, 0, 4, 2]);
  });

  test("all same characters", () => {
    // Suffixes of "aaa":
    // 0: aaa, 1: aa, 2: a
    // Sorted: a(2), aa(1), aaa(0)
    expect(buildSuffixArray("aaa")).toEqual([2, 1, 0]);
  });

  test("abcabc", () => {
    const sa = buildSuffixArray("abcabc");
    // Verify it produces a valid suffix array
    expect(sa.length).toBe(6);
    // Each index 0-5 should appear exactly once
    expect([...sa].sort()).toEqual([0, 1, 2, 3, 4, 5]);

    // Verify suffixes are in sorted order
    const s = "abcabc";
    for (let i = 0; i < sa.length - 1; i++) {
      expect(s.substring(sa[i]) <= s.substring(sa[i + 1])).toBe(true);
    }
  });

  test("result contains valid permutation of indices", () => {
    const s = "mississippi";
    const sa = buildSuffixArray(s);
    expect(sa.length).toBe(s.length);
    expect([...sa].sort((a, b) => a - b)).toEqual(
      Array.from({ length: s.length }, (_, i) => i)
    );

    // Verify sorted order
    for (let i = 0; i < sa.length - 1; i++) {
      expect(s.substring(sa[i]) <= s.substring(sa[i + 1])).toBe(true);
    }
  });
});
