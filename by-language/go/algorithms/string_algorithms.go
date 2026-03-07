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

// BoyerMooreSearch finds all occurrences using Boyer-Moore bad character rule.
func BoyerMooreSearch(text, pattern string) []int {
	var result []int
	n, m := len(text), len(pattern)
	if m == 0 || m > n {
		return result
	}

	// Build bad character table
	badChar := make(map[byte]int)
	for i := 0; i < m; i++ {
		badChar[pattern[i]] = i
	}

	s := 0
	for s <= n-m {
		j := m - 1
		for j >= 0 && pattern[j] == text[s+j] {
			j--
		}
		if j < 0 {
			result = append(result, s)
			if s+m < n {
				bc, ok := badChar[text[s+m]]
				if ok {
					s += m - bc
				} else {
					s += m + 1
				}
			} else {
				s++
			}
		} else {
			bc, ok := badChar[text[s+j]]
			shift := 1
			if ok {
				shift = j - bc
				if shift < 1 {
					shift = 1
				}
			} else {
				shift = j + 1
			}
			s += shift
		}
	}
	return result
}

// LevenshteinDistance computes the edit distance between two strings.
func LevenshteinDistance(s1, s2 string) int {
	m, n := len(s1), len(s2)
	if m == 0 {
		return n
	}
	if n == 0 {
		return m
	}

	// Use two rows for space optimization
	prev := make([]int, n+1)
	curr := make([]int, n+1)

	for j := 0; j <= n; j++ {
		prev[j] = j
	}

	for i := 1; i <= m; i++ {
		curr[0] = i
		for j := 1; j <= n; j++ {
			if s1[i-1] == s2[j-1] {
				curr[j] = prev[j-1]
			} else {
				del := prev[j] + 1
				ins := curr[j-1] + 1
				rep := prev[j-1] + 1
				curr[j] = del
				if ins < curr[j] {
					curr[j] = ins
				}
				if rep < curr[j] {
					curr[j] = rep
				}
			}
		}
		prev, curr = curr, prev
	}
	return prev[n]
}

// RepeatedSubstringPattern checks if string can be formed by repeating a substring.
func RepeatedSubstringPattern(s string) bool {
	n := len(s)
	if n <= 1 {
		return false
	}

	for length := 1; length <= n/2; length++ {
		if n%length != 0 {
			continue
		}
		pattern := s[:length]
		match := true
		for i := length; i < n; i += length {
			if s[i:i+length] != pattern {
				match = false
				break
			}
		}
		if match {
			return true
		}
	}
	return false
}

// ShortestPalindrome finds the shortest palindrome by adding characters to the front.
func ShortestPalindrome(s string) string {
	if len(s) <= 1 {
		return s
	}

	rev := ReverseString(s)
	// Find the longest palindrome prefix
	concat := s + "#" + rev
	lps := ComputeLPS(concat)
	longestPalPrefix := lps[len(concat)-1]

	suffix := s[longestPalPrefix:]
	return ReverseString(suffix) + s
}

// GroupAnagrams groups strings that are anagrams of each other.
func GroupAnagrams(strs []string) [][]string {
	groups := make(map[[26]int][]string)
	for _, s := range strs {
		var key [26]int
		for _, c := range s {
			key[c-'a']++
		}
		groups[key] = append(groups[key], s)
	}
	result := make([][]string, 0, len(groups))
	for _, group := range groups {
		result = append(result, group)
	}
	return result
}

// LongestRepeatingSubstring finds the longest substring that appears at least twice.
func LongestRepeatingSubstring(s string) string {
	n := len(s)
	if n <= 1 {
		return ""
	}

	best := ""
	// Binary search on length
	lo, hi := 1, n-1
	for lo <= hi {
		mid := (lo + hi) / 2
		found := longestRepeatOfLength(s, mid)
		if found != "" {
			best = found
			lo = mid + 1
		} else {
			hi = mid - 1
		}
	}
	return best
}

func longestRepeatOfLength(s string, length int) string {
	seen := make(map[string]bool)
	for i := 0; i <= len(s)-length; i++ {
		sub := s[i : i+length]
		if seen[sub] {
			return sub
		}
		seen[sub] = true
	}
	return ""
}

// RunLengthEncode encodes a string using run-length encoding.
func RunLengthEncode(s string) string {
	if len(s) == 0 {
		return ""
	}
	var result []byte
	count := 1
	for i := 1; i < len(s); i++ {
		if s[i] == s[i-1] {
			count++
		} else {
			if count > 1 {
				result = append(result, []byte(intToStr(count))...)
			}
			result = append(result, s[i-1])
			count = 1
		}
	}
	if count > 1 {
		result = append(result, []byte(intToStr(count))...)
	}
	result = append(result, s[len(s)-1])
	return string(result)
}

func intToStr(n int) string {
	if n == 0 {
		return "0"
	}
	var digits []byte
	for n > 0 {
		digits = append([]byte{byte('0' + n%10)}, digits...)
		n /= 10
	}
	return string(digits)
}

// LongestPalindromicSubsequence returns the length of the longest palindromic subsequence.
func LongestPalindromicSubsequence(s string) int {
	n := len(s)
	if n <= 1 {
		return n
	}

	dp := make([][]int, n)
	for i := range dp {
		dp[i] = make([]int, n)
		dp[i][i] = 1
	}

	for length := 2; length <= n; length++ {
		for i := 0; i <= n-length; i++ {
			j := i + length - 1
			if s[i] == s[j] {
				dp[i][j] = dp[i+1][j-1] + 2
			} else {
				dp[i][j] = dp[i+1][j]
				if dp[i][j-1] > dp[i][j] {
					dp[i][j] = dp[i][j-1]
				}
			}
		}
	}
	return dp[0][n-1]
}

// MinimumWindowSort returns the minimum window that needs to be sorted
// for the whole array to become sorted.
func MinimumWindowSort(arr []int) (int, int) {
	n := len(arr)
	if n <= 1 {
		return -1, -1
	}

	// Find left boundary
	left := 0
	for left < n-1 && arr[left] <= arr[left+1] {
		left++
	}
	if left == n-1 {
		return -1, -1 // already sorted
	}

	// Find right boundary
	right := n - 1
	for right > 0 && arr[right] >= arr[right-1] {
		right--
	}

	// Find min and max in the unsorted subarray
	subMin, subMax := arr[left], arr[left]
	for i := left; i <= right; i++ {
		if arr[i] < subMin {
			subMin = arr[i]
		}
		if arr[i] > subMax {
			subMax = arr[i]
		}
	}

	// Extend left while elements are greater than subMin
	for left > 0 && arr[left-1] > subMin {
		left--
	}

	// Extend right while elements are less than subMax
	for right < n-1 && arr[right+1] < subMax {
		right++
	}

	return left, right
}

// StrStr implements strStr() - find first occurrence of needle in haystack.
func StrStr(haystack, needle string) int {
	if len(needle) == 0 {
		return 0
	}
	matches := KMPSearch(haystack, needle)
	if len(matches) > 0 {
		return matches[0]
	}
	return -1
}
