"""
Graph Data Structure

Umbrella module that combines core graph operations with algorithms.
For backward compatibility, import Graph from this module.

For separate imports:
- graph_core.py: GraphCore class with core operations
- graph_algorithms.py: GraphAlgorithmsMixin with algorithms

VISUAL REPRESENTATION:

  Undirected Graph:              Directed Graph (Digraph):
       (0)                            (0)
      / | \\                          /   \\
    (1)-(2)-(3)                    (1)    (2)
      \\ |                           |    /
       (4)                         (3)<-(4)

  Adjacency List Representation:
  +---------+--------------------+
  | Vertex  | Neighbors          |
  +---------+--------------------+
  |   0     | [1, 2, 3]          |
  |   1     | [0, 2, 4]          |
  |   2     | [0, 1, 3, 4]       |
  |   3     | [0, 2]             |
  |   4     | [1, 2]             |
  +---------+--------------------+

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
| BFS/DFS             | O(V + E)        | O(V^2)          |
+---------------------+-----------------+-----------------+

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

from typing import TypeVar, List, Tuple

from .graph_core import GraphCore
from .graph_algorithms import GraphAlgorithmsMixin

T = TypeVar('T')


class Graph(GraphAlgorithmsMixin, GraphCore[T]):
    """
    Graph implementation using adjacency list.

    Supports both directed and undirected graphs, weighted and unweighted edges.

    This class combines:
    - GraphCore: Core graph operations (add/remove vertices/edges, etc.)
    - GraphAlgorithmsMixin: Graph algorithms (BFS, DFS, Dijkstra, MST, SCC, etc.)

    Example:
        >>> g = Graph()
        >>> g.add_edge('A', 'B', 4)
        >>> g.add_edge('A', 'C', 2)
        >>> g.add_edge('B', 'C', 1)
        >>> g.bfs('A')
        ['A', 'B', 'C']
        >>> g.dijkstra('A')
        ({'A': 0, 'B': 4, 'C': 2}, {'A': None, 'B': 'A', 'C': 'A'})
    """

    def copy(self) -> "Graph[T]":
        """Create a deep copy of the graph."""
        new_graph: Graph[T] = Graph(directed=self._directed)
        for u in self._adj:
            for v, weight in self._adj[u].items():
                if self._directed or u <= v:  # Avoid duplicate edges in undirected
                    new_graph.add_edge(u, v, weight)
        return new_graph

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
