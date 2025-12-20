/**
 * B-Tree Implementation
 */

#include "b_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ============== Node Creation/Destruction ============== */

static BTreeNode *create_node(int t, bool is_leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    if (!node) return NULL;

    int max_keys = 2 * t - 1;

    node->keys = malloc(sizeof(int) * max_keys);
    node->values = malloc(sizeof(int) * max_keys);
    node->children = malloc(sizeof(BTreeNode *) * (max_keys + 1));

    if (!node->keys || !node->values || !node->children) {
        free(node->keys);
        free(node->values);
        free(node->children);
        free(node);
        return NULL;
    }

    node->num_keys = 0;
    node->is_leaf = is_leaf;

    /* Initialize children to NULL */
    for (int i = 0; i <= max_keys; i++) {
        node->children[i] = NULL;
    }

    return node;
}

static void destroy_node(BTreeNode *node) {
    if (!node) return;

    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            destroy_node(node->children[i]);
        }
    }

    free(node->keys);
    free(node->values);
    free(node->children);
    free(node);
}

/* ============== Search Helpers ============== */

/* Binary search within a node */
static int find_key_index(BTreeNode *node, int key) {
    int left = 0, right = node->num_keys - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (node->keys[mid] == key) {
            return mid;
        } else if (node->keys[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return left;  /* Return insertion point */
}

/* Recursive search */
static bool search_node(BTreeNode *node, int key, int *value_out) {
    if (!node) return false;

    int i = find_key_index(node, key);

    if (i < node->num_keys && node->keys[i] == key) {
        if (value_out) *value_out = node->values[i];
        return true;
    }

    if (node->is_leaf) {
        return false;
    }

    return search_node(node->children[i], key, value_out);
}

/* ============== Split/Merge Helpers ============== */

/* Split a full child node */
static void split_child(BTree *tree, BTreeNode *parent, int index) {
    int t = tree->min_degree;
    BTreeNode *full_child = parent->children[index];

    /* Create new node to hold t-1 keys */
    BTreeNode *new_node = create_node(t, full_child->is_leaf);
    new_node->num_keys = t - 1;

    /* Copy the last t-1 keys to new node */
    for (int i = 0; i < t - 1; i++) {
        new_node->keys[i] = full_child->keys[i + t];
        new_node->values[i] = full_child->values[i + t];
    }

    /* Copy children if not leaf */
    if (!full_child->is_leaf) {
        for (int i = 0; i < t; i++) {
            new_node->children[i] = full_child->children[i + t];
        }
    }

    full_child->num_keys = t - 1;

    /* Shift parent's children to make room */
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = new_node;

    /* Shift parent's keys to make room */
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
        parent->values[i + 1] = parent->values[i];
    }

    /* Move median key up to parent */
    parent->keys[index] = full_child->keys[t - 1];
    parent->values[index] = full_child->values[t - 1];
    parent->num_keys++;
}

/* Insert into a non-full node */
static bool insert_non_full(BTree *tree, BTreeNode *node, int key, int value) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        /* Check for duplicate */
        int idx = find_key_index(node, key);
        if (idx < node->num_keys && node->keys[idx] == key) {
            node->values[idx] = value;
            return false;  /* Updated existing key */
        }

        /* Shift keys to make room */
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->num_keys++;
        return true;
    }

    /* Find child to descend to */
    while (i >= 0 && node->keys[i] > key) {
        i--;
    }
    i++;

    /* Check for duplicate before potentially splitting */
    int idx = find_key_index(node, key);
    if (idx < node->num_keys && node->keys[idx] == key) {
        node->values[idx] = value;
        return false;  /* Updated existing key */
    }

    /* Split child if full */
    if (node->children[i]->num_keys == 2 * tree->min_degree - 1) {
        split_child(tree, node, i);

        /* Decide which child to descend to */
        if (key > node->keys[i]) {
            i++;
        } else if (key == node->keys[i]) {
            node->values[i] = value;
            return false;  /* Updated existing key */
        }
    }

    return insert_non_full(tree, node->children[i], key, value);
}

/* ============== Delete Helpers ============== */

/* Get predecessor key (largest in left subtree) */
static void get_predecessor(BTreeNode *node, int idx, int *key_out, int *value_out) {
    BTreeNode *cur = node->children[idx];
    while (!cur->is_leaf) {
        cur = cur->children[cur->num_keys];
    }
    *key_out = cur->keys[cur->num_keys - 1];
    *value_out = cur->values[cur->num_keys - 1];
}

