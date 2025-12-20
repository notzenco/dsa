/**
 * BINARY HEAP AND PRIORITY QUEUE
 *
 * A complete binary tree where each node satisfies the heap property:
 * - Min Heap: parent <= children
 * - Max Heap: parent >= children
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Min Heap:                         Max Heap:                              ║
 * ║                                                                           ║
 * ║        ┌───┐                            ┌───┐                             ║
 * ║        │ 1 │                            │ 9 │                             ║
 * ║        └─┬─┘                            └─┬─┘                             ║
 * ║      ┌───┴───┐                        ┌───┴───┐                           ║
 * ║    ┌─┴─┐   ┌─┴─┐                    ┌─┴─┐   ┌─┴─┐                         ║
 * ║    │ 3 │   │ 2 │                    │ 7 │   │ 8 │                         ║
 * ║    └─┬─┘   └─┬─┘                    └─┬─┘   └─┬─┘                         ║
 * ║    ┌─┴─┐   ┌─┴─┐                    ┌─┴─┐   ┌─┴─┐                         ║
 * ║    │ 5 │   │ 4 │                    │ 3 │   │ 5 │                         ║
 * ║    └───┘   └───┘                    └───┘   └───┘                         ║
 * ║                                                                           ║
 * ║  Array representation: [1, 3, 2, 5, 4]                                    ║
 * ║  Parent of i: (i-1)/2                                                     ║
 * ║  Left child: 2*i + 1                                                      ║
 * ║  Right child: 2*i + 2                                                     ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Insert             │ O(log n)     │ O(log n)   │ O(1)    │
 * │ Extract Min/Max    │ O(log n)     │ O(log n)   │ O(1)    │
 * │ Peek Min/Max       │ O(1)         │ O(1)       │ O(1)    │
 * │ Heapify (build)    │ O(n)         │ O(n)       │ O(1)    │
 * │ Decrease/Inc Key   │ O(log n)     │ O(log n)   │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 *
 * LEETCODE PROBLEMS:
 * - #215 Kth Largest Element in an Array
 * - #347 Top K Frequent Elements
 * - #295 Find Median from Data Stream
 * - #23 Merge k Sorted Lists
 * - #703 Kth Largest Element in a Stream
 *
 * USE CASES:
 * - Priority queues (task scheduling)
 * - Dijkstra's algorithm
 * - Heap sort
 * - K-way merging
 * - Running median
 */

#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdbool.h>

#define HEAP_INITIAL_CAPACITY 16

/* ============== Min Heap ============== */

/**
 * Min Heap structure
 */
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MinHeap;

/**
 * Create a new empty min heap.
 * @return Pointer to new heap, or NULL on allocation failure
 */
MinHeap *min_heap_create(void);

/**
 * Create a min heap from an array (heapify).
 * @param arr Array of integers
 * @param n Size of array
 * @return Pointer to new heap, or NULL on allocation failure
 */
MinHeap *min_heap_from_array(const int *arr, size_t n);

/**
 * Free all memory associated with the heap.
 * @param heap Pointer to the heap
 */
void min_heap_destroy(MinHeap *heap);

/**
 * Insert a value into the heap.
 * @param heap Pointer to the heap
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool min_heap_push(MinHeap *heap, int value);

/**
 * Remove and return the minimum value.
 * @param heap Pointer to the heap
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if heap is empty
 */
bool min_heap_pop(MinHeap *heap, int *out_value);

/**
 * Get the minimum value without removing it.
 * @param heap Pointer to the heap
 * @param out_value Pointer to store the value
 * @return true if successful, false if heap is empty
 */
bool min_heap_peek(const MinHeap *heap, int *out_value);

/**
 * Get the number of elements in the heap.
 * @param heap Pointer to the heap
 * @return Number of elements
 */
size_t min_heap_size(const MinHeap *heap);

/**
 * Check if the heap is empty.
 * @param heap Pointer to the heap
 * @return true if empty, false otherwise
 */
bool min_heap_is_empty(const MinHeap *heap);

