"""Focused tests for the public GraphCore operations and edge cases."""

import pytest

from data_structures.graph_core import GraphCore


@pytest.mark.parametrize('directed', [False, True])
def test_missing_vertices_and_edges_do_not_mutate_graph(directed):
    graph = GraphCore(directed=directed)
    graph.add_vertex('isolated')
    assert not graph.remove_vertex('missing')
    assert not graph.remove_edge('missing', 'isolated')
    assert not graph.remove_edge('isolated', 'missing')
    assert not graph.has_edge('missing', 'isolated')
    assert graph.get_weight('missing', 'isolated') is None
    assert graph.get_weight('isolated', 'missing') is None
    assert graph.neighbors('missing') == []
    assert graph.degree('missing') == 0
    assert graph.in_degree('missing') == 0
    assert list(graph) == ['isolated']
    assert len(graph) == 1
    assert graph.edge_count == 0
    assert graph.is_directed == directed
    kind = 'Directed' if directed else 'Undirected'
    assert repr(graph) == f'{kind}Graph(vertices=1, edges=0)'


@pytest.mark.parametrize('directed', [False, True])
def test_in_degree_and_vertex_removal(directed):
    graph = GraphCore(directed=directed)
    graph.add_edge('a', 'b', 2)
    graph.add_edge('c', 'b', 3)
    graph.add_edge('b', 'b', 4)
    graph.add_vertex('isolated')
    assert graph.in_degree('b') == 3
    assert graph.degree('b') == (1 if directed else 3)
    assert graph.edge_count == 3
    assert graph.remove_vertex('b')
    assert set(graph.vertices) == {'a', 'c', 'isolated'}
    assert graph.edge_count == 0
    assert all(graph.neighbors(v) == [] for v in graph)
    assert not graph.remove_vertex('b')


@pytest.mark.parametrize('directed', [False, True])
def test_copy_preserves_weights_loops_isolated_vertices_and_independence(directed):
    graph = GraphCore(directed=directed)
    graph.add_vertex('isolated')
    graph.add_edge('a', 'b', 2.5)
    graph.add_edge('b', 'b', 7)
    if directed:
        graph.add_edge('b', 'a', 3.5)
    clone = graph.copy()
    assert clone is not graph
    assert clone.is_directed == directed
    assert set(clone) == set(graph)
    assert clone.edge_count == graph.edge_count
    for u in graph:
        for v in graph:
            assert clone.get_weight(u, v) == graph.get_weight(u, v)
    clone.add_edge('a', 'b', 99)
    assert graph.get_weight('a', 'b') == 2.5
    assert clone.remove_edge('b', 'b')
    assert not clone.remove_edge('b', 'b')
    assert graph.has_edge('b', 'b')
    clone.clear()
    assert len(clone) == 0
    assert clone.edge_count == 0
    assert len(graph) == 3


@pytest.mark.parametrize('directed', [False, True])
def test_from_edge_list_and_matrix(directed):
    graph = GraphCore.from_edge_list([('a', 'b'), ('b', 'c', 5), ('a', 'a', 2)], directed=directed)
    assert graph.get_weight('a', 'b') == 1.0
    assert graph.get_weight('b', 'c') == 5
    assert graph.get_weight('a', 'a') == 2
    assert graph.edge_count == 3
    vertices, matrix = graph.to_adjacency_matrix()
    assert vertices == ['a', 'b', 'c']
    inf = float('inf')
    if directed:
        assert matrix == [[2, 1, inf], [inf, 0, 5], [inf, inf, 0]]
    else:
        assert matrix == [[2, 1, inf], [1, 0, 5], [inf, 5, 0]]
    matrix[0][1] = 99
    assert graph.get_weight('a', 'b') == 1.0


def test_empty_conversions():
    graph = GraphCore.from_edge_list([])
    assert graph.to_adjacency_matrix() == ([], [])
    assert list(graph.copy()) == []
