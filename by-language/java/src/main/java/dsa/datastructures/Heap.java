package dsa.datastructures;

import java.util.*;

public class Heap {

    // ========== MinHeap ==========
    public static class MinHeap<T extends Comparable<T>> {
        private final List<T> data;

        public MinHeap() {
            this.data = new ArrayList<>();
        }

        public MinHeap(List<T> items) {
            this.data = new ArrayList<>(items);
            heapify();
        }

        public void push(T item) {
            data.add(item);
            bubbleUp(data.size() - 1);
        }

        public T pop() {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            T result = data.get(0);
            T last = data.remove(data.size() - 1);
            if (!data.isEmpty()) {
                data.set(0, last);
                bubbleDown(0);
            }
            return result;
        }

        public T peek() {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            return data.get(0);
        }

        public T pushpop(T item) {
            if (!isEmpty() && data.get(0).compareTo(item) < 0) {
                T top = data.get(0);
                data.set(0, item);
                bubbleDown(0);
                return top;
            }
            return item;
        }

        public T replace(T item) {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            T top = data.get(0);
            data.set(0, item);
            bubbleDown(0);
            return top;
        }

        public boolean isEmpty() { return data.isEmpty(); }
        public int size() { return data.size(); }

        public void clear() { data.clear(); }

        public List<T> toSortedList() {
            MinHeap<T> copy = new MinHeap<>(new ArrayList<>(data));
            List<T> result = new ArrayList<>();
            while (!copy.isEmpty()) {
                result.add(copy.pop());
            }
            return result;
        }

        public void heapify(List<T> items) {
            data.clear();
            data.addAll(items);
            heapify();
        }

        private void heapify() {
            for (int i = (data.size() / 2) - 1; i >= 0; i--) {
                bubbleDown(i);
            }
        }

        public static <T extends Comparable<T>> MinHeap<T> fromList(List<T> items) {
            return new MinHeap<>(items);
        }

        private void bubbleUp(int index) {
            while (index > 0) {
                int parent = (index - 1) / 2;
                if (data.get(index).compareTo(data.get(parent)) < 0) {
                    swap(index, parent);
                    index = parent;
                } else {
                    break;
                }
            }
        }

        private void bubbleDown(int index) {
            int size = data.size();
            while (true) {
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int smallest = index;

                if (left < size && data.get(left).compareTo(data.get(smallest)) < 0) {
                    smallest = left;
                }
                if (right < size && data.get(right).compareTo(data.get(smallest)) < 0) {
                    smallest = right;
                }
                if (smallest != index) {
                    swap(index, smallest);
                    index = smallest;
                } else {
                    break;
                }
            }
        }

        private void swap(int i, int j) {
            T temp = data.get(i);
            data.set(i, data.get(j));
            data.set(j, temp);
        }
    }

    // ========== MaxHeap ==========
    public static class MaxHeap<T extends Comparable<T>> {
        private final List<T> data;

        public MaxHeap() {
            this.data = new ArrayList<>();
        }

        public MaxHeap(List<T> items) {
            this.data = new ArrayList<>(items);
            heapify();
        }

        public void push(T item) {
            data.add(item);
            bubbleUp(data.size() - 1);
        }

        public T pop() {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            T result = data.get(0);
            T last = data.remove(data.size() - 1);
            if (!data.isEmpty()) {
                data.set(0, last);
                bubbleDown(0);
            }
            return result;
        }

        public T peek() {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            return data.get(0);
        }

        public T pushpop(T item) {
            if (!isEmpty() && data.get(0).compareTo(item) > 0) {
                T top = data.get(0);
                data.set(0, item);
                bubbleDown(0);
                return top;
            }
            return item;
        }

        public T replace(T item) {
            if (isEmpty()) throw new NoSuchElementException("Heap is empty");
            T top = data.get(0);
            data.set(0, item);
            bubbleDown(0);
            return top;
        }

        public boolean isEmpty() { return data.isEmpty(); }
        public int size() { return data.size(); }

        public void clear() { data.clear(); }

        public List<T> toSortedList() {
            MaxHeap<T> copy = new MaxHeap<>(new ArrayList<>(data));
            List<T> result = new ArrayList<>();
            while (!copy.isEmpty()) {
                result.add(copy.pop());
            }
            return result;
        }

        public void heapify(List<T> items) {
            data.clear();
            data.addAll(items);
            heapify();
        }

