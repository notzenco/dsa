package dsa.datastructures;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.NoSuchElementException;

public class MonotonicQueue {

    private final Deque<int[]> deque; // each entry is [index, value]

    public MonotonicQueue() {
        this.deque = new ArrayDeque<>();
    }

    /**
     * Push a value with its index onto the monotonic queue.
     * Maintains decreasing order: removes all elements from the back
     * that are smaller than the new value.
     */
    public void push(int index, int value) {
        while (!deque.isEmpty() && deque.peekLast()[1] <= value) {
            deque.pollLast();
        }
        deque.addLast(new int[]{index, value});
    }

    /**
     * Remove the front element if its index equals the given index.
     * This is used to expire elements that fall out of the sliding window.
     */
    public void pop(int index) {
        if (!deque.isEmpty() && deque.peekFirst()[0] == index) {
            deque.pollFirst();
        }
    }

    /**
     * Get the current maximum value (front of the deque).
     */
    public int getMax() {
        if (deque.isEmpty()) {
            throw new NoSuchElementException("MonotonicQueue is empty");
        }
        return deque.peekFirst()[1];
    }

    public boolean isEmpty() {
        return deque.isEmpty();
    }

    public int size() {
        return deque.size();
    }

    /**
     * Compute the maximum value in each sliding window of size k.
     *
     * @param nums the input array
     * @param k    the window size
     * @return an array of maximums for each window position
     */
    public static int[] slidingWindowMaximum(int[] nums, int k) {
        if (nums == null || nums.length == 0 || k <= 0) {
            return new int[0];
        }
        if (k > nums.length) {
            k = nums.length;
        }
        int n = nums.length;
        int[] result = new int[n - k + 1];
        MonotonicQueue mq = new MonotonicQueue();

        for (int i = 0; i < n; i++) {
            // Remove element that is out of the window
            mq.pop(i - k);
            // Push current element
            mq.push(i, nums[i]);
            // Record the max once we have a full window
            if (i >= k - 1) {
                result[i - k + 1] = mq.getMax();
            }
        }
        return result;
    }
}
