package dsa.datastructures;

import java.util.*;

/**
 * Generic Binary Search Tree implementation.
 *
 * @param <T> the type of elements, must be Comparable
 */
public class BinarySearchTree<T extends Comparable<T>> {

    public class TreeNode {
        T value;
        TreeNode left;
        TreeNode right;
        TreeNode parent;

        public TreeNode(T value) {
            this.value = value;
        }

        public T getValue() {
            return value;
        }

        public TreeNode getLeft() {
            return left;
        }

        public TreeNode getRight() {
            return right;
        }

        public TreeNode getParent() {
            return parent;
        }
    }

    private TreeNode root;
    private int size;

    public BinarySearchTree() {
        this.root = null;
        this.size = 0;
    }

    public TreeNode getRoot() {
        return root;
    }

    /**
     * Insert a value into the BST.
     */
    public TreeNode insert(T value) {
        TreeNode newNode = new TreeNode(value);
        if (root == null) {
            root = newNode;
            size++;
            return newNode;
        }
        TreeNode current = root;
        TreeNode parent = null;
        while (current != null) {
            parent = current;
            int cmp = value.compareTo(current.value);
            if (cmp < 0) {
                current = current.left;
            } else if (cmp > 0) {
                current = current.right;
            } else {
                // Duplicate: go right
                current = current.right;
            }
        }
        newNode.parent = parent;
        if (value.compareTo(parent.value) < 0) {
            parent.left = newNode;
        } else {
            parent.right = newNode;
        }
        size++;
        return newNode;
    }

    /**
     * Search for a value in the BST.
     */
    public Optional<TreeNode> search(T value) {
        TreeNode node = searchNode(root, value);
        return Optional.ofNullable(node);
    }

    private TreeNode searchNode(TreeNode node, T value) {
        if (node == null) {
            return null;
        }
        int cmp = value.compareTo(node.value);
        if (cmp == 0) {
            return node;
        } else if (cmp < 0) {
            return searchNode(node.left, value);
        } else {
            return searchNode(node.right, value);
        }
    }

    /**
     * Delete a value from the BST.
     */
    public boolean delete(T value) {
        TreeNode node = searchNode(root, value);
        if (node == null) {
            return false;
        }
        deleteNode(node);
        size--;
        return true;
    }

    private void transplant(TreeNode u, TreeNode v) {
        if (u.parent == null) {
            root = v;
        } else if (u == u.parent.left) {
            u.parent.left = v;
        } else {
            u.parent.right = v;
        }
        if (v != null) {
            v.parent = u.parent;
        }
    }

    private void deleteNode(TreeNode node) {
        if (node.left == null) {
            transplant(node, node.right);
        } else if (node.right == null) {
            transplant(node, node.left);
        } else {
            TreeNode successor = minimumNode(node.right);
            if (successor.parent != node) {
                transplant(successor, successor.right);
                successor.right = node.right;
                successor.right.parent = successor;
            }
            transplant(node, successor);
            successor.left = node.left;
            successor.left.parent = successor;
        }
    }

    /**
     * Find the minimum value node.
     */
    public Optional<T> minimum() {
        if (root == null) return Optional.empty();
        return Optional.of(minimumNode(root).value);
    }

    private TreeNode minimumNode(TreeNode node) {
        while (node.left != null) {
            node = node.left;
        }
        return node;
    }

    /**
     * Find the maximum value node.
     */
    public Optional<T> maximum() {
        if (root == null) return Optional.empty();
        return Optional.of(maximumNode(root).value);
    }

    private TreeNode maximumNode(TreeNode node) {
        while (node.right != null) {
            node = node.right;
        }
        return node;
    }

    /**
     * Find the in-order successor of a value.
     */
    public Optional<T> successor(T value) {
        TreeNode node = searchNode(root, value);
        if (node == null) return Optional.empty();
        if (node.right != null) {
            return Optional.of(minimumNode(node.right).value);
        }
        TreeNode parent = node.parent;
        while (parent != null && node == parent.right) {
            node = parent;
            parent = parent.parent;
        }
        return parent == null ? Optional.empty() : Optional.of(parent.value);
    }

