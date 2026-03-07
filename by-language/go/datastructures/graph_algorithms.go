package datastructures

import (
	"container/heap"
	"math"
	"sort"
)

// --- Priority Queue for Dijkstra ---

type pqItem struct {
	vertex   int
	priority float64
	index    int
}

type pq []*pqItem

func (p pq) Len() int            { return len(p) }
func (p pq) Less(i, j int) bool  { return p[i].priority < p[j].priority }
func (p pq) Swap(i, j int)       { p[i], p[j] = p[j], p[i]; p[i].index = i; p[j].index = j }
func (p *pq) Push(x interface{}) { item := x.(*pqItem); item.index = len(*p); *p = append(*p, item) }
func (p *pq) Pop() interface{}   { old := *p; n := len(old); item := old[n-1]; *p = old[:n-1]; return item }

// --- BFS / DFS ---

func (g *GraphCore) BFS(source int) []int {
	if !g.Contains(source) {
		return nil
	}
	visited := make(map[int]bool)
	visited[source] = true
	queue := []int{source}
	var result []int

	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		result = append(result, v)
		neighbors := g.Neighbors(v)
		sort.Ints(neighbors)
		for _, u := range neighbors {
			if !visited[u] {
				visited[u] = true
				queue = append(queue, u)
			}
		}
	}
	return result
}

func (g *GraphCore) DFS(source int) []int {
	if !g.Contains(source) {
		return nil
	}
	visited := make(map[int]bool)
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
		neighbors := g.Neighbors(v)
		sort.Sort(sort.Reverse(sort.IntSlice(neighbors)))
		for _, u := range neighbors {
			if !visited[u] {
				stack = append(stack, u)
			}
		}
	}
	return result
}

func (g *GraphCore) DFSRecursive(source int) []int {
	if !g.Contains(source) {
		return nil
	}
	visited := make(map[int]bool)
	var result []int
	g.dfsHelper(source, visited, &result)
	return result
}

func (g *GraphCore) dfsHelper(v int, visited map[int]bool, result *[]int) {
	visited[v] = true
	*result = append(*result, v)
	neighbors := g.Neighbors(v)
	sort.Ints(neighbors)
	for _, u := range neighbors {
		if !visited[u] {
			g.dfsHelper(u, visited, result)
		}
	}
}

// --- Shortest Path ---

func (g *GraphCore) ShortestPathBFS(source, target int) []int {
	if !g.Contains(source) || !g.Contains(target) {
		return nil
	}

	parent := make(map[int]int)
	parent[source] = -1
	queue := []int{source}

	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		if v == target {
			var path []int
			for v != -1 {
				path = append([]int{v}, path...)
				v = parent[v]
			}
			return path
		}
		neighbors := g.Neighbors(v)
		sort.Ints(neighbors)
		for _, u := range neighbors {
			if _, ok := parent[u]; !ok {
				parent[u] = v
				queue = append(queue, u)
			}
		}
	}
	return nil
}

func (g *GraphCore) Dijkstra(source int) (map[int]float64, map[int]int) {
	dist := make(map[int]float64)
	prev := make(map[int]int)
	for v := range g.adj {
		dist[v] = math.Inf(1)
		prev[v] = -1
	}
	dist[source] = 0

	p := &pq{}
	heap.Init(p)
	heap.Push(p, &pqItem{vertex: source, priority: 0})

	for p.Len() > 0 {
		item := heap.Pop(p).(*pqItem)
		v := item.vertex
		d := item.priority

		if d > dist[v] {
			continue
		}

		for u, w := range g.adj[v] {
			newDist := dist[v] + w
			if newDist < dist[u] {
				dist[u] = newDist
				prev[u] = v
				heap.Push(p, &pqItem{vertex: u, priority: newDist})
			}
		}
	}
	return dist, prev
}

func (g *GraphCore) DijkstraPath(source, target int) ([]int, float64) {
	dist, prev := g.Dijkstra(source)
	if math.IsInf(dist[target], 1) {
		return nil, math.Inf(1)
	}
	var path []int
	for v := target; v != -1; v = prev[v] {
		path = append([]int{v}, path...)
	}
	return path, dist[target]
}

func (g *GraphCore) BellmanFord(source int) (map[int]float64, map[int]int, bool) {
	dist := make(map[int]float64)
	prev := make(map[int]int)
	vertices := g.Vertices()

	for _, v := range vertices {
		dist[v] = math.Inf(1)
		prev[v] = -1
	}
	dist[source] = 0

	for i := 0; i < len(vertices)-1; i++ {
		for _, u := range vertices {
			if math.IsInf(dist[u], 1) {
				continue
			}
			for v, w := range g.adj[u] {
				if dist[u]+w < dist[v] {
					dist[v] = dist[u] + w
					prev[v] = u
				}
			}
		}
	}

	// Check for negative cycles
	for _, u := range vertices {
		if math.IsInf(dist[u], 1) {
			continue
		}
		for v, w := range g.adj[u] {
			if dist[u]+w < dist[v] {
				return dist, prev, false
			}
		}
	}
	return dist, prev, true
}

