/**
 * Adjacency Matrix Graph Representation
 *
 * For a graph with vertices {0, 1, 2, 3}:
 *
 *     0 --5-- 1                  0   1   2   3
 *     |       |               0 [ 0   5   0   2 ]
 *     2       3               1 [ 5   0   3   0 ]
 *     |       |               2 [ 0   3   0   0 ]
 *     3 ------+               3 [ 2   0   0   0 ]
 *
 * 0 = no edge, value = edge weight
 *
 * COMPLEXITY:
 * | Operation         | Adjacency Matrix | Adjacency List |
 * |-------------------|------------------|----------------|
 * | Space             | O(V^2)           | O(V + E)       |
 * | Add Edge          | O(1)             | O(1)           |
 * | Remove Edge       | O(1)             | O(E)           |
 * | Check Edge        | O(1)             | O(degree)      |
 * | Get Neighbors     | O(V)             | O(degree)      |
 * | Iterate All Edges | O(V^2)           | O(E)           |
 */

export class AdjacencyMatrix {
  private readonly _numVertices: number;
  private readonly _directed: boolean;
  private readonly _weighted: boolean;
  private readonly _matrix: number[][];

  constructor(
    numVertices: number,
    directed: boolean = false,
    weighted: boolean = false,
  ) {
    if (numVertices < 0) {
      throw new Error("Number of vertices must be non-negative");
    }

    this._numVertices = numVertices;
    this._directed = directed;
    this._weighted = weighted;
    this._matrix = Array.from({ length: numVertices }, () =>
      new Array<number>(numVertices).fill(0),
    );
  }

  // =========================================================================
  // PROPERTIES
  // =========================================================================

  get numVertices(): number {
    return this._numVertices;
  }

  get isDirected(): boolean {
    return this._directed;
  }

  get isWeighted(): boolean {
    return this._weighted;
  }

  get edgeCount(): number {
    if (this._directed) {
      let count = 0;
      for (let i = 0; i < this._numVertices; i++) {
        for (let j = 0; j < this._numVertices; j++) {
          if (this._matrix[i][j] !== 0) count++;
        }
      }
      return count;
    } else {
      let count = 0;
      for (let i = 0; i < this._numVertices; i++) {
        for (let j = i; j < this._numVertices; j++) {
          if (this._matrix[i][j] !== 0) count++;
        }
      }
      return count;
    }
  }

  get matrix(): number[][] {
    return this._matrix.map((row) => [...row]);
  }

  // =========================================================================
  // EDGE OPERATIONS
  // =========================================================================

  addEdge(src: number, dest: number, weight: number = 1): boolean {
    if (!this._isValidVertex(src) || !this._isValidVertex(dest)) {
      return false;
    }
    if (weight === 0) {
      return false;
    }

    this._matrix[src][dest] = weight;

    if (!this._directed) {
      this._matrix[dest][src] = weight;
    }

    return true;
  }

  removeEdge(src: number, dest: number): boolean {
    if (!this._isValidVertex(src) || !this._isValidVertex(dest)) {
      return false;
    }

    const hadEdge = this._matrix[src][dest] !== 0;
    this._matrix[src][dest] = 0;

    if (!this._directed) {
      this._matrix[dest][src] = 0;
    }

    return hadEdge;
  }

  hasEdge(src: number, dest: number): boolean {
    if (!this._isValidVertex(src) || !this._isValidVertex(dest)) {
      return false;
    }
    return this._matrix[src][dest] !== 0;
  }

  getWeight(src: number, dest: number): number {
    if (!this._isValidVertex(src) || !this._isValidVertex(dest)) {
      return 0;
    }
    return this._matrix[src][dest];
  }

  setWeight(src: number, dest: number, weight: number): boolean {
    return this.addEdge(src, dest, weight);
  }

  // =========================================================================
  // VERTEX PROPERTIES
  // =========================================================================

  outDegree(vertex: number): number {
    if (!this._isValidVertex(vertex)) return 0;
    let count = 0;
    for (let j = 0; j < this._numVertices; j++) {
      if (this._matrix[vertex][j] !== 0) count++;
    }
    return count;
  }

  inDegree(vertex: number): number {
    if (!this._isValidVertex(vertex)) return 0;
    let count = 0;
    for (let i = 0; i < this._numVertices; i++) {
      if (this._matrix[i][vertex] !== 0) count++;
    }
    return count;
  }

  degree(vertex: number): number {
    return this.outDegree(vertex);
  }

  neighbors(vertex: number): number[] {
    if (!this._isValidVertex(vertex)) return [];
    const result: number[] = [];
    for (let j = 0; j < this._numVertices; j++) {
      if (this._matrix[vertex][j] !== 0) {
        result.push(j);
      }
    }
    return result;
  }

  neighborsWeighted(vertex: number): [number, number][] {
    if (!this._isValidVertex(vertex)) return [];
    const result: [number, number][] = [];
    for (let j = 0; j < this._numVertices; j++) {
      if (this._matrix[vertex][j] !== 0) {
        result.push([j, this._matrix[vertex][j]]);
      }
    }
    return result;
  }

  // =========================================================================
  // GRAPH TRAVERSAL
  // =========================================================================

  bfs(source: number): number[] {
    if (!this._isValidVertex(source)) return [];

    const visited = new Array<boolean>(this._numVertices).fill(false);
    const result: number[] = [];
    const queue: number[] = [source];
    visited[source] = true;
    let front = 0;

    while (front < queue.length) {
      const vertex = queue[front++];
      result.push(vertex);

      for (let j = 0; j < this._numVertices; j++) {
        if (this._matrix[vertex][j] !== 0 && !visited[j]) {
          visited[j] = true;
          queue.push(j);
        }
      }
    }

    return result;
  }

