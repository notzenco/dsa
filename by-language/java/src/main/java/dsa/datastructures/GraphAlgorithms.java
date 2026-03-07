package dsa.datastructures;

import java.util.*;

/**
 * Static graph algorithms that operate on {@link GraphCore} instances.
 * All vertex labels are Strings.
 */
public final class GraphAlgorithms {

    private GraphAlgorithms() {
        // utility class
    }

    /**
     * Performs a breadth-first search starting from the given vertex.
     *
     * @param graph the graph
     * @param start the starting vertex
     * @return list of vertices in BFS order
     * @throws IllegalArgumentException if start does not exist in the graph
     */
    public static List<String> bfs(GraphCore graph, String start) {
        validateVertex(graph, start);
        List<String> order = new ArrayList<>();
        Set<String> visited = new LinkedHashSet<>();
        java.util.Queue<String> queue = new java.util.LinkedList<>();
        visited.add(start);
        queue.add(start);
        while (!queue.isEmpty()) {
            String current = queue.poll();
            order.add(current);
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(current)) {
                String neighbor = entry.getKey();
                if (visited.add(neighbor)) {
                    queue.add(neighbor);
                }
            }
        }
        return order;
    }

    /**
     * Performs a depth-first search starting from the given vertex (iterative).
     *
     * @param graph the graph
     * @param start the starting vertex
     * @return list of vertices in DFS order
     * @throws IllegalArgumentException if start does not exist in the graph
     */
    public static List<String> dfs(GraphCore graph, String start) {
        validateVertex(graph, start);
        List<String> order = new ArrayList<>();
        Set<String> visited = new LinkedHashSet<>();
        Deque<String> stack = new ArrayDeque<>();
        stack.push(start);
        while (!stack.isEmpty()) {
            String current = stack.pop();
            if (!visited.add(current)) {
                continue;
            }
            order.add(current);
            // Push neighbors in reverse order so that the first neighbor is processed first
            List<Map.Entry<String, Double>> neighbors = graph.getNeighborEntries(current);
            for (int i = neighbors.size() - 1; i >= 0; i--) {
                String neighbor = neighbors.get(i).getKey();
                if (!visited.contains(neighbor)) {
                    stack.push(neighbor);
                }
            }
        }
        return order;
    }

    /**
     * Computes shortest distances from the start vertex using Dijkstra's algorithm.
     * Assumes non-negative edge weights.
     *
     * @param graph the graph
     * @param start the source vertex
     * @return map from vertex label to shortest distance from start
     * @throws IllegalArgumentException if start does not exist in the graph
     */
    public static Map<String, Double> dijkstra(GraphCore graph, String start) {
        validateVertex(graph, start);
        Map<String, Double> dist = new HashMap<>();
        for (String v : graph.vertices()) {
            dist.put(v, Double.POSITIVE_INFINITY);
        }
        dist.put(start, 0.0);

        // Priority queue: [distance, vertex]
        PriorityQueue<double[]> pq = new PriorityQueue<>(Comparator.comparingDouble(a -> a[0]));
        Map<String, Integer> vertexIndex = new HashMap<>();
        List<String> vertexList = new ArrayList<>(graph.vertices());
        for (int i = 0; i < vertexList.size(); i++) {
            vertexIndex.put(vertexList.get(i), i);
        }
        // Use a wrapper to carry String with distance
        record VertexDist(double dist, String vertex) {}
        PriorityQueue<VertexDist> queue = new PriorityQueue<>(Comparator.comparingDouble(vd -> vd.dist));
        queue.add(new VertexDist(0.0, start));

        Set<String> settled = new HashSet<>();

        while (!queue.isEmpty()) {
            VertexDist current = queue.poll();
            String u = current.vertex;
            if (!settled.add(u)) {
                continue;
            }
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                String v = entry.getKey();
                double weight = entry.getValue();
                double newDist = dist.get(u) + weight;
                if (newDist < dist.get(v)) {
                    dist.put(v, newDist);
                    queue.add(new VertexDist(newDist, v));
                }
            }
        }
        return dist;
    }

    /**
     * Computes shortest distances from the start vertex using the Bellman-Ford algorithm.
     * Handles negative edge weights. Detects negative-weight cycles.
     *
     * @param graph the graph
     * @param start the source vertex
     * @return map from vertex label to shortest distance from start
     * @throws IllegalArgumentException if start does not exist or a negative-weight cycle is detected
     */
    public static Map<String, Double> bellmanFord(GraphCore graph, String start) {
        validateVertex(graph, start);
        Map<String, Double> dist = new HashMap<>();
        for (String v : graph.vertices()) {
            dist.put(v, Double.POSITIVE_INFINITY);
        }
        dist.put(start, 0.0);

        int vertexCount = graph.numVertices();

        // Relax all edges (V - 1) times
        for (int i = 0; i < vertexCount - 1; i++) {
            boolean updated = false;
            for (String u : graph.vertices()) {
                if (dist.get(u) == Double.POSITIVE_INFINITY) continue;
                for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                    String v = entry.getKey();
                    double weight = entry.getValue();
                    double newDist = dist.get(u) + weight;
                    if (newDist < dist.get(v)) {
                        dist.put(v, newDist);
                        updated = true;
                    }
                }
            }
            if (!updated) break; // early termination
        }

        // Check for negative-weight cycles
        for (String u : graph.vertices()) {
            if (dist.get(u) == Double.POSITIVE_INFINITY) continue;
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                String v = entry.getKey();
                double weight = entry.getValue();
                if (dist.get(u) + weight < dist.get(v)) {
                    throw new IllegalArgumentException("Graph contains a negative-weight cycle");
                }
            }
        }

        return dist;
    }

    /**
     * Returns a topological ordering of a directed acyclic graph (DAG).
     * Uses Kahn's algorithm (BFS-based).
     *
     * @param graph the directed graph
     * @return list of vertices in topological order
     * @throws IllegalArgumentException if the graph is not directed or contains a cycle
     */
    public static List<String> topologicalSort(GraphCore graph) {
        if (!graph.isDirected()) {
            throw new IllegalArgumentException("Topological sort requires a directed graph");
        }

        Map<String, Integer> inDegree = new HashMap<>();
        for (String v : graph.vertices()) {
            inDegree.put(v, 0);
        }
        for (String u : graph.vertices()) {
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                inDegree.merge(entry.getKey(), 1, Integer::sum);
            }
        }

        java.util.Queue<String> queue = new java.util.LinkedList<>();
        for (Map.Entry<String, Integer> entry : inDegree.entrySet()) {
            if (entry.getValue() == 0) {
                queue.add(entry.getKey());
            }
        }

        List<String> result = new ArrayList<>();
        while (!queue.isEmpty()) {
            String u = queue.poll();
            result.add(u);
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                String v = entry.getKey();
                int newDeg = inDegree.get(v) - 1;
                inDegree.put(v, newDeg);
                if (newDeg == 0) {
                    queue.add(v);
                }
            }
        }

        if (result.size() != graph.numVertices()) {
            throw new IllegalArgumentException("Graph contains a cycle; topological sort is not possible");
        }
        return result;
    }

    /**
     * Detects whether the graph contains a cycle.
     * For directed graphs, uses DFS with coloring (white/gray/black).
     * For undirected graphs, uses DFS with parent tracking.
     *
     * @param graph the graph
     * @return true if the graph contains a cycle
     */
    public static boolean hasCycle(GraphCore graph) {
        if (graph.isDirected()) {
            return hasCycleDirected(graph);
        } else {
            return hasCycleUndirected(graph);
        }
    }

    private static boolean hasCycleDirected(GraphCore graph) {
        // 0 = white (unvisited), 1 = gray (in progress), 2 = black (finished)
        Map<String, Integer> color = new HashMap<>();
        for (String v : graph.vertices()) {
            color.put(v, 0);
        }
        for (String v : graph.vertices()) {
            if (color.get(v) == 0) {
                if (dfsCycleDirected(graph, v, color)) {
                    return true;
                }
            }
        }
        return false;
    }

    private static boolean dfsCycleDirected(GraphCore graph, String u, Map<String, Integer> color) {
        color.put(u, 1);
        for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
            String v = entry.getKey();
            if (color.get(v) == 1) {
                return true; // back edge found
            }
            if (color.get(v) == 0 && dfsCycleDirected(graph, v, color)) {
                return true;
            }
        }
        color.put(u, 2);
        return false;
    }

    private static boolean hasCycleUndirected(GraphCore graph) {
        Set<String> visited = new HashSet<>();
        for (String v : graph.vertices()) {
            if (!visited.contains(v)) {
                if (dfsCycleUndirected(graph, v, null, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    private static boolean dfsCycleUndirected(GraphCore graph, String u, String parent,
                                               Set<String> visited) {
        visited.add(u);
        for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
            String v = entry.getKey();
            if (!visited.contains(v)) {
                if (dfsCycleUndirected(graph, v, u, visited)) {
                    return true;
                }
            } else if (!v.equals(parent)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Finds all connected components of an undirected graph.
     * For directed graphs, finds weakly connected components.
     *
     * @param graph the graph
     * @return list of components, each component is a list of vertex labels
     */
    public static List<List<String>> connectedComponents(GraphCore graph) {
        List<List<String>> components = new ArrayList<>();
        Set<String> visited = new HashSet<>();

        if (graph.isDirected()) {
            // Build an undirected view for weakly connected components
            GraphCore undirected = new GraphCore(false);
            for (String v : graph.vertices()) {
                undirected.addVertex(v);
            }
            for (Object[] edge : graph.edges()) {
                String u = (String) edge[0];
                String v = (String) edge[1];
                double w = (Double) edge[2];
                undirected.addEdge(u, v, w);
            }
            return connectedComponents(undirected);
        }

        for (String v : graph.vertices()) {
            if (!visited.contains(v)) {
                List<String> component = new ArrayList<>();
                java.util.Queue<String> queue = new java.util.LinkedList<>();
                visited.add(v);
                queue.add(v);
                while (!queue.isEmpty()) {
                    String current = queue.poll();
                    component.add(current);
                    for (Map.Entry<String, Double> entry : graph.getNeighborEntries(current)) {
                        String neighbor = entry.getKey();
                        if (visited.add(neighbor)) {
                            queue.add(neighbor);
                        }
                    }
                }
                components.add(component);
            }
        }
        return components;
    }

    /**
     * Checks whether the graph is connected.
     * For undirected graphs: checks if there is exactly one connected component.
     * For directed graphs: checks weak connectivity.
     *
     * @param graph the graph
     * @return true if the graph is connected
     */
    public static boolean isConnected(GraphCore graph) {
        if (graph.numVertices() == 0) {
            return true;
        }
        return connectedComponents(graph).size() == 1;
    }

    /**
     * Finds the shortest path from start to end using Dijkstra's algorithm
     * and reconstructs the path.
     *
     * @param graph the graph
     * @param start the source vertex
     * @param end   the destination vertex
     * @return list of vertices forming the shortest path from start to end,
     *         or an empty list if no path exists
     * @throws IllegalArgumentException if start or end does not exist
     */
    public static List<String> shortestPath(GraphCore graph, String start, String end) {
        validateVertex(graph, start);
        validateVertex(graph, end);

        Map<String, Double> dist = new HashMap<>();
        Map<String, String> prev = new HashMap<>();
        for (String v : graph.vertices()) {
            dist.put(v, Double.POSITIVE_INFINITY);
            prev.put(v, null);
        }
        dist.put(start, 0.0);

        record VertexDist(double dist, String vertex) {}
        PriorityQueue<VertexDist> queue = new PriorityQueue<>(Comparator.comparingDouble(vd -> vd.dist));
        queue.add(new VertexDist(0.0, start));
        Set<String> settled = new HashSet<>();

        while (!queue.isEmpty()) {
            VertexDist current = queue.poll();
            String u = current.vertex;
            if (!settled.add(u)) {
                continue;
            }
            if (u.equals(end)) {
                break;
            }
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                String v = entry.getKey();
                double weight = entry.getValue();
                double newDist = dist.get(u) + weight;
                if (newDist < dist.get(v)) {
                    dist.put(v, newDist);
                    prev.put(v, u);
                    queue.add(new VertexDist(newDist, v));
                }
            }
        }

        // Reconstruct path
        if (dist.get(end) == Double.POSITIVE_INFINITY) {
            return Collections.emptyList();
        }

        LinkedList<String> path = new LinkedList<>();
        String current = end;
        while (current != null) {
            path.addFirst(current);
            current = prev.get(current);
        }
        return path;
    }

    private static void validateVertex(GraphCore graph, String v) {
        if (!graph.hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist in the graph");
        }
    }
}
