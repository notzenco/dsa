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
}
