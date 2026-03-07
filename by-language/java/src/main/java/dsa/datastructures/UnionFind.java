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
}
