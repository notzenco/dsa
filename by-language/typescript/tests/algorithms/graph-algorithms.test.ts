import { describe, test, expect } from "bun:test";
import { GraphCore } from "../../src/data-structures/graph-core";
import {
  bfs,
  dfs,
  dijkstra,
  bellmanFord,
  shortestPathBfs,
  topologicalSort,
  isBipartite,
  connectedComponents,
  hasCycle,
  primMST,
  kruskalMST,
  floydWarshall,
  tarjanSCC,
  kosarajuSCC,
} from "../../src/algorithms/graph-algorithms";

// =========================================================================
// HELPER: create commonly used test graphs
// =========================================================================

function undirectedPathGraph(): GraphCore<number> {
  // 1 - 2 - 3 - 4 - 5
  const g = new GraphCore<number>();
  g.addEdge(1, 2);
  g.addEdge(2, 3);
  g.addEdge(3, 4);
  g.addEdge(4, 5);
  return g;
}

function undirectedCyclicGraph(): GraphCore<number> {
  // 1 - 2 - 3
  // |       |
  // 4 - 5 --+
  const g = new GraphCore<number>();
  g.addEdge(1, 2);
  g.addEdge(2, 3);
  g.addEdge(1, 4);
  g.addEdge(4, 5);
  g.addEdge(5, 3);
  return g;
}

function directedDAG(): GraphCore<string> {
  // A -> B -> D
  // A -> C -> D
  const g = new GraphCore<string>(true);
  g.addEdge("A", "B");
  g.addEdge("A", "C");
  g.addEdge("B", "D");
  g.addEdge("C", "D");
  return g;
}

function weightedUndirected(): GraphCore<string> {
  //       4
  //   A -------- B
  //   |\  8    / |
  // 2 | \   /  2| 7
  //   |  \ /    |
  //   C --D-----E
  //     3    9
  const g = new GraphCore<string>();
  g.addEdge("A", "B", 4);
  g.addEdge("A", "C", 2);
  g.addEdge("A", "D", 8);
  g.addEdge("B", "D", 2);
  g.addEdge("B", "E", 7);
  g.addEdge("C", "D", 3);
  g.addEdge("D", "E", 9);
  return g;
}

// =========================================================================
// TESTS
// =========================================================================

