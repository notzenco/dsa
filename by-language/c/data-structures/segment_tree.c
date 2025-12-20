/**
 * Segment Tree Implementation
 * See segment_tree.h for documentation
 */

#include "segment_tree.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ============== Helper Functions ============== */

static int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static int combine(SegmentTreeType type, int a, int b) {
    switch (type) {
        case SEG_SUM: return a + b;
        case SEG_MIN: return (a < b) ? a : b;
        case SEG_MAX: return (a > b) ? a : b;
        case SEG_GCD: return gcd(a, b);
        default: return a + b;
    }
}

static int get_identity(SegmentTreeType type) {
    switch (type) {
        case SEG_SUM: return 0;
        case SEG_MIN: return INT_MAX;
        case SEG_MAX: return INT_MIN;
        case SEG_GCD: return 0;
        default: return 0;
    }
}

/* ============== Build ============== */

static void build(SegmentTree *st, const int *arr, size_t node,
                  size_t start, size_t end) {
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }

    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    build(st, arr, left_child, start, mid);
    build(st, arr, right_child, mid + 1, end);

    st->tree[node] = combine(st->type, st->tree[left_child], st->tree[right_child]);
}

/* ============== Creation/Destruction ============== */

SegmentTree *segtree_create(const int *arr, size_t n, SegmentTreeType type) {
    if (arr == NULL || n == 0) return NULL;

    SegmentTree *st = malloc(sizeof(SegmentTree));
    if (st == NULL) return NULL;

    st->size = n;
    st->type = type;
    st->identity = get_identity(type);

    /* Allocate tree array (4*n is safe upper bound) */
    st->tree = malloc(4 * n * sizeof(int));
    st->lazy = calloc(4 * n, sizeof(int));

    if (st->tree == NULL || st->lazy == NULL) {
        free(st->tree);
        free(st->lazy);
        free(st);
        return NULL;
    }

    /* Initialize with identity */
    for (size_t i = 0; i < 4 * n; i++) {
        st->tree[i] = st->identity;
    }

    build(st, arr, 0, 0, n - 1);
    return st;
}

SegmentTree *segtree_create_empty(size_t n, SegmentTreeType type) {
    if (n == 0) return NULL;

    SegmentTree *st = malloc(sizeof(SegmentTree));
    if (st == NULL) return NULL;

    st->size = n;
    st->type = type;
    st->identity = get_identity(type);

    st->tree = malloc(4 * n * sizeof(int));
    st->lazy = calloc(4 * n, sizeof(int));

    if (st->tree == NULL || st->lazy == NULL) {
        free(st->tree);
        free(st->lazy);
        free(st);
        return NULL;
    }

    for (size_t i = 0; i < 4 * n; i++) {
        st->tree[i] = st->identity;
    }

    return st;
}

void segtree_destroy(SegmentTree *tree) {
    if (tree == NULL) return;
    free(tree->tree);
    free(tree->lazy);
    free(tree);
}

/* ============== Lazy Propagation ============== */

static void push_down(SegmentTree *st, size_t node, size_t start, size_t end) {
    if (st->lazy[node] != 0) {
        size_t mid = start + (end - start) / 2;
        size_t left_child = 2 * node + 1;
        size_t right_child = 2 * node + 2;

        if (st->type == SEG_SUM) {
            st->tree[left_child] += st->lazy[node] * (int)(mid - start + 1);
            st->tree[right_child] += st->lazy[node] * (int)(end - mid);
        } else {
            st->tree[left_child] += st->lazy[node];
            st->tree[right_child] += st->lazy[node];
        }

        st->lazy[left_child] += st->lazy[node];
        st->lazy[right_child] += st->lazy[node];
        st->lazy[node] = 0;
    }
}

/* ============== Query ============== */

static int query_helper(SegmentTree *st, size_t node, size_t start, size_t end,
                        size_t left, size_t right) {
    if (right < start || end < left) {
        return st->identity;
    }

    if (left <= start && end <= right) {
        return st->tree[node];
    }

    push_down(st, node, start, end);

    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    int left_result = query_helper(st, left_child, start, mid, left, right);
    int right_result = query_helper(st, right_child, mid + 1, end, left, right);

    return combine(st->type, left_result, right_result);
}

int segtree_query(SegmentTree *tree, size_t left, size_t right) {
    if (tree == NULL || left > right || right >= tree->size) {
        return 0;
    }
    return query_helper(tree, 0, 0, tree->size - 1, left, right);
}

/* ============== Point Update ============== */

static void update_helper(SegmentTree *st, size_t node, size_t start, size_t end,
                          size_t index, int value, bool set) {
    if (start == end) {
        if (set) {
            st->tree[node] = value;
        } else {
            st->tree[node] += value;
        }
        return;
    }

    push_down(st, node, start, end);

    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    if (index <= mid) {
        update_helper(st, left_child, start, mid, index, value, set);
    } else {
        update_helper(st, right_child, mid + 1, end, index, value, set);
    }

    st->tree[node] = combine(st->type, st->tree[left_child], st->tree[right_child]);
}

void segtree_update(SegmentTree *tree, size_t index, int value) {
    if (tree == NULL || index >= tree->size) return;
    update_helper(tree, 0, 0, tree->size - 1, index, value, true);
}

void segtree_add(SegmentTree *tree, size_t index, int delta) {
    if (tree == NULL || index >= tree->size) return;
    update_helper(tree, 0, 0, tree->size - 1, index, delta, false);
}

/* ============== Range Update ============== */

static void range_update_helper(SegmentTree *st, size_t node, size_t start, size_t end,
                                 size_t left, size_t right, int delta) {
    if (right < start || end < left) {
        return;
    }

    if (left <= start && end <= right) {
        if (st->type == SEG_SUM) {
            st->tree[node] += delta * (int)(end - start + 1);
        } else {
            st->tree[node] += delta;
        }
        st->lazy[node] += delta;
        return;
    }

    push_down(st, node, start, end);

    size_t mid = start + (end - start) / 2;
    size_t left_child = 2 * node + 1;
    size_t right_child = 2 * node + 2;

    range_update_helper(st, left_child, start, mid, left, right, delta);
    range_update_helper(st, right_child, mid + 1, end, left, right, delta);

    st->tree[node] = combine(st->type, st->tree[left_child], st->tree[right_child]);
}

void segtree_range_update(SegmentTree *tree, size_t left, size_t right, int delta) {
    if (tree == NULL || left > right || right >= tree->size) return;
    range_update_helper(tree, 0, 0, tree->size - 1, left, right, delta);
}

/* ============== Utility ============== */

size_t segtree_size(const SegmentTree *tree) {
    return tree ? tree->size : 0;
}

int segtree_get(SegmentTree *tree, size_t index) {
    if (tree == NULL || index >= tree->size) return 0;
    return segtree_query(tree, index, index);
}