/* Get successor key (smallest in right subtree) */
static void get_successor(BTreeNode *node, int idx, int *key_out, int *value_out) {
    BTreeNode *cur = node->children[idx + 1];
    while (!cur->is_leaf) {
        cur = cur->children[0];
    }
    *key_out = cur->keys[0];
    *value_out = cur->values[0];
}

/* Merge children[idx] and children[idx+1] */
static void merge_children(BTree *tree, BTreeNode *node, int idx) {
    int t = tree->min_degree;
    BTreeNode *left = node->children[idx];
    BTreeNode *right = node->children[idx + 1];

    /* Move key from parent to left child */
    left->keys[t - 1] = node->keys[idx];
    left->values[t - 1] = node->values[idx];

    /* Copy keys from right to left */
    for (int i = 0; i < right->num_keys; i++) {
        left->keys[t + i] = right->keys[i];
        left->values[t + i] = right->values[i];
    }

    /* Copy children from right to left */
    if (!left->is_leaf) {
        for (int i = 0; i <= right->num_keys; i++) {
            left->children[t + i] = right->children[i];
        }
    }

    left->num_keys = 2 * t - 1;

    /* Shift keys in parent */
    for (int i = idx; i < node->num_keys - 1; i++) {
        node->keys[i] = node->keys[i + 1];
        node->values[i] = node->values[i + 1];
    }

    /* Shift children in parent */
    for (int i = idx + 1; i < node->num_keys; i++) {
        node->children[i] = node->children[i + 1];
    }

    node->num_keys--;

    /* Free right node */
    free(right->keys);
    free(right->values);
    free(right->children);
    free(right);
}

/* Fill child[idx] which has t-1 keys */
static void fill_child(BTree *tree, BTreeNode *node, int idx) {
    int t = tree->min_degree;

    if (idx > 0 && node->children[idx - 1]->num_keys >= t) {
        /* Borrow from left sibling */
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx - 1];

        /* Shift child's keys right */
        for (int i = child->num_keys - 1; i >= 0; i--) {
            child->keys[i + 1] = child->keys[i];
            child->values[i + 1] = child->values[i];
        }

        /* Shift children if not leaf */
        if (!child->is_leaf) {
            for (int i = child->num_keys; i >= 0; i--) {
                child->children[i + 1] = child->children[i];
            }
            child->children[0] = sibling->children[sibling->num_keys];
        }

        /* Move parent key to child */
        child->keys[0] = node->keys[idx - 1];
        child->values[0] = node->values[idx - 1];

        /* Move sibling's last key to parent */
        node->keys[idx - 1] = sibling->keys[sibling->num_keys - 1];
        node->values[idx - 1] = sibling->values[sibling->num_keys - 1];

        child->num_keys++;
        sibling->num_keys--;
    } else if (idx < node->num_keys && node->children[idx + 1]->num_keys >= t) {
        /* Borrow from right sibling */
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx + 1];

        /* Move parent key to child */
        child->keys[child->num_keys] = node->keys[idx];
        child->values[child->num_keys] = node->values[idx];

        /* Move sibling's first key to parent */
        node->keys[idx] = sibling->keys[0];
        node->values[idx] = sibling->values[0];

        /* Copy first child if not leaf */
        if (!child->is_leaf) {
            child->children[child->num_keys + 1] = sibling->children[0];
        }

        /* Shift sibling's keys left */
        for (int i = 0; i < sibling->num_keys - 1; i++) {
            sibling->keys[i] = sibling->keys[i + 1];
            sibling->values[i] = sibling->values[i + 1];
        }

        /* Shift sibling's children left */
        if (!sibling->is_leaf) {
            for (int i = 0; i < sibling->num_keys; i++) {
                sibling->children[i] = sibling->children[i + 1];
            }
        }

        child->num_keys++;
        sibling->num_keys--;
    } else {
        /* Merge with a sibling */
        if (idx < node->num_keys) {
            merge_children(tree, node, idx);
        } else {
            merge_children(tree, node, idx - 1);
        }
    }
}

/* Delete from leaf node */
static void delete_from_leaf(BTreeNode *node, int idx) {
    for (int i = idx; i < node->num_keys - 1; i++) {
        node->keys[i] = node->keys[i + 1];
        node->values[i] = node->values[i + 1];
    }
    node->num_keys--;
}

/* Delete from internal node */
static bool delete_from_internal(BTree *tree, BTreeNode *node, int idx);

