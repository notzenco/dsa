package dsa.datastructures;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class SegmentTreeTest {

    @Test
    void constructorAndSize() {
        SegmentTree st = new SegmentTree(new int[]{1, 2, 3, 4, 5});
        assertEquals(5, st.size());
    }

    @Test
    void constructorRejectsInvalid() {
        assertThrows(IllegalArgumentException.class, () -> new SegmentTree(null));
        assertThrows(IllegalArgumentException.class, () -> new SegmentTree(new int[]{}));
    }

    @Test
    void queryFullRange() {
        int[] arr = {1, 3, 5, 7, 9, 11};
        SegmentTree st = new SegmentTree(arr);
        assertEquals(36, st.query(0, 5));
    }

    @Test
    void querySingleElement() {
        int[] arr = {10, 20, 30, 40, 50};
        SegmentTree st = new SegmentTree(arr);
        assertEquals(10, st.query(0, 0));
        assertEquals(30, st.query(2, 2));
        assertEquals(50, st.query(4, 4));
    }

    @Test
    void querySubranges() {
        int[] arr = {1, 2, 3, 4, 5};
        SegmentTree st = new SegmentTree(arr);
        assertEquals(3, st.query(0, 1));    // 1 + 2
        assertEquals(6, st.query(0, 2));    // 1 + 2 + 3
        assertEquals(12, st.query(2, 4));   // 3 + 4 + 5
        assertEquals(9, st.query(3, 4));    // 4 + 5
        assertEquals(5, st.query(1, 2));    // 2 + 3
    }

    @Test
    void queryOutOfBoundsThrows() {
        SegmentTree st = new SegmentTree(new int[]{1, 2, 3});
        assertThrows(IndexOutOfBoundsException.class, () -> st.query(-1, 2));
        assertThrows(IndexOutOfBoundsException.class, () -> st.query(0, 3));
    }

    @Test
    void queryInvalidRangeThrows() {
        SegmentTree st = new SegmentTree(new int[]{1, 2, 3});
        assertThrows(IllegalArgumentException.class, () -> st.query(2, 1));
    }

    @Test
    void pointUpdate() {
        int[] arr = {1, 2, 3, 4, 5};
        SegmentTree st = new SegmentTree(arr);
        assertEquals(15, st.query(0, 4));
        st.update(2, 10); // change 3 -> 10
        assertEquals(22, st.query(0, 4));   // 1 + 2 + 10 + 4 + 5
        assertEquals(10, st.query(2, 2));
        assertEquals(13, st.query(0, 2));   // 1 + 2 + 10
    }

    @Test
    void pointUpdateOutOfBoundsThrows() {
        SegmentTree st = new SegmentTree(new int[]{1, 2, 3});
        assertThrows(IndexOutOfBoundsException.class, () -> st.update(-1, 5));
        assertThrows(IndexOutOfBoundsException.class, () -> st.update(3, 5));
    }

    @Test
    void multiplePointUpdates() {
        int[] arr = {0, 0, 0, 0, 0};
        SegmentTree st = new SegmentTree(arr);
        st.update(0, 5);
        st.update(2, 3);
        st.update(4, 7);
        assertEquals(15, st.query(0, 4));
        assertEquals(8, st.query(0, 2));
        assertEquals(10, st.query(2, 4));
    }

    @Test
    void rangeUpdateAddsToRange() {
        int[] arr = {1, 2, 3, 4, 5};
        SegmentTree st = new SegmentTree(arr);
        st.rangeUpdate(1, 3, 10); // add 10 to indices 1,2,3
        // Array becomes [1, 12, 13, 14, 5]
        assertEquals(1, st.query(0, 0));
        assertEquals(12, st.query(1, 1));
        assertEquals(13, st.query(2, 2));
        assertEquals(14, st.query(3, 3));
        assertEquals(5, st.query(4, 4));
        assertEquals(45, st.query(0, 4));
    }

    @Test
    void rangeUpdateFullRange() {
        int[] arr = {1, 1, 1, 1};
        SegmentTree st = new SegmentTree(arr);
        st.rangeUpdate(0, 3, 5);
        assertEquals(24, st.query(0, 3)); // (1+5)*4 = 24
    }

    @Test
    void rangeUpdateSingleElement() {
        int[] arr = {10, 20, 30};
        SegmentTree st = new SegmentTree(arr);
        st.rangeUpdate(1, 1, 100);
        assertEquals(10, st.query(0, 0));
        assertEquals(120, st.query(1, 1));
        assertEquals(30, st.query(2, 2));
    }

    @Test
    void multipleRangeUpdates() {
        int[] arr = {0, 0, 0, 0, 0};
        SegmentTree st = new SegmentTree(arr);
        st.rangeUpdate(0, 4, 1);  // [1,1,1,1,1]
        st.rangeUpdate(1, 3, 2);  // [1,3,3,3,1]
        st.rangeUpdate(2, 2, 5);  // [1,3,8,3,1]
        assertEquals(16, st.query(0, 4));
        assertEquals(8, st.query(2, 2));
        assertEquals(14, st.query(1, 3));
    }

    @Test
    void rangeUpdateThenPointUpdate() {
        int[] arr = {1, 2, 3};
        SegmentTree st = new SegmentTree(arr);
        st.rangeUpdate(0, 2, 10); // [11, 12, 13]
        st.update(1, 0);          // [11, 0, 13]
        assertEquals(11, st.query(0, 0));
        assertEquals(0, st.query(1, 1));
        assertEquals(13, st.query(2, 2));
        assertEquals(24, st.query(0, 2));
    }

    @Test
    void singleElementTree() {
        SegmentTree st = new SegmentTree(new int[]{42});
        assertEquals(1, st.size());
        assertEquals(42, st.query(0, 0));
        st.update(0, 100);
        assertEquals(100, st.query(0, 0));
        st.rangeUpdate(0, 0, 5);
        assertEquals(105, st.query(0, 0));
    }

    @Test
    void negativeValues() {
        int[] arr = {-3, -1, 4, 1, -5};
        SegmentTree st = new SegmentTree(arr);
        assertEquals(-4, st.query(0, 4));
        assertEquals(-4, st.query(0, 1));
        assertEquals(3, st.query(1, 2));    // -1 + 4
    }
}
