"""
Union-Find (Disjoint Set Union) Implementation.

A data structure that tracks elements partitioned into disjoint (non-overlapping) sets.
Supports near-constant time operations for union and find.

================================================================================
                        UNION-FIND VISUAL REPRESENTATION
================================================================================

    Initial state (5 elements, each in own set):

        [0]   [1]   [2]   [3]   [4]

    After union(0, 1) and union(2, 3):

         0       2
         |       |
         1       3       [4]

    After union(0, 2):

              0
            / |
           1  2
              |
              3         [4]

    With path compression, after find(3):

              0
           / | \
          1  2  3       [4]

================================================================================

COMPLEXITY (with path compression + union by rank):
+-----------+---------------+
| Operation | Time          |
+-----------+---------------+
| Find      | O(alpha(n))   |  (alpha = inverse Ackermann, effectively O(1))
| Union     | O(alpha(n))   |
| Connected | O(alpha(n))   |
| Make Set  | O(1)          |
+-----------+---------------+
Space: O(n)

LEETCODE PROBLEMS:
- #200 Number of Islands
- #547 Number of Provinces
- #684 Redundant Connection
- #721 Accounts Merge
- #839 Similar String Groups
- #924 Minimize Malware Spread
- #990 Satisfiability of Equality Equations
- #1319 Number of Operations to Make Network Connected
- #1584 Min Cost to Connect All Points (Kruskal's MST)

USE CASES:
- Network connectivity
- Image processing (connected components)
- Kruskal's minimum spanning tree
- Least common ancestor in trees
- Equivalence class detection
- Social network friend groups
"""

from typing import TypeVar, Generic, Dict, List, Optional, Set, Iterator

T = TypeVar('T')


class UnionFind(Generic[T]):
    """
    Union-Find (Disjoint Set Union) with path compression and union by rank.

    A data structure that efficiently tracks a partition of elements into
    disjoint sets and supports union and find operations in nearly O(1) time.

    Example:
        >>> uf = UnionFind()
        >>> uf.make_set(1)
        >>> uf.make_set(2)
        >>> uf.make_set(3)
        >>> uf.union(1, 2)
        True
        >>> uf.connected(1, 2)
        True
        >>> uf.connected(1, 3)
        False
        >>> uf.num_components
        2
    """

    def __init__(self) -> None:
        """Initialize an empty Union-Find structure."""
        self._parent: Dict[T, T] = {}
        self._rank: Dict[T, int] = {}
        self._size: Dict[T, int] = {}
        self._num_components = 0

    def make_set(self, x: T) -> bool:
        """
        Create a new set containing only element x.

        Args:
            x: Element to create a set for

        Returns:
            True if new set was created, False if x already exists

        Time: O(1)
        """
        if x in self._parent:
            return False

        self._parent[x] = x
        self._rank[x] = 0
        self._size[x] = 1
        self._num_components += 1
        return True

    def find(self, x: T) -> T:
        """
        Find the representative (root) of the set containing x.

        Uses path compression for efficiency.

        Args:
            x: Element to find the root for

        Returns:
            The representative of x's set

        Raises:
            KeyError: If x is not in any set

        Time: O(alpha(n)) amortized, effectively O(1)
        """
        if x not in self._parent:
            raise KeyError(f"Element {x} not found in Union-Find")

        # Path compression: make all nodes point directly to root
        root = x
        while self._parent[root] != root:
            root = self._parent[root]

        # Compress path
        while self._parent[x] != root:
            next_x = self._parent[x]
            self._parent[x] = root
            x = next_x

        return root

    def union(self, x: T, y: T) -> bool:
        """
        Merge the sets containing x and y.

        Uses union by rank to keep trees balanced.

        Args:
            x: First element
            y: Second element

        Returns:
            True if sets were merged, False if already in same set

        Raises:
            KeyError: If x or y is not in any set

        Time: O(alpha(n)) amortized, effectively O(1)
        """
        root_x = self.find(x)
        root_y = self.find(y)

        if root_x == root_y:
            return False

        # Union by rank: attach smaller tree under larger
        if self._rank[root_x] < self._rank[root_y]:
            root_x, root_y = root_y, root_x

        self._parent[root_y] = root_x
        self._size[root_x] += self._size[root_y]

        if self._rank[root_x] == self._rank[root_y]:
            self._rank[root_x] += 1

        self._num_components -= 1
        return True

    def connected(self, x: T, y: T) -> bool:
        """
        Check if x and y are in the same set.

        Args:
            x: First element
            y: Second element

        Returns:
            True if x and y are in the same set

        Raises:
            KeyError: If x or y is not in any set

        Time: O(alpha(n)) amortized, effectively O(1)
        """
        return self.find(x) == self.find(y)

    def set_size(self, x: T) -> int:
        """
        Get the size of the set containing x.

        Args:
            x: Element to find set size for

        Returns:
            Number of elements in x's set

        Time: O(alpha(n))
        """
        root = self.find(x)
        return self._size[root]

    def get_components(self) -> List[Set[T]]:
        """
        Get all connected components as sets.

        Returns:
            List of sets, each containing elements in one component

        Time: O(n * alpha(n))
        """
        components: Dict[T, Set[T]] = {}

        for x in self._parent:
            root = self.find(x)
            if root not in components:
                components[root] = set()
            components[root].add(x)

        return list(components.values())

    @property
    def num_components(self) -> int:
        """Return the number of disjoint sets."""
        return self._num_components

    def __len__(self) -> int:
        """Return total number of elements."""
        return len(self._parent)

    def __contains__(self, x: T) -> bool:
        """Check if element exists."""
        return x in self._parent

    def __repr__(self) -> str:
        """Return string representation."""
        return f"UnionFind(elements={len(self)}, components={self._num_components})"


