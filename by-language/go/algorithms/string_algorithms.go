package algorithms

// NaiveSearch finds all occurrences of pattern in text. O(n*m)
func NaiveSearch(text, pattern string) []int {
	var result []int
	n, m := len(text), len(pattern)
	for i := 0; i <= n-m; i++ {
		match := true
		for j := 0; j < m; j++ {
			if text[i+j] != pattern[j] {
				match = false
				break
			}
		}
		if match {
			result = append(result, i)
		}
	}
	return result
}

// ComputeLPS computes the Longest Proper Prefix-Suffix array for KMP.
func ComputeLPS(pattern string) []int {
	m := len(pattern)
	lps := make([]int, m)
	length := 0
	i := 1

	for i < m {
		if pattern[i] == pattern[length] {
			length++
			lps[i] = length
			i++
		} else {
			if length != 0 {
				length = lps[length-1]
			} else {
				lps[i] = 0
				i++
			}
		}
	}
	return lps
}

// KMPSearch finds all occurrences using Knuth-Morris-Pratt. O(n+m)
func KMPSearch(text, pattern string) []int {
	var result []int
	n, m := len(text), len(pattern)
	if m == 0 {
		return result
	}

	lps := ComputeLPS(pattern)
	i, j := 0, 0

	for i < n {
		if text[i] == pattern[j] {
			i++
			j++
		}
		if j == m {
			result = append(result, i-j)
			j = lps[j-1]
		} else if i < n && text[i] != pattern[j] {
			if j != 0 {
				j = lps[j-1]
			} else {
				i++
			}
		}
	}
	return result
}

// RabinKarpSearch finds all occurrences using Rabin-Karp. O(n+m) average.
func RabinKarpSearch(text, pattern string) []int {
	var result []int
	n, m := len(text), len(pattern)
	if m > n {
		return result
	}

	const base = 256
	const mod = 101

	// Compute hash of pattern and first window
	patHash := 0
	textHash := 0
	h := 1

	for i := 0; i < m-1; i++ {
		h = (h * base) % mod
	}

	for i := 0; i < m; i++ {
		patHash = (base*patHash + int(pattern[i])) % mod
		textHash = (base*textHash + int(text[i])) % mod
	}

	for i := 0; i <= n-m; i++ {
		if patHash == textHash {
			if text[i:i+m] == pattern {
				result = append(result, i)
			}
		}
		if i < n-m {
			textHash = (base*(textHash-int(text[i])*h) + int(text[i+m])) % mod
			if textHash < 0 {
				textHash += mod
			}
		}
	}
	return result
}

// ComputeZArray computes the Z-array for the Z-algorithm.
func ComputeZArray(s string) []int {
	n := len(s)
	z := make([]int, n)
	if n == 0 {
		return z
	}

	l, r := 0, 0
	for i := 1; i < n; i++ {
		if i < r {
			z[i] = min(r-i, z[i-l])
		}
		for i+z[i] < n && s[z[i]] == s[i+z[i]] {
			z[i]++
		}
		if i+z[i] > r {
			l = i
			r = i + z[i]
		}
	}
	return z
}

// ZAlgorithmSearch finds all occurrences using Z-algorithm. O(n+m)
func ZAlgorithmSearch(text, pattern string) []int {
	var result []int
	concat := pattern + "$" + text
	z := ComputeZArray(concat)
	m := len(pattern)

	for i := m + 1; i < len(concat); i++ {
		if z[i] == m {
			result = append(result, i-m-1)
		}
	}
	return result
}

// Manacher computes the longest palindromic radius centered at each position.
func Manacher(s string) []int {
	// Transform: "abc" -> "^#a#b#c#$"
	t := make([]byte, 0, 2*len(s)+3)
	t = append(t, '^')
	for _, c := range s {
		t = append(t, '#')
		t = append(t, byte(c))
	}
	t = append(t, '#', '$')

	n := len(t)
	p := make([]int, n)
	c, r := 0, 0

	for i := 1; i < n-1; i++ {
		mirror := 2*c - i
		if i < r {
			p[i] = min(r-i, p[mirror])
		}
		for t[i+p[i]+1] == t[i-p[i]-1] {
			p[i]++
		}
		if i+p[i] > r {
			c = i
			r = i + p[i]
		}
	}

	// Extract original radii (for actual characters only)
	result := make([]int, 2*len(s)+1)
	for i := 1; i < n-1; i++ {
		result[i-1] = p[i]
	}
	return result
}

// LongestPalindromeManacher finds the longest palindrome using Manacher's.
func LongestPalindromeManacher(s string) string {
	if len(s) <= 1 {
		return s
	}

	p := Manacher(s)
	maxLen := 0
	center := 0
	for i, v := range p {
		if v > maxLen {
			maxLen = v
			center = i
		}
	}

	// Map back to original string indices
	start := (center - maxLen) / 2
	return s[start : start+maxLen]
}

// CountPalindromicSubstrings counts all palindromic substrings.
func CountPalindromicSubstrings(s string) int {
	n := len(s)
	count := 0

	expand := func(left, right int) {
		for left >= 0 && right < n && s[left] == s[right] {
			count++
			left--
			right++
		}
	}

	for i := 0; i < n; i++ {
		expand(i, i)   // odd
		expand(i, i+1) // even
	}
	return count
}

