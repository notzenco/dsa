package algorithms

import (
	"reflect"
	"sort"
	"testing"
)

func TestNaiveSearch(t *testing.T) {
	result := NaiveSearch("AABAACAADAABAABA", "AABA")
	expected := []int{0, 9, 12}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = NaiveSearch("hello", "xyz")
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}
}

func TestKMPSearch(t *testing.T) {
	result := KMPSearch("AABAACAADAABAABA", "AABA")
	expected := []int{0, 9, 12}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = KMPSearch("hello", "xyz")
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}

	result = KMPSearch("aaaaaa", "aa")
	if len(result) != 5 {
		t.Errorf("expected 5 matches, got %d", len(result))
	}

	result = KMPSearch("abc", "")
	if len(result) != 0 {
		t.Errorf("expected empty for empty pattern, got %v", result)
	}
}

func TestComputeLPS(t *testing.T) {
	lps := ComputeLPS("AABAACAABAA")
	expected := []int{0, 1, 0, 1, 2, 0, 1, 2, 3, 4, 5}
	if !reflect.DeepEqual(lps, expected) {
		t.Errorf("expected %v, got %v", expected, lps)
	}
}

func TestRabinKarpSearch(t *testing.T) {
	result := RabinKarpSearch("AABAACAADAABAABA", "AABA")
	expected := []int{0, 9, 12}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = RabinKarpSearch("hello", "xyz")
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}

	result = RabinKarpSearch("ab", "abc")
	if len(result) != 0 {
		t.Errorf("expected empty when pattern longer than text, got %v", result)
	}
}

func TestComputeZArray(t *testing.T) {
	z := ComputeZArray("aabxaab")
	// z[0] is undefined (usually 0), z[1]=1, z[2]=0, z[3]=0, z[4]=3, z[5]=1, z[6]=0
	if z[1] != 1 || z[4] != 3 {
		t.Errorf("unexpected Z array: %v", z)
	}
}

func TestZAlgorithmSearch(t *testing.T) {
	if got := ZAlgorithmSearch("$$", "$"); !reflect.DeepEqual(got, []int{0, 1}) {
		t.Fatalf("delimiter matches: got %v", got)
	}
	result := ZAlgorithmSearch("AABAACAADAABAABA", "AABA")
	expected := []int{0, 9, 12}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = ZAlgorithmSearch("hello", "xyz")
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}
}

func TestAllSearchAlgorithmsAgree(t *testing.T) {
	text := "abcabcabcabc"
	pattern := "abc"

	naive := NaiveSearch(text, pattern)
	kmp := KMPSearch(text, pattern)
	rk := RabinKarpSearch(text, pattern)
	za := ZAlgorithmSearch(text, pattern)

	if !reflect.DeepEqual(naive, kmp) {
		t.Errorf("naive %v != kmp %v", naive, kmp)
	}
	if !reflect.DeepEqual(naive, rk) {
		t.Errorf("naive %v != rabin-karp %v", naive, rk)
	}
	if !reflect.DeepEqual(naive, za) {
		t.Errorf("naive %v != z-algorithm %v", naive, za)
	}
}

func TestManacher(t *testing.T) {
	p := Manacher("abacaba")
	if len(p) == 0 {
		t.Error("expected non-empty result")
	}
}

func TestLongestPalindromeManacher(t *testing.T) {
	result := LongestPalindromeManacher("babad")
	if result != "bab" && result != "aba" && result != "babad" {
		t.Errorf("expected palindrome of length >=3, got %q", result)
	}
	if len(result) < 3 {
		t.Errorf("expected length >= 3, got %d", len(result))
	}

	result = LongestPalindromeManacher("cbbd")
	if result != "bb" {
		t.Errorf("expected 'bb', got %q", result)
	}

	result = LongestPalindromeManacher("a")
	if result != "a" {
		t.Errorf("expected 'a', got %q", result)
	}

	result = LongestPalindromeManacher("")
	if result != "" {
		t.Errorf("expected empty, got %q", result)
	}
}

