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

    // ======================== setWeight ========================

    @Test
    void setWeightUpdatesExistingEdge() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        g.addEdge(0, 1, 1.0);
        g.setWeight(0, 1, 5.0);
        assertEquals(5.0, g.getWeight(0, 1));
        assertEquals(5.0, g.getWeight(1, 0));
        assertEquals(1, g.numEdges());
    }

    @Test
    void setWeightDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, true);
        g.addEdge(0, 1, 2.0);
        g.setWeight(0, 1, 9.0);
        assertEquals(9.0, g.getWeight(0, 1));
        assertFalse(g.hasEdge(1, 0));
    }

    @Test
    void setWeightNonExistentThrows() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        assertThrows(IllegalArgumentException.class, () -> g.setWeight(0, 1, 5.0));
    }

    // ======================== bfs ========================

    @Test
    void bfsTraversal() {
        AdjacencyMatrix g = new AdjacencyMatrix(5, false);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(1, 3, 1.0);
        g.addEdge(2, 4, 1.0);
        List<Integer> order = g.bfs(0);
        assertEquals(5, order.size());
        assertEquals(0, order.get(0));
        // Level 1: 1, 2; Level 2: 3, 4
        assertTrue(order.indexOf(1) < order.indexOf(3));
        assertTrue(order.indexOf(2) < order.indexOf(4));
    }

    @Test
    void bfsDisconnected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        g.addEdge(0, 1, 1.0);
        // Vertices 2 and 3 are disconnected
        List<Integer> order = g.bfs(0);
        assertEquals(2, order.size());
        assertTrue(order.contains(0));
        assertTrue(order.contains(1));
    }

    // ======================== dfs ========================

    @Test
    void dfsTraversal() {
        AdjacencyMatrix g = new AdjacencyMatrix(5, false);
        g.addEdge(0, 1, 1.0);
        g.addEdge(0, 2, 1.0);
        g.addEdge(1, 3, 1.0);
        g.addEdge(2, 4, 1.0);
        List<Integer> order = g.dfs(0);
        assertEquals(5, order.size());
        assertEquals(0, order.get(0));
    }

    @Test
    void dfsDisconnected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, false);
        g.addEdge(0, 1, 1.0);
        List<Integer> order = g.dfs(0);
        assertEquals(2, order.size());
        assertTrue(order.contains(0));
        assertTrue(order.contains(1));
    }

    // ======================== floydWarshall ========================

    @Test
    void floydWarshallShortestPaths() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 3.0);
        g.addEdge(0, 2, 7.0);
        g.addEdge(1, 2, 1.0);
        g.addEdge(2, 3, 2.0);
        double[][] dist = g.floydWarshall();
        assertEquals(0.0, dist[0][0]);
        assertEquals(3.0, dist[0][1]);
        assertEquals(4.0, dist[0][2]); // 0->1->2
        assertEquals(6.0, dist[0][3]); // 0->1->2->3
        assertEquals(Double.POSITIVE_INFINITY, dist[3][0]);
    }

    // ======================== transitiveClosure ========================

    @Test
    void transitiveClosureDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(1, 2, 1.0);
        g.addEdge(2, 3, 1.0);
        boolean[][] tc = g.transitiveClosure();
        assertTrue(tc[0][3]); // path 0->1->2->3
        assertFalse(tc[3][0]); // no reverse path
        assertTrue(tc[0][0]); // self
    }

    // ======================== hasPath ========================

    @Test
    void hasPathExists() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(1, 2, 1.0);
        g.addEdge(2, 3, 1.0);
        assertTrue(g.hasPath(0, 3));
        assertTrue(g.hasPath(0, 0));
    }

    @Test
    void hasPathNotExists() {
        AdjacencyMatrix g = new AdjacencyMatrix(4, true);
        g.addEdge(0, 1, 1.0);
        g.addEdge(2, 3, 1.0);
        assertFalse(g.hasPath(0, 3));
        assertFalse(g.hasPath(3, 0));
    }

    // ======================== getEdges ========================

    @Test
    void getEdgesUndirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        g.addEdge(0, 1, 2.0);
        g.addEdge(1, 2, 3.0);
        List<AdjacencyMatrix.Edge> edges = g.getEdges();
        assertEquals(2, edges.size());
    }

    @Test
    void getEdgesDirected() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, true);
        g.addEdge(0, 1, 2.0);
        g.addEdge(1, 0, 3.0);
        List<AdjacencyMatrix.Edge> edges = g.getEdges();
        assertEquals(2, edges.size());
        // Verify edge data
        AdjacencyMatrix.Edge e = edges.get(0);
        assertEquals(0, e.from);
        assertEquals(1, e.to);
        assertEquals(2.0, e.weight);
    }

    @Test
    void getEdgesEmpty() {
        AdjacencyMatrix g = new AdjacencyMatrix(3, false);
        assertTrue(g.getEdges().isEmpty());
    }
}
