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

    // --- WeightedUnionFind tests ---

    @Test
    void weightedConstruction() {
        var wuf = new UnionFind.WeightedUnionFind(5);
        assertEquals(5, wuf.size());
        assertEquals(5, wuf.getComponentCount());
        for (int i = 0; i < 5; i++) {
            assertEquals(i, wuf.find(i));
        }
    }

    @Test
    void weightedConstructorRejectsNonPositive() {
        assertThrows(IllegalArgumentException.class,
                () -> new UnionFind.WeightedUnionFind(0));
        assertThrows(IllegalArgumentException.class,
                () -> new UnionFind.WeightedUnionFind(-1));
    }

    @Test
    void weightedUnionAndGetWeight() {
        var wuf = new UnionFind.WeightedUnionFind(4);
        // weight(0) - weight(1) = 3.0
        wuf.union(0, 1, 3.0);
        assertTrue(wuf.connected(0, 1));
        assertEquals(3.0, wuf.getWeight(0, 1), 1e-9);
        assertEquals(-3.0, wuf.getWeight(1, 0), 1e-9);
    }

    @Test
    void weightedTransitiveWeights() {
        var wuf = new UnionFind.WeightedUnionFind(4);
        // w(0) - w(1) = 2.0
        // w(1) - w(2) = 5.0
        wuf.union(0, 1, 2.0);
        wuf.union(1, 2, 5.0);
        // w(0) - w(2) should be 7.0
        assertEquals(7.0, wuf.getWeight(0, 2), 1e-9);
        assertEquals(-7.0, wuf.getWeight(2, 0), 1e-9);
        assertEquals(2.0, wuf.getWeight(0, 1), 1e-9);
    }

    @Test
    void weightedPathCompressionPreservesWeights() {
        var wuf = new UnionFind.WeightedUnionFind(5);
        wuf.union(0, 1, 1.0);
        wuf.union(1, 2, 2.0);
        wuf.union(2, 3, 3.0);
        wuf.union(3, 4, 4.0);
        // After path compression via find, weights should remain correct
        wuf.find(4);
        assertEquals(10.0, wuf.getWeight(0, 4), 1e-9);
        assertEquals(1.0, wuf.getWeight(0, 1), 1e-9);
        assertEquals(2.0, wuf.getWeight(1, 2), 1e-9);
    }

    @Test
    void weightedDisconnectedThrows() {
        var wuf = new UnionFind.WeightedUnionFind(4);
        wuf.union(0, 1, 1.0);
        assertThrows(IllegalArgumentException.class,
                () -> wuf.getWeight(0, 2));
        assertThrows(IllegalArgumentException.class,
                () -> wuf.getWeight(1, 3));
    }

    @Test
    void weightedUnionReturnsFalseIfAlreadyConnected() {
        var wuf = new UnionFind.WeightedUnionFind(3);
        assertTrue(wuf.union(0, 1, 1.0));
        assertFalse(wuf.union(0, 1, 2.0));
    }

    @Test
    void weightedComponentCount() {
        var wuf = new UnionFind.WeightedUnionFind(4);
        assertEquals(4, wuf.getComponentCount());
        wuf.union(0, 1, 1.0);
        assertEquals(3, wuf.getComponentCount());
        wuf.union(2, 3, 2.0);
        assertEquals(2, wuf.getComponentCount());
        wuf.union(0, 2, 3.0);
        assertEquals(1, wuf.getComponentCount());
    }

    @Test
    void weightedSelfWeight() {
        var wuf = new UnionFind.WeightedUnionFind(3);
        wuf.union(0, 1, 5.0);
        assertEquals(0.0, wuf.getWeight(0, 0), 1e-9);
        assertEquals(0.0, wuf.getWeight(1, 1), 1e-9);
    }

    @Test
    void weightedNegativeWeights() {
        var wuf = new UnionFind.WeightedUnionFind(3);
        wuf.union(0, 1, -2.5);
        wuf.union(1, 2, 3.5);
        assertEquals(-2.5, wuf.getWeight(0, 1), 1e-9);
        assertEquals(1.0, wuf.getWeight(0, 2), 1e-9);
    }

    @Test
    void weightedFindOutOfBounds() {
        var wuf = new UnionFind.WeightedUnionFind(3);
        assertThrows(IndexOutOfBoundsException.class, () -> wuf.find(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> wuf.find(3));
    }

    // --- numIslands tests ---

    @Test
    void numIslandsEmpty() {
        assertEquals(0, UnionFind.numIslands(new char[0][0]));
        assertEquals(0, UnionFind.numIslands(null));
    }

    @Test
    void numIslandsSingleIsland() {
        char[][] grid = {
                {'1', '1', '1'},
                {'1', '1', '1'}
        };
        assertEquals(1, UnionFind.numIslands(grid));
    }

    @Test
    void numIslandsMultipleIslands() {
        char[][] grid = {
                {'1', '1', '0', '0', '0'},
                {'1', '1', '0', '0', '0'},
                {'0', '0', '1', '0', '0'},
                {'0', '0', '0', '1', '1'}
        };
        assertEquals(3, UnionFind.numIslands(grid));
    }

    @Test
    void numIslandsAllWater() {
        char[][] grid = {
                {'0', '0'},
                {'0', '0'}
        };
        assertEquals(0, UnionFind.numIslands(grid));
    }

    @Test
    void numIslandsAllLand() {
        char[][] grid = {
                {'1', '1'},
                {'1', '1'}
        };
        assertEquals(1, UnionFind.numIslands(grid));
    }

    @Test
    void numIslandsSingleCell() {
        assertEquals(1, UnionFind.numIslands(new char[][]{{'1'}}));
        assertEquals(0, UnionFind.numIslands(new char[][]{{'0'}}));
    }

    @Test
    void numIslandsDiagonalNotConnected() {
        char[][] grid = {
                {'1', '0'},
                {'0', '1'}
        };
        // Diagonal cells are not adjacent
        assertEquals(2, UnionFind.numIslands(grid));
    }

    // --- findRedundantConnection tests ---

    @Test
    void findRedundantConnectionTriangle() {
        int[][] edges = {{1, 2}, {1, 3}, {2, 3}};
        int[] result = UnionFind.findRedundantConnection(edges);
        assertArrayEquals(new int[]{2, 3}, result);
    }

    @Test
    void findRedundantConnectionLargerGraph() {
        int[][] edges = {{1, 2}, {2, 3}, {3, 4}, {1, 4}, {1, 5}};
        int[] result = UnionFind.findRedundantConnection(edges);
        assertArrayEquals(new int[]{1, 4}, result);
    }

    @Test
    void findRedundantConnectionSingleExtraEdge() {
        int[][] edges = {{1, 2}, {1, 3}, {2, 3}};
        int[] result = UnionFind.findRedundantConnection(edges);
        // The last edge that forms a cycle
        assertArrayEquals(new int[]{2, 3}, result);
    }

    @Test
    void findRedundantConnectionFourNodes() {
        int[][] edges = {{1, 2}, {2, 3}, {3, 4}, {4, 2}};
        int[] result = UnionFind.findRedundantConnection(edges);
        assertArrayEquals(new int[]{4, 2}, result);
    }
}
