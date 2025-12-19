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

.PHONY: all clean test test-da test-sll test-dll test-stack test-queue

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

# Run all tests
test: test-da

clean:
	rm -rf $(BUILD_DIR)
