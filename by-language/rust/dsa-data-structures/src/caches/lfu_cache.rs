//! # LFU Cache (Least Frequently Used Cache)
//!
//! An LFU Cache evicts the least frequently used item when capacity is exceeded.
//! When there are ties in frequency, the least recently used item is evicted.
//!
//! ## Visual Representation
//!
//! ```text
//!     Frequency Buckets (doubly linked lists per frequency)
//!
//!     freq=1: [d] ←→ [e]     (least frequent)
//!     freq=2: [b] ←→ [c]
//!     freq=3: [a]            (most frequent)
//!
//!     HashMap: key → (value, freq, node_ptr)
//!
//!     On access: move item to next frequency bucket
//!     On eviction: remove from lowest frequency bucket (LRU within bucket)
//! ```
//!
//! ## Complexity Analysis
//!
//! | Operation | Time Complexity | Space Complexity |
//! |-----------|-----------------|------------------|
//! | get(key)  | O(1)            | O(1)             |
//! | put(k,v)  | O(1)            | O(1)             |
//! | Overall   | -               | O(capacity)      |
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::caches::LFUCache;
//!
//! let mut cache = LFUCache::new(2);
//!
//! cache.put(1, 1);
//! cache.put(2, 2);
//! assert_eq!(cache.get(&1), Some(&1));  // freq(1) = 2
//!
//! cache.put(3, 3);  // Evicts key 2 (freq=1, LRU)
//! assert_eq!(cache.get(&2), None);
//! assert_eq!(cache.get(&3), Some(&3));
//! ```

use alloc::collections::BTreeMap;
use alloc::vec::Vec;

/// An LFU (Least Frequently Used) Cache.
///
/// Provides O(1) get and put operations with automatic eviction of the
/// least frequently used item when capacity is exceeded.
pub struct LFUCache<K, V>
where
    K: Ord + Clone,
{
    capacity: usize,
    min_freq: usize,
    // key -> (value, frequency)
    cache: BTreeMap<K, (V, usize)>,
    // frequency -> list of keys (in LRU order, oldest first)
    freq_to_keys: BTreeMap<usize, Vec<K>>,
    // key -> position in frequency list
    key_to_pos: BTreeMap<K, usize>,
}

