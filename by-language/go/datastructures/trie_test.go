package datastructures

import (
	"sort"
	"testing"
)

func TestTrie(t *testing.T) {
	trie := NewTrie()

	trie.Insert("apple")
	trie.Insert("app")
	trie.Insert("banana")

	if trie.Size() != 3 {
		t.Errorf("expected size 3, got %d", trie.Size())
	}

	if !trie.Search("apple") {
		t.Error("should find 'apple'")
	}
	if !trie.Search("app") {
		t.Error("should find 'app'")
	}
	if trie.Search("ap") {
		t.Error("should not find 'ap'")
	}
}

func TestTrieStartsWith(t *testing.T) {
	trie := NewTrie()
	trie.Insert("apple")
	trie.Insert("app")

	if !trie.StartsWith("app") {
		t.Error("should start with 'app'")
	}
	if !trie.StartsWith("ap") {
		t.Error("should start with 'ap'")
	}
	if trie.StartsWith("ban") {
		t.Error("should not start with 'ban'")
	}
}

func TestTrieDelete(t *testing.T) {
	trie := NewTrie()
	trie.Insert("apple")
	trie.Insert("app")

	trie.Delete("apple")
	if trie.Search("apple") {
		t.Error("should not find 'apple' after delete")
	}
	if !trie.Search("app") {
		t.Error("should still find 'app'")
	}

	if trie.Delete("nonexistent") {
		t.Error("delete nonexistent should return false")
	}
}

func TestTrieGetWordsWithPrefix(t *testing.T) {
	trie := NewTrie()
	trie.Insert("apple")
	trie.Insert("app")
	trie.Insert("application")
	trie.Insert("banana")

	words := trie.GetWordsWithPrefix("app")
	sort.Strings(words)

	expected := []string{"app", "apple", "application"}
	if len(words) != len(expected) {
		t.Fatalf("expected %d words, got %d: %v", len(expected), len(words), words)
	}
	for i, v := range expected {
		if words[i] != v {
			t.Errorf("index %d: expected %q, got %q", i, v, words[i])
		}
	}
}

func TestTrieAutocomplete(t *testing.T) {
	trie := NewTrie()
	trie.Insert("apple")
	trie.Insert("app")
	trie.Insert("application")
	trie.Insert("banana")

	words := trie.Autocomplete("app", 2)
	if len(words) != 2 {
		t.Errorf("expected 2 results, got %d", len(words))
	}
}

func TestTrieLongestCommonPrefix(t *testing.T) {
	trie := NewTrie()
	trie.Insert("flower")
	trie.Insert("flow")
	trie.Insert("flight")

	prefix := trie.LongestCommonPrefix()
	if prefix != "fl" {
		t.Errorf("expected 'fl', got %q", prefix)
	}
}

func TestTrieCountWordsWithPrefix(t *testing.T) {
	trie := NewTrie()
	trie.Insert("apple")
	trie.Insert("app")
	trie.Insert("application")
	trie.Insert("banana")

	count := trie.CountWordsWithPrefix("app")
	if count != 3 {
		t.Errorf("expected 3, got %d", count)
	}
}

func TestTrieDuplicateInsert(t *testing.T) {
	trie := NewTrie()
	trie.Insert("hello")
	trie.Insert("hello")

	if trie.Size() != 1 {
		t.Errorf("expected size 1 for duplicate, got %d", trie.Size())
	}
}

func TestWordDictionary(t *testing.T) {
	wd := NewWordDictionary()
	wd.AddWord("bad")
	wd.AddWord("dad")
	wd.AddWord("mad")

	if !wd.SearchWord("pad") == true {
		// "pad" should match with ".ad"
	}
	if !wd.SearchWord("bad") {
		t.Error("should find 'bad'")
	}
	if !wd.SearchWord(".ad") {
		t.Error("should match '.ad'")
	}
	if !wd.SearchWord("b..") {
		t.Error("should match 'b..'")
	}
	if wd.SearchWord("b.") {
		t.Error("should not match 'b.' (wrong length)")
	}
}
