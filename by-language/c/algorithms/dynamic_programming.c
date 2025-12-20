/**
 * Dynamic Programming Algorithms Implementation
 * See dynamic_programming.h for documentation
 */

#include "dynamic_programming.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Helper macros */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* ============== Fibonacci Variants ============== */

long long fibonacci(int n) {
    if (n < 0) return 0;
    if (n <= 1) return n;

    long long prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

long long climbing_stairs(int n) {
    if (n <= 0) return 0;
    return fibonacci(n + 1);
}

long long tribonacci(int n) {
    if (n < 0) return 0;
    if (n == 0) return 0;
    if (n <= 2) return 1;

    long long t0 = 0, t1 = 1, t2 = 1;
    for (int i = 3; i <= n; i++) {
        long long t3 = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = t3;
    }
    return t2;
}

/* ============== Knapsack Problems ============== */

int knapsack_01(const int *weights, const int *values, int n, int capacity) {
    if (weights == NULL || values == NULL || n <= 0 || capacity <= 0) {
        return 0;
    }

    int *dp = calloc(capacity + 1, sizeof(int));
    if (dp == NULL) return 0;

    for (int i = 0; i < n; i++) {
        /* Traverse backwards to avoid using same item twice */
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = MAX(dp[w], dp[w - weights[i]] + values[i]);
        }
    }

    int result = dp[capacity];
    free(dp);
    return result;
}

int knapsack_unbounded(const int *weights, const int *values, int n, int capacity) {
    if (weights == NULL || values == NULL || n <= 0 || capacity <= 0) {
        return 0;
    }

    int *dp = calloc(capacity + 1, sizeof(int));
    if (dp == NULL) return 0;

    for (int w = 1; w <= capacity; w++) {
        for (int i = 0; i < n; i++) {
            if (weights[i] <= w) {
                dp[w] = MAX(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
    }

    int result = dp[capacity];
    free(dp);
    return result;
}

bool subset_sum(const int *nums, int n, int target) {
    if (nums == NULL || n <= 0 || target < 0) {
        return target == 0;
    }

    bool *dp = calloc(target + 1, sizeof(bool));
    if (dp == NULL) return false;

    dp[0] = true;

    for (int i = 0; i < n; i++) {
        for (int s = target; s >= nums[i]; s--) {
            dp[s] = dp[s] || dp[s - nums[i]];
        }
    }

    bool result = dp[target];
    free(dp);
    return result;
}

bool can_partition(const int *nums, int n) {
    if (nums == NULL || n <= 0) return false;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += nums[i];
    }

    if (sum % 2 != 0) return false;

    return subset_sum(nums, n, sum / 2);
}

int target_sum_ways(const int *nums, int n, int target) {
    if (nums == NULL || n <= 0) {
        return target == 0 ? 1 : 0;
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += nums[i];
    }

    /* Target = P - N, Sum = P + N, so P = (Sum + Target) / 2 */
    if ((sum + target) % 2 != 0 || sum + target < 0) {
        return 0;
    }

    int subset = (sum + target) / 2;
    int *dp = calloc(subset + 1, sizeof(int));
    if (dp == NULL) return 0;

    dp[0] = 1;

    for (int i = 0; i < n; i++) {
        for (int s = subset; s >= nums[i]; s--) {
            dp[s] += dp[s - nums[i]];
        }
    }

    int result = dp[subset];
    free(dp);
    return result;
}

/* ============== String DP ============== */

int longest_common_subsequence(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return 0;

    size_t m = strlen(s1);
    size_t n = strlen(s2);

    if (m == 0 || n == 0) return 0;

    /* Use smaller dimension for space optimization */
    if (m < n) {
        const char *temp = s1; s1 = s2; s2 = temp;
        size_t tmp = m; m = n; n = tmp;
    }

    int *dp = calloc(n + 1, sizeof(int));
    if (dp == NULL) return 0;

    for (size_t i = 1; i <= m; i++) {
        int prev = 0;
        for (size_t j = 1; j <= n; j++) {
            int temp = dp[j];
            if (s1[i - 1] == s2[j - 1]) {
                dp[j] = prev + 1;
            } else {
                dp[j] = MAX(dp[j], dp[j - 1]);
            }
            prev = temp;
        }
    }

    int result = dp[n];
    free(dp);
    return result;
}

int longest_common_substring(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return 0;

    size_t m = strlen(s1);
    size_t n = strlen(s2);

    if (m == 0 || n == 0) return 0;

    int *dp = calloc(n + 1, sizeof(int));
    if (dp == NULL) return 0;

    int max_len = 0;

    for (size_t i = 1; i <= m; i++) {
        /* Traverse backwards to avoid overwriting needed values */
        for (size_t j = n; j >= 1; j--) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[j] = dp[j - 1] + 1;
                max_len = MAX(max_len, dp[j]);
            } else {
                dp[j] = 0;
            }
        }
    }

    free(dp);
    return max_len;
}

