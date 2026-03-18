#ifndef ERROR_H
#define ERROR_H

/* 统一错误码定义 */
typedef enum {
    CALC_OK = 0,                // 成功
    CALC_ERR_NULL_PTR = -1,     // 空指针
    CALC_ERR_INVALID_INPUT = -2,// 无效输入
    CALC_ERR_DIV_BY_ZERO = -3,  // 除零错误
    CALC_ERR_MEMORY = -4,       // 内存分配失败
    CALC_ERR_STACK_UNDERFLOW = -5,  // 栈下溢
    CALC_ERR_MISMATCHED_PAREN = -6, // 括号不匹配
    CALC_ERR_BUFFER_OVERFLOW = -7,  // 缓冲区溢出
    CALC_ERR_FILE = -8          // 文件操作错误
} CalcError;

/* 错误码转字符串 */
static inline const char* calc_error_string(CalcError err) {
    switch (err) {
        case CALC_OK: return "Success";
        case CALC_ERR_NULL_PTR: return "Null pointer";
        case CALC_ERR_INVALID_INPUT: return "Invalid input";
        case CALC_ERR_DIV_BY_ZERO: return "Division by zero";
        case CALC_ERR_MEMORY: return "Memory allocation failed";
        case CALC_ERR_STACK_UNDERFLOW: return "Stack underflow";
        case CALC_ERR_MISMATCHED_PAREN: return "Mismatched parentheses";
        case CALC_ERR_BUFFER_OVERFLOW: return "Buffer overflow";
        case CALC_ERR_FILE: return "File error";
        default: return "Unknown error";
    }
}

#endif /* ERROR_H */