  dfs(source: number): number[] {
    if (!this._isValidVertex(source)) return [];

    const visited = new Array<boolean>(this._numVertices).fill(false);
    const result: number[] = [];
    const stack: number[] = [source];

    while (stack.length > 0) {
      const vertex = stack.pop()!;

      if (!visited[vertex]) {
        visited[vertex] = true;
        result.push(vertex);

        // Add neighbors in reverse order for consistent ordering
        for (let j = this._numVertices - 1; j >= 0; j--) {
          if (this._matrix[vertex][j] !== 0 && !visited[j]) {
            stack.push(j);
          }
        }
      }
    }

    return result;
  }

  dfsRecursive(source: number): number[] {
    if (!this._isValidVertex(source)) return [];

    const visited = new Array<boolean>(this._numVertices).fill(false);
    const result: number[] = [];

    const dfsHelper = (vertex: number): void => {
      visited[vertex] = true;
      result.push(vertex);
      for (let j = 0; j < this._numVertices; j++) {
        if (this._matrix[vertex][j] !== 0 && !visited[j]) {
          dfsHelper(j);
        }
      }
    };

    dfsHelper(source);
    return result;
  }

  // =========================================================================
  // GRAPH PROPERTIES
  // =========================================================================

  isConnected(): boolean {
    if (this._numVertices === 0) return true;
    return this.bfs(0).length === this._numVertices;
  }

  // =========================================================================
  // ALL-PAIRS SHORTEST PATH
  // =========================================================================

  floydWarshall(): [number[][], boolean] {
    const n = this._numVertices;

    // Initialize distance matrix
    const dist: number[][] = Array.from({ length: n }, () =>
      new Array<number>(n).fill(Infinity),
    );

    for (let i = 0; i < n; i++) {
      dist[i][i] = 0;
    }

    for (let i = 0; i < n; i++) {
      for (let j = 0; j < n; j++) {
        if (this._matrix[i][j] !== 0) {
          dist[i][j] = this._matrix[i][j];
        }
      }
    }

    // Floyd-Warshall
    for (let k = 0; k < n; k++) {
      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
          if (dist[i][k] !== Infinity && dist[k][j] !== Infinity) {
            if (dist[i][k] + dist[k][j] < dist[i][j]) {
              dist[i][j] = dist[i][k] + dist[k][j];
            }
          }
        }
      }
    }

    // Check for negative cycles
    for (let i = 0; i < n; i++) {
      if (dist[i][i] < 0) {
        return [dist, false];
      }
    }

    return [dist, true];
  }

  transitiveClosure(): boolean[][] {
    const n = this._numVertices;

    const closure: boolean[][] = Array.from({ length: n }, (_, i) =>
      Array.from(
        { length: n },
        (_, j) => this._matrix[i][j] !== 0 || i === j,
      ),
    );

    for (let k = 0; k < n; k++) {
      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
          closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
        }
      }
    }

    return closure;
  }

  // =========================================================================
  // UTILITY METHODS
  // =========================================================================

  clear(): void {
    for (let i = 0; i < this._numVertices; i++) {
      for (let j = 0; j < this._numVertices; j++) {
        this._matrix[i][j] = 0;
      }
    }
  }

  copy(): AdjacencyMatrix {
    const newGraph = new AdjacencyMatrix(
      this._numVertices,
      this._directed,
      this._weighted,
    );
    for (let i = 0; i < this._numVertices; i++) {
      for (let j = 0; j < this._numVertices; j++) {
        newGraph._matrix[i][j] = this._matrix[i][j];
      }
    }
    return newGraph;
  }

  transpose(): AdjacencyMatrix {
    const trans = new AdjacencyMatrix(
      this._numVertices,
      this._directed,
      this._weighted,
    );
    for (let i = 0; i < this._numVertices; i++) {
      for (let j = 0; j < this._numVertices; j++) {
        trans._matrix[j][i] = this._matrix[i][j];
      }
    }
    return trans;
  }

  toString(): string {
    if (this._numVertices === 0) {
      return "(empty graph)";
    }

    const graphType = this._directed ? "directed" : "undirected";
    const weightType = this._weighted ? "weighted" : "unweighted";
    const lines: string[] = [
      `Adjacency Matrix (${this._numVertices} vertices, ${graphType}, ${weightType}):`,
    ];

    // Header
    let header = "    ";
    for (let j = 0; j < this._numVertices; j++) {
      header += String(j).padStart(4);
    }
    lines.push(header);

    // Rows
    for (let i = 0; i < this._numVertices; i++) {
      let row = String(i).padStart(3) + ":";
      for (let j = 0; j < this._numVertices; j++) {
        if (this._matrix[i][j] === 0) {
          row += "   .";
        } else {
          row += String(this._matrix[i][j]).padStart(4);
        }
      }
      lines.push(row);
    }

    return lines.join("\n");
  }

  contains(vertex: number): boolean {
    return this._isValidVertex(vertex);
  }

  [Symbol.iterator](): Iterator<number> {
    let index = 0;
    const n = this._numVertices;
    return {
      next(): IteratorResult<number> {
        if (index < n) {
          return { value: index++, done: false };
        }
        return { value: undefined as unknown as number, done: true };
      },
    };
  }

  private _isValidVertex(vertex: number): boolean {
    return vertex >= 0 && vertex < this._numVertices;
  }
}
