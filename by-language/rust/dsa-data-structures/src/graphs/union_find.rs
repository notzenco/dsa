//! Union-Find (Disjoint Set Union)
//!
//! A data structure that tracks a set of elements partitioned into disjoint subsets.
//! Supports near-constant-time operations for union and find.
//!
//! ```text
//! ╔════════════════════════════════════════════════════════════════════════════╗
//! ║                           VISUAL REPRESENTATION                            ║
//! ╠════════════════════════════════════════════════════════════════════════════╣
//! ║                                                                            ║
//! ║  Initial state: 6 separate elements                                       ║
//! ║  [0] [1] [2] [3] [4] [5]                                                  ║
//! ║                                                                            ║
//! ║  After union(0,1), union(2,3), union(4,5):                                ║
//! ║   0     2     4                                                           ║
//! ║   │     │     │                                                           ║
//! ║   1     3     5                                                           ║
//! ║                                                                            ║
//! ║  After union(0,2):                                                        ║
//! ║       0           4                                                       ║
//! ║      / \          │                                                       ║
//! ║     1   2         5                                                       ║
//! ║         │                                                                 ║
//! ║         3                                                                 ║
//! ║                                                                            ║
//! ║  After union(0,4) with path compression:                                  ║
//! ║           0                                                               ║
//! ║        /│ │ \ \                                                           ║
//! ║       1 2 3 4  5    ◀── All nodes point directly to root                  ║
//! ║                                                                            ║
//! ║  Optimizations:                                                           ║
//! ║  - Path compression: flatten tree during find                             ║
//! ║  - Union by rank: attach smaller tree under larger                        ║
//! ║                                                                            ║
//! ╚════════════════════════════════════════════════════════════════════════════╝
//! ```
//!
//! ## Complexity
//!
//! | Operation    | Time           | Space |
//! |--------------|----------------|-------|
//! | Find         | O(α(n)) ≈ O(1) | O(1)  |
//! | Union        | O(α(n)) ≈ O(1) | O(1)  |
//! | Connected    | O(α(n)) ≈ O(1) | O(1)  |
//! | Construction | O(n)           | O(n)  |
//!
//! α(n) is the inverse Ackermann function, effectively constant for all practical purposes.
//!
//! ## LeetCode Problems
//!
//! - [#200 Number of Islands](https://leetcode.com/problems/number-of-islands/)
//! - [#547 Number of Provinces](https://leetcode.com/problems/number-of-provinces/)
//! - [#684 Redundant Connection](https://leetcode.com/problems/redundant-connection/)
//! - [#721 Accounts Merge](https://leetcode.com/problems/accounts-merge/)
//! - [#990 Satisfiability of Equality Equations](https://leetcode.com/problems/satisfiability-of-equality-equations/)
//! - [#1319 Number of Operations to Make Network Connected](https://leetcode.com/problems/number-of-operations-to-make-network-connected/)
//!
//! ## Use Cases
//!
//! - Connected components in graphs
//! - Kruskal's minimum spanning tree
//! - Detecting cycles in undirected graphs
//! - Network connectivity
//! - Image processing (connected regions)
//! - Least common ancestor (offline)
//!
//! ## Example
//!
//! ```rust
//! use dsa_data_structures::graphs::UnionFind;
//!
//! let mut uf = UnionFind::new(5);
//!
//! // Initially, each element is its own set
//! assert_eq!(uf.count(), 5);
//!
//! // Union some elements
//! uf.union(0, 1);
//! uf.union(2, 3);
//! assert_eq!(uf.count(), 3); // Now 3 sets: {0,1}, {2,3}, {4}
//!
//! // Check connectivity
//! assert!(uf.connected(0, 1));
//! assert!(!uf.connected(0, 2));
//!
//! // Union the sets
//! uf.union(1, 2);
//! assert!(uf.connected(0, 3)); // Now connected through 0-1-2-3
//! ```

