package dsa.datastructures;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class FenwickTreeTest {

    @Test
    void constructorWithSize() {
        FenwickTree ft = new FenwickTree(5);
        assertEquals(5, ft.size());
        assertEquals(0, ft.query(0));
        assertEquals(0, ft.query(4));
    }

    @Test
    void constructorWithArray() {
        int[] arr = {1, 2, 3, 4, 5};
        FenwickTree ft = new FenwickTree(arr);
        assertEquals(5, ft.size());
        assertEquals(1, ft.query(0));           // prefix sum [0..0]
        assertEquals(3, ft.query(1));           // prefix sum [0..1]
        assertEquals(6, ft.query(2));           // prefix sum [0..2]
        assertEquals(10, ft.query(3));          // prefix sum [0..3]
        assertEquals(15, ft.query(4));          // prefix sum [0..4]
    }

    @Test
    void constructorRejectsInvalid() {
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree(0));
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree(-1));
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree((int[]) null));
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree(new int[]{}));
    }

    @Test
    void updateAndQuery() {
        FenwickTree ft = new FenwickTree(5);
        ft.update(0, 3);
        ft.update(1, 5);
        ft.update(2, 7);
        ft.update(3, 1);
        ft.update(4, 4);
        assertEquals(3, ft.query(0));
        assertEquals(8, ft.query(1));
        assertEquals(15, ft.query(2));
        assertEquals(16, ft.query(3));
        assertEquals(20, ft.query(4));
    }

    @Test
    void updateAddsToExistingValue() {
        FenwickTree ft = new FenwickTree(new int[]{10, 20, 30});
        ft.update(1, 5); // 20 + 5 = 25
        assertEquals(10, ft.query(0));
        assertEquals(35, ft.query(1));     // 10 + 25
        assertEquals(65, ft.query(2));     // 10 + 25 + 30
    }

    @Test
    void updateWithNegativeDelta() {
        FenwickTree ft = new FenwickTree(new int[]{10, 20, 30});
        ft.update(1, -5); // 20 - 5 = 15
        assertEquals(25, ft.query(1));     // 10 + 15
    }

    @Test
    void queryOutOfBoundsThrows() {
        FenwickTree ft = new FenwickTree(5);
        assertThrows(IndexOutOfBoundsException.class, () -> ft.query(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.query(5));
    }

    @Test
    void updateOutOfBoundsThrows() {
        FenwickTree ft = new FenwickTree(5);
        assertThrows(IndexOutOfBoundsException.class, () -> ft.update(-1, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.update(5, 1));
    }

    @Test
    void rangeQueryBasic() {
        int[] arr = {1, 2, 3, 4, 5};
        FenwickTree ft = new FenwickTree(arr);
        assertEquals(1, ft.rangeQuery(0, 0));
        assertEquals(3, ft.rangeQuery(0, 1));
        assertEquals(6, ft.rangeQuery(0, 2));
        assertEquals(15, ft.rangeQuery(0, 4));
    }

    @Test
    void rangeQuerySubranges() {
        int[] arr = {1, 2, 3, 4, 5};
        FenwickTree ft = new FenwickTree(arr);
        assertEquals(5, ft.rangeQuery(1, 2));    // 2 + 3
        assertEquals(12, ft.rangeQuery(2, 4));   // 3 + 4 + 5
        assertEquals(9, ft.rangeQuery(3, 4));    // 4 + 5
        assertEquals(9, ft.rangeQuery(1, 3));    // 2 + 3 + 4
    }

    @Test
    void rangeQuerySingleElement() {
        int[] arr = {10, 20, 30, 40, 50};
        FenwickTree ft = new FenwickTree(arr);
        assertEquals(10, ft.rangeQuery(0, 0));
        assertEquals(20, ft.rangeQuery(1, 1));
        assertEquals(30, ft.rangeQuery(2, 2));
        assertEquals(40, ft.rangeQuery(3, 3));
        assertEquals(50, ft.rangeQuery(4, 4));
    }

    @Test
    void rangeQueryOutOfBoundsThrows() {
        FenwickTree ft = new FenwickTree(5);
        assertThrows(IndexOutOfBoundsException.class, () -> ft.rangeQuery(-1, 2));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.rangeQuery(0, 5));
    }

    @Test
    void rangeQueryInvalidRangeThrows() {
        FenwickTree ft = new FenwickTree(5);
        assertThrows(IllegalArgumentException.class, () -> ft.rangeQuery(3, 2));
    }

    @Test
    void rangeQueryAfterUpdates() {
        int[] arr = {1, 2, 3, 4, 5};
        FenwickTree ft = new FenwickTree(arr);
        ft.update(2, 10); // arr becomes [1, 2, 13, 4, 5]
        assertEquals(19, ft.rangeQuery(1, 3));   // 2 + 13 + 4
        assertEquals(25, ft.rangeQuery(0, 4));   // 1 + 2 + 13 + 4 + 5
    }

    @Test
    void singleElementTree() {
        FenwickTree ft = new FenwickTree(new int[]{42});
        assertEquals(1, ft.size());
        assertEquals(42, ft.query(0));
        assertEquals(42, ft.rangeQuery(0, 0));
        ft.update(0, 8);
        assertEquals(50, ft.query(0));
    }

    @Test
    void largeArray() {
        int n = 1000;
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) {
            arr[i] = i + 1;
        }
        FenwickTree ft = new FenwickTree(arr);
        // Sum of 1..1000 = 500500
        assertEquals(500500, ft.query(n - 1));
        // Sum of 100..200 (1-indexed values at 0-indexed 99..199)
        // = sum(1..200) - sum(1..99) = 20100 - 4950 = 15150
        assertEquals(15150, ft.rangeQuery(99, 199));
    }

    // ======================== FenwickTree2D ========================

    @Test
    void fenwickTree2DConstructor() {
        var ft = new FenwickTree.FenwickTree2D(3, 4);
        assertEquals(0, ft.query(0, 0));
        assertEquals(0, ft.query(2, 3));
    }

    @Test
    void fenwickTree2DConstructorRejectsInvalid() {
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree.FenwickTree2D(0, 5));
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree.FenwickTree2D(5, 0));
        assertThrows(IllegalArgumentException.class, () -> new FenwickTree.FenwickTree2D(-1, -1));
    }

    @Test
    void fenwickTree2DUpdateAndQuery() {
        var ft = new FenwickTree.FenwickTree2D(3, 3);
        ft.update(0, 0, 1);
        ft.update(0, 1, 2);
        ft.update(1, 0, 3);
        ft.update(1, 1, 4);
        assertEquals(1, ft.query(0, 0));
        assertEquals(3, ft.query(0, 1)); // 1 + 2
        assertEquals(4, ft.query(1, 0)); // 1 + 3
        assertEquals(10, ft.query(1, 1)); // 1 + 2 + 3 + 4
    }

    @Test
    void fenwickTree2DRangeQuery() {
        var ft = new FenwickTree.FenwickTree2D(3, 3);
        // Fill a 3x3 grid:
        // 1 2 3
        // 4 5 6
        // 7 8 9
        ft.update(0, 0, 1); ft.update(0, 1, 2); ft.update(0, 2, 3);
        ft.update(1, 0, 4); ft.update(1, 1, 5); ft.update(1, 2, 6);
        ft.update(2, 0, 7); ft.update(2, 1, 8); ft.update(2, 2, 9);

        // Full grid sum
        assertEquals(45, ft.rangeQuery(0, 0, 2, 2));
        // Single element
        assertEquals(5, ft.rangeQuery(1, 1, 1, 1));
        // Sub-rectangle: rows 1-2, cols 1-2 -> 5+6+8+9 = 28
        assertEquals(28, ft.rangeQuery(1, 1, 2, 2));
        // First row: 1+2+3 = 6
        assertEquals(6, ft.rangeQuery(0, 0, 0, 2));
        // First column: 1+4+7 = 12
        assertEquals(12, ft.rangeQuery(0, 0, 2, 0));
    }

    @Test
    void fenwickTree2DRangeQueryInvalidThrows() {
        var ft = new FenwickTree.FenwickTree2D(3, 3);
        assertThrows(IllegalArgumentException.class, () -> ft.rangeQuery(2, 0, 1, 0));
        assertThrows(IllegalArgumentException.class, () -> ft.rangeQuery(0, 2, 0, 1));
    }

    @Test
    void fenwickTree2DOutOfBoundsThrows() {
        var ft = new FenwickTree.FenwickTree2D(3, 3);
        assertThrows(IndexOutOfBoundsException.class, () -> ft.update(-1, 0, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.update(0, 3, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.query(3, 0));
        assertThrows(IndexOutOfBoundsException.class, () -> ft.query(0, -1));
    }

    // ======================== countInversions ========================

    @Test
    void countInversionsBasic() {
        // Pairs: (5,4),(5,3),(5,2),(5,1),(4,3),(4,2),(4,1),(3,2),(3,1),(2,1)
        assertEquals(10, FenwickTree.countInversions(new int[]{5, 4, 3, 2, 1}));
    }

    @Test
    void countInversionsSorted() {
        assertEquals(0, FenwickTree.countInversions(new int[]{1, 2, 3, 4, 5}));
    }

    @Test
    void countInversionsSingle() {
        assertEquals(0, FenwickTree.countInversions(new int[]{42}));
    }

    @Test
    void countInversionsEmpty() {
        assertEquals(0, FenwickTree.countInversions(new int[]{}));
    }

    @Test
    void countInversionsNull() {
        assertEquals(0, FenwickTree.countInversions(null));
    }

    @Test
    void countInversionsMixed() {
        // [2, 4, 1, 3, 5] -> inversions: (2,1), (4,1), (4,3) = 3
        assertEquals(3, FenwickTree.countInversions(new int[]{2, 4, 1, 3, 5}));
    }

    @Test
    void countInversionsDoesNotModifyOriginal() {
        int[] arr = {3, 1, 2};
        FenwickTree.countInversions(arr);
        assertArrayEquals(new int[]{3, 1, 2}, arr);
    }
}
