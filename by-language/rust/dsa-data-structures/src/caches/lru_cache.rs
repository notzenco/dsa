//! # LRU Cache (Least Recently Used Cache)
//!
//! An LRU Cache is a data structure that stores a fixed number of items and
//! evicts the least recently used item when the capacity is exceeded.
//!
//! ## Visual Representation
//!
//! ```text
//!     HashMap                    Doubly Linked List (most recent → least recent)
//!    ┌─────────┐                ┌─────┐   ┌─────┐   ┌─────┐   ┌─────┐
//!    │ "a" → ●─┼───────────────►│  a  │◄─►│  b  │◄─►│  c  │◄─►│  d  │
//!    ├─────────┤                └─────┘   └─────┘   └─────┘   └─────┘
//!    │ "b" → ●─┼──────────────────────►     HEAD                TAIL
//!    ├─────────┤                            (MRU)               (LRU)
//!    │ "c" → ●─┼──────────────────────────────────►
//!    ├─────────┤
//!    │ "d" → ●─┼──────────────────────────────────────────────►
//!    └─────────┘
//!
//!    Access Pattern:
//!    1. get("c") → Move "c" to head
//!    2. put("e") → Add "e" to head, evict "d" (tail)
//! ```
//!
//! ## Complexity Analysis
//!
//! | Operation | Time Complexity | Space Complexity |
//! |-----------|-----------------|------------------|
//! | get(key)  | O(1)            | O(1)             |
//! | put(k,v)  | O(1)            | O(1)             |
//! | peek(key) | O(1)            | O(1)             |
//! | remove(k) | O(1)            | O(1)             |
//! | Overall   | -               | O(capacity)      |
//!
//! ## LeetCode Problems
//!
//! - [146. LRU Cache](https://leetcode.com/problems/lru-cache/) - Design LRU Cache
//! - [460. LFU Cache](https://leetcode.com/problems/lfu-cache/) - Related problem
//!
//! ## Use Cases
//!
//! - **Page caching**: Web browsers cache pages, evicting least recently viewed
//! - **Database caching**: Cache query results, evict stale data
//! - **Memory management**: Operating systems use LRU for page replacement
//! - **CDN caching**: Content delivery networks cache popular content
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::caches::LRUCache;
//!
//! // Create cache with capacity 3
//! let mut cache = LRUCache::new(3);
//!
//! // Add items
//! cache.put("a", 1);
//! cache.put("b", 2);
//! cache.put("c", 3);
//!
//! // Access item (moves to front)
//! assert_eq!(cache.get(&"a"), Some(&1));
//!
//! // Add new item (evicts least recently used: "b")
//! cache.put("d", 4);
//!
//! // "b" was evicted
//! assert_eq!(cache.get(&"b"), None);
//! ```

use alloc::boxed::Box;
use alloc::collections::BTreeMap;
use alloc::vec::Vec;
use core::ptr::NonNull;

/// Node in the doubly linked list.
struct Node<K, V> {
    key: K,
    value: V,
    prev: Option<NonNull<Node<K, V>>>,
    next: Option<NonNull<Node<K, V>>>,
}

impl<K, V> Node<K, V> {
    fn new(key: K, value: V) -> Self {
        Node {
            key,
            value,
            prev: None,
            next: None,
        }
    }
}

/// An LRU (Least Recently Used) Cache.
///
/// Provides O(1) get and put operations with automatic eviction of the
/// least recently used item when capacity is exceeded.
///
/// # Type Parameters
///
/// * `K` - The key type, must implement `Ord` and `Clone`
/// * `V` - The value type
///
/// # Example
///
/// ```rust
/// use dsa_data_structures::caches::LRUCache;
///
/// let mut cache = LRUCache::new(2);
/// cache.put("one", 1);
/// cache.put("two", 2);
///
/// assert_eq!(cache.get(&"one"), Some(&1));
///
/// // Adding third item evicts "two" (least recently used)
/// cache.put("three", 3);
/// assert_eq!(cache.get(&"two"), None);
/// ```
pub struct LRUCache<K, V>
where
    K: Ord + Clone,
{
    capacity: usize,
    map: BTreeMap<K, NonNull<Node<K, V>>>,
    head: Option<NonNull<Node<K, V>>>,
    tail: Option<NonNull<Node<K, V>>>,
}

