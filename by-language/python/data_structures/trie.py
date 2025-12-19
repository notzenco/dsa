"""
Trie (Prefix Tree) Implementation.

A Trie is a tree-like data structure for storing strings that allows
efficient prefix-based operations.

╔═══════════════════════════════════════════════════════════════════════════════╗
║                           TRIE VISUAL REPRESENTATION                           ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                                                               ║
║    Words: ["cat", "car", "card", "care", "dog"]                               ║
║                                                                               ║
║                        (root)                                                 ║
║                       /      \                                                ║
║                      c        d                                               ║
║                      |        |                                               ║
║                      a        o                                               ║
║                      |        |                                               ║
║                      t*       g*                                              ║
║                     /                                                         ║
║                    r*                                                         ║
║                   / \                                                         ║
║                  d*  e*                                                       ║
║                                                                               ║
║    * = end of word marker                                                     ║
║                                                                               ║
║    Search "car":  root -> c -> a -> r (found, is_end=True)                    ║
║    Search "ca":   root -> c -> a (found, but is_end=False)                    ║
║    Prefix "ca":   Returns ["cat", "car", "card", "care"]                      ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬──────────┬──────────┐
│ Operation           │ Time     │ Space    │
├─────────────────────┼──────────┼──────────┤
│ Insert              │ O(m)     │ O(m)     │
│ Search              │ O(m)     │ O(1)     │
│ Starts With         │ O(m)     │ O(1)     │
│ Delete              │ O(m)     │ O(1)     │
│ Get All With Prefix │ O(m + k) │ O(k)     │
└─────────────────────┴──────────┴──────────┘
Where m = length of word/prefix, k = total characters in matching words

LEETCODE PROBLEMS:
- #208 Implement Trie (Prefix Tree)
- #211 Design Add and Search Words Data Structure
- #212 Word Search II
- #648 Replace Words
- #677 Map Sum Pairs
- #1268 Search Suggestions System

USE CASES:
- Autocomplete systems
- Spell checkers
- IP routing (longest prefix matching)
- Dictionary implementations
- Word games (Scrabble, Boggle)
"""

from typing import Dict, List, Optional, Iterator, Tuple
from collections import deque


class TrieNode:
    """A node in the Trie."""

    __slots__ = ['children', 'is_end', 'value', 'count']

    def __init__(self) -> None:
        """Initialize a trie node."""
        self.children: Dict[str, 'TrieNode'] = {}
        self.is_end: bool = False
        self.value: Optional[any] = None  # For key-value storage
        self.count: int = 0  # Number of words passing through this node


