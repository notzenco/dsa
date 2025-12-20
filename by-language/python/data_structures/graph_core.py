"""
Graph Core Data Structure

Core graph operations using adjacency list representation.
For algorithms (traversal, shortest path, MST, SCC), see graph_algorithms.py.

COMPLEXITY (V = vertices, E = edges):
+---------------------+-----------------+-----------------+
| Operation           | Adjacency List  | Adjacency Matrix|
+---------------------+-----------------+-----------------+
| Space               | O(V + E)        | O(V^2)          |
| Add Vertex          | O(1)            | O(V^2)          |
| Add Edge            | O(1)            | O(1)            |
| Remove Edge         | O(E)            | O(1)            |
| Check Edge          | O(degree)       | O(1)            |
| Get Neighbors       | O(1)            | O(V)            |
+---------------------+-----------------+-----------------+
"""

from typing import TypeVar, Generic, Dict, List, Optional, Tuple, Iterator
from collections import defaultdict

T = TypeVar('T')


class GraphCore(Generic[T]):
    """
    Graph core implementation using adjacency list.

    Supports both directed and undirected graphs, weighted and unweighted edges.
    """

    def __init__(self, directed: bool = False) -> None:
        """
        Initialize an empty graph.

        Args:
            directed: If True, creates a directed graph.
        """
        self._adj: Dict[T, Dict[T, float]] = defaultdict(dict)
        self._directed = directed

    def __len__(self) -> int:
        """Return number of vertices."""
        return len(self._adj)

    def __contains__(self, vertex: T) -> bool:
        """Check if vertex exists."""
        return vertex in self._adj

    def __iter__(self) -> Iterator[T]:
        """Iterate over vertices."""
        return iter(self._adj)

    def __repr__(self) -> str:
        """String representation."""
        graph_type = "Directed" if self._directed else "Undirected"
        return f"{graph_type}Graph(vertices={len(self)}, edges={self.edge_count})"

    @property
    def is_directed(self) -> bool:
        """Return True if graph is directed."""
        return self._directed

    @property
    def vertices(self) -> List[T]:
        """Return list of all vertices."""
        return list(self._adj.keys())

    @property
    def edge_count(self) -> int:
        """Return number of edges."""
        count = sum(len(neighbors) for neighbors in self._adj.values())
        return count if self._directed else count // 2

    def add_vertex(self, vertex: T) -> None:
        """
        Add a vertex to the graph.

        Args:
            vertex: Vertex to add.
        """
        if vertex not in self._adj:
            self._adj[vertex] = {}

    def add_edge(self, u: T, v: T, weight: float = 1.0) -> None:
        """
        Add an edge between two vertices.

        Args:
            u: Source vertex.
            v: Destination vertex.
            weight: Edge weight (default 1.0).
        """
        self.add_vertex(u)
        self.add_vertex(v)

        self._adj[u][v] = weight

        if not self._directed:
            self._adj[v][u] = weight

    def remove_edge(self, u: T, v: T) -> bool:
        """
        Remove an edge between two vertices.

        Args:
            u: Source vertex.
            v: Destination vertex.

        Returns:
            True if edge existed and was removed.
        """
        if u not in self._adj or v not in self._adj[u]:
            return False

        del self._adj[u][v]

        if not self._directed:
            del self._adj[v][u]

        return True

    def remove_vertex(self, vertex: T) -> bool:
        """
        Remove a vertex and all its edges.

        Args:
            vertex: Vertex to remove.

        Returns:
            True if vertex existed and was removed.
        """
        if vertex not in self._adj:
            return False

        # Remove all edges to this vertex
        for v in self._adj:
            if vertex in self._adj[v]:
                del self._adj[v][vertex]

        del self._adj[vertex]
        return True

    def has_edge(self, u: T, v: T) -> bool:
        """
        Check if edge exists.

        Args:
            u: Source vertex.
            v: Destination vertex.

        Returns:
            True if edge exists.
        """
        return u in self._adj and v in self._adj[u]

    def get_weight(self, u: T, v: T) -> Optional[float]:
        """
        Get weight of edge.

        Args:
            u: Source vertex.
            v: Destination vertex.

        Returns:
            Edge weight or None if no edge.
        """
        if u in self._adj and v in self._adj[u]:
            return self._adj[u][v]
        return None

    def neighbors(self, vertex: T) -> List[T]:
        """
        Get neighbors of a vertex.

        Args:
            vertex: The vertex.

        Returns:
            List of neighboring vertices.
        """
        if vertex not in self._adj:
            return []
        return list(self._adj[vertex].keys())

    def degree(self, vertex: T) -> int:
        """
        Get degree of a vertex.

        For directed graphs, returns out-degree.

        Args:
            vertex: The vertex.

        Returns:
            Number of edges from this vertex.
        """
        if vertex not in self._adj:
            return 0
        return len(self._adj[vertex])

    def in_degree(self, vertex: T) -> int:
        """
        Get in-degree of a vertex (directed graphs).

        Args:
            vertex: The vertex.

        Returns:
            Number of edges into this vertex.
        """
        if vertex not in self._adj:
            return 0
        return sum(1 for v in self._adj if vertex in self._adj[v])

    def clear(self) -> None:
        """Remove all vertices and edges."""
        self._adj.clear()

    def copy(self) -> "GraphCore[T]":
        """Create a deep copy of the graph."""
        new_graph: GraphCore[T] = GraphCore(directed=self._directed)
        for u in self._adj:
            for v, weight in self._adj[u].items():
                if self._directed or u <= v:  # Avoid duplicate edges in undirected
                    new_graph.add_edge(u, v, weight)
        return new_graph

    def to_adjacency_matrix(self) -> Tuple[List[T], List[List[float]]]:
        """
        Convert to adjacency matrix representation.

        Returns:
            Tuple of (vertex list, matrix) where matrix[i][j] is weight.
        """
        vertices = list(self._adj.keys())
        n = len(vertices)
        index = {v: i for i, v in enumerate(vertices)}

        matrix = [[float('inf')] * n for _ in range(n)]

        for i in range(n):
            matrix[i][i] = 0

        for u in self._adj:
            for v, weight in self._adj[u].items():
                matrix[index[u]][index[v]] = weight

        return vertices, matrix

    @classmethod
    def from_edge_list(
        cls,
        edges: List[Tuple[T, T, float]],
        directed: bool = False
    ) -> "GraphCore[T]":
        """
        Create graph from edge list.

        Args:
            edges: List of (u, v, weight) tuples.
            directed: If True, create directed graph.

        Returns:
            New GraphCore.
        """
        graph: GraphCore[T] = cls(directed=directed)
        for edge in edges:
            if len(edge) == 3:
                u, v, weight = edge
            else:
                u, v = edge  # type: ignore
                weight = 1.0
            graph.add_edge(u, v, weight)
        return graph
