package dsa.datastructures;

import java.util.*;

/**
 * Self-balancing AVL Tree implementation.
 *
 * @param <K> the type of keys, must be Comparable
 * @param <V> the type of values
 */
public class AvlTree<K extends Comparable<K>, V> {

    public class AVLNode {
        K key;
        V value;
        int height;
        AVLNode left;
        AVLNode right;

        public AVLNode(K key, V value) {
            this.key = key;
            this.value = value;
            this.height = 0;
        }

        public K getKey() {
            return key;
        }

        public V getValue() {
            return value;
        }

        public int getHeight() {
            return height;
        }

        public AVLNode getLeft() {
            return left;
        }

        public AVLNode getRight() {
            return right;
        }
    }

    private AVLNode root;
    private int size;

    public AvlTree() {
        this.root = null;
        this.size = 0;
    }

    public AVLNode getRoot() {
        return root;
    }

    private int nodeHeight(AVLNode node) {
        return node == null ? -1 : node.height;
    }

    private void updateHeight(AVLNode node) {
        node.height = 1 + Math.max(nodeHeight(node.left), nodeHeight(node.right));
    }

    private int balanceFactor(AVLNode node) {
        return node == null ? 0 : nodeHeight(node.left) - nodeHeight(node.right);
    }

    /**
     * Right rotation around the given node.
     */
    private AVLNode rotateRight(AVLNode y) {
        AVLNode x = y.left;
        AVLNode t2 = x.right;
        x.right = y;
        y.left = t2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    /**
     * Left rotation around the given node.
     */
    private AVLNode rotateLeft(AVLNode x) {
        AVLNode y = x.right;
        AVLNode t2 = y.left;
        y.left = x;
        x.right = t2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    /**
     * Rebalance the subtree rooted at the given node.
     */
    private AVLNode rebalance(AVLNode node) {
        updateHeight(node);
        int balance = balanceFactor(node);

        // Left-heavy
        if (balance > 1) {
            if (balanceFactor(node.left) < 0) {
                // Left-Right case
                node.left = rotateLeft(node.left);
            }
            // Left-Left case
            return rotateRight(node);
        }

        // Right-heavy
        if (balance < -1) {
            if (balanceFactor(node.right) > 0) {
                // Right-Left case
                node.right = rotateRight(node.right);
            }
            // Right-Right case
            return rotateLeft(node);
        }

        return node;
    }

    /**
     * Insert a key-value pair.
     */
    public void insert(K key, V value) {
        root = insertHelper(root, key, value);
    }

    private AVLNode insertHelper(AVLNode node, K key, V value) {
        if (node == null) {
            size++;
            return new AVLNode(key, value);
        }
        int cmp = key.compareTo(node.key);
        if (cmp < 0) {
            node.left = insertHelper(node.left, key, value);
        } else if (cmp > 0) {
            node.right = insertHelper(node.right, key, value);
        } else {
            // Update value for existing key
            node.value = value;
            return node;
        }
        return rebalance(node);
    }

    /**
     * Delete a key from the tree.
     */
    public boolean delete(K key) {
        if (!search(key)) return false;
        root = deleteHelper(root, key);
        size--;
        return true;
    }

    private AVLNode deleteHelper(AVLNode node, K key) {
        if (node == null) return null;

        int cmp = key.compareTo(node.key);
        if (cmp < 0) {
            node.left = deleteHelper(node.left, key);
        } else if (cmp > 0) {
            node.right = deleteHelper(node.right, key);
        } else {
            // Found the node to delete
            if (node.left == null) return node.right;
            if (node.right == null) return node.left;

            // Two children: replace with in-order successor
            AVLNode successor = findMin(node.right);
            node.key = successor.key;
            node.value = successor.value;
            node.right = deleteHelper(node.right, successor.key);
        }
        return rebalance(node);
    }

    private AVLNode findMin(AVLNode node) {
        while (node.left != null) {
            node = node.left;
        }
        return node;
    }

    private AVLNode findMax(AVLNode node) {
        while (node.right != null) {
            node = node.right;
        }
        return node;
    }

    /**
     * Search for a key.
     */
    public boolean search(K key) {
        return searchNode(root, key) != null;
    }

    private AVLNode searchNode(AVLNode node, K key) {
        if (node == null) return null;
        int cmp = key.compareTo(node.key);
        if (cmp == 0) return node;
        if (cmp < 0) return searchNode(node.left, key);
        return searchNode(node.right, key);
    }

    /**
     * Get the value associated with a key.
     */
    public Optional<V> get(K key) {
        AVLNode node = searchNode(root, key);
        return node == null ? Optional.empty() : Optional.of(node.value);
    }

    /**
     * Return the minimum key.
     */
    public Optional<K> minimum() {
        if (root == null) return Optional.empty();
        return Optional.of(findMin(root).key);
    }

    /**
     * Return the maximum key.
     */
    public Optional<K> maximum() {
        if (root == null) return Optional.empty();
        return Optional.of(findMax(root).key);
    }

    /**
     * In-order traversal returning list of keys.
     */
    public List<K> inorder() {
        List<K> result = new ArrayList<>();
        inorderHelper(root, result);
        return result;
    }

    private void inorderHelper(AVLNode node, List<K> result) {
        if (node == null) return;
        inorderHelper(node.left, result);
        result.add(node.key);
        inorderHelper(node.right, result);
    }

    /**
     * Pre-order traversal returning list of keys.
     */
    public List<K> preorder() {
        List<K> result = new ArrayList<>();
        preorderHelper(root, result);
        return result;
    }

    private void preorderHelper(AVLNode node, List<K> result) {
        if (node == null) return;
        result.add(node.key);
        preorderHelper(node.left, result);
        preorderHelper(node.right, result);
    }

    /**
     * Level-order traversal returning list of keys.
     */
    public List<K> levelOrder() {
        List<K> result = new ArrayList<>();
        if (root == null) return result;
        java.util.Queue<AVLNode> queue = new java.util.LinkedList<>();
        queue.add(root);
        while (!queue.isEmpty()) {
            AVLNode current = queue.poll();
            result.add(current.key);
            if (current.left != null) queue.add(current.left);
            if (current.right != null) queue.add(current.right);
        }
        return result;
    }

    /**
     * Check if the tree is balanced (all nodes have balance factor in {-1, 0, 1}).
     */
    public boolean isBalanced() {
        return isBalancedHelper(root);
    }

    private boolean isBalancedHelper(AVLNode node) {
        if (node == null) return true;
        int balance = balanceFactor(node);
        if (balance < -1 || balance > 1) return false;
        return isBalancedHelper(node.left) && isBalancedHelper(node.right);
    }

    /**
     * Return the height of the tree. Empty tree has height -1.
     */
    public int height() {
        return nodeHeight(root);
    }

    /**
     * Clear the tree.
     */
    public void clear() {
        root = null;
        size = 0;
    }

    /**
     * Return the number of entries.
     */
    public int size() {
        return size;
    }
}
