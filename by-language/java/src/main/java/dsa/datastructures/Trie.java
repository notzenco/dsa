package dsa.datastructures;

import java.util.*;

/**
 * Trie (prefix tree) implementation for strings.
 */
public class Trie {

    public class TrieNode {
        Map<Character, TrieNode> children;
        boolean isEnd;
        int count; // number of words that pass through this node

        public TrieNode() {
            this.children = new HashMap<>();
            this.isEnd = false;
            this.count = 0;
        }

        public Map<Character, TrieNode> getChildren() {
            return children;
        }

        public boolean isEnd() {
            return isEnd;
        }

        public int getCount() {
            return count;
        }
    }

    private final TrieNode root;
    private int wordCount;

    public Trie() {
        this.root = new TrieNode();
        this.wordCount = 0;
    }

    public TrieNode getRoot() {
        return root;
    }

    /**
     * Insert a word into the trie.
     */
    public void insert(String word) {
        if (word == null) return;
        TrieNode current = root;
        for (char ch : word.toCharArray()) {
            current.children.putIfAbsent(ch, new TrieNode());
            current = current.children.get(ch);
            current.count++;
        }
        if (!current.isEnd) {
            current.isEnd = true;
            wordCount++;
        }
    }

    /**
     * Search for an exact word in the trie.
     */
    public boolean search(String word) {
        if (word == null) return false;
        TrieNode node = findNode(word);
        return node != null && node.isEnd;
    }

    /**
     * Check if any word in the trie starts with the given prefix.
     */
    public boolean startsWith(String prefix) {
        if (prefix == null) return false;
        return findNode(prefix) != null;
    }

    /**
     * Delete a word from the trie.
     *
     * @return true if the word was found and deleted
     */
    public boolean delete(String word) {
        if (word == null || !search(word)) return false;
        deleteHelper(root, word, 0);
        wordCount--;
        return true;
    }

    private boolean deleteHelper(TrieNode node, String word, int depth) {
        if (depth == word.length()) {
            node.isEnd = false;
            return node.children.isEmpty();
        }

        char ch = word.charAt(depth);
        TrieNode child = node.children.get(ch);
        if (child == null) return false;

        child.count--;
        boolean shouldDelete = deleteHelper(child, word, depth + 1);

        if (shouldDelete) {
            node.children.remove(ch);
            return !node.isEnd && node.children.isEmpty();
        }
        return false;
    }

    /**
     * Get all words that start with the given prefix.
     */
    public List<String> getWordsWithPrefix(String prefix) {
        List<String> result = new ArrayList<>();
        if (prefix == null) return result;
        TrieNode node = findNode(prefix);
        if (node == null) return result;
        collectWords(node, new StringBuilder(prefix), result);
        return result;
    }

    private void collectWords(TrieNode node, StringBuilder prefix, List<String> result) {
        if (node.isEnd) {
            result.add(prefix.toString());
        }
        // Use a TreeMap to get sorted order
        TreeMap<Character, TrieNode> sorted = new TreeMap<>(node.children);
        for (Map.Entry<Character, TrieNode> entry : sorted.entrySet()) {
            prefix.append(entry.getKey());
            collectWords(entry.getValue(), prefix, result);
            prefix.deleteCharAt(prefix.length() - 1);
        }
    }

    /**
     * Count the number of words that start with the given prefix.
     */
    public int countWordsWithPrefix(String prefix) {
        if (prefix == null) return 0;
        TrieNode node = findNode(prefix);
        if (node == null) return 0;
        return countWordsBelow(node);
    }

    private int countWordsBelow(TrieNode node) {
        int count = node.isEnd ? 1 : 0;
        for (TrieNode child : node.children.values()) {
            count += countWordsBelow(child);
        }
        return count;
    }

    /**
     * Find the longest common prefix among all words in the trie.
     */
    public String longestCommonPrefix() {
        if (wordCount == 0) return "";
        StringBuilder prefix = new StringBuilder();
        TrieNode current = root;
        while (current.children.size() == 1 && !current.isEnd) {
            Map.Entry<Character, TrieNode> entry = current.children.entrySet().iterator().next();
            prefix.append(entry.getKey());
            current = entry.getValue();
        }
        return prefix.toString();
    }

    /**
     * Autocomplete: return up to 'limit' words that start with the given prefix.
     */
    public List<String> autocomplete(String prefix, int limit) {
        List<String> result = new ArrayList<>();
        if (prefix == null || limit <= 0) return result;
        TrieNode node = findNode(prefix);
        if (node == null) return result;
        collectWordsLimited(node, new StringBuilder(prefix), result, limit);
        return result;
    }

    private void collectWordsLimited(TrieNode node, StringBuilder prefix, List<String> result, int limit) {
        if (result.size() >= limit) return;
        if (node.isEnd) {
            result.add(prefix.toString());
            if (result.size() >= limit) return;
        }
        TreeMap<Character, TrieNode> sorted = new TreeMap<>(node.children);
        for (Map.Entry<Character, TrieNode> entry : sorted.entrySet()) {
            if (result.size() >= limit) return;
            prefix.append(entry.getKey());
            collectWordsLimited(entry.getValue(), prefix, result, limit);
            prefix.deleteCharAt(prefix.length() - 1);
        }
    }

    /**
     * Return the total number of unique words in the trie.
     */
    public int size() {
        return wordCount;
    }

    /**
     * Check if a word exists (alias for search).
     */
    public boolean contains(String word) {
        return search(word);
    }

