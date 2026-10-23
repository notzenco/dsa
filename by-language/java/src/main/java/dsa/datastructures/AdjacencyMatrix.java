package dsa.datastructures;

import java.util.ArrayList;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.List;
import java.util.Queue;

/**
 * Graph representation using an adjacency matrix.
 * Supports weighted, directed, and undirected graphs.
 * Vertices are numbered from 0 to numVertices - 1.
 */
public class AdjacencyMatrix {

    private static final double NO_EDGE = Double.POSITIVE_INFINITY;

    private final double[][] matrix;
    private final int numVertices;
    private final boolean directed;
    private int numEdges;

    /**
     * Creates a graph with the given number of vertices.
     *
     * @param numVertices the number of vertices (must be positive)
     * @param directed    true for a directed graph, false for undirected
     * @throws IllegalArgumentException if numVertices is not positive
     */
    public AdjacencyMatrix(int numVertices, boolean directed) {
        if (numVertices <= 0) {
            throw new IllegalArgumentException("Number of vertices must be positive");
        }
        this.numVertices = numVertices;
        this.directed = directed;
        this.matrix = new double[numVertices][numVertices];
        this.numEdges = 0;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                matrix[i][j] = NO_EDGE;
            }
        }
    }

    /**
     * Adds an edge from vertex u to vertex v with the given weight.
     * For undirected graphs, the reverse edge is also added.
     * If the edge already exists, its weight is updated.
     *
     * @param u      the source vertex
     * @param v      the destination vertex
     * @param weight the edge weight
     * @throws IndexOutOfBoundsException if u or v is out of range
     * @throws IllegalArgumentException if weight is positive infinity, the
     *         sentinel used for a missing edge
     */
    public void addEdge(int u, int v, double weight) {
        validateVertex(u);
        validateVertex(v);
        if (weight == NO_EDGE) {
            throw new IllegalArgumentException("Infinite weight is reserved for missing edges");
        }
        boolean isNew = !hasEdge(u, v);
        matrix[u][v] = weight;
        if (!directed) {
            matrix[v][u] = weight;
        }
        if (isNew) {
            numEdges++;
        }
    }

    /**
     * Removes the edge from vertex u to vertex v.
     * For undirected graphs, the reverse edge is also removed.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @throws IndexOutOfBoundsException if u or v is out of range
     * @throws IllegalArgumentException  if the edge does not exist
     */
    public void removeEdge(int u, int v) {
        validateVertex(u);
        validateVertex(v);
        if (!hasEdge(u, v)) {
            throw new IllegalArgumentException(
                    "Edge (" + u + ", " + v + ") does not exist");
        }
        matrix[u][v] = NO_EDGE;
        if (!directed) {
            matrix[v][u] = NO_EDGE;
        }
        numEdges--;
    }

    /**
     * Checks whether an edge exists from vertex u to vertex v.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @return true if the edge exists
     * @throws IndexOutOfBoundsException if u or v is out of range
     */
    public boolean hasEdge(int u, int v) {
        validateVertex(u);
        validateVertex(v);
        return matrix[u][v] != NO_EDGE;
    }

    /**
     * Returns the weight of the edge from u to v.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @return the edge weight
     * @throws IndexOutOfBoundsException if u or v is out of range
     * @throws IllegalArgumentException  if the edge does not exist
     */
    public double getWeight(int u, int v) {
        validateVertex(u);
        validateVertex(v);
        if (!hasEdge(u, v)) {
            throw new IllegalArgumentException(
                    "Edge (" + u + ", " + v + ") does not exist");
        }
        return matrix[u][v];
    }

    /**
     * Returns a list of vertices adjacent to v (vertices that v has an edge to).
     *
     * @param v the vertex
     * @return list of neighbor vertex indices
     * @throws IndexOutOfBoundsException if v is out of range
     */
    public List<Integer> getNeighbors(int v) {
        validateVertex(v);
        List<Integer> neighbors = new ArrayList<>();
        for (int i = 0; i < numVertices; i++) {
            if (matrix[v][i] != NO_EDGE) {
                neighbors.add(i);
            }
        }
        return neighbors;
    }

    /**
     * Returns the in-degree of vertex v (number of edges pointing to v).
     *
     * @param v the vertex
     * @return the in-degree
     * @throws IndexOutOfBoundsException if v is out of range
     */
    public int getInDegree(int v) {
        validateVertex(v);
        int count = 0;
        for (int i = 0; i < numVertices; i++) {
            if (matrix[i][v] != NO_EDGE) {
                count++;
            }
        }
        return count;
    }

    /**
     * Returns the out-degree of vertex v (number of edges going out from v).
     *
     * @param v the vertex
     * @return the out-degree
     * @throws IndexOutOfBoundsException if v is out of range
     */
    public int getOutDegree(int v) {
        validateVertex(v);
        int count = 0;
        for (int j = 0; j < numVertices; j++) {
            if (matrix[v][j] != NO_EDGE) {
                count++;
            }
        }
        return count;
    }

    /**
     * Returns the degree of vertex v.
     * For directed graphs, this is the sum of in-degree and out-degree.
     * For undirected graphs, this is the out-degree (same as in-degree).
     *
     * @param v the vertex
     * @return the degree
     * @throws IndexOutOfBoundsException if v is out of range
     */
    public int getDegree(int v) {
        if (directed) {
            return getInDegree(v) + getOutDegree(v);
        }
        return getOutDegree(v);
    }

    /**
     * Returns the number of vertices in the graph.
     *
     * @return the vertex count
     */
    public int numVertices() {
        return numVertices;
    }

    /**
     * Returns the number of edges in the graph.
     *
     * @return the edge count
     */
    public int numEdges() {
        return numEdges;
    }

    /**
     * Returns whether the graph is directed.
     *
     * @return true if directed
     */
    public boolean isDirected() {
        return directed;
    }

    /**
     * Represents an edge in the graph.
     */
    public static class Edge {
        public final int from, to;
        public final double weight;

        public Edge(int from, int to, double weight) {
            this.from = from;
            this.to = to;
            this.weight = weight;
        }
    }

    /**
     * Updates the weight of an existing edge from u to v.
     * For undirected graphs, the reverse edge is also updated.
     *
     * @param u      the source vertex
     * @param v      the destination vertex
     * @param weight the new weight
     * @throws IndexOutOfBoundsException if u or v is out of range
     * @throws IllegalArgumentException  if the edge does not exist
     */
    public void setWeight(int u, int v, double weight) {
        validateVertex(u);
        validateVertex(v);
        if (!hasEdge(u, v)) {
            throw new IllegalArgumentException(
                    "Edge (" + u + ", " + v + ") does not exist");
        }
        matrix[u][v] = weight;
        if (!directed) {
            matrix[v][u] = weight;
        }
    }

    /**
     * Performs a breadth-first search starting from the given vertex.
     *
     * @param start the starting vertex
     * @return list of vertices in BFS order
     * @throws IndexOutOfBoundsException if start is out of range
     */
    public List<Integer> bfs(int start) {
        validateVertex(start);
        List<Integer> result = new ArrayList<>();
        boolean[] visited = new boolean[numVertices];
        Queue<Integer> queue = new ArrayDeque<>();
        visited[start] = true;
        queue.add(start);
        while (!queue.isEmpty()) {
            int v = queue.poll();
            result.add(v);
            for (int i = 0; i < numVertices; i++) {
                if (matrix[v][i] != NO_EDGE && !visited[i]) {
                    visited[i] = true;
                    queue.add(i);
                }
            }
        }
        return result;
    }

    /**
     * Performs an iterative depth-first search starting from the given vertex.
     *
     * @param start the starting vertex
     * @return list of vertices in DFS order
     * @throws IndexOutOfBoundsException if start is out of range
     */
    public List<Integer> dfs(int start) {
        validateVertex(start);
        List<Integer> result = new ArrayList<>();
        boolean[] visited = new boolean[numVertices];
        Deque<Integer> stack = new ArrayDeque<>();
        stack.push(start);
        while (!stack.isEmpty()) {
            int v = stack.pop();
            if (visited[v]) {
                continue;
            }
            visited[v] = true;
            result.add(v);
            // Push neighbors in reverse order so that lower-numbered vertices
            // are visited first (popped first from the stack).
            for (int i = numVertices - 1; i >= 0; i--) {
                if (matrix[v][i] != NO_EDGE && !visited[i]) {
                    stack.push(i);
                }
            }
        }
        return result;
    }

    /**
     * Computes all-pairs shortest distances using the Floyd-Warshall algorithm.
     * Uses {@link Double#POSITIVE_INFINITY} for unreachable pairs.
     *
     * @return a 2D array where [i][j] is the shortest distance from i to j
     */
    public double[][] floydWarshall() {
        double[][] dist = new double[numVertices][numVertices];
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    dist[i][j] = matrix[i][j];
                }
            }
        }
        for (int k = 0; k < numVertices; k++) {
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }

    /**
     * Computes the transitive closure of the graph.
     *
     * @return a 2D boolean array where [i][j] is true if there is a path from i to j
     */
    public boolean[][] transitiveClosure() {
        boolean[][] reach = new boolean[numVertices][numVertices];
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                reach[i][j] = (i == j) || (matrix[i][j] != NO_EDGE);
            }
        }
        for (int k = 0; k < numVertices; k++) {
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    if (reach[i][k] && reach[k][j]) {
                        reach[i][j] = true;
                    }
                }
            }
        }
        return reach;
    }

    /**
     * Checks whether there is a path from vertex u to vertex v using BFS.
     *
     * @param u the source vertex
     * @param v the destination vertex
     * @return true if a path exists from u to v
     * @throws IndexOutOfBoundsException if u or v is out of range
     */
    public boolean hasPath(int u, int v) {
        validateVertex(u);
        validateVertex(v);
        if (u == v) {
            return true;
        }
        boolean[] visited = new boolean[numVertices];
        Queue<Integer> queue = new ArrayDeque<>();
        visited[u] = true;
        queue.add(u);
        while (!queue.isEmpty()) {
            int curr = queue.poll();
            for (int i = 0; i < numVertices; i++) {
                if (matrix[curr][i] != NO_EDGE && !visited[i]) {
                    if (i == v) {
                        return true;
                    }
                    visited[i] = true;
                    queue.add(i);
                }
            }
        }
        return false;
    }

    /**
     * Returns all edges in the graph.
     * For undirected graphs, each edge is returned once (u &lt; v).
     *
     * @return list of edges
     */
    public List<Edge> getEdges() {
        List<Edge> edges = new ArrayList<>();
        for (int i = 0; i < numVertices; i++) {
            int start = directed ? 0 : i;
            for (int j = start; j < numVertices; j++) {
                if (matrix[i][j] != NO_EDGE) {
                    edges.add(new Edge(i, j, matrix[i][j]));
                }
            }
        }
        return edges;
    }

    private void validateVertex(int v) {
        if (v < 0 || v >= numVertices) {
            throw new IndexOutOfBoundsException(
                    "Vertex " + v + " is out of range [0, " + (numVertices - 1) + "]");
        }
    }
}
