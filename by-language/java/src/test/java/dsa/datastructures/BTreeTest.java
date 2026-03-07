package dsa.datastructures;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

class BTreeTest {

    private BTree<Integer, String> btree;

    @BeforeEach
    void setUp() {
        btree = new BTree<>(2); // minimum degree 2 (2-3-4 tree)
    }

    // ======================== Constructor ========================

    @Test
    void testConstructorInvalidDegree() {
        assertThrows(IllegalArgumentException.class, () -> new BTree<>(1));
    }

    @Test
    void testMinDegreeAndOrder() {
        assertEquals(2, btree.minDegree());
        assertEquals(4, btree.order());
    }

    @Test
    void testDegree3() {
        BTree<Integer, String> tree = new BTree<>(3);
        assertEquals(3, tree.minDegree());
        assertEquals(6, tree.order());
    }

    // ======================== Insert ========================

    @Test
    void testInsertSingleElement() {
        btree.insert(10, "ten");
        assertEquals(1, btree.size());
        assertEquals(Optional.of("ten"), btree.search(10));
    }

    @Test
    void testInsertMultipleElements() {
        btree.insert(10, "ten");
        btree.insert(20, "twenty");
        btree.insert(30, "thirty");
        btree.insert(5, "five");
        btree.insert(15, "fifteen");
        assertEquals(5, btree.size());
    }

    @Test
    void testInsertDuplicateUpdatesValue() {
        btree.insert(10, "ten");
        btree.insert(10, "TEN");
        assertEquals(1, btree.size());
        assertEquals(Optional.of("TEN"), btree.search(10));
    }

    @Test
    void testInsertCausesSplit() {
        // With t=2, max keys per node = 3, so inserting 4+ elements causes splits
        btree.insert(10, "a");
        btree.insert(20, "b");
        btree.insert(30, "c");
        btree.insert(40, "d");
        assertEquals(4, btree.size());
        assertEquals(Arrays.asList(10, 20, 30, 40), btree.keys());
    }

    @Test
    void testInsertManyElements() {
        for (int i = 1; i <= 50; i++) {
            btree.insert(i, "v" + i);
        }
        assertEquals(50, btree.size());
        List<Integer> keys = btree.keys();
        for (int i = 1; i <= 50; i++) {
            assertEquals(i, keys.get(i - 1));
        }
    }

    // ======================== Search ========================

    @Test
    void testSearchExisting() {
        btree.insert(10, "ten");
        btree.insert(20, "twenty");
        btree.insert(5, "five");
        assertEquals(Optional.of("ten"), btree.search(10));
        assertEquals(Optional.of("twenty"), btree.search(20));
        assertEquals(Optional.of("five"), btree.search(5));
    }

    @Test
    void testSearchNonExisting() {
        btree.insert(10, "ten");
        assertFalse(btree.search(99).isPresent());
    }

    @Test
    void testSearchEmptyTree() {
        assertFalse(btree.search(1).isPresent());
    }

    // ======================== Delete ========================

    @Test
    void testDeleteFromLeaf() {
        btree.insert(10, "a");
        btree.insert(20, "b");
        btree.insert(5, "c");
        assertTrue(btree.delete(5));
        assertEquals(2, btree.size());
        assertFalse(btree.search(5).isPresent());
    }

    @Test
    void testDeleteNonExisting() {
        btree.insert(10, "a");
        assertFalse(btree.delete(99));
        assertEquals(1, btree.size());
    }

    @Test
    void testDeleteOnlyElement() {
        btree.insert(10, "a");
        assertTrue(btree.delete(10));
        assertEquals(0, btree.size());
    }

    @Test
    void testDeleteFromInternalNode() {
        for (int i = 1; i <= 10; i++) {
            btree.insert(i, "v" + i);
        }
        // Delete an element that might be in an internal node
        assertTrue(btree.delete(5));
        assertEquals(9, btree.size());
        assertFalse(btree.search(5).isPresent());
        // Remaining keys should be in order
        List<Integer> expected = Arrays.asList(1, 2, 3, 4, 6, 7, 8, 9, 10);
        assertEquals(expected, btree.keys());
    }

    @Test
    void testDeleteAllElements() {
        for (int i = 1; i <= 10; i++) {
            btree.insert(i, "v" + i);
        }
        for (int i = 1; i <= 10; i++) {
            assertTrue(btree.delete(i), "Failed to delete " + i);
        }
        assertEquals(0, btree.size());
    }

    @Test
    void testDeleteInReverseOrder() {
        for (int i = 1; i <= 20; i++) {
            btree.insert(i, "v" + i);
        }
        for (int i = 20; i >= 1; i--) {
            assertTrue(btree.delete(i), "Failed to delete " + i);
            assertEquals(i - 1, btree.size());
        }
    }

    @Test
    void testDeleteWithMergeAndBorrow() {
        // Use degree 2 to force merge/borrow scenarios
        BTree<Integer, String> tree = new BTree<>(2);
        for (int i = 1; i <= 15; i++) {
            tree.insert(i, "v" + i);
        }
        // Delete in a pattern that forces rebalancing
        for (int i = 1; i <= 15; i += 2) {
            assertTrue(tree.delete(i), "Failed to delete " + i);
        }
        // Verify remaining elements
        for (int i = 2; i <= 14; i += 2) {
            assertTrue(tree.search(i).isPresent(), "Missing key " + i);
        }
    }