int edit_distance(const char *s1, const char *s2) {
    if (s1 == NULL) s1 = "";
    if (s2 == NULL) s2 = "";

    size_t m = strlen(s1);
    size_t n = strlen(s2);

    /* Space optimization: use 1D array */
    int *dp = malloc((n + 1) * sizeof(int));
    if (dp == NULL) return -1;

    for (size_t j = 0; j <= n; j++) {
        dp[j] = (int)j;
    }

    for (size_t i = 1; i <= m; i++) {
        int prev = dp[0];
        dp[0] = (int)i;

        for (size_t j = 1; j <= n; j++) {
            int temp = dp[j];
            if (s1[i - 1] == s2[j - 1]) {
                dp[j] = prev;
            } else {
                dp[j] = 1 + MIN(prev, MIN(dp[j], dp[j - 1]));
            }
            prev = temp;
        }
    }

    int result = dp[n];
    free(dp);
    return result;
}

int longest_palindromic_subsequence(const char *s) {
    if (s == NULL) return 0;

    size_t n = strlen(s);
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* LPS(s) = LCS(s, reverse(s)) */
    char *rev = malloc(n + 1);
    if (rev == NULL) return 0;

    for (size_t i = 0; i < n; i++) {
        rev[i] = s[n - 1 - i];
    }
    rev[n] = '\0';

    int result = longest_common_subsequence(s, rev);
    free(rev);
    return result;
}

void longest_palindromic_substring(const char *s, int *start_out, int *len_out) {
    if (s == NULL || start_out == NULL || len_out == NULL) {
        if (start_out) *start_out = 0;
        if (len_out) *len_out = 0;
        return;
    }

    size_t n = strlen(s);
    if (n == 0) {
        *start_out = 0;
        *len_out = 0;
        return;
    }

    int max_start = 0, max_len = 1;

    /* Expand around center approach */
    for (size_t i = 0; i < n; i++) {
        /* Odd length palindrome */
        int left = (int)i, right = (int)i;
        while (left >= 0 && right < (int)n && s[left] == s[right]) {
            if (right - left + 1 > max_len) {
                max_start = left;
                max_len = right - left + 1;
            }
            left--;
            right++;
        }

        /* Even length palindrome */
        left = (int)i;
        right = (int)i + 1;
        while (left >= 0 && right < (int)n && s[left] == s[right]) {
            if (right - left + 1 > max_len) {
                max_start = left;
                max_len = right - left + 1;
            }
            left--;
            right++;
        }
    }

    *start_out = max_start;
    *len_out = max_len;
}

/* ============== Sequence DP ============== */