// BuildSuffixArray builds a suffix array for string s. O(n log² n)
func BuildSuffixArray(s string) []int {
	n := len(s)
	if n == 0 {
		return nil
	}

	type suffix struct {
		index int
		rank  [2]int
	}

	suffixes := make([]suffix, n)
	for i := 0; i < n; i++ {
		suffixes[i].index = i
		suffixes[i].rank[0] = int(s[i])
		if i+1 < n {
			suffixes[i].rank[1] = int(s[i+1])
		} else {
			suffixes[i].rank[1] = -1
		}
	}

	// Sort suffixes
	sortSuffixes := func(arr []suffix) {
		// Simple insertion sort for the suffix array construction
		for i := 1; i < len(arr); i++ {
			key := arr[i]
			j := i - 1
			for j >= 0 && (arr[j].rank[0] > key.rank[0] ||
				(arr[j].rank[0] == key.rank[0] && arr[j].rank[1] > key.rank[1])) {
				arr[j+1] = arr[j]
				j--
			}
			arr[j+1] = key
		}
	}

	sortSuffixes(suffixes)

	ind := make([]int, n)
	for k := 4; k < 2*n; k *= 2 {
		rank := 0
		prevRank := suffixes[0].rank[0]
		suffixes[0].rank[0] = rank
		ind[suffixes[0].index] = 0

		for i := 1; i < n; i++ {
			if suffixes[i].rank[0] == prevRank && suffixes[i].rank[1] == suffixes[i-1].rank[1] {
				prevRank = suffixes[i].rank[0]
				suffixes[i].rank[0] = rank
			} else {
				prevRank = suffixes[i].rank[0]
				rank++
				suffixes[i].rank[0] = rank
			}
			ind[suffixes[i].index] = i
		}

		for i := 0; i < n; i++ {
			nextIdx := suffixes[i].index + k/2
			if nextIdx < n {
				suffixes[i].rank[1] = suffixes[ind[nextIdx]].rank[0]
			} else {
				suffixes[i].rank[1] = -1
			}
		}

		sortSuffixes(suffixes)
	}

	sa := make([]int, n)
	for i := 0; i < n; i++ {
		sa[i] = suffixes[i].index
	}
	return sa
}

// BuildLCPArray builds the LCP (Longest Common Prefix) array.
func BuildLCPArray(s string, sa []int) []int {
	n := len(s)
	if n == 0 {
		return nil
	}

	rank := make([]int, n)
	for i, v := range sa {
		rank[v] = i
	}

	lcp := make([]int, n-1)
	h := 0
	for i := 0; i < n; i++ {
		if rank[i] > 0 {
			j := sa[rank[i]-1]
			for i+h < n && j+h < n && s[i+h] == s[j+h] {
				h++
			}
			lcp[rank[i]-1] = h
			if h > 0 {
				h--
			}
		} else {
			h = 0
		}
	}
	return lcp
}

// SearchWithSuffixArray searches for pattern in text using suffix array.
func SearchWithSuffixArray(text, pattern string, sa []int) []int {
	var result []int
	n := len(text)
	m := len(pattern)

	// Binary search for lower bound
	lo, hi := 0, n
	for lo < hi {
		mid := (lo + hi) / 2
		suffix := text[sa[mid]:]
		if len(suffix) > m {
			suffix = suffix[:m]
		}
		if suffix < pattern {
			lo = mid + 1
		} else {
			hi = mid
		}
	}
	start := lo

	// Binary search for upper bound
	lo, hi = start, n
	for lo < hi {
		mid := (lo + hi) / 2
		suffix := text[sa[mid]:]
		if len(suffix) > m {
			suffix = suffix[:m]
		}
		if suffix <= pattern {
			lo = mid + 1
		} else {
			hi = mid
		}
	}

	for i := start; i < lo; i++ {
		result = append(result, sa[i])
	}
	return result
}

// IsAnagram checks if two strings are anagrams.
func IsAnagram(s1, s2 string) bool {
	if len(s1) != len(s2) {
		return false
	}
	counts := make(map[rune]int)
	for _, c := range s1 {
		counts[c]++
	}
	for _, c := range s2 {
		counts[c]--
		if counts[c] < 0 {
			return false
		}
	}
	return true
}

// LongestCommonSubstring returns the longest common substring.
func LongestCommonSubstring(s1, s2 string) string {
	m, n := len(s1), len(s2)
	maxLen := 0
	endIdx := 0

	dp := make([][]int, m+1)
	for i := range dp {
		dp[i] = make([]int, n+1)
	}

	for i := 1; i <= m; i++ {
		for j := 1; j <= n; j++ {
			if s1[i-1] == s2[j-1] {
				dp[i][j] = dp[i-1][j-1] + 1
				if dp[i][j] > maxLen {
					maxLen = dp[i][j]
					endIdx = i
				}
			}
		}
	}
	return s1[endIdx-maxLen : endIdx]
}

// ReverseString reverses a string.
func ReverseString(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}

// IsPalindrome checks if a string is a palindrome.
func IsPalindrome(s string) bool {
	for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
		if s[i] != s[j] {
			return false
		}
	}
	return true
}
