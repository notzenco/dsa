/**
 * Dynamic Array Implementation
 * See dynamic_array.h for documentation
 */

#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal: Resize the array to new capacity */
static bool da_resize(DynamicArray *arr, size_t new_capacity) {
    if (new_capacity < arr->size) {
        return false;
    }

    int *new_data = realloc(arr->data, new_capacity * sizeof(int));
    if (new_data == NULL && new_capacity > 0) {
        return false;
    }

    arr->data = new_data;
    arr->capacity = new_capacity;
    return true;
}

DynamicArray *da_create(void) {
    return da_create_with_capacity(DA_INITIAL_CAPACITY);
}

DynamicArray *da_create_with_capacity(size_t capacity) {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = DA_INITIAL_CAPACITY;
    }

    arr->data = malloc(capacity * sizeof(int));
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }

    arr->size = 0;
    arr->capacity = capacity;
    return arr;
}

void da_destroy(DynamicArray *arr) {
    if (arr != NULL) {
        free(arr->data);
        free(arr);
    }
}

size_t da_size(const DynamicArray *arr) {
    return arr != NULL ? arr->size : 0;
}

size_t da_capacity(const DynamicArray *arr) {
    return arr != NULL ? arr->capacity : 0;
}

bool da_is_empty(const DynamicArray *arr) {
    return arr == NULL || arr->size == 0;
}

bool da_get(const DynamicArray *arr, size_t index, int *out_value) {
    if (arr == NULL || index >= arr->size || out_value == NULL) {
        return false;
    }
    *out_value = arr->data[index];
    return true;
}

bool da_set(DynamicArray *arr, size_t index, int value) {
    if (arr == NULL || index >= arr->size) {
        return false;
    }
    arr->data[index] = value;
    return true;
}

bool da_push_back(DynamicArray *arr, int value) {
    if (arr == NULL) {
        return false;
    }

    /* Resize if necessary */
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * DA_GROWTH_FACTOR;
        if (!da_resize(arr, new_capacity)) {
            return false;
        }
    }

    arr->data[arr->size++] = value;
    return true;
}

bool da_pop_back(DynamicArray *arr, int *out_value) {
    if (arr == NULL || arr->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = arr->data[arr->size - 1];
    }
    arr->size--;

    /* Shrink if necessary */
    if (arr->size > 0 && arr->size < arr->capacity / DA_SHRINK_THRESHOLD) {
        da_resize(arr, arr->capacity / DA_GROWTH_FACTOR);
    }

    return true;
}

bool da_insert(DynamicArray *arr, size_t index, int value) {
    if (arr == NULL || index > arr->size) {
        return false;
    }

    /* Resize if necessary */
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * DA_GROWTH_FACTOR;
        if (!da_resize(arr, new_capacity)) {
            return false;
        }
    }

    /* Shift elements to the right */
    for (size_t i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }

    arr->data[index] = value;
    arr->size++;
    return true;
}

bool da_remove(DynamicArray *arr, size_t index, int *out_value) {
    if (arr == NULL || index >= arr->size) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = arr->data[index];
    }

    /* Shift elements to the left */
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }

    arr->size--;

    /* Shrink if necessary */
    if (arr->size > 0 && arr->size < arr->capacity / DA_SHRINK_THRESHOLD) {
        da_resize(arr, arr->capacity / DA_GROWTH_FACTOR);
    }

    return true;
}

int da_find(const DynamicArray *arr, int value) {
    if (arr == NULL) {
        return -1;
    }

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) {
            return (int)i;
        }
    }
    return -1;
}

bool da_contains(const DynamicArray *arr, int value) {
    return da_find(arr, value) != -1;
}

void da_clear(DynamicArray *arr) {
    if (arr != NULL) {
        arr->size = 0;
    }
}

bool da_reserve(DynamicArray *arr, size_t capacity) {
    if (arr == NULL) {
        return false;
    }

    if (capacity > arr->capacity) {
        return da_resize(arr, capacity);
    }
    return true;
}

bool da_shrink_to_fit(DynamicArray *arr) {
    if (arr == NULL || arr->size == 0) {
        return false;
    }
    return da_resize(arr, arr->size);
}

void da_reverse(DynamicArray *arr) {
    if (arr == NULL || arr->size <= 1) {
        return;
    }

    size_t left = 0;
    size_t right = arr->size - 1;
    while (left < right) {
        int temp = arr->data[left];
        arr->data[left] = arr->data[right];
        arr->data[right] = temp;
        left++;
        right--;
    }
}

void da_print(const DynamicArray *arr) {
    if (arr == NULL) {
        printf("NULL\n");
        return;
    }

    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d", arr->data[i]);
        if (i < arr->size - 1) {
            printf(", ");
        }
    }
    printf("] (size=%lu, capacity=%lu)\n", (unsigned long)arr->size, (unsigned long)arr->capacity);
}
