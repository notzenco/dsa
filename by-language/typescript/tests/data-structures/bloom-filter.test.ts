import { describe, test, expect } from "bun:test";
import {
  BloomFilter,
  CountingBloomFilter,
  ScalableBloomFilter,
} from "../../src/data-structures/bloom-filter";

// ============================================================================
// BloomFilter
// ============================================================================
describe("BloomFilter", () => {
  describe("add and contains", () => {
    test("should report contains for added string items", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add("hello");
      bf.add("world");
      expect(bf.contains("hello")).toBe(true);
      expect(bf.contains("world")).toBe(true);
    });

    test("should report not contains for items never added", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add("hello");
      expect(bf.contains("definitely_not_in_filter_xyz_123")).toBe(false);
    });

    test("should handle number items", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add(42);
      bf.add(100);
      expect(bf.contains(42)).toBe(true);
      expect(bf.contains(100)).toBe(true);
      expect(bf.contains(999)).toBe(false);
    });

    test("should handle empty string", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add("");
      expect(bf.contains("")).toBe(true);
    });

    test("should handle null and undefined items via String coercion", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add(null);
      bf.add(undefined);
      expect(bf.contains(null)).toBe(true);
      expect(bf.contains(undefined)).toBe(true);
    });

    test("should handle boolean items", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add(true);
      expect(bf.contains(true)).toBe(true);
      expect(bf.contains(false)).toBe(false);
    });
  });

  describe("count tracking", () => {
    test("should start at 0", () => {
      const bf = new BloomFilter(100, 0.01);
      expect(bf.count).toBe(0);
    });

    test("should increment on every add (even duplicates)", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add("a");
      bf.add("b");
      bf.add("a"); // duplicate still increments count
      expect(bf.count).toBe(3);
    });
  });

  describe("properties", () => {
    test("should have positive bitArraySize and numHashes", () => {
      const bf = new BloomFilter(1000, 0.01);
      expect(bf.bitArraySize).toBeGreaterThan(0);
      expect(bf.numHashes).toBeGreaterThan(0);
    });

    test("lower false positive rate should yield larger bit array", () => {
      const bfHigh = new BloomFilter(1000, 0.1);
      const bfLow = new BloomFilter(1000, 0.001);
      expect(bfLow.bitArraySize).toBeGreaterThan(bfHigh.bitArraySize);
    });

    test("should use defaults when constructed with no arguments", () => {
      const bf = new BloomFilter();
      expect(bf.bitArraySize).toBeGreaterThan(0);
      expect(bf.numHashes).toBeGreaterThan(0);
      expect(bf.count).toBe(0);
    });
  });

  describe("false positive rate", () => {
    test("estimated rate should be 0 when empty", () => {
      const bf = new BloomFilter(100, 0.01);
      expect(bf.estimatedFalsePositiveRate()).toBe(0);
    });

    test("estimated rate should increase with insertions", () => {
      const bf = new BloomFilter(100, 0.01);
      bf.add("a");
      const rateAfterOne = bf.estimatedFalsePositiveRate();
      expect(rateAfterOne).toBeGreaterThan(0);

      for (let i = 0; i < 50; i++) {
        bf.add(`item_${i}`);
      }
      const rateAfterMany = bf.estimatedFalsePositiveRate();
      expect(rateAfterMany).toBeGreaterThan(rateAfterOne);
    });

    test("actual false positive rate should be within reasonable bounds", () => {
      const n = 1000;
      const targetRate = 0.05;
      const bf = new BloomFilter(n, targetRate);

      for (let i = 0; i < n; i++) {
        bf.add(`item_${i}`);
      }

      // Verify all inserted items are found (no false negatives)
      for (let i = 0; i < n; i++) {
        expect(bf.contains(`item_${i}`)).toBe(true);
      }

      // Measure actual false positive rate
      let falsePositives = 0;
      const testCount = 10000;
      for (let i = 0; i < testCount; i++) {
        if (bf.contains(`notinserted_${i}`)) {
          falsePositives++;
        }
      }

      const actualRate = falsePositives / testCount;
      // Allow 3x slack for randomness
      expect(actualRate).toBeLessThan(targetRate * 3);
    });
  });

  describe("edge cases", () => {
    test("should handle very small expected items", () => {
      const bf = new BloomFilter(1, 0.01);
      bf.add("only");
      expect(bf.contains("only")).toBe(true);
      expect(bf.bitArraySize).toBeGreaterThanOrEqual(64); // minimum size
    });

    test("should handle zero or negative false positive rate gracefully", () => {
      // Constructor clamps to 0.0001 internally
      const bf = new BloomFilter(100, 0);
      bf.add("test");
      expect(bf.contains("test")).toBe(true);
    });

    test("contains on empty filter returns false", () => {
      const bf = new BloomFilter(100, 0.01);
      expect(bf.contains("anything")).toBe(false);
    });
  });
});

