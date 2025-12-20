# Graphs

Graphs are data structures consisting of vertices (nodes) connected by edges, used to model relationships and networks.

## Implementations

| Structure/Algorithm | Python | C |
|---------------------|--------|---|
| Adjacency List | [graph.py](../../by-language/python/data_structures/graph.py) | [graph.c](../../by-language/c/data-structures/graph.c) |
| Adjacency Matrix | [adjacency_matrix.py](../../by-language/python/data_structures/adjacency_matrix.py) | [adjacency_matrix.c](../../by-language/c/data-structures/adjacency_matrix.c) |
| Union-Find | [union_find.py](../../by-language/python/data_structures/union_find.py) | [union_find.c](../../by-language/c/data-structures/union_find.c) |

## Complexity

### Representation Comparison
```
┌─────────────────┬─────────────────┬─────────────────┐
│ Operation       │ Adjacency List  │ Adjacency Matrix│
├─────────────────┼─────────────────┼─────────────────┤
│ Space           │ O(V + E)        │ O(V²)           │
│ Add Edge        │ O(1)            │ O(1)            │
│ Remove Edge     │ O(E)            │ O(1)            │
│ Check Edge      │ O(degree)       │ O(1)            │
│ Get Neighbors   │ O(1)            │ O(V)            │
│ Iterate Edges   │ O(E)            │ O(V²)           │
└─────────────────┴─────────────────┴─────────────────┘
```

### Graph Algorithms
```
┌────────────────────┬────────────────┬───────────────┐
│ Algorithm          │ Time           │ Space         │
├────────────────────┼────────────────┼───────────────┤
│ BFS                │ O(V + E)       │ O(V)          │
│ DFS                │ O(V + E)       │ O(V)          │
│ Dijkstra           │ O((V+E) log V) │ O(V)          │
│ Bellman-Ford       │ O(V * E)       │ O(V)          │
│ Floyd-Warshall     │ O(V³)          │ O(V²)         │
│ Prim's MST         │ O((V+E) log V) │ O(V)          │
│ Kruskal's MST      │ O(E log E)     │ O(V)          │
│ Topological Sort   │ O(V + E)       │ O(V)          │
│ Tarjan's SCC       │ O(V + E)       │ O(V)          │
└────────────────────┴────────────────┴───────────────┘
```

### Union-Find
```
┌─────────────┬───────────────────┐
│ Operation   │ Time (amortized)  │
├─────────────┼───────────────────┤
│ Find        │ O(α(n)) ≈ O(1)    │
│ Union       │ O(α(n)) ≈ O(1)    │
│ Connected   │ O(α(n)) ≈ O(1)    │
└─────────────┴───────────────────┘
α(n) = inverse Ackermann function
```

## When to Use Each Representation

**Adjacency List:**
- Sparse graph (E << V²)
- Need to iterate neighbors frequently
- Memory is constrained
- Most graph algorithms

**Adjacency Matrix:**
- Dense graph (E ≈ V²)
- Need O(1) edge existence checks
- Graph is small
- Frequently check if edge exists

## Graph Types

```
Undirected:          Directed (Digraph):    Weighted:
  A --- B              A --> B              A --5-- B
  |     |              |     ↓              |       |
  C --- D              C <-- D              2       3
                                            |       |
                                            C --4-- D
```

## Algorithm Selection

### Shortest Path
| Condition | Best Algorithm |
|-----------|----------------|
| Unweighted graph | BFS |
| Non-negative weights | Dijkstra |
| Negative weights possible | Bellman-Ford |
| All pairs needed | Floyd-Warshall |
| Single source, sparse | Dijkstra with heap |

### Minimum Spanning Tree
| Condition | Best Algorithm |
|-----------|----------------|
| Dense graph | Prim's |
| Sparse graph | Kruskal's |
| Already have sorted edges | Kruskal's |

## Common Patterns

### BFS Template
```python
from collections import deque

def bfs(graph, start):
    visited = {start}
    queue = deque([start])
    distance = {start: 0}

    while queue:
        node = queue.popleft()
        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)
                distance[neighbor] = distance[node] + 1

    return distance
```

### DFS Template
```python
def dfs(graph, start, visited=None):
    if visited is None:
        visited = set()

    visited.add(start)
    for neighbor in graph[start]:
        if neighbor not in visited:
            dfs(graph, neighbor, visited)

    return visited
```