    /**
     * Find the in-order predecessor of a value.
     */
    public Optional<T> predecessor(T value) {
        TreeNode node = searchNode(root, value);
        if (node == null) return Optional.empty();
        if (node.left != null) {
            return Optional.of(maximumNode(node.left).value);
        }
        TreeNode parent = node.parent;
        while (parent != null && node == parent.left) {
            node = parent;
            parent = parent.parent;
        }
        return parent == null ? Optional.empty() : Optional.of(parent.value);
    }

    /**
     * In-order traversal.
     */
    public List<T> inorder() {
        List<T> result = new ArrayList<>();
        inorderHelper(root, result);
        return result;
    }

    private void inorderHelper(TreeNode node, List<T> result) {
        if (node == null) return;
        inorderHelper(node.left, result);
        result.add(node.value);
        inorderHelper(node.right, result);
    }

    /**
     * Pre-order traversal.
     */
    public List<T> preorder() {
        List<T> result = new ArrayList<>();
        preorderHelper(root, result);
        return result;
    }

    private void preorderHelper(TreeNode node, List<T> result) {
        if (node == null) return;
        result.add(node.value);
        preorderHelper(node.left, result);
        preorderHelper(node.right, result);
    }

    /**
     * Post-order traversal.
     */
    public List<T> postorder() {
        List<T> result = new ArrayList<>();
        postorderHelper(root, result);
        return result;
    }

    private void postorderHelper(TreeNode node, List<T> result) {
        if (node == null) return;
        postorderHelper(node.left, result);
        postorderHelper(node.right, result);
        result.add(node.value);
    }

    /**
     * Level-order (BFS) traversal.
     */
    public List<T> levelorder() {
        List<T> result = new ArrayList<>();
        if (root == null) return result;
        java.util.Queue<TreeNode> queue = new java.util.LinkedList<>();
        queue.add(root);
        while (!queue.isEmpty()) {
            TreeNode current = queue.poll();
            result.add(current.value);
            if (current.left != null) queue.add(current.left);
            if (current.right != null) queue.add(current.right);
        }
        return result;
    }

    /**
     * Check if the tree is a valid BST.
     */
    public boolean isValidBst() {
        return isValidBstHelper(root, null, null);
    }

    private boolean isValidBstHelper(TreeNode node, T min, T max) {
        if (node == null) return true;
        if (min != null && node.value.compareTo(min) <= 0) return false;
        if (max != null && node.value.compareTo(max) >= 0) return false;
        return isValidBstHelper(node.left, min, node.value)
                && isValidBstHelper(node.right, node.value, max);
    }

    /**
     * Find the kth smallest element (1-indexed).
     */
    public Optional<T> kthSmallest(int k) {
        if (k < 1 || k > size) return Optional.empty();
        int[] counter = {0};
        T[] result = (T[]) new Comparable[1];
        kthSmallestHelper(root, k, counter, result);
        return Optional.ofNullable(result[0]);
    }

    private void kthSmallestHelper(TreeNode node, int k, int[] counter, T[] result) {
        if (node == null || result[0] != null) return;
        kthSmallestHelper(node.left, k, counter, result);
        counter[0]++;
        if (counter[0] == k) {
            result[0] = node.value;
            return;
        }
        kthSmallestHelper(node.right, k, counter, result);
    }

    /**
     * Find the largest value less than or equal to the given value.
     */
    public Optional<T> floor(T value) {
        return Optional.ofNullable(floorHelper(root, value));
    }

    private T floorHelper(TreeNode node, T value) {
        if (node == null) return null;
        int cmp = value.compareTo(node.value);
        if (cmp == 0) return node.value;
        if (cmp < 0) return floorHelper(node.left, value);
        T rightFloor = floorHelper(node.right, value);
        return rightFloor != null ? rightFloor : node.value;
    }

