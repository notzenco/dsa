package datastructures

import "math"

// GraphCore is a graph using adjacency list representation.
type GraphCore struct {
	adj      map[int]map[int]float64
	directed bool
}

func NewGraphCore(directed bool) *GraphCore {
	return &GraphCore{
		adj:      make(map[int]map[int]float64),
		directed: directed,
	}
}

func (g *GraphCore) IsDirected() bool { return g.directed }

func (g *GraphCore) NumVertices() int { return len(g.adj) }

func (g *GraphCore) EdgeCount() int {
	count := 0
	for _, neighbors := range g.adj {
		count += len(neighbors)
	}
	if !g.directed {
		count /= 2
	}
	return count
}

func (g *GraphCore) Vertices() []int {
	result := make([]int, 0, len(g.adj))
	for v := range g.adj {
		result = append(result, v)
	}
	return result
}

func (g *GraphCore) AddVertex(v int) {
	if _, ok := g.adj[v]; !ok {
		g.adj[v] = make(map[int]float64)
	}
}

func (g *GraphCore) AddEdge(u, v int, weight float64) {
	g.AddVertex(u)
	g.AddVertex(v)
	g.adj[u][v] = weight
	if !g.directed {
		g.adj[v][u] = weight
	}
}

func (g *GraphCore) RemoveEdge(u, v int) bool {
	if _, ok := g.adj[u]; !ok {
		return false
	}
	if _, ok := g.adj[u][v]; !ok {
		return false
	}
	delete(g.adj[u], v)
	if !g.directed {
		delete(g.adj[v], u)
	}
	return true
}

func (g *GraphCore) RemoveVertex(v int) bool {
	if _, ok := g.adj[v]; !ok {
		return false
	}
	for u := range g.adj {
		delete(g.adj[u], v)
	}
	delete(g.adj, v)
	return true
}

func (g *GraphCore) HasEdge(u, v int) bool {
	if neighbors, ok := g.adj[u]; ok {
		_, exists := neighbors[v]
		return exists
	}
	return false
}

func (g *GraphCore) GetWeight(u, v int) (float64, bool) {
	if neighbors, ok := g.adj[u]; ok {
		w, exists := neighbors[v]
		return w, exists
	}
	return 0, false
}

func (g *GraphCore) Neighbors(v int) []int {
	if neighbors, ok := g.adj[v]; ok {
		result := make([]int, 0, len(neighbors))
		for u := range neighbors {
			result = append(result, u)
		}
		return result
	}
	return nil
}

func (g *GraphCore) NeighborsWeighted(v int) []struct{ Vertex int; Weight float64 } {
	if neighbors, ok := g.adj[v]; ok {
		result := make([]struct{ Vertex int; Weight float64 }, 0, len(neighbors))
		for u, w := range neighbors {
			result = append(result, struct{ Vertex int; Weight float64 }{u, w})
		}
		return result
	}
	return nil
}

func (g *GraphCore) Contains(v int) bool {
	_, ok := g.adj[v]
	return ok
}

func (g *GraphCore) ToAdjacencyMatrix() ([]int, [][]float64) {
	vertices := g.Vertices()
	n := len(vertices)
	index := make(map[int]int)
	for i, v := range vertices {
		index[v] = i
	}

	matrix := make([][]float64, n)
	for i := range matrix {
		matrix[i] = make([]float64, n)
		for j := range matrix[i] {
			if i == j {
				matrix[i][j] = 0
			} else {
				matrix[i][j] = math.Inf(1)
			}
		}
	}

	for u := range g.adj {
		for v, w := range g.adj[u] {
			matrix[index[u]][index[v]] = w
		}
	}
	return vertices, matrix
}

func GraphCoreFromEdgeList(edges [][3]int, directed bool) *GraphCore {
	g := NewGraphCore(directed)
	for _, e := range edges {
		g.AddEdge(e[0], e[1], float64(e[2]))
	}
	return g
}
