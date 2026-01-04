//! # Bloom Filter
//!
//! A space-efficient probabilistic data structure for set membership testing.
//! May return false positives but never false negatives.
//!
//! ## Complexity Analysis
//!
//! | Operation | Time     | Space    |
//! |-----------|----------|----------|
//! | Insert    | O(k)     | O(1)     |
//! | Contains  | O(k)     | O(1)     |
//! | Space     | -        | O(m)     |
//!
//! Where k = number of hash functions, m = number of bits.
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::hashing::BloomFilter;
//!
//! let mut filter = BloomFilter::new(1000, 0.01);
//! filter.insert(&"hello");
//! filter.insert(&"world");
//!
//! assert!(filter.may_contain(&"hello"));
//! assert!(filter.may_contain(&"world"));
//! // may_contain could return true for items not inserted (false positive)
//! ```

use alloc::vec;
use alloc::vec::Vec;
use core::hash::{Hash, Hasher};

/// A Bloom filter for probabilistic set membership testing.
///
/// # Type Parameters
///
/// * `T` - The element type, must implement `Hash`
pub struct BloomFilter {
    bits: Vec<u64>,
    num_bits: usize,
    num_hashes: usize,
    count: usize,
}

impl BloomFilter {
    /// Creates a new Bloom filter with optimal parameters.
    ///
    /// # Arguments
    ///
    /// * `expected_items` - Expected number of items to insert
    /// * `false_positive_rate` - Desired false positive probability (0.0 to 1.0)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::BloomFilter;
    ///
    /// // Create filter for ~1000 items with 1% false positive rate
    /// let filter = BloomFilter::new(1000, 0.01);
    /// ```
    pub fn new(expected_items: usize, false_positive_rate: f64) -> Self {
        let expected_items = expected_items.max(1);
        let fp_rate = false_positive_rate.max(0.0001).min(0.5);

        // Optimal number of bits: m = -n * ln(p) / (ln(2)^2)
        let num_bits = (-(expected_items as f64) * fp_rate.ln() / (2.0_f64.ln().powi(2)))
            .ceil() as usize;
        let num_bits = num_bits.max(64);

        // Optimal number of hash functions: k = (m/n) * ln(2)
        let num_hashes = ((num_bits as f64 / expected_items as f64) * 2.0_f64.ln())
            .ceil() as usize;
        let num_hashes = num_hashes.max(1).min(16);

        // Number of u64 words needed
        let num_words = (num_bits + 63) / 64;

        BloomFilter {
            bits: vec![0u64; num_words],
            num_bits,
            num_hashes,
            count: 0,
        }
    }

    /// Creates a Bloom filter with specific bit size and hash count.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::BloomFilter;
    ///
    /// let filter = BloomFilter::with_size(1024, 7);
    /// ```
    pub fn with_size(num_bits: usize, num_hashes: usize) -> Self {
        let num_bits = num_bits.max(64);
        let num_hashes = num_hashes.max(1).min(16);
        let num_words = (num_bits + 63) / 64;

        BloomFilter {
            bits: vec![0u64; num_words],
            num_bits,
            num_hashes,
            count: 0,
        }
    }

    /// Returns the number of bits in the filter.
    pub fn num_bits(&self) -> usize {
        self.num_bits
    }

    /// Returns the number of hash functions.
    pub fn num_hashes(&self) -> usize {
        self.num_hashes
    }

    /// Returns the number of items inserted.
    pub fn count(&self) -> usize {
        self.count
    }

    /// Returns `true` if no items have been inserted.
    pub fn is_empty(&self) -> bool {
        self.count == 0
    }

    /// Computes hash values for an item.
    fn get_hash_indices<T: Hash>(&self, item: &T) -> Vec<usize> {
        // Use double hashing: h(i) = h1 + i * h2
        let mut hasher1 = FnvHasher::new();
        item.hash(&mut hasher1);
        let h1 = hasher1.finish();

        let mut hasher2 = FnvHasher::with_seed(0x517cc1b727220a95);
        item.hash(&mut hasher2);
        let h2 = hasher2.finish();

        (0..self.num_hashes)
            .map(|i| {
                let hash = h1.wrapping_add((i as u64).wrapping_mul(h2));
                (hash as usize) % self.num_bits
            })
            .collect()
    }

    /// Sets a bit at the given index.
    fn set_bit(&mut self, index: usize) {
        let word_idx = index / 64;
        let bit_idx = index % 64;
        self.bits[word_idx] |= 1u64 << bit_idx;
    }

    /// Gets a bit at the given index.
    fn get_bit(&self, index: usize) -> bool {
        let word_idx = index / 64;
        let bit_idx = index % 64;
        (self.bits[word_idx] >> bit_idx) & 1 == 1
    }

    /// Inserts an item into the filter.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::BloomFilter;
    ///
    /// let mut filter = BloomFilter::new(100, 0.01);
    /// filter.insert(&"hello");
    /// assert!(filter.may_contain(&"hello"));
    /// ```
    pub fn insert<T: Hash>(&mut self, item: &T) {
        for idx in self.get_hash_indices(item) {
            self.set_bit(idx);
        }
        self.count += 1;
    }

    /// Checks if an item may be in the filter.
    ///
    /// Returns `true` if the item might be in the set (could be false positive).
    /// Returns `false` if the item is definitely not in the set.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::BloomFilter;
    ///
    /// let mut filter = BloomFilter::new(100, 0.01);
    /// filter.insert(&"hello");
    ///
    /// assert!(filter.may_contain(&"hello"));  // Definitely true
    /// // filter.may_contain(&"world") could be true or false
    /// ```
    pub fn may_contain<T: Hash>(&self, item: &T) -> bool {
        self.get_hash_indices(item).iter().all(|&idx| self.get_bit(idx))
    }

