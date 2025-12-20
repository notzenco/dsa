# DSA C Implementation Makefile
#
# Usage:
#   make all        - Build all object files
#   make test       - Build and run all tests
#   make test-da    - Test dynamic array only
#   make test-sll   - Test singly linked list only
#   make clean      - Remove build artifacts

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I.
LDFLAGS =

# Directories
SRC_DS = data-structures
SRC_ALG = algorithms
TEST_DIR = tests
BUILD_DIR = build

# Source files
DS_SOURCES = $(wildcard $(SRC_DS)/*.c)
ALG_SOURCES = $(wildcard $(SRC_ALG)/*.c)

# Object files
DS_OBJECTS = $(patsubst $(SRC_DS)/%.c,$(BUILD_DIR)/ds_%.o,$(DS_SOURCES))
ALG_OBJECTS = $(patsubst $(SRC_ALG)/%.c,$(BUILD_DIR)/alg_%.o,$(ALG_SOURCES))

.PHONY: all clean test test-da test-sll test-dll test-stack test-queue test-sort test-graph test-search test-dp test-string test-trie test-avl test-cache test-range test-bloom test-skiplist test-rb test-btree test-adjmat test-uf test-mq test-ttl

all: $(BUILD_DIR) $(DS_OBJECTS) $(ALG_OBJECTS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Data structure object files
$(BUILD_DIR)/ds_%.o: $(SRC_DS)/%.c $(SRC_DS)/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Algorithm object files
$(BUILD_DIR)/alg_%.o: $(SRC_ALG)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Individual test targets
test-da: $(BUILD_DIR) $(BUILD_DIR)/ds_dynamic_array.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_dynamic_array.c $(BUILD_DIR)/ds_dynamic_array.o -o $(BUILD_DIR)/test_da $(LDFLAGS)
	./$(BUILD_DIR)/test_da

test-sll: $(BUILD_DIR) $(BUILD_DIR)/ds_singly_linked_list.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_singly_linked_list.c $(BUILD_DIR)/ds_singly_linked_list.o -o $(BUILD_DIR)/test_sll $(LDFLAGS)
	./$(BUILD_DIR)/test_sll

test-dll: $(BUILD_DIR) $(BUILD_DIR)/ds_doubly_linked_list.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_doubly_linked_list.c $(BUILD_DIR)/ds_doubly_linked_list.o -o $(BUILD_DIR)/test_dll $(LDFLAGS)
	./$(BUILD_DIR)/test_dll

test-stack: $(BUILD_DIR) $(BUILD_DIR)/ds_stack.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_stack.c $(BUILD_DIR)/ds_stack.o -o $(BUILD_DIR)/test_stack $(LDFLAGS)
	./$(BUILD_DIR)/test_stack

test-queue: $(BUILD_DIR) $(BUILD_DIR)/ds_queue.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_queue.c $(BUILD_DIR)/ds_queue.o -o $(BUILD_DIR)/test_queue $(LDFLAGS)
	./$(BUILD_DIR)/test_queue

test-bst: $(BUILD_DIR) $(BUILD_DIR)/ds_binary_search_tree.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_bst.c $(BUILD_DIR)/ds_binary_search_tree.o -o $(BUILD_DIR)/test_bst $(LDFLAGS)
	./$(BUILD_DIR)/test_bst

test-ht: $(BUILD_DIR) $(BUILD_DIR)/ds_hash_table.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_hash_table.c $(BUILD_DIR)/ds_hash_table.o -o $(BUILD_DIR)/test_ht $(LDFLAGS)
	./$(BUILD_DIR)/test_ht

test-heap: $(BUILD_DIR) $(BUILD_DIR)/ds_heap.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_heap.c $(BUILD_DIR)/ds_heap.o -o $(BUILD_DIR)/test_heap $(LDFLAGS)
	./$(BUILD_DIR)/test_heap

test-sort: $(BUILD_DIR) $(BUILD_DIR)/alg_sorting.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_sorting.c $(BUILD_DIR)/alg_sorting.o -o $(BUILD_DIR)/test_sort $(LDFLAGS)
	./$(BUILD_DIR)/test_sort

test-graph: $(BUILD_DIR) $(BUILD_DIR)/ds_graph.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_graph.c $(BUILD_DIR)/ds_graph.o -o $(BUILD_DIR)/test_graph $(LDFLAGS)
	./$(BUILD_DIR)/test_graph

test-search: $(BUILD_DIR) $(BUILD_DIR)/alg_searching.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_searching.c $(BUILD_DIR)/alg_searching.o -o $(BUILD_DIR)/test_search $(LDFLAGS) -lm
	./$(BUILD_DIR)/test_search

test-dp: $(BUILD_DIR) $(BUILD_DIR)/alg_dynamic_programming.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_dp.c $(BUILD_DIR)/alg_dynamic_programming.o -o $(BUILD_DIR)/test_dp $(LDFLAGS)
	./$(BUILD_DIR)/test_dp

test-string: $(BUILD_DIR) $(BUILD_DIR)/alg_string_algorithms.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_string.c $(BUILD_DIR)/alg_string_algorithms.o -o $(BUILD_DIR)/test_string $(LDFLAGS)
	./$(BUILD_DIR)/test_string

test-trie: $(BUILD_DIR) $(BUILD_DIR)/ds_trie.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_trie.c $(BUILD_DIR)/ds_trie.o -o $(BUILD_DIR)/test_trie $(LDFLAGS)
	./$(BUILD_DIR)/test_trie

test-avl: $(BUILD_DIR) $(BUILD_DIR)/ds_avl_tree.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_avl.c $(BUILD_DIR)/ds_avl_tree.o -o $(BUILD_DIR)/test_avl $(LDFLAGS)
	./$(BUILD_DIR)/test_avl

test-cache: $(BUILD_DIR) $(BUILD_DIR)/ds_lru_cache.o $(BUILD_DIR)/ds_lfu_cache.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_cache.c $(BUILD_DIR)/ds_lru_cache.o $(BUILD_DIR)/ds_lfu_cache.o -o $(BUILD_DIR)/test_cache $(LDFLAGS)
	./$(BUILD_DIR)/test_cache

test-range: $(BUILD_DIR) $(BUILD_DIR)/ds_segment_tree.o $(BUILD_DIR)/ds_fenwick_tree.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_range_trees.c $(BUILD_DIR)/ds_segment_tree.o $(BUILD_DIR)/ds_fenwick_tree.o -o $(BUILD_DIR)/test_range $(LDFLAGS)
	./$(BUILD_DIR)/test_range

test-bloom: $(BUILD_DIR) $(BUILD_DIR)/ds_bloom_filter.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_bloom.c $(BUILD_DIR)/ds_bloom_filter.o -o $(BUILD_DIR)/test_bloom $(LDFLAGS) -lm
	./$(BUILD_DIR)/test_bloom

test-skiplist: $(BUILD_DIR) $(BUILD_DIR)/ds_skip_list.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_skiplist.c $(BUILD_DIR)/ds_skip_list.o -o $(BUILD_DIR)/test_skiplist $(LDFLAGS)
	./$(BUILD_DIR)/test_skiplist

test-rb: $(BUILD_DIR) $(BUILD_DIR)/ds_red_black_tree.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_rb.c $(BUILD_DIR)/ds_red_black_tree.o -o $(BUILD_DIR)/test_rb $(LDFLAGS)
	./$(BUILD_DIR)/test_rb

test-btree: $(BUILD_DIR) $(BUILD_DIR)/ds_b_tree.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_btree.c $(BUILD_DIR)/ds_b_tree.o -o $(BUILD_DIR)/test_btree $(LDFLAGS)
	./$(BUILD_DIR)/test_btree

test-adjmat: $(BUILD_DIR) $(BUILD_DIR)/ds_adjacency_matrix.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_adjmat.c $(BUILD_DIR)/ds_adjacency_matrix.o -o $(BUILD_DIR)/test_adjmat $(LDFLAGS)
	./$(BUILD_DIR)/test_adjmat

test-uf: $(BUILD_DIR) $(BUILD_DIR)/ds_union_find.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_union_find.c $(BUILD_DIR)/ds_union_find.o -o $(BUILD_DIR)/test_uf $(LDFLAGS)
	./$(BUILD_DIR)/test_uf

test-mq: $(BUILD_DIR) $(BUILD_DIR)/ds_monotonic_queue.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_monotonic_queue.c $(BUILD_DIR)/ds_monotonic_queue.o -o $(BUILD_DIR)/test_mq $(LDFLAGS)
	./$(BUILD_DIR)/test_mq

test-ttl: $(BUILD_DIR) $(BUILD_DIR)/ds_ttl_cache.o
	$(CC) $(CFLAGS) $(TEST_DIR)/test_ttl_cache.c $(BUILD_DIR)/ds_ttl_cache.o -o $(BUILD_DIR)/test_ttl $(LDFLAGS)
	./$(BUILD_DIR)/test_ttl

# Run all tests
test: test-da test-sll test-dll test-stack test-queue test-bst test-ht test-heap test-sort test-graph test-search test-dp test-string test-trie test-avl test-cache test-range test-bloom test-skiplist test-rb test-btree test-adjmat test-uf test-mq test-ttl

clean:
	rm -rf $(BUILD_DIR)