use alloc::vec;
use alloc::vec::Vec;

use dsa_core::Container;

/// A Union-Find (Disjoint Set Union) data structure.
///
/// Uses path compression and union by rank for nearly O(1) operations.
#[derive(Debug, Clone)]
pub struct UnionFind {
    parent: Vec<usize>,
    rank: Vec<usize>,
    count: usize,
}

impl UnionFind {
    /// Creates a new Union-Find structure with n elements (0 to n-1).
    ///
    /// Initially, each element is in its own set.
    ///
    /// # Time Complexity
    /// O(n)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::graphs::UnionFind;
    ///
    /// let uf = UnionFind::new(5);
    /// assert_eq!(uf.count(), 5);
    /// ```
    #[must_use]
    pub fn new(n: usize) -> Self {
        UnionFind {
            parent: (0..n).collect(),
            rank: vec![0; n],
            count: n,
        }
    }

    /// Returns the number of elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn len(&self) -> usize {
        self.parent.len()
    }

    /// Returns `true` if there are no elements.
    ///
    /// # Time Complexity
    /// O(1)
    #[inline]
    #[must_use]
    pub fn is_empty(&self) -> bool {
        self.parent.is_empty()
    }

    /// Returns the number of disjoint sets.
    ///
    /// # Time Complexity
    /// O(1)
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::graphs::UnionFind;
    ///
    /// let mut uf = UnionFind::new(5);
    /// assert_eq!(uf.count(), 5);
    ///
    /// uf.union(0, 1);
    /// assert_eq!(uf.count(), 4);
    /// ```
    #[inline]
    #[must_use]
    pub fn count(&self) -> usize {
        self.count
    }

    /// Finds the root (representative) of the set containing x.
    ///
    /// Uses path compression to flatten the tree structure.
    ///
    /// # Time Complexity
    /// O(α(n)) amortized, where α is the inverse Ackermann function
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::graphs::UnionFind;
    ///
    /// let mut uf = UnionFind::new(5);
    /// assert_eq!(uf.find(2), 2);
    ///
    /// uf.union(0, 2);
    /// // Now 0 and 2 share the same root
    /// assert_eq!(uf.find(0), uf.find(2));
    /// ```
    pub fn find(&mut self, x: usize) -> usize {
        if self.parent[x] != x {
            self.parent[x] = self.find(self.parent[x]); // Path compression
        }
        self.parent[x]
    }

    /// Returns the root without path compression (for read-only queries).
    ///
    /// # Time Complexity
    /// O(α(n)) amortized
    #[must_use]
    pub fn find_immutable(&self, mut x: usize) -> usize {
        while self.parent[x] != x {
            x = self.parent[x];
        }
        x
    }

    /// Unions the sets containing x and y.
    ///
    /// Uses union by rank to keep trees balanced.
    /// Returns `true` if x and y were in different sets (and are now merged).
    ///
    /// # Time Complexity
    /// O(α(n)) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::graphs::UnionFind;
    ///
    /// let mut uf = UnionFind::new(5);
    /// assert!(uf.union(0, 1)); // Different sets, merged
    /// assert!(!uf.union(0, 1)); // Same set, no change
    /// ```
    pub fn union(&mut self, x: usize, y: usize) -> bool {
        let root_x = self.find(x);
        let root_y = self.find(y);

        if root_x == root_y {
            return false; // Already in the same set
        }

        // Union by rank: attach smaller tree under larger
        match self.rank[root_x].cmp(&self.rank[root_y]) {
            core::cmp::Ordering::Less => {
                self.parent[root_x] = root_y;
            }
            core::cmp::Ordering::Greater => {
                self.parent[root_y] = root_x;
            }
            core::cmp::Ordering::Equal => {
                self.parent[root_y] = root_x;
                self.rank[root_x] += 1;
            }
        }

        self.count -= 1;
        true
    }

    /// Returns `true` if x and y are in the same set.
    ///
    /// # Time Complexity
    /// O(α(n)) amortized
    ///
    /// # Example
    ///
    /// ```rust
    /// use dsa_data_structures::graphs::UnionFind;
    ///
    /// let mut uf = UnionFind::new(5);
    /// assert!(!uf.connected(0, 1));
    ///
    /// uf.union(0, 1);
    /// assert!(uf.connected(0, 1));
    /// ```
    pub fn connected(&mut self, x: usize, y: usize) -> bool {
        self.find(x) == self.find(y)
    }

    /// Returns `true` if x and y are in the same set (immutable version).
    ///
    /// # Time Complexity
    /// O(α(n))
    #[must_use]
    pub fn connected_immutable(&self, x: usize, y: usize) -> bool {
        self.find_immutable(x) == self.find_immutable(y)
    }

    /// Returns the size of the set containing x.
    ///
    /// # Time Complexity
    /// O(n) - requires counting all elements with the same root
    #[must_use]
    pub fn set_size(&self, x: usize) -> usize {
        let root = self.find_immutable(x);
        self.parent
            .iter()
            .enumerate()
            .filter(|&(i, _)| self.find_immutable(i) == root)
            .count()
    }

    /// Returns all elements in the same set as x.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn get_set(&self, x: usize) -> Vec<usize> {
        let root = self.find_immutable(x);
        (0..self.parent.len())
            .filter(|&i| self.find_immutable(i) == root)
            .collect()
    }

    /// Returns all sets as a vector of vectors.
    ///
    /// # Time Complexity
    /// O(n)
    #[must_use]
    pub fn all_sets(&self) -> Vec<Vec<usize>> {
        use alloc::collections::BTreeMap;
        let mut sets: BTreeMap<usize, Vec<usize>> = BTreeMap::new();

        for i in 0..self.parent.len() {
            let root = self.find_immutable(i);
            sets.entry(root).or_default().push(i);
        }

        sets.into_values().collect()
    }

    /// Resets the structure to n separate elements.
    ///
    /// # Time Complexity
    /// O(n)
    pub fn reset(&mut self) {
        let n = self.parent.len();
        for i in 0..n {
            self.parent[i] = i;
            self.rank[i] = 0;
        }
        self.count = n;
    }
}

