//! Cache data structures.
//!
//! This module contains implementations of cache data structures:
//!
//! - [`LRUCache`] - Least Recently Used cache
//! - [`LFUCache`] - Least Frequently Used cache

pub mod lfu_cache;
pub mod lru_cache;

pub use lfu_cache::LFUCache;
pub use lru_cache::LRUCache;
