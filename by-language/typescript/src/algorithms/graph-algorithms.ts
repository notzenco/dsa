/**
 * Graph Algorithms
 *
 * Standalone functions providing graph algorithms for traversal, shortest paths,
 * minimum spanning trees, strongly connected components, and more.
 *
 * ALGORITHM COMPLEXITY (V = vertices, E = edges):
 * | Algorithm       | Time           | Space  | Notes                    |
 * |-----------------|----------------|--------|--------------------------|
 * | BFS             | O(V + E)       | O(V)   | Unweighted shortest path |
 * | DFS             | O(V + E)       | O(V)   | Cycle detection, topo    |
 * | Dijkstra        | O((V+E)log V)  | O(V)   | Non-negative weights     |
 * | Bellman-Ford    | O(V * E)       | O(V)   | Handles negative weights |
 * | Topological Sort| O(V + E)       | O(V)   | DAG only                 |
 * | Kruskal MST     | O(E log E)     | O(V)   | Uses Union-Find          |
 * | Prim MST        | O((V+E)log V)  | O(V)   | Uses Min-Heap            |
 * | Tarjan SCC      | O(V + E)       | O(V)   | Strongly connected comp  |
 * | Kosaraju SCC    | O(V + E)       | O(V)   | Two DFS passes           |
 * | Floyd-Warshall  | O(V^3)         | O(V^2) | All-pairs shortest path  |
 */

import { GraphCore } from "../data-structures/graph-core";

/** Min-priority queue with one entry per vertex and O(log V) decreases. */
class IndexedMinQueue<T> {
  private items: { vertex: T; priority: number }[] = [];
  private positions = new Map<T, number>();

  get size(): number {
    return this.items.length;
  }

  upsert(vertex: T, priority: number): boolean {
    const position = this.positions.get(vertex);
    if (position !== undefined) {
      if (priority >= this.items[position].priority) return false;
      this.items[position].priority = priority;
      this.siftUp(position);
      return true;
    }

    const index = this.items.length;
    this.items.push({ vertex, priority });
    this.positions.set(vertex, index);
    this.siftUp(index);
    return true;
  }

  pop(): { vertex: T; priority: number } {
    const minimum = this.items[0];
    const last = this.items.pop()!;
    this.positions.delete(minimum.vertex);
    if (this.items.length > 0) {
      this.items[0] = last;
      this.positions.set(last.vertex, 0);
      this.siftDown(0);
    }
    return minimum;
  }

  private swap(left: number, right: number): void {
    [this.items[left], this.items[right]] = [this.items[right], this.items[left]];
    this.positions.set(this.items[left].vertex, left);
    this.positions.set(this.items[right].vertex, right);
  }

  private siftUp(index: number): void {
    while (index > 0) {
      const parent = Math.floor((index - 1) / 2);
      if (this.items[parent].priority <= this.items[index].priority) break;
      this.swap(parent, index);
      index = parent;
    }
  }

  private siftDown(index: number): void {
    while (2 * index + 1 < this.items.length) {
      const left = 2 * index + 1;
      const right = left + 1;
      let smallest = left;
      if (right < this.items.length &&
          this.items[right].priority < this.items[left].priority) {
        smallest = right;
      }
      if (this.items[index].priority <= this.items[smallest].priority) break;
      this.swap(index, smallest);
      index = smallest;
    }
  }
}

// =========================================================================
// GRAPH TRAVERSAL
// =========================================================================

/**
 * Breadth-first search traversal.
 *
 * Time: O(V + E), Space: O(V)
 */
export function bfs<T>(graph: GraphCore<T>, start: T): T[] {
  if (!graph.contains(start)) return [];

  const visited = new Set<T>();
  const result: T[] = [];
  const queue: T[] = [start];
  visited.add(start);
  let front = 0;

  while (front < queue.length) {
    const vertex = queue[front++];
    result.push(vertex);

    for (const neighbor of graph.neighbors(vertex)) {
      if (!visited.has(neighbor)) {
        visited.add(neighbor);
        queue.push(neighbor);
      }
    }
  }

  return result;
}

/**
 * Depth-first search traversal (iterative).
 *
 * Time: O(V + E), Space: O(V)
 */
