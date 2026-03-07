package dsa.datastructures;

import java.util.*;

/**
 * B-Tree implementation with minimum degree t.
 * Each node has at most 2t-1 keys and at least t-1 keys (except root).
 *
 * @param <K> the type of keys, must be Comparable
 * @param <V> the type of values
 */
public class BTree<K extends Comparable<K>, V> {

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

    public class BTreeNode {
        List<K> keys;
        List<V> values;
        List<BTreeNode> children;
        boolean leaf;

        public BTreeNode(boolean leaf) {
            this.leaf = leaf;
            this.keys = new ArrayList<>();
            this.values = new ArrayList<>();
            this.children = new ArrayList<>();
        }

        public List<K> getKeys() {
            return Collections.unmodifiableList(keys);
        }

        public List<V> getValues() {
            return Collections.unmodifiableList(values);
        }

        public boolean isLeaf() {
            return leaf;
        }

        public int numKeys() {
            return keys.size();
        }
    }

    private final int t; // minimum degree
    private BTreeNode root;
    private int size;

    /**
     * Create a B-Tree with the given minimum degree.
     *
     * @param t minimum degree (must be >= 2)
     */
    public BTree(int t) {
        if (t < 2) {
            throw new IllegalArgumentException("Minimum degree must be at least 2");
        }
        this.t = t;
        this.root = new BTreeNode(true);
        this.size = 0;
    }

    public BTreeNode getRoot() {
        return root;
    }

    // ======================== Search ========================

    /**
     * Search for a key in the B-Tree.
     */
    public Optional<V> search(K key) {
        return searchHelper(root, key);
    }

    private Optional<V> searchHelper(BTreeNode node, K key) {
        int i = 0;
        while (i < node.keys.size() && key.compareTo(node.keys.get(i)) > 0) {
            i++;
        }
        if (i < node.keys.size() && key.compareTo(node.keys.get(i)) == 0) {
            return Optional.of(node.values.get(i));
        }
        if (node.leaf) {
            return Optional.empty();
        }
        return searchHelper(node.children.get(i), key);
    }

    // ======================== Insert ========================

    /**
     * Insert a key-value pair into the B-Tree.
     */
    public void insert(K key, V value) {
        // Check if key already exists, if so update
        if (updateIfExists(root, key, value)) {
            return;
        }

        BTreeNode r = root;
        if (r.keys.size() == 2 * t - 1) {
            // Root is full, split it
            BTreeNode s = new BTreeNode(false);
            s.children.add(r);
            root = s;
            splitChild(s, 0);
            insertNonFull(s, key, value);
        } else {
            insertNonFull(r, key, value);
        }
        size++;
    }

    private boolean updateIfExists(BTreeNode node, K key, V value) {
        int i = 0;
        while (i < node.keys.size() && key.compareTo(node.keys.get(i)) > 0) {
            i++;
        }
        if (i < node.keys.size() && key.compareTo(node.keys.get(i)) == 0) {
            node.values.set(i, value);
            return true;
        }
        if (node.leaf) return false;
        return updateIfExists(node.children.get(i), key, value);
    }

    private void insertNonFull(BTreeNode node, K key, V value) {
        int i = node.keys.size() - 1;
        if (node.leaf) {
            // Find position and insert
            while (i >= 0 && key.compareTo(node.keys.get(i)) < 0) {
                i--;
            }
            node.keys.add(i + 1, key);
            node.values.add(i + 1, value);
        } else {
            while (i >= 0 && key.compareTo(node.keys.get(i)) < 0) {
                i--;
            }
            i++;
            if (node.children.get(i).keys.size() == 2 * t - 1) {
                splitChild(node, i);
                if (key.compareTo(node.keys.get(i)) > 0) {
                    i++;
                }
            }
            insertNonFull(node.children.get(i), key, value);
        }
    }

    /**
     * Split the i-th child of the given node (which must be full).
     */
    private void splitChild(BTreeNode parent, int i) {
        BTreeNode fullChild = parent.children.get(i);
        BTreeNode newChild = new BTreeNode(fullChild.leaf);

        // Move the upper t-1 keys to new child
        for (int j = 0; j < t - 1; j++) {
            newChild.keys.add(fullChild.keys.get(t));
            newChild.values.add(fullChild.values.get(t));
            fullChild.keys.remove(t);
            fullChild.values.remove(t);
        }

        // Move children if not a leaf
        if (!fullChild.leaf) {
            for (int j = 0; j < t; j++) {
                newChild.children.add(fullChild.children.get(t));
                fullChild.children.remove(t);
            }
        }

        // Move median key up to parent
        K medianKey = fullChild.keys.get(t - 1);
        V medianValue = fullChild.values.get(t - 1);
        fullChild.keys.remove(t - 1);
        fullChild.values.remove(t - 1);

        parent.keys.add(i, medianKey);
        parent.values.add(i, medianValue);
        parent.children.add(i + 1, newChild);
    }

