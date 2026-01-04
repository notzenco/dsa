//! Cache data structures.
//!
//! This module contains implementations of cache data structures:
//!
//! - [`LRUCache`] - Least Recently Used cache
//! - [`LFUCache`] - Least Frequently Used cache

pub mod lru_cache;
pub mod lfu_cache;

pub use lru_cache::LRUCache;
pub use lfu_cache::LFUCache;
