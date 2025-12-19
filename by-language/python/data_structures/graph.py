"""
Graph Data Structure

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Undirected Graph:              Directed Graph (Digraph):                  ║
║       (0)                            (0)                                   ║
║      / | \                          ↙   ↘                                  ║
║    (1)-(2)-(3)                    (1)    (2)                               ║
║      \ |                           ↓    ↙                                  ║
║       (4)                         (3)←(4)                                  ║
║                                                                            ║
║  Adjacency List Representation:                                            ║
║  ┌───────┬────────────────────┐                                            ║
║  │ Vertex│ Neighbors          │                                            ║
║  ├───────┼────────────────────┤                                            ║
║  │   0   │ [1, 2, 3]          │                                            ║
║  │   1   │ [0, 2, 4]          │                                            ║
║  │   2   │ [0, 1, 3, 4]       │                                            ║
║  │   3   │ [0, 2]             │                                            ║
║  │   4   │ [1, 2]             │                                            ║
║  └───────┴────────────────────┘                                            ║
║                                                                            ║
║  Adjacency Matrix Representation:                                          ║
║      0  1  2  3  4                                                         ║
║    ┌─────────────────┐                                                     ║
║  0 │ 0  1  1  1  0   │                                                     ║
║  1 │ 1  0  1  0  1   │                                                     ║
║  2 │ 1  1  0  1  1   │                                                     ║
║  3 │ 1  0  1  0  0   │                                                     ║
║  4 │ 0  1  1  0  0   │                                                     ║
║    └─────────────────┘                                                     ║
║                                                                            ║
║  Weighted Graph (Adjacency List):                                          ║
║  ┌───────┬──────────────────────────┐                                      ║
║  │ Vertex│ (Neighbor, Weight)       │                                      ║
║  ├───────┼──────────────────────────┤                                      ║
║  │   A   │ [(B, 4), (C, 2)]         │                                      ║
║  │   B   │ [(A, 4), (C, 1), (D, 5)] │                                      ║
║  │   C   │ [(A, 2), (B, 1), (D, 8)] │                                      ║
║  │   D   │ [(B, 5), (C, 8)]         │                                      ║
║  └───────┴──────────────────────────┘                                      ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY (V = vertices, E = edges):
┌─────────────────────┬─────────────────┬─────────────────┐
│ Operation           │ Adjacency List  │ Adjacency Matrix│
├─────────────────────┼─────────────────┼─────────────────┤
│ Space               │ O(V + E)        │ O(V²)           │
│ Add Vertex          │ O(1)            │ O(V²)           │
│ Add Edge            │ O(1)            │ O(1)            │
│ Remove Edge         │ O(E)            │ O(1)            │
│ Check Edge          │ O(degree)       │ O(1)            │
│ Get Neighbors       │ O(1)            │ O(V)            │
│ BFS/DFS             │ O(V + E)        │ O(V²)           │
└─────────────────────┴─────────────────┴─────────────────┘

LEETCODE PROBLEMS:
- #133 Clone Graph
- #200 Number of Islands
- #207 Course Schedule
- #210 Course Schedule II
- #261 Graph Valid Tree
- #269 Alien Dictionary
- #323 Number of Connected Components
- #417 Pacific Atlantic Water Flow
- #547 Number of Provinces
- #684 Redundant Connection
- #743 Network Delay Time
- #785 Is Graph Bipartite?
- #797 All Paths From Source to Target
- #841 Keys and Rooms
- #994 Rotting Oranges
- #1091 Shortest Path in Binary Matrix

USE CASES:
- Social networks (friends, followers)
- Road networks and navigation
- Web page linking (PageRank)
- Dependency resolution
- Network topology
- Recommendation systems
- Circuit design
"""

from typing import TypeVar, Generic, Dict, List, Set, Optional, Tuple, Iterator
from collections import deque, defaultdict
import heapq

T = TypeVar('T')


