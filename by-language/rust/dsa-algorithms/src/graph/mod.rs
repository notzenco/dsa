//! # Graph Algorithms
//!
//! This module contains implementations of fundamental graph algorithms.
//!
//! ## Traversal Algorithms
//!
//! - [`bfs`] - Breadth-First Search
//! - [`dfs`] - Depth-First Search
//!
//! ## Shortest Path Algorithms
//!
//! - [`dijkstra`] - Single-source shortest path (non-negative weights)
//! - [`bellman_ford`] - Single-source shortest path (handles negative weights)
//! - [`floyd_warshall`] - All-pairs shortest path
//!
//! ## Minimum Spanning Tree
//!
//! - [`prim`] - Prim's MST algorithm
//! - [`kruskal`] - Kruskal's MST algorithm
//!
//! ## Other Algorithms
//!
//! - [`topological_sort`] - Linear ordering of DAG vertices
//! - [`kosaraju_scc`] - Strongly connected components

use alloc::collections::BTreeMap;
use alloc::collections::BTreeSet;
use alloc::collections::VecDeque;
use alloc::vec;
use alloc::vec::Vec;

/// Represents a weighted directed edge.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Edge {
    pub to: usize,
    pub weight: i64,
}

impl Edge {
    /// Creates a new edge.
    pub fn new(to: usize, weight: i64) -> Self {
        Edge { to, weight }
    }
}

/// Represents an adjacency list graph.
pub type Graph = Vec<Vec<Edge>>;

/// Creates an empty graph with n vertices.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::new_graph;
///
/// let g = new_graph(5);
/// assert_eq!(g.len(), 5);
/// ```
pub fn new_graph(n: usize) -> Graph {
    vec![Vec::new(); n]
}

/// Adds a directed edge to the graph.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge};
///
/// let mut g = new_graph(3);
/// add_edge(&mut g, 0, 1, 10);
/// add_edge(&mut g, 1, 2, 20);
/// ```
pub fn add_edge(graph: &mut Graph, from: usize, to: usize, weight: i64) {
    graph[from].push(Edge::new(to, weight));
}

/// Adds an undirected edge to the graph.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_undirected_edge};
///
/// let mut g = new_graph(3);
/// add_undirected_edge(&mut g, 0, 1, 10);
/// ```
pub fn add_undirected_edge(graph: &mut Graph, u: usize, v: usize, weight: i64) {
    graph[u].push(Edge::new(v, weight));
    graph[v].push(Edge::new(u, weight));
}

/// Breadth-First Search - Explores vertices level by level.
///
/// Returns the order in which vertices are visited.
///
/// # Complexity
///
/// - Time: O(V + E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, bfs};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 0, 2, 1);
/// add_edge(&mut g, 1, 3, 1);
/// add_edge(&mut g, 2, 3, 1);
///
/// let order = bfs(&g, 0);
/// assert_eq!(order[0], 0); // Start vertex
/// assert!(order.contains(&1));
/// assert!(order.contains(&2));
/// assert!(order.contains(&3));
/// ```
pub fn bfs(graph: &Graph, start: usize) -> Vec<usize> {
    let n = graph.len();
    let mut visited = vec![false; n];
    let mut order = Vec::new();
    let mut queue = VecDeque::new();

    visited[start] = true;
    queue.push_back(start);

    while let Some(v) = queue.pop_front() {
        order.push(v);

        for edge in &graph[v] {
            if !visited[edge.to] {
                visited[edge.to] = true;
                queue.push_back(edge.to);
            }
        }
    }

    order
}

/// BFS with distance tracking - Returns distances from start vertex.
///
/// Returns (distances, parents) where:
/// - distances[v] = shortest number of edges from start to v (-1 if unreachable)
/// - parents[v] = predecessor of v in shortest path (-1 if start or unreachable)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, bfs_distances};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 1, 2, 1);
/// add_edge(&mut g, 2, 3, 1);
///
/// let (dist, _) = bfs_distances(&g, 0);
/// assert_eq!(dist[0], 0);
/// assert_eq!(dist[1], 1);
/// assert_eq!(dist[2], 2);
/// assert_eq!(dist[3], 3);
/// ```
pub fn bfs_distances(graph: &Graph, start: usize) -> (Vec<i64>, Vec<i64>) {
    let n = graph.len();
    let mut dist = vec![-1i64; n];
    let mut parent = vec![-1i64; n];
    let mut queue = VecDeque::new();

    dist[start] = 0;
    queue.push_back(start);

    while let Some(v) = queue.pop_front() {
        for edge in &graph[v] {
            if dist[edge.to] == -1 {
                dist[edge.to] = dist[v] + 1;
                parent[edge.to] = v as i64;
                queue.push_back(edge.to);
            }
        }
    }

    (dist, parent)
}

