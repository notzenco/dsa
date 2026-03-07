import { describe, test, expect } from "bun:test";
import {
  UnionFind,
  UnionFindArray,
  WeightedUnionFind,
  PersistentUnionFind,
  numIslands,
  findRedundantConnection,
} from "../../src/data-structures/union-find";

// ============================================================================
// UnionFind<T> (Generic dict-based)
// ============================================================================
describe("UnionFind<T>", () => {
  test("makeSet creates new sets", () => {
    const uf = new UnionFind<number>();
    expect(uf.makeSet(1)).toBe(true);
    expect(uf.makeSet(2)).toBe(true);
    expect(uf.makeSet(3)).toBe(true);
    expect(uf.size).toBe(3);
    expect(uf.numComponents).toBe(3);
  });

  test("makeSet returns false for duplicate", () => {
    const uf = new UnionFind<number>();
    expect(uf.makeSet(1)).toBe(true);
    expect(uf.makeSet(1)).toBe(false);
    expect(uf.size).toBe(1);
    expect(uf.numComponents).toBe(1);
  });

  test("find returns element itself when alone", () => {
    const uf = new UnionFind<number>();
    uf.makeSet(42);
    expect(uf.find(42)).toBe(42);
  });

  test("find throws for nonexistent element", () => {
    const uf = new UnionFind<number>();
    expect(() => uf.find(99)).toThrow("not found");
  });

  test("union merges two sets", () => {
    const uf = new UnionFind<number>();
    uf.makeSet(1);
    uf.makeSet(2);
    expect(uf.union(1, 2)).toBe(true);
    expect(uf.numComponents).toBe(1);
    expect(uf.connected(1, 2)).toBe(true);
  });

  test("union returns false when already connected", () => {
    const uf = new UnionFind<number>();
    uf.makeSet(1);
    uf.makeSet(2);
    uf.union(1, 2);
    expect(uf.union(1, 2)).toBe(false);
  });

  test("connected returns false for elements in different sets", () => {
    const uf = new UnionFind<number>();
    uf.makeSet(1);
    uf.makeSet(2);
    uf.makeSet(3);
    uf.union(1, 2);
    expect(uf.connected(1, 3)).toBe(false);
    expect(uf.connected(2, 3)).toBe(false);
  });

  test("transitive connectivity", () => {
    const uf = new UnionFind<number>();
    for (let i = 0; i < 5; i++) uf.makeSet(i);
    uf.union(0, 1);
    uf.union(1, 2);
    uf.union(3, 4);
    expect(uf.connected(0, 2)).toBe(true);
    expect(uf.connected(0, 3)).toBe(false);
    uf.union(2, 3);
    expect(uf.connected(0, 4)).toBe(true);
    expect(uf.numComponents).toBe(1);
  });

  test("componentSize tracks correctly", () => {
    const uf = new UnionFind<string>();
    uf.makeSet("a");
    uf.makeSet("b");
    uf.makeSet("c");
    expect(uf.componentSize("a")).toBe(1);
    uf.union("a", "b");
    expect(uf.componentSize("a")).toBe(2);
    expect(uf.componentSize("b")).toBe(2);
    uf.union("a", "c");
    expect(uf.componentSize("c")).toBe(3);
  });

  test("getComponents returns correct sets", () => {
    const uf = new UnionFind<number>();
    for (let i = 0; i < 6; i++) uf.makeSet(i);
    uf.union(0, 1);
    uf.union(2, 3);
    uf.union(4, 5);

    const components = uf.getComponents();
    expect(components.length).toBe(3);

    // Each component should be a set of 2 elements
    for (const comp of components) {
      expect(comp.size).toBe(2);
    }

    // Verify specific groupings
    const findComponent = (x: number) =>
      components.find((c) => c.has(x))!;
    expect(findComponent(0)).toBe(findComponent(1));
    expect(findComponent(2)).toBe(findComponent(3));
    expect(findComponent(4)).toBe(findComponent(5));
  });

  test("has checks membership", () => {
    const uf = new UnionFind<number>();
    expect(uf.has(1)).toBe(false);
    uf.makeSet(1);
    expect(uf.has(1)).toBe(true);
  });

  test("works with string elements", () => {
    const uf = new UnionFind<string>();
    uf.makeSet("alice");
    uf.makeSet("bob");
    uf.makeSet("charlie");
    uf.union("alice", "bob");
    expect(uf.connected("alice", "bob")).toBe(true);
    expect(uf.connected("alice", "charlie")).toBe(false);
  });

  test("path compression flattens tree", () => {
    const uf = new UnionFind<number>();
    // Create a chain: 0 <- 1 <- 2 <- 3 <- 4
    for (let i = 0; i < 5; i++) uf.makeSet(i);
    uf.union(0, 1);
    uf.union(1, 2);
    uf.union(2, 3);
    uf.union(3, 4);

    // Finding 4 should compress path
    const root = uf.find(4);
    // After path compression, find(4) should return directly
    expect(uf.find(4)).toBe(root);
    expect(uf.connected(0, 4)).toBe(true);
  });

  test("union by rank keeps trees balanced", () => {
    const uf = new UnionFind<number>();
    // Create two sets of different sizes
    for (let i = 0; i < 10; i++) uf.makeSet(i);

    // Build a larger set: {0,1,2,3,4}
    uf.union(0, 1);
    uf.union(0, 2);
    uf.union(0, 3);
    uf.union(0, 4);

    // Build a smaller set: {5,6}
    uf.union(5, 6);

    // When we merge, the smaller tree should attach under the larger
    uf.union(0, 5);
    expect(uf.componentSize(0)).toBe(7);
    expect(uf.numComponents).toBe(3);
  });

  test("toString gives meaningful output", () => {
    const uf = new UnionFind<number>();
    uf.makeSet(1);
    uf.makeSet(2);
    expect(uf.toString()).toBe("UnionFind(elements=2, components=2)");
  });

  test("large number of elements", () => {
    const uf = new UnionFind<number>();
    const n = 1000;
    for (let i = 0; i < n; i++) uf.makeSet(i);

    // Union all even numbers together
    for (let i = 2; i < n; i += 2) uf.union(0, i);
    // Union all odd numbers together
    for (let i = 3; i < n; i += 2) uf.union(1, i);

    expect(uf.numComponents).toBe(2);
    expect(uf.componentSize(0)).toBe(n / 2);
    expect(uf.componentSize(1)).toBe(n / 2);
    expect(uf.connected(0, 2)).toBe(true);
    expect(uf.connected(1, 3)).toBe(true);
    expect(uf.connected(0, 1)).toBe(false);
  });
});

