package dsa.datastructures;

/**
 * Union-Find (Disjoint Set Union) data structure with path compression
 * and union by rank for near-constant amortized operations.
 */
public class UnionFind {

    private final int[] parent;
    private final int[] rank;
    private final int[] componentSize;
    private int componentCount;
    private final int n;

    /**
     * Creates a Union-Find structure with n elements (0 to n-1).
     * Each element starts in its own singleton set.
     *
     * @param n the number of elements
     * @throws IllegalArgumentException if n is not positive
     */
    public UnionFind(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("Number of elements must be positive");
        }
        this.n = n;
        this.parent = new int[n];
        this.rank = new int[n];
        this.componentSize = new int[n];
        this.componentCount = n;
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
            componentSize[i] = 1;
        }
    }

    /**
     * Finds the root representative of the set containing element x,
     * applying path compression to flatten the tree.
     *
     * @param x the element to find
     * @return the root representative of x's set
     * @throws IndexOutOfBoundsException if x is out of range
     */
    public int find(int x) {
        validate(x);
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    /**
     * Merges the sets containing elements x and y using union by rank.
     *
     * @param x an element in the first set
     * @param y an element in the second set
     * @return true if a merge occurred, false if x and y were already in the same set
     * @throws IndexOutOfBoundsException if x or y is out of range
     */
    public boolean union(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) {
            return false;
        }
        // Union by rank: attach the shorter tree under the taller tree
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
            componentSize[rootY] += componentSize[rootX];
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
            componentSize[rootX] += componentSize[rootY];
        } else {
            parent[rootY] = rootX;
            componentSize[rootX] += componentSize[rootY];
            rank[rootX]++;
        }
        componentCount--;
        return true;
    }

    /**
     * Checks whether elements x and y belong to the same set.
     *
     * @param x the first element
     * @param y the second element
     * @return true if x and y are in the same set
     * @throws IndexOutOfBoundsException if x or y is out of range
     */
    public boolean connected(int x, int y) {
        return find(x) == find(y);
    }

    /**
     * Returns the number of disjoint sets (connected components).
     *
     * @return the current component count
     */
    public int getComponentCount() {
        return componentCount;
    }

    /**
     * Returns the size of the component containing element x.
     *
     * @param x the element
     * @return the number of elements in x's component
     * @throws IndexOutOfBoundsException if x is out of range
     */
    public int getComponentSize(int x) {
        return componentSize[find(x)];
    }

    /**
     * Returns the total number of elements in the Union-Find structure.
     *
     * @return the total element count
     */
    public int size() {
        return n;
    }

    private void validate(int x) {
        if (x < 0 || x >= n) {
            throw new IndexOutOfBoundsException(
                    "Index " + x + " is out of range [0, " + (n - 1) + "]");
        }
    }

    /**
     * Union-Find variant that tracks edge weights/distances between elements.
     * weight[x] represents the weight relationship from x to its parent,
     * such that weight(x) - weight(parent(x)) = weight[x].
     */
    public static class WeightedUnionFind {

        private final int[] parent;
        private final int[] rank;
        private final double[] weight;
        private int componentCount;
        private final int n;

        public WeightedUnionFind(int n) {
            if (n <= 0) {
                throw new IllegalArgumentException("Number of elements must be positive");
            }
            this.n = n;
            this.parent = new int[n];
            this.rank = new int[n];
            this.weight = new double[n];
            this.componentCount = n;
            for (int i = 0; i < n; i++) {
                parent[i] = i;
                rank[i] = 0;
                weight[i] = 0.0;
            }
        }

        /**
         * Finds the root of x with path compression, updating weights along the path.
         */
        public int find(int x) {
            validate(x);
            if (parent[x] == x) {
                return x;
            }
            int root = find(parent[x]);
            weight[x] += weight[parent[x]];
            parent[x] = root;
            return root;
        }

        /**
         * Unions x and y with weight w, meaning weight(x) - weight(y) = w.
         * Returns true if a merge occurred, false if already connected.
         */
        public boolean union(int x, int y, double w) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) {
                return false;
            }
            // weight[x] is now the weight from x to rootX
            // weight[y] is now the weight from y to rootY
            // We want: weight(x) - weight(y) = w
            // So: weight(rootY) relative to rootX = weight[x] - weight[y] - w
            // But we're attaching rootY under rootX, so:
            // weight[rootY] = weight[x] - w - weight[y]
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
                weight[rootX] = weight[y] + w - weight[x];
                // swap is handled by attaching smaller under bigger
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
                weight[rootY] = weight[x] - w - weight[y];
            } else {
                parent[rootY] = rootX;
                weight[rootY] = weight[x] - w - weight[y];
                rank[rootX]++;
            }
            componentCount--;
            return true;
        }

        public boolean connected(int x, int y) {
            return find(x) == find(y);
        }

        /**
         * Returns weight(x) - weight(y). Both must be in the same component.
         *
         * @throws IllegalArgumentException if x and y are not connected
         */
        public double getWeight(int x, int y) {
            if (!connected(x, y)) {
                throw new IllegalArgumentException(
                        "Elements " + x + " and " + y + " are not connected");
            }
            return weight[x] - weight[y];
        }

        public int getComponentCount() {
            return componentCount;
        }

        public int size() {
            return n;
        }

        private void validate(int x) {
            if (x < 0 || x >= n) {
                throw new IndexOutOfBoundsException(
                        "Index " + x + " is out of range [0, " + (n - 1) + "]");
            }
        }
    }

    /**
     * Counts the number of islands in a 2D grid using union-find.
     * '1' represents land and '0' represents water.
     * Adjacent (horizontally/vertically) land cells form a single island.
     */
    public static int numIslands(char[][] grid) {
        if (grid == null || grid.length == 0 || grid[0].length == 0) {
            return 0;
        }
        int rows = grid.length;
        int cols = grid[0].length;
        int totalCells = rows * cols;
        UnionFind uf = new UnionFind(totalCells);

        int landCount = 0;
        int[][] directions = {{0, 1}, {1, 0}};

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == '1') {
                    landCount++;
                    for (int[] dir : directions) {
                        int nr = r + dir[0];
                        int nc = c + dir[1];
                        if (nr < rows && nc < cols && grid[nr][nc] == '1') {
                            int cellA = r * cols + c;
                            int cellB = nr * cols + nc;
                            if (uf.union(cellA, cellB)) {
                                landCount--;
                            }
                        }
                    }
                }
            }
        }
        return landCount;
    }

    /**
     * Given edges of a graph that would form a tree plus one extra edge,
     * returns the extra edge that causes a cycle.
     * Uses union-find: the first edge where both vertices are already connected.
     * Edges are 1-indexed: edges[i] = {u, v}.
     */
    public static int[] findRedundantConnection(int[][] edges) {
        int n = edges.length;
        UnionFind uf = new UnionFind(n + 1);
        for (int[] edge : edges) {
            if (!uf.union(edge[0], edge[1])) {
                return edge;
            }
        }
        return new int[0];
    }
}
