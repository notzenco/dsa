/**
 * DOUBLY LINKED LIST
 *
 * A linear data structure where each node contains data and pointers to both
 * the next and previous nodes, allowing bidirectional traversal.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Doubly Linked List with 4 nodes:                                         ║
 * ║                                                                           ║
 * ║       HEAD                                                 TAIL           ║
 * ║         │                                                    │            ║
 * ║         ▼                                                    ▼            ║
 * ║    ┌────┬────┬────┐   ┌────┬────┬────┐   ┌────┬────┬────┐   ┌────┬────┬────┐
 * ║    │ ⊘  │ 10 │  ──┼──►│◄── │ 20 │  ──┼──►│◄── │ 30 │  ──┼──►│◄── │ 40 │ ⊘  │
 * ║    └────┴────┴────┘   └────┴────┴────┘   └────┴────┴────┘   └────┴────┴────┘
 * ║     prev data next     prev data next     prev data next     prev data next
 * ║                                                                           ║
 * ║  Key Advantage: O(1) deletion when node pointer is known                  ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Insert at head     │ O(1)         │ O(1)       │ O(1)    │
 * │ Insert at tail     │ O(1)         │ O(1)       │ O(1)    │
 * │ Insert at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Delete at head     │ O(1)         │ O(1)       │ O(1)    │
 * │ Delete at tail     │ O(1)         │ O(1)       │ O(1)    │
 * │ Delete at index    │ O(n)         │ O(n)       │ O(1)    │
 * │ Delete given node  │ O(1)         │ O(1)       │ O(1)    │
 * │ Search             │ O(n)         │ O(n)       │ O(1)    │
 * │ Access by index    │ O(n)         │ O(n)       │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 *
 * LEETCODE PROBLEMS:
 * - #146 LRU Cache (uses doubly linked list internally)
 * - #460 LFU Cache
 * - #432 All O`one Data Structure
 *
 * USE CASES:
 * - LRU Cache implementation
 * - Browser forward/back navigation
 * - Undo/redo functionality
 * - Music playlist (next/previous)
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Node structure for doubly linked list
 */
typedef struct DLLNode {
    int data;
    struct DLLNode *prev;
    struct DLLNode *next;
} DLLNode;

/**
 * Doubly Linked List structure
 */
typedef struct {
    DLLNode *head;
    DLLNode *tail;
    size_t size;
} DoublyLinkedList;

/**
 * Create a new empty doubly linked list.
 * @return Pointer to new list, or NULL on allocation failure
 */
DoublyLinkedList *dll_create(void);

/**
 * Free all memory associated with the list.
 * @param list Pointer to the list
 */
void dll_destroy(DoublyLinkedList *list);

/**
 * Get the number of elements in the list.
 * @param list Pointer to the list
 * @return Number of elements
 */
size_t dll_size(const DoublyLinkedList *list);

/**
 * Check if the list is empty.
 * @param list Pointer to the list
 * @return true if empty, false otherwise
 */
bool dll_is_empty(const DoublyLinkedList *list);

/**
 * Insert a value at the head of the list.
 * @param list Pointer to the list
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool dll_push_front(DoublyLinkedList *list, int value);

/**
 * Insert a value at the tail of the list.
 * @param list Pointer to the list
 * @param value Value to insert
 * @return true if successful, false on allocation failure
 */
bool dll_push_back(DoublyLinkedList *list, int value);

/**
 * Insert a value at the specified index.
 * @param list Pointer to the list
 * @param index Index at which to insert (0-based)
 * @param value Value to insert
 * @return true if successful, false if index > size or allocation failure
 */
bool dll_insert(DoublyLinkedList *list, size_t index, int value);

/**
 * Remove and return the value at the head.
 * @param list Pointer to the list
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if list is empty
 */
bool dll_pop_front(DoublyLinkedList *list, int *out_value);

/**
 * Remove and return the value at the tail.
 * @param list Pointer to the list
 * @param out_value Pointer to store the removed value
 * @return true if successful, false if list is empty
 */
bool dll_pop_back(DoublyLinkedList *list, int *out_value);

/**
 * Remove element at the specified index.
 * @param list Pointer to the list
 * @param index Index of element to remove
 * @param out_value Pointer to store the removed value (can be NULL)
 * @return true if successful, false if index out of bounds
 */
bool dll_remove(DoublyLinkedList *list, size_t index, int *out_value);

/**
 * Remove a specific node from the list (O(1) operation).
 * @param list Pointer to the list
 * @param node Pointer to the node to remove
 * @param out_value Pointer to store the removed value (can be NULL)
 * @return true if successful, false if node is NULL
 */
bool dll_remove_node(DoublyLinkedList *list, DLLNode *node, int *out_value);

/**
 * Remove the first occurrence of a value.
 * @param list Pointer to the list
 * @param value Value to remove
 * @return true if found and removed, false otherwise
 */
bool dll_remove_value(DoublyLinkedList *list, int value);

/**
 * Get the value at the head without removing it.
 * @param list Pointer to the list
 * @param out_value Pointer to store the value
 * @return true if successful, false if list is empty
 */
bool dll_peek_front(const DoublyLinkedList *list, int *out_value);

/**
 * Get the value at the tail without removing it.
 * @param list Pointer to the list
 * @param out_value Pointer to store the value
 * @return true if successful, false if list is empty
 */
bool dll_peek_back(const DoublyLinkedList *list, int *out_value);

/**
 * Get the value at the specified index.
 * @param list Pointer to the list
 * @param index Index of the element
 * @param out_value Pointer to store the value
 * @return true if successful, false if index out of bounds
 */
bool dll_get(const DoublyLinkedList *list, size_t index, int *out_value);

/**
 * Set the value at the specified index.
 * @param list Pointer to the list
 * @param index Index of the element
 * @param value New value to set
 * @return true if successful, false if index out of bounds
 */
bool dll_set(DoublyLinkedList *list, size_t index, int value);

/**
 * Get the node at the specified index.
 * @param list Pointer to the list
 * @param index Index of the element
 * @return Pointer to the node, or NULL if index out of bounds
 */
DLLNode *dll_get_node(const DoublyLinkedList *list, size_t index);

/**
 * Find the index of the first occurrence of a value.
 * @param list Pointer to the list
 * @param value Value to find
 * @return Index of the value, or -1 if not found
 */
int dll_find(const DoublyLinkedList *list, int value);

/**
 * Check if the list contains a value.
 * @param list Pointer to the list
 * @param value Value to check
 * @return true if found, false otherwise
 */
bool dll_contains(const DoublyLinkedList *list, int value);

/**
 * Clear all elements from the list.
 * @param list Pointer to the list
 */
void dll_clear(DoublyLinkedList *list);

/**
 * Reverse the list in place.
 * @param list Pointer to the list
 */
void dll_reverse(DoublyLinkedList *list);

/**
 * Move a node to the front of the list (useful for LRU cache).
 * @param list Pointer to the list
 * @param node Pointer to the node to move
 */
void dll_move_to_front(DoublyLinkedList *list, DLLNode *node);

/**
 * Move a node to the back of the list.
 * @param list Pointer to the list
 * @param node Pointer to the node to move
 */
void dll_move_to_back(DoublyLinkedList *list, DLLNode *node);

/**
 * Print the list contents forward (for debugging).
 * @param list Pointer to the list
 */
void dll_print_forward(const DoublyLinkedList *list);

/**
 * Print the list contents backward (for debugging).
 * @param list Pointer to the list
 */
void dll_print_backward(const DoublyLinkedList *list);

#endif /* DOUBLY_LINKED_LIST_H */
