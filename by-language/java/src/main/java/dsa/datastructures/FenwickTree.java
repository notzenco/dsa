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

    // ========== FenwickTree2D ==========

    /**
     * 2D Fenwick Tree (Binary Indexed Tree) for efficient 2D prefix sum queries
     * and point updates.
     */
    public static class FenwickTree2D {
        private final int[][] tree;
        private final int rows;
        private final int cols;

        /**
         * Creates a 2D Fenwick Tree of the given dimensions, initialized to all zeros.
         *
         * @param rows the number of rows
         * @param cols the number of columns
         * @throws IllegalArgumentException if rows or cols is not positive
         */
        public FenwickTree2D(int rows, int cols) {
            if (rows <= 0 || cols <= 0) {
                throw new IllegalArgumentException("Dimensions must be positive");
            }
            this.rows = rows;
            this.cols = cols;
            this.tree = new int[rows + 1][cols + 1];
        }

        /**
         * Adds delta to the element at the given 0-based position.
         *
         * @param row   the 0-based row index
         * @param col   the 0-based column index
         * @param delta the value to add
         * @throws IndexOutOfBoundsException if row or col is out of range
         */
        public void update(int row, int col, int delta) {
            validatePosition(row, col);
            int r = row + 1;
            while (r <= rows) {
                int c = col + 1;
                while (c <= cols) {
                    tree[r][c] += delta;
                    c += c & -c;
                }
                r += r & -r;
            }
        }

        /**
         * Returns the prefix sum from (0,0) to (row,col) inclusive.
         *
         * @param row the 0-based row index
         * @param col the 0-based column index
         * @return the prefix sum
         * @throws IndexOutOfBoundsException if row or col is out of range
         */
        public int query(int row, int col) {
            validatePosition(row, col);
            int sum = 0;
            int r = row + 1;
            while (r > 0) {
                int c = col + 1;
                while (c > 0) {
                    sum += tree[r][c];
                    c -= c & -c;
                }
                r -= r & -r;
            }
            return sum;
        }

        /**
         * Returns the sum of elements in the rectangle from (r1,c1) to (r2,c2) inclusive.
         *
         * @param r1 the top-left row (0-indexed)
         * @param c1 the top-left column (0-indexed)
         * @param r2 the bottom-right row (0-indexed)
         * @param c2 the bottom-right column (0-indexed)
         * @return the range sum
         * @throws IndexOutOfBoundsException if any index is out of range
         * @throws IllegalArgumentException  if r1 > r2 or c1 > c2
         */
        public int rangeQuery(int r1, int c1, int r2, int c2) {
            validatePosition(r1, c1);
            validatePosition(r2, c2);
            if (r1 > r2 || c1 > c2) {
                throw new IllegalArgumentException(
                        "Invalid range: (" + r1 + "," + c1 + ") to (" + r2 + "," + c2 + ")");
            }
            int sum = query(r2, c2);
            if (r1 > 0) {
                sum -= query(r1 - 1, c2);
            }
            if (c1 > 0) {
                sum -= query(r2, c1 - 1);
            }
            if (r1 > 0 && c1 > 0) {
                sum += query(r1 - 1, c1 - 1);
            }
            return sum;
        }

        private void validatePosition(int row, int col) {
            if (row < 0 || row >= rows) {
                throw new IndexOutOfBoundsException(
                        "Row " + row + " is out of range [0, " + (rows - 1) + "]");
            }
            if (col < 0 || col >= cols) {
                throw new IndexOutOfBoundsException(
                        "Col " + col + " is out of range [0, " + (cols - 1) + "]");
            }
        }
    }

    // ========== Static utilities ==========

    /**
     * Counts the number of inversions in the given array.
     * An inversion is a pair (i, j) where i &lt; j but arr[i] &gt; arr[j].
     * Uses a modified merge sort approach for O(n log n) performance.
     *
     * @param arr the input array
     * @return the number of inversions
     */
    public static int countInversions(int[] arr) {
        if (arr == null || arr.length <= 1) {
            return 0;
        }
        int[] temp = arr.clone();
        return mergeSortCount(temp, 0, temp.length - 1);
    }

    private static int mergeSortCount(int[] arr, int left, int right) {
        if (left >= right) {
            return 0;
        }
        int mid = left + (right - left) / 2;
        int count = 0;
        count += mergeSortCount(arr, left, mid);
        count += mergeSortCount(arr, mid + 1, right);
        count += mergeCount(arr, left, mid, right);
        return count;
    }

    private static int mergeCount(int[] arr, int left, int mid, int right) {
        int[] leftArr = new int[mid - left + 1];
        int[] rightArr = new int[right - mid];
        System.arraycopy(arr, left, leftArr, 0, leftArr.length);
        System.arraycopy(arr, mid + 1, rightArr, 0, rightArr.length);

        int i = 0, j = 0, k = left;
        int count = 0;
        while (i < leftArr.length && j < rightArr.length) {
            if (leftArr[i] <= rightArr[j]) {
                arr[k++] = leftArr[i++];
            } else {
                // All remaining elements in leftArr from i onwards
                // form inversions with rightArr[j]
                count += leftArr.length - i;
                arr[k++] = rightArr[j++];
            }
        }
        while (i < leftArr.length) {
            arr[k++] = leftArr[i++];
        }
        while (j < rightArr.length) {
            arr[k++] = rightArr[j++];
        }
        return count;
    }
}
