"""Small, reproducible model checks using only standard-library references."""

from bisect import bisect_left, bisect_right
from collections import OrderedDict, deque
from functools import lru_cache
import heapq
from itertools import combinations, product
import random

import pytest

from algorithms import dynamic_programming as dp, searching, sorting
from algorithms.string_algorithms import (
    kmp_search, naive_search, rabin_karp_search, z_algorithm_search,
)
from data_structures.avl_tree import AVLTreeMap
from data_structures.b_tree import BTree
from data_structures.binary_search_tree import BinarySearchTree
from data_structures.fenwick_tree import FenwickTree
from data_structures.hash_table import HashMap, HashTableChaining, HashTableOpenAddressing
from data_structures.heap import MaxHeap, MinHeap
from data_structures.lfu_cache import LFUCache
from data_structures.lru_cache import LRUCache
from data_structures.red_black_tree import RedBlackTree
from data_structures.segment_tree import SegmentTree, SegmentTreeLazy
from data_structures.skip_list import SkipList
from data_structures.trie import Trie
from data_structures.union_find import UnionFind, UnionFindArray


SEED = 20260417


def context(iteration, **inputs):
    return f"seed={SEED}, iteration={iteration}, input={inputs!r}"


@pytest.mark.parametrize('kind', ['red_black', 'b_tree_2', 'b_tree_3', 'skip', 'avl', 'bst'])
def test_ordered_maps(kind, monkeypatch):
    rng = random.Random(SEED)
    # Skip-list height generation must also be reproducible, without changing
    # the process-wide random generator or leaking state into other tests.
    monkeypatch.setattr('data_structures.skip_list.random', random.Random(SEED + 1))
    factories = {
        'red_black': RedBlackTree,
        'b_tree_2': lambda: BTree(t=2),
        'b_tree_3': lambda: BTree(t=3),
        'skip': SkipList,
        'avl': AVLTreeMap,
        'bst': BinarySearchTree,
    }
    tree = factories[kind]()
    model = {}
    for iteration in range(220):
        key, value = rng.randrange(-25, 26), rng.choice([None, rng.randrange(-100, 101)])
        op = rng.choice(['put', 'put', 'delete', 'search'])
        message = context(iteration, kind=kind, op=op, key=key, value=value, model=model)
        if op == 'put':
            if kind == 'bst':
                old = tree.search(key)
                node = tree.insert(key)
                assert node.value == key, message
                if old is not None:
                    assert node is old, message
                model[key] = key
            else:
                tree[key] = value
                model[key] = value
        elif op == 'delete':
            if kind == 'avl':
                if key in model:
                    del tree[key]
                else:
                    with pytest.raises(KeyError):
                        del tree[key]
            else:
                assert tree.delete(key) == (key in model), message
            model.pop(key, None)
        else:
            assert (key in tree) == (key in model), message
        keys = sorted(model)
        assert list(tree) == keys, message
        assert len(tree) == len(model), message
        if kind == 'bst':
            assert tree.is_valid_bst(), message
        else:
            assert list(tree.items()) == [(k, model[k]) for k in keys], message
            for k in keys:
                assert tree[k] == model[k], message
        if kind == 'red_black':
            assert tree.is_valid(), message
        if hasattr(tree, 'floor'):
            left, right = bisect_left(keys, key), bisect_right(keys, key)
            assert tree.floor(key) == (keys[right - 1] if right else None), message
            assert tree.ceiling(key) == (keys[left] if left < len(keys) else None), message
    # Exercise deletion fixups after a nontrivial tree, not just isolated leaves.
    remaining = list(model)
    rng.shuffle(remaining)
    for iteration, key in enumerate(remaining, start=220):
        message = context(iteration, kind=kind, delete=key, model=model)
        if kind == 'avl':
            del tree[key]
        else:
            assert tree.delete(key), message
        del model[key]
        assert list(tree) == sorted(model), message
        assert len(tree) == len(model), message
        if kind == 'red_black':
            assert tree.is_valid(), message


@pytest.mark.parametrize('factory', [HashMap, HashTableChaining, HashTableOpenAddressing])
def test_hash_tables(factory):
    rng = random.Random(SEED)
    table, model = factory(capacity=4), {}
    for iteration in range(300):
        # Multiples of the initial capacity exercise collisions and tombstones.
        key, value = rng.randrange(-30, 31) * 4, rng.choice([None, rng.randrange(-100, 101)])
        op = rng.choice(['put', 'put', 'remove', 'get'])
        message = context(iteration, table=factory.__name__, op=op, key=key, value=value, model=model)
        if op == 'put':
            table[key] = value
            model[key] = value
        elif op == 'remove':
            assert table.remove(key) == (key in model), message
            model.pop(key, None)
        else:
            assert table.get(key, 'missing') == model.get(key, 'missing'), message
        assert len(table) == len(model), message
        assert dict(table.items()) == model, message
        assert (key in table) == (key in model), message


