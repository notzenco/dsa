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

// --- Adjacency Matrix Extended Tests ---

func TestAdjacencyMatrixDirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)

	if !g.HasEdge(0, 1) {
		t.Error("should have edge 0->1")
	}
	if g.HasEdge(1, 0) {
		t.Error("directed: should not have edge 1->0")
	}
	if g.OutDegree(0) != 1 {
		t.Errorf("expected out-degree 1, got %d", g.OutDegree(0))
	}
	if g.InDegree(1) != 1 {
		t.Errorf("expected in-degree 1, got %d", g.InDegree(1))
	}
}

func TestAdjacencyMatrixRemoveEdge(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if !g.RemoveEdge(0, 1) {
		t.Error("remove should succeed")
	}
	if g.HasEdge(0, 1) {
		t.Error("should not have edge after remove")
	}
	if g.HasEdge(1, 0) {
		t.Error("undirected: reverse should be removed too")
	}
}

func TestAdjacencyMatrixHasCycleDirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	if !g.HasCycle() {
		t.Error("should detect cycle")
	}
}

func TestAdjacencyMatrixNoCycleDirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if g.HasCycle() {
		t.Error("should not detect cycle in DAG")
	}
}

func TestAdjacencyMatrixHasCycleUndirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	if !g.HasCycle() {
		t.Error("should detect cycle in undirected graph")
	}
}

func TestAdjacencyMatrixNoCycleUndirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if g.HasCycle() {
		t.Error("should not detect cycle in tree")
	}
}

func TestAdjacencyMatrixIsComplete(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 2, 1)

	if !g.IsComplete() {
		t.Error("should be complete")
	}

	g2 := NewAdjacencyMatrix(3, false, false)
	g2.AddEdge(0, 1, 1)
	if g2.IsComplete() {
		t.Error("should not be complete")
	}
}

func TestAdjacencyMatrixComplement(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)

	comp := g.Complement()
	if comp.HasEdge(0, 1) {
		t.Error("complement should not have original edge")
	}
	if !comp.HasEdge(0, 2) {
		t.Error("complement should have missing edges")
	}
	if !comp.HasEdge(1, 2) {
		t.Error("complement should have missing edges")
	}
}

func TestAdjacencyMatrixDensity(t *testing.T) {
	g := NewAdjacencyMatrix(4, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(0, 3, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 3, 1)

	d := g.Density()
	if d < 0.99 || d > 1.01 {
		t.Errorf("complete graph density should be 1.0, got %f", d)
	}
}

func TestAdjacencyMatrixDijkstra(t *testing.T) {
	g := NewAdjacencyMatrix(4, true, true)
	g.AddEdge(0, 1, 4)
	g.AddEdge(0, 2, 1)
	g.AddEdge(2, 1, 2)
	g.AddEdge(1, 3, 1)

	path, dist := g.ShortestPathDijkstra(0, 3)
	if dist != 4 {
		t.Errorf("expected distance 4, got %d", dist)
	}
	if len(path) != 4 || path[0] != 0 || path[len(path)-1] != 3 {
		t.Errorf("unexpected path: %v", path)
	}
}

func TestAdjacencyMatrixDijkstraUnreachable(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, true)
	g.AddEdge(0, 1, 5)

	path, _ := g.ShortestPathDijkstra(0, 2)
	if path != nil {
		t.Error("should return nil for unreachable target")
	}
}

func TestAdjacencyMatrixDijkstraLargeWeight(t *testing.T) {
	g := NewAdjacencyMatrix(2, true, true)
	g.AddEdge(0, 1, math.MaxInt32)
	path, distance := g.ShortestPathDijkstra(0, 1)
	if len(path) != 2 || distance != math.MaxInt32 {
		t.Fatalf("path = %v, distance = %d", path, distance)
	}
}

func TestAdjacencyMatrixFloydWarshallNegativeSelfLoop(t *testing.T) {
	g := NewAdjacencyMatrix(1, true, true)
	g.AddEdge(0, 0, -1)
	_, ok := g.FloydWarshall()
	if ok {
		t.Fatal("negative self-loop must be a negative cycle")
	}
}

