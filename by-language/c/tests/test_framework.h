/**
 * Simple Test Framework for C
 *
 * A lightweight testing framework for unit testing in C.
 * Provides macros for assertions and test organization.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Color codes for terminal output */
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_RESET   "\x1b[0m"

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;
static int current_test_failed = 0;

/* Current test name */
static const char *current_test_name = NULL;

/**
 * Start a test suite
 */
#define TEST_SUITE_START(name) \
    do { \
        printf("\n" COLOR_YELLOW "═══ Test Suite: %s ═══" COLOR_RESET "\n\n", name); \
        tests_run = 0; \
        tests_passed = 0; \
        tests_failed = 0; \
    } while (0)

/**
 * End a test suite and print summary
 */
#define TEST_SUITE_END() \
    do { \
        printf("\n" COLOR_YELLOW "═══ Summary ═══" COLOR_RESET "\n"); \
        printf("Tests run: %d\n", tests_run); \
        printf(COLOR_GREEN "Passed: %d" COLOR_RESET "\n", tests_passed); \
        if (tests_failed > 0) { \
            printf(COLOR_RED "Failed: %d" COLOR_RESET "\n", tests_failed); \
        } else { \
            printf("Failed: %d\n", tests_failed); \
        } \
        printf("\n"); \
    } while (0)

/**
 * Define a test function
 */
#define TEST(name) \
    void test_##name(void); \
    void test_##name(void)

/**
 * Run a test
 */
#define RUN_TEST(name) \
    do { \
        current_test_name = #name; \
        current_test_failed = 0; \
        tests_run++; \
        test_##name(); \
        if (current_test_failed) { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ %s" COLOR_RESET "\n", #name); \
        } else { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ %s" COLOR_RESET "\n", #name); \
        } \
    } while (0)

/**
 * Assert that a condition is true
 */
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_TRUE(%s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #condition); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that a condition is false
 */
#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_FALSE(%s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #condition); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that two integers are equal
 */
#define ASSERT_EQ(expected, actual) \
    do { \
        long long _exp = (long long)(expected); \
        long long _act = (long long)(actual); \
        if (_exp != _act) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_EQ(%s, %s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #expected, #actual); \
            printf("      Expected: %lld\n", _exp); \
            printf("      Actual:   %lld\n", _act); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that two integers are not equal
 */
#define ASSERT_NEQ(val1, val2) \
    do { \
        long long _v1 = (long long)(val1); \
        long long _v2 = (long long)(val2); \
        if (_v1 == _v2) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_NEQ(%s, %s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #val1, #val2); \
            printf("      Both values: %lld\n", _v1); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that a pointer is NULL
 */
#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_NULL(%s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #ptr); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that a pointer is not NULL
 */
#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_NOT_NULL(%s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #ptr); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Assert that two strings are equal
 */
#define ASSERT_STR_EQ(expected, actual) \
    do { \
        const char *_exp = (expected); \
        const char *_act = (actual); \
        if (_exp == NULL || _act == NULL || strcmp(_exp, _act) != 0) { \
            printf(COLOR_RED "    FAIL: %s:%d: ASSERT_STR_EQ(%s, %s)" COLOR_RESET "\n", \
                   __FILE__, __LINE__, #expected, #actual); \
            printf("      Expected: \"%s\"\n", _exp ? _exp : "NULL"); \
            printf("      Actual:   \"%s\"\n", _act ? _act : "NULL"); \
            current_test_failed = 1; \
            return; \
        } \
    } while (0)

/**
 * Get the number of failed tests
 */
#define GET_FAILED_COUNT() tests_failed

#endif /* TEST_FRAMEWORK_H */