/// Depth-First Search - Explores as far as possible before backtracking.
///
/// Returns the order in which vertices are visited.
///
/// # Complexity
///
/// - Time: O(V + E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, dfs};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 0, 2, 1);
/// add_edge(&mut g, 1, 3, 1);
///
/// let order = dfs(&g, 0);
/// assert_eq!(order[0], 0); // Start vertex first
/// assert_eq!(order.len(), 4);
/// ```
pub fn dfs(graph: &Graph, start: usize) -> Vec<usize> {
    let n = graph.len();
    let mut visited = vec![false; n];
    let mut order = Vec::new();

    dfs_helper(graph, start, &mut visited, &mut order);

    order
}

fn dfs_helper(graph: &Graph, v: usize, visited: &mut [bool], order: &mut Vec<usize>) {
    visited[v] = true;
    order.push(v);

    for edge in &graph[v] {
        if !visited[edge.to] {
            dfs_helper(graph, edge.to, visited, order);
        }
    }
}

/// Iterative DFS using explicit stack.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, dfs_iterative};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 0, 2, 1);
/// add_edge(&mut g, 1, 3, 1);
///
/// let order = dfs_iterative(&g, 0);
/// assert_eq!(order[0], 0);
/// assert_eq!(order.len(), 4);
/// ```
pub fn dfs_iterative(graph: &Graph, start: usize) -> Vec<usize> {
    let n = graph.len();
    let mut visited = vec![false; n];
    let mut order = Vec::new();
    let mut stack = vec![start];

    while let Some(v) = stack.pop() {
        if visited[v] {
            continue;
        }

        visited[v] = true;
        order.push(v);

        // Push neighbors in reverse order for consistent traversal
        for edge in graph[v].iter().rev() {
            if !visited[edge.to] {
                stack.push(edge.to);
            }
        }
    }

    order
}

/// Dijkstra's Algorithm - Single-source shortest path for non-negative weights.
///
/// Returns (distances, parents) where:
/// - distances[v] = shortest distance from start to v (i64::MAX if unreachable)
/// - parents[v] = predecessor of v in shortest path (-1 if start or unreachable)
///
/// # Complexity
///
/// - Time: O((V + E) log V) with binary heap
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, dijkstra};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 0, 2, 4);
/// add_edge(&mut g, 1, 2, 2);
/// add_edge(&mut g, 2, 3, 1);
///
/// let (dist, _) = dijkstra(&g, 0);
/// assert_eq!(dist[0], 0);
/// assert_eq!(dist[1], 1);
/// assert_eq!(dist[2], 3); // 0 -> 1 -> 2
/// assert_eq!(dist[3], 4); // 0 -> 1 -> 2 -> 3
/// ```
pub fn dijkstra(graph: &Graph, start: usize) -> (Vec<i64>, Vec<i64>) {
    let n = graph.len();
    let mut dist = vec![i64::MAX; n];
    let mut parent = vec![-1i64; n];
    let mut visited = vec![false; n];

    // Use BTreeMap as a simple priority queue (distance -> vertices)
    let mut pq: BTreeMap<i64, Vec<usize>> = BTreeMap::new();

    dist[start] = 0;
    pq.entry(0).or_default().push(start);

    while !pq.is_empty() {
        // Get minimum distance vertex
        let (&min_dist, vertices) = pq.iter_mut().next().unwrap();
        let v = vertices.pop().unwrap();

        if vertices.is_empty() {
            pq.remove(&min_dist);
        }

        if visited[v] {
            continue;
        }
        visited[v] = true;

        for edge in &graph[v] {
            let new_dist = dist[v].saturating_add(edge.weight);
            if new_dist < dist[edge.to] {
                dist[edge.to] = new_dist;
                parent[edge.to] = v as i64;
                pq.entry(new_dist).or_default().push(edge.to);
            }
        }
    }

    (dist, parent)
}

