package dsa.algorithms;

import java.util.*;

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

    // ==================== Two-pointer techniques ====================

    public static int[] twoSumSorted(int[] arr, int target) {
        int left = 0, right = arr.length - 1;
        while (left < right) {
            int sum = arr[left] + arr[right];
            if (sum == target) {
                return new int[]{left, right};
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
        return new int[]{};
    }

    public static List<List<Integer>> threeSum(int[] nums) {
        List<List<Integer>> result = new ArrayList<>();
        int[] sorted = nums.clone();
        Arrays.sort(sorted);
        int n = sorted.length;
        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && sorted[i] == sorted[i - 1]) continue;
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = sorted[i] + sorted[left] + sorted[right];
                if (sum == 0) {
                    result.add(Arrays.asList(sorted[i], sorted[left], sorted[right]));
                    while (left < right && sorted[left] == sorted[left + 1]) left++;
                    while (left < right && sorted[right] == sorted[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        return result;
    }

    public static int containerWithMostWater(int[] height) {
        int left = 0, right = height.length - 1;
        int maxArea = 0;
        while (left < right) {
            int h = Math.min(height[left], height[right]);
            int area = h * (right - left);
            maxArea = Math.max(maxArea, area);
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        return maxArea;
    }

    public static int removeDuplicatesSorted(int[] arr) {
        if (arr.length == 0) return 0;
        int writeIdx = 1;
        for (int i = 1; i < arr.length; i++) {
            if (arr[i] != arr[i - 1]) {
                arr[writeIdx] = arr[i];
                writeIdx++;
            }
        }
        return writeIdx;
    }

    // ==================== Sliding window ====================

    public static int longestSubstringWithoutRepeating(String s) {
        Map<Character, Integer> lastSeen = new HashMap<>();
        int maxLen = 0;
        int start = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (lastSeen.containsKey(c) && lastSeen.get(c) >= start) {
                start = lastSeen.get(c) + 1;
            }
            lastSeen.put(c, i);
            maxLen = Math.max(maxLen, i - start + 1);
        }
        return maxLen;
    }

    public static String minWindowSubstring(String s, String t) {
        if (s.isEmpty() || t.isEmpty()) return "";
        Map<Character, Integer> need = new HashMap<>();
        for (char c : t.toCharArray()) {
            need.merge(c, 1, Integer::sum);
        }
        Map<Character, Integer> window = new HashMap<>();
        int have = 0, total = need.size();
        int start = 0;
        int minLen = Integer.MAX_VALUE;
        int minStart = 0;
        for (int end = 0; end < s.length(); end++) {
            char c = s.charAt(end);
            window.merge(c, 1, Integer::sum);
            if (need.containsKey(c) && window.get(c).intValue() == need.get(c).intValue()) {
                have++;
            }
            while (have == total) {
                int len = end - start + 1;
                if (len < minLen) {
                    minLen = len;
                    minStart = start;
                }
                char leftChar = s.charAt(start);
                window.merge(leftChar, -1, Integer::sum);
                if (need.containsKey(leftChar) && window.get(leftChar) < need.get(leftChar)) {
                    have--;
                }
                start++;
            }
        }
        return minLen == Integer.MAX_VALUE ? "" : s.substring(minStart, minStart + minLen);
    }

    public static List<Integer> findAllAnagrams(String s, String p) {
        List<Integer> result = new ArrayList<>();
        if (s.length() < p.length()) return result;
        int[] pCount = new int[26];
        int[] sCount = new int[26];
        for (char c : p.toCharArray()) {
            pCount[c - 'a']++;
        }
        for (int i = 0; i < s.length(); i++) {
            sCount[s.charAt(i) - 'a']++;
            if (i >= p.length()) {
                sCount[s.charAt(i - p.length()) - 'a']--;
            }
            if (i >= p.length() - 1 && Arrays.equals(pCount, sCount)) {
                result.add(i - p.length() + 1);
            }
        }
        return result;
    }

    public static int maxSumSubarray(int[] arr, int k) {
        if (arr.length < k || k <= 0) return 0;
        int windowSum = 0;
        for (int i = 0; i < k; i++) {
            windowSum += arr[i];
        }
        int maxSum = windowSum;
        for (int i = k; i < arr.length; i++) {
            windowSum += arr[i] - arr[i - k];
            maxSum = Math.max(maxSum, windowSum);
        }
        return maxSum;
    }

    public static int longestSubstringKDistinct(String s, int k) {
        if (k == 0 || s.isEmpty()) return 0;
        Map<Character, Integer> charCount = new HashMap<>();
        int maxLen = 0;
        int start = 0;
        for (int end = 0; end < s.length(); end++) {
            char c = s.charAt(end);
            charCount.merge(c, 1, Integer::sum);
            while (charCount.size() > k) {
                char leftChar = s.charAt(start);
                charCount.merge(leftChar, -1, Integer::sum);
                if (charCount.get(leftChar) == 0) {
                    charCount.remove(leftChar);
                }
                start++;
            }
            maxLen = Math.max(maxLen, end - start + 1);
        }
        return maxLen;
    }

    // ==================== Binary search on answer ====================

    public static int findRotationPoint(int[] arr) {
        if (arr.length == 0) return -1;
        int low = 0, high = arr.length - 1;
        if (arr[low] <= arr[high]) return 0; // not rotated
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (mid < arr.length - 1 && arr[mid] > arr[mid + 1]) {
                return mid + 1;
            }
            if (mid > 0 && arr[mid - 1] > arr[mid]) {
                return mid;
            }
            if (arr[low] <= arr[mid]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return 0;
    }

    public static int sqrtInt(int n) {
        if (n < 0) throw new IllegalArgumentException("Cannot compute square root of negative number");
        if (n == 0) return 0;
        long low = 1, high = n;
        long result = 0;
        while (low <= high) {
            long mid = low + (high - low) / 2;
            if (mid * mid <= n) {
                result = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return (int) result;
    }
}