/* Recursive delete */
static bool delete_key(BTree *tree, BTreeNode *node, int key) {
    int idx = find_key_index(node, key);

    if (idx < node->num_keys && node->keys[idx] == key) {
        /* Key found in this node */
        if (node->is_leaf) {
            delete_from_leaf(node, idx);
            return true;
        } else {
            return delete_from_internal(tree, node, idx);
        }
    } else {
        /* Key not in this node */
        if (node->is_leaf) {
            return false;  /* Key doesn't exist */
        }

        bool is_last_child = (idx == node->num_keys);

        /* Ensure child has enough keys */
        if (node->children[idx]->num_keys < tree->min_degree) {
            fill_child(tree, node, idx);
        }

        /* After fill, child at idx might have been merged */
        if (is_last_child && idx > node->num_keys) {
            return delete_key(tree, node->children[idx - 1], key);
        } else {
            return delete_key(tree, node->children[idx], key);
        }
    }
}

static bool delete_from_internal(BTree *tree, BTreeNode *node, int idx) {
    int key = node->keys[idx];
    int t = tree->min_degree;

    if (node->children[idx]->num_keys >= t) {
        /* Get predecessor and replace */
        int pred_key, pred_value;
        get_predecessor(node, idx, &pred_key, &pred_value);
        node->keys[idx] = pred_key;
        node->values[idx] = pred_value;
        return delete_key(tree, node->children[idx], pred_key);
    } else if (node->children[idx + 1]->num_keys >= t) {
        /* Get successor and replace */
        int succ_key, succ_value;
        get_successor(node, idx, &succ_key, &succ_value);
        node->keys[idx] = succ_key;
        node->values[idx] = succ_value;
        return delete_key(tree, node->children[idx + 1], succ_key);
    } else {
        /* Merge children and delete from merged node */
        merge_children(tree, node, idx);
        return delete_key(tree, node->children[idx], key);
    }
}

/* ============== Traversal Helpers ============== */

static size_t inorder_helper(BTreeNode *node, int *keys, size_t max_keys, size_t count) {
    if (!node || count >= max_keys) return count;

    for (int i = 0; i < node->num_keys; i++) {
        if (!node->is_leaf) {
            count = inorder_helper(node->children[i], keys, max_keys, count);
        }

        if (count < max_keys) {
            keys[count++] = node->keys[i];
        }
    }

    if (!node->is_leaf) {
        count = inorder_helper(node->children[node->num_keys], keys, max_keys, count);
    }

    return count;
}

static size_t range_helper(BTreeNode *node, int min_key, int max_key,
                           int *keys, size_t max_keys, size_t count) {
    if (!node || count >= max_keys) return count;

    int i = 0;

    /* Skip children with keys less than min_key */
    while (i < node->num_keys && node->keys[i] < min_key) {
        if (!node->is_leaf) {
            count = range_helper(node->children[i], min_key, max_key,
                               keys, max_keys, count);
        }
        i++;
    }

    /* Process keys in range */
    while (i < node->num_keys && node->keys[i] <= max_key) {
        if (!node->is_leaf) {
            count = range_helper(node->children[i], min_key, max_key,
                               keys, max_keys, count);
        }

        if (count < max_keys && node->keys[i] >= min_key) {
            keys[count++] = node->keys[i];
        }
        i++;
    }

    /* Process last child */
    if (!node->is_leaf && i <= node->num_keys) {
        count = range_helper(node->children[i], min_key, max_key,
                           keys, max_keys, count);
    }

    return count;
}

/* ============== Validation Helpers ============== */

static bool validate_node(BTreeNode *node, int t, int min_key, int max_key, int depth, int *leaf_depth) {
    if (!node) return true;

    /* Check key count bounds */
    if (depth > 0) {
        if (node->num_keys < t - 1) return false;
    }
    if (node->num_keys > 2 * t - 1) return false;

    /* Check keys are sorted and in range */
    for (int i = 0; i < node->num_keys; i++) {
        if (node->keys[i] < min_key || node->keys[i] > max_key) return false;
        if (i > 0 && node->keys[i] <= node->keys[i - 1]) return false;
    }

    if (node->is_leaf) {
        if (*leaf_depth == -1) {
            *leaf_depth = depth;
        } else if (*leaf_depth != depth) {
            return false;  /* Leaves at different depths */
        }
        return true;
    }

    /* Validate children */
    for (int i = 0; i <= node->num_keys; i++) {
        int child_min = (i == 0) ? min_key : node->keys[i - 1] + 1;
        int child_max = (i == node->num_keys) ? max_key : node->keys[i] - 1;

        if (!validate_node(node->children[i], t, child_min, child_max, depth + 1, leaf_depth)) {
            return false;
        }
    }

    return true;
}

/* ============== Print Helper ============== */

static void print_node(BTreeNode *node, int depth) {
    if (!node) return;

    /* Print indentation */
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    /* Print keys */
    printf("[");
    for (int i = 0; i < node->num_keys; i++) {
        if (i > 0) printf(", ");
        printf("%d", node->keys[i]);
    }
    printf("]\n");

    /* Print children */
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            print_node(node->children[i], depth + 1);
        }
    }
}

