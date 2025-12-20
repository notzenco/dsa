/**
 * Sorting Algorithms Implementation
 * See sorting.h for documentation
 */

#include "sorting.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ============== Helper Functions ============== */

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ============== Bubble Sort ============== */

void bubble_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break; /* Already sorted */
        }
    }
}

/* ============== Selection Sort ============== */

void selection_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    for (size_t i = 0; i < n - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

/* ============== Insertion Sort ============== */

void insertion_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    for (size_t i = 1; i < n; i++) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

/* ============== Merge Sort ============== */

static void merge(int *arr, int *temp, size_t left, size_t mid, size_t right) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

static void merge_sort_recursive(int *arr, int *temp, size_t left, size_t right) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        merge_sort_recursive(arr, temp, left, mid);
        merge_sort_recursive(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }
}

void merge_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    int *temp = malloc(n * sizeof(int));
    if (temp == NULL) {
        return;
    }

    merge_sort_recursive(arr, temp, 0, n - 1);
    free(temp);
}

/* ============== Quick Sort ============== */

static size_t partition(int *arr, size_t low, size_t high) {
    int pivot = arr[high];
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

static void quick_sort_recursive(int *arr, size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(arr, low, high);
        if (pi > 0) {
            quick_sort_recursive(arr, low, pi - 1);
        }
        quick_sort_recursive(arr, pi + 1, high);
    }
}

void quick_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }
    quick_sort_recursive(arr, 0, n - 1);
}

/* ============== Quick Sort with Median-of-Three ============== */

static size_t median_of_three(int *arr, size_t low, size_t high) {
    size_t mid = low + (high - low) / 2;

    if (arr[low] > arr[mid]) {
        swap(&arr[low], &arr[mid]);
    }
    if (arr[low] > arr[high]) {
        swap(&arr[low], &arr[high]);
    }
    if (arr[mid] > arr[high]) {
        swap(&arr[mid], &arr[high]);
    }

    swap(&arr[mid], &arr[high - 1]);
    return high - 1;
}

static size_t partition_median(int *arr, size_t low, size_t high) {
    if (high - low < 3) {
        return partition(arr, low, high);
    }

    size_t pivot_idx = median_of_three(arr, low, high);
    int pivot = arr[pivot_idx];
    size_t i = low;
    size_t j = high - 1;

    while (true) {
        while (arr[++i] < pivot);
        while (arr[--j] > pivot);
        if (i >= j) {
            break;
        }
        swap(&arr[i], &arr[j]);
    }
    swap(&arr[i], &arr[high - 1]);
    return i;
}

static void quick_sort_median_recursive(int *arr, size_t low, size_t high) {
    if (low + 10 > high) {
        /* Use insertion sort for small subarrays */
        for (size_t i = low + 1; i <= high; i++) {
            int key = arr[i];
            size_t j = i;
            while (j > low && arr[j - 1] > key) {
                arr[j] = arr[j - 1];
                j--;
            }
            arr[j] = key;
        }
        return;
    }

    size_t pi = partition_median(arr, low, high);
    if (pi > low) {
        quick_sort_median_recursive(arr, low, pi - 1);
    }
    quick_sort_median_recursive(arr, pi + 1, high);
}

void quick_sort_median(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }
    quick_sort_median_recursive(arr, 0, n - 1);
}

/* ============== Counting Sort ============== */

void counting_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    /* Find range */
    int min_val = arr[0];
    int max_val = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }

    size_t range = (size_t)(max_val - min_val + 1);
    int *count = calloc(range, sizeof(int));
    int *output = malloc(n * sizeof(int));

    if (count == NULL || output == NULL) {
        free(count);
        free(output);
        return;
    }

    /* Count occurrences */
    for (size_t i = 0; i < n; i++) {
        count[arr[i] - min_val]++;
    }

    /* Cumulative count */
    for (size_t i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    /* Build output (stable) */
    for (size_t i = n; i > 0; i--) {
        output[count[arr[i - 1] - min_val] - 1] = arr[i - 1];
        count[arr[i - 1] - min_val]--;
    }

    /* Copy back */
    memcpy(arr, output, n * sizeof(int));

    free(count);
    free(output);
}

/* ============== Radix Sort (LSD) ============== */

static void counting_sort_digit(int *arr, size_t n, int exp) {
    int *output = malloc(n * sizeof(int));
    int count[10] = {0};

    if (output == NULL) {
        return;
    }

    /* Count occurrences of each digit */
    for (size_t i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    /* Cumulative count */
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    /* Build output (stable) */
    for (size_t i = n; i > 0; i--) {
        int digit = (arr[i - 1] / exp) % 10;
        output[count[digit] - 1] = arr[i - 1];
        count[digit]--;
    }

    /* Copy back */
    memcpy(arr, output, n * sizeof(int));
    free(output);
}

void radix_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    /* Find max to know number of digits */
    int max_val = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    /* Sort by each digit */
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        counting_sort_digit(arr, n, exp);
    }
}

/* ============== Shell Sort ============== */

void shell_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    /* Marcin Ciura's gap sequence */
    size_t gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    size_t num_gaps = sizeof(gaps) / sizeof(gaps[0]);

    for (size_t g = 0; g < num_gaps; g++) {
        size_t gap = gaps[g];
        if (gap >= n) continue;

        for (size_t i = gap; i < n; i++) {
            int temp = arr[i];
            size_t j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

/* ============== Dutch National Flag ============== */

void dutch_flag_sort(int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return;
    }

    size_t low = 0;
    size_t mid = 0;
    size_t high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            swap(&arr[low], &arr[mid]);
            low++;
            mid++;
        } else if (arr[mid] == 1) {
            mid++;
        } else { /* arr[mid] == 2 */
            swap(&arr[mid], &arr[high]);
            if (high == 0) break;
            high--;
        }
    }
}

/* ============== Quick Select ============== */

static size_t partition_select(int *arr, size_t low, size_t high) {
    int pivot = arr[high];
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

int quick_select(int *arr, size_t n, size_t k) {
    if (arr == NULL || n == 0 || k >= n) {
        return INT_MIN;
    }

    /* Make a copy to avoid modifying original */
    int *copy = malloc(n * sizeof(int));
    if (copy == NULL) {
        return INT_MIN;
    }
    memcpy(copy, arr, n * sizeof(int));

    size_t low = 0;
    size_t high = n - 1;

    while (low <= high) {
        size_t pi = partition_select(copy, low, high);

        if (pi == k) {
            int result = copy[pi];
            free(copy);
            return result;
        } else if (pi < k) {
            low = pi + 1;
        } else {
            if (pi == 0) break;
            high = pi - 1;
        }
    }

    free(copy);
    return INT_MIN;
}

/* ============== Utility Functions ============== */

bool is_sorted(const int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return true;
    }

    for (size_t i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

bool is_sorted_desc(const int *arr, size_t n) {
    if (arr == NULL || n <= 1) {
        return true;
    }

    for (size_t i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            return false;
        }
    }
    return true;
}