func TestAdjacencyMatrixTopologicalSort(t *testing.T) {
	g := NewAdjacencyMatrix(4, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 3, 1)

	order, ok := g.TopologicalSort()
	if !ok {
		t.Error("should succeed for DAG")
	}
	if len(order) != 4 {
		t.Errorf("expected 4 vertices, got %d", len(order))
	}

	// Verify topological property
	pos := make(map[int]int)
	for i, v := range order {
		pos[v] = i
	}
	if pos[0] >= pos[1] || pos[0] >= pos[2] {
		t.Error("0 should come before 1 and 2")
	}
	if pos[1] >= pos[3] || pos[2] >= pos[3] {
		t.Error("1 and 2 should come before 3")
	}
}

func TestAdjacencyMatrixTopologicalSortUndirected(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	_, ok := g.TopologicalSort()
	if ok {
		t.Error("topological sort should fail for undirected graph")
	}
}

func TestAdjacencyMatrixTopologicalSortCycle(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	_, ok := g.TopologicalSort()
	if ok {
		t.Error("topological sort should fail for graph with cycle")
	}
}

func TestAdjacencyMatrixConnectedComponents(t *testing.T) {
	g := NewAdjacencyMatrix(5, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(2, 3, 1)
	// vertex 4 is isolated

	components := g.ConnectedComponents()
	if len(components) != 3 {
		t.Errorf("expected 3 components, got %d", len(components))
	}
}

func TestAdjacencyMatrixIsBipartite(t *testing.T) {
	g := NewAdjacencyMatrix(4, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 3, 1)
	g.AddEdge(3, 0, 1)

	if !g.IsBipartite() {
		t.Error("even cycle should be bipartite")
	}

	g2 := NewAdjacencyMatrix(3, false, false)
	g2.AddEdge(0, 1, 1)
	g2.AddEdge(1, 2, 1)
	g2.AddEdge(2, 0, 1)

	if g2.IsBipartite() {
		t.Error("triangle should not be bipartite")
	}
}

func TestAdjacencyMatrixIsConnected(t *testing.T) {
	g := NewAdjacencyMatrix(3, false, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if !g.IsConnected() {
		t.Error("should be connected")
	}

	g2 := NewAdjacencyMatrix(3, false, false)
	g2.AddEdge(0, 1, 1)
	if g2.IsConnected() {
		t.Error("should not be connected with isolated vertex")
	}
}

func TestAdjacencyMatrixTranspose(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	trans := g.Transpose()
	if !trans.HasEdge(1, 0) {
		t.Error("transpose should have edge 1->0")
	}
	if !trans.HasEdge(2, 1) {
		t.Error("transpose should have edge 2->1")
	}
	if trans.HasEdge(0, 1) {
		t.Error("transpose should not have original edge 0->1")
	}
}

func TestAdjacencyMatrixGetWeight(t *testing.T) {
	g := NewAdjacencyMatrix(3, true, true)
	g.AddEdge(0, 1, 5)

	w := g.GetWeight(0, 1)
	if w != 5 {
		t.Errorf("expected weight 5, got %d", w)
	}

	w = g.GetWeight(1, 2)
	if w != 0 {
		t.Errorf("expected 0 for non-existent edge, got %d", w)
	}
}

func TestAdjacencyMatrixMatrix(t *testing.T) {
	g := NewAdjacencyMatrix(2, false, false)
	g.AddEdge(0, 1, 1)

	m := g.Matrix()
	if m[0][1] != 1 || m[1][0] != 1 {
		t.Error("matrix should reflect edges")
	}
}

func TestAdjacencyMatrixDensityEmpty(t *testing.T) {
	g := NewAdjacencyMatrix(1, false, false)
	d := g.Density()
	if d != 0 {
		t.Errorf("single vertex density should be 0, got %f", d)
	}
}

// --- Graph Core Extended Algorithm Tests ---

func TestGraphArticulationPoints(t *testing.T) {
	g := NewGraphCore(false)
	// Bridge graph: 0-1-2 with 1 as articulation point
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	aps := g.ArticulationPoints()
	if len(aps) != 1 || aps[0] != 1 {
		t.Errorf("expected [1], got %v", aps)
	}
}

func TestGraphNegativeVertexAndMissingDijkstraTarget(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(-1, 1, 1)
	g.AddEdge(1, 2, 1)
	path := g.ShortestPathBFS(-1, 2)
	if len(path) != 3 || path[0] != -1 || path[1] != 1 || path[2] != 2 {
		t.Fatalf("BFS path = %v", path)
	}
	aps := g.ArticulationPoints()
	if len(aps) != 1 || aps[0] != 1 {
		t.Fatalf("articulation points = %v", aps)
	}
	missing, _ := g.DijkstraPath(-1, 99)
	if missing != nil {
		t.Fatalf("missing target path = %v", missing)
	}
}

func TestGraphArticulationPointsComplex(t *testing.T) {
	g := NewGraphCore(false)
	// Graph: 0-1, 1-2, 2-0, 1-3, 3-4, 4-5, 5-3
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(3, 4, 1)
	g.AddEdge(4, 5, 1)
	g.AddEdge(5, 3, 1)

	aps := g.ArticulationPoints()
	// Vertex 1 connects cycle 0-1-2 to cycle 3-4-5
	// Vertex 3 connects the bridge edge 1-3 to cycle 3-4-5
	// Wait: 3-4-5-3 is a cycle, but 3 is still AP because removing 3 disconnects 4,5 from 0,1,2
	// Actually 1 is AP and 3 is also AP (removing 3 breaks the only path from 1 to 4,5)
	sort.Ints(aps)
	if len(aps) != 2 {
		t.Errorf("expected 2 articulation points, got %d: %v", len(aps), aps)
	}
}

func TestGraphArticulationPointsNone(t *testing.T) {
	g := NewGraphCore(false)
	// Complete graph K3 has no articulation points
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	aps := g.ArticulationPoints()
	if len(aps) != 0 {
		t.Errorf("expected 0 articulation points, got %v", aps)
	}
}

func TestGraphArticulationPointsDirected(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	aps := g.ArticulationPoints()
	if aps != nil {
		t.Error("should return nil for directed graph")
	}
}

func TestGraphBridges(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)
	g.AddEdge(1, 3, 1)

	bridges := g.Bridges()
	if len(bridges) != 1 {
		t.Errorf("expected 1 bridge, got %d: %v", len(bridges), bridges)
	}
	b := bridges[0]
	if !((b[0] == 1 && b[1] == 3) || (b[0] == 3 && b[1] == 1)) {
		t.Errorf("expected bridge (1,3), got %v", b)
	}
}

func TestGraphBridgesNone(t *testing.T) {
	g := NewGraphCore(false)
	// Complete graph has no bridges
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 0, 1)

	bridges := g.Bridges()
	if len(bridges) != 0 {
		t.Errorf("expected 0 bridges, got %v", bridges)
	}
}