/// Bellman-Ford Algorithm - Single-source shortest path that handles negative weights.
///
/// Returns `Some((distances, parents))` if no negative cycle, `None` if negative cycle exists.
///
/// # Complexity
///
/// - Time: O(V * E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, bellman_ford};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 1, 2, -2);
/// add_edge(&mut g, 2, 3, 1);
///
/// let result = bellman_ford(&g, 0);
/// assert!(result.is_some());
///
/// let (dist, _) = result.unwrap();
/// assert_eq!(dist[0], 0);
/// assert_eq!(dist[1], 1);
/// assert_eq!(dist[2], -1);
/// assert_eq!(dist[3], 0);
/// ```
pub fn bellman_ford(graph: &Graph, start: usize) -> Option<(Vec<i64>, Vec<i64>)> {
    let n = graph.len();
    let mut dist = vec![i64::MAX; n];
    let mut parent = vec![-1i64; n];

    dist[start] = 0;

    // Relax all edges V-1 times
    for _ in 0..n - 1 {
        for u in 0..n {
            if dist[u] == i64::MAX {
                continue;
            }
            for edge in &graph[u] {
                let new_dist = dist[u].saturating_add(edge.weight);
                if new_dist < dist[edge.to] {
                    dist[edge.to] = new_dist;
                    parent[edge.to] = u as i64;
                }
            }
        }
    }

    // Check for negative cycles
    for u in 0..n {
        if dist[u] == i64::MAX {
            continue;
        }
        for edge in &graph[u] {
            let new_dist = dist[u].saturating_add(edge.weight);
            if new_dist < dist[edge.to] {
                return None; // Negative cycle detected
            }
        }
    }

    Some((dist, parent))
}

/// Floyd-Warshall Algorithm - All-pairs shortest path.
///
/// Returns a matrix where result[i][j] = shortest distance from i to j.
/// Uses i64::MAX for unreachable pairs.
///
/// # Complexity
///
/// - Time: O(V³)
/// - Space: O(V²)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, floyd_warshall};
///
/// let mut g = new_graph(3);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 1, 2, 2);
/// add_edge(&mut g, 0, 2, 5);
///
/// let dist = floyd_warshall(&g);
/// assert_eq!(dist[0][0], 0);
/// assert_eq!(dist[0][1], 1);
/// assert_eq!(dist[0][2], 3); // 0 -> 1 -> 2
/// assert_eq!(dist[1][2], 2);
/// ```
pub fn floyd_warshall(graph: &Graph) -> Vec<Vec<i64>> {
    let n = graph.len();
    let mut dist = vec![vec![i64::MAX; n]; n];

    // Initialize distances
    for i in 0..n {
        dist[i][i] = 0;
        for edge in &graph[i] {
            dist[i][edge.to] = edge.weight;
        }
    }

    // Floyd-Warshall
    for k in 0..n {
        for i in 0..n {
            for j in 0..n {
                if dist[i][k] != i64::MAX && dist[k][j] != i64::MAX {
                    let new_dist = dist[i][k].saturating_add(dist[k][j]);
                    if new_dist < dist[i][j] {
                        dist[i][j] = new_dist;
                    }
                }
            }
        }
    }

    dist
}

/// Prim's Algorithm - Minimum Spanning Tree.
///
/// Returns (total weight, edges in MST) for a connected graph.
/// Edges are represented as (from, to, weight).
///
/// # Complexity
///
/// - Time: O((V + E) log V)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_undirected_edge, prim};
///
/// let mut g = new_graph(4);
/// add_undirected_edge(&mut g, 0, 1, 10);
/// add_undirected_edge(&mut g, 0, 2, 6);
/// add_undirected_edge(&mut g, 0, 3, 5);
/// add_undirected_edge(&mut g, 1, 3, 15);
/// add_undirected_edge(&mut g, 2, 3, 4);
///
/// let (total, edges) = prim(&g);
/// assert_eq!(total, 19); // 5 + 4 + 10
/// assert_eq!(edges.len(), 3);
/// ```
pub fn prim(graph: &Graph) -> (i64, Vec<(usize, usize, i64)>) {
    let n = graph.len();
    if n == 0 {
        return (0, Vec::new());
    }

    let mut in_mst = vec![false; n];
    let mut mst_edges = Vec::new();
    let mut total_weight = 0i64;

    // Use BTreeMap as priority queue: (weight, from, to)
    let mut pq: BTreeSet<(i64, usize, usize)> = BTreeSet::new();

    // Start from vertex 0
    in_mst[0] = true;
    for edge in &graph[0] {
        pq.insert((edge.weight, 0, edge.to));
    }

    while !pq.is_empty() && mst_edges.len() < n - 1 {
        let (weight, from, to) = *pq.iter().next().unwrap();
        pq.remove(&(weight, from, to));

        if in_mst[to] {
            continue;
        }

        in_mst[to] = true;
        mst_edges.push((from, to, weight));
        total_weight += weight;

        for edge in &graph[to] {
            if !in_mst[edge.to] {
                pq.insert((edge.weight, to, edge.to));
            }
        }
    }

    (total_weight, mst_edges)
}

