//! # Hash Table
//!
//! A hash table implementation using open addressing with linear probing.
//!
//! ## Complexity Analysis
//!
//! | Operation | Average | Worst Case |
//! |-----------|---------|------------|
//! | Insert    | O(1)    | O(n)       |
//! | Get       | O(1)    | O(n)       |
//! | Remove    | O(1)    | O(n)       |
//! | Space     | O(n)    | O(n)       |
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::hashing::HashTable;
//!
//! let mut table = HashTable::new();
//! table.insert("key1", 100);
//! table.insert("key2", 200);
//!
//! assert_eq!(table.get(&"key1"), Some(&100));
//! assert_eq!(table.get(&"key2"), Some(&200));
//! ```

use alloc::vec;
use alloc::vec::Vec;
use core::hash::{Hash, Hasher};

const INITIAL_CAPACITY: usize = 16;
const LOAD_FACTOR_THRESHOLD: f64 = 0.75;

/// Entry state in the hash table.
#[derive(Clone)]
enum Entry<K, V> {
    Empty,
    Deleted,
    Occupied(K, V),
}

impl<K, V> Entry<K, V> {
    fn is_occupied(&self) -> bool {
        matches!(self, Entry::Occupied(_, _))
    }
}

/// A hash table using open addressing with linear probing.
///
/// # Type Parameters
///
/// * `K` - Key type, must implement `Hash` and `Eq`
/// * `V` - Value type
pub struct HashTable<K, V> {
    entries: Vec<Entry<K, V>>,
    len: usize,
    capacity: usize,
}

impl<K: Hash + Eq + Clone, V: Clone> HashTable<K, V> {
    /// Creates a new empty hash table.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::HashTable;
    ///
    /// let table: HashTable<String, i32> = HashTable::new();
    /// assert!(table.is_empty());
    /// ```
    pub fn new() -> Self {
        Self::with_capacity(INITIAL_CAPACITY)
    }

    /// Creates a hash table with the specified capacity.
    pub fn with_capacity(capacity: usize) -> Self {
        let capacity = capacity.max(1);
        HashTable {
            entries: vec![Entry::Empty; capacity],
            len: 0,
            capacity,
        }
    }

    /// Returns the number of elements.
    pub fn len(&self) -> usize {
        self.len
    }

    /// Returns `true` if empty.
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// Returns the current capacity.
    pub fn capacity(&self) -> usize {
        self.capacity
    }

    /// Simple hash function using FNV-1a.
    fn hash(&self, key: &K) -> usize {
        let mut hasher = FnvHasher::new();
        key.hash(&mut hasher);
        hasher.finish() as usize % self.capacity
    }

    /// Finds the index for a key.
    fn find_index(&self, key: &K) -> Option<usize> {
        let start = self.hash(key);
        let mut idx = start;

        loop {
            match &self.entries[idx] {
                Entry::Occupied(k, _) if k == key => return Some(idx),
                Entry::Empty => return None,
                _ => {
                    idx = (idx + 1) % self.capacity;
                    if idx == start {
                        return None;
                    }
                }
            }
        }
    }

    /// Finds an index to insert at.
    fn find_insert_index(&self, key: &K) -> usize {
        let start = self.hash(key);
        let mut idx = start;
        let mut first_deleted: Option<usize> = None;

        loop {
            match &self.entries[idx] {
                Entry::Occupied(k, _) if k == key => return idx,
                Entry::Deleted if first_deleted.is_none() => {
                    first_deleted = Some(idx);
                    idx = (idx + 1) % self.capacity;
                }
                Entry::Empty => return first_deleted.unwrap_or(idx),
                _ => {
                    idx = (idx + 1) % self.capacity;
                }
            }

            if idx == start {
                return first_deleted.unwrap_or(idx);
            }
        }
    }

    /// Resizes the table when load factor is exceeded.
    fn resize(&mut self) {
        let new_capacity = self.capacity * 2;
        let mut new_entries = vec![Entry::Empty; new_capacity];

        for entry in self.entries.drain(..) {
            if let Entry::Occupied(k, v) = entry {
                let mut hasher = FnvHasher::new();
                k.hash(&mut hasher);
                let mut idx = hasher.finish() as usize % new_capacity;

                while new_entries[idx].is_occupied() {
                    idx = (idx + 1) % new_capacity;
                }

                new_entries[idx] = Entry::Occupied(k, v);
            }
        }

        self.entries = new_entries;
        self.capacity = new_capacity;
    }

