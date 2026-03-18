#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "error.h"
#include <stdio.h>

/* 常量定义 */
#define MAX_EXPRESSION_LEN 1024
#define MAX_TOKEN_LEN 256

/* 计算结果结构体 - 修复悬空指针问题 */
typedef struct {
    double value;
    CalcError error;
} CalcResult;

/* 运算符优先级判断 */
int is_operator(char c);
int precedence(char op);

/* 核心计算函数 - 中缀表达式求值 */
CalcResult calculate_expression(const char* infix, FILE* history);

/* 文件批处理模式 */
CalcError process_file(const char* input_file, const char* output_file, FILE* history);

#endif /* CALCULATOR_H */