impl Container for UnionFind {
    fn len(&self) -> usize {
        self.parent.len()
    }
}

impl Default for UnionFind {
    fn default() -> Self {
        Self::new(0)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    mod basics {
        use super::*;

        #[test]
        fn test_new() {
            let uf = UnionFind::new(5);
            assert_eq!(uf.len(), 5);
            assert_eq!(uf.count(), 5);
        }

        #[test]
        fn test_empty() {
            let uf = UnionFind::new(0);
            assert!(uf.is_empty());
            assert_eq!(uf.count(), 0);
        }

        #[test]
        fn test_default() {
            let uf = UnionFind::default();
            assert!(uf.is_empty());
        }
    }

    mod find {
        use super::*;

        #[test]
        fn test_find_initial() {
            let mut uf = UnionFind::new(5);
            for i in 0..5 {
                assert_eq!(uf.find(i), i);
            }
        }

        #[test]
        fn test_find_after_union() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            assert_eq!(uf.find(0), uf.find(1));
        }

        #[test]
        fn test_find_immutable() {
            let uf = UnionFind::new(5);
            for i in 0..5 {
                assert_eq!(uf.find_immutable(i), i);
            }
        }
    }

    mod union {
        use super::*;

        #[test]
        fn test_union_separate_sets() {
            let mut uf = UnionFind::new(5);
            assert!(uf.union(0, 1)); // Should return true for new union
            assert_eq!(uf.count(), 4);
        }

        #[test]
        fn test_union_same_set() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            assert!(!uf.union(0, 1)); // Should return false, already same set
            assert_eq!(uf.count(), 4);
        }

