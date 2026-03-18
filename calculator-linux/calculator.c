#include "calculator.h"

enum{Fail=-1,Success=0};
enum type{lf,word};


// 初始化栈
void StackInit(Stack* ps)
{
    ps->number = NULL;
    ps->ch = NULL;
    //ps->da = NULL;
    ps->top = 0;
    ps->capacity = 0;
}

// 入栈
int StackPush(Stack* ps, void* data, int type)
{
    if (ps == NULL) {
    return Fail;
    }

    if(type==lf){
        //如果栈满了，则先进行扩容
    if (ps->top == ps->capacity)
    {
    size_t newCapacity = ps->capacity ? ps->capacity * 2 : 8;
    double* temp = ps->number; // 保存原始指针的副本
    ps->number = (double*)realloc(ps->number, sizeof(double) * newCapacity);
        if (ps->number == NULL) // 检查realloc是否成功
        {
        free(temp); // 如果失败，释放原始内存块
        printf("realloc fail!!!\n");
        return Fail;
        }
    ps->capacity = newCapacity;
    }
        ps->number[ps->top] = *((double*)data);
    }

    else if(type==word){
    if (ps->top == ps->capacity)
    {
    size_t newCapacity = ps->capacity ? ps->capacity * 2 : 4;
    char* temp = ps->ch; // 保存原始指针的副本
    ps->ch = (char*)realloc(ps->ch, sizeof(char) * newCapacity);
        if (ps->ch == NULL) // 检查realloc是否成功
        {
        free(temp); // 如果失败，释放原始内存块
        printf("realloc fail!!!\n");
        exit(-1);
        }
    ps->capacity = newCapacity;
    }
        ps->ch[ps->top] = *((char*)data);
    }
    
    ++ps->top;
    return 0;
}



// 获取栈顶元素
int StackGetTop(Stack* ps, void* topElement,int type)
{
if (ps->top == 0) {
    printf("the stack is empty.");
    return Fail; // 栈为空
}
else if(type==lf){
    *(double*)topElement = (ps->number[ps->top - 1]); // 将栈顶元素存储在topElement中
    return Success; 
}
else if(type==word){
    *(char*)topElement = (ps->ch[ps->top - 1]); // 将栈顶元素存储在topElement中
    return Success; 
}

}

// 出栈:弹出并储存栈顶元素
int StackPop(Stack* ps, void* topElement,int type)
{
if (ps == NULL) {
return -1;
}
else if (StackEmpty(ps))
{
printf("Stack is empty.");
return -1;
}
else {
StackGetTop(ps, topElement,type);
--ps->top;
return 0;
}
}


int Stackpd(Stack* ps)
{
if (ps == NULL) {
return -1;
}
if (StackEmpty(ps))
{
printf("Stack is empty.");
return -1;
}
else {
--ps->top;
return 0;
}
}

// 求栈长：获取栈中有效元素个数
size_t StackSize(Stack* ps)
{
return ps->top;
}


// 检测栈是否为空
int StackEmpty(Stack* ps)
{
// assert(ps);
if (ps->top == 0) {
return 1;
}
else {
return 0;
}
}


// 清空栈
int Stackclear(Stack* ps)
{
// assert(ps);
free(ps->number);
free(ps->ch);
ps->number = NULL;
ps->ch = NULL;
ps->top = 0;
ps->capacity = 0;
return 0;
}
// 栈的遍历
// 判断运算符的优先级
// 返回值：该运算符的优先级，数值越大优先级越高，0表示不是运算符
int Precedence(char op) {
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

int isoperator(const char c) {
return (c == '+' || c == '-' || c == '*' || c == '/'); // 判断c是否为四个运算符之一
}

//运算
double Calculate(const char op,const double a,const double b,int* safe) {
switch (op) {
case '+':
return a + b;
case '-':
return (b-a);
case '*':
return a * b;
case '/':
if (a == 0) {
printf("Error:division by zero.");
*safe=1;
return -1;
}
else {
return b / a;
}
default:
return 0;
}
}


//UI
void gotoxy(int x,int y){
   // COORD c;
   // c.X=x;
   // c.Y=y;
   // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

int color(int c){
	//SetConsoleTextAttribute是API设置控制台窗口字体颜色和背景色的函数
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
	return 0;
}


void startUI(){
    printf("Welcome to Hanphone's quadratic operator.\n");
    printf("You can enter 'help' to ask for help.\n");
    printf("You can quit through entering 'quit'.\n");

}



int readhistory(FILE* p,int key){
    char read[1024];
    int i=0;
    if(key==72){
        fscanf(p,"%s",read);
        printf("%s",read);
        return 0;
    }
    else if(key==80){

        return 0;
    }
    else{
        return 0;
    }
}

//清除当前输入
int delete(){
    
}

char getche()
{
char c;
system("stty -icanon");
c=getchar();
system("stty icanon");
return c; 
}

int help()
{
printf("You will enter the default mode if you don't import any other file,\n");
printf("which means that you must input one infix expression every time by yourself.\n");
printf("You will enter the second mode if you import another file\n");
printf("which means that you input infix expression from a file.txt.\n");
printf("The first file should include expression and the second one will be input the result.The second file is not necessary but once it exists,the first one shoule exist.\n");
printf("You can press 'ctrl+u' to clear your input when you enter.Likewise,you can use familiar shortcuts in other bash.\n");
printf("You can press '↑'or'↓' to read historical expression when you enter.\n");
printf("You can enter 'help' to ask for help when you enter your expression.\n");
printf("Remember,the expression must be infix.\n");
printf("You can exit through entering 'quit'.\n");
return 0;
}

int checkinput(char input){
    if(isdigit(input) ||(isoperator(input))|| input=='(' || input==')'){
        return 0;
    }
    else{
        return 1;
    }
}

