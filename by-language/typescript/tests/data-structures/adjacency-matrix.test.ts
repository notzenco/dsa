import { describe, test, expect } from "bun:test";
import { AdjacencyMatrix } from "../../src/data-structures/adjacency-matrix";

describe("AdjacencyMatrix", () => {
  describe("constructor and basic properties", () => {
    test("default initialization", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.numVertices).toBe(5);
      expect(g.isDirected).toBe(false);
      expect(g.isWeighted).toBe(false);
      expect(g.edgeCount).toBe(0);
    });

    test("directed graph initialization", () => {
      const g = new AdjacencyMatrix(5, true);
      expect(g.isDirected).toBe(true);
    });

    test("weighted graph initialization", () => {
      const g = new AdjacencyMatrix(5, false, true);
      expect(g.isWeighted).toBe(true);
    });

    test("directed and weighted initialization", () => {
      const g = new AdjacencyMatrix(5, true, true);
      expect(g.isDirected).toBe(true);
      expect(g.isWeighted).toBe(true);
    });

    test("negative numVertices throws", () => {
      expect(() => new AdjacencyMatrix(-1)).toThrow(
        "Number of vertices must be non-negative",
      );
    });

    test("zero-vertex graph", () => {
      const g = new AdjacencyMatrix(0);
      expect(g.numVertices).toBe(0);
      expect(g.edgeCount).toBe(0);
    });
  });

  describe("contains", () => {
    test("valid vertices", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.contains(0)).toBe(true);
      expect(g.contains(4)).toBe(true);
    });

    test("invalid vertices", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.contains(5)).toBe(false);
      expect(g.contains(-1)).toBe(false);
      expect(g.contains(100)).toBe(false);
    });
  });

  describe("iterator", () => {
    test("iterates over all vertices", () => {
      const g = new AdjacencyMatrix(5);
      const vertices = [...g];
      expect(vertices).toEqual([0, 1, 2, 3, 4]);
    });

    test("empty graph iterates to empty array", () => {
      const g = new AdjacencyMatrix(0);
      expect([...g]).toEqual([]);
    });
  });

  describe("edge operations - undirected", () => {
    test("add edge creates symmetric edges", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.addEdge(0, 1)).toBe(true);

      expect(g.hasEdge(0, 1)).toBe(true);
      expect(g.hasEdge(1, 0)).toBe(true);
      expect(g.edgeCount).toBe(1);
    });

    test("add multiple edges", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      expect(g.edgeCount).toBe(3);
      expect(g.hasEdge(0, 1)).toBe(true);
      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.hasEdge(2, 3)).toBe(true);
    });

    test("add edge with invalid vertex returns false", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.addEdge(-1, 0)).toBe(false);
      expect(g.addEdge(0, 5)).toBe(false);
      expect(g.addEdge(10, 20)).toBe(false);
    });

    test("add edge with zero weight returns false", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.addEdge(0, 1, 0)).toBe(false);
    });

    test("remove edge", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(1, 2);

      expect(g.removeEdge(0, 1)).toBe(true);
      expect(g.hasEdge(0, 1)).toBe(false);
      expect(g.hasEdge(1, 0)).toBe(false);
      expect(g.hasEdge(1, 2)).toBe(true);
    });

    test("remove nonexistent edge returns false", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.removeEdge(0, 1)).toBe(false);
    });

    test("remove edge with invalid vertex returns false", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.removeEdge(-1, 0)).toBe(false);
      expect(g.removeEdge(0, 10)).toBe(false);
    });

    test("hasEdge with invalid vertices returns false", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.hasEdge(-1, 0)).toBe(false);
      expect(g.hasEdge(0, 10)).toBe(false);
    });
  });

  describe("edge operations - directed", () => {
    test("add edge is one-directional", () => {
      const g = new AdjacencyMatrix(5, true);
      expect(g.addEdge(0, 1)).toBe(true);

      expect(g.hasEdge(0, 1)).toBe(true);
      expect(g.hasEdge(1, 0)).toBe(false);
      expect(g.edgeCount).toBe(1);
    });

    test("remove edge only removes one direction", () => {
      const g = new AdjacencyMatrix(5, true);
      g.addEdge(0, 1);
      g.addEdge(1, 0);

      g.removeEdge(0, 1);
      expect(g.hasEdge(0, 1)).toBe(false);
      expect(g.hasEdge(1, 0)).toBe(true);
    });

    test("directed edge count", () => {
      const g = new AdjacencyMatrix(5, true);
      g.addEdge(0, 1);
      g.addEdge(1, 0);
      g.addEdge(0, 2);

      expect(g.edgeCount).toBe(3);
    });
  });

  describe("weighted edge operations", () => {
    test("add weighted edge", () => {
      const g = new AdjacencyMatrix(5, false, true);
      g.addEdge(0, 1, 5);

      expect(g.getWeight(0, 1)).toBe(5);
      expect(g.getWeight(1, 0)).toBe(5);
    });

    test("set weight updates existing edge", () => {
      const g = new AdjacencyMatrix(5, false, true);
      g.addEdge(0, 1, 5);
      g.setWeight(0, 1, 10);

      expect(g.getWeight(0, 1)).toBe(10);
    });

    test("getWeight returns 0 for no edge", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.getWeight(0, 1)).toBe(0);
    });

    test("getWeight returns 0 for invalid vertex", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.getWeight(-1, 0)).toBe(0);
      expect(g.getWeight(0, 10)).toBe(0);
    });
  });

  describe("vertex properties", () => {
    test("outDegree directed", () => {
      const g = new AdjacencyMatrix(5, true);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(0, 3);
      g.addEdge(1, 0);

      expect(g.outDegree(0)).toBe(3);
      expect(g.outDegree(1)).toBe(1);
      expect(g.outDegree(2)).toBe(0);
    });

    test("inDegree directed", () => {
      const g = new AdjacencyMatrix(5, true);
      g.addEdge(0, 1);
      g.addEdge(2, 1);
      g.addEdge(3, 1);

      expect(g.inDegree(1)).toBe(3);
      expect(g.inDegree(0)).toBe(0);
    });

    test("degree undirected", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(0, 3);

      expect(g.degree(0)).toBe(3);
      expect(g.degree(1)).toBe(1);
    });

    test("degree of invalid vertex returns 0", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.outDegree(10)).toBe(0);
      expect(g.inDegree(-1)).toBe(0);
    });

    test("neighbors", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(0, 4);

      const neighbors = g.neighbors(0);
      expect(new Set(neighbors)).toEqual(new Set([1, 2, 4]));
    });

    test("neighborsWeighted", () => {
      const g = new AdjacencyMatrix(5, false, true);
      g.addEdge(0, 1, 5);
      g.addEdge(0, 2, 3);

      const neighbors = g.neighborsWeighted(0);
      const asSet = new Set(neighbors.map(([n, w]) => `${n}:${w}`));
      expect(asSet).toEqual(new Set(["1:5", "2:3"]));
    });

    test("neighbors of invalid vertex returns empty", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.neighbors(10)).toEqual([]);
      expect(g.neighborsWeighted(10)).toEqual([]);
    });
  });

  describe("BFS traversal", () => {
    test("bfs visits all connected vertices", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 4);

      const result = g.bfs(0);
      expect(result[0]).toBe(0);
      expect(new Set(result)).toEqual(new Set([0, 1, 2, 3, 4]));
    });

    test("bfs on disconnected graph", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(3, 4);

      const result = g.bfs(0);
      expect(new Set(result)).toEqual(new Set([0, 1]));
    });

    test("bfs with invalid source returns empty", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.bfs(10)).toEqual([]);
    });

    test("bfs single vertex", () => {
      const g = new AdjacencyMatrix(1);
      expect(g.bfs(0)).toEqual([0]);
    });

    test("bfs directed graph", () => {
      const g = new AdjacencyMatrix(4, true);
      g.addEdge(0, 1);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      const result = g.bfs(0);
      expect(result).toEqual([0, 1, 2, 3]);

      // Starting from 3 should only reach 3
      const result2 = g.bfs(3);
      expect(result2).toEqual([3]);
    });
  });

  describe("DFS traversal", () => {
    test("dfs visits all connected vertices", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 4);

      const result = g.dfs(0);
      expect(result[0]).toBe(0);
      expect(new Set(result)).toEqual(new Set([0, 1, 2, 3, 4]));
    });

    test("dfsRecursive visits all connected vertices", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 4);

      const result = g.dfsRecursive(0);
      expect(result[0]).toBe(0);
      expect(new Set(result)).toEqual(new Set([0, 1, 2, 3, 4]));
    });

    test("dfs with invalid source returns empty", () => {
      const g = new AdjacencyMatrix(5);
      expect(g.dfs(10)).toEqual([]);
      expect(g.dfsRecursive(10)).toEqual([]);
    });

    test("dfs and dfsRecursive produce same vertex set", () => {
      const g = new AdjacencyMatrix(6);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 4);
      g.addEdge(3, 5);

      const iterative = g.dfs(0);
      const recursive = g.dfsRecursive(0);
      expect(new Set(iterative)).toEqual(new Set(recursive));
    });
  });

  describe("isConnected", () => {
    test("connected graph", () => {
      const g = new AdjacencyMatrix(4);
      g.addEdge(0, 1);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      expect(g.isConnected()).toBe(true);
    });

    test("disconnected graph", () => {
      const g = new AdjacencyMatrix(4);
      g.addEdge(0, 1);
      g.addEdge(2, 3);

      expect(g.isConnected()).toBe(false);
    });

    test("empty graph is connected", () => {
      const g = new AdjacencyMatrix(0);
      expect(g.isConnected()).toBe(true);
    });

    test("single vertex is connected", () => {
      const g = new AdjacencyMatrix(1);
      expect(g.isConnected()).toBe(true);
    });
  });

  describe("Floyd-Warshall", () => {
    test("basic weighted undirected", () => {
      const g = new AdjacencyMatrix(4, false, true);
      g.addEdge(0, 1, 3);
      g.addEdge(0, 2, 6);
      g.addEdge(1, 2, 2);
      g.addEdge(2, 3, 1);

      const [dist, success] = g.floydWarshall();
      expect(success).toBe(true);

      expect(dist[0][0]).toBe(0);
      expect(dist[0][1]).toBe(3);
      expect(dist[0][2]).toBe(5); // Through vertex 1
      expect(dist[0][3]).toBe(6); // Through vertex 1, 2
    });

    test("directed graph", () => {
      const g = new AdjacencyMatrix(3, true, true);
      g.addEdge(0, 1, 1);
      g.addEdge(1, 2, 2);

      const [dist, success] = g.floydWarshall();
      expect(success).toBe(true);

      expect(dist[0][2]).toBe(3);
      expect(dist[2][0]).toBe(Infinity);
    });

    test("empty graph", () => {
      const g = new AdjacencyMatrix(0);
      const [dist, success] = g.floydWarshall();
      expect(success).toBe(true);
      expect(dist).toEqual([]);
    });

    test("single vertex", () => {
      const g = new AdjacencyMatrix(1);
      const [dist, success] = g.floydWarshall();
      expect(success).toBe(true);
      expect(dist[0][0]).toBe(0);
    });

    test("negative weight with no negative cycle succeeds", () => {
      const g = new AdjacencyMatrix(3, true, true);
      g.addEdge(0, 1, 4);
      g.addEdge(0, 2, 5);
      g.addEdge(1, 2, -2);

      const [dist, success] = g.floydWarshall();
      expect(success).toBe(true);
      expect(dist[0][2]).toBe(2); // 0->1->2 = 4 + (-2) = 2
    });
  });

  describe("transitive closure", () => {
    test("basic directed", () => {
      const g = new AdjacencyMatrix(4, true);
      g.addEdge(0, 1);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      const closure = g.transitiveClosure();

      // Diagonal is true
      for (let i = 0; i < 4; i++) {
        expect(closure[i][i]).toBe(true);
      }

      // Path from 0 to all
      expect(closure[0][1]).toBe(true);
      expect(closure[0][2]).toBe(true);
      expect(closure[0][3]).toBe(true);

      // No path backwards
      expect(closure[3][0]).toBe(false);
      expect(closure[2][0]).toBe(false);
    });

    test("undirected graph has symmetric closure", () => {
      const g = new AdjacencyMatrix(3);
      g.addEdge(0, 1);
      g.addEdge(1, 2);

      const closure = g.transitiveClosure();

      // All reachable from all
      for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
          expect(closure[i][j]).toBe(true);
        }
      }
    });

    test("disconnected graph", () => {
      const g = new AdjacencyMatrix(4, true);
      g.addEdge(0, 1);
      // vertices 2, 3 isolated

      const closure = g.transitiveClosure();
      expect(closure[0][1]).toBe(true);
      expect(closure[0][2]).toBe(false);
      expect(closure[0][3]).toBe(false);
      expect(closure[2][3]).toBe(false);
    });
  });

  describe("utility methods", () => {
    test("clear removes all edges", () => {
      const g = new AdjacencyMatrix(5);
      g.addEdge(0, 1);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      g.clear();

      expect(g.edgeCount).toBe(0);
      expect(g.hasEdge(0, 1)).toBe(false);
      expect(g.numVertices).toBe(5); // vertices remain
    });

    test("copy creates independent deep copy", () => {
      const g = new AdjacencyMatrix(5, true, true);
      g.addEdge(0, 1, 5);
      g.addEdge(1, 2, 3);

      const cp = g.copy();

      expect(cp.numVertices).toBe(g.numVertices);
      expect(cp.isDirected).toBe(g.isDirected);
      expect(cp.isWeighted).toBe(g.isWeighted);
      expect(cp.hasEdge(0, 1)).toBe(true);
      expect(cp.getWeight(0, 1)).toBe(5);

      // Modify copy should not affect original
      cp.removeEdge(0, 1);
      expect(g.hasEdge(0, 1)).toBe(true);
    });

    test("transpose reverses edges", () => {
      const g = new AdjacencyMatrix(3, true);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      g.addEdge(1, 2);

      const trans = g.transpose();

      expect(trans.hasEdge(1, 0)).toBe(true);
      expect(trans.hasEdge(2, 0)).toBe(true);
      expect(trans.hasEdge(2, 1)).toBe(true);
      expect(trans.hasEdge(0, 1)).toBe(false);
    });

    test("matrix returns a copy", () => {
      const g = new AdjacencyMatrix(3);
      g.addEdge(0, 1);

      const matrix = g.matrix;
      matrix[0][2] = 1;
      expect(g.hasEdge(0, 2)).toBe(false);
    });

    test("toString", () => {
      const g = new AdjacencyMatrix(3);
      g.addEdge(0, 1);

      const s = g.toString();
      expect(s).toContain("Adjacency Matrix");
      expect(s).toContain("3 vertices");
    });

    test("toString empty graph", () => {
      const g = new AdjacencyMatrix(0);
      expect(g.toString()).toBe("(empty graph)");
    });
  });

  describe("edge cases", () => {
    test("self loop", () => {
      const g = new AdjacencyMatrix(3);
      g.addEdge(0, 0, 1);

      expect(g.hasEdge(0, 0)).toBe(true);
      expect(g.edgeCount).toBe(1);
    });

    test("complete graph", () => {
      const n = 5;
      const g = new AdjacencyMatrix(n);

      for (let i = 0; i < n; i++) {
        for (let j = i + 1; j < n; j++) {
          g.addEdge(i, j);
        }
      }

      expect(g.edgeCount).toBe((n * (n - 1)) / 2);
      expect(g.isConnected()).toBe(true);
    });

    test("overwrite existing edge with new weight", () => {
      const g = new AdjacencyMatrix(3, false, true);
      g.addEdge(0, 1, 5);
      g.addEdge(0, 1, 10);

      expect(g.getWeight(0, 1)).toBe(10);
      expect(g.edgeCount).toBe(1);
    });

    test("large graph BFS/DFS consistency", () => {
      const n = 20;
      const g = new AdjacencyMatrix(n);
      // Create a path graph
      for (let i = 0; i < n - 1; i++) {
        g.addEdge(i, i + 1);
      }

      const bfsResult = g.bfs(0);
      const dfsResult = g.dfs(0);
      const dfsRecResult = g.dfsRecursive(0);

      expect(bfsResult.length).toBe(n);
      expect(dfsResult.length).toBe(n);
      expect(dfsRecResult.length).toBe(n);
      expect(new Set(bfsResult)).toEqual(new Set(dfsResult));
    });
  });
});
