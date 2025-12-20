"""
Graph Algorithms

Mixin class providing graph algorithms for traversal, shortest paths,
minimum spanning trees, strongly connected components, and more.

ALGORITHM COMPLEXITY (V = vertices, E = edges):
+---------------------+----------------+---------+-------------------------+
| Algorithm           | Time           | Space   | Notes                   |
+---------------------+----------------+---------+-------------------------+
| BFS                 | O(V + E)       | O(V)    | Unweighted shortest path|
| DFS                 | O(V + E)       | O(V)    | Cycle detection, topo   |
| Dijkstra            | O((V+E)log V)  | O(V)    | Non-negative weights    |
| Bellman-Ford        | O(V * E)       | O(V)    | Handles negative weights|
| Topological Sort    | O(V + E)       | O(V)    | DAG only                |
| Kruskal MST         | O(E log E)     | O(V)    | Uses Union-Find         |
| Prim MST            | O((V+E)log V)  | O(V)    | Uses Min-Heap           |
| Tarjan SCC          | O(V + E)       | O(V)    | Strongly connected comp |
| Kosaraju SCC        | O(V + E)       | O(V)    | Two DFS passes          |
| Floyd-Warshall      | O(V^3)         | O(V^2)  | All-pairs shortest path |
+---------------------+----------------+---------+-------------------------+

LEETCODE PROBLEMS:
- #133 Clone Graph
- #200 Number of Islands
- #207 Course Schedule
- #210 Course Schedule II
- #743 Network Delay Time
- #787 Cheapest Flights Within K Stops
- #1334 Find the City With the Smallest Number of Neighbors
- #1584 Min Cost to Connect All Points
"""

from typing import TypeVar, Dict, List, Set, Optional, Tuple
from collections import deque, defaultdict
import heapq

T = TypeVar('T')


