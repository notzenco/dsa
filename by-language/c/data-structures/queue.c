/**
 * Queue, Circular Queue, and Deque Implementation
 * See queue.h for documentation
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============== Queue Implementation ============== */

static bool queue_resize(Queue *queue, size_t new_capacity) {
    int *new_data = malloc(new_capacity * sizeof(int));
    if (new_data == NULL) {
        return false;
    }

    /* Copy elements in order */
    for (size_t i = 0; i < queue->size; i++) {
        new_data[i] = queue->data[(queue->front + i) % queue->capacity];
    }

    free(queue->data);
    queue->data = new_data;
    queue->front = 0;
    queue->rear = queue->size;
    queue->capacity = new_capacity;
    return true;
}

Queue *queue_create(void) {
    return queue_create_with_capacity(QUEUE_INITIAL_CAPACITY);
}

Queue *queue_create_with_capacity(size_t capacity) {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = QUEUE_INITIAL_CAPACITY;
    }

    queue->data = malloc(capacity * sizeof(int));
    if (queue->data == NULL) {
        free(queue);
        return NULL;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void queue_destroy(Queue *queue) {
    if (queue != NULL) {
        free(queue->data);
        free(queue);
    }
}

bool queue_enqueue(Queue *queue, int value) {
    if (queue == NULL) {
        return false;
    }

    if (queue->size >= queue->capacity) {
        if (!queue_resize(queue, queue->capacity * 2)) {
            return false;
        }
    }

    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return true;
}

bool queue_dequeue(Queue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = queue->data[queue->front];
    }

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    /* Shrink if needed */
    if (queue->size > 0 && queue->size < queue->capacity / 4) {
        queue_resize(queue, queue->capacity / 2);
    }

    return true;
}

bool queue_front(const Queue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0 || out_value == NULL) {
        return false;
    }
    *out_value = queue->data[queue->front];
    return true;
}

bool queue_rear(const Queue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0 || out_value == NULL) {
        return false;
    }
    size_t rear_idx = (queue->rear + queue->capacity - 1) % queue->capacity;
    *out_value = queue->data[rear_idx];
    return true;
}

size_t queue_size(const Queue *queue) {
    return queue != NULL ? queue->size : 0;
}

bool queue_is_empty(const Queue *queue) {
    return queue == NULL || queue->size == 0;
}

void queue_clear(Queue *queue) {
    if (queue != NULL) {
        queue->front = 0;
        queue->rear = 0;
        queue->size = 0;
    }
}

void queue_print(const Queue *queue) {
    if (queue == NULL) {
        printf("NULL\n");
        return;
    }

    printf("FRONT -> ");
    for (size_t i = 0; i < queue->size; i++) {
        printf("[%d] ", queue->data[(queue->front + i) % queue->capacity]);
    }
    printf("<- REAR (size=%lu)\n", (unsigned long)queue->size);
}

/* ============== Circular Queue Implementation ============== */

CircularQueue *circular_queue_create(size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    CircularQueue *queue = malloc(sizeof(CircularQueue));
    if (queue == NULL) {
        return NULL;
    }

    queue->data = malloc(capacity * sizeof(int));
    if (queue->data == NULL) {
        free(queue);
        return NULL;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void circular_queue_destroy(CircularQueue *queue) {
    if (queue != NULL) {
        free(queue->data);
        free(queue);
    }
}

bool circular_queue_enqueue(CircularQueue *queue, int value) {
    if (queue == NULL || circular_queue_is_full(queue)) {
        return false;
    }

    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return true;
}

bool circular_queue_dequeue(CircularQueue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = queue->data[queue->front];
    }

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return true;
}

bool circular_queue_front(const CircularQueue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0 || out_value == NULL) {
        return false;
    }
    *out_value = queue->data[queue->front];
    return true;
}

bool circular_queue_rear(const CircularQueue *queue, int *out_value) {
    if (queue == NULL || queue->size == 0 || out_value == NULL) {
        return false;
    }
    size_t rear_idx = (queue->rear + queue->capacity - 1) % queue->capacity;
    *out_value = queue->data[rear_idx];
    return true;
}

bool circular_queue_is_empty(const CircularQueue *queue) {
    return queue == NULL || queue->size == 0;
}

bool circular_queue_is_full(const CircularQueue *queue) {
    return queue != NULL && queue->size == queue->capacity;
}

size_t circular_queue_size(const CircularQueue *queue) {
    return queue != NULL ? queue->size : 0;
}

