/**
 * Heap and Priority Queue Implementation
 * See heap.h for documentation
 */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============== Helper Functions ============== */

static inline size_t parent(size_t i) { return (i - 1) / 2; }
static inline size_t left_child(size_t i) { return 2 * i + 1; }
static inline size_t right_child(size_t i) { return 2 * i + 2; }

static void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ============== Min Heap Implementation ============== */

static void min_heap_sift_up(MinHeap *heap, size_t i) {
    while (i > 0 && heap->data[parent(i)] > heap->data[i]) {
        swap_int(&heap->data[parent(i)], &heap->data[i]);
        i = parent(i);
    }
}

static void min_heap_sift_down(MinHeap *heap, size_t i) {
    size_t min_idx = i;

    size_t left = left_child(i);
    if (left < heap->size && heap->data[left] < heap->data[min_idx]) {
        min_idx = left;
    }

    size_t right = right_child(i);
    if (right < heap->size && heap->data[right] < heap->data[min_idx]) {
        min_idx = right;
    }

    if (min_idx != i) {
        swap_int(&heap->data[i], &heap->data[min_idx]);
        min_heap_sift_down(heap, min_idx);
    }
}

static bool min_heap_resize(MinHeap *heap, size_t new_capacity) {
    int *new_data = realloc(heap->data, new_capacity * sizeof(int));
    if (new_data == NULL && new_capacity > 0) {
        return false;
    }
    heap->data = new_data;
    heap->capacity = new_capacity;
    return true;
}

MinHeap *min_heap_create(void) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }

    heap->data = malloc(HEAP_INITIAL_CAPACITY * sizeof(int));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = HEAP_INITIAL_CAPACITY;
    return heap;
}

MinHeap *min_heap_from_array(const int *arr, size_t n) {
    if (arr == NULL && n > 0) {
        return NULL;
    }

    MinHeap *heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }

    size_t capacity = n > HEAP_INITIAL_CAPACITY ? n : HEAP_INITIAL_CAPACITY;
    heap->data = malloc(capacity * sizeof(int));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    memcpy(heap->data, arr, n * sizeof(int));
    heap->size = n;
    heap->capacity = capacity;

    /* Heapify - start from last non-leaf node */
    if (n > 1) {
        for (size_t i = n / 2; i > 0; i--) {
            min_heap_sift_down(heap, i - 1);
        }
        min_heap_sift_down(heap, 0);
    }

    return heap;
}

void min_heap_destroy(MinHeap *heap) {
    if (heap != NULL) {
        free(heap->data);
        free(heap);
    }
}

bool min_heap_push(MinHeap *heap, int value) {
    if (heap == NULL) {
        return false;
    }

    if (heap->size >= heap->capacity) {
        if (!min_heap_resize(heap, heap->capacity * 2)) {
            return false;
        }
    }

    heap->data[heap->size] = value;
    min_heap_sift_up(heap, heap->size);
    heap->size++;
    return true;
}

bool min_heap_pop(MinHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = heap->data[0];
    }

    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    if (heap->size > 0) {
        min_heap_sift_down(heap, 0);
    }

    return true;
}

bool min_heap_peek(const MinHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0 || out_value == NULL) {
        return false;
    }

    *out_value = heap->data[0];
    return true;
}

size_t min_heap_size(const MinHeap *heap) {
    return heap != NULL ? heap->size : 0;
}

bool min_heap_is_empty(const MinHeap *heap) {
    return heap == NULL || heap->size == 0;
}

void min_heap_clear(MinHeap *heap) {
    if (heap != NULL) {
        heap->size = 0;
    }
}

/* ============== Max Heap Implementation ============== */

static void max_heap_sift_up(MaxHeap *heap, size_t i) {
    while (i > 0 && heap->data[parent(i)] < heap->data[i]) {
        swap_int(&heap->data[parent(i)], &heap->data[i]);
        i = parent(i);
    }
}

static void max_heap_sift_down(MaxHeap *heap, size_t i) {
    size_t max_idx = i;

    size_t left = left_child(i);
    if (left < heap->size && heap->data[left] > heap->data[max_idx]) {
        max_idx = left;
    }

    size_t right = right_child(i);
    if (right < heap->size && heap->data[right] > heap->data[max_idx]) {
        max_idx = right;
    }

    if (max_idx != i) {
        swap_int(&heap->data[i], &heap->data[max_idx]);
        max_heap_sift_down(heap, max_idx);
    }
}

static bool max_heap_resize(MaxHeap *heap, size_t new_capacity) {
    int *new_data = realloc(heap->data, new_capacity * sizeof(int));
    if (new_data == NULL && new_capacity > 0) {
        return false;
    }
    heap->data = new_data;
    heap->capacity = new_capacity;
    return true;
}

MaxHeap *max_heap_create(void) {
    MaxHeap *heap = malloc(sizeof(MaxHeap));
    if (heap == NULL) {
        return NULL;
    }

    heap->data = malloc(HEAP_INITIAL_CAPACITY * sizeof(int));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = HEAP_INITIAL_CAPACITY;
    return heap;
}

MaxHeap *max_heap_from_array(const int *arr, size_t n) {
    if (arr == NULL && n > 0) {
        return NULL;
    }

    MaxHeap *heap = malloc(sizeof(MaxHeap));
    if (heap == NULL) {
        return NULL;
    }

    size_t capacity = n > HEAP_INITIAL_CAPACITY ? n : HEAP_INITIAL_CAPACITY;
    heap->data = malloc(capacity * sizeof(int));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    memcpy(heap->data, arr, n * sizeof(int));
    heap->size = n;
    heap->capacity = capacity;

    /* Heapify */
    if (n > 1) {
        for (size_t i = n / 2; i > 0; i--) {
            max_heap_sift_down(heap, i - 1);
        }
        max_heap_sift_down(heap, 0);
    }

    return heap;
}

