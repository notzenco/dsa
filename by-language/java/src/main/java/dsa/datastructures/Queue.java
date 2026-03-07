package dsa.datastructures;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Optional;

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
    }
}