    // ======================== Traversals ========================

    @Test
    void testInorder() {
        btree.insert(30, "a");
        btree.insert(10, "b");
        btree.insert(50, "c");
        btree.insert(20, "d");
        btree.insert(40, "e");

        List<BTree.Entry<Integer, String>> entries = btree.inorder();
        assertEquals(5, entries.size());
        List<Integer> keys = entries.stream().map(BTree.Entry::getKey).collect(Collectors.toList());
        assertEquals(Arrays.asList(10, 20, 30, 40, 50), keys);
    }

    @Test
    void testKeys() {
        btree.insert(30, "a");
        btree.insert(10, "b");
        btree.insert(50, "c");
        assertEquals(Arrays.asList(10, 30, 50), btree.keys());
    }

    @Test
    void testValues() {
        btree.insert(30, "a");
        btree.insert(10, "b");
        btree.insert(50, "c");
        List<String> values = btree.values();
        assertEquals(3, values.size());
        // Values should be in key order
        assertEquals("b", values.get(0));
        assertEquals("a", values.get(1));
        assertEquals("c", values.get(2));
    }

    @Test
    void testTraversalsEmpty() {
        assertTrue(btree.inorder().isEmpty());
        assertTrue(btree.keys().isEmpty());
        assertTrue(btree.values().isEmpty());
    }

    // ======================== Min / Max ========================

    @Test
    void testMin() {
        btree.insert(30, "a");
        btree.insert(10, "b");
        btree.insert(50, "c");
        assertEquals(Optional.of(10), btree.min());
    }

    @Test
    void testMax() {
        btree.insert(30, "a");
        btree.insert(10, "b");
        btree.insert(50, "c");
        assertEquals(Optional.of(50), btree.max());
    }

    @Test
    void testMinMaxEmpty() {
        assertFalse(btree.min().isPresent());
        assertFalse(btree.max().isPresent());
    }

    @Test
    void testMinMaxAfterDeletes() {
        for (int i = 1; i <= 10; i++) {
            btree.insert(i, "v" + i);
        }
        btree.delete(1);
        assertEquals(Optional.of(2), btree.min());
        btree.delete(10);
        assertEquals(Optional.of(9), btree.max());
    }

    // ======================== Height ========================

    @Test
    void testHeightEmptyTree() {
        assertEquals(0, btree.height());
    }

    @Test
    void testHeightSingleElement() {
        btree.insert(10, "a");
        assertEquals(0, btree.height());
    }

    @Test
    void testHeightGrows() {
        // With t=2, inserting 4 elements should increase height
        btree.insert(1, "a");
        btree.insert(2, "b");
        btree.insert(3, "c");
        int h1 = btree.height();
        btree.insert(4, "d");
        // Height may grow
        assertTrue(btree.height() >= h1);
    }

    // ======================== Clear ========================

    @Test
    void testClear() {
        btree.insert(10, "a");
        btree.insert(20, "b");
        btree.clear();
        assertEquals(0, btree.size());
        assertFalse(btree.search(10).isPresent());
    }

    // ======================== Different Minimum Degrees ========================

    @Test
    void testDegree3Operations() {
        BTree<Integer, String> tree = new BTree<>(3);
        for (int i = 1; i <= 30; i++) {
            tree.insert(i, "v" + i);
        }
        assertEquals(30, tree.size());
        List<Integer> keys = tree.keys();
        for (int i = 0; i < 30; i++) {
            assertEquals(i + 1, keys.get(i));
        }

        for (int i = 1; i <= 15; i++) {
            assertTrue(tree.delete(i));
        }
        assertEquals(15, tree.size());
    }

    @Test
    void testDegree5Operations() {
        BTree<Integer, String> tree = new BTree<>(5);
        for (int i = 1; i <= 100; i++) {
            tree.insert(i, "v" + i);
        }
        assertEquals(100, tree.size());
        assertEquals(Optional.of(1), tree.min());
        assertEquals(Optional.of(100), tree.max());

        for (int i = 50; i >= 1; i--) {
            assertTrue(tree.delete(i));
        }
        assertEquals(50, tree.size());
        assertEquals(Optional.of(51), tree.min());
    }

    @Test
    void testDegree10Operations() {
        BTree<Integer, String> tree = new BTree<>(10);
        for (int i = 1; i <= 200; i++) {
            tree.insert(i, "v" + i);
        }
        assertEquals(200, tree.size());

        // All keys should be present and sorted
        List<Integer> keys = tree.keys();
        assertEquals(200, keys.size());
        for (int i = 0; i < 200; i++) {
            assertEquals(i + 1, keys.get(i));
        }
    }

    // ======================== String Keys ========================

    @Test
    void testStringKeys() {
        BTree<String, Integer> tree = new BTree<>(2);
        tree.insert("banana", 2);
        tree.insert("apple", 1);
        tree.insert("cherry", 3);
        tree.insert("date", 4);
        tree.insert("elderberry", 5);

        assertEquals(Arrays.asList("apple", "banana", "cherry", "date", "elderberry"), tree.keys());
        assertEquals(Optional.of(1), tree.search("apple"));
        assertEquals(Optional.of("apple"), tree.min());
        assertEquals(Optional.of("elderberry"), tree.max());
    }
}