export function dfs<T>(graph: GraphCore<T>, start: T): T[] {
  if (!graph.contains(start)) return [];

  const visited = new Set<T>();
  const result: T[] = [];
  const stack: T[] = [start];

  while (stack.length > 0) {
    const vertex = stack.pop()!;

    if (!visited.has(vertex)) {
      visited.add(vertex);
      result.push(vertex);

      // Add neighbors in reverse order for consistent ordering
      const neighs = graph.neighbors(vertex);
      for (let i = neighs.length - 1; i >= 0; i--) {
        if (!visited.has(neighs[i])) {
          stack.push(neighs[i]);
        }
      }
    }
  }

  return result;
}

// =========================================================================
// SHORTEST PATH ALGORITHMS
// =========================================================================

/**
 * Dijkstra's algorithm for single-source shortest paths (non-negative weights).
 *
 * Time: O((V + E) log V), Space: O(V)
 *
 * Returns a Map from each vertex to its shortest distance from start.
 */
export function dijkstra<T>(graph: GraphCore<T>, start: T): Map<T, number> {
  if (!graph.contains(start)) return new Map();

  const distances = new Map<T, number>();
  for (const v of graph.vertices) {
    distances.set(v, Infinity);
  }
  distances.set(start, 0);

  const visited = new Set<T>();

  const pq = new IndexedMinQueue<T>();
  pq.upsert(start, 0);

  while (pq.size > 0) {
    const { priority: dist, vertex } = pq.pop();

    if (visited.has(vertex)) continue;
    visited.add(vertex);

    for (const [neighbor, weight] of graph.neighborsWeighted(vertex)) {
      if (visited.has(neighbor)) continue;

      const newDist = dist + weight;
      if (newDist < distances.get(neighbor)!) {
        distances.set(neighbor, newDist);
        pq.upsert(neighbor, newDist);
      }
    }
  }

  return distances;
}

/**
 * Bellman-Ford algorithm (handles negative weights).
 *
 * Time: O(V * E), Space: O(V)
 *
 * Returns distances and a flag indicating whether a negative cycle was detected.
 */
export function bellmanFord<T>(
  graph: GraphCore<T>,
  start: T,
): { distances: Map<T, number>; hasNegativeCycle: boolean } {
  if (!graph.contains(start)) {
    return { distances: new Map(), hasNegativeCycle: false };
  }

  const distances = new Map<T, number>();
  for (const v of graph.vertices) {
    distances.set(v, Infinity);
  }
  distances.set(start, 0);

  const verts = graph.vertices;
  const numVertices = verts.length;

  // Relax edges V-1 times
  for (let iter = 0; iter < numVertices - 1; iter++) {
    for (const u of verts) {
      for (const [v, weight] of graph.neighborsWeighted(u)) {
        const distU = distances.get(u)!;
        if (distU !== Infinity && distU + weight < distances.get(v)!) {
          distances.set(v, distU + weight);
        }
      }
    }
  }

  // Check for negative cycles
  for (const u of verts) {
    for (const [v, weight] of graph.neighborsWeighted(u)) {
      const distU = distances.get(u)!;
      if (distU !== Infinity && distU + weight < distances.get(v)!) {
        return { distances, hasNegativeCycle: true };
      }
    }
  }

  return { distances, hasNegativeCycle: false };
}

/**
 * Find shortest path using BFS (unweighted graphs).
 *
 * Time: O(V + E)
 *
 * Returns the path as an array of vertices, or null if no path exists.
 */
export function shortestPathBfs<T>(
  graph: GraphCore<T>,
  start: T,
  end: T,
): T[] | null {
  if (!graph.contains(start) || !graph.contains(end)) return null;

  if (start === end) return [start];

  const visited = new Set<T>();
  visited.add(start);
  const parent = new Map<T, T>();
  const queue: T[] = [start];
  let front = 0;

  while (front < queue.length) {
    const vertex = queue[front++];

    for (const neighbor of graph.neighbors(vertex)) {
      if (visited.has(neighbor)) continue;
      visited.add(neighbor);
      parent.set(neighbor, vertex);
      if (neighbor === end) {
        const path = [end];
        let current = end;
        while (current !== start) {
          current = parent.get(current)!;
          path.push(current);
        }
        return path.reverse();
      }
      queue.push(neighbor);
    }
  }

  return null;
}

