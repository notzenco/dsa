/**
 * Monotonic Queue Implementation
 *
 * A deque that maintains elements in monotonically decreasing order.
 * Efficient for sliding window maximum problems.
 */

#include "monotonic_queue.h"
#include <stdlib.h>
#include <string.h>

/* ============== Helper Functions ============== */

/**
 * Resize the queue when capacity is reached.
 */
static bool mq_resize(MonotonicQueue *mq, size_t new_capacity) {
    MQEntry *new_data = malloc(new_capacity * sizeof(MQEntry));
    if (new_data == NULL) {
        return false;
    }

    /* Copy elements in order from front to rear */
    size_t j = 0;
    for (size_t i = 0; i < mq->size; i++) {
        size_t idx = (mq->front + i) % mq->capacity;
        new_data[j++] = mq->data[idx];
    }

    free(mq->data);
    mq->data = new_data;
    mq->front = 0;
    mq->rear = mq->size;
    mq->capacity = new_capacity;

    return true;
}

/**
 * Get the index into the circular buffer for rear - 1.
 */
static size_t mq_rear_prev(const MonotonicQueue *mq) {
    if (mq->rear == 0) {
        return mq->capacity - 1;
    }
    return mq->rear - 1;
}

/* ============== Creation and Destruction ============== */

MonotonicQueue *mq_create(void) {
    return mq_create_with_capacity(MQ_INITIAL_CAPACITY);
}

MonotonicQueue *mq_create_with_capacity(size_t capacity) {
    if (capacity == 0) {
        capacity = MQ_INITIAL_CAPACITY;
    }

    MonotonicQueue *mq = malloc(sizeof(MonotonicQueue));
    if (mq == NULL) {
        return NULL;
    }

    mq->data = malloc(capacity * sizeof(MQEntry));
    if (mq->data == NULL) {
        free(mq);
        return NULL;
    }

    mq->front = 0;
    mq->rear = 0;
    mq->size = 0;
    mq->capacity = capacity;

    return mq;
}

void mq_destroy(MonotonicQueue *mq) {
    if (mq != NULL) {
        free(mq->data);
        free(mq);
    }
}

void mq_clear(MonotonicQueue *mq) {
    if (mq != NULL) {
        mq->front = 0;
        mq->rear = 0;
        mq->size = 0;
    }
}

/* ============== Core Operations ============== */

bool mq_push(MonotonicQueue *mq, int index, int value) {
    if (mq == NULL) {
        return false;
    }

    /* Remove all elements from rear that are smaller than new value */
    while (mq->size > 0) {
        size_t rear_idx = mq_rear_prev(mq);
        if (mq->data[rear_idx].value >= value) {
            break;
        }
        /* Remove from rear */
        mq->rear = rear_idx;
        mq->size--;
    }

    /* Resize if needed */
    if (mq->size >= mq->capacity) {
        if (!mq_resize(mq, mq->capacity * 2)) {
            return false;
        }
    }

    /* Add new element at rear */
    mq->data[mq->rear].index = index;
    mq->data[mq->rear].value = value;
    mq->rear = (mq->rear + 1) % mq->capacity;
    mq->size++;

    return true;
}

void mq_pop(MonotonicQueue *mq, int index) {
    if (mq == NULL || mq->size == 0) {
        return;
    }

    /* Only pop if front element's index matches */
    if (mq->data[mq->front].index == index) {
        mq->front = (mq->front + 1) % mq->capacity;
        mq->size--;
    }
}

bool mq_get_max(const MonotonicQueue *mq, int *value) {
    if (mq == NULL || mq->size == 0 || value == NULL) {
        return false;
    }

    *value = mq->data[mq->front].value;
    return true;
}

bool mq_get_max_index(const MonotonicQueue *mq, int *index) {
    if (mq == NULL || mq->size == 0 || index == NULL) {
        return false;
    }

    *index = mq->data[mq->front].index;
    return true;
}

/* ============== Properties ============== */

bool mq_is_empty(const MonotonicQueue *mq) {
    return mq == NULL || mq->size == 0;
}

size_t mq_size(const MonotonicQueue *mq) {
    if (mq == NULL) {
        return 0;
    }
    return mq->size;
}

/* ============== Utility Functions ============== */

int *mq_sliding_window_max(const int *nums, size_t n, size_t k, size_t *result_size) {
    if (nums == NULL || n == 0 || k == 0 || k > n || result_size == NULL) {
        if (result_size != NULL) {
            *result_size = 0;
        }
        return NULL;
    }

    *result_size = n - k + 1;
    int *result = malloc(*result_size * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    MonotonicQueue *mq = mq_create_with_capacity(k);
    if (mq == NULL) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        /* Remove elements outside the window */
        if (i >= k) {
            mq_pop(mq, (int)(i - k));
        }

        /* Push current element */
        mq_push(mq, (int)i, nums[i]);

        /* Record maximum once we have a full window */
        if (i >= k - 1) {
            mq_get_max(mq, &result[i - k + 1]);
        }
    }

    mq_destroy(mq);
    return result;
}