    /// Inserts a key-value pair.
    ///
    /// Returns the old value if the key existed.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::HashTable;
    ///
    /// let mut table = HashTable::new();
    /// assert_eq!(table.insert("key", 100), None);
    /// assert_eq!(table.insert("key", 200), Some(100));
    /// ```
    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        // Check load factor
        if (self.len + 1) as f64 / self.capacity as f64 > LOAD_FACTOR_THRESHOLD {
            self.resize();
        }

        let idx = self.find_insert_index(&key);

        match &self.entries[idx] {
            Entry::Occupied(k, _) if *k == key => {
                let old = core::mem::replace(&mut self.entries[idx], Entry::Occupied(key, value));
                if let Entry::Occupied(_, v) = old {
                    Some(v)
                } else {
                    None
                }
            }
            _ => {
                self.entries[idx] = Entry::Occupied(key, value);
                self.len += 1;
                None
            }
        }
    }

    /// Gets a reference to the value for a key.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::HashTable;
    ///
    /// let mut table = HashTable::new();
    /// table.insert("key", 100);
    /// assert_eq!(table.get(&"key"), Some(&100));
    /// assert_eq!(table.get(&"missing"), None);
    /// ```
    pub fn get(&self, key: &K) -> Option<&V> {
        self.find_index(key).and_then(|idx| {
            if let Entry::Occupied(_, v) = &self.entries[idx] {
                Some(v)
            } else {
                None
            }
        })
    }

    /// Gets a mutable reference to the value.
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> {
        self.find_index(key).and_then(|idx| {
            if let Entry::Occupied(_, v) = &mut self.entries[idx] {
                Some(v)
            } else {
                None
            }
        })
    }

    /// Returns `true` if the key exists.
    pub fn contains(&self, key: &K) -> bool {
        self.find_index(key).is_some()
    }

    /// Removes a key and returns its value.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::hashing::HashTable;
    ///
    /// let mut table = HashTable::new();
    /// table.insert("key", 100);
    /// assert_eq!(table.remove(&"key"), Some(100));
    /// assert_eq!(table.remove(&"key"), None);
    /// ```
    pub fn remove(&mut self, key: &K) -> Option<V> {
        self.find_index(key).and_then(|idx| {
            let old = core::mem::replace(&mut self.entries[idx], Entry::Deleted);
            if let Entry::Occupied(_, v) = old {
                self.len -= 1;
                Some(v)
            } else {
                None
            }
        })
    }

    /// Clears the hash table.
    pub fn clear(&mut self) {
        self.entries = vec![Entry::Empty; INITIAL_CAPACITY];
        self.len = 0;
        self.capacity = INITIAL_CAPACITY;
    }

    /// Returns an iterator over keys.
    pub fn keys(&self) -> impl Iterator<Item = &K> {
        self.entries.iter().filter_map(|e| {
            if let Entry::Occupied(k, _) = e {
                Some(k)
            } else {
                None
            }
        })
    }

    /// Returns an iterator over values.
    pub fn values(&self) -> impl Iterator<Item = &V> {
        self.entries.iter().filter_map(|e| {
            if let Entry::Occupied(_, v) = e {
                Some(v)
            } else {
                None
            }
        })
    }

    /// Returns an iterator over key-value pairs.
    pub fn iter(&self) -> impl Iterator<Item = (&K, &V)> {
        self.entries.iter().filter_map(|e| {
            if let Entry::Occupied(k, v) = e {
                Some((k, v))
            } else {
                None
            }
        })
    }
}

impl<K: Hash + Eq + Clone, V: Clone> Default for HashTable<K, V> {
    fn default() -> Self {
        Self::new()
    }
}

/// FNV-1a hasher for no_std compatibility.
struct FnvHasher {
    state: u64,
}

impl FnvHasher {
    fn new() -> Self {
        FnvHasher {
            state: 0xcbf29ce484222325, // FNV offset basis
        }
    }
}

impl Hasher for FnvHasher {
    fn finish(&self) -> u64 {
        self.state
    }

