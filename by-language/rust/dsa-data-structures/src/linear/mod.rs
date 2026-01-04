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

pub mod dynamic_array;
pub mod singly_linked_list;
pub mod doubly_linked_list;
pub mod stack;
pub mod queue;
// pub mod deque;
// pub mod monotonic_queue;

pub use dynamic_array::DynamicArray;
pub use singly_linked_list::SinglyLinkedList;
pub use doubly_linked_list::DoublyLinkedList;
pub use stack::Stack;
pub use queue::Queue;
