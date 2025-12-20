/**
 * TRIE (PREFIX TREE)
 *
 * A tree-like data structure for efficient string operations.
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                              STRUCTURE                                    ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║                       (root)                                              ║
 * ║                      /   |   \                                            ║
 * ║                    a     b     c                                          ║
 * ║                   /|\    |     |                                          ║
 * ║                  p n t   a     a                                          ║
 * ║                  |  |    |     |                                          ║
 * ║                  p* d*   t*    t*                                         ║
 * ║                  |                                                        ║
 * ║                  l                                                        ║
 * ║                  |                                                        ║
 * ║                  e*       Words: app, apple, and, ant, bat, cat           ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                            COMPLEXITY                                     ║
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * ║                                                                           ║
 * ║  Operation          │ Time   │ Space                                      ║
 * ║  ───────────────────┼────────┼──────────                                  ║
 * ║  Insert             │ O(m)   │ O(m)    m = word length                    ║
 * ║  Search             │ O(m)   │ O(1)                                       ║
 * ║  Delete             │ O(m)   │ O(1)                                       ║
 * ║  Prefix Search      │ O(p)   │ O(1)    p = prefix length                  ║
 * ║  Autocomplete       │ O(p+k) │ O(k)    k = number of results              ║
 * ║                                                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 *
 * LEETCODE PROBLEMS:
 * - #208 Implement Trie (Prefix Tree)
 * - #211 Design Add and Search Words Data Structure
 * - #212 Word Search II
 * - #648 Replace Words
 * - #677 Map Sum Pairs
 */

#ifndef TRIE_H
#define TRIE_H

#include <stddef.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

/* ============== Trie Node ============== */

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
    int prefix_count;   /* Number of words with this prefix */
} TrieNode;

/* ============== Trie Structure ============== */

typedef struct {
    TrieNode *root;
    int word_count;
} Trie;

/* ============== Creation/Destruction ============== */

/**
 * Create a new trie.
 * @return New trie or NULL on failure
 */
Trie *trie_create(void);

/**
 * Destroy a trie and free all memory.
 * @param trie Trie to destroy
 */
void trie_destroy(Trie *trie);

/**
 * Remove all words from trie.
 * @param trie Trie to clear
 */
void trie_clear(Trie *trie);

/* ============== Basic Operations ============== */

/**
 * Insert a word into the trie.
 * @param trie Trie
 * @param word Word to insert (lowercase letters only)
 * @return true on success
 */
bool trie_insert(Trie *trie, const char *word);

/**
 * Search for a word in the trie.
 * @param trie Trie
 * @param word Word to search for
 * @return true if word exists
 */
bool trie_search(const Trie *trie, const char *word);

/**
 * Delete a word from the trie.
 * @param trie Trie
 * @param word Word to delete
 * @return true if word was found and deleted
 */
bool trie_delete(Trie *trie, const char *word);

/**
 * Check if any word starts with given prefix.
 * @param trie Trie
 * @param prefix Prefix to check
 * @return true if prefix exists
 */
bool trie_starts_with(const Trie *trie, const char *prefix);

/* ============== Advanced Operations ============== */

/**
 * Count words with given prefix.
 * @param trie Trie
 * @param prefix Prefix to count
 * @return Number of words with this prefix
 */
int trie_count_prefix(const Trie *trie, const char *prefix);

/**
 * Get all words in the trie.
 * @param trie Trie
 * @param words Output array of words (caller must free each and array)
 * @param max_words Maximum words to return
 * @return Number of words found
 */
int trie_get_all_words(const Trie *trie, char **words, int max_words);

/**
 * Get all words with given prefix (autocomplete).
 * @param trie Trie
 * @param prefix Prefix to search
 * @param words Output array of words
 * @param max_words Maximum words to return
 * @return Number of words found
 */
int trie_autocomplete(const Trie *trie, const char *prefix,
                      char **words, int max_words);

/**
 * Find longest common prefix of all words.
 * @param trie Trie
 * @param result Buffer for result
 * @param max_len Maximum length of result
 * @return Length of longest common prefix
 */
int trie_longest_common_prefix(const Trie *trie, char *result, int max_len);

/**
 * Search with wildcard '.' matching any character.
 * @param trie Trie
 * @param pattern Pattern with possible '.' wildcards
 * @return true if any word matches
 */
bool trie_search_wildcard(const Trie *trie, const char *pattern);

/* ============== Utility ============== */

/**
 * Get number of words in trie.
 * @param trie Trie
 * @return Number of words
 */
int trie_word_count(const Trie *trie);

/**
 * Check if trie is empty.
 * @param trie Trie
 * @return true if empty
 */
bool trie_is_empty(const Trie *trie);

/**
 * Get shortest unique prefix for a word.
 * @param trie Trie
 * @param word Word to find prefix for
 * @param result Buffer for result
 * @param max_len Maximum length
 * @return Length of prefix, or -1 if word not found
 */
int trie_shortest_unique_prefix(const Trie *trie, const char *word,
                                char *result, int max_len);

#endif /* TRIE_H */