func TestGraphBridgesDirected(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	bridges := g.Bridges()
	if bridges != nil {
		t.Error("should return nil for directed graph")
	}
}

func TestGraphBridgesChain(t *testing.T) {
	g := NewGraphCore(false)
	// Chain: 0-1-2-3 => all edges are bridges
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(2, 3, 1)

	bridges := g.Bridges()
	if len(bridges) != 3 {
		t.Errorf("expected 3 bridges in chain, got %d", len(bridges))
	}
}

func TestGraphShortestPathUnweighted(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(0, 3, 1)
	g.AddEdge(3, 2, 1)

	dist := g.ShortestPathUnweighted(0)
	if dist[0] != 0 {
		t.Errorf("dist to self should be 0, got %d", dist[0])
	}
	if dist[1] != 1 {
		t.Errorf("dist to 1 should be 1, got %d", dist[1])
	}
	if dist[2] != 2 {
		t.Errorf("dist to 2 should be 2, got %d", dist[2])
	}
}

func TestGraphInDegree(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(2, 1, 1)
	g.AddEdge(3, 1, 1)

	if g.InDegree(1) != 3 {
		t.Errorf("expected in-degree 3, got %d", g.InDegree(1))
	}
	if g.InDegree(0) != 0 {
		t.Errorf("expected in-degree 0, got %d", g.InDegree(0))
	}
}

