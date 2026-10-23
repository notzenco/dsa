/** Mulberry32. `next()` returns a float in [0, 1). */
export function mulberry32(seed: number): () => number {
  let state = seed >>> 0;
  return () => {
    state = (state + 0x6d2b79f5) >>> 0;
    let t = Math.imul(state ^ (state >>> 15), 1 | state);
    t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

/** Inclusive integer in [lo, hi]. */
export function randInt(next: () => number, lo: number, hi: number): number {
  return lo + Math.floor(next() * (hi - lo + 1));
}