class UnionFindArray:
    """
    Array-based Union-Find for integer elements [0, n).

    Optimized version when elements are consecutive integers starting from 0.
    More memory efficient and cache-friendly than dictionary-based version.

    Example:
        >>> uf = UnionFindArray(5)
        >>> uf.union(0, 1)
        True
        >>> uf.union(2, 3)
        True
        >>> uf.connected(0, 1)
        True
        >>> uf.num_components
        3
    """

    def __init__(self, n: int) -> None:
        """
        Initialize Union-Find with n elements [0, n).

        Args:
            n: Number of elements
        """
        self._n = n
        self._parent = list(range(n))
        self._rank = [0] * n
        self._size = [1] * n
        self._num_components = n

    def find(self, x: int) -> int:
        """
        Find root of x with path compression.

        Time: O(alpha(n))
        """
        if not 0 <= x < self._n:
            raise IndexError(f"Index {x} out of range [0, {self._n})")

        root = x
        while self._parent[root] != root:
            root = self._parent[root]

        # Path compression
        while self._parent[x] != root:
            next_x = self._parent[x]
            self._parent[x] = root
            x = next_x

        return root

    def union(self, x: int, y: int) -> bool:
        """
        Merge sets containing x and y.

        Time: O(alpha(n))
        """
        root_x = self.find(x)
        root_y = self.find(y)

        if root_x == root_y:
            return False

        # Union by rank
        if self._rank[root_x] < self._rank[root_y]:
            root_x, root_y = root_y, root_x

        self._parent[root_y] = root_x
        self._size[root_x] += self._size[root_y]

        if self._rank[root_x] == self._rank[root_y]:
            self._rank[root_x] += 1

        self._num_components -= 1
        return True

    def connected(self, x: int, y: int) -> bool:
        """Check if x and y are connected."""
        return self.find(x) == self.find(y)

    def set_size(self, x: int) -> int:
        """Get size of set containing x."""
        return self._size[self.find(x)]

    @property
    def num_components(self) -> int:
        """Return number of components."""
        return self._num_components

    def get_components(self) -> List[List[int]]:
        """Get all components as lists."""
        components: Dict[int, List[int]] = {}
        for i in range(self._n):
            root = self.find(i)
            if root not in components:
                components[root] = []
            components[root].append(i)
        return list(components.values())

    def __len__(self) -> int:
        """Return total number of elements."""
        return self._n

    def __repr__(self) -> str:
        """Return string representation."""
        return f"UnionFindArray(n={self._n}, components={self._num_components})"


