package dsa.datastructures;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Optional;
import java.util.TreeMap;

public class Queue<T> {
    private final LinkedList<T> data;

    public Queue() {
        this.data = new LinkedList<>();
    }

    public void enqueue(T value) {
        data.addLast(value);
    }

    public T dequeue() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty");
        }
        return data.removeFirst();
    }

    public T peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty");
        }
        return data.getFirst();
    }

    public T rear() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty");
        }
        return data.getLast();
    }

    public boolean isEmpty() {
        return data.isEmpty();
    }

    public int size() {
        return data.size();
    }

    public void clear() {
        data.clear();
    }

    public List<T> toList() {
        return new ArrayList<>(data);
    }

    public boolean contains(T value) {
        return data.contains(value);
    }

    public T removeAt(int index) {
        if (index < 0 || index >= data.size()) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + data.size());
        }
        return data.remove(index);
    }

    public Object[] toArray() {
        return data.toArray();
    }

    public static <T> Queue<T> fromList(List<T> list) {
        Queue<T> queue = new Queue<>();
        for (T item : list) {
            queue.enqueue(item);
        }
        return queue;
    }

    // ---- CircularQueue ----
    public static class CircularQueue<T> {
        private final Object[] data;
        private int front;
        private int rear;
        private int count;
        private final int capacity;

        public CircularQueue(int capacity) {
            if (capacity <= 0) {
                throw new IllegalArgumentException("Capacity must be positive");
            }
            this.capacity = capacity;
            this.data = new Object[capacity];
            this.front = 0;
            this.rear = -1;
            this.count = 0;
        }

        public boolean enqueue(T value) {
            if (isFull()) {
                return false;
            }
            rear = (rear + 1) % capacity;
            data[rear] = value;
            count++;
            return true;
        }

        @SuppressWarnings("unchecked")
        public Optional<T> dequeue() {
            if (isEmpty()) {
                return Optional.empty();
            }
            T value = (T) data[front];
            data[front] = null;
            front = (front + 1) % capacity;
            count--;
            return Optional.of(value);
        }

        @SuppressWarnings("unchecked")
        public T peek() {
            if (isEmpty()) {
                throw new NoSuchElementException("CircularQueue is empty");
            }
            return (T) data[front];
        }

        @SuppressWarnings("unchecked")
        public T rear() {
            if (isEmpty()) {
                throw new NoSuchElementException("CircularQueue is empty");
            }
            return (T) data[rear];
        }

        public boolean isEmpty() {
            return count == 0;
        }

        public boolean isFull() {
            return count == capacity;
        }

        public int capacity() {
            return capacity;
        }

        public int size() {
            return count;
        }

        @SuppressWarnings("unchecked")
        public List<T> toList() {
            List<T> result = new ArrayList<>();
            for (int i = 0; i < count; i++) {
                result.add((T) data[(front + i) % capacity]);
            }
            return result;
        }

        @SuppressWarnings("unchecked")
        public boolean contains(T value) {
            for (int i = 0; i < count; i++) {
                T item = (T) data[(front + i) % capacity];
                if (item != null ? item.equals(value) : value == null) {
                    return true;
                }
            }
            return false;
        }
    }

    // ---- Deque ----
    public static class Deque<T> {
        private final LinkedList<T> data;

        public Deque() {
            this.data = new LinkedList<>();
        }

        public void addFront(T value) {
            data.addFirst(value);
        }

        public void addRear(T value) {
            data.addLast(value);
        }

        public T removeFront() {
            if (isEmpty()) {
                throw new NoSuchElementException("Deque is empty");
            }
            return data.removeFirst();
        }

        public T removeRear() {
            if (isEmpty()) {
                throw new NoSuchElementException("Deque is empty");
            }
            return data.removeLast();
        }

        public T peekFront() {
            if (isEmpty()) {
                throw new NoSuchElementException("Deque is empty");
            }
            return data.getFirst();
        }

        public T peekRear() {
            if (isEmpty()) {
                throw new NoSuchElementException("Deque is empty");
            }
            return data.getLast();
        }

        public boolean isEmpty() {
            return data.isEmpty();
        }

        public int size() {
            return data.size();
        }

        public void clear() {
            data.clear();
        }

        public List<T> toList() {
            return new ArrayList<>(data);
        }

        public boolean contains(T value) {
            return data.contains(value);
        }

        public boolean removeFirst(T value) {
            return data.removeFirstOccurrence(value);
        }

        public Object[] toArray() {
            return data.toArray();
        }

        public Deque<T> reversed() {
            Deque<T> result = new Deque<>();
            for (int i = data.size() - 1; i >= 0; i--) {
                result.addRear(data.get(i));
            }
            return result;
        }
    }

    // ---- Sliding Window Maximum ----

    public static int[] slidingWindowMaximum(int[] nums, int k) {
        if (nums == null || nums.length == 0 || k <= 0) {
            return new int[0];
        }
        int n = nums.length;
        int[] result = new int[n - k + 1];
        ArrayDeque<Integer> deque = new ArrayDeque<>(); // stores indices

        for (int i = 0; i < n; i++) {
            // Remove indices that are out of window
            while (!deque.isEmpty() && deque.peekFirst() < i - k + 1) {
                deque.pollFirst();
            }
            // Remove smaller elements from back
            while (!deque.isEmpty() && nums[deque.peekLast()] < nums[i]) {
                deque.pollLast();
            }
            deque.addLast(i);
            if (i >= k - 1) {
                result[i - k + 1] = nums[deque.peekFirst()];
            }
        }
        return result;
    }

    // ---- PriorityDeque ----

    public static class PriorityDeque<T extends Comparable<T>> {
        private final TreeMap<T, Integer> map;
        private int size;

        public PriorityDeque() {
            this.map = new TreeMap<>();
            this.size = 0;
        }

        public void add(T value) {
            map.merge(value, 1, Integer::sum);
            size++;
        }

        public T removeMin() {
            if (isEmpty()) {
                throw new NoSuchElementException("PriorityDeque is empty");
            }
            T min = map.firstKey();
            decrementOrRemove(min);
            size--;
            return min;
        }

        public T removeMax() {
            if (isEmpty()) {
                throw new NoSuchElementException("PriorityDeque is empty");
            }
            T max = map.lastKey();
            decrementOrRemove(max);
            size--;
            return max;
        }

        public T peekMin() {
            if (isEmpty()) {
                throw new NoSuchElementException("PriorityDeque is empty");
            }
            return map.firstKey();
        }

        public T peekMax() {
            if (isEmpty()) {
                throw new NoSuchElementException("PriorityDeque is empty");
            }
            return map.lastKey();
        }

        public int size() {
            return size;
        }

        public boolean isEmpty() {
            return size == 0;
        }

        private void decrementOrRemove(T key) {
            int count = map.get(key);
            if (count == 1) {
                map.remove(key);
            } else {
                map.put(key, count - 1);
            }
        }
    }
}
