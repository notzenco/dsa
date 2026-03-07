package dsa.algorithms;

public final class Searching {

    private Searching() {}

    public static int linearSearch(int[] arr, int target) {
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] == target) return i;
        }
        return -1;
    }

    public static int binarySearch(int[] arr, int target) {
        int low = 0, high = arr.length - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) return mid;
            else if (arr[mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    public static int binarySearchRecursive(int[] arr, int target) {
        return binarySearchRecursiveHelper(arr, target, 0, arr.length - 1);
    }

    private static int binarySearchRecursiveHelper(int[] arr, int target, int low, int high) {
        if (low > high) return -1;
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) return binarySearchRecursiveHelper(arr, target, mid + 1, high);
        else return binarySearchRecursiveHelper(arr, target, low, mid - 1);
    }

    public static int lowerBound(int[] arr, int target) {
        int low = 0, high = arr.length;
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] < target) low = mid + 1;
            else high = mid;
        }
        return low;
    }

    public static int upperBound(int[] arr, int target) {
        int low = 0, high = arr.length;
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] <= target) low = mid + 1;
            else high = mid;
        }
        return low;
    }

    public static int jumpSearch(int[] arr, int target) {
        int n = arr.length;
        if (n == 0) return -1;
        int step = (int) Math.floor(Math.sqrt(n));
        int prev = 0;
        while (prev < n && arr[Math.min(prev + step, n) - 1] < target) {
            prev += step;
            if (prev >= n) return -1;
        }
        // Linear search in the block
        for (int i = prev; i < Math.min(prev + step, n); i++) {
            if (arr[i] == target) return i;
        }
        return -1;
    }

    public static int interpolationSearch(int[] arr, int target) {
        int low = 0, high = arr.length - 1;
        while (low <= high && target >= arr[low] && target <= arr[high]) {
            if (low == high) {
                return arr[low] == target ? low : -1;
            }
            int pos = low + ((target - arr[low]) * (high - low)) / (arr[high] - arr[low]);
            if (pos < low || pos > high) return -1;
            if (arr[pos] == target) return pos;
            else if (arr[pos] < target) low = pos + 1;
            else high = pos - 1;
        }
        return -1;
    }

    public static int exponentialSearch(int[] arr, int target) {
        int n = arr.length;
        if (n == 0) return -1;
        if (arr[0] == target) return 0;
        int bound = 1;
        while (bound < n && arr[bound] <= target) {
            bound *= 2;
        }
        // Binary search within the range [bound/2, min(bound, n-1)]
        int low = bound / 2;
        int high = Math.min(bound, n - 1);
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) return mid;
            else if (arr[mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    public static int ternarySearch(int[] arr, int target) {
        return ternarySearchHelper(arr, target, 0, arr.length - 1);
    }

    private static int ternarySearchHelper(int[] arr, int target, int low, int high) {
        if (low > high) return -1;
        int third = (high - low) / 3;
        int mid1 = low + third;
        int mid2 = high - third;
        if (arr[mid1] == target) return mid1;
        if (arr[mid2] == target) return mid2;
        if (target < arr[mid1]) return ternarySearchHelper(arr, target, low, mid1 - 1);
        else if (target > arr[mid2]) return ternarySearchHelper(arr, target, mid2 + 1, high);
        else return ternarySearchHelper(arr, target, mid1 + 1, mid2 - 1);
    }

    public static int findFirstOccurrence(int[] arr, int target) {
        int low = 0, high = arr.length - 1;
        int result = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) {
                result = mid;
                high = mid - 1;
            } else if (arr[mid] < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return result;
    }

    public static int findLastOccurrence(int[] arr, int target) {
        int low = 0, high = arr.length - 1;
        int result = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) {
                result = mid;
                low = mid + 1;
            } else if (arr[mid] < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return result;
    }

    public static int countOccurrences(int[] arr, int target) {
        int first = findFirstOccurrence(arr, target);
        if (first == -1) return 0;
        int last = findLastOccurrence(arr, target);
        return last - first + 1;
    }

    public static int searchInsertPosition(int[] arr, int target) {
        int low = 0, high = arr.length;
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] < target) low = mid + 1;
            else high = mid;
        }
        return low;
    }

    public static int findPeakElement(int[] arr) {
        int n = arr.length;
        if (n == 0) return -1;
        if (n == 1) return 0;
        int low = 0, high = n - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            boolean greaterThanLeft = (mid == 0 || arr[mid] > arr[mid - 1]);
            boolean greaterThanRight = (mid == n - 1 || arr[mid] > arr[mid + 1]);
            if (greaterThanLeft && greaterThanRight) return mid;
            if (mid > 0 && arr[mid - 1] > arr[mid]) high = mid - 1;
            else low = mid + 1;
        }
        return -1;
    }

    public static int searchInRotatedArray(int[] arr, int target) {
        int low = 0, high = arr.length - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) return mid;
            // Left half is sorted
            if (arr[low] <= arr[mid]) {
                if (target >= arr[low] && target < arr[mid]) {
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }
            // Right half is sorted
            else {
                if (target > arr[mid] && target <= arr[high]) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
        }
        return -1;
    }
}
