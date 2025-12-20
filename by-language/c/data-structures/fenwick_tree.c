/**
 * Fenwick Tree Implementation
 * See fenwick_tree.h for documentation
 */

#include "fenwick_tree.h"
#include <stdlib.h>
#include <string.h>

/* ============== Helper Functions ============== */

/* Get lowest set bit */
static inline size_t lowbit(size_t x) {
    return x & (~x + 1);  /* x & (-x) */
}

/* ============== Creation/Destruction ============== */

FenwickTree *fenwick_create(const int *arr, size_t n) {
    if (n == 0) return NULL;

    FenwickTree *ft = malloc(sizeof(FenwickTree));
    if (ft == NULL) return NULL;

    ft->size = n;
    /* Use 1-indexed internally, so allocate n+1 */
    ft->tree = calloc(n + 1, sizeof(int));

    if (ft->tree == NULL) {
        free(ft);
        return NULL;
    }

    /* Build in O(n) using the optimization */
    if (arr != NULL) {
        /* Copy array to tree (shifted by 1) */
        for (size_t i = 0; i < n; i++) {
            ft->tree[i + 1] = arr[i];
        }

        /* Build BIT in O(n) */
        for (size_t i = 1; i <= n; i++) {
            size_t parent = i + lowbit(i);
            if (parent <= n) {
                ft->tree[parent] += ft->tree[i];
            }
        }
    }

    return ft;
}

FenwickTree *fenwick_create_empty(size_t n) {
    return fenwick_create(NULL, n);
}

void fenwick_destroy(FenwickTree *tree) {
    if (tree == NULL) return;
    free(tree->tree);
    free(tree);
}

/* ============== Core Operations ============== */

void fenwick_add(FenwickTree *tree, size_t index, int delta) {
    if (tree == NULL || index >= tree->size) return;

    /* Convert to 1-indexed */
    index++;

    while (index <= tree->size) {
        tree->tree[index] += delta;
        index += lowbit(index);
    }
}

void fenwick_update(FenwickTree *tree, size_t index, int value) {
    if (tree == NULL || index >= tree->size) return;

    int current = fenwick_get(tree, index);
    fenwick_add(tree, index, value - current);
}

int fenwick_prefix_sum(const FenwickTree *tree, size_t index) {
    if (tree == NULL || index >= tree->size) return 0;

    /* Convert to 1-indexed */
    index++;

    int sum = 0;
    while (index > 0) {
        sum += tree->tree[index];
        index -= lowbit(index);
    }
    return sum;
}

int fenwick_range_sum(const FenwickTree *tree, size_t left, size_t right) {
    if (tree == NULL || left > right || right >= tree->size) return 0;

    int right_sum = fenwick_prefix_sum(tree, right);
    int left_sum = (left > 0) ? fenwick_prefix_sum(tree, left - 1) : 0;

    return right_sum - left_sum;
}

int fenwick_get(const FenwickTree *tree, size_t index) {
    if (tree == NULL || index >= tree->size) return 0;
    return fenwick_range_sum(tree, index, index);
}

/* ============== Utility ============== */

size_t fenwick_size(const FenwickTree *tree) {
    return tree ? tree->size : 0;
}

size_t fenwick_lower_bound(const FenwickTree *tree, int value) {
    if (tree == NULL || tree->size == 0) return 0;

    size_t pos = 0;
    int sum = 0;

    /* Find highest bit */
    size_t bit = 1;
    while (bit <= tree->size) bit <<= 1;
    bit >>= 1;

    while (bit > 0) {
        size_t next = pos + bit;
        if (next <= tree->size && sum + tree->tree[next] < value) {
            pos = next;
            sum += tree->tree[next];
        }
        bit >>= 1;
    }

    return pos;  /* Return 0-indexed result */
}
