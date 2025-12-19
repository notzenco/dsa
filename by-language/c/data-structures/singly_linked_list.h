/**
 * SINGLY LINKED LIST
 *
 * A linear data structure where elements are stored in nodes, each pointing
 * to the next node in the sequence.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Singly Linked List with 4 nodes:                                         ║
 * ║                                                                           ║
 * ║     HEAD                                                      TAIL        ║
 * ║       │                                                         │         ║
 * ║       ▼                                                         ▼         ║
 * ║    ┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐    ┌──────┬───┐          ║
 * ║    │  10  │ ──┼───►│  20  │ ──┼───►│  30  │ ──┼───►│  40  │ ⊘ │          ║
 * ║    └──────┴───┘    └──────┴───┘    └──────┴───┘    └──────┴───┘          ║
 * ║      data  next      data  next      data  next      data  next          ║
 * ║                                                                           ║
 * ║  Insertion at head: O(1)                                                  ║
 * ║  ┌──────┬───┐    ┌──────┬───┐                                            ║
 * ║  │  5   │ ──┼───►│  10  │ ──┼───► ...                                    ║
 * ║  └──────┴───┘    └──────┴───┘                                            ║
 * ║    NEW HEAD        OLD HEAD                                               ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Insert at head     │ O(1)         │ O(1)       │ O(1)    │
 * │ Insert at tail     │ O(1)*        │ O(1)*      │ O(1)    │
 * │ Insert at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Delete at head     │ O(1)         │ O(1)       │ O(1)    │
 * │ Delete at tail     │ O(n)         │ O(n)       │ O(1)    │
 * │ Delete at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Search             │ O(n)         │ O(n)       │ O(1)    │
 * │ Access by index    │ O(n)         │ O(n)       │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * O(1) with tail pointer
 *
 * LEETCODE PROBLEMS:
 * - #206 Reverse Linked List
 * - #21 Merge Two Sorted Lists
 * - #141 Linked List Cycle
 * - #19 Remove Nth Node From End
 * - #234 Palindrome Linked List
 *
 * USE CASES:
 * - When frequent insertions/deletions at beginning are needed
 * - Implementation of stacks, queues
 * - Polynomial arithmetic, sparse matrix representation
 * - Memory-efficient for unknown size collections
 */

#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Node structure for singly linked list
 */
typedef struct SLLNode {
    int data;
    struct SLLNode *next;
} SLLNode;

/**
 * Singly Linked List structure
 */
typedef struct {
    SLLNode *head;
    SLLNode *tail;
    size_t size;
} SinglyLinkedList;

/**
 * Create a new empty singly linked list.
 * @return Pointer to new list, or NULL on allocation failure
 */
SinglyLinkedList *sll_create(void);

/**
 * Free all memory associated with the list.
 * @param list Pointer to the list
 */
void sll_destroy(SinglyLinkedList *list);

/**
 * Get the number of elements in the list.
 * @param list Pointer to the list
 * @return Number of elements
 */
size_t sll_size(const SinglyLinkedList *list);

/**
 * Check if the list is empty.
 * @param list Pointer to the list
 * @return true if empty, false otherwise
 */
bool sll_is_empty(const SinglyLinkedList *list);

/**
 * Insert a value at the head of the list.
 * @param list Pointer to the list
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool sll_push_front(SinglyLinkedList *list, int value);

/**
 * Insert a value at the tail of the list.
 * @param list Pointer to the list
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool sll_push_back(SinglyLinkedList *list, int value);

/**
 * Insert a value at the specified index.
 * @param list Pointer to the list
 * @param index Index at which to insert (0-based)
 * @param value Value to insert
 * @return true if successful, false if index > size or allocation failure
 */
bool sll_insert(SinglyLinkedList *list, size_t index, int value);

/**
 * Remove and return the value at the head.
 * @param list Pointer to the list
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if list is empty
 */
bool sll_pop_front(SinglyLinkedList *list, int *out_value);

/**
 * Remove and return the value at the tail.
 * @param list Pointer to the list
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if list is empty
 */
bool sll_pop_back(SinglyLinkedList *list, int *out_value);

/**
 * Remove element at the specified index.
 * @param list Pointer to the list
 * @param index Index of element to remove
 * @param out_value Pointer to store the removed value (can be NULL)
 * @return true if successful, false if index out of bounds
 */
bool sll_remove(SinglyLinkedList *list, size_t index, int *out_value);

/**
 * Remove the first occurrence of a value.
 * @param list Pointer to the list
 * @param value Value to remove
 * @return true if found and removed, false otherwise
 */
bool sll_remove_value(SinglyLinkedList *list, int value);

/**
 * Get the value at the head without removing it.
 * @param list Pointer to the list
 * @param out_value Pointer to store the value
 * @return true if successful, false if list is empty
 */
bool sll_peek_front(const SinglyLinkedList *list, int *out_value);

/**
 * Get the value at the tail without removing it.
 * @param list Pointer to the list
 * @param out_value Pointer to store the value
 * @return true if successful, false if list is empty
 */
bool sll_peek_back(const SinglyLinkedList *list, int *out_value);

/**
 * Get the value at the specified index.
 * @param list Pointer to the list
 * @param index Index of the element
 * @param out_value Pointer to store the value
 * @return true if successful, false if index out of bounds
 */
bool sll_get(const SinglyLinkedList *list, size_t index, int *out_value);

/**
 * Set the value at the specified index.
 * @param list Pointer to the list
 * @param index Index of the element
 * @param value New value to set
 * @return true if successful, false if index out of bounds
 */
bool sll_set(SinglyLinkedList *list, size_t index, int value);

/**
 * Find the index of the first occurrence of a value.
 * @param list Pointer to the list
 * @param value Value to find
 * @return Index of the value, or -1 if not found
 */
int sll_find(const SinglyLinkedList *list, int value);

/**
 * Check if the list contains a value.
 * @param list Pointer to the list
 * @param value Value to check
 * @return true if found, false otherwise
 */
bool sll_contains(const SinglyLinkedList *list, int value);

/**
 * Clear all elements from the list.
 * @param list Pointer to the list
 */
void sll_clear(SinglyLinkedList *list);

/**
 * Reverse the list in place.
 * @param list Pointer to the list
 */
void sll_reverse(SinglyLinkedList *list);

/**
 * Get the middle node value (for odd length, exact middle; for even, second middle).
 * @param list Pointer to the list
 * @param out_value Pointer to store the middle value
 * @return true if successful, false if list is empty
 */
bool sll_get_middle(const SinglyLinkedList *list, int *out_value);

/**
 * Check if the list has a cycle (for educational purposes - this list shouldn't).
 * @param list Pointer to the list
 * @return true if cycle detected, false otherwise
 */
bool sll_has_cycle(const SinglyLinkedList *list);

/**
 * Convert list to an array.
 * @param list Pointer to the list
 * @param out_array Pointer to store the allocated array
 * @param out_size Pointer to store the array size
 * @return true if successful, false on allocation failure
 */
bool sll_to_array(const SinglyLinkedList *list, int **out_array, size_t *out_size);

/**
 * Print the list contents (for debugging).
 * @param list Pointer to the list
 */
void sll_print(const SinglyLinkedList *list);

#endif /* SINGLY_LINKED_LIST_H */