    /**
     * Find the smallest value greater than or equal to the given value.
     */
    public Optional<T> ceiling(T value) {
        return Optional.ofNullable(ceilingHelper(root, value));
    }

    private T ceilingHelper(TreeNode node, T value) {
        if (node == null) return null;
        int cmp = value.compareTo(node.value);
        if (cmp == 0) return node.value;
        if (cmp > 0) return ceilingHelper(node.right, value);
        T leftCeiling = ceilingHelper(node.left, value);
        return leftCeiling != null ? leftCeiling : node.value;
    }

    /**
     * Return all values in [low, high] inclusive, in sorted order.
     */
    public List<T> rangeQuery(T low, T high) {
        List<T> result = new ArrayList<>();
        rangeQueryHelper(root, low, high, result);
        return result;
    }

    private void rangeQueryHelper(TreeNode node, T low, T high, List<T> result) {
        if (node == null) return;
        if (low.compareTo(node.value) < 0) {
            rangeQueryHelper(node.left, low, high, result);
        }
        if (low.compareTo(node.value) <= 0 && high.compareTo(node.value) >= 0) {
            result.add(node.value);
        }
        if (high.compareTo(node.value) > 0) {
            rangeQueryHelper(node.right, low, high, result);
        }
    }

    /**
     * Return the height of the tree. Empty tree has height -1.
     */
    public int height() {
        return heightHelper(root);
    }

    private int heightHelper(TreeNode node) {
        if (node == null) return -1;
        return 1 + Math.max(heightHelper(node.left), heightHelper(node.right));
    }

    /**
     * Return the number of nodes.
     */
    public int size() {
        return size;
    }

    /**
     * Check if the tree is empty.
     */
    public boolean isEmpty() {
        return root == null;
    }

    /**
     * Remove all nodes.
     */
    public void clear() {
        root = null;
        size = 0;
    }

    /**
     * Return in-order traversal as a list.
     */
    public List<T> toList() {
        return inorder();
    }

    /**
     * Build a BST from an unsorted list by inserting elements sequentially.
     */
    public static <T extends Comparable<T>> BinarySearchTree<T> fromList(List<T> list) {
        BinarySearchTree<T> bst = new BinarySearchTree<>();
        for (T item : list) {
            bst.insert(item);
        }
        return bst;
    }

    /**
     * Build a balanced BST from a sorted list.
     */
    public static <T extends Comparable<T>> BinarySearchTree<T> fromSortedList(List<T> sortedList) {
        BinarySearchTree<T> bst = new BinarySearchTree<>();
        bst.root = bst.buildFromSorted(sortedList, 0, sortedList.size() - 1, null);
        bst.size = sortedList.size();
        return bst;
    }

    private TreeNode buildFromSorted(List<T> list, int start, int end, TreeNode parent) {
        if (start > end) return null;
        int mid = start + (end - start) / 2;
        TreeNode node = new TreeNode(list.get(mid));
        node.parent = parent;
        node.left = buildFromSorted(list, start, mid - 1, node);
        node.right = buildFromSorted(list, mid + 1, end, node);
        return node;
    }

    /**
     * Find the lowest common ancestor of two values.
     */
    public Optional<T> lowestCommonAncestor(T val1, T val2) {
        // Both values must exist
        if (searchNode(root, val1) == null || searchNode(root, val2) == null) {
            return Optional.empty();
        }
        TreeNode lca = lcaHelper(root, val1, val2);
        return lca == null ? Optional.empty() : Optional.of(lca.value);
    }

    private TreeNode lcaHelper(TreeNode node, T val1, T val2) {
        if (node == null) return null;
        if (val1.compareTo(node.value) < 0 && val2.compareTo(node.value) < 0) {
            return lcaHelper(node.left, val1, val2);
        }
        if (val1.compareTo(node.value) > 0 && val2.compareTo(node.value) > 0) {
            return lcaHelper(node.right, val1, val2);
        }
        return node;
    }
}