/* ============== Public Functions ============== */

BTree *btree_create(int min_degree) {
    if (min_degree < 2) return NULL;

    BTree *tree = malloc(sizeof(BTree));
    if (!tree) return NULL;

    tree->root = create_node(min_degree, true);
    if (!tree->root) {
        free(tree);
        return NULL;
    }

    tree->min_degree = min_degree;
    tree->size = 0;

    return tree;
}

BTree *btree_create_default(void) {
    return btree_create(BTREE_DEFAULT_MIN_DEGREE);
}

void btree_destroy(BTree *tree) {
    if (!tree) return;

    destroy_node(tree->root);
    free(tree);
}

void btree_clear(BTree *tree) {
    if (!tree) return;

    destroy_node(tree->root);
    tree->root = create_node(tree->min_degree, true);
    tree->size = 0;
}

bool btree_insert(BTree *tree, int key, int value) {
    if (!tree) return false;

    BTreeNode *root = tree->root;

    if (root->num_keys == 2 * tree->min_degree - 1) {
        /* Root is full, need to split */
        BTreeNode *new_root = create_node(tree->min_degree, false);
        new_root->children[0] = root;
        tree->root = new_root;

        split_child(tree, new_root, 0);

        bool inserted = insert_non_full(tree, new_root, key, value);
        if (inserted) tree->size++;
        return inserted;
    }

    bool inserted = insert_non_full(tree, root, key, value);
    if (inserted) tree->size++;
    return inserted;
}

bool btree_search(const BTree *tree, int key, int *value_out) {
    if (!tree || !tree->root) return false;
    return search_node(tree->root, key, value_out);
}

bool btree_delete(BTree *tree, int key) {
    if (!tree || !tree->root) return false;

    bool deleted = delete_key(tree, tree->root, key);

    if (deleted) {
        tree->size--;

        /* Shrink tree if root is empty */
        if (tree->root->num_keys == 0 && !tree->root->is_leaf) {
            BTreeNode *old_root = tree->root;
            tree->root = tree->root->children[0];
            free(old_root->keys);
            free(old_root->values);
            free(old_root->children);
            free(old_root);
        }
    }

    return deleted;
}

bool btree_contains(const BTree *tree, int key) {
    return btree_search(tree, key, NULL);
}

size_t btree_size(const BTree *tree) {
    return tree ? tree->size : 0;
}

bool btree_is_empty(const BTree *tree) {
    return !tree || tree->size == 0;
}

int btree_height(const BTree *tree) {
    if (!tree || !tree->root || tree->root->num_keys == 0) {
        return 0;
    }

    int height = 1;
    BTreeNode *cur = tree->root;

    while (!cur->is_leaf) {
        height++;
        cur = cur->children[0];
    }

    return height;
}

bool btree_min(const BTree *tree, int *key_out) {
    if (!tree || !tree->root || tree->root->num_keys == 0) {
        return false;
    }

    BTreeNode *cur = tree->root;
    while (!cur->is_leaf) {
        cur = cur->children[0];
    }

    if (key_out) *key_out = cur->keys[0];
    return true;
}

bool btree_max(const BTree *tree, int *key_out) {
    if (!tree || !tree->root || tree->root->num_keys == 0) {
        return false;
    }

    BTreeNode *cur = tree->root;
    while (!cur->is_leaf) {
        cur = cur->children[cur->num_keys];
    }

    if (key_out) *key_out = cur->keys[cur->num_keys - 1];
    return true;
}

size_t btree_inorder(const BTree *tree, int *keys, size_t max_keys) {
    if (!tree || !keys || max_keys == 0) return 0;
    return inorder_helper(tree->root, keys, max_keys, 0);
}

size_t btree_range(const BTree *tree, int min_key, int max_key,
                   int *keys, size_t max_keys) {
    if (!tree || !keys || max_keys == 0 || min_key > max_key) return 0;
    return range_helper(tree->root, min_key, max_key, keys, max_keys, 0);
}

bool btree_validate(const BTree *tree) {
    if (!tree) return false;
    if (!tree->root) return false;

    int leaf_depth = -1;
    return validate_node(tree->root, tree->min_degree, -2147483648, 2147483647, 0, &leaf_depth);
}

void btree_print(const BTree *tree) {
    if (!tree) {
        printf("(null tree)\n");
        return;
    }

    if (!tree->root || tree->root->num_keys == 0) {
        printf("(empty)\n");
        return;
    }

    print_node(tree->root, 0);
}