class Graph(Generic[T]):
    """
    Graph implementation using adjacency list.

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

    # =========================================================================
    # GRAPH TRAVERSAL
    # =========================================================================

    def bfs(self, start: T) -> List[T]:
        """
        Breadth-first search traversal.

        Time Complexity: O(V + E)
        Space Complexity: O(V)

        Args:
            start: Starting vertex.

        Returns:
            List of vertices in BFS order.
        """
        if start not in self._adj:
            return []

        visited: Set[T] = set()
        result: List[T] = []
        queue: deque[T] = deque([start])
        visited.add(start)

        while queue:
            vertex = queue.popleft()
            result.append(vertex)

            for neighbor in self._adj[vertex]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append(neighbor)

        return result

    def dfs(self, start: T) -> List[T]:
        """
        Depth-first search traversal (iterative).

        Time Complexity: O(V + E)
        Space Complexity: O(V)

        Args:
            start: Starting vertex.

        Returns:
            List of vertices in DFS order.
        """
        if start not in self._adj:
            return []

        visited: Set[T] = set()
        result: List[T] = []
        stack: List[T] = [start]

        while stack:
            vertex = stack.pop()

            if vertex not in visited:
                visited.add(vertex)
                result.append(vertex)

                # Add neighbors in reverse order for consistent ordering
                for neighbor in reversed(list(self._adj[vertex].keys())):
                    if neighbor not in visited:
                        stack.append(neighbor)

        return result

    def dfs_recursive(self, start: T) -> List[T]:
        """
        Depth-first search traversal (recursive).

        Args:
            start: Starting vertex.

        Returns:
            List of vertices in DFS order.
        """
        if start not in self._adj:
            return []

        visited: Set[T] = set()
        result: List[T] = []

        def _dfs(vertex: T) -> None:
            visited.add(vertex)
            result.append(vertex)
            for neighbor in self._adj[vertex]:
                if neighbor not in visited:
                    _dfs(neighbor)

        _dfs(start)
        return result

    # =========================================================================
    # SHORTEST PATH ALGORITHMS
    # =========================================================================

    def shortest_path_bfs(self, start: T, end: T) -> Optional[List[T]]:
        """
        Find shortest path using BFS (unweighted graphs).

        Time Complexity: O(V + E)

        Args:
            start: Starting vertex.
            end: Destination vertex.

        Returns:
            List of vertices in path, or None if no path exists.
        """
        if start not in self._adj or end not in self._adj:
            return None

        if start == end:
            return [start]

        visited: Set[T] = {start}
        queue: deque[Tuple[T, List[T]]] = deque([(start, [start])])

        while queue:
            vertex, path = queue.popleft()

            for neighbor in self._adj[vertex]:
                if neighbor == end:
                    return path + [neighbor]

                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append((neighbor, path + [neighbor]))

        return None

    def dijkstra(self, start: T) -> Tuple[Dict[T, float], Dict[T, Optional[T]]]:
        """
        Dijkstra's algorithm for shortest paths from start to all vertices.

        Time Complexity: O((V + E) log V)

        Args:
            start: Starting vertex.

        Returns:
            Tuple of (distances dict, predecessors dict).
        """
        if start not in self._adj:
            return {}, {}

        distances: Dict[T, float] = {v: float('inf') for v in self._adj}
        distances[start] = 0
        predecessors: Dict[T, Optional[T]] = {v: None for v in self._adj}

        # Priority queue: (distance, vertex)
        pq: List[Tuple[float, T]] = [(0, start)]
        visited: Set[T] = set()

        while pq:
            dist, vertex = heapq.heappop(pq)

            if vertex in visited:
                continue

            visited.add(vertex)

            for neighbor, weight in self._adj[vertex].items():
                if neighbor in visited:
                    continue

                new_dist = dist + weight

                if new_dist < distances[neighbor]:
                    distances[neighbor] = new_dist
                    predecessors[neighbor] = vertex
                    heapq.heappush(pq, (new_dist, neighbor))

        return distances, predecessors

    def shortest_path_dijkstra(self, start: T, end: T) -> Optional[Tuple[float, List[T]]]:
        """
        Find shortest path using Dijkstra's algorithm.

        Args:
            start: Starting vertex.
            end: Destination vertex.

        Returns:
            Tuple of (distance, path) or None if no path exists.
        """
        distances, predecessors = self.dijkstra(start)

        if end not in distances or distances[end] == float('inf'):
            return None

        # Reconstruct path
        path: List[T] = []
        current: Optional[T] = end

        while current is not None:
            path.append(current)
            current = predecessors[current]

        return (distances[end], path[::-1])

    def bellman_ford(self, start: T) -> Optional[Tuple[Dict[T, float], Dict[T, Optional[T]]]]:
        """
        Bellman-Ford algorithm (handles negative weights).

        Time Complexity: O(V * E)

        Args:
            start: Starting vertex.

        Returns:
            Tuple of (distances, predecessors) or None if negative cycle exists.
        """
        if start not in self._adj:
            return None

        distances: Dict[T, float] = {v: float('inf') for v in self._adj}
        distances[start] = 0
        predecessors: Dict[T, Optional[T]] = {v: None for v in self._adj}

        vertices = list(self._adj.keys())

        # Relax edges V-1 times
        for _ in range(len(vertices) - 1):
            for u in self._adj:
                for v, weight in self._adj[u].items():
                    if distances[u] != float('inf') and distances[u] + weight < distances[v]:
                        distances[v] = distances[u] + weight
                        predecessors[v] = u

        # Check for negative cycles
        for u in self._adj:
            for v, weight in self._adj[u].items():
                if distances[u] != float('inf') and distances[u] + weight < distances[v]:
                    return None  # Negative cycle detected

        return distances, predecessors

    # =========================================================================
    # GRAPH PROPERTIES
    # =========================================================================

    def is_connected(self) -> bool:
        """
        Check if graph is connected (undirected) or weakly connected (directed).

        Returns:
            True if all vertices are reachable from any vertex.
        """
        if len(self._adj) == 0:
            return True

        start = next(iter(self._adj))
        visited = set(self.bfs(start))

        return len(visited) == len(self._adj)

    def connected_components(self) -> List[Set[T]]:
        """
        Find all connected components.

        Returns:
            List of sets, each containing vertices in a component.
        """
        visited: Set[T] = set()
        components: List[Set[T]] = []

        for vertex in self._adj:
            if vertex not in visited:
                component = set(self.bfs(vertex))
                visited.update(component)
                components.append(component)

        return components

    def has_cycle(self) -> bool:
        """
        Check if graph contains a cycle.

        Returns:
            True if cycle exists.
        """
        visited: Set[T] = set()
        rec_stack: Set[T] = set()

        def _has_cycle_util(vertex: T, parent: Optional[T]) -> bool:
            visited.add(vertex)

            if self._directed:
                rec_stack.add(vertex)

            for neighbor in self._adj[vertex]:
                if neighbor not in visited:
                    if _has_cycle_util(neighbor, vertex):
                        return True
                elif self._directed:
                    if neighbor in rec_stack:
                        return True
                else:
                    if neighbor != parent:
                        return True

            if self._directed:
                rec_stack.remove(vertex)

            return False

        for vertex in self._adj:
            if vertex not in visited:
                if _has_cycle_util(vertex, None):
                    return True

        return False

    def topological_sort(self) -> Optional[List[T]]:
        """
        Topological sort (Kahn's algorithm) for directed acyclic graphs.

        Returns:
            List of vertices in topological order, or None if cycle exists.
        """
        if not self._directed:
            return None

        in_degrees = {v: 0 for v in self._adj}
        for u in self._adj:
            for v in self._adj[u]:
                in_degrees[v] += 1

        queue: deque[T] = deque([v for v in in_degrees if in_degrees[v] == 0])
        result: List[T] = []

        while queue:
            vertex = queue.popleft()
            result.append(vertex)

            for neighbor in self._adj[vertex]:
                in_degrees[neighbor] -= 1
                if in_degrees[neighbor] == 0:
                    queue.append(neighbor)

        if len(result) != len(self._adj):
            return None  # Cycle detected

        return result

    def is_bipartite(self) -> bool:
        """
        Check if graph is bipartite (2-colorable).

        Returns:
            True if graph is bipartite.
        """
        color: Dict[T, int] = {}

        for start in self._adj:
            if start in color:
                continue

            queue: deque[T] = deque([start])
            color[start] = 0

            while queue:
                vertex = queue.popleft()

                for neighbor in self._adj[vertex]:
                    if neighbor not in color:
                        color[neighbor] = 1 - color[vertex]
                        queue.append(neighbor)
                    elif color[neighbor] == color[vertex]:
                        return False

        return True

    # =========================================================================
    # UTILITY METHODS
    # =========================================================================

    def clear(self) -> None:
        """Remove all vertices and edges."""
        self._adj.clear()

    def copy(self) -> "Graph[T]":
        """Create a deep copy of the graph."""
        new_graph: Graph[T] = Graph(directed=self._directed)
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
    ) -> "Graph[T]":
        """
        Create graph from edge list.

        Args:
            edges: List of (u, v, weight) tuples.
            directed: If True, create directed graph.

        Returns:
            New Graph.
        """
        graph: Graph[T] = cls(directed=directed)
        for edge in edges:
            if len(edge) == 3:
                u, v, weight = edge
            else:
                u, v = edge  # type: ignore
                weight = 1.0
            graph.add_edge(u, v, weight)
        return graph
