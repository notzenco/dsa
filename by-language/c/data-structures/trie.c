/**
 * Trie Implementation
 * See trie.h for documentation
 */

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============== Helper Functions ============== */

static TrieNode *create_node(void) {
    TrieNode *node = calloc(1, sizeof(TrieNode));
    return node;
}

static void destroy_node(TrieNode *node) {
    if (node == NULL) return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        destroy_node(node->children[i]);
    }
    free(node);
}

static int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A';
    return -1;
}

static bool is_valid_word(const char *word) {
    if (word == NULL) return false;
    for (size_t i = 0; word[i] != '\0'; i++) {
        if (!isalpha((unsigned char)word[i])) return false;
    }
    return true;
}

static bool has_children(const TrieNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) return true;
    }
    return false;
}

/* ============== Creation/Destruction ============== */

Trie *trie_create(void) {
    Trie *trie = malloc(sizeof(Trie));
    if (trie == NULL) return NULL;

    trie->root = create_node();
    if (trie->root == NULL) {
        free(trie);
        return NULL;
    }

    trie->word_count = 0;
    return trie;
}

void trie_destroy(Trie *trie) {
    if (trie == NULL) return;
    destroy_node(trie->root);
    free(trie);
}

void trie_clear(Trie *trie) {
    if (trie == NULL) return;

    /* Destroy all children of root */
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        destroy_node(trie->root->children[i]);
        trie->root->children[i] = NULL;
    }
    trie->root->is_end_of_word = false;
    trie->root->prefix_count = 0;
    trie->word_count = 0;
}

/* ============== Basic Operations ============== */

bool trie_insert(Trie *trie, const char *word) {
    if (trie == NULL || word == NULL || !is_valid_word(word)) {
        return false;
    }

    TrieNode *current = trie->root;

    for (size_t i = 0; word[i] != '\0'; i++) {
        int index = char_to_index(word[i]);
        if (index < 0) return false;

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
            if (current->children[index] == NULL) {
                return false;
            }
        }
        current = current->children[index];
        current->prefix_count++;
    }

    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        trie->word_count++;
    }
    return true;
}

