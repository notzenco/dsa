package dsa.datastructures;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class AvlTreeTest {

    private AvlTree<Integer, String> avl;

    @BeforeEach
    void setUp() {
        avl = new AvlTree<>();
    }

    // ======================== Insert ========================

    @Test
    void testInsertSingleElement() {
        avl.insert(10, "ten");
        assertEquals(1, avl.size());
        assertTrue(avl.search(10));
        assertEquals(Optional.of("ten"), avl.get(10));
    }

    @Test
    void testInsertMultipleElements() {
        avl.insert(10, "ten");
        avl.insert(20, "twenty");
        avl.insert(30, "thirty");
        assertEquals(3, avl.size());
        assertTrue(avl.isBalanced());
    }

    @Test
    void testInsertDuplicateKeyUpdatesValue() {
        avl.insert(10, "ten");
        avl.insert(10, "TEN");
        assertEquals(1, avl.size());
        assertEquals(Optional.of("TEN"), avl.get(10));
    }

    // ======================== Balance after Insert ========================

    @Test
    void testLeftLeftRotation() {
        // Inserting in decreasing order triggers right rotations
        avl.insert(30, "a");
        avl.insert(20, "b");
        avl.insert(10, "c");
        assertTrue(avl.isBalanced());
        assertEquals(1, avl.height());
        assertEquals(Arrays.asList(10, 20, 30), avl.inorder());
    }

    @Test
    void testRightRightRotation() {
        // Inserting in increasing order triggers left rotations
        avl.insert(10, "a");
        avl.insert(20, "b");
        avl.insert(30, "c");
        assertTrue(avl.isBalanced());
        assertEquals(1, avl.height());
    }

    @Test
    void testLeftRightRotation() {
        avl.insert(30, "a");
        avl.insert(10, "b");
        avl.insert(20, "c");
        assertTrue(avl.isBalanced());
        assertEquals(1, avl.height());
    }

    @Test
    void testRightLeftRotation() {
        avl.insert(10, "a");
        avl.insert(30, "b");
        avl.insert(20, "c");
        assertTrue(avl.isBalanced());
        assertEquals(1, avl.height());
    }

    @Test
    void testBalanceAfterManyInserts() {
        for (int i = 1; i <= 100; i++) {
            avl.insert(i, "v" + i);
        }
        assertTrue(avl.isBalanced());
        assertEquals(100, avl.size());
        // Height of AVL with 100 nodes should be at most ~7
        assertTrue(avl.height() <= 8);
    }

    // ======================== Delete ========================

    @Test
    void testDeleteExisting() {
        avl.insert(10, "ten");
        avl.insert(20, "twenty");
        avl.insert(30, "thirty");
        assertTrue(avl.delete(20));
        assertEquals(2, avl.size());
        assertFalse(avl.search(20));
        assertTrue(avl.isBalanced());
    }

    @Test
    void testDeleteNonExisting() {
        avl.insert(10, "ten");
        assertFalse(avl.delete(99));
        assertEquals(1, avl.size());
    }

    @Test
    void testDeleteRoot() {
        avl.insert(10, "ten");
        avl.insert(5, "five");
        avl.insert(15, "fifteen");
        assertTrue(avl.delete(10));
        assertEquals(2, avl.size());
        assertTrue(avl.isBalanced());
    }

    @Test
    void testDeleteAllElements() {
        avl.insert(10, "a");
        avl.insert(5, "b");
        avl.insert(15, "c");
        assertTrue(avl.delete(10));
        assertTrue(avl.delete(5));
        assertTrue(avl.delete(15));
        assertEquals(0, avl.size());
        assertEquals(-1, avl.height());
    }

    @Test
    void testDeleteMaintainsBalance() {
        // Insert many elements then delete some
        for (int i = 1; i <= 20; i++) {
            avl.insert(i, "v" + i);
        }
        for (int i = 1; i <= 10; i++) {
            avl.delete(i);
            assertTrue(avl.isBalanced(), "Tree not balanced after deleting " + i);
        }
        assertEquals(10, avl.size());
    }

    // ======================== Search and Get ========================

    @Test
    void testSearchExisting() {
        avl.insert(10, "ten");
        avl.insert(5, "five");
        assertTrue(avl.search(10));
        assertTrue(avl.search(5));
    }

    @Test
    void testSearchNonExisting() {
        avl.insert(10, "ten");
        assertFalse(avl.search(99));
    }

    @Test
    void testSearchEmptyTree() {
        assertFalse(avl.search(1));
    }

    @Test
    void testGetExisting() {
        avl.insert(10, "ten");
        assertEquals(Optional.of("ten"), avl.get(10));
    }

    @Test
    void testGetNonExisting() {
        assertFalse(avl.get(99).isPresent());
    }

    // ======================== Traversals ========================

    @Test
    void testInorder() {
        avl.insert(5, "a");
        avl.insert(3, "b");
        avl.insert(7, "c");
        avl.insert(1, "d");
        avl.insert(4, "e");
        assertEquals(Arrays.asList(1, 3, 4, 5, 7), avl.inorder());
    }

    @Test
    void testPreorder() {
        avl.insert(5, "a");
        avl.insert(3, "b");
        avl.insert(7, "c");
        List<Integer> preorder = avl.preorder();
        assertEquals(3, preorder.size());
        // Root should be first in preorder
        assertEquals(avl.getRoot().getKey(), preorder.get(0));
    }

    @Test
    void testLevelOrder() {
        avl.insert(5, "a");
        avl.insert(3, "b");
        avl.insert(7, "c");
        List<Integer> levelOrder = avl.levelOrder();
        assertEquals(3, levelOrder.size());
        // Root should be first in level order
        assertEquals(avl.getRoot().getKey(), levelOrder.get(0));
    }

    @Test
    void testTraversalsEmpty() {
        assertTrue(avl.inorder().isEmpty());
        assertTrue(avl.preorder().isEmpty());
        assertTrue(avl.levelOrder().isEmpty());
    }

    // ======================== Min / Max ========================

    @Test
    void testMinimum() {
        avl.insert(10, "a");
        avl.insert(5, "b");
        avl.insert(15, "c");
        avl.insert(3, "d");
        assertEquals(Optional.of(3), avl.minimum());
    }

    @Test
    void testMaximum() {
        avl.insert(10, "a");
        avl.insert(5, "b");
        avl.insert(15, "c");
        avl.insert(20, "d");
        assertEquals(Optional.of(20), avl.maximum());
    }

    @Test
    void testMinMaxEmpty() {
        assertFalse(avl.minimum().isPresent());
        assertFalse(avl.maximum().isPresent());
    }

    // ======================== Height ========================

    @Test
    void testHeightEmptyTree() {
        assertEquals(-1, avl.height());
    }

    @Test
    void testHeightSingleNode() {
        avl.insert(10, "ten");
        assertEquals(0, avl.height());
    }

    @Test
    void testHeightAfterInserts() {
        avl.insert(10, "a");
        avl.insert(5, "b");
        avl.insert(15, "c");
        assertEquals(1, avl.height());
    }

    // ======================== Clear ========================

    @Test
    void testClear() {
        avl.insert(10, "a");
        avl.insert(5, "b");
        avl.insert(15, "c");
        avl.clear();
        assertEquals(0, avl.size());
        assertEquals(-1, avl.height());
        assertNull(avl.getRoot());
    }

    // ======================== Stress Test ========================

    @Test
    void testLargeSequentialInsertAndDelete() {
        for (int i = 0; i < 1000; i++) {
            avl.insert(i, "v" + i);
        }
        assertEquals(1000, avl.size());
        assertTrue(avl.isBalanced());

        for (int i = 0; i < 500; i++) {
            assertTrue(avl.delete(i));
        }
        assertEquals(500, avl.size());
        assertTrue(avl.isBalanced());

        // Verify remaining elements
        for (int i = 500; i < 1000; i++) {
            assertTrue(avl.search(i));
        }
    }
}