class GraphAlgorithmsMixin:
    """
    Mixin class providing graph algorithms.

    Requires the class to have:
    - self._adj: Dict[T, Dict[T, float]] - adjacency list
    - self._directed: bool - whether graph is directed
    """

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
    # MINIMUM SPANNING TREE ALGORITHMS
    # =========================================================================

    def prim_mst(self, start: Optional[T] = None) -> Tuple[List[Tuple[T, T, float]], float]:
        """
        Prim's algorithm for Minimum Spanning Tree.

        Time Complexity: O((V + E) log V) with binary heap
        Space Complexity: O(V)

        LeetCode: #1584 Min Cost to Connect All Points

        Args:
            start: Starting vertex (defaults to first vertex).

        Returns:
            Tuple of (list of MST edges, total weight).
        """
        if len(self._adj) == 0:
            return [], 0.0

        if self._directed:
            raise ValueError("MST is only defined for undirected graphs")

        if start is None:
            start = next(iter(self._adj))

        mst_edges: List[Tuple[T, T, float]] = []
        total_weight = 0.0
        visited: Set[T] = {start}

        # Priority queue: (weight, from_vertex, to_vertex)
        edges_pq: List[Tuple[float, T, T]] = []

        # Add all edges from start vertex
        for neighbor, weight in self._adj[start].items():
            heapq.heappush(edges_pq, (weight, start, neighbor))

        while edges_pq and len(visited) < len(self._adj):
            weight, u, v = heapq.heappop(edges_pq)

            if v in visited:
                continue

            visited.add(v)
            mst_edges.append((u, v, weight))
            total_weight += weight

            # Add all edges from newly added vertex
            for neighbor, edge_weight in self._adj[v].items():
                if neighbor not in visited:
                    heapq.heappush(edges_pq, (edge_weight, v, neighbor))

        return mst_edges, total_weight

    def kruskal_mst(self) -> Tuple[List[Tuple[T, T, float]], float]:
        """
        Kruskal's algorithm for Minimum Spanning Tree.

        Uses Union-Find for efficient cycle detection.

        Time Complexity: O(E log E) or O(E log V)
        Space Complexity: O(V)

        LeetCode: #1584 Min Cost to Connect All Points

        Returns:
            Tuple of (list of MST edges, total weight).
        """
        if len(self._adj) == 0:
            return [], 0.0

        if self._directed:
            raise ValueError("MST is only defined for undirected graphs")

        # Collect all edges
        edges: List[Tuple[float, T, T]] = []
        seen: Set[Tuple[T, T]] = set()

        for u in self._adj:
            for v, weight in self._adj[u].items():
                edge_key = (min(u, v), max(u, v)) if isinstance(u, (int, str)) else (u, v)
                if edge_key not in seen:
                    seen.add(edge_key)
                    edges.append((weight, u, v))

        # Sort edges by weight
        edges.sort()

        # Union-Find implementation
        parent: Dict[T, T] = {v: v for v in self._adj}
        rank: Dict[T, int] = {v: 0 for v in self._adj}

        def find(x: T) -> T:
            if parent[x] != x:
                parent[x] = find(parent[x])  # Path compression
            return parent[x]

        def union(x: T, y: T) -> bool:
            px, py = find(x), find(y)
            if px == py:
                return False  # Already in same set
            # Union by rank
            if rank[px] < rank[py]:
                px, py = py, px
            parent[py] = px
            if rank[px] == rank[py]:
                rank[px] += 1
            return True

        mst_edges: List[Tuple[T, T, float]] = []
        total_weight = 0.0

        for weight, u, v in edges:
            if union(u, v):
                mst_edges.append((u, v, weight))
                total_weight += weight
                if len(mst_edges) == len(self._adj) - 1:
                    break

        return mst_edges, total_weight

    # =========================================================================
    # ALL-PAIRS SHORTEST PATH
    # =========================================================================

    def floyd_warshall(self) -> Tuple[List[T], List[List[float]], List[List[Optional[int]]]]:
        """
        Floyd-Warshall algorithm for all-pairs shortest paths.

        Time Complexity: O(V^3)
        Space Complexity: O(V^2)

        LeetCode: #1334 Find the City With the Smallest Number of Neighbors

        Returns:
            Tuple of (vertices, distances matrix, predecessors matrix).
            distances[i][j] = shortest distance from vertex i to vertex j.
            predecessors[i][j] = predecessor of j on shortest path from i.
        """
        vertices = list(self._adj.keys())
        n = len(vertices)
        index = {v: i for i, v in enumerate(vertices)}

        # Initialize distance matrix
        dist = [[float('inf')] * n for _ in range(n)]
        pred: List[List[Optional[int]]] = [[None] * n for _ in range(n)]

        # Self-loops have distance 0
        for i in range(n):
            dist[i][i] = 0

        # Initialize with direct edges
        for u in self._adj:
            for v, weight in self._adj[u].items():
                i, j = index[u], index[v]
                dist[i][j] = weight
                pred[i][j] = i

        # Floyd-Warshall main loop
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if dist[i][k] + dist[k][j] < dist[i][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]
                        pred[i][j] = pred[k][j]

        return vertices, dist, pred

    def floyd_warshall_path(self, start: T, end: T) -> Optional[Tuple[float, List[T]]]:
        """
        Find shortest path between two vertices using Floyd-Warshall.

        Args:
            start: Starting vertex.
            end: Destination vertex.

        Returns:
            Tuple of (distance, path) or None if no path exists.
        """
        vertices, dist, pred = self.floyd_warshall()

        if start not in self._adj or end not in self._adj:
            return None

        index = {v: i for i, v in enumerate(vertices)}
        i, j = index[start], index[end]

        if dist[i][j] == float('inf'):
            return None

        # Reconstruct path
        path: List[T] = []
        current: Optional[int] = j

        while current is not None and current != i:
            path.append(vertices[current])
            current = pred[i][current]

        if current is None:
            return None

        path.append(vertices[i])
        return dist[i][j], path[::-1]

    # =========================================================================
    # STRONGLY CONNECTED COMPONENTS
    # =========================================================================

    def tarjan_scc(self) -> List[Set[T]]:
        """
        Tarjan's algorithm for Strongly Connected Components.

        Time Complexity: O(V + E)
        Space Complexity: O(V)

        Returns:
            List of sets, each containing vertices in an SCC.
        """
        if not self._directed:
            return self.connected_components()

        index_counter = [0]
        indices: Dict[T, int] = {}
        low_links: Dict[T, int] = {}
        on_stack: Dict[T, bool] = {}
        stack: List[T] = []
        sccs: List[Set[T]] = []

        def strongconnect(v: T) -> None:
            indices[v] = index_counter[0]
            low_links[v] = index_counter[0]
            index_counter[0] += 1
            stack.append(v)
            on_stack[v] = True

            for w in self._adj[v]:
                if w not in indices:
                    strongconnect(w)
                    low_links[v] = min(low_links[v], low_links[w])
                elif on_stack.get(w, False):
                    low_links[v] = min(low_links[v], indices[w])

            # If v is a root node, pop the stack and generate an SCC
            if low_links[v] == indices[v]:
                scc: Set[T] = set()
                while True:
                    w = stack.pop()
                    on_stack[w] = False
                    scc.add(w)
                    if w == v:
                        break
                sccs.append(scc)

        for v in self._adj:
            if v not in indices:
                strongconnect(v)

        return sccs

    def kosaraju_scc(self) -> List[Set[T]]:
        """
        Kosaraju's algorithm for Strongly Connected Components.

        Time Complexity: O(V + E)
        Space Complexity: O(V)

        Returns:
            List of sets, each containing vertices in an SCC.
        """
        if not self._directed:
            return self.connected_components()

        # Step 1: DFS to compute finish times
        visited: Set[T] = set()
        finish_order: List[T] = []

        def dfs1(v: T) -> None:
            visited.add(v)
            for w in self._adj[v]:
                if w not in visited:
                    dfs1(w)
            finish_order.append(v)

        for v in self._adj:
            if v not in visited:
                dfs1(v)

        # Step 2: Build reverse graph
        reverse: Dict[T, Set[T]] = defaultdict(set)
        for u in self._adj:
            for v in self._adj[u]:
                reverse[v].add(u)

        # Step 3: DFS on reverse graph in reverse finish order
        visited.clear()
        sccs: List[Set[T]] = []

        def dfs2(v: T, scc: Set[T]) -> None:
            visited.add(v)
            scc.add(v)
            for w in reverse[v]:
                if w not in visited:
                    dfs2(w, scc)

        for v in reversed(finish_order):
            if v not in visited:
                scc: Set[T] = set()
                dfs2(v, scc)
                sccs.append(scc)

        return sccs
