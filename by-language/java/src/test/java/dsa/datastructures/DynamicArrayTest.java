package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DynamicArrayTest {

    // ---- append ----

    @Test
    void testAppend() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        arr.append(2);
        arr.append(3);
        assertEquals(List.of(1, 2, 3), arr.toList());
        assertEquals(3, arr.size());
    }

    // ---- get ----

    @Test
    void testGet() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(10);
        arr.append(20);
        arr.append(30);
        assertEquals(10, arr.get(0));
        assertEquals(20, arr.get(1));
        assertEquals(30, arr.get(2));
    }

    @Test
    void testGetOutOfBounds() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        assertThrows(IndexOutOfBoundsException.class, () -> arr.get(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> arr.get(1));
    }

    // ---- set ----

    @Test
    void testSet() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        arr.append(2);
        arr.set(0, 99);
        assertEquals(99, arr.get(0));
    }

    @Test
    void testSetOutOfBounds() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        assertThrows(IndexOutOfBoundsException.class, () -> arr.set(0, 1));
    }

    // ---- insert ----

    @Test
    void testInsertAtBeginning() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(2);
        arr.append(3);
        arr.insert(0, 1);
        assertEquals(List.of(1, 2, 3), arr.toList());
    }

    @Test
    void testInsertAtEnd() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        arr.append(2);
        arr.insert(2, 3);
        assertEquals(List.of(1, 2, 3), arr.toList());
    }

    @Test
    void testInsertAtMiddle() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        arr.append(3);
        arr.insert(1, 2);
        assertEquals(List.of(1, 2, 3), arr.toList());
    }

    @Test
    void testInsertOutOfBounds() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        assertThrows(IndexOutOfBoundsException.class, () -> arr.insert(-1, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> arr.insert(1, 1));
    }

    // ---- pop ----

    @Test
    void testPop() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(10, 20, 30, 40));
        assertEquals(20, arr.pop(1));
        assertEquals(List.of(10, 30, 40), arr.toList());
    }

    @Test
    void testPopFirst() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(10, 20, 30));
        assertEquals(10, arr.pop(0));
        assertEquals(List.of(20, 30), arr.toList());
    }

    @Test
    void testPopLast() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(10, 20, 30));
        assertEquals(30, arr.pop(2));
        assertEquals(List.of(10, 20), arr.toList());
    }

    @Test
    void testPopOutOfBounds() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(1, 2));
        assertThrows(IndexOutOfBoundsException.class, () -> arr.pop(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> arr.pop(2));
    }

    // ---- remove ----

    @Test
    void testRemove() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(1, 2, 3));
        assertTrue(arr.remove(2));
        assertEquals(List.of(1, 3), arr.toList());
    }

    @Test
    void testRemoveNotFound() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(1, 2, 3));
        assertFalse(arr.remove(99));
    }

    // ---- find ----

    @Test
    void testFind() {
        DynamicArray<String> arr = DynamicArray.fromList(List.of("a", "b", "c"));
        assertEquals(0, arr.find("a"));
        assertEquals(1, arr.find("b"));
        assertEquals(2, arr.find("c"));
        assertEquals(-1, arr.find("z"));
    }

    // ---- clear ----

    @Test
    void testClear() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(1, 2, 3));
        arr.clear();
        assertTrue(arr.isEmpty());
        assertEquals(0, arr.size());
        assertEquals(DynamicArray.DEFAULT_CAPACITY, arr.capacity());
    }

    // ---- reverse ----

    @Test
    void testReverse() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(1, 2, 3, 4));
        arr.reverse();
        assertEquals(List.of(4, 3, 2, 1), arr.toList());
    }

    @Test
    void testReverseSingle() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(42));
        arr.reverse();
        assertEquals(List.of(42), arr.toList());
    }

    @Test
    void testReverseEmpty() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.reverse();
        assertEquals(List.of(), arr.toList());
    }

    // ---- toList ----

    @Test
    void testToList() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        assertEquals(List.of(), arr.toList());
        arr.append(1);
        assertEquals(List.of(1), arr.toList());
    }

    // ---- fromList ----

    @Test
    void testFromList() {
        DynamicArray<Integer> arr = DynamicArray.fromList(List.of(5, 10, 15));
        assertEquals(List.of(5, 10, 15), arr.toList());
        assertEquals(3, arr.size());
    }

    // ---- Resize behavior (grow) ----

    @Test
    void testGrowCapacity() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        assertEquals(DynamicArray.DEFAULT_CAPACITY, arr.capacity());

        // Fill to capacity
        for (int i = 0; i < DynamicArray.DEFAULT_CAPACITY; i++) {
            arr.append(i);
        }
        assertEquals(DynamicArray.DEFAULT_CAPACITY, arr.capacity());

        // One more should trigger growth
        arr.append(99);
        assertEquals(DynamicArray.DEFAULT_CAPACITY * DynamicArray.GROWTH_FACTOR, arr.capacity());
        assertEquals(DynamicArray.DEFAULT_CAPACITY + 1, arr.size());

        // Verify all elements are preserved
        for (int i = 0; i < DynamicArray.DEFAULT_CAPACITY; i++) {
            assertEquals(i, arr.get(i));
        }
        assertEquals(99, arr.get(DynamicArray.DEFAULT_CAPACITY));
    }

    // ---- Resize behavior (shrink) ----

    @Test
    void testShrinkCapacity() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        // Fill to trigger growth: 8 -> 16
        for (int i = 0; i < 9; i++) {
            arr.append(i);
        }
        assertEquals(16, arr.capacity());
        assertEquals(9, arr.size());

        // Pop until at or below SHRINK_THRESHOLD (0.25 of 16 = 4)
        // Remove elements until size <= 4
        while (arr.size() > 4) {
            arr.pop(arr.size() - 1);
        }
        assertEquals(16, arr.capacity()); // 4/16 = 0.25, should not shrink yet

        // Pop one more to go below threshold
        arr.pop(arr.size() - 1);
        // size = 3, capacity should shrink: 16 / 2 = 8
        assertEquals(8, arr.capacity());
        assertEquals(3, arr.size());
    }

    @Test
    void testShrinkNeverBelowDefault() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        arr.append(1);
        arr.pop(0);
        // Should still be at DEFAULT_CAPACITY
        assertEquals(DynamicArray.DEFAULT_CAPACITY, arr.capacity());
    }

    // ---- isEmpty and size ----

    @Test
    void testIsEmpty() {
        DynamicArray<Integer> arr = new DynamicArray<>();
        assertTrue(arr.isEmpty());
        arr.append(1);
        assertFalse(arr.isEmpty());
        arr.pop(0);
        assertTrue(arr.isEmpty());
    }

    @Test
    void testCustomInitialCapacity() {
        DynamicArray<Integer> arr = new DynamicArray<>(4);
        assertEquals(4, arr.capacity());
        for (int i = 0; i < 5; i++) {
            arr.append(i);
        }
        assertEquals(8, arr.capacity()); // 4 * 2 = 8
    }

    @Test
    void testInvalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new DynamicArray<>(0));
        assertThrows(IllegalArgumentException.class, () -> new DynamicArray<>(-5));
    }
}