func TestGraphOutDegree(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(0, 3, 1)

	if g.OutDegree(0) != 3 {
		t.Errorf("expected out-degree 3, got %d", g.OutDegree(0))
	}
	if g.OutDegree(1) != 0 {
		t.Errorf("expected out-degree 0, got %d", g.OutDegree(1))
	}
}

func TestGraphDegreeUndirected(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)

	if g.Degree(0) != 2 {
		t.Errorf("expected degree 2, got %d", g.Degree(0))
	}
}

func TestGraphIsDAG(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)

	if !g.IsDAG() {
		t.Error("should be a DAG")
	}

	g.AddEdge(2, 0, 1)
	if g.IsDAG() {
		t.Error("should not be a DAG with cycle")
	}
}

func TestGraphIsDAGUndirected(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)

	if g.IsDAG() {
		t.Error("undirected graph should not be a DAG")
	}
}

func TestGraphTranspose(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 2)
	g.AddEdge(1, 2, 3)

	trans := g.Transpose()
	if !trans.HasEdge(1, 0) {
		t.Error("transpose should have edge 1->0")
	}
	if !trans.HasEdge(2, 1) {
		t.Error("transpose should have edge 2->1")
	}
	if trans.HasEdge(0, 1) {
		t.Error("transpose should not have original edge 0->1")
	}
}

func TestGraphTransposeUndirected(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)

	trans := g.Transpose()
	if trans != nil {
		t.Error("transpose of undirected graph should be nil")
	}
}

func TestGraphAllPaths(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 3, 1)
	g.AddEdge(1, 2, 1)

	paths := g.AllPaths(0, 3)
	// 0->1->3, 0->2->3, 0->1->2->3
	if len(paths) != 3 {
		t.Errorf("expected 3 paths, got %d: %v", len(paths), paths)
	}
	for _, path := range paths {
		if path[0] != 0 || path[len(path)-1] != 3 {
			t.Errorf("path should start at 0 and end at 3: %v", path)
		}
	}
}

func TestGraphAllPathsNoPath(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddVertex(2)

	paths := g.AllPaths(0, 2)
	if len(paths) != 0 {
		t.Errorf("expected 0 paths, got %d", len(paths))
	}
}

func TestGraphCountPaths(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)
	g.AddEdge(2, 3, 1)

	count := g.CountPaths(0, 3)
	if count != 2 {
		t.Errorf("expected 2 paths, got %d", count)
	}
}

func TestGraphDFSRecursive(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)
	g.AddEdge(0, 2, 1)
	g.AddEdge(1, 3, 1)

	dfs := g.DFSRecursive(0)
	if len(dfs) != 4 {
		t.Errorf("expected 4 vertices, got %d", len(dfs))
	}
	if dfs[0] != 0 {
		t.Errorf("DFS should start from 0")
	}
}

func TestGraphDFSRecursiveNonExistent(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)

	dfs := g.DFSRecursive(99)
	if dfs != nil {
		t.Error("DFS from non-existent vertex should return nil")
	}
}

func TestGraphBFSNonExistent(t *testing.T) {
	g := NewGraphCore(false)
	g.AddEdge(0, 1, 1)

	bfs := g.BFS(99)
	if bfs != nil {
		t.Error("BFS from non-existent vertex should return nil")
	}
}

func TestGraphAllPathsSameVertex(t *testing.T) {
	g := NewGraphCore(true)
	g.AddVertex(0)

	paths := g.AllPaths(0, 0)
	if len(paths) != 1 {
		t.Errorf("expected 1 path (self), got %d", len(paths))
	}
}

func TestGraphShortestPathUnweightedDirected(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(1, 2, 1)
	g.AddEdge(0, 2, 1)

	dist := g.ShortestPathUnweighted(0)
	if dist[2] != 1 {
		t.Errorf("expected direct distance 1, got %d", dist[2])
	}
}

func TestGraphDegreeDirected(t *testing.T) {
	g := NewGraphCore(true)
	g.AddEdge(0, 1, 1)
	g.AddEdge(2, 0, 1)

	// Degree for directed = in + out
	deg := g.Degree(0)
	if deg != 2 {
		t.Errorf("expected degree 2 (in=1, out=1), got %d", deg)
	}
}

