/**
 * QUEUE (FIFO - First In First Out) AND DEQUE (Double-Ended Queue)
 *
 * Queue: Elements are added at the rear and removed from the front.
 * Deque: Elements can be added/removed from both ends.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Queue (FIFO):                                                            ║
 * ║                                                                           ║
 * ║    FRONT                                            REAR                  ║
 * ║      │                                                │                   ║
 * ║      ▼                                                ▼                   ║
 * ║    ┌─────┬─────┬─────┬─────┬─────┐                                       ║
 * ║    │ 10  │ 20  │ 30  │ 40  │ 50  │                                       ║
 * ║    └─────┴─────┴─────┴─────┴─────┘                                       ║
 * ║      ↑                         ↑                                          ║
 * ║   dequeue()               enqueue()                                       ║
 * ║                                                                           ║
 * ║  Deque (Double-Ended Queue):                                              ║
 * ║                                                                           ║
 * ║    FRONT                                            REAR                  ║
 * ║      │                                                │                   ║
 * ║      ▼                                                ▼                   ║
 * ║    ┌─────┬─────┬─────┬─────┬─────┐                                       ║
 * ║    │ 10  │ 20  │ 30  │ 40  │ 50  │                                       ║
 * ║    └─────┴─────┴─────┴─────┴─────┘                                       ║
 * ║     ↑ ↓                     ↑ ↓                                           ║
 * ║  push/pop                push/pop                                         ║
 * ║   front                    back                                           ║
 * ║                                                                           ║
 * ║  Circular Queue (Ring Buffer):                                            ║
 * ║                                                                           ║
 * ║         ┌───┬───┬───┬───┬───┬───┬───┬───┐                                ║
 * ║         │ 30│ 40│ 50│   │   │   │ 10│ 20│                                ║
 * ║         └───┴───┴───┴───┴───┴───┴───┴───┘                                ║
 * ║           ↑               ↑           ↑                                   ║
 * ║          rear           (empty)     front                                 ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Enqueue            │ O(1)*        │ O(n)       │ O(1)    │
 * │ Dequeue            │ O(1)         │ O(1)       │ O(1)    │
 * │ Front/Rear         │ O(1)         │ O(1)       │ O(1)    │
 * │ isEmpty            │ O(1)         │ O(1)       │ O(1)    │
 * │ Push Front (Deque) │ O(1)*        │ O(n)       │ O(1)    │
 * │ Push Back (Deque)  │ O(1)*        │ O(n)       │ O(1)    │
 * │ Pop Front (Deque)  │ O(1)         │ O(1)       │ O(1)    │
 * │ Pop Back (Deque)   │ O(1)         │ O(1)       │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * Amortized O(1) for dynamic implementations
 *
 * LEETCODE PROBLEMS:
 * - #232 Implement Queue using Stacks
 * - #225 Implement Stack using Queues
 * - #622 Design Circular Queue
 * - #641 Design Circular Deque
 * - #239 Sliding Window Maximum (Monotonic Deque)
 *
 * USE CASES:
 * - Task scheduling (CPU, printer)
 * - BFS traversal
 * - Message queues
 * - Sliding window problems (deque)
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdbool.h>

#define QUEUE_INITIAL_CAPACITY 16

/* ============== Queue (Circular Buffer Implementation) ============== */

/**
 * Queue structure using circular buffer
 */
typedef struct {
    int *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} Queue;

/**
 * Create a new empty queue.
 * @return Pointer to new queue, or NULL on allocation failure
 */
Queue *queue_create(void);

/**
 * Create a new queue with specified capacity.
 * @param capacity Initial capacity
 * @return Pointer to new queue, or NULL on allocation failure
 */
Queue *queue_create_with_capacity(size_t capacity);

/**
 * Free all memory associated with the queue.
 * @param queue Pointer to the queue
 */
void queue_destroy(Queue *queue);

/**
 * Add an element to the rear of the queue.
 * @param queue Pointer to the queue
 * @param value Value to add
 * @return true if successful, false on allocation failure
 */
bool queue_enqueue(Queue *queue, int value);

/**
 * Remove and return the front element.
 * @param queue Pointer to the queue
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if queue is empty
 */
bool queue_dequeue(Queue *queue, int *out_value);

/**
 * Get the front element without removing it.
 * @param queue Pointer to the queue
 * @param out_value Pointer to store the value
 * @return true if successful, false if queue is empty
 */
bool queue_front(const Queue *queue, int *out_value);

/**
 * Get the rear element without removing it.
 * @param queue Pointer to the queue
 * @param out_value Pointer to store the value
 * @return true if successful, false if queue is empty
 */
bool queue_rear(const Queue *queue, int *out_value);

/**
 * Get the number of elements in the queue.
 * @param queue Pointer to the queue
 * @return Number of elements
 */
size_t queue_size(const Queue *queue);

/**
 * Check if the queue is empty.
 * @param queue Pointer to the queue
 * @return true if empty, false otherwise
 */
bool queue_is_empty(const Queue *queue);