    // ======================== Delete ========================

    /**
     * Delete a key from the B-Tree.
     */
    public boolean delete(K key) {
        if (search(key).isEmpty()) return false;
        deleteHelper(root, key);
        // Shrink tree if root has no keys
        if (root.keys.isEmpty() && !root.leaf) {
            root = root.children.get(0);
        }
        size--;
        return true;
    }

    private void deleteHelper(BTreeNode node, K key) {
        int idx = findKeyIndex(node, key);

        if (idx < node.keys.size() && key.compareTo(node.keys.get(idx)) == 0) {
            // Key found in this node
            if (node.leaf) {
                // Case 1: key is in a leaf node
                node.keys.remove(idx);
                node.values.remove(idx);
            } else {
                // Case 2: key is in an internal node
                deleteFromInternalNode(node, idx);
            }
        } else {
            // Key is not in this node, must be in a child
            if (node.leaf) {
                return; // key not found
            }
            boolean isLastChild = (idx == node.keys.size());

            // If the child where the key should be has fewer than t keys, fill it
            if (node.children.get(idx).keys.size() < t) {
                fill(node, idx);
            }

            // If the last child was merged, go to the previous child
            if (isLastChild && idx > node.keys.size()) {
                deleteHelper(node.children.get(idx - 1), key);
            } else {
                deleteHelper(node.children.get(idx), key);
            }
        }
    }

    private int findKeyIndex(BTreeNode node, K key) {
        int idx = 0;
        while (idx < node.keys.size() && key.compareTo(node.keys.get(idx)) > 0) {
            idx++;
        }
        return idx;
    }

    private void deleteFromInternalNode(BTreeNode node, int idx) {
        K key = node.keys.get(idx);

        if (node.children.get(idx).keys.size() >= t) {
            // Case 2a: predecessor child has at least t keys
            BTreeNode predChild = node.children.get(idx);
            while (!predChild.leaf) {
                predChild = predChild.children.get(predChild.keys.size());
            }
            K predKey = predChild.keys.get(predChild.keys.size() - 1);
            V predValue = predChild.values.get(predChild.values.size() - 1);
            node.keys.set(idx, predKey);
            node.values.set(idx, predValue);
            deleteHelper(node.children.get(idx), predKey);
        } else if (node.children.get(idx + 1).keys.size() >= t) {
            // Case 2b: successor child has at least t keys
            BTreeNode succChild = node.children.get(idx + 1);
            while (!succChild.leaf) {
                succChild = succChild.children.get(0);
            }
            K succKey = succChild.keys.get(0);
            V succValue = succChild.values.get(0);
            node.keys.set(idx, succKey);
            node.values.set(idx, succValue);
            deleteHelper(node.children.get(idx + 1), succKey);
        } else {
            // Case 2c: both children have t-1 keys, merge them
            merge(node, idx);
            deleteHelper(node.children.get(idx), key);
        }
    }

    /**
     * Fill child node at index idx that has fewer than t keys.
     */
    private void fill(BTreeNode node, int idx) {
        if (idx > 0 && node.children.get(idx - 1).keys.size() >= t) {
            borrowFromPrev(node, idx);
        } else if (idx < node.keys.size() && node.children.get(idx + 1).keys.size() >= t) {
            borrowFromNext(node, idx);
        } else {
            if (idx < node.keys.size()) {
                merge(node, idx);
            } else {
                merge(node, idx - 1);
            }
        }
    }

    /**
     * Borrow a key from the previous sibling.
     */
    private void borrowFromPrev(BTreeNode node, int idx) {
        BTreeNode child = node.children.get(idx);
        BTreeNode sibling = node.children.get(idx - 1);

        // Move the separator key from parent down to child
        child.keys.add(0, node.keys.get(idx - 1));
        child.values.add(0, node.values.get(idx - 1));

        // Move the last key from sibling up to parent
        node.keys.set(idx - 1, sibling.keys.get(sibling.keys.size() - 1));
        node.values.set(idx - 1, sibling.values.get(sibling.values.size() - 1));

        // Move the last child of sibling to child
        if (!sibling.leaf) {
            child.children.add(0, sibling.children.get(sibling.children.size() - 1));
            sibling.children.remove(sibling.children.size() - 1);
        }

        sibling.keys.remove(sibling.keys.size() - 1);
        sibling.values.remove(sibling.values.size() - 1);
    }