class WeightedUnionFind(Generic[T]):
    """
    Union-Find with weighted edges (potential/distance tracking).

    Tracks relative weights/distances between elements within the same set.
    Useful for problems involving relative relationships.

    Example:
        >>> wuf = WeightedUnionFind()
        >>> wuf.make_set('A')
        >>> wuf.make_set('B')
        >>> wuf.union('A', 'B', 5)  # A is 5 units "above" B
        True
        >>> wuf.diff('A', 'B')  # Returns 5
        5
    """

    def __init__(self) -> None:
        """Initialize weighted union-find."""
        self._parent: Dict[T, T] = {}
        self._rank: Dict[T, int] = {}
        self._weight: Dict[T, float] = {}  # Weight to parent
        self._num_components = 0

    def make_set(self, x: T) -> bool:
        """Create a new set containing x."""
        if x in self._parent:
            return False

        self._parent[x] = x
        self._rank[x] = 0
        self._weight[x] = 0.0
        self._num_components += 1
        return True

    def find(self, x: T) -> tuple:
        """
        Find root of x and total weight from x to root.

        Returns:
            Tuple of (root, weight_to_root)
        """
        if x not in self._parent:
            raise KeyError(f"Element {x} not found")

        if self._parent[x] == x:
            return (x, 0.0)

        # Find with path compression and weight accumulation
        root, weight = self.find(self._parent[x])
        self._parent[x] = root
        self._weight[x] += weight

        return (root, self._weight[x])

    def union(self, x: T, y: T, weight: float = 0.0) -> bool:
        """
        Union sets containing x and y with relative weight.

        weight represents: value[x] - value[y] = weight

        Args:
            x: First element
            y: Second element
            weight: Relative weight (x - y)

        Returns:
            True if union was performed
        """
        root_x, weight_x = self.find(x)
        root_y, weight_y = self.find(y)

        if root_x == root_y:
            return False

        # Union by rank
        if self._rank[root_x] < self._rank[root_y]:
            self._parent[root_x] = root_y
            # weight_x + new_weight = weight + weight_y
            self._weight[root_x] = weight + weight_y - weight_x
        else:
            self._parent[root_y] = root_x
            # weight_y + new_weight = -weight + weight_x
            self._weight[root_y] = -weight + weight_x - weight_y
            if self._rank[root_x] == self._rank[root_y]:
                self._rank[root_x] += 1

        self._num_components -= 1
        return True

    def connected(self, x: T, y: T) -> bool:
        """Check if x and y are connected."""
        if x not in self._parent or y not in self._parent:
            return False
        root_x, _ = self.find(x)
        root_y, _ = self.find(y)
        return root_x == root_y

    def diff(self, x: T, y: T) -> Optional[float]:
        """
        Get the difference value[x] - value[y].

        Returns:
            The difference if x and y are connected, None otherwise
        """
        if not self.connected(x, y):
            return None

        _, weight_x = self.find(x)
        _, weight_y = self.find(y)
        return weight_x - weight_y

    @property
    def num_components(self) -> int:
        """Return number of components."""
        return self._num_components

    def __len__(self) -> int:
        """Return number of elements."""
        return len(self._parent)

    def __contains__(self, x: T) -> bool:
        """Check if element exists."""
        return x in self._parent


