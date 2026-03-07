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
}
