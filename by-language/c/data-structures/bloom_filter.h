/**
 * BLOOM FILTER
 *
 * A space-efficient probabilistic data structure for membership testing.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║   Element "hello" -> hash1("hello") = 3                                   ║
 * ║                   -> hash2("hello") = 7                                   ║
 * ║                   -> hash3("hello") = 12                                  ║
 * ║                                                                           ║
 * ║   Bit Array:                                                              ║
 * ║   ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐      ║
 * ║   │ 0 │ 0 │ 0 │ 1 │ 0 │ 0 │ 0 │ 1 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │ 0 │ 0 │      ║
 * ║   └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘      ║
 * ║     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15        ║
 * ║                 ↑               ↑                   ↑                     ║
 * ║              hash1          hash2               hash3                     ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Time   │ Space                                      ║
 * ║  ───────────────────┼────────┼──────────                                  ║
 * ║  Insert             │ O(k)   │ O(m)     m = bit array size                ║
 * ║  Query              │ O(k)   │ O(1)     k = number of hash functions      ║
 * ║                                                                           ║
 * ║  False Positive Rate ≈ (1 - e^(-kn/m))^k                                  ║
 * ║  Optimal k = (m/n) * ln(2)                                                ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * PROPERTIES:
 * - No false negatives (if says "not present", definitely not present)
 * - Possible false positives (if says "present", might not be)
 * - Cannot delete elements (use Counting Bloom Filter for deletion)
 */

#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ============== Bloom Filter Structure ============== */

typedef struct {
    uint8_t *bits;          /* Bit array */
    size_t num_bits;        /* Size of bit array */
    size_t num_hashes;      /* Number of hash functions */
    size_t count;           /* Approximate count of inserted elements */
} BloomFilter;

/* ============== Creation/Destruction ============== */

/**
 * Create a Bloom filter.
 * @param num_bits Size of bit array
 * @param num_hashes Number of hash functions
 * @return New Bloom filter or NULL on failure
 */
BloomFilter *bloom_create(size_t num_bits, size_t num_hashes);

/**
 * Create a Bloom filter with optimal parameters.
 * @param expected_elements Expected number of elements
 * @param false_positive_rate Desired false positive rate (0 < rate < 1)
 * @return New Bloom filter or NULL on failure
 */
BloomFilter *bloom_create_optimal(size_t expected_elements, double false_positive_rate);

/**
 * Destroy a Bloom filter.
 * @param filter Filter to destroy
 */
void bloom_destroy(BloomFilter *filter);

/**
 * Clear all bits in the filter.
 * @param filter Filter to clear
 */
void bloom_clear(BloomFilter *filter);

/* ============== Core Operations ============== */

/**
 * Add a string to the filter.
 * @param filter Bloom filter
 * @param str String to add
 */
void bloom_add(BloomFilter *filter, const char *str);

/**
 * Add raw bytes to the filter.
 * @param filter Bloom filter
 * @param data Data to add
 * @param len Length of data
 */
void bloom_add_bytes(BloomFilter *filter, const void *data, size_t len);

/**
 * Check if a string might be in the filter.
 * @param filter Bloom filter
 * @param str String to check
 * @return true if possibly present, false if definitely not present
 */
bool bloom_contains(const BloomFilter *filter, const char *str);

/**
 * Check if raw bytes might be in the filter.
 * @param filter Bloom filter
 * @param data Data to check
 * @param len Length of data
 * @return true if possibly present, false if definitely not present
 */
bool bloom_contains_bytes(const BloomFilter *filter, const void *data, size_t len);

/* ============== Utility ============== */

/**
 * Get the number of bits set in the filter.
 * @param filter Bloom filter
 * @return Number of set bits
 */
size_t bloom_bits_set(const BloomFilter *filter);

/**
 * Get approximate count of inserted elements.
 * @param filter Bloom filter
 * @return Approximate count
 */
size_t bloom_count(const BloomFilter *filter);

/**
 * Estimate current false positive rate.
 * @param filter Bloom filter
 * @return Estimated false positive rate
 */
double bloom_false_positive_rate(const BloomFilter *filter);

/**
 * Get the size of the bit array.
 * @param filter Bloom filter
 * @return Number of bits
 */
size_t bloom_num_bits(const BloomFilter *filter);

/**
 * Get the number of hash functions.
 * @param filter Bloom filter
 * @return Number of hash functions
 */
size_t bloom_num_hashes(const BloomFilter *filter);

/**
 * Merge two Bloom filters (OR operation).
 * Filters must have same size and number of hash functions.
 * @param dest Destination filter
 * @param src Source filter
 * @return true on success, false on incompatible filters
 */
bool bloom_merge(BloomFilter *dest, const BloomFilter *src);

#endif /* BLOOM_FILTER_H */
