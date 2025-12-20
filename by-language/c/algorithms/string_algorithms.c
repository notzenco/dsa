/**
 * String Algorithms Implementation
 * See string_algorithms.h for documentation
 */

#include "string_algorithms.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PRIME 101
#define BASE 256

/* ============== Pattern Matching ============== */

int naive_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return -1;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    for (size_t i = 0; i <= n - m; i++) {
        size_t j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == m) return (int)i;
    }
    return -1;
}

int naive_search_all(const char *text, const char *pattern,
                     int *indices, int max_results) {
    if (text == NULL || pattern == NULL || indices == NULL) return 0;

    size_t n = strlen(text);
    size_t m = strlen(pattern);
    int count = 0;

    if (m == 0 || m > n) return 0;

    for (size_t i = 0; i <= n - m && count < max_results; i++) {
        size_t j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == m) {
            indices[count++] = (int)i;
        }
    }
    return count;
}

void kmp_build_lps(const char *pattern, int *lps, size_t m) {
    if (pattern == NULL || lps == NULL || m == 0) return;

    lps[0] = 0;
    size_t len = 0;
    size_t i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = (int)len;
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
}

int kmp_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return -1;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    int *lps = malloc(m * sizeof(int));
    if (lps == NULL) return -1;

    kmp_build_lps(pattern, lps, m);

    size_t i = 0, j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            free(lps);
            return (int)(i - j);
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return -1;
}

int kmp_search_all(const char *text, const char *pattern,
                   int *indices, int max_results) {
    if (text == NULL || pattern == NULL || indices == NULL) return 0;

    size_t n = strlen(text);
    size_t m = strlen(pattern);
    int count = 0;

    if (m == 0 || m > n) return 0;

    int *lps = malloc(m * sizeof(int));
    if (lps == NULL) return 0;

    kmp_build_lps(pattern, lps, m);

    size_t i = 0, j = 0;
    while (i < n && count < max_results) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            indices[count++] = (int)(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return count;
}

int rabin_karp_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return -1;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    /* Calculate hash of pattern and first window */
    long long p_hash = 0;
    long long t_hash = 0;
    long long h = 1;

    /* h = BASE^(m-1) % PRIME */
    for (size_t i = 0; i < m - 1; i++) {
        h = (h * BASE) % PRIME;
    }

    /* Initial hash values */
    for (size_t i = 0; i < m; i++) {
        p_hash = (BASE * p_hash + pattern[i]) % PRIME;
        t_hash = (BASE * t_hash + text[i]) % PRIME;
    }

    /* Slide pattern over text */
    for (size_t i = 0; i <= n - m; i++) {
        if (p_hash == t_hash) {
            /* Check character by character */
            size_t j;
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) break;
            }
            if (j == m) return (int)i;
        }

        /* Calculate hash for next window */
        if (i < n - m) {
            t_hash = (BASE * (t_hash - text[i] * h) + text[i + m]) % PRIME;
            if (t_hash < 0) t_hash += PRIME;
        }
    }

    return -1;
}

int rabin_karp_search_all(const char *text, const char *pattern,
                          int *indices, int max_results) {
    if (text == NULL || pattern == NULL || indices == NULL) return 0;

    size_t n = strlen(text);
    size_t m = strlen(pattern);
    int count = 0;

    if (m == 0 || m > n) return 0;

    long long p_hash = 0;
    long long t_hash = 0;
    long long h = 1;

    for (size_t i = 0; i < m - 1; i++) {
        h = (h * BASE) % PRIME;
    }

    for (size_t i = 0; i < m; i++) {
        p_hash = (BASE * p_hash + pattern[i]) % PRIME;
        t_hash = (BASE * t_hash + text[i]) % PRIME;
    }

    for (size_t i = 0; i <= n - m && count < max_results; i++) {
        if (p_hash == t_hash) {
            size_t j;
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) break;
            }
            if (j == m) {
                indices[count++] = (int)i;
            }
        }

        if (i < n - m) {
            t_hash = (BASE * (t_hash - text[i] * h) + text[i + m]) % PRIME;
            if (t_hash < 0) t_hash += PRIME;
        }
    }

    return count;
}