// ============================================================================
// CountingBloomFilter
// ============================================================================
describe("CountingBloomFilter", () => {
  describe("add and contains", () => {
    test("should add and check membership", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      expect(cbf.add("a")).toBe(true);
      expect(cbf.add("b")).toBe(true);
      expect(cbf.contains("a")).toBe(true);
      expect(cbf.contains("b")).toBe(true);
      expect(cbf.count).toBe(2);
    });

    test("should report missing items as not contained", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("hello");
      expect(cbf.contains("missing_item_xyz_456")).toBe(false);
    });

    test("should allow adding same item multiple times", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("a");
      cbf.add("a");
      expect(cbf.count).toBe(2);
      expect(cbf.contains("a")).toBe(true);
    });
  });

  describe("delete (remove) support", () => {
    test("should delete items successfully", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("a");
      cbf.add("b");
      expect(cbf.delete("a")).toBe(true);
      expect(cbf.contains("a")).toBe(false);
      expect(cbf.contains("b")).toBe(true);
      expect(cbf.count).toBe(1);
    });

    test("should return false when deleting non-existent item", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      expect(cbf.delete("nope")).toBe(false);
    });

    test("should handle add-delete-add cycle", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("x");
      expect(cbf.contains("x")).toBe(true);
      cbf.delete("x");
      expect(cbf.contains("x")).toBe(false);
      cbf.add("x");
      expect(cbf.contains("x")).toBe(true);
    });

    test("should not allow double-delete (underflow protection)", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("a");
      expect(cbf.delete("a")).toBe(true);
      expect(cbf.delete("a")).toBe(false);
    });

    test("deleting one of many identical adds should not remove membership if other adds remain", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      cbf.add("x");
      cbf.add("x");
      cbf.delete("x"); // count goes from 2 to 1 on each counter
      // After one delete, counters still > 0 so contains should remain true
      expect(cbf.contains("x")).toBe(true);
    });
  });

  describe("counter overflow", () => {
    test("should return false on overflow with small counter bits", () => {
      // With 2-bit counters, max count is 3
      const cbf = new CountingBloomFilter(10, 0.01, 2);
      expect(cbf.add("x")).toBe(true);
      expect(cbf.add("x")).toBe(true);
      expect(cbf.add("x")).toBe(true);
      // 4th add should fail since counter reached max
      expect(cbf.add("x")).toBe(false);
    });

    test("should not corrupt state after overflow failure", () => {
      const cbf = new CountingBloomFilter(10, 0.01, 2);
      cbf.add("x");
      cbf.add("x");
      cbf.add("x");
      cbf.add("x"); // fails
      // The item should still be contained
      expect(cbf.contains("x")).toBe(true);
    });
  });

  describe("edge cases", () => {
    test("default constructor arguments", () => {
      const cbf = new CountingBloomFilter();
      expect(cbf.count).toBe(0);
      cbf.add("test");
      expect(cbf.contains("test")).toBe(true);
    });

    test("contains on empty filter returns false", () => {
      const cbf = new CountingBloomFilter(100, 0.01);
      expect(cbf.contains("anything")).toBe(false);
    });
  });
});