    fn write(&mut self, bytes: &[u8]) {
        for &byte in bytes {
            self.state ^= byte as u64;
            self.state = self.state.wrapping_mul(0x100000001b3); // FNV prime
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
            let table: HashTable<i32, i32> = HashTable::new();
            assert!(table.is_empty());
            assert_eq!(table.len(), 0);
        }

        #[test]
        fn test_with_capacity() {
            let table: HashTable<i32, i32> = HashTable::with_capacity(32);
            assert_eq!(table.capacity(), 32);
        }

        #[test]
        fn test_default() {
            let table: HashTable<i32, i32> = HashTable::default();
            assert!(table.is_empty());
        }
    }

    mod insert_and_get {
        use super::*;

        #[test]
        fn test_insert_and_get() {
            let mut table = HashTable::new();
            table.insert("key1", 100);
            assert_eq!(table.get(&"key1"), Some(&100));
        }

        #[test]
        fn test_insert_multiple() {
            let mut table = HashTable::new();
            table.insert("a", 1);
            table.insert("b", 2);
            table.insert("c", 3);

            assert_eq!(table.get(&"a"), Some(&1));
            assert_eq!(table.get(&"b"), Some(&2));
            assert_eq!(table.get(&"c"), Some(&3));
            assert_eq!(table.len(), 3);
        }

        #[test]
        fn test_update() {
            let mut table = HashTable::new();
            assert_eq!(table.insert("key", 100), None);
            assert_eq!(table.insert("key", 200), Some(100));
            assert_eq!(table.get(&"key"), Some(&200));
            assert_eq!(table.len(), 1);
        }

        #[test]
        fn test_get_nonexistent() {
            let table: HashTable<&str, i32> = HashTable::new();
            assert_eq!(table.get(&"missing"), None);
        }

        #[test]
        fn test_get_mut() {
            let mut table = HashTable::new();
            table.insert("key", 100);

            if let Some(v) = table.get_mut(&"key") {
                *v = 200;
            }

            assert_eq!(table.get(&"key"), Some(&200));
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove() {
            let mut table = HashTable::new();
            table.insert("key", 100);
            assert_eq!(table.remove(&"key"), Some(100));
            assert_eq!(table.get(&"key"), None);
            assert_eq!(table.len(), 0);
        }

        #[test]
        fn test_remove_nonexistent() {
            let mut table: HashTable<&str, i32> = HashTable::new();
            assert_eq!(table.remove(&"missing"), None);
        }

        #[test]
        fn test_remove_and_reinsert() {
            let mut table = HashTable::new();
            table.insert("key", 100);
            table.remove(&"key");
            table.insert("key", 200);
            assert_eq!(table.get(&"key"), Some(&200));
        }
    }

    mod contains {
        use super::*;

        #[test]
        fn test_contains() {
            let mut table = HashTable::new();
            table.insert("key", 100);
            assert!(table.contains(&"key"));
            assert!(!table.contains(&"missing"));
        }
    }

    mod resize {
        use super::*;

        #[test]
        fn test_resize() {
            let mut table = HashTable::with_capacity(4);

            for i in 0..20 {
                table.insert(i, i * 10);
            }

            assert!(table.capacity() > 4);

            for i in 0..20 {
                assert_eq!(table.get(&i), Some(&(i * 10)));
            }
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut table = HashTable::new();
            table.insert("a", 1);
            table.insert("b", 2);
            table.clear();

            assert!(table.is_empty());
            assert_eq!(table.get(&"a"), None);
        }
    }

    mod iterators {
        use super::*;

        #[test]
        fn test_keys() {
            let mut table = HashTable::new();
            table.insert(1, 10);
            table.insert(2, 20);

            let mut keys: Vec<_> = table.keys().cloned().collect();
            keys.sort();
            assert_eq!(keys, vec![1, 2]);
        }

        #[test]
        fn test_values() {
            let mut table = HashTable::new();
            table.insert(1, 10);
            table.insert(2, 20);

            let mut values: Vec<_> = table.values().cloned().collect();
            values.sort();
            assert_eq!(values, vec![10, 20]);
        }

        #[test]
        fn test_iter() {
            let mut table = HashTable::new();
            table.insert(1, 10);
            table.insert(2, 20);

            let items: Vec<_> = table.iter().collect();
            assert_eq!(items.len(), 2);
        }
    }

    mod collision {
        use super::*;

        #[test]
        fn test_collision_handling() {
            // Insert many items to force collisions
            let mut table = HashTable::with_capacity(4);

            for i in 0..10 {
                table.insert(i, i * 100);
            }

            for i in 0..10 {
                assert_eq!(table.get(&i), Some(&(i * 100)));
            }
        }
    }
}