class Trie:
    """
    Trie (Prefix Tree) implementation.

    A tree-like data structure that stores strings character by character,
    enabling efficient prefix-based operations.

    Example:
        >>> trie = Trie()
        >>> trie.insert("hello")
        >>> trie.insert("help")
        >>> trie.insert("world")
        >>> trie.search("hello")
        True
        >>> trie.starts_with("hel")
        True
        >>> trie.get_words_with_prefix("hel")
        ['hello', 'help']
    """

    def __init__(self) -> None:
        """Initialize an empty trie."""
        self._root = TrieNode()
        self._word_count = 0

    def insert(self, word: str) -> None:
        """
        Insert a word into the trie.

        Args:
            word: The word to insert

        Time: O(m) where m is length of word
        Space: O(m) for new nodes
        """
        node = self._root
        for char in word:
            node.count += 1
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]

        if not node.is_end:
            self._word_count += 1
        node.is_end = True
        node.count += 1

    def search(self, word: str) -> bool:
        """
        Check if a word exists in the trie.

        Args:
            word: The word to search for

        Returns:
            True if word exists, False otherwise

        Time: O(m) where m is length of word
        Space: O(1)
        """
        node = self._find_node(word)
        return node is not None and node.is_end

    def starts_with(self, prefix: str) -> bool:
        """
        Check if any word in the trie starts with the given prefix.

        Args:
            prefix: The prefix to search for

        Returns:
            True if any word starts with prefix, False otherwise

        Time: O(m) where m is length of prefix
        Space: O(1)
        """
        return self._find_node(prefix) is not None

    def delete(self, word: str) -> bool:
        """
        Delete a word from the trie.

        Args:
            word: The word to delete

        Returns:
            True if word was deleted, False if word didn't exist

        Time: O(m) where m is length of word
        Space: O(1)
        """
        # First check if word exists
        if not self.search(word):
            return False

        node = self._root
        path: List[Tuple[TrieNode, str]] = []

        # Traverse and record path
        for char in word:
            path.append((node, char))
            node.count -= 1
            node = node.children[char]

        node.count -= 1
        node.is_end = False
        self._word_count -= 1

        # Clean up nodes that are no longer needed
        for parent, char in reversed(path):
            child = parent.children[char]
            if child.count == 0:
                del parent.children[char]
            else:
                break

        return True

    def get_words_with_prefix(self, prefix: str) -> List[str]:
        """
        Get all words that start with the given prefix.

        Args:
            prefix: The prefix to search for

        Returns:
            List of all words starting with prefix

        Time: O(m + k) where m is prefix length, k is total chars in results
        Space: O(k) for result storage
        """
        node = self._find_node(prefix)
        if node is None:
            return []

        words = []
        self._collect_words(node, prefix, words)
        return words

    def count_words_with_prefix(self, prefix: str) -> int:
        """
        Count how many words start with the given prefix.

        Args:
            prefix: The prefix to search for

        Returns:
            Number of words starting with prefix

        Time: O(m) where m is prefix length
        Space: O(1)
        """
        node = self._find_node(prefix)
        return node.count if node else 0

    def longest_common_prefix(self) -> str:
        """
        Find the longest common prefix of all words in the trie.

        Returns:
            The longest common prefix, or empty string if none

        Time: O(m) where m is length of longest common prefix
        Space: O(m) for result string
        """
        if self._word_count == 0:
            return ""

        prefix = []
        node = self._root

        while len(node.children) == 1 and not node.is_end:
            char = next(iter(node.children))
            prefix.append(char)
            node = node.children[char]

        return ''.join(prefix)

    def autocomplete(self, prefix: str, limit: int = 10) -> List[str]:
        """
        Get autocomplete suggestions for a prefix.

        Args:
            prefix: The prefix to complete
            limit: Maximum number of suggestions

        Returns:
            List of suggested words (up to limit)

        Time: O(m + k) where m is prefix length, k is chars explored
        Space: O(limit) for result
        """
        node = self._find_node(prefix)
        if node is None:
            return []

        words = []
        self._collect_words_limited(node, prefix, words, limit)
        return words

    def _find_node(self, prefix: str) -> Optional[TrieNode]:
        """Find the node corresponding to the given prefix."""
        node = self._root
        for char in prefix:
            if char not in node.children:
                return None
            node = node.children[char]
        return node

    def _collect_words(self, node: TrieNode, prefix: str, words: List[str]) -> None:
        """Recursively collect all words from a node."""
        if node.is_end:
            words.append(prefix)

        for char, child in node.children.items():
            self._collect_words(child, prefix + char, words)

    def _collect_words_limited(
        self, node: TrieNode, prefix: str, words: List[str], limit: int
    ) -> None:
        """Collect words with a limit using BFS for shortest words first."""
        queue = deque([(node, prefix)])

        while queue and len(words) < limit:
            curr_node, curr_prefix = queue.popleft()

            if curr_node.is_end:
                words.append(curr_prefix)

            for char in sorted(curr_node.children.keys()):
                if len(words) >= limit:
                    break
                queue.append((curr_node.children[char], curr_prefix + char))

    def __len__(self) -> int:
        """Return the number of words in the trie."""
        return self._word_count

    def __contains__(self, word: str) -> bool:
        """Check if word is in the trie."""
        return self.search(word)

    def __iter__(self) -> Iterator[str]:
        """Iterate over all words in the trie."""
        words = []
        self._collect_words(self._root, "", words)
        return iter(words)

    def __repr__(self) -> str:
        """Return string representation."""
        return f"Trie(words={self._word_count})"


