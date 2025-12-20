/**
 * Red-Black Tree Implementation
 */

#include "red_black_tree.h"
#include <stdlib.h>
#include <stdio.h>

/* ============== Helper Functions ============== */

static RBNode *create_node(RBTree *tree, int key, int value) {
    RBNode *node = malloc(sizeof(RBNode));
    if (!node) return NULL;

    node->key = key;
    node->value = value;
    node->color = RB_RED;  /* New nodes are always red */
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;

    return node;
}

/* Left rotation around node x */
static void rotate_left(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;

    /* Turn y's left subtree into x's right subtree */
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }

    /* Link x's parent to y */
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    /* Put x on y's left */
    y->left = x;
    x->parent = y;
}

/* Right rotation around node x */
static void rotate_right(RBTree *tree, RBNode *x) {
    RBNode *y = x->left;

    /* Turn y's right subtree into x's left subtree */
    x->left = y->right;
    if (y->right != tree->nil) {
        y->right->parent = x;
    }

    /* Link x's parent to y */
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    /* Put x on y's right */
    y->right = x;
    x->parent = y;
}

/* Fix Red-Black properties after insertion */
static void insert_fixup(RBTree *tree, RBNode *z) {
    while (z->parent->color == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;  /* Uncle */

            if (y->color == RB_RED) {
                /* Case 1: Uncle is red */
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    /* Case 2: Uncle is black, z is right child */
                    z = z->parent;
                    rotate_left(tree, z);
                }
                /* Case 3: Uncle is black, z is left child */
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            /* Mirror cases */
            RBNode *y = z->parent->parent->left;  /* Uncle */

            if (y->color == RB_RED) {
                /* Case 1: Uncle is red */
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    /* Case 2: Uncle is black, z is left child */
                    z = z->parent;
                    rotate_right(tree, z);
                }
                /* Case 3: Uncle is black, z is right child */
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = RB_BLACK;
}

/* Transplant subtree u with subtree v */
static void transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/* Get minimum node in subtree */
static RBNode *tree_minimum(RBTree *tree, RBNode *x) {
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}

/* Get maximum node in subtree */
static RBNode *tree_maximum(RBTree *tree, RBNode *x) {
    while (x->right != tree->nil) {
        x = x->right;
    }
    return x;
}

/* Fix Red-Black properties after deletion */
static void delete_fixup(RBTree *tree, RBNode *x) {
    while (x != tree->root && x->color == RB_BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;  /* Sibling */

            if (w->color == RB_RED) {
                /* Case 1: Sibling is red */
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
                /* Case 2: Sibling's children are both black */
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->right->color == RB_BLACK) {
                    /* Case 3: Sibling's right child is black */
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_right(tree, w);
                    w = x->parent->right;
                }
                /* Case 4: Sibling's right child is red */
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            /* Mirror cases */
            RBNode *w = x->parent->left;  /* Sibling */

            if (w->color == RB_RED) {
                /* Case 1: Sibling is red */
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RB_BLACK && w->left->color == RB_BLACK) {
                /* Case 2: Sibling's children are both black */
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->left->color == RB_BLACK) {
                    /* Case 3: Sibling's left child is black */
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                /* Case 4: Sibling's left child is red */
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RB_BLACK;
}

/* Recursively free nodes */
static void free_subtree(RBTree *tree, RBNode *node) {
    if (node != tree->nil) {
        free_subtree(tree, node->left);
        free_subtree(tree, node->right);
        free(node);
    }
}

/* In-order traversal helper */
static size_t inorder_helper(const RBTree *tree, RBNode *node,
                             int *keys, size_t max_keys, size_t count) {
    if (node == tree->nil || count >= max_keys) {
        return count;
    }

    count = inorder_helper(tree, node->left, keys, max_keys, count);
    if (count < max_keys) {
        keys[count++] = node->key;
    }
    count = inorder_helper(tree, node->right, keys, max_keys, count);

    return count;
}

/* Range query helper */
static size_t range_helper(const RBTree *tree, RBNode *node,
                           int min_key, int max_key,
                           int *keys, size_t max_keys, size_t count) {
    if (node == tree->nil || count >= max_keys) {
        return count;
    }

    if (node->key > min_key) {
        count = range_helper(tree, node->left, min_key, max_key,
                            keys, max_keys, count);
    }

    if (count < max_keys && node->key >= min_key && node->key <= max_key) {
        keys[count++] = node->key;
    }

    if (node->key < max_key) {
        count = range_helper(tree, node->right, min_key, max_key,
                            keys, max_keys, count);
    }

    return count;
}

/* Calculate height helper */
static int height_helper(const RBTree *tree, RBNode *node) {
    if (node == tree->nil) {
        return 0;
    }

    int left_height = height_helper(tree, node->left);
    int right_height = height_helper(tree, node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

/* Validate helper - returns black height or -1 if invalid */
static int validate_helper(const RBTree *tree, RBNode *node) {
    if (node == tree->nil) {
        return 1;  /* NIL nodes are black */
    }

    /* Property 4: Red node can't have red children */
    if (node->color == RB_RED) {
        if (node->left->color == RB_RED || node->right->color == RB_RED) {
            return -1;
        }
    }

    /* BST property */
    if (node->left != tree->nil && node->left->key >= node->key) {
        return -1;
    }
    if (node->right != tree->nil && node->right->key <= node->key) {
        return -1;
    }

    int left_bh = validate_helper(tree, node->left);
    int right_bh = validate_helper(tree, node->right);

    /* Property 5: All paths have same black height */
    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh) {
        return -1;
    }

    return left_bh + (node->color == RB_BLACK ? 1 : 0);
}

/* Print helper */
static void print_helper(const RBTree *tree, RBNode *node, int depth) {
    if (node == tree->nil) {
        return;
    }

    print_helper(tree, node->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%d(%c)\n", node->key, node->color == RB_RED ? 'R' : 'B');

    print_helper(tree, node->left, depth + 1);
}

/* ============== Public Functions ============== */

RBTree *rbtree_create(void) {
    RBTree *tree = malloc(sizeof(RBTree));
    if (!tree) return NULL;

    /* Create sentinel NIL node */
    tree->nil = malloc(sizeof(RBNode));
    if (!tree->nil) {
        free(tree);
        return NULL;
    }

    tree->nil->color = RB_BLACK;
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;

    tree->root = tree->nil;
    tree->size = 0;

    return tree;
}

void rbtree_destroy(RBTree *tree) {
    if (!tree) return;

    free_subtree(tree, tree->root);
    free(tree->nil);
    free(tree);
}

void rbtree_clear(RBTree *tree) {
    if (!tree) return;

    free_subtree(tree, tree->root);
    tree->root = tree->nil;
    tree->size = 0;
}

bool rbtree_insert(RBTree *tree, int key, int value) {
    if (!tree) return false;

    /* Find insertion point */
    RBNode *y = tree->nil;
    RBNode *x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            /* Key exists, update value */
            x->value = value;
            return false;
        }
    }

    /* Create new node */
    RBNode *z = create_node(tree, key, value);
    if (!z) return false;

    z->parent = y;

    if (y == tree->nil) {
        tree->root = z;
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    tree->size++;

    /* Fix Red-Black properties */
    insert_fixup(tree, z);

    return true;
}

bool rbtree_search(const RBTree *tree, int key, int *value_out) {
    if (!tree) return false;

    RBNode *x = tree->root;

    while (x != tree->nil) {
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            if (value_out) {
                *value_out = x->value;
            }
            return true;
        }
    }

    return false;
}

bool rbtree_delete(RBTree *tree, int key) {
    if (!tree) return false;

    /* Find node to delete */
    RBNode *z = tree->root;
    while (z != tree->nil) {
        if (key < z->key) {
            z = z->left;
        } else if (key > z->key) {
            z = z->right;
        } else {
            break;
        }
    }

    if (z == tree->nil) {
        return false;  /* Key not found */
    }

    RBNode *y = z;
    RBNode *x;
    RBColor y_original_color = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = tree_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);
    tree->size--;

    if (y_original_color == RB_BLACK) {
        delete_fixup(tree, x);
    }

    return true;
}

bool rbtree_contains(const RBTree *tree, int key) {
    return rbtree_search(tree, key, NULL);
}

size_t rbtree_size(const RBTree *tree) {
    return tree ? tree->size : 0;
}

bool rbtree_is_empty(const RBTree *tree) {
    return !tree || tree->root == tree->nil;
}

bool rbtree_min(const RBTree *tree, int *key_out) {
    if (!tree || tree->root == tree->nil) {
        return false;
    }

    RBNode *min = tree_minimum((RBTree *)tree, tree->root);
    if (key_out) {
        *key_out = min->key;
    }
    return true;
}

bool rbtree_max(const RBTree *tree, int *key_out) {
    if (!tree || tree->root == tree->nil) {
        return false;
    }

    RBNode *max = tree_maximum((RBTree *)tree, tree->root);
    if (key_out) {
        *key_out = max->key;
    }
    return true;
}

bool rbtree_floor(const RBTree *tree, int key, int *result_out) {
    if (!tree || tree->root == tree->nil) {
        return false;
    }

    RBNode *x = tree->root;
    RBNode *floor = NULL;

    while (x != tree->nil) {
        if (key == x->key) {
            if (result_out) *result_out = x->key;
            return true;
        } else if (key < x->key) {
            x = x->left;
        } else {
            floor = x;
            x = x->right;
        }
    }

    if (floor) {
        if (result_out) *result_out = floor->key;
        return true;
    }
    return false;
}

bool rbtree_ceiling(const RBTree *tree, int key, int *result_out) {
    if (!tree || tree->root == tree->nil) {
        return false;
    }

    RBNode *x = tree->root;
    RBNode *ceiling = NULL;

    while (x != tree->nil) {
        if (key == x->key) {
            if (result_out) *result_out = x->key;
            return true;
        } else if (key > x->key) {
            x = x->right;
        } else {
            ceiling = x;
            x = x->left;
        }
    }

    if (ceiling) {
        if (result_out) *result_out = ceiling->key;
        return true;
    }
    return false;
}

size_t rbtree_inorder(const RBTree *tree, int *keys, size_t max_keys) {
    if (!tree || !keys || max_keys == 0) {
        return 0;
    }
    return inorder_helper(tree, tree->root, keys, max_keys, 0);
}

size_t rbtree_range(const RBTree *tree, int min_key, int max_key,
                    int *keys, size_t max_keys) {
    if (!tree || !keys || max_keys == 0 || min_key > max_key) {
        return 0;
    }
    return range_helper(tree, tree->root, min_key, max_key, keys, max_keys, 0);
}

bool rbtree_validate(const RBTree *tree) {
    if (!tree) return false;

    /* Property 2: Root must be black */
    if (tree->root != tree->nil && tree->root->color != RB_BLACK) {
        return false;
    }

    return validate_helper(tree, tree->root) != -1;
}

int rbtree_black_height(const RBTree *tree) {
    if (!tree || tree->root == tree->nil) {
        return 0;
    }

    int bh = 0;
    RBNode *x = tree->root;

    while (x != tree->nil) {
        if (x->color == RB_BLACK) {
            bh++;
        }
        x = x->left;
    }

    return bh + 1;  /* +1 for NIL node */
}

int rbtree_height(const RBTree *tree) {
    if (!tree) return 0;
    return height_helper(tree, tree->root);
}

void rbtree_print(const RBTree *tree) {
    if (!tree) {
        printf("(null tree)\n");
        return;
    }

    if (tree->root == tree->nil) {
        printf("(empty)\n");
        return;
    }

    print_helper(tree, tree->root, 0);
}