// ============================================================================
// UnionFindArray
// ============================================================================
describe("UnionFindArray", () => {
  test("constructor initializes n singletons", () => {
    const uf = new UnionFindArray(5);
    expect(uf.size).toBe(5);
    expect(uf.numComponents).toBe(5);
  });

  test("find returns self for singletons", () => {
    const uf = new UnionFindArray(3);
    expect(uf.find(0)).toBe(0);
    expect(uf.find(1)).toBe(1);
    expect(uf.find(2)).toBe(2);
  });

  test("find throws for out-of-range index", () => {
    const uf = new UnionFindArray(5);
    expect(() => uf.find(-1)).toThrow("out of range");
    expect(() => uf.find(5)).toThrow("out of range");
  });

  test("union and connected", () => {
    const uf = new UnionFindArray(5);
    expect(uf.union(0, 1)).toBe(true);
    expect(uf.connected(0, 1)).toBe(true);
    expect(uf.connected(0, 2)).toBe(false);
    expect(uf.numComponents).toBe(4);
  });

  test("union returns false for already connected", () => {
    const uf = new UnionFindArray(3);
    uf.union(0, 1);
    expect(uf.union(0, 1)).toBe(false);
  });

  test("componentSize", () => {
    const uf = new UnionFindArray(5);
    expect(uf.componentSize(0)).toBe(1);
    uf.union(0, 1);
    expect(uf.componentSize(0)).toBe(2);
    expect(uf.componentSize(1)).toBe(2);
    uf.union(0, 2);
    expect(uf.componentSize(2)).toBe(3);
  });

  test("getComponents returns correct groupings", () => {
    const uf = new UnionFindArray(6);
    uf.union(0, 1);
    uf.union(2, 3);
    uf.union(4, 5);

    const components = uf.getComponents();
    expect(components.length).toBe(3);
    for (const comp of components) {
      expect(comp.length).toBe(2);
    }
  });

  test("path compression works correctly", () => {
    const uf = new UnionFindArray(6);
    // Create chain
    uf.union(0, 1);
    uf.union(1, 2);
    uf.union(2, 3);
    uf.union(3, 4);
    uf.union(4, 5);

    // All should share the same root after find
    const root = uf.find(5);
    expect(uf.find(0)).toBe(root);
    expect(uf.find(3)).toBe(root);
    expect(uf.numComponents).toBe(1);
  });

  test("large scale operations", () => {
    const n = 10000;
    const uf = new UnionFindArray(n);
    for (let i = 1; i < n; i++) {
      uf.union(0, i);
    }
    expect(uf.numComponents).toBe(1);
    expect(uf.componentSize(0)).toBe(n);
  });

  test("toString", () => {
    const uf = new UnionFindArray(5);
    expect(uf.toString()).toBe("UnionFindArray(n=5, components=5)");
  });
});

