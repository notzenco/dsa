package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Map;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;

class GraphCoreTest {

    @Test
    void addVertexAndHasVertex() {
        GraphCore g = new GraphCore(false);
        assertFalse(g.hasVertex("A"));
        g.addVertex("A");
        assertTrue(g.hasVertex("A"));
        assertFalse(g.hasVertex("B"));
    }

    @Test
    void addVertexDuplicate() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("A"); // no error
        assertEquals(1, g.numVertices());
    }

    @Test
    void addVertexNullThrows() {
        GraphCore g = new GraphCore(false);
        assertThrows(IllegalArgumentException.class, () -> g.addVertex(null));
    }

    @Test
    void addEdgeUndirected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        assertTrue(g.hasEdge("A", "B"));
        assertTrue(g.hasEdge("B", "A"));
        assertEquals(1, g.numEdges());
        assertEquals(2, g.numVertices());
    }

    @Test
    void addEdgeDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 2.0);
        assertTrue(g.hasEdge("A", "B"));
        assertFalse(g.hasEdge("B", "A"));
        assertEquals(1, g.numEdges());
    }

    @Test
    void addEdgeCreatesVertices() {
        GraphCore g = new GraphCore(false);
        assertFalse(g.hasVertex("X"));
        assertFalse(g.hasVertex("Y"));
        g.addEdge("X", "Y", 1.0);
        assertTrue(g.hasVertex("X"));
        assertTrue(g.hasVertex("Y"));
    }

    @Test
    void addEdgeUpdatesWeight() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        assertEquals(1.0, g.getWeight("A", "B"));
        g.addEdge("A", "B", 5.0);
        assertEquals(5.0, g.getWeight("A", "B"));
    }

    @Test
    void removeEdgeUndirected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "C", 2.0);
        g.removeEdge("A", "B");
        assertFalse(g.hasEdge("A", "B"));
        assertFalse(g.hasEdge("B", "A"));
        assertTrue(g.hasEdge("B", "C"));
        assertEquals(1, g.numEdges());
    }

    @Test
    void removeEdgeDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "A", 2.0);
        g.removeEdge("A", "B");
        assertFalse(g.hasEdge("A", "B"));
        assertTrue(g.hasEdge("B", "A"));
        assertEquals(1, g.numEdges());
    }

    @Test
    void removeNonExistentEdgeThrows() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("B");
        assertThrows(IllegalArgumentException.class, () -> g.removeEdge("A", "B"));
    }

    @Test
    void removeVertex() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "C", 2.0);
        g.addEdge("A", "C", 3.0);
        g.removeVertex("B");
        assertFalse(g.hasVertex("B"));
        assertFalse(g.hasEdge("A", "B"));
        assertFalse(g.hasEdge("B", "C"));
        assertTrue(g.hasEdge("A", "C"));
        assertEquals(2, g.numVertices());
        assertEquals(1, g.numEdges());
    }

    @Test
    void removeVertexDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1.0);
        g.addEdge("C", "B", 2.0);
        g.addEdge("B", "C", 3.0);
        g.removeVertex("B");
        assertFalse(g.hasVertex("B"));
        assertFalse(g.hasEdge("A", "B"));
        assertFalse(g.hasEdge("C", "B"));
        assertEquals(2, g.numVertices());
        assertEquals(0, g.numEdges());
    }

    @Test
    void removeNonExistentVertexThrows() {
        GraphCore g = new GraphCore(false);
        assertThrows(IllegalArgumentException.class, () -> g.removeVertex("X"));
    }

    @Test
    void hasEdgeReturnsFalseForNonExistent() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        assertFalse(g.hasEdge("A", "B"));
        assertFalse(g.hasEdge(null, "A"));
    }

    @Test
    void getWeight() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 3.14);
        assertEquals(3.14, g.getWeight("A", "B"), 1e-9);
        assertEquals(3.14, g.getWeight("B", "A"), 1e-9);
    }

    @Test
    void getWeightNonExistentThrows() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("B");
        assertThrows(IllegalArgumentException.class, () -> g.getWeight("A", "B"));
    }

    @Test
    void getNeighborEntries() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        g.addEdge("A", "C", 2.0);
        g.addEdge("A", "D", 3.0);
        List<Map.Entry<String, Double>> neighbors = g.getNeighborEntries("A");
        assertEquals(3, neighbors.size());
        Set<String> neighborLabels = Set.of("B", "C", "D");
        for (Map.Entry<String, Double> entry : neighbors) {
            assertTrue(neighborLabels.contains(entry.getKey()));
        }
    }

    @Test
    void getNeighborMap() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1.0);
        g.addEdge("A", "C", 2.0);
        Map<String, Double> map = g.getNeighborMap("A");
        assertEquals(2, map.size());
        assertEquals(1.0, map.get("B"));
        assertEquals(2.0, map.get("C"));
    }

    @Test
    void getNeighborMapNonExistentVertexThrows() {
        GraphCore g = new GraphCore(false);
        assertThrows(IllegalArgumentException.class, () -> g.getNeighborMap("X"));
    }

    @Test
    void vertices() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        Set<String> verts = g.vertices();
        assertEquals(3, verts.size());
        assertTrue(verts.contains("A"));
        assertTrue(verts.contains("B"));
        assertTrue(verts.contains("C"));
    }

    @Test
    void edges() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "C", 2.0);
        List<Object[]> edgeList = g.edges();
        assertEquals(2, edgeList.size());
    }

    @Test
    void edgesKeepDistinctPairsWithDelimiterLabels() {
        GraphCore graph = new GraphCore(false);
        graph.addEdge("a||b", "c", 1.0);
        graph.addEdge("a", "b||c", 2.0);
        assertEquals(2, graph.edges().size());
    }

    @Test
    void edgesDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "A", 2.0);
        List<Object[]> edgeList = g.edges();
        assertEquals(2, edgeList.size());
    }

    @Test
    void numVerticesAndNumEdges() {
        GraphCore g = new GraphCore(false);
        assertEquals(0, g.numVertices());
        assertEquals(0, g.numEdges());
        g.addEdge("A", "B", 1.0);
        g.addEdge("B", "C", 2.0);
        assertEquals(3, g.numVertices());
        assertEquals(2, g.numEdges());
    }

    @Test
    void getInDegreeUndirected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1.0);
        g.addEdge("A", "C", 1.0);
        assertEquals(2, g.getInDegree("A"));
        assertEquals(1, g.getInDegree("B"));
    }

    @Test
    void getInDegreeDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "C", 1.0);
        g.addEdge("B", "C", 1.0);
        g.addEdge("C", "A", 1.0);
        assertEquals(2, g.getInDegree("C"));
        assertEquals(1, g.getInDegree("A"));
        assertEquals(0, g.getInDegree("B"));
    }

    @Test
    void getOutDegree() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1.0);
        g.addEdge("A", "C", 1.0);
        g.addEdge("A", "D", 1.0);
        assertEquals(3, g.getOutDegree("A"));
        assertEquals(0, g.getOutDegree("B"));
    }

    @Test
    void degreeNonExistentVertexThrows() {
        GraphCore g = new GraphCore(false);
        assertThrows(IllegalArgumentException.class, () -> g.getInDegree("X"));
        assertThrows(IllegalArgumentException.class, () -> g.getOutDegree("X"));
    }

    @Test
    void isDirected() {
        assertTrue(new GraphCore(true).isDirected());
        assertFalse(new GraphCore(false).isDirected());
    }

    @Test
    void isolatedVertices() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        assertEquals(3, g.numVertices());
        assertEquals(0, g.numEdges());
        assertEquals(0, g.getOutDegree("A"));
        assertTrue(g.getNeighborEntries("A").isEmpty());
    }
}