    /**
     * Borrow a key from the next sibling.
     */
    private void borrowFromNext(BTreeNode node, int idx) {
        BTreeNode child = node.children.get(idx);
        BTreeNode sibling = node.children.get(idx + 1);

        // Move the separator key from parent down to child
        child.keys.add(node.keys.get(idx));
        child.values.add(node.values.get(idx));

        // Move the first key from sibling up to parent
        node.keys.set(idx, sibling.keys.get(0));
        node.values.set(idx, sibling.values.get(0));

        // Move the first child of sibling to child
        if (!sibling.leaf) {
            child.children.add(sibling.children.get(0));
            sibling.children.remove(0);
        }

        sibling.keys.remove(0);
        sibling.values.remove(0);
    }

    /**
     * Merge child at idx with child at idx+1.
     */
    private void merge(BTreeNode node, int idx) {
        BTreeNode left = node.children.get(idx);
        BTreeNode right = node.children.get(idx + 1);

        // Pull down the separator key from parent
        left.keys.add(node.keys.get(idx));
        left.values.add(node.values.get(idx));

        // Copy keys and values from right to left
        left.keys.addAll(right.keys);
        left.values.addAll(right.values);

        // Copy children from right to left
        if (!left.leaf) {
            left.children.addAll(right.children);
        }

        // Remove the separator key and right child from parent
        node.keys.remove(idx);
        node.values.remove(idx);
        node.children.remove(idx + 1);
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

    private void inorderHelper(BTreeNode node, List<Entry<K, V>> result) {
        if (node == null) return;
        for (int i = 0; i < node.keys.size(); i++) {
            if (!node.leaf) {
                inorderHelper(node.children.get(i), result);
            }
            result.add(new Entry<>(node.keys.get(i), node.values.get(i)));
        }
        if (!node.leaf) {
            inorderHelper(node.children.get(node.keys.size()), result);
        }
    }

    /**
     * Return all keys in sorted order.
     */
    public List<K> keys() {
        List<K> result = new ArrayList<>();
        keysHelper(root, result);
        return result;
    }

    private void keysHelper(BTreeNode node, List<K> result) {
        if (node == null) return;
        for (int i = 0; i < node.keys.size(); i++) {
            if (!node.leaf) {
                keysHelper(node.children.get(i), result);
            }
            result.add(node.keys.get(i));
        }
        if (!node.leaf) {
            keysHelper(node.children.get(node.keys.size()), result);
        }
    }

    /**
     * Return all values in key-sorted order.
     */
    public List<V> values() {
        List<V> result = new ArrayList<>();
        valuesHelper(root, result);
        return result;
    }

    private void valuesHelper(BTreeNode node, List<V> result) {
        if (node == null) return;
        for (int i = 0; i < node.keys.size(); i++) {
            if (!node.leaf) {
                valuesHelper(node.children.get(i), result);
            }
            result.add(node.values.get(i));
        }
        if (!node.leaf) {
            valuesHelper(node.children.get(node.keys.size()), result);
        }
    }

    // ======================== Min / Max ========================

    /**
     * Return the minimum key.
     */
    public Optional<K> min() {
        if (root.keys.isEmpty()) return Optional.empty();
        BTreeNode node = root;
        while (!node.leaf) {
            node = node.children.get(0);
        }
        return Optional.of(node.keys.get(0));
    }

    /**
     * Return the maximum key.
     */
    public Optional<K> max() {
        if (root.keys.isEmpty()) return Optional.empty();
        BTreeNode node = root;
        while (!node.leaf) {
            node = node.children.get(node.children.size() - 1);
        }
        return Optional.of(node.keys.get(node.keys.size() - 1));
    }

    // ======================== Properties ========================

    /**
     * Return the height of the tree.
     */
    public int height() {
        return heightHelper(root);
    }

    private int heightHelper(BTreeNode node) {
        if (node == null || node.keys.isEmpty()) return 0;
        if (node.leaf) return 0;
        return 1 + heightHelper(node.children.get(0));
    }

    /**
     * Clear the tree.
     */
    public void clear() {
        root = new BTreeNode(true);
        size = 0;
    }

    /**
     * Return the number of key-value pairs.
     */
    public int size() {
        return size;
    }

    /**
     * Return the order of the B-Tree (maximum number of children).
     */
    public int order() {
        return 2 * t;
    }

    /**
     * Return the minimum degree.
     */
    public int minDegree() {
        return t;
    }
}
