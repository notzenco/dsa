//! Error types for DSA operations.
//!
//! This module provides a unified error type for all data structure
//! and algorithm operations.

use core::fmt;

/// The main error type for DSA operations.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum DsaError {
    /// Index is out of bounds.
    IndexOutOfBounds {
        /// The invalid index that was accessed.
        index: usize,
        /// The size of the container.
        size: usize,
    },

    /// Operation attempted on an empty container.
    EmptyContainer,

    /// Capacity has been exceeded.
    CapacityExceeded {
        /// The maximum allowed capacity.
        max: usize,
        /// The requested capacity.
        requested: usize,
    },

    /// Key was not found in the container.
    KeyNotFound,

    /// Duplicate key was inserted.
    DuplicateKey,

    /// Invalid argument was provided.
    InvalidArgument {
        /// Description of the invalid argument.
        message: &'static str,
    },

    /// Graph vertex not found.
    VertexNotFound {
        /// The vertex that was not found.
        vertex: usize,
    },

    /// Graph contains a cycle when it shouldn't.
    CycleDetected,

    /// Negative cycle detected in graph.
    NegativeCycle,
}

impl fmt::Display for DsaError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::IndexOutOfBounds { index, size } => {
                write!(f, "index {index} out of bounds for size {size}")
            }
            Self::EmptyContainer => f.write_str("operation on empty container"),
            Self::CapacityExceeded { max, requested } => write!(
                f,
                "capacity exceeded: maximum is {max}, requested {requested}"
            ),
            Self::KeyNotFound => f.write_str("key not found"),
            Self::DuplicateKey => f.write_str("duplicate key"),
            Self::InvalidArgument { message } => write!(f, "invalid argument: {message}"),
            Self::VertexNotFound { vertex } => write!(f, "vertex {vertex} not found"),
            Self::CycleDetected => f.write_str("graph contains a cycle"),
            Self::NegativeCycle => f.write_str("negative cycle detected"),
        }
    }
}

#[cfg(feature = "std")]
impl std::error::Error for DsaError {}

/// A specialized Result type for DSA operations.
pub type Result<T> = core::result::Result<T, DsaError>;
