package dsa.datastructures;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class TrieTest {

    private Trie trie;

    @BeforeEach
    void setUp() {
        trie = new Trie();
    }

    // ======================== Insert ========================

    @Test
    void testInsertSingleWord() {
        trie.insert("hello");
        assertEquals(1, trie.size());
        assertTrue(trie.search("hello"));
    }

    @Test
    void testInsertMultipleWords() {
        trie.insert("hello");
        trie.insert("world");
        trie.insert("help");
        assertEquals(3, trie.size());
    }

    @Test
    void testInsertDuplicateWord() {
        trie.insert("hello");
        trie.insert("hello");
        assertEquals(1, trie.size());
    }

    @Test
    void testInsertEmptyString() {
        trie.insert("");
        assertEquals(1, trie.size());
        assertTrue(trie.search(""));
    }

    @Test
    void testInsertNull() {
        trie.insert(null);
        assertEquals(0, trie.size());
    }

    @Test
    void testInsertPrefixOfExistingWord() {
        trie.insert("hello");
        trie.insert("hell");
        assertEquals(2, trie.size());
        assertTrue(trie.search("hello"));
        assertTrue(trie.search("hell"));
    }

    // ======================== Search ========================

    @Test
    void testSearchExisting() {
        trie.insert("apple");
        trie.insert("banana");
        assertTrue(trie.search("apple"));
        assertTrue(trie.search("banana"));
    }

    @Test
    void testSearchNonExisting() {
        trie.insert("apple");
        assertFalse(trie.search("app"));
        assertFalse(trie.search("application"));
        assertFalse(trie.search("orange"));
    }

    @Test
    void testSearchEmptyTrie() {
        assertFalse(trie.search("hello"));
    }

    @Test
    void testSearchNull() {
        assertFalse(trie.search(null));
    }

    @Test
    void testSearchEmptyString() {
        assertFalse(trie.search(""));
        trie.insert("");
        assertTrue(trie.search(""));
    }

    // ======================== startsWith ========================

    @Test
    void testStartsWithExistingPrefix() {
        trie.insert("apple");
        trie.insert("application");
        assertTrue(trie.startsWith("app"));
        assertTrue(trie.startsWith("apple"));
        assertTrue(trie.startsWith("a"));
    }

    @Test
    void testStartsWithNonExistingPrefix() {
        trie.insert("apple");
        assertFalse(trie.startsWith("b"));
        assertFalse(trie.startsWith("applez"));
    }

    @Test
    void testStartsWithEmptyPrefix() {
        trie.insert("hello");
        assertTrue(trie.startsWith(""));
    }

    @Test
    void testStartsWithNull() {
        assertFalse(trie.startsWith(null));
    }

    // ======================== Delete ========================

    @Test
    void testDeleteExistingWord() {
        trie.insert("hello");
        assertTrue(trie.delete("hello"));
        assertEquals(0, trie.size());
        assertFalse(trie.search("hello"));
    }

    @Test
    void testDeleteNonExistingWord() {
        trie.insert("hello");
        assertFalse(trie.delete("world"));
        assertEquals(1, trie.size());
    }

    @Test
    void testDeleteNull() {
        assertFalse(trie.delete(null));
    }

    @Test
    void testDeletePrefixDoesNotAffectLongerWord() {
        trie.insert("hello");
        trie.insert("hell");
        assertTrue(trie.delete("hell"));
        assertEquals(1, trie.size());
        assertTrue(trie.search("hello"));
        assertFalse(trie.search("hell"));
    }

    @Test
    void testDeleteLongerWordDoesNotAffectPrefix() {
        trie.insert("hello");
        trie.insert("hell");
        assertTrue(trie.delete("hello"));
        assertEquals(1, trie.size());
        assertTrue(trie.search("hell"));
        assertFalse(trie.search("hello"));
    }

    @Test
    void testDeleteWordWithSharedPrefix() {
        trie.insert("apple");
        trie.insert("application");
        trie.insert("app");
        assertTrue(trie.delete("apple"));
        assertEquals(2, trie.size());
        assertFalse(trie.search("apple"));
        assertTrue(trie.search("application"));
        assertTrue(trie.search("app"));
    }

    @Test
    void testDeleteAllWords() {
        trie.insert("a");
        trie.insert("b");
        trie.insert("c");
        assertTrue(trie.delete("a"));
        assertTrue(trie.delete("b"));
        assertTrue(trie.delete("c"));
        assertEquals(0, trie.size());
    }

    // ======================== getWordsWithPrefix ========================

    @Test
    void testGetWordsWithPrefix() {
        trie.insert("apple");
        trie.insert("application");
        trie.insert("app");
        trie.insert("banana");
        List<String> words = trie.getWordsWithPrefix("app");
        assertEquals(3, words.size());
        assertTrue(words.contains("app"));
        assertTrue(words.contains("apple"));
        assertTrue(words.contains("application"));
        assertFalse(words.contains("banana"));
    }

    @Test
    void testGetWordsWithPrefixNoMatch() {
        trie.insert("apple");
        List<String> words = trie.getWordsWithPrefix("xyz");
        assertTrue(words.isEmpty());
    }

    @Test
    void testGetWordsWithPrefixEmptyPrefix() {
        trie.insert("a");
        trie.insert("b");
        trie.insert("c");
        List<String> words = trie.getWordsWithPrefix("");
        assertEquals(3, words.size());
    }

    @Test
    void testGetWordsWithPrefixNull() {
        List<String> words = trie.getWordsWithPrefix(null);
        assertTrue(words.isEmpty());
    }

    @Test
    void testGetWordsWithPrefixSorted() {
        trie.insert("cat");
        trie.insert("car");
        trie.insert("card");
        trie.insert("care");
        List<String> words = trie.getWordsWithPrefix("car");
        assertEquals(Arrays.asList("car", "card", "care"), words);
    }

    // ======================== countWordsWithPrefix ========================

    @Test
    void testCountWordsWithPrefix() {
        trie.insert("apple");
        trie.insert("application");
        trie.insert("app");
        trie.insert("banana");
        assertEquals(3, trie.countWordsWithPrefix("app"));
        assertEquals(1, trie.countWordsWithPrefix("ban"));
        assertEquals(4, trie.countWordsWithPrefix(""));
    }

    @Test
    void testCountWordsWithPrefixNoMatch() {
        trie.insert("apple");
        assertEquals(0, trie.countWordsWithPrefix("xyz"));
    }

    @Test
    void testCountWordsWithPrefixNull() {
        assertEquals(0, trie.countWordsWithPrefix(null));
    }

    // ======================== longestCommonPrefix ========================

    @Test
    void testLongestCommonPrefix() {
        trie.insert("flower");
        trie.insert("flow");
        trie.insert("flight");
        assertEquals("fl", trie.longestCommonPrefix());
    }

    @Test
    void testLongestCommonPrefixAllSame() {
        trie.insert("hello");
        trie.insert("hello");
        assertEquals("hello", trie.longestCommonPrefix());
    }

    @Test
    void testLongestCommonPrefixNoCommon() {
        trie.insert("abc");
        trie.insert("xyz");
        assertEquals("", trie.longestCommonPrefix());
    }

    @Test
    void testLongestCommonPrefixSingleWord() {
        trie.insert("hello");
        assertEquals("hello", trie.longestCommonPrefix());
    }

    @Test
    void testLongestCommonPrefixEmpty() {
        assertEquals("", trie.longestCommonPrefix());
    }

    // ======================== autocomplete ========================

    @Test
    void testAutocomplete() {
        trie.insert("apple");
        trie.insert("application");
        trie.insert("app");
        trie.insert("appreciate");
        trie.insert("banana");

        List<String> results = trie.autocomplete("app", 3);
        assertEquals(3, results.size());
        // Should return first 3 alphabetically
        for (String s : results) {
            assertTrue(s.startsWith("app"));
        }
    }

    @Test
    void testAutocompleteLimitExceedsResults() {
        trie.insert("apple");
        trie.insert("app");
        List<String> results = trie.autocomplete("app", 10);
        assertEquals(2, results.size());
    }

    @Test
    void testAutocompleteNoMatch() {
        trie.insert("apple");
        List<String> results = trie.autocomplete("xyz", 5);
        assertTrue(results.isEmpty());
    }

    @Test
    void testAutocompleteLimitZero() {
        trie.insert("apple");
        List<String> results = trie.autocomplete("app", 0);
        assertTrue(results.isEmpty());
    }

    @Test
    void testAutocompleteNull() {
        List<String> results = trie.autocomplete(null, 5);
        assertTrue(results.isEmpty());
    }

    @Test
    void testAutocompleteExactMatch() {
        trie.insert("hello");
        List<String> results = trie.autocomplete("hello", 5);
        assertEquals(1, results.size());
        assertEquals("hello", results.get(0));
    }

    // ======================== size / contains ========================

    @Test
    void testSize() {
        assertEquals(0, trie.size());
        trie.insert("a");
        assertEquals(1, trie.size());
        trie.insert("b");
        assertEquals(2, trie.size());
        trie.delete("a");
        assertEquals(1, trie.size());
    }

    @Test
    void testContains() {
        trie.insert("hello");
        assertTrue(trie.contains("hello"));
        assertFalse(trie.contains("hell"));
        assertFalse(trie.contains("world"));
    }

    // ======================== Edge Cases ========================

    @Test
    void testSingleCharacterWords() {
        trie.insert("a");
        trie.insert("b");
        trie.insert("c");
        assertTrue(trie.search("a"));
        assertTrue(trie.search("b"));
        assertTrue(trie.search("c"));
        assertEquals(3, trie.size());
    }

    @Test
    void testLongWords() {
        String longWord = "a".repeat(1000);
        trie.insert(longWord);
        assertTrue(trie.search(longWord));
        assertTrue(trie.startsWith("a".repeat(500)));
    }

    @Test
    void testSpecialCharacters() {
        trie.insert("hello-world");
        trie.insert("hello_world");
        trie.insert("hello world");
        assertEquals(3, trie.size());
        assertTrue(trie.search("hello-world"));
        assertTrue(trie.search("hello_world"));
        assertTrue(trie.search("hello world"));
    }

    @Test
    void testDeleteThenReinsert() {
        trie.insert("hello");
        assertTrue(trie.delete("hello"));
        assertFalse(trie.search("hello"));
        trie.insert("hello");
        assertTrue(trie.search("hello"));
        assertEquals(1, trie.size());
    }

    @Test
    void testMixedOperations() {
        trie.insert("apple");
        trie.insert("app");
        trie.insert("application");
        trie.insert("apply");
        trie.insert("apt");

        assertEquals(5, trie.size());
        assertTrue(trie.delete("apply"));
        assertEquals(4, trie.size());
        assertEquals(3, trie.countWordsWithPrefix("app"));
        assertTrue(trie.startsWith("app"));

        List<String> words = trie.getWordsWithPrefix("app");
        assertEquals(3, words.size());
        assertTrue(words.contains("app"));
        assertTrue(words.contains("apple"));
        assertTrue(words.contains("application"));
    }
}
