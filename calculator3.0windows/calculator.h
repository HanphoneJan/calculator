#pragma once //在头文件的最开始加入这条预处理指令，保证头文件只被编译一次
#include <stdio.h>
#include <stdlib.h>
// #include <assert.h> 调试时使用assert函数
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>
#include <assert.h>
#include<conio.h>
// 支持动态增长的数组栈
typedef struct Stack
{
    char* ch;
    double* number;
    //void* da;
    size_t top; // 栈顶元素下一位置的下标，top 的大小，恰为数组中元素的个数
    size_t capacity; // 容量：动态数组最大容量
}Stack;

// 初始化栈
void StackInit(Stack* ps);

// 获取栈顶元素
int StackGetTop(Stack* ps, void* topElement,int type);
// 入栈
int StackPush(Stack* ps, void* data, int type);

// 出栈
int StackPop(Stack* ps, void* topElement,int type);
int Stackpd(Stack* ps);
// 求栈长：获取栈中有效元素个数
size_t StackSize(Stack* ps);
// 检测栈是否为空，如果为空返回非零结果，如果不为空返回0
int StackEmpty(Stack* ps);
//清空栈
int Stackclear(Stack* ps);
//栈的遍历
//int Stacktraverse(Stack* ps);
//判断是否为运算符
int isoperator(const char c);
//比较运算符优先级
int Precedence(const char op);
//计算
double Calculate(const char op,const double a,const double,int* safe);
//开始界面
void startUI();
//打印帮助
int help();
int quit();
int readhistory(FILE* p,int key);

int checkinput(char input);

void gotoxy(int x, int y);