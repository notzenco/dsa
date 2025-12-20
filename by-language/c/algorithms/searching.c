/**
 * Searching Algorithms Implementation
 * See searching.h for documentation
 */

#include "searching.h"
#include <stdlib.h>
#include <math.h>

/* ============== Basic Search ============== */

int linear_search(const int *arr, size_t n, int target) {
    if (arr == NULL) return -1;

    for (size_t i = 0; i < n; i++) {
        if (arr[i] == target) {
            return (int)i;
        }
    }
    return -1;
}

int binary_search(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t left = 0;
    size_t right = n - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return (int)mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return -1;
}

static int binary_search_helper(const int *arr, size_t left, size_t right, int target) {
    if (left > right) return -1;

    size_t mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        return (int)mid;
    } else if (arr[mid] < target) {
        return binary_search_helper(arr, mid + 1, right, target);
    } else {
        if (mid == 0) return -1;
        return binary_search_helper(arr, left, mid - 1, target);
    }
}

int binary_search_recursive(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;
    return binary_search_helper(arr, 0, n - 1, target);
}

/* ============== Binary Search Variants ============== */

size_t lower_bound(const int *arr, size_t n, int target) {
    if (arr == NULL) return 0;

    size_t left = 0;
    size_t right = n;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

size_t upper_bound(const int *arr, size_t n, int target) {
    if (arr == NULL) return 0;

    size_t left = 0;
    size_t right = n;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int find_first(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t left = 0;
    size_t right = n - 1;
    int result = -1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = (int)mid;
            if (mid == 0) break;
            right = mid - 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return result;
}

int find_last(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t left = 0;
    size_t right = n - 1;
    int result = -1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = (int)mid;
            left = mid + 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return result;
}

size_t count_occurrences(const int *arr, size_t n, int target) {
    int first = find_first(arr, n, target);
    if (first == -1) return 0;

    int last = find_last(arr, n, target);
    return (size_t)(last - first + 1);
}

size_t search_insert(const int *arr, size_t n, int target) {
    return lower_bound(arr, n, target);
}

/* ============== Advanced Search ============== */

int interpolation_search(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t left = 0;
    size_t right = n - 1;

    while (left <= right && target >= arr[left] && target <= arr[right]) {
        if (left == right) {
            if (arr[left] == target) return (int)left;
            return -1;
        }

        /* Interpolation formula */
        size_t pos = left + (((double)(right - left) / (arr[right] - arr[left])) * (target - arr[left]));

        if (pos > right) pos = right;

        if (arr[pos] == target) {
            return (int)pos;
        } else if (arr[pos] < target) {
            left = pos + 1;
        } else {
            if (pos == 0) break;
            right = pos - 1;
        }
    }
    return -1;
}

int exponential_search(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    if (arr[0] == target) return 0;

    /* Find range by repeated doubling */
    size_t bound = 1;
    while (bound < n && arr[bound] <= target) {
        bound *= 2;
    }

    /* Binary search in found range */
    size_t left = bound / 2;
    size_t right = (bound < n) ? bound : n - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return (int)mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return -1;
}

int jump_search(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t step = (size_t)sqrt((double)n);
    size_t prev = 0;
    size_t curr = step;

    /* Jump until we find a block that might contain target */
    while (curr < n && arr[curr] < target) {
        prev = curr;
        curr += step;
    }

    /* Linear search in the block */
    for (size_t i = prev; i < n && i <= curr; i++) {
        if (arr[i] == target) {
            return (int)i;
        }
    }
    return -1;
}

size_t ternary_search_min(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;

    size_t left = 0;
    size_t right = n - 1;

    while (right - left > 2) {
        size_t mid1 = left + (right - left) / 3;
        size_t mid2 = right - (right - left) / 3;

        if (arr[mid1] < arr[mid2]) {
            right = mid2;
        } else {
            left = mid1;
        }
    }

    /* Find minimum in remaining elements */
    size_t min_idx = left;
    for (size_t i = left + 1; i <= right; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

size_t ternary_search_max(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;

    size_t left = 0;
    size_t right = n - 1;

    while (right - left > 2) {
        size_t mid1 = left + (right - left) / 3;
        size_t mid2 = right - (right - left) / 3;

        if (arr[mid1] > arr[mid2]) {
            right = mid2;
        } else {
            left = mid1;
        }
    }

    /* Find maximum in remaining elements */
    size_t max_idx = left;
    for (size_t i = left + 1; i <= right; i++) {
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }
    return max_idx;
}

/* ============== Rotated Array Search ============== */

int search_rotated(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return -1;

    size_t left = 0;
    size_t right = n - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return (int)mid;
        }

        /* Determine which half is sorted */
        if (arr[left] <= arr[mid]) {
            /* Left half is sorted */
            if (target >= arr[left] && target < arr[mid]) {
                if (mid == 0) break;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            /* Right half is sorted */
            if (target > arr[mid] && target <= arr[right]) {
                left = mid + 1;
            } else {
                if (mid == 0) break;
                right = mid - 1;
            }
        }
    }
    return -1;
}

size_t find_rotation_point(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;
    if (n == 1 || arr[0] < arr[n - 1]) return 0;

    size_t left = 0;
    size_t right = n - 1;

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] > arr[right]) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

size_t find_peak(const int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;
    if (n == 1) return 0;

    size_t left = 0;
    size_t right = n - 1;

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        if (arr[mid] > arr[mid + 1]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

/* ============== 2D Search ============== */

bool search_matrix(const int *matrix, size_t rows, size_t cols, int target,
                   size_t *row_out, size_t *col_out) {
    if (matrix == NULL || rows == 0 || cols == 0) return false;

    /* Start from top-right corner */
    size_t row = 0;
    size_t col = cols - 1;

    while (row < rows && col < cols) {
        int current = matrix[row * cols + col];

        if (current == target) {
            if (row_out) *row_out = row;
            if (col_out) *col_out = col;
            return true;
        } else if (current > target) {
            if (col == 0) break;
            col--;
        } else {
            row++;
        }
    }
    return false;
}

bool search_matrix_sorted(const int *matrix, size_t rows, size_t cols, int target,
                          size_t *row_out, size_t *col_out) {
    if (matrix == NULL || rows == 0 || cols == 0) return false;

    size_t left = 0;
    size_t right = rows * cols - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        int current = matrix[mid];

        if (current == target) {
            if (row_out) *row_out = mid / cols;
            if (col_out) *col_out = mid % cols;
            return true;
        } else if (current < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return false;
}

/* ============== Special Search ============== */

int sqrt_binary(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;

    int left = 1;
    int right = n;
    int result = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        /* Check for overflow */
        if (mid <= n / mid) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

int kth_missing(const int *arr, size_t n, int k) {
    if (arr == NULL || n == 0) return k;

    /* Binary search for position where missing count >= k */
    size_t left = 0;
    size_t right = n;

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        /* Number of missing positive integers before arr[mid] */
        int missing = arr[mid] - (int)(mid + 1);

        if (missing < k) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    /* left is the position where missing count >= k */
    /* The answer is k + left (since 'left' elements from arr are present before it) */
    return k + (int)left;
}
