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

func TestTrieMapBasic(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("apple", 5)
	tm.Put("app", 3)
	tm.Put("banana", 7)

	if tm.Size() != 3 {
		t.Errorf("expected size 3, got %d", tm.Size())
	}

	val, ok := tm.Get("apple")
	if !ok || val.(int) != 5 {
		t.Errorf("expected 5, got %v", val)
	}

	val, ok = tm.Get("app")
	if !ok || val.(int) != 3 {
		t.Errorf("expected 3, got %v", val)
	}

	_, ok = tm.Get("ap")
	if ok {
		t.Error("should not find 'ap'")
	}
}

func TestTrieMapContains(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("hello", 1)

	if !tm.Contains("hello") {
		t.Error("should contain 'hello'")
	}
	if tm.Contains("hell") {
		t.Error("should not contain 'hell'")
	}
}

func TestTrieMapDelete(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("hello", 1)
	tm.Put("help", 2)

	if !tm.Delete("hello") {
		t.Error("delete should return true")
	}
	if tm.Contains("hello") {
		t.Error("should not contain 'hello' after delete")
	}
	if !tm.Contains("help") {
		t.Error("should still contain 'help'")
	}
	if tm.Size() != 1 {
		t.Errorf("expected size 1, got %d", tm.Size())
	}
}

func TestTrieMapSumWithPrefix(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("apple", 5)
	tm.Put("app", 3)
	tm.Put("banana", 7)

	sum := tm.SumWithPrefix("app")
	if sum != 8 { // apple(5) + app(3)
		t.Errorf("expected sum 8, got %d", sum)
	}
}

func TestTrieMapItemsWithPrefix(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("apple", 5)
	tm.Put("app", 3)
	tm.Put("banana", 7)

	items := tm.ItemsWithPrefix("app")
	if len(items) != 2 {
		t.Errorf("expected 2 items, got %d", len(items))
	}
}

func TestTrieMapKeys(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("a", 1)
	tm.Put("b", 2)
	tm.Put("c", 3)

	keys := tm.Keys()
	sort.Strings(keys)
	if len(keys) != 3 {
		t.Errorf("expected 3 keys, got %d", len(keys))
	}
}

func TestTrieMapUpdate(t *testing.T) {
	tm := NewTrieMap()
	tm.Put("key", 1)
	tm.Put("key", 2)

	if tm.Size() != 1 {
		t.Errorf("expected size 1 after update, got %d", tm.Size())
	}
	val, _ := tm.Get("key")
	if val.(int) != 2 {
		t.Errorf("expected 2 after update, got %v", val)
	}
}

func TestCompressedTrieBasic(t *testing.T) {
	ct := NewCompressedTrie()
	ct.Insert("test")
	ct.Insert("team")
	ct.Insert("toast")

	if ct.Size() != 3 {
		t.Errorf("expected size 3, got %d", ct.Size())
	}

	if !ct.Search("test") {
		t.Error("should find 'test'")
	}
	if !ct.Search("team") {
		t.Error("should find 'team'")
	}
	if !ct.Search("toast") {
		t.Error("should find 'toast'")
	}
	if ct.Search("tea") {
		t.Error("should not find 'tea'")
	}
}

func TestCompressedTrieStartsWith(t *testing.T) {
	ct := NewCompressedTrie()
	ct.Insert("hello")
	ct.Insert("help")

	if !ct.StartsWith("hel") {
		t.Error("should start with 'hel'")
	}
	if !ct.StartsWith("hello") {
		t.Error("should start with 'hello'")
	}
	if ct.StartsWith("world") {
		t.Error("should not start with 'world'")
	}
}

func TestCompressedTrieSplit(t *testing.T) {
	ct := NewCompressedTrie()
	ct.Insert("abcdef")
	ct.Insert("abcxyz")

	if !ct.Search("abcdef") {
		t.Error("should find 'abcdef'")
	}
	if !ct.Search("abcxyz") {
		t.Error("should find 'abcxyz'")
	}
	if ct.Search("abc") {
		t.Error("should not find 'abc'")
	}
}

func TestTrieEmptyOperations(t *testing.T) {
	trie := NewTrie()

	if !trie.IsEmpty() {
		t.Error("new trie should be empty")
	}
	if trie.Search("") {
		t.Error("should not find empty string")
	}

	words := trie.GetWordsWithPrefix("")
	if len(words) != 0 {
		t.Error("empty trie should return no words")
	}

	prefix := trie.LongestCommonPrefix()
	if prefix != "" {
		t.Errorf("expected empty prefix, got %q", prefix)
	}
}

func TestWordDictionaryAllWild(t *testing.T) {
	wd := NewWordDictionary()
	wd.AddWord("ab")
	wd.AddWord("cd")

	if !wd.SearchWord("..") {
		t.Error("should match '..' for 2-char words")
	}
	if wd.SearchWord("...") {
		t.Error("should not match '...' (wrong length)")
	}
}

func TestTrieMapEmpty(t *testing.T) {
	tm := NewTrieMap()
	if !tm.IsEmpty() {
		t.Error("new TrieMap should be empty")
	}

	sum := tm.SumWithPrefix("anything")
	if sum != 0 {
		t.Errorf("expected 0, got %d", sum)
	}
}
