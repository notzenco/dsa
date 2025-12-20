/**
 * Tests for Union-Find implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../data-structures/union_find.h"

static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT(cond) do { \
    tests_run++; \
    if (cond) { \
        tests_passed++; \
    } else { \
        printf("FAIL: %s (line %d)\n", #cond, __LINE__); \
    } \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    tests_run++; \
    if ((expected) == (actual)) { \
        tests_passed++; \
    } else { \
        printf("FAIL: expected %d, got %d (line %d)\n", (int)(expected), (int)(actual), __LINE__); \
    } \
} while(0)

/* ============== Basic Operations Tests ============== */

void test_create_destroy(void) {
    printf("Testing create/destroy...\n");

    UnionFind *uf = uf_create(10);
    ASSERT(uf != NULL);
    ASSERT_EQ(10, uf_size(uf));
    ASSERT_EQ(10, uf_num_components(uf));

    uf_destroy(uf);

    /* Invalid create */
    uf = uf_create(0);
    ASSERT(uf == NULL);

    uf = uf_create(-1);
    ASSERT(uf == NULL);
}

void test_find(void) {
    printf("Testing find...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    /* Each element is its own root initially */
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(i, uf_find(uf, i));
    }

    /* Invalid finds */
    ASSERT_EQ(-1, uf_find(uf, -1));
    ASSERT_EQ(-1, uf_find(uf, 5));

    uf_destroy(uf);
}