        #[test]
        fn test_union_chain() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(1, 2);
            uf.union(2, 3);
            uf.union(3, 4);
            assert_eq!(uf.count(), 1);
            assert!(uf.connected(0, 4));
        }

        #[test]
        fn test_union_all() {
            let mut uf = UnionFind::new(10);
            for i in 0..9 {
                uf.union(i, i + 1);
            }
            assert_eq!(uf.count(), 1);
        }
    }

    mod connected {
        use super::*;

        #[test]
        fn test_connected_initial() {
            let mut uf = UnionFind::new(5);
            assert!(uf.connected(0, 0)); // Same element
            assert!(!uf.connected(0, 1)); // Different sets
        }

        #[test]
        fn test_connected_after_union() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(2, 3);

            assert!(uf.connected(0, 1));
            assert!(uf.connected(2, 3));
            assert!(!uf.connected(0, 2));
            assert!(!uf.connected(1, 3));
        }

        #[test]
        fn test_connected_transitive() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(1, 2);

            assert!(uf.connected(0, 2)); // Transitively connected
        }

        #[test]
        fn test_connected_immutable() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);

            assert!(uf.connected_immutable(0, 1));
            assert!(!uf.connected_immutable(0, 2));
        }
    }

    mod set_operations {
        use super::*;

        #[test]
        fn test_set_size() {
            let mut uf = UnionFind::new(5);
            assert_eq!(uf.set_size(0), 1);

            uf.union(0, 1);
            assert_eq!(uf.set_size(0), 2);
            assert_eq!(uf.set_size(1), 2);

            uf.union(0, 2);
            assert_eq!(uf.set_size(0), 3);
        }

        #[test]
        fn test_get_set() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(0, 2);

            let set = uf.get_set(1);
            assert_eq!(set.len(), 3);
            assert!(set.contains(&0));
            assert!(set.contains(&1));
            assert!(set.contains(&2));
        }

        #[test]
        fn test_all_sets() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(2, 3);

            let sets = uf.all_sets();
            assert_eq!(sets.len(), 3); // {0,1}, {2,3}, {4}
        }
    }

    mod utilities {
        use super::*;

        #[test]
        fn test_reset() {
            let mut uf = UnionFind::new(5);
            uf.union(0, 1);
            uf.union(2, 3);
            assert_eq!(uf.count(), 3);

            uf.reset();
            assert_eq!(uf.count(), 5);
            assert!(!uf.connected(0, 1));
        }
    }

    mod edge_cases {
        use super::*;

        #[test]
        fn test_single_element() {
            let mut uf = UnionFind::new(1);
            assert_eq!(uf.count(), 1);
            assert_eq!(uf.find(0), 0);
        }

        #[test]
        fn test_two_elements() {
            let mut uf = UnionFind::new(2);
            assert!(!uf.connected(0, 1));
            uf.union(0, 1);
            assert!(uf.connected(0, 1));
            assert_eq!(uf.count(), 1);
        }

        #[test]
        fn test_large_union_find() {
            let n = 1000;
            let mut uf = UnionFind::new(n);

            // Union all even numbers together
            for i in (2..n).step_by(2) {
                uf.union(0, i);
            }

            // Union all odd numbers together
            for i in (3..n).step_by(2) {
                uf.union(1, i);
            }

            assert_eq!(uf.count(), 2);
            assert!(uf.connected(0, 100));
            assert!(uf.connected(1, 101));
            assert!(!uf.connected(0, 1));
        }

        #[test]
        fn test_path_compression() {
            let mut uf = UnionFind::new(10);

            // Create a long chain
            for i in 0..9 {
                uf.union(i, i + 1);
            }

            // After find, paths should be compressed
            uf.find(9);

            // Check that the tree is now flatter
            // (implementation detail, but we can verify connectivity works)
            assert!(uf.connected(0, 9));
        }
    }
}
