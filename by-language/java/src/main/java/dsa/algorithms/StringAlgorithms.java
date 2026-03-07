package dsa.algorithms;

import java.util.*;

public final class StringAlgorithms {

    private StringAlgorithms() {}

    public static List<Integer> naiveSearch(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty()) return result;
        int n = text.length(), m = pattern.length();
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            while (j < m && text.charAt(i + j) == pattern.charAt(j)) {
                j++;
            }
            if (j == m) result.add(i);
        }
        return result;
    }

    public static List<Integer> kmpSearch(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty()) return result;
        int[] lps = computeLPSArray(pattern);
        int n = text.length(), m = pattern.length();
        int i = 0, j = 0;
        while (i < n) {
            if (text.charAt(i) == pattern.charAt(j)) {
                i++;
                j++;
            }
            if (j == m) {
                result.add(i - j);
                j = lps[j - 1];
            } else if (i < n && text.charAt(i) != pattern.charAt(j)) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        return result;
    }

    public static int[] computeLPSArray(String pattern) {
        int m = pattern.length();
        int[] lps = new int[m];
        int len = 0;
        int i = 1;
        while (i < m) {
            if (pattern.charAt(i) == pattern.charAt(len)) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
        return lps;
    }

    public static List<Integer> rabinKarp(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty()) return result;
        int n = text.length(), m = pattern.length();
        if (m > n) return result;
        long prime = 1_000_000_007L;
        long base = 256;
        // Compute hash of pattern and first window of text
        long patternHash = 0, textHash = 0;
        long highPow = 1; // base^(m-1) mod prime
        for (int i = 0; i < m - 1; i++) {
            highPow = (highPow * base) % prime;
        }
        for (int i = 0; i < m; i++) {
            patternHash = (patternHash * base + pattern.charAt(i)) % prime;
            textHash = (textHash * base + text.charAt(i)) % prime;
        }
        for (int i = 0; i <= n - m; i++) {
            if (patternHash == textHash) {
                // Verify character by character
                boolean match = true;
                for (int j = 0; j < m; j++) {
                    if (text.charAt(i + j) != pattern.charAt(j)) {
                        match = false;
                        break;
                    }
                }
                if (match) result.add(i);
            }
            // Compute hash for next window
            if (i < n - m) {
                textHash = ((textHash - text.charAt(i) * highPow % prime + prime) * base
                        + text.charAt(i + m)) % prime;
            }
        }
        return result;
    }

    public static int[] computeZArray(String s) {
        int n = s.length();
        int[] z = new int[n];
        if (n == 0) return z;
        z[0] = n;
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i < r) {
                z[i] = Math.min(r - i, z[i - l]);
            }
            while (i + z[i] < n && s.charAt(z[i]) == s.charAt(i + z[i])) {
                z[i]++;
            }
            if (i + z[i] > r) {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }

    public static List<Integer> zAlgorithm(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty()) return result;
        String concat = pattern + "$" + text;
        int[] z = computeZArray(concat);
        int m = pattern.length();
        for (int i = m + 1; i < concat.length(); i++) {
            if (z[i] == m) {
                result.add(i - m - 1);
            }
        }
        return result;
    }

    public static String manacher(String s) {
        if (s == null || s.isEmpty()) return "";
        // Transform: "abc" -> "^#a#b#c#$"
        StringBuilder t = new StringBuilder("^");
        for (int i = 0; i < s.length(); i++) {
            t.append('#').append(s.charAt(i));
        }
        t.append("#$");
        String transformed = t.toString();
        int n = transformed.length();
        int[] p = new int[n];
        int center = 0, right = 0;
        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * center - i;
            if (i < right) {
                p[i] = Math.min(right - i, p[mirror]);
            }
            while (transformed.charAt(i + p[i] + 1) == transformed.charAt(i - p[i] - 1)) {
                p[i]++;
            }
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }
        // Find the maximum element in p
        int maxLen = 0, maxCenter = 0;
        for (int i = 1; i < n - 1; i++) {
            if (p[i] > maxLen) {
                maxLen = p[i];
                maxCenter = i;
            }
        }
        int start = (maxCenter - maxLen - 1) / 2;
        return s.substring(start, start + maxLen);
    }

    public static int[] buildSuffixArray(String s) {
        int n = s.length();
        Integer[] order = new Integer[n];
        for (int i = 0; i < n; i++) order[i] = i;
        // Simple O(n log^2 n) construction
        int[] rank = new int[n];
        int[] tmp = new int[n];
        for (int i = 0; i < n; i++) rank[i] = s.charAt(i);

        for (int gap = 1; gap < n; gap *= 2) {
            final int g = gap;
            final int[] r = rank;
            Comparator<Integer> cmp = (a, b) -> {
                if (r[a] != r[b]) return Integer.compare(r[a], r[b]);
                int ra = a + g < n ? r[a + g] : -1;
                int rb = b + g < n ? r[b + g] : -1;
                return Integer.compare(ra, rb);
            };
            Arrays.sort(order, cmp);

            tmp[order[0]] = 0;
            for (int i = 1; i < n; i++) {
                tmp[order[i]] = tmp[order[i - 1]] + (cmp.compare(order[i], order[i - 1]) != 0 ? 1 : 0);
            }
            System.arraycopy(tmp, 0, rank, 0, n);
        }
        int[] sa = new int[n];
        for (int i = 0; i < n; i++) sa[i] = order[i];
        return sa;
    }

    public static int levenshteinDistance(String s1, String s2) {
        int m = s1.length(), n = s2.length();
        int[][] dp = new int[m + 1][n + 1];
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (s1.charAt(i - 1) == s2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + Math.min(dp[i - 1][j - 1],
                                   Math.min(dp[i - 1][j], dp[i][j - 1]));
                }
            }
        }
        return dp[m][n];
    }

    public static String longestCommonPrefix(String[] strs) {
        if (strs == null || strs.length == 0) return "";
        String prefix = strs[0];
        for (int i = 1; i < strs.length; i++) {
            while (!strs[i].startsWith(prefix)) {
                prefix = prefix.substring(0, prefix.length() - 1);
                if (prefix.isEmpty()) return "";
            }
        }
        return prefix;
    }

    public static boolean repeatedSubstringPattern(String s) {
        if (s == null || s.length() <= 1) return false;
        String doubled = s + s;
        // Remove first and last character, then check if s is in the result
        String sub = doubled.substring(1, doubled.length() - 1);
        return sub.contains(s);
    }

    public static List<Integer> boyerMoore(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty() || text.length() < pattern.length()) return result;
        int n = text.length(), m = pattern.length();
        // Build bad character table
        int[] badChar = new int[256];
        Arrays.fill(badChar, -1);
        for (int i = 0; i < m; i++) {
            badChar[pattern.charAt(i)] = i;
        }
        int shift = 0;
        while (shift <= n - m) {
            int j = m - 1;
            while (j >= 0 && pattern.charAt(j) == text.charAt(shift + j)) {
                j--;
            }
            if (j < 0) {
                result.add(shift);
                shift += (shift + m < n) ? m - badChar[text.charAt(shift + m)] : 1;
            } else {
                int bc = badChar[text.charAt(shift + j)];
                shift += Math.max(1, j - bc);
            }
        }
        return result;
    }

    public static String shortestPalindrome(String s) {
        if (s == null || s.length() <= 1) return s == null ? "" : s;
        String rev = new StringBuilder(s).reverse().toString();
        String concat = s + "#" + rev;
        int[] lps = computeLPSArray(concat);
        int longestPrefixPalin = lps[concat.length() - 1];
        String suffix = s.substring(longestPrefixPalin);
        return new StringBuilder(suffix).reverse().toString() + s;
    }

    public static String longestHappyPrefix(String s) {
        if (s == null || s.length() <= 1) return "";
        int[] lps = computeLPSArray(s);
        int len = lps[s.length() - 1];
        return s.substring(0, len);
    }

    public static int countPalindromicSubstrings(String s) {
        if (s == null || s.isEmpty()) return 0;
        int count = 0;
        int n = s.length();
        for (int center = 0; center < n; center++) {
            // Odd length palindromes
            int left = center, right = center;
            while (left >= 0 && right < n && s.charAt(left) == s.charAt(right)) {
                count++;
                left--;
                right++;
            }
            // Even length palindromes
            left = center;
            right = center + 1;
            while (left >= 0 && right < n && s.charAt(left) == s.charAt(right)) {
                count++;
                left--;
                right++;
            }
        }
        return count;
    }

    public static int[] computeLCPArray(String s, int[] suffixArray) {
        int n = s.length();
        int[] rank = new int[n];
        int[] lcp = new int[n];
        for (int i = 0; i < n; i++) {
            rank[suffixArray[i]] = i;
        }
        int k = 0;
        for (int i = 0; i < n; i++) {
            if (rank[i] == 0) {
                k = 0;
                continue;
            }
            int j = suffixArray[rank[i] - 1];
            while (i + k < n && j + k < n && s.charAt(i + k) == s.charAt(j + k)) {
                k++;
            }
            lcp[rank[i]] = k;
            if (k > 0) k--;
        }
        return lcp;
    }

    public static List<Integer> searchWithSuffixArray(String text, String pattern) {
        List<Integer> result = new ArrayList<>();
        if (pattern.isEmpty() || text.isEmpty()) return result;
        int[] sa = buildSuffixArray(text);
        int n = text.length(), m = pattern.length();
        // Binary search for lower bound
        int lo = 0, hi = n - 1;
        int first = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int cmp = compareSubstring(text, sa[mid], pattern);
            if (cmp >= 0) {
                if (cmp == 0) first = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        if (first == n) return result;
        // Binary search for upper bound
        lo = first;
        hi = n - 1;
        int last = first;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int cmp = compareSubstring(text, sa[mid], pattern);
            if (cmp <= 0) {
                if (cmp == 0) last = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        for (int i = first; i <= last; i++) {
            result.add(sa[i]);
        }
        Collections.sort(result);
        return result;
    }

    private static int compareSubstring(String text, int suffixStart, String pattern) {
        int n = text.length(), m = pattern.length();
        int len = Math.min(n - suffixStart, m);
        for (int i = 0; i < len; i++) {
            if (text.charAt(suffixStart + i) != pattern.charAt(i)) {
                return Character.compare(text.charAt(suffixStart + i), pattern.charAt(i));
            }
        }
        if (n - suffixStart < m) return -1;
        return 0;
    }

    public static List<Integer> rabinKarpMultiple(String text, String[] patterns) {
        Set<Integer> resultSet = new TreeSet<>();
        if (text.isEmpty() || patterns.length == 0) return new ArrayList<>(resultSet);
        long prime = 1_000_000_007L;
        long base = 256;
        // Group patterns by length
        Map<Integer, List<String>> byLength = new HashMap<>();
        for (String p : patterns) {
            if (!p.isEmpty() && p.length() <= text.length()) {
                byLength.computeIfAbsent(p.length(), k -> new ArrayList<>()).add(p);
            }
        }
        for (Map.Entry<Integer, List<String>> entry : byLength.entrySet()) {
            int m = entry.getKey();
            List<String> pats = entry.getValue();
            // Compute hashes for all patterns of this length
            Set<Long> patHashes = new HashSet<>();
            Set<String> patSet = new HashSet<>(pats);
            for (String p : pats) {
                long h = 0;
                for (int i = 0; i < m; i++) {
                    h = (h * base + p.charAt(i)) % prime;
                }
                patHashes.add(h);
            }
            long highPow = 1;
            for (int i = 0; i < m - 1; i++) {
                highPow = (highPow * base) % prime;
            }
            long textHash = 0;
            for (int i = 0; i < m; i++) {
                textHash = (textHash * base + text.charAt(i)) % prime;
            }
            for (int i = 0; i <= text.length() - m; i++) {
                if (patHashes.contains(textHash)) {
                    String window = text.substring(i, i + m);
                    if (patSet.contains(window)) {
                        resultSet.add(i);
                    }
                }
                if (i < text.length() - m) {
                    textHash = ((textHash - text.charAt(i) * highPow % prime + prime) * base
                            + text.charAt(i + m)) % prime;
                }
            }
        }
        return new ArrayList<>(resultSet);
    }

    public static boolean isSubsequence(String s, String t) {
        int si = 0, ti = 0;
        while (si < s.length() && ti < t.length()) {
            if (s.charAt(si) == t.charAt(ti)) {
                si++;
            }
            ti++;
        }
        return si == s.length();
    }

    public static int longestRepeatingSubstring(String s) {
        if (s == null || s.length() <= 1) return 0;
        int n = s.length();
        int lo = 1, hi = n - 1;
        int result = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (hasRepeatingSubstring(s, mid)) {
                result = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return result;
    }

    private static boolean hasRepeatingSubstring(String s, int len) {
        long prime = 1_000_000_007L;
        long base = 31;
        long hash = 0;
        long highPow = 1;
        for (int i = 0; i < len - 1; i++) {
            highPow = (highPow * base) % prime;
        }
        for (int i = 0; i < len; i++) {
            hash = (hash * base + s.charAt(i)) % prime;
        }
        Map<Long, List<Integer>> seen = new HashMap<>();
        seen.computeIfAbsent(hash, k -> new ArrayList<>()).add(0);
        for (int i = 1; i <= s.length() - len; i++) {
            hash = ((hash - s.charAt(i - 1) * highPow % prime + prime) * base
                    + s.charAt(i + len - 1)) % prime;
            List<Integer> positions = seen.get(hash);
            if (positions != null) {
                String candidate = s.substring(i, i + len);
                for (int pos : positions) {
                    if (s.substring(pos, pos + len).equals(candidate)) {
                        return true;
                    }
                }
            }
            seen.computeIfAbsent(hash, k -> new ArrayList<>()).add(i);
        }
        return false;
    }
}
