//! Trie (Prefix Tree)
//!
//! A trie is a tree-like data structure used to store and retrieve strings
//! efficiently. Each node represents a character, and paths from root to
//! nodes represent prefixes.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Trie containing: "apple", "app", "apt", "bat", "ball"                     ║
//! ║                                                                            ║
//! ║                         ┌───┐                                              ║
//! ║                         │ ε │ (root)                                       ║
//! ║                         └─┬─┘                                              ║
//! ║               ┌───────────┼───────────┐                                    ║
//! ║               ▼                       ▼                                    ║
//! ║             ┌───┐                   ┌───┐                                  ║
//! ║             │ a │                   │ b │                                  ║
//! ║             └─┬─┘                   └─┬─┘                                  ║
//! ║               ▼                       ▼                                    ║
//! ║             ┌───┐                   ┌───┐                                  ║
//! ║             │ p │                   │ a │                                  ║
//! ║             └─┬─┘                   └─┬─┘                                  ║
//! ║         ┌─────┼─────┐           ┌─────┴─────┐                              ║
//! ║         ▼           ▼           ▼           ▼                              ║
//! ║       ┌───┐       ┌───┐       ┌───┐       ┌───┐                            ║
//! ║       │ p*│       │ t*│       │ t*│       │ l │                            ║
//! ║       └─┬─┘       └───┘       └───┘       └─┬─┘                            ║
//! ║         ▼                                   ▼                              ║
//! ║       ┌───┐                               ┌───┐                            ║
//! ║       │ l │                               │ l*│                            ║
//! ║       └─┬─┘                               └───┘                            ║
//! ║         ▼                                                                  ║
//! ║       ┌───┐                                                                ║
//! ║       │ e*│                                                                ║
//! ║       └───┘                                                                ║
//! ║                                                                            ║
//! ║  * = end of word marker                                                    ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation       | Average | Worst | Space |
//! |-----------------|---------|-------|-------|
//! | Insert          | O(m)    | O(m)  | O(m)  |
//! | Search          | O(m)    | O(m)  | O(1)  |
//! | StartsWith      | O(m)    | O(m)  | O(1)  |
//! | Delete          | O(m)    | O(m)  | O(1)  |
//! | Autocomplete    | O(p+n)  | O(p+n)| O(n)  |
//!
//! Where m = length of the word, p = prefix length, n = number of results
//!
//! ## LeetCode Problems
//!
//! - [#208 Implement Trie (Prefix Tree)](https://leetcode.com/problems/implement-trie-prefix-tree/)
//! - [#211 Design Add and Search Words Data Structure](https://leetcode.com/problems/design-add-and-search-words-data-structure/)
//! - [#212 Word Search II](https://leetcode.com/problems/word-search-ii/)
//! - [#648 Replace Words](https://leetcode.com/problems/replace-words/)
//! - [#677 Map Sum Pairs](https://leetcode.com/problems/map-sum-pairs/)
//! - [#1268 Search Suggestions System](https://leetcode.com/problems/search-suggestions-system/)
//!
//! ## Use Cases
//!
//! - Autocomplete systems
//! - Spell checkers
//! - IP routing (longest prefix match)
//! - Dictionary implementations
//! - Word games (Boggle, Scrabble)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::trees::Trie;
//!
//! let mut trie = Trie::new();
//! trie.insert("apple");
//! trie.insert("app");
//! trie.insert("application");
//!
//! assert!(trie.search("apple"));
//! assert!(trie.search("app"));
//! assert!(!trie.search("appl"));
//!
//! assert!(trie.starts_with("app"));
//! assert!(!trie.starts_with("ban"));
//!
//! let suggestions = trie.autocomplete("app");
//! assert_eq!(suggestions.len(), 3);
//! ```

use alloc::boxed::Box;
use alloc::collections::BTreeMap;
use alloc::string::String;
use alloc::vec::Vec;

use dsa_core::Container;