impl<K, V> LRUCache<K, V>
where
    K: Ord + Clone,
{
    /// Creates a new LRU cache with the specified capacity.
    ///
    /// # Arguments
    ///
    /// * `capacity` - Maximum number of items the cache can hold
    ///
    /// # Panics
    ///
    /// Panics if capacity is 0.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let cache: LRUCache<String, i32> = LRUCache::new(100);
    /// assert!(cache.is_empty());
    /// assert_eq!(cache.capacity(), 100);
    /// ```
    pub fn new(capacity: usize) -> Self {
        assert!(capacity > 0, "LRU cache capacity must be greater than 0");

        LRUCache {
            capacity,
            map: BTreeMap::new(),
            head: None,
            tail: None,
        }
    }

    /// Returns the capacity of the cache.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let cache: LRUCache<i32, i32> = LRUCache::new(10);
    /// assert_eq!(cache.capacity(), 10);
    /// ```
    pub fn capacity(&self) -> usize {
        self.capacity
    }

    /// Returns the number of items currently in the cache.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// assert_eq!(cache.len(), 0);
    ///
    /// cache.put("a", 1);
    /// assert_eq!(cache.len(), 1);
    /// ```
    pub fn len(&self) -> usize {
        self.map.len()
    }

    /// Returns `true` if the cache is empty.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// assert!(cache.is_empty());
    ///
    /// cache.put("a", 1);
    /// assert!(!cache.is_empty());
    /// ```
    pub fn is_empty(&self) -> bool {
        self.map.is_empty()
    }

    /// Returns `true` if the cache is at full capacity.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(2);
    /// cache.put("a", 1);
    /// assert!(!cache.is_full());
    ///
    /// cache.put("b", 2);
    /// assert!(cache.is_full());
    /// ```
    pub fn is_full(&self) -> bool {
        self.map.len() >= self.capacity
    }

    /// Inserts a key-value pair into the cache.
    ///
    /// If the key already exists, updates the value and moves it to the front.
    /// If the cache is at capacity, evicts the least recently used item.
    ///
    /// Returns the evicted value if an eviction occurred due to capacity.
    ///
    /// # Arguments
    ///
    /// * `key` - The key to insert
    /// * `value` - The value to associate with the key
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(2);
    /// cache.put("a", 1);
    /// cache.put("b", 2);
    ///
    /// // This will evict "a"
    /// let evicted = cache.put("c", 3);
    /// assert_eq!(evicted, Some(("a", 1)));
    /// ```
    pub fn put(&mut self, key: K, value: V) -> Option<(K, V)> {
        // If key exists, update value and move to front
        if let Some(&node_ptr) = self.map.get(&key) {
            unsafe {
                (*node_ptr.as_ptr()).value = value;
            }
            self.move_to_front(node_ptr);
            return None;
        }

        // Create new node
        let node = Box::new(Node::new(key.clone(), value));
        let node_ptr = NonNull::new(Box::into_raw(node)).unwrap();

        // Add to map
        self.map.insert(key, node_ptr);

        // Add to front of list
        self.push_front(node_ptr);

        // Evict if over capacity
        if self.map.len() > self.capacity {
            return self.evict_lru();
        }

        None
    }

    /// Gets a reference to the value for the given key.
    ///
    /// This marks the key as recently used, moving it to the front.
    ///
    /// # Arguments
    ///
    /// * `key` - The key to look up
    ///
    /// # Returns
    ///
    /// `Some(&V)` if the key exists, `None` otherwise.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    ///
    /// assert_eq!(cache.get(&"a"), Some(&1));
    /// assert_eq!(cache.get(&"b"), None);
    /// ```
    pub fn get(&mut self, key: &K) -> Option<&V> {
        if let Some(&node_ptr) = self.map.get(key) {
            self.move_to_front(node_ptr);
            unsafe { Some(&(*node_ptr.as_ptr()).value) }
        } else {
            None
        }
    }

    /// Gets a mutable reference to the value for the given key.
    ///
    /// This marks the key as recently used, moving it to the front.
    ///
    /// # Arguments
    ///
    /// * `key` - The key to look up
    ///
    /// # Returns
    ///
    /// `Some(&mut V)` if the key exists, `None` otherwise.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    ///
    /// if let Some(value) = cache.get_mut(&"a") {
    ///     *value = 100;
    /// }
    ///
    /// assert_eq!(cache.get(&"a"), Some(&100));
    /// ```
    pub fn get_mut(&mut self, key: &K) -> Option<&mut V> {
        if let Some(&node_ptr) = self.map.get(key) {
            self.move_to_front(node_ptr);
            unsafe { Some(&mut (*node_ptr.as_ptr()).value) }
        } else {
            None
        }
    }

    /// Peeks at a value without marking it as recently used.
    ///
    /// # Arguments
    ///
    /// * `key` - The key to look up
    ///
    /// # Returns
    ///
    /// `Some(&V)` if the key exists, `None` otherwise.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    ///
    /// // Peek doesn't update LRU order
    /// assert_eq!(cache.peek(&"a"), Some(&1));
    /// ```
    pub fn peek(&self, key: &K) -> Option<&V> {
        self.map
            .get(key)
            .map(|&node_ptr| unsafe { &(*node_ptr.as_ptr()).value })
    }

    /// Returns `true` if the cache contains the given key.
    ///
    /// This does not affect the LRU order.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    ///
    /// assert!(cache.contains(&"a"));
    /// assert!(!cache.contains(&"b"));
    /// ```
    pub fn contains(&self, key: &K) -> bool {
        self.map.contains_key(key)
    }

    /// Removes a key from the cache, returning its value if it existed.
    ///
    /// # Arguments
    ///
    /// * `key` - The key to remove
    ///
    /// # Returns
    ///
    /// `Some(V)` if the key existed, `None` otherwise.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    ///
    /// assert_eq!(cache.remove(&"a"), Some(1));
    /// assert_eq!(cache.remove(&"a"), None);
    /// ```
    pub fn remove(&mut self, key: &K) -> Option<V> {
        if let Some(node_ptr) = self.map.remove(key) {
            self.unlink(node_ptr);
            let node = unsafe { Box::from_raw(node_ptr.as_ptr()) };
            Some(node.value)
        } else {
            None
        }
    }

    /// Clears the cache, removing all items.
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    /// cache.put("b", 2);
    ///
    /// cache.clear();
    /// assert!(cache.is_empty());
    /// ```
    pub fn clear(&mut self) {
        // Free all nodes
        let mut current = self.head;
        while let Some(node_ptr) = current {
            unsafe {
                current = (*node_ptr.as_ptr()).next;
                let _ = Box::from_raw(node_ptr.as_ptr());
            }
        }

        self.map.clear();
        self.head = None;
        self.tail = None;
    }

    /// Returns the keys in LRU order (most recent first).
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    /// cache.put("b", 2);
    /// cache.put("c", 3);
    /// cache.get(&"a"); // Make "a" most recent
    ///
    /// let keys = cache.keys();
    /// assert_eq!(keys[0], "a"); // Most recent
    /// assert_eq!(keys[2], "b"); // Least recent
    /// ```
    pub fn keys(&self) -> Vec<K> {
        let mut keys = Vec::new();
        let mut current = self.head;
        while let Some(node_ptr) = current {
            unsafe {
                keys.push((*node_ptr.as_ptr()).key.clone());
                current = (*node_ptr.as_ptr()).next;
            }
        }
        keys
    }

    /// Returns an iterator over the cache in LRU order (most recent first).
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::caches::LRUCache;
    ///
    /// let mut cache = LRUCache::new(10);
    /// cache.put("a", 1);
    /// cache.put("b", 2);
    ///
    /// let items: Vec<_> = cache.iter().collect();
    /// assert_eq!(items.len(), 2);
    /// ```
    pub fn iter(&self) -> LRUIterator<'_, K, V> {
        LRUIterator {
            current: self.head,
            _marker: core::marker::PhantomData,
        }
    }

    // Internal helper methods

    /// Pushes a node to the front of the list.
    fn push_front(&mut self, node_ptr: NonNull<Node<K, V>>) {
        unsafe {
            (*node_ptr.as_ptr()).prev = None;
            (*node_ptr.as_ptr()).next = self.head;

            if let Some(head) = self.head {
                (*head.as_ptr()).prev = Some(node_ptr);
            }

            self.head = Some(node_ptr);

            if self.tail.is_none() {
                self.tail = Some(node_ptr);
            }
        }
    }

    /// Unlinks a node from the list.
    fn unlink(&mut self, node_ptr: NonNull<Node<K, V>>) {
        unsafe {
            let prev = (*node_ptr.as_ptr()).prev;
            let next = (*node_ptr.as_ptr()).next;

            if let Some(prev) = prev {
                (*prev.as_ptr()).next = next;
            } else {
                self.head = next;
            }

            if let Some(next) = next {
                (*next.as_ptr()).prev = prev;
            } else {
                self.tail = prev;
            }
        }
    }

    /// Moves a node to the front of the list.
    fn move_to_front(&mut self, node_ptr: NonNull<Node<K, V>>) {
        // Skip if already at front
        if Some(node_ptr) == self.head {
            return;
        }

        self.unlink(node_ptr);
        self.push_front(node_ptr);
    }

    /// Evicts the least recently used item.
    fn evict_lru(&mut self) -> Option<(K, V)> {
        if let Some(tail) = self.tail {
            unsafe {
                let key = (*tail.as_ptr()).key.clone();
                self.map.remove(&key);
                self.unlink(tail);
                let node = Box::from_raw(tail.as_ptr());
                Some((node.key, node.value))
            }
        } else {
            None
        }
    }
}

