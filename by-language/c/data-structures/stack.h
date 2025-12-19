/**
 * STACK (LIFO - Last In First Out)
 *
 * A linear data structure that follows the Last-In-First-Out principle.
 * Elements are added and removed from the same end (top).
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                         VISUAL REPRESENTATION                             ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Stack with 4 elements:                                                   ║
 * ║                                                                           ║
 * ║       ┌─────────┐                                                         ║
 * ║  TOP →│   40    │  ← push() adds here                                     ║
 * ║       ├─────────┤     pop() removes from here                             ║
 * ║       │   30    │                                                         ║
 * ║       ├─────────┤                                                         ║
 * ║       │   20    │                                                         ║
 * ║       ├─────────┤                                                         ║
 * ║       │   10    │                                                         ║
 * ║       └─────────┘                                                         ║
 * ║        BOTTOM                                                             ║
 * ║                                                                           ║
 * ║  Push Operation:              Pop Operation:                              ║
 * ║       ┌───┐                        ┌───┐                                  ║
 * ║  50 → │40 │                   40 ← │40 │                                  ║
 * ║       │30 │                        │30 │                                  ║
 * ║       │...│                        │...│                                  ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * COMPLEXITY:
 * ┌────────────────────┬──────────────┬────────────┬─────────┐
 * │ Operation          │ Average Time │ Worst Time │ Space   │
 * ├────────────────────┼──────────────┼────────────┼─────────┤
 * │ Push               │ O(1)*        │ O(n)       │ O(1)    │
 * │ Pop                │ O(1)         │ O(1)       │ O(1)    │
 * │ Peek/Top           │ O(1)         │ O(1)       │ O(1)    │
 * │ isEmpty            │ O(1)         │ O(1)       │ O(1)    │
 * │ Size               │ O(1)         │ O(1)       │ O(1)    │
 * └────────────────────┴──────────────┴────────────┴─────────┘
 * * Amortized O(1) for dynamic array implementation
 *
 * LEETCODE PROBLEMS:
 * - #20 Valid Parentheses
 * - #155 Min Stack
 * - #232 Implement Queue using Stacks
 * - #150 Evaluate Reverse Polish Notation
 * - #739 Daily Temperatures
 *
 * USE CASES:
 * - Function call stack (recursion)
 * - Undo/Redo operations
 * - Expression evaluation
 * - Parentheses matching
 * - Backtracking algorithms
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

#define STACK_INITIAL_CAPACITY 16

/**
 * Stack structure (array-based implementation)
 */
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Stack;

/**
 * Min Stack structure - supports O(1) getMin operation
 */
typedef struct {
    Stack *main_stack;
    Stack *min_stack;
} MinStack;

/**
 * Max Stack structure - supports O(1) getMax operation
 */
typedef struct {
    Stack *main_stack;
    Stack *max_stack;
} MaxStack;

/* ============== Basic Stack Operations ============== */

/**
 * Create a new empty stack.
 * @return Pointer to new stack, or NULL on allocation failure
 */
Stack *stack_create(void);

/**
 * Create a new stack with specified initial capacity.
 * @param capacity Initial capacity
 * @return Pointer to new stack, or NULL on allocation failure
 */
Stack *stack_create_with_capacity(size_t capacity);

/**
 * Free all memory associated with the stack.
 * @param stack Pointer to the stack
 */
void stack_destroy(Stack *stack);

/**
 * Push a value onto the stack.
 * @param stack Pointer to the stack
 * @param value Value to push
 * @return true if successful, false on allocation failure
 */
bool stack_push(Stack *stack, int value);

/**
 * Pop and return the top value from the stack.
 * @param stack Pointer to the stack
 * @param out_value Pointer to store the popped value
 * @return true if successful, false if stack is empty
 */
bool stack_pop(Stack *stack, int *out_value);

/**
 * Get the top value without removing it.
 * @param stack Pointer to the stack
 * @param out_value Pointer to store the value
 * @return true if successful, false if stack is empty
 */
