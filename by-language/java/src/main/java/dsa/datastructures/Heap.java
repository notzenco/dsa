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

    /**
     * Merges k sorted lists into a single sorted list using a min-heap.
     *
     * @param lists the list of sorted lists to merge
     * @param <T>   the element type
     * @return a single sorted list containing all elements
     */
    public static <T extends Comparable<T>> List<T> kWayMerge(List<List<T>> lists) {
        List<T> result = new ArrayList<>();
        if (lists == null || lists.isEmpty()) {
            return result;
        }
        // Each entry: [listIndex, elementIndex] with the actual value
        java.util.PriorityQueue<int[]> pq = new java.util.PriorityQueue<>((a, b) -> {
            @SuppressWarnings("unchecked")
            Comparable<T> va = (Comparable<T>) lists.get(a[0]).get(a[1]);
            T vb = lists.get(b[0]).get(b[1]);
            return va.compareTo(vb);
        });
        for (int i = 0; i < lists.size(); i++) {
            if (lists.get(i) != null && !lists.get(i).isEmpty()) {
                pq.offer(new int[]{i, 0});
            }
        }
        while (!pq.isEmpty()) {
            int[] entry = pq.poll();
            int listIdx = entry[0];
            int elemIdx = entry[1];
            result.add(lists.get(listIdx).get(elemIdx));
            if (elemIdx + 1 < lists.get(listIdx).size()) {
                pq.offer(new int[]{listIdx, elemIdx + 1});
            }
        }
        return result;
    }

    /**
     * Finds the k most frequent elements in the given array.
     *
     * @param nums the input array
     * @param k    the number of top frequent elements to return
     * @return an array of the k most frequent elements
     */
    public static int[] topKFrequent(int[] nums, int k) {
        Map<Integer, Integer> freq = new HashMap<>();
        for (int num : nums) {
            freq.merge(num, 1, Integer::sum);
        }
        // Min-heap by frequency
        java.util.PriorityQueue<Map.Entry<Integer, Integer>> pq =
                new java.util.PriorityQueue<>(Comparator.comparingInt(Map.Entry::getValue));
        for (Map.Entry<Integer, Integer> entry : freq.entrySet()) {
            pq.offer(entry);
            if (pq.size() > k) {
                pq.poll();
            }
        }
        int[] result = new int[pq.size()];
        int i = result.length - 1;
        while (!pq.isEmpty()) {
            result[i--] = pq.poll().getKey();
        }
        return result;
    }

    /**
     * Finds the median of two sorted arrays using binary search.
     *
     * @param nums1 the first sorted array
     * @param nums2 the second sorted array
     * @return the median value
     * @throws IllegalArgumentException if both arrays are empty
     */
    public static double findMedianSortedArrays(int[] nums1, int[] nums2) {
        if (nums1.length == 0 && nums2.length == 0) {
            throw new IllegalArgumentException("Both arrays must not be empty");
        }
        // Ensure nums1 is the shorter array
        if (nums1.length > nums2.length) {
            return findMedianSortedArrays(nums2, nums1);
        }
        int m = nums1.length;
        int n = nums2.length;
        int lo = 0, hi = m;
        while (lo <= hi) {
            int i = (lo + hi) / 2;
            int j = (m + n + 1) / 2 - i;

            int maxLeftA = (i == 0) ? Integer.MIN_VALUE : nums1[i - 1];
            int minRightA = (i == m) ? Integer.MAX_VALUE : nums1[i];
            int maxLeftB = (j == 0) ? Integer.MIN_VALUE : nums2[j - 1];
            int minRightB = (j == n) ? Integer.MAX_VALUE : nums2[j];

            if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
                if ((m + n) % 2 == 0) {
                    return ((long) Math.max(maxLeftA, maxLeftB)
                            + (long) Math.min(minRightA, minRightB)) / 2.0;
                } else {
                    return Math.max(maxLeftA, maxLeftB);
                }
            } else if (maxLeftA > minRightB) {
                hi = i - 1;
            } else {
                lo = i + 1;
            }
        }
        throw new IllegalStateException("Input arrays are not sorted");
    }

    // ========== MedianFinder ==========

    /**
     * Finds the running median efficiently using two heaps.
     * The lower half is stored in a max-heap, and the upper half in a min-heap.
     */
    public static class MedianFinder {
        private final MaxHeap<Integer> lower;
        private final MinHeap<Integer> upper;

        public MedianFinder() {
            this.lower = new MaxHeap<>();
            this.upper = new MinHeap<>();
        }

        /**
         * Adds a number to the data stream.
         */
        public void addNum(int num) {
            // Always add to lower first
            lower.push(num);
            // Balance: max of lower must be <= min of upper
            if (!upper.isEmpty() && lower.peek() > upper.peek()) {
                upper.push(lower.pop());
            }
            // Maintain size property: lower.size >= upper.size
            // and lower.size - upper.size <= 1
            if (lower.size() > upper.size() + 1) {
                upper.push(lower.pop());
            } else if (upper.size() > lower.size()) {
                lower.push(upper.pop());
            }
        }

        /**
         * Returns the current median.
         *
         * @throws NoSuchElementException if no numbers have been added
         */
        public double findMedian() {
            if (lower.isEmpty() && upper.isEmpty()) {
                throw new NoSuchElementException("No numbers added");
            }
            if (lower.size() > upper.size()) {
                return lower.peek();
            }
            return (lower.peek() + upper.peek()) / 2.0;
        }

        /**
         * Returns the total number of elements added.
         */
        public int size() {
            return lower.size() + upper.size();
        }
    }
}
