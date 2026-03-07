package dsa.datastructures;

import java.util.BitSet;

public class BloomFilter {

    private final BitSet bitSet;
    private final int bitSize;
    private final int numHashFunctions;
    private int insertedCount;

    /**
     * Creates a BloomFilter with optimal size and hash count based on
     * expected insertions and desired false positive rate.
     *
     * @param expectedInsertions the expected number of elements to insert
     * @param falsePositiveRate  the desired false positive probability (e.g. 0.01 for 1%)
     */
    public BloomFilter(int expectedInsertions, double falsePositiveRate) {
        this.bitSize = optimalBitSize(expectedInsertions, falsePositiveRate);
        this.numHashFunctions = optimalHashCount(expectedInsertions, bitSize);
        this.bitSet = new BitSet(bitSize);
        this.insertedCount = 0;
    }

    /**
     * Creates a BloomFilter with explicit size and hash count.
     *
     * @param bitSize          the number of bits in the filter
     * @param numHashFunctions the number of hash functions to use
     */
    public BloomFilter(int bitSize, int numHashFunctions, @SuppressWarnings("unused") boolean explicit) {
        this.bitSize = bitSize;
        this.numHashFunctions = numHashFunctions;
        this.bitSet = new BitSet(bitSize);
        this.insertedCount = 0;
    }

    /**
     * Adds an element to the Bloom filter.
     */
    public void add(String element) {
        for (int i = 0; i < numHashFunctions; i++) {
            int index = getHash(element, i);
            bitSet.set(index);
        }
        insertedCount++;
    }

