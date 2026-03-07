package dsa.datastructures;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class UnionFindTest {

    @Test
    void constructorInitializesCorrectly() {
        UnionFind uf = new UnionFind(5);
        assertEquals(5, uf.size());
        assertEquals(5, uf.getComponentCount());
        for (int i = 0; i < 5; i++) {
            assertEquals(i, uf.find(i));
            assertEquals(1, uf.getComponentSize(i));
        }
    }

    @Test
    void constructorRejectsNonPositive() {
        assertThrows(IllegalArgumentException.class, () -> new UnionFind(0));
        assertThrows(IllegalArgumentException.class, () -> new UnionFind(-1));
    }

    @Test
    void findWithPathCompression() {
        UnionFind uf = new UnionFind(10);
        uf.union(0, 1);
        uf.union(1, 2);
        uf.union(2, 3);
        // After find with path compression, all should point to same root
        int root = uf.find(3);
        assertEquals(root, uf.find(0));
        assertEquals(root, uf.find(1));
        assertEquals(root, uf.find(2));
    }

    @Test
    void findThrowsOnOutOfBounds() {
        UnionFind uf = new UnionFind(5);
        assertThrows(IndexOutOfBoundsException.class, () -> uf.find(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> uf.find(5));
    }

    @Test
    void unionReturnsTrueOnMerge() {
        UnionFind uf = new UnionFind(5);
        assertTrue(uf.union(0, 1));
        assertTrue(uf.union(2, 3));
        assertTrue(uf.union(1, 2));
    }

    @Test
    void unionReturnsFalseIfAlreadySameSet() {
        UnionFind uf = new UnionFind(5);
        uf.union(0, 1);
        assertFalse(uf.union(0, 1));
        assertFalse(uf.union(1, 0));
    }

    @Test
    void unionDecreasesComponentCount() {
        UnionFind uf = new UnionFind(4);
        assertEquals(4, uf.getComponentCount());
        uf.union(0, 1);
        assertEquals(3, uf.getComponentCount());
        uf.union(2, 3);
        assertEquals(2, uf.getComponentCount());
        uf.union(0, 2);
        assertEquals(1, uf.getComponentCount());
    }

    @Test
    void unionDoesNotDecreaseComponentCountOnRedundantUnion() {
        UnionFind uf = new UnionFind(3);
        uf.union(0, 1);
        assertEquals(2, uf.getComponentCount());
        uf.union(0, 1);
        assertEquals(2, uf.getComponentCount());
    }

    @Test
    void connectedBasic() {
        UnionFind uf = new UnionFind(6);
        assertFalse(uf.connected(0, 1));
        uf.union(0, 1);
        assertTrue(uf.connected(0, 1));
        assertTrue(uf.connected(1, 0));
        assertFalse(uf.connected(0, 2));
    }

    @Test
    void connectedTransitive() {
        UnionFind uf = new UnionFind(5);
        uf.union(0, 1);
        uf.union(1, 2);
        assertTrue(uf.connected(0, 2));
        uf.union(3, 4);
        assertFalse(uf.connected(0, 3));
        uf.union(2, 4);
        assertTrue(uf.connected(0, 3));
        assertTrue(uf.connected(1, 4));
    }

    @Test
    void getComponentSizeGrowsWithUnion() {
        UnionFind uf = new UnionFind(5);
        assertEquals(1, uf.getComponentSize(0));
        uf.union(0, 1);
        assertEquals(2, uf.getComponentSize(0));
        assertEquals(2, uf.getComponentSize(1));
        uf.union(2, 3);
        uf.union(0, 2);
        assertEquals(4, uf.getComponentSize(0));
        assertEquals(4, uf.getComponentSize(3));
        assertEquals(1, uf.getComponentSize(4));
    }

    @Test
    void singleElement() {
        UnionFind uf = new UnionFind(1);
        assertEquals(1, uf.size());
        assertEquals(1, uf.getComponentCount());
        assertEquals(0, uf.find(0));
        assertTrue(uf.connected(0, 0));
    }

    @Test
    void largeUnionFind() {
        int n = 1000;
        UnionFind uf = new UnionFind(n);
        // Union all even numbers together
        for (int i = 2; i < n; i += 2) {
            uf.union(0, i);
        }
        // Union all odd numbers together
        for (int i = 3; i < n; i += 2) {
            uf.union(1, i);
        }
        assertEquals(2, uf.getComponentCount());
        assertEquals(500, uf.getComponentSize(0));
        assertEquals(500, uf.getComponentSize(1));
        assertFalse(uf.connected(0, 1));
        // Now merge them
        uf.union(0, 1);
        assertEquals(1, uf.getComponentCount());
        assertEquals(1000, uf.getComponentSize(0));
    }
}