@pytest.mark.parametrize('factory,sign', [(MinHeap, 1), (MaxHeap, -1)])
def test_heaps(factory, sign):
    rng = random.Random(SEED)
    heap, model = factory(), []
    for iteration in range(300):
        value = rng.randrange(-100, 101)
        op = rng.choice(['push', 'push', 'pop', 'peek']) if model else 'push'
        message = context(iteration, heap=factory.__name__, op=op, value=value, model=model)
        if op == 'push':
            heap.push(value)
            heapq.heappush(model, sign * value)
        elif op == 'pop':
            assert heap.pop() == sign * heapq.heappop(model), message
        else:
            assert heap.peek() == sign * model[0], message
        assert len(heap) == len(model), message
    message = context(300, heap=factory.__name__, model=model)
    assert [heap.pop() for _ in range(len(heap))] == [sign * heapq.heappop(model) for _ in range(len(model))], message


def test_lru_cache():
    rng = random.Random(SEED)
    cache, model = LRUCache(4), OrderedDict()
    for iteration in range(300):
        key, value = rng.randrange(9), rng.choice([None, rng.randrange(100)])
        op = rng.choice(['put', 'get', 'delete', 'peek'])
        message = context(iteration, op=op, key=key, value=value, model=list(model.items()))
        if op == 'put':
            evicted = None
            if key in model:
                del model[key]
            elif len(model) == 4:
                evicted = model.popitem(last=False)
            model[key] = value
            assert cache.put(key, value) == evicted, message
        elif op == 'get':
            assert cache.get(key) == model.get(key), message
            if key in model:
                model.move_to_end(key)
        elif op == 'delete':
            assert cache.delete(key) == model.pop(key, None), message
        else:
            assert cache.peek(key) == model.get(key), message
        assert len(cache) == len(model), message
        assert list(cache.items()) == list(reversed(model.items())), message


def test_lfu_cache():
    rng = random.Random(SEED)
    cache, model = LFUCache(4), {}
    for iteration in range(300):
        key, value = rng.randrange(9), rng.choice([None, rng.randrange(100)])
        op = rng.choice(['put', 'put', 'get', 'delete', 'peek'])
        message = context(iteration, op=op, key=key, value=value, model=model)
        if op == 'put':
            evicted = None
            if key in model:
                model[key] = (value, model[key][1] + 1, iteration)
            else:
                if len(model) == 4:
                    victim = min(model, key=lambda k: model[k][1:])
                    evicted = (victim, model.pop(victim)[0])
                model[key] = (value, 1, iteration)
            assert cache.put(key, value) == evicted, message
        elif op == 'get':
            assert cache.get(key) == (model[key][0] if key in model else None), message
            if key in model:
                old_value, frequency, _ = model[key]
                model[key] = (old_value, frequency + 1, iteration)
        elif op == 'delete':
            assert cache.delete(key) == (key in model), message
            model.pop(key, None)
        else:
            assert cache.peek(key) == (model[key][0] if key in model else None), message
        assert len(cache) == len(model), message
        assert cache.min_frequency() == min((v[1] for v in model.values()), default=0), message
        for k in range(9):
            assert (k in cache) == (k in model), message
            assert cache.get_frequency(k) == (model[k][1] if k in model else -1), message
            assert cache.peek(k) == (model[k][0] if k in model else None), message


@pytest.mark.parametrize('size', [1, 7, 20])
def test_range_sum_structures(size):
    rng = random.Random(SEED)
    model = [rng.randrange(-20, 21) for _ in range(size)]
    segment, lazy, fenwick = SegmentTree(model), SegmentTreeLazy(model), FenwickTree(model)
    for iteration in range(150):
        left, right = sorted([rng.randrange(size), rng.randrange(size)])
        value = rng.randrange(-20, 21)
        op = rng.choice(['set', 'add', 'range_add', 'query'])
        message = context(iteration, op=op, left=left, right=right, value=value, model=model)
        if op == 'set':
            lazy.range_update(left, left, value - model[left])
            segment.update(left, value)
            fenwick.set(left, value)
            model[left] = value
        elif op in ('add', 'range_add'):
            end = right if op == 'range_add' else left
            lazy.range_update(left, end, value)
            for i in range(left, end + 1):
                model[i] += value
                segment.update(i, model[i])
                fenwick.update(i, value)
        expected = sum(model[left:right + 1])
        assert segment.query(left, right) == expected, message
        assert lazy.query(left, right) == expected, message
        assert fenwick.range_sum(left, right) == expected, message
        assert fenwick.prefix_sum(right) == sum(model[:right + 1]), message
        assert lazy.query(0, size - 1) == sum(model), message


