//! # DSA Core
//!
//! Core traits and utilities for Data Structures and Algorithms implementations.
//!
//! This crate provides the foundational traits and error types used across
//! the `dsa-data-structures` and `dsa-algorithms` crates.

#![cfg_attr(not(feature = "std"), no_std)]

pub mod error;
pub mod traits;

pub use error::{DsaError, Result};
pub use traits::*;
