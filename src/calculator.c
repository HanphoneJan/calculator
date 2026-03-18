#include "calculator.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* 判断是否为运算符 */
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || '/');
}

/* 获取运算符优先级 */
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

/* 执行运算 */
static CalcError do_operation(char op, double a, double b, double* result) {
    switch (op) {
        case '+':
            *result = a + b;
            return CALC_OK;
        case '-':
            *result = b - a;  /* 注意顺序: b - a */
            return CALC_OK;
        case '*':
            *result = a * b;
            return CALC_OK;
        case '/':
            if (fabs(a) < 1e-15) {
                return CALC_ERR_DIV_BY_ZERO;
            }
            *result = b / a;  /* 注意顺序: b / a */
            return CALC_OK;
        default:
            return CALC_ERR_INVALID_INPUT;
    }
}

/* 中缀转后缀并计算（使用调度场算法） */
CalcResult calculate_expression(const char* infix, FILE* history) {
    CalcResult result = {0.0, CALC_OK};
    Stack* op_stack = NULL;
    Stack* val_stack = NULL;
    char* postfix = NULL;
    int safe = 0;
    int bracket_count = 0;
    size_t infix_len = 0;
    size_t j = 0;

    /* 参数检查 */
    if (infix == NULL) {
        result.error = CALC_ERR_NULL_PTR;
        goto cleanup;
    }

    infix_len = strlen(infix);
    if (infix_len == 0) {
        result.error = CALC_ERR_INVALID_INPUT;
        goto cleanup;
    }

    /* 分配资源 */
    op_stack = stack_create();
    val_stack = stack_create();
    postfix = (char*)malloc(MAX_EXPRESSION_LEN);

    if (op_stack == NULL || val_stack == NULL || postfix == NULL) {
        result.error = CALC_ERR_MEMORY;
        goto cleanup;
    }

    memset(postfix, 0, MAX_EXPRESSION_LEN);

    /* 中缀转后缀 */
    for (size_t i = 0; i < infix_len && !safe; i++) {
        char ch = infix[i];

        /* 跳过空格 */
        if (ch == ' ' || ch == '\t') {
            continue;
        }

        /* 检查字符有效性 */
        if (!isdigit(ch) && !is_operator(ch) && ch != '(' && ch != ')' && ch != '.' && ch != '-') {
            safe = 1;
            break;
        }

        /* 处理数字（包括负数和带小数的数字） */
        if (isdigit(ch) || (ch == '-' && (i == 0 || infix[i-1] == '('))) {
            /* 检查缓冲区溢出 */
            if (j >= MAX_EXPRESSION_LEN - 2) {
                safe = 1;
                result.error = CALC_ERR_BUFFER_OVERFLOW;
                break;
            }

            /* 复制数字到后缀表达式 */
            int is_negative = (ch == '-');
            if (is_negative) {
                postfix[j++] = ch;
                i++;
                if (i >= infix_len || !isdigit(infix[i])) {
                    safe = 1;
                    break;
                }
                ch = infix[i];
            }

            while (i < infix_len && (isdigit(infix[i]) || infix[i] == '.')) {
                if (j >= MAX_EXPRESSION_LEN - 2) {
                    safe = 1;
                    result.error = CALC_ERR_BUFFER_OVERFLOW;
                    break;
                }
                postfix[j++] = infix[i];
                i++;
            }
            i--; /* 回退一位 */
            postfix[j++] = '#'; /* 数字分隔符 */
        }
        else if (is_operator(ch)) {
            /* 处理运算符 */
            char top_op;
            while (!stack_is_empty(op_stack)) {
                if (stack_peek(op_stack, &top_op, STACK_TYPE_CHAR) != CALC_OK) {
                    break;
                }
                if (top_op == '(' || precedence(ch) > precedence(top_op)) {
                    break;
                }
                /* 弹出栈顶运算符到后缀表达式 */
                stack_pop(op_stack, &top_op, STACK_TYPE_CHAR);
                if (j >= MAX_EXPRESSION_LEN - 1) {
                    safe = 1;
                    result.error = CALC_ERR_BUFFER_OVERFLOW;
                    break;
                }
                postfix[j++] = top_op;
            }
            if (safe) break;

            /* 当前运算符入栈 */
            CalcError err = stack_push(op_stack, &ch, STACK_TYPE_CHAR);
            if (err != CALC_OK) {
                result.error = err;
                safe = 1;
                break;
            }
        }
        else if (ch == '(') {
            bracket_count++;
            CalcError err = stack_push(op_stack, &ch, STACK_TYPE_CHAR);
            if (err != CALC_OK) {
                result.error = err;
                safe = 1;
                break;
            }
        }
        else if (ch == ')') {
            bracket_count++;
            char top_op;
            int found_left = 0;

            /* 弹出运算符直到遇到左括号 */
            while (!stack_is_empty(op_stack)) {
                if (stack_pop(op_stack, &top_op, STACK_TYPE_CHAR) != CALC_OK) {
                    break;
                }
                if (top_op == '(') {
                    found_left = 1;
                    break;
                }
                if (j >= MAX_EXPRESSION_LEN - 1) {
                    safe = 1;
                    result.error = CALC_ERR_BUFFER_OVERFLOW;
                    break;
                }
                postfix[j++] = top_op;
            }

            if (!found_left || safe) {
                safe = 1;
                if (result.error == CALC_OK) {
                    result.error = CALC_ERR_MISMATCHED_PAREN;
                }
                break;
            }
        }
    }

    /* 检查括号匹配 */
    if (bracket_count % 2 != 0) {
        safe = 1;
        result.error = CALC_ERR_MISMATCHED_PAREN;
    }

    if (!safe) {
        /* 将剩余运算符弹出到后缀表达式 */
        while (!stack_is_empty(op_stack)) {
            char top_op;
            if (stack_pop(op_stack, &top_op, STACK_TYPE_CHAR) != CALC_OK) {
                break;
            }
            if (top_op == '(' || top_op == ')') {
                safe = 1;
                result.error = CALC_ERR_MISMATCHED_PAREN;
                break;
            }
            if (j >= MAX_EXPRESSION_LEN - 1) {
                safe = 1;
                result.error = CALC_ERR_BUFFER_OVERFLOW;
                break;
            }
            postfix[j++] = top_op;
        }
    }

    if (safe) {
        if (result.error == CALC_OK) {
            result.error = CALC_ERR_INVALID_INPUT;
        }
        goto cleanup;
    }

    postfix[j] = '\0';

    /* 计算后缀表达式 */
    for (size_t k = 0; postfix[k] != '\0' && !safe; k++) {
        if (postfix[k] == '#') {
            continue; /* 跳过分隔符 */
        }

        /* 处理数字 */
        if (isdigit(postfix[k]) || (postfix[k] == '-' && isdigit(postfix[k+1]))) {
            char num_str[MAX_TOKEN_LEN];
            size_t num_len = 0;

            /* 复制数字字符串 */
            while (k < j && postfix[k] != '#' && num_len < MAX_TOKEN_LEN - 1) {
                num_str[num_len++] = postfix[k];
                k++;
            }
            num_str[num_len] = '\0';

            /* 转换为double并入栈 */
            char* endptr;
            double val = strtod(num_str, &endptr);
            if (*endptr != '\0') {
                safe = 1;
                result.error = CALC_ERR_INVALID_INPUT;
                break;
            }

            CalcError err = stack_push(val_stack, &val, STACK_TYPE_DOUBLE);
            if (err != CALC_OK) {
                result.error = err;
                safe = 1;
                break;
            }
        }
        /* 处理运算符 */
        else if (is_operator(postfix[k])) {
            double a, b, res;

            /* 弹出两个操作数 */
            if (stack_pop(val_stack, &a, STACK_TYPE_DOUBLE) != CALC_OK ||
                stack_pop(val_stack, &b, STACK_TYPE_DOUBLE) != CALC_OK) {
                safe = 1;
                result.error = CALC_ERR_STACK_UNDERFLOW;
                break;
            }

            /* 执行运算 */
            CalcError err = do_operation(postfix[k], a, b, &res);
            if (err != CALC_OK) {
                result.error = err;
                safe = 1;
                break;
            }

            /* 结果入栈 */
            err = stack_push(val_stack, &res, STACK_TYPE_DOUBLE);
            if (err != CALC_OK) {
                result.error = err;
                safe = 1;
                break;
            }
        }
    }

    if (!safe) {
        /* 最终结果应该只有一个值在栈中 */
        if (stack_size(val_stack) != 1) {
            result.error = CALC_ERR_INVALID_INPUT;
            safe = 1;
        } else {
            if (stack_pop(val_stack, &result.value, STACK_TYPE_DOUBLE) != CALC_OK) {
                result.error = CALC_ERR_STACK_UNDERFLOW;
                safe = 1;
            }
        }
    }

cleanup:
    /* 统一资源释放 - 修复内存泄漏 */
    if (op_stack) stack_destroy(op_stack);
    if (val_stack) stack_destroy(val_stack);
    if (postfix) free(postfix);

    /* 写入历史记录 */
    if (history != NULL) {
        if (result.error == CALC_OK && !safe) {
            fprintf(history, "Result:%.6f\n", result.value);
        } else {
            fprintf(history, "Error:%s\n", calc_error_string(result.error));
        }
        fflush(history);
    }

    return result;
}