describe("Graph Algorithms", () => {
  // =======================================================================
  // BFS
  // =======================================================================
  describe("bfs", () => {
    test("traverses all connected vertices", () => {
      const g = undirectedCyclicGraph();
      const result = bfs(g, 1);
      expect(result[0]).toBe(1);
      expect(new Set(result)).toEqual(new Set([1, 2, 3, 4, 5]));
    });

    test("bfs order: neighbors before deeper vertices", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(1, 3);
      g.addEdge(2, 4);
      g.addEdge(3, 5);

      const result = bfs(g, 1);
      expect(result[0]).toBe(1);
      // 2, 3 should come before 4, 5
      const idx2 = result.indexOf(2);
      const idx3 = result.indexOf(3);
      const idx4 = result.indexOf(4);
      const idx5 = result.indexOf(5);
      expect(idx2).toBeLessThan(idx4);
      expect(idx3).toBeLessThan(idx5);
    });

    test("bfs on disconnected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);

      const result = bfs(g, 1);
      expect(new Set(result)).toEqual(new Set([1, 2]));
    });

    test("bfs from nonexistent vertex returns empty", () => {
      const g = new GraphCore<number>();
      expect(bfs(g, 99)).toEqual([]);
    });

    test("bfs single vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(bfs(g, 1)).toEqual([1]);
    });

    test("bfs directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);

      expect(bfs(g, 1)).toEqual([1, 2, 3, 4]);
      expect(bfs(g, 4)).toEqual([4]);
    });
  });

  // =======================================================================
  // DFS
  // =======================================================================
  describe("dfs", () => {
    test("traverses all connected vertices", () => {
      const g = undirectedCyclicGraph();
      const result = dfs(g, 1);
      expect(result[0]).toBe(1);
      expect(new Set(result)).toEqual(new Set([1, 2, 3, 4, 5]));
    });

    test("dfs from nonexistent vertex returns empty", () => {
      const g = new GraphCore<number>();
      expect(dfs(g, 99)).toEqual([]);
    });

    test("dfs single vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(dfs(g, 1)).toEqual([1]);
    });

    test("dfs on disconnected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);

      const result = dfs(g, 1);
      expect(new Set(result)).toEqual(new Set([1, 2]));
    });

    test("dfs directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(1, 4);

      const result = dfs(g, 1);
      expect(result[0]).toBe(1);
      expect(new Set(result)).toEqual(new Set([1, 2, 3, 4]));
    });
  });

  // =======================================================================
  // Dijkstra
  // =======================================================================
  describe("dijkstra", () => {
    test("basic shortest paths", () => {
      const g = new GraphCore<string>();
      g.addEdge("A", "B", 4);
      g.addEdge("A", "C", 2);
      g.addEdge("B", "C", 1);
      g.addEdge("B", "D", 5);
      g.addEdge("C", "D", 8);

      const distances = dijkstra(g, "A");

      expect(distances.get("A")).toBe(0);
      expect(distances.get("B")).toBe(3); // A -> C -> B
      expect(distances.get("C")).toBe(2);
      expect(distances.get("D")).toBe(8); // A -> C -> B -> D
    });

    test("unreachable vertex has Infinity distance", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addVertex(3);

      const distances = dijkstra(g, 1);
      expect(distances.get(3)).toBe(Infinity);
    });

    test("dijkstra from nonexistent vertex returns empty", () => {
      const g = new GraphCore<number>();
      const result = dijkstra(g, 99);
      expect(result.size).toBe(0);
    });

    test("dijkstra directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);
      g.addEdge(2, 3, 2);
      g.addEdge(1, 3, 10);

      const distances = dijkstra(g, 1);
      expect(distances.get(3)).toBe(3); // 1 -> 2 -> 3
    });

    test("dijkstra single vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);

      const distances = dijkstra(g, 1);
      expect(distances.get(1)).toBe(0);
    });
  });

  // =======================================================================
  // Bellman-Ford
  // =======================================================================
  describe("bellmanFord", () => {
    test("basic shortest paths with positive weights", () => {
      const g = new GraphCore<string>(true);
      g.addEdge("A", "B", 4);
      g.addEdge("A", "C", 2);
      g.addEdge("B", "C", -3);
      g.addEdge("C", "D", 2);

      const { distances, hasNegativeCycle } = bellmanFord(g, "A");
      expect(hasNegativeCycle).toBe(false);
      expect(distances.get("B")).toBe(4);
      expect(distances.get("C")).toBe(1); // A -> B -> C = 4 + (-3) = 1
      expect(distances.get("D")).toBe(3); // A -> B -> C -> D = 1 + 2 = 3
    });

    test("negative cycle detection", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);
      g.addEdge(2, 3, -1);
      g.addEdge(3, 1, -1);

      const { hasNegativeCycle } = bellmanFord(g, 1);
      expect(hasNegativeCycle).toBe(true);
    });

    test("no negative cycle", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 4);
      g.addEdge(2, 3, -2);
      g.addEdge(1, 3, 5);

      const { distances, hasNegativeCycle } = bellmanFord(g, 1);
      expect(hasNegativeCycle).toBe(false);
      expect(distances.get(3)).toBe(2); // 1 -> 2 -> 3 = 4 + (-2) = 2
    });

    test("bellmanFord from nonexistent vertex", () => {
      const g = new GraphCore<number>();
      const { distances, hasNegativeCycle } = bellmanFord(g, 99);
      expect(distances.size).toBe(0);
      expect(hasNegativeCycle).toBe(false);
    });

    test("unreachable vertex has Infinity distance", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);
      g.addVertex(3);

      const { distances, hasNegativeCycle } = bellmanFord(g, 1);
      expect(hasNegativeCycle).toBe(false);
      expect(distances.get(3)).toBe(Infinity);
    });
  });

  // =======================================================================
  // Shortest Path BFS
  // =======================================================================
  describe("shortestPathBfs", () => {
    test("finds shortest unweighted path", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(1, 4);
      g.addEdge(4, 3);

      const path = shortestPathBfs(g, 1, 3);
      expect(path).not.toBeNull();
      expect(path!.length).toBe(3); // 1 -> 2 -> 3 or 1 -> 4 -> 3
      expect(path![0]).toBe(1);
      expect(path![path!.length - 1]).toBe(3);
    });

    test("same start and end", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(shortestPathBfs(g, 1, 1)).toEqual([1]);
    });

    test("no path exists", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addVertex(3);

      expect(shortestPathBfs(g, 1, 3)).toBeNull();
    });

    test("nonexistent vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(shortestPathBfs(g, 1, 99)).toBeNull();
      expect(shortestPathBfs(g, 99, 1)).toBeNull();
    });

    test("directed graph path", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      expect(shortestPathBfs(g, 1, 3)).toEqual([1, 2, 3]);
      expect(shortestPathBfs(g, 3, 1)).toBeNull();
    });
  });

  // =======================================================================
  // Topological Sort
  // =======================================================================
  describe("topologicalSort", () => {
    test("basic DAG", () => {
      const g = directedDAG();
      const result = topologicalSort(g);
      expect(result).not.toBeNull();

      // A must come before B and C
      expect(result!.indexOf("A")).toBeLessThan(result!.indexOf("B"));
      expect(result!.indexOf("A")).toBeLessThan(result!.indexOf("C"));
      // B and C must come before D
      expect(result!.indexOf("B")).toBeLessThan(result!.indexOf("D"));
      expect(result!.indexOf("C")).toBeLessThan(result!.indexOf("D"));
    });

    test("cycle returns null", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      expect(topologicalSort(g)).toBeNull();
    });

    test("undirected graph returns null", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);

      expect(topologicalSort(g)).toBeNull();
    });

    test("single vertex", () => {
      const g = new GraphCore<number>(true);
      g.addVertex(1);

      const result = topologicalSort(g);
      expect(result).toEqual([1]);
    });

    test("linear chain", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);

      const result = topologicalSort(g);
      expect(result).toEqual([1, 2, 3, 4]);
    });

    test("multiple valid orderings", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 3);
      g.addEdge(2, 3);

      const result = topologicalSort(g);
      expect(result).not.toBeNull();
      expect(result!.indexOf(1)).toBeLessThan(result!.indexOf(3));
      expect(result!.indexOf(2)).toBeLessThan(result!.indexOf(3));
    });
  });

  // =======================================================================
  // Is Bipartite
  // =======================================================================
  describe("isBipartite", () => {
    test("bipartite path graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);

      expect(isBipartite(g)).toBe(true);
    });

    test("non-bipartite odd cycle", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      expect(isBipartite(g)).toBe(false);
    });

    test("bipartite even cycle", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);
      g.addEdge(4, 1);

      expect(isBipartite(g)).toBe(true);
    });

    test("empty graph is bipartite", () => {
      const g = new GraphCore<number>();
      expect(isBipartite(g)).toBe(true);
    });

    test("single vertex is bipartite", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(isBipartite(g)).toBe(true);
    });

    test("disconnected graph - all components bipartite", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);

      expect(isBipartite(g)).toBe(true);
    });

    test("disconnected graph - one component not bipartite", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);
      g.addEdge(4, 5);
      g.addEdge(5, 3); // odd cycle

      expect(isBipartite(g)).toBe(false);
    });
  });

  // =======================================================================
  // Connected Components
  // =======================================================================
  describe("connectedComponents", () => {
    test("three components", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);
      g.addVertex(5);

      const components = connectedComponents(g);
      expect(components.length).toBe(3);

      const sets = components.map((c) => new Set(c));
      expect(sets).toContainEqual(new Set([1, 2]));
      expect(sets).toContainEqual(new Set([3, 4]));
      expect(sets).toContainEqual(new Set([5]));
    });

    test("fully connected graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);

      const components = connectedComponents(g);
      expect(components.length).toBe(1);
      expect(new Set(components[0])).toEqual(new Set([1, 2, 3]));
    });

    test("empty graph", () => {
      const g = new GraphCore<number>();
      expect(connectedComponents(g)).toEqual([]);
    });

    test("all isolated vertices", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      g.addVertex(2);
      g.addVertex(3);

      const components = connectedComponents(g);
      expect(components.length).toBe(3);
    });
  });

  // =======================================================================
  // Has Cycle
  // =======================================================================
  describe("hasCycle", () => {
    test("undirected acyclic", () => {
      const g = undirectedPathGraph();
      expect(hasCycle(g)).toBe(false);
    });

    test("undirected cyclic", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      expect(hasCycle(g)).toBe(true);
    });

    test("directed acyclic", () => {
      const g = directedDAG();
      expect(hasCycle(g)).toBe(false);
    });

    test("directed cyclic", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      expect(hasCycle(g)).toBe(true);
    });

    test("empty graph has no cycle", () => {
      const g = new GraphCore<number>();
      expect(hasCycle(g)).toBe(false);
    });

    test("single vertex has no cycle", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      expect(hasCycle(g)).toBe(false);
    });

    test("self loop is a cycle", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 1);
      expect(hasCycle(g)).toBe(true);
    });

    test("directed graph with back edge only", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      // No cycle yet
      expect(hasCycle(g)).toBe(false);
      // Add back edge
      g.addEdge(3, 1);
      expect(hasCycle(g)).toBe(true);
    });
  });

  // =======================================================================
  // Prim MST
  // =======================================================================
  describe("primMST", () => {
    test("basic MST", () => {
      const g = weightedUndirected();
      const { edges, totalWeight } = primMST(g);

      // MST should have V-1 = 4 edges
      expect(edges.length).toBe(4);
      // Optimal: A-C(2) + B-D(2) + C-D(3) + B-E(7) = 14
      expect(totalWeight).toBe(14);
    });

    test("empty graph", () => {
      const g = new GraphCore<number>();
      const { edges, totalWeight } = primMST(g);
      expect(edges).toEqual([]);
      expect(totalWeight).toBe(0);
    });

    test("single vertex", () => {
      const g = new GraphCore<number>();
      g.addVertex(1);
      const { edges, totalWeight } = primMST(g);
      expect(edges).toEqual([]);
      expect(totalWeight).toBe(0);
    });

    test("directed graph throws", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);

      expect(() => primMST(g)).toThrow("MST is only defined for undirected graphs");
    });

    test("two vertices", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 5);

      const { edges, totalWeight } = primMST(g);
      expect(edges.length).toBe(1);
      expect(totalWeight).toBe(5);
    });
  });

  // =======================================================================
  // Kruskal MST
  // =======================================================================
  describe("kruskalMST", () => {
    test("basic MST", () => {
      const g = weightedUndirected();
      const { edges, totalWeight } = kruskalMST(g);

      expect(edges.length).toBe(4);
      expect(totalWeight).toBe(14);
    });

    test("empty graph", () => {
      const g = new GraphCore<number>();
      const { edges, totalWeight } = kruskalMST(g);
      expect(edges).toEqual([]);
      expect(totalWeight).toBe(0);
    });

    test("directed graph throws", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);

      expect(() => kruskalMST(g)).toThrow("MST is only defined for undirected graphs");
    });

    test("prim and kruskal produce same total weight", () => {
      const g = weightedUndirected();
      const primResult = primMST(g);
      const kruskalResult = kruskalMST(g);

      expect(primResult.totalWeight).toBe(kruskalResult.totalWeight);
    });

    test("triangle graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 1);
      g.addEdge(2, 3, 2);
      g.addEdge(1, 3, 3);

      const { edges, totalWeight } = kruskalMST(g);
      expect(edges.length).toBe(2);
      expect(totalWeight).toBe(3); // 1-2(1) + 2-3(2)
    });
  });

  // =======================================================================
  // Floyd-Warshall
  // =======================================================================
  describe("floydWarshall", () => {
    test("basic directed graph", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 3);
      g.addEdge(2, 3, 1);
      g.addEdge(1, 3, 7);

      const dist = floydWarshall(g);

      expect(dist.get(1)!.get(2)).toBe(3);
      expect(dist.get(1)!.get(3)).toBe(4); // 1 -> 2 -> 3 = 4, not direct 7
      expect(dist.get(2)!.get(3)).toBe(1);
    });

    test("self distances are zero", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2, 5);

      const dist = floydWarshall(g);
      expect(dist.get(1)!.get(1)).toBe(0);
      expect(dist.get(2)!.get(2)).toBe(0);
    });

    test("unreachable vertex has Infinity", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2, 1);
      g.addVertex(3);

      const dist = floydWarshall(g);
      expect(dist.get(1)!.get(3)).toBe(Infinity);
    });

    test("undirected graph", () => {
      const g = new GraphCore<string>();
      g.addEdge("A", "B", 1);
      g.addEdge("B", "C", 2);
      g.addEdge("A", "C", 10);

      const dist = floydWarshall(g);
      expect(dist.get("A")!.get("C")).toBe(3); // A -> B -> C
    });

    test("empty graph", () => {
      const g = new GraphCore<number>();
      const dist = floydWarshall(g);
      expect(dist.size).toBe(0);
    });
  });

  // =======================================================================
  // Tarjan SCC
  // =======================================================================
  describe("tarjanSCC", () => {
    test("three SCCs", () => {
      // 1 -> 2 -> 3
      // ^    |    |
      // |    v    v
      // 4 <- 5   6 -> 7
      //           ^    |
      //           +----+
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(2, 5);
      g.addEdge(3, 6);
      g.addEdge(5, 4);
      g.addEdge(4, 1);
      g.addEdge(6, 7);
      g.addEdge(7, 6);

      const sccs = tarjanSCC(g);
      expect(sccs.length).toBe(3);

      const sccSets = sccs.map((scc) => new Set(scc));
      expect(sccSets).toContainEqual(new Set([1, 2, 4, 5]));
      expect(sccSets).toContainEqual(new Set([3]));
      expect(sccSets).toContainEqual(new Set([6, 7]));
    });

    test("fully strongly connected", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      const sccs = tarjanSCC(g);
      expect(sccs.length).toBe(1);
      expect(new Set(sccs[0])).toEqual(new Set([1, 2, 3]));
    });

    test("all isolated vertices", () => {
      const g = new GraphCore<number>(true);
      g.addVertex(1);
      g.addVertex(2);
      g.addEdge(3, 4);

      const sccs = tarjanSCC(g);
      // {1}, {2}, {3}, {4}
      expect(sccs.length).toBe(4);
    });

    test("undirected graph returns connected components", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);

      const sccs = tarjanSCC(g);
      expect(sccs.length).toBe(2);
    });

    test("DAG: each vertex is its own SCC", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 4);

      const sccs = tarjanSCC(g);
      expect(sccs.length).toBe(4);
      for (const scc of sccs) {
        expect(scc.length).toBe(1);
      }
    });
  });

  // =======================================================================
  // Kosaraju SCC
  // =======================================================================
  describe("kosarajuSCC", () => {
    test("three SCCs", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(2, 5);
      g.addEdge(3, 6);
      g.addEdge(5, 4);
      g.addEdge(4, 1);
      g.addEdge(6, 7);
      g.addEdge(7, 6);

      const sccs = kosarajuSCC(g);
      expect(sccs.length).toBe(3);

      const sccSets = sccs.map((scc) => new Set(scc));
      expect(sccSets).toContainEqual(new Set([1, 2, 4, 5]));
      expect(sccSets).toContainEqual(new Set([3]));
      expect(sccSets).toContainEqual(new Set([6, 7]));
    });

    test("fully strongly connected", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(3, 1);

      const sccs = kosarajuSCC(g);
      expect(sccs.length).toBe(1);
      expect(new Set(sccs[0])).toEqual(new Set([1, 2, 3]));
    });

    test("tarjan and kosaraju produce same SCCs", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(2, 5);
      g.addEdge(3, 6);
      g.addEdge(5, 4);
      g.addEdge(4, 1);
      g.addEdge(6, 7);
      g.addEdge(7, 6);

      const tarjan = tarjanSCC(g);
      const kosaraju = kosarajuSCC(g);

      const tarjanFrozen = new Set(tarjan.map((scc) => JSON.stringify([...new Set(scc)].sort())));
      const kosarajuFrozen = new Set(
        kosaraju.map((scc) => JSON.stringify([...new Set(scc)].sort())),
      );

      expect(tarjanFrozen).toEqual(kosarajuFrozen);
    });

    test("undirected graph returns connected components", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(3, 4);

      const sccs = kosarajuSCC(g);
      expect(sccs.length).toBe(2);
    });
  });

  // =======================================================================
  // Integration / Complex Tests
  // =======================================================================
  describe("integration tests", () => {
    test("BFS shortest path consistency with Dijkstra on unweighted graph", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(1, 4);
      g.addEdge(4, 3);

      const bfsPath = shortestPathBfs(g, 1, 3);
      const dijkDist = dijkstra(g, 1);

      // Both should give distance 2 from 1 to 3
      expect(bfsPath!.length - 1).toBe(dijkDist.get(3));
    });

    test("complete graph properties", () => {
      const g = new GraphCore<number>();
      const n = 5;
      for (let i = 0; i < n; i++) {
        for (let j = i + 1; j < n; j++) {
          g.addEdge(i, j);
        }
      }

      // All vertices reachable from any vertex
      expect(bfs(g, 0).length).toBe(n);

      // Connected
      expect(connectedComponents(g).length).toBe(1);

      // Has cycle (any triangle)
      expect(hasCycle(g)).toBe(true);

      // Bipartite only for n <= 2
      expect(isBipartite(g)).toBe(false);
    });

    test("star graph is bipartite and acyclic", () => {
      const g = new GraphCore<number>();
      for (let i = 1; i <= 5; i++) {
        g.addEdge(0, i);
      }

      expect(isBipartite(g)).toBe(true);
      expect(hasCycle(g)).toBe(false);
      expect(connectedComponents(g).length).toBe(1);
    });

    test("complex weighted graph - all shortest path algorithms agree", () => {
      const g = new GraphCore<number>(true);
      g.addEdge(0, 1, 4);
      g.addEdge(0, 2, 1);
      g.addEdge(2, 1, 2);
      g.addEdge(1, 3, 1);
      g.addEdge(2, 3, 5);

      const dijkDist = dijkstra(g, 0);
      const bfResult = bellmanFord(g, 0);
      const fwDist = floydWarshall(g);

      // All should agree: 0->1=3, 0->2=1, 0->3=4
      expect(dijkDist.get(1)).toBe(3);
      expect(dijkDist.get(2)).toBe(1);
      expect(dijkDist.get(3)).toBe(4);

      expect(bfResult.distances.get(1)).toBe(3);
      expect(bfResult.distances.get(2)).toBe(1);
      expect(bfResult.distances.get(3)).toBe(4);

      expect(fwDist.get(0)!.get(1)).toBe(3);
      expect(fwDist.get(0)!.get(2)).toBe(1);
      expect(fwDist.get(0)!.get(3)).toBe(4);
    });

    test("disconnected graph components", () => {
      const g = new GraphCore<number>();
      g.addEdge(1, 2);
      g.addEdge(2, 3);
      g.addEdge(10, 11);
      g.addVertex(20);

      const components = connectedComponents(g);
      expect(components.length).toBe(3);

      // BFS from 1 only reaches component 1
      expect(new Set(bfs(g, 1))).toEqual(new Set([1, 2, 3]));
    });
  });
});