// =========================================================================
// GRAPH PROPERTIES
// =========================================================================

/**
 * Topological sort using Kahn's algorithm (for directed acyclic graphs).
 *
 * Time: O(V + E), Space: O(V)
 *
 * Returns null if the graph has a cycle or is not directed.
 */
export function topologicalSort<T>(graph: GraphCore<T>): T[] | null {
  if (!graph.isDirected) return null;

  const inDegrees = new Map<T, number>();
  for (const v of graph.vertices) {
    inDegrees.set(v, 0);
  }

  for (const u of graph.vertices) {
    for (const v of graph.neighbors(u)) {
      inDegrees.set(v, (inDegrees.get(v) ?? 0) + 1);
    }
  }

  const queue: T[] = [];
  for (const [v, deg] of inDegrees) {
    if (deg === 0) queue.push(v);
  }

  const result: T[] = [];
  let front = 0;

  while (front < queue.length) {
    const vertex = queue[front++];
    result.push(vertex);

    for (const neighbor of graph.neighbors(vertex)) {
      const newDeg = inDegrees.get(neighbor)! - 1;
      inDegrees.set(neighbor, newDeg);
      if (newDeg === 0) {
        queue.push(neighbor);
      }
    }
  }

  if (result.length !== graph.size) return null; // Cycle detected

  return result;
}

/**
 * Check if graph is bipartite (2-colorable).
 *
 * Time: O(V + E), Space: O(V)
 */
export function isBipartite<T>(graph: GraphCore<T>): boolean {
  const color = new Map<T, number>();

  for (const start of graph.vertices) {
    if (color.has(start)) continue;

    const queue: T[] = [start];
    color.set(start, 0);
    let front = 0;

    while (front < queue.length) {
      const vertex = queue[front++];

      for (const neighbor of graph.neighbors(vertex)) {
        if (!color.has(neighbor)) {
          color.set(neighbor, 1 - color.get(vertex)!);
          queue.push(neighbor);
        } else if (color.get(neighbor) === color.get(vertex)) {
          return false;
        }
      }
    }
  }

  return true;
}

/**
 * Find all connected components.
 *
 * Time: O(V + E), Space: O(V)
 *
 * Returns list of components, each component being an array of vertices.
 */
export function connectedComponents<T>(graph: GraphCore<T>): T[][] {
  const visited = new Set<T>();
  const components: T[][] = [];

  for (const vertex of graph.vertices) {
    if (!visited.has(vertex)) {
      const component = bfs(graph, vertex);
      for (const v of component) {
        visited.add(v);
      }
      components.push(component);
    }
  }

  return components;
}

/**
 * Check if graph contains a cycle.
 *
 * Time: O(V + E), Space: O(V)
 */
export function hasCycle<T>(graph: GraphCore<T>): boolean {
  const visited = new Set<T>();
  const recStack = new Set<T>();

  const hasCycleUtil = (vertex: T, parent: T | null): boolean => {
    visited.add(vertex);

    if (graph.isDirected) {
      recStack.add(vertex);
    }

    for (const neighbor of graph.neighbors(vertex)) {
      if (!visited.has(neighbor)) {
        if (hasCycleUtil(neighbor, vertex)) return true;
      } else if (graph.isDirected) {
        if (recStack.has(neighbor)) return true;
      } else {
        if (neighbor !== parent) return true;
      }
    }

    if (graph.isDirected) {
      recStack.delete(vertex);
    }

    return false;
  };

  for (const vertex of graph.vertices) {
    if (!visited.has(vertex)) {
      if (hasCycleUtil(vertex, null)) return true;
    }
  }

  return false;
}

// =========================================================================
// MINIMUM SPANNING TREE ALGORITHMS
// =========================================================================

/**
 * Prim's algorithm for Minimum Spanning Tree.
 *
 * Time: O((V + E) log V), Space: O(V)
 *
 * Only works on undirected graphs. Throws on directed graphs.
 */
