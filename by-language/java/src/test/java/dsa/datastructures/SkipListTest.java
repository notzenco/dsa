package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

class SkipListTest {

    @Test
    void insertAndSearch() {
        var sl = new SkipList<Integer, String>();
        sl.insert(5, "five");
        sl.insert(3, "three");
        sl.insert(7, "seven");
        assertEquals(Optional.of("five"), sl.search(5));
        assertEquals(Optional.of("three"), sl.search(3));
        assertEquals(Optional.of("seven"), sl.search(7));
    }

    @Test
    void searchNonExistent() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        assertEquals(Optional.empty(), sl.search(99));
    }

    @Test
    void insertOverwrite() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        sl.insert(1, "ONE");
        assertEquals(Optional.of("ONE"), sl.search(1));
        assertEquals(1, sl.size());
    }

    @Test
    void delete() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        sl.insert(2, "two");
        sl.insert(3, "three");
        assertTrue(sl.delete(2));
        assertEquals(Optional.empty(), sl.search(2));
        assertEquals(2, sl.size());
    }

    @Test
    void deleteNonExistent() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        assertFalse(sl.delete(99));
        assertEquals(1, sl.size());
    }

    @Test
    void deleteAll() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        sl.insert(2, "two");
        assertTrue(sl.delete(1));
        assertTrue(sl.delete(2));
        assertTrue(sl.isEmpty());
        assertEquals(0, sl.size());
    }

    @Test
    void sizeAndEmpty() {
        var sl = new SkipList<Integer, String>();
        assertTrue(sl.isEmpty());
        assertEquals(0, sl.size());
        sl.insert(1, "one");
        assertFalse(sl.isEmpty());
        assertEquals(1, sl.size());
        sl.insert(2, "two");
        assertEquals(2, sl.size());
    }

    @Test
    void clear() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        sl.insert(2, "two");
        sl.insert(3, "three");
        sl.clear();
        assertTrue(sl.isEmpty());
        assertEquals(0, sl.size());
        assertEquals(Optional.empty(), sl.search(1));
    }

    @Test
    void min() {
        var sl = new SkipList<Integer, String>();
        assertTrue(sl.min().isEmpty());
        sl.insert(5, "five");
        sl.insert(3, "three");
        sl.insert(7, "seven");
        sl.insert(1, "one");
        var min = sl.min();
        assertTrue(min.isPresent());
        assertEquals(1, min.get().getKey());
        assertEquals("one", min.get().getValue());
    }

    @Test
    void max() {
        var sl = new SkipList<Integer, String>();
        assertTrue(sl.max().isEmpty());
        sl.insert(5, "five");
        sl.insert(3, "three");
        sl.insert(7, "seven");
        sl.insert(1, "one");
        var max = sl.max();
        assertTrue(max.isPresent());
        assertEquals(7, max.get().getKey());
        assertEquals("seven", max.get().getValue());
    }

    @Test
    void rangeQuery() {
        var sl = new SkipList<Integer, String>();
        for (int i = 1; i <= 10; i++) {
            sl.insert(i, "val" + i);
        }
        var range = sl.range(3, 7);
        assertEquals(5, range.size());
        assertEquals(3, range.get(0).getKey());
        assertEquals(4, range.get(1).getKey());
        assertEquals(5, range.get(2).getKey());
        assertEquals(6, range.get(3).getKey());
        assertEquals(7, range.get(4).getKey());
    }

    @Test
    void rangeQueryEmpty() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "one");
        sl.insert(10, "ten");
        var range = sl.range(3, 7);
        assertEquals(0, range.size());
    }

    @Test
    void rangeQuerySingleElement() {
        var sl = new SkipList<Integer, String>();
        sl.insert(5, "five");
        var range = sl.range(5, 5);
        assertEquals(1, range.size());
        assertEquals(5, range.get(0).getKey());
    }

    @Test
    void floor() {
        var sl = new SkipList<Integer, String>();
        sl.insert(2, "two");
        sl.insert(4, "four");
        sl.insert(6, "six");
        sl.insert(8, "eight");

        // Exact match
        var floor4 = sl.floor(4);
        assertTrue(floor4.isPresent());
        assertEquals(4, floor4.get().getKey());

        // Between elements
        var floor5 = sl.floor(5);
        assertTrue(floor5.isPresent());
        assertEquals(4, floor5.get().getKey());

        // Greater than all
        var floor10 = sl.floor(10);
        assertTrue(floor10.isPresent());
        assertEquals(8, floor10.get().getKey());

        // Less than all
        var floor1 = sl.floor(1);
        assertTrue(floor1.isEmpty());
    }

    @Test
    void ceiling() {
        var sl = new SkipList<Integer, String>();
        sl.insert(2, "two");
        sl.insert(4, "four");
        sl.insert(6, "six");
        sl.insert(8, "eight");

        // Exact match
        var ceil4 = sl.ceiling(4);
        assertTrue(ceil4.isPresent());
        assertEquals(4, ceil4.get().getKey());

        // Between elements
        var ceil5 = sl.ceiling(5);
        assertTrue(ceil5.isPresent());
        assertEquals(6, ceil5.get().getKey());

        // Less than all
        var ceil1 = sl.ceiling(1);
        assertTrue(ceil1.isPresent());
        assertEquals(2, ceil1.get().getKey());

        // Greater than all
        var ceil10 = sl.ceiling(10);
        assertTrue(ceil10.isEmpty());
    }

    @Test
    void manyInsertions() {
        var sl = new SkipList<Integer, Integer>();
        for (int i = 0; i < 1000; i++) {
            sl.insert(i, i * 10);
        }
        assertEquals(1000, sl.size());
        for (int i = 0; i < 1000; i++) {
            assertEquals(Optional.of(i * 10), sl.search(i));
        }
    }

    @Test
    void manyDeletions() {
        var sl = new SkipList<Integer, Integer>();
        for (int i = 0; i < 100; i++) {
            sl.insert(i, i);
        }
        for (int i = 0; i < 50; i++) {
            assertTrue(sl.delete(i));
        }
        assertEquals(50, sl.size());
        for (int i = 0; i < 50; i++) {
            assertEquals(Optional.empty(), sl.search(i));
        }
        for (int i = 50; i < 100; i++) {
            assertEquals(Optional.of(i), sl.search(i));
        }
    }

    @Test
    void withStringKeys() {
        var sl = new SkipList<String, Integer>();
        sl.insert("banana", 2);
        sl.insert("apple", 1);
        sl.insert("cherry", 3);
        sl.insert("date", 4);

        assertEquals(Optional.of(1), sl.search("apple"));
        assertEquals(Optional.of(3), sl.search("cherry"));

        var min = sl.min();
        assertTrue(min.isPresent());
        assertEquals("apple", min.get().getKey());

        var range = sl.range("banana", "date");
        assertEquals(3, range.size());
    }

    @Test
    void insertDeleteInsert() {
        var sl = new SkipList<Integer, String>();
        sl.insert(1, "first");
        sl.delete(1);
        sl.insert(1, "second");
        assertEquals(Optional.of("second"), sl.search(1));
        assertEquals(1, sl.size());
    }
}