    /**
     * Tests whether an element might be in the set.
     * False positives are possible, false negatives are not.
     */
    public boolean mightContain(String element) {
        for (int i = 0; i < numHashFunctions; i++) {
            int index = getHash(element, i);
            if (!bitSet.get(index)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Returns the expected false positive rate based on the current state
     * (number of inserted elements, bit size, and hash count).
     * Formula: (1 - e^(-k*n/m))^k
     */
    public double expectedFalsePositiveRate() {
        if (insertedCount == 0) return 0.0;
        double exponent = -((double) numHashFunctions * insertedCount) / bitSize;
        return Math.pow(1.0 - Math.exp(exponent), numHashFunctions);
    }

    /**
     * Returns the bit size of the filter.
     */
    public int size() {
        return bitSize;
    }

    /**
     * Returns the number of hash functions.
     */
    public int hashCount() {
        return numHashFunctions;
    }

    /**
     * Returns the number of elements inserted.
     */
    public int insertedCount() {
        return insertedCount;
    }

    /**
     * Clears the Bloom filter.
     */
    public void clear() {
        bitSet.clear();
        insertedCount = 0;
    }

    /**
     * Generates a hash for the given element using the i-th hash function.
     * Uses double hashing: h(i) = (h1 + i * h2) mod m
     */
    private int getHash(String element, int i) {
        int h1 = element.hashCode();
        int h2 = fnvHash(element);
        long combined = (long) h1 + (long) i * h2;
        return (int) (Math.abs(combined) % bitSize);
    }

    /**
     * FNV-1a hash for the second hash function.
     */
    private int fnvHash(String element) {
        int hash = 0x811c9dc5;
        for (int i = 0; i < element.length(); i++) {
            hash ^= element.charAt(i);
            hash *= 0x01000193;
        }
        return hash;
    }

    /**
     * Computes the optimal bit size: m = -(n * ln(p)) / (ln(2)^2)
     */
    private static int optimalBitSize(int expectedInsertions, double falsePositiveRate) {
        double m = -(expectedInsertions * Math.log(falsePositiveRate)) / (Math.log(2) * Math.log(2));
        return Math.max(1, (int) Math.ceil(m));
    }

    /**
     * Computes the optimal number of hash functions: k = (m/n) * ln(2)
     */
    private static int optimalHashCount(int expectedInsertions, int bitSize) {
        double k = ((double) bitSize / expectedInsertions) * Math.log(2);
        return Math.max(1, (int) Math.round(k));
    }

    /**
     * A Bloom filter variant that uses integer counters instead of single bits,
     * supporting deletion of elements in addition to insertion.
     */
    public static class CountingBloomFilter {

        private final int[] counters;
        private final int counterSize;
        private final int numHashFunctions;
        private int insertedCount;

        public CountingBloomFilter(int expectedInsertions, double falsePositiveRate) {
            this.counterSize = optimalBitSize(expectedInsertions, falsePositiveRate);
            this.numHashFunctions = optimalHashCount(expectedInsertions, counterSize);
            this.counters = new int[counterSize];
            this.insertedCount = 0;
        }

        public void add(String element) {
            for (int i = 0; i < numHashFunctions; i++) {
                int index = getHash(element, i);
                counters[index]++;
            }
            insertedCount++;
        }

        /**
         * Removes an element from the filter by decrementing its counters.
         * Returns false if any counter is already zero (element might not exist).
         */
        public boolean remove(String element) {
            if (!mightContain(element)) {
                return false;
            }
            for (int i = 0; i < numHashFunctions; i++) {
                int index = getHash(element, i);
                counters[index]--;
            }
            insertedCount--;
            return true;
        }

        public boolean mightContain(String element) {
            for (int i = 0; i < numHashFunctions; i++) {
                int index = getHash(element, i);
                if (counters[index] <= 0) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Returns the minimum counter value across all hash positions for the element.
         * This serves as an estimate of how many times the element was inserted.
         */
        public int count(String element) {
            int min = Integer.MAX_VALUE;
            for (int i = 0; i < numHashFunctions; i++) {
                int index = getHash(element, i);
                min = Math.min(min, counters[index]);
            }
            return Math.max(0, min);
        }

        public double expectedFalsePositiveRate() {
            if (insertedCount == 0) return 0.0;
            double exponent = -((double) numHashFunctions * insertedCount) / counterSize;
            return Math.pow(1.0 - Math.exp(exponent), numHashFunctions);
        }

        public int size() {
            return counterSize;
        }

        public int insertedCount() {
            return insertedCount;
        }

        public void clear() {
            java.util.Arrays.fill(counters, 0);
            insertedCount = 0;
        }

        private int getHash(String element, int i) {
            int h1 = element.hashCode();
            int h2 = fnvHash(element);
            long combined = (long) h1 + (long) i * h2;
            return (int) (Math.abs(combined) % counterSize);
        }

        private int fnvHash(String element) {
            int hash = 0x811c9dc5;
            for (int j = 0; j < element.length(); j++) {
                hash ^= element.charAt(j);
                hash *= 0x01000193;
            }
            return hash;
        }
    }

    /**
     * A Bloom filter that automatically grows by adding new internal BloomFilter
     * instances when the current one exceeds its target false positive rate.
     */
    public static class ScalableBloomFilter {

        private final java.util.List<BloomFilter> filters;
        private final double falsePositiveRate;
        private final double growthFactor;
        private int currentCapacity;
        private int totalInserted;

        public ScalableBloomFilter(int initialCapacity, double falsePositiveRate, double growthFactor) {
            if (initialCapacity <= 0) {
                throw new IllegalArgumentException("Initial capacity must be positive");
            }
            if (falsePositiveRate <= 0 || falsePositiveRate >= 1) {
                throw new IllegalArgumentException("False positive rate must be between 0 and 1");
            }
            if (growthFactor <= 1) {
                throw new IllegalArgumentException("Growth factor must be greater than 1");
            }
            this.falsePositiveRate = falsePositiveRate;
            this.growthFactor = growthFactor;
            this.currentCapacity = initialCapacity;
            this.totalInserted = 0;
            this.filters = new java.util.ArrayList<>();
            this.filters.add(new BloomFilter(initialCapacity, falsePositiveRate));
        }

        public void add(String element) {
            BloomFilter current = filters.get(filters.size() - 1);
            if (current.insertedCount() >= currentCapacity) {
                currentCapacity = (int) Math.ceil(currentCapacity * growthFactor);
                BloomFilter next = new BloomFilter(currentCapacity, falsePositiveRate);
                filters.add(next);
                current = next;
            }
            current.add(element);
            totalInserted++;
        }

        public boolean mightContain(String element) {
            for (BloomFilter filter : filters) {
                if (filter.mightContain(element)) {
                    return true;
                }
            }
            return false;
        }

        public int insertedCount() {
            return totalInserted;
        }

        public int filterCount() {
            return filters.size();
        }

        public void clear() {
            int numGrowths = filters.size() - 1;
            filters.clear();
            currentCapacity = (int) Math.ceil(currentCapacity / Math.pow(growthFactor, numGrowths));
            if (currentCapacity <= 0) currentCapacity = 1;
            filters.add(new BloomFilter(currentCapacity, falsePositiveRate));
            totalInserted = 0;
        }
    }
}
