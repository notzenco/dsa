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
}
