package datastructures

import (
	"math"
	"sort"
	"testing"
)

// --- Adjacency Matrix Tests ---

func TestAdjacencyMatrix(t *testing.T) {
	g := NewAdjacencyMatrix(5, false, false)

	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 4, 1)

	if !g.HasEdge(0, 1) {
		t.Error("should have edge 0-1")
	}
	if !g.HasEdge(1, 0) {
		t.Error("undirected: should have edge 1-0")
	}

	neighbors := g.Neighbors(0)
	sort.Ints(neighbors)
	if len(neighbors) != 2 || neighbors[0] != 1 || neighbors[1] != 2 {
		t.Errorf("expected neighbors [1,2], got %v", neighbors)
	}

	if g.EdgeCount() != 4 {
		t.Errorf("expected 4 edges, got %d", g.EdgeCount())
	}
}

func TestAdjacencyMatrixBFS(t *testing.T) {
	g := NewAdjacencyMatrix(4, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)

	bfs := g.BFS(0)
	if len(bfs) != 4 {
		t.Errorf("expected 4 vertices in BFS, got %d", len(bfs))
	}
	if bfs[0] != 0 {
		t.Errorf("BFS should start from 0, got %d", bfs[0])
	}
}

func TestAdjacencyMatrixDFS(t *testing.T) {
	g := NewAdjacencyMatrix(4, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)

	dfs := g.DFS(0)
	if len(dfs) != 4 {
		t.Errorf("expected 4 vertices in DFS, got %d", len(dfs))
	}
}

func TestAdjacencyMatrixFloydWarshall(t *testing.T) {
	g := NewAdjacencyMatrix(4, true, true)
	g.AddEdge(0, 1, 5)
	g.AddEdge(0, 3, 10)
	g.AddEdge(1, 2, 3)
	g.AddEdge(2, 3, 1)

	dist, ok := g.FloydWarshall()
	if !ok {
		t.Error("should not have negative cycle")
	}
	if dist[0][3] != 9 {
		t.Errorf("expected dist 0->3 = 9, got %f", dist[0][3])
	}
}

func TestAdjacencyMatrixTransitiveClosure(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	closure := g.TransitiveClosure()
	if !closure[0][2] {
		t.Error("0 should reach 2 transitively")
	}
}

// --- Graph Core Tests ---

func TestGraphCore(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)

	if g.NumVertices() != 4 {
		t.Errorf("expected 4 vertices, got %d", g.NumVertices())
	}
	if g.EdgeCount() != 3 {
		t.Errorf("expected 3 edges, got %d", g.EdgeCount())
	}

	if !g.HasEdge(0, 1) {
		t.Error("should have edge 0-1")
	}
	if !g.HasEdge(1, 0) {
		t.Error("undirected: should have edge 1-0")
	}
}

func TestGraphCoreDirected(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)

	if !g.HasEdge(0, 1) {
		t.Error("should have edge 0->1")
	}
	if g.HasEdge(1, 0) {
		t.Error("directed: should not have edge 1->0")
	}
}

func TestGraphCoreRemove(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)

	g.RemoveEdge(0, 1)
	if g.HasEdge(0, 1) {
		t.Error("should not have edge after remove")
	}

	g.RemoveVertex(2)
	if g.Contains(2) {
		t.Error("should not contain vertex 2")
	}
}

// --- Graph Algorithms Tests ---

func TestGraphBFS(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 4, 1)

	bfs := g.BFS(0)
	if len(bfs) != 5 {
		t.Errorf("expected 5 vertices, got %d", len(bfs))
	}
	if bfs[0] != 0 {
		t.Errorf("BFS should start from 0")
	}
}

func TestGraphDFS(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)

	dfs := g.DFS(0)
	if len(dfs) != 4 {
		t.Errorf("expected 4 vertices, got %d", len(dfs))
	}
}

func TestGraphShortestPathBFS(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(0, 3, 1)
	g.AddEdge(3, 2, 1)

	path := g.ShortestPathBFS(0, 2)
	if len(path) != 3 {
		t.Errorf("expected path length 3, got %d: %v", len(path), path)
	}
}

func TestGraphDijkstra(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 4)
	g.AddEdge(0, 2, 1)
	g.AddEdge(2, 1, 2)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 3, 5)

	path, dist := g.DijkstraPath(0, 3)
	if dist != 4 {
		t.Errorf("expected distance 4, got %f", dist)
	}
	if len(path) != 4 || path[0] != 0 || path[len(path)-1] != 3 {
		t.Errorf("unexpected path: %v", path)
	}
}

func TestGraphBellmanFord(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 4)
	g.AddEdge(0, 2, 1)
	g.AddEdge(2, 1, 2)
	g.AddEdge(1, 3, 1)

	dist, _, ok := g.BellmanFord(0)
	if !ok {
		t.Error("should not detect negative cycle")
	}
	if dist[3] != 4 {
		t.Errorf("expected dist[3]=4, got %f", dist[3])
	}
}