SORTS = [sorting.bubble_sort, sorting.selection_sort, sorting.insertion_sort,
         sorting.merge_sort, sorting.quick_sort, sorting.heap_sort, sorting.shell_sort,
         sorting.counting_sort, sorting.radix_sort, sorting.bucket_sort]


@pytest.mark.parametrize('sort', SORTS, ids=lambda fn: fn.__name__)
def test_sorting(sort):
    rng = random.Random(SEED)
    cases = [[], [0], [-5, -5, 0, 3], list(range(-10, 11)), list(range(10, -11, -1))]
    cases += [[rng.randrange(-50, 51) for _ in range(rng.randrange(35))] for _ in range(50)]
    for iteration, values in enumerate(cases):
        assert sort(values.copy()) == sorted(values), context(iteration, sort=sort.__name__, values=values)


def test_comparator_sort():
    rng = random.Random(SEED)
    for iteration in range(50):
        values = [rng.randrange(-50, 51) for _ in range(rng.randrange(35))]
        message = context(iteration, values=values)
        assert sorting.sort_with_comparator(values, lambda a, b: a - b) == sorted(values), message
        assert sorting.sort_with_comparator(values, lambda a, b: b - a) == sorted(values, reverse=True), message


def test_binary_search_variants():
    rng = random.Random(SEED)
    for iteration in range(150):
        values = sorted(rng.randrange(-20, 21) for _ in range(rng.randrange(50)))
        target = rng.randrange(-25, 26)
        message = context(iteration, values=values, target=target)
        left, right = bisect_left(values, target), bisect_right(values, target)
        assert searching.lower_bound(values, target) == left, message
        assert searching.upper_bound(values, target) == right, message
        assert searching.find_first_occurrence(values, target) == (left if left != right else -1), message
        assert searching.find_last_occurrence(values, target) == (right - 1 if left != right else -1), message
        for search in [searching.binary_search, searching.binary_search_recursive]:
            index = search(values, target)
            assert (left <= index < right) if left != right else index == -1, message


@pytest.mark.parametrize('search', [kmp_search, rabin_karp_search, z_algorithm_search, naive_search], ids=lambda fn: fn.__name__)
def test_string_search(search):
    rng = random.Random(SEED)
    cases = [('', ''), ('abc', ''), ('', 'a'), ('aaaaa', 'aaa'), ('a$b$a$b', '$b')]
    for _ in range(100):
        text = ''.join(rng.choices('ab$c', k=rng.randrange(35)))
        pattern = ''.join(rng.choices('ab$c', k=rng.randrange(8)))
        cases.append((text, pattern))
    for iteration, (text, pattern) in enumerate(cases):
        # Preserve the existing empty-pattern contracts: KMP returns [0]
        # for nonempty text, while the other all-match APIs return [].
        expected = [i for i in range(len(text) - len(pattern) + 1)
                    if text[i:i + len(pattern)] == pattern] if pattern else []
        if search is kmp_search and not pattern and text:
            expected = [0]
        assert search(text, pattern) == expected, context(iteration, search=search.__name__, text=text, pattern=pattern)


def brute_edit_distance(a, b):
    @lru_cache(None)
    def solve(i, j):
        if i == len(a):
            return len(b) - j
        if j == len(b):
            return len(a) - i
        if a[i] == b[j]:
            return solve(i + 1, j + 1)
        return 1 + min(solve(i + 1, j), solve(i, j + 1), solve(i + 1, j + 1))
    return solve(0, 0)


def test_edit_distance():
    rng = random.Random(SEED)
    for iteration in range(80):
        a = ''.join(rng.choices('abc', k=rng.randrange(6)))
        b = ''.join(rng.choices('abc', k=rng.randrange(6)))
        expected = brute_edit_distance(a, b)
        message = context(iteration, a=a, b=b)
        assert dp.edit_distance(a, b) == expected, message
        assert dp.edit_distance_optimized(a, b) == expected, message