void z_function(const char *s, int *z, size_t n) {
    if (s == NULL || z == NULL || n == 0) return;

    z[0] = 0;
    size_t l = 0, r = 0;

    for (size_t i = 1; i < n; i++) {
        if (i < r) {
            z[i] = (int)((r - i < (size_t)z[i - l]) ? (r - i) : z[i - l]);
        } else {
            z[i] = 0;
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }

        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
}

int z_search(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return -1;

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    /* Concatenate: pattern + '$' + text */
    size_t concat_len = m + 1 + n;
    char *concat = malloc(concat_len + 1);
    if (concat == NULL) return -1;

    memcpy(concat, pattern, m);
    concat[m] = '$';
    memcpy(concat + m + 1, text, n + 1);

    int *z = calloc(concat_len, sizeof(int));
    if (z == NULL) {
        free(concat);
        return -1;
    }

    z_function(concat, z, concat_len);

    int result = -1;
    for (size_t i = m + 1; i < concat_len; i++) {
        if ((size_t)z[i] == m) {
            result = (int)(i - m - 1);
            break;
        }
    }

    free(concat);
    free(z);
    return result;
}

int z_search_all(const char *text, const char *pattern,
                 int *indices, int max_results) {
    if (text == NULL || pattern == NULL || indices == NULL) return 0;

    size_t n = strlen(text);
    size_t m = strlen(pattern);
    int count = 0;

    if (m == 0 || m > n) return 0;

    size_t concat_len = m + 1 + n;
    char *concat = malloc(concat_len + 1);
    if (concat == NULL) return 0;

    memcpy(concat, pattern, m);
    concat[m] = '$';
    memcpy(concat + m + 1, text, n + 1);

    int *z = calloc(concat_len, sizeof(int));
    if (z == NULL) {
        free(concat);
        return 0;
    }

    z_function(concat, z, concat_len);

    for (size_t i = m + 1; i < concat_len && count < max_results; i++) {
        if ((size_t)z[i] == m) {
            indices[count++] = (int)(i - m - 1);
        }
    }

    free(concat);
    free(z);
    return count;
}

/* ============== String Properties ============== */

bool is_palindrome(const char *s) {
    if (s == NULL) return true;

    size_t len = strlen(s);
    if (len <= 1) return true;

    size_t left = 0, right = len - 1;
    while (left < right) {
        if (s[left] != s[right]) return false;
        left++;
        right--;
    }
    return true;
}

bool is_repeated_pattern(const char *s) {
    if (s == NULL) return false;

    size_t n = strlen(s);
    if (n == 0) return false;

    int *lps = malloc(n * sizeof(int));
    if (lps == NULL) return false;

    kmp_build_lps(s, lps, n);

    int len = lps[n - 1];
    free(lps);

    /* If pattern length divides n and is not n itself */
    return len > 0 && n % (n - len) == 0;
}

int longest_happy_prefix(const char *s) {
    if (s == NULL) return 0;

    size_t n = strlen(s);
    if (n == 0) return 0;

    int *lps = malloc(n * sizeof(int));
    if (lps == NULL) return 0;

    kmp_build_lps(s, lps, n);

    int result = lps[n - 1];
    free(lps);
    return result;
}

void shortest_palindrome(const char *s, char *result) {
    if (s == NULL || result == NULL) {
        if (result) result[0] = '\0';
        return;
    }

    size_t n = strlen(s);
    if (n == 0) {
        result[0] = '\0';
        return;
    }

    /* Create s + '#' + reverse(s) */
    char *rev = malloc(n + 1);
    if (rev == NULL) {
        strcpy(result, s);
        return;
    }

    for (size_t i = 0; i < n; i++) {
        rev[i] = s[n - 1 - i];
    }
    rev[n] = '\0';

    size_t concat_len = 2 * n + 1;
    char *concat = malloc(concat_len + 1);
    if (concat == NULL) {
        free(rev);
        strcpy(result, s);
        return;
    }

    memcpy(concat, s, n);
    concat[n] = '#';
    memcpy(concat + n + 1, rev, n + 1);

    int *lps = malloc(concat_len * sizeof(int));
    if (lps == NULL) {
        free(rev);
        free(concat);
        strcpy(result, s);
        return;
    }

    kmp_build_lps(concat, lps, concat_len);

    int prefix_len = lps[concat_len - 1];

    /* Prepend reverse of suffix */
    size_t add_len = n - prefix_len;
    for (size_t i = 0; i < add_len; i++) {
        result[i] = rev[i];
    }
    strcpy(result + add_len, s);

    free(rev);
    free(concat);
    free(lps);
}

/* ============== String Transformation ============== */

void string_reverse(char *s) {
    if (s == NULL) return;

    size_t len = strlen(s);
    if (len <= 1) return;

    size_t left = 0, right = len - 1;
    while (left < right) {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left++;
        right--;
    }
}

static void reverse_range(char *s, size_t start, size_t end) {
    while (start < end) {
        char temp = s[start];
        s[start] = s[end];
        s[end] = temp;
        start++;
        end--;
    }
}

void reverse_words(char *s) {
    if (s == NULL) return;

    size_t len = strlen(s);
    if (len == 0) return;

    /* Reverse entire string */
    reverse_range(s, 0, len - 1);

    /* Reverse each word */
    size_t start = 0;
    for (size_t i = 0; i <= len; i++) {
        if (s[i] == ' ' || s[i] == '\0') {
            if (i > start) {
                reverse_range(s, start, i - 1);
            }
            start = i + 1;
        }
    }
}

bool are_anagrams(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return true;
    if (s1 == NULL || s2 == NULL) return false;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) return false;

    int count[256] = {0};

    for (size_t i = 0; i < len1; i++) {
        count[(unsigned char)s1[i]]++;
        count[(unsigned char)s2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) return false;
    }
    return true;
}

bool is_rotation(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return true;
    if (s1 == NULL || s2 == NULL) return false;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2) return false;
    if (len1 == 0) return true;

    /* s2 is rotation of s1 iff s2 is substring of s1+s1 */
    char *doubled = malloc(2 * len1 + 1);
    if (doubled == NULL) return false;

    memcpy(doubled, s1, len1);
    memcpy(doubled + len1, s1, len1 + 1);

    bool result = kmp_search(doubled, s2) != -1;
    free(doubled);
    return result;
}

/* ============== Utility ============== */

int count_pattern(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return 0;

    size_t m = strlen(pattern);
    if (m == 0) return 0;

    int count = 0;
    int indices[1000];
    count = kmp_search_all(text, pattern, indices, 1000);
    return count;
}

int *find_all_indices(const char *text, const char *pattern, int *count) {
    if (text == NULL || pattern == NULL || count == NULL) {
        if (count) *count = 0;
        return NULL;
    }

    size_t n = strlen(text);
    size_t m = strlen(pattern);

    if (m == 0 || m > n) {
        *count = 0;
        return NULL;
    }

    /* First pass to count */
    int temp_indices[1000];
    int temp_count = kmp_search_all(text, pattern, temp_indices, 1000);

    if (temp_count == 0) {
        *count = 0;
        return NULL;
    }

    int *result = malloc(temp_count * sizeof(int));
    if (result == NULL) {
        *count = 0;
        return NULL;
    }

    memcpy(result, temp_indices, temp_count * sizeof(int));
    *count = temp_count;
    return result;
}