impl<K, V> LFUCache<K, V>
where
    K: Ord + Clone,
{
    /// Creates a new LFU cache with the specified capacity.
    ///
    /// # Panics
    ///
    /// Panics if capacity is 0.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LFUCache;
    ///
    /// let cache: LFUCache<i32, i32> = LFUCache::new(100);
    /// assert!(cache.is_empty());
    /// ```
    pub fn new(capacity: usize) -> Self {
        assert!(capacity > 0, "LFU cache capacity must be greater than 0");

        LFUCache {
            capacity,
            min_freq: 0,
            cache: BTreeMap::new(),
            freq_to_keys: BTreeMap::new(),
            key_to_pos: BTreeMap::new(),
        }
    }

    /// Returns the capacity of the cache.
    pub fn capacity(&self) -> usize {
        self.capacity
    }

    /// Returns the number of items in the cache.
    pub fn len(&self) -> usize {
        self.cache.len()
    }

    /// Returns `true` if the cache is empty.
    pub fn is_empty(&self) -> bool {
        self.cache.is_empty()
    }

    /// Gets a reference to the value for the given key.
    ///
    /// This increments the access frequency of the key.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LFUCache;
    ///
    /// let mut cache = LFUCache::new(10);
    /// cache.put(1, 100);
    /// assert_eq!(cache.get(&1), Some(&100));
    /// assert_eq!(cache.get(&2), None);
    /// ```
    pub fn get(&mut self, key: &K) -> Option<&V> {
        if !self.cache.contains_key(key) {
            return None;
        }

        self.increment_frequency(key);
        self.cache.get(key).map(|(v, _)| v)
    }

    /// Gets a mutable reference to the value for the given key.
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> {
        if !self.cache.contains_key(key) {
            return None;
        }

        self.increment_frequency(key);
        self.cache.get_mut(key).map(|(v, _)| v)
    }

    /// Inserts a key-value pair into the cache.
    ///
    /// If the key exists, updates the value and increments frequency.
    /// If at capacity, evicts the least frequently used item.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LFUCache;
    ///
    /// let mut cache = LFUCache::new(2);
    /// cache.put(1, 1);
    /// cache.put(2, 2);
    /// cache.get(&1);      // freq(1) = 2
    /// cache.put(3, 3);    // Evicts 2
    /// assert_eq!(cache.get(&2), None);
    /// ```
    pub fn put(&mut self, key: K, value: V) {
        if self.capacity == 0 {
            return;
        }

        // Update existing key
        if self.cache.contains_key(&key) {
            self.cache.get_mut(&key).unwrap().0 = value;
            self.increment_frequency(&key);
            return;
        }

        // Evict if at capacity
        if self.cache.len() >= self.capacity {
            self.evict();
        }

        // Insert new key with frequency 1
        self.cache.insert(key.clone(), (value, 1));
        self.freq_to_keys.entry(1).or_insert_with(Vec::new).push(key.clone());
        let pos = self.freq_to_keys.get(&1).unwrap().len() - 1;
        self.key_to_pos.insert(key, pos);
        self.min_freq = 1;
    }

    /// Returns `true` if the cache contains the given key.
    pub fn contains(&self, key: &K) -> bool {
        self.cache.contains_key(key)
    }

    /// Removes a key from the cache.
    pub fn remove(&mut self, key: &K) -> Option<V> {
        if let Some((value, freq)) = self.cache.remove(key) {
            self.remove_from_freq_list(key, freq);
            self.key_to_pos.remove(key);
            Some(value)
        } else {
            None
        }
    }

    /// Clears the cache.
    pub fn clear(&mut self) {
        self.cache.clear();
        self.freq_to_keys.clear();
        self.key_to_pos.clear();
        self.min_freq = 0;
    }

    /// Returns the frequency of access for a key.
    pub fn frequency(&self, key: &K) -> Option<usize> {
        self.cache.get(key).map(|(_, freq)| *freq)
    }

    // Internal helpers

    fn increment_frequency(&mut self, key: &K) {
        let (_, freq) = self.cache.get_mut(key).unwrap();
        let old_freq = *freq;
        *freq += 1;
        let new_freq = *freq;

        // Remove from old frequency list
        self.remove_from_freq_list(key, old_freq);

        // Add to new frequency list
        self.freq_to_keys.entry(new_freq).or_insert_with(Vec::new).push(key.clone());
        let pos = self.freq_to_keys.get(&new_freq).unwrap().len() - 1;
        self.key_to_pos.insert(key.clone(), pos);

        // Update min_freq if needed
        if old_freq == self.min_freq && self.freq_to_keys.get(&old_freq).map_or(true, |v| v.is_empty()) {
            self.min_freq = new_freq;
        }
    }

    fn remove_from_freq_list(&mut self, key: &K, freq: usize) {
        if let Some(keys) = self.freq_to_keys.get_mut(&freq) {
            if let Some(&pos) = self.key_to_pos.get(key) {
                if pos < keys.len() && &keys[pos] == key {
                    keys.remove(pos);
                    // Update positions for keys after this one
                    for i in pos..keys.len() {
                        self.key_to_pos.insert(keys[i].clone(), i);
                    }
                }
            }
        }
    }

    fn evict(&mut self) {
        if let Some(keys) = self.freq_to_keys.get_mut(&self.min_freq) {
            if !keys.is_empty() {
                let evict_key = keys.remove(0);
                // Update positions
                for i in 0..keys.len() {
                    self.key_to_pos.insert(keys[i].clone(), i);
                }
                self.cache.remove(&evict_key);
                self.key_to_pos.remove(&evict_key);
            }
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
            let cache: LFUCache<i32, i32> = LFUCache::new(10);
            assert_eq!(cache.capacity(), 10);
            assert!(cache.is_empty());
        }

        #[test]
        #[should_panic(expected = "capacity must be greater than 0")]
        fn test_zero_capacity() {
            let _: LFUCache<i32, i32> = LFUCache::new(0);
        }
    }

    mod put_and_get {
        use super::*;

        #[test]
        fn test_put_and_get() {
            let mut cache = LFUCache::new(10);
            cache.put(1, 100);
            assert_eq!(cache.get(&1), Some(&100));
        }

        #[test]
        fn test_update() {
            let mut cache = LFUCache::new(10);
            cache.put(1, 100);
            cache.put(1, 200);
            assert_eq!(cache.get(&1), Some(&200));
            assert_eq!(cache.len(), 1);
        }

        #[test]
        fn test_get_nonexistent() {
            let mut cache: LFUCache<i32, i32> = LFUCache::new(10);
            assert_eq!(cache.get(&1), None);
        }
    }

    mod eviction {
        use super::*;

        #[test]
        fn test_lfu_eviction() {
            let mut cache = LFUCache::new(2);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.get(&1);  // freq(1) = 2, freq(2) = 1
            cache.put(3, 3);  // Evicts 2

            assert_eq!(cache.get(&2), None);
            assert_eq!(cache.get(&1), Some(&1));
            assert_eq!(cache.get(&3), Some(&3));
        }

        #[test]
        fn test_lru_tiebreaker() {
            let mut cache = LFUCache::new(2);
            cache.put(1, 1);
            cache.put(2, 2);
            // Both have freq=1, but 1 was added first (LRU)
            cache.put(3, 3);  // Evicts 1

            assert_eq!(cache.get(&1), None);
            assert_eq!(cache.get(&2), Some(&2));
            assert_eq!(cache.get(&3), Some(&3));
        }

        #[test]
        fn test_frequency_tracking() {
            let mut cache = LFUCache::new(3);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.put(3, 3);

            cache.get(&1);  // freq(1) = 2
            cache.get(&1);  // freq(1) = 3
            cache.get(&2);  // freq(2) = 2

            assert_eq!(cache.frequency(&1), Some(3));
            assert_eq!(cache.frequency(&2), Some(2));
            assert_eq!(cache.frequency(&3), Some(1));

            cache.put(4, 4);  // Evicts 3 (lowest freq)
            assert_eq!(cache.get(&3), None);
        }
    }

    mod remove_and_clear {
        use super::*;

        #[test]
        fn test_remove() {
            let mut cache = LFUCache::new(10);
            cache.put(1, 100);
            assert_eq!(cache.remove(&1), Some(100));
            assert_eq!(cache.get(&1), None);
        }

        #[test]
        fn test_clear() {
            let mut cache = LFUCache::new(10);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.clear();
            assert!(cache.is_empty());
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_capacity_one() {
            let mut cache = LFUCache::new(1);
            cache.put(1, 1);
            cache.put(2, 2);
            assert_eq!(cache.get(&1), None);
            assert_eq!(cache.get(&2), Some(&2));
        }

        #[test]
        fn test_multiple_evictions() {
            let mut cache = LFUCache::new(2);

            for i in 0..10 {
                cache.put(i, i * 10);
            }

            // Only last 2 should remain
            assert_eq!(cache.len(), 2);
        }
    }
}