def test_lis():
    rng = random.Random(SEED)
    for iteration in range(60):
        values = [rng.randrange(-4, 5) for _ in range(rng.randrange(9))]
        expected = max((len(subseq) for size in range(len(values) + 1)
                        for subseq in combinations(values, size)
                        if all(a < b for a, b in zip(subseq, subseq[1:]))), default=0)
        message = context(iteration, values=values)
        assert dp.lis_length(values) == expected, message
        assert dp.lis_binary_search(values) == expected, message
        sequence = dp.lis_sequence(values)
        assert len(sequence) == expected, message
        assert all(a < b for a, b in zip(sequence, sequence[1:])), message
        remaining = iter(values)
        assert all(any(item == wanted for item in remaining) for wanted in sequence), message


def test_coin_change():
    rng = random.Random(SEED)
    for iteration in range(60):
        coins = sorted(rng.sample(range(1, 7), rng.randrange(1, 5)))
        amount = rng.randrange(15)
        # Enumerate counts for unordered combinations; BFS is an independent
        # shortest-path reference for the minimum number of coins.
        ways = sum(sum(c * n for c, n in zip(coins, counts)) == amount
                   for counts in product(*(range(amount // c + 1) for c in coins)))
        queue, seen, minimum = deque([(0, 0)]), {0}, -1
        while queue:
            total, count = queue.popleft()
            if total == amount:
                minimum = count
                break
            for coin in coins:
                if total + coin <= amount and total + coin not in seen:
                    seen.add(total + coin)
                    queue.append((total + coin, count + 1))
        message = context(iteration, coins=coins, amount=amount)
        assert dp.coin_change_min(coins, amount) == minimum, message
        assert dp.coin_change_ways(coins, amount) == ways, message


def test_knapsack():
    rng = random.Random(SEED)
    for iteration in range(70):
        size, capacity = rng.randrange(8), rng.randrange(16)
        weights = [rng.randrange(1, 8) for _ in range(size)]
        values = [rng.randrange(15) for _ in range(size)]
        expected = max(sum(v for i, v in enumerate(values) if mask & (1 << i))
                       for mask in range(1 << size)
                       if sum(w for i, w in enumerate(weights) if mask & (1 << i)) <= capacity)
        message = context(iteration, weights=weights, values=values, capacity=capacity)
        assert dp.knapsack_01(weights, values, capacity) == expected, message
        assert dp.knapsack_01_optimized(weights, values, capacity) == expected, message


@pytest.mark.parametrize('factory', [UnionFind, UnionFindArray])
def test_union_find(factory):
    rng = random.Random(SEED)
    size = 16
    if factory is UnionFind:
        uf = factory()
        for i in range(size):
            uf.make_set(i)
    else:
        uf = factory(size)
    labels = list(range(size))
    for iteration in range(120):
        a, b = rng.randrange(size), rng.randrange(size)
        message = context(iteration, a=a, b=b, labels=labels)
        assert uf.union(a, b) == (labels[a] != labels[b]), message
        old, new = labels[b], labels[a]
        labels = [new if label == old else label for label in labels]
        assert uf.num_components == len(set(labels)), message
        for x in range(size):
            assert uf.set_size(x) == labels.count(labels[x]), message
            for y in range(size):
                assert uf.connected(x, y) == (labels[x] == labels[y]), message


def test_trie_prefix_counts():
    rng = random.Random(SEED)
    trie, model = Trie(), set()
    words = ['', 'a', 'ab', 'abc', 'b', 'ba', 'c']
    words += [''.join(rng.choices('abc', k=rng.randrange(1, 6))) for _ in range(40)]
    for iteration in range(200):
        word = rng.choice(words)
        op = rng.choice(['insert', 'insert', 'delete', 'search'])
        message = context(iteration, op=op, word=word, model=sorted(model))
        if op == 'insert':
            trie.insert(word)
            model.add(word)
        elif op == 'delete':
            assert trie.delete(word) == (word in model), message
            model.discard(word)
        else:
            assert trie.search(word) == (word in model), message
        assert len(trie) == len(model), message
        assert set(trie) == model, message
        for prefix in ['', 'a', 'ab', 'abc', 'b', word[:2], 'z']:
            matches = {w for w in model if w.startswith(prefix)}
            prefix_message = context(iteration, op=op, word=word, prefix=prefix, model=sorted(model))
            assert trie.count_words_with_prefix(prefix) == len(matches), prefix_message
            assert set(trie.get_words_with_prefix(prefix)) == matches, prefix_message
