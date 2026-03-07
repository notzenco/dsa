package dsa.datastructures;

/**
 * Segment Tree supporting range sum queries, point updates, and range updates
 * with lazy propagation.
 */
public class SegmentTree {

    private final int[] tree;
    private final int[] lazy;
    private final int n;

    /**
     * Builds a segment tree from the given array.
     *
     * @param array the input array
     * @throws IllegalArgumentException if array is null or empty
     */
    public SegmentTree(int[] array) {
        if (array == null || array.length == 0) {
            throw new IllegalArgumentException("Array must not be null or empty");
        }
        this.n = array.length;
        this.tree = new int[4 * n];
        this.lazy = new int[4 * n];
        build(array, 1, 0, n - 1);
    }

    private void build(int[] array, int node, int start, int end) {
        if (start == end) {
            tree[node] = array[start];
        } else {
            int mid = start + (end - start) / 2;
            build(array, 2 * node, start, mid);
            build(array, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    /**
     * Queries the sum of elements in the range [left, right] (inclusive).
     *
     * @param left  the left bound (0-indexed)
     * @param right the right bound (0-indexed, inclusive)
     * @return the sum of elements in [left, right]
     * @throws IndexOutOfBoundsException if indices are out of range
     * @throws IllegalArgumentException  if left > right
     */
    public int query(int left, int right) {
        validateRange(left, right);
        return query(1, 0, n - 1, left, right);
    }

    private int query(int node, int start, int end, int l, int r) {
        pushDown(node, start, end);
        if (r < start || end < l) {
            return 0;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = start + (end - start) / 2;
        return query(2 * node, start, mid, l, r)
                + query(2 * node + 1, mid + 1, end, l, r);
    }

    /**
     * Sets the value at the given index to the new value (point update).
     *
     * @param index the 0-indexed position to update
     * @param value the new value to set
     * @throws IndexOutOfBoundsException if index is out of range
     */
    public void update(int index, int value) {
        validateIndex(index);
        update(1, 0, n - 1, index, value);
    }

    private void update(int node, int start, int end, int index, int value) {
        pushDown(node, start, end);
        if (start == end) {
            tree[node] = value;
            return;
        }
        int mid = start + (end - start) / 2;
        if (index <= mid) {
            update(2 * node, start, mid, index, value);
            pushDown(2 * node + 1, mid + 1, end);
        } else {
            pushDown(2 * node, start, mid);
            update(2 * node + 1, mid + 1, end, index, value);
        }
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    /**
     * Adds the given value to every element in the range [left, right]
     * using lazy propagation.
     *
     * @param left  the left bound (0-indexed)
     * @param right the right bound (0-indexed, inclusive)
     * @param value the value to add to each element in the range
     * @throws IndexOutOfBoundsException if indices are out of range
     * @throws IllegalArgumentException  if left > right
     */
    public void rangeUpdate(int left, int right, int value) {
        validateRange(left, right);
        rangeUpdate(1, 0, n - 1, left, right, value);
    }

    private void rangeUpdate(int node, int start, int end, int l, int r, int value) {
        pushDown(node, start, end);
        if (r < start || end < l) {
            return;
        }
        if (l <= start && end <= r) {
            lazy[node] += value;
            pushDown(node, start, end);
            return;
        }
        int mid = start + (end - start) / 2;
        rangeUpdate(2 * node, start, mid, l, r, value);
        rangeUpdate(2 * node + 1, mid + 1, end, l, r, value);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    private void pushDown(int node, int start, int end) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node] * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    /**
     * Returns the number of elements in the original array.
     *
     * @return the size of the underlying array
     */
    public int size() {
        return n;
    }

    private void validateIndex(int index) {
        if (index < 0 || index >= n) {
            throw new IndexOutOfBoundsException(
                    "Index " + index + " is out of range [0, " + (n - 1) + "]");
        }
    }

    private void validateRange(int left, int right) {
        if (left < 0 || right >= n) {
            throw new IndexOutOfBoundsException(
                    "Range [" + left + ", " + right + "] is out of bounds [0, " + (n - 1) + "]");
        }
        if (left > right) {
            throw new IllegalArgumentException(
                    "Left index " + left + " must not exceed right index " + right);
        }
    }
}
