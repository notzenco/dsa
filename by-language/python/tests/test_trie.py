"""
Tests for Trie implementations.
"""

import pytest
from data_structures.trie import Trie, TrieMap, WordDictionary, CompressedTrie


class TestTrieBasics:
    """Test basic trie operations."""

    def test_init(self):
        """Test trie initialization."""
        trie = Trie()
        assert len(trie) == 0

    def test_insert_and_search(self):
        """Test basic insert and search."""
        trie = Trie()
        trie.insert("hello")

        assert trie.search("hello")
        assert not trie.search("hell")
        assert not trie.search("helloo")

    def test_insert_multiple(self):
        """Test inserting multiple words."""
        trie = Trie()
        words = ["cat", "car", "card", "care", "careful", "egg"]
        for word in words:
            trie.insert(word)

        assert len(trie) == 6
        for word in words:
            assert trie.search(word)

    def test_starts_with(self):
        """Test prefix checking."""
        trie = Trie()
        trie.insert("hello")
        trie.insert("help")
        trie.insert("world")

        assert trie.starts_with("hel")
        assert trie.starts_with("hello")
        assert trie.starts_with("wor")
        assert not trie.starts_with("abc")
        assert not trie.starts_with("helper")

    def test_insert_duplicate(self):
        """Test inserting duplicate word."""
        trie = Trie()
        trie.insert("test")
        trie.insert("test")

        assert len(trie) == 1
        assert trie.search("test")

    def test_empty_string(self):
        """Test handling empty string."""
        trie = Trie()
        trie.insert("")

        assert trie.search("")
        assert trie.starts_with("")

    def test_single_character(self):
        """Test single character words."""
        trie = Trie()
        trie.insert("a")
        trie.insert("b")

        assert trie.search("a")
        assert trie.search("b")
        assert not trie.search("c")


class TestTrieDelete:
    """Test trie deletion operations."""

    def test_delete_existing(self):
        """Test deleting existing word."""
        trie = Trie()
        trie.insert("hello")
        trie.insert("help")

        assert trie.delete("hello")
        assert not trie.search("hello")
        assert trie.search("help")
        assert len(trie) == 1

    def test_delete_nonexistent(self):
        """Test deleting non-existent word."""
        trie = Trie()
        trie.insert("hello")

        assert not trie.delete("world")
        assert len(trie) == 1

    def test_delete_prefix_word(self):
        """Test deleting word that is prefix of another."""
        trie = Trie()
        trie.insert("car")
        trie.insert("card")

        assert trie.delete("car")
        assert not trie.search("car")
        assert trie.search("card")

    def test_delete_longer_word(self):
        """Test deleting longer word with shared prefix."""
        trie = Trie()
        trie.insert("car")
        trie.insert("card")

        assert trie.delete("card")
        assert trie.search("car")
        assert not trie.search("card")

    def test_delete_all_words(self):
        """Test deleting all words."""
        trie = Trie()
        trie.insert("a")
        trie.insert("b")

        trie.delete("a")
        trie.delete("b")

        assert len(trie) == 0
        assert not trie.search("a")
        assert not trie.search("b")


class TestTriePrefixOperations:
    """Test prefix-based operations."""

    def test_get_words_with_prefix(self):
        """Test getting all words with prefix."""
        trie = Trie()
        trie.insert("cat")
        trie.insert("car")
        trie.insert("card")
        trie.insert("care")
        trie.insert("dog")

        result = trie.get_words_with_prefix("car")
        assert set(result) == {"car", "card", "care"}

    def test_get_words_with_prefix_no_match(self):
        """Test prefix with no matches."""
        trie = Trie()
        trie.insert("hello")

        assert trie.get_words_with_prefix("world") == []

    def test_get_words_with_empty_prefix(self):
        """Test empty prefix returns all words."""
        trie = Trie()
        trie.insert("a")
        trie.insert("b")
        trie.insert("c")

        result = trie.get_words_with_prefix("")
        assert set(result) == {"a", "b", "c"}

    def test_count_words_with_prefix(self):
        """Test counting words with prefix."""
        trie = Trie()
        trie.insert("test")
        trie.insert("testing")
        trie.insert("tested")
        trie.insert("tester")
        trie.insert("other")

        assert trie.count_words_with_prefix("test") == 4
        assert trie.count_words_with_prefix("testing") == 1
        assert trie.count_words_with_prefix("oth") == 1
        assert trie.count_words_with_prefix("xyz") == 0

    def test_autocomplete(self):
        """Test autocomplete functionality."""
        trie = Trie()
        words = ["apple", "application", "apply", "apt", "banana"]
        for word in words:
            trie.insert(word)

        result = trie.autocomplete("app", limit=2)
        assert len(result) == 2
        assert all(w.startswith("app") for w in result)

    def test_autocomplete_limit(self):
        """Test autocomplete respects limit."""
        trie = Trie()
        for i in range(100):
            trie.insert(f"word{i}")

        result = trie.autocomplete("word", limit=5)
        assert len(result) == 5