/**
 * Clear all elements from the queue.
 * @param queue Pointer to the queue
 */
void queue_clear(Queue *queue);

/**
 * Print the queue contents (for debugging).
 * @param queue Pointer to the queue
 */
void queue_print(const Queue *queue);

/* ============== Circular Queue (Fixed Size) ============== */

/**
 * Fixed-size circular queue structure
 */
typedef struct {
    int *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} CircularQueue;

/**
 * Create a new circular queue with fixed capacity.
 * @param capacity Maximum capacity
 * @return Pointer to new circular queue, or NULL on allocation failure
 */
CircularQueue *circular_queue_create(size_t capacity);

/**
 * Free all memory associated with the circular queue.
 * @param queue Pointer to the circular queue
 */
void circular_queue_destroy(CircularQueue *queue);

/**
 * Add an element to the circular queue.
 * @param queue Pointer to the circular queue
 * @param value Value to add
 * @return true if successful, false if queue is full
 */
bool circular_queue_enqueue(CircularQueue *queue, int value);

/**
 * Remove and return the front element.
 * @param queue Pointer to the circular queue
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if queue is empty
 */
bool circular_queue_dequeue(CircularQueue *queue, int *out_value);

/**
 * Get the front element without removing it.
 * @param queue Pointer to the circular queue
 * @param out_value Pointer to store the value
 * @return true if successful, false if queue is empty
 */
bool circular_queue_front(const CircularQueue *queue, int *out_value);

/**
 * Get the rear element without removing it.
 * @param queue Pointer to the circular queue
 * @param out_value Pointer to store the value
 * @return true if successful, false if queue is empty
 */
bool circular_queue_rear(const CircularQueue *queue, int *out_value);

/**
 * Check if the circular queue is empty.
 * @param queue Pointer to the circular queue
 * @return true if empty, false otherwise
 */
bool circular_queue_is_empty(const CircularQueue *queue);

/**
 * Check if the circular queue is full.
 * @param queue Pointer to the circular queue
 * @return true if full, false otherwise
 */
bool circular_queue_is_full(const CircularQueue *queue);

/**
 * Get the number of elements in the circular queue.
 * @param queue Pointer to the circular queue
 * @return Number of elements
 */
size_t circular_queue_size(const CircularQueue *queue);

/* ============== Deque (Double-Ended Queue) ============== */

/**
 * Deque structure using circular buffer
 */
typedef struct {
    int *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} Deque;

/**
 * Create a new empty deque.
 * @return Pointer to new deque, or NULL on allocation failure
 */
Deque *deque_create(void);

/**
 * Create a new deque with specified capacity.
 * @param capacity Initial capacity
 * @return Pointer to new deque, or NULL on allocation failure
 */
Deque *deque_create_with_capacity(size_t capacity);

/**
 * Free all memory associated with the deque.
 * @param deque Pointer to the deque
 */
void deque_destroy(Deque *deque);

/**
 * Add an element to the front of the deque.
 * @param deque Pointer to the deque
 * @param value Value to add
 * @return true if successful, false on allocation failure
 */
bool deque_push_front(Deque *deque, int value);

/**
 * Add an element to the back of the deque.
 * @param deque Pointer to the deque
 * @param value Value to add
 * @return true if successful, false on allocation failure
 */
bool deque_push_back(Deque *deque, int value);

/**
 * Remove and return the front element.
 * @param deque Pointer to the deque
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if deque is empty
 */
bool deque_pop_front(Deque *deque, int *out_value);

/**
 * Remove and return the back element.
 * @param deque Pointer to the deque
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if deque is empty
 */
bool deque_pop_back(Deque *deque, int *out_value);

/**
 * Get the front element without removing it.
 * @param deque Pointer to the deque
 * @param out_value Pointer to store the value
 * @return true if successful, false if deque is empty
 */
bool deque_front(const Deque *deque, int *out_value);

/**
 * Get the back element without removing it.
 * @param deque Pointer to the deque
 * @param out_value Pointer to store the value
 * @return true if successful, false if deque is empty
 */
bool deque_back(const Deque *deque, int *out_value);

/**
 * Get the number of elements in the deque.
 * @param deque Pointer to the deque
 * @return Number of elements
 */
size_t deque_size(const Deque *deque);

/**
 * Check if the deque is empty.
 * @param deque Pointer to the deque
 * @return true if empty, false otherwise
 */
bool deque_is_empty(const Deque *deque);

/**
 * Clear all elements from the deque.
 * @param deque Pointer to the deque
 */
void deque_clear(Deque *deque);

/**
 * Get element at index (0 = front).
 * @param deque Pointer to the deque
 * @param index Index of element
 * @param out_value Pointer to store the value
 * @return true if successful, false if index out of bounds
 */
bool deque_get(const Deque *deque, size_t index, int *out_value);

/**
 * Print the deque contents (for debugging).
 * @param deque Pointer to the deque
 */
void deque_print(const Deque *deque);

#endif /* QUEUE_H */
