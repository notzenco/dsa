package dsa.datastructures;

import java.util.ArrayList;
import java.util.List;

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
     */
    public void addEdge(int u, int v, double weight) {
        validateVertex(u);
        validateVertex(v);
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

    private void validateVertex(int v) {
        if (v < 0 || v >= numVertices) {
            throw new IndexOutOfBoundsException(
                    "Vertex " + v + " is out of range [0, " + (numVertices - 1) + "]");
        }
    }
}
