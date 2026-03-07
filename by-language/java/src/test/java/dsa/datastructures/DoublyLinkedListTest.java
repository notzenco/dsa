package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

class DoublyLinkedListTest {

    // ---- prepend ----

    @Test
    void testPrepend() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.prepend(3);
        list.prepend(2);
        list.prepend(1);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(3, list.size());
    }

    // ---- append ----

    @Test
    void testAppend() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(1);
        list.append(2);
        list.append(3);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    // ---- insert ----

    @Test
    void testInsertAtBeginning() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(2);
        list.append(3);
        list.insert(0, 1);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertAtEnd() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(1);
        list.append(2);
        list.insert(2, 3);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertAtMiddle() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(1);
        list.append(3);
        list.insert(1, 2);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testInsertOutOfBounds() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IndexOutOfBoundsException.class, () -> list.insert(-1, 1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.insert(1, 1));
    }

    // ---- popFirst ----

    @Test
    void testPopFirst() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(1, list.popFirst());
        assertEquals(List.of(2, 3), list.toList());
    }

    @Test
    void testPopFirstSingleElement() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(42);
        assertEquals(42, list.popFirst());
        assertTrue(list.isEmpty());
    }

    @Test
    void testPopFirstEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(NoSuchElementException.class, list::popFirst);
    }

    // ---- popLast ----

    @Test
    void testPopLast() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(3, list.popLast());
        assertEquals(List.of(1, 2), list.toList());
    }

    @Test
    void testPopLastSingleElement() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(42);
        assertEquals(42, list.popLast());
        assertTrue(list.isEmpty());
    }

    @Test
    void testPopLastEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(NoSuchElementException.class, list::popLast);
    }

    // ---- pop(index) ----

    @Test
    void testPopByIndex() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20, 30, 40));
        assertEquals(20, list.pop(1));
        assertEquals(List.of(10, 30, 40), list.toList());
    }

    @Test
    void testPopByIndexFirst() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20));
        assertEquals(10, list.pop(0));
        assertEquals(List.of(20), list.toList());
    }

    @Test
    void testPopByIndexLast() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20));
        assertEquals(20, list.pop(1));
        assertEquals(List.of(10), list.toList());
    }

    @Test
    void testPopByIndexOutOfBounds() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2));
        assertThrows(IndexOutOfBoundsException.class, () -> list.pop(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.pop(2));
    }

    // ---- removeNode ----

    @Test
    void testRemoveNodeMiddle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(2);
        list.removeNode(node);
        assertEquals(List.of(1, 3), list.toList());
    }

    @Test
    void testRemoveNodeHead() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(1);
        list.removeNode(node);
        assertEquals(List.of(2, 3), list.toList());
    }

    @Test
    void testRemoveNodeTail() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(3);
        list.removeNode(node);
        assertEquals(List.of(1, 2), list.toList());
    }

    @Test
    void testRemoveNodeSingle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(42));
        DoublyLinkedList.DNode<Integer> node = list.findNode(42);
        list.removeNode(node);
        assertTrue(list.isEmpty());
    }

    @Test
    void testRemoveNodeNull() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IllegalArgumentException.class, () -> list.removeNode(null));
    }

    // ---- remove(value) ----

    @Test
    void testRemoveValue() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertTrue(list.remove(2));
        assertEquals(List.of(1, 3), list.toList());
    }

    @Test
    void testRemoveValueNotFound() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertFalse(list.remove(99));
    }

    // ---- find ----

    @Test
    void testFind() {
        DoublyLinkedList<String> list = DoublyLinkedList.fromList(List.of("a", "b", "c"));
        assertEquals(0, list.find("a"));
        assertEquals(1, list.find("b"));
        assertEquals(2, list.find("c"));
        assertEquals(-1, list.find("z"));
    }

    // ---- findNode ----

    @Test
    void testFindNode() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20, 30));
        DoublyLinkedList.DNode<Integer> node = list.findNode(20);
        assertNotNull(node);
        assertEquals(20, node.data);
    }

    @Test
    void testFindNodeNotFound() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertNull(list.findNode(99));
    }

    // ---- get ----

    @Test
    void testGet() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20, 30));
        assertEquals(10, list.get(0));
        assertEquals(20, list.get(1));
        assertEquals(30, list.get(2));
    }

    @Test
    void testGetOutOfBounds() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.get(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.get(1));
    }

    // ---- set ----

    @Test
    void testSet() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        list.set(1, 99);
        assertEquals(List.of(1, 99, 3), list.toList());
    }

    @Test
    void testSetOutOfBounds() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1));
        assertThrows(IndexOutOfBoundsException.class, () -> list.set(-1, 0));
        assertThrows(IndexOutOfBoundsException.class, () -> list.set(1, 0));
    }

    // ---- clear ----

    @Test
    void testClear() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        list.clear();
        assertTrue(list.isEmpty());
        assertEquals(0, list.size());
    }

    // ---- reverse ----

    @Test
    void testReverse() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3, 4));
        list.reverse();
        assertEquals(List.of(4, 3, 2, 1), list.toList());
        // Also verify reverse traversal works
        assertEquals(List.of(1, 2, 3, 4), list.toListReversed());
    }

    @Test
    void testReverseSingle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(42));
        list.reverse();
        assertEquals(List.of(42), list.toList());
    }

    @Test
    void testReverseEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.reverse();
        assertEquals(List.of(), list.toList());
    }

    // ---- toList ----

    @Test
    void testToList() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    // ---- toListReversed ----

    @Test
    void testToListReversed() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(List.of(3, 2, 1), list.toListReversed());
    }

    @Test
    void testToListReversedEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertEquals(List.of(), list.toListReversed());
    }

    // ---- fromList ----

    @Test
    void testFromList() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(5, 10, 15));
        assertEquals(List.of(5, 10, 15), list.toList());
        assertEquals(3, list.size());
    }

    // ---- moveToFront ----

    @Test
    void testMoveToFrontMiddle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3, 4));
        DoublyLinkedList.DNode<Integer> node = list.findNode(3);
        list.moveToFront(node);
        assertEquals(List.of(3, 1, 2, 4), list.toList());
        assertEquals(List.of(4, 2, 1, 3), list.toListReversed());
    }

    @Test
    void testMoveToFrontTail() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(3);
        list.moveToFront(node);
        assertEquals(List.of(3, 1, 2), list.toList());
        assertEquals(List.of(2, 1, 3), list.toListReversed());
    }

    @Test
    void testMoveToFrontAlreadyFront() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(1);
        list.moveToFront(node);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testMoveToFrontNull() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IllegalArgumentException.class, () -> list.moveToFront(null));
    }

    // ---- moveToBack ----

    @Test
    void testMoveToBackMiddle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3, 4));
        DoublyLinkedList.DNode<Integer> node = list.findNode(2);
        list.moveToBack(node);
        assertEquals(List.of(1, 3, 4, 2), list.toList());
        assertEquals(List.of(2, 4, 3, 1), list.toListReversed());
    }

    @Test
    void testMoveToBackHead() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(1);
        list.moveToBack(node);
        assertEquals(List.of(2, 3, 1), list.toList());
        assertEquals(List.of(1, 3, 2), list.toListReversed());
    }

    @Test
    void testMoveToBackAlreadyBack() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(3);
        list.moveToBack(node);
        assertEquals(List.of(1, 2, 3), list.toList());
    }

    @Test
    void testMoveToBackNull() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IllegalArgumentException.class, () -> list.moveToBack(null));
    }

    // ---- Bidirectional integrity ----

    @Test
    void testBidirectionalIntegrityAfterOperations() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        list.append(1);
        list.append(2);
        list.prepend(0);
        list.insert(2, 99);
        // List should be: 0, 1, 99, 2
        assertEquals(List.of(0, 1, 99, 2), list.toList());
        assertEquals(List.of(2, 99, 1, 0), list.toListReversed());

        list.pop(1); // remove 1
        // List should be: 0, 99, 2
        assertEquals(List.of(0, 99, 2), list.toList());
        assertEquals(List.of(2, 99, 0), list.toListReversed());
    }

    // ---- contains ----

    @Test
    void testContains() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertTrue(list.contains(2));
        assertFalse(list.contains(99));
    }

    @Test
    void testContainsEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertFalse(list.contains(1));
    }

    // ---- toArray ----

    @Test
    void testToArray() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(10, 20, 30));
        assertArrayEquals(new Object[]{10, 20, 30}, list.toArray());
    }

    @Test
    void testToArrayEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertEquals(0, list.toArray().length);
    }

    // ---- insertAfter ----

    @Test
    void testInsertAfterMiddle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(1);
        list.insertAfter(node, 2);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(List.of(3, 2, 1), list.toListReversed());
        assertEquals(3, list.size());
    }

    @Test
    void testInsertAfterTail() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2));
        DoublyLinkedList.DNode<Integer> node = list.findNode(2);
        list.insertAfter(node, 3);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(List.of(3, 2, 1), list.toListReversed());
    }

    @Test
    void testInsertAfterNull() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IllegalArgumentException.class, () -> list.insertAfter(null, 1));
    }

    // ---- insertBefore ----

    @Test
    void testInsertBeforeMiddle() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(3);
        list.insertBefore(node, 2);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(List.of(3, 2, 1), list.toListReversed());
        assertEquals(3, list.size());
    }

    @Test
    void testInsertBeforeHead() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(2, 3));
        DoublyLinkedList.DNode<Integer> node = list.findNode(2);
        list.insertBefore(node, 1);
        assertEquals(List.of(1, 2, 3), list.toList());
        assertEquals(List.of(3, 2, 1), list.toListReversed());
    }

    @Test
    void testInsertBeforeNull() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertThrows(IllegalArgumentException.class, () -> list.insertBefore(null, 1));
    }

    // ---- getHead / getTail ----

    @Test
    void testGetHeadAndTail() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        assertEquals(1, list.getHead().data);
        assertEquals(3, list.getTail().data);
    }

    @Test
    void testGetHeadAndTailEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        assertNull(list.getHead());
        assertNull(list.getTail());
    }

    // ---- mergeSorted ----

    @Test
    void testMergeSorted() {
        DoublyLinkedList<Integer> a = DoublyLinkedList.fromList(List.of(1, 3, 5));
        DoublyLinkedList<Integer> b = DoublyLinkedList.fromList(List.of(2, 4, 6));
        DoublyLinkedList<Integer> merged = DoublyLinkedList.mergeSorted(a, b);
        assertEquals(List.of(1, 2, 3, 4, 5, 6), merged.toList());
        assertEquals(List.of(6, 5, 4, 3, 2, 1), merged.toListReversed());
    }

    @Test
    void testMergeSortedOneEmpty() {
        DoublyLinkedList<Integer> a = DoublyLinkedList.fromList(List.of(1, 2));
        DoublyLinkedList<Integer> b = new DoublyLinkedList<>();
        assertEquals(List.of(1, 2), DoublyLinkedList.mergeSorted(a, b).toList());
        assertEquals(List.of(1, 2), DoublyLinkedList.mergeSorted(b, a).toList());
    }

    // ---- map ----

    @Test
    void testMap() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        DoublyLinkedList<String> mapped = list.map(i -> "v" + i);
        assertEquals(List.of("v1", "v2", "v3"), mapped.toList());
    }

    @Test
    void testMapEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        DoublyLinkedList<String> mapped = list.map(Object::toString);
        assertTrue(mapped.isEmpty());
    }

    // ---- filter ----

    @Test
    void testFilter() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3, 4, 5));
        DoublyLinkedList<Integer> even = list.filter(i -> i % 2 == 0);
        assertEquals(List.of(2, 4), even.toList());
    }

    @Test
    void testFilterNoneMatch() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 3, 5));
        DoublyLinkedList<Integer> even = list.filter(i -> i % 2 == 0);
        assertTrue(even.isEmpty());
    }

    // ---- forEach ----

    @Test
    void testForEach() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        List<Integer> collected = new ArrayList<>();
        list.forEach(collected::add);
        assertEquals(List.of(1, 2, 3), collected);
    }

    // ---- forEachReversed ----

    @Test
    void testForEachReversed() {
        DoublyLinkedList<Integer> list = DoublyLinkedList.fromList(List.of(1, 2, 3));
        List<Integer> collected = new ArrayList<>();
        list.forEachReversed(collected::add);
        assertEquals(List.of(3, 2, 1), collected);
    }

    @Test
    void testForEachReversedEmpty() {
        DoublyLinkedList<Integer> list = new DoublyLinkedList<>();
        List<Integer> collected = new ArrayList<>();
        list.forEachReversed(collected::add);
        assertTrue(collected.isEmpty());
    }
}
