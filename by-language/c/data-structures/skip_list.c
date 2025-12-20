/**
 * Skip List Implementation
 * See skip_list.h for documentation
 */

#include "skip_list.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============== Helper Functions ============== */

static int random_level(double prob) {
    int level = 1;
    /* Use rand() for simplicity - could use better RNG */
    while ((rand() / (double)RAND_MAX) < prob && level < SKIPLIST_MAX_LEVEL) {
        level++;
    }
    return level;
}

static SkipListNode *create_node(int key, int value, int level) {
    SkipListNode *node = malloc(sizeof(SkipListNode));
    if (node == NULL) return NULL;

    node->forward = calloc(level, sizeof(SkipListNode *));
    if (node->forward == NULL) {
        free(node);
        return NULL;
    }

    node->key = key;
    node->value = value;
    node->level = level;

    return node;
}

static void destroy_node(SkipListNode *node) {
    if (node == NULL) return;
    free(node->forward);
    free(node);
}

/* ============== Creation/Destruction ============== */

SkipList *skiplist_create_with_prob(double prob) {
    if (prob <= 0 || prob >= 1) prob = 0.5;

    SkipList *list = malloc(sizeof(SkipList));
    if (list == NULL) return NULL;

    /* Create header node */
    list->head = create_node(0, 0, SKIPLIST_MAX_LEVEL);
    if (list->head == NULL) {
        free(list);
        return NULL;
    }

    list->level = 1;
    list->size = 0;
    list->prob = prob;

    /* Seed random number generator */
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    return list;
}

SkipList *skiplist_create(void) {
    return skiplist_create_with_prob(0.5);
}

void skiplist_destroy(SkipList *list) {
    if (list == NULL) return;

    SkipListNode *node = list->head;
    while (node != NULL) {
        SkipListNode *next = node->forward[0];
        destroy_node(node);
        node = next;
    }

    free(list);
}

void skiplist_clear(SkipList *list) {
    if (list == NULL) return;

    /* Free all nodes except header */
    SkipListNode *node = list->head->forward[0];
    while (node != NULL) {
        SkipListNode *next = node->forward[0];
        destroy_node(node);
        node = next;
    }

    /* Reset header */
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) {
        list->head->forward[i] = NULL;
    }

    list->level = 1;
    list->size = 0;
}

/* ============== Core Operations ============== */

bool skiplist_insert(SkipList *list, int key, int value) {
    if (list == NULL) return false;

    SkipListNode *update[SKIPLIST_MAX_LEVEL];
    SkipListNode *current = list->head;

    /* Find insert position at each level */
    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    /* Key already exists - update value */
    if (current != NULL && current->key == key) {
        current->value = value;
        return false;
    }

    /* Generate random level for new node */
    int new_level = random_level(list->prob);

    /* Update list level if needed */
    if (new_level > list->level) {
        for (int i = list->level; i < new_level; i++) {
            update[i] = list->head;
        }
        list->level = new_level;
    }

    /* Create and insert new node */
    SkipListNode *new_node = create_node(key, value, new_level);
    if (new_node == NULL) return false;

    for (int i = 0; i < new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    list->size++;
    return true;
}

bool skiplist_search(const SkipList *list, int key, int *value) {
    if (list == NULL) return false;

    SkipListNode *current = list->head;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != NULL && current->key == key) {
        if (value != NULL) *value = current->value;
        return true;
    }

    return false;
}

bool skiplist_delete(SkipList *list, int key) {
    if (list == NULL) return false;

    SkipListNode *update[SKIPLIST_MAX_LEVEL];
    SkipListNode *current = list->head;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key) {
        return false;
    }

    /* Update forward pointers */
    for (int i = 0; i < list->level; i++) {
        if (update[i]->forward[i] != current) {
            break;
        }
        update[i]->forward[i] = current->forward[i];
    }

    destroy_node(current);

    /* Update list level */
    while (list->level > 1 && list->head->forward[list->level - 1] == NULL) {
        list->level--;
    }

    list->size--;
    return true;
}

bool skiplist_contains(const SkipList *list, int key) {
    return skiplist_search(list, key, NULL);
}

/* ============== Range Operations ============== */

size_t skiplist_range(const SkipList *list, int min_key, int max_key,
                      int *keys, size_t max_keys) {
    if (list == NULL || keys == NULL || max_keys == 0 || min_key > max_key) {
        return 0;
    }

    SkipListNode *current = list->head;

    /* Find first node >= min_key */
    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < min_key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    /* Collect keys in range */
    size_t count = 0;
    while (current != NULL && current->key <= max_key && count < max_keys) {
        keys[count++] = current->key;
        current = current->forward[0];
    }

    return count;
}

bool skiplist_ceiling(const SkipList *list, int key, int *result) {
    if (list == NULL || result == NULL) return false;

    SkipListNode *current = list->head;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != NULL) {
        *result = current->key;
        return true;
    }

    return false;
}

bool skiplist_floor(const SkipList *list, int key, int *result) {
    if (list == NULL || result == NULL) return false;

    SkipListNode *current = list->head;
    SkipListNode *floor = NULL;

    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key <= key) {
            current = current->forward[i];
        }
        if (current != list->head && current->key <= key) {
            floor = current;
        }
    }

    if (floor != NULL) {
        *result = floor->key;
        return true;
    }

    return false;
}

/* ============== Utility ============== */

size_t skiplist_size(const SkipList *list) {
    return list ? list->size : 0;
}

bool skiplist_is_empty(const SkipList *list) {
    return list == NULL || list->size == 0;
}

bool skiplist_min(const SkipList *list, int *key) {
    if (list == NULL || key == NULL || list->size == 0) return false;

    *key = list->head->forward[0]->key;
    return true;
}

bool skiplist_max(const SkipList *list, int *key) {
    if (list == NULL || key == NULL || list->size == 0) return false;

    SkipListNode *current = list->head;
    for (int i = list->level - 1; i >= 0; i--) {
        while (current->forward[i] != NULL) {
            current = current->forward[i];
        }
    }

    *key = current->key;
    return true;
}

size_t skiplist_get_keys(const SkipList *list, int *keys, size_t max_keys) {
    if (list == NULL || keys == NULL || max_keys == 0) return 0;

    SkipListNode *current = list->head->forward[0];
    size_t count = 0;

    while (current != NULL && count < max_keys) {
        keys[count++] = current->key;
        current = current->forward[0];
    }

    return count;
}
