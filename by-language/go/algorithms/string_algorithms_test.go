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
