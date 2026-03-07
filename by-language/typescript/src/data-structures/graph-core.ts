/**
 * Graph Core Data Structure
 *
 * Core graph operations using adjacency list representation.
 * For algorithms (traversal, shortest path, MST, SCC), see graph-algorithms.ts.
 *
 * COMPLEXITY (V = vertices, E = edges):
 * | Operation       | Adjacency List | Adjacency Matrix |
 * |-----------------|----------------|------------------|
 * | Space           | O(V + E)       | O(V^2)           |
 * | Add Vertex      | O(1)           | O(V^2)           |
 * | Add Edge        | O(1)           | O(1)             |
 * | Remove Edge     | O(E)           | O(1)             |
 * | Check Edge      | O(degree)      | O(1)             |
 * | Get Neighbors   | O(1)           | O(V)             |
 */

export class GraphCore<T> {
  private _adj: Map<T, Map<T, number>>;
  private readonly _directed: boolean;

  constructor(directed: boolean = false) {
    this._adj = new Map();
    this._directed = directed;
  }

  // =========================================================================
  // PROPERTIES
  // =========================================================================

  get isDirected(): boolean {
    return this._directed;
  }

  get vertices(): T[] {
    return [...this._adj.keys()];
  }

  get edgeCount(): number {
    let count = 0;
    for (const neighbors of this._adj.values()) {
      count += neighbors.size;
    }
    return this._directed ? count : Math.floor(count / 2);
  }

  get size(): number {
    return this._adj.size;
  }

  // =========================================================================
  // VERTEX OPERATIONS
  // =========================================================================

  addVertex(vertex: T): void {
    if (!this._adj.has(vertex)) {
      this._adj.set(vertex, new Map());
    }
  }

  removeVertex(vertex: T): boolean {
    if (!this._adj.has(vertex)) return false;

    // Remove all edges to this vertex
    for (const [, neighbors] of this._adj) {
      neighbors.delete(vertex);
    }

    this._adj.delete(vertex);
    return true;
  }

  contains(vertex: T): boolean {
    return this._adj.has(vertex);
  }

  // =========================================================================
  // EDGE OPERATIONS
  // =========================================================================

  addEdge(u: T, v: T, weight: number = 1): void {
    this.addVertex(u);
    this.addVertex(v);

    this._adj.get(u)!.set(v, weight);

    if (!this._directed) {
      this._adj.get(v)!.set(u, weight);
    }
  }

  removeEdge(u: T, v: T): boolean {
    if (!this._adj.has(u) || !this._adj.get(u)!.has(v)) {
      return false;
    }

    this._adj.get(u)!.delete(v);

    if (!this._directed) {
      this._adj.get(v)!.delete(u);
    }

    return true;
  }

  hasEdge(u: T, v: T): boolean {
    return this._adj.has(u) && this._adj.get(u)!.has(v);
  }

  getWeight(u: T, v: T): number | null {
    if (this._adj.has(u) && this._adj.get(u)!.has(v)) {
      return this._adj.get(u)!.get(v)!;
    }
    return null;
  }

  // =========================================================================
  // VERTEX PROPERTIES
  // =========================================================================

  neighbors(vertex: T): T[] {
    if (!this._adj.has(vertex)) return [];
    return [...this._adj.get(vertex)!.keys()];
  }

  neighborsWeighted(vertex: T): [T, number][] {
    if (!this._adj.has(vertex)) return [];
    const result: [T, number][] = [];
    for (const [neighbor, weight] of this._adj.get(vertex)!) {
      result.push([neighbor, weight]);
    }
    return result;
  }

  degree(vertex: T): number {
    if (!this._adj.has(vertex)) return 0;
    return this._adj.get(vertex)!.size;
  }

  inDegree(vertex: T): number {
    if (!this._adj.has(vertex)) return 0;
    let count = 0;
    for (const [, neighbors] of this._adj) {
      if (neighbors.has(vertex)) count++;
    }
    return count;
  }

  // =========================================================================
  // UTILITY METHODS
  // =========================================================================

  clear(): void {
    this._adj.clear();
  }

  copy(): GraphCore<T> {
    const newGraph = new GraphCore<T>(this._directed);
    for (const [u, neighbors] of this._adj) {
      for (const [v, weight] of neighbors) {
        if (this._directed || !newGraph.hasEdge(v, u)) {
          newGraph.addEdge(u, v, weight);
        }
      }
    }
    // Add isolated vertices
    for (const v of this._adj.keys()) {
      newGraph.addVertex(v);
    }
    return newGraph;
  }

  toAdjacencyMatrix(): [T[], number[][]] {
    const verts = [...this._adj.keys()];
    const n = verts.length;
    const index = new Map<T, number>();
    for (let i = 0; i < n; i++) {
      index.set(verts[i], i);
    }

    const matrix: number[][] = Array.from({ length: n }, () =>
      new Array<number>(n).fill(Infinity),
    );

    for (let i = 0; i < n; i++) {
      matrix[i][i] = 0;
    }

    for (const [u, neighbors] of this._adj) {
      for (const [v, weight] of neighbors) {
        matrix[index.get(u)!][index.get(v)!] = weight;
      }
    }

    return [verts, matrix];
  }

  static fromEdgeList<T>(
    edges: ([T, T] | [T, T, number])[],
    directed: boolean = false,
  ): GraphCore<T> {
    const graph = new GraphCore<T>(directed);
    for (const edge of edges) {
      if (edge.length === 3) {
        graph.addEdge(edge[0], edge[1], edge[2]);
      } else {
        graph.addEdge(edge[0], edge[1], 1);
      }
    }
    return graph;
  }

  /**
   * Get internal adjacency data. Used by graph algorithms.
   */
  getAdjacencyMap(): Map<T, Map<T, number>> {
    return this._adj;
  }

  [Symbol.iterator](): Iterator<T> {
    return this._adj.keys();
  }

  toString(): string {
    const graphType = this._directed ? "Directed" : "Undirected";
    return `${graphType}Graph(vertices=${this.size}, edges=${this.edgeCount})`;
  }
}