/* ============== Deque Implementation ============== */

static bool deque_resize(Deque *deque, size_t new_capacity) {
    int *new_data = malloc(new_capacity * sizeof(int));
    if (new_data == NULL) {
        return false;
    }

    /* Copy elements in order */
    for (size_t i = 0; i < deque->size; i++) {
        new_data[i] = deque->data[(deque->front + i) % deque->capacity];
    }

    free(deque->data);
    deque->data = new_data;
    deque->front = 0;
    deque->rear = deque->size;
    deque->capacity = new_capacity;
    return true;
}

Deque *deque_create(void) {
    return deque_create_with_capacity(QUEUE_INITIAL_CAPACITY);
}

Deque *deque_create_with_capacity(size_t capacity) {
    Deque *deque = malloc(sizeof(Deque));
    if (deque == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = QUEUE_INITIAL_CAPACITY;
    }

    deque->data = malloc(capacity * sizeof(int));
    if (deque->data == NULL) {
        free(deque);
        return NULL;
    }

    deque->front = 0;
    deque->rear = 0;
    deque->size = 0;
    deque->capacity = capacity;
    return deque;
}

void deque_destroy(Deque *deque) {
    if (deque != NULL) {
        free(deque->data);
        free(deque);
    }
}

bool deque_push_front(Deque *deque, int value) {
    if (deque == NULL) {
        return false;
    }

    if (deque->size >= deque->capacity) {
        if (!deque_resize(deque, deque->capacity * 2)) {
            return false;
        }
    }

    deque->front = (deque->front + deque->capacity - 1) % deque->capacity;
    deque->data[deque->front] = value;
    deque->size++;
    return true;
}

bool deque_push_back(Deque *deque, int value) {
    if (deque == NULL) {
        return false;
    }

    if (deque->size >= deque->capacity) {
        if (!deque_resize(deque, deque->capacity * 2)) {
            return false;
        }
    }

    deque->data[deque->rear] = value;
    deque->rear = (deque->rear + 1) % deque->capacity;
    deque->size++;
    return true;
}

bool deque_pop_front(Deque *deque, int *out_value) {
    if (deque == NULL || deque->size == 0) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = deque->data[deque->front];
    }

    deque->front = (deque->front + 1) % deque->capacity;
    deque->size--;

    /* Shrink if needed */
    if (deque->size > 0 && deque->size < deque->capacity / 4) {
        deque_resize(deque, deque->capacity / 2);
    }

    return true;
}

bool deque_pop_back(Deque *deque, int *out_value) {
    if (deque == NULL || deque->size == 0) {
        return false;
    }

    deque->rear = (deque->rear + deque->capacity - 1) % deque->capacity;

    if (out_value != NULL) {
        *out_value = deque->data[deque->rear];
    }

    deque->size--;

    /* Shrink if needed */
    if (deque->size > 0 && deque->size < deque->capacity / 4) {
        deque_resize(deque, deque->capacity / 2);
    }

    return true;
}

bool deque_front(const Deque *deque, int *out_value) {
    if (deque == NULL || deque->size == 0 || out_value == NULL) {
        return false;
    }
    *out_value = deque->data[deque->front];
    return true;
}

bool deque_back(const Deque *deque, int *out_value) {
    if (deque == NULL || deque->size == 0 || out_value == NULL) {
        return false;
    }
    size_t back_idx = (deque->rear + deque->capacity - 1) % deque->capacity;
    *out_value = deque->data[back_idx];
    return true;
}

size_t deque_size(const Deque *deque) {
    return deque != NULL ? deque->size : 0;
}

bool deque_is_empty(const Deque *deque) {
    return deque == NULL || deque->size == 0;
}

void deque_clear(Deque *deque) {
    if (deque != NULL) {
        deque->front = 0;
        deque->rear = 0;
        deque->size = 0;
    }
}

bool deque_get(const Deque *deque, size_t index, int *out_value) {
    if (deque == NULL || index >= deque->size || out_value == NULL) {
        return false;
    }
    *out_value = deque->data[(deque->front + index) % deque->capacity];
    return true;
}

void deque_print(const Deque *deque) {
    if (deque == NULL) {
        printf("NULL\n");
        return;
    }

    printf("FRONT -> ");
    for (size_t i = 0; i < deque->size; i++) {
        printf("[%d] ", deque->data[(deque->front + i) % deque->capacity]);
    }
    printf("<- BACK (size=%lu)\n", (unsigned long)deque->size);
}
