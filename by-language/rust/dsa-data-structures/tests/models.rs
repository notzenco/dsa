use dsa_data_structures::{
    advanced::SkipList,
    caches::{LFUCache, LRUCache},
    graphs::UnionFind,
    trees::{AVLTree, BTree, BinarySearchTree, RedBlackTree},
};
use rand::{rngs::StdRng, Rng, SeedableRng};
use std::collections::{BTreeMap, BTreeSet};

const SEED: u64 = 0xD5A_2026;

#[test]
fn ordered_sets_match_btree_set() {
    let mut rng = StdRng::seed_from_u64(SEED);
    let mut bst = BinarySearchTree::new();
    let mut avl = AVLTree::new();
    let mut skip = SkipList::new();
    let mut expected = BTreeSet::new();
    let mut operations = Vec::new();
    for iteration in 0..1000 {
        let key = rng.gen_range(-40..=40);
        let insert = rng.gen_bool(0.55);
        operations.push((insert, key));
        let context = format!("seed={SEED} iteration={iteration} operations={operations:?}");
        if insert {
            let inserted = expected.insert(key);
            bst.insert(key);
            avl.insert(key);
            assert_eq!(skip.insert(key), inserted, "{context}");
        } else {
            let removed = expected.remove(&key);
            assert_eq!(bst.remove(&key), removed, "{context}");
            assert_eq!(avl.remove(&key), removed, "{context}");
            assert_eq!(skip.remove(&key), removed, "{context}");
        }
        let sorted: Vec<_> = expected.iter().copied().collect();
        assert_eq!(
            bst.inorder().copied().collect::<Vec<_>>(),
            sorted,
            "{context}"
        );
        assert_eq!(
            avl.inorder().copied().collect::<Vec<_>>(),
            sorted,
            "{context}"
        );
        assert_eq!(
            skip.iter().copied().collect::<Vec<_>>(),
            sorted,
            "{context}"
        );
        assert_eq!(
            (bst.len(), avl.len(), skip.len()),
            (expected.len(), expected.len(), expected.len()),
            "{context}"
        );
        assert!(
            bst.is_valid() && avl.is_valid() && avl.is_balanced(),
            "{context}"
        );
        for key in -41..=41 {
            let present = expected.contains(&key);
            assert_eq!(
                (bst.contains(&key), avl.contains(&key), skip.contains(&key)),
                (present, present, present),
                "{context} query={key}"
            );
        }
    }
}

#[test]
fn insert_only_trees_match_models() {
    let mut rng = StdRng::seed_from_u64(SEED);
    for degree in 2..=5 {
        let mut rb = RedBlackTree::new();
        let mut btree = BTree::new(degree);
        let mut set = BTreeSet::new();
        let mut counts = BTreeMap::<i32, usize>::new();
        let mut input = Vec::new();
        for iteration in 0..250 {
            let key = rng.gen_range(-40..=40);
            input.push(key);
            let context =
                format!("seed={SEED} iteration={iteration} degree={degree} input={input:?}");
            assert_eq!(rb.insert(key), set.insert(key), "{context}");
            btree.insert(key);
            *counts.entry(key).or_default() += 1;
            let expected: Vec<_> = counts
                .iter()
                .flat_map(|(&key, &count)| std::iter::repeat(key).take(count))
                .collect();
            assert_eq!(
                btree.iter().copied().collect::<Vec<_>>(),
                expected,
                "{context}"
            );
            assert_eq!(
                rb.iter().copied().collect::<Vec<_>>(),
                set.iter().copied().collect::<Vec<_>>(),
                "{context}"
            );
            assert_eq!(
                (btree.len(), rb.len()),
                (input.len(), set.len()),
                "{context}"
            );
            for query in -41..=41 {
                let present = set.contains(&query);
                assert_eq!(
                    (rb.contains(&query), btree.contains(&query)),
                    (present, present),
                    "{context} query={query}"
                );
            }
        }
    }
}

#[test]
fn bst_duplicate_preserves_shape() {
    let mut tree = BinarySearchTree::from_slice(&[5, 3, 7, 1, 4, 6, 9]);
    let preorder = tree.preorder().copied().collect::<Vec<_>>();
    let height = tree.height();
    for key in [5, 3, 7, 1, 4, 6, 9] {
        tree.insert(key);
        assert_eq!(tree.len(), 7);
        assert_eq!(tree.height(), height);
        assert_eq!(tree.preorder().copied().collect::<Vec<_>>(), preorder);
    }
}

