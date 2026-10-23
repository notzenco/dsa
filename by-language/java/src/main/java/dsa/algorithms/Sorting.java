package dsa.algorithms;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class Sorting {

    private Sorting() {}

    public static int[] bubbleSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        for (int i = 0; i < n - 1; i++) {
            boolean swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (a[j] > a[j + 1]) {
                    int tmp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = tmp;
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
        return a;
    }

    public static int[] selectionSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                if (a[j] < a[minIdx]) {
                    minIdx = j;
                }
            }
            int tmp = a[i];
            a[i] = a[minIdx];
            a[minIdx] = tmp;
        }
        return a;
    }

    public static int[] insertionSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        for (int i = 1; i < n; i++) {
            int key = a[i];
            int j = i - 1;
            while (j >= 0 && a[j] > key) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = key;
        }
        return a;
    }

    public static int[] mergeSort(int[] arr) {
        int[] a = arr.clone();
        if (a.length <= 1) return a;
        mergeSortHelper(a, 0, a.length - 1);
        return a;
    }

    private static void mergeSortHelper(int[] a, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSortHelper(a, left, mid);
        mergeSortHelper(a, mid + 1, right);
        merge(a, left, mid, right);
    }

    private static void merge(int[] a, int left, int mid, int right) {
        int[] leftArr = Arrays.copyOfRange(a, left, mid + 1);
        int[] rightArr = Arrays.copyOfRange(a, mid + 1, right + 1);
        int i = 0, j = 0, k = left;
        while (i < leftArr.length && j < rightArr.length) {
            if (leftArr[i] <= rightArr[j]) {
                a[k++] = leftArr[i++];
            } else {
                a[k++] = rightArr[j++];
            }
        }
        while (i < leftArr.length) a[k++] = leftArr[i++];
        while (j < rightArr.length) a[k++] = rightArr[j++];
    }

    public static int[] quickSort(int[] arr) {
        int[] a = arr.clone();
        if (a.length <= 1) return a;
        quickSortHelper(a, 0, a.length - 1);
        return a;
    }

    private static void quickSortHelper(int[] a, int low, int high) {
        if (low >= high) return;
        int pivot = partition(a, low, high);
        quickSortHelper(a, low, pivot - 1);
        quickSortHelper(a, pivot + 1, high);
    }

    private static int partition(int[] a, int low, int high) {
        // Median-of-three pivot selection for better average performance
        int mid = low + (high - low) / 2;
        if (a[mid] < a[low]) { int t = a[low]; a[low] = a[mid]; a[mid] = t; }
        if (a[high] < a[low]) { int t = a[low]; a[low] = a[high]; a[high] = t; }
        if (a[mid] < a[high]) { int t = a[mid]; a[mid] = a[high]; a[high] = t; }
        int pivot = a[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (a[j] <= pivot) {
                i++;
                int tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
        int tmp = a[i + 1];
        a[i + 1] = a[high];
        a[high] = tmp;
        return i + 1;
    }

    public static int[] heapSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        // Build max heap
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(a, n, i);
        }
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; i--) {
            int tmp = a[0];
            a[0] = a[i];
            a[i] = tmp;
            heapify(a, i, 0);
        }
        return a;
    }

    private static void heapify(int[] a, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && a[left] > a[largest]) largest = left;
        if (right < n && a[right] > a[largest]) largest = right;
        if (largest != i) {
            int tmp = a[i];
            a[i] = a[largest];
            a[largest] = tmp;
            heapify(a, n, largest);
        }
    }

    public static int[] radixSort(int[] arr) {
        int[] a = arr.clone();
        if (a.length <= 1) return a;
        int max = a[0];
        for (int v : a) {
            if (v > max) max = v;
        }
        for (int exp = 1; max / exp > 0; exp *= 10) {
            countingSortByDigit(a, exp);
        }
        return a;
    }

    private static void countingSortByDigit(int[] a, int exp) {
        int n = a.length;
        int[] output = new int[n];
        int[] count = new int[10];
        for (int v : a) {
            count[(v / exp) % 10]++;
        }
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }
        for (int i = n - 1; i >= 0; i--) {
            int digit = (a[i] / exp) % 10;
            output[count[digit] - 1] = a[i];
            count[digit]--;
        }
        System.arraycopy(output, 0, a, 0, n);
    }

    public static int[] countingSort(int[] arr) {
        int[] a = arr.clone();
        if (a.length <= 1) return a;
        int max = a[0];
        for (int v : a) {
            if (v > max) max = v;
        }
        int[] count = new int[max + 1];
        for (int v : a) {
            count[v]++;
        }
        int idx = 0;
        for (int i = 0; i <= max; i++) {
            while (count[i] > 0) {
                a[idx++] = i;
                count[i]--;
            }
        }
        return a;
    }

    public static int[] shellSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                int key = a[i];
                int j = i;
                while (j >= gap && a[j - gap] > key) {
                    a[j] = a[j - gap];
                    j -= gap;
                }
                a[j] = key;
            }
        }
        return a;
    }

    public static boolean isSorted(int[] arr) {
        for (int i = 1; i < arr.length; i++) {
            if (arr[i] < arr[i - 1]) return false;
        }
        return true;
    }

    public static int[] bucketSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        if (n <= 1) return a;
        int min = a[0], max = a[0];
        for (int v : a) {
            if (v < min) min = v;
            if (v > max) max = v;
        }
        if (min == max) return a;
        int bucketCount = (int) Math.max(1, Math.sqrt(n));
        @SuppressWarnings("unchecked")
        List<Integer>[] buckets = new List[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            buckets[i] = new ArrayList<>();
        }
        for (int v : a) {
            int idx = (int) (((long) v - min) * (bucketCount - 1)
                    / ((long) max - min));
            buckets[idx].add(v);
        }
        // Insertion sort each bucket
        int pos = 0;
        for (List<Integer> bucket : buckets) {
            int size = bucket.size();
            int[] b = new int[size];
            for (int i = 0; i < size; i++) b[i] = bucket.get(i);
            // Insertion sort on b
            for (int i = 1; i < size; i++) {
                int key = b[i];
                int j = i - 1;
                while (j >= 0 && b[j] > key) {
                    b[j + 1] = b[j];
                    j--;
                }
                b[j + 1] = key;
            }
            for (int v : b) {
                a[pos++] = v;
            }
        }
        return a;
    }

    private static final int TIM_MIN_RUN = 32;

    public static int[] timSort(int[] arr) {
        int[] a = arr.clone();
        int n = a.length;
        if (n <= 1) return a;

        // Insertion sort individual runs of TIM_MIN_RUN
        for (int i = 0; i < n; i += TIM_MIN_RUN) {
            int end = Math.min(i + TIM_MIN_RUN - 1, n - 1);
            // Insertion sort from i to end
            for (int j = i + 1; j <= end; j++) {
                int key = a[j];
                int k = j - 1;
                while (k >= i && a[k] > key) {
                    a[k + 1] = a[k];
                    k--;
                }
                a[k + 1] = key;
            }
        }

        // Merge runs, doubling size each iteration
        for (int size = TIM_MIN_RUN; size < n; size *= 2) {
            for (int left = 0; left < n; left += 2 * size) {
                int mid = Math.min(left + size - 1, n - 1);
                int right = Math.min(left + 2 * size - 1, n - 1);
                if (mid < right) {
                    merge(a, left, mid, right);
                }
            }
        }
        return a;
    }

    public static int quickSelect(int[] arr, int k) {
        if (k < 0 || k >= arr.length) {
            throw new IllegalArgumentException("k is out of bounds");
        }
        int[] a = arr.clone();
        return quickSelectHelper(a, 0, a.length - 1, k);
    }

    private static int quickSelectHelper(int[] a, int low, int high, int k) {
        if (low == high) return a[low];
        int pivotIdx = partition(a, low, high);
        if (k == pivotIdx) {
            return a[k];
        } else if (k < pivotIdx) {
            return quickSelectHelper(a, low, pivotIdx - 1, k);
        } else {
            return quickSelectHelper(a, pivotIdx + 1, high, k);
        }
    }

    public static int[] stableSort(int[] arr) {
        return mergeSort(arr);
    }

    public static boolean isStable(String algorithmName) {
        switch (algorithmName.toLowerCase()) {
            case "bubble":
            case "insertion":
            case "merge":
            case "counting":
            case "tim":
                return true;
            case "selection":
            case "quick":
            case "heap":
            case "shell":
                return false;
            default:
                throw new IllegalArgumentException("Unknown algorithm: " + algorithmName);
        }
    }
}