void max_heap_destroy(MaxHeap *heap) {
    if (heap != NULL) {
        free(heap->data);
        free(heap);
    }
}

bool max_heap_push(MaxHeap *heap, int value) {
    if (heap == NULL) {
        return false;
    }

    if (heap->size >= heap->capacity) {
        if (!max_heap_resize(heap, heap->capacity * 2)) {
            return false;
        }
    }

    heap->data[heap->size] = value;
    max_heap_sift_up(heap, heap->size);
    heap->size++;
    return true;
}

bool max_heap_pop(MaxHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = heap->data[0];
    }

    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    if (heap->size > 0) {
        max_heap_sift_down(heap, 0);
    }

    return true;
}

bool max_heap_peek(const MaxHeap *heap, int *out_value) {
    if (heap == NULL || heap->size == 0 || out_value == NULL) {
        return false;
    }

    *out_value = heap->data[0];
    return true;
}

size_t max_heap_size(const MaxHeap *heap) {
    return heap != NULL ? heap->size : 0;
}

bool max_heap_is_empty(const MaxHeap *heap) {
    return heap == NULL || heap->size == 0;
}

void max_heap_clear(MaxHeap *heap) {
    if (heap != NULL) {
        heap->size = 0;
    }
}

/* ============== Priority Queue Implementation ============== */

static void pq_swap(PQEntry *a, PQEntry *b) {
    PQEntry temp = *a;
    *a = *b;
    *b = temp;
}

static void pq_sift_up(PriorityQueue *pq, size_t i) {
    while (i > 0 && pq->data[parent(i)].priority > pq->data[i].priority) {
        pq_swap(&pq->data[parent(i)], &pq->data[i]);
        i = parent(i);
    }
}

static void pq_sift_down(PriorityQueue *pq, size_t i) {
    size_t min_idx = i;

    size_t left = left_child(i);
    if (left < pq->size && pq->data[left].priority < pq->data[min_idx].priority) {
        min_idx = left;
    }

    size_t right = right_child(i);
    if (right < pq->size && pq->data[right].priority < pq->data[min_idx].priority) {
        min_idx = right;
    }

    if (min_idx != i) {
        pq_swap(&pq->data[i], &pq->data[min_idx]);
        pq_sift_down(pq, min_idx);
    }
}

static bool pq_resize(PriorityQueue *pq, size_t new_capacity) {
    PQEntry *new_data = realloc(pq->data, new_capacity * sizeof(PQEntry));
    if (new_data == NULL && new_capacity > 0) {
        return false;
    }
    pq->data = new_data;
    pq->capacity = new_capacity;
    return true;
}

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    if (pq == NULL) {
        return NULL;
    }

    pq->data = malloc(HEAP_INITIAL_CAPACITY * sizeof(PQEntry));
    if (pq->data == NULL) {
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = HEAP_INITIAL_CAPACITY;
    return pq;
}

void pq_destroy(PriorityQueue *pq) {
    if (pq != NULL) {
        free(pq->data);
        free(pq);
    }
}

bool pq_push(PriorityQueue *pq, int value, int priority) {
    if (pq == NULL) {
        return false;
    }

    if (pq->size >= pq->capacity) {
        if (!pq_resize(pq, pq->capacity * 2)) {
            return false;
        }
    }

    pq->data[pq->size].value = value;
    pq->data[pq->size].priority = priority;
    pq_sift_up(pq, pq->size);
    pq->size++;
    return true;
}

bool pq_pop(PriorityQueue *pq, int *out_value) {
    if (pq == NULL || pq->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = pq->data[0].value;
    }

    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;

    if (pq->size > 0) {
        pq_sift_down(pq, 0);
    }

    return true;
}

bool pq_peek(const PriorityQueue *pq, int *out_value) {
    if (pq == NULL || pq->size == 0 || out_value == NULL) {
        return false;
    }

    *out_value = pq->data[0].value;
    return true;
}

size_t pq_size(const PriorityQueue *pq) {
    return pq != NULL ? pq->size : 0;
}

bool pq_is_empty(const PriorityQueue *pq) {
    return pq == NULL || pq->size == 0;
}

void pq_clear(PriorityQueue *pq) {
    if (pq != NULL) {
        pq->size = 0;
    }
}

/* ============== Heap Sort Implementation ============== */

static void heapify_max(int *arr, size_t n, size_t i) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap_int(&arr[i], &arr[largest]);
        heapify_max(arr, n, largest);
    }
}

static void heapify_min(int *arr, size_t n, size_t i) {
    size_t smallest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && arr[left] < arr[smallest]) {
        smallest = left;
    }

    if (right < n && arr[right] < arr[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        swap_int(&arr[i], &arr[smallest]);
        heapify_min(arr, n, smallest);
    }
}

void heap_sort_asc(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    /* Build max heap */
    for (size_t i = n / 2; i > 0; i--) {
        heapify_max(arr, n, i - 1);
    }

    /* Extract elements */
    for (size_t i = n - 1; i > 0; i--) {
        swap_int(&arr[0], &arr[i]);
        heapify_max(arr, i, 0);
    }
}

void heap_sort_desc(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    /* Build min heap */
    for (size_t i = n / 2; i > 0; i--) {
        heapify_min(arr, n, i - 1);
    }

    /* Extract elements */
    for (size_t i = n - 1; i > 0; i--) {
        swap_int(&arr[0], &arr[i]);
        heapify_min(arr, i, 0);
    }
}
