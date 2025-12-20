"""
Adjacency Matrix Graph Representation

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  For a graph with vertices {0, 1, 2, 3}:                                   ║
║                                                                            ║
║      0 ──5── 1                    0   1   2   3                            ║
║      │       │                 0 [ 0   5   0   2 ]                         ║
║      2       3                 1 [ 5   0   3   0 ]                         ║
║      │       │                 2 [ 0   3   0   0 ]                         ║
║      3 ──────┘                 3 [ 2   0   0   0 ]                         ║
║                                                                            ║
║  0 = no edge, value = edge weight                                          ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

╔════════════════════════════════════════════════════════════════════════════╗
║                    COMPARISON WITH ADJACENCY LIST                          ║
╠════════════════════════════════════════════════════════════════════════════╣
║  Operation         │ Adjacency Matrix │ Adjacency List                     ║
║  ──────────────────┼──────────────────┼────────────────────────────        ║
║  Space             │ O(V²)            │ O(V + E)                           ║
║  Add Edge          │ O(1)             │ O(1)                               ║
║  Remove Edge       │ O(1)             │ O(E)                               ║
║  Check Edge        │ O(1)             │ O(degree)                          ║
║  Get Neighbors     │ O(V)             │ O(degree)                          ║
║  Iterate All Edges │ O(V²)            │ O(E)                               ║
╚════════════════════════════════════════════════════════════════════════════╝

USE CASES:
- Dense graphs (E ≈ V²)
- Need O(1) edge existence checks
- Small to medium sized graphs
- Floyd-Warshall algorithm
- Transitive closure

LEETCODE PROBLEMS:
- #785 Is Graph Bipartite?
- #797 All Paths From Source to Target
- #1557 Minimum Number of Vertices to Reach All Nodes
"""

from typing import List, Optional, Tuple, Set, Iterator
from collections import deque