func TestCountPalindromicSubstrings(t *testing.T) {
	count := CountPalindromicSubstrings("abc")
	if count != 3 {
		t.Errorf("expected 3, got %d", count)
	}

	count = CountPalindromicSubstrings("aaa")
	if count != 6 {
		t.Errorf("expected 6, got %d", count)
	}
}

func TestBuildSuffixArray(t *testing.T) {
	sa := BuildSuffixArray("banana")
	// Suffix array for "banana": [5,3,1,0,4,2] (a, ana, anana, banana, na, nana)
	expected := []int{5, 3, 1, 0, 4, 2}
	if !reflect.DeepEqual(sa, expected) {
		t.Errorf("expected %v, got %v", expected, sa)
	}

	sa = BuildSuffixArray("")
	if sa != nil {
		t.Errorf("expected nil for empty string, got %v", sa)
	}
}

func TestBuildLCPArray(t *testing.T) {
	s := "banana"
	sa := BuildSuffixArray(s)
	lcp := BuildLCPArray(s, sa)
	// LCP for banana with SA [5,3,1,0,4,2]: [1,3,0,0,2]
	expected := []int{1, 3, 0, 0, 2}
	if !reflect.DeepEqual(lcp, expected) {
		t.Errorf("expected %v, got %v", expected, lcp)
	}
}

func TestSearchWithSuffixArray(t *testing.T) {
	text := "banana"
	sa := BuildSuffixArray(text)

	result := SearchWithSuffixArray(text, "ana", sa)
	sort.Ints(result)
	expected := []int{1, 3}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = SearchWithSuffixArray(text, "xyz", sa)
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}
}

func TestIsAnagram(t *testing.T) {
	if !IsAnagram("listen", "silent") {
		t.Error("expected true")
	}
	if IsAnagram("hello", "world") {
		t.Error("expected false")
	}
	if IsAnagram("abc", "ab") {
		t.Error("expected false for different lengths")
	}
}

func TestLongestCommonSubstring(t *testing.T) {
	result := LongestCommonSubstring("abcdef", "zbcdf")
	if result != "bcd" {
		t.Errorf("expected 'bcd', got %q", result)
	}

	result = LongestCommonSubstring("abc", "xyz")
	if result != "" {
		t.Errorf("expected empty, got %q", result)
	}
}

func TestReverseString(t *testing.T) {
	if ReverseString("hello") != "olleh" {
		t.Error("expected 'olleh'")
	}
	if ReverseString("") != "" {
		t.Error("expected empty")
	}
	if ReverseString("a") != "a" {
		t.Error("expected 'a'")
	}
}

func TestIsPalindrome(t *testing.T) {
	if !IsPalindrome("racecar") {
		t.Error("expected true")
	}
	if !IsPalindrome("aba") {
		t.Error("expected true")
	}
	if IsPalindrome("hello") {
		t.Error("expected false")
	}
	if !IsPalindrome("") {
		t.Error("expected true for empty")
	}
	if !IsPalindrome("a") {
		t.Error("expected true for single char")
	}
}

func TestBoyerMooreSearch(t *testing.T) {
	result := BoyerMooreSearch("AABAACAADAABAABA", "AABA")
	expected := []int{0, 9, 12}
	if !reflect.DeepEqual(result, expected) {
		t.Errorf("expected %v, got %v", expected, result)
	}

	result = BoyerMooreSearch("hello", "xyz")
	if len(result) != 0 {
		t.Errorf("expected empty, got %v", result)
	}

	result = BoyerMooreSearch("aaaaaa", "aa")
	if len(result) != 5 {
		t.Errorf("expected 5 matches, got %d", len(result))
	}
}

func TestBoyerMooreSearchEmpty(t *testing.T) {
	result := BoyerMooreSearch("hello", "")
	if len(result) != 0 {
		t.Errorf("expected empty for empty pattern, got %v", result)
	}
}

func TestLevenshteinDistance(t *testing.T) {
	if LevenshteinDistance("kitten", "sitting") != 3 {
		t.Error("expected 3")
	}
	if LevenshteinDistance("", "abc") != 3 {
		t.Error("expected 3")
	}
	if LevenshteinDistance("abc", "") != 3 {
		t.Error("expected 3")
	}
	if LevenshteinDistance("abc", "abc") != 0 {
		t.Error("expected 0")
	}
	if LevenshteinDistance("", "") != 0 {
		t.Error("expected 0")
	}
}

