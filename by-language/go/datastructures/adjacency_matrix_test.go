package datastructures

import "testing"

func TestAdjacencyMatrixAccessors(t *testing.T) {
	directed := NewAdjacencyMatrix(4, true, true)
	if directed.NumVertices() != 4 {
		t.Fatalf("NumVertices = %d, want 4", directed.NumVertices())
	}
	if !directed.IsDirected() || !directed.IsWeighted() {
		t.Fatalf("directed=%v weighted=%v", directed.IsDirected(), directed.IsWeighted())
	}

	plain := NewAdjacencyMatrix(2, false, false)
	if plain.IsDirected() || plain.IsWeighted() {
		t.Fatalf("undirected flags directed=%v weighted=%v", plain.IsDirected(), plain.IsWeighted())
	}
	if plain.NumVertices() != 2 {
		t.Fatalf("NumVertices = %d, want 2", plain.NumVertices())
	}

	if directed.AddEdge(-1, 0, 1) || directed.AddEdge(0, 4, 1) || directed.AddEdge(0, 1, 0) {
		t.Fatal("invalid AddEdge should fail")
	}
	if !directed.AddEdge(0, 1, 5) || !directed.AddEdge(0, 2, 3) || !directed.AddEdge(1, 2, 7) {
		t.Fatal("AddEdge should succeed")
	}

	if directed.HasEdge(-1, 0) || directed.HasEdge(0, 4) || directed.HasEdge(2, 0) {
		t.Fatal("HasEdge should reject invalid vertices and missing edges")
	}
	if !directed.HasEdge(0, 1) || !directed.HasEdge(1, 2) {
		t.Fatal("HasEdge missed an inserted edge")
	}

	if directed.GetWeight(-1, 0) != 0 || directed.GetWeight(0, 4) != 0 || directed.GetWeight(3, 0) != 0 {
		t.Fatal("GetWeight should be 0 for invalid or missing edges")
	}
	if directed.GetWeight(0, 1) != 5 || directed.GetWeight(1, 2) != 7 {
		t.Fatalf("weights = %d, %d", directed.GetWeight(0, 1), directed.GetWeight(1, 2))
	}

	if directed.Neighbors(-1) != nil || directed.OutDegree(-1) != 0 || directed.InDegree(9) != 0 {
		t.Fatal("invalid vertex accessors")
	}
	if neighbors := directed.Neighbors(0); len(neighbors) != 2 || neighbors[0] != 1 || neighbors[1] != 2 {
		t.Fatalf("neighbors of 0 = %v", neighbors)
	}
	if directed.OutDegree(0) != 2 || directed.InDegree(2) != 2 {
		t.Fatalf("degrees out=%d in=%d", directed.OutDegree(0), directed.InDegree(2))
	}
	if directed.BFS(-1) != nil || directed.DFS(-1) != nil {
		t.Fatal("search from an invalid vertex should be nil")
	}
	if bfs := directed.BFS(0); len(bfs) != 3 || bfs[0] != 0 || bfs[1] != 1 || bfs[2] != 2 {
		t.Fatalf("BFS = %v", bfs)
	}
	if dfs := directed.DFS(0); len(dfs) != 3 || dfs[0] != 0 {
		t.Fatalf("DFS = %v", dfs)
	}

	if directed.RemoveEdge(-1, 0) || directed.RemoveEdge(3, 3) {
		t.Fatal("RemoveEdge should fail for an invalid vertex or a missing edge")
	}
	if !directed.RemoveEdge(0, 2) || directed.HasEdge(0, 2) {
		t.Fatal("RemoveEdge should drop 0-2")
	}

	if path, dist := directed.ShortestPathDijkstra(-1, 1); path != nil || dist != 0 {
		t.Fatalf("invalid Dijkstra = %v, %d", path, dist)
	}

	empty := NewAdjacencyMatrix(0, false, false)
	if empty.NumVertices() != 0 {
		t.Fatalf("empty NumVertices = %d", empty.NumVertices())
	}
	if !empty.IsConnected() {
		t.Fatal("empty graph is connected")
	}
	if empty.HasEdge(0, 0) || empty.GetWeight(0, 0) != 0 {
		t.Fatal("empty graph has no edges")
	}

	disconnected := NewAdjacencyMatrix(3, false, false)
	if !disconnected.AddEdge(0, 1, 1) {
		t.Fatal("undirected AddEdge")
	}
	if disconnected.IsConnected() {
		t.Fatal("graph with an isolated vertex is not connected")
	}
	if !disconnected.HasEdge(1, 0) || disconnected.GetWeight(1, 0) != 1 {
		t.Fatal("undirected edge should be stored both ways")
	}

	connected := NewAdjacencyMatrix(3, false, false)
	connected.AddEdge(0, 1, 1)
	connected.AddEdge(1, 2, 1)
	if !connected.IsConnected() {
		t.Fatal("path graph should be connected")
	}
}
