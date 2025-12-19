/**
 * Stack Implementation
 * See stack.h for documentation
 */

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ============== Basic Stack Implementation ============== */

static bool stack_resize(Stack *stack, size_t new_capacity) {
    int *new_data = realloc(stack->data, new_capacity * sizeof(int));
    if (new_data == NULL && new_capacity > 0) {
        return false;
    }
    stack->data = new_data;
    stack->capacity = new_capacity;
    return true;
}

Stack *stack_create(void) {
    return stack_create_with_capacity(STACK_INITIAL_CAPACITY);
}

Stack *stack_create_with_capacity(size_t capacity) {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = STACK_INITIAL_CAPACITY;
    }

    stack->data = malloc(capacity * sizeof(int));
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = capacity;
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack != NULL) {
        free(stack->data);
        free(stack);
    }
}

bool stack_push(Stack *stack, int value) {
    if (stack == NULL) {
        return false;
    }

    if (stack->size >= stack->capacity) {
        if (!stack_resize(stack, stack->capacity * 2)) {
            return false;
        }
    }

    stack->data[stack->size++] = value;
    return true;
}

bool stack_pop(Stack *stack, int *out_value) {
    if (stack == NULL || stack->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = stack->data[stack->size - 1];
    }
    stack->size--;

    /* Shrink if needed */
    if (stack->size > 0 && stack->size < stack->capacity / 4) {
        stack_resize(stack, stack->capacity / 2);
    }

    return true;
}

bool stack_peek(const Stack *stack, int *out_value) {
    if (stack == NULL || stack->size == 0 || out_value == NULL) {
        return false;
    }

    *out_value = stack->data[stack->size - 1];
    return true;
}

size_t stack_size(const Stack *stack) {
    return stack != NULL ? stack->size : 0;
}

bool stack_is_empty(const Stack *stack) {
    return stack == NULL || stack->size == 0;
}

void stack_clear(Stack *stack) {
    if (stack != NULL) {
        stack->size = 0;
    }
}

void stack_print(const Stack *stack) {
    if (stack == NULL) {
        printf("NULL\n");
        return;
    }

    printf("TOP -> ");
    for (size_t i = stack->size; i > 0; i--) {
        printf("[%d] ", stack->data[i - 1]);
    }
    printf("<- BOTTOM (size=%lu)\n", (unsigned long)stack->size);
}

/* ============== Min Stack Implementation ============== */

MinStack *min_stack_create(void) {
    MinStack *stack = malloc(sizeof(MinStack));
    if (stack == NULL) {
        return NULL;
    }

    stack->main_stack = stack_create();
    if (stack->main_stack == NULL) {
        free(stack);
        return NULL;
    }

    stack->min_stack = stack_create();
    if (stack->min_stack == NULL) {
        stack_destroy(stack->main_stack);
        free(stack);
        return NULL;
    }

    return stack;
}

void min_stack_destroy(MinStack *stack) {
    if (stack != NULL) {
        stack_destroy(stack->main_stack);
        stack_destroy(stack->min_stack);
        free(stack);
    }
}

bool min_stack_push(MinStack *stack, int value) {
    if (stack == NULL) {
        return false;
    }

    if (!stack_push(stack->main_stack, value)) {
        return false;
    }

    /* Push to min stack if it's empty or value is <= current min */
    int current_min;
    if (stack_is_empty(stack->min_stack) ||
        (stack_peek(stack->min_stack, &current_min) && value <= current_min)) {
        if (!stack_push(stack->min_stack, value)) {
            stack_pop(stack->main_stack, NULL);
            return false;
        }
    }

    return true;
}

bool min_stack_pop(MinStack *stack, int *out_value) {
    if (stack == NULL || stack_is_empty(stack->main_stack)) {
        return false;
    }

    int popped;
    if (!stack_pop(stack->main_stack, &popped)) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = popped;
    }

    /* Pop from min stack if popped value equals current min */
    int current_min;
    if (stack_peek(stack->min_stack, &current_min) && popped == current_min) {
        stack_pop(stack->min_stack, NULL);
    }

    return true;
}

bool min_stack_peek(const MinStack *stack, int *out_value) {
    if (stack == NULL) {
        return false;
    }
    return stack_peek(stack->main_stack, out_value);
}

bool min_stack_get_min(const MinStack *stack, int *out_value) {
    if (stack == NULL) {
        return false;
    }
    return stack_peek(stack->min_stack, out_value);
}

size_t min_stack_size(const MinStack *stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack_size(stack->main_stack);
}

bool min_stack_is_empty(const MinStack *stack) {
    return stack == NULL || stack_is_empty(stack->main_stack);
}

/* ============== Max Stack Implementation ============== */

MaxStack *max_stack_create(void) {
    MaxStack *stack = malloc(sizeof(MaxStack));
    if (stack == NULL) {
        return NULL;
    }

    stack->main_stack = stack_create();
    if (stack->main_stack == NULL) {
        free(stack);
        return NULL;
    }

    stack->max_stack = stack_create();
    if (stack->max_stack == NULL) {
        stack_destroy(stack->main_stack);
        free(stack);
        return NULL;
    }

    return stack;
}

void max_stack_destroy(MaxStack *stack) {
    if (stack != NULL) {
        stack_destroy(stack->main_stack);
        stack_destroy(stack->max_stack);
        free(stack);
    }
}

bool max_stack_push(MaxStack *stack, int value) {
    if (stack == NULL) {
        return false;
    }

    if (!stack_push(stack->main_stack, value)) {
        return false;
    }

    /* Push to max stack if it's empty or value is >= current max */
    int current_max;
    if (stack_is_empty(stack->max_stack) ||
        (stack_peek(stack->max_stack, &current_max) && value >= current_max)) {
        if (!stack_push(stack->max_stack, value)) {
            stack_pop(stack->main_stack, NULL);
            return false;
        }
    }

    return true;
}

bool max_stack_pop(MaxStack *stack, int *out_value) {
    if (stack == NULL || stack_is_empty(stack->main_stack)) {
        return false;
    }

    int popped;
    if (!stack_pop(stack->main_stack, &popped)) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = popped;
    }

    /* Pop from max stack if popped value equals current max */
    int current_max;
    if (stack_peek(stack->max_stack, &current_max) && popped == current_max) {
        stack_pop(stack->max_stack, NULL);
    }

    return true;
}

bool max_stack_peek(const MaxStack *stack, int *out_value) {
    if (stack == NULL) {
        return false;
    }
    return stack_peek(stack->main_stack, out_value);
}

bool max_stack_get_max(const MaxStack *stack, int *out_value) {
    if (stack == NULL) {
        return false;
    }
    return stack_peek(stack->max_stack, out_value);
}

size_t max_stack_size(const MaxStack *stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack_size(stack->main_stack);
}

bool max_stack_is_empty(const MaxStack *stack) {
    return stack == NULL || stack_is_empty(stack->main_stack);
}
