package dsa.datastructures;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class RedBlackTreeTest {

    private RedBlackTree<Integer, String> rbt;

    @BeforeEach
    void setUp() {
        rbt = new RedBlackTree<>();
    }

    // ======================== Insert ========================

    @Test
    void testInsertSingleElement() {
        rbt.insert(10, "ten");
        assertEquals(1, rbt.size());
        assertTrue(rbt.search(10).isPresent());
        assertEquals("ten", rbt.search(10).get());
    }

    @Test
    void testInsertMultipleElements() {
        rbt.insert(10, "ten");
        rbt.insert(20, "twenty");
        rbt.insert(30, "thirty");
        assertEquals(3, rbt.size());
        assertTrue(rbt.isValid());
    }

    @Test
    void testInsertDuplicateKeyUpdatesValue() {
        rbt.insert(10, "ten");
        rbt.insert(10, "TEN");
        assertEquals(1, rbt.size());
        assertEquals(Optional.of("TEN"), rbt.search(10));
    }

    @Test
    void testInsertSequentialAscending() {
        for (int i = 1; i <= 20; i++) {
            rbt.insert(i, "v" + i);
            assertTrue(rbt.isValid(), "RB properties violated after inserting " + i);
        }
        assertEquals(20, rbt.size());
    }

    @Test
    void testInsertSequentialDescending() {
        for (int i = 20; i >= 1; i--) {
            rbt.insert(i, "v" + i);
            assertTrue(rbt.isValid(), "RB properties violated after inserting " + i);
        }
        assertEquals(20, rbt.size());
    }

    @Test
    void testRootIsAlwaysBlack() {
        rbt.insert(10, "a");
        assertEquals(RedBlackTree.Color.BLACK, rbt.getRoot().getColor());

        rbt.insert(5, "b");
        rbt.insert(15, "c");
        assertEquals(RedBlackTree.Color.BLACK, rbt.getRoot().getColor());
    }

    // ======================== Delete ========================

    @Test
    void testDeleteExisting() {
        rbt.insert(10, "ten");
        rbt.insert(20, "twenty");
        rbt.insert(30, "thirty");
        assertTrue(rbt.delete(20));
        assertEquals(2, rbt.size());
        assertFalse(rbt.search(20).isPresent());
        assertTrue(rbt.isValid());
    }

    @Test
    void testDeleteNonExisting() {
        rbt.insert(10, "ten");
        assertFalse(rbt.delete(99));
        assertEquals(1, rbt.size());
    }

    @Test
    void testDeleteRoot() {
        rbt.insert(10, "ten");
        rbt.insert(5, "five");
        rbt.insert(15, "fifteen");
        assertTrue(rbt.delete(10));
        assertEquals(2, rbt.size());
        assertTrue(rbt.isValid());
    }

    @Test
    void testDeleteAllElements() {
        rbt.insert(10, "a");
        rbt.insert(5, "b");
        rbt.insert(15, "c");
        assertTrue(rbt.delete(10));
        assertTrue(rbt.delete(5));
        assertTrue(rbt.delete(15));
        assertEquals(0, rbt.size());
        assertNull(rbt.getRoot());
    }

    @Test
    void testDeleteMaintainsRBProperties() {
        for (int i = 1; i <= 30; i++) {
            rbt.insert(i, "v" + i);
        }
        for (int i = 1; i <= 15; i++) {
            assertTrue(rbt.delete(i), "Failed to delete " + i);
            assertTrue(rbt.isValid(), "RB properties violated after deleting " + i);
        }
        assertEquals(15, rbt.size());
    }

    @Test
    void testDeleteLeafNodes() {
        rbt.insert(20, "a");
        rbt.insert(10, "b");
        rbt.insert(30, "c");
        rbt.insert(5, "d");
        rbt.insert(15, "e");
        rbt.insert(25, "f");
        rbt.insert(35, "g");

        assertTrue(rbt.delete(5));
        assertTrue(rbt.isValid());
        assertTrue(rbt.delete(35));
        assertTrue(rbt.isValid());
    }

    // ======================== Search ========================

    @Test
    void testSearchExisting() {
        rbt.insert(10, "ten");
        rbt.insert(5, "five");
        rbt.insert(15, "fifteen");
        assertEquals(Optional.of("ten"), rbt.search(10));
        assertEquals(Optional.of("five"), rbt.search(5));
        assertEquals(Optional.of("fifteen"), rbt.search(15));
    }

    @Test
    void testSearchNonExisting() {
        rbt.insert(10, "ten");
        assertFalse(rbt.search(99).isPresent());
    }

    @Test
    void testSearchEmptyTree() {
        assertFalse(rbt.search(1).isPresent());
    }

    // ======================== isValid ========================

    @Test
    void testIsValidEmptyTree() {
        assertTrue(rbt.isValid());
    }

    @Test
    void testIsValidSingleNode() {
        rbt.insert(10, "ten");
        assertTrue(rbt.isValid());
    }

    @Test
    void testIsValidAfterMixedOperations() {
        rbt.insert(50, "a");
        rbt.insert(25, "b");
        rbt.insert(75, "c");
        rbt.insert(10, "d");
        rbt.insert(30, "e");
        rbt.insert(60, "f");
        rbt.insert(90, "g");
        assertTrue(rbt.isValid());

        rbt.delete(25);
        assertTrue(rbt.isValid());
        rbt.delete(75);
        assertTrue(rbt.isValid());
        rbt.insert(80, "h");
        assertTrue(rbt.isValid());
    }

    // ======================== Traversals ========================

    @Test
    void testInorder() {
        rbt.insert(30, "a");
        rbt.insert(10, "b");
        rbt.insert(50, "c");
        rbt.insert(20, "d");
        rbt.insert(40, "e");

        List<RedBlackTree.Entry<Integer, String>> entries = rbt.inorder();
        assertEquals(5, entries.size());
        assertEquals(10, entries.get(0).getKey());
        assertEquals(20, entries.get(1).getKey());
        assertEquals(30, entries.get(2).getKey());
        assertEquals(40, entries.get(3).getKey());
        assertEquals(50, entries.get(4).getKey());
    }

    @Test
    void testKeys() {
        rbt.insert(30, "a");
        rbt.insert(10, "b");
        rbt.insert(50, "c");
        assertEquals(Arrays.asList(10, 30, 50), rbt.keys());
    }

    @Test
    void testValues() {
        rbt.insert(30, "a");
        rbt.insert(10, "b");
        rbt.insert(50, "c");
        assertEquals(Arrays.asList("b", "a", "c"), rbt.values());
    }

    @Test
    void testTraversalsEmpty() {
        assertTrue(rbt.inorder().isEmpty());
        assertTrue(rbt.keys().isEmpty());
        assertTrue(rbt.values().isEmpty());
    }

    // ======================== Min / Max ========================

    @Test
    void testMin() {
        rbt.insert(30, "a");
        rbt.insert(10, "b");
        rbt.insert(50, "c");
        assertEquals(Optional.of(10), rbt.min());
    }

    @Test
    void testMax() {
        rbt.insert(30, "a");
        rbt.insert(10, "b");
        rbt.insert(50, "c");
        assertEquals(Optional.of(50), rbt.max());
    }

    @Test
    void testMinMaxEmpty() {
        assertFalse(rbt.min().isPresent());
        assertFalse(rbt.max().isPresent());
    }

    // ======================== Height ========================

    @Test
    void testHeightEmptyTree() {
        assertEquals(-1, rbt.height());
    }

    @Test
    void testHeightSingleNode() {
        rbt.insert(10, "ten");
        assertEquals(0, rbt.height());
    }

    @Test
    void testHeightAfterInserts() {
        for (int i = 1; i <= 15; i++) {
            rbt.insert(i, "v" + i);
        }
        // RB tree height should be bounded: h <= 2*log2(n+1)
        int h = rbt.height();
        assertTrue(h <= 2 * (int) Math.ceil(Math.log(16) / Math.log(2)));
    }

    // ======================== Black Height ========================

    @Test
    void testBlackHeightEmptyTree() {
        assertEquals(0, rbt.blackHeight());
    }

    @Test
    void testBlackHeightConsistency() {
        for (int i = 1; i <= 31; i++) {
            rbt.insert(i, "v" + i);
        }
        int bh = rbt.blackHeight();
        assertTrue(bh > 0);
        assertTrue(rbt.isValid());
    }

    // ======================== Clear ========================

    @Test
    void testClear() {
        rbt.insert(10, "a");
        rbt.insert(5, "b");
        rbt.insert(15, "c");
        rbt.clear();
        assertEquals(0, rbt.size());
        assertNull(rbt.getRoot());
        assertTrue(rbt.isValid());
    }

    // ======================== Stress Test ========================

    @Test
    void testLargeScaleInsertDeleteValid() {
        for (int i = 0; i < 500; i++) {
            rbt.insert(i, "v" + i);
        }
        assertEquals(500, rbt.size());
        assertTrue(rbt.isValid());

        for (int i = 0; i < 250; i++) {
            assertTrue(rbt.delete(i));
            assertTrue(rbt.isValid(), "RB violation after deleting " + i);
        }
        assertEquals(250, rbt.size());

        for (int i = 250; i < 500; i++) {
            assertTrue(rbt.search(i).isPresent());
        }
    }

    @Test
    void testStringKeys() {
        RedBlackTree<String, Integer> tree = new RedBlackTree<>();
        tree.insert("banana", 2);
        tree.insert("apple", 1);
        tree.insert("cherry", 3);
        tree.insert("date", 4);

        assertEquals(Arrays.asList("apple", "banana", "cherry", "date"), tree.keys());
        assertTrue(tree.isValid());
        assertEquals(Optional.of(1), tree.search("apple"));
    }
}
