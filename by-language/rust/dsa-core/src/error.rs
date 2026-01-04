//! Error types for DSA operations.
//!
//! This module provides a unified error type for all data structure
//! and algorithm operations.

use thiserror::Error;

/// The main error type for DSA operations.
#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum DsaError {
    /// Index is out of bounds.
    #[error("index {index} out of bounds for size {size}")]
    IndexOutOfBounds {
        /// The invalid index that was accessed.
        index: usize,
        /// The size of the container.
        size: usize,
    },

    /// Operation attempted on an empty container.
    #[error("operation on empty container")]
    EmptyContainer,

    /// Capacity has been exceeded.
    #[error("capacity exceeded: maximum is {max}, requested {requested}")]
    CapacityExceeded {
        /// The maximum allowed capacity.
        max: usize,
        /// The requested capacity.
        requested: usize,
    },

    /// Key was not found in the container.
    #[error("key not found")]
    KeyNotFound,

    /// Duplicate key was inserted.
    #[error("duplicate key")]
    DuplicateKey,

    /// Invalid argument was provided.
    #[error("invalid argument: {message}")]
    InvalidArgument {
        /// Description of the invalid argument.
        message: &'static str,
    },

    /// Graph vertex not found.
    #[error("vertex {vertex} not found")]
    VertexNotFound {
        /// The vertex that was not found.
        vertex: usize,
    },

    /// Graph contains a cycle when it shouldn't.
    #[error("graph contains a cycle")]
    CycleDetected,

    /// Negative cycle detected in graph.
    #[error("negative cycle detected")]
    NegativeCycle,
}

/// A specialized Result type for DSA operations.
pub type Result<T> = core::result::Result<T, DsaError>;
