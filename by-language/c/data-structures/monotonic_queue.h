/**
 * MONOTONIC QUEUE (Monotonically Decreasing Deque)
 *
 * A specialized double-ended queue that maintains elements in monotonically
 * decreasing order. Used primarily for sliding window maximum problems.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Monotonic Queue (Decreasing):                                            ║
 * ║                                                                           ║
 * ║    FRONT (max)                                      REAR                  ║
 * ║      │                                                │                   ║
 * ║      ▼                                                ▼                   ║
 * ║    ┌─────┬─────┬─────┬─────┐                                             ║
 * ║    │ 50  │ 40  │ 30  │ 10  │   ← Values in decreasing order              ║
 * ║    │ [2] │ [4] │ [5] │ [6] │   ← Corresponding indices                   ║
 * ║    └─────┴─────┴─────┴─────┘                                             ║
 * ║      ↑                   ↑                                                ║
 * ║   get_max()           push()                                              ║
 * ║                    (removes smaller)                                      ║
 * ║                                                                           ║
 * ║  Sliding Window Maximum Example:                                          ║
 * ║                                                                           ║
 * ║    Array: [1, 3, -1, -3, 5, 3, 6, 7], k=3                                ║
 * ║                                                                           ║
 * ║    Window [1,3,-1]    → Queue: [3,-1]     → Max: 3                       ║
 * ║    Window [3,-1,-3]   → Queue: [3,-1,-3]  → Max: 3                       ║
 * ║    Window [-1,-3,5]   → Queue: [5]        → Max: 5                       ║
 * ║    Window [-3,5,3]    → Queue: [5,3]      → Max: 5                       ║
 * ║    Window [5,3,6]     → Queue: [6]        → Max: 6                       ║
 * ║    Window [3,6,7]     → Queue: [7]        → Max: 7                       ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Push               │ O(1)*        │ O(n)       │ O(1)    │
 * │ Pop                │ O(1)         │ O(1)       │ O(1)    │
 * │ Get Max            │ O(1)         │ O(1)       │ O(1)    │
 * │ Sliding Window Max │ O(n)         │ O(n)       │ O(k)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * Amortized O(1) - each element is added and removed at most once
 *
 * LEETCODE PROBLEMS:
 * - #239 Sliding Window Maximum
 * - #1438 Longest Continuous Subarray With Absolute Diff <= Limit
 * - #862 Shortest Subarray with Sum at Least K
 * - #1499 Max Value of Equation
 *
 * USE CASES:
 * - Sliding window maximum/minimum
 * - Finding next greater element efficiently
 * - Stock span problems
 * - Histogram problems
 */

#ifndef MONOTONIC_QUEUE_H
#define MONOTONIC_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

#define MQ_INITIAL_CAPACITY 16

/**
 * Entry in the monotonic queue storing index and value
 */
typedef struct {
    int index;
    int value;
} MQEntry;

/**
 * Monotonic Queue structure using circular buffer
 */
typedef struct {
    MQEntry *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} MonotonicQueue;

/* ============== Creation and Destruction ============== */

/**
 * Create a new empty monotonic queue.
 * @return Pointer to new queue, or NULL on allocation failure
 */
MonotonicQueue *mq_create(void);

/**
 * Create a new monotonic queue with specified capacity.
 * @param capacity Initial capacity
 * @return Pointer to new queue, or NULL on allocation failure
 */
MonotonicQueue *mq_create_with_capacity(size_t capacity);

/**
 * Destroy queue and free all memory.
 * @param mq Queue to destroy
 */
void mq_destroy(MonotonicQueue *mq);

/**
 * Remove all elements from queue.
 * @param mq Queue to clear
 */
void mq_clear(MonotonicQueue *mq);

/* ============== Core Operations ============== */

/**
 * Push a new element onto the queue.
 * Removes all elements from the rear that are smaller than the new value
 * to maintain the monotonically decreasing property.
 *
 * @param mq Queue to push to
 * @param index Index of the element (for window tracking)
 * @param value Value to push
 * @return true on success, false on allocation failure
 */
bool mq_push(MonotonicQueue *mq, int index, int value);

/**
 * Pop element from front if its index matches.
 * Used to remove elements that fall outside the sliding window.
 *
 * @param mq Queue to pop from
 * @param index Index to match for removal
 */
void mq_pop(MonotonicQueue *mq, int index);

/**
 * Get the maximum value (front of queue).
 * @param mq Queue to query
 * @param value Output parameter for the maximum value
 * @return true if queue is not empty, false otherwise
 */
bool mq_get_max(const MonotonicQueue *mq, int *value);

/**
 * Get the index of the maximum value.
 * @param mq Queue to query
 * @param index Output parameter for the index
 * @return true if queue is not empty, false otherwise
 */
bool mq_get_max_index(const MonotonicQueue *mq, int *index);

/* ============== Properties ============== */

/**
 * Check if queue is empty.
 * @param mq Queue to check
 * @return true if empty, false otherwise
 */
bool mq_is_empty(const MonotonicQueue *mq);

/**
 * Get number of elements in queue.
 * @param mq Queue to query
 * @return Number of elements
 */
size_t mq_size(const MonotonicQueue *mq);

/* ============== Utility Functions ============== */

/**
 * Compute sliding window maximum for an array.
 *
 * @param nums Input array
 * @param n Length of input array
 * @param k Window size
 * @param result_size Output parameter for result array size
 * @return Array of maximum values for each window, or NULL on failure.
 *         Caller is responsible for freeing the returned array.
 */
int *mq_sliding_window_max(const int *nums, size_t n, size_t k, size_t *result_size);

/**
 * Compute sliding window minimum for an array.
 *
 * @param nums Input array
 * @param n Length of input array
 * @param k Window size
 * @param result_size Output parameter for result array size
 * @return Array of minimum values for each window, or NULL on failure.
 *         Caller is responsible for freeing the returned array.
 */
int *mq_sliding_window_min(const int *nums, size_t n, size_t k, size_t *result_size);

#endif /* MONOTONIC_QUEUE_H */
