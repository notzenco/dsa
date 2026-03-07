package dsa.datastructures;

/**
 * Fenwick Tree (Binary Indexed Tree) for efficient prefix sum queries
 * and point updates in O(log n) time.
 */
public class FenwickTree {

    private final int[] tree;
    private final int n;

    /**
     * Creates a Fenwick Tree of the given size, initialized to all zeros.
     *
     * @param size the number of elements
     * @throws IllegalArgumentException if size is not positive
     */
    public FenwickTree(int size) {
        if (size <= 0) {
            throw new IllegalArgumentException("Size must be positive");
        }
        this.n = size;
        this.tree = new int[n + 1]; // 1-indexed internally
    }

    /**
     * Creates a Fenwick Tree initialized with the given array values.
     *
     * @param array the input array
     * @throws IllegalArgumentException if array is null or empty
     */
    public FenwickTree(int[] array) {
        if (array == null || array.length == 0) {
            throw new IllegalArgumentException("Array must not be null or empty");
        }
        this.n = array.length;
        this.tree = new int[n + 1];
        // Build in O(n) by copying then propagating
        for (int i = 0; i < n; i++) {
            tree[i + 1] = array[i];
        }
        for (int i = 1; i <= n; i++) {
            int parent = i + (i & -i);
            if (parent <= n) {
                tree[parent] += tree[i];
            }
        }
    }

    /**
     * Adds delta to the element at the given 0-based index.
     *
     * @param index the 0-based index to update
     * @param delta the value to add
     * @throws IndexOutOfBoundsException if index is out of range
     */
    public void update(int index, int delta) {
        validateIndex(index);
        int i = index + 1; // convert to 1-indexed
        while (i <= n) {
            tree[i] += delta;
            i += i & -i;
        }
    }

    /**
     * Returns the prefix sum from index 0 to the given index (inclusive).
     *
     * @param index the 0-based index (inclusive)
     * @return the prefix sum from 0 to index
     * @throws IndexOutOfBoundsException if index is out of range
     */
    public int query(int index) {
        validateIndex(index);
        int sum = 0;
        int i = index + 1; // convert to 1-indexed
        while (i > 0) {
            sum += tree[i];
            i -= i & -i;
        }
        return sum;
    }

    /**
     * Returns the sum of elements in the range [left, right] (inclusive).
     *
     * @param left  the left bound (0-indexed)
     * @param right the right bound (0-indexed, inclusive)
     * @return the range sum
     * @throws IndexOutOfBoundsException if indices are out of range
     * @throws IllegalArgumentException  if left > right
     */
    public int rangeQuery(int left, int right) {
        if (left < 0 || right >= n) {
            throw new IndexOutOfBoundsException(
                    "Range [" + left + ", " + right + "] is out of bounds [0, " + (n - 1) + "]");
        }
        if (left > right) {
            throw new IllegalArgumentException(
                    "Left index " + left + " must not exceed right index " + right);
        }
        if (left == 0) {
            return query(right);
        }
        return query(right) - query(left - 1);
    }

    /**
     * Returns the number of elements in the Fenwick Tree.
     *
     * @return the size
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
}