/// Union-Find for Kruskal's algorithm (local implementation).
struct UnionFind {
    parent: Vec<usize>,
    rank: Vec<usize>,
}

impl UnionFind {
    fn new(n: usize) -> Self {
        UnionFind {
            parent: (0..n).collect(),
            rank: vec![0; n],
        }
    }

    fn find(&mut self, x: usize) -> usize {
        if self.parent[x] != x {
            self.parent[x] = self.find(self.parent[x]);
        }
        self.parent[x]
    }

    fn union(&mut self, x: usize, y: usize) -> bool {
        let px = self.find(x);
        let py = self.find(y);

        if px == py {
            return false;
        }

        if self.rank[px] < self.rank[py] {
            self.parent[px] = py;
        } else if self.rank[px] > self.rank[py] {
            self.parent[py] = px;
        } else {
            self.parent[py] = px;
            self.rank[px] += 1;
        }

        true
    }
}

/// Kruskal's Algorithm - Minimum Spanning Tree.
///
/// Returns (total weight, edges in MST) for a connected graph.
///
/// # Complexity
///
/// - Time: O(E log E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_undirected_edge, kruskal};
///
/// let mut g = new_graph(4);
/// add_undirected_edge(&mut g, 0, 1, 10);
/// add_undirected_edge(&mut g, 0, 2, 6);
/// add_undirected_edge(&mut g, 0, 3, 5);
/// add_undirected_edge(&mut g, 1, 3, 15);
/// add_undirected_edge(&mut g, 2, 3, 4);
///
/// let (total, edges) = kruskal(&g);
/// assert_eq!(total, 19);
/// assert_eq!(edges.len(), 3);
/// ```
pub fn kruskal(graph: &Graph) -> (i64, Vec<(usize, usize, i64)>) {
    let n = graph.len();
    if n == 0 {
        return (0, Vec::new());
    }

    // Collect all edges
    let mut edges: Vec<(i64, usize, usize)> = Vec::new();
    for u in 0..n {
        for edge in &graph[u] {
            if u < edge.to {
                // Avoid duplicates for undirected graphs
                edges.push((edge.weight, u, edge.to));
            }
        }
    }

    // Sort edges by weight
    edges.sort_by_key(|e| e.0);

    let mut uf = UnionFind::new(n);
    let mut mst_edges = Vec::new();
    let mut total_weight = 0i64;

    for (weight, u, v) in edges {
        if uf.union(u, v) {
            mst_edges.push((u, v, weight));
            total_weight += weight;

            if mst_edges.len() == n - 1 {
                break;
            }
        }
    }

    (total_weight, mst_edges)
}

/// Topological Sort - Linear ordering of DAG vertices.
///
/// Returns `Some(order)` if the graph is a DAG, `None` if it contains a cycle.
///
/// # Complexity
///
/// - Time: O(V + E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, topological_sort};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 0, 2, 1);
/// add_edge(&mut g, 1, 3, 1);
/// add_edge(&mut g, 2, 3, 1);
///
/// let order = topological_sort(&g);
/// assert!(order.is_some());
/// let order = order.unwrap();
/// assert_eq!(order[0], 0); // 0 must come first
/// assert_eq!(order[3], 3); // 3 must come last
/// ```
pub fn topological_sort(graph: &Graph) -> Option<Vec<usize>> {
    let n = graph.len();
    let mut in_degree = vec![0usize; n];

    // Calculate in-degrees
    for edges in graph {
        for edge in edges {
            in_degree[edge.to] += 1;
        }
    }

    // Add all vertices with in-degree 0 to queue
    let mut queue: VecDeque<usize> = VecDeque::new();
    for (v, &deg) in in_degree.iter().enumerate() {
        if deg == 0 {
            queue.push_back(v);
        }
    }

    let mut order = Vec::new();

    while let Some(v) = queue.pop_front() {
        order.push(v);

        for edge in &graph[v] {
            in_degree[edge.to] -= 1;
            if in_degree[edge.to] == 0 {
                queue.push_back(edge.to);
            }
        }
    }

    if order.len() == n {
        Some(order)
    } else {
        None // Cycle detected
    }
}

