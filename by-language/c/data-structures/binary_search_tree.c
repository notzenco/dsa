/**
 * Binary Search Tree Implementation
 * See binary_search_tree.h for documentation
 */

#include "binary_search_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Internal: Create a new node */
static BSTNode *bst_create_node(int key, int value) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

/* Internal: Find minimum node in subtree */
static BSTNode *bst_min_node(BSTNode *node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

/* Internal: Find maximum node in subtree */
static BSTNode *bst_max_node(BSTNode *node) {
    while (node != NULL && node->right != NULL) {
        node = node->right;
    }
    return node;
}

/* Internal: Find node by key */
static BSTNode *bst_find_node(const BST *tree, int key) {
    BSTNode *current = tree->root;
    while (current != NULL) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            return current;
        }
    }
    return NULL;
}

/* Internal: Transplant subtree */
static void bst_transplant(BST *tree, BSTNode *u, BSTNode *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

/* Internal: Recursively free nodes */
static void bst_destroy_recursive(BSTNode *node) {
    if (node == NULL) {
        return;
    }
    bst_destroy_recursive(node->left);
    bst_destroy_recursive(node->right);
    free(node);
}

/* Internal: Calculate height recursively */
static int bst_height_recursive(const BSTNode *node) {
    if (node == NULL) {
        return 0;
    }
    int left_height = bst_height_recursive(node->left);
    int right_height = bst_height_recursive(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

/* Internal: Validate BST property */
static bool bst_is_valid_recursive(const BSTNode *node, long min, long max) {
    if (node == NULL) {
        return true;
    }
    if (node->key <= min || node->key >= max) {
        return false;
    }
    return bst_is_valid_recursive(node->left, min, node->key) &&
           bst_is_valid_recursive(node->right, node->key, max);
}

/* Internal: Inorder traversal */
static void bst_inorder_recursive(const BSTNode *node, bst_visit_fn visit, void *user_data) {
    if (node == NULL) {
        return;
    }
    bst_inorder_recursive(node->left, visit, user_data);
    visit(node->key, node->value, user_data);
    bst_inorder_recursive(node->right, visit, user_data);
}

/* Internal: Preorder traversal */
static void bst_preorder_recursive(const BSTNode *node, bst_visit_fn visit, void *user_data) {
    if (node == NULL) {
        return;
    }
    visit(node->key, node->value, user_data);
    bst_preorder_recursive(node->left, visit, user_data);
    bst_preorder_recursive(node->right, visit, user_data);
}

/* Internal: Postorder traversal */
static void bst_postorder_recursive(const BSTNode *node, bst_visit_fn visit, void *user_data) {
    if (node == NULL) {
        return;
    }
    bst_postorder_recursive(node->left, visit, user_data);
    bst_postorder_recursive(node->right, visit, user_data);
    visit(node->key, node->value, user_data);
}

/* Internal: Print tree structure */
static void bst_print_recursive(const BSTNode *node, int depth, char prefix) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%c-[%d:%d]\n", prefix, node->key, node->value);
    bst_print_recursive(node->left, depth + 1, 'L');
    bst_print_recursive(node->right, depth + 1, 'R');
}

/* ============== Public Functions ============== */

BST *bst_create(void) {
    BST *tree = malloc(sizeof(BST));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void bst_destroy(BST *tree) {
    if (tree == NULL) {
        return;
    }
    bst_destroy_recursive(tree->root);
    free(tree);
}

bool bst_insert(BST *tree, int key, int value) {
    if (tree == NULL) {
        return false;
    }

    BSTNode *node = bst_create_node(key, value);
    if (node == NULL) {
        return false;
    }

    if (tree->root == NULL) {
        tree->root = node;
        tree->size++;
        return true;
    }

    BSTNode *current = tree->root;
    BSTNode *parent = NULL;

    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            /* Key already exists - update value */
            current->value = value;
            free(node);
            return true;
        }
    }

    node->parent = parent;
    if (key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    tree->size++;
    return true;
}

bool bst_search(const BST *tree, int key, int *out_value) {
    if (tree == NULL) {
        return false;
    }

    BSTNode *node = bst_find_node(tree, key);
    if (node == NULL) {
        return false;
    }

    if (out_value != NULL) {
        *out_value = node->value;
    }
    return true;
}

bool bst_contains(const BST *tree, int key) {
    if (tree == NULL) {
        return false;
    }
    return bst_find_node(tree, key) != NULL;
}

bool bst_delete(BST *tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }

    BSTNode *node = bst_find_node(tree, key);
    if (node == NULL) {
        return false;
    }

    if (node->left == NULL) {
        bst_transplant(tree, node, node->right);
    } else if (node->right == NULL) {
        bst_transplant(tree, node, node->left);
    } else {
        /* Node has two children - find inorder successor */
        BSTNode *successor = bst_min_node(node->right);
        if (successor->parent != node) {
            bst_transplant(tree, successor, successor->right);
            successor->right = node->right;
            successor->right->parent = successor;
        }
        bst_transplant(tree, node, successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }

    free(node);
    tree->size--;
    return true;
}

bool bst_min(const BST *tree, int *out_key, int *out_value) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }

    BSTNode *node = bst_min_node(tree->root);
    if (out_key != NULL) {
        *out_key = node->key;
    }
    if (out_value != NULL) {
        *out_value = node->value;
    }
    return true;
}

bool bst_max(const BST *tree, int *out_key, int *out_value) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }

    BSTNode *node = bst_max_node(tree->root);
    if (out_key != NULL) {
        *out_key = node->key;
    }
    if (out_value != NULL) {
        *out_value = node->value;
    }
    return true;
}

size_t bst_size(const BST *tree) {
    return tree != NULL ? tree->size : 0;
}

