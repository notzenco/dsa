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

/* ============== Two Pointers Techniques ============== */

/* Helper for sorting used by three_sum */
static int compare_ints(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

bool two_sum_sorted(const int *arr, size_t n, int target, size_t *i_out, size_t *j_out) {
    if (arr == NULL || n < 2) return false;

    size_t left = 0;
    size_t right = n - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];

        if (sum == target) {
            if (i_out) *i_out = left;
            if (j_out) *j_out = right;
            return true;
        } else if (sum < target) {
            left++;
        } else {
            right--;
        }
    }
    return false;
}

size_t three_sum(int *arr, size_t n, int target, int (*triplets)[3], size_t max_triplets) {
    if (arr == NULL || n < 3 || triplets == NULL || max_triplets == 0) return 0;

    /* Sort the array first */
    qsort(arr, n, sizeof(int), compare_ints);

    size_t count = 0;

    for (size_t i = 0; i < n - 2 && count < max_triplets; i++) {
        /* Skip duplicates for first element */
        if (i > 0 && arr[i] == arr[i - 1]) continue;

        size_t left = i + 1;
        size_t right = n - 1;

        while (left < right && count < max_triplets) {
            int sum = arr[i] + arr[left] + arr[right];

            if (sum == target) {
                triplets[count][0] = arr[i];
                triplets[count][1] = arr[left];
                triplets[count][2] = arr[right];
                count++;

                /* Skip duplicates */
                while (left < right && arr[left] == arr[left + 1]) left++;
                while (left < right && arr[right] == arr[right - 1]) right--;

                left++;
                right--;
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
    }

    return count;
}

size_t remove_duplicates(int *arr, size_t n) {
    if (arr == NULL || n == 0) return 0;
    if (n == 1) return 1;

    size_t write = 1;

    for (size_t read = 1; read < n; read++) {
        if (arr[read] != arr[write - 1]) {
            arr[write++] = arr[read];
        }
    }

    return write;
}

void move_zeros(int *arr, size_t n) {
    if (arr == NULL || n == 0) return;

    size_t write = 0;

    /* Move all non-zero elements to front */
    for (size_t read = 0; read < n; read++) {
        if (arr[read] != 0) {
            arr[write++] = arr[read];
        }
    }

    /* Fill remaining with zeros */
    while (write < n) {
        arr[write++] = 0;
    }
}

int max_water_container(const int *heights, size_t n) {
    if (heights == NULL || n < 2) return 0;

    size_t left = 0;
    size_t right = n - 1;
    int max_area = 0;

    while (left < right) {
        int height = heights[left] < heights[right] ? heights[left] : heights[right];
        int area = height * (int)(right - left);

        if (area > max_area) {
            max_area = area;
        }

        if (heights[left] < heights[right]) {
            left++;
        } else {
            right--;
        }
    }

    return max_area;
}

static bool is_alnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

static char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

bool is_palindrome_str(const char *s) {
    if (s == NULL) return true;

    size_t left = 0;
    size_t right = 0;

    /* Find string length */
    while (s[right] != '\0') right++;
    if (right == 0) return true;
    right--;

    while (left < right) {
        /* Skip non-alphanumeric characters */
        while (left < right && !is_alnum(s[left])) left++;
        while (left < right && !is_alnum(s[right])) right--;

        if (left < right) {
            if (to_lower(s[left]) != to_lower(s[right])) {
                return false;
            }
            left++;
            right--;
        }
    }

    return true;
}

void reverse_array(int *arr, size_t n) {
    if (arr == NULL || n < 2) return;

    size_t left = 0;
    size_t right = n - 1;

    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

void sort_colors(int *arr, size_t n) {
    if (arr == NULL || n < 2) return;

    size_t low = 0;
    size_t mid = 0;
    size_t high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            int temp = arr[low];
            arr[low] = arr[mid];
            arr[mid] = temp;
            low++;
            mid++;
        } else if (arr[mid] == 1) {
            mid++;
        } else {  /* arr[mid] == 2 */
            int temp = arr[mid];
            arr[mid] = arr[high];
            arr[high] = temp;
            if (high == 0) break;
            high--;
        }
    }
}

size_t partition_array(int *arr, size_t n, int pivot) {
    if (arr == NULL || n == 0) return 0;

    size_t write = 0;

    for (size_t i = 0; i < n; i++) {
        if (arr[i] < pivot) {
            int temp = arr[i];
            arr[i] = arr[write];
            arr[write] = temp;
            write++;
        }
    }

    return write;
}

/* ============== Sliding Window Techniques ============== */

int max_sum_subarray_k(const int *arr, size_t n, size_t k) {
    if (arr == NULL || n == 0 || k == 0 || k > n) return 0;

    /* Calculate sum of first window */
    int window_sum = 0;
    for (size_t i = 0; i < k; i++) {
        window_sum += arr[i];
    }

    int max_sum = window_sum;

    /* Slide the window */
    for (size_t i = k; i < n; i++) {
        window_sum = window_sum + arr[i] - arr[i - k];
        if (window_sum > max_sum) {
            max_sum = window_sum;
        }
    }

    return max_sum;
}

size_t min_subarray_sum(const int *arr, size_t n, int target) {
    if (arr == NULL || n == 0) return 0;

    size_t min_len = n + 1;  /* Impossible value */
    int sum = 0;
    size_t left = 0;

    for (size_t right = 0; right < n; right++) {
        sum += arr[right];

        while (sum >= target) {
            size_t len = right - left + 1;
            if (len < min_len) {
                min_len = len;
            }
            sum -= arr[left++];
        }
    }

    return (min_len == n + 1) ? 0 : min_len;
}

size_t longest_unique_substring(const char *s) {
    if (s == NULL || s[0] == '\0') return 0;

    int char_index[256];
    for (int i = 0; i < 256; i++) {
        char_index[i] = -1;
    }

    size_t max_len = 0;
    size_t left = 0;

    for (size_t right = 0; s[right] != '\0'; right++) {
        unsigned char c = (unsigned char)s[right];

        if (char_index[c] >= (int)left) {
            left = (size_t)(char_index[c] + 1);
        }

        char_index[c] = (int)right;

        size_t len = right - left + 1;
        if (len > max_len) {
            max_len = len;
        }
    }

    return max_len;
}

size_t count_subarrays_sum(const int *arr, size_t n, int k) {
    if (arr == NULL || n == 0) return 0;

    /* Use prefix sum with hash map */
    /* Simple approach: O(n^2) */
    size_t count = 0;

    for (size_t i = 0; i < n; i++) {
        int sum = 0;
        for (size_t j = i; j < n; j++) {
            sum += arr[j];
            if (sum == k) {
                count++;
            }
        }
    }

    return count;
}

size_t sliding_window_max(const int *arr, size_t n, size_t k, int *result) {
    if (arr == NULL || result == NULL || n == 0 || k == 0 || k > n) return 0;

    /* Use a deque to store indices */
    size_t *deque = malloc(sizeof(size_t) * n);
    if (!deque) return 0;

    size_t front = 0, rear = 0;
    size_t result_count = 0;

    for (size_t i = 0; i < n; i++) {
        /* Remove elements outside current window */
        while (front < rear && i >= k && deque[front] <= i - k) {
            front++;
        }

        /* Remove smaller elements from back */
        while (front < rear && arr[deque[rear - 1]] <= arr[i]) {
            rear--;
        }

        /* Add current element */
        deque[rear++] = i;

        /* Output for windows that are complete */
        if (i >= k - 1) {
            result[result_count++] = arr[deque[front]];
        }
    }

    free(deque);
    return result_count;
}

size_t find_anagrams(const char *s, const char *p, size_t *indices, size_t max_indices) {
    if (s == NULL || p == NULL || indices == NULL || max_indices == 0) return 0;

    size_t s_len = 0, p_len = 0;
    while (s[s_len] != '\0') s_len++;
    while (p[p_len] != '\0') p_len++;

    if (s_len < p_len) return 0;

    /* Count characters in pattern */
    int p_count[26] = {0};
    int s_count[26] = {0};

    for (size_t i = 0; i < p_len; i++) {
        p_count[p[i] - 'a']++;
        s_count[s[i] - 'a']++;
    }

    size_t count = 0;

    /* Check first window */
    bool match = true;
    for (int i = 0; i < 26; i++) {
        if (p_count[i] != s_count[i]) {
            match = false;
            break;
        }
    }
    if (match && count < max_indices) {
        indices[count++] = 0;
    }

    /* Slide window */
    for (size_t i = p_len; i < s_len && count < max_indices; i++) {
        s_count[s[i] - 'a']++;
        s_count[s[i - p_len] - 'a']--;

        match = true;
        for (int j = 0; j < 26; j++) {
            if (p_count[j] != s_count[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            indices[count++] = i - p_len + 1;
        }
    }

    return count;
}

bool min_window_substring(const char *s, const char *t, size_t *start_out, size_t *len_out) {
    if (s == NULL || t == NULL) return false;

    size_t s_len = 0, t_len = 0;
    while (s[s_len] != '\0') s_len++;
    while (t[t_len] != '\0') t_len++;

    if (s_len < t_len || t_len == 0) return false;

    /* Count characters in target */
    int t_count[128] = {0};
    int window_count[128] = {0};

    for (size_t i = 0; i < t_len; i++) {
        t_count[(unsigned char)t[i]]++;
    }

    size_t required = 0;
    for (int i = 0; i < 128; i++) {
        if (t_count[i] > 0) required++;
    }

    size_t formed = 0;
    size_t left = 0;
    size_t min_len = s_len + 1;
    size_t min_start = 0;

    for (size_t right = 0; right < s_len; right++) {
        unsigned char c = (unsigned char)s[right];
        window_count[c]++;

        if (t_count[c] > 0 && window_count[c] == t_count[c]) {
            formed++;
        }

        /* Try to shrink window */
        while (left <= right && formed == required) {
            if (right - left + 1 < min_len) {
                min_len = right - left + 1;
                min_start = left;
            }

            unsigned char lc = (unsigned char)s[left];
            window_count[lc]--;
            if (t_count[lc] > 0 && window_count[lc] < t_count[lc]) {
                formed--;
            }
            left++;
        }
    }

    if (min_len == s_len + 1) return false;

    if (start_out) *start_out = min_start;
    if (len_out) *len_out = min_len;
    return true;
}