void test_union_basic(void) {
    printf("Testing basic union...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    /* Union 0 and 1 */
    ASSERT(uf_union(uf, 0, 1) == true);
    ASSERT_EQ(4, uf_num_components(uf));
    ASSERT_EQ(uf_find(uf, 0), uf_find(uf, 1));

    /* Union 2 and 3 */
    ASSERT(uf_union(uf, 2, 3) == true);
    ASSERT_EQ(3, uf_num_components(uf));
    ASSERT_EQ(uf_find(uf, 2), uf_find(uf, 3));

    /* Different components are not connected */
    ASSERT(uf_find(uf, 0) != uf_find(uf, 2));

    /* Union the two components */
    ASSERT(uf_union(uf, 1, 2) == true);
    ASSERT_EQ(2, uf_num_components(uf));
    ASSERT_EQ(uf_find(uf, 0), uf_find(uf, 3));

    uf_destroy(uf);
}

void test_union_duplicate(void) {
    printf("Testing duplicate union...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    ASSERT(uf_union(uf, 0, 1) == true);
    ASSERT_EQ(4, uf_num_components(uf));

    /* Duplicate union should return false */
    ASSERT(uf_union(uf, 0, 1) == false);
    ASSERT_EQ(4, uf_num_components(uf));

    ASSERT(uf_union(uf, 1, 0) == false);
    ASSERT_EQ(4, uf_num_components(uf));

    uf_destroy(uf);
}

void test_connected(void) {
    printf("Testing connected...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    /* Initially not connected */
    ASSERT(uf_connected(uf, 0, 1) == false);
    ASSERT(uf_connected(uf, 0, 4) == false);

    /* Each element is connected to itself */
    for (int i = 0; i < 5; i++) {
        ASSERT(uf_connected(uf, i, i) == true);
    }

    /* After union, connected */
    uf_union(uf, 0, 1);
    ASSERT(uf_connected(uf, 0, 1) == true);
    ASSERT(uf_connected(uf, 1, 0) == true);

    /* Still not connected to others */
    ASSERT(uf_connected(uf, 0, 2) == false);

    /* Invalid elements */
    ASSERT(uf_connected(uf, -1, 0) == false);
    ASSERT(uf_connected(uf, 0, 10) == false);

    uf_destroy(uf);
}

void test_component_size(void) {
    printf("Testing component size...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    /* Initially each component has size 1 */
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(1, uf_component_size(uf, i));
    }

    /* After union, size increases */
    uf_union(uf, 0, 1);
    ASSERT_EQ(2, uf_component_size(uf, 0));
    ASSERT_EQ(2, uf_component_size(uf, 1));

    uf_union(uf, 2, 3);
    uf_union(uf, 0, 2);
    ASSERT_EQ(4, uf_component_size(uf, 0));
    ASSERT_EQ(4, uf_component_size(uf, 3));

    /* Unconnected element still has size 1 */
    ASSERT_EQ(1, uf_component_size(uf, 4));

    uf_destroy(uf);
}

void test_clear(void) {
    printf("Testing clear...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    uf_union(uf, 0, 1);
    uf_union(uf, 2, 3);
    uf_union(uf, 0, 2);
    ASSERT_EQ(2, uf_num_components(uf));

    uf_clear(uf);

    ASSERT_EQ(5, uf_num_components(uf));
    ASSERT(uf_connected(uf, 0, 1) == false);
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(i, uf_find(uf, i));
        ASSERT_EQ(1, uf_component_size(uf, i));
    }

    uf_destroy(uf);
}

void test_get_component(void) {
    printf("Testing get component...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    uf_union(uf, 0, 1);
    uf_union(uf, 0, 2);

    int members[5];
    int count = uf_get_component(uf, 1, members);

    ASSERT_EQ(3, count);

    /* Check that 0, 1, 2 are in the component */
    int found[3] = {0, 0, 0};
    for (int i = 0; i < count; i++) {
        if (members[i] >= 0 && members[i] <= 2) {
            found[members[i]] = 1;
        }
    }
    ASSERT(found[0] && found[1] && found[2]);

    uf_destroy(uf);
}

void test_get_roots(void) {
    printf("Testing get roots...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    int roots[5];
    int count = uf_get_roots(uf, roots);
    ASSERT_EQ(5, count);

    uf_union(uf, 0, 1);
    uf_union(uf, 2, 3);

    count = uf_get_roots(uf, roots);
    ASSERT_EQ(3, count); /* 3 components: {0,1}, {2,3}, {4} */

    uf_destroy(uf);
}

void test_is_root(void) {
    printf("Testing is_root...\n");

    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    /* Initially all are roots */
    for (int i = 0; i < 5; i++) {
        ASSERT(uf_is_root(uf, i) == true);
    }

    uf_union(uf, 0, 1);

    /* After union, only one is root */
    int root = uf_find(uf, 0);
    ASSERT(uf_is_root(uf, root) == true);

    int non_root = (root == 0) ? 1 : 0;
    ASSERT(uf_is_root(uf, non_root) == false);

    uf_destroy(uf);
}

void test_path_compression(void) {
    printf("Testing path compression...\n");

    UnionFind *uf = uf_create(10);
    ASSERT(uf != NULL);

    /* Create a long chain: 0 <- 1 <- 2 <- 3 <- 4 <- 5 <- 6 <- 7 <- 8 <- 9 */
    for (int i = 0; i < 9; i++) {
        uf_union(uf, i, i + 1);
    }

    /* Find on last element should compress path */
    int root = uf_find(uf, 9);
    ASSERT_EQ(10, uf_component_size(uf, root));

    /* After find, all elements should point directly to root */
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(root, uf_find(uf, i));
    }

    uf_destroy(uf);
}

void test_many_elements(void) {
    printf("Testing with many elements...\n");

    int n = 1000;
    UnionFind *uf = uf_create(n);
    ASSERT(uf != NULL);

    ASSERT_EQ(n, uf_num_components(uf));

    /* Union all elements into one component */
    for (int i = 0; i < n - 1; i++) {
        uf_union(uf, i, i + 1);
    }

    ASSERT_EQ(1, uf_num_components(uf));
    ASSERT_EQ(n, uf_component_size(uf, 0));

    /* All connected */
    int root = uf_find(uf, 0);
    for (int i = 0; i < n; i++) {
        ASSERT_EQ(root, uf_find(uf, i));
    }

    uf_destroy(uf);
}

void test_leetcode_example(void) {
    printf("Testing LeetCode-style example (number of provinces)...\n");

    /* Graph:
     * 0 -- 1
     * 2 -- 3
     * 4 (isolated)
     *
     * Expected: 3 provinces
     */
    UnionFind *uf = uf_create(5);
    ASSERT(uf != NULL);

    uf_union(uf, 0, 1);
    uf_union(uf, 2, 3);

    ASSERT_EQ(3, uf_num_components(uf));

    /* Union two provinces */
    uf_union(uf, 1, 2);
    ASSERT_EQ(2, uf_num_components(uf));

    uf_destroy(uf);
}

/* ============== Main ============== */

int main(void) {
    printf("=== Union-Find Tests ===\n\n");

    test_create_destroy();
    test_find();
    test_union_basic();
    test_union_duplicate();
    test_connected();
    test_component_size();
    test_clear();
    test_get_component();
    test_get_roots();
    test_is_root();
    test_path_compression();
    test_many_elements();
    test_leetcode_example();

    printf("\n=== Results: %d/%d tests passed ===\n", tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}