#[test]
fn caches_match_reference_models() {
    let mut rng = StdRng::seed_from_u64(SEED);
    for capacity in 1..=5 {
        let mut lru = LRUCache::new(capacity);
        let mut lfu = LFUCache::new(capacity);
        // Value, access frequency, last access time.
        let mut lru_model = BTreeMap::<i32, (i32, usize, usize)>::new();
        let mut lfu_model = lru_model.clone();
        let mut operations = Vec::new();
        for iteration in 0..1000 {
            let key = rng.gen_range(0..9);
            let value = rng.gen_range(-100..=100);
            let operation = rng.gen_range(0..5);
            operations.push((operation, key, value));
            let context = format!(
                "seed={SEED} iteration={iteration} capacity={capacity} operations={operations:?}"
            );
            match operation {
                0 | 1 => {
                    let evicted =
                        reference_put(&mut lru_model, capacity, key, value, iteration, false);
                    reference_put(&mut lfu_model, capacity, key, value, iteration, true);
                    assert_eq!(lru.put(key, value), evicted, "{context}");
                    lfu.put(key, value);
                }
                2 | 3 => {
                    let expected_lru = reference_get(&mut lru_model, key, iteration);
                    let expected_lfu = reference_get(&mut lfu_model, key, iteration);
                    if operation == 2 {
                        assert_eq!(lru.get(&key).copied(), expected_lru, "{context}");
                        assert_eq!(lfu.get(&key).copied(), expected_lfu, "{context}");
                    } else {
                        assert_eq!(
                            lru.get_mut(&key).map(|v| {
                                *v += 1;
                                *v
                            }),
                            expected_lru.map(|v| v + 1),
                            "{context}"
                        );
                        assert_eq!(
                            lfu.get_mut(&key).map(|v| {
                                *v += 1;
                                *v
                            }),
                            expected_lfu.map(|v| v + 1),
                            "{context}"
                        );
                        if let Some(entry) = lru_model.get_mut(&key) {
                            entry.0 += 1;
                        }
                        if let Some(entry) = lfu_model.get_mut(&key) {
                            entry.0 += 1;
                        }
                    }
                }
                _ => {
                    assert_eq!(
                        lru.remove(&key),
                        lru_model.remove(&key).map(|entry| entry.0),
                        "{context}"
                    );
                    assert_eq!(
                        lfu.remove(&key),
                        lfu_model.remove(&key).map(|entry| entry.0),
                        "{context}"
                    );
                }
            }
            assert_eq!(
                (lru.len(), lfu.len()),
                (lru_model.len(), lfu_model.len()),
                "{context}"
            );
            for query in 0..9 {
                assert_eq!(
                    lru.peek(&query).copied(),
                    lru_model.get(&query).map(|entry| entry.0),
                    "{context} query={query}"
                );
                assert_eq!(
                    lfu.contains(&query),
                    lfu_model.contains_key(&query),
                    "{context} query={query}"
                );
                assert_eq!(
                    lfu.frequency(&query),
                    lfu_model.get(&query).map(|entry| entry.1),
                    "{context} query={query}"
                );
            }
        }
    }
}

fn reference_get(
    model: &mut BTreeMap<i32, (i32, usize, usize)>,
    key: i32,
    time: usize,
) -> Option<i32> {
    model.get_mut(&key).map(|entry| {
        entry.1 += 1;
        entry.2 = time;
        entry.0
    })
}

fn reference_put(
    model: &mut BTreeMap<i32, (i32, usize, usize)>,
    capacity: usize,
    key: i32,
    value: i32,
    time: usize,
    lfu: bool,
) -> Option<(i32, i32)> {
    if let Some(entry) = model.get_mut(&key) {
        *entry = (value, entry.1 + 1, time);
        return None;
    }
    let evicted = if model.len() == capacity {
        let victim = *model
            .iter()
            .min_by_key(|(_, entry)| (if lfu { entry.1 } else { 0 }, entry.2))
            .unwrap()
            .0;
        Some((victim, model.remove(&victim).unwrap().0))
    } else {
        None
    };
    model.insert(key, (value, 1, time));
    evicted
}

#[test]
fn union_find_matches_component_labels() {
    let mut rng = StdRng::seed_from_u64(SEED);
    for size in [1, 2, 8, 32] {
        let mut uf = UnionFind::new(size);
        let mut labels: Vec<_> = (0..size).collect();
        let mut input = Vec::new();
        for iteration in 0..250 {
            let a = rng.gen_range(0..size);
            let b = rng.gen_range(0..size);
            input.push((a, b));
            let context = format!("seed={SEED} iteration={iteration} size={size} input={input:?}");
            assert_eq!(uf.union(a, b), labels[a] != labels[b], "{context}");
            let old = labels[b];
            let new = labels[a];
            for label in &mut labels {
                if *label == old {
                    *label = new;
                }
            }
            assert_eq!(
                uf.count(),
                labels.iter().collect::<BTreeSet<_>>().len(),
                "{context}"
            );
            for x in 0..size {
                assert_eq!(
                    uf.set_size(x),
                    labels.iter().filter(|&&label| label == labels[x]).count(),
                    "{context} x={x}"
                );
                for y in 0..size {
                    assert_eq!(
                        uf.connected(x, y),
                        labels[x] == labels[y],
                        "{context} x={x} y={y}"
                    );
                    assert_eq!(
                        uf.find_immutable(x) == uf.find_immutable(y),
                        labels[x] == labels[y],
                        "{context} x={x} y={y}"
                    );
                }
            }
        }
    }
}
