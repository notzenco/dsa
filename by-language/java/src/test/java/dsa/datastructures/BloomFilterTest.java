package dsa.datastructures;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class BloomFilterTest {

    @Test
    void addAndMightContain() {
        var bf = new BloomFilter(100, 0.01);
        bf.add("hello");
        bf.add("world");
        assertTrue(bf.mightContain("hello"));
        assertTrue(bf.mightContain("world"));
    }

    @Test
    void mightContainReturnsFalseForUnadded() {
        var bf = new BloomFilter(100, 0.01);
        bf.add("hello");
        // It's possible (but unlikely with good params) for a false positive,
        // so we test several strings that were not added
        // With a well-sized filter, most should return false
        int falsePositives = 0;
        for (int i = 0; i < 100; i++) {
            if (bf.mightContain("not_added_" + i)) {
                falsePositives++;
            }
        }
        // With 1% false positive rate and only 1 element inserted, false positives should be very rare
        assertTrue(falsePositives < 10, "Too many false positives: " + falsePositives);
    }

    @Test
    void noFalseNegatives() {
        var bf = new BloomFilter(1000, 0.01);
        for (int i = 0; i < 500; i++) {
            bf.add("item_" + i);
        }
        // Every added item MUST be found (no false negatives)
        for (int i = 0; i < 500; i++) {
            assertTrue(bf.mightContain("item_" + i), "False negative for item_" + i);
        }
    }

    @Test
    void falsePositiveRate() {
        int n = 1000;
        double targetFpr = 0.05;
        var bf = new BloomFilter(n, targetFpr);
        for (int i = 0; i < n; i++) {
            bf.add("element_" + i);
        }

        // Test false positive rate empirically
        int tests = 10000;
        int falsePositives = 0;
        for (int i = 0; i < tests; i++) {
            if (bf.mightContain("not_element_" + i)) {
                falsePositives++;
            }
        }
        double empiricalFpr = (double) falsePositives / tests;
        // Allow some tolerance - empirical rate should be in a reasonable range
        assertTrue(empiricalFpr < targetFpr * 3,
                "Empirical FPR too high: " + empiricalFpr + " (target: " + targetFpr + ")");
    }

    @Test
    void expectedFalsePositiveRateCalculation() {
        var bf = new BloomFilter(100, 0.01);
        assertEquals(0.0, bf.expectedFalsePositiveRate(), 0.001);
        bf.add("test");
        assertTrue(bf.expectedFalsePositiveRate() > 0);
        assertTrue(bf.expectedFalsePositiveRate() < 1);
    }

    @Test
    void expectedFalsePositiveRateIncreasesWithInsertions() {
        var bf = new BloomFilter(100, 0.01);
        bf.add("a");
        double rate1 = bf.expectedFalsePositiveRate();
        for (int i = 0; i < 50; i++) {
            bf.add("item_" + i);
        }
        double rate2 = bf.expectedFalsePositiveRate();
        assertTrue(rate2 > rate1, "FPR should increase with more insertions");
    }

    @Test
    void sizeReturnsFilterBitSize() {
        var bf = new BloomFilter(100, 0.01);
        assertTrue(bf.size() > 0);
        // For n=100, p=0.01, optimal m ~ 959
        assertTrue(bf.size() > 100);
    }

    @Test
    void hashCountIsOptimal() {
        var bf = new BloomFilter(100, 0.01);
        assertTrue(bf.hashCount() > 0);
        // For n=100, p=0.01, optimal k ~ 7
        assertTrue(bf.hashCount() >= 3 && bf.hashCount() <= 15);
    }

    @Test
    void clear() {
        var bf = new BloomFilter(100, 0.01);
        bf.add("hello");
        bf.add("world");
        bf.clear();
        assertEquals(0, bf.insertedCount());
        assertEquals(0.0, bf.expectedFalsePositiveRate());
        // After clear, previously added items should (most likely) not be found
        // Note: this is probabilistic, but with a cleared filter it's guaranteed
        assertFalse(bf.mightContain("hello"));
        assertFalse(bf.mightContain("world"));
    }

    @Test
    void insertedCount() {
        var bf = new BloomFilter(100, 0.01);
        assertEquals(0, bf.insertedCount());
        bf.add("a");
        assertEquals(1, bf.insertedCount());
        bf.add("b");
        assertEquals(2, bf.insertedCount());
        bf.add("a"); // Adding same element again increments count
        assertEquals(3, bf.insertedCount());
    }

    @Test
    void explicitConstructor() {
        var bf = new BloomFilter(1024, 5, true);
        assertEquals(1024, bf.size());
        assertEquals(5, bf.hashCount());
        bf.add("test");
        assertTrue(bf.mightContain("test"));
    }

    @Test
    void emptyFilterHasNoElements() {
        var bf = new BloomFilter(100, 0.01);
        assertFalse(bf.mightContain("anything"));
        assertFalse(bf.mightContain(""));
        assertFalse(bf.mightContain("test123"));
    }

    @Test
    void specialCharacterStrings() {
        var bf = new BloomFilter(100, 0.01);
        bf.add("");
        bf.add("special!@#$%");
        bf.add("unicode\u00e9\u00e8\u00ea");
        assertTrue(bf.mightContain(""));
        assertTrue(bf.mightContain("special!@#$%"));
        assertTrue(bf.mightContain("unicode\u00e9\u00e8\u00ea"));
    }

    @Test
    void smallFilterHighFpr() {
        // Very small filter should have high FPR
        var bf = new BloomFilter(10, 0.5);
        for (int i = 0; i < 10; i++) {
            bf.add("item_" + i);
        }
        // Expected FPR should be significant
        assertTrue(bf.expectedFalsePositiveRate() > 0.1);
    }

    // --- CountingBloomFilter tests ---

    @Test
    void countingAddAndContain() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        cbf.add("hello");
        cbf.add("world");
        assertTrue(cbf.mightContain("hello"));
        assertTrue(cbf.mightContain("world"));
        assertFalse(cbf.mightContain("missing"));
    }

    @Test
    void countingRemove() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        cbf.add("hello");
        assertTrue(cbf.mightContain("hello"));
        assertTrue(cbf.remove("hello"));
        assertFalse(cbf.mightContain("hello"));
    }

    @Test
    void countingRemoveNonExistent() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        cbf.add("hello");
        assertFalse(cbf.remove("not_present"));
        // Original element should still be there
        assertTrue(cbf.mightContain("hello"));
    }

    @Test
    void countingMultipleAddsAndRemoves() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        cbf.add("x");
        cbf.add("x");
        cbf.add("x");
        assertEquals(3, cbf.count("x"));
        assertTrue(cbf.remove("x"));
        assertTrue(cbf.mightContain("x")); // still present (count=2)
        assertTrue(cbf.remove("x"));
        assertTrue(cbf.mightContain("x")); // still present (count=1)
        assertTrue(cbf.remove("x"));
        assertFalse(cbf.mightContain("x")); // now gone
    }

    @Test
    void countingCountAccuracy() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        assertEquals(0, cbf.count("x"));
        cbf.add("x");
        assertTrue(cbf.count("x") >= 1);
        cbf.add("x");
        assertTrue(cbf.count("x") >= 2);
    }

    @Test
    void countingInsertedCount() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        assertEquals(0, cbf.insertedCount());
        cbf.add("a");
        cbf.add("b");
        assertEquals(2, cbf.insertedCount());
        cbf.remove("a");
        assertEquals(1, cbf.insertedCount());
    }

    @Test
    void countingClear() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        cbf.add("a");
        cbf.add("b");
        cbf.clear();
        assertEquals(0, cbf.insertedCount());
        assertFalse(cbf.mightContain("a"));
        assertFalse(cbf.mightContain("b"));
    }

    @Test
    void countingFalsePositiveRate() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        assertEquals(0.0, cbf.expectedFalsePositiveRate());
        cbf.add("test");
        assertTrue(cbf.expectedFalsePositiveRate() > 0);
    }

    @Test
    void countingSize() {
        var cbf = new BloomFilter.CountingBloomFilter(100, 0.01);
        assertTrue(cbf.size() > 0);
    }

    // --- ScalableBloomFilter tests ---

    @Test
    void scalableBasicAddAndContain() {
        var sbf = new BloomFilter.ScalableBloomFilter(10, 0.01, 2.0);
        sbf.add("hello");
        sbf.add("world");
        assertTrue(sbf.mightContain("hello"));
        assertTrue(sbf.mightContain("world"));
        assertFalse(sbf.mightContain("missing"));
    }

    @Test
    void scalableAutoScaling() {
        var sbf = new BloomFilter.ScalableBloomFilter(5, 0.01, 2.0);
        assertEquals(1, sbf.filterCount());
        for (int i = 0; i < 20; i++) {
            sbf.add("item_" + i);
        }
        // Should have created additional filters
        assertTrue(sbf.filterCount() > 1);
        assertEquals(20, sbf.insertedCount());
    }

    @Test
    void scalableContainsAcrossFilters() {
        var sbf = new BloomFilter.ScalableBloomFilter(5, 0.01, 2.0);
        for (int i = 0; i < 20; i++) {
            sbf.add("item_" + i);
        }
        // All items should be found across all internal filters
        for (int i = 0; i < 20; i++) {
            assertTrue(sbf.mightContain("item_" + i), "Missing item_" + i);
        }
    }

    @Test
    void scalableClear() {
        var sbf = new BloomFilter.ScalableBloomFilter(5, 0.01, 2.0);
        for (int i = 0; i < 20; i++) {
            sbf.add("item_" + i);
        }
        sbf.clear();
        assertEquals(0, sbf.insertedCount());
        assertEquals(1, sbf.filterCount());
        assertFalse(sbf.mightContain("item_0"));
    }

    @Test
    void scalableInvalidConstructorArgs() {
        assertThrows(IllegalArgumentException.class,
                () -> new BloomFilter.ScalableBloomFilter(0, 0.01, 2.0));
        assertThrows(IllegalArgumentException.class,
                () -> new BloomFilter.ScalableBloomFilter(10, 0.0, 2.0));
        assertThrows(IllegalArgumentException.class,
                () -> new BloomFilter.ScalableBloomFilter(10, 1.0, 2.0));
        assertThrows(IllegalArgumentException.class,
                () -> new BloomFilter.ScalableBloomFilter(10, 0.01, 1.0));
    }

    @Test
    void scalableNoFalseNegatives() {
        var sbf = new BloomFilter.ScalableBloomFilter(10, 0.01, 2.0);
        for (int i = 0; i < 100; i++) {
            sbf.add("element_" + i);
        }
        for (int i = 0; i < 100; i++) {
            assertTrue(sbf.mightContain("element_" + i),
                    "False negative for element_" + i);
        }
    }
}
