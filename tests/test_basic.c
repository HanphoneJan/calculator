#include "../src/calculator.h"
#include "../src/stack.h"
#include "../src/error.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define EPSILON 0.0001
#define TEST_PASS "[PASS]"
#define TEST_FAIL "[FAIL]"

static int tests_passed = 0;
static int tests_failed = 0;

/* 辅助函数：比较double */
int double_eq(double a, double b) {
    return fabs(a - b) < EPSILON;
}

/* 测试栈操作 */
void test_stack(void) {
    printf("\n=== Testing Stack Operations ===\n");

    /* 测试创建和销毁 */
    Stack* s = stack_create();
    if (s == NULL) {
        printf("%s stack_create\n", TEST_FAIL);
        tests_failed++;
        return;
    }
    printf("%s stack_create\n", TEST_PASS);
    tests_passed++;

    /* 测试空栈检查 */
    if (!stack_is_empty(s)) {
        printf("%s stack_is_empty (empty)\n", TEST_FAIL);
        tests_failed++;
    } else {
        printf("%s stack_is_empty (empty)\n", TEST_PASS);
        tests_passed++;
    }

    /* 测试push/pop */
    double val = 3.14;
    if (stack_push(s, &val, STACK_TYPE_DOUBLE) != CALC_OK) {
        printf("%s stack_push\n", TEST_FAIL);
        tests_failed++;
    } else {
        printf("%s stack_push\n", TEST_PASS);
        tests_passed++;
    }

    double out;
    if (stack_pop(s, &out, STACK_TYPE_DOUBLE) != CALC_OK || !double_eq(out, 3.14)) {
        printf("%s stack_pop\n", TEST_FAIL);
        tests_failed++;
    } else {
        printf("%s stack_pop\n", TEST_PASS);
        tests_passed++;
    }

    /* 测试栈下溢 */
    if (stack_pop(s, &out, STACK_TYPE_DOUBLE) != CALC_ERR_STACK_UNDERFLOW) {
        printf("%s stack_underflow detection\n", TEST_FAIL);
        tests_failed++;
    } else {
        printf("%s stack_underflow detection\n", TEST_PASS);
        tests_passed++;
    }

    stack_destroy(s);
    printf("%s stack_destroy\n", TEST_PASS);
    tests_passed++;
}

/* 测试基本运算 */
void test_basic_operations(void) {
    printf("\n=== Testing Basic Operations ===\n");

    FILE* devnull = fopen("NUL", "w");
    if (devnull == NULL) {
        devnull = stdout;  /* 回退到stdout */
    }

    struct {
        const char* expr;
        double expected;
    } tests[] = {
        {"1+2", 3.0},
        {"5-3", 2.0},
        {"4*5", 20.0},
        {"10/2", 5.0},
        {"0+0", 0.0},
        {"100-50", 50.0},
        {"3*3", 9.0},
        {"7/2", 3.5},
    };

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++) {
        CalcResult r = calculate_expression(tests[i].expr, devnull);
        if (r.error != CALC_OK) {
            printf("%s '%s' - Error: %s\n", TEST_FAIL, tests[i].expr, calc_error_string(r.error));
            tests_failed++;
        } else if (!double_eq(r.value, tests[i].expected)) {
            printf("%s '%s' - Expected %.4f, got %.4f\n", TEST_FAIL, tests[i].expr, tests[i].expected, r.value);
            tests_failed++;
        } else {
            printf("%s '%s' = %.4f\n", TEST_PASS, tests[i].expr, r.value);
            tests_passed++;
        }
    }

    if (devnull != stdout) fclose(devnull);
}

