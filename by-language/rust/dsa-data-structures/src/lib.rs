//! # DSA Data Structures
//!
//! A comprehensive collection of data structure implementations in Rust.
//!
//! This crate provides educational implementations of common data structures
//! with detailed documentation, complexity analysis, and comprehensive tests.
//!
//! ## Modules
//!
//! - `linear` - Linear data structures (arrays, lists, stacks, queues)
//! - `trees` - Tree data structures (BST, AVL, Red-Black, B-Tree, etc.)
//! - `hashing` - Hash-based structures (hash table, bloom filter)
//! - `heaps` - Heap data structures
//! - `graphs` - Graph representations and algorithms
//! - `caches` - Cache implementations (LRU, LFU, TTL)
//! - `advanced` - Advanced data structures (skip list, etc.)

#![cfg_attr(not(feature = "std"), no_std)]

extern crate alloc;

pub mod linear;
pub mod trees;
// pub mod hashing;    // TODO: Phase 4
// pub mod heaps;      // TODO: Phase 4
// pub mod graphs;     // TODO: Phase 5
// pub mod caches;     // TODO: Phase 6
// pub mod advanced;   // TODO: Phase 6

pub use dsa_core::{DsaError, Result};