func TestGraphConnected(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if !g.IsConnected() {
		t.Error("should be connected")
	}

	g.AddVertex(3)
	if g.IsConnected() {
		t.Error("should not be connected with isolated vertex")
	}
}

func TestGraphConnectedComponents(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(2, 3, 1)
	g.AddVertex(4)

	components := g.ConnectedComponents()
	if len(components) != 3 {
		t.Errorf("expected 3 components, got %d", len(components))
	}
}

func TestGraphHasCycle(t *testing.T) {
	// Undirected with cycle
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	if !g.HasCycle() {
		t.Error("should detect cycle")
	}

	// Directed without cycle
	g2 := NewGraphCore(true)
	g2.AddEdge(0, 1, 1)
	g2.AddEdge(1, 2, 1)

	if g2.HasCycle() {
		t.Error("should not detect cycle in DAG")
	}

	// Directed with cycle
	g3 := NewGraphCore(true)
	g3.AddEdge(0, 1, 1)
	g3.AddEdge(1, 2, 1)
	g3.AddEdge(2, 0, 1)

	if !g3.HasCycle() {
		t.Error("should detect cycle in directed graph")
	}
}

func TestGraphTopologicalSort(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(5, 2, 1)
	g.AddEdge(5, 0, 1)
	g.AddEdge(4, 0, 1)
	g.AddEdge(4, 1, 1)
	g.AddEdge(2, 3, 1)
	g.AddEdge(3, 1, 1)

	order, ok := g.TopologicalSort()
	if !ok {
		t.Error("should succeed for DAG")
	}
	if len(order) != 6 {
		t.Errorf("expected 6 vertices, got %d", len(order))
	}

	// Verify topological property
	pos := make(map[int]int)
	for i, v := range order {
		pos[v] = i
	}
	for u := range g.adj {
		for v := range g.adj[u] {
			if pos[u] >= pos[v] {
				t.Errorf("topological violation: %d before %d", u, v)
			}
		}
	}
}

func TestGraphIsBipartite(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 3, 1)
	g.AddEdge(3, 0, 1)

	if !g.IsBipartite() {
		t.Error("even cycle should be bipartite")
	}

	g2 := NewGraphCore(false)
	g2.AddEdge(0, 1, 1)
	g2.AddEdge(1, 2, 1)
	g2.AddEdge(2, 0, 1)

	if g2.IsBipartite() {
		t.Error("odd cycle should not be bipartite")
	}
}

func TestGraphPrimMST(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 4)
	g.AddEdge(0, 2, 3)
	g.AddEdge(1, 2, 1)
	g.AddEdge(1, 3, 2)
	g.AddEdge(2, 3, 4)

	edges, weight := g.PrimMST()
	if len(edges) != 3 {
		t.Errorf("MST should have 3 edges, got %d", len(edges))
	}
	if weight != 6 {
		t.Errorf("MST weight should be 6, got %f", weight)
	}
}

func TestGraphKruskalMST(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 4)
	g.AddEdge(0, 2, 3)
	g.AddEdge(1, 2, 1)
	g.AddEdge(1, 3, 2)
	g.AddEdge(2, 3, 4)

	edges, weight := g.KruskalMST()
	if len(edges) != 3 {
		t.Errorf("MST should have 3 edges, got %d", len(edges))
	}
	if weight != 6 {
		t.Errorf("MST weight should be 6, got %f", weight)
	}
}

func TestGraphFloydWarshall(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 5)
	g.AddEdge(0, 3, 10)
	g.AddEdge(1, 2, 3)
	g.AddEdge(2, 3, 1)

	dist, ok := g.FloydWarshall()
	if !ok {
		t.Error("should not have negative cycle")
	}
	if dist[0][3] != 9 {
		t.Errorf("expected dist 0->3 = 9, got %f", dist[0][3])
	}
	if !math.IsInf(dist[3][0], 1) {
		t.Errorf("expected dist 3->0 = inf, got %f", dist[3][0])
	}
}

func TestGraphTarjanSCC(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)
	g.AddEdge(3, 4, 1)
	g.AddEdge(4, 5, 1)
	g.AddEdge(5, 3, 1)
	g.AddEdge(2, 3, 1)

	sccs := g.TarjanSCC()
	if len(sccs) != 2 {
		t.Errorf("expected 2 SCCs, got %d", len(sccs))
	}
}

func TestGraphKosarajuSCC(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)
	g.AddEdge(3, 4, 1)
	g.AddEdge(4, 5, 1)
	g.AddEdge(5, 3, 1)
	g.AddEdge(2, 3, 1)

	sccs := g.KosarajuSCC()
	if len(sccs) != 2 {
		t.Errorf("expected 2 SCCs, got %d", len(sccs))
	}
}