        private void heapify() {
            for (int i = (data.size() / 2) - 1; i >= 0; i--) {
                bubbleDown(i);
            }
        }

        public static <T extends Comparable<T>> MaxHeap<T> fromList(List<T> items) {
            return new MaxHeap<>(items);
        }

        private void bubbleUp(int index) {
            while (index > 0) {
                int parent = (index - 1) / 2;
                if (data.get(index).compareTo(data.get(parent)) > 0) {
                    swap(index, parent);
                    index = parent;
                } else {
                    break;
                }
            }
        }

        private void bubbleDown(int index) {
            int size = data.size();
            while (true) {
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int largest = index;

                if (left < size && data.get(left).compareTo(data.get(largest)) > 0) {
                    largest = left;
                }
                if (right < size && data.get(right).compareTo(data.get(largest)) > 0) {
                    largest = right;
                }
                if (largest != index) {
                    swap(index, largest);
                    index = largest;
                } else {
                    break;
                }
            }
        }

        private void swap(int i, int j) {
            T temp = data.get(i);
            data.set(i, data.get(j));
            data.set(j, temp);
        }
    }

    // ========== PriorityQueue ==========
    public static class PriorityQueue<T> {
        private final List<T> data;
        private final Comparator<T> comparator;

        public PriorityQueue(Comparator<T> comparator) {
            this.data = new ArrayList<>();
            this.comparator = comparator;
        }

        @SuppressWarnings("unchecked")
        public PriorityQueue() {
            this((a, b) -> ((Comparable<T>) a).compareTo(b));
        }

        public void enqueue(T item) {
            data.add(item);
            bubbleUp(data.size() - 1);
        }

        public T dequeue() {
            if (isEmpty()) throw new NoSuchElementException("Priority queue is empty");
            T result = data.get(0);
            T last = data.remove(data.size() - 1);
            if (!data.isEmpty()) {
                data.set(0, last);
                bubbleDown(0);
            }
            return result;
        }

        public T peek() {
            if (isEmpty()) throw new NoSuchElementException("Priority queue is empty");
            return data.get(0);
        }

        public boolean isEmpty() { return data.isEmpty(); }
        public int size() { return data.size(); }
        public void clear() { data.clear(); }

        private void bubbleUp(int index) {
            while (index > 0) {
                int parent = (index - 1) / 2;
                if (comparator.compare(data.get(index), data.get(parent)) < 0) {
                    swap(index, parent);
                    index = parent;
                } else {
                    break;
                }
            }
        }

        private void bubbleDown(int index) {
            int size = data.size();
            while (true) {
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int smallest = index;

                if (left < size && comparator.compare(data.get(left), data.get(smallest)) < 0) {
                    smallest = left;
                }
                if (right < size && comparator.compare(data.get(right), data.get(smallest)) < 0) {
                    smallest = right;
                }
                if (smallest != index) {
                    swap(index, smallest);
                    index = smallest;
                } else {
                    break;
                }
            }
        }

        private void swap(int i, int j) {
            T temp = data.get(i);
            data.set(i, data.get(j));
            data.set(j, temp);
        }
    }

    // ========== Static utility methods ==========

    public static void heapSort(int[] arr) {
        int n = arr.length;
        // Build max heap
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapSortSiftDown(arr, n, i);
        }
        // Extract elements one by one
        for (int i = n - 1; i > 0; i--) {
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            heapSortSiftDown(arr, i, 0);
        }
    }

    private static void heapSortSiftDown(int[] arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest]) largest = left;
        if (right < n && arr[right] > arr[largest]) largest = right;

        if (largest != i) {
            int temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            heapSortSiftDown(arr, n, largest);
        }
    }

    public static <T extends Comparable<T>> List<T> nLargest(int n, List<T> list) {
        MinHeap<T> heap = new MinHeap<>();
        for (T item : list) {
            heap.push(item);
            if (heap.size() > n) {
                heap.pop();
            }
        }
        List<T> result = heap.toSortedList();
        Collections.reverse(result);
        return result;
    }

    public static <T extends Comparable<T>> List<T> nSmallest(int n, List<T> list) {
        MaxHeap<T> heap = new MaxHeap<>();
        for (T item : list) {
            heap.push(item);
            if (heap.size() > n) {
                heap.pop();
            }
        }
        List<T> result = heap.toSortedList();
        Collections.reverse(result);
        return result;
    }
}
