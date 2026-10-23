use dsa_algorithms::{searching::*, sorting::*, string::*};
use rand::{rngs::StdRng, Rng, SeedableRng};

const SEED: u64 = 0xD5A_2026;

#[test]
fn sorts_match_slice_sort() {
    let mut rng = StdRng::seed_from_u64(SEED);
    let sorts: &[fn(&mut [i32])] = &[
        bubble_sort,
        selection_sort,
        insertion_sort,
        merge_sort,
        quick_sort,
        heap_sort,
        counting_sort_i32,
    ];
    for iteration in 0..200 {
        let input: Vec<i32> = (0..rng.gen_range(0..65))
            .map(|_| {
                if iteration % 2 == 0 {
                    rng.gen_range(-16..=16)
                } else {
                    rng.gen()
                }
            })
            .collect();
        let mut expected = input.clone();
        expected.sort();
        for (sort_index, sort) in sorts.iter().enumerate() {
            let mut actual = input.clone();
            sort(&mut actual);
            assert_eq!(
                actual, expected,
                "seed={SEED} iteration={iteration} sort={sort_index} input={input:?}"
            );
        }
        let input: Vec<u32> = input
            .iter()
            .map(|&v| match iteration % 3 {
                0 => v as u32,
                1 => v.unsigned_abs() % 32,
                _ => u32::MAX - v.unsigned_abs() % 32,
            })
            .collect();
        let mut expected = input.clone();
        expected.sort();
        for sort in [counting_sort, radix_sort] {
            let mut actual = input.clone();
            sort(&mut actual);
            assert_eq!(
                actual, expected,
                "seed={SEED} iteration={iteration} input={input:?}"
            );
        }
    }
}

#[test]
fn integer_sort_extremes() {
    for input in [
        vec![i32::MAX, i32::MIN, 0, -1],
        vec![i32::MIN + 1, i32::MIN],
        vec![i32::MAX, i32::MAX - 1],
        vec![-1000000, 1000000],
    ] {
        let mut expected = input.clone();
        expected.sort();
        let mut actual = input;
        counting_sort_i32(&mut actual);
        assert_eq!(actual, expected);
    }
    for input in [
        vec![u32::MAX, 0, 1, u32::MAX - 1],
        vec![u32::MAX, u32::MAX - 1],
        vec![1000000, 0],
        vec![u32::MAX; 3],
    ] {
        let mut expected = input.clone();
        expected.sort();
        for sort in [counting_sort, radix_sort] {
            let mut actual = input.clone();
            sort(&mut actual);
            assert_eq!(actual, expected, "input={input:?}");
        }
    }
}

#[test]
fn binary_search_matches_partition_points() {
    let mut rng = StdRng::seed_from_u64(SEED);
    for iteration in 0..300 {
        let mut input: Vec<i32> = (0..rng.gen_range(0..80))
            .map(|_| rng.gen_range(-20..=20))
            .collect();
        input.sort();
        for target in -21..=21 {
            let lower = input.partition_point(|&x| x < target);
            let upper = input.partition_point(|&x| x <= target);
            let context =
                format!("seed={SEED} iteration={iteration} input={input:?} target={target}");
            assert_eq!(lower_bound(&input, &target), lower, "{context}");
            assert_eq!(upper_bound(&input, &target), upper, "{context}");
            assert_eq!(
                count_occurrences(&input, &target),
                upper - lower,
                "{context}"
            );
            assert_eq!(
                binary_search_first(&input, &target),
                (lower < upper).then_some(lower),
                "{context}"
            );
            assert_eq!(
                binary_search_last(&input, &target),
                (lower < upper).then(|| upper - 1),
                "{context}"
            );
            let actual = binary_search(&input, &target);
            assert_eq!(actual.is_some(), lower < upper, "{context}");
            if let Some(index) = actual {
                assert!((lower..upper).contains(&index), "{context} index={index}");
            }
        }
    }
}

#[test]
fn string_search_matches_naive_search() {
    let mut rng = StdRng::seed_from_u64(SEED);
    for iteration in 0..500 {
        let text: String = (0..rng.gen_range(0..60))
            .map(|_| ['a', 'b', 'c', '\0'][rng.gen_range(0..4)])
            .collect();
        let pattern: String = (0..rng.gen_range(0..12))
            .map(|_| ['a', 'b', 'c', '\0'][rng.gen_range(0..4)])
            .collect();
        let expected: Vec<usize> = if pattern.is_empty() {
            vec![]
        } else {
            text.as_bytes()
                .windows(pattern.len())
                .enumerate()
                .filter_map(|(i, window)| (window == pattern.as_bytes()).then_some(i))
                .collect()
        };
        for search in [kmp_search, rabin_karp, z_search] {
            assert_eq!(
                search(&text, &pattern),
                expected,
                "seed={SEED} iteration={iteration} text={text:?} pattern={pattern:?}"
            );
        }
    }
}