/// Kosaraju's Algorithm - Strongly Connected Components.
///
/// Returns a list of strongly connected components.
///
/// # Complexity
///
/// - Time: O(V + E)
/// - Space: O(V)
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, kosaraju_scc};
///
/// let mut g = new_graph(5);
/// // SCC 1: 0, 1, 2
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 1, 2, 1);
/// add_edge(&mut g, 2, 0, 1);
/// // SCC 2: 3, 4
/// add_edge(&mut g, 3, 4, 1);
/// add_edge(&mut g, 4, 3, 1);
/// // Connection between SCCs
/// add_edge(&mut g, 2, 3, 1);
///
/// let sccs = kosaraju_scc(&g);
/// assert_eq!(sccs.len(), 2);
/// ```
pub fn kosaraju_scc(graph: &Graph) -> Vec<Vec<usize>> {
    let n = graph.len();
    let mut visited = vec![false; n];
    let mut finish_order = Vec::new();

    // First DFS to get finish order
    for v in 0..n {
        if !visited[v] {
            kosaraju_dfs1(graph, v, &mut visited, &mut finish_order);
        }
    }

    // Build reverse graph
    let mut reverse: Graph = vec![Vec::new(); n];
    for u in 0..n {
        for edge in &graph[u] {
            reverse[edge.to].push(Edge::new(u, edge.weight));
        }
    }

    // Second DFS on reverse graph in reverse finish order
    visited.fill(false);
    let mut sccs = Vec::new();

    for &v in finish_order.iter().rev() {
        if !visited[v] {
            let mut scc = Vec::new();
            kosaraju_dfs2(&reverse, v, &mut visited, &mut scc);
            sccs.push(scc);
        }
    }

    sccs
}

fn kosaraju_dfs1(graph: &Graph, v: usize, visited: &mut [bool], finish_order: &mut Vec<usize>) {
    visited[v] = true;
    for edge in &graph[v] {
        if !visited[edge.to] {
            kosaraju_dfs1(graph, edge.to, visited, finish_order);
        }
    }
    finish_order.push(v);
}

fn kosaraju_dfs2(graph: &Graph, v: usize, visited: &mut [bool], scc: &mut Vec<usize>) {
    visited[v] = true;
    scc.push(v);
    for edge in &graph[v] {
        if !visited[edge.to] {
            kosaraju_dfs2(graph, edge.to, visited, scc);
        }
    }
}

