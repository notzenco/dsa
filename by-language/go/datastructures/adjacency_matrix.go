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

func (g *AdjacencyMatrix) NumVertices() int { return g.n }
func (g *AdjacencyMatrix) IsDirected() bool { return g.directed }
func (g *AdjacencyMatrix) IsWeighted() bool { return g.weighted }

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
				if g.matrix[i][j] < 0 {
					dist[i][j] = float64(g.matrix[i][j])
				}
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

func (g *AdjacencyMatrix) HasCycle() bool {
	if g.directed {
		return g.hasCycleDirectedMatrix()
	}
	return g.hasCycleUndirectedMatrix()
}

func (g *AdjacencyMatrix) hasCycleDirectedMatrix() bool {
	white, gray, black := 0, 1, 2
	color := make([]int, g.n)

	var dfs func(int) bool
	dfs = func(v int) bool {
		color[v] = gray
		for j := 0; j < g.n; j++ {
			if g.matrix[v][j] != 0 {
				if color[j] == gray {
					return true
				}
				if color[j] == white && dfs(j) {
					return true
				}
			}
		}
		color[v] = black
		return false
	}

	for v := 0; v < g.n; v++ {
		if color[v] == white {
			if dfs(v) {
				return true
			}
		}
	}
	return false
}

func (g *AdjacencyMatrix) hasCycleUndirectedMatrix() bool {
	visited := make([]bool, g.n)

	var dfs func(int, int) bool
	dfs = func(v, parent int) bool {
		visited[v] = true
		for j := 0; j < g.n; j++ {
			if g.matrix[v][j] != 0 {
				if !visited[j] {
					if dfs(j, v) {
						return true
					}
				} else if j != parent {
					return true
				}
			}
		}
		return false
	}

	for v := 0; v < g.n; v++ {
		if !visited[v] {
			if dfs(v, -1) {
				return true
			}
		}
	}
	return false
}

// IsComplete checks if the graph is a complete graph.
func (g *AdjacencyMatrix) IsComplete() bool {
	for i := 0; i < g.n; i++ {
		for j := 0; j < g.n; j++ {
			if i != j && g.matrix[i][j] == 0 {
				return false
			}
		}
	}
	return true
}

// Complement returns the complement graph.
func (g *AdjacencyMatrix) Complement() *AdjacencyMatrix {
	comp := NewAdjacencyMatrix(g.n, g.directed, g.weighted)
	for i := 0; i < g.n; i++ {
		for j := 0; j < g.n; j++ {
			if i != j && g.matrix[i][j] == 0 {
				comp.matrix[i][j] = 1
			}
		}
	}
	return comp
}

// Density returns the graph density (ratio of edges to possible edges).
func (g *AdjacencyMatrix) Density() float64 {
	if g.n <= 1 {
		return 0
	}
	edges := g.EdgeCount()
	maxEdges := g.n * (g.n - 1)
	if !g.directed {
		maxEdges /= 2
	}
	return float64(edges) / float64(maxEdges)
}

// ShortestPathDijkstra finds shortest path using Dijkstra's algorithm.
func (g *AdjacencyMatrix) ShortestPathDijkstra(source, target int) ([]int, int) {
	if !g.isValid(source) || !g.isValid(target) {
		return nil, 0
	}

	dist := make([]int, g.n)
	prev := make([]int, g.n)
	visited := make([]bool, g.n)
	reached := make([]bool, g.n)

	for i := range dist {
		prev[i] = -1
	}
	dist[source] = 0
	reached[source] = true

	for count := 0; count < g.n; count++ {
		// Find minimum distance unvisited vertex
		u := -1
		for v := 0; v < g.n; v++ {
			if reached[v] && !visited[v] && (u == -1 || dist[v] < dist[u]) {
				u = v
			}
		}
		if u == -1 {
			break
		}
		visited[u] = true

		for v := 0; v < g.n; v++ {
			if g.matrix[u][v] != 0 && !visited[v] {
				weight := g.matrix[u][v]
				if (weight > 0 && dist[u] > math.MaxInt-weight) ||
					(weight < 0 && dist[u] < math.MinInt-weight) {
					continue
				}
				newDist := dist[u] + weight
				if !reached[v] || newDist < dist[v] {
					dist[v] = newDist
					prev[v] = u
					reached[v] = true
				}
			}
		}
	}

	if !reached[target] {
		return nil, 0
	}

	// Reconstruct path
	var path []int
	for v := target; v != -1; v = prev[v] {
		path = append([]int{v}, path...)
	}
	return path, dist[target]
}

// TopologicalSort returns topological ordering for a directed acyclic graph.
func (g *AdjacencyMatrix) TopologicalSort() ([]int, bool) {
	if !g.directed {
		return nil, false
	}

	inDegree := make([]int, g.n)
	for j := 0; j < g.n; j++ {
		for i := 0; i < g.n; i++ {
			if g.matrix[i][j] != 0 {
				inDegree[j]++
			}
		}
	}

	var queue []int
	for v := 0; v < g.n; v++ {
		if inDegree[v] == 0 {
			queue = append(queue, v)
		}
	}

	var result []int
	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		result = append(result, v)

		for j := 0; j < g.n; j++ {
			if g.matrix[v][j] != 0 {
				inDegree[j]--
				if inDegree[j] == 0 {
					queue = append(queue, j)
				}
			}
		}
	}

	if len(result) != g.n {
		return nil, false // cycle detected
	}
	return result, true
}

// ConnectedComponents returns the connected components for undirected graphs.
func (g *AdjacencyMatrix) ConnectedComponents() [][]int {
	visited := make([]bool, g.n)
	var components [][]int

	for v := 0; v < g.n; v++ {
		if !visited[v] {
			component := g.BFS(v)
			for _, u := range component {
				visited[u] = true
			}
			components = append(components, component)
		}
	}
	return components
}

// IsBipartite checks if the graph is bipartite.
func (g *AdjacencyMatrix) IsBipartite() bool {
	color := make([]int, g.n)
	for i := range color {
		color[i] = -1
	}

	for start := 0; start < g.n; start++ {
		if color[start] != -1 {
			continue
		}
		color[start] = 0
		queue := []int{start}
		for len(queue) > 0 {
			v := queue[0]
			queue = queue[1:]
			for j := 0; j < g.n; j++ {
				if g.matrix[v][j] != 0 {
					if color[j] == -1 {
						color[j] = 1 - color[v]
						queue = append(queue, j)
					} else if color[j] == color[v] {
						return false
					}
				}
			}
		}
	}
	return true
}

func (g *AdjacencyMatrix) Matrix() [][]int {
	result := make([][]int, g.n)
	for i := range result {
		result[i] = make([]int, g.n)
		copy(result[i], g.matrix[i])
	}
	return result
}