func TestGraphCoreAccessors(t *testing.T) {
	undirected := NewGraphCore(false)
	if undirected.IsDirected() {
		t.Fatal("undirected graph reports directed")
	}
	if _, ok := undirected.GetWeight(0, 1); ok {
		t.Fatal("missing vertex should have no weight")
	}
	if undirected.NeighborsWeighted(0) != nil {
		t.Fatal("missing vertex should have nil weighted neighbors")
	}

	undirected.AddVertex(4)
	if got := undirected.NeighborsWeighted(4); len(got) != 0 {
		t.Fatalf("isolated vertex neighbors = %+v", got)
	}
	undirected.AddEdge(0, 1, 2.5)
	undirected.AddEdge(0, 2, 4)

	w, ok := undirected.GetWeight(0, 1)
	if !ok || w != 2.5 {
		t.Fatalf("weight 0-1 = %v, %v", w, ok)
	}
	w, ok = undirected.GetWeight(1, 0)
	if !ok || w != 2.5 {
		t.Fatalf("undirected reverse weight = %v, %v", w, ok)
	}
	if _, ok = undirected.GetWeight(1, 2); ok {
		t.Fatal("unexpected edge 1-2")
	}

	nbrs := undirected.NeighborsWeighted(0)
	gotW := map[int]float64{}
	for _, n := range nbrs {
		gotW[n.Vertex] = n.Weight
	}
	if len(gotW) != 2 || gotW[1] != 2.5 || gotW[2] != 4 {
		t.Fatalf("weighted neighbors = %v", gotW)
	}

	verts, matrix := undirected.ToAdjacencyMatrix()
	if len(verts) != 4 || len(matrix) != 4 {
		t.Fatalf("matrix shape vertices=%v rows=%d", verts, len(matrix))
	}
	index := make(map[int]int, len(verts))
	for i, v := range verts {
		index[v] = i
		if len(matrix[i]) != len(verts) {
			t.Fatalf("row %d len = %d", i, len(matrix[i]))
		}
		if matrix[i][i] != 0 {
			t.Fatalf("diagonal %d = %v", i, matrix[i][i])
		}
	}
	if matrix[index[0]][index[1]] != 2.5 || matrix[index[1]][index[0]] != 2.5 {
		t.Fatal("matrix missing undirected edge 0-1")
	}
	if !math.IsInf(matrix[index[1]][index[2]], 1) {
		t.Fatal("missing edge should be +Inf")
	}
	if !math.IsInf(matrix[index[0]][index[4]], 1) {
		t.Fatal("edge to isolated vertex should be +Inf")
	}

	emptyVerts, emptyMatrix := NewGraphCore(true).ToAdjacencyMatrix()
	if len(emptyVerts) != 0 || len(emptyMatrix) != 0 {
		t.Fatalf("empty matrix = %v, %v", emptyVerts, emptyMatrix)
	}

	directed := GraphCoreFromEdgeList([][3]int{{1, 2, 5}, {2, 3, -1}}, true)
	if !directed.IsDirected() {
		t.Fatal("edge list should build a directed graph")
	}
	if !directed.HasEdge(1, 2) || directed.HasEdge(2, 1) {
		t.Fatal("directed edge list edges")
	}
	w, ok = directed.GetWeight(2, 3)
	if !ok || w != -1 {
		t.Fatalf("weight 2-3 = %v, %v", w, ok)
	}
	dVerts, dMatrix := directed.ToAdjacencyMatrix()
	dIndex := make(map[int]int, len(dVerts))
	for i, v := range dVerts {
		dIndex[v] = i
	}
	if dMatrix[dIndex[1]][dIndex[2]] != 5 {
		t.Fatal("directed matrix weight")
	}
	if !math.IsInf(dMatrix[dIndex[2]][dIndex[1]], 1) {
		t.Fatal("directed matrix should not mirror the edge")
	}

	none := GraphCoreFromEdgeList(nil, false)
	if none.IsDirected() || none.NumVertices() != 0 {
		t.Fatalf("empty edge list directed=%v vertices=%d", none.IsDirected(), none.NumVertices())
	}
}
