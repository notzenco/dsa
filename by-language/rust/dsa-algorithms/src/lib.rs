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

pub mod dynamic_programming;
pub mod graph;
pub mod searching;
pub mod sorting;
pub mod string;

pub use dsa_core::{DsaError, Result};