int longest_increasing_subsequence(const int *nums, int n) {
    if (nums == NULL || n <= 0) return 0;

    /* O(n log n) using patience sorting */
    int *tails = malloc(n * sizeof(int));
    if (tails == NULL) return 0;

    int len = 0;

    for (int i = 0; i < n; i++) {
        /* Binary search for position */
        int left = 0, right = len;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (tails[mid] < nums[i]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        tails[left] = nums[i];
        if (left == len) len++;
    }

    free(tails);
    return len;
}

int max_subarray_sum(const int *nums, int n) {
    if (nums == NULL || n <= 0) return 0;

    int max_sum = nums[0];
    int current_sum = nums[0];

    for (int i = 1; i < n; i++) {
        current_sum = MAX(nums[i], current_sum + nums[i]);
        max_sum = MAX(max_sum, current_sum);
    }

    return max_sum;
}

int max_product_subarray(const int *nums, int n) {
    if (nums == NULL || n <= 0) return 0;

    int max_prod = nums[0];
    int min_prod = nums[0];
    int result = nums[0];

    for (int i = 1; i < n; i++) {
        if (nums[i] < 0) {
            int temp = max_prod;
            max_prod = min_prod;
            min_prod = temp;
        }

        max_prod = MAX(nums[i], max_prod * nums[i]);
        min_prod = MIN(nums[i], min_prod * nums[i]);
        result = MAX(result, max_prod);
    }

    return result;
}

/* ============== Coin/Change Problems ============== */

int coin_change_min(const int *coins, int n, int amount) {
    if (coins == NULL || n <= 0 || amount < 0) {
        return amount == 0 ? 0 : -1;
    }

    int *dp = malloc((amount + 1) * sizeof(int));
    if (dp == NULL) return -1;

    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1; /* Impossible value */
    }
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int j = 0; j < n; j++) {
            if (coins[j] <= i && dp[i - coins[j]] < dp[i] - 1) {
                dp[i] = dp[i - coins[j]] + 1;
            }
        }
    }

    int result = dp[amount] > amount ? -1 : dp[amount];
    free(dp);
    return result;
}

int coin_change_ways(const int *coins, int n, int amount) {
    if (coins == NULL || n <= 0 || amount < 0) {
        return amount == 0 ? 1 : 0;
    }

    int *dp = calloc(amount + 1, sizeof(int));
    if (dp == NULL) return 0;

    dp[0] = 1;

    for (int i = 0; i < n; i++) {
        for (int a = coins[i]; a <= amount; a++) {
            dp[a] += dp[a - coins[i]];
        }
    }

    int result = dp[amount];
    free(dp);
    return result;
}

/* ============== Grid DP ============== */

long long unique_paths(int m, int n) {
    if (m <= 0 || n <= 0) return 0;

    /* Use combinatorics: C(m+n-2, m-1) */
    /* Or DP with space optimization */
    long long *dp = malloc(n * sizeof(long long));
    if (dp == NULL) return 0;

    for (int j = 0; j < n; j++) {
        dp[j] = 1;
    }

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[j] += dp[j - 1];
        }
    }

    long long result = dp[n - 1];
    free(dp);
    return result;
}

int unique_paths_obstacles(const int *grid, int m, int n) {
    if (grid == NULL || m <= 0 || n <= 0) return 0;
    if (grid[0] == 1 || grid[(m - 1) * n + (n - 1)] == 1) return 0;

    int *dp = calloc(n, sizeof(int));
    if (dp == NULL) return 0;

    dp[0] = 1;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i * n + j] == 1) {
                dp[j] = 0;
            } else if (j > 0) {
                dp[j] += dp[j - 1];
            }
        }
    }

    int result = dp[n - 1];
    free(dp);
    return result;
}

int min_path_sum(const int *grid, int m, int n) {
    if (grid == NULL || m <= 0 || n <= 0) return 0;

    int *dp = malloc(n * sizeof(int));
    if (dp == NULL) return 0;

    dp[0] = grid[0];
    for (int j = 1; j < n; j++) {
        dp[j] = dp[j - 1] + grid[j];
    }

    for (int i = 1; i < m; i++) {
        dp[0] += grid[i * n];
        for (int j = 1; j < n; j++) {
            dp[j] = MIN(dp[j], dp[j - 1]) + grid[i * n + j];
        }
    }

    int result = dp[n - 1];
    free(dp);
    return result;
}

