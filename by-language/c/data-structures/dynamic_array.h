/**
 * DYNAMIC ARRAY (Vector)
 *
 * A resizable array that grows automatically when elements are added.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Dynamic Array with size=5, capacity=8:                                   ║
 * ║                                                                           ║
 * ║  Index:    0     1     2     3     4     5     6     7                    ║
 * ║         ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐                 ║
 * ║  Data:  │  10 │  20 │  30 │  40 │  50 │  -  │  -  │  -  │                 ║
 * ║         └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘                 ║
 * ║         ╰───────────────────────────────╯ ╰───────────────╯               ║
 * ║                    Used (size)            Unused capacity                 ║
 * ║                                                                           ║
 * ║  Growth Strategy: Double capacity when full                               ║
 * ║  Shrink Strategy: Halve capacity when size < capacity/4                   ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Access by index    │ O(1)         │ O(1)       │ O(1)    │
 * │ Push back          │ O(1)*        │ O(n)       │ O(1)    │
 * │ Pop back           │ O(1)         │ O(1)       │ O(1)    │
 * │ Insert at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Delete at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Search             │ O(n)         │ O(n)       │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * Amortized O(1) due to occasional resizing
 *
 * LEETCODE PROBLEMS:
 * - #27 Remove Element
 * - #26 Remove Duplicates from Sorted Array
 * - #88 Merge Sorted Array
 * - #283 Move Zeroes
 *
 * USE CASES:
 * - When you need random access by index
 * - When the number of elements changes frequently
 * - As the underlying structure for stacks, queues, heaps
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

/* Default initial capacity */
#define DA_INITIAL_CAPACITY 8

/* Growth factor for resizing */
#define DA_GROWTH_FACTOR 2

/* Shrink threshold (shrink when size < capacity / DA_SHRINK_THRESHOLD) */
#define DA_SHRINK_THRESHOLD 4

/**
 * Dynamic Array structure for integers
 */
typedef struct {
    int *data;        /* Pointer to the data array */
    size_t size;      /* Number of elements currently stored */
    size_t capacity;  /* Total capacity of the array */
} DynamicArray;

/**
 * Create a new dynamic array with default capacity.
 * @return Pointer to new DynamicArray, or NULL on allocation failure
 */
DynamicArray *da_create(void);

/**
 * Create a new dynamic array with specified initial capacity.
 * @param capacity Initial capacity
 * @return Pointer to new DynamicArray, or NULL on allocation failure
 */
DynamicArray *da_create_with_capacity(size_t capacity);

/**
 * Free all memory associated with the dynamic array.
 * @param arr Pointer to the DynamicArray
 */
void da_destroy(DynamicArray *arr);

/**
 * Get the number of elements in the array.
 * @param arr Pointer to the DynamicArray
 * @return Number of elements
 */
size_t da_size(const DynamicArray *arr);

/**
 * Get the current capacity of the array.
 * @param arr Pointer to the DynamicArray
 * @return Current capacity
 */
size_t da_capacity(const DynamicArray *arr);

/**
 * Check if the array is empty.
 * @param arr Pointer to the DynamicArray
 * @return true if empty, false otherwise
 */
bool da_is_empty(const DynamicArray *arr);

/**
 * Get element at the specified index.
 * @param arr Pointer to the DynamicArray
 * @param index Index of the element
 * @param out_value Pointer to store the retrieved value
 * @return true if successful, false if index out of bounds
 */
bool da_get(const DynamicArray *arr, size_t index, int *out_value);

/**
 * Set element at the specified index.
 * @param arr Pointer to the DynamicArray
 * @param index Index of the element
 * @param value Value to set
 * @return true if successful, false if index out of bounds
 */
bool da_set(DynamicArray *arr, size_t index, int value);

/**
 * Add an element to the end of the array.
 * @param arr Pointer to the DynamicArray
 * @param value Value to add
 * @return true if successful, false on allocation failure
 */
bool da_push_back(DynamicArray *arr, int value);

/**
 * Remove and return the last element.
 * @param arr Pointer to the DynamicArray
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if array is empty
 */
bool da_pop_back(DynamicArray *arr, int *out_value);

/**
 * Insert an element at the specified index.
 * @param arr Pointer to the DynamicArray
 * @param index Index at which to insert
 * @param value Value to insert
 * @return true if successful, false if index > size or allocation failure
 */
bool da_insert(DynamicArray *arr, size_t index, int value);

/**
 * Remove element at the specified index.
 * @param arr Pointer to the DynamicArray
 * @param index Index of element to remove
 * @param out_value Pointer to store the removed value (can be NULL)
 * @return true if successful, false if index out of bounds
 */
bool da_remove(DynamicArray *arr, size_t index, int *out_value);

/**
 * Find the first occurrence of a value.
 * @param arr Pointer to the DynamicArray
 * @param value Value to find
 * @return Index of the value, or -1 if not found
 */
int da_find(const DynamicArray *arr, int value);

/**
 * Check if the array contains a value.
 * @param arr Pointer to the DynamicArray
 * @param value Value to check
 * @return true if found, false otherwise
 */
bool da_contains(const DynamicArray *arr, int value);

/**
 * Clear all elements from the array.
 * @param arr Pointer to the DynamicArray
 */
void da_clear(DynamicArray *arr);

/**
 * Reserve capacity for at least n elements.
 * @param arr Pointer to the DynamicArray
 * @param capacity Minimum capacity to reserve
 * @return true if successful, false on allocation failure
 */
bool da_reserve(DynamicArray *arr, size_t capacity);

/**
 * Shrink capacity to fit the current size.
 * @param arr Pointer to the DynamicArray
 * @return true if successful, false on allocation failure
 */
bool da_shrink_to_fit(DynamicArray *arr);

/**
 * Reverse the array in place.
 * @param arr Pointer to the DynamicArray
 */
void da_reverse(DynamicArray *arr);

/**
 * Print the array contents (for debugging).
 * @param arr Pointer to the DynamicArray
 */
void da_print(const DynamicArray *arr);

#endif /* DYNAMIC_ARRAY_H */