    /// Clears the filter.
    pub fn clear(&mut self) {
        self.bits.fill(0);
        self.count = 0;
    }

    /// Returns the estimated false positive rate based on current fill.
    pub fn estimated_fp_rate(&self) -> f64 {
        let ones = self.bits.iter().map(|w| w.count_ones() as usize).sum::<usize>();
        let fill_ratio = ones as f64 / self.num_bits as f64;
        fill_ratio.powi(self.num_hashes as i32)
    }

    /// Merges another Bloom filter into this one (union).
    ///
    /// Both filters must have the same size and hash count.
    pub fn merge(&mut self, other: &BloomFilter) -> bool {
        if self.num_bits != other.num_bits || self.num_hashes != other.num_hashes {
            return false;
        }

        for (a, b) in self.bits.iter_mut().zip(other.bits.iter()) {
            *a |= *b;
        }
        self.count += other.count;
        true
    }
}

/// FNV-1a hasher.
struct FnvHasher {
    state: u64,
}

impl FnvHasher {
    fn new() -> Self {
        FnvHasher {
            state: 0xcbf29ce484222325,
        }
    }

    fn with_seed(seed: u64) -> Self {
        FnvHasher { state: seed }
    }
}

impl Hasher for FnvHasher {
    fn finish(&self) -> u64 {
        self.state
    }

    fn write(&mut self, bytes: &[u8]) {
        for &byte in bytes {
            self.state ^= byte as u64;
            self.state = self.state.wrapping_mul(0x100000001b3);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let filter = BloomFilter::new(1000, 0.01);
            assert!(filter.is_empty());
            assert_eq!(filter.count(), 0);
            assert!(filter.num_bits() > 0);
            assert!(filter.num_hashes() > 0);
        }

        #[test]
        fn test_with_size() {
            let filter = BloomFilter::with_size(1024, 7);
            assert_eq!(filter.num_bits(), 1024);
            assert_eq!(filter.num_hashes(), 7);
        }
    }

    mod insert_and_contains {
        use super::*;

        #[test]
        fn test_insert_and_may_contain() {
            let mut filter = BloomFilter::new(100, 0.01);
            filter.insert(&"hello");

            assert!(filter.may_contain(&"hello"));
            assert_eq!(filter.count(), 1);
        }

        #[test]
        fn test_multiple_inserts() {
            let mut filter = BloomFilter::new(100, 0.01);
            filter.insert(&"apple");
            filter.insert(&"banana");
            filter.insert(&"cherry");

            assert!(filter.may_contain(&"apple"));
            assert!(filter.may_contain(&"banana"));
            assert!(filter.may_contain(&"cherry"));
        }

        #[test]
        fn test_integer_items() {
            let mut filter = BloomFilter::new(100, 0.01);

            for i in 0..50 {
                filter.insert(&i);
            }

            for i in 0..50 {
                assert!(filter.may_contain(&i));
            }
        }

        #[test]
        fn test_no_false_negatives() {
            let mut filter = BloomFilter::new(1000, 0.01);
            let items: Vec<i32> = (0..500).collect();

            for item in &items {
                filter.insert(item);
            }

            // All inserted items MUST be found (no false negatives)
            for item in &items {
                assert!(filter.may_contain(item), "False negative for {}", item);
            }
        }
    }

    mod false_positives {
        use super::*;

        #[test]
        fn test_false_positive_rate() {
            let mut filter = BloomFilter::new(1000, 0.01);

            // Insert items 0-999
            for i in 0..1000 {
                filter.insert(&i);
            }

            // Check items 1000-1999 (not inserted)
            let mut false_positives = 0;
            for i in 1000..2000 {
                if filter.may_contain(&i) {
                    false_positives += 1;
                }
            }

            // False positive rate should be roughly around 1%
            // Allow some variance (up to 5%)
            let fp_rate = false_positives as f64 / 1000.0;
            assert!(fp_rate < 0.05, "False positive rate too high: {}", fp_rate);
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut filter = BloomFilter::new(100, 0.01);
            filter.insert(&"hello");
            filter.insert(&"world");

            filter.clear();

            assert!(filter.is_empty());
            assert_eq!(filter.count(), 0);
            // After clear, previously inserted items should (likely) not be found
            // Note: This isn't guaranteed, but with a cleared filter it's very unlikely
        }
    }

    mod merge {
        use super::*;

        #[test]
        fn test_merge() {
            let mut filter1 = BloomFilter::with_size(1024, 7);
            let mut filter2 = BloomFilter::with_size(1024, 7);

            filter1.insert(&"apple");
            filter2.insert(&"banana");

            assert!(filter1.merge(&filter2));

            assert!(filter1.may_contain(&"apple"));
            assert!(filter1.may_contain(&"banana"));
        }

        #[test]
        fn test_merge_incompatible() {
            let mut filter1 = BloomFilter::with_size(1024, 7);
            let filter2 = BloomFilter::with_size(2048, 7);

            assert!(!filter1.merge(&filter2));
        }
    }

    mod estimated_fp_rate {
        use super::*;

        #[test]
        fn test_estimated_fp_rate() {
            let mut filter = BloomFilter::new(100, 0.01);

            // Empty filter should have ~0 FP rate
            assert!(filter.estimated_fp_rate() < 0.001);

            // Add items
            for i in 0..100 {
                filter.insert(&i);
            }

            // FP rate should be reasonable
            assert!(filter.estimated_fp_rate() < 0.5);
        }
    }
}