/* 文件批处理模式 */
CalcError process_file(const char* input_file, const char* output_file, FILE* history) {
    FILE* input = NULL;
    FILE* output = NULL;
    char line[MAX_EXPRESSION_LEN];
    int line_num = 0;
    CalcError final_error = CALC_OK;

    if (input_file == NULL) {
        return CALC_ERR_NULL_PTR;
    }

    input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error: Cannot open input file: %s\n", input_file);
        return CALC_ERR_FILE;
    }

    if (output_file != NULL) {
        output = fopen(output_file, "w");
        if (output == NULL) {
            fprintf(stderr, "Error: Cannot open output file: %s\n", output_file);
            fclose(input);
            return CALC_ERR_FILE;
        }
    }

    /* 逐行读取处理 - 修复feof()误用问题 */
    while (fgets(line, sizeof(line), input) != NULL) {
        line_num++;

        /* 去除换行符 */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        /* 跳过空行 */
        if (strlen(line) == 0) {
            continue;
        }

        /* 计算 */
        CalcResult res = calculate_expression(line, history);

        /* 输出结果 */
        if (output != NULL) {
            if (res.error == CALC_OK) {
                fprintf(output, "Result%d:%.6f\n", line_num, res.value);
            } else {
                fprintf(output, "Result%d:Error - %s\n", line_num, calc_error_string(res.error));
                final_error = res.error;
            }
        } else {
            if (res.error != CALC_OK) {
                final_error = res.error;
            }
        }
    }

    if (output != NULL) {
        fclose(output);
    }
    fclose(input);

    return final_error;
}
