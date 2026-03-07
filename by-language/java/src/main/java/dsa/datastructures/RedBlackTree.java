package dsa.datastructures;

import java.util.*;

/**
 * Red-Black Tree implementation.
 *
 * @param <K> the type of keys, must be Comparable
 * @param <V> the type of values
 */
public class RedBlackTree<K extends Comparable<K>, V> {

    public enum Color {
        RED, BLACK
    }

    public class RBNode {
        K key;
        V value;
        Color color;
        RBNode parent;
        RBNode left;
        RBNode right;

        public RBNode(K key, V value, Color color) {
            this.key = key;
            this.value = value;
            this.color = color;
        }

        public K getKey() {
            return key;
        }

        public V getValue() {
            return value;
        }

        public Color getColor() {
            return color;
        }

        public RBNode getParent() {
            return parent;
        }

        public RBNode getLeft() {
            return left;
        }

        public RBNode getRight() {
            return right;
        }
    }

    /**
     * Simple key-value entry for traversal results.
     */
    public static class Entry<K, V> {
        private final K key;
        private final V value;

        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }

        public K getKey() {
            return key;
        }

        public V getValue() {
            return value;
        }

        @Override
        public String toString() {
            return key + "=" + value;
        }
    }

    private final RBNode NIL;
    private RBNode root;
    private int size;

    public RedBlackTree() {
        NIL = new RBNode(null, null, Color.BLACK);
        root = NIL;
        size = 0;
    }

    public RBNode getRoot() {
        return root == NIL ? null : root;
    }

    private boolean isNil(RBNode node) {
        return node == NIL || node == null;
    }

    // ======================== Rotations ========================

    private void rotateLeft(RBNode x) {
        RBNode y = x.right;
        x.right = y.left;
        if (y.left != NIL) {
            y.left.parent = x;
        }
        y.parent = x.parent;
        if (x.parent == null) {
            root = y;
        } else if (x == x.parent.left) {
            x.parent.left = y;
        } else {
            x.parent.right = y;
        }
        y.left = x;
        x.parent = y;
    }

    private void rotateRight(RBNode x) {
        RBNode y = x.left;
        x.left = y.right;
        if (y.right != NIL) {
            y.right.parent = x;
        }
        y.parent = x.parent;
        if (x.parent == null) {
            root = y;
        } else if (x == x.parent.right) {
            x.parent.right = y;
        } else {
            x.parent.left = y;
        }
        y.right = x;
        x.parent = y;
    }

    // ======================== Insert ========================

    /**
     * Insert a key-value pair into the tree.
     */
    public void insert(K key, V value) {
        RBNode newNode = new RBNode(key, value, Color.RED);
        newNode.left = NIL;
        newNode.right = NIL;

        RBNode parent = null;
        RBNode current = root;

        while (current != NIL) {
            parent = current;
            int cmp = key.compareTo(current.key);
            if (cmp < 0) {
                current = current.left;
            } else if (cmp > 0) {
                current = current.right;
            } else {
                // Update existing key
                current.value = value;
                return;
            }
        }

        newNode.parent = parent;
        if (parent == null) {
            root = newNode;
        } else if (key.compareTo(parent.key) < 0) {
            parent.left = newNode;
        } else {
            parent.right = newNode;
        }

        size++;
        insertFixup(newNode);
    }

    private void insertFixup(RBNode z) {
        while (z.parent != null && z.parent.color == Color.RED) {
            if (z.parent == z.parent.parent.left) {
                RBNode uncle = z.parent.parent.right;
                if (uncle.color == Color.RED) {
                    // Case 1: uncle is red
                    z.parent.color = Color.BLACK;
                    uncle.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;
                    z = z.parent.parent;
                } else {
                    if (z == z.parent.right) {
                        // Case 2: z is right child
                        z = z.parent;
                        rotateLeft(z);
                    }
                    // Case 3: z is left child
                    z.parent.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;
                    rotateRight(z.parent.parent);
                }
            } else {
                RBNode uncle = z.parent.parent.left;
                if (uncle.color == Color.RED) {
                    z.parent.color = Color.BLACK;
                    uncle.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;
                    z = z.parent.parent;
                } else {
                    if (z == z.parent.left) {
                        z = z.parent;
                        rotateRight(z);
                    }
                    z.parent.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;
                    rotateLeft(z.parent.parent);
                }
            }
        }
        root.color = Color.BLACK;
    }

    // ======================== Delete ========================

    /**
     * Delete a key from the tree.
     */
    public boolean delete(K key) {
        RBNode z = findNode(root, key);
        if (z == NIL) return false;

        RBNode y = z;
        Color yOriginalColor = y.color;
        RBNode x;

        if (z.left == NIL) {
            x = z.right;
            transplant(z, z.right);
        } else if (z.right == NIL) {
            x = z.left;
            transplant(z, z.left);
        } else {
            y = minimumNode(z.right);
            yOriginalColor = y.color;
            x = y.right;
            if (y.parent == z) {
                x.parent = y;
            } else {
                transplant(y, y.right);
                y.right = z.right;
                y.right.parent = y;
            }
            transplant(z, y);
            y.left = z.left;
            y.left.parent = y;
            y.color = z.color;
        }

        size--;
        if (yOriginalColor == Color.BLACK) {
            deleteFixup(x);
        }
        return true;
    }

    private void transplant(RBNode u, RBNode v) {
        if (u.parent == null) {
            root = v;
        } else if (u == u.parent.left) {
            u.parent.left = v;
        } else {
            u.parent.right = v;
        }
        v.parent = u.parent;
    }

    private void deleteFixup(RBNode x) {
        while (x != root && x.color == Color.BLACK) {
            if (x == x.parent.left) {
                RBNode w = x.parent.right;
                if (w.color == Color.RED) {
                    // Case 1
                    w.color = Color.BLACK;
                    x.parent.color = Color.RED;
                    rotateLeft(x.parent);
                    w = x.parent.right;
                }
                if (w.left.color == Color.BLACK && w.right.color == Color.BLACK) {
                    // Case 2
                    w.color = Color.RED;
                    x = x.parent;
                } else {
                    if (w.right.color == Color.BLACK) {
                        // Case 3
                        w.left.color = Color.BLACK;
                        w.color = Color.RED;
                        rotateRight(w);
                        w = x.parent.right;
                    }
                    // Case 4
                    w.color = x.parent.color;
                    x.parent.color = Color.BLACK;
                    w.right.color = Color.BLACK;
                    rotateLeft(x.parent);
                    x = root;
                }
            } else {
                RBNode w = x.parent.left;
                if (w.color == Color.RED) {
                    w.color = Color.BLACK;
                    x.parent.color = Color.RED;
                    rotateRight(x.parent);
                    w = x.parent.left;
                }
                if (w.right.color == Color.BLACK && w.left.color == Color.BLACK) {
                    w.color = Color.RED;
                    x = x.parent;
                } else {
                    if (w.left.color == Color.BLACK) {
                        w.right.color = Color.BLACK;
                        w.color = Color.RED;
                        rotateLeft(w);
                        w = x.parent.left;
                    }
                    w.color = x.parent.color;
                    x.parent.color = Color.BLACK;
                    w.left.color = Color.BLACK;
                    rotateRight(x.parent);
                    x = root;
                }
            }
        }
        x.color = Color.BLACK;
    }

    // ======================== Search ========================

    /**
     * Search for a key.
     */
    public Optional<V> search(K key) {
        RBNode node = findNode(root, key);
        return node == NIL ? Optional.empty() : Optional.of(node.value);
    }

    private RBNode findNode(RBNode node, K key) {
        while (node != NIL) {
            int cmp = key.compareTo(node.key);
            if (cmp == 0) return node;
            if (cmp < 0) node = node.left;
            else node = node.right;
        }
        return NIL;
    }

    // ======================== Traversals ========================

    /**
     * In-order traversal returning list of entries.
     */
    public List<Entry<K, V>> inorder() {
        List<Entry<K, V>> result = new ArrayList<>();
        inorderHelper(root, result);
        return result;
    }

    private void inorderHelper(RBNode node, List<Entry<K, V>> result) {
        if (node == NIL) return;
        inorderHelper(node.left, result);
        result.add(new Entry<>(node.key, node.value));
        inorderHelper(node.right, result);
    }

    /**
     * Return all keys in sorted order.
     */
    public List<K> keys() {
        List<K> result = new ArrayList<>();
        keysHelper(root, result);
        return result;
    }

    private void keysHelper(RBNode node, List<K> result) {
        if (node == NIL) return;
        keysHelper(node.left, result);
        result.add(node.key);
        keysHelper(node.right, result);
    }

    /**
     * Return all values in key-sorted order.
     */
    public List<V> values() {
        List<V> result = new ArrayList<>();
        valuesHelper(root, result);
        return result;
    }

    private void valuesHelper(RBNode node, List<V> result) {
        if (node == NIL) return;
        valuesHelper(node.left, result);
        result.add(node.value);
        valuesHelper(node.right, result);
    }

    // ======================== Min / Max ========================

    /**
     * Return the minimum key.
     */
    public Optional<K> min() {
        if (root == NIL) return Optional.empty();
        return Optional.of(minimumNode(root).key);
    }

    private RBNode minimumNode(RBNode node) {
        while (node.left != NIL) {
            node = node.left;
        }
        return node;
    }

    /**
     * Return the maximum key.
     */
    public Optional<K> max() {
        if (root == NIL) return Optional.empty();
        RBNode node = root;
        while (node.right != NIL) {
            node = node.right;
        }
        return Optional.of(node.key);
    }

    // ======================== Properties ========================

    /**
     * Return the height of the tree.
     */
    public int height() {
        return heightHelper(root);
    }

    private int heightHelper(RBNode node) {
        if (node == NIL) return -1;
        return 1 + Math.max(heightHelper(node.left), heightHelper(node.right));
    }

    /**
     * Return the black-height (number of black nodes on any path from root to a leaf).
     */
    public int blackHeight() {
        return blackHeightHelper(root);
    }

    private int blackHeightHelper(RBNode node) {
        if (node == NIL) return 0;
        int leftBH = blackHeightHelper(node.left);
        return leftBH + (node.color == Color.BLACK ? 1 : 0);
    }

    /**
     * Validate all Red-Black tree properties:
     * 1. Every node is either red or black.
     * 2. Root is black.
     * 3. NIL leaves are black.
     * 4. Red node has only black children.
     * 5. All paths from a node to its descendant NIL nodes have the same black count.
     */
    public boolean isValid() {
        if (root == NIL) return true;
        if (root.color != Color.BLACK) return false;
        return validateHelper(root) != -1;
    }

    /**
     * Returns the black height if valid, -1 if invalid.
     */
    private int validateHelper(RBNode node) {
        if (node == NIL) return 0;

        // Check red-red violation
        if (node.color == Color.RED) {
            if (node.left.color == Color.RED || node.right.color == Color.RED) {
                return -1;
            }
        }

        int leftBH = validateHelper(node.left);
        int rightBH = validateHelper(node.right);

        if (leftBH == -1 || rightBH == -1) return -1;
        if (leftBH != rightBH) return -1;

        return leftBH + (node.color == Color.BLACK ? 1 : 0);
    }

    /**
     * Clear the tree.
     */
    public void clear() {
        root = NIL;
        size = 0;
    }

    /**
     * Return the number of entries.
     */
    public int size() {
        return size;
    }
}
