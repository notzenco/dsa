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
}
