package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class AdjacencyMatrixTest {

    @Test
    void constructorInitializesCorrectly() {
        AdjacencyMatrix g = new AdjacencyMatrix(5, false);
        assertEquals(5, g.numVertices());
        assertEquals(0, g.numEdges());
        assertFalse(g.isDirected());
    }

    @Test
    void constructorDirectedGraph() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, true);
        assertTrue(g.isDirected());
    }

    @Test
    void constructorRejectsNonPositive() {
        assertThrows(IllegalArgumentException.class, () -> new AdjacencyMatrix(0, false));
        assertThrows(IllegalArgumentException.class, () -> new AdjacencyMatrix(-1, true));
    }

    @Test
    void addEdgeUndirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        g.addEdge(0, 1, 1.0);
        assertEquals(1, g.numEdges());
        assertTrue(g.hasEdge(0, 1));
        assertTrue(g.hasEdge(1, 0));
        assertEquals(1.0, g.getWeight(0, 1));
        assertEquals(1.0, g.getWeight(1, 0));
    }

    @Test
    void addEdgeDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 2.5);
        assertEquals(1, g.numEdges());
        assertTrue(g.hasEdge(0, 1));
        assertFalse(g.hasEdge(1, 0));
        assertEquals(2.5, g.getWeight(0, 1));
    }

    @Test
    void addEdgeUpdatesWeight() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        g.addEdge(0, 1, 1.0);
        assertEquals(1, g.numEdges());
        g.addEdge(0, 1, 5.0);
        assertEquals(1, g.numEdges()); // same edge, count unchanged
        assertEquals(5.0, g.getWeight(0, 1));
    }

    @Test
    void addEdgeOutOfBoundsThrows() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        assertThrows(IndexOutOfBoundsException.class, () -> g.addEdge(-1, 0, 1.0));
        assertThrows(IndexOutOfBoundsException.class, () -> g.addEdge(0, 3, 1.0));
    }

    @Test
    void removeEdgeUndirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        g.addEdge(0, 1, 1.0);
        g.addEdge(1, 2, 2.0);
        assertEquals(2, g.numEdges());
        g.removeEdge(0, 1);
        assertEquals(1, g.numEdges());
        assertFalse(g.hasEdge(0, 1));
        assertFalse(g.hasEdge(1, 0));
        assertTrue(g.hasEdge(1, 2));
    }

    @Test
    void removeEdgeDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(1, 0, 2.0);
        assertEquals(2, g.numEdges());
        g.removeEdge(0, 1);
        assertEquals(1, g.numEdges());
        assertFalse(g.hasEdge(0, 1));
        assertTrue(g.hasEdge(1, 0));
    }

    @Test
    void removeNonExistentEdgeThrows() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        assertThrows(IllegalArgumentException.class, () -> g.removeEdge(0, 1));
    }

    @Test
    void hasEdge() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, true);
        assertFalse(g.hasEdge(0, 1));
        g.addEdge(0, 1, 1.0);
        assertTrue(g.hasEdge(0, 1));
        assertFalse(g.hasEdge(1, 0));
    }

    @Test
    void getWeightNonExistentThrows() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        assertThrows(IllegalArgumentException.class, () -> g.getWeight(0, 1));
    }

    @Test
    void getNeighborsUndirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(5, false);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 3, 1.0);
        g.addEdge(0, 4, 1.0);
        List<Integer> neighbors = g.getNeighbors(0);
        assertEquals(3, neighbors.size());
        assertTrue(neighbors.contains(1));
        assertTrue(neighbors.contains(3));
        assertTrue(neighbors.contains(4));
    }

    @Test
    void getNeighborsDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(3, 0, 1.0);
        List<Integer> neighbors = g.getNeighbors(0);
        assertEquals(2, neighbors.size());
        assertTrue(neighbors.contains(1));
        assertTrue(neighbors.contains(2));
        assertFalse(neighbors.contains(3));
    }

    @Test
    void getNeighborsNoNeighbors() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        List<Integer> neighbors = g.getNeighbors(1);
        assertTrue(neighbors.isEmpty());
    }

    @Test
    void getInDegreeDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 2, 1.0);
        g.addEdge(1, 2, 1.0);
        g.addEdge(3, 2, 1.0);
        assertEquals(3, g.getInDegree(2));
        assertEquals(0, g.getInDegree(0));
    }

    @Test
    void getOutDegreeDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(0, 3, 1.0);
        assertEquals(3, g.getOutDegree(0));
        assertEquals(0, g.getOutDegree(1));
    }

    @Test
    void getDegreeUndirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(0, 3, 1.0);
        assertEquals(3, g.getDegree(0));
        assertEquals(1, g.getDegree(1));
    }

    @Test
    void getDegreeDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(2, 0, 1.0);
        // in-degree of 0 = 1 (from 2), out-degree of 0 = 1 (to 1)
        assertEquals(2, g.getDegree(0));
    }

    @Test
    void numEdgesTracking() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        assertEquals(0, g.numEdges());
        g.addEdge(0, 1, 1.0);
        assertEquals(1, g.numEdges());
        g.addEdge(1, 2, 1.0);
        assertEquals(2, g.numEdges());
        g.addEdge(2, 3, 1.0);
        assertEquals(3, g.numEdges());
        g.removeEdge(1, 2);
        assertEquals(2, g.numEdges());
    }

    @Test
    void selfLoop() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, true);
        g.addEdge(1, 1, 5.0);
        assertTrue(g.hasEdge(1, 1));
        assertEquals(5.0, g.getWeight(1, 1));
        assertEquals(1, g.numEdges());
    }

    @Test
    void weightedEdges() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        g.addEdge(0, 1, 3.14);
        g.addEdge(1, 2, 2.71);
        assertEquals(3.14, g.getWeight(0, 1), 1e-9);
        assertEquals(2.71, g.getWeight(1, 2), 1e-9);
    }

    @Test
    void completeGraph() {
        int n = 5;
        AdjacencyMatrix g = new AdjacencyMatrix(n, false);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                g.addEdge(i, j, 1.0);
            }
        }
        assertEquals(10, g.numEdges()); // C(5,2) = 10
        for (int i = 0; i < n; i++) {
            assertEquals(4, g.getDegree(i));
        }
    }
}
