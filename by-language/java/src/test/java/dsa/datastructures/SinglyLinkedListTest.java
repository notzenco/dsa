package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

class SinglyLinkedListTest {

    // ---- prepend ----

    @Test
    void testPrepend() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.prepend(3);
        list.prepend(2);
        list.prepend(1);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(3, list.size());
    }

    // ---- append ----

    @Test
    void testAppend() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(1);
        list.append(2);
        list.append(3);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(3, list.size());
    }

    // ---- insert ----

    @Test
    void testInsertAtBeginning() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(2);
        list.append(3);
        list.insert(0, 1);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertAtEnd() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(1);
        list.append(2);
        list.insert(2, 3);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertAtMiddle() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(1);
        list.append(3);
        list.insert(1, 2);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertOutOfBounds() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertThrows(IndexOutOfBoundsException.class, () -> list.insert(-1, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.insert(1, 1));
    }

    // ---- popFirst ----

    @Test
    void testPopFirst() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(1, list.popFirst());
        assertEquals(List.of(2, 3), list.toList());
    }

    @Test
    void testPopFirstSingleElement() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(42);
        assertEquals(42, list.popFirst());
        assertTrue(list.isEmpty());
    }

    @Test
    void testPopFirstEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertThrows(NoSuchElementException.class, list::popFirst);
    }

    // ---- popLast ----

    @Test
    void testPopLast() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(3, list.popLast());
        assertEquals(List.of(1, 2), list.toList());
    }

    @Test
    void testPopLastSingleElement() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.append(42);
        assertEquals(42, list.popLast());
        assertTrue(list.isEmpty());
    }

    @Test
    void testPopLastEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertThrows(NoSuchElementException.class, list::popLast);
    }

    // ---- pop(index) ----

    @Test
    void testPopByIndex() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(10, 20, 30, 40));
        assertEquals(20, list.pop(1));
        assertEquals(List.of(10, 30, 40), list.toList());
    }

    @Test
    void testPopByIndexFirst() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(10, 20));
        assertEquals(10, list.pop(0));
        assertEquals(List.of(20), list.toList());
    }

    @Test
    void testPopByIndexLast() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(10, 20));
        assertEquals(20, list.pop(1));
        assertEquals(List.of(10), list.toList());
    }

    @Test
    void testPopByIndexOutOfBounds() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2));
        assertThrows(IndexOutOfBoundsException.class, () -> list.pop(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.pop(2));
    }

    // ---- remove(value) ----

    @Test
    void testRemoveValue() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertTrue(list.remove(2));
        assertEquals(List.of(1, 3), list.toList());
    }

    @Test
    void testRemoveValueHead() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertTrue(list.remove(1));
        assertEquals(List.of(2, 3), list.toList());
    }

    @Test
    void testRemoveValueTail() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertTrue(list.remove(3));
        assertEquals(List.of(1, 2), list.toList());
    }

    @Test
    void testRemoveValueNotFound() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertFalse(list.remove(99));
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testRemoveValueEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertFalse(list.remove(1));
    }

    // ---- find ----

    @Test
    void testFind() {
        SinglyLinkedList<String> list = SinglyLinkedList.fromList(List.of("a", "b", "c"));
        assertEquals(0, list.find("a"));
        assertEquals(1, list.find("b"));
        assertEquals(2, list.find("c"));
        assertEquals(-1, list.find("z"));
    }

    // ---- get ----

    @Test
    void testGet() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(10, 20, 30));
        assertEquals(10, list.get(0));
        assertEquals(20, list.get(1));
        assertEquals(30, list.get(2));
    }

    @Test
    void testGetOutOfBounds() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.get(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.get(1));
    }

    // ---- clear ----

    @Test
    void testClear() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        list.clear();
        assertTrue(list.isEmpty());
        assertEquals(0, list.size());
        assertEquals(List.of(), list.toList());
    }

    // ---- reverse ----

    @Test
    void testReverse() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3, 4));
        list.reverse();
        assertEquals(List.of(4, 3, 2, 1), list.toList());
    }

    @Test
    void testReverseSingleElement() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(42));
        list.reverse();
        assertEquals(List.of(42), list.toList());
    }

    @Test
    void testReverseEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.reverse(); // should not throw
        assertEquals(List.of(), list.toList());
    }

    // ---- toList ----

    @Test
    void testToListEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertEquals(List.of(), list.toList());
    }

    // ---- fromList ----

    @Test
    void testFromList() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(5, 10, 15));
        assertEquals(List.of(5, 10, 15), list.toList());
        assertEquals(3, list.size());
    }

    // ---- getMiddle ----

    @Test
    void testGetMiddleOddLength() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3, 4, 5));
        assertEquals(3, list.getMiddle());
    }

    @Test
    void testGetMiddleEvenLength() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3, 4));
        assertEquals(2, list.getMiddle());
    }

    @Test
    void testGetMiddleSingle() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(42));
        assertEquals(42, list.getMiddle());
    }

    @Test
    void testGetMiddleEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertThrows(NoSuchElementException.class, list::getMiddle);
    }

    // ---- hasCycle ----

    @Test
    void testHasCycleNoCycle() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        assertFalse(list.hasCycle());
    }

    @Test
    void testHasCycleEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assertFalse(list.hasCycle());
    }

    @Test
    void testHasCycleSingleNoCycle() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1));
        assertFalse(list.hasCycle());
    }

    // ---- getNthFromEnd ----

    @Test
    void testGetNthFromEnd() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(10, 20, 30, 40, 50));
        assertEquals(50, list.getNthFromEnd(1));
        assertEquals(40, list.getNthFromEnd(2));
        assertEquals(30, list.getNthFromEnd(3));
        assertEquals(10, list.getNthFromEnd(5));
    }

    @Test
    void testGetNthFromEndInvalid() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2));
        assertThrows(IllegalArgumentException.class, () -> list.getNthFromEnd(0));
        assertThrows(IllegalArgumentException.class, () -> list.getNthFromEnd(3));
    }

    // ---- removeDuplicates ----

    @Test
    void testRemoveDuplicates() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 2, 3, 3, 3, 4));
        list.removeDuplicates();
        assertEquals(List.of(1, 2, 3, 4), list.toList());
    }

    @Test
    void testRemoveDuplicatesNoDuplicates() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 2, 3));
        list.removeDuplicates();
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testRemoveDuplicatesAllSame() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(5, 5, 5, 5));
        list.removeDuplicates();
        assertEquals(List.of(5), list.toList());
        assertEquals(1, list.size());
    }

    @Test
    void testRemoveDuplicatesEmpty() {
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        list.removeDuplicates(); // should not throw
        assertEquals(List.of(), list.toList());
    }

    @Test
    void testRemoveDuplicatesNonConsecutive() {
        SinglyLinkedList<Integer> list = SinglyLinkedList.fromList(List.of(1, 3, 2, 3, 1));
        list.removeDuplicates();
        assertEquals(List.of(1, 3, 2), list.toList());
    }
}
