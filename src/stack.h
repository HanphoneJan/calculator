#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "error.h"

/* 栈元素类型枚举 */
typedef enum {
    STACK_TYPE_DOUBLE,
    STACK_TYPE_CHAR
} StackType;

/* 动态栈结构体 */
typedef struct Stack {
    char* ch;           // 字符栈（用于运算符）
    double* number;     // 数字栈
    size_t top;         // 栈顶索引（指向下一个空闲位置）
    size_t capacity;    // 当前容量
} Stack;

/* 栈生命周期管理 */
Stack* stack_create(void);
void stack_destroy(Stack* ps);

/* 栈操作 */
CalcError stack_push(Stack* ps, const void* data, StackType type);
CalcError stack_pop(Stack* ps, void* out_data, StackType type);
CalcError stack_peek(const Stack* ps, void* out_data, StackType type);
int stack_is_empty(const Stack* ps);
size_t stack_size(const Stack* ps);
void stack_clear(Stack* ps);

#endif /* STACK_H */