impl<K, V> Drop for LRUCache<K, V>
where
    K: Ord + Clone,
{
    fn drop(&mut self) {
        self.clear();
    }
}

/// Iterator over LRU cache entries in order from most to least recently used.
pub struct LRUIterator<'a, K, V> {
    current: Option<NonNull<Node<K, V>>>,
    _marker: core::marker::PhantomData<&'a (K, V)>,
}

impl<'a, K, V> Iterator for LRUIterator<'a, K, V> {
    type Item = (&'a K, &'a V);

    fn next(&mut self) -> Option<Self::Item> {
        self.current.map(|node_ptr| unsafe {
            let node = &*node_ptr.as_ptr();
            self.current = node.next;
            (&node.key, &node.value)
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let cache: LRUCache<i32, i32> = LRUCache::new(10);
            assert_eq!(cache.capacity(), 10);
            assert_eq!(cache.len(), 0);
            assert!(cache.is_empty());
        }

        #[test]
        #[should_panic(expected = "capacity must be greater than 0")]
        fn test_zero_capacity() {
            let _: LRUCache<i32, i32> = LRUCache::new(0);
        }

        #[test]
        fn test_len_and_empty() {
            let mut cache = LRUCache::new(10);
            assert!(cache.is_empty());
            assert_eq!(cache.len(), 0);

            cache.put("a", 1);
            assert!(!cache.is_empty());
            assert_eq!(cache.len(), 1);

            cache.put("b", 2);
            assert_eq!(cache.len(), 2);
        }

        #[test]
        fn test_is_full() {
            let mut cache = LRUCache::new(2);
            assert!(!cache.is_full());

            cache.put("a", 1);
            assert!(!cache.is_full());

            cache.put("b", 2);
            assert!(cache.is_full());
        }
    }

    mod put_and_get {
        use super::*;

        #[test]
        fn test_put_single() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            assert_eq!(cache.get(&"a"), Some(&1));
        }

        #[test]
        fn test_put_multiple() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3);