/* 测试优先级和括号 */
void test_precedence(void) {
    printf("\n=== Testing Operator Precedence ===\n");

    FILE* devnull = fopen("NUL", "w");
    if (devnull == NULL) devnull = stdout;

    struct {
        const char* expr;
        double expected;
    } tests[] = {
        {"2+3*4", 14.0},      /* 乘法优先 */
        {"(2+3)*4", 20.0},    /* 括号优先 */
        {"10-2*3", 4.0},      /* 乘法优先 */
        {"10/2+3", 8.0},      /* 除法优先 */
        {"(10+2)/3", 4.0},    /* 括号优先 */
        {"2*3+4*5", 26.0},    /* 左到右 */
        {"((1+2)*3)", 9.0},   /* 嵌套括号 */
    };

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++) {
        CalcResult r = calculate_expression(tests[i].expr, devnull);
        if (r.error != CALC_OK) {
            printf("%s '%s' - Error: %s\n", TEST_FAIL, tests[i].expr, calc_error_string(r.error));
            tests_failed++;
        } else if (!double_eq(r.value, tests[i].expected)) {
            printf("%s '%s' - Expected %.4f, got %.4f\n", TEST_FAIL, tests[i].expr, tests[i].expected, r.value);
            tests_failed++;
        } else {
            printf("%s '%s' = %.4f\n", TEST_PASS, tests[i].expr, r.value);
            tests_passed++;
        }
    }

    if (devnull != stdout) fclose(devnull);
}

/* 测试负数 */
void test_negative(void) {
    printf("\n=== Testing Negative Numbers ===\n");

    FILE* devnull = fopen("NUL", "w");
    if (devnull == NULL) devnull = stdout;

    struct {
        const char* expr;
        double expected;
    } tests[] = {
        {"-5+3", -2.0},
        {"3+(-2)", 1.0},
        {"-5*-2", 10.0},
        {"10/-2", -5.0},
        {"(-5)+(-3)", -8.0},
    };

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++) {
        CalcResult r = calculate_expression(tests[i].expr, devnull);
        if (r.error != CALC_OK) {
            printf("%s '%s' - Error: %s\n", TEST_FAIL, tests[i].expr, calc_error_string(r.error));
            tests_failed++;
        } else if (!double_eq(r.value, tests[i].expected)) {
            printf("%s '%s' - Expected %.4f, got %.4f\n", TEST_FAIL, tests[i].expr, tests[i].expected, r.value);
            tests_failed++;
        } else {
            printf("%s '%s' = %.4f\n", TEST_PASS, tests[i].expr, r.value);
            tests_passed++;
        }
    }

    if (devnull != stdout) fclose(devnull);
}

/* 测试错误处理 */
void test_errors(void) {
    printf("\n=== Testing Error Handling ===\n");

    FILE* devnull = fopen("NUL", "w");
    if (devnull == NULL) devnull = stdout;

    struct {
        const char* expr;
        CalcError expected_error;
    } tests[] = {
        {"1/0", CALC_ERR_DIV_BY_ZERO},
        {"(1+2", CALC_ERR_MISMATCHED_PAREN},
        {"1+2)", CALC_ERR_MISMATCHED_PAREN},
        {"1+a", CALC_ERR_INVALID_INPUT},
        {"", CALC_ERR_INVALID_INPUT},
    };

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++) {
        CalcResult r = calculate_expression(tests[i].expr, devnull);
        if (r.error != tests[i].expected_error) {
            printf("%s '%s' - Expected error %d, got %d\n", TEST_FAIL, tests[i].expr, tests[i].expected_error, r.error);
            tests_failed++;
        } else {
            printf("%s '%s' - Correctly detected: %s\n", TEST_PASS, tests[i].expr, calc_error_string(r.error));
            tests_passed++;
        }
    }

    if (devnull != stdout) fclose(devnull);
}

/* 主函数 */
int main(void) {
    printf("========================================\n");
    printf("    Calculator Unit Tests\n");
    printf("========================================\n");

    test_stack();
    test_basic_operations();
    test_precedence();
    test_negative();
    test_errors();

    printf("\n========================================\n");
    printf("    Test Summary\n");
    printf("========================================\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);

    if (tests_failed == 0) {
        printf("\nAll tests passed!\n");
        return 0;
    } else {
        printf("\nSome tests failed!\n");
        return 1;
    }
}
