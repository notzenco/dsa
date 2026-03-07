package datastructures

import "math"

// AdjacencyMatrix represents a graph using an adjacency matrix.
type AdjacencyMatrix struct {
	matrix   [][]int
	n        int
	directed bool
	weighted bool
}

func NewAdjacencyMatrix(numVertices int, directed, weighted bool) *AdjacencyMatrix {
	matrix := make([][]int, numVertices)
	for i := range matrix {
		matrix[i] = make([]int, numVertices)
	}
	return &AdjacencyMatrix{
		matrix:   matrix,
		n:        numVertices,
		directed: directed,
		weighted: weighted,
	}
}

func (g *AdjacencyMatrix) NumVertices() int  { return g.n }
func (g *AdjacencyMatrix) IsDirected() bool  { return g.directed }
func (g *AdjacencyMatrix) IsWeighted() bool  { return g.weighted }

func (g *AdjacencyMatrix) EdgeCount() int {
	count := 0
	for i := 0; i < g.n; i++ {
		jStart := 0
		if !g.directed {
			jStart = i
		}
		for j := jStart; j < g.n; j++ {
			if g.matrix[i][j] != 0 {
				count++
			}
		}
	}
	return count
}

func (g *AdjacencyMatrix) isValid(v int) bool {
	return v >= 0 && v < g.n
}

func (g *AdjacencyMatrix) AddEdge(src, dest, weight int) bool {
	if !g.isValid(src) || !g.isValid(dest) || weight == 0 {
		return false
	}
	g.matrix[src][dest] = weight
	if !g.directed {
		g.matrix[dest][src] = weight
	}
	return true
}

func (g *AdjacencyMatrix) RemoveEdge(src, dest int) bool {
	if !g.isValid(src) || !g.isValid(dest) {
		return false
	}
	had := g.matrix[src][dest] != 0
	g.matrix[src][dest] = 0
	if !g.directed {
		g.matrix[dest][src] = 0
	}
	return had
}

func (g *AdjacencyMatrix) HasEdge(src, dest int) bool {
	if !g.isValid(src) || !g.isValid(dest) {
		return false
	}
	return g.matrix[src][dest] != 0
}

func (g *AdjacencyMatrix) GetWeight(src, dest int) int {
	if !g.isValid(src) || !g.isValid(dest) {
		return 0
	}
	return g.matrix[src][dest]
}

func (g *AdjacencyMatrix) Neighbors(v int) []int {
	if !g.isValid(v) {
		return nil
	}
	var result []int
	for j := 0; j < g.n; j++ {
		if g.matrix[v][j] != 0 {
			result = append(result, j)
		}
	}
	return result
}

func (g *AdjacencyMatrix) OutDegree(v int) int {
	if !g.isValid(v) {
		return 0
	}
	count := 0
	for j := 0; j < g.n; j++ {
		if g.matrix[v][j] != 0 {
			count++
		}
	}
	return count
}

func (g *AdjacencyMatrix) InDegree(v int) int {
	if !g.isValid(v) {
		return 0
	}
	count := 0
	for i := 0; i < g.n; i++ {
		if g.matrix[i][v] != 0 {
			count++
		}
	}
	return count
}

func (g *AdjacencyMatrix) BFS(source int) []int {
	if !g.isValid(source) {
		return nil
	}
	visited := make([]bool, g.n)
	visited[source] = true
	queue := []int{source}
	var result []int

	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		result = append(result, v)
		for j := 0; j < g.n; j++ {
			if g.matrix[v][j] != 0 && !visited[j] {
				visited[j] = true
				queue = append(queue, j)
			}
		}
	}
	return result
}

func (g *AdjacencyMatrix) DFS(source int) []int {
	if !g.isValid(source) {
		return nil
	}
	visited := make([]bool, g.n)
	var result []int
	stack := []int{source}

	for len(stack) > 0 {
		v := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		if visited[v] {
			continue
		}
		visited[v] = true
		result = append(result, v)
		for j := g.n - 1; j >= 0; j-- {
			if g.matrix[v][j] != 0 && !visited[j] {
				stack = append(stack, j)
			}
		}
	}
	return result
}

func (g *AdjacencyMatrix) IsConnected() bool {
	if g.n == 0 {
		return true
	}
	return len(g.BFS(0)) == g.n
}

func (g *AdjacencyMatrix) FloydWarshall() ([][]float64, bool) {
	n := g.n
	inf := math.Inf(1)
	dist := make([][]float64, n)
	for i := range dist {
		dist[i] = make([]float64, n)
		for j := range dist[i] {
			if i == j {
				dist[i][j] = 0
			} else if g.matrix[i][j] != 0 {
				dist[i][j] = float64(g.matrix[i][j])
			} else {
				dist[i][j] = inf
			}
		}
	}

	for k := 0; k < n; k++ {
		for i := 0; i < n; i++ {
			for j := 0; j < n; j++ {
				if !math.IsInf(dist[i][k], 1) && !math.IsInf(dist[k][j], 1) {
					if dist[i][k]+dist[k][j] < dist[i][j] {
						dist[i][j] = dist[i][k] + dist[k][j]
					}
				}
			}
		}
	}

	for i := 0; i < n; i++ {
		if dist[i][i] < 0 {
			return dist, false
		}
	}
	return dist, true
}

func (g *AdjacencyMatrix) TransitiveClosure() [][]bool {
	n := g.n
	closure := make([][]bool, n)
	for i := range closure {
		closure[i] = make([]bool, n)
		closure[i][i] = true
		for j := 0; j < n; j++ {
			if g.matrix[i][j] != 0 {
				closure[i][j] = true
			}
		}
	}

	for k := 0; k < n; k++ {
		for i := 0; i < n; i++ {
			for j := 0; j < n; j++ {
				closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j])
			}
		}
	}
	return closure
}

func (g *AdjacencyMatrix) Transpose() *AdjacencyMatrix {
	trans := NewAdjacencyMatrix(g.n, g.directed, g.weighted)
	for i := 0; i < g.n; i++ {
		for j := 0; j < g.n; j++ {
			trans.matrix[j][i] = g.matrix[i][j]
		}
	}
	return trans
}

func (g *AdjacencyMatrix) Matrix() [][]int {
	result := make([][]int, g.n)
	for i := range result {
		result[i] = make([]int, g.n)
		copy(result[i], g.matrix[i])
	}
	return result
}