class TestTrieLongestCommonPrefix:
    """Test longest common prefix."""

    def test_common_prefix_exists(self):
        """Test finding common prefix."""
        trie = Trie()
        trie.insert("flower")
        trie.insert("flow")
        trie.insert("flight")

        assert trie.longest_common_prefix() == "fl"

    def test_no_common_prefix(self):
        """Test when no common prefix."""
        trie = Trie()
        trie.insert("dog")
        trie.insert("car")
        trie.insert("racecar")

        assert trie.longest_common_prefix() == ""

    def test_all_same_word(self):
        """Test all same words."""
        trie = Trie()
        trie.insert("test")

        assert trie.longest_common_prefix() == "test"

    def test_empty_trie(self):
        """Test empty trie."""
        trie = Trie()
        assert trie.longest_common_prefix() == ""


class TestTrieDunderMethods:
    """Test magic methods."""

    def test_len(self):
        """Test __len__."""
        trie = Trie()
        assert len(trie) == 0

        trie.insert("a")
        assert len(trie) == 1

        trie.insert("b")
        trie.insert("c")
        assert len(trie) == 3

    def test_contains(self):
        """Test __contains__."""
        trie = Trie()
        trie.insert("hello")

        assert "hello" in trie
        assert "world" not in trie

    def test_iter(self):
        """Test __iter__."""
        trie = Trie()
        words = ["cat", "dog", "bird"]
        for word in words:
            trie.insert(word)

        assert set(trie) == set(words)

    def test_repr(self):
        """Test __repr__."""
        trie = Trie()
        trie.insert("test")

        assert "Trie" in repr(trie)
        assert "1" in repr(trie)


class TestTrieMap:
    """Test TrieMap implementation."""

    def test_setitem_getitem(self):
        """Test setting and getting items."""
        tm = TrieMap()
        tm["hello"] = 1
        tm["world"] = 2

        assert tm["hello"] == 1
        assert tm["world"] == 2

    def test_getitem_missing(self):
        """Test getting missing key raises KeyError."""
        tm = TrieMap()
        with pytest.raises(KeyError):
            _ = tm["missing"]

    def test_contains(self):
        """Test membership check."""
        tm = TrieMap()
        tm["test"] = 1

        assert "test" in tm
        assert "missing" not in tm

    def test_get_with_default(self):
        """Test get with default value."""
        tm = TrieMap()
        tm["key"] = "value"

        assert tm.get("key") == "value"
        assert tm.get("missing") is None
        assert tm.get("missing", "default") == "default"

    def test_sum_with_prefix(self):
        """Test summing values with prefix."""
        tm = TrieMap()
        tm["apple"] = 10
        tm["application"] = 20
        tm["apply"] = 30
        tm["banana"] = 100

        assert tm.sum_with_prefix("app") == 60
        assert tm.sum_with_prefix("apple") == 10
        assert tm.sum_with_prefix("ban") == 100
        assert tm.sum_with_prefix("xyz") == 0

    def test_items_with_prefix(self):
        """Test getting items with prefix."""
        tm = TrieMap()
        tm["cat"] = 1
        tm["car"] = 2
        tm["card"] = 3
        tm["dog"] = 4

        items = tm.items_with_prefix("ca")
        assert set(items) == {("cat", 1), ("car", 2), ("card", 3)}

    def test_len(self):
        """Test length."""
        tm = TrieMap()
        assert len(tm) == 0

        tm["a"] = 1
        tm["b"] = 2
        assert len(tm) == 2

        # Update existing key
        tm["a"] = 10
        assert len(tm) == 2