class TrieMap:
    """
    Trie-based map that stores key-value pairs.

    Similar to a dictionary but with efficient prefix operations.

    Example:
        >>> trie_map = TrieMap()
        >>> trie_map["hello"] = 1
        >>> trie_map["help"] = 2
        >>> trie_map["hello"]
        1
        >>> trie_map.sum_with_prefix("hel")
        3
    """

    def __init__(self) -> None:
        """Initialize an empty trie map."""
        self._root = TrieNode()
        self._size = 0

    def __setitem__(self, key: str, value: any) -> None:
        """Set a key-value pair."""
        node = self._root
        for char in key:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]

        if not node.is_end:
            self._size += 1
        node.is_end = True
        node.value = value

    def __getitem__(self, key: str) -> any:
        """Get value for a key."""
        node = self._find_node(key)
        if node is None or not node.is_end:
            raise KeyError(key)
        return node.value

    def __contains__(self, key: str) -> bool:
        """Check if key exists."""
        node = self._find_node(key)
        return node is not None and node.is_end

    def __len__(self) -> int:
        """Return number of key-value pairs."""
        return self._size

    def get(self, key: str, default: any = None) -> any:
        """Get value for key with default."""
        node = self._find_node(key)
        if node is None or not node.is_end:
            return default
        return node.value

    def sum_with_prefix(self, prefix: str) -> int:
        """
        Sum all values for keys starting with prefix.

        Args:
            prefix: The prefix to match

        Returns:
            Sum of values (assumes numeric values)
        """
        node = self._find_node(prefix)
        if node is None:
            return 0
        return self._sum_values(node)

    def items_with_prefix(self, prefix: str) -> List[Tuple[str, any]]:
        """Get all key-value pairs for keys starting with prefix."""
        node = self._find_node(prefix)
        if node is None:
            return []

        items = []
        self._collect_items(node, prefix, items)
        return items

    def _find_node(self, prefix: str) -> Optional[TrieNode]:
        """Find node for prefix."""
        node = self._root
        for char in prefix:
            if char not in node.children:
                return None
            node = node.children[char]
        return node

    def _sum_values(self, node: TrieNode) -> int:
        """Recursively sum values from a node."""
        total = node.value if node.is_end else 0
        for child in node.children.values():
            total += self._sum_values(child)
        return total

    def _collect_items(
        self, node: TrieNode, prefix: str, items: List[Tuple[str, any]]
    ) -> None:
        """Collect key-value pairs from a node."""
        if node.is_end:
            items.append((prefix, node.value))

        for char, child in node.children.items():
            self._collect_items(child, prefix + char, items)


class WordDictionary:
    """
    Trie with wildcard search support.

    Supports '.' as a wildcard that matches any single character.

    LeetCode #211: Design Add and Search Words Data Structure

    Example:
        >>> wd = WordDictionary()
        >>> wd.add_word("bad")
        >>> wd.add_word("dad")
        >>> wd.search("pad")
        False
        >>> wd.search("b.d")
        True
        >>> wd.search(".ad")
        True
    """

    def __init__(self) -> None:
        """Initialize word dictionary."""
        self._root = TrieNode()

    def add_word(self, word: str) -> None:
        """Add a word to the dictionary."""
        node = self._root
        for char in word:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.is_end = True

    def search(self, word: str) -> bool:
        """
        Search for a word, where '.' matches any character.

        Args:
            word: The pattern to search for

        Returns:
            True if pattern matches any word
        """
        return self._search_helper(word, 0, self._root)

    def _search_helper(self, word: str, index: int, node: TrieNode) -> bool:
        """Recursive helper for wildcard search."""
        if index == len(word):
            return node.is_end

        char = word[index]

        if char == '.':
            # Try all children
            for child in node.children.values():
                if self._search_helper(word, index + 1, child):
                    return True
            return False
        else:
            if char not in node.children:
                return False
            return self._search_helper(word, index + 1, node.children[char])