// --- Connectivity ---

func (g *GraphCore) IsConnected() bool {
	vertices := g.Vertices()
	if len(vertices) == 0 {
		return true
	}
	visited := g.BFS(vertices[0])
	return len(visited) == len(vertices)
}

func (g *GraphCore) ConnectedComponents() [][]int {
	visited := make(map[int]bool)
	var components [][]int

	for v := range g.adj {
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

// --- Cycle Detection ---

func (g *GraphCore) HasCycle() bool {
	if g.directed {
		return g.hasCycleDirected()
	}
	return g.hasCycleUndirected()
}

func (g *GraphCore) hasCycleDirected() bool {
	white := 0 // not visited
	gray := 1  // in progress
	black := 2 // done

	color := make(map[int]int)
	for v := range g.adj {
		color[v] = white
	}

	var dfs func(int) bool
	dfs = func(v int) bool {
		color[v] = gray
		for u := range g.adj[v] {
			if color[u] == gray {
				return true
			}
			if color[u] == white && dfs(u) {
				return true
			}
		}
		color[v] = black
		return false
	}

	for v := range g.adj {
		if color[v] == white {
			if dfs(v) {
				return true
			}
		}
	}
	return false
}

func (g *GraphCore) hasCycleUndirected() bool {
	visited := make(map[int]bool)

	var dfs func(int, int) bool
	dfs = func(v, parent int) bool {
		visited[v] = true
		for u := range g.adj[v] {
			if !visited[u] {
				if dfs(u, v) {
					return true
				}
			} else if u != parent {
				return true
			}
		}
		return false
	}

	for v := range g.adj {
		if !visited[v] {
			if dfs(v, -1) {
				return true
			}
		}
	}
	return false
}

// --- Topological Sort ---

func (g *GraphCore) TopologicalSort() ([]int, bool) {
	if !g.directed {
		return nil, false
	}

	inDegree := make(map[int]int)
	for v := range g.adj {
		if _, ok := inDegree[v]; !ok {
			inDegree[v] = 0
		}
		for u := range g.adj[v] {
			inDegree[u]++
		}
	}

	var queue []int
	for v, d := range inDegree {
		if d == 0 {
			queue = append(queue, v)
		}
	}
	sort.Ints(queue)

	var result []int
	for len(queue) > 0 {
		v := queue[0]
		queue = queue[1:]
		result = append(result, v)

		neighbors := make([]int, 0)
		for u := range g.adj[v] {
			inDegree[u]--
			if inDegree[u] == 0 {
				neighbors = append(neighbors, u)
			}
		}
		sort.Ints(neighbors)
		queue = append(queue, neighbors...)
	}

	if len(result) != len(g.adj) {
		return nil, false // cycle detected
	}
	return result, true
}

// --- Bipartite Check ---

func (g *GraphCore) IsBipartite() bool {
	color := make(map[int]int) // -1: uncolored, 0/1: two colors

	for v := range g.adj {
		if _, ok := color[v]; ok {
			continue
		}
		color[v] = 0
		queue := []int{v}
		for len(queue) > 0 {
			u := queue[0]
			queue = queue[1:]
			for w := range g.adj[u] {
				if c, ok := color[w]; ok {
					if c == color[u] {
						return false
					}
				} else {
					color[w] = 1 - color[u]
					queue = append(queue, w)
				}
			}
		}
	}
	return true
}

// --- MST ---

type mstEdge struct {
	U, V   int
	Weight float64
}

func (g *GraphCore) PrimMST() ([]mstEdge, float64) {
	if len(g.adj) == 0 {
		return nil, 0
	}

	visited := make(map[int]bool)
	var edges []mstEdge
	totalWeight := 0.0

	// Start from first vertex
	var start int
	for v := range g.adj {
		start = v
		break
	}
	visited[start] = true

	p := &pq{}
	heap.Init(p)
	for u, w := range g.adj[start] {
		heap.Push(p, &pqItem{vertex: u, priority: w})
	}

	parent := make(map[int]int)
	parent[start] = -1

	// Track edges by destination
	edgeFrom := make(map[int]int)
	edgeWeight := make(map[int]float64)
	for u, w := range g.adj[start] {
		edgeFrom[u] = start
		edgeWeight[u] = w
	}

	for p.Len() > 0 && len(visited) < len(g.adj) {
		item := heap.Pop(p).(*pqItem)
		v := item.vertex
		if visited[v] {
			continue
		}
		visited[v] = true
		edges = append(edges, mstEdge{edgeFrom[v], v, edgeWeight[v]})
		totalWeight += edgeWeight[v]

		for u, w := range g.adj[v] {
			if !visited[u] {
				if oldW, ok := edgeWeight[u]; !ok || w < oldW {
					edgeFrom[u] = v
					edgeWeight[u] = w
				}
				heap.Push(p, &pqItem{vertex: u, priority: w})
			}
		}
	}
	return edges, totalWeight
}

func (g *GraphCore) KruskalMST() ([]mstEdge, float64) {
	var allEdges []mstEdge
	seen := make(map[[2]int]bool)

	for u := range g.adj {
		for v, w := range g.adj[u] {
			key := [2]int{u, v}
			if u > v {
				key = [2]int{v, u}
			}
			if !seen[key] {
				seen[key] = true
				allEdges = append(allEdges, mstEdge{u, v, w})
			}
		}
	}

	sort.Slice(allEdges, func(i, j int) bool {
		return allEdges[i].Weight < allEdges[j].Weight
	})

	// Map vertices to 0..n-1 for union-find
	vertexIndex := make(map[int]int)
	idx := 0
	for v := range g.adj {
		vertexIndex[v] = idx
		idx++
	}

	uf := NewUnionFind(len(g.adj))
	var result []mstEdge
	totalWeight := 0.0

	for _, e := range allEdges {
		ui, vi := vertexIndex[e.U], vertexIndex[e.V]
		if uf.Union(ui, vi) {
			result = append(result, e)
			totalWeight += e.Weight
		}
	}
	return result, totalWeight
}

// --- All-pairs Shortest Path ---

func (g *GraphCore) FloydWarshall() (map[int]map[int]float64, bool) {
	vertices := g.Vertices()
	n := len(vertices)
	index := make(map[int]int)
	for i, v := range vertices {
		index[v] = i
	}

	dist := make([][]float64, n)
	for i := range dist {
		dist[i] = make([]float64, n)
		for j := range dist[i] {
			if i == j {
				dist[i][j] = 0
			} else {
				dist[i][j] = math.Inf(1)
			}
		}
	}

	for u := range g.adj {
		for v, w := range g.adj[u] {
			dist[index[u]][index[v]] = w
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

	// Check negative cycles
	for i := 0; i < n; i++ {
		if dist[i][i] < 0 {
			return nil, false
		}
	}

	result := make(map[int]map[int]float64)
	for i, u := range vertices {
		result[u] = make(map[int]float64)
		for j, v := range vertices {
			result[u][v] = dist[i][j]
		}
	}
	return result, true
}

// --- SCC ---

func (g *GraphCore) TarjanSCC() [][]int {
	index := 0
	stackSlice := make([]int, 0)
	onStack := make(map[int]bool)
	indices := make(map[int]int)
	lowlinks := make(map[int]int)
	var sccs [][]int

	for v := range g.adj {
		indices[v] = -1
	}

	var strongConnect func(int)
	strongConnect = func(v int) {
		indices[v] = index
		lowlinks[v] = index
		index++
		stackSlice = append(stackSlice, v)
		onStack[v] = true

		for w := range g.adj[v] {
			if indices[w] == -1 {
				strongConnect(w)
				if lowlinks[w] < lowlinks[v] {
					lowlinks[v] = lowlinks[w]
				}
			} else if onStack[w] {
				if indices[w] < lowlinks[v] {
					lowlinks[v] = indices[w]
				}
			}
		}

		if lowlinks[v] == indices[v] {
			var scc []int
			for {
				w := stackSlice[len(stackSlice)-1]
				stackSlice = stackSlice[:len(stackSlice)-1]
				onStack[w] = false
				scc = append(scc, w)
				if w == v {
					break
				}
			}
			sccs = append(sccs, scc)
		}
	}

	for v := range g.adj {
		if indices[v] == -1 {
			strongConnect(v)
		}
	}
	return sccs
}

func (g *GraphCore) KosarajuSCC() [][]int {
	// First pass: get finish order
	visited := make(map[int]bool)
	var order []int

	var dfs1 func(int)
	dfs1 = func(v int) {
		visited[v] = true
		for u := range g.adj[v] {
			if !visited[u] {
				dfs1(u)
			}
		}
		order = append(order, v)
	}

	for v := range g.adj {
		if !visited[v] {
			dfs1(v)
		}
	}

	// Build transpose
	transpose := NewGraphCore(true)
	for v := range g.adj {
		transpose.AddVertex(v)
		for u, w := range g.adj[v] {
			transpose.AddEdge(u, v, w)
		}
	}

	// Second pass: DFS on transpose in reverse finish order
	visited2 := make(map[int]bool)
	var sccs [][]int

	var dfs2 func(int, *[]int)
	dfs2 = func(v int, component *[]int) {
		visited2[v] = true
		*component = append(*component, v)
		for u := range transpose.adj[v] {
			if !visited2[u] {
				dfs2(u, component)
			}
		}
	}

	for i := len(order) - 1; i >= 0; i-- {
		v := order[i]
		if !visited2[v] {
			var component []int
			dfs2(v, &component)
			sccs = append(sccs, component)
		}
	}
	return sccs
}