    /**
     * Find the node corresponding to the last character of the given string.
     */
    private TrieNode findNode(String str) {
        TrieNode current = root;
        for (char ch : str.toCharArray()) {
            TrieNode child = current.children.get(ch);
            if (child == null) return null;
            current = child;
        }
        return current;
    }

    // ========== TrieMap ==========

    /**
     * A trie-based map that maps string keys to values.
     *
     * @param <V> the value type
     */
    public static class TrieMap<V> {

        private static class Node<V> {
            Map<Character, Node<V>> children = new HashMap<>();
            V value;
            boolean hasValue;
        }

        private final Node<V> root;
        private int size;

        public TrieMap() {
            this.root = new Node<>();
            this.size = 0;
        }

        /**
         * Associates the given value with the given key.
         */
        public void put(String key, V value) {
            if (key == null) {
                throw new IllegalArgumentException("Key must not be null");
            }
            Node<V> current = root;
            for (char ch : key.toCharArray()) {
                current.children.putIfAbsent(ch, new Node<>());
                current = current.children.get(ch);
            }
            if (!current.hasValue) {
                size++;
            }
            current.value = value;
            current.hasValue = true;
        }

        /**
         * Returns the value associated with the given key, if present.
         */
        public Optional<V> get(String key) {
            if (key == null) {
                return Optional.empty();
            }
            Node<V> node = findNode(key);
            if (node == null || !node.hasValue) {
                return Optional.empty();
            }
            return Optional.ofNullable(node.value);
        }

        /**
         * Checks whether the map contains the given key.
         */
        public boolean containsKey(String key) {
            if (key == null) {
                return false;
            }
            Node<V> node = findNode(key);
            return node != null && node.hasValue;
        }

        /**
         * Removes the given key from the map.
         *
         * @return true if the key was found and removed
         */
        public boolean delete(String key) {
            if (key == null || !containsKey(key)) {
                return false;
            }
            deleteHelper(root, key, 0);
            size--;
            return true;
        }

        private boolean deleteHelper(Node<V> node, String key, int depth) {
            if (depth == key.length()) {
                node.hasValue = false;
                node.value = null;
                return node.children.isEmpty();
            }
            char ch = key.charAt(depth);
            Node<V> child = node.children.get(ch);
            if (child == null) {
                return false;
            }
            boolean shouldDelete = deleteHelper(child, key, depth + 1);
            if (shouldDelete) {
                node.children.remove(ch);
                return !node.hasValue && node.children.isEmpty();
            }
            return false;
        }

        /**
         * Returns the number of key-value mappings in the map.
         */
        public int size() {
            return size;
        }

        /**
         * Returns all keys in the map.
         */
        public List<String> keys() {
            List<String> result = new ArrayList<>();
            collectKeys(root, new StringBuilder(), result);
            return result;
        }

        /**
         * Returns all keys that start with the given prefix.
         */
        public List<String> keysWithPrefix(String prefix) {
            List<String> result = new ArrayList<>();
            if (prefix == null) {
                return result;
            }
            Node<V> node = findNode(prefix);
            if (node == null) {
                return result;
            }
            collectKeys(node, new StringBuilder(prefix), result);
            return result;
        }

        private void collectKeys(Node<V> node, StringBuilder prefix, List<String> result) {
            if (node.hasValue) {
                result.add(prefix.toString());
            }
            TreeMap<Character, Node<V>> sorted = new TreeMap<>(node.children);
            for (Map.Entry<Character, Node<V>> entry : sorted.entrySet()) {
                prefix.append(entry.getKey());
                collectKeys(entry.getValue(), prefix, result);
                prefix.deleteCharAt(prefix.length() - 1);
            }
        }

        private Node<V> findNode(String key) {
            Node<V> current = root;
            for (char ch : key.toCharArray()) {
                Node<V> child = current.children.get(ch);
                if (child == null) {
                    return null;
                }
                current = child;
            }
            return current;
        }
    }

    // ========== WordDictionary ==========

    /**
     * A dictionary that supports adding words and searching with wildcard '.'
     * which matches any single character.
     */
    public static class WordDictionary {

        private static class Node {
            Map<Character, Node> children = new HashMap<>();
            boolean isEnd;
        }

        private final Node root;

        public WordDictionary() {
            this.root = new Node();
        }

        /**
         * Adds a word to the dictionary.
         */
        public void addWord(String word) {
            if (word == null) {
                return;
            }
            Node current = root;
            for (char ch : word.toCharArray()) {
                current.children.putIfAbsent(ch, new Node());
                current = current.children.get(ch);
            }
            current.isEnd = true;
        }

        /**
         * Searches for a pattern that may contain '.' wildcards.
         * Each '.' matches exactly one character.
         *
         * @param pattern the search pattern
         * @return true if a matching word exists
         */
        public boolean search(String pattern) {
            if (pattern == null) {
                return false;
            }
            return searchHelper(root, pattern, 0);
        }

        private boolean searchHelper(Node node, String pattern, int index) {
            if (index == pattern.length()) {
                return node.isEnd;
            }
            char ch = pattern.charAt(index);
            if (ch == '.') {
                for (Node child : node.children.values()) {
                    if (searchHelper(child, pattern, index + 1)) {
                        return true;
                    }
                }
                return false;
            } else {
                Node child = node.children.get(ch);
                if (child == null) {
                    return false;
                }
                return searchHelper(child, pattern, index + 1);
            }
        }
    }
}
