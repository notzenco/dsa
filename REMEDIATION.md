# Audit remediation

The repository-wide audit identified memory-safety, correctness, and API
boundary cases across the six language implementations. The accompanying
regression tests cover the confirmed cases. This file records behavior that
callers and future maintainers need to preserve; it is not a claim that every
possible input or deployment has been audited.

## Caller-visible changes

| Area | Current contract |
|------|------------------|
| C shortest paths | `shortest_path_reconstruct` takes `path_capacity` in elements and returns `false` for an insufficient buffer, invalid destination, unreachable path, or negative-cycle result. |
| C segment tree | Both constructors return `NULL` when `4 * n` integer slots cannot fit in `size_t`; callers must check the result before querying or updating. |
| C B-tree | `btree_create` returns `NULL` when the minimum degree is below 2 or too large for signed arithmetic or node allocations. |
| Rust LRU cache | Eviction removes the map entry by node identity before freeing the node, including for keys whose clones compare differently. The map is a `BTreeMap`; eviction scans it, so `put` is O(n) in the worst case. |
| Rust shortest paths | `reconstruct_path(parent, source, target)` requires the actual source and returns `None` for invalid or cyclic parent chains. |
| Fenwick updates | Rust uses one-based indices; Go, Python, and TypeScript use zero-based indices. Invalid update indices are rejected before entering the update loop. |
| Python segment tree | Point updates outside `[0, len)` raise `IndexError`. |
| Java adjacency matrix | Positive infinity is reserved for a missing edge and is rejected as an edge weight. |
| Trie insertion | Reinserting an existing word does not increase unique size or prefix counts. |
| Python maps and trees | A stored `None` value is distinct from an absent key for membership and deletion. `AVLTree.insert(key, None)` stores `None`; omitting the value still stores the key. |
| Python LFU cache | `data_structures.lru_cache.LFUCache` is now a re-export of `data_structures.lfu_cache.LFUCache`, so `put()` returns the evicted `(key, value)` pair instead of `None`. `min_frequency()` stays correct after deleting the last entry at the minimum frequency; that delete is O(n) in the worst case. |
| Binary search trees | Keys are unique in every language. Inserting a value that is already present leaves the size and shape unchanged: Python, Java, TypeScript, and Go return the existing node, Rust returns `false`, and C (a key-to-value map) replaces the stored value. Python, Java, and TypeScript previously stored duplicates, which made their own validity checks fail. The sorted-input builders (Python `from_sorted_list`, Java `fromSortedList`, TypeScript `fromSortedArray`, Go `BSTFromSortedSlice`) keep repeated values once. |
| Radix sort | C, Go, Java, and TypeScript accept negative values and the full range of their integer type; digits come from each value's distance to the minimum. Python already handled negatives, and Rust takes `u32`. Before this change, negative input overflowed a stack buffer in C, panicked in Go, threw in Java, and returned wrong output in TypeScript. |
| Counting sort | C, Go, Java, Rust, and TypeScript accept the full range of their integer type and fall back to a comparison sort when the value range is more than 16 times the input length, instead of allocating the whole range. Java previously threw on negative values, and Go and Rust could overflow while computing the range. Python's version allocates the whole range, as its docstring states. |
| Go B-tree | Inserting a key that already lives in an internal node updates its value. Previously the duplicate check looked at the wrong slot, so the key was inserted a second time. |
| Go interpolation search | `InterpolationSearch` accepts the full `int` range and repeated endpoint values. The probe is computed from unsigned distances instead of a `float64` ratio that could overflow or divide by zero. |
| Go string search | `KMPSearch`, `RabinKarpSearch`, and `ZAlgorithmSearch` all return no matches for an empty pattern. `RabinKarpSearch` previously returned `[0]` and `ZAlgorithmSearch` returned every index except the last. |
| TypeScript sorts | `countingSort` and `radixSort` throw `RangeError` for values that are not safe integers instead of returning a wrongly ordered array. |

## Regression areas

- C allocation arithmetic, including segment-tree sizes and B-tree degree,
  trie enumeration, search byte counters, graph path buffers, counting/radix
  sort, hash tombstones, and B-tree root changes.
- Graph algorithms: absent destinations, negative vertex labels, self-loops,
  parallel edges, object-valued vertices, and path reconstruction from a
  nonzero source.
- String algorithms: delimiters occurring in input, Unicode boundaries, and
  palindrome sentinels.
- Data structures: lazy segment-tree logical length, skip-list live length,
  Fenwick index validation, LFU frequency buckets, panic-safe LRU clear, and
  node-identity eviction for non-equivalent Rust key clones.
- Sorting: negative values, all-negative input, and integer-type extremes for
  every sort, including the counting-sort comparison fallback.
- Randomized model tests in every language compare sorts, searches, string
  matching, ordered trees, caches, and range trees against simple reference
  implementations. Each uses a fixed seed and reports the seed, iteration,
  and input on failure. Go also has native fuzz targets whose seed corpus runs
  with `go test`.

## Verification

From each language directory, run the project suite:

```bash
(cd by-language/c && make -B test CFLAGS='-Wall -Wextra -std=c11 -g -O1 -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer -I.' LDFLAGS='-fsanitize=address,undefined')
(cd by-language/go && test -z "$(gofmt -l .)" && go vet ./... && go test -race ./...)
(cd by-language/java && ./gradlew test)
(cd by-language/python && python3 -W error::SyntaxWarning -m compileall -q -f data_structures algorithms tests && python3 -m pytest -q -o addopts='')
(cd by-language/rust && cargo test --offline && cargo fmt --all -- --check && cargo clippy --offline --workspace --all-targets --all-features -- -D warnings && cargo check --offline --workspace --no-default-features && RUSTDOCFLAGS='-D warnings' cargo doc --offline --workspace --no-deps)
(cd by-language/typescript && bun install --frozen-lockfile && bun run typecheck && bun test)
```

CI runs the same checks, plus the Rust library crates on the declared minimum
version (`cargo +1.70.0 check -p dsa-core -p dsa-data-structures -p dsa-algorithms --lib`)
and Go on both 1.21 and the current stable release.

The Python suite treats warnings as errors (`filterwarnings = ["error"]` in
`pyproject.toml`), so an invalid escape sequence in a docstring fails it.

The C Makefile tracks header dependencies, so editing a header rebuilds the
objects that include it. Changing `CFLAGS` or `LDFLAGS` is not tracked: the
sanitizer command uses `make -B`, and switching back to ordinary flags needs
`make -B` again.