class PersistentUnionFind:
    """
    Partially persistent Union-Find that supports querying past states.

    Maintains version history allowing queries at any past version.
    Only find operations can query past versions.

    Example:
        >>> puf = PersistentUnionFind(5)
        >>> v1 = puf.union(0, 1)  # Version 1
        >>> v2 = puf.union(2, 3)  # Version 2
        >>> puf.connected(0, 1, version=0)  # Before any unions
        False
        >>> puf.connected(0, 1, version=1)  # After first union
        True
    """

    def __init__(self, n: int) -> None:
        """Initialize with n elements."""
        self._n = n
        self._parent = list(range(n))
        self._rank = [0] * n
        self._version = 0
        self._history: List[tuple] = []  # (version, index, old_parent, old_rank)

    def find(self, x: int, version: Optional[int] = None) -> int:
        """
        Find root of x, optionally at a specific version.

        Args:
            x: Element to find root for
            version: Optional version to query (default: current)

        Returns:
            Root of x's set
        """
        if version is None:
            version = self._version

        # Get parent at specified version
        parent = self._get_parent_at_version(x, version)

        if parent == x:
            return x

        return self.find(parent, version)

    def _get_parent_at_version(self, x: int, version: int) -> int:
        """Get parent of x at specific version."""
        parent = self._parent[x]

        # Check history for earlier values
        for v, idx, old_parent, _ in reversed(self._history):
            if v > version and idx == x:
                parent = old_parent

        return parent

    def union(self, x: int, y: int) -> int:
        """
        Union sets and return new version number.

        Returns:
            New version number after union
        """
        root_x = self.find(x)
        root_y = self.find(y)

        if root_x == root_y:
            return self._version

        self._version += 1

        # Union by rank
        if self._rank[root_x] < self._rank[root_y]:
            root_x, root_y = root_y, root_x

        # Record history before modification
        self._history.append(
            (self._version, root_y, self._parent[root_y], self._rank[root_y])
        )
        self._parent[root_y] = root_x

        if self._rank[root_x] == self._rank[root_y]:
            self._history.append(
                (self._version, root_x, self._parent[root_x], self._rank[root_x])
            )
            self._rank[root_x] += 1

        return self._version

    def connected(self, x: int, y: int, version: Optional[int] = None) -> bool:
        """Check if x and y are connected at specific version."""
        return self.find(x, version) == self.find(y, version)

    @property
    def current_version(self) -> int:
        """Return current version number."""
        return self._version

    def __len__(self) -> int:
        """Return number of elements."""
        return self._n


# Utility functions for common use cases

def num_islands(grid: List[List[int]]) -> int:
    """
    Count number of islands (connected 1s) in a grid.

    LeetCode #200

    Args:
        grid: 2D grid where 1 is land and 0 is water

    Returns:
        Number of islands
    """
    if not grid or not grid[0]:
        return 0

    rows, cols = len(grid), len(grid[0])
    uf = UnionFindArray(rows * cols)

    land_count = 0

    for i in range(rows):
        for j in range(cols):
            if grid[i][j] == 1:
                land_count += 1
                idx = i * cols + j

                # Check right and down neighbors
                if j + 1 < cols and grid[i][j + 1] == 1:
                    uf.union(idx, idx + 1)

                if i + 1 < rows and grid[i + 1][j] == 1:
                    uf.union(idx, idx + cols)

    # Count unique components among land cells
    if land_count == 0:
        return 0

    roots = set()
    for i in range(rows):
        for j in range(cols):
            if grid[i][j] == 1:
                roots.add(uf.find(i * cols + j))

    return len(roots)


def find_redundant_connection(edges: List[List[int]]) -> List[int]:
    """
    Find the edge that makes a tree cyclic.

    LeetCode #684

    Args:
        edges: List of [u, v] edges (1-indexed)

    Returns:
        The redundant edge
    """
    n = len(edges)
    uf = UnionFindArray(n + 1)  # 1-indexed

    for u, v in edges:
        if not uf.union(u, v):
            return [u, v]

    return []
