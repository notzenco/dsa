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
| Rust shortest paths | `reconstruct_path(parent, source, target)` requires the actual source and returns `None` for invalid or cyclic parent chains. |
| Fenwick updates | Rust uses one-based indices; Go, Python, and TypeScript use zero-based indices. Invalid update indices are rejected before entering the update loop. |
| Python segment tree | Point updates outside `[0, len)` raise `IndexError`. |
| Java adjacency matrix | Positive infinity is reserved for a missing edge and is rejected as an edge weight. |
| Trie insertion | Reinserting an existing word does not increase unique size or prefix counts. |
| Python maps and trees | A stored `None` value is distinct from an absent key for membership and deletion. |

## Regression areas

- C allocation arithmetic, trie enumeration, search byte counters, graph path
  buffers, counting/radix sort, hash tombstones, and B-tree root changes.
- Graph algorithms: absent destinations, negative vertex labels, self-loops,
  parallel edges, object-valued vertices, and path reconstruction from a
  nonzero source.
- String algorithms: delimiters occurring in input, Unicode boundaries, and
  palindrome sentinels.
- Data structures: lazy segment-tree logical length, skip-list live length,
  Fenwick index validation, LFU frequency buckets, and panic-safe LRU clear.

## Verification

From each language directory, run the project suite:

```bash
(cd by-language/c && make -B test CFLAGS='-Wall -Wextra -std=c11 -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer -I.' LDFLAGS='-fsanitize=address,undefined')
(cd by-language/go && go test ./... && go vet ./...)
(cd by-language/java && ./gradlew test)
(cd by-language/python && python3 -m pytest -q -o addopts='')
(cd by-language/rust && cargo test --offline && cargo fmt --all -- --check && cargo check --offline --workspace --no-default-features)
(cd by-language/typescript && bun test)
```

The TypeScript project also has a strict `tsconfig.json`; run a local TypeScript
compiler with `--noEmit -p by-language/typescript/tsconfig.json` when one is
available. It requires the project's `bun-types` type declarations.

The C sanitizer command rebuilds objects with sanitizer flags. Run `make -B`
again when switching back to ordinary compiler or linker flags.