export function primMST<T>(
  graph: GraphCore<T>,
): { edges: [T, T, number][]; totalWeight: number } {
  if (graph.size === 0) return { edges: [], totalWeight: 0 };

  if (graph.isDirected) {
    throw new Error("MST is only defined for undirected graphs");
  }

  const start = graph.vertices[0];
  const mstEdges: [T, T, number][] = [];
  let totalWeight = 0;
  const visited = new Set<T>();
  const parent = new Map<T, T>();
  const pq = new IndexedMinQueue<T>();
  pq.upsert(start, 0);

  while (pq.size > 0) {
    const { vertex: v, priority: weight } = pq.pop();

    visited.add(v);
    if (parent.has(v)) {
      mstEdges.push([parent.get(v)!, v, weight]);
      totalWeight += weight;
    }

    for (const [neighbor, edgeWeight] of graph.neighborsWeighted(v)) {
      if (!visited.has(neighbor) && pq.upsert(neighbor, edgeWeight)) {
        parent.set(neighbor, v);
      }
    }
  }

  return { edges: mstEdges, totalWeight };
}

/**
 * Kruskal's algorithm for Minimum Spanning Tree.
 *
 * Uses Union-Find for efficient cycle detection.
 *
 * Time: O(E log E), Space: O(V)
 *
 * Only works on undirected graphs. Throws on directed graphs.
 */
export function kruskalMST<T>(
  graph: GraphCore<T>,
): { edges: [T, T, number][]; totalWeight: number } {
  if (graph.size === 0) return { edges: [], totalWeight: 0 };

  if (graph.isDirected) {
    throw new Error("MST is only defined for undirected graphs");
  }

  // Collect all edges (avoid duplicates for undirected)
  const allEdges: [number, T, T][] = [];
  const seen = new Set<T>();

  for (const u of graph.vertices) {
    for (const [v, weight] of graph.neighborsWeighted(u)) {
      if (!seen.has(v)) {
        allEdges.push([weight, u, v]);
      }
    }
    seen.add(u);
  }

  allEdges.sort((a, b) => a[0] - b[0]);

  // Union-Find
  const parent = new Map<T, T>();
  const rank = new Map<T, number>();

  for (const v of graph.vertices) {
    parent.set(v, v);
    rank.set(v, 0);
  }

  const find = (x: T): T => {
    if (parent.get(x) !== x) {
      parent.set(x, find(parent.get(x)!)); // Path compression
    }
    return parent.get(x)!;
  };

  const union = (x: T, y: T): boolean => {
    let px = find(x);
    let py = find(y);
    if (px === py) return false; // Already in same set

    // Union by rank
    if (rank.get(px)! < rank.get(py)!) {
      [px, py] = [py, px];
    }
    parent.set(py, px);
    if (rank.get(px) === rank.get(py)) {
      rank.set(px, rank.get(px)! + 1);
    }
    return true;
  };

  const mstEdges: [T, T, number][] = [];
  let totalWeight = 0;

  for (const [weight, u, v] of allEdges) {
    if (union(u, v)) {
      mstEdges.push([u, v, weight]);
      totalWeight += weight;
      if (mstEdges.length === graph.size - 1) break;
    }
  }

  return { edges: mstEdges, totalWeight };
}

// =========================================================================
// ALL-PAIRS SHORTEST PATH
// =========================================================================

/**
 * Floyd-Warshall algorithm for all-pairs shortest paths.
 *
 * Time: O(V^3), Space: O(V^2)
 *
 * Returns a Map of Maps: result.get(u).get(v) = shortest distance from u to v.
 */
