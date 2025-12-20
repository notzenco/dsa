/**
 * AVL Tree Implementation
 * See avl_tree.h for documentation
 */

#include "avl_tree.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ============== Helper Functions ============== */

static int max_int(int a, int b) {
    return (a > b) ? a : b;
}

static int node_height(const AVLNode *node) {
    return node ? node->height : 0;
}

static int balance_factor(const AVLNode *node) {
    return node ? node_height(node->left) - node_height(node->right) : 0;
}

static void update_height(AVLNode *node) {
    if (node) {
        node->height = 1 + max_int(node_height(node->left), node_height(node->right));
    }
}

static AVLNode *create_node(int value) {
    AVLNode *node = malloc(sizeof(AVLNode));
    if (node) {
        node->data = value;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}

static void destroy_node(AVLNode *node) {
    if (node == NULL) return;
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
}

/* ============== Rotations ============== */

static AVLNode *rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

static AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

static AVLNode *rebalance(AVLNode *node) {
    if (node == NULL) return NULL;

    update_height(node);
    int bf = balance_factor(node);

    /* Left heavy */
    if (bf > 1) {
        /* Left-Right case */
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        /* Left-Left case */
        return rotate_right(node);
    }

    /* Right heavy */
    if (bf < -1) {
        /* Right-Left case */
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        /* Right-Right case */
        return rotate_left(node);
    }

    return node;
}

/* ============== Creation/Destruction ============== */

AVLTree *avl_create(void) {
    AVLTree *tree = malloc(sizeof(AVLTree));
    if (tree) {
        tree->root = NULL;
        tree->size = 0;
    }
    return tree;
}

void avl_destroy(AVLTree *tree) {
    if (tree == NULL) return;
    destroy_node(tree->root);
    free(tree);
}

void avl_clear(AVLTree *tree) {
    if (tree == NULL) return;
    destroy_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

/* ============== Insert ============== */

static AVLNode *insert_node(AVLNode *node, int value, bool *inserted) {
    if (node == NULL) {
        *inserted = true;
        return create_node(value);
    }

    if (value < node->data) {
        node->left = insert_node(node->left, value, inserted);
    } else if (value > node->data) {
        node->right = insert_node(node->right, value, inserted);
    } else {
        /* Duplicate value */
        *inserted = false;
        return node;
    }

    return rebalance(node);
}

bool avl_insert(AVLTree *tree, int value) {
    if (tree == NULL) return false;

    bool inserted = false;
    tree->root = insert_node(tree->root, value, &inserted);

    if (inserted) {
        tree->size++;
    }
    return inserted;
}

/* ============== Delete ============== */

static AVLNode *find_min_node(AVLNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static AVLNode *delete_node(AVLNode *node, int value, bool *deleted) {
    if (node == NULL) {
        *deleted = false;
        return NULL;
    }

    if (value < node->data) {
        node->left = delete_node(node->left, value, deleted);
    } else if (value > node->data) {
        node->right = delete_node(node->right, value, deleted);
    } else {
        /* Node found */
        *deleted = true;

        /* Node with one or no child */
        if (node->left == NULL || node->right == NULL) {
            AVLNode *temp = node->left ? node->left : node->right;
            free(node);
            return temp;
        }

        /* Node with two children */
        AVLNode *successor = find_min_node(node->right);
        node->data = successor->data;
        node->right = delete_node(node->right, successor->data, deleted);
        *deleted = true;  /* Restore since recursive call may have set it */
    }

    return rebalance(node);
}

bool avl_delete(AVLTree *tree, int value) {
    if (tree == NULL || tree->root == NULL) return false;

    bool deleted = false;
    tree->root = delete_node(tree->root, value, &deleted);

    if (deleted) {
        tree->size--;
    }
    return deleted;
}

/* ============== Search ============== */

static const AVLNode *search_node(const AVLNode *node, int value) {
    if (node == NULL) return NULL;

    if (value < node->data) {
        return search_node(node->left, value);
    } else if (value > node->data) {
        return search_node(node->right, value);
    }
    return node;
}

bool avl_search(const AVLTree *tree, int value) {
    if (tree == NULL) return false;
    return search_node(tree->root, value) != NULL;
}

bool avl_contains(const AVLTree *tree, int value) {
    return avl_search(tree, value);
}

/* ============== Tree Properties ============== */

bool avl_minimum(const AVLTree *tree, int *result) {
    if (tree == NULL || tree->root == NULL || result == NULL) {
        return false;
    }

    const AVLNode *node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }
    *result = node->data;
    return true;
}

bool avl_maximum(const AVLTree *tree, int *result) {
    if (tree == NULL || tree->root == NULL || result == NULL) {
        return false;
    }

    const AVLNode *node = tree->root;
    while (node->right != NULL) {
        node = node->right;
    }
    *result = node->data;
    return true;
}

int avl_height(const AVLTree *tree) {
    if (tree == NULL || tree->root == NULL) return -1;
    return tree->root->height - 1;  /* Convert 1-indexed to 0-indexed */
}

size_t avl_size(const AVLTree *tree) {
    return tree ? tree->size : 0;
}

bool avl_is_empty(const AVLTree *tree) {
    return tree == NULL || tree->size == 0;
}

/* ============== Traversal ============== */

static void inorder_helper(const AVLNode *node, int *result,
                           size_t *count, size_t max_size) {
    if (node == NULL || *count >= max_size) return;

    inorder_helper(node->left, result, count, max_size);
    if (*count < max_size) {
        result[*count] = node->data;
        (*count)++;
    }
    inorder_helper(node->right, result, count, max_size);
}

size_t avl_inorder(const AVLTree *tree, int *result, size_t max_size) {
    if (tree == NULL || result == NULL || max_size == 0) return 0;

    size_t count = 0;
    inorder_helper(tree->root, result, &count, max_size);
    return count;
}

static void preorder_helper(const AVLNode *node, int *result,
                            size_t *count, size_t max_size) {
    if (node == NULL || *count >= max_size) return;

    result[*count] = node->data;
    (*count)++;
    preorder_helper(node->left, result, count, max_size);
    preorder_helper(node->right, result, count, max_size);
}

size_t avl_preorder(const AVLTree *tree, int *result, size_t max_size) {
    if (tree == NULL || result == NULL || max_size == 0) return 0;

    size_t count = 0;
    preorder_helper(tree->root, result, &count, max_size);
    return count;
}

static void postorder_helper(const AVLNode *node, int *result,
                             size_t *count, size_t max_size) {
    if (node == NULL || *count >= max_size) return;

    postorder_helper(node->left, result, count, max_size);
    postorder_helper(node->right, result, count, max_size);
    if (*count < max_size) {
        result[*count] = node->data;
        (*count)++;
    }
}

size_t avl_postorder(const AVLTree *tree, int *result, size_t max_size) {
    if (tree == NULL || result == NULL || max_size == 0) return 0;

    size_t count = 0;
    postorder_helper(tree->root, result, &count, max_size);
    return count;
}

size_t avl_level_order(const AVLTree *tree, int *result, size_t max_size) {
    if (tree == NULL || tree->root == NULL || result == NULL || max_size == 0) {
        return 0;
    }

    /* Simple array-based queue */
    AVLNode **queue = malloc(tree->size * sizeof(AVLNode *));
    if (queue == NULL) return 0;

    size_t front = 0, rear = 0, count = 0;
    queue[rear++] = tree->root;

    while (front < rear && count < max_size) {
        AVLNode *node = queue[front++];
        result[count++] = node->data;

        if (node->left != NULL && rear < tree->size) {
            queue[rear++] = node->left;
        }
        if (node->right != NULL && rear < tree->size) {
            queue[rear++] = node->right;
        }
    }

    free(queue);
    return count;
}

/* ============== Advanced Operations ============== */

bool avl_successor(const AVLTree *tree, int value, int *result) {
    if (tree == NULL || result == NULL) return false;

    const AVLNode *successor = NULL;
    const AVLNode *node = tree->root;

    while (node != NULL) {
        if (value < node->data) {
            successor = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (successor != NULL) {
        *result = successor->data;
        return true;
    }
    return false;
}

bool avl_predecessor(const AVLTree *tree, int value, int *result) {
    if (tree == NULL || result == NULL) return false;

    const AVLNode *predecessor = NULL;
    const AVLNode *node = tree->root;

    while (node != NULL) {
        if (value > node->data) {
            predecessor = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (predecessor != NULL) {
        *result = predecessor->data;
        return true;
    }
    return false;
}

static void kth_smallest_helper(const AVLNode *node, size_t k,
                                 size_t *count, int *result, bool *found) {
    if (node == NULL || *found) return;

    kth_smallest_helper(node->left, k, count, result, found);

    if (*found) return;

    (*count)++;
    if (*count == k) {
        *result = node->data;
        *found = true;
        return;
    }

    kth_smallest_helper(node->right, k, count, result, found);
}

bool avl_kth_smallest(const AVLTree *tree, size_t k, int *result) {
    if (tree == NULL || result == NULL || k == 0 || k > tree->size) {
        return false;
    }

    size_t count = 0;
    bool found = false;
    kth_smallest_helper(tree->root, k, &count, result, &found);
    return found;
}

bool avl_floor(const AVLTree *tree, int value, int *result) {
    if (tree == NULL || result == NULL) return false;

    const AVLNode *floor = NULL;
    const AVLNode *node = tree->root;

    while (node != NULL) {
        if (node->data == value) {
            *result = value;
            return true;
        }
        if (node->data < value) {
            floor = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (floor != NULL) {
        *result = floor->data;
        return true;
    }
    return false;
}

bool avl_ceiling(const AVLTree *tree, int value, int *result) {
    if (tree == NULL || result == NULL) return false;

    const AVLNode *ceiling = NULL;
    const AVLNode *node = tree->root;

    while (node != NULL) {
        if (node->data == value) {
            *result = value;
            return true;
        }
        if (node->data > value) {
            ceiling = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (ceiling != NULL) {
        *result = ceiling->data;
        return true;
    }
    return false;
}

static void count_range_helper(const AVLNode *node, int low, int high,
                               size_t *count) {
    if (node == NULL) return;

    if (node->data >= low && node->data <= high) {
        (*count)++;
    }

    if (node->data > low) {
        count_range_helper(node->left, low, high, count);
    }
    if (node->data < high) {
        count_range_helper(node->right, low, high, count);
    }
}

size_t avl_count_range(const AVLTree *tree, int low, int high) {
    if (tree == NULL || low > high) return 0;

    size_t count = 0;
    count_range_helper(tree->root, low, high, &count);
    return count;
}

/* ============== Validation ============== */

static bool is_valid_helper(const AVLNode *node, int min, int max,
                            int *height) {
    if (node == NULL) {
        *height = 0;
        return true;
    }

    /* Check BST property */
    if (node->data <= min || node->data >= max) {
        return false;
    }

    int left_height = 0, right_height = 0;

    if (!is_valid_helper(node->left, min, node->data, &left_height) ||
        !is_valid_helper(node->right, node->data, max, &right_height)) {
        return false;
    }

    /* Check AVL property */
    int bf = left_height - right_height;
    if (bf < -1 || bf > 1) {
        return false;
    }

    /* Check height consistency */
    *height = 1 + max_int(left_height, right_height);
    if (node->height != *height) {
        return false;
    }

    return true;
}

bool avl_is_valid(const AVLTree *tree) {
    if (tree == NULL) return true;
    if (tree->root == NULL) return tree->size == 0;

    int height = 0;
    return is_valid_helper(tree->root, INT_MIN, INT_MAX, &height);
}

int avl_balance_factor(const AVLTree *tree) {
    if (tree == NULL || tree->root == NULL) return 0;
    return balance_factor(tree->root);
}
