/**
 * Bloom Filter Implementation
 * See bloom_filter.h for documentation
 */

#include "bloom_filter.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ============== Hash Functions ============== */

/* MurmurHash3 32-bit finalizer for mixing */
static uint32_t murmur_mix(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

/* FNV-1a hash */
static uint32_t fnv1a_hash(const void *data, size_t len) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }

    return hash;
}

/* DJB2 hash */
static uint32_t djb2_hash(const void *data, size_t len) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = 5381;

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + bytes[i];
    }

    return hash;
}

/* Generate k hashes using double hashing technique */
static void get_hash_indices(const void *data, size_t len, size_t num_bits,
                             size_t num_hashes, size_t *indices) {
    uint32_t hash1 = fnv1a_hash(data, len);
    uint32_t hash2 = djb2_hash(data, len);

    for (size_t i = 0; i < num_hashes; i++) {
        /* Enhanced double hashing: h(i) = h1 + i*h2 + i*i */
        uint32_t combined = hash1 + (uint32_t)i * hash2 + (uint32_t)(i * i);
        combined = murmur_mix(combined);
        indices[i] = combined % num_bits;
    }
}

/* ============== Bit Operations ============== */

static inline void set_bit(uint8_t *bits, size_t index) {
    bits[index / 8] |= (1 << (index % 8));
}

static inline bool get_bit(const uint8_t *bits, size_t index) {
    return (bits[index / 8] & (1 << (index % 8))) != 0;
}

/* ============== Creation/Destruction ============== */

BloomFilter *bloom_create(size_t num_bits, size_t num_hashes) {
    if (num_bits == 0 || num_hashes == 0) return NULL;

    BloomFilter *filter = malloc(sizeof(BloomFilter));
    if (filter == NULL) return NULL;

    /* Round up to nearest byte */
    size_t num_bytes = (num_bits + 7) / 8;

    filter->bits = calloc(num_bytes, sizeof(uint8_t));
    if (filter->bits == NULL) {
        free(filter);
        return NULL;
    }

    filter->num_bits = num_bits;
    filter->num_hashes = num_hashes;
    filter->count = 0;

    return filter;
}

BloomFilter *bloom_create_optimal(size_t expected_elements, double false_positive_rate) {
    if (expected_elements == 0 || false_positive_rate <= 0 || false_positive_rate >= 1) {
        return NULL;
    }

    /* Optimal number of bits: m = -n * ln(p) / (ln(2)^2) */
    double ln2 = 0.693147180559945;
    double ln2_sq = ln2 * ln2;
    double m = -((double)expected_elements * log(false_positive_rate)) / ln2_sq;

    /* Optimal number of hashes: k = (m/n) * ln(2) */
    double k = (m / (double)expected_elements) * ln2;

    size_t num_bits = (size_t)ceil(m);
    size_t num_hashes = (size_t)ceil(k);

    /* Ensure at least 1 hash function */
    if (num_hashes == 0) num_hashes = 1;

    return bloom_create(num_bits, num_hashes);
}

void bloom_destroy(BloomFilter *filter) {
    if (filter == NULL) return;
    free(filter->bits);
    free(filter);
}

void bloom_clear(BloomFilter *filter) {
    if (filter == NULL) return;
    size_t num_bytes = (filter->num_bits + 7) / 8;
    memset(filter->bits, 0, num_bytes);
    filter->count = 0;
}

/* ============== Core Operations ============== */

void bloom_add_bytes(BloomFilter *filter, const void *data, size_t len) {
    if (filter == NULL || data == NULL || len == 0) return;

    size_t *indices = malloc(filter->num_hashes * sizeof(size_t));
    if (indices == NULL) return;

    get_hash_indices(data, len, filter->num_bits, filter->num_hashes, indices);

    for (size_t i = 0; i < filter->num_hashes; i++) {
        set_bit(filter->bits, indices[i]);
    }

    filter->count++;
    free(indices);
}

void bloom_add(BloomFilter *filter, const char *str) {
    if (filter == NULL || str == NULL) return;
    bloom_add_bytes(filter, str, strlen(str));
}

bool bloom_contains_bytes(const BloomFilter *filter, const void *data, size_t len) {
    if (filter == NULL || data == NULL || len == 0) return false;

    size_t *indices = malloc(filter->num_hashes * sizeof(size_t));
    if (indices == NULL) return false;

    get_hash_indices(data, len, filter->num_bits, filter->num_hashes, indices);

    bool result = true;
    for (size_t i = 0; i < filter->num_hashes; i++) {
        if (!get_bit(filter->bits, indices[i])) {
            result = false;
            break;
        }
    }

    free(indices);
    return result;
}

bool bloom_contains(const BloomFilter *filter, const char *str) {
    if (filter == NULL || str == NULL) return false;
    return bloom_contains_bytes(filter, str, strlen(str));
}

/* ============== Utility ============== */

size_t bloom_bits_set(const BloomFilter *filter) {
    if (filter == NULL) return 0;

    size_t count = 0;
    size_t num_bytes = (filter->num_bits + 7) / 8;

    for (size_t i = 0; i < num_bytes; i++) {
        uint8_t byte = filter->bits[i];
        /* Count bits using Brian Kernighan's algorithm */
        while (byte) {
            count++;
            byte &= (byte - 1);
        }
    }

    return count;
}

size_t bloom_count(const BloomFilter *filter) {
    return filter ? filter->count : 0;
}

double bloom_false_positive_rate(const BloomFilter *filter) {
    if (filter == NULL || filter->num_bits == 0) return 1.0;

    /* Estimate based on formula: (1 - e^(-kn/m))^k */
    double m = (double)filter->num_bits;
    double k = (double)filter->num_hashes;
    double n = (double)filter->count;

    double exponent = -k * n / m;
    double base = 1.0 - exp(exponent);

    return pow(base, k);
}

size_t bloom_num_bits(const BloomFilter *filter) {
    return filter ? filter->num_bits : 0;
}

size_t bloom_num_hashes(const BloomFilter *filter) {
    return filter ? filter->num_hashes : 0;
}

bool bloom_merge(BloomFilter *dest, const BloomFilter *src) {
    if (dest == NULL || src == NULL) return false;
    if (dest->num_bits != src->num_bits) return false;
    if (dest->num_hashes != src->num_hashes) return false;

    size_t num_bytes = (dest->num_bits + 7) / 8;
    for (size_t i = 0; i < num_bytes; i++) {
        dest->bits[i] |= src->bits[i];
    }

    dest->count += src->count;
    return true;
}