export function floydWarshall<T>(graph: GraphCore<T>): Map<T, Map<T, number>> {
  const verts = graph.vertices;
  const n = verts.length;
  const index = new Map<T, number>();
  for (let i = 0; i < n; i++) {
    index.set(verts[i], i);
  }

  // Initialize distance matrix
  const dist: number[][] = Array.from({ length: n }, () =>
    new Array<number>(n).fill(Infinity),
  );

  for (let i = 0; i < n; i++) {
    dist[i][i] = 0;
  }

  for (const u of verts) {
    for (const [v, weight] of graph.neighborsWeighted(u)) {
      const i = index.get(u)!;
      const j = index.get(v)!;
      dist[i][j] = Math.min(dist[i][j], weight);
    }
  }

  // Floyd-Warshall main loop
  for (let k = 0; k < n; k++) {
    for (let i = 0; i < n; i++) {
      for (let j = 0; j < n; j++) {
        if (dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }

  // Convert to Map<T, Map<T, number>>
  const result = new Map<T, Map<T, number>>();
  for (let i = 0; i < n; i++) {
    const row = new Map<T, number>();
    for (let j = 0; j < n; j++) {
      row.set(verts[j], dist[i][j]);
    }
    result.set(verts[i], row);
  }

  return result;
}

// =========================================================================
// STRONGLY CONNECTED COMPONENTS
// =========================================================================

/**
 * Tarjan's algorithm for Strongly Connected Components.
 *
 * Time: O(V + E), Space: O(V)
 *
 * Returns list of components. For undirected graphs, returns connected components.
 */
export function tarjanSCC<T>(graph: GraphCore<T>): T[][] {
  if (!graph.isDirected) {
    return connectedComponents(graph);
  }

  let indexCounter = 0;
  const indices = new Map<T, number>();
  const lowLinks = new Map<T, number>();
  const onStack = new Map<T, boolean>();
  const stack: T[] = [];
  const sccs: T[][] = [];

  const strongconnect = (v: T): void => {
    indices.set(v, indexCounter);
    lowLinks.set(v, indexCounter);
    indexCounter++;
    stack.push(v);
    onStack.set(v, true);

    for (const w of graph.neighbors(v)) {
      if (!indices.has(w)) {
        strongconnect(w);
        lowLinks.set(v, Math.min(lowLinks.get(v)!, lowLinks.get(w)!));
      } else if (onStack.get(w)) {
        lowLinks.set(v, Math.min(lowLinks.get(v)!, indices.get(w)!));
      }
    }

    // If v is a root node, pop the stack and generate an SCC
    if (lowLinks.get(v) === indices.get(v)) {
      const scc: T[] = [];
      while (true) {
        const w = stack.pop()!;
        onStack.set(w, false);
        scc.push(w);
        if (w === v) break;
      }
      sccs.push(scc);
    }
  };

  for (const v of graph.vertices) {
    if (!indices.has(v)) {
      strongconnect(v);
    }
  }

  return sccs;
}

/**
 * Kosaraju's algorithm for Strongly Connected Components.
 *
 * Time: O(V + E), Space: O(V)
 *
 * Returns list of components. For undirected graphs, returns connected components.
 */
export function kosarajuSCC<T>(graph: GraphCore<T>): T[][] {
  if (!graph.isDirected) {
    return connectedComponents(graph);
  }

  // Step 1: DFS to compute finish times
  const visited = new Set<T>();
  const finishOrder: T[] = [];

  const dfs1 = (v: T): void => {
    visited.add(v);
    for (const w of graph.neighbors(v)) {
      if (!visited.has(w)) {
        dfs1(w);
      }
    }
    finishOrder.push(v);
  };

  for (const v of graph.vertices) {
    if (!visited.has(v)) {
      dfs1(v);
    }
  }

  // Step 2: Build reverse graph
  const reverse = new Map<T, Set<T>>();
  for (const v of graph.vertices) {
    reverse.set(v, new Set());
  }
  for (const u of graph.vertices) {
    for (const v of graph.neighbors(u)) {
      reverse.get(v)!.add(u);
    }
  }

  // Step 3: DFS on reverse graph in reverse finish order
  visited.clear();
  const sccs: T[][] = [];

  const dfs2 = (v: T, scc: T[]): void => {
    visited.add(v);
    scc.push(v);
    for (const w of reverse.get(v) ?? []) {
      if (!visited.has(w)) {
        dfs2(w, scc);
      }
    }
  };

  for (let i = finishOrder.length - 1; i >= 0; i--) {
    const v = finishOrder[i];
    if (!visited.has(v)) {
      const scc: T[] = [];
      dfs2(v, scc);
      sccs.push(scc);
    }
  }

  return sccs;
}