// ============================================================================
// WeightedUnionFind
// ============================================================================
describe("WeightedUnionFind", () => {
  test("makeSet and find", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    const [root, weight] = wuf.find("A");
    expect(root).toBe("A");
    expect(weight).toBe(0);
  });

  test("makeSet returns false for duplicates", () => {
    const wuf = new WeightedUnionFind<string>();
    expect(wuf.makeSet("A")).toBe(true);
    expect(wuf.makeSet("A")).toBe(false);
  });

  test("find throws for nonexistent element", () => {
    const wuf = new WeightedUnionFind<string>();
    expect(() => wuf.find("X")).toThrow("not found");
  });

  test("union with weight tracking", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    wuf.makeSet("B");
    // A - B = 5 => A is 5 more than B
    expect(wuf.union("A", "B", 5)).toBe(true);
    expect(wuf.connected("A", "B")).toBe(true);
  });

  test("union returns false when already connected", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    wuf.makeSet("B");
    wuf.union("A", "B", 5);
    expect(wuf.union("A", "B", 3)).toBe(false);
  });

  test("diff returns correct weight difference", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    wuf.makeSet("B");
    wuf.union("A", "B", 5);

    expect(wuf.diff("A", "B")).toBe(5);
    expect(wuf.diff("B", "A")).toBe(-5);
  });

  test("diff returns undefined for disconnected elements", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    wuf.makeSet("B");
    expect(wuf.diff("A", "B")).toBeUndefined();
  });

  test("transitive weight calculation", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    wuf.makeSet("B");
    wuf.makeSet("C");

    // A - B = 3, B - C = 7 => A - C = 10
    wuf.union("A", "B", 3);
    wuf.union("B", "C", 7);

    expect(wuf.diff("A", "C")).toBe(10);
    expect(wuf.diff("C", "A")).toBe(-10);
    expect(wuf.diff("A", "B")).toBe(3);
    expect(wuf.diff("B", "C")).toBe(7);
  });

  test("getWeight returns weight to root", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    expect(wuf.getWeight("A")).toBe(0);
    expect(wuf.getWeight("X" as string)).toBeUndefined();
  });

  test("connected returns false for unknown elements", () => {
    const wuf = new WeightedUnionFind<string>();
    wuf.makeSet("A");
    expect(wuf.connected("A", "B" as string)).toBe(false);
  });

  test("numComponents decreases on union", () => {
    const wuf = new WeightedUnionFind<number>();
    wuf.makeSet(0);
    wuf.makeSet(1);
    wuf.makeSet(2);
    expect(wuf.numComponents).toBe(3);
    wuf.union(0, 1, 1);
    expect(wuf.numComponents).toBe(2);
    wuf.union(1, 2, 2);
    expect(wuf.numComponents).toBe(1);
  });

  test("chain of weighted unions", () => {
    const wuf = new WeightedUnionFind<number>();
    for (let i = 0; i < 5; i++) wuf.makeSet(i);

    // 0-1=2, 1-2=3, 2-3=4, 3-4=5
    wuf.union(0, 1, 2);
    wuf.union(1, 2, 3);
    wuf.union(2, 3, 4);
    wuf.union(3, 4, 5);

    expect(wuf.diff(0, 4)).toBe(2 + 3 + 4 + 5); // 14
    expect(wuf.diff(4, 0)).toBe(-14);
    expect(wuf.diff(1, 3)).toBe(3 + 4); // 7
  });

  test("has checks membership", () => {
    const wuf = new WeightedUnionFind<number>();
    expect(wuf.has(1)).toBe(false);
    wuf.makeSet(1);
    expect(wuf.has(1)).toBe(true);
  });

  test("size returns element count", () => {
    const wuf = new WeightedUnionFind<number>();
    expect(wuf.size).toBe(0);
    wuf.makeSet(1);
    wuf.makeSet(2);
    expect(wuf.size).toBe(2);
  });
});

