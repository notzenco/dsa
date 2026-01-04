//! Benchmarks for algorithms.
//!
//! Run with: `cargo bench --bench algorithms`

use criterion::{criterion_group, criterion_main, Criterion};

fn placeholder_bench(_c: &mut Criterion) {
    // TODO: Add benchmarks as algorithms are implemented
}

criterion_group!(benches, placeholder_bench);
criterion_main!(benches);