func TestRepeatedSubstringPattern(t *testing.T) {
	if !RepeatedSubstringPattern("abab") {
		t.Error("expected true for 'abab'")
	}
	if RepeatedSubstringPattern("aba") {
		t.Error("expected false for 'aba'")
	}
	if !RepeatedSubstringPattern("abcabcabc") {
		t.Error("expected true for 'abcabcabc'")
	}
}

func TestShortestPalindrome(t *testing.T) {
	if got := ShortestPalindrome("a#a"); got != "a#a" {
		t.Fatalf("delimiter palindrome: got %q", got)
	}
	result := ShortestPalindrome("aacecaaa")
	if result != "aaacecaaa" {
		t.Errorf("expected 'aaacecaaa', got %q", result)
	}

	result = ShortestPalindrome("abcd")
	if result != "dcbabcd" {
		t.Errorf("expected 'dcbabcd', got %q", result)
	}
}

func TestGroupAnagrams(t *testing.T) {
	result := GroupAnagrams([]string{"eat", "tea", "tan", "ate", "nat", "bat"})
	if len(result) != 3 {
		t.Errorf("expected 3 groups, got %d", len(result))
	}

	// Verify total count
	total := 0
	for _, group := range result {
		total += len(group)
	}
	if total != 6 {
		t.Errorf("expected 6 total items, got %d", total)
	}
}

func TestLongestRepeatingSubstring(t *testing.T) {
	result := LongestRepeatingSubstring("aabcaabdaab")
	if result != "aab" {
		t.Errorf("expected 'aab', got %q", result)
	}

	result = LongestRepeatingSubstring("abcd")
	if result != "" {
		t.Errorf("expected empty for no repeats, got %q", result)
	}
}

func TestRunLengthEncode(t *testing.T) {
	result := RunLengthEncode("aaabbbcc")
	if result != "3a3b2c" {
		t.Errorf("expected '3a3b2c', got %q", result)
	}

	// Single occurrences have no count prefix
	result = RunLengthEncode("abcd")
	if result != "abcd" {
		t.Errorf("expected 'abcd', got %q", result)
	}

	result = RunLengthEncode("")
	if result != "" {
		t.Errorf("expected empty, got %q", result)
	}

	result = RunLengthEncode("aaa")
	if result != "3a" {
		t.Errorf("expected '3a', got %q", result)
	}
}

func TestLongestPalindromicSubsequence(t *testing.T) {
	result := LongestPalindromicSubsequence("bbbab")
	if result != 4 {
		t.Errorf("expected 4, got %d", result)
	}

	result = LongestPalindromicSubsequence("cbbd")
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}
}

func TestMinimumWindowSort(t *testing.T) {
	start, end := MinimumWindowSort([]int{2, 6, 4, 8, 10, 9, 15})
	if start != 1 || end != 5 {
		t.Errorf("expected (1, 5), got (%d, %d)", start, end)
	}

	start, end = MinimumWindowSort([]int{1, 2, 3})
	if start != -1 || end != -1 {
		t.Errorf("expected (-1, -1) for sorted, got (%d, %d)", start, end)
	}
}

func TestStrStr(t *testing.T) {
	result := StrStr("hello", "ll")
	if result != 2 {
		t.Errorf("expected 2, got %d", result)
	}

	result = StrStr("aaaaa", "bba")
	if result != -1 {
		t.Errorf("expected -1, got %d", result)
	}

	result = StrStr("", "")
	if result != 0 {
		t.Errorf("expected 0 for empty strings, got %d", result)
	}
}

func TestAllSearchAlgorithmsAgreeBoyerMoore(t *testing.T) {
	text := "abcabcabcabc"
	pattern := "abc"

	naive := NaiveSearch(text, pattern)
	bm := BoyerMooreSearch(text, pattern)

	sort.Ints(naive)
	sort.Ints(bm)

	if !reflect.DeepEqual(naive, bm) {
		t.Errorf("naive %v != boyer-moore %v", naive, bm)
	}
}