bool stack_peek(const Stack *stack, int *out_value);

/**
 * Get the number of elements in the stack.
 * @param stack Pointer to the stack
 * @return Number of elements
 */
size_t stack_size(const Stack *stack);

/**
 * Check if the stack is empty.
 * @param stack Pointer to the stack
 * @return true if empty, false otherwise
 */
bool stack_is_empty(const Stack *stack);

/**
 * Clear all elements from the stack.
 * @param stack Pointer to the stack
 */
void stack_clear(Stack *stack);

/**
 * Print the stack contents (for debugging).
 * @param stack Pointer to the stack
 */
void stack_print(const Stack *stack);

/* ============== Min Stack Operations ============== */

/**
 * Create a new min stack.
 * @return Pointer to new min stack, or NULL on allocation failure
 */
MinStack *min_stack_create(void);

/**
 * Free all memory associated with the min stack.
 * @param stack Pointer to the min stack
 */
void min_stack_destroy(MinStack *stack);

/**
 * Push a value onto the min stack.
 * @param stack Pointer to the min stack
 * @param value Value to push
 * @return true if successful, false on allocation failure
 */
bool min_stack_push(MinStack *stack, int value);

/**
 * Pop and return the top value from the min stack.
 * @param stack Pointer to the min stack
 * @param out_value Pointer to store the popped value
 * @return true if successful, false if stack is empty
 */
bool min_stack_pop(MinStack *stack, int *out_value);

/**
 * Get the top value without removing it.
 * @param stack Pointer to the min stack
 * @param out_value Pointer to store the value
 * @return true if successful, false if stack is empty
 */
bool min_stack_peek(const MinStack *stack, int *out_value);

/**
 * Get the minimum value in the stack in O(1) time.
 * @param stack Pointer to the min stack
 * @param out_value Pointer to store the minimum value
 * @return true if successful, false if stack is empty
 */
bool min_stack_get_min(const MinStack *stack, int *out_value);

/**
 * Get the number of elements in the min stack.
 * @param stack Pointer to the min stack
 * @return Number of elements
 */
size_t min_stack_size(const MinStack *stack);

/**
 * Check if the min stack is empty.
 * @param stack Pointer to the min stack
 * @return true if empty, false otherwise
 */
bool min_stack_is_empty(const MinStack *stack);

/* ============== Max Stack Operations ============== */

/**
 * Create a new max stack.
 * @return Pointer to new max stack, or NULL on allocation failure
 */
MaxStack *max_stack_create(void);

/**
 * Free all memory associated with the max stack.
 * @param stack Pointer to the max stack
 */
void max_stack_destroy(MaxStack *stack);

/**
 * Push a value onto the max stack.
 * @param stack Pointer to the max stack
 * @param value Value to push
 * @return true if successful, false on allocation failure
 */
bool max_stack_push(MaxStack *stack, int value);

/**
 * Pop and return the top value from the max stack.
 * @param stack Pointer to the max stack
 * @param out_value Pointer to store the popped value
 * @return true if successful, false if stack is empty
 */
bool max_stack_pop(MaxStack *stack, int *out_value);

/**
 * Get the top value without removing it.
 * @param stack Pointer to the max stack
 * @param out_value Pointer to store the value
 * @return true if successful, false if stack is empty
 */
bool max_stack_peek(const MaxStack *stack, int *out_value);

/**
 * Get the maximum value in the stack in O(1) time.
 * @param stack Pointer to the max stack
 * @param out_value Pointer to store the maximum value
 * @return true if successful, false if stack is empty
 */
bool max_stack_get_max(const MaxStack *stack, int *out_value);

/**
 * Get the number of elements in the max stack.
 * @param stack Pointer to the max stack
 * @return Number of elements
 */
size_t max_stack_size(const MaxStack *stack);

/**
 * Check if the max stack is empty.
 * @param stack Pointer to the max stack
 * @return true if empty, false otherwise
 */
bool max_stack_is_empty(const MaxStack *stack);

#endif /* STACK_H */