class TestWordDictionary:
    """Test WordDictionary with wildcard support."""

    def test_basic_operations(self):
        """Test basic add and search."""
        wd = WordDictionary()
        wd.add_word("bad")
        wd.add_word("dad")
        wd.add_word("mad")

        assert wd.search("bad")
        assert wd.search("dad")
        assert not wd.search("pad")

    def test_wildcard_single(self):
        """Test single wildcard."""
        wd = WordDictionary()
        wd.add_word("bad")
        wd.add_word("dad")

        assert wd.search("b.d")
        assert wd.search("d.d")
        assert wd.search(".ad")
        assert not wd.search(".od")

    def test_wildcard_multiple(self):
        """Test multiple wildcards."""
        wd = WordDictionary()
        wd.add_word("bad")

        assert wd.search("...")
        assert wd.search("b..")
        assert wd.search(".a.")
        assert wd.search("..d")
        assert not wd.search("....")

    def test_wildcard_all(self):
        """Test all wildcards."""
        wd = WordDictionary()
        wd.add_word("abc")
        wd.add_word("xyz")

        assert wd.search("...")

    def test_no_match(self):
        """Test no match cases."""
        wd = WordDictionary()
        wd.add_word("hello")

        assert not wd.search("hell")
        assert not wd.search("helloo")
        assert not wd.search("h.....")  # 6 dots - too long
        assert wd.search("h....")  # 5 chars should match "hello"


class TestCompressedTrie:
    """Test compressed trie (radix tree)."""

    def test_insert_and_search(self):
        """Test basic insert and search."""
        ct = CompressedTrie()
        ct.insert("test")
        ct.insert("testing")
        ct.insert("tested")

        assert ct.search("test")
        assert ct.search("testing")
        assert ct.search("tested")
        assert not ct.search("tes")

    def test_starts_with(self):
        """Test prefix checking."""
        ct = CompressedTrie()
        ct.insert("hello")
        ct.insert("help")

        assert ct.starts_with("hel")
        assert ct.starts_with("hello")
        assert not ct.starts_with("world")

    def test_shared_prefix(self):
        """Test words with shared prefix."""
        ct = CompressedTrie()
        ct.insert("romane")
        ct.insert("romanus")
        ct.insert("romulus")
        ct.insert("rubens")

        assert ct.search("romane")
        assert ct.search("romanus")
        assert ct.search("romulus")
        assert ct.search("rubens")
        assert not ct.search("rom")
        assert not ct.search("roman")

    def test_len(self):
        """Test length."""
        ct = CompressedTrie()
        assert len(ct) == 0

        ct.insert("a")
        ct.insert("ab")
        ct.insert("abc")
        assert len(ct) == 3

    def test_contains(self):
        """Test membership."""
        ct = CompressedTrie()
        ct.insert("test")

        assert "test" in ct
        assert "testing" not in ct

    def test_edge_splitting(self):
        """Test edge splitting when inserting."""
        ct = CompressedTrie()
        ct.insert("test")
        ct.insert("team")  # Should split at 'te'

        assert ct.search("test")
        assert ct.search("team")

    def test_exact_prefix_insert(self):
        """Test inserting exact prefix."""
        ct = CompressedTrie()
        ct.insert("testing")
        ct.insert("test")  # Insert prefix of existing

        assert ct.search("test")
        assert ct.search("testing")


class TestTrieEdgeCases:
    """Test edge cases."""

    def test_unicode_characters(self):
        """Test unicode support."""
        trie = Trie()
        trie.insert("hello")
        trie.insert("helloworld")

        assert trie.search("hello")
        assert trie.search("helloworld")

    def test_long_words(self):
        """Test very long words."""
        trie = Trie()
        long_word = "a" * 1000
        trie.insert(long_word)

        assert trie.search(long_word)
        assert not trie.search(long_word[:-1])

    def test_many_similar_prefixes(self):
        """Test many words with similar prefixes."""
        trie = Trie()
        for i in range(100):
            trie.insert(f"prefix{i}")

        assert len(trie) == 100
        assert trie.starts_with("prefix")
        assert trie.count_words_with_prefix("prefix") == 100

    def test_special_characters(self):
        """Test special characters in words."""
        trie = Trie()
        trie.insert("hello-world")
        trie.insert("hello_world")
        trie.insert("hello.world")

        assert trie.search("hello-world")
        assert trie.search("hello_world")
        assert trie.search("hello.world")
