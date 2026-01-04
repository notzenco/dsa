//! Linear data structures.
//!
//! This module contains implementations of linear data structures:
//!
//! - [`DynamicArray`] - A resizable array (Vec-like)
//! - `SinglyLinkedList` - A singly linked list
//! - `DoublyLinkedList` - A doubly linked list
//! - `Stack` - LIFO stack
//! - `Queue` - FIFO queue
//! - `Deque` - Double-ended queue
//! - `MonotonicQueue` - Monotonic queue for sliding window problems

pub mod deque;
pub mod doubly_linked_list;
pub mod dynamic_array;
pub mod monotonic_queue;
pub mod queue;
pub mod singly_linked_list;
pub mod stack;

pub use deque::Deque;
pub use doubly_linked_list::DoublyLinkedList;
pub use dynamic_array::DynamicArray;
pub use monotonic_queue::{
    sliding_window_maximum, sliding_window_minimum, MonotonicOrder, MonotonicQueue,
};
pub use queue::Queue;
pub use singly_linked_list::SinglyLinkedList;
pub use stack::Stack;
