package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

class MonotonicQueueTest {

    // ---- push ----

    @Test
    void testPushMaintainsDecreasingOrder() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 5);
        assertEquals(5, mq.getMax());

        mq.push(1, 3);
        assertEquals(5, mq.getMax()); // 5 > 3, so 5 stays

        mq.push(2, 7);
        assertEquals(7, mq.getMax()); // 7 > 5, so 5 and 3 removed
    }

    @Test
    void testPushAllDecreasing() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 10);
        mq.push(1, 8);
        mq.push(2, 6);
        assertEquals(10, mq.getMax());
        assertEquals(3, mq.size());
    }

    @Test
    void testPushAllIncreasing() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 1);
        mq.push(1, 2);
        mq.push(2, 3);
        assertEquals(3, mq.getMax());
        assertEquals(1, mq.size()); // only the last element remains
    }

    @Test
    void testPushEqual() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 5);
        mq.push(1, 5);
        assertEquals(5, mq.getMax());
        assertEquals(1, mq.size()); // equal replaces
    }

    // ---- pop ----

    @Test
    void testPopFrontElement() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 10);
        mq.push(1, 8);
        mq.push(2, 6);
        // Front is index 0
        mq.pop(0);
        assertEquals(8, mq.getMax());
    }

    @Test
    void testPopNonFrontElement() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 10);
        mq.push(1, 8);
        // Pop index 1 (not front), should do nothing
        mq.pop(1);
        assertEquals(10, mq.getMax());
        assertEquals(2, mq.size()); // unchanged
    }

    @Test
    void testPopFromEmpty() {
        MonotonicQueue mq = new MonotonicQueue();
        // Should not throw
        mq.pop(0);
        assertTrue(mq.isEmpty());
    }

    // ---- getMax ----

    @Test
    void testGetMaxEmpty() {
        MonotonicQueue mq = new MonotonicQueue();
        assertThrows(NoSuchElementException.class, mq::getMax);
    }

    @Test
    void testGetMaxSingleElement() {
        MonotonicQueue mq = new MonotonicQueue();
        mq.push(0, 42);
        assertEquals(42, mq.getMax());
    }

    // ---- isEmpty ----

    @Test
    void testIsEmpty() {
        MonotonicQueue mq = new MonotonicQueue();
        assertTrue(mq.isEmpty());
        mq.push(0, 1);
        assertFalse(mq.isEmpty());
    }

    // ---- slidingWindowMaximum ----

    @Test
    void testSlidingWindowMaximumBasic() {
        int[] nums = {1, 3, -1, -3, 5, 3, 6, 7};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{3, 3, 5, 5, 6, 7}, result);
    }

    @Test
    void testSlidingWindowMaximumWindowSizeOne() {
        int[] nums = {4, 2, 7, 1};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 1);
        assertArrayEquals(new int[]{4, 2, 7, 1}, result);
    }

    @Test
    void testSlidingWindowMaximumWindowSizeEqualToArray() {
        int[] nums = {3, 1, 4, 1, 5};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 5);
        assertArrayEquals(new int[]{5}, result);
    }

    @Test
    void testSlidingWindowMaximumDecreasing() {
        int[] nums = {5, 4, 3, 2, 1};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{5, 4, 3}, result);
    }

    @Test
    void testSlidingWindowMaximumIncreasing() {
        int[] nums = {1, 2, 3, 4, 5};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{3, 4, 5}, result);
    }

    @Test
    void testSlidingWindowMaximumAllSame() {
        int[] nums = {7, 7, 7, 7};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 2);
        assertArrayEquals(new int[]{7, 7, 7}, result);
    }

    @Test
    void testSlidingWindowMaximumEmpty() {
        int[] result = MonotonicQueue.slidingWindowMaximum(new int[]{}, 3);
        assertArrayEquals(new int[]{}, result);
    }

    @Test
    void testSlidingWindowMaximumNull() {
        int[] result = MonotonicQueue.slidingWindowMaximum(null, 3);
        assertArrayEquals(new int[]{}, result);
    }

    @Test
    void testSlidingWindowMaximumKZero() {
        int[] result = MonotonicQueue.slidingWindowMaximum(new int[]{1, 2, 3}, 0);
        assertArrayEquals(new int[]{}, result);
    }

    @Test
    void testSlidingWindowMaximumKLargerThanArray() {
        int[] nums = {1, 5, 3};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 10);
        assertArrayEquals(new int[]{5}, result);
    }

    @Test
    void testSlidingWindowMaximumSingleElement() {
        int[] nums = {42};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 1);
        assertArrayEquals(new int[]{42}, result);
    }

    @Test
    void testSlidingWindowMaximumWindowSizeTwo() {
        int[] nums = {1, 3, 2, 5, 4};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 2);
        assertArrayEquals(new int[]{3, 3, 5, 5}, result);
    }

    @Test
    void testSlidingWindowMaximumNegativeNumbers() {
        int[] nums = {-1, -3, -5, -2, -4};
        int[] result = MonotonicQueue.slidingWindowMaximum(nums, 3);
        assertArrayEquals(new int[]{-1, -2, -2}, result);
    }
}
