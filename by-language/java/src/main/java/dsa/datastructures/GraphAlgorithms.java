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

    public static Map<String, Map<String, Double>> floydWarshall(GraphCore graph) {
        Set<String> vertices = graph.vertices();
        Map<String, Map<String, Double>> dist = new HashMap<>();
        // Initialize distances
        for (String u : vertices) {
            Map<String, Double> row = new HashMap<>();
            for (String v : vertices) {
                if (u.equals(v)) {
                    row.put(v, 0.0);
                } else {
                    row.put(v, Double.POSITIVE_INFINITY);
                }
            }
            dist.put(u, row);
        }
        // Set edge weights
        for (Object[] edge : graph.edges()) {
            String u = (String) edge[0];
            String v = (String) edge[1];
            double w = (Double) edge[2];
            dist.get(u).put(v, Math.min(dist.get(u).get(v), w));
            if (!graph.isDirected()) {
                dist.get(v).put(u, Math.min(dist.get(v).get(u), w));
            }
        }
        // Relax through all intermediates
        for (String k : vertices) {
            for (String i : vertices) {
                for (String j : vertices) {
                    double throughK = dist.get(i).get(k) + dist.get(k).get(j);
                    if (throughK < dist.get(i).get(j)) {
                        dist.get(i).put(j, throughK);
                    }
                }
            }
        }
        return dist;
    }

    public static List<Object[]> primMST(GraphCore graph) {
        List<Object[]> mst = new ArrayList<>();
        if (graph.numVertices() == 0) return mst;

        Set<String> inMST = new HashSet<>();
        // Priority queue: [weight, from, to]
        PriorityQueue<double[]> pq = new PriorityQueue<>(Comparator.comparingDouble(a -> a[0]));
        Map<String, Integer> vertexIdx = new HashMap<>();
        List<String> vertexList = new ArrayList<>(graph.vertices());
        for (int i = 0; i < vertexList.size(); i++) {
            vertexIdx.put(vertexList.get(i), i);
        }

        // Store edge info separately since we need String labels
        record EdgeInfo(double weight, String from, String to) {}
        PriorityQueue<EdgeInfo> queue = new PriorityQueue<>(Comparator.comparingDouble(e -> e.weight));

        String start = vertexList.get(0);
        inMST.add(start);
        for (Map.Entry<String, Double> entry : graph.getNeighborEntries(start)) {
            queue.add(new EdgeInfo(entry.getValue(), start, entry.getKey()));
        }

        while (!queue.isEmpty() && inMST.size() < graph.numVertices()) {
            EdgeInfo edge = queue.poll();
            if (inMST.contains(edge.to)) continue;
            inMST.add(edge.to);
            mst.add(new Object[]{edge.from, edge.to, edge.weight});
            for (Map.Entry<String, Double> entry : graph.getNeighborEntries(edge.to)) {
                if (!inMST.contains(entry.getKey())) {
                    queue.add(new EdgeInfo(entry.getValue(), edge.to, entry.getKey()));
                }
            }
        }
        return mst;
    }

    public static List<Object[]> kruskalMST(GraphCore graph) {
        List<Object[]> mst = new ArrayList<>();
        List<Object[]> edges = graph.edges();
        edges.sort(Comparator.comparingDouble(e -> (Double) e[2]));

        // Simple union-find using maps
        Map<String, String> parent = new HashMap<>();
        Map<String, Integer> rank = new HashMap<>();
        for (String v : graph.vertices()) {
            parent.put(v, v);
            rank.put(v, 0);
        }

        for (Object[] edge : edges) {
            String u = (String) edge[0];
            String v = (String) edge[1];
            double w = (Double) edge[2];
            String rootU = find(parent, u);
            String rootV = find(parent, v);
            if (!rootU.equals(rootV)) {
                mst.add(new Object[]{u, v, w});
                union(parent, rank, rootU, rootV);
            }
        }
        return mst;
    }

    private static String find(Map<String, String> parent, String x) {
        while (!parent.get(x).equals(x)) {
            parent.put(x, parent.get(parent.get(x))); // path compression
            x = parent.get(x);
        }
        return x;
    }

    private static void union(Map<String, String> parent, Map<String, Integer> rank,
                               String x, String y) {
        if (rank.get(x) < rank.get(y)) {
            parent.put(x, y);
        } else if (rank.get(x) > rank.get(y)) {
            parent.put(y, x);
        } else {
            parent.put(y, x);
            rank.put(x, rank.get(x) + 1);
        }
    }

    public static List<List<String>> tarjanSCC(GraphCore graph) {
        List<List<String>> sccs = new ArrayList<>();
        Map<String, Integer> disc = new HashMap<>();
        Map<String, Integer> low = new HashMap<>();
        Set<String> onStack = new HashSet<>();
        Deque<String> stack = new ArrayDeque<>();
        int[] timer = {0};

        for (String v : graph.vertices()) {
            if (!disc.containsKey(v)) {
                tarjanDFS(graph, v, disc, low, onStack, stack, sccs, timer);
            }
        }
        return sccs;
    }

    private static void tarjanDFS(GraphCore graph, String u,
                                   Map<String, Integer> disc, Map<String, Integer> low,
                                   Set<String> onStack, Deque<String> stack,
                                   List<List<String>> sccs, int[] timer) {
        disc.put(u, timer[0]);
        low.put(u, timer[0]);
        timer[0]++;
        stack.push(u);
        onStack.add(u);

        for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
            String v = entry.getKey();
            if (!disc.containsKey(v)) {
                tarjanDFS(graph, v, disc, low, onStack, stack, sccs, timer);
                low.put(u, Math.min(low.get(u), low.get(v)));
            } else if (onStack.contains(v)) {
                low.put(u, Math.min(low.get(u), disc.get(v)));
            }
        }

        if (low.get(u).equals(disc.get(u))) {
            List<String> scc = new ArrayList<>();
            while (true) {
                String v = stack.pop();
                onStack.remove(v);
                scc.add(v);
                if (v.equals(u)) break;
            }
            sccs.add(scc);
        }
    }

    public static List<List<String>> kosarajuSCC(GraphCore graph) {
        // Step 1: Compute finish order using DFS
        Set<String> visited = new HashSet<>();
        Deque<String> finishOrder = new ArrayDeque<>();
        for (String v : graph.vertices()) {
            if (!visited.contains(v)) {
                kosarajuDFS1(graph, v, visited, finishOrder);
            }
        }

        // Step 2: Build transposed graph
        GraphCore transposed = new GraphCore(true);
        for (String v : graph.vertices()) {
            transposed.addVertex(v);
        }
        for (Object[] edge : graph.edges()) {
            String u = (String) edge[0];
            String v = (String) edge[1];
            double w = (Double) edge[2];
            transposed.addEdge(v, u, w);
        }

        // Step 3: DFS in reverse finish order on transposed graph
        visited.clear();
        List<List<String>> sccs = new ArrayList<>();
        while (!finishOrder.isEmpty()) {
            String v = finishOrder.pop();
            if (!visited.contains(v)) {
                List<String> scc = new ArrayList<>();
                kosarajuDFS2(transposed, v, visited, scc);
                sccs.add(scc);
            }
        }
        return sccs;
    }

    private static void kosarajuDFS1(GraphCore graph, String u, Set<String> visited,
                                      Deque<String> finishOrder) {
        visited.add(u);
        for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
            String v = entry.getKey();
            if (!visited.contains(v)) {
                kosarajuDFS1(graph, v, visited, finishOrder);
            }
        }
        finishOrder.push(u);
    }

    private static void kosarajuDFS2(GraphCore transposed, String u, Set<String> visited,
                                      List<String> scc) {
        visited.add(u);
        scc.add(u);
        for (Map.Entry<String, Double> entry : transposed.getNeighborEntries(u)) {
            String v = entry.getKey();
            if (!visited.contains(v)) {
                kosarajuDFS2(transposed, v, visited, scc);
            }
        }
    }

    public static boolean isBipartite(GraphCore graph) {
        Map<String, Integer> color = new HashMap<>();
        for (String v : graph.vertices()) {
            if (!color.containsKey(v)) {
                // BFS coloring
                java.util.Queue<String> queue = new java.util.LinkedList<>();
                color.put(v, 0);
                queue.add(v);
                while (!queue.isEmpty()) {
                    String u = queue.poll();
                    int uColor = color.get(u);
                    for (Map.Entry<String, Double> entry : graph.getNeighborEntries(u)) {
                        String neighbor = entry.getKey();
                        if (!color.containsKey(neighbor)) {
                            color.put(neighbor, 1 - uColor);
                            queue.add(neighbor);
                        } else if (color.get(neighbor) == uColor) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    private static void validateVertex(GraphCore graph, String v) {
        if (!graph.hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist in the graph");
        }
    }
}