/// Reconstruct path from parent array.
///
/// # Example
///
/// ```rust
/// use dsa_algorithms::graph::{new_graph, add_edge, dijkstra, reconstruct_path};
///
/// let mut g = new_graph(4);
/// add_edge(&mut g, 0, 1, 1);
/// add_edge(&mut g, 1, 2, 1);
/// add_edge(&mut g, 2, 3, 1);
///
/// let (_, parent) = dijkstra(&g, 0);
/// let path = reconstruct_path(&parent, 3);
/// assert_eq!(path, Some(vec![0, 1, 2, 3]));
/// ```
pub fn reconstruct_path(parent: &[i64], target: usize) -> Option<Vec<usize>> {
    if parent[target] == -1 && target != 0 {
        return None; // Check if target is the start or unreachable
    }

    let mut path = Vec::new();
    let mut current = target as i64;

    while current != -1 {
        path.push(current as usize);
        current = parent[current as usize];
    }

    path.reverse();

    // If path starts with 0 or only has target (start==target), it's valid
    if path.is_empty() {
        None
    } else {
        Some(path)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod bfs_tests {
        use super::*;

        #[test]
        fn test_empty_graph() {
            let _g: Graph = vec![];
            // Can't run BFS on empty graph
        }

        #[test]
        fn test_single_vertex() {
            let g = new_graph(1);
            let order = bfs(&g, 0);
            assert_eq!(order, vec![0]);
        }

        #[test]
        fn test_linear_graph() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 3, 1);

            let order = bfs(&g, 0);
            assert_eq!(order, vec![0, 1, 2, 3]);
        }

        #[test]
        fn test_bfs_distances() {
            let mut g = new_graph(5);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 0, 2, 1);
            add_edge(&mut g, 1, 3, 1);
            add_edge(&mut g, 2, 3, 1);
            add_edge(&mut g, 3, 4, 1);

            let (dist, _) = bfs_distances(&g, 0);
            assert_eq!(dist[0], 0);
            assert_eq!(dist[1], 1);
            assert_eq!(dist[2], 1);
            assert_eq!(dist[3], 2);
            assert_eq!(dist[4], 3);
        }

        #[test]
        fn test_disconnected() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            // 2, 3 are disconnected

            let order = bfs(&g, 0);
            assert_eq!(order.len(), 2);
            assert!(order.contains(&0));
            assert!(order.contains(&1));

            let (dist, _) = bfs_distances(&g, 0);
            assert_eq!(dist[2], -1);
            assert_eq!(dist[3], -1);
        }
    }

    mod dfs_tests {
        use super::*;

        #[test]
        fn test_single_vertex() {
            let g = new_graph(1);
            let order = dfs(&g, 0);
            assert_eq!(order, vec![0]);
        }

        #[test]
        fn test_linear_graph() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 3, 1);

            let order = dfs(&g, 0);
            assert_eq!(order, vec![0, 1, 2, 3]);
        }

        #[test]
        fn test_dfs_iterative() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 0, 2, 1);
            add_edge(&mut g, 1, 3, 1);

            let order = dfs_iterative(&g, 0);
            assert_eq!(order[0], 0);
            assert_eq!(order.len(), 4);
        }

        #[test]
        fn test_cycle() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 0, 1);

            let order = dfs(&g, 0);
            assert_eq!(order.len(), 3);
        }
    }

    mod dijkstra_tests {
        use super::*;

        #[test]
        fn test_single_vertex() {
            let g = new_graph(1);
            let (dist, _) = dijkstra(&g, 0);
            assert_eq!(dist[0], 0);
        }

        #[test]
        fn test_linear() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 2);
            add_edge(&mut g, 2, 3, 3);

            let (dist, _) = dijkstra(&g, 0);
            assert_eq!(dist[0], 0);
            assert_eq!(dist[1], 1);
            assert_eq!(dist[2], 3);
            assert_eq!(dist[3], 6);
        }

        #[test]
        fn test_shortest_path() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 0, 2, 4);
            add_edge(&mut g, 1, 2, 2);
            add_edge(&mut g, 2, 3, 1);

            let (dist, parent) = dijkstra(&g, 0);
            assert_eq!(dist[2], 3); // 0 -> 1 -> 2
            assert_eq!(dist[3], 4);

            let path = reconstruct_path(&parent, 3);
            assert_eq!(path, Some(vec![0, 1, 2, 3]));
        }

        #[test]
        fn test_unreachable() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            // Vertex 2 is unreachable

            let (dist, _) = dijkstra(&g, 0);
            assert_eq!(dist[2], i64::MAX);
        }
    }

    mod bellman_ford_tests {
        use super::*;

        #[test]
        fn test_basic() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 2);
            add_edge(&mut g, 2, 3, 3);

            let result = bellman_ford(&g, 0);
            assert!(result.is_some());

            let (dist, _) = result.unwrap();
            assert_eq!(dist[0], 0);
            assert_eq!(dist[1], 1);
            assert_eq!(dist[2], 3);
            assert_eq!(dist[3], 6);
        }

        #[test]
        fn test_negative_weights() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, -2);
            add_edge(&mut g, 2, 3, 1);

            let result = bellman_ford(&g, 0);
            assert!(result.is_some());

            let (dist, _) = result.unwrap();
            assert_eq!(dist[0], 0);
            assert_eq!(dist[1], 1);
            assert_eq!(dist[2], -1);
            assert_eq!(dist[3], 0);
        }

        #[test]
        fn test_negative_cycle() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, -3);
            add_edge(&mut g, 2, 0, 1);

            let result = bellman_ford(&g, 0);
            assert!(result.is_none());
        }
    }

    mod floyd_warshall_tests {
        use super::*;

        #[test]
        fn test_basic() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 2);
            add_edge(&mut g, 0, 2, 5);

            let dist = floyd_warshall(&g);
            assert_eq!(dist[0][0], 0);
            assert_eq!(dist[0][1], 1);
            assert_eq!(dist[0][2], 3);
            assert_eq!(dist[1][2], 2);
        }

        #[test]
        fn test_no_path() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);

            let dist = floyd_warshall(&g);
            assert_eq!(dist[0][2], i64::MAX);
            assert_eq!(dist[1][0], i64::MAX);
        }
    }

    mod mst_tests {
        use super::*;

        #[test]
        fn test_prim() {
            let mut g = new_graph(4);
            add_undirected_edge(&mut g, 0, 1, 10);
            add_undirected_edge(&mut g, 0, 2, 6);
            add_undirected_edge(&mut g, 0, 3, 5);
            add_undirected_edge(&mut g, 1, 3, 15);
            add_undirected_edge(&mut g, 2, 3, 4);

            let (total, edges) = prim(&g);
            assert_eq!(total, 19);
            assert_eq!(edges.len(), 3);
        }

        #[test]
        fn test_kruskal() {
            let mut g = new_graph(4);
            add_undirected_edge(&mut g, 0, 1, 10);
            add_undirected_edge(&mut g, 0, 2, 6);
            add_undirected_edge(&mut g, 0, 3, 5);
            add_undirected_edge(&mut g, 1, 3, 15);
            add_undirected_edge(&mut g, 2, 3, 4);

            let (total, edges) = kruskal(&g);
            assert_eq!(total, 19);
            assert_eq!(edges.len(), 3);
        }

        #[test]
        fn test_prim_single() {
            let g = new_graph(1);
            let (total, edges) = prim(&g);
            assert_eq!(total, 0);
            assert!(edges.is_empty());
        }

        #[test]
        fn test_kruskal_single() {
            let g = new_graph(1);
            let (total, edges) = kruskal(&g);
            assert_eq!(total, 0);
            assert!(edges.is_empty());
        }
    }

    mod topological_sort_tests {
        use super::*;

        #[test]
        fn test_linear() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 3, 1);

            let order = topological_sort(&g);
            assert!(order.is_some());
            assert_eq!(order.unwrap(), vec![0, 1, 2, 3]);
        }

        #[test]
        fn test_diamond() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 0, 2, 1);
            add_edge(&mut g, 1, 3, 1);
            add_edge(&mut g, 2, 3, 1);

            let order = topological_sort(&g);
            assert!(order.is_some());
            let order = order.unwrap();
            assert_eq!(order[0], 0);
            assert_eq!(order[3], 3);
        }

        #[test]
        fn test_cycle() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 0, 1);

            let order = topological_sort(&g);
            assert!(order.is_none());
        }

        #[test]
        fn test_empty() {
            let g = new_graph(3);
            let order = topological_sort(&g);
            assert!(order.is_some());
            assert_eq!(order.unwrap().len(), 3);
        }
    }

    mod scc_tests {
        use super::*;

        #[test]
        fn test_single_scc() {
            let mut g = new_graph(3);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 0, 1);

            let sccs = kosaraju_scc(&g);
            assert_eq!(sccs.len(), 1);
            assert_eq!(sccs[0].len(), 3);
        }

        #[test]
        fn test_multiple_sccs() {
            let mut g = new_graph(5);
            // SCC 1: 0, 1, 2
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 0, 1);
            // SCC 2: 3, 4
            add_edge(&mut g, 3, 4, 1);
            add_edge(&mut g, 4, 3, 1);
            // Connection
            add_edge(&mut g, 2, 3, 1);

            let sccs = kosaraju_scc(&g);
            assert_eq!(sccs.len(), 2);
        }

        #[test]
        fn test_no_edges() {
            let g = new_graph(3);
            let sccs = kosaraju_scc(&g);
            assert_eq!(sccs.len(), 3); // Each vertex is its own SCC
        }

        #[test]
        fn test_dag() {
            let mut g = new_graph(4);
            add_edge(&mut g, 0, 1, 1);
            add_edge(&mut g, 1, 2, 1);
            add_edge(&mut g, 2, 3, 1);

            let sccs = kosaraju_scc(&g);
            assert_eq!(sccs.len(), 4); // Each vertex is its own SCC
        }
    }
}