// ============================================================================
// PersistentUnionFind
// ============================================================================
describe("PersistentUnionFind", () => {
  test("initial state: all elements disconnected", () => {
    const puf = new PersistentUnionFind(5);
    expect(puf.size).toBe(5);
    expect(puf.currentVersion).toBe(0);
    for (let i = 0; i < 5; i++) {
      expect(puf.find(i)).toBe(i);
    }
  });

  test("union increments version", () => {
    const puf = new PersistentUnionFind(5);
    const v1 = puf.union(0, 1);
    expect(v1).toBe(1);
    expect(puf.currentVersion).toBe(1);
  });

  test("union of already connected returns same version", () => {
    const puf = new PersistentUnionFind(5);
    puf.union(0, 1);
    const v = puf.union(0, 1);
    expect(v).toBe(1); // No new version created
  });

  test("connected at different versions", () => {
    const puf = new PersistentUnionFind(5);
    puf.union(0, 1); // version 1
    puf.union(2, 3); // version 2

    // At version 0 (before any unions), nothing is connected
    expect(puf.connected(0, 1, 0)).toBe(false);
    expect(puf.connected(2, 3, 0)).toBe(false);

    // At version 1, only 0-1 are connected
    expect(puf.connected(0, 1, 1)).toBe(true);
    expect(puf.connected(2, 3, 1)).toBe(false);

    // At version 2 (current), both pairs connected
    expect(puf.connected(0, 1, 2)).toBe(true);
    expect(puf.connected(2, 3, 2)).toBe(true);
  });

  test("find at past version returns correct root", () => {
    const puf = new PersistentUnionFind(4);
    puf.union(0, 1); // version 1
    puf.union(2, 3); // version 2
    puf.union(0, 2); // version 3

    // At version 2, 0-2 not connected
    expect(puf.connected(0, 2, 2)).toBe(false);

    // At current version, all connected
    expect(puf.connected(0, 3)).toBe(true);
  });

  test("snapshot returns current version", () => {
    const puf = new PersistentUnionFind(5);
    const s0 = puf.snapshot();
    expect(s0).toBe(0);

    puf.union(0, 1);
    const s1 = puf.snapshot();
    expect(s1).toBe(1);
  });

  test("restore validates version range", () => {
    const puf = new PersistentUnionFind(5);
    puf.union(0, 1);
    expect(() => puf.restore(-1)).toThrow("out of range");
    expect(() => puf.restore(100)).toThrow("out of range");
    expect(() => puf.restore(0)).not.toThrow();
    expect(() => puf.restore(1)).not.toThrow();
  });

  test("multiple unions and version queries", () => {
    const puf = new PersistentUnionFind(6);
    puf.union(0, 1); // v1
    puf.union(2, 3); // v2
    puf.union(4, 5); // v3
    puf.union(0, 2); // v4
    puf.union(0, 4); // v5

    // At version 3, three pairs
    expect(puf.connected(0, 1, 3)).toBe(true);
    expect(puf.connected(2, 3, 3)).toBe(true);
    expect(puf.connected(4, 5, 3)).toBe(true);
    expect(puf.connected(0, 2, 3)).toBe(false);

    // At version 5, all connected
    expect(puf.connected(0, 5)).toBe(true);
    expect(puf.connected(1, 4)).toBe(true);
  });
});

// ============================================================================
// Utility: numIslands
// ============================================================================
describe("numIslands", () => {
  test("empty grid returns 0", () => {
    expect(numIslands([])).toBe(0);
    expect(numIslands([[]])).toBe(0);
  });

  test("all water returns 0", () => {
    expect(
      numIslands([
        [0, 0],
        [0, 0],
      ])
    ).toBe(0);
  });

  test("single island", () => {
    expect(
      numIslands([
        [1, 1],
        [1, 1],
      ])
    ).toBe(1);
  });

  test("multiple islands", () => {
    const grid = [
      [1, 1, 0, 0, 0],
      [1, 1, 0, 0, 0],
      [0, 0, 1, 0, 0],
      [0, 0, 0, 1, 1],
    ];
    expect(numIslands(grid)).toBe(3);
  });

  test("single cell islands", () => {
    const grid = [
      [1, 0, 1],
      [0, 1, 0],
      [1, 0, 1],
    ];
    expect(numIslands(grid)).toBe(5);
  });

  test("all land is one island", () => {
    const grid = [
      [1, 1, 1],
      [1, 1, 1],
      [1, 1, 1],
    ];
    expect(numIslands(grid)).toBe(1);
  });

  test("L-shaped island", () => {
    const grid = [
      [1, 0, 0],
      [1, 0, 0],
      [1, 1, 1],
    ];
    expect(numIslands(grid)).toBe(1);
  });
});

// ============================================================================
// Utility: findRedundantConnection
// ============================================================================
describe("findRedundantConnection", () => {
  test("simple cycle", () => {
    const edges: [number, number][] = [
      [1, 2],
      [1, 3],
      [2, 3],
    ];
    expect(findRedundantConnection(edges)).toEqual([2, 3]);
  });

  test("larger graph", () => {
    const edges: [number, number][] = [
      [1, 2],
      [2, 3],
      [3, 4],
      [1, 4],
      [1, 5],
    ];
    expect(findRedundantConnection(edges)).toEqual([1, 4]);
  });

  test("edge that completes the first cycle is returned", () => {
    const edges: [number, number][] = [
      [1, 2],
      [3, 4],
      [2, 3],
      [1, 4],
    ];
    // After edges 1-2, 3-4, 2-3: 1-2-3-4 are connected
    // Edge 1-4 makes the cycle
    expect(findRedundantConnection(edges)).toEqual([1, 4]);
  });
});
