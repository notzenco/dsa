package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

class GraphAlgorithmsTest {

    // ========== BFS Tests ==========

    @Test
    void bfsSimpleUndirected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "D", 1);
        g.addEdge("C", "D", 1);
        List<String> order = GraphAlgorithms.bfs(g, "A");
        assertEquals("A", order.get(0));
        assertEquals(4, order.size());
        // B and C should come before D
        assertTrue(order.indexOf("B") < order.indexOf("D"));
        assertTrue(order.indexOf("C") < order.indexOf("D"));
    }

    @Test
    void bfsDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "D", 1);
        List<String> order = GraphAlgorithms.bfs(g, "A");
        assertEquals("A", order.get(0));
        assertTrue(order.contains("B"));
        assertTrue(order.contains("C"));
        assertTrue(order.contains("D"));
    }

    @Test
    void bfsSingleVertex() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        List<String> order = GraphAlgorithms.bfs(g, "A");
        assertEquals(List.of("A"), order);
    }

    @Test
    void bfsDisconnected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        List<String> order = GraphAlgorithms.bfs(g, "A");
        assertEquals(2, order.size());
        assertFalse(order.contains("C"));
    }

    @Test
    void bfsInvalidStartThrows() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        assertThrows(IllegalArgumentException.class, () -> GraphAlgorithms.bfs(g, "Z"));
    }

    // ========== DFS Tests ==========

    @Test
    void dfsSimpleUndirected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "D", 1);
        List<String> order = GraphAlgorithms.dfs(g, "A");
        assertEquals("A", order.get(0));
        assertEquals(4, order.size());
        assertTrue(order.contains("B"));
        assertTrue(order.contains("C"));
        assertTrue(order.contains("D"));
    }

    @Test
    void dfsDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "D", 1);
        List<String> order = GraphAlgorithms.dfs(g, "A");
        assertEquals(List.of("A", "B", "C", "D"), order);
    }

    @Test
    void dfsSingleVertex() {
        GraphCore g = new GraphCore(false);
        g.addVertex("X");
        assertEquals(List.of("X"), GraphAlgorithms.dfs(g, "X"));
    }

    @Test
    void dfsDisconnected() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        List<String> order = GraphAlgorithms.dfs(g, "A");
        assertEquals(2, order.size());
        assertFalse(order.contains("C"));
    }

    @Test
    void dfsInvalidStartThrows() {
        GraphCore g = new GraphCore(false);
        assertThrows(IllegalArgumentException.class, () -> GraphAlgorithms.dfs(g, "A"));
    }

    // ========== Dijkstra Tests ==========

    @Test
    void dijkstraSimple() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 2);
        g.addEdge("A", "C", 10);
        Map<String, Double> dist = GraphAlgorithms.dijkstra(g, "A");
        assertEquals(0.0, dist.get("A"));
        assertEquals(1.0, dist.get("B"));
        assertEquals(3.0, dist.get("C")); // A->B->C = 3, not A->C = 10
    }

    @Test
    void dijkstraDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 4);
        g.addEdge("A", "C", 2);
        g.addEdge("C", "B", 1);
        g.addEdge("B", "D", 3);
        g.addEdge("C", "D", 8);
        Map<String, Double> dist = GraphAlgorithms.dijkstra(g, "A");
        assertEquals(0.0, dist.get("A"));
        assertEquals(3.0, dist.get("B")); // A->C->B = 2+1 = 3
        assertEquals(2.0, dist.get("C"));
        assertEquals(6.0, dist.get("D")); // A->C->B->D = 2+1+3 = 6
    }

    @Test
    void dijkstraUnreachable() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        Map<String, Double> dist = GraphAlgorithms.dijkstra(g, "A");
        assertEquals(Double.POSITIVE_INFINITY, dist.get("C"));
    }

    @Test
    void dijkstraSingleVertex() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        Map<String, Double> dist = GraphAlgorithms.dijkstra(g, "A");
        assertEquals(0.0, dist.get("A"));
    }

    // ========== Bellman-Ford Tests ==========

    @Test
    void bellmanFordSimple() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 4);
        g.addEdge("A", "C", 2);
        g.addEdge("C", "B", 1);
        Map<String, Double> dist = GraphAlgorithms.bellmanFord(g, "A");
        assertEquals(0.0, dist.get("A"));
        assertEquals(3.0, dist.get("B"));
        assertEquals(2.0, dist.get("C"));
    }

    @Test
    void bellmanFordNegativeWeights() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 5);
        g.addEdge("A", "C", 2);
        g.addEdge("C", "B", -4);
        Map<String, Double> dist = GraphAlgorithms.bellmanFord(g, "A");
        assertEquals(0.0, dist.get("A"));
        assertEquals(-2.0, dist.get("B")); // A->C->B = 2 + (-4) = -2
        assertEquals(2.0, dist.get("C"));
    }

    @Test
    void bellmanFordNegativeCycleThrows() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", -3);
        g.addEdge("C", "A", 1);
        assertThrows(IllegalArgumentException.class, () -> GraphAlgorithms.bellmanFord(g, "A"));
    }

    @Test
    void bellmanFordUnreachable() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        Map<String, Double> dist = GraphAlgorithms.bellmanFord(g, "A");
        assertEquals(Double.POSITIVE_INFINITY, dist.get("C"));
    }

    @Test
    void bellmanFordMatchesDijkstra() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 3);
        g.addEdge("A", "C", 6);
        g.addEdge("B", "C", 2);
        g.addEdge("B", "D", 1);
        g.addEdge("C", "D", 5);
        Map<String, Double> dijkDist = GraphAlgorithms.dijkstra(g, "A");
        Map<String, Double> bfDist = GraphAlgorithms.bellmanFord(g, "A");
        for (String v : g.vertices()) {
            assertEquals(dijkDist.get(v), bfDist.get(v), 1e-9,
                    "Distance mismatch for vertex " + v);
        }
    }

    // ========== Topological Sort Tests ==========

    @Test
    void topologicalSortSimple() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "D", 1);
        g.addEdge("C", "D", 1);
        List<String> order = GraphAlgorithms.topologicalSort(g);
        assertEquals(4, order.size());
        assertTrue(order.indexOf("A") < order.indexOf("B"));
        assertTrue(order.indexOf("A") < order.indexOf("C"));
        assertTrue(order.indexOf("B") < order.indexOf("D"));
        assertTrue(order.indexOf("C") < order.indexOf("D"));
    }

    @Test
    void topologicalSortLinear() {
        GraphCore g = new GraphCore(true);
        g.addEdge("1", "2", 1);
        g.addEdge("2", "3", 1);
        g.addEdge("3", "4", 1);
        List<String> order = GraphAlgorithms.topologicalSort(g);
        assertEquals(List.of("1", "2", "3", "4"), order);
    }

    @Test
    void topologicalSortWithCycleThrows() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "A", 1);
        assertThrows(IllegalArgumentException.class, () -> GraphAlgorithms.topologicalSort(g));
    }

    @Test
    void topologicalSortUndirectedThrows() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        assertThrows(IllegalArgumentException.class, () -> GraphAlgorithms.topologicalSort(g));
    }

    @Test
    void topologicalSortSingleVertex() {
        GraphCore g = new GraphCore(true);
        g.addVertex("A");
        assertEquals(List.of("A"), GraphAlgorithms.topologicalSort(g));
    }

    @Test
    void topologicalSortDisconnected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        g.addVertex("D");
        List<String> order = GraphAlgorithms.topologicalSort(g);
        assertEquals(4, order.size());
        assertTrue(order.indexOf("A") < order.indexOf("B"));
    }

    // ========== hasCycle Tests ==========

    @Test
    void hasCycleDirectedWithCycle() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "A", 1);
        assertTrue(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleDirectedWithoutCycle() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "C", 1);
        assertFalse(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleUndirectedWithCycle() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "A", 1);
        assertTrue(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleUndirectedWithoutCycle() {
        // Tree: no cycle
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("A", "C", 1);
        g.addEdge("B", "D", 1);
        assertFalse(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleSingleVertex() {
        GraphCore g = new GraphCore(true);
        g.addVertex("A");
        assertFalse(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleSelfLoop() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "A", 1);
        assertTrue(GraphAlgorithms.hasCycle(g));
    }

    @Test
    void hasCycleDirectedDisconnectedWithCycle() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 1);
        // Separate cycle
        g.addEdge("C", "D", 1);
        g.addEdge("D", "C", 1);
        assertTrue(GraphAlgorithms.hasCycle(g));
    }

    // ========== Connected Components Tests ==========

    @Test
    void connectedComponentsSingleComponent() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "D", 1);
        List<List<String>> components = GraphAlgorithms.connectedComponents(g);
        assertEquals(1, components.size());
        assertEquals(4, components.get(0).size());
    }

    @Test
    void connectedComponentsMultiple() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("C", "D", 1);
        g.addVertex("E");
        List<List<String>> components = GraphAlgorithms.connectedComponents(g);
        assertEquals(3, components.size());
    }

    @Test
    void connectedComponentsAllIsolated() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        g.addVertex("B");
        g.addVertex("C");
        List<List<String>> components = GraphAlgorithms.connectedComponents(g);
        assertEquals(3, components.size());
        for (List<String> comp : components) {
            assertEquals(1, comp.size());
        }
    }

    @Test
    void connectedComponentsEmpty() {
        GraphCore g = new GraphCore(false);
        List<List<String>> components = GraphAlgorithms.connectedComponents(g);
        assertTrue(components.isEmpty());
    }

    // ========== isConnected Tests ==========

    @Test
    void isConnectedTrue() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        assertTrue(GraphAlgorithms.isConnected(g));
    }

    @Test
    void isConnectedFalse() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addVertex("C");
        assertFalse(GraphAlgorithms.isConnected(g));
    }

    @Test
    void isConnectedEmptyGraph() {
        GraphCore g = new GraphCore(false);
        assertTrue(GraphAlgorithms.isConnected(g));
    }

    @Test
    void isConnectedSingleVertex() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        assertTrue(GraphAlgorithms.isConnected(g));
    }

    // ========== Shortest Path Tests ==========

    @Test
    void shortestPathSimple() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 2);
        g.addEdge("A", "C", 10);
        List<String> path = GraphAlgorithms.shortestPath(g, "A", "C");
        assertEquals(List.of("A", "B", "C"), path);
    }

    @Test
    void shortestPathDirect() {
        GraphCore g = new GraphCore(false);
        g.addEdge("A", "B", 1);
        List<String> path = GraphAlgorithms.shortestPath(g, "A", "B");
        assertEquals(List.of("A", "B"), path);
    }

    @Test
    void shortestPathSameVertex() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        List<String> path = GraphAlgorithms.shortestPath(g, "A", "A");
        assertEquals(List.of("A"), path);
    }

    @Test
    void shortestPathNoPath() {
        GraphCore g = new GraphCore(true);
        g.addVertex("A");
        g.addVertex("B");
        List<String> path = GraphAlgorithms.shortestPath(g, "A", "B");
        assertTrue(path.isEmpty());
    }

    @Test
    void shortestPathDirected() {
        GraphCore g = new GraphCore(true);
        g.addEdge("A", "B", 2);
        g.addEdge("B", "C", 3);
        g.addEdge("A", "C", 100);
        g.addEdge("C", "D", 1);
        List<String> path = GraphAlgorithms.shortestPath(g, "A", "D");
        assertEquals(List.of("A", "B", "C", "D"), path);
    }

    @Test
    void shortestPathLongerGraph() {
        GraphCore g = new GraphCore(false);
        g.addEdge("S", "A", 1);
        g.addEdge("S", "B", 4);
        g.addEdge("A", "B", 2);
        g.addEdge("A", "C", 5);
        g.addEdge("B", "C", 1);
        g.addEdge("C", "T", 3);
        List<String> path = GraphAlgorithms.shortestPath(g, "S", "T");
        // S->A(1)->B(3)->C(4)->T(7)
        assertEquals(List.of("S", "A", "B", "C", "T"), path);
    }

    @Test
    void shortestPathInvalidVertexThrows() {
        GraphCore g = new GraphCore(false);
        g.addVertex("A");
        assertThrows(IllegalArgumentException.class,
                () -> GraphAlgorithms.shortestPath(g, "A", "Z"));
        assertThrows(IllegalArgumentException.class,
                () -> GraphAlgorithms.shortestPath(g, "Z", "A"));
    }
}