/* ============== House Robber ============== */

int house_robber(const int *nums, int n) {
    if (nums == NULL || n <= 0) return 0;
    if (n == 1) return nums[0];

    int prev2 = 0, prev1 = nums[0];

    for (int i = 1; i < n; i++) {
        int curr = MAX(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

static int rob_range(const int *nums, int start, int end) {
    int prev2 = 0, prev1 = 0;

    for (int i = start; i <= end; i++) {
        int curr = MAX(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

int house_robber_circular(const int *nums, int n) {
    if (nums == NULL || n <= 0) return 0;
    if (n == 1) return nums[0];
    if (n == 2) return MAX(nums[0], nums[1]);

    /* Either rob houses 0 to n-2 or 1 to n-1 */
    return MAX(rob_range(nums, 0, n - 2), rob_range(nums, 1, n - 1));
}

/* ============== Stock Problems ============== */

int max_profit_one_txn(const int *prices, int n) {
    if (prices == NULL || n <= 1) return 0;

    int min_price = prices[0];
    int max_profit = 0;

    for (int i = 1; i < n; i++) {
        if (prices[i] < min_price) {
            min_price = prices[i];
        } else {
            max_profit = MAX(max_profit, prices[i] - min_price);
        }
    }

    return max_profit;
}

int max_profit_unlimited(const int *prices, int n) {
    if (prices == NULL || n <= 1) return 0;

    int profit = 0;
    for (int i = 1; i < n; i++) {
        if (prices[i] > prices[i - 1]) {
            profit += prices[i] - prices[i - 1];
        }
    }

    return profit;
}

int max_profit_cooldown(const int *prices, int n) {
    if (prices == NULL || n <= 1) return 0;

    int sold = 0;      /* Max profit if we sold on previous day */
    int hold = INT_MIN; /* Max profit if we're holding stock */
    int rest = 0;       /* Max profit if we did nothing */

    for (int i = 0; i < n; i++) {
        int prev_sold = sold;
        sold = hold + prices[i];
        hold = MAX(hold, rest - prices[i]);
        rest = MAX(rest, prev_sold);
    }

    return MAX(sold, rest);
}

/* ============== Classic DP ============== */

int rod_cutting(const int *prices, int n) {
    if (prices == NULL || n <= 0) return 0;

    int *dp = calloc(n + 1, sizeof(int));
    if (dp == NULL) return 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            dp[i] = MAX(dp[i], prices[j - 1] + dp[i - j]);
        }
    }

    int result = dp[n];
    free(dp);
    return result;
}

int matrix_chain_multiply(const int *dims, int n) {
    if (dims == NULL || n <= 1) return 0;

    /* dp[i][j] = min cost to multiply matrices i to j */
    int **dp = malloc(n * sizeof(int *));
    if (dp == NULL) return 0;

    for (int i = 0; i < n; i++) {
        dp[i] = calloc(n, sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) free(dp[j]);
            free(dp);
            return 0;
        }
    }

    /* len is chain length */
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;

            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    int result = dp[0][n - 1];

    for (int i = 0; i < n; i++) free(dp[i]);
    free(dp);

    return result;
}

bool word_break(const char *s, const char **word_dict, int dict_size) {
    if (s == NULL || word_dict == NULL || dict_size <= 0) {
        return s == NULL || strlen(s) == 0;
    }

    size_t n = strlen(s);
    bool *dp = calloc(n + 1, sizeof(bool));
    if (dp == NULL) return false;

    dp[0] = true;

    for (size_t i = 1; i <= n; i++) {
        for (int j = 0; j < dict_size && !dp[i]; j++) {
            size_t word_len = strlen(word_dict[j]);
            if (word_len <= i && dp[i - word_len]) {
                if (strncmp(s + i - word_len, word_dict[j], word_len) == 0) {
                    dp[i] = true;
                }
            }
        }
    }

    bool result = dp[n];
    free(dp);
    return result;
}
