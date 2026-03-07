package dsa.datastructures;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class BinarySearchTreeTest {

    private BinarySearchTree<Integer> bst;

    @BeforeEach
    void setUp() {
        bst = new BinarySearchTree<>();
    }

    // ======================== Insert ========================

    @Test
    void testInsertSingleElement() {
        bst.insert(10);
        assertEquals(1, bst.size());
        assertFalse(bst.isEmpty());
        assertEquals(10, bst.getRoot().getValue());
    }

    @Test
    void testInsertMultipleElements() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(5, bst.size());
    }

    @Test
    void testInsertReturnsNode() {
        BinarySearchTree<Integer>.TreeNode node = bst.insert(42);
        assertNotNull(node);
        assertEquals(42, node.getValue());
    }

    // ======================== Search ========================

    @Test
    void testSearchExistingElement() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        Optional<BinarySearchTree<Integer>.TreeNode> result = bst.search(5);
        assertTrue(result.isPresent());
        assertEquals(5, result.get().getValue());
    }

    @Test
    void testSearchNonExistingElement() {
        bst.insert(10);
        bst.insert(5);
        Optional<BinarySearchTree<Integer>.TreeNode> result = bst.search(20);
        assertFalse(result.isPresent());
    }

    @Test
    void testSearchEmptyTree() {
        assertFalse(bst.search(1).isPresent());
    }

    // ======================== Delete ========================

    @Test
    void testDeleteLeafNode() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertTrue(bst.delete(5));
        assertEquals(2, bst.size());
        assertFalse(bst.search(5).isPresent());
    }

    @Test
    void testDeleteNodeWithOneChild() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(3);
        assertTrue(bst.delete(5));
        assertEquals(2, bst.size());
        assertTrue(bst.search(3).isPresent());
    }

    @Test
    void testDeleteNodeWithTwoChildren() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertTrue(bst.delete(5));
        assertEquals(4, bst.size());
        assertTrue(bst.search(3).isPresent());
        assertTrue(bst.search(7).isPresent());
    }

    @Test
    void testDeleteRoot() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertTrue(bst.delete(10));
        assertEquals(2, bst.size());
        assertFalse(bst.search(10).isPresent());
    }

    @Test
    void testDeleteNonExisting() {
        bst.insert(10);
        assertFalse(bst.delete(99));
        assertEquals(1, bst.size());
    }

    @Test
    void testDeleteOnlyElement() {
        bst.insert(10);
        assertTrue(bst.delete(10));
        assertTrue(bst.isEmpty());
    }

    // ======================== Traversals ========================

    @Test
    void testInorder() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(Arrays.asList(3, 5, 7, 10, 15), bst.inorder());
    }

    @Test
    void testPreorder() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(Arrays.asList(10, 5, 3, 7, 15), bst.preorder());
    }

    @Test
    void testPostorder() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(Arrays.asList(3, 7, 5, 15, 10), bst.postorder());
    }

    @Test
    void testLevelorder() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(Arrays.asList(10, 5, 15, 3, 7), bst.levelorder());
    }

    @Test
    void testTraversalsEmptyTree() {
        assertTrue(bst.inorder().isEmpty());
        assertTrue(bst.preorder().isEmpty());
        assertTrue(bst.postorder().isEmpty());
        assertTrue(bst.levelorder().isEmpty());
    }

    // ======================== Min / Max ========================

    @Test
    void testMinimum() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        assertEquals(Optional.of(3), bst.minimum());
    }

    @Test
    void testMaximum() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(20);
        assertEquals(Optional.of(20), bst.maximum());
    }

    @Test
    void testMinMaxEmptyTree() {
        assertFalse(bst.minimum().isPresent());
        assertFalse(bst.maximum().isPresent());
    }

    // ======================== Successor / Predecessor ========================

    @Test
    void testSuccessor() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        bst.insert(12);
        assertEquals(Optional.of(7), bst.successor(5));
        assertEquals(Optional.of(12), bst.successor(10));
        assertEquals(Optional.of(10), bst.successor(7));
    }

    @Test
    void testSuccessorOfMaximum() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertFalse(bst.successor(15).isPresent());
    }

    @Test
    void testSuccessorNotFound() {
        bst.insert(10);
        assertFalse(bst.successor(99).isPresent());
    }

    @Test
    void testPredecessor() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        bst.insert(12);
        assertEquals(Optional.of(3), bst.predecessor(5));
        assertEquals(Optional.of(7), bst.predecessor(10));
        assertEquals(Optional.of(12), bst.predecessor(15));
    }

    @Test
    void testPredecessorOfMinimum() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertFalse(bst.predecessor(5).isPresent());
    }

    // ======================== isValidBst ========================

    @Test
    void testIsValidBst() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertTrue(bst.isValidBst());
    }

    @Test
    void testIsValidBstEmptyTree() {
        assertTrue(bst.isValidBst());
    }

    // ======================== kthSmallest ========================

    @Test
    void testKthSmallest() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        assertEquals(Optional.of(3), bst.kthSmallest(1));
        assertEquals(Optional.of(5), bst.kthSmallest(2));
        assertEquals(Optional.of(7), bst.kthSmallest(3));
        assertEquals(Optional.of(10), bst.kthSmallest(4));
        assertEquals(Optional.of(15), bst.kthSmallest(5));
    }

    @Test
    void testKthSmallestOutOfRange() {
        bst.insert(10);
        assertFalse(bst.kthSmallest(0).isPresent());
        assertFalse(bst.kthSmallest(2).isPresent());
    }

    // ======================== Floor / Ceiling ========================

    @Test
    void testFloor() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        assertEquals(Optional.of(5), bst.floor(6));
        assertEquals(Optional.of(10), bst.floor(10));
        assertEquals(Optional.of(15), bst.floor(20));
    }

    @Test
    void testFloorNoFloor() {
        bst.insert(10);
        assertFalse(bst.floor(5).isPresent());
    }

    @Test
    void testCeiling() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        assertEquals(Optional.of(10), bst.ceiling(6));
        assertEquals(Optional.of(5), bst.ceiling(5));
        assertEquals(Optional.of(3), bst.ceiling(1));
    }

    @Test
    void testCeilingNoCeiling() {
        bst.insert(10);
        assertFalse(bst.ceiling(15).isPresent());
    }

    // ======================== Range Query ========================

    @Test
    void testRangeQuery() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(3);
        bst.insert(7);
        bst.insert(12);
        bst.insert(20);
        assertEquals(Arrays.asList(5, 7, 10, 12), bst.rangeQuery(5, 12));
    }

    @Test
    void testRangeQueryNoResults() {
        bst.insert(10);
        bst.insert(5);
        assertTrue(bst.rangeQuery(6, 9).isEmpty());
    }

    @Test
    void testRangeQuerySingleElement() {
        bst.insert(10);
        assertEquals(Arrays.asList(10), bst.rangeQuery(10, 10));
    }

    // ======================== Height / Size / isEmpty / clear ========================

    @Test
    void testHeight() {
        assertEquals(-1, bst.height());
        bst.insert(10);
        assertEquals(0, bst.height());
        bst.insert(5);
        bst.insert(15);
        assertEquals(1, bst.height());
        bst.insert(3);
        assertEquals(2, bst.height());
    }

    @Test
    void testSizeAndIsEmpty() {
        assertTrue(bst.isEmpty());
        assertEquals(0, bst.size());
        bst.insert(10);
        assertFalse(bst.isEmpty());
        assertEquals(1, bst.size());
    }

    @Test
    void testClear() {
        bst.insert(10);
        bst.insert(5);
        bst.clear();
        assertTrue(bst.isEmpty());
        assertEquals(0, bst.size());
        assertNull(bst.getRoot());
    }

    // ======================== toList ========================

    @Test
    void testToList() {
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        assertEquals(Arrays.asList(5, 10, 15), bst.toList());
    }

    // ======================== fromList ========================

    @Test
    void testFromList() {
        BinarySearchTree<Integer> tree = BinarySearchTree.fromList(Arrays.asList(10, 5, 15, 3, 7));
        assertEquals(5, tree.size());
        assertEquals(Arrays.asList(3, 5, 7, 10, 15), tree.inorder());
    }

    // ======================== fromSortedList ========================

    @Test
    void testFromSortedList() {
        List<Integer> sorted = Arrays.asList(1, 2, 3, 4, 5, 6, 7);
        BinarySearchTree<Integer> tree = BinarySearchTree.fromSortedList(sorted);
        assertEquals(7, tree.size());
        assertEquals(sorted, tree.inorder());
        assertTrue(tree.isValidBst());
        // A balanced tree from 7 elements should have height 2
        assertEquals(2, tree.height());
    }

    @Test
    void testFromSortedListEmpty() {
        BinarySearchTree<Integer> tree = BinarySearchTree.fromSortedList(List.<Integer>of());
        assertTrue(tree.isEmpty());
    }

    // ======================== Lowest Common Ancestor ========================

    @Test
    void testLowestCommonAncestor() {
        bst.insert(20);
        bst.insert(10);
        bst.insert(30);
        bst.insert(5);
        bst.insert(15);
        bst.insert(25);
        bst.insert(35);
        assertEquals(Optional.of(20), bst.lowestCommonAncestor(5, 35));
        assertEquals(Optional.of(10), bst.lowestCommonAncestor(5, 15));
        assertEquals(Optional.of(30), bst.lowestCommonAncestor(25, 35));
    }

    @Test
    void testLowestCommonAncestorSameNode() {
        bst.insert(10);
        bst.insert(5);
        assertEquals(Optional.of(5), bst.lowestCommonAncestor(5, 5));
    }

    @Test
    void testLowestCommonAncestorNotFound() {
        bst.insert(10);
        assertFalse(bst.lowestCommonAncestor(10, 99).isPresent());
    }

    // ======================== String BST ========================

    @Test
    void testStringBst() {
        BinarySearchTree<String> stringBst = new BinarySearchTree<>();
        stringBst.insert("mango");
        stringBst.insert("apple");
        stringBst.insert("zebra");
        stringBst.insert("banana");
        assertEquals(Arrays.asList("apple", "banana", "mango", "zebra"), stringBst.inorder());
        assertEquals(Optional.of("apple"), stringBst.minimum());
        assertEquals(Optional.of("zebra"), stringBst.maximum());
    }

    // ======================== Delete then verify structure ========================

    @Test
    void testDeleteAndVerifyTraversal() {
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);
        bst.insert(60);
        bst.insert(80);

        bst.delete(30);
        List<Integer> expected = Arrays.asList(20, 40, 50, 60, 70, 80);
        assertEquals(expected, bst.inorder());
        assertTrue(bst.isValidBst());
    }
}