bool bst_is_empty(const BST *tree) {
    return tree == NULL || tree->size == 0;
}

int bst_height(const BST *tree) {
    if (tree == NULL) {
        return 0;
    }
    return bst_height_recursive(tree->root);
}

void bst_clear(BST *tree) {
    if (tree == NULL) {
        return;
    }
    bst_destroy_recursive(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

bool bst_is_valid(const BST *tree) {
    if (tree == NULL) {
        return true;
    }
    return bst_is_valid_recursive(tree->root, LONG_MIN, LONG_MAX);
}

void bst_inorder(const BST *tree, bst_visit_fn visit, void *user_data) {
    if (tree == NULL || visit == NULL) {
        return;
    }
    bst_inorder_recursive(tree->root, visit, user_data);
}

void bst_preorder(const BST *tree, bst_visit_fn visit, void *user_data) {
    if (tree == NULL || visit == NULL) {
        return;
    }
    bst_preorder_recursive(tree->root, visit, user_data);
}

void bst_postorder(const BST *tree, bst_visit_fn visit, void *user_data) {
    if (tree == NULL || visit == NULL) {
        return;
    }
    bst_postorder_recursive(tree->root, visit, user_data);
}

void bst_levelorder(const BST *tree, bst_visit_fn visit, void *user_data) {
    if (tree == NULL || tree->root == NULL || visit == NULL) {
        return;
    }

    /* Simple queue implementation for BFS */
    BSTNode **queue = malloc(tree->size * sizeof(BSTNode *));
    if (queue == NULL) {
        return;
    }

    size_t front = 0, rear = 0;
    queue[rear++] = tree->root;

    while (front < rear) {
        BSTNode *node = queue[front++];
        visit(node->key, node->value, user_data);

        if (node->left != NULL) {
            queue[rear++] = node->left;
        }
        if (node->right != NULL) {
            queue[rear++] = node->right;
        }
    }

    free(queue);
}

bool bst_successor(const BST *tree, int key, int *out_key) {
    if (tree == NULL || out_key == NULL) {
        return false;
    }

    BSTNode *node = bst_find_node(tree, key);
    if (node == NULL) {
        return false;
    }

    /* If right subtree exists, successor is minimum in right subtree */
    if (node->right != NULL) {
        BSTNode *successor = bst_min_node(node->right);
        *out_key = successor->key;
        return true;
    }

    /* Otherwise, go up until we find a node that is a left child */
    BSTNode *parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }

    if (parent == NULL) {
        return false;
    }

    *out_key = parent->key;
    return true;
}

bool bst_predecessor(const BST *tree, int key, int *out_key) {
    if (tree == NULL || out_key == NULL) {
        return false;
    }

    BSTNode *node = bst_find_node(tree, key);
    if (node == NULL) {
        return false;
    }

    /* If left subtree exists, predecessor is maximum in left subtree */
    if (node->left != NULL) {
        BSTNode *predecessor = bst_max_node(node->left);
        *out_key = predecessor->key;
        return true;
    }

    /* Otherwise, go up until we find a node that is a right child */
    BSTNode *parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }

    if (parent == NULL) {
        return false;
    }

    *out_key = parent->key;
    return true;
}

/* Helper struct for kth smallest */
typedef struct {
    size_t k;
    size_t count;
    int result;
    bool found;
} KthContext;

static void kth_visitor(int key, int value, void *user_data) {
    (void)value; /* Unused */
    KthContext *ctx = (KthContext *)user_data;
    if (ctx->found) {
        return;
    }
    ctx->count++;
    if (ctx->count == ctx->k) {
        ctx->result = key;
        ctx->found = true;
    }
}

bool bst_kth_smallest(const BST *tree, size_t k, int *out_key) {
    if (tree == NULL || k == 0 || k > tree->size || out_key == NULL) {
        return false;
    }

    KthContext ctx = {k, 0, 0, false};
    bst_inorder(tree, kth_visitor, &ctx);

    if (ctx.found) {
        *out_key = ctx.result;
        return true;
    }
    return false;
}

bool bst_lca(const BST *tree, int key1, int key2, int *out_key) {
    if (tree == NULL || tree->root == NULL || out_key == NULL) {
        return false;
    }

    /* Verify both keys exist */
    if (!bst_contains(tree, key1) || !bst_contains(tree, key2)) {
        return false;
    }

    BSTNode *node = tree->root;
    while (node != NULL) {
        if (key1 < node->key && key2 < node->key) {
            node = node->left;
        } else if (key1 > node->key && key2 > node->key) {
            node = node->right;
        } else {
            *out_key = node->key;
            return true;
        }
    }

    return false;
}

/* Helper struct for to_array */
typedef struct {
    int *array;
    size_t index;
} ArrayContext;

static void array_visitor(int key, int value, void *user_data) {
    (void)value;
    ArrayContext *ctx = (ArrayContext *)user_data;
    ctx->array[ctx->index++] = key;
}

bool bst_to_array(const BST *tree, int **out_array, size_t *out_size) {
    if (tree == NULL || out_array == NULL || out_size == NULL) {
        return false;
    }

    if (tree->size == 0) {
        *out_array = NULL;
        *out_size = 0;
        return true;
    }

    *out_array = malloc(tree->size * sizeof(int));
    if (*out_array == NULL) {
        return false;
    }

    ArrayContext ctx = {*out_array, 0};
    bst_inorder(tree, array_visitor, &ctx);
    *out_size = tree->size;
    return true;
}

void bst_print(const BST *tree) {
    if (tree == NULL) {
        printf("NULL tree\n");
        return;
    }
    if (tree->root == NULL) {
        printf("Empty tree\n");
        return;
    }
    printf("BST (size=%lu):\n", (unsigned long)tree->size);
    bst_print_recursive(tree->root, 0, 'R');
}