### Dijkstra's Algorithm
```python
import heapq

def dijkstra(graph, start):
    dist = {start: 0}
    heap = [(0, start)]

    while heap:
        d, u = heapq.heappop(heap)
        if d > dist.get(u, float('inf')):
            continue

        for v, weight in graph[u]:
            if d + weight < dist.get(v, float('inf')):
                dist[v] = d + weight
                heapq.heappush(heap, (dist[v], v))

    return dist
```

### Topological Sort (Kahn's Algorithm)
```python
from collections import deque

def topological_sort(graph, n):
    in_degree = [0] * n
    for u in range(n):
        for v in graph[u]:
            in_degree[v] += 1

    queue = deque([u for u in range(n) if in_degree[u] == 0])
    result = []

    while queue:
        u = queue.popleft()
        result.append(u)
        for v in graph[u]:
            in_degree[v] -= 1
            if in_degree[v] == 0:
                queue.append(v)

    return result if len(result) == n else []  # Empty if cycle exists
```

### Union-Find Template
```python
class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])  # Path compression
        return self.parent[x]

    def union(self, x, y):
        px, py = self.find(x), self.find(y)
        if px == py:
            return False
        if self.rank[px] < self.rank[py]:
            px, py = py, px
        self.parent[py] = px
        if self.rank[px] == self.rank[py]:
            self.rank[px] += 1
        return True
```

## Related LeetCode Problems

| # | Problem | Difficulty | Pattern |
|---|---------|------------|---------|
| 127 | Word Ladder | Hard | BFS |
| 130 | Surrounded Regions | Medium | DFS/BFS |
| 133 | Clone Graph | Medium | DFS/BFS |
| 200 | Number of Islands | Medium | DFS/BFS |
| 207 | Course Schedule | Medium | Topological sort |
| 210 | Course Schedule II | Medium | Topological sort |
| 261 | Graph Valid Tree | Medium | Union-Find |
| 269 | Alien Dictionary | Hard | Topological sort |
| 277 | Find the Celebrity | Medium | Graph logic |
| 286 | Walls and Gates | Medium | Multi-source BFS |
| 305 | Number of Islands II | Hard | Union-Find |
| 310 | Minimum Height Trees | Medium | Topological pruning |
| 323 | Number of Connected Components | Medium | Union-Find |
| 332 | Reconstruct Itinerary | Hard | Eulerian path |
| 399 | Evaluate Division | Medium | Graph BFS/DFS |
| 417 | Pacific Atlantic Water Flow | Medium | Multi-source DFS |
| 444 | Sequence Reconstruction | Medium | Topological sort |
| 490 | The Maze | Medium | BFS |
| 505 | The Maze II | Medium | Dijkstra |
| 542 | 01 Matrix | Medium | Multi-source BFS |
| 547 | Number of Provinces | Medium | Union-Find |
| 684 | Redundant Connection | Medium | Union-Find |
| 685 | Redundant Connection II | Hard | Union-Find |
| 695 | Max Area of Island | Medium | DFS |
| 721 | Accounts Merge | Medium | Union-Find |
| 743 | Network Delay Time | Medium | Dijkstra |
| 752 | Open the Lock | Medium | BFS |
| 778 | Swim in Rising Water | Hard | Binary search + BFS |
| 785 | Is Graph Bipartite? | Medium | BFS/DFS coloring |
| 787 | Cheapest Flights K Stops | Medium | Bellman-Ford |
| 797 | All Paths From Source to Target | Medium | DFS backtrack |
| 802 | Find Eventual Safe States | Medium | Reverse graph DFS |
| 841 | Keys and Rooms | Medium | DFS |
| 847 | Shortest Path Visiting All Nodes | Hard | BFS + bitmask |
| 863 | All Nodes Distance K in Binary Tree | Medium | BFS |
| 886 | Possible Bipartition | Medium | Graph coloring |
| 934 | Shortest Bridge | Medium | DFS + BFS |
| 947 | Most Stones Removed | Medium | Union-Find |
| 994 | Rotting Oranges | Medium | Multi-source BFS |
| 1091 | Shortest Path in Binary Matrix | Medium | BFS |
| 1192 | Critical Connections | Hard | Tarjan's bridges |
| 1202 | Smallest String With Swaps | Medium | Union-Find |
| 1584 | Min Cost to Connect All Points | Medium | Prim's/Kruskal's |

## See Also

- [Trees](../trees/README.md) - Trees are acyclic graphs
- [Heaps](../heaps/README.md) - Used in Dijkstra's algorithm
- [Dynamic Programming](../dynamic-programming/README.md) - Floyd-Warshall, shortest paths
- [Queues](../queues/README.md) - BFS uses queues
- [Stacks](../stacks/README.md) - DFS uses stacks