/// A node in the trie.
#[derive(Debug, Clone, Default)]
struct TrieNode {
    children: BTreeMap<char, Box<TrieNode>>,
    is_end_of_word: bool,
}

impl TrieNode {
    fn new() -> Self {
        TrieNode {
            children: BTreeMap::new(),
            is_end_of_word: false,
        }
    }
}

/// A trie (prefix tree) implementation for efficient string operations.
#[derive(Debug, Clone)]
pub struct Trie {
    root: TrieNode,
    size: usize,
}

impl Trie {
    /// Creates a new empty trie.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let trie = Trie::new();
    /// assert!(trie.is_empty());
    /// ```
    #[must_use]
    pub fn new() -> Self {
        Trie {
            root: TrieNode::new(),
            size: 0,
        }
    }

    /// Returns the number of words in the trie.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.size
    }

    /// Returns `true` if the trie contains no words.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    /// Inserts a word into the trie.
    ///
    /// # Time Complexity
    /// O(m) where m is the length of the word
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let mut trie = Trie::new();
    /// trie.insert("hello");
    /// trie.insert("world");
    /// assert_eq!(trie.len(), 2);
    /// ```
    pub fn insert(&mut self, word: &str) {
        let mut current = &mut self.root;

        for ch in word.chars() {
            current = current
                .children
                .entry(ch)
                .or_insert_with(|| Box::new(TrieNode::new()));
        }

        if !current.is_end_of_word {
            current.is_end_of_word = true;
            self.size += 1;
        }
    }

    /// Searches for a word in the trie.
    ///
    /// Returns `true` if the exact word exists in the trie.
    ///
    /// # Time Complexity
    /// O(m) where m is the length of the word
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let mut trie = Trie::new();
    /// trie.insert("hello");
    /// assert!(trie.search("hello"));
    /// assert!(!trie.search("hell"));
    /// assert!(!trie.search("help"));
    /// ```
    #[must_use]
    pub fn search(&self, word: &str) -> bool {
        self.find_node(word)
            .map_or(false, |node| node.is_end_of_word)
    }

    /// Checks if any word in the trie starts with the given prefix.
    ///
    /// # Time Complexity
    /// O(m) where m is the length of the prefix
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let mut trie = Trie::new();
    /// trie.insert("hello");
    /// trie.insert("help");
    /// assert!(trie.starts_with("hel"));
    /// assert!(trie.starts_with("hello"));
    /// assert!(!trie.starts_with("world"));
    /// ```
    #[must_use]
    pub fn starts_with(&self, prefix: &str) -> bool {
        self.find_node(prefix).is_some()
    }

    /// Helper function to find a node matching the given prefix.
    fn find_node(&self, prefix: &str) -> Option<&TrieNode> {
        let mut current = &self.root;

        for ch in prefix.chars() {
            match current.children.get(&ch) {
                Some(node) => current = node,
                None => return None,
            }
        }

        Some(current)
    }

    /// Helper function to find a mutable node matching the given prefix.
    fn find_node_mut(&mut self, prefix: &str) -> Option<&mut TrieNode> {
        let mut current = &mut self.root;

        for ch in prefix.chars() {
            match current.children.get_mut(&ch) {
                Some(node) => current = node,
                None => return None,
            }
        }

        Some(current)
    }

    /// Returns all words with the given prefix (autocomplete).
    ///
    /// # Time Complexity
    /// O(p + n) where p is the prefix length and n is the number of matching words
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let mut trie = Trie::new();
    /// trie.insert("apple");
    /// trie.insert("app");
    /// trie.insert("application");
    /// trie.insert("banana");
    ///
    /// let results = trie.autocomplete("app");
    /// assert!(results.contains(&String::from("app")));
    /// assert!(results.contains(&String::from("apple")));
    /// assert!(results.contains(&String::from("application")));
    /// ```
    #[must_use]
    pub fn autocomplete(&self, prefix: &str) -> Vec<String> {
        let mut results = Vec::new();

        if let Some(node) = self.find_node(prefix) {
            let mut current_word = String::from(prefix);
            Self::collect_words(node, &mut current_word, &mut results);
        }

        results
    }

    /// Helper function to collect all words from a node.
    fn collect_words(node: &TrieNode, current: &mut String, results: &mut Vec<String>) {
        if node.is_end_of_word {
            results.push(current.clone());
        }

        for (&ch, child) in &node.children {
            current.push(ch);
            Self::collect_words(child, current, results);
            current.pop();
        }
    }

    /// Removes a word from the trie.
    ///
    /// Returns `true` if the word was found and removed.
    ///
    /// # Time Complexity
    /// O(m) where m is the length of the word
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::trees::Trie;
    ///
    /// let mut trie = Trie::new();
    /// trie.insert("hello");
    /// assert!(trie.search("hello"));
    ///
    /// trie.remove("hello");
    /// assert!(!trie.search("hello"));
    /// ```
    pub fn remove(&mut self, word: &str) -> bool {
        if let Some(node) = self.find_node_mut(word) {
            if node.is_end_of_word {
                node.is_end_of_word = false;
                self.size -= 1;
                return true;
            }
        }
        false
    }

    /// Returns the number of words with the given prefix.
    ///
    /// # Time Complexity
    /// O(p + n) where p is the prefix length and n is the number of matching nodes
    #[must_use]
    pub fn count_words_with_prefix(&self, prefix: &str) -> usize {
        match self.find_node(prefix) {
            Some(node) => Self::count_words(node),
            None => 0,
        }
    }

    fn count_words(node: &TrieNode) -> usize {
        let mut count = if node.is_end_of_word { 1 } else { 0 };

        for child in node.children.values() {
            count += Self::count_words(child);
        }

        count
    }

    /// Returns the longest common prefix of all words in the trie.
    ///
    /// # Time Complexity
    /// O(m) where m is the length of the longest common prefix
    #[must_use]
    pub fn longest_common_prefix(&self) -> String {
        let mut prefix = String::new();
        let mut current = &self.root;

        while current.children.len() == 1 && !current.is_end_of_word {
            if let Some((&ch, child)) = current.children.iter().next() {
                prefix.push(ch);
                current = child;
            }
        }

        prefix
    }

    /// Clears the trie, removing all words.
    ///
    /// # Time Complexity
    /// O(1)
    pub fn clear(&mut self) {
        self.root = TrieNode::new();
        self.size = 0;
    }

    /// Returns all words in the trie.
    ///
    /// # Time Complexity
    /// O(n) where n is the total number of characters in all words
    #[must_use]
    pub fn all_words(&self) -> Vec<String> {
        let mut results = Vec::new();
        let mut current = String::new();
        Self::collect_words(&self.root, &mut current, &mut results);
        results
    }

    /// Searches for a word using wildcard matching where '.' matches any single character.
    ///
    /// # Time Complexity
    /// O(m * 26) worst case for all wildcards, O(m) for no wildcards
    #[must_use]
    pub fn search_with_wildcard(&self, word: &str) -> bool {
        Self::search_wildcard_helper(&self.root, word.chars().collect::<Vec<_>>().as_slice())
    }

    fn search_wildcard_helper(node: &TrieNode, chars: &[char]) -> bool {
        if chars.is_empty() {
            return node.is_end_of_word;
        }

        let ch = chars[0];
        let rest = &chars[1..];

        if ch == '.' {
            // Wildcard: try all children
            for child in node.children.values() {
                if Self::search_wildcard_helper(child, rest) {
                    return true;
                }
            }
            false
        } else {
            // Exact match
            match node.children.get(&ch) {
                Some(child) => Self::search_wildcard_helper(child, rest),
                None => false,
            }
        }
    }
}

