package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class HashTableTest {

    // ==================== HashTableChaining Tests ====================

    @Test
    void chainingPutAndGet() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        ht.put("c", 3);
        assertEquals(1, ht.get("a"));
        assertEquals(2, ht.get("b"));
        assertEquals(3, ht.get("c"));
    }

    @Test
    void chainingPutOverwrite() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("a", 10);
        assertEquals(10, ht.get("a"));
        assertEquals(1, ht.size());
    }

    @Test
    void chainingGetNonExistent() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        assertNull(ht.get("missing"));
    }

    @Test
    void chainingRemove() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        assertTrue(ht.remove("a"));
        assertNull(ht.get("a"));
        assertFalse(ht.remove("nonexistent"));
        assertEquals(1, ht.size());
    }

    @Test
    void chainingContainsKey() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("x", 42);
        assertTrue(ht.containsKey("x"));
        assertFalse(ht.containsKey("y"));
    }

    @Test
    void chainingKeys() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        List<String> keys = ht.keys();
        assertEquals(2, keys.size());
        assertTrue(keys.contains("a"));
        assertTrue(keys.contains("b"));
    }

    @Test
    void chainingValues() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        List<Integer> vals = ht.values();
        assertEquals(2, vals.size());
        assertTrue(vals.contains(1));
        assertTrue(vals.contains(2));
    }

    @Test
    void chainingItems() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        var items = ht.items();
        assertEquals(2, items.size());
    }

    @Test
    void chainingSizeAndEmpty() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        assertTrue(ht.isEmpty());
        assertEquals(0, ht.size());
        ht.put("a", 1);
        assertFalse(ht.isEmpty());
        assertEquals(1, ht.size());
    }

    @Test
    void chainingClear() {
        var ht = new HashTable.HashTableChaining<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        ht.clear();
        assertTrue(ht.isEmpty());
        assertEquals(0, ht.size());
        assertNull(ht.get("a"));
    }

    @Test
    void chainingLoadFactorAndCapacity() {
        var ht = new HashTable.HashTableChaining<String, Integer>(4);
        assertEquals(4, ht.capacity());
        assertEquals(0.0, ht.loadFactor(), 0.001);
        ht.put("a", 1);
        assertEquals(0.25, ht.loadFactor(), 0.001);
    }

    @Test
    void chainingAutoResize() {
        var ht = new HashTable.HashTableChaining<Integer, Integer>(4);
        // Load factor threshold is 0.75, so inserting 3 items into capacity 4 triggers resize
        for (int i = 0; i < 10; i++) {
            ht.put(i, i * 10);
        }
        assertEquals(10, ht.size());
        assertTrue(ht.capacity() > 4);
        // Verify all values still accessible
        for (int i = 0; i < 10; i++) {
            assertEquals(i * 10, ht.get(i));
        }
    }

    @Test
    void chainingManyEntries() {
        var ht = new HashTable.HashTableChaining<Integer, String>();
        for (int i = 0; i < 100; i++) {
            ht.put(i, "val" + i);
        }
        assertEquals(100, ht.size());
        for (int i = 0; i < 100; i++) {
            assertEquals("val" + i, ht.get(i));
        }
    }

    // ==================== HashTableOpenAddressing Tests ====================

    @Test
    void openAddressingPutAndGet() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        ht.put("c", 3);
        assertEquals(1, ht.get("a"));
        assertEquals(2, ht.get("b"));
        assertEquals(3, ht.get("c"));
    }

    @Test
    void openAddressingPutOverwrite() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("a", 10);
        assertEquals(10, ht.get("a"));
        assertEquals(1, ht.size());
    }

    @Test
    void openAddressingGetNonExistent() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        assertNull(ht.get("missing"));
    }

    @Test
    void openAddressingRemove() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        assertTrue(ht.remove("a"));
        assertNull(ht.get("a"));
        assertFalse(ht.remove("nonexistent"));
        assertEquals(1, ht.size());
    }

    @Test
    void openAddressingRemoveAndReinsert() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        ht.remove("a");
        // Should still find b even if a was tombstoned
        assertEquals(2, ht.get("b"));
        // Re-insert a
        ht.put("a", 100);
        assertEquals(100, ht.get("a"));
    }

    @Test
    void openAddressingContainsKey() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("x", 42);
        assertTrue(ht.containsKey("x"));
        assertFalse(ht.containsKey("y"));
    }

    @Test
    void openAddressingKeys() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        List<String> keys = ht.keys();
        assertEquals(2, keys.size());
        assertTrue(keys.contains("a"));
        assertTrue(keys.contains("b"));
    }

    @Test
    void openAddressingValues() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        List<Integer> vals = ht.values();
        assertEquals(2, vals.size());
        assertTrue(vals.contains(1));
        assertTrue(vals.contains(2));
    }

    @Test
    void openAddressingItems() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        var items = ht.items();
        assertEquals(2, items.size());
    }

    @Test
    void openAddressingSizeAndEmpty() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        assertTrue(ht.isEmpty());
        assertEquals(0, ht.size());
        ht.put("a", 1);
        assertFalse(ht.isEmpty());
        assertEquals(1, ht.size());
    }

    @Test
    void openAddressingClear() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>();
        ht.put("a", 1);
        ht.put("b", 2);
        ht.clear();
        assertTrue(ht.isEmpty());
        assertEquals(0, ht.size());
        assertNull(ht.get("a"));
    }

    @Test
    void openAddressingLoadFactorAndCapacity() {
        var ht = new HashTable.HashTableOpenAddressing<String, Integer>(8);
        assertEquals(8, ht.capacity());
        assertEquals(0.0, ht.loadFactor(), 0.001);
        ht.put("a", 1);
        assertEquals(0.125, ht.loadFactor(), 0.001);
    }

    @Test
    void openAddressingAutoResize() {
        var ht = new HashTable.HashTableOpenAddressing<Integer, Integer>(4);
        for (int i = 0; i < 10; i++) {
            ht.put(i, i * 10);
        }
        assertEquals(10, ht.size());
        assertTrue(ht.capacity() > 4);
        for (int i = 0; i < 10; i++) {
            assertEquals(i * 10, ht.get(i));
        }
    }

    @Test
    void openAddressingTombstoneHandling() {
        var ht = new HashTable.HashTableOpenAddressing<Integer, Integer>(16);
        // Insert and remove several items to create tombstones
        for (int i = 0; i < 8; i++) {
            ht.put(i, i);
        }
        for (int i = 0; i < 4; i++) {
            ht.remove(i);
        }
        assertEquals(4, ht.size());
        // Insert new items - should reuse tombstone slots
        for (int i = 10; i < 14; i++) {
            ht.put(i, i);
        }
        assertEquals(8, ht.size());
        for (int i = 4; i < 8; i++) {
            assertEquals(i, ht.get(i));
        }
        for (int i = 10; i < 14; i++) {
            assertEquals(i, ht.get(i));
        }
    }

    // ==================== HashSet Tests ====================

    @Test
    void hashSetAdd() {
        var set = new HashTable.HashSet<String>();
        set.add("a");
        set.add("b");
        set.add("a"); // duplicate
        assertEquals(2, set.size());
    }

    @Test
    void hashSetRemove() {
        var set = new HashTable.HashSet<String>();
        set.add("a");
        set.add("b");
        assertTrue(set.remove("a"));
        assertFalse(set.contains("a"));
        assertFalse(set.remove("nonexistent"));
    }

    @Test
    void hashSetContains() {
        var set = new HashTable.HashSet<Integer>();
        set.add(1);
        set.add(2);
        assertTrue(set.contains(1));
        assertTrue(set.contains(2));
        assertFalse(set.contains(3));
    }

    @Test
    void hashSetSizeAndEmpty() {
        var set = new HashTable.HashSet<String>();
        assertTrue(set.isEmpty());
        set.add("x");
        assertFalse(set.isEmpty());
        assertEquals(1, set.size());
    }

    @Test
    void hashSetClear() {
        var set = new HashTable.HashSet<String>();
        set.add("a");
        set.add("b");
        set.clear();
        assertTrue(set.isEmpty());
        assertFalse(set.contains("a"));
    }

    @Test
    void hashSetToList() {
        var set = new HashTable.HashSet<Integer>();
        set.add(1);
        set.add(2);
        set.add(3);
        List<Integer> list = set.toList();
        assertEquals(3, list.size());
        assertTrue(list.contains(1));
        assertTrue(list.contains(2));
        assertTrue(list.contains(3));
    }

    @Test
    void hashSetFromList() {
        var set = HashTable.HashSet.fromList(List.of(1, 2, 3, 2, 1));
        assertEquals(3, set.size());
        assertTrue(set.contains(1));
        assertTrue(set.contains(2));
        assertTrue(set.contains(3));
    }

    @Test
    void hashSetUnion() {
        var set1 = HashTable.HashSet.fromList(List.of(1, 2, 3));
        var set2 = HashTable.HashSet.fromList(List.of(3, 4, 5));
        var union = set1.union(set2);
        assertEquals(5, union.size());
        for (int i = 1; i <= 5; i++) {
            assertTrue(union.contains(i));
        }
    }

    @Test
    void hashSetIntersection() {
        var set1 = HashTable.HashSet.fromList(List.of(1, 2, 3, 4));
        var set2 = HashTable.HashSet.fromList(List.of(3, 4, 5, 6));
        var intersection = set1.intersection(set2);
        assertEquals(2, intersection.size());
        assertTrue(intersection.contains(3));
        assertTrue(intersection.contains(4));
        assertFalse(intersection.contains(1));
        assertFalse(intersection.contains(5));
    }

    @Test
    void hashSetDifference() {
        var set1 = HashTable.HashSet.fromList(List.of(1, 2, 3, 4));
        var set2 = HashTable.HashSet.fromList(List.of(3, 4, 5, 6));
        var diff = set1.difference(set2);
        assertEquals(2, diff.size());
        assertTrue(diff.contains(1));
        assertTrue(diff.contains(2));
        assertFalse(diff.contains(3));
    }

    @Test
    void hashSetEmptyOperations() {
        var set1 = new HashTable.HashSet<Integer>();
        var set2 = HashTable.HashSet.fromList(List.of(1, 2));
        var union = set1.union(set2);
        assertEquals(2, union.size());
        var intersection = set1.intersection(set2);
        assertEquals(0, intersection.size());
        var diff = set1.difference(set2);
        assertEquals(0, diff.size());
    }
}