class CompressedTrie:
    """
    Compressed Trie (Radix Tree / Patricia Trie).

    A space-optimized trie where chains of single-child nodes are merged.

    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║                        COMPRESSED TRIE EXAMPLE                            ║
    ╠═══════════════════════════════════════════════════════════════════════════╣
    ║                                                                           ║
    ║    Standard Trie for ["test", "team", "toast"]:                           ║
    ║                                                                           ║
    ║            (root)                     Compressed:        (root)           ║
    ║              |                                            /  \            ║
    ║              t                                          "t"   "toast"*    ║
    ║             / \                         →               / \               ║
    ║            e   o                                     "e"  "oast"*         ║
    ║           / \   \                                    / \                  ║
    ║          s   a   a                               "st"* "am"*              ║
    ║          |   |   |                                                        ║
    ║          t*  m*  s                                                        ║
    ║                  |                                                        ║
    ║                  t*                                                       ║
    ║                                                                           ║
    ╚═══════════════════════════════════════════════════════════════════════════╝
    """

    def __init__(self) -> None:
        """Initialize compressed trie."""
        self._root: Dict[str, any] = {}  # edge_label -> (is_end, children)
        self._word_count = 0

    def insert(self, word: str) -> None:
        """Insert a word into the compressed trie."""
        if not word:
            return

        node = self._root
        i = 0

        while i < len(word):
            # Find edge that shares a prefix
            found = False
            for edge_label in list(node.keys()):
                common_len = self._common_prefix_length(edge_label, word[i:])

                if common_len > 0:
                    found = True

                    if common_len == len(edge_label):
                        # Edge is a prefix of remaining word
                        is_end, children = node[edge_label]
                        if common_len == len(word) - i:
                            # Exact match
                            if not is_end:
                                self._word_count += 1
                            node[edge_label] = (True, children)
                            return
                        else:
                            # Continue with remaining
                            i += common_len
                            node = children
                    else:
                        # Need to split the edge
                        is_end, children = node[edge_label]

                        # Create new node for split point
                        new_children = {}
                        new_children[edge_label[common_len:]] = (is_end, children)

                        del node[edge_label]

                        remaining = word[i + common_len:]
                        if remaining:
                            new_children[remaining] = (True, {})
                            node[edge_label[:common_len]] = (False, new_children)
                        else:
                            node[edge_label[:common_len]] = (True, new_children)

                        self._word_count += 1
                        return
                    break

            if not found:
                # No matching edge, create new one
                node[word[i:]] = (True, {})
                self._word_count += 1
                return

    def search(self, word: str) -> bool:
        """Search for exact word in compressed trie."""
        if not word:
            return False

        node = self._root
        i = 0

        while i < len(word):
            found = False
            for edge_label in node:
                if word[i:].startswith(edge_label):
                    is_end, children = node[edge_label]
                    i += len(edge_label)
                    if i == len(word):
                        return is_end
                    node = children
                    found = True
                    break
                elif edge_label.startswith(word[i:]):
                    # Word is prefix of edge
                    return False

            if not found:
                return False

        return False

    def starts_with(self, prefix: str) -> bool:
        """Check if any word starts with prefix."""
        if not prefix:
            return self._word_count > 0

        node = self._root
        i = 0

        while i < len(prefix):
            found = False
            for edge_label in node:
                remaining = prefix[i:]

                if edge_label.startswith(remaining):
                    return True
                elif remaining.startswith(edge_label):
                    _, children = node[edge_label]
                    i += len(edge_label)
                    node = children
                    found = True
                    break

            if not found:
                return False

        return True

    def _common_prefix_length(self, s1: str, s2: str) -> int:
        """Find length of common prefix between two strings."""
        min_len = min(len(s1), len(s2))
        for i in range(min_len):
            if s1[i] != s2[i]:
                return i
        return min_len

    def __len__(self) -> int:
        """Return number of words."""
        return self._word_count

    def __contains__(self, word: str) -> bool:
        """Check if word exists."""
        return self.search(word)