impl Default for Trie {
    fn default() -> Self {
        Self::new()
    }
}

impl Container for Trie {
    fn len(&self) -> usize {
        self.size
    }
}

impl FromIterator<String> for Trie {
    fn from_iter<I: IntoIterator<Item = String>>(iter: I) -> Self {
        let mut trie = Trie::new();
        for word in iter {
            trie.insert(&word);
        }
        trie
    }
}

impl<'a> FromIterator<&'a str> for Trie {
    fn from_iter<I: IntoIterator<Item = &'a str>>(iter: I) -> Self {
        let mut trie = Trie::new();
        for word in iter {
            trie.insert(word);
        }
        trie
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let trie = Trie::new();
            assert!(trie.is_empty());
            assert_eq!(trie.len(), 0);
        }

        #[test]
        fn test_default() {
            let trie = Trie::default();
            assert!(trie.is_empty());
        }

        #[test]
        fn test_from_iter() {
            let trie: Trie = ["apple", "app", "banana"].iter().copied().collect();
            assert_eq!(trie.len(), 3);
            assert!(trie.search("apple"));
            assert!(trie.search("banana"));
        }
    }

    mod insert_search {
        use super::*;

        #[test]
        fn test_insert_single() {
            let mut trie = Trie::new();
            trie.insert("hello");
            assert_eq!(trie.len(), 1);
            assert!(trie.search("hello"));
        }

        #[test]
        fn test_insert_multiple() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("world");
            trie.insert("help");
            assert_eq!(trie.len(), 3);
        }

        #[test]
        fn test_insert_duplicate() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("hello");
            assert_eq!(trie.len(), 1);
        }

        #[test]
        fn test_insert_prefix() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("app");
            assert_eq!(trie.len(), 2);
            assert!(trie.search("apple"));
            assert!(trie.search("app"));
        }

        #[test]
        fn test_search_not_found() {
            let mut trie = Trie::new();
            trie.insert("hello");
            assert!(!trie.search("hell")); // Prefix only, not a word
            assert!(!trie.search("world")); // Not in trie
        }

        #[test]
        fn test_search_empty_trie() {
            let trie = Trie::new();
            assert!(!trie.search("hello"));
        }

        #[test]
        fn test_search_empty_string() {
            let mut trie = Trie::new();
            trie.insert("");
            assert!(trie.search(""));
        }
    }

    mod starts_with {
        use super::*;

        #[test]
        fn test_starts_with() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("help");
            assert!(trie.starts_with("hel"));
            assert!(trie.starts_with("hello"));
            assert!(trie.starts_with("h"));
            assert!(trie.starts_with(""));
        }

        #[test]
        fn test_starts_with_not_found() {
            let mut trie = Trie::new();
            trie.insert("hello");
            assert!(!trie.starts_with("world"));
            assert!(!trie.starts_with("hey"));
        }
    }

    mod remove {
        use super::*;

        #[test]
        fn test_remove() {
            let mut trie = Trie::new();
            trie.insert("hello");
            assert!(trie.remove("hello"));
            assert!(!trie.search("hello"));
            assert_eq!(trie.len(), 0);
        }

        #[test]
        fn test_remove_not_found() {
            let mut trie = Trie::new();
            trie.insert("hello");
            assert!(!trie.remove("world"));
            assert_eq!(trie.len(), 1);
        }

        #[test]
        fn test_remove_prefix() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("app");
            assert!(trie.remove("app"));
            assert!(!trie.search("app"));
            assert!(trie.search("apple"));
            assert_eq!(trie.len(), 1);
        }

        #[test]
        fn test_remove_leaves_prefix() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("app");
            assert!(trie.remove("apple"));
            assert!(!trie.search("apple"));
            assert!(trie.search("app"));
            assert!(trie.starts_with("app"));
        }
    }

    mod autocomplete {
        use super::*;

        #[test]
        fn test_autocomplete() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("app");
            trie.insert("application");
            trie.insert("banana");

            let results = trie.autocomplete("app");
            assert_eq!(results.len(), 3);
            assert!(results.contains(&String::from("app")));
            assert!(results.contains(&String::from("apple")));
            assert!(results.contains(&String::from("application")));
        }

        #[test]
        fn test_autocomplete_no_match() {
            let mut trie = Trie::new();
            trie.insert("hello");
            let results = trie.autocomplete("world");
            assert!(results.is_empty());
        }

        #[test]
        fn test_autocomplete_all() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("world");
            let results = trie.autocomplete("");
            assert_eq!(results.len(), 2);
        }
    }

    mod count_prefix {
        use super::*;

        #[test]
        fn test_count_words_with_prefix() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("app");
            trie.insert("application");
            trie.insert("banana");

            assert_eq!(trie.count_words_with_prefix("app"), 3);
            assert_eq!(trie.count_words_with_prefix("apple"), 1);
            assert_eq!(trie.count_words_with_prefix("b"), 1);
            assert_eq!(trie.count_words_with_prefix("c"), 0);
        }
    }

    mod longest_common_prefix {
        use super::*;

        #[test]
        fn test_longest_common_prefix() {
            let mut trie = Trie::new();
            trie.insert("flower");
            trie.insert("flow");
            trie.insert("flight");

            assert_eq!(trie.longest_common_prefix(), "fl");
        }

        #[test]
        fn test_longest_common_prefix_none() {
            let mut trie = Trie::new();
            trie.insert("apple");
            trie.insert("banana");

            assert_eq!(trie.longest_common_prefix(), "");
        }

        #[test]
        fn test_longest_common_prefix_full() {
            let mut trie = Trie::new();
            trie.insert("hello");

            assert_eq!(trie.longest_common_prefix(), "hello");
        }
    }

    mod wildcard_search {
        use super::*;

        #[test]
        fn test_search_with_wildcard() {
            let mut trie = Trie::new();
            trie.insert("bad");
            trie.insert("dad");
            trie.insert("mad");

            assert!(trie.search_with_wildcard("pad") == false);
            assert!(trie.search_with_wildcard("bad"));
            assert!(trie.search_with_wildcard(".ad"));
            assert!(trie.search_with_wildcard("b.."));
        }

        #[test]
        fn test_wildcard_all_dots() {
            let mut trie = Trie::new();
            trie.insert("abc");
            trie.insert("xyz");

            assert!(trie.search_with_wildcard("..."));
            assert!(!trie.search_with_wildcard(".."));
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_clear() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("world");
            trie.clear();
            assert!(trie.is_empty());
            assert!(!trie.search("hello"));
        }

        #[test]
        fn test_all_words() {
            let mut trie = Trie::new();
            trie.insert("hello");
            trie.insert("world");
            trie.insert("help");

            let words = trie.all_words();
            assert_eq!(words.len(), 3);
            assert!(words.contains(&String::from("hello")));
            assert!(words.contains(&String::from("world")));
            assert!(words.contains(&String::from("help")));
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_char_words() {
            let mut trie = Trie::new();
            trie.insert("a");
            trie.insert("b");
            trie.insert("c");
            assert_eq!(trie.len(), 3);
            assert!(trie.search("a"));
            assert!(trie.search("b"));
        }

        #[test]
        fn test_unicode() {
            let mut trie = Trie::new();
            trie.insert("こんにちは");
            trie.insert("世界");
            assert!(trie.search("こんにちは"));
            assert!(trie.starts_with("こん"));
        }

        #[test]
        fn test_long_words() {
            let mut trie = Trie::new();
            let long_word = "a".repeat(1000);
            trie.insert(&long_word);
            assert!(trie.search(&long_word));
        }

        #[test]
        fn test_many_words() {
            let mut trie = Trie::new();
            for i in 0..1000 {
                trie.insert(&format!("word{}", i));
            }
            assert_eq!(trie.len(), 1000);
            assert!(trie.search("word500"));
        }
    }
}
