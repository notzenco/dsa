import { describe, test, expect } from "bun:test";
import { GraphCore } from "../../src/data-structures/graph-core";

// =============================================================================
// Constructor and basic properties
// =============================================================================

describe("GraphCore", () => {
  describe("constructor and basic properties", () => {
    test("default undirected graph", () => {
      const g = new GraphCore<number>();
      expect(g.size).toBe(0);
      expect(g.isDirected).toBe(false);
      expect(g.edgeCount).toBe(0);
      expect(g.vertices).toEqual([]);
    });

    test("directed graph", () => {
      const g = new GraphCore<number>(true);
      expect(g.isDirected).toBe(true);
      expect(g.size).toBe(0);
    });

    test("toString undirected", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      const s = g.toString();
      expect(s).toContain("Undirected");
      expect(s).toContain("vertices=2");
      expect(s).toContain("edges=1");
    });

    test("toString directed", () => {
      const g = new GraphCore<string>(true);
      g.addEdge("A", "B");
      const s = g.toString();
      expect(s).toContain("Directed");
      expect(s).toContain("vertices=2");
      expect(s).toContain("edges=1");
    });

    test("toString empty graph", () => {
      const g = new GraphCore<number>();
      expect(g.toString()).toBe("UndirectedGraph(vertices=0, edges=0)");
    });
  });

  // ===========================================================================
  // Vertex operations
  // ===========================================================================

  describe("addVertex", () => {
    test("adds a single vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(g.contains(1)).toBe(true);
      expect(g.size).toBe(1);
    });

    test("adding duplicate vertex is idempotent", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(1);
      expect(g.size).toBe(1);
    });

    test("add multiple vertices", () => {
      const g = new GraphCore<string>();
      g.addVertex("A");
      g.addVertex("B");
      g.addVertex("C");
      expect(g.size).toBe(3);
      expect(g.contains("A")).toBe(true);
      expect(g.contains("B")).toBe(true);
      expect(g.contains("C")).toBe(true);
    });

    test("contains returns false for missing vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(g.contains(2)).toBe(false);
    });
  });

  describe("removeVertex", () => {
    test("removes vertex and all its edges in undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(1, 3);

      expect(g.removeVertex(2)).toBe(true);
      expect(g.contains(2)).toBe(false);
      expect(g.hasEdge(1, 2)).toBe(false);
      expect(g.hasEdge(2, 3)).toBe(false);
      expect(g.hasEdge(1, 3)).toBe(true);
      expect(g.size).toBe(2);
    });

    test("removes vertex and cleans up incoming edges in directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(3, 2);
      g.addEdge(4, 2);
      g.addEdge(2, 5);

      g.removeVertex(2);
      expect(g.contains(2)).toBe(false);
      expect(g.hasEdge(1, 2)).toBe(false);
      expect(g.hasEdge(3, 2)).toBe(false);
      expect(g.hasEdge(4, 2)).toBe(false);
      expect(g.contains(1)).toBe(true);
      expect(g.contains(5)).toBe(true);
    });

    test("returns false for nonexistent vertex", () => {
      const g = new GraphCore<number>();
      expect(g.removeVertex(99)).toBe(false);
    });

    test("removing isolated vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(2);
      expect(g.removeVertex(1)).toBe(true);
      expect(g.size).toBe(1);
      expect(g.contains(1)).toBe(false);
      expect(g.contains(2)).toBe(true);
    });

    test("removing vertex updates edge count", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 3);
      expect(g.edgeCount).toBe(3);

      g.removeVertex(1);
      expect(g.edgeCount).toBe(1);
    });
  });

  describe("vertices property", () => {
    test("returns all vertices", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      expect(new Set(g.vertices)).toEqual(new Set([1, 2, 3]));
    });

    test("returns empty array for empty graph", () => {
      const g = new GraphCore<number>();
      expect(g.vertices).toEqual([]);
    });

    test("includes isolated vertices", () => {
      const g = new GraphCore<number>();
      g.addVertex(10);
      g.addEdge(1, 2);
      expect(new Set(g.vertices)).toEqual(new Set([10, 1, 2]));
    });
  });

  // ===========================================================================
  // Edge operations - undirected
  // ===========================================================================

  describe("edge operations - undirected", () => {
    test("addEdge creates symmetric edges", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);

      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.hasEdge(2, 1)).toBe(true);
      expect(g.edgeCount).toBe(1);
    });

    test("addEdge auto-creates vertices", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);

      expect(g.contains(1)).toBe(true);
      expect(g.contains(2)).toBe(true);
      expect(g.size).toBe(2);
    });

    test("addEdge with weight", () => {
      const g = new GraphCore<string>();
      g.addEdge("A", "B", 5.0);

      expect(g.getWeight("A", "B")).toBe(5.0);
      expect(g.getWeight("B", "A")).toBe(5.0);
    });

    test("default weight is 1", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      expect(g.getWeight(1, 2)).toBe(1);
      expect(g.getWeight(2, 1)).toBe(1);
    });

    test("overwrite edge weight", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 5);
      g.addEdge(1, 2, 10);
      expect(g.getWeight(1, 2)).toBe(10);
      expect(g.edgeCount).toBe(1);
    });

    test("removeEdge symmetric removal", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      expect(g.removeEdge(1, 2)).toBe(true);
      expect(g.hasEdge(1, 2)).toBe(false);
      expect(g.hasEdge(2, 1)).toBe(false);
      expect(g.hasEdge(2, 3)).toBe(true);
      expect(g.edgeCount).toBe(1);
    });

    test("removeEdge nonexistent returns false", () => {
      const g = new GraphCore<number>();
      expect(g.removeEdge(1, 2)).toBe(false);
    });

    test("removeEdge with existing vertex but no edge returns false", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(2);
      expect(g.removeEdge(1, 2)).toBe(false);
    });

    test("hasEdge for nonexistent vertex returns false", () => {
      const g = new GraphCore<number>();
      expect(g.hasEdge(1, 2)).toBe(false);
    });

    test("getWeight returns null for no edge", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(g.getWeight(1, 2)).toBeNull();
    });

    test("getWeight returns null for nonexistent vertices", () => {
      const g = new GraphCore<number>();
      expect(g.getWeight(99, 100)).toBeNull();
    });

    test("edgeCount with multiple edges", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);
      g.addEdge(4, 1);
      expect(g.edgeCount).toBe(4);
    });

    test("self loop in undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 1);
      expect(g.hasEdge(1, 1)).toBe(true);
      expect(g.size).toBe(1);
    });
  });

  // ===========================================================================
  // Edge operations - directed
  // ===========================================================================

  describe("edge operations - directed", () => {
    test("addEdge is one-directional", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);

      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.hasEdge(2, 1)).toBe(false);
      expect(g.edgeCount).toBe(1);
    });

    test("addEdge both directions creates two edges", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 1);

      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.hasEdge(2, 1)).toBe(true);
      expect(g.edgeCount).toBe(2);
    });

    test("removeEdge only removes one direction", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 1);

      g.removeEdge(1, 2);
      expect(g.hasEdge(1, 2)).toBe(false);
      expect(g.hasEdge(2, 1)).toBe(true);
      expect(g.edgeCount).toBe(1);
    });

    test("edgeCount directed with multiple edges", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 1);
      g.addEdge(1, 3);
      expect(g.edgeCount).toBe(3);
    });

    test("directed weighted edges are asymmetric", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 5);
      g.addEdge(2, 1, 10);

      expect(g.getWeight(1, 2)).toBe(5);
      expect(g.getWeight(2, 1)).toBe(10);
    });
  });

  // ===========================================================================
  // Vertex properties: neighbors, degree, inDegree
  // ===========================================================================

  describe("neighbors", () => {
    test("returns all neighbors in undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(1, 4);

      expect(new Set(g.neighbors(1))).toEqual(new Set([2, 3, 4]));
    });

    test("returns outgoing neighbors in directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(4, 1);

      expect(new Set(g.neighbors(1))).toEqual(new Set([2, 3]));
    });

    test("returns empty for nonexistent vertex", () => {
      const g = new GraphCore<number>();
      expect(g.neighbors(99)).toEqual([]);
    });

    test("returns empty for isolated vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(g.neighbors(1)).toEqual([]);
    });
  });

  describe("neighborsWeighted", () => {
    test("returns neighbors with weights", () => {
      const g = new GraphCore<string>();
      g.addEdge("A", "B", 5);
      g.addEdge("A", "C", 3);

      const nw = g.neighborsWeighted("A");
      const asMap = new Map(nw);
      expect(asMap.get("B")).toBe(5);
      expect(asMap.get("C")).toBe(3);
      expect(nw.length).toBe(2);
    });

    test("returns empty for nonexistent vertex", () => {
      const g = new GraphCore<number>();
      expect(g.neighborsWeighted(99)).toEqual([]);
    });
  });

  describe("degree", () => {
    test("degree in undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(1, 4);

      expect(g.degree(1)).toBe(3);
      expect(g.degree(2)).toBe(1);
    });

    test("degree in directed graph counts outgoing", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(4, 1);

      expect(g.degree(1)).toBe(2); // outgoing: 2, 3
    });

    test("degree of nonexistent vertex returns 0", () => {
      const g = new GraphCore<number>();
      expect(g.degree(99)).toBe(0);
    });

    test("degree of isolated vertex is 0", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(g.degree(1)).toBe(0);
    });
  });

  describe("inDegree", () => {
    test("inDegree directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(3, 2);
      g.addEdge(4, 2);

      expect(g.inDegree(2)).toBe(3);
      expect(g.inDegree(1)).toBe(0);
    });

    test("inDegree undirected graph counts adjacencies", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      // Vertex 1 appears in neighbors of 2 and 3
      expect(g.inDegree(1)).toBe(2);
    });

    test("inDegree of nonexistent vertex returns 0", () => {
      const g = new GraphCore<number>();
      expect(g.inDegree(99)).toBe(0);
    });

    test("inDegree of isolated vertex is 0", () => {
      const g = new GraphCore<number>(true);
      g.addVertex(1);
      expect(g.inDegree(1)).toBe(0);
    });

    test("inDegree with self-loop", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 1);
      expect(g.inDegree(1)).toBe(1);
    });
  });

  // ===========================================================================
  // Utility methods
  // ===========================================================================

  describe("clear", () => {
    test("removes all vertices and edges", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      g.clear();
      expect(g.size).toBe(0);
      expect(g.edgeCount).toBe(0);
      expect(g.vertices).toEqual([]);
    });

    test("clear on empty graph is safe", () => {
      const g = new GraphCore<number>();
      g.clear();
      expect(g.size).toBe(0);
    });
  });

  describe("copy", () => {
    test("creates independent deep copy of undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 5.0);
      g.addEdge(2, 3, 3.0);

      const g2 = g.copy();
      expect(g2.hasEdge(1, 2)).toBe(true);
      expect(g2.getWeight(1, 2)).toBe(5.0);
      expect(g2.edgeCount).toBe(2);
      expect(g2.size).toBe(3);

      // Modify original, copy should be unchanged
      g.removeEdge(1, 2);
      expect(g2.hasEdge(1, 2)).toBe(true);
    });

    test("copy preserves directed flag", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);

      const g2 = g.copy();
      expect(g2.isDirected).toBe(true);
      expect(g2.hasEdge(1, 2)).toBe(true);
      expect(g2.hasEdge(2, 1)).toBe(false);
    });

    test("copy preserves isolated vertices", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(2);
      g.addEdge(3, 4);

      const g2 = g.copy();
      expect(g2.contains(1)).toBe(true);
      expect(g2.contains(2)).toBe(true);
      expect(g2.hasEdge(3, 4)).toBe(true);
      expect(g2.size).toBe(4);
    });

    test("modifying copy does not affect original", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);

      const g2 = g.copy();
      g2.addEdge(3, 4);
      expect(g.contains(3)).toBe(false);
      expect(g.size).toBe(2);
    });

    test("copy of empty graph", () => {
      const g = new GraphCore<number>();
      const g2 = g.copy();
      expect(g2.size).toBe(0);
      expect(g2.edgeCount).toBe(0);
    });
  });

  describe("toAdjacencyMatrix", () => {
    test("basic undirected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(0, 1, 2.0);
      g.addEdge(1, 2, 3.0);

      const [vertices, matrix] = g.toAdjacencyMatrix();
      const idx: Record<number, number> = {};
      for (let i = 0; i < vertices.length; i++) {
        idx[vertices[i]] = i;
      }

      expect(matrix[idx[0]][idx[1]]).toBe(2.0);
      expect(matrix[idx[1]][idx[0]]).toBe(2.0);
      expect(matrix[idx[1]][idx[2]]).toBe(3.0);
      expect(matrix[idx[0]][idx[0]]).toBe(0);
      expect(matrix[idx[0]][idx[2]]).toBe(Infinity);
    });

    test("directed graph matrix is asymmetric", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(0, 1, 5);

      const [vertices, matrix] = g.toAdjacencyMatrix();
      const idx: Record<number, number> = {};
      for (let i = 0; i < vertices.length; i++) {
        idx[vertices[i]] = i;
      }

      expect(matrix[idx[0]][idx[1]]).toBe(5);
      expect(matrix[idx[1]][idx[0]]).toBe(Infinity);
    });

    test("empty graph returns empty matrix", () => {
      const g = new GraphCore<number>();
      const [vertices, matrix] = g.toAdjacencyMatrix();
      expect(vertices).toEqual([]);
      expect(matrix).toEqual([]);
    });

    test("diagonal is always 0", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(2);
      g.addVertex(3);

      const [, matrix] = g.toAdjacencyMatrix();
      for (let i = 0; i < matrix.length; i++) {
        expect(matrix[i][i]).toBe(0);
      }
    });
  });

  describe("fromEdgeList", () => {
    test("weighted edges", () => {
      const edges: [number, number, number][] = [
        [1, 2, 1.0],
        [2, 3, 2.0],
        [3, 1, 3.0],
      ];
      const g = GraphCore.fromEdgeList(edges);

      expect(g.size).toBe(3);
      expect(g.edgeCount).toBe(3);
      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.getWeight(2, 3)).toBe(2.0);
      expect(g.getWeight(3, 1)).toBe(3.0);
      expect(g.isDirected).toBe(false);
    });

    test("unweighted edges default weight is 1", () => {
      const edges: [number, number][] = [
        [1, 2],
        [2, 3],
      ];
      const g = GraphCore.fromEdgeList(edges);

      expect(g.hasEdge(1, 2)).toBe(true);
      expect(g.getWeight(1, 2)).toBe(1);
    });

    test("directed edge list", () => {
      const edges: [string, string][] = [
        ["A", "B"],
        ["B", "C"],
      ];
      const g = GraphCore.fromEdgeList(edges, true);

      expect(g.isDirected).toBe(true);
      expect(g.hasEdge("A", "B")).toBe(true);
      expect(g.hasEdge("B", "A")).toBe(false);
    });

    test("empty edge list", () => {
      const g = GraphCore.fromEdgeList<number>([]);
      expect(g.size).toBe(0);
      expect(g.edgeCount).toBe(0);
    });

    test("mixed weighted and unweighted edges", () => {
      const edges: ([number, number] | [number, number, number])[] = [
        [1, 2, 5],
        [2, 3],
      ];
      const g = GraphCore.fromEdgeList(edges);

      expect(g.getWeight(1, 2)).toBe(5);
      expect(g.getWeight(2, 3)).toBe(1);
    });
  });

  // ===========================================================================
  // Iterator support
  // ===========================================================================

  describe("iterator support", () => {
    test("iterate over vertices with spread", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      const vertices = [...g];
      expect(new Set(vertices)).toEqual(new Set([1, 2, 3]));
    });

    test("iterate over empty graph", () => {
      const g = new GraphCore<number>();
      expect([...g]).toEqual([]);
    });

    test("for...of loop", () => {
      const g = new GraphCore<number>();
      g.addVertex(10);
      g.addVertex(20);

      const collected: number[] = [];
      for (const v of g) {
        collected.push(v);
      }
      expect(new Set(collected)).toEqual(new Set([10, 20]));
    });
  });

  // ===========================================================================
  // String vertex type
  // ===========================================================================

  describe("string vertex type", () => {
    test("works with string vertices", () => {
      const g = new GraphCore<string>();
      g.addEdge("Alice", "Bob");
      g.addEdge("Bob", "Charlie");

      expect(g.contains("Alice")).toBe(true);
      expect(g.hasEdge("Alice", "Bob")).toBe(true);
      expect(new Set(g.neighbors("Bob"))).toEqual(
        new Set(["Alice", "Charlie"]),
      );
    });
  });

  // ===========================================================================
  // Edge cases
  // ===========================================================================

  describe("edge cases", () => {
    test("self loop", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 1);
      expect(g.hasEdge(1, 1)).toBe(true);
      expect(g.degree(1)).toBe(1);
    });

    test("self loop directed", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 1);
      expect(g.hasEdge(1, 1)).toBe(true);
      expect(g.degree(1)).toBe(1);
      expect(g.edgeCount).toBe(1);
    });

    test("getAdjacencyMap returns internal map", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);

      const adj = g.getAdjacencyMap();
      expect(adj.has(1)).toBe(true);
      expect(adj.get(1)!.has(2)).toBe(true);
    });

    test("large graph with many vertices", () => {
      const g = new GraphCore<number>();
      for (let i = 0; i < 100; i++) {
        g.addEdge(i, i + 1, i * 0.5);
      }
      expect(g.size).toBe(101);
      expect(g.edgeCount).toBe(100);
      expect(g.getWeight(50, 51)).toBe(25);
    });

    test("remove all edges then all vertices", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      g.removeEdge(1, 2);
      g.removeEdge(2, 3);
      expect(g.edgeCount).toBe(0);
      expect(g.size).toBe(3);

      g.removeVertex(1);
      g.removeVertex(2);
      g.removeVertex(3);
      expect(g.size).toBe(0);
    });

    test("zero weight edge", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 0);
      expect(g.getWeight(1, 2)).toBe(0);
      expect(g.hasEdge(1, 2)).toBe(true);
    });

    test("negative weight edge", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, -3);
      expect(g.getWeight(1, 2)).toBe(-3);
    });
  });
});
