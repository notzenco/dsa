package dsa.datastructures;

import java.util.*;

/**
 * Graph using adjacency list representation with String vertex labels.
 * Supports weighted, directed, and undirected graphs.
 */
public class GraphCore {

    private final boolean directed;
    private final Map<String, Map<String, Double>> adjacencyList;

    /**
     * Creates a graph.
     *
     * @param directed true for a directed graph, false for undirected
     */
    public GraphCore(boolean directed) {
        this.directed = directed;
        this.adjacencyList = new LinkedHashMap<>();
    }

    /**
     * Adds a vertex to the graph. Does nothing if the vertex already exists.
     *
     * @param v the vertex label
     * @throws IllegalArgumentException if v is null
     */
    public void addVertex(String v) {
        requireNonNull(v);
        adjacencyList.putIfAbsent(v, new LinkedHashMap<>());
    }

    /**
     * Adds a weighted edge from u to v. Creates vertices if they do not exist.
     * For undirected graphs, the reverse edge is also added.
     * If the edge already exists, the weight is updated.
     *
     * @param u      the source vertex
     * @param v      the destination vertex
     * @param weight the edge weight
     * @throws IllegalArgumentException if u or v is null
     */
    public void addEdge(String u, String v, double weight) {
        requireNonNull(u);
        requireNonNull(v);
        addVertex(u);
        addVertex(v);
        adjacencyList.get(u).put(v, weight);
        if (!directed) {
            adjacencyList.get(v).put(u, weight);
        }
    }

    /**
     * Removes the edge from u to v.
     * For undirected graphs, the reverse edge is also removed.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @throws IllegalArgumentException if u or v is null, or if the edge does not exist
     */
    public void removeEdge(String u, String v) {
        requireNonNull(u);
        requireNonNull(v);
        if (!hasEdge(u, v)) {
            throw new IllegalArgumentException(
                    "Edge (" + u + ", " + v + ") does not exist");
        }
        adjacencyList.get(u).remove(v);
        if (!directed) {
            adjacencyList.get(v).remove(u);
        }
    }

    /**
     * Removes a vertex and all its incident edges from the graph.
     *
     * @param v the vertex to remove
     * @throws IllegalArgumentException if v is null or does not exist
     */
    public void removeVertex(String v) {
        requireNonNull(v);
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        // Remove all edges pointing to v
        for (Map<String, Double> neighbors : adjacencyList.values()) {
            neighbors.remove(v);
        }
        // Remove the vertex entry itself
        adjacencyList.remove(v);
    }

    /**
     * Checks if the graph contains the given vertex.
     *
     * @param v the vertex label
     * @return true if the vertex exists
     */
    public boolean hasVertex(String v) {
        return v != null && adjacencyList.containsKey(v);
    }

    /**
     * Checks if an edge exists from u to v.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @return true if the edge exists
     */
    public boolean hasEdge(String u, String v) {
        if (u == null || v == null) return false;
        Map<String, Double> neighbors = adjacencyList.get(u);
        return neighbors != null && neighbors.containsKey(v);
    }

    /**
     * Returns the weight of the edge from u to v.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @return the edge weight
     * @throws IllegalArgumentException if the edge does not exist
     */
    public double getWeight(String u, String v) {
        if (!hasEdge(u, v)) {
            throw new IllegalArgumentException(
                    "Edge (" + u + ", " + v + ") does not exist");
        }
        return adjacencyList.get(u).get(v);
    }

    /**
     * Returns the neighbors of vertex v as a list of [neighbor, weight] pairs.
     * Each pair is a two-element array: {neighborLabel, weightAsString}.
     *
     * @param v the vertex
     * @return list of neighbor entries, each as a double-array [neighbor, weight]
     * @throws IllegalArgumentException if v does not exist
     */
    public List<double[]> getNeighbors(String v) {
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        // Return as list of maps for flexibility; but spec says [neighbor, weight].
        // We'll return a wrapper. Since neighbor is String and weight is double,
        // use a Map.Entry list instead.
        throw new UnsupportedOperationException("Use getNeighborEntries instead");
    }

    /**
     * Returns the neighbors of vertex v as a list of Map entries (neighbor -> weight).
     *
     * @param v the vertex
     * @return list of entries mapping neighbor label to weight
     * @throws IllegalArgumentException if v does not exist
     */
    public List<Map.Entry<String, Double>> getNeighborEntries(String v) {
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        return new ArrayList<>(adjacencyList.get(v).entrySet());
    }

    /**
     * Returns a map of neighbor -> weight for the given vertex.
     *
     * @param v the vertex
     * @return unmodifiable map of neighbors with their weights
     * @throws IllegalArgumentException if v does not exist
     */
    public Map<String, Double> getNeighborMap(String v) {
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        return Collections.unmodifiableMap(adjacencyList.get(v));
    }

    /**
     * Returns all vertex labels in the graph.
     *
     * @return set of vertex labels
     */
    public Set<String> vertices() {
        return Collections.unmodifiableSet(adjacencyList.keySet());
    }

    /**
     * Returns all edges in the graph. Each edge is represented as a three-element
     * array: [source, destination, weight]. For undirected graphs, each edge
     * appears once (u, v) where u is lexicographically <= v.
     *
     * @return list of edges
     */
    public List<Object[]> edges() {
        List<Object[]> result = new ArrayList<>();
        Set<String> visited = new HashSet<>();
        for (String u : adjacencyList.keySet()) {
            for (Map.Entry<String, Double> entry : adjacencyList.get(u).entrySet()) {
                String v = entry.getKey();
                if (directed) {
                    result.add(new Object[]{u, v, entry.getValue()});
                } else {
                    String key = u.compareTo(v) <= 0 ? u + "||" + v : v + "||" + u;
                    if (visited.add(key)) {
                        result.add(new Object[]{u, v, entry.getValue()});
                    }
                }
            }
        }
        return result;
    }

    /**
     * Returns the number of vertices.
     *
     * @return vertex count
     */
    public int numVertices() {
        return adjacencyList.size();
    }

    /**
     * Returns the number of edges.
     *
     * @return edge count
     */
    public int numEdges() {
        int count = 0;
        for (Map<String, Double> neighbors : adjacencyList.values()) {
            count += neighbors.size();
        }
        if (!directed) {
            count /= 2;
        }
        return count;
    }

    /**
     * Returns the in-degree of vertex v (number of edges pointing to v).
     * For undirected graphs, this equals the degree.
     *
     * @param v the vertex
     * @return the in-degree
     * @throws IllegalArgumentException if v does not exist
     */
    public int getInDegree(String v) {
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        if (!directed) {
            return adjacencyList.get(v).size();
        }
        int count = 0;
        for (Map.Entry<String, Map<String, Double>> entry : adjacencyList.entrySet()) {
            if (entry.getValue().containsKey(v)) {
                count++;
            }
        }
        return count;
    }

    /**
     * Returns the out-degree of vertex v (number of edges going out from v).
     * For undirected graphs, this equals the degree.
     *
     * @param v the vertex
     * @return the out-degree
     * @throws IllegalArgumentException if v does not exist
     */
    public int getOutDegree(String v) {
        if (!hasVertex(v)) {
            throw new IllegalArgumentException("Vertex " + v + " does not exist");
        }
        return adjacencyList.get(v).size();
    }

    /**
     * Returns whether the graph is directed.
     *
     * @return true if directed
     */
    public boolean isDirected() {
        return directed;
    }

    private void requireNonNull(String v) {
        if (v == null) {
            throw new IllegalArgumentException("Vertex label must not be null");
        }
    }
}
