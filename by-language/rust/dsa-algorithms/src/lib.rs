//! # DSA Algorithms
//!
//! A comprehensive collection of algorithm implementations in Rust.
//!
//! This crate provides educational implementations of common algorithms
//! with detailed documentation, complexity analysis, and comprehensive tests.
//!
//! ## Modules
//!
//! - `sorting` - Sorting algorithms
//! - `searching` - Searching algorithms
//! - `graph` - Graph algorithms
//! - `dynamic_programming` - Dynamic programming algorithms
//! - `string` - String algorithms

#![cfg_attr(not(feature = "std"), no_std)]

extern crate alloc;

pub mod sorting;
// pub mod searching;            // TODO: Phase 8
// pub mod graph;                // TODO: Phase 8
// pub mod dynamic_programming;  // TODO: Phase 8
// pub mod string;               // TODO: Phase 8

pub use dsa_core::{DsaError, Result};
