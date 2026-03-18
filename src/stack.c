#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

/* 创建栈 */
Stack* stack_create(void) {
    Stack* ps = (Stack*)malloc(sizeof(Stack));
    if (ps == NULL) {
        return NULL;
    }
    ps->ch = NULL;
    ps->number = NULL;
    ps->top = 0;
    ps->capacity = 0;
    return ps;
}

/* 销毁栈 */
void stack_destroy(Stack* ps) {
    if (ps == NULL) {
        return;
    }
    free(ps->ch);
    free(ps->number);
    ps->ch = NULL;
    ps->number = NULL;
    ps->top = 0;
    ps->capacity = 0;
    free(ps);
}

/* 扩容辅助函数 */
static CalcError stack_grow(Stack* ps, StackType type) {
    if (ps->capacity == 0) {
        ps->capacity = INITIAL_CAPACITY;
    } else {
        /* 检查溢出 */
        if (ps->capacity > SIZE_MAX / 2) {
            return CALC_ERR_MEMORY;
        }
        ps->capacity *= 2;
    }

    if (type == STACK_TYPE_DOUBLE) {
        double* new_arr = (double*)realloc(ps->number, sizeof(double) * ps->capacity);
        if (new_arr == NULL) {
            return CALC_ERR_MEMORY;
        }
        ps->number = new_arr;
    } else {
        char* new_arr = (char*)realloc(ps->ch, sizeof(char) * ps->capacity);
        if (new_arr == NULL) {
            return CALC_ERR_MEMORY;
        }
        ps->ch = new_arr;
    }
    return CALC_OK;
}

/* 入栈 */
CalcError stack_push(Stack* ps, const void* data, StackType type) {
    if (ps == NULL || data == NULL) {
        return CALC_ERR_NULL_PTR;
    }

    if (ps->top == ps->capacity) {
        CalcError err = stack_grow(ps, type);
        if (err != CALC_OK) {
            return err;
        }
    }

    if (type == STACK_TYPE_DOUBLE) {
        ps->number[ps->top] = *(const double*)data;
    } else {
        ps->ch[ps->top] = *(const char*)data;
    }
    ps->top++;
    return CALC_OK;
}

/* 出栈 */
CalcError stack_pop(Stack* ps, void* out_data, StackType type) {
    if (ps == NULL || out_data == NULL) {
        return CALC_ERR_NULL_PTR;
    }
    if (stack_is_empty(ps)) {
        return CALC_ERR_STACK_UNDERFLOW;
    }

    ps->top--;
    if (type == STACK_TYPE_DOUBLE) {
        *(double*)out_data = ps->number[ps->top];
    } else {
        *(char*)out_data = ps->ch[ps->top];
    }
    return CALC_OK;
}

/* 查看栈顶（不出栈） */
CalcError stack_peek(const Stack* ps, void* out_data, StackType type) {
    if (ps == NULL || out_data == NULL) {
        return CALC_ERR_NULL_PTR;
    }
    if (stack_is_empty(ps)) {
        return CALC_ERR_STACK_UNDERFLOW;
    }

    if (type == STACK_TYPE_DOUBLE) {
        *(double*)out_data = ps->number[ps->top - 1];
    } else {
        *(char*)out_data = ps->ch[ps->top - 1];
    }
    return CALC_OK;
}

/* 检查栈是否为空 */
int stack_is_empty(const Stack* ps) {
    return (ps == NULL || ps->top == 0);
}

/* 获取栈大小 */
size_t stack_size(const Stack* ps) {
    return (ps == NULL) ? 0 : ps->top;
}

/* 清空栈（不释放内存） */
void stack_clear(Stack* ps) {
    if (ps != NULL) {
        ps->top = 0;
    }
}