/**
 * Clear all elements from the heap.
 * @param heap Pointer to the heap
 */
void min_heap_clear(MinHeap *heap);

/* ============== Max Heap ============== */

/**
 * Max Heap structure
 */
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MaxHeap;

/**
 * Create a new empty max heap.
 * @return Pointer to new heap, or NULL on allocation failure
 */
MaxHeap *max_heap_create(void);

/**
 * Create a max heap from an array (heapify).
 * @param arr Array of integers
 * @param n Size of array
 * @return Pointer to new heap, or NULL on allocation failure
 */
MaxHeap *max_heap_from_array(const int *arr, size_t n);

/**
 * Free all memory associated with the heap.
 * @param heap Pointer to the heap
 */
void max_heap_destroy(MaxHeap *heap);

/**
 * Insert a value into the heap.
 * @param heap Pointer to the heap
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool max_heap_push(MaxHeap *heap, int value);

/**
 * Remove and return the maximum value.
 * @param heap Pointer to the heap
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if heap is empty
 */
bool max_heap_pop(MaxHeap *heap, int *out_value);

/**
 * Get the maximum value without removing it.
 * @param heap Pointer to the heap
 * @param out_value Pointer to store the value
 * @return true if successful, false if heap is empty
 */
bool max_heap_peek(const MaxHeap *heap, int *out_value);

/**
 * Get the number of elements in the heap.
 * @param heap Pointer to the heap
 * @return Number of elements
 */
size_t max_heap_size(const MaxHeap *heap);

/**
 * Check if the heap is empty.
 * @param heap Pointer to the heap
 * @return true if empty, false otherwise
 */
bool max_heap_is_empty(const MaxHeap *heap);

/**
 * Clear all elements from the heap.
 * @param heap Pointer to the heap
 */
void max_heap_clear(MaxHeap *heap);

/* ============== Priority Queue (with priority values) ============== */

/**
 * Priority Queue entry
 */
typedef struct {
    int value;
    int priority;
} PQEntry;

/**
 * Priority Queue structure (min priority = highest priority)
 */
typedef struct {
    PQEntry *data;
    size_t size;
    size_t capacity;
} PriorityQueue;

/**
 * Create a new empty priority queue.
 * @return Pointer to new priority queue, or NULL on allocation failure
 */
PriorityQueue *pq_create(void);

/**
 * Free all memory associated with the priority queue.
 * @param pq Pointer to the priority queue
 */
void pq_destroy(PriorityQueue *pq);

/**
 * Insert a value with priority.
 * @param pq Pointer to the priority queue
 * @param value Value to insert
 * @param priority Priority (lower = higher priority)
 * @return true if successful, false on allocation failure
 */
bool pq_push(PriorityQueue *pq, int value, int priority);

/**
 * Remove and return the highest priority element.
 * @param pq Pointer to the priority queue
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if queue is empty
 */
bool pq_pop(PriorityQueue *pq, int *out_value);

/**
 * Get the highest priority element without removing it.
 * @param pq Pointer to the priority queue
 * @param out_value Pointer to store the value
 * @return true if successful, false if queue is empty
 */
bool pq_peek(const PriorityQueue *pq, int *out_value);

/**
 * Get the number of elements.
 * @param pq Pointer to the priority queue
 * @return Number of elements
 */
size_t pq_size(const PriorityQueue *pq);

/**
 * Check if empty.
 * @param pq Pointer to the priority queue
 * @return true if empty, false otherwise
 */
bool pq_is_empty(const PriorityQueue *pq);

/**
 * Clear all elements.
 * @param pq Pointer to the priority queue
 */
void pq_clear(PriorityQueue *pq);

/* ============== Heap Sort ============== */

/**
 * Sort array in ascending order using heap sort.
 * @param arr Array to sort
 * @param n Size of array
 */
void heap_sort_asc(int *arr, size_t n);

/**
 * Sort array in descending order using heap sort.
 * @param arr Array to sort
 * @param n Size of array
 */
void heap_sort_desc(int *arr, size_t n);

#endif /* HEAP_H */