// ============================================================================
// ScalableBloomFilter
// ============================================================================
describe("ScalableBloomFilter", () => {
  describe("basic operations", () => {
    test("should add and check membership", () => {
      const sbf = new ScalableBloomFilter(100, 0.01);
      sbf.add("hello");
      sbf.add("world");
      expect(sbf.contains("hello")).toBe(true);
      expect(sbf.contains("world")).toBe(true);
      expect(sbf.count).toBe(2);
    });

    test("should not double-count duplicates", () => {
      const sbf = new ScalableBloomFilter(100, 0.01);
      sbf.add("hello");
      sbf.add("hello");
      expect(sbf.count).toBe(1);
    });

    test("should report false for items not added", () => {
      const sbf = new ScalableBloomFilter(100, 0.01);
      sbf.add("hello");
      expect(sbf.contains("missing_item_abc_789")).toBe(false);
    });
  });

  describe("auto-growth", () => {
    test("should start with one filter", () => {
      const sbf = new ScalableBloomFilter(100, 0.01);
      expect(sbf.numFilters).toBe(1);
    });

    test("should grow when capacity is reached", () => {
      const sbf = new ScalableBloomFilter(10, 0.01);
      for (let i = 0; i < 30; i++) {
        sbf.add(`item_${i}`);
      }
      expect(sbf.numFilters).toBeGreaterThan(1);
      expect(sbf.count).toBe(30);
    });

    test("should create additional filters as items increase beyond multiple capacities", () => {
      const sbf = new ScalableBloomFilter(5, 0.01, 2);
      for (let i = 0; i < 100; i++) {
        sbf.add(`item_${i}`);
      }
      expect(sbf.numFilters).toBeGreaterThan(2);
      // Count may be slightly less than 100 due to false positives in contains check
      expect(sbf.count).toBeGreaterThanOrEqual(95);
      expect(sbf.count).toBeLessThanOrEqual(100);
    });

    test("all items should still be found after growth", () => {
      const sbf = new ScalableBloomFilter(10, 0.01);
      const items: string[] = [];
      for (let i = 0; i < 50; i++) {
        const item = `item_${i}`;
        items.push(item);
        sbf.add(item);
      }

      // No false negatives
      for (const item of items) {
        expect(sbf.contains(item)).toBe(true);
      }
    });
  });

  describe("false positive behavior", () => {
    test("should not have excessive false positives after scaling", () => {
      const sbf = new ScalableBloomFilter(100, 0.05);
      for (let i = 0; i < 1000; i++) {
        sbf.add(`inserted_${i}`);
      }

      let falsePositives = 0;
      const testCount = 5000;
      for (let i = 0; i < testCount; i++) {
        if (sbf.contains(`notinserted_${i}`)) {
          falsePositives++;
        }
      }

      const rate = falsePositives / testCount;
      expect(rate).toBeLessThan(0.15);
    });
  });

  describe("edge cases", () => {
    test("default constructor arguments", () => {
      const sbf = new ScalableBloomFilter();
      expect(sbf.numFilters).toBe(1);
      expect(sbf.count).toBe(0);
    });

    test("contains on empty scalable filter returns false", () => {
      const sbf = new ScalableBloomFilter(100, 0.01);
      expect(sbf.contains("anything")).toBe(false);
    });

    test("custom growth rate", () => {
      const sbf = new ScalableBloomFilter(5, 0.01, 4);
      for (let i = 0; i < 25; i++) {
        sbf.add(`item_${i}`);
      }
      // With growth rate 4 and initial capacity 5, second filter has capacity 20
      // So 25 items might fit in 2 filters
      expect(sbf.numFilters).toBeGreaterThanOrEqual(2);
      expect(sbf.count).toBe(25);
    });
  });
});