bool trie_search(const Trie *trie, const char *word) {
    if (trie == NULL || word == NULL) return false;

    const TrieNode *current = trie->root;

    for (size_t i = 0; word[i] != '\0'; i++) {
        int index = char_to_index(word[i]);
        if (index < 0 || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }

    return current->is_end_of_word;
}

static bool delete_helper(TrieNode *node, const char *word, size_t depth, bool *deleted) {
    if (node == NULL) return false;

    if (word[depth] == '\0') {
        if (node->is_end_of_word) {
            node->is_end_of_word = false;
            *deleted = true;
            return !has_children(node);
        }
        return false;
    }

    int index = char_to_index(word[depth]);
    if (index < 0 || node->children[index] == NULL) {
        return false;
    }

    bool should_delete = delete_helper(node->children[index], word, depth + 1, deleted);

    if (should_delete) {
        free(node->children[index]);
        node->children[index] = NULL;
        return !node->is_end_of_word && !has_children(node);
    }

    if (*deleted && node->children[index] != NULL) {
        node->children[index]->prefix_count--;
    }

    return false;
}

bool trie_delete(Trie *trie, const char *word) {
    if (trie == NULL || word == NULL || word[0] == '\0') {
        return false;
    }

    bool deleted = false;
    delete_helper(trie->root, word, 0, &deleted);

    if (deleted) {
        trie->word_count--;
    }
    return deleted;
}

bool trie_starts_with(const Trie *trie, const char *prefix) {
    if (trie == NULL || prefix == NULL) return false;

    const TrieNode *current = trie->root;

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        int index = char_to_index(prefix[i]);
        if (index < 0 || current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }

    return true;
}

/* ============== Advanced Operations ============== */

int trie_count_prefix(const Trie *trie, const char *prefix) {
    if (trie == NULL || prefix == NULL) return 0;

    const TrieNode *current = trie->root;

    for (size_t i = 0; prefix[i] != '\0'; i++) {
        int index = char_to_index(prefix[i]);
        if (index < 0 || current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }

    return current->prefix_count;
}

static void collect_words(const TrieNode *node, char *buffer, int depth,
                          char **words, int *count, int max_words) {
    if (*count >= max_words) return;

    if (node->is_end_of_word) {
        buffer[depth] = '\0';
        words[*count] = strdup(buffer);
        if (words[*count] != NULL) {
            (*count)++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE && *count < max_words; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = 'a' + i;
            collect_words(node->children[i], buffer, depth + 1,
                          words, count, max_words);
        }
    }
}

int trie_get_all_words(const Trie *trie, char **words, int max_words) {
    if (trie == NULL || words == NULL || max_words <= 0) return 0;

    char buffer[256];
    int count = 0;
    collect_words(trie->root, buffer, 0, words, &count, max_words);
    return count;
}

int trie_autocomplete(const Trie *trie, const char *prefix,
                      char **words, int max_words) {
    if (trie == NULL || prefix == NULL || words == NULL || max_words <= 0) {
        return 0;
    }

    /* Navigate to prefix node */
    const TrieNode *current = trie->root;
    size_t prefix_len = strlen(prefix);

    for (size_t i = 0; i < prefix_len; i++) {
        int index = char_to_index(prefix[i]);
        if (index < 0 || current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }

    /* Collect all words from this point */
    char buffer[256];
    memcpy(buffer, prefix, prefix_len);
    int count = 0;
    collect_words(current, buffer, (int)prefix_len, words, &count, max_words);
    return count;
}

int trie_longest_common_prefix(const Trie *trie, char *result, int max_len) {
    if (trie == NULL || result == NULL || max_len <= 0) {
        if (result && max_len > 0) result[0] = '\0';
        return 0;
    }

    if (trie->word_count == 0) {
        result[0] = '\0';
        return 0;
    }

    const TrieNode *current = trie->root;
    int len = 0;

    while (len < max_len - 1) {
        int child_count = 0;
        int last_child = -1;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i] != NULL) {
                child_count++;
                last_child = i;
            }
        }

        /* Must have exactly one child and not be end of word */
        if (child_count != 1 || current->is_end_of_word) {
            break;
        }

        result[len++] = 'a' + last_child;
        current = current->children[last_child];
    }

    result[len] = '\0';
    return len;
}

static bool wildcard_search(const TrieNode *node, const char *pattern, size_t index) {
    if (node == NULL) return false;

    if (pattern[index] == '\0') {
        return node->is_end_of_word;
    }

    if (pattern[index] == '.') {
        /* Match any character */
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i] != NULL &&
                wildcard_search(node->children[i], pattern, index + 1)) {
                return true;
            }
        }
        return false;
    }

    int idx = char_to_index(pattern[index]);
    if (idx < 0) return false;

    return wildcard_search(node->children[idx], pattern, index + 1);
}

bool trie_search_wildcard(const Trie *trie, const char *pattern) {
    if (trie == NULL || pattern == NULL) return false;
    return wildcard_search(trie->root, pattern, 0);
}

/* ============== Utility ============== */

int trie_word_count(const Trie *trie) {
    return trie != NULL ? trie->word_count : 0;
}

bool trie_is_empty(const Trie *trie) {
    return trie == NULL || trie->word_count == 0;
}

int trie_shortest_unique_prefix(const Trie *trie, const char *word,
                                char *result, int max_len) {
    if (trie == NULL || word == NULL || result == NULL || max_len <= 0) {
        if (result && max_len > 0) result[0] = '\0';
        return -1;
    }

    if (!trie_search(trie, word)) {
        result[0] = '\0';
        return -1;
    }

    const TrieNode *current = trie->root;
    int len = 0;

    for (size_t i = 0; word[i] != '\0' && len < max_len - 1; i++) {
        int index = char_to_index(word[i]);
        if (index < 0 || current->children[index] == NULL) {
            break;
        }

        result[len++] = word[i];
        current = current->children[index];

        /* If prefix_count is 1, this prefix is unique */
        if (current->prefix_count == 1) {
            break;
        }
    }

    result[len] = '\0';
    return len;
}
