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

    // ---- Queue removeAt ----

    @Test
    void testRemoveAt() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        assertEquals(20, q.removeAt(1));
        assertEquals(List.of(10, 30), q.toList());
    }

    @Test
    void testRemoveAtFirst() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(10);
        q.enqueue(20);
        assertEquals(10, q.removeAt(0));
        assertEquals(List.of(20), q.toList());
    }

    @Test
    void testRemoveAtOutOfBounds() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(1);
        assertThrows(IndexOutOfBoundsException.class, () -> q.removeAt(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> q.removeAt(1));
    }

    // ---- Queue toArray ----

    @Test
    void testQueueToArray() {
        Queue<Integer> q = new Queue<>();
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        Object[] arr = q.toArray();
        assertArrayEquals(new Object[]{1, 2, 3}, arr);
    }

    @Test
    void testQueueToArrayEmpty() {
        Queue<Integer> q = new Queue<>();
        assertEquals(0, q.toArray().length);
    }

    // ---- CircularQueue toList ----

    @Test
    void testCircularQueueToList() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(5);
        cq.enqueue(1);
        cq.enqueue(2);
        cq.enqueue(3);
        assertEquals(List.of(1, 2, 3), cq.toList());
    }

    @Test
    void testCircularQueueToListAfterWrapAround() {
        Queue.CircularQueue<Integer> cq = new Queue.CircularQueue<>(3);
        cq.enqueue(1);
        cq.enqueue(2);
        cq.enqueue(3);
        cq.dequeue(); // remove 1
        cq.enqueue(4); // wraps around
        assertEquals(List.of(2, 3, 4), cq.toList());
    }

    // ---- CircularQueue contains ----

    @Test
    void testCircularQueueContains() {
        Queue.CircularQueue<String> cq = new Queue.CircularQueue<>(5);
        cq.enqueue("hello");
        cq.enqueue("world");
        assertTrue(cq.contains("hello"));
        assertFalse(cq.contains("foo"));
    }

    // ---- Deque contains ----

    @Test
    void testDequeContains() {
        Queue.Deque<String> dq = new Queue.Deque<>();
        dq.addRear("a");
        dq.addRear("b");
        assertTrue(dq.contains("a"));
        assertFalse(dq.contains("z"));
    }

    // ---- Deque removeFirst ----

    @Test
    void testDequeRemoveFirst() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        dq.addRear(2);
        dq.addRear(3);
        dq.addRear(2);
        assertTrue(dq.removeFirst(2));
        assertEquals(List.of(1, 3, 2), dq.toList());
    }

    @Test
    void testDequeRemoveFirstNotFound() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        assertFalse(dq.removeFirst(99));
    }

    // ---- Deque toArray ----

    @Test
    void testDequeToArray() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        dq.addRear(2);
        dq.addRear(3);
        assertArrayEquals(new Object[]{1, 2, 3}, dq.toArray());
    }

    // ---- Deque reversed ----

    @Test
    void testDequeReversed() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        dq.addRear(1);
        dq.addRear(2);
        dq.addRear(3);
        Queue.Deque<Integer> rev = dq.reversed();
        assertEquals(List.of(3, 2, 1), rev.toList());
        // Original unchanged
        assertEquals(List.of(1, 2, 3), dq.toList());
    }

    @Test
    void testDequeReversedEmpty() {
        Queue.Deque<Integer> dq = new Queue.Deque<>();
        Queue.Deque<Integer> rev = dq.reversed();
        assertTrue(rev.isEmpty());
    }

    // ---- slidingWindowMaximum ----

    @Test
    void testSlidingWindowMaximumNormal() {
        int[] nums = {1, 3, -1, -3, 5, 3, 6, 7};
        int[] result = Queue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{3, 3, 5, 5, 6, 7}, result);
    }

    @Test
    void testSlidingWindowMaximumSingleWindow() {
        int[] nums = {4, 2, 1};
        int[] result = Queue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{4}, result);
    }

    @Test
    void testSlidingWindowMaximumWindowSizeOne() {
        int[] nums = {1, 3, 2};
        int[] result = Queue.slidingWindowMaximum(nums, 1);
        assertArrayEquals(new int[]{1, 3, 2}, result);
    }

    @Test
    void testSlidingWindowMaximumEmpty() {
        int[] result = Queue.slidingWindowMaximum(new int[]{}, 3);
        assertEquals(0, result.length);
    }

    // ---- PriorityDeque ----

    @Test
    void testPriorityDequeAddAndPeek() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        pq.add(5);
        pq.add(1);
        pq.add(9);
        pq.add(3);
        assertEquals(1, pq.peekMin());
        assertEquals(9, pq.peekMax());
        assertEquals(4, pq.size());
    }

    @Test
    void testPriorityDequeRemoveMin() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        pq.add(5);
        pq.add(1);
        pq.add(9);
        assertEquals(1, pq.removeMin());
        assertEquals(5, pq.peekMin());
        assertEquals(2, pq.size());
    }

    @Test
    void testPriorityDequeRemoveMax() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        pq.add(5);
        pq.add(1);
        pq.add(9);
        assertEquals(9, pq.removeMax());
        assertEquals(5, pq.peekMax());
        assertEquals(2, pq.size());
    }

    @Test
    void testPriorityDequeInterleaved() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        pq.add(3);
        pq.add(1);
        pq.add(7);
        pq.add(5);
        assertEquals(1, pq.removeMin());  // [3, 5, 7]
        assertEquals(7, pq.removeMax());  // [3, 5]
        pq.add(2);                        // [2, 3, 5]
        assertEquals(2, pq.removeMin());  // [3, 5]
        assertEquals(5, pq.removeMax());  // [3]
        assertEquals(3, pq.peekMin());
        assertEquals(3, pq.peekMax());
    }

    @Test
    void testPriorityDequeDuplicates() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        pq.add(3);
        pq.add(3);
        pq.add(3);
        assertEquals(3, pq.size());
        assertEquals(3, pq.removeMin());
        assertEquals(2, pq.size());
        assertEquals(3, pq.peekMin());
    }

    @Test
    void testPriorityDequeEmpty() {
        Queue.PriorityDeque<Integer> pq = new Queue.PriorityDeque<>();
        assertTrue(pq.isEmpty());
        assertThrows(NoSuchElementException.class, pq::removeMin);
        assertThrows(NoSuchElementException.class, pq::removeMax);
        assertThrows(NoSuchElementException.class, pq::peekMin);
        assertThrows(NoSuchElementException.class, pq::peekMax);
    }
}