            assert_eq!(cache.get(&"a"), Some(&1));
            assert_eq!(cache.get(&"b"), Some(&2));
            assert_eq!(cache.get(&"c"), Some(&3));
        }

        #[test]
        fn test_put_update() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            assert_eq!(cache.get(&"a"), Some(&1));

            cache.put("a", 100);
            assert_eq!(cache.get(&"a"), Some(&100));
            assert_eq!(cache.len(), 1);
        }

        #[test]
        fn test_get_nonexistent() {
            let mut cache: LRUCache<&str, i32> = LRUCache::new(10);
            assert_eq!(cache.get(&"a"), None);
        }

        #[test]
        fn test_get_mut() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);

            if let Some(value) = cache.get_mut(&"a") {
                *value = 100;
            }

            assert_eq!(cache.get(&"a"), Some(&100));
        }
    }

    mod eviction {
        use super::*;

        #[test]
        fn test_eviction_on_capacity() {
            let mut cache = LRUCache::new(2);
            cache.put("a", 1);
            cache.put("b", 2);

            // This should evict "a"
            let evicted = cache.put("c", 3);
            assert_eq!(evicted, Some(("a", 1)));

            assert_eq!(cache.get(&"a"), None);
            assert_eq!(cache.get(&"b"), Some(&2));
            assert_eq!(cache.get(&"c"), Some(&3));
        }

        #[test]
        fn test_lru_ordering() {
            let mut cache = LRUCache::new(2);
            cache.put("a", 1);
            cache.put("b", 2);

            // Access "a" to make it most recently used
            cache.get(&"a");

            // This should evict "b" now (least recently used)
            let evicted = cache.put("c", 3);
            assert_eq!(evicted, Some(("b", 2)));

            assert_eq!(cache.get(&"a"), Some(&1));
            assert_eq!(cache.get(&"b"), None);
            assert_eq!(cache.get(&"c"), Some(&3));
        }

        #[test]
        fn test_update_moves_to_front() {
            let mut cache = LRUCache::new(2);
            cache.put("a", 1);
            cache.put("b", 2);

            // Update "a" to make it most recently used
            cache.put("a", 100);

            // This should evict "b"
            let evicted = cache.put("c", 3);
            assert_eq!(evicted, Some(("b", 2)));

            assert_eq!(cache.get(&"a"), Some(&100));
        }

        #[test]
        fn test_multiple_evictions() {
            let mut cache = LRUCache::new(2);

            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3); // Evicts "a"
            cache.put("d", 4); // Evicts "b"

            assert_eq!(cache.get(&"a"), None);
            assert_eq!(cache.get(&"b"), None);
            assert_eq!(cache.get(&"c"), Some(&3));
            assert_eq!(cache.get(&"d"), Some(&4));
        }
    }

    mod peek_and_contains {
        use super::*;

        #[test]
        fn test_peek() {
            let mut cache = LRUCache::new(2);
            cache.put("a", 1);
            cache.put("b", 2);

            // Peek at "a" (doesn't change order)
            assert_eq!(cache.peek(&"a"), Some(&1));

            // "a" should still be LRU (evicted first)
            let evicted = cache.put("c", 3);
            assert_eq!(evicted, Some(("a", 1)));
        }

        #[test]
        fn test_peek_nonexistent() {
            let cache: LRUCache<&str, i32> = LRUCache::new(10);
            assert_eq!(cache.peek(&"a"), None);
        }

        #[test]
        fn test_contains() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);

            assert!(cache.contains(&"a"));
            assert!(!cache.contains(&"b"));
        }

        #[test]
        fn test_contains_doesnt_change_order() {
            let mut cache = LRUCache::new(2);
            cache.put("a", 1);
            cache.put("b", 2);

            // contains doesn't change order
            assert!(cache.contains(&"a"));

            // "a" should still be LRU
            let evicted = cache.put("c", 3);
            assert_eq!(evicted, Some(("a", 1)));
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);

            assert_eq!(cache.remove(&"a"), Some(1));
            assert_eq!(cache.len(), 1);
            assert_eq!(cache.get(&"a"), None);
        }

        #[test]
        fn test_remove_nonexistent() {
            let mut cache: LRUCache<&str, i32> = LRUCache::new(10);
            assert_eq!(cache.remove(&"a"), None);
        }

        #[test]
        fn test_remove_head() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3); // c is head

            assert_eq!(cache.remove(&"c"), Some(3));
            assert_eq!(cache.keys(), vec!["b", "a"]);
        }

        #[test]
        fn test_remove_tail() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3); // a is tail

            assert_eq!(cache.remove(&"a"), Some(1));
            assert_eq!(cache.keys(), vec!["c", "b"]);
        }

        #[test]
        fn test_remove_middle() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3);

            assert_eq!(cache.remove(&"b"), Some(2));
            assert_eq!(cache.keys(), vec!["c", "a"]);
        }
    }

    mod clear {
        use super::*;

        #[test]
        fn test_clear() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);

            cache.clear();

            assert!(cache.is_empty());
            assert_eq!(cache.get(&"a"), None);
        }

        #[test]
        fn test_clear_empty() {
            let mut cache: LRUCache<&str, i32> = LRUCache::new(10);
            cache.clear();
            assert!(cache.is_empty());
        }

        #[test]
        fn test_reuse_after_clear() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.clear();

            cache.put("b", 2);
            assert_eq!(cache.get(&"b"), Some(&2));
            assert_eq!(cache.len(), 1);
        }
    }

    mod keys_and_iter {
        use super::*;

        #[test]
        fn test_keys() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3);

            // Most recent first
            assert_eq!(cache.keys(), vec!["c", "b", "a"]);
        }

        #[test]
        fn test_keys_after_access() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3);

            cache.get(&"a");

            // "a" is now most recent
            assert_eq!(cache.keys(), vec!["a", "c", "b"]);
        }

        #[test]
        fn test_iter() {
            let mut cache = LRUCache::new(10);
            cache.put("a", 1);
            cache.put("b", 2);
            cache.put("c", 3);

            let items: Vec<_> = cache.iter().collect();
            assert_eq!(items, vec![(&"c", &3), (&"b", &2), (&"a", &1)]);
        }

        #[test]
        fn test_iter_empty() {
            let cache: LRUCache<&str, i32> = LRUCache::new(10);
            let items: Vec<_> = cache.iter().collect();
            assert!(items.is_empty());
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_capacity_one() {
            let mut cache = LRUCache::new(1);
            cache.put("a", 1);
            assert_eq!(cache.get(&"a"), Some(&1));

            let evicted = cache.put("b", 2);
            assert_eq!(evicted, Some(("a", 1)));
            assert_eq!(cache.get(&"a"), None);
            assert_eq!(cache.get(&"b"), Some(&2));
        }

        #[test]
        fn test_same_key_multiple_updates() {
            let mut cache = LRUCache::new(10);

            for i in 0..100 {
                cache.put("key", i);
            }

            assert_eq!(cache.len(), 1);
            assert_eq!(cache.get(&"key"), Some(&99));
        }

        #[test]
        fn test_stress_test() {
            let mut cache = LRUCache::new(100);

            for i in 0..1000 {
                cache.put(i, i * 2);
            }

            // Only last 100 should remain
            assert_eq!(cache.len(), 100);

            for i in 900..1000 {
                assert_eq!(cache.get(&i), Some(&(i * 2)));
            }

            for i in 0..900 {
                assert_eq!(cache.get(&i), None);
            }
        }
    }
}
