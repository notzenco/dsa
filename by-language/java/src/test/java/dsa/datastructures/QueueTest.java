package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.NoSuchElementException;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class QueueTest {

    // ---- Queue<T> tests ----

    @Test
    void testEnqueueAndDequeue() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        assertEquals(1, q.dequeue());
        assertEquals(2, q.dequeue());
        assertEquals(3, q.dequeue());
    }

    @Test
    void testDequeueEmpty() {
        Queue<Integer> q = new Queue<>();
        assertThrows(NoSuchElementException.class, q::dequeue);
    }

    @Test
    void testPeek() {
        Queue<String> q = new Queue<>();
        q.enqueue("first");
        q.enqueue("second");
        assertEquals("first", q.peek());
        assertEquals(2, q.size());
    }

    @Test
    void testPeekEmpty() {
        Queue<Integer> q = new Queue<>();
        assertThrows(NoSuchElementException.class, q::peek);
    }

    @Test
    void testRear() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        assertEquals(30, q.rear());
    }

    @Test
    void testRearEmpty() {
        Queue<Integer> q = new Queue<>();
        assertThrows(NoSuchElementException.class, q::rear);
    }

    @Test
    void testIsEmpty() {
        Queue<Integer> q = new Queue<>();
        assertTrue(q.isEmpty());
        q.enqueue(1);
        assertFalse(q.isEmpty());
        q.dequeue();
        assertTrue(q.isEmpty());
    }

    @Test
    void testSize() {
        Queue<Integer> q = new Queue<>();
        assertEquals(0, q.size());
        q.enqueue(1);
        q.enqueue(2);
        assertEquals(2, q.size());
        q.dequeue();
        assertEquals(1, q.size());
    }

    @Test
    void testClear() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(1);
        q.enqueue(2);
        q.clear();
        assertTrue(q.isEmpty());
        assertEquals(0, q.size());
    }

    @Test
    void testToList() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        assertEquals(List.of(1, 2, 3), q.toList());
    }

    @Test
    void testContains() {
        Queue<String> q = new Queue<>();
        q.enqueue("hello");
        q.enqueue("world");
        assertTrue(q.contains("hello"));
        assertFalse(q.contains("foo"));
    }

    @Test
    void testFromList() {
        List<Integer> list = List.of(10, 20, 30);
        Queue<Integer> q = Queue.fromList(list);
        assertEquals(3, q.size());
        assertEquals(10, q.dequeue());
        assertEquals(20, q.dequeue());
        assertEquals(30, q.dequeue());
    }

    // ---- CircularQueue tests ----

    @Test
    void testCircularQueueEnqueueDequeue() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(3);
        assertTrue(cq.enqueue(1));
        assertTrue(cq.enqueue(2));
        assertTrue(cq.enqueue(3));
        assertFalse(cq.enqueue(4)); // full

        assertEquals(Optional.of(1), cq.dequeue());
        assertEquals(Optional.of(2), cq.dequeue());

        assertTrue(cq.enqueue(4)); // wraps around
        assertTrue(cq.enqueue(5));
        assertFalse(cq.enqueue(6)); // full again

        assertEquals(Optional.of(3), cq.dequeue());
        assertEquals(Optional.of(4), cq.dequeue());
        assertEquals(Optional.of(5), cq.dequeue());
        assertEquals(Optional.empty(), cq.dequeue()); // empty
    }

    @Test
    void testCircularQueuePeekAndRear() {
        Queue.CircularQueue<String> cq = new Queue.CircularQueue<>(5);
        cq.enqueue("a");
        cq.enqueue("b");
        cq.enqueue("c");
        assertEquals("a", cq.peek());
        assertEquals("c", cq.rear());
    }

    @Test
    void testCircularQueuePeekEmpty() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(3);
        assertThrows(NoSuchElementException.class, cq::peek);
    }

    @Test
    void testCircularQueueRearEmpty() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(3);
        assertThrows(NoSuchElementException.class, cq::rear);
    }

    @Test
    void testCircularQueueIsEmptyIsFull() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(2);
        assertTrue(cq.isEmpty());
        assertFalse(cq.isFull());

        cq.enqueue(1);
        assertFalse(cq.isEmpty());
        assertFalse(cq.isFull());

        cq.enqueue(2);
        assertFalse(cq.isEmpty());
        assertTrue(cq.isFull());
    }

    @Test
    void testCircularQueueCapacity() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(10);
        assertEquals(10, cq.capacity());
    }

    @Test
    void testCircularQueueSize() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(5);
        assertEquals(0, cq.size());
        cq.enqueue(1);
        cq.enqueue(2);
        assertEquals(2, cq.size());
        cq.dequeue();
        assertEquals(1, cq.size());
    }

    @Test
    void testCircularQueueWrapAround() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(3);
        cq.enqueue(1);
        cq.enqueue(2);
        cq.enqueue(3);
        cq.dequeue(); // remove 1
        cq.dequeue(); // remove 2
        cq.enqueue(4);
        cq.enqueue(5);
        assertEquals(3, cq.peek());
        assertEquals(5, cq.rear());
    }

    @Test
    void testCircularQueueInvalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new Queue.CircularQueue<>(0));
        assertThrows(IllegalArgumentException.class, () -> new Queue.CircularQueue<>(-1));
    }

    // ---- Deque tests ----

    @Test
    void testDequeAddFrontAndRemoveFront() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addFront(1);
        dq.addFront(2);
        dq.addFront(3);
        assertEquals(3, dq.removeFront());
        assertEquals(2, dq.removeFront());
        assertEquals(1, dq.removeFront());
    }

    @Test
    void testDequeAddRearAndRemoveRear() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        dq.addRear(2);
        dq.addRear(3);
        assertEquals(3, dq.removeRear());
        assertEquals(2, dq.removeRear());
        assertEquals(1, dq.removeRear());
    }

    @Test
    void testDequeMixed() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addFront(1);
        dq.addRear(2);
        dq.addFront(3);
        dq.addRear(4);
        // order: 3, 1, 2, 4
        assertEquals(3, dq.peekFront());
        assertEquals(4, dq.peekRear());
        assertEquals(3, dq.removeFront());
        assertEquals(4, dq.removeRear());
        assertEquals(List.of(1, 2), dq.toList());
    }

    @Test
    void testDequeRemoveFrontEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertThrows(NoSuchElementException.class, dq::removeFront);
    }

    @Test
    void testDequeRemoveRearEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertThrows(NoSuchElementException.class, dq::removeRear);
    }

    @Test
    void testDequePeekFrontEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertThrows(NoSuchElementException.class, dq::peekFront);
    }

    @Test
    void testDequePeekRearEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertThrows(NoSuchElementException.class, dq::peekRear);
    }

    @Test
    void testDequeIsEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertTrue(dq.isEmpty());
        dq.addFront(1);
        assertFalse(dq.isEmpty());
    }

    @Test
    void testDequeSize() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        assertEquals(0, dq.size());
        dq.addFront(1);
        dq.addRear(2);
        assertEquals(2, dq.size());
        dq.removeFront();
        assertEquals(1, dq.size());
    }

    @Test
    void testDequeClear() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addFront(1);
        dq.addRear(2);
        dq.clear();
        assertTrue(dq.isEmpty());
        assertEquals(0, dq.size());
    }

    @Test
    void testDequeToList() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        dq.addRear(2);
        dq.addRear(3);
        assertEquals(List.of(1, 2, 3), dq.toList());
    }
}