int *mq_sliding_window_min(const int *nums, size_t n, size_t k, size_t *result_size) {
    if (nums == NULL || n == 0 || k == 0 || k > n || result_size == NULL) {
        if (result_size != NULL) {
            *result_size = 0;
        }
        return NULL;
    }

    *result_size = n - k + 1;
    int *result = malloc(*result_size * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    /* Use MonotonicQueueMin for proper encapsulation */
    MonotonicQueueMin *mq = mqmin_create_with_capacity(k);
    if (mq == NULL) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        /* Remove elements outside the window */
        if (i >= k) {
            mqmin_pop(mq, (int)(i - k));
        }

        /* Push current element */
        if (!mqmin_push(mq, (int)i, nums[i])) {
            /* Allocation failure during push */
            mqmin_destroy(mq);
            free(result);
            *result_size = 0;
            return NULL;
        }

        /* Record minimum once we have a full window */
        if (i >= k - 1) {
            mqmin_get_min(mq, &result[i - k + 1]);
        }
    }

    mqmin_destroy(mq);
    return result;
}

/* ============== MonotonicQueueMin Implementation ============== */

/**
 * Get the index into the circular buffer for rear - 1.
 */
static size_t mqmin_rear_prev(const MonotonicQueueMin *mq) {
    if (mq->rear == 0) {
        return mq->capacity - 1;
    }
    return mq->rear - 1;
}

/**
 * Resize the min queue when capacity is reached.
 */
static bool mqmin_resize(MonotonicQueueMin *mq, size_t new_capacity) {
    MQEntry *new_data = malloc(new_capacity * sizeof(MQEntry));
    if (new_data == NULL) {
        return false;
    }

    /* Copy elements in order from front to rear */
    size_t j = 0;
    for (size_t i = 0; i < mq->size; i++) {
        size_t idx = (mq->front + i) % mq->capacity;
        new_data[j++] = mq->data[idx];
    }

    free(mq->data);
    mq->data = new_data;
    mq->front = 0;
    mq->rear = mq->size;
    mq->capacity = new_capacity;

    return true;
}

MonotonicQueueMin *mqmin_create(void) {
    return mqmin_create_with_capacity(MQ_INITIAL_CAPACITY);
}

MonotonicQueueMin *mqmin_create_with_capacity(size_t capacity) {
    if (capacity == 0) {
        capacity = MQ_INITIAL_CAPACITY;
    }

    MonotonicQueueMin *mq = malloc(sizeof(MonotonicQueueMin));
    if (mq == NULL) {
        return NULL;
    }

    mq->data = malloc(capacity * sizeof(MQEntry));
    if (mq->data == NULL) {
        free(mq);
        return NULL;
    }

    mq->front = 0;
    mq->rear = 0;
    mq->size = 0;
    mq->capacity = capacity;

    return mq;
}

void mqmin_destroy(MonotonicQueueMin *mq) {
    if (mq != NULL) {
        free(mq->data);
        free(mq);
    }
}

void mqmin_clear(MonotonicQueueMin *mq) {
    if (mq != NULL) {
        mq->front = 0;
        mq->rear = 0;
        mq->size = 0;
    }
}

bool mqmin_push(MonotonicQueueMin *mq, int index, int value) {
    if (mq == NULL) {
        return false;
    }

    /* Remove all elements from rear that are LARGER than new value */
    while (mq->size > 0) {
        size_t rear_idx = mqmin_rear_prev(mq);
        if (mq->data[rear_idx].value <= value) {
            break;
        }
        /* Remove from rear */
        mq->rear = rear_idx;
        mq->size--;
    }

    /* Resize if needed */
    if (mq->size >= mq->capacity) {
        if (!mqmin_resize(mq, mq->capacity * 2)) {
            return false;
        }
    }

    /* Add new element at rear */
    mq->data[mq->rear].index = index;
    mq->data[mq->rear].value = value;
    mq->rear = (mq->rear + 1) % mq->capacity;
    mq->size++;

    return true;
}

void mqmin_pop(MonotonicQueueMin *mq, int index) {
    if (mq == NULL || mq->size == 0) {
        return;
    }

    /* Only pop if front element's index matches */
    if (mq->data[mq->front].index == index) {
        mq->front = (mq->front + 1) % mq->capacity;
        mq->size--;
    }
}

bool mqmin_get_min(const MonotonicQueueMin *mq, int *value) {
    if (mq == NULL || mq->size == 0 || value == NULL) {
        return false;
    }

    *value = mq->data[mq->front].value;
    return true;
}

bool mqmin_get_min_index(const MonotonicQueueMin *mq, int *index) {
    if (mq == NULL || mq->size == 0 || index == NULL) {
        return false;
    }

    *index = mq->data[mq->front].index;
    return true;
}

bool mqmin_is_empty(const MonotonicQueueMin *mq) {
    return mq == NULL || mq->size == 0;
}

size_t mqmin_size(const MonotonicQueueMin *mq) {
    if (mq == NULL) {
        return 0;
    }
    return mq->size;
}
