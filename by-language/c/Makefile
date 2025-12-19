# DSA C Implementation Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -I.
LDFLAGS =

# Directories
SRC_DS = data-structures
SRC_ALG = algorithms
TEST_DIR = tests
BUILD_DIR = build

# Source files
DS_SOURCES = $(wildcard $(SRC_DS)/*.c)
ALG_SOURCES = $(wildcard $(SRC_ALG)/*.c)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

# Object files
DS_OBJECTS = $(patsubst $(SRC_DS)/%.c,$(BUILD_DIR)/%.o,$(DS_SOURCES))
ALG_OBJECTS = $(patsubst $(SRC_ALG)/%.c,$(BUILD_DIR)/%.o,$(ALG_SOURCES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

# Test executable
TEST_BIN = $(BUILD_DIR)/run_tests

.PHONY: all clean test

all: $(BUILD_DIR) $(DS_OBJECTS) $(ALG_OBJECTS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DS)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_ALG)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BUILD_DIR) $(DS_OBJECTS) $(ALG_OBJECTS) $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(DS_OBJECTS) $(ALG_OBJECTS) $(TEST_OBJECTS) -o $(TEST_BIN) $(LDFLAGS)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