class AdjacencyMatrix:
    """
    Graph implementation using adjacency matrix.

    Provides O(1) edge lookup and is optimal for dense graphs.
    Supports both directed and undirected graphs, weighted and unweighted edges.
    """

    def __init__(
        self,
        num_vertices: int,
        directed: bool = False,
        weighted: bool = False
    ) -> None:
        """
        Initialize an adjacency matrix graph.

        Args:
            num_vertices: Number of vertices (0 to num_vertices-1).
            directed: If True, creates a directed graph.
            weighted: If True, creates a weighted graph.
        """
        if num_vertices < 0:
            raise ValueError("Number of vertices must be non-negative")

        self._num_vertices = num_vertices
        self._directed = directed
        self._weighted = weighted
        self._matrix: List[List[int]] = [
            [0] * num_vertices for _ in range(num_vertices)
        ]

    def __len__(self) -> int:
        """Return number of vertices."""
        return self._num_vertices

    def __repr__(self) -> str:
        """String representation."""
        graph_type = "Directed" if self._directed else "Undirected"
        weight_type = "Weighted" if self._weighted else "Unweighted"
        return (
            f"AdjacencyMatrix({graph_type}, {weight_type}, "
            f"vertices={self._num_vertices}, edges={self.edge_count})"
        )

    def __contains__(self, vertex: int) -> bool:
        """Check if vertex exists."""
        return 0 <= vertex < self._num_vertices

    def __iter__(self) -> Iterator[int]:
        """Iterate over vertices."""
        return iter(range(self._num_vertices))

    @property
    def is_directed(self) -> bool:
        """Return True if graph is directed."""
        return self._directed

    @property
    def is_weighted(self) -> bool:
        """Return True if graph is weighted."""
        return self._weighted

    @property
    def num_vertices(self) -> int:
        """Return number of vertices."""
        return self._num_vertices

    @property
    def edge_count(self) -> int:
        """Return number of edges."""
        if self._directed:
            return sum(
                1 for i in range(self._num_vertices)
                for j in range(self._num_vertices)
                if self._matrix[i][j] != 0
            )
        else:
            # For undirected: count upper triangle + diagonal
            count = 0
            for i in range(self._num_vertices):
                for j in range(i, self._num_vertices):
                    if self._matrix[i][j] != 0:
                        count += 1
            return count

    @property
    def matrix(self) -> List[List[int]]:
        """Return copy of the adjacency matrix."""
        return [row[:] for row in self._matrix]

    # =========================================================================
    # EDGE OPERATIONS
    # =========================================================================

    def add_edge(self, src: int, dest: int, weight: int = 1) -> bool:
        """
        Add an edge to the graph.

        Args:
            src: Source vertex.
            dest: Destination vertex.
            weight: Edge weight (default 1).

        Returns:
            True on success, False if vertices are invalid.
        """
        if not self._is_valid_vertex(src) or not self._is_valid_vertex(dest):
            return False
        if weight == 0:
            return False

        self._matrix[src][dest] = weight

        if not self._directed:
            self._matrix[dest][src] = weight

        return True

    def remove_edge(self, src: int, dest: int) -> bool:
        """
        Remove an edge from the graph.

        Args:
            src: Source vertex.
            dest: Destination vertex.

        Returns:
            True if edge existed and was removed.
        """
        if not self._is_valid_vertex(src) or not self._is_valid_vertex(dest):
            return False

        had_edge = self._matrix[src][dest] != 0
        self._matrix[src][dest] = 0

        if not self._directed:
            self._matrix[dest][src] = 0

        return had_edge

    def has_edge(self, src: int, dest: int) -> bool:
        """
        Check if edge exists.

        Args:
            src: Source vertex.
            dest: Destination vertex.

        Returns:
            True if edge exists.
        """
        if not self._is_valid_vertex(src) or not self._is_valid_vertex(dest):
            return False
        return self._matrix[src][dest] != 0

    def get_weight(self, src: int, dest: int) -> int:
        """
        Get edge weight.

        Args:
            src: Source vertex.
            dest: Destination vertex.

        Returns:
            Edge weight or 0 if no edge.
        """
        if not self._is_valid_vertex(src) or not self._is_valid_vertex(dest):
            return 0
        return self._matrix[src][dest]

    def set_weight(self, src: int, dest: int, weight: int) -> bool:
        """
        Set edge weight.

        Args:
            src: Source vertex.
            dest: Destination vertex.
            weight: New weight.

        Returns:
            True on success.
        """
        return self.add_edge(src, dest, weight)

    # =========================================================================
    # VERTEX PROPERTIES
    # =========================================================================

    def out_degree(self, vertex: int) -> int:
        """
        Get out-degree of a vertex.

        Args:
            vertex: The vertex.

        Returns:
            Number of outgoing edges.
        """
        if not self._is_valid_vertex(vertex):
            return 0
        return sum(1 for j in range(self._num_vertices) if self._matrix[vertex][j] != 0)

    def in_degree(self, vertex: int) -> int:
        """
        Get in-degree of a vertex.

        Args:
            vertex: The vertex.

        Returns:
            Number of incoming edges.
        """
        if not self._is_valid_vertex(vertex):
            return 0
        return sum(1 for i in range(self._num_vertices) if self._matrix[i][vertex] != 0)

    def degree(self, vertex: int) -> int:
        """
        Get degree of a vertex.

        For directed graphs, returns out-degree.

        Args:
            vertex: The vertex.

        Returns:
            Degree of vertex.
        """
        return self.out_degree(vertex)

    def neighbors(self, vertex: int) -> List[int]:
        """
        Get neighbors of a vertex.

        Args:
            vertex: The vertex.

        Returns:
            List of neighboring vertices.
        """
        if not self._is_valid_vertex(vertex):
            return []
        return [j for j in range(self._num_vertices) if self._matrix[vertex][j] != 0]

    def neighbors_weighted(self, vertex: int) -> List[Tuple[int, int]]:
        """
        Get neighbors with weights.

        Args:
            vertex: The vertex.

        Returns:
            List of (neighbor, weight) tuples.
        """
        if not self._is_valid_vertex(vertex):
            return []
        return [
            (j, self._matrix[vertex][j])
            for j in range(self._num_vertices)
            if self._matrix[vertex][j] != 0
        ]

    # =========================================================================
    # GRAPH TRAVERSAL
    # =========================================================================

    def bfs(self, source: int) -> List[int]:
        """
        Breadth-first search traversal.

        Time Complexity: O(V²)
        Space Complexity: O(V)

        Args:
            source: Starting vertex.

        Returns:
            List of vertices in BFS order.
        """
        if not self._is_valid_vertex(source):
            return []

        visited = [False] * self._num_vertices
        result: List[int] = []
        queue: deque[int] = deque([source])
        visited[source] = True

        while queue:
            vertex = queue.popleft()
            result.append(vertex)

            for j in range(self._num_vertices):
                if self._matrix[vertex][j] != 0 and not visited[j]:
                    visited[j] = True
                    queue.append(j)

        return result

    def dfs(self, source: int) -> List[int]:
        """
        Depth-first search traversal (iterative).

        Time Complexity: O(V²)
        Space Complexity: O(V)

        Args:
            source: Starting vertex.

        Returns:
            List of vertices in DFS order.
        """
        if not self._is_valid_vertex(source):
            return []

        visited = [False] * self._num_vertices
        result: List[int] = []
        stack: List[int] = [source]

        while stack:
            vertex = stack.pop()

            if not visited[vertex]:
                visited[vertex] = True
                result.append(vertex)

                # Add neighbors in reverse order for consistent ordering
                for j in range(self._num_vertices - 1, -1, -1):
                    if self._matrix[vertex][j] != 0 and not visited[j]:
                        stack.append(j)

        return result

    def dfs_recursive(self, source: int) -> List[int]:
        """
        Depth-first search traversal (recursive).

        Args:
            source: Starting vertex.

        Returns:
            List of vertices in DFS order.
        """
        if not self._is_valid_vertex(source):
            return []

        visited = [False] * self._num_vertices
        result: List[int] = []

        def _dfs(vertex: int) -> None:
            visited[vertex] = True
            result.append(vertex)
            for j in range(self._num_vertices):
                if self._matrix[vertex][j] != 0 and not visited[j]:
                    _dfs(j)

        _dfs(source)
        return result

    # =========================================================================
    # GRAPH PROPERTIES
    # =========================================================================

    def is_connected(self) -> bool:
        """
        Check if graph is connected (undirected) or weakly connected (directed).

        Returns:
            True if all vertices are reachable from vertex 0.
        """
        if self._num_vertices == 0:
            return True

        visited_count = len(self.bfs(0))
        return visited_count == self._num_vertices

    # =========================================================================
    # ALL-PAIRS SHORTEST PATH
    # =========================================================================

    def floyd_warshall(self) -> Tuple[List[List[float]], bool]:
        """
        Floyd-Warshall algorithm for all-pairs shortest paths.

        Time Complexity: O(V³)
        Space Complexity: O(V²)

        Returns:
            Tuple of (distance matrix, success flag).
            success is False if negative cycle detected.
        """
        n = self._num_vertices
        INF = float('inf')

        # Initialize distance matrix
        dist: List[List[float]] = [[INF] * n for _ in range(n)]

        for i in range(n):
            dist[i][i] = 0

        for i in range(n):
            for j in range(n):
                if self._matrix[i][j] != 0:
                    dist[i][j] = self._matrix[i][j]

        # Floyd-Warshall
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if dist[i][k] != INF and dist[k][j] != INF:
                        if dist[i][k] + dist[k][j] < dist[i][j]:
                            dist[i][j] = dist[i][k] + dist[k][j]

        # Check for negative cycles
        for i in range(n):
            if dist[i][i] < 0:
                return dist, False

        return dist, True

    def transitive_closure(self) -> List[List[bool]]:
        """
        Compute transitive closure using Floyd-Warshall.

        Time Complexity: O(V³)
        Space Complexity: O(V²)

        Returns:
            Boolean matrix where closure[i][j] = True if j is reachable from i.
        """
        n = self._num_vertices

        # Initialize closure
        closure = [
            [self._matrix[i][j] != 0 or i == j for j in range(n)]
            for i in range(n)
        ]

        # Floyd-Warshall for reachability
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    closure[i][j] = closure[i][j] or (closure[i][k] and closure[k][j])

        return closure

    # =========================================================================
    # UTILITY METHODS
    # =========================================================================

    def clear(self) -> None:
        """Remove all edges."""
        for i in range(self._num_vertices):
            for j in range(self._num_vertices):
                self._matrix[i][j] = 0

    def copy(self) -> "AdjacencyMatrix":
        """Create a deep copy of the graph."""
        new_graph = AdjacencyMatrix(
            self._num_vertices,
            directed=self._directed,
            weighted=self._weighted
        )
        for i in range(self._num_vertices):
            for j in range(self._num_vertices):
                new_graph._matrix[i][j] = self._matrix[i][j]
        return new_graph

    def transpose(self) -> "AdjacencyMatrix":
        """
        Get transpose of directed graph.

        Returns:
            New graph with all edges reversed.
        """
        trans = AdjacencyMatrix(
            self._num_vertices,
            directed=self._directed,
            weighted=self._weighted
        )
        for i in range(self._num_vertices):
            for j in range(self._num_vertices):
                trans._matrix[j][i] = self._matrix[i][j]
        return trans

    def _is_valid_vertex(self, vertex: int) -> bool:
        """Check if vertex is valid."""
        return 0 <= vertex < self._num_vertices

    def __str__(self) -> str:
        """Pretty print the matrix."""
        if self._num_vertices == 0:
            return "(empty graph)"

        lines = [
            f"Adjacency Matrix ({self._num_vertices} vertices, "
            f"{'directed' if self._directed else 'undirected'}, "
            f"{'weighted' if self._weighted else 'unweighted'}):"
        ]

        # Header
        header = "    " + "".join(f"{j:4d}" for j in range(self._num_vertices))
        lines.append(header)

        # Rows
        for i in range(self._num_vertices):
            row = f"{i:3d}:"
            for j in range(self._num_vertices):
                if self._matrix[i][j] == 0:
                    row += "   ."
                else:
                    row += f"{self._matrix[i][j]:4d}"
            lines.append(row)

        return "\n".join(lines)
